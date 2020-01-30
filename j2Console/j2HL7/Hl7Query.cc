// ********************************************************************
//
// FILENAME:  
// 
// 		Hl7Query.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains functions to retrieve results records from the results database.
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

#include "Hl7Query.h"
#include "Hl7Data.h"
#include "ResultsDb.h"
#include "LogErr.h"
#include "AStrings.h"
#include "Shmem.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "CounterData.h"
#include "SystemError.h"
#include "Species.h"
#include "Loinc.h"
#include "FormatResults.h"
#include "Units.h"
#include "VersionData.h"

//Delimiters used in the OBX results fields constructed 
//outside the j2Hl7 project
#define ABAXIS_RESULT_FIELD_DELIMITER			'|'
#define ABAXIS_RESULT_REPEAT_DELIMITER			'\\'
#define ABAXIS_RESULT_COMPONENT_DELIMITER		'^'
#define ABAXIS_RESULT_ESCAPE_CHARACTER			'&'





static int					CompareDate( const void * a, const void * b );
static void					FormatCounter( char * str, UINT32 value, int size );
static void					FormatDate( Date_t date, char * buff, int size );
static Hl7Record_t *		GetResult( int recordId );
static FILETIME				GetTime( const char * tStr, unsigned int def );
static ResultsDbIndex_t *	ReadIndex();
static bool					RetrieveResult( ResultsDbEntry_t * entry, int index );

static VersionData_t *		versionData = (VersionData_t *)		MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );
static FactoryData_t *		factoryData = (FactoryData_t *)		MapSharedMem( FACTORY_SHMEM_NAME,	sizeof( FactoryData_t ) );
static SystemData_t *		systemData	= (SystemData_t *)		MapSharedMem( SYSTEM_SHMEM_NAME,	sizeof( SystemData_t ) );
static CounterData_t *		counterData	= (CounterData_t *)		MapSharedMem( COUNTERS_SHMEM_NAME,	sizeof( CounterData_t ) );
static ResultsDbChanged_t *	dbChanged	= (ResultsDbChanged_t *)MapSharedMem( DATABASE_SHMEM_NAME,	sizeof( ResultsDbChanged_t ) );
static int					idList[ RESULTS_MAX ];
static int					idCount;
static int					numRecordsFound;
static AStrings				strings;
static Units                units;

char observationStartTime[16];
char observationEndTime[16];


void
MarkTransmitResultsRecordsDone( void )
{
	LogDebug( "MarkTransmitResultsRecordsDone" );
}

void
MarkResultsRecordSent( int recordId )
{
	FILE *				fd;
	HANDLE				lock;
	const char *		dbData;
	ResultsDbEntry_t	entry;
	//off_t				offset;
	DWORD				dwWaitResult;

	if ( IN_CUSTOMER_MODE ) {
		dbData = CUSTOMER_DB_DATA;
	}
	else {
		dbData = OTHER_DB_DATA;
	}
	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL )	{
		LogError( "unable to lock database" );
		return;
	}
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 )	{
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return;
	}

	// Update the database.
	fd = fopen( dbData, "r+b" );
	if ( fd != NULL )	{
		if ( ( fseek( fd, recordId * sizeof( ResultsDbEntry_t ), SEEK_SET )) == 0 ) {
			int bytesRead = fread( &entry, 1, sizeof( ResultsDbEntry_t ), fd );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )	{
				fseek( fd, recordId * sizeof( ResultsDbEntry_t ), SEEK_SET );
				entry.dataSent = 1;
				fwrite( &entry, sizeof( ResultsDbEntry_t ), 1, fd );
			}
		}
		fclose( fd );
	}
	else	{
		LogError( "failed to open %s", dbData );
	}

	ReleaseMutex(lock);
	CloseHandle(lock);
}

/*
 * FUNCTION
 * 	GetInstrumentInfo
 * 
 * DESCRIPTION
 * 	Gets info for the instrument that is independent of a result set.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed instrument information.
 */

