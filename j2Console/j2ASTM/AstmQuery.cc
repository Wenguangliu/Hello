/*
 * FILENAME
 * 	AstmQuery.cc
 * 
 * MODULE DESCRIPTION
 * 	Contains functions to retrieve results records from the results database.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>

#include "Astm.h"
#include "AstmQuery.h"
#include "AstmData.h"
#include "ResultsDb.h"
#include "LogErr.h"
#include "AStrings.h"
#include "Shmem.h"
//#include "FactoryData.h"
#include "SystemData.h"
#include "CounterData.h"
#include "SystemError.h"
//#include "Species.h"
#include "Loinc.h"
#include "FormatResults.h"
#include "Units.h"
#include "VersionData.h"

Species                     species;

static int					CompareDate( const void * a, const void * b );
static void					FormatCounter( char * str, UINT32 value );
static void					FormatDate( Date_t date, char * buff );
static AstmRecord_t *		GetResult( int recordId );
static FILETIME				GetTime( const char * tStr, unsigned int def );
static ResultsDbIndex_t *	ReadIndex();
static bool					RetrieveResult( ResultsDbEntry_t * entry, int index );

static VersionData_t *		versionData = (VersionData_t *)		MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );
static CounterData_t *		counterData	= (CounterData_t *)		MapSharedMem( COUNTERS_SHMEM_NAME,	sizeof( CounterData_t ) );
static ResultsDbChanged_t *	dbChanged	= (ResultsDbChanged_t *)MapSharedMem( DATABASE_SHMEM_NAME,	sizeof( ResultsDbChanged_t ) );
static int					idList[ RESULTS_MAX ];
static int					idCount;
static AStrings				strings;
static Units                units;

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

	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;

	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL )
	{
		LogError( "unable to lock database" );
		return;
	}
	//flock( lock, LOCK_EX );
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 )
	{
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return;
	}

	// Update the database.
	fd = fopen( dbData, "r+b" );
	if ( fd != NULL )
	{
		if ( ( fseek( fd, recordId * sizeof( ResultsDbEntry_t ), SEEK_SET )) == 0 )
		{
			int bytesRead = fread( &entry, 1, sizeof( ResultsDbEntry_t ), fd );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )
			{
				//lseek( fd, offset, SEEK_SET );
				fseek( fd, recordId * sizeof( ResultsDbEntry_t ), SEEK_SET );
				entry.dataSent = 1;
				//write( fd, (void *)&entry, sizeof( ResultsDbEntry_t ) );
				fwrite( &entry, sizeof( ResultsDbEntry_t ), 1, fd );
			}
		}
		fclose( fd );
	}
	else
	{
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
	char	buff [512];
	static	InstrumentInfo_t	instrumentInfo;
	
	wcstombs (buff, strings.English( STR_PRODUCT_NAME ), 512);
	strcpy( instrumentInfo.hAnalyzerName,     buff );
	strcpy( instrumentInfo.hSoftwareVersion, versionData->version );
	strcpy( instrumentInfo.hSerialNumber, factoryData->instrumentSerialNumber );
	if ( systemData->inDemoMode )
	{
		instrumentInfo.hProcessingId = 'D';
	}
	else
	{
		instrumentInfo.hProcessingId = 'P';
	}
	
	instrumentInfo.productCode = factoryData->abaxisProductCode;

	return &instrumentInfo;
}

/*
 * FUNCTION
 * 	GetCurrentDateTime
 * 
 * DESCRIPTION
 * 	Returns the current date and time in an ASTM date/time string.
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

	//FormatDateTime( time( 0 ), dateTime );
	FormatDateTime( currentFT, dateTime );

	return dateTime;
} 

/*
 * FUNCTION
 * 	SearchAstmResultsRecord
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

AstmRecord_t *
SearchAstmResultsRecord( const char * patientControlId, const char * startTime, const char * endTime )
{
	FILETIME			start, end;
	int					resultCount;
	ResultsDbIndex_t *	resultsIndex;
	bool				doAll = false;
	int					i;
	
	// Read the index into local memory.
	resultsIndex = ReadIndex();
	if ( resultsIndex == false )
	{
		return 0;
	}

	doAll = strcmp( patientControlId, "ALL" ) == 0 ? true : false;
	
	// Get the start and end times as time_t's, which are what we use in the database.
	start = GetTime( startTime, 0 );
	end = GetTime( endTime, UINT_MAX );
	
	for ( resultCount = 0; resultCount < RESULTS_MAX; resultCount++ )
	{
		if ( resultsIndex->entries[ resultCount ].patientControlId[0] == (char)0xFF )
		{
			break;
		}
	}
	
	// Sort the results by date, with the oldest date last, since we're going
	// to count down from the end of the list.
	qsort( &resultsIndex->entries, resultCount, sizeof( ResultsDbIndexEntry_t ), CompareDate );
	
	// Find the results that match our criteria and add them to the list.
	for ( i = 0, idCount = 0; i < resultCount; i++ )
	{
		//if ( resultsIndex->entries[ i ].dateTime >= start && resultsIndex->entries[ i ].dateTime <= end )
		if ( CompareFileTime(&resultsIndex->entries[ i ].dateTime, &start) >= 0 && CompareFileTime(&resultsIndex->entries[ i ].dateTime, &end) <= 0 )
		{
			if ( doAll || strcmp( patientControlId, resultsIndex->entries[ i ].patientControlId ) == 0 )
			{
				idList[ idCount ] = resultsIndex->entries[ i ].index;
				idCount++;
			}
		}
	}

	// Return the result record at the end of the list (oldest).
	return GetNextAstmResultsRecord();
}


/*
 * FUNCTION
 * 	GetNextAstmResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves the next record in the sequence of records matching the query parameters
 * 	passed to the most recent call to SearchAstmResultsRecord.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed result information, or NULL if
 * 	there are no more records matching the query parameters.
 */

