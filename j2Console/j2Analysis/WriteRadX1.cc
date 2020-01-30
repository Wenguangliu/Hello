#include <stdio.h>
#include <string.h>

#include "WriteRad.h"
#include "WriteRadX1.h"
#include "LogErr.h"
#include "Species.h"
#include "StateAnalysis.h"


static void		WriteInstrumentInformationX1( FILE * fp, SystemData_t * systemData, VersionData_t * versionData );
static void		WriteIntermediateValuesX1( FILE * fp, RotorResults_t * results );
static void		WriteRotorAnalyteResultsX1( FILE * fp, RotorResults_t * results );
static void		WriteRotorInformationResultsX1( FILE *				fp,
												BarcodeData_t *		barcodeData,
												RotorResults_t *	results,
												RunInfo_t *			runInfo );
static void		WriteRotorRawReadingsX1( FILE * fp, RawData_t * rawData );

void
WriteRadX1( FILE *				fp,
			RunInfo_t *			runInfo,
			SystemData_t *		systemData,
			VersionData_t *		versionData,
			BarcodeData_t *		barcodeData,
			RotorResults_t *	rotorResults,
		  	RawData_t *			rawData )
{
	WriteInstrumentInformationX1( fp, systemData, versionData );	// GLD - done
	
	WriteRotorInformationResultsX1( fp, barcodeData, rotorResults, runInfo );
	
	WriteRotorAnalyteResultsX1( fp, rotorResults );
	
	WriteRotorRawReadingsX1( fp, rawData );

	WriteIntermediateValuesX1( fp, rotorResults );
}

void
WriteInstrumentInformationX1( FILE * fp, SystemData_t * systemData, VersionData_t * versionData )
{
	char	mode;
	
	if ( systemData->inResearchMode )
		mode = 'R';
	else if ( systemData->inDemoMode )
		mode = 'D';
	else if ( systemData->inServiceMode )
		mode = 'S';
	else if ( systemData->inManufacturingMode )
		mode = 'M';
	else
		mode = 'C';
	
	CrcWrite( &versionData->releaseType, sizeof( char ), 1, fp );
	CrcWrite( &mode, sizeof( char ), 1, fp );
}



void
WriteIntermediateValuesX1( FILE * fp, RotorResults_t * results )
{
	int i, j;

	for ( i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( results->rotorAnalyteResult[i].analyteType != 0xFF )
		{
			for ( j = 0; j < MAX_INTERMEDIATE_VALUES; j++ )
			{
				CrcWrite( &results->rotorIntermediateValues[i][j], sizeof( float ), 1, fp );
			}
		}
	}
}


void
WriteRotorAnalyteResultsX1( FILE * fp, RotorResults_t * results )
{
	for ( int i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( results->rotorAnalyteResult[i].analyteType != 0xFF )
		{
			CrcWrite( &results->rotorAnalyteResult[i].printOrder, sizeof( char ), 1, fp );
			CrcWrite( &results->rotorAnalyteResult[i].analyteSuppressionMask, sizeof( int ), 1, fp );
			CrcWrite( &results->rotorAnalyteResult[i].resultPrintFlags, sizeof( int ), 1, fp );
		}
	}
}

