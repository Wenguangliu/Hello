// ********************************************************************
//
// FILENAME:  
// 
// 		SharedRaw.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Shared rotor raw readings data definition and test load code for algorithm testing
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#ifdef UNDER_CE
#include <windows.h>
//#include <fcntl.h>
//#include <sys/mman.h>
#endif


#include <string.h>
#include "CommonDefines.h"
#include "RawData.h"
#include "FactoryData.h"
#include "ResultsData.h"
#include "CalcBfds.h"

static RotorRawReadings_t	rotorRawReadings;
RotorRawReadings_t *		rotorRawReadings_g = &rotorRawReadings;


// ****  CODE from Barcode Processing used in calculating rotorAgeInDays from RAD      ****
// ****									 & to calc/check CA special mfg lot correction ****

// ********************************************************************
// FUNCTION:
//
//		JulianDate 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines the Julian date for a given date. 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned
JulianDate( char month, char day, unsigned short year )
{
	unsigned jDate;
	unsigned temp;
	
	temp = 12 * ( year + 4800 ) + month - 3;
	jDate = ( 2 * ( temp - ( temp / 12 ) * 12 ) + 7 + 365 * temp ) / 12;
	jDate += day + temp / 48 - 32045 + temp / 4800 - temp / 1200;
	
	return jDate;
}

// ****  END  from Barcode Processing used in calculating rotorAgeInDays from RAD      ****
// ****									 & to calc/check CA special mfg lot correction ****
	

#ifndef __QNX__
#include <stdio.h>
#include "SharedFactory.h"
#include "SharedBarcode.h"


// X1 extractions from RAD

#include "Algorithms.h"
#include <string.h>

char runTime_m[ 13 ];

// END X1 extractions from RAD


#pragma pack(1)
typedef struct
{
	unsigned char	resultsIndices[ 64 ];
	unsigned char	statusCount;
	unsigned char	temperatureCount;
	unsigned char	passCount;
	int				absorbanceCount;
	unsigned char	rawDataIndices[ 128 ];
} DataDescriptor_t;
	
static DataDescriptor_t		dataDescriptor_m;
	
static void ReadAnalyteResultsX1( FILE * radFile );
static void ReadDataDescriptor( FILE * radFile );
static void ReadInstrumentInformation( FILE * radFile );
static void ReadInstrumentInformationX1( FILE * radFile );
static void ReadIntermediateValues( FILE * radFile );
static void ReadRotorInformationResults( FILE * radFile );
static void ReadRotorInformationResultsX1( FILE * radFile );
static void ReadRotorRawReadingsX1( FILE * radFile );
static void ReadStatus( FILE * fp, RotorRawReadings_t * rawReadings );
static void ReadTemperatures( FILE * fp, RotorRawReadings_t * rawReadings );
static void ReadWavelengths( FILE * fp, RotorRawReadings_t * rawReadings );
static void ReadWavelength( FILE * fp, RawFlashWavelengths_t * w );
static void ReadX1( FILE * fp );
static void SkipAbsorbances( FILE * radFile );
static void SkipRotorAnalyteResults( FILE * radFile );

// ********************************************************************
// FUNCTION:
//
//		LoadTestRaw() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load test  data to structure for algorithm testing 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

void LoadTestRaw( char * fileName )
{
	FILE *			radFile;
	long			version;
	unsigned long	crc;


// DLR
// set all of rotorRawReadings_g to 0xff to simulate real time QNX rotor run 

	memset(rotorRawReadings_g, 0xff, sizeof( RotorRawReadings_t ));

// init ADC bias to typical value for debugging (to be removed if add ADC bias to RAD & load from RAD in following code)

	rotorRawReadings_g->adcBias.adcReferenceOffset = 800;	



	// replace the file name here

// DLR
//	radFile = fopen("rad.52.90001", "rb");

//	radFile = fopen("2.00001.200509161613.C.stripped.rad", "rb");

	radFile = fopen( fileName, "rb" );
	if ( radFile == 0 )
		return;

	// Get the version
	fread( &version, sizeof( long ), 1, radFile );

	// Get what we need in InstrumentInformation.
	ReadInstrumentInformation( radFile );

	// Get what we need in RotorInformationResults.	
	ReadRotorInformationResults( radFile );

	// Get info from the DataDescriptor section.
	ReadDataDescriptor( radFile );
	
	// Skip over RotorAnalyteResults.
	SkipRotorAnalyteResults( radFile );

	// Skip over Absorbances.
	SkipAbsorbances( radFile );
	
	// get the statuses, temps, bead checks, dist checks, optical DACs, black offsets, and pre-sample FRS 
	ReadStatus( radFile, rotorRawReadings_g );
	ReadTemperatures( radFile, rotorRawReadings_g );
	ReadWavelengths( radFile, rotorRawReadings_g );

	if ( version >= 8 )
	{
		ReadX1( radFile );
	}

	fread( &crc, sizeof( long ), 1, radFile );

	fclose(radFile);
}