AstmRecord_t *
GetNextAstmResultsRecord()
{
	while ( idCount > 0 )
	{
		--idCount;
		if ( (*dbChanged)[ idList[ idCount ] ] == false )
		{
			return GetResult( idList[ idCount ] );
		}
	}

	return 0;
}


/*
 * FUNCTION
 * 	GetAstmResultsRecord
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record.  This is triggered by Analysis
 * 	telling ASTM there's a new record.
 * 
 * PARAMETERS
 * 	recordId - Database record ID of the desired record.
 *
 * RETURNS
 * 	Pointer to a data structure holding the needed result information.
 */

AstmRecord_t *
GetAstmResultsRecord( int recordId )
{
	ResultsDbIndex_t *	resultsIndex;

	// Make sure there's data in the specified record.
	resultsIndex = ReadIndex();
	if ( (resultsIndex == 0) || (resultsIndex->entries[ recordId ].patientControlId[0] == (char)0xFF) )
	{
			return 0;
	}
	// Retrieve the specified record and return a populated ASTM result record.
	return GetResult( recordId );
}
/*
 * FUNCTION
 * 	GetResult
 * 
 * DESCRIPTION
 * 	Retrieves a specific database record and puts the result into an ASTM
 * 	results record.
 * 
 * PARAMETERS
 * 	recordId - Database record ID of the desired record.
 *
 * RETURNS
 * 	Pointer to a data structure holding the needed result information.
 */