void
WriteRotorInformationResultsX1( FILE *				fp,
								BarcodeData_t *		barcodeData,
								RotorResults_t *	results,
								RunInfo_t *			runInfo )
{
	int							i;
	FILE *						bcFp;
	char						barcodeDigits[ NUM_BARCODE_DIGITS ];
	unsigned short				barcodeTransitions[ NUM_BARCODE_TRANSITIONS ];
	unsigned					transition;
	static Species				species;
	char						gender;
	unsigned char				race;
	unsigned char				ageUnits;
	RotorInformationResults_t *	rotorInfo = &results->rotorInformationResults;
    char                        buff [512];

	for ( i = 0; i < MAX_ROTOR_ERROR_NUMBERS; i++ )
		CrcWrite( &rotorInfo->rotorErrorNumber[ i ], sizeof( short ), 1, fp );
		
	CrcWrite( &rotorInfo->rotorErrorNumberIndex, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->rotorBeadMissingCheckCount, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->rotorDistributionCheckCount, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->rotorFlashLampSaturationMax, sizeof( short ), 1, fp );
	CrcWrite( &rotorInfo->rotorFlashLampIntensityMin, sizeof( short ), 1, fp );
	CrcWrite( &rotorInfo->rotorFlashRangeMax, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->rotorOffsetSdMax, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->rotorOpticalDacCvMax, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->rotorAverageTemperature, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->rotorRqcAbused, sizeof( int ), 1, fp );

	// dilutionCheckResults
	CrcWrite( &rotorInfo->dilutionCheckResults.rotorDilutionCheckError, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->dilutionCheckResults.rotorDilutionCheckNumberSuppressed, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->dilutionCheckResults.rotorDilutionCheckNumberChecked, sizeof( int ), 1, fp );
	CrcWrite( &rotorInfo->dilutionCheckResults.rotorShortSampleProbability, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->dilutionCheckResults.rotorLongSampleProbability, sizeof( float ), 1, fp );
	
	// sampleBlankCalcs
	CrcWrite( &rotorInfo->sampleBlankCalcs.sampleBlankHemolyzed405, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->sampleBlankCalcs.sampleBlankLipemic340, sizeof( float ), 1, fp );
	CrcWrite( &rotorInfo->sampleBlankCalcs.sampleBlankIcteric467, sizeof( float ), 1, fp );
	
	CrcWrite( &runInfo->patientType, sizeof( int ), 1, fp );
	
	// barcodeData
	CrcWrite( &barcodeData->rotorId, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorFormat, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorExpiryMonth, sizeof( char ), 1, fp );	
	CrcWrite( &barcodeData->rotorExpiryDayOfMonth, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorExpiryYear, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorAgeInDays, sizeof( float ), 1, fp );
	
	// rotorMfgLot
	CrcWrite( &barcodeData->rotorMfgLot.lsdOfYear, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorMfgLot.weekOfYear, sizeof( char ), 1, fp );
	CrcWrite( &barcodeData->rotorMfgLot.dayOfWeek, sizeof( char ), 1, fp );
	WriteString( barcodeData->rotorMfgLot.code, 4, fp );

	// barcodeDigits
	memset( barcodeDigits, 0, NUM_BARCODE_DIGITS );
	bcFp = fopen( BC_DIGITS_FILE, "rb" );
	if ( bcFp == 0 )
	{
		LogDebug( "unable to open barcode digits file" );
	}
	else
	{
		for ( i = 0; i < NUM_BARCODE_DIGITS; i++ )
			barcodeDigits[i] = fgetc( bcFp );
		fclose( bcFp );
	}
	for ( i = 0; i < NUM_BARCODE_DIGITS; i++ )
		CrcWrite( &barcodeDigits[i], sizeof( char ), 1, fp );

	// barcodeTransitions
	bcFp = fopen( BC_TRANSITIONS_FILE, "w+b" );
	if ( bcFp == 0 )
	{
		LogDebug( "unable to open barcode transitions file" );
	}
	else
	{
		for ( i = 0; i < NUM_BARCODE_TRANSITIONS; i++ )
		{
			fscanf( bcFp, "%x", &transition );
			barcodeTransitions[i] = transition;
		}
		fclose( bcFp );
	}
	for ( i = 0; i < NUM_BARCODE_TRANSITIONS; i++ )
		CrcWrite( &barcodeDigits[i], sizeof( short ), 1, fp );
	
	// sampleType, convert to char string
    wcstombs (buff, species.English( (SampleType_t)runInfo->sampleType, &strings), 512);
	WriteString( buff, 15, fp );
	
	// doctorId, location, phoneNumber and admissionOwnerId
	WriteString( runInfo->doctorId, ID_LEN, fp );
	WriteString( runInfo->location, ID_LEN, fp );
	WriteString( runInfo->phoneNumber, ID_LEN, fp );
	WriteString( runInfo->admissionOwnerId, ID_LEN, fp );

	// dateOfBirth
	CrcWrite( &runInfo->dateOfBirth.year, sizeof( short ), 1, fp );			// 0 - 9999
	CrcWrite( &runInfo->dateOfBirth.month, sizeof( char ), 1, fp );			// 0 - 11 qnx 1-12 wince
	CrcWrite( &runInfo->dateOfBirth.day, sizeof( char ), 1, fp );			// 1 - 31
	
	// lastVaccination
	CrcWrite( &runInfo->lastVaccination.year, sizeof( short ), 1, fp );		// 0 - 9999
	CrcWrite( &runInfo->lastVaccination.month, sizeof( char ), 1, fp );		// 0 - 11 qnx 1-12 wince
	CrcWrite( &runInfo->lastVaccination.day, sizeof( char ), 1, fp );		// 1 - 31

	// sampleId
	WriteString( runInfo->sampleId, ID_LEN, fp );

	// gender
	gender = (unsigned char)runInfo->gender;
	CrcWrite( &gender, sizeof( char ), 1, fp );

	// age and ageUnits
	CrcWrite( &runInfo->age, sizeof( short ), 1, fp );
	ageUnits = (char)runInfo->ageUnits;
	CrcWrite( &ageUnits, sizeof( char ), 1, fp );
		
	// race
	race = (unsigned char)runInfo->race;
	CrcWrite( &race, sizeof( char ), 1, fp );

	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		CrcWrite( &results->rotorCuvetteEmptyLimits[i], sizeof( float ), 1, fp );
	}
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		CrcWrite( &results->rotorCuvetteBeadLimits[i], sizeof( float ), 1, fp );
	}
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		CrcWrite( &results->rotorCuvetteDistributionLimits[i], sizeof( float ), 1, fp );
	}
}

void
WriteRotorRawReadingsX1( FILE * fp, RawData_t * rawData )
{
	CrcWrite( &rawData->rotorRawReadings.adcBias.engineTime, sizeof( int ), 1, fp );
	CrcWrite( &rawData->rotorRawReadings.adcBias.adcReferenceOffset, sizeof( short ), 1, fp );
}