static void
ReadInstrumentInformation( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		char			factorConfigFileName[ 32 ];
		char			versionConfigFileName[ 32 ];
		char			calibrationConfigFileName[ 32 ];
		char			rocFileName[ 32 ];
		char			risFileName[ 32 ];
		char			productCode;
		char			serialNumber[ SERIAL_NUMBER_LEN ];
		char			softwareVersion[ 16 ];
		float			airTempOffset;
		float			cuvetteTransmitLimit;
		float			bfdCals[ MAX_BFD_CALS ];
		unsigned short	wavelengthFilterCurves[ NUM_WAVELENGTHS-1 ][ MAX_BFD_CALS ];	
		unsigned short	globalCuvetteDelay;
		unsigned short	cuvetteDelays[ NUM_CUVETTES ];
		unsigned short	flashLampThresholds[ NUM_WAVELENGTHS ];
		unsigned char	heaterDacSettings[ NUM_HEATER_DAC_SETTINGS ];
		unsigned char	fanTempSetting;
		unsigned char	barcodeDacs[ NUM_BARCODE_DACS ];
		unsigned char	cuvetteMarkDacs[ NUM_CUVETTE_MARK_DACS ];
		unsigned char	wavelengthDacTrims[ NUM_WAVELENGTHS  ];
		unsigned short	wavelengthAdcOffsets[ NUM_WAVELENGTHS ];
	} xInstrumentInformation_t;
	
	xInstrumentInformation_t	instrumentInformation;
	int							i;
	int							j;

	fread( &instrumentInformation, sizeof( xInstrumentInformation_t ), 1, fp );
	
	factoryData_g->airTemperatureOffset = instrumentInformation.airTempOffset;


	// setup wavelengthFilterCurves from RAD file in factoryData_g for bfdAvgCurveFactors input 

	for ( i = 0; i < NUM_WAVELENGTHS - 1; i++ )
	{
		for ( j = 0; j < MAX_BFD_CALS; j++ )
		{
			factoryData_g->wavelengthFilterCurves [ i ][ j ] = instrumentInformation.wavelengthFilterCurves[ i ][ j ];
		}
	}

	// compute & set bfdAvgCurveFactors in factoryData_g using wavelengthFilterCurves from RAD

	CalcBfds( factoryData_g );



	// set bfdCalibrationFactors direct from RAD file in factoryData_g to write over recomputed (as before)
	 
	for ( i = 0; i < MAX_BFD_CALS; i++ )
	{
		factoryData_g->bfdCalibrationFactors[ i ] = instrumentInformation.bfdCals[ i ];
	}
}


// ****  CODE extracted from Barcode Processing to calculate rotorAgeInDays from RAD  ****