AstmRecord_t *
GetResult( int recordId )
{
    static AstmRecord_t r;
    static ResultsDbEntry_t     entry;
    static Analytes             analytes;
    SystemError                 systemError;
    char *                      refRange;
    //const char *				ageUnits;
    AnalyteResultType_t         analyteIndex;
	char						buff [512];

    memset( &entry, 0, sizeof( ResultsDbEntry_t ) );
    memset( &r, 0, sizeof( AstmRecord_t ) );

    // Retrieve the entry from the database.  If there's an error, return null.
    if ( RetrieveResult( &entry, recordId ) == false )
    {
        return 0;
    }

    r.astmRecordType = entry.errNum == SERR_NONE ? 0 : 1;
    r.recordId = recordId;
    r.astmResultRecordSent = entry.dataSent;

    /*
     * 'P' record
     */

    strcpy( r.pSampleId, entry.sampleId );
    strcpy( r.pPatientControlId, entry.patientControlId );
    strcpy( r.pAlternateId, entry.alternateId );

    if ( entry.dateOfBirth.year != 0 )
    {
        FormatDate( entry.dateOfBirth, r.pDateOfBirth );
    }

    if ( entry.gender == GENDER_MALE || entry.gender == GENDER_NEUTERED )
    {
        r.pGender = 'M';
    }
    else if ( entry.gender == GENDER_FEMALE || entry.gender == GENDER_SPAYED )
    {
        r.pGender = 'F';
    }
    else
    {
        r.pGender = 'U';
    }

    switch ( entry.race )
    {
        case RACE_WHITE:                    strcpy( r.pRace, "W" );     break;
        case RACE_BLACK:                    strcpy( r.pRace, "B" );     break;
        case RACE_ASIAN_PACIFIC_ISLANDER:   strcpy( r.pRace, "O" );     break;
        case RACE_NATIVE_AMERICAN:          strcpy( r.pRace, "NA" );    break;
        case RACE_HISPANIC:                 strcpy( r.pRace, "H" );     break;
        case RACE_UNKNOWN:
        case RACE_NONE:
        default:                            r.pRace[0] = '\0';          break;
    }

    strcpy( r.pPhoneNumber, entry.phoneNumber );
    strcpy( r.pOwnerAdmissionId, entry.admissionOwnerId );

    if ( entry.age > 0 )
    {
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
        sprintf( r.pAge, "%d %s", entry.age, buff );
    }

    if ( entry.lastVaccination.year != 0 )
    {
        FormatDate( entry.lastVaccination, r.pLastVaccination );
    }
	wcstombs (buff, species.English( (SampleType_t)entry.sampleType, &strings ), 512);
    strcpy( r.pReferenceRangeLabel, buff);
    strcpy( r.pLocation, entry.location );

    /*
     * 'O' record
     */

    strcpy( r.oRotorName, entry.rotorName );
    strcpy( r.oRotorLotNumber, entry.lotNumber );

    FormatDateTime( entry.dateTime, r.oRunDateTime );

    strcpy( r.oOperatorId, entry.operatorId );
    strcpy( r.oDoctorId, entry.doctorId );
   /*
     * 'O' record, sequence '1' "Rotor Results", 'C' record, sequence '1'-'2'
     */

    sprintf( r.o1C2HemString, "%d", (int)( entry.rotorInformationResults.rotorHemolyzedIndex + 0.5 ) );
    sprintf( r.o1C2LipString, "%d", (int)( entry.rotorInformationResults.rotorLipemicIndex + 0.5 ) );
    sprintf( r.o1C2IctString, "%d", (int)( entry.rotorInformationResults.rotorIctericIndex + 0.5 ) );


    r.o1C3RotorDilutionCheckError = entry.rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError;

    if ( entry.operatorExpired )
    {
		wcstombs (buff, strings.English( RSTR_OPERATOR_RENEWAL ), 512);
		strcpy( r.o1C4OperatorSecurityStatus, buff);
    }
    else if ( entry.operatorUnauthorized )
    {
		wcstombs (buff, strings.English( RSTR_UNAUTHORIZED_OPERATOR ), 512);
		strcpy( r.o1C4OperatorSecurityStatus, buff);
    }

    if ( entry.controlExpired )
    {
		wcstombs (buff, strings.English( RSTR_CONTROL_RENEWAL), 512);
        strcpy( r.o1C5ControlExpired, buff );
    }

    /*
     * 'O' record, sequence '2' "Rotor iQC", 'C' record, sequence '1'-'2'
     */

    sprintf( r.o2C1RqcResultString, "%d", (int)( entry.rotorAnalyteResults[ RQC ].analyteResult + 0.5 ) );
//  sprintf( r.o2C2RqcLowLimitString, "%d", (int)round( entry.rotorInformationResults.rotorRqcLowLimit ) );
    sprintf( r.o2C2RqcLowLimitString, "%d", RQC_ACCEPTABLE_MINIMUM );

    /*
     * 'O' record, sequence '1' "Rotor Error", 'C' record, sequence '1'-'7'
     */

    FormatCounter( r.o1C1RunCount, counterData->rotorRunCount );
    FormatCounter( r.o1C1AbortCount, counterData->rotorAbortCount );
    FormatCounter( r.o1C2PrintCount, counterData->printCount );
    FormatCounter( r.o1C2FlashCount, counterData->flashCount );

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
	wcstombs (buff,strings.English( systemError.GetScreenText( entry.errNum ) ), 512);
    strcpy( r.o1C7SystemErrorString, buff );


    /*
     * 'R' record
     */

    for ( int i = 1, k = 0; (i <= MAX_ROTOR_RESULTS) && (k < 16); i++ )
    {
        for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
        {
            if ( entry.rotorAnalyteResults[ j ].printOrder == i )
            {
                analyteIndex = (AnalyteResultType_t)entry.rotorAnalyteResults[ j ].analyteType;

                AnalyteId_t analyteId = analytes.GetAnalyteId( analyteIndex );

                strcpy( r.rAnalyteResults[ k ].rnUniversalTestId, GetLoinc( analyteId ) );

                strcpy( r.rAnalyteResults[ k ].rnResultString, FormatResult( &entry, j, FMT_NORMAL, &strings) );

                r.rAnalyteResults[ k ].rnC1ResultRaw = entry.rotorAnalyteResults[ j ].analyteResult;
				
				wcstombs (buff,units.String( analyteId ), 512);
                strcpy( r.rAnalyteResults[ k ].rnUnitsString, buff);

                refRange = FormatRefRange( &entry, j );
                if ( strcmp( refRange, "         " ) != 0 )
                {
                    strncpy( r.rAnalyteResults[ k ].rnLoRefRangeString, refRange, 4 );
                    strncpy( r.rAnalyteResults[ k ].rnHiRefRangeString, refRange + 5, 4 );
                }

                if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & ( MASK_RESULTS_SUPPRESSED | MASK_HEMOLYZED | MASK_LIPEMIC | MASK_ICTERIC ) )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '\0';
                }
                else if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & (MASK_LOW_SYSTEM_LIMIT | MASK_LOW_DYNAMIC_LIMIT) )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '<';
                }
                else if ( entry.rotorAnalyteResults[ j ].resultPrintFlags & (MASK_HIGH_SYSTEM_LIMIT | MASK_HIGH_DYNAMIC_LIMIT) )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '>';
                }
                else if ( strcmp( refRange, "         " ) == 0 )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = '\0';
                }
              else if ( r.rAnalyteResults[ k ].rnC1ResultRaw < entry.rotorAnalyteResults[ j ].lowReferenceRangeLimit )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = 'L';
                }
                else if ( r.rAnalyteResults[ k ].rnC1ResultRaw > entry.rotorAnalyteResults[ j ].highReferenceRangeLimit )
                {
                    r.rAnalyteResults[ k ].rnAbnormalFlag = 'H';
                }
                else
                {
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
        sprintf( r.rIqcResultsStrings[ i ], "%d", (int)( (entry.rotorInformationResults.rotorIqcLevel1Results[ i ] * 100.0) + 0.5 ) );
        sprintf( r.rIqcResultsStrings[ i+9 ], "%d", (int)( (entry.rotorInformationResults.rotorIqcLevel2Results[ i ] * 100.0) + 0.5 ) );

    }
    sprintf( r.rIqcResultsStrings[ 8 ], "%d", (int)( (entry.rotorInformationResults.rotorIqcRatioPrecision * 100.0) + 0.5 ) );

    return &r;
}