InstrumentInfo_t *
GetInstrumentInfo()
{
	static InstrumentInfo_t	instrumentInfo;
	char   buff [512];

	//For now only log an error. More robust error handling should be added
	if ( (versionData == NULL) ||
		 (factoryData == NULL) ||
		 (systemData == NULL) ||
		 (counterData == NULL) ||
		 (dbChanged == NULL) ) {
		 LogError( "MapSharedMem function failed versionData=%x, factoryData=%x, systemData=%x, counterData=%x, dbChanged=%x",
					versionData, factoryData, systemData, counterData, dbChanged);
	}
	else {

		wcstombs (buff,strings.English( STR_PRODUCT_NAME ), 512); 
		strncpy( instrumentInfo.hAnalyzerName,buff, sizeof(instrumentInfo.hAnalyzerName)-1 );
		strncpy( instrumentInfo.hSoftwareVersion, versionData->version, sizeof(instrumentInfo.hSoftwareVersion)-1 );
		strncpy( instrumentInfo.hSerialNumber, factoryData->instrumentSerialNumber, sizeof(instrumentInfo.hSerialNumber)-1 );
		if ( systemData->inDemoMode ) {
			instrumentInfo.hProcessingId = 'D';
		}
		else {
			instrumentInfo.hProcessingId = 'P';
		}
		
		instrumentInfo.productCode = factoryData->abaxisProductCode;
	}
	return &instrumentInfo;
}

/*
 * FUNCTION
 * 	GetCurrentDateTime
 * 
 * DESCRIPTION
 * 	Returns the current date and time in an HL7 date/time string.
 * 
 * RETURNS
 * 	Pointer to the date/time string.
 */

char *
GetCurrentDateTime()
{
	static char	dateTime[ 20 ];

	SYSTEMTIME	currentST;
	FILETIME	currentFT;
	GetLocalTime(&currentST);
	SystemTimeToFileTime(&currentST, &currentFT);

	FormatDateTime( currentFT, dateTime, sizeof (dateTime));

	return dateTime;
} 

/*
 * FUNCTION
 * 	SearchHl7ResultsRecord
 * 
 * DESCRIPTION
 * 	Searches the database for records with the given sample ID and with run times
 *  between the given start time and end time.
 * 
 * PARAMETERS
 *	patientControlId - ID to search for, or "ALL" if all records in date range requested.
 *	startTime - Beginning time for the time range, or NULL if all records prior to
 * 				endTime requested.
 * 	endTime - Ending time for data range, or NULL if all records after startTime
 * 			  requested.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed result information for the
 * 	first result record that matches the query parameters.
 */

Hl7Record_t *
SearchHl7ResultsRecord( const char * patientControlId, const char * startTime, const char * endTime )
{
	FILETIME			start, end;
	int					resultCount;
	ResultsDbIndex_t *	resultsIndex;
	bool				doAll = false;
	int					i;
	Hl7Record_t * tempRecord;
	
	// Read the index into local memory.
	resultsIndex = ReadIndex();
	if ( resultsIndex == false ) {
		return 0;
	}

	doAll = strcmp( patientControlId, "ALL" ) == 0 ? true : false;
	
	// Get the start and end times as time_t's, which are what we use in the database.
	start = GetTime( startTime, 0 );
	end = GetTime( endTime, UINT_MAX );
	
	for ( resultCount = 0; resultCount < RESULTS_MAX; resultCount++ )
	{
		if ( resultsIndex->entries[ resultCount ].patientControlId[0] == (char)0xFF )	{
			break;
		}
	}
	
	// Sort the results by date, with the oldest date last, since we're going
	// to count down from the end of the list.
	qsort( &resultsIndex->entries, resultCount, sizeof( ResultsDbIndexEntry_t ), CompareDate );
	
	// Find the results that match our criteria and add them to the list.

	for ( i = 0, idCount = 0; i < resultCount; i++ )
	{
		if ( CompareFileTime(&resultsIndex->entries[ i ].dateTime, &start) >= 0 && CompareFileTime(&resultsIndex->entries[ i ].dateTime, &end) <= 0 )	{
			if ( doAll || strcmp( patientControlId, resultsIndex->entries[ i ].patientControlId ) == 0 )	{
				idList[ idCount ] = resultsIndex->entries[ i ].index;
				idCount++;
			}
		}
	}
	
	//Remember how many records were found
	numRecordsFound = idCount;
	tempRecord = GetOldestHl7ResultsRecord();				// observation date/time start					       
	if (tempRecord != NULL) {
		strncpy(observationStartTime,tempRecord->oRunDateTime, sizeof(observationStartTime));
	}
	tempRecord = GetNewestHl7ResultsRecord();				// observation date/time end					       
	if (tempRecord != NULL) {
		strncpy(observationEndTime,tempRecord->oRunDateTime,sizeof(observationEndTime));
	}
	// Return the result record at the end of the list (oldest).
	return GetNextHl7ResultsRecord();
}