// ********************************************************************
// FUNCTION:
//
//		CalcRotorAgeInDays 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Determines the rotor age in days. 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float
CalcRotorAgeInDays(  char *lotNumber, char *runTime  )
{
	char			currentMonth;
	char			currentDay;
	unsigned short	currentYear;
	int				lsdOfCurrentYear;
	unsigned short	mfgYear;
	unsigned		currentJulian;
	unsigned		mfgJulian;
	float			rotorAgeInDays;
	
	// Get the current date info from runTime string
	currentYear  = ((runTime[0]- '0')*1000) + ((runTime[1]-'0')*100) + ((runTime[2]-'0')*10) + (runTime[3]-'0');
	currentMonth = (runTime[4]- '0')*10 + (runTime[5]- '0');
	currentDay   = (runTime[6]- '0')*10 + (runTime[7]- '0');
		
	// Determine the manufacturing year.
	lsdOfCurrentYear = currentYear % 10;
	mfgYear = currentYear - ( lsdOfCurrentYear - (lotNumber[0]-'0') );
	if ( lsdOfCurrentYear < (lotNumber[0]-'0') )
	{
		mfgYear -= 10;
	}

	// Get the Julian date for the current date.
	currentJulian = JulianDate( currentMonth, currentDay, currentYear );
	
	// Get the Julian date for January 1 of the manufacturing year.
	mfgJulian = JulianDate( 1, 1, mfgYear );
	
	// Add the manufacturing week and day-of-week, and subtract the day of week
	// that the manufacturing year started on.
	// Subtract 1 from weekOfYear so we get 0-based value.
	mfgJulian += (unsigned)( (((lotNumber[1]-'0')*10) +(lotNumber[2]-'0')) - 1 ) * 7 + (unsigned)(lotNumber[3]-'0') - ( ( mfgJulian % 7 ) + 1 );
	
	// Get the rotor age in days.
	rotorAgeInDays = (long)(currentJulian - mfgJulian);
	
	return rotorAgeInDays;
}


// ****  END extracted from Barcode Processing to calculate rotorAgeInDays from RAD  ****


static void
ReadRotorInformationResults( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		char			rotorNumber;
		char			rotorFormat;
		char			productCode;
		char			lotNumber[ 8 ];
		char			expiry[ 9 ];
		char			resultType;
		char			rotorName[ 32 ];
		char			runTime[ 13 ];
		char			sampleId[ 15 ];
		char			operatorId[ 15 ];
		char			alternateSampleId[ 15 ];
		unsigned		systemFlags;
		unsigned short	dacTrimFlags;
		unsigned short	offsetErrorFlags;
		unsigned		beadCheck1Flags;
		unsigned		beadCheck2Flags;
		unsigned		distCheckFlags;
		unsigned short	qcFlags;
		unsigned short	offsetSdErrorFlags;
		unsigned short	wavelengthCvFlags;
		float			hemolyzedIndex;
		float			lipemicIndex;
		float			ictericIndex;
		float			rqcLowLimit;
		float			iqcRatioPrecision;
		float			iqcLevel1Results[ 8 ];
		float			iqcLevel2Results[ 8 ];
		unsigned		referenceRangeType;
		float			barcodeFactors[ MAX_BC_FACTORS ];//8888888888888888
		unsigned short	rotorErrorNumber;
	}xRotorInformationResults_t;
	
	xRotorInformationResults_t	rotorInformationResults;
	int							i;

	fread( &rotorInformationResults, sizeof( xRotorInformationResults_t ), 1, fp );

	for ( i = 0; i < MAX_BC_FACTORS; i++ )
	{
		barcodeData_g->rotorAnalyteFactor[ i ] = rotorInformationResults.barcodeFactors[ i ];
	}

	barcodeData_g->rotorAgeInDays = CalcRotorAgeInDays( rotorInformationResults.lotNumber, rotorInformationResults.runTime );

	barcodeData_g->rotorId = rotorInformationResults.rotorNumber;
	barcodeData_g->rotorFormat = rotorInformationResults.rotorFormat;

	barcodeData_g->rotorMfgLot.lsdOfYear  = rotorInformationResults.lotNumber[0] - '0';
	barcodeData_g->rotorMfgLot.weekOfYear = ( ( rotorInformationResults.lotNumber[1] - '0' ) * 10 ) + ( rotorInformationResults.lotNumber[2] - '0' );
	barcodeData_g->rotorMfgLot.dayOfWeek  = rotorInformationResults.lotNumber[3] - '0';

	strcpy( runTime_m, rotorInformationResults.runTime ); 
}

static void
ReadDataDescriptor( FILE * fp )
{
	fread( &dataDescriptor_m, sizeof( DataDescriptor_t ), 1, fp );
}