bool
RetrieveResult( ResultsDbEntry_t * entry, int index )
{
	FILE *			fd;
	HANDLE			lock;
	bool			retVal = false;
	const char *	dbData;
	DWORD			dwWaitResult;

	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;

	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL )
	{
		LogError( "unable to lock database" );
		return false;
	}
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 )
	{
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return false;
	}

	// Get the data from the database.
	fd = fopen( dbData, "rb" );
	if ( fd != NULL )
	{
		if ( fseek( fd, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) == 0 )
		{
			int bytesRead = fread( entry, 1, sizeof( ResultsDbEntry_t ), fd );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )
			{
				retVal = true;
			}
		}
		fclose( fd );
	}
	else
	{
		LogError( "failed to open %s", dbData );
	}

	ReleaseMutex(lock);
	CloseHandle(lock);

	return retVal;
}	

void
FormatDateTime( FILETIME t, char * buff )
{
	//struct tm * tm = gmtime( &t );
	SYSTEMTIME tm;
	FileTimeToSystemTime(&t, &tm);
	
	sprintf( buff, "%04d%02d%02d%02d%02d%02d", tm.wYear,
											   tm.wMonth,
											   tm.wDay,
											   tm.wHour,
											   tm.wMinute,
											   tm.wSecond );
}

void
FormatDate( Date_t date, char * buff )
{
	sprintf( buff, "%04d%02d%02d", date.year, date.month/* + 1*/, date.day );
}