/*
 * FUNCTION
 * 	GetNextHl7ResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves the next record in the sequence of records matching the query parameters
 * 	passed to the most recent call to SearchHl7ResultsRecord.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed result information, or NULL if
 * 	there are no more records matching the query parameters.
 */

Hl7Record_t *
GetNextHl7ResultsRecord()
{
	while ( idCount > 0 )
	{
		--idCount;
		if ( (*dbChanged)[ idList[ idCount ] ] == false )	{
			return GetResult( idList[ idCount ] );
		}
	}

	return 0;
}

/*
 * FUNCTION
 * 	GetOldestHl7ResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves the oldest record in the sequence of records matching the query parameters
 * 	passed to the most recent call to SearchHl7ResultsRecord.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed result information, or NULL if
 * 	there are no more records matching the query parameters.
 */

Hl7Record_t *
GetOldestHl7ResultsRecord()
{
	//Have to account for record in idList[0]
	if (numRecordsFound > 0) {
		return GetResult( idList[ (numRecordsFound-1) ] );
	}
	else {
		return NULL;
	}
}

/*
 * FUNCTION
 * 	GetNewestHl7ResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves the first record in the sequence of records matching the query parameters
 * 	passed to the most recent call to SearchHl7ResultsRecord.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed result information, or NULL if
 * 	there are no more records matching the query parameters.
 */

Hl7Record_t *
GetNewestHl7ResultsRecord()
{
	if (numRecordsFound > 0) {
		return GetResult( idList[ 0 ] );
	}
	else {
		return NULL;
	}
}



/*
 * FUNCTION
 * 	GetHl7ResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record.  This is triggered by Analysis
 * 	telling HL7 there's a new record.
 * 
 * PARAMETERS
 * 	recordId - Database record ID of the desired record.
 *
 * RETURNS
 * 	Pointer to a data structure holding the needed result information.
 */

Hl7Record_t *
GetHl7ResultsRecord( int recordId )
{
	ResultsDbIndex_t *	resultsIndex;

	// Make sure there's data in the specified record.
	resultsIndex = ReadIndex();
	if ( (resultsIndex == 0) || (resultsIndex->entries[ recordId ].patientControlId[0] == (char)0xFF) )	{
		return 0;
	}
	// Retrieve the specified record and return a populated HL7 result record.
	return GetResult( recordId );
}

/*
 * FUNCTION
 * 	clearResultList
 * 
 * DESCRIPTION
 * 	Clear the number of records found in the database.
 * 
 * PARAMETERS
 * 	None
 *
 * RETURNS
 * 	None
 */

void clearResultList()
{
	idCount = 0;
}
//
// ********************************************************************
// FUNCTION:
//
//		ProcessRecordTxFields() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Configures the delimiter characters to agree with the configuration file settings
// _____________________________________________________________________
// PARAMETERS:
//
//		record - pointer to the record
//		size of record
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessRecordTxFields(char *record, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		switch (*record)
		{
			case ABAXIS_RESULT_FIELD_DELIMITER:
				*record = hl7Configuration_m->segmentMSHOut.mshFieldDelimiter;
				break;
			case ABAXIS_RESULT_COMPONENT_DELIMITER:
				*record = hl7Configuration_m->segmentMSHOut.mshComponentDelimiter;
				break;
			case ABAXIS_RESULT_REPEAT_DELIMITER:
				*record = hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter;
				break;
			case ABAXIS_RESULT_ESCAPE_CHARACTER:
				*record = hl7Configuration_m->segmentMSHOut.mshEscapeCharacter;
				break;
			default:
				break;
		}
		record++;
	}
}


