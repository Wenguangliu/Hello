/*
 * FILENAME
 * 	WriteRad.c
 * 
 * MODULE DESCRIPTION
 * 	Writes the RAD file.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "WriteRad.h"
#include "WriteRadX1.h"
#include "ResultsData.h"
#include "RawData.h"
#include "Crc32.h"
#include "CommonDefines.h"
#include "LogErr.h"
#include "Shmem.h"
#include "VersionData.h"

static char *	GetAstmDateTime( FILETIME time );
static void		UpdateRadDb( char * fileName );
static void		WriteAbsorbances( FILE * fp, RotorResults_t *	rotorResults );
static void		WriteDataDescriptor( FILE *						fp,
									 ReceivedDataCounter_t *	counter,
									 RotorResults_t *			rotorResults );
static void		WriteInstrumentInformation( FILE *				fp,
											RunInfo_t *			runInfo,
											RotorResults_t *	rotorResults,
											FactoryData_t *		factoryData,
											VersionData_t *		versionData,
											CalibrationData_t *	calibrationData );
static void		WriteRotorAnalyteResults( FILE * fp, RotorAnalyteResult_t results[] );
static void		WriteRotorInformationResults( FILE *			fp,
											  RotorResults_t *	rotorResults,
											  BarcodeData_t *	barcodeData,
											  RunInfo_t *		runInfo );
static void		WriteRotorRawReadings( FILE * fp, RawData_t * rawData );
static void		WriteStatus( FILE * fp, RawData_t * rawData );
static void		WriteTemperatures( FILE * fp, RawData_t * rawData );
static void		WriteWavelengths( FILE * fp, RawData_t * rawData );
static void		WriteWavelength( FILE * fp, RawFlashWavelengths_t * w );

unsigned long	crc;


/*
 * FUNCTION
 * 	WriteRadFile
 * 
 * DESCRIPTION
 * 	Creates and writes a RAD file.
 * 
 * PARAMETERS
 * 	runInfo - Runtime info.
 *  factoryData - Data from the factory config file.
 * 	systemData - Data from the system config file.
 * 	versionData - Data from the software version config file.
 *  calibrationData - Data from the calibration config file.
 * 	barcodeData - Data from the rotor barcode.
 *  rotorResults - Results generated by CalcResults
 * 	rawData - Raw data received from the Engine.
 * 
 * RETURNS
 * 	Name of the RAD file, or an empty string if there is a problem writing the
 * 	RAD file.
 */

const char *
WriteRadFile( RunInfo_t *			runInfo,
			  FactoryData_t *		factoryData,
			  SystemData_t *		systemData,
			  VersionData_t *		versionData,
			  CalibrationData_t *	calibrationData,
			  BarcodeData_t *		barcodeData,
			  RotorResults_t *		rotorResults,
			  RawData_t *			rawData )
{
	static char		radFileName[ MAX_RAD_FILENAME_LEN ];
	unsigned		version = RAD_FILE_VERSION_NUMBER;
	FILE *			fp;
	char			productCode;
	const char *	serialNumber;
	char			patientControl;
	DWORD			error;
	
	// Init the CRC.
	crc = Crc32Init();
	
	// Take care of missing data used for the RAD file name.
	productCode = factoryData->abaxisProductCode == 0 ? '_' : factoryData->abaxisProductCode;
	serialNumber = factoryData->instrumentSerialNumber[0] == 0 ? "_" : factoryData->instrumentSerialNumber;
	patientControl = runInfo->patientControl == 0 ? '_' : runInfo->patientControl;
	
	// Create the file name.
	sprintf( radFileName, "%s/%c.%s.%s.%c.rad",
			RAD_FILE_DIRECTORY,
			productCode,
			serialNumber,
			GetAstmDateTime( runInfo->runTime ),
			patientControl );
		
	// Add this RAD file to the RAD database, deleting the oldest one if necessary.
	UpdateRadDb( radFileName );

	fp = fopen( radFileName, "wb" );
	error = GetLastError();
	if ( fp == NULL )
	{
		LogError( "error opening RAD file <%s> (%d)", radFileName, error );
		return "";
	}

	CrcWrite( &version, sizeof( int ), 1, fp );
	
	WriteInstrumentInformation( fp, runInfo, rotorResults, factoryData, versionData, calibrationData );
	
	WriteRotorInformationResults( fp, rotorResults, barcodeData, runInfo );

	WriteDataDescriptor( fp, &rawData->receivedDataCounter, rotorResults );
	
	WriteRotorAnalyteResults( fp, rotorResults->rotorAnalyteResult );

	WriteAbsorbances( fp, rotorResults );
	
	WriteRotorRawReadings( fp, rawData );
	
	WriteRadX1( fp, runInfo, systemData, versionData, barcodeData, rotorResults, rawData );
	
	crc = Crc32Finalize( crc );
	fwrite( &crc, sizeof( int ), 1, fp );

	fclose( fp );
	
	return radFileName;
}