static void
SkipRotorAnalyteResults( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		unsigned char	analyteType;
		char			versionNumber[6];
		float			analyteResult;
		unsigned		analyteFlags;
		float			lowReferenceRangeLimit;
		float			highReferenceRangeLimit;
		float 			lowDynamicRangeLimit;
		float 			highDynamicRangeLimit;
		float 			lowSystemRangeLimit;
		float 			highSystemRangeLimit;
	} xAnalyteResults_t;
	
	xAnalyteResults_t	analyteResult;
	int					i;	

	for ( i = 0; i < 64; i++ )
	{
		if ( dataDescriptor_m.resultsIndices[ i ] != 0xFF )
		{
			fread( &analyteResult, sizeof( xAnalyteResults_t ), 1, fp );
		}
		else
		{
			break;
		}
	}
}

static void
SkipAbsorbances( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		float			engineMinutesTime;
		unsigned char	sampleType;
		unsigned char	sampleTypeOpticalBlank;
		unsigned char	samplePass;
		unsigned char	wavelengthPair;
		float			desiredPathLength;
		unsigned char	cuvetteNumber;
		unsigned long	analyteError;
		float			sampleAbsorbance;
	} xAbsorbance_t;
	
	xAbsorbance_t	absorbance;
	int				i;
	
	for ( i = 0; i < dataDescriptor_m.absorbanceCount; i++ )
	{
		fread( &absorbance, sizeof( xAbsorbance_t ), 1, fp );
	}
}

static void
ReadStatus( FILE * fp, RotorRawReadings_t * rawReadings )
{
	int i;

	for ( i = 0; i < dataDescriptor_m.statusCount; i++ )
	{
		fread( &rawReadings->analysisStatus[i].engineTime, sizeof( unsigned long ), 1, fp );
		fread( &rawReadings->analysisStatus[i].analysisStatus, sizeof( unsigned char ), 1, fp );
		fread( &rawReadings->analysisStatus[i].analysisParameter, sizeof( unsigned char ), 1, fp );
	}
}


static void
ReadTemperatures( FILE * fp, RotorRawReadings_t * rawReadings )
{
	int i;
	
	for ( i = 0; i < dataDescriptor_m.temperatureCount; i++ )
	{
		fread( &rawReadings->temperatures[i].engineTime, sizeof( unsigned long ), 1, fp );
		fread( &rawReadings->temperatures[i].rotorTemperature, sizeof( unsigned short ), 1, fp );
		fread( &rawReadings->temperatures[i].ambientTemperature, sizeof( unsigned short ), 1, fp );
		fread( &rawReadings->temperatures[i].topPlateTemperature, sizeof( unsigned short ), 1, fp );
		fread( &rawReadings->temperatures[i].bottomPlateTemperature, sizeof( unsigned short ), 1, fp );
		fread( &rawReadings->temperatures[i].topPlateCurrent, sizeof( unsigned short ), 1, fp );
		fread( &rawReadings->temperatures[i].bottomPlateCurrent, sizeof( unsigned short ), 1, fp );
	}
}

static void
ReadWavelengths( FILE * fp, RotorRawReadings_t * rawReadings )
{
	int i, j, k;

	// Read beadCheck1
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		for ( j = 0; j < NUM_BEAD_CHECK_FLASHES + 1; j++ )
		{
			ReadWavelength( fp, &rawReadings->beadCheck1[i][j] );
		}
	}
	
	// Read beadCheck2
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		for ( j = 0; j < NUM_BEAD_CHECK_FLASHES + 1; j++ )
		{
			ReadWavelength( fp, &rawReadings->beadCheck2[i][j] );
		}
	}
	
	// Read distributionCheck
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		for ( j = 0; j < NUM_DIST_CHECK_FLASHES + 1; j++ )
		{
			ReadWavelength( fp, &rawReadings->distributionCheck[i][j] );
		}
	}

	// Read opticalDacs
	for ( i = 0; i < NUM_OPTICAL_DACS_FLASHES + 1; i++ )
	{
		ReadWavelength( fp, &rawReadings->opticalDacs[i] );
	}
	
	// Read blackOffsets
	for ( i = 0; i < NUM_BLACK_OFFSETS_FLASHES + 1; i++ )
	{
		ReadWavelength( fp, &rawReadings->blackOffsets[i] );
	}
	
	// Read preSampleFullRotorScan
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		for ( j = 0; j < NUM_FRS_SAMPLE_FLASHES + 1; j++ )
		{
			ReadWavelength( fp, &rawReadings->preSampleFullRotorScan[i][j] );
		}	
	}
	
	// Read rotorAnalyteSamples
	for ( i = 0; i < 128; i++ )
	{
		if ( dataDescriptor_m.rawDataIndices[i] == 0xFF )
		{
			break;
		}
		for ( j = 0; j < dataDescriptor_m.passCount; j++ )
		{
			for ( k = 0; k < NUM_SAMPLE_FLASHES + 1; k++ )
			{
				ReadWavelength( fp, &rawReadings->rotorAnalyteSamples[dataDescriptor_m.rawDataIndices[i]][j][k] );

			}
		}
	}
	
	// Read postSampleFullRotorScan
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		for ( j = 0; j < NUM_FRS_SAMPLE_FLASHES + 1; j++ )
		{
			ReadWavelength( fp, &rawReadings->postSampleFullRotorScan[i][j] );
		}	
	}
}