/*
 * FUNCTION
 * 	GetResult
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	recordId - Database record ID of the desired record.
 *
 * RETURNS
 * 	Pointer to a data structure holding the needed result information.
 */
    static Hl7Record_t r;
    static ResultsDbEntry_t		entry;
Hl7Record_t *
GetResult( int recordId )
{
    static Analytes				analytes;
    SystemError					systemError;
    Species						species;
    char *						refRange;
    //const char *				ageUnits;
	char						buff [512];
    AnalyteResultType_t			analyteIndex;
    memset( &entry, 0, sizeof( ResultsDbEntry_t ) );
    memset( &r, 0, sizeof( Hl7Record_t ) );
	 
    // Retrieve the entry from the database.  If there's an error, return null.
    if ( RetrieveResult( &entry, recordId ) == false )  {
        return 0;
    }

    r.hl7RecordType = entry.errNum == SERR_NONE ? 0 : 1;
    r.recordId = recordId;
    r.hl7ResultRecordSent = entry.dataSent;

    /*
     * 'PID' record
     */
    strncpy( r.pSampleId, entry.sampleId, sizeof (r.pSampleId)-1 );
    strncpy( r.pPatientControlId, entry.patientControlId, sizeof (r.pPatientControlId)-1 );
    strncpy( r.pAlternateId, entry.alternateId, sizeof (r.pAlternateId)-1 );

    if ( entry.dateOfBirth.year != 0 )
    {
        FormatDate( entry.dateOfBirth, r.pDateOfBirth, sizeof(r.pDateOfBirth) );
    }
    if ( entry.gender == GENDER_MALE || entry.gender == GENDER_NEUTERED ) {
        r.pGender = 'M';
    }
    else if ( entry.gender == GENDER_FEMALE || entry.gender == GENDER_SPAYED ) {
        r.pGender = 'F';
    }
    else {
        r.pGender = 'U';
   }

    switch ( entry.race )
    {
        case RACE_WHITE:
			r.pRace = 'W';
			break;
        case RACE_BLACK:
			r.pRace = 'B';
			break;
        case RACE_ASIAN_PACIFIC_ISLANDER:
			r.pRace = 'O';
			break;
        case RACE_NATIVE_AMERICAN:
			r.pRace = 'N';
			break;
        case RACE_HISPANIC:
			r.pRace = 'H';
			break;
        case RACE_UNKNOWN:
        case RACE_NONE:
        default:
			r.pRace = '\0';
			break;
    }

    strncpy( r.pPhoneNumber, entry.phoneNumber, sizeof (r.pPhoneNumber)-1 );
    strncpy( r.pOwnerAdmissionId, entry.admissionOwnerId, sizeof (r.pOwnerAdmissionId)-1 );

    if ( entry.age > 0 )  {
        switch ( entry.ageUnits )
        {
            case AGE_UNITS_DAYS:
				wcstombs (buff, strings.English( MSTR_DAYS ), 512);
			break;
            case AGE_UNITS_WEEKS:   
				wcstombs (buff, strings.English( MSTR_WKS ), 512);
			break;
            case AGE_UNITS_MONTHS: 
				wcstombs (buff, strings.English( MSTR_MOS ), 512);  
			break;
            case AGE_UNITS_YEARS:  
				wcstombs (buff, strings.English( MSTR_YRS ), 512);    
			break;
            default:
				buff [0] = '\0';                                          
			break;
        }
        _snprintf( r.pAge, sizeof(r.pAge)-1, "%d %s", entry.age, buff);
    }

    if ( entry.lastVaccination.year != 0 )  {
        FormatDate( entry.lastVaccination, r.pLastVaccination, sizeof(r.pLastVaccination) );
    }
	wcstombs (buff, species.English( (SampleType_t)entry.sampleType, &strings ), 512);
	strncpy( r.pReferenceRangeLabel, buff,sizeof (r.pReferenceRangeLabel)-1 );

	strncpy( r.pLocation, entry.location, sizeof (r.pLocation) -1);

    /*
     * 'OBR' record
     */

    strncpy( r.oRotorName, entry.rotorName, sizeof (r.oRotorName) -1);
    strncpy( r.oRotorLotNumber, entry.lotNumber, sizeof (r.oRotorLotNumber) -1);

    FormatDateTime( entry.dateTime, r.oRunDateTime, sizeof(r.oRunDateTime));

    strncpy( r.oOperatorId, entry.operatorId, sizeof (r.oOperatorId) -1);
    strncpy( r.oDoctorId, entry.doctorId, sizeof (r.oDoctorId)-1 );
   /*
     * 'OBR' record, sequence '1' "Rotor Results", 'NTE' record, sequence '1'-'2'
     */

    _snprintf( r.o1C2HemString, (sizeof (r.o1C2HemString)-1), "%d", (int)( entry.rotorInformationResults.rotorHemolyzedIndex + 0.5 ) );
    _snprintf( r.o1C2LipString, (sizeof (r.o1C2LipString)-1), "%d", (int)( entry.rotorInformationResults.rotorLipemicIndex + 0.5 ));
    _snprintf( r.o1C2IctString, (sizeof (r.o1C2IctString)-1), "%d", (int)( entry.rotorInformationResults.rotorIctericIndex + 0.5 ));


    r.o1C3RotorDilutionCheckError = entry.rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError;

    if ( entry.operatorExpired ) {
		wcstombs (buff, strings.English( RSTR_OPERATOR_RENEWAL ), 512);
        strncpy( r.o1C4OperatorSecurityStatus, buff, sizeof (r.o1C4OperatorSecurityStatus)-1 );
    }
    else if ( entry.operatorUnauthorized )  {
		wcstombs (buff, strings.English( RSTR_UNAUTHORIZED_OPERATOR ), 512);
        strncpy( r.o1C4OperatorSecurityStatus, buff, sizeof (r.o1C4OperatorSecurityStatus) -1);
    }

    if ( entry.controlExpired )  {
		wcstombs (buff, strings.English( RSTR_CONTROL_RENEWAL ) , 512);
        strncpy( r.o1C5ControlExpired, buff, sizeof (r.o1C5ControlExpired) -1);
    }

    /*
     * 'OBR' record, sequence '2' "Rotor iQC", 'NTE' record, sequence '1'-'2'
     */


    _snprintf( r.o2C1RqcResultString, (sizeof (r.o2C1RqcResultString)-1), "%d", (int)( entry.rotorAnalyteResults[ RQC ].analyteResult + 0.5 ));
    _snprintf( r.o2C2RqcLowLimitString, (sizeof (r.o2C2RqcLowLimitString)-1), "%d", RQC_ACCEPTABLE_MINIMUM);

    /*
     * 'OBR' record, sequence '1' "Rotor Error", 'NTE' record, sequence '1'-'7'
     */

    FormatCounter( r.o1C1RunCount, counterData->rotorRunCount, sizeof(r.o1C1RunCount) );
    FormatCounter( r.o1C1AbortCount, counterData->rotorAbortCount, sizeof(r.o1C1AbortCount) );
    FormatCounter( r.o1C2PrintCount, counterData->printCount, sizeof(r.o1C2PrintCount) );
    FormatCounter( r.o1C2FlashCount, counterData->flashCount, sizeof(r.o1C2FlashCount) );

    r.o1C3DacTrimFlags = entry.rotorInformationResults.rotorDacTrimFlags;
    r.o1C3OffsetErrorFlags = entry.rotorInformationResults.rotorOffsetErrorFlags;
    r.o1C3SystemFlags = entry.rotorInformationResults.rotorSystemFlags;
    r.o1C4BeadCheck1Flags = entry.rotorInformationResults.rotorBeadCheck1Flags;
    r.o1C4BeadCheck2Flags = entry.rotorInformationResults.rotorBeadCheck2Flags;
    r.o1C5EmptyCuvFlags = 0;
    r.o1C5DistCheckFlags = entry.rotorInformationResults.rotorDistributionCheckFlags;
    r.o1C6QcFlags = entry.rotorInformationResults.rotorQcFlags;
    r.o1C6OffsetSdErrorFlags = entry.rotorInformationResults.rotorOffsetSdErrorFlags;
    r.o1C6DacWavelengthCvFlags = entry.rotorInformationResults.rotorWavelengthCvFlags;
    r.o1C7SystemErrorNumber = entry.errNum;
	wcstombs (buff, strings.English( systemError.GetScreenText( entry.errNum ) ), 512);
    strncpy( r.o1C7SystemErrorString, buff, sizeof(r.o1C7SystemErrorString)-1 );


    /*
     * 'OBX' record
     */

    for ( int i = 1, k = 0; (i <= MAX_ROTOR_RESULTS) && (k < 16); i++ )
    {
        for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
        {
            if ( entry.rotorAnalyteResults[ j ].printOrder == i ) {
                analyteIndex = (AnalyteResultType_t)entry.rotorAnalyteResults[ j ].analyteType;

                AnalyteId_t analyteId = analytes.GetAnalyteId( analyteIndex );

                strncpy( r.rAnalyteResults[ k ].rnUniversalTestId, GetLoinc( analyteId ), sizeof (r.rAnalyteResults[ k ].rnUniversalTestId) -1);

                strncpy( r.rAnalyteResults[ k ].rnResultString, FormatResult( &entry, j, FMT_NORMAL, &strings ), sizeof ( r.rAnalyteResults[ k ].rnResultString)-1);

                r.rAnalyteResults[ k ].rnC1ResultRaw = entry.rotorAnalyteResults[ j ].analyteResult;
				wcstombs (buff, units.String( analyteId ), 512);
                strncpy( r.rAnalyteResults[ k ].rnUnitsString, buff , sizeof (r.rAnalyteResults[ k ].rnUnitsString)-1 );

                refRange = FormatRefRange( &entry, j );
                if ( strcmp( refRange, "         " ) != 0 ) {
                    strncpy( r.rAnalyteResults[ k ].rnLoRefRangeString, refRange, 4 );
                    strncpy( r.rAnalyteResults[ k ].rnHiRefRangeString, refRange + 5, 4 );
                }

                if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & ( MASK_RESULTS_SUPPRESSED | MASK_HEMOLYZED | MASK_LIPEMIC | MASK_ICTERIC ) ) {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '\0';
                }
                else if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & (MASK_LOW_SYSTEM_LIMIT | MASK_LOW_DYNAMIC_LIMIT) ) {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '<';
                }
                else if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & (MASK_HIGH_SYSTEM_LIMIT | MASK_HIGH_DYNAMIC_LIMIT) ) {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '>';
                }
                else if ( strcmp( refRange, "         " ) == 0 )  {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '\0';
                }
				else if ( r.rAnalyteResults[ k ].rnC1ResultRaw < entry.rotorAnalyteResults[ j ].lowReferenceRangeLimit ) {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = 'L';
                }
                else if ( r.rAnalyteResults[ k ].rnC1ResultRaw > entry.rotorAnalyteResults[ j ].highReferenceRangeLimit )  {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = 'H';
                }
                else {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = 'N';
                }

                r.rAnalyteResults[ k ].rnC1AnalyteFlags = entry.rotorAnalyteResults[ j ].analyteFlags;
                r.rAnalyteResults[ k ].rnC1ResultPrintFlags = entry.rotorAnalyteResults[ j ].resultPrintFlags;

                k++;
                break;
            }
        }
    }

	for ( int i = 0; i < 8; i++ )
    {
        _snprintf( r.rIqcResultsStrings[ i ],(IQC_LEVEL_NAME_STRING_SIZE-1), "%d", (int)( (entry.rotorInformationResults.rotorIqcLevel1Results[ i ] * 100.0) + 0.5 ) );
        _snprintf( r.rIqcResultsStrings[ i+9 ],(IQC_LEVEL_NAME_STRING_SIZE-1), "%d", (int)( (entry.rotorInformationResults.rotorIqcLevel2Results[ i ] * 100.0) + 0.5 ) );

    }
    _snprintf( r.rIqcResultsStrings[ 8 ],(IQC_LEVEL_NAME_STRING_SIZE-1), "%d", (int)( (entry.rotorInformationResults.rotorIqcRatioPrecision * 100.0) + 0.5 ) );
	// IQC Results - OBX record text strings for IQC results for 0BR2OBX1 to OBR2OBX17

	// IQC Results - OBX record text strings for IQC results for 0BR2OBX1 to OBR2OBX17
	_snprintf(r.iqcHl7LevelNameStrings[0],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 1 ");
	_snprintf(r.iqcHl7LevelNameStrings[1],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 2 ");
	_snprintf(r.iqcHl7LevelNameStrings[2],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 3 ");
	_snprintf(r.iqcHl7LevelNameStrings[3],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 4 ");
	_snprintf(r.iqcHl7LevelNameStrings[4],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 5 ");
	_snprintf(r.iqcHl7LevelNameStrings[5],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 6 ");
	_snprintf(r.iqcHl7LevelNameStrings[6],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 7 ");
	_snprintf(r.iqcHl7LevelNameStrings[7],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 1: IQC 8 ");
	_snprintf(r.iqcHl7LevelNameStrings[8],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 2: PRE   ");
	_snprintf(r.iqcHl7LevelNameStrings[9],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s",	"^^^LEVEL 2: 340 nm");
	_snprintf(r.iqcHl7LevelNameStrings[10],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 405 nm");
	_snprintf(r.iqcHl7LevelNameStrings[11],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 467 nm");
	_snprintf(r.iqcHl7LevelNameStrings[12],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 500 nm");
	_snprintf(r.iqcHl7LevelNameStrings[13],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 515 nm");
	_snprintf(r.iqcHl7LevelNameStrings[14],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 550 nm");
	_snprintf(r.iqcHl7LevelNameStrings[15],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 600 nm");
	_snprintf(r.iqcHl7LevelNameStrings[16],(IQC_LEVEL_NAME_STRING_SIZE-1), "%s","^^^LEVEL 2: 630 nm");
	//Convert delimiters defined in the configuration file
	ProcessRecordTxFields((char *)&r, sizeof(Hl7Record_t));
	return &r;
}
/*
 * FUNCTION
 * 	RetrieveResult
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	entry - 
 *  index - index into database
 *
 * RETURNS
 * 	found or not found
 */

bool
RetrieveResult( ResultsDbEntry_t * entry, int index )
{
	FILE *			fd;
	HANDLE			lock;
	bool			retVal = false;
	const char *	dbData;
	DWORD			dwWaitResult;

	if ( IN_CUSTOMER_MODE ) {
		dbData = CUSTOMER_DB_DATA;
	}
	else {
		dbData = OTHER_DB_DATA;
	}
	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL ) {
		LogError( "unable to lock database" );
		return false;
	}
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 ) {
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return false;
	}

	// Get the data from the database.
	fd = fopen( dbData, "rb" );
	if ( fd != NULL ) {
		if ( fseek( fd, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) == 0 ) {
			int bytesRead = fread( entry, 1, sizeof( ResultsDbEntry_t ), fd );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) ) {
				retVal = true;
			}
		}
		fclose( fd );
	}
	else {
		LogError( "failed to open %s", dbData );
	}

	ReleaseMutex(lock);
	CloseHandle(lock);

	return retVal;
}	
/*
 * FUNCTION
 * 	FormatDateTime
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	entry - 
 *  index - index into database
 *
 * RETURNS
 * 	found or not found
 */

void
FormatDateTime( FILETIME t, char * buff, int size )
{
	//struct tm * tm = gmtime( &t );
	SYSTEMTIME tm;
	FileTimeToSystemTime(&t, &tm);
	
	_snprintf( buff,(size-1), "%04d%02d%02d%02d%02d%02d", tm.wYear,
											   tm.wMonth,
											   tm.wDay,
											   tm.wHour,
											   tm.wMinute,
											   tm.wSecond );
}

/*
 * FUNCTION
 * 	FormatDate
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	entry - 
 *  index - index into database
 *  size - max size
 *
 * RETURNS
 * 	found or not found
 */

void
FormatDate( Date_t date, char * buff, int size )
{
	_snprintf( buff, (size-1), "%04d%02d%02d", date.year, date.month/* + 1*/, date.day );
}

/*
 * FUNCTION
 * 	GetTime
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	entry - 
 *  index - index into database
 *
 * RETURNS
 * 	found or not found
 */

FILETIME
GetTime( const char * tStr, unsigned int def )
{
	SYSTEMTIME	tm;
	char		buff[10];
	FILETIME	t;
	int			result;
	
	if ( (tStr == 0) || (tStr[0] == '\0') ) {
		t.dwLowDateTime = def;
		t.dwHighDateTime = def;
	}
	
	else {
		strncpy( buff, tStr, 4 );
		buff[4] = '\0';
		tm.wYear = atoi( buff );
		strncpy( buff, tStr + 4, 2 );
		buff[2] = '\0';
		tm.wMonth = atoi( buff );
		strncpy( buff, tStr + 6, 2 );
		tm.wDay = atoi( buff );
		strncpy( buff, tStr + 8, 2 );
		tm.wHour = atoi( buff );
		strncpy( buff, tStr + 10, 2 );
		tm.wMinute = atoi( buff );
		strncpy( buff, tStr + 12, 2 );
		tm.wSecond = atoi( buff );
		
		result = SystemTimeToFileTime(&tm, &t);
		if ( result == 0 ) {
			t.dwLowDateTime = def;
			t.dwHighDateTime = def;
		}
	}
	
	return t;
}

/*
 * FUNCTION
 * 	CompareDate
 * 
 * DESCRIPTION
 * 	Compares the dates for two index table entries.  Called by qsort().
 * 
 * PARAMETERS
 * 	a, b - Pointers to the index table entries.
 * 
 * RETURNS
 * 	<0 if the date for the first entry is greater than that for the second,
 *   0 if the dates are the same, or
 *  >0 if the date for the first entry is less than that for the second.
 */

int
CompareDate( const void * a, const void * b )
{
	FILETIME timeA = ((ResultsDbIndexEntry_t *)a)->dateTime;
	FILETIME timeB = ((ResultsDbIndexEntry_t *)b)->dateTime;

	//return timeB - timeA;	
	return CompareFileTime(&timeB, &timeA);
}

/*
 * FUNCTION
 * 	FormatCounter
 * 
 * DESCRIPTION
 * 	Formats a counter string.  The 2's-complement of the value is taken and then
 * 	put into a string in reverse order.
 * 
 * PARAMETERS
 * 	str - String into which to place the formatted value.
 *  value - Value to format.
 *  size - max size
 */

void
FormatCounter( char * str, UINT32 value, int size )
{
	char	buff[20];
	int		i;
	
	_snprintf( buff,(size-1), "%08X", ~value + 1 );
	for ( i = 0; i < 4; i++ )
	{
		str[ i ] = buff[ 7 - i ];
	}
	str[ 4 ] = ' ';
	for ( i = 4; i < 8; i++ )
	{
		str[ i + 1 ] = buff[ 7 - i ];
	}
	str[ 9 ] = '\0';
}

/*
 * FUNCTION
 * 	ReadIndex
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an HL7
 * 	results record.
 * 
 * PARAMETERS
 * 	entry - 
 *  index - index into database
 *
 * RETURNS
 * 	found or not found
 */


ResultsDbIndex_t *
ReadIndex()
{
	FILE *					fd;
	HANDLE					lock;
	static ResultsDbIndex_t	index;
	ResultsDbIndex_t *		retVal = 0;
	DWORD					bytesRead;
	const char *			dbIndex;
	DWORD					dwWaitResult;

	memset( &index, 0, sizeof( ResultsDbIndex_t ) );

	// Lock the database.
	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL ) {
		LogError( "unable to lock database" );
		return 0;
	}
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 ) {
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return 0;
	}

	// Open the index file.
	if ( IN_CUSTOMER_MODE ) {
		dbIndex = CUSTOMER_DB_INDEX;
	}
	else {
		dbIndex = OTHER_DB_INDEX;
	}
	fd = fopen( dbIndex, "rb" );

	if ( fd == NULL ) {
		LogError( "unable to open index %s", dbIndex );
	}

	// Read the index file.
	else {
		bytesRead = fread( &index, 1, sizeof( ResultsDbIndex_t ), fd );
		if ( (bytesRead != sizeof( ResultsDbIndex_t )) || (index.dbVersion != RESULTS_DB_VERSION) ) {
			LogError( "read db version of %d", index.dbVersion );
		}
		else {
			retVal = &index;
		}
		fclose( fd );
	}

	// Clear the changed flags.
	memset( dbChanged, 0, sizeof( ResultsDbChanged_t ) );

	// Unlock the database
	ReleaseMutex(lock);
	CloseHandle(lock);

	return retVal;	
}