static void
WriteInstrumentInformation( FILE * fp,
							RunInfo_t * runInfo,
							RotorResults_t * rotorResults,
							FactoryData_t * factoryData,
							VersionData_t * versionData,
							CalibrationData_t * calibrationData )
{
	int						i, j;

	WriteString( FACTORY_CONFIG_FILE_BASENAME, 32, fp );
	WriteString( VERSION_CONFIG_FILE_BASENAME, 32, fp );
	WriteString( CALIBRATION_CONFIG_FILE_BASENAME, 32, fp );
	WriteString( runInfo->rocFileName, 32, fp );
	WriteString( runInfo->risFileName, 32, fp );
	
	CrcWrite( &factoryData->abaxisProductCode, sizeof( char ), 1, fp );

	WriteString( factoryData->instrumentSerialNumber, SERIAL_NUMBER_LEN, fp );

	WriteString( versionData->version, 16, fp );

	CrcWrite( &factoryData->airTemperatureOffset, sizeof( float ), 1, fp );
	CrcWrite( &factoryData->cuvetteTransmitLimit, sizeof( float ), 1, fp );

	for ( i = 0; i < MAX_BFD_CALS; i++ )
	{
		CrcWrite( &factoryData->bfdCalibrationFactors[i], sizeof( float ), 1, fp );
	}
	
	for ( i = 0; i < NUM_WAVELENGTHS - 1; i++ )
	{
		for ( j = 0; j < MAX_BFD_CALS; j++ )
		{
			CrcWrite( &factoryData->wavelengthFilterCurves[i][j], sizeof( short ), 1, fp );
		}
	}
	
	CrcWrite( &factoryData->globalCuvetteDelay, sizeof( short ), 1, fp );
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		CrcWrite( &factoryData->cuvetteDelays[i], sizeof( short ), 1, fp );
	}

	for ( i = 0; i < NUM_WAVELENGTHS; i++ )
	{
		CrcWrite( &factoryData->flashLampThresholds[i], sizeof( short ), 1, fp );
	}
	
	for ( i = 0; i < NUM_HEATER_DAC_SETTINGS; i++ )
	{
		CrcWrite( &factoryData->heaterDacSettings[i], sizeof( char ), 1, fp );
	}

	CrcWrite( &factoryData->fanTemperatureSetting, sizeof( char ), 1, fp );
	
	for ( i = 0; i < NUM_BARCODE_DACS; i++ )
	{
		CrcWrite( &calibrationData->barcodeDacs[i], sizeof( char ), 1, fp );
	}
	
	for ( i = 0; i < NUM_CUVETTE_MARK_DACS; i++ )
	{		
		CrcWrite( &calibrationData->cuvetteMarkDacs[i], sizeof( char ), 1, fp );
	}
	
	for ( i = 0; i < NUM_WAVELENGTHS; i++ )
	{		
		CrcWrite( &calibrationData->wavelengthDacTrims[i], sizeof( char ), 1, fp );
	}
	
	for ( i = 0; i < NUM_WAVELENGTHS; i++ )
	{		
		CrcWrite( &rotorResults->rotorInformationResults.wavelengthAdcOffsets[i], sizeof( short ), 1, fp );
	}	
}