static void
ReadWavelength( FILE * fp, RawFlashWavelengths_t * w )
{
	int i;
	
	fread( &w->engineTime, sizeof( unsigned long ), 1, fp );
	for ( i = 0; i < NUM_WAVELENGTHS; i++ )
	{
		fread( &w->rawFlashReadings[i], sizeof( unsigned short ), 1, fp );
	}
	fread( &w->sampleType, sizeof( unsigned char ), 1, fp );
	fread( &w->cuvetteNumber, sizeof( unsigned char ), 1, fp );
	fread( &w->flashNumber, sizeof( unsigned char ), 1, fp );
}

static void
ReadX1( FILE * fp )
{
	ReadInstrumentInformationX1( fp );
	
	ReadRotorInformationResultsX1( fp );
	
	ReadAnalyteResultsX1( fp );
	
	ReadRotorRawReadingsX1( fp );
	
	ReadIntermediateValues( fp );
}

static void
ReadInstrumentInformationX1( FILE * fp )
{
	char	softwareReleaseType;
	char	instrumentMode;
	
	fread( &softwareReleaseType, sizeof( char ), 1, fp );
	fread( &instrumentMode, sizeof( char ), 1, fp );
}
	

// ****  CODE extracted from RAD file generation code  ****

// ********************************************************************
// FUNCTION:
//
//		CalculateAge 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculates the patient's age based on either the age entered or the DOB entered. If the latter, then 
//		the age is based on today's date.
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		Age in years, rounded down.
// 
// ********************************************************************/

int
CalculateAge( char *runTime, short ageEntered, char ageUnits, Date_t dateOfBirth )
{
	unsigned char	currentMonth;
	unsigned char	currentDay;
	unsigned short	currentYear;

    int            	age;

	age = NO_AGE_ENTERED;

	// Get the current date info from runTime string
	currentYear  = ((runTime[0]- '0')*1000) + ((runTime[1]-'0')*100) + ((runTime[2]-'0')*10) + (runTime[3]-'0');
	currentMonth = (runTime[4]- '0')*10 + (runTime[5]- '0');
	currentDay   = (runTime[6]- '0')*10 + (runTime[7]- '0');
		
    // If an age was entered, we use it. The maximum value that can be entered is 99,
    // so we only care if the units are years, months or weeks.
    if ( ageEntered > 0 )
    {
        switch ( ageUnits )
        {
            case AGE_UNITS_YEARS:    age = ageEntered;         break;
            case AGE_UNITS_MONTHS:   age = ageEntered / 12;    break;
            case AGE_UNITS_WEEKS:    age = ageEntered / 52;    break;
            default:                 age = 0;                  break;
        }
    }

    // Otherwise, if a DOB was entered, we use it.
    else if ( dateOfBirth.year > 0 )
    {
        // Set the age equal to the difference between current year and birth year.
        age = currentYear - dateOfBirth.year;

        // If it's currently an earlier month, or an earlier day in the same month, as the birth date, then decrement the age.
        if ( (currentMonth < (dateOfBirth.month + 1)) ||
             ((currentMonth == (dateOfBirth.month + 1)) && (currentDay < dateOfBirth.day)) )
        {
            age--;
        }
    }

    return age;    
}


// ****  END extracted from RAD file generation code  ****