FILETIME
GetTime( const char * tStr, unsigned int def )
{
	SYSTEMTIME	tm;
	char		buff[10];
	FILETIME	t;
	int			result;
	
	if ( (tStr == 0) || (tStr[0] == '\0') )
	{
		t.dwLowDateTime = def;
		t.dwHighDateTime = def;
	}
	
	else
	{
		strncpy( buff, tStr, 4 );
		buff[4] = '\0';
		//tm.tm_year = atoi( buff ) - 1900;
		tm.wYear = atoi( buff );
		strncpy( buff, tStr + 4, 2 );
		buff[2] = '\0';
		//tm.tm_mon = atoi( buff ) - 1;
		tm.wMonth = atoi( buff );
		strncpy( buff, tStr + 6, 2 );
		//tm.tm_mday = atoi( buff );
		tm.wDay = atoi( buff );
		strncpy( buff, tStr + 8, 2 );
		//tm.tm_hour = atoi( buff );
		tm.wHour = atoi( buff );
		strncpy( buff, tStr + 10, 2 );
		//tm.tm_min = atoi( buff );
		tm.wMinute = atoi( buff );
		strncpy( buff, tStr + 12, 2 );
		//tm.tm_sec = atoi( buff );
		tm.wSecond = atoi( buff );
		
		//t = mktime( &tm );
		result = SystemTimeToFileTime(&tm, &t);
		//if ( t == (time_t)-1 )
		if ( result == 0 )
		{
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
 */

void
FormatCounter( char * str, UINT32 value )
{
	char	buff[20];
	int		i;
	
	sprintf( buff, "%08X", ~value + 1 );
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
	//lock = open( RESULTS_LOCK, O_WRONLY | O_CREAT );
	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL )
	{
		LogError( "unable to lock database" );
		return 0;
	}
	//flock( lock, LOCK_EX );
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 )
	{
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return 0;
	}

	// Open the index file.
	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;

	//fd = open( dbIndex, O_RDONLY );
	fd = fopen( dbIndex, "rb" );

	if ( fd == NULL )
	{
		LogError( "unable to open index %s", dbIndex );
	}

	// Read the index file.
	else
	{
		//bytesRead = read( fd, (char *)&index, sizeof( ResultsDbIndex_t ) );
		bytesRead = fread( &index, 1, sizeof( ResultsDbIndex_t ), fd );
		if ( (bytesRead != sizeof( ResultsDbIndex_t )) || (index.dbVersion != RESULTS_DB_VERSION) )
		{
			LogError( "read db version of %d", index.dbVersion );
		}
		else
		{
			retVal = &index;
		}
		fclose( fd );
	}

	// Clear the changed flags.
	memset( dbChanged, 0, sizeof( ResultsDbChanged_t ) );

	// Unlock the database
	//flock( lock, LOCK_UN );
	//close( lock );
	ReleaseMutex(lock);
	CloseHandle(lock);

	return retVal;	
}