static void
WriteRotorInformationResults( FILE * fp,
							  RotorResults_t * rotorResults,
							  BarcodeData_t * barcodeData,
							  RunInfo_t * runInfo )
{
	int		i;
	char	buff[ 20 ];
	short	sTemp;
	
	CrcWrite( &rotorResults->rotorNumber, sizeof( char ), 1, fp );
	CrcWrite( &rotorResults->rotorFormat, sizeof( char ), 1, fp );
	CrcWrite( &rotorResults->rotorProductCode, sizeof( char ), 1, fp );

	// Rotor lot number.
	sprintf( buff, "%d%02d%d%3.3s", barcodeData->rotorMfgLot.lsdOfYear,
								 barcodeData->rotorMfgLot.weekOfYear,
								 barcodeData->rotorMfgLot.dayOfWeek,
								 barcodeData->rotorMfgLot.code );
	WriteString( buff, 8, fp );

	// Rotor expiry date.
	sprintf( buff, "20%02d%02d%02d", barcodeData->rotorExpiryYear,
									 barcodeData->rotorExpiryMonth,
									 barcodeData->rotorExpiryDayOfMonth );
	WriteString( buff, 9, fp );
	
	CrcWrite( &runInfo->patientControl, sizeof( char ), 1, fp );
	WriteString( runInfo->rotorName, 32, fp );
	WriteString( GetAstmDateTime( runInfo->runTime ), 13, fp );
	
	WriteString( runInfo->patientControlId, 15, fp );
	WriteString( runInfo->operatorId, 15, fp );
	WriteString( runInfo->alternateId, 15, fp );

	CrcWrite( &rotorResults->rotorInformationResults.rotorSystemFlags, sizeof( int ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorDacTrimFlags, sizeof( short ), 1, fp ); 
	CrcWrite( &rotorResults->rotorInformationResults.rotorOffsetErrorFlags, sizeof( short ), 1, fp );	
	CrcWrite( &rotorResults->rotorInformationResults.rotorBeadCheck1Flags, sizeof( int ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorBeadCheck2Flags, sizeof( int ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorDistributionCheckFlags, sizeof( int ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorQcFlags, sizeof( short ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorOffsetSdErrorFlags, sizeof( short ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorWavelengthCvFlags, sizeof( short ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorHemolyzedIndex, sizeof( float ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorLipemicIndex, sizeof( float ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorIctericIndex, sizeof( float ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorRqcLowLimit, sizeof( float ), 1, fp );
	CrcWrite( &rotorResults->rotorInformationResults.rotorIqcRatioPrecision, sizeof( float ), 1, fp );

	for ( i = 0; i < MAX_IQC_LEVEL1_CHECKS; i++ )
	{
		CrcWrite( &rotorResults->rotorInformationResults.rotorIqcLevel1Results[i], sizeof( float ), 1, fp );
	}

	for ( i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++ )
	{
		CrcWrite( &rotorResults->rotorInformationResults.rotorIqcLevel2Results[i], sizeof( float ), 1, fp );
	}

	CrcWrite( &rotorResults->rotorInformationResults.referenceRangeType, sizeof( int ), 1, fp );
	
	for ( i = 0; i < MAX_BC_FACTORS; i++ )
	{
		CrcWrite( &barcodeData->rotorAnalyteFactor[i], sizeof( float ), 1, fp );	// Rotor barcode factors
	}

	sTemp = (short)runInfo->errNum;
	CrcWrite( &sTemp, sizeof( short ), 1, fp );
}

static char *
GetAstmDateTime( FILETIME time )
{
	static char	buff[ 13 ];
	//struct tm *	t = gmtime( &time );
	SYSTEMTIME sysTime;
	FileTimeToSystemTime( &time, &sysTime );
	
	sprintf( buff, "%4.4d%2.2d%2.2d%2.2d%2.2d", sysTime.wYear,
												sysTime.wMonth,
												sysTime.wDay,
												sysTime.wHour,
												sysTime.wMinute );

	return buff;
}

void
WriteString( const char * string, int width, FILE * fp )
{
	int			i;
	int			stringLen = strlen( string );
	const int	nullChar = 0;
	
	if ( stringLen >= width )
	{
		stringLen = width - 1;
	}
	
	CrcWrite( string, sizeof( char ), stringLen, fp );
	for ( i = stringLen; i < width; i++ )
	{
		fputc( nullChar,  fp );
		crc = Crc32Update( crc, (const unsigned char *)&nullChar, sizeof( char ) );
	}
}

static void
WriteRotorAnalyteResults( FILE * fp, RotorAnalyteResult_t results[] )
{
//	for ( int i = 0; i < MAX_ROTOR_RESULTS; i++ )
    for ( int i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( results[i].analyteType != 0xFF )
		{
			CrcWrite( &results[i].analyteType, sizeof( char ), 1, fp );
			CrcWrite( &results[i].versionNumber, sizeof( char ), 6, fp );
			CrcWrite( &results[i].analyteResult, sizeof( float ), 1, fp );
			CrcWrite( &results[i].analyteFlags, sizeof( int ), 1, fp );
			CrcWrite( &results[i].lowReferenceRangeLimit, sizeof( float ), 1, fp );
			CrcWrite( &results[i].highReferenceRangeLimit, sizeof( float ), 1, fp );
			CrcWrite( &results[i].lowDynamicRangeLimit, sizeof( float ), 1, fp );
			CrcWrite( &results[i].highDynamicRangeLimit, sizeof( float ), 1, fp );
			CrcWrite( &results[i].lowSystemRangeLimit, sizeof( float ), 1, fp );
			CrcWrite( &results[i].highSystemRangeLimit, sizeof( float ), 1, fp );
		}
	}
}

static void
WriteAbsorbances( FILE * fp, RotorResults_t * rotorResults )
{
	unsigned			i;
	SampleAbsRecord_t *	aRecord;	
	
	for ( i = 0; i < rotorResults->rotorAbsorbances.numberAbsRecords; i++ )
	{
		aRecord = &(rotorResults->rotorAbsorbances.sampleAbsRecords[i]);
		
		CrcWrite( &aRecord->engineMinutesTime, sizeof( float ), 1, fp );
		CrcWrite( &aRecord->sampleType, sizeof( char ), 1, fp );
		CrcWrite( &aRecord->sampleTypeOpticalBlank, sizeof( char ), 1, fp );
		CrcWrite( &aRecord->samplePass, sizeof( char ), 1, fp );
		CrcWrite( &aRecord->wavelengthPair, sizeof( char ), 1, fp );
		CrcWrite( &aRecord->desiredPathLength, sizeof( float ), 1, fp );
		CrcWrite( &aRecord->cuvetteNumber, sizeof( char ), 1, fp );
		CrcWrite( &aRecord->analyteError, sizeof( long ), 1, fp );
		CrcWrite( &aRecord->sampleAbsorbance, sizeof( float ), 1, fp );
	}
}

static void
WriteRotorRawReadings( FILE * fp, RawData_t * rawData )
{
	// Write the Status info.
	WriteStatus( fp, rawData );
	
	// Write the Temperature info.
	WriteTemperatures( fp, rawData );
	
	// Write the Wavelength info.
	WriteWavelengths( fp, rawData );
}

/*
 * FUNCTION
 * 	WriteStatus
 * 
 * DESCRIPTION
 * 	Writes all the status message data to the file.
 * 
 * PARAMETERS
 * 	fp - RAD file pointer.
 * 	rawData - Received data structure.
 * 
 * RETURNS
 * 	void
 */
	
static void
WriteStatus( FILE * fp, RawData_t * rawData )
{
	int i;
	
	for ( i = 0; i < rawData->receivedDataCounter.statusCount; i++ )
	{
		CrcWrite( &rawData->rotorRawReadings.analysisStatus[i].engineTime, sizeof( int ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.analysisStatus[i].analysisStatus, sizeof( char ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.analysisStatus[i].analysisParameter, sizeof( char ), 1, fp );
	}
}

/*
 * FUNCTION
 * 	WriteTemperatures
 * 
 * DESCRIPTION
 * 	Writes all the temperature data to the file.
 * 
 * PARAMETERS
 * 	fp - RAD file pointer.
 * 	rawReadings - Received data structure.
 * 
 * RETURNS
 * 	void
 */

static void
WriteTemperatures( FILE * fp, RawData_t * rawData )
{
	int i;
	
	for ( i = 0; i < rawData->receivedDataCounter.temperatureCount; i++ )
	{
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].engineTime, sizeof( int ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].rotorTemperature, sizeof( short ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].ambientTemperature, sizeof( short ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].topPlateTemperature, sizeof( short ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].bottomPlateTemperature, sizeof( short ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].topPlateCurrent, sizeof( short ), 1, fp );
		CrcWrite( &rawData->rotorRawReadings.temperatures[i].bottomPlateCurrent, sizeof( short ), 1, fp );
	}
}


/*
 * FUNCTION
 * 	WriteWavelengths
 * 
 * DESCRIPTION
 * 	Writes all the wavelength data.
 * 
 * PARAMETERS
 * 	fp - RAD file pointer.
 * 	rawData - Received data structure.
 * 
 * RETURNS
 * 	void
 */

static void
WriteWavelengths( FILE * fp, RawData_t * rawData )
{
	int i, j, k;
	
	// Write beadCheck1
	for ( i = 0; i < 30; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			WriteWavelength( fp, &rawData->rotorRawReadings.beadCheck1[i][j] );
		}
	}
	
	// Write beadCheck2
	for ( i = 0; i < 30; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			WriteWavelength( fp, &rawData->rotorRawReadings.beadCheck2[i][j] );
		}
	}
	
	// Write distributionCheck
	for ( i = 0; i < 30; i++ )
	{
		for ( j = 0; j < 11; j++ )
		{
			WriteWavelength( fp, &rawData->rotorRawReadings.distributionCheck[i][j] );
		}
	}

	// Write opticalDacs
	for ( i = 0; i < 11; i++ )
	{
		WriteWavelength( fp, &rawData->rotorRawReadings.opticalDacs[i] );
	}
	
	// Write blackOffsets
	for ( i = 0; i < 51; i++ )
	{
		WriteWavelength( fp, &rawData->rotorRawReadings.blackOffsets[i] );
	}
	
	// Write preSampleFullRotorScan
	for ( i = 0; i < 30; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			WriteWavelength( fp, &rawData->rotorRawReadings.preSampleFullRotorScan[i][j] );
		}	
	}
	
	// Write rotorAnalyteSamples
	for ( i = 0; i < 128; i++ )
	{
		if ( rawData->receivedDataCounter.analyteHasData[i] )
		{
			for ( j = 0; j < rawData->receivedDataCounter.passCount; j++ )
			{
				for ( k = 0; k < 11; k++ )
				{
					WriteWavelength( fp, &rawData->rotorRawReadings.rotorAnalyteSamples[i][j][k] );
				}
			}
		}
	}
	
	// Write postSampleFullRotorScan
	for ( i = 0; i < 30; i++ )
	{
		for ( j = 0; j < 3; j++ )
		{
			WriteWavelength( fp, &rawData->rotorRawReadings.postSampleFullRotorScan[i][j] );
		}	
	}
}

/*
 * FUNCTION
 * 	WriteWavelength
 * 
 * DESCRIPTION
 * 	Writes wavelength data to the file.
 * 
 * PARAMETERS
 * 	fp - RAD file pointer.
 * 	w - Wavelength data to write.
 * 
 * RETURNS
 * 	void
 */

static void
WriteWavelength( FILE * fp, RawFlashWavelengths_t * w )
{
	int i;
	
	CrcWrite( &w->engineTime, sizeof( int ), 1, fp );
	for ( i = 0; i < 10; i++ )
	{
		CrcWrite( &w->rawFlashReadings[i], sizeof( short ), 1, fp );
	}
	CrcWrite( &w->sampleType, sizeof( char ), 1, fp );
	CrcWrite( &w->cuvetteNumber, sizeof( char ), 1, fp );
	CrcWrite( &w->flashNumber, sizeof( char ), 1, fp );
}

/*
 * FUNCTION
 * 	WriteDataDescriptor
 * 
 * DESCRIPTION
 * 	Writes the data structure describing the variable length data fields.
 * 
 * PARAMETERS
 * 	fp - RAD file pointer.
 * 	counter - Counter structure.
 *  analyteResults - Analyte results array.
 * 
 * RETURNS
 * 	void
 */

static void
WriteDataDescriptor( FILE * fp, ReceivedDataCounter_t * counter, RotorResults_t * rotorResults )
{
	int				i, j;
	unsigned char	indices[128];
	
// GLD
// #include files entries needed for constants.

	// Write the results indices list.
	memset( indices, 0xFF, 128 );
	for ( i = 0, j = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( rotorResults->rotorAnalyteResult[i].analyteType != 0xFF )
		{
			indices[j] = i;
			j++;
		}
	}
	CrcWrite( indices, sizeof( char ), 64, fp );

	// Write the status, temperature and pass counts.
	CrcWrite( &counter->statusCount, sizeof( char ), 1, fp );
	CrcWrite( &counter->temperatureCount, sizeof( char ), 1, fp );
	CrcWrite( &counter->passCount, sizeof( char ), 1, fp );
	
	// Write the absorbance records count.
	CrcWrite( &rotorResults->rotorAbsorbances.numberAbsRecords, sizeof( int ), 1, fp );

	// Write the raw data indices list.
	memset( indices, 0xFF, 128 );
	for ( i = 0, j = 0; i < 128; i++ )
	{
		if ( counter->analyteHasData[i] )
		{
			indices[j] = i;
			j++;
		}
	}
	CrcWrite( indices, sizeof( char ), 128, fp );
}

size_t
CrcWrite( const void * ptr, size_t size, size_t nobj, FILE * stream )
{
	crc = Crc32Update( crc, (const unsigned char *)ptr, size * nobj );
	return fwrite( ptr, size, nobj, stream );
}

static void
UpdateRadDb( char * fileName )
{
	FILE * 			fp;
	static RadDbIndex_t	radDbIndex;
	DWORD			error;
	
	// Open the RAD index file.
	fp = fopen( RAD_FILE_INDEX, "r+b" );

	error = GetLastError();
	if ( fp == NULL )
	{
		// File doesn't exist.
		if ( error == ERROR_FILE_NOT_FOUND )
		{
			fp = fopen( RAD_FILE_INDEX, "wb" );
			error = GetLastError();
			if ( fp == NULL )
			{
				LogError( "error creating RAD db (%s)", error );
				return;
			}
			memset( &radDbIndex, 0, sizeof( RadDbIndex_t ) );
			radDbIndex.lastEntry = MAX_RAD_FILES - 1;
		}
		
		// Some other error.
		else
		{
			LogError( "error opening RAD db (%s)", error );
			return;
		}
	}

	// File exists, so read it.
	else
	{
		fread( &radDbIndex, sizeof( RadDbIndex_t ), 1, fp );
	}
	
	// Update the entry number.
	if ( ++radDbIndex.lastEntry >= MAX_RAD_FILES )
	{
		radDbIndex.lastEntry = 0;
	}
	
	// We're at the limit on RAD files, so delete the oldest.
	if ( radDbIndex.fileName[ radDbIndex.lastEntry ][0] != '\0' )
	{
		wchar_t	localWSTR[MAX_RAD_FILENAME_LEN];
		wsprintf(localWSTR, L"%S", radDbIndex.fileName[ radDbIndex.lastEntry ]);
		if ( DeleteFile( localWSTR ) == FALSE )
		{
			LogError( "failed to delete RAD file %s", radDbIndex.fileName[ radDbIndex.lastEntry ] );
		}
	}
	
	// Put the new filename in the index.
	strcpy( radDbIndex.fileName[ radDbIndex.lastEntry ], fileName );
	
	// Write the updated index.
	fseek(fp, 0L, SEEK_SET);
	fwrite( &radDbIndex, sizeof( RadDbIndex_t ), 1, fp );
	fclose( fp );
}