static void
ReadRotorInformationResultsX1( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		unsigned short	rotorErrorNumber[20];
		unsigned		rotorErrorNumberIndex;
		unsigned		rotorBeadMissingCount;
		unsigned		rotorDistributionCheckCount;
		unsigned short	rotorFlashLampSaturationMax;
		unsigned short	rotorFlashLampIntensityMin;
		float			rotorFlashRangeMax;
		float			rotorOffsetSdMax;
		float			rotorOpticalDacCvMax;
		float			rotorAverageTemperature;
		unsigned		rotorRqcAbused;

		// dilutionCheckResults
			unsigned		rotorDilutionCheckError;
			unsigned		rotorDilutionCheckNumberSuppressed;
			unsigned		rotorDilutionCheckNumberChecked;
			float			rotorShortSampleProbability;
			float			rotorLongSampleProbability;
		
		//sampleBlankCalcs
			float			sampleBlankHemolyzed405;
			float			sampleBlankLipemic340;
			float			sampleBlankIcteric467;
		
		unsigned		patientType;
		
		// barcodeData
			unsigned char	rotorId;
			unsigned char	rotorFormat;
			unsigned char	rotorExpiryMonth;
			unsigned char	rotorExpiryDayOfMonth;
			unsigned char	rotorExpiryYear;
			float			rotorAgeInDays;
			
			// rotorMfgLot
				unsigned char	lsdOfYear;
				unsigned char	weekOfYear;
				unsigned char	dayOfWeek;
				char			code[4];
				
		char			barcodeDigits[110];
		unsigned short	barcodeTransitions[600];
		char			species[15];
		char			doctorId[ ID_LEN ];
		char			location[ ID_LEN ];
		char			phoneNumber[ ID_LEN ];
		char			admissionOwnerId[ ID_LEN ];
		Date_t			dateOfBirth;
		Date_t			lastVaccination;
		char			sampleId[ ID_LEN ];
		char			gender;
		short			age;
		char			ageUnits;
		char			race;
		float			rotorCuvetteEmptyLimits[ NUM_CUVETTES ];
		float			rotorCuvetteBeadLimits[ NUM_CUVETTES ];
		float			rotorCuvetteDistributionLimits[ NUM_CUVETTES ];
	} xRotorInformationResultsX1_t;

	xRotorInformationResultsX1_t	rotorInformationResultsX1;
	
	fread( &rotorInformationResultsX1, sizeof( xRotorInformationResultsX1_t ), 1, fp );

	commonAlgGlobals_g->patientType = rotorInformationResultsX1.patientType;

	commonAlgGlobals_g->age    = CalculateAge( runTime_m, rotorInformationResultsX1.age, rotorInformationResultsX1.ageUnits, rotorInformationResultsX1.dateOfBirth );
	commonAlgGlobals_g->gender = rotorInformationResultsX1.gender;
	commonAlgGlobals_g->race   = rotorInformationResultsX1.race;
}
	
static void
ReadAnalyteResultsX1( FILE * fp )
{
#pragma pack(1)
	typedef struct
	{
		unsigned char	printOrder;
		unsigned		analyteSuppressionMask;
		unsigned		resultPrintFlags;
	} xRotorAnalyteResultsX1_t;
	
	xRotorAnalyteResultsX1_t	rotorAnalyteResultsX1[ MAX_ANALYTE_TYPES ];
	int						i;
	
	for ( i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( dataDescriptor_m.resultsIndices[ i ] != 0xFF )
		{
			fread( &rotorAnalyteResultsX1[ i ], sizeof( xRotorAnalyteResultsX1_t ), 1, fp );
		}
		else
		{
			break;
		}
	}
}
	
static void
ReadRotorRawReadingsX1( FILE * fp )
{
	unsigned		engineTime;
	unsigned short	adcReferenceOffset;
	
	fread( &engineTime, sizeof( int ), 1, fp );
	fread( &adcReferenceOffset, sizeof( short ), 1, fp );

	rotorRawReadings_g->adcBias.adcReferenceOffset = adcReferenceOffset;
}
	
static void
ReadIntermediateValues( FILE * fp )
{
	float	intermediateValues[MAX_ANALYTE_TYPES][MAX_INTERMEDIATE_VALUES];
	int		i;
	
	for ( i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( dataDescriptor_m.resultsIndices[ i ] != 0xFF )
		{
			fread( &intermediateValues[ i ], sizeof( float ), MAX_INTERMEDIATE_VALUES, fp );
		}
		else
		{
			break;
		}
	}
}

#endif
