#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <windows.h>
#include <commctrl.h>
#define HAVE_MODE_T 1
#include <FtdiConfig.h>

#include "Analytes.h"
#include "CommonDefines.h"
#include "CounterData.h"
#include "EncodeString.h"
#include "FactoryData.h"
#include "FormatResults.h"
#include "LogErr.h"
#include "Loinc.h"
#include "ResultsDb.h"
#include "SerialComm.h"
#include "Shmem.h"
#include "AStrings.h"
#include "SystemData.h"
#include "Transmit.h"
#include "Units.h"
#include "VersionData.h"
#include "SetSerialPortSpeed.h"

static const float		hemLimits[] = { 20.0, 45.0, 181.0 };
static const float		lipLimits[] = { 150.0, 221.0, 301.0 };
static const float		ictLimits[] = { 2.8, 6.0, 12.1 };

static Analytes			analytes;
static Species			species;
static Units			units;

static unsigned			checksum;
static char				encodeBuff[1000];	// Buffer used by multiple functions for XML-encoding strings.

static CounterData_t *	counterData		= (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
static FactoryData_t *	factoryData		= (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
static SystemData_t *	systemData		= (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
static VersionData_t *	versionData		= (VersionData_t *)MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );

static char *		AgeUnitsElement( const char * elementName, AgeUnits_t ageUnits );
static const char *	EndogenousElement( const char * elementName, float value, const float limits[] );
static void         GetAnalyteValue( ResultsDbEntry_t * result, int resultIndex, char * analyteValue, char * flag );
static char *		StringElement( const char * elementName, const char * data );
static char *		IntElement( const char * elementName, int data );
static char *		HexElement( const char * elementName, unsigned data, int numDigits = 0);
static char *		CounterElement( const char * elementName, unsigned data );
static char *		EndTag( const char * elementName );
static char *		StartTag( const char * elementName );
static char *		DateElement( const char * elementName, FILETIME date );
static char *		DateElement( const char * elementName, Date_t date );
static char *		RefRangeElement( const char * elementName, AnalyteId_t analyteId, float value );
static char *		TimeElement( const char * elementName, FILETIME time );
static bool			ReadDatabase( int index, ResultsDbEntry_t * entry );
#if defined(QNX)
static void			Send( int fdSer, size_t indent, const char * str );
static void			SendFooter( int fdSer );
static void			SendHeader( int fdSer );
static void 		SendResult( int fdSer, int index );
#elif defined(WINCE)
static void			Send( FT_HANDLE fdSer, size_t indent, const char * str );
static void			SendFooter( FT_HANDLE fdSer );
static void			SendHeader( FT_HANDLE fdSer );
static void 		SendResult( FT_HANDLE fdSer, int index );
#endif

#undef NATHON_DEBUG

#ifdef NATHON_DEBUG
#define MAX_DEBUG_LENGTH    12000
char    debugBuf [MAX_DEBUG_LENGTH];
unsigned int      dIndex = 0;
#endif

void
Transmit::
SendXmlReport( FT_HANDLE fdSer, int index )
{
    SetSerialPortSpeed (BAUD_RATE_ASCII_DUMP);

	checksum = 0;
#ifdef NATHON_DEBUG
    dIndex = 0;
    memset (debugBuf, 0, sizeof (debugBuf));
#endif

	// Send the "header" - everything up to the first <result>.
	SendHeader( fdSer );

	// Send the report.
	SendResult( fdSer, index );
	
	// Send the "footer" - everything after the last </result>.
	SendFooter( fdSer );
#ifdef NATHON_DEBUG
    FILE    *fp;
    fp = fopen ("/Hard Disk/Abaxis/NathanDebug.data","wb");
    if (fp == 0) {
        LogDebug ("Fail to open Nathon debug file");
    } else {
        fwrite (debugBuf, dIndex, 1, fp);
        fclose (fp);
    }
#endif

    RecoverPrevSpeed ();
}

void *
Transmit::
SendXmlRange( void * arg )
{
	SendArg_t *				sendArg = (SendArg_t *)arg;
	FILE *					fp;
	int						i;
	int						numResults;
	int						numSent;
	RecallMsg_t				msg;
	const char *			dbIndex;
	bool					sendRange;
	// NOTE: The following variable made static to keep it off the stack, which
	// has a limited size.
	static ResultsDbIndex_t	resultsIndex;

	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;

	sendRange = (sendArg->startDate.dwHighDateTime != 0 || sendArg->startDate.dwLowDateTime != 0)
				|| (sendArg->endDate.dwHighDateTime != 0 || sendArg->endDate.dwLowDateTime != 0);

	// Read the index file.
	fp = fopen( dbIndex, "rb" );
	if ( fp == 0 )
	{
		LogError( "failed to open results index" );
		return 0;
	}
	
	fread( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, fp );
	fclose( fp );
		
	// Check for the version.
	if ( resultsIndex.dbVersion != RESULTS_DB_VERSION )
	{
		return 0;
	}

    SetSerialPortSpeed (BAUD_RATE_ASCII_DUMP);

	// Get the number of results.
	for ( i = 0, numResults = 0; i < RESULTS_MAX; i++ )
	{
		if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
		{
			/*if ( !sendRange
				 || ((resultsIndex.entries[i].dateTime >= sendArg->startDate)
					&& (resultsIndex.entries[i].dateTime <= sendArg->endDate)) )*/
			if ( !sendRange
					|| ((CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg->startDate) >= 0) 
					&& (CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg->endDate) <= 0)) )
			{
				numResults++;
			}
		}
	}

	if ( numResults > 0 )
	{
		checksum = 0;

		// Go thru all the results, looking for ones that match the specified range.
		i = resultsIndex.lastEntry;
		numSent = 0;
		msg.msgId = MSG_RECALL_SEND_PROGRESS;
		do
		{
			i = (i + 1) % RESULTS_MAX;
			
			if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
			{
				/*if ( !sendRange
					 || ((resultsIndex.entries[i].dateTime >= sendArg->startDate)
						&& (resultsIndex.entries[i].dateTime <= sendArg->endDate)) )*/
				if ( !sendRange
						|| ((CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg->startDate) >= 0) 
						&& (CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg->endDate) <= 0)) )
				{
					if ( numSent == 0 )
					{
						// If this is the first result, send the "header" - everything up to the first <result>.
						SendHeader( sendArg->fdSer );
					}

					// Send the report.
					SendResult( sendArg->fdSer, resultsIndex.entries[i].index );
	
					// Send percentage completion.
					msg.idata = (++numSent * 100) / numResults;
					SendRecallMessage( &msg, sizeof( int ) );
				}
			}
		} while ( !cancelSend && (i != resultsIndex.lastEntry) );

		// If we sent any results, send the "footer" - everything after the last </result>.
		if ( numSent > 0 )
		{
			SendFooter( sendArg->fdSer );
		}			
	}

	// Indicate that we're done.
	// PAK: pthread_t is opaque, should not set
	// sendThread = -1;
	sendThreadDone = 1;

	if ( !cancelSend )
	{
		msg.msgId = MSG_RECALL_SEND_RESULTS_COMPLETE;
		SendRecallMessage( &msg, 0 );
	}
	
    
	// Restore the serial port settings.  This takes some time, since
	// the output buffer is 2K.
    RecoverPrevSpeed ();

	return 0;
}

char *
EndTag( const char * elementName )
{
	static char	buff[100];
	
	sprintf( buff, "</%s>\r\n", elementName );
	return buff;
}

char *
StartTag( const char * elementName )
{
	static char	buff[100];
	
	sprintf( buff, "<%s>\r\n", elementName );
	
	return buff;
}

char *
StringElement( const char * elementName, const char * data )
{
	static char	buff[100];

	if ( (data == 0) || (data[0] == '\0') )
	{
		sprintf( buff, "<%s/>\r\n", elementName );
	}
	else
	{
		EncodeString( encodeBuff, data );
		sprintf( buff, "<%s>%s</%s>\r\n", elementName, encodeBuff, elementName );
	}
	
	return buff;
}

char *
IntElement( const char * elementName, int data )
{
	static char	buff[100];

	sprintf( buff, "<%s>%d</%s>\r\n", elementName, data, elementName );
	
	return buff;
}

char *
HexElement( const char * elementName, unsigned data, int numDigits )
{
	static char	buff[100];
    
    if (numDigits > 0) {
        sprintf( buff, "<%s>0x%0*X</%s>\r\n", elementName, numDigits, data, elementName );
    } else {
	    sprintf( buff, "<%s>0x%X</%s>\r\n", elementName, data, elementName );
    }
	
	return buff;
}

char *
CounterElement( const char * elementName, unsigned data )
{
	static char	buff[100];
	char		formatBuff[20];
	int			i;
	
	// Format the data string in two's complement reverse order.
	sprintf( buff, "%08X", ~data + 1 );
	for ( i = 0; i < 4; i++ )
	{
		formatBuff[ i ] = buff[ 7 - i ];
	}
	formatBuff[ 4 ] = ' ';
	formatBuff[ 5 ] = ' ';
	for ( i = 4; i < 8; i++ )
	{
		formatBuff[ i + 2 ] = buff[ 7 - i ];
	}
	formatBuff[ 10 ] = '\0';

	// Now, create the output string.
	sprintf( buff, "<%s>0x%X</%s>\r\n", elementName, data, elementName );
	
	return buff;
}



char *
DateElement( const char * elementName, FILETIME date )
{
	static char	buff[100];
	char		dateBuff[10];
	SYSTEMTIME		tp;
	FileTimeToSystemTime(&date, &tp);

	if ( date.dwHighDateTime == 0 && date.dwLowDateTime == 0 )
	{
		sprintf( buff, "<%s/>\r\n", elementName );
	}
	else
	{
		sprintf( dateBuff, "%04d%02d%02d", tp.wYear, tp.wMonth, tp.wDay );
	
		sprintf( buff, "<%s>%s</%s>\r\n", elementName, dateBuff, elementName );
	}

	return buff;
}

char *
DateElement( const char * elementName, Date_t date )
{
	static char	buff[100];
	
	if ( date.year == 0 )
	{
		sprintf( buff, "<%s/>\r\n", elementName );
	}
	else
	{
		sprintf( buff, "<%s>%04d%02d%02d</%s>\r\n", elementName, date.year, date.month, date.day, elementName );
	}

	return buff;
}

char *
TimeElement( const char * elementName, FILETIME time )
{
	static char	buff[100];
	char		timeBuff[10];
	//struct tm *	tp = gmtime( &time );
	SYSTEMTIME		tp;
	FileTimeToSystemTime(&time, &tp);

	sprintf( timeBuff, "%02d%02d%02d", tp.wHour, tp.wMinute, tp.wSecond );

	sprintf( buff, "<%s>%s</%s>\r\n", elementName, timeBuff, elementName );

	return buff;
}

char *
AgeUnitsElement( const char * elementName, AgeUnits_t ageUnits )
{
	static char	buff[100];
	StringId_t	sid = MSTR_DAYS;

	switch ( ageUnits )
	{
		case AGE_UNITS_DAYS:	sid = MSTR_DAYS;	break;
		case AGE_UNITS_WEEKS:	sid = MSTR_WKS;		break;
		case AGE_UNITS_MONTHS:	sid = MSTR_MOS;		break;
		case AGE_UNITS_YEARS:	sid = MSTR_YRS;		break;
	}
	wcstombs (buff, strings.PrintString (sid), 100);
	EncodeString( encodeBuff, buff );
	memset (buff, 0, 100);
	sprintf( buff, "<%s>%s</%s>\r\n", elementName, encodeBuff, elementName );

	return buff;
}

const char *
EndogenousElement( const char * elementName, float value, const float limits[] )
{
	static char		buff[100];
	const char *	endogenousStrings[] = { "0", "1+", "2+", "3+" };
	const char *	str;

	if ( value < limits[0] )
	{
		str = endogenousStrings[0];
	}
	else if ( value < limits[1] )
	{
		str = endogenousStrings[1];
	}
	else if ( value < limits[2] )
	{
		str = endogenousStrings[2];
	}
	else
	{
		str = endogenousStrings[3];
	}

	EncodeString( encodeBuff, str );
	sprintf( buff, "<%s>%s</%s>\r\n", elementName, encodeBuff, elementName );
	
	return buff;
}

char *
RefRangeElement( const char * elementName, AnalyteId_t analyteId, float value )
{
	static char oBbuff[100];
	static char vBuff[10];
	// Convert to the current units for this analyte.
	value *= units[ analyteId ].mult;

    // Format the value.
    FormatValue( value, vBuff, units[ analyteId ].format );

	sprintf( oBbuff, "<%s>%s</%s>\r\n", elementName, vBuff, elementName );

	return oBbuff;
}


void
#ifdef QNX
SendFooter( int fdSer )
#endif
#ifdef WINCE
SendFooter( FT_HANDLE fdSer )
#endif
{
	char	etx = ETX;
	char	buff[10];

	// Close up the XML.
	Send( fdSer, 1, EndTag( "records" ) );
	Send( fdSer, 0, EndTag( "data" ) );
	
	// Signal end of the checksummed data and send the checksum.
    WriteSerPort (&etx, 1);
    LogDebug ("CheckSum =%d", checksum);
	sprintf( buff, "%08X", checksum );
#ifdef NATHON_DEBUG 
    memcpy (&debugBuf [dIndex], buff, 8);
    dIndex += 8;
#endif
	WriteSerPort (&buff, 8);
}
    

void
SendHeader( FT_HANDLE fdSer )
{
	SYSTEMTIME			time;
	FILETIME			t;
	const char *	    mode = "Normal";
	char			    buff [128];

    memset (buff, 0, sizeof (buff));
	
	GetLocalTime(&time);
	SystemTimeToFileTime(&time, &t);

	if ( systemData->inResearchMode )
		mode = "Research";
	else if ( systemData->inDemoMode )
		mode = "Demo";
	else if ( systemData->inServiceMode )
		mode = "Service";
	else if ( systemData->inManufacturingMode )
		mode = "Manufacturing";

	Send( fdSer, 0, StartTag( "?xml version=\"1.0\" encoding=\"utf-8\"?" ) );
	Send( fdSer, 0, StartTag( "data" ) );

	Send( fdSer, 1, StringElement( "version", "1" ) );
	Send( fdSer, 1, DateElement( "reportDate", t ) );
	Send( fdSer, 1, TimeElement( "reportTime", t ) );
	Send( fdSer, 1, StartTag( "analyzerInfo" ) );
	wcstombs (buff,strings.PrintString (STR_PRODUCT_NAME) , 128);
	Send( fdSer, 2, StringElement( "name", buff) );
	Send( fdSer, 2, StringElement( "serialNumber", factoryData->instrumentSerialNumber ) );
	Send( fdSer, 2, StringElement( "softwareVersion", versionData->version ) );
	Send( fdSer, 2, StringElement( "mode", mode ) );
	Send( fdSer, 2, CounterElement( "runCount", counterData->rotorRunCount ) );
	Send( fdSer, 2, CounterElement( "cancelCount", counterData->rotorAbortCount ) );
	Send( fdSer, 2, CounterElement( "printCount", counterData->printCount ) );
	Send( fdSer, 2, CounterElement( "flashCount", counterData->flashCount ) );

	Send( fdSer, 1, EndTag( "analyzerInfo" ) );
	Send( fdSer, 1, StartTag( "records" ) );
}

void
SendResult( FT_HANDLE fdSer, int index )
{
	static ResultsDbEntry_t result;
	int						i, j;
	static char				buff[100];
	static char				analyteValue[20];
	static char				flag[5];
	const char *			wavelength[] = { "340 nm", "405 nm", "467 nm", "500 nm", "515 nm", "550 nm", "600 nm", "630 nm" };
	RotorAnalyteResult_t *	analyte;
	AnalyteId_t				analyteId;
    AnalyteResultType_t     analyteIndex;
    char                    cBuff [128];

	
	if ( ReadDatabase( index, &result ) == false )
	{
		return;
	}
	
	Send( fdSer, 2, StartTag( "result" ) );

	Send( fdSer, 3, StartTag( "productInfo" ) );
	Send( fdSer, 4, StringElement( "rotorName", result.rotorName ) );
	Send( fdSer, 4, StringElement( "lotNumber", result.lotNumber ) );
	Send( fdSer, 3, EndTag( "productInfo" ) );

	Send( fdSer, 3, StartTag( "sampleInfo" ) );
	Send( fdSer, 4, DateElement( "runDate", result.dateTime ) );
	Send( fdSer, 4, TimeElement( "runTime", result.dateTime ) );
	Send( fdSer, 4, StringElement( "patientControlId", result.patientControlId ) );
	Send( fdSer, 4, StringElement( "operatorId", result.operatorId ) );
	Send( fdSer, 4, StringElement( "alternateId", result.alternateId ) );
	Send( fdSer, 4, StringElement( "doctorId", result.doctorId ) );
	Send( fdSer, 4, StringElement( "location", result.location ) );
	Send( fdSer, 4, StringElement( "phoneNumber", result.phoneNumber ) );
	Send( fdSer, 4, StringElement( "admissionOwnerId", result.admissionOwnerId ) );
	Send( fdSer, 4, DateElement( "dateOfBirth", result.dateOfBirth ) );
	Send( fdSer, 4, DateElement( "lastVaccination", result.lastVaccination ) );
	Send( fdSer, 4, StringElement( "sampleId", result.sampleId ) );
    wcstombs (cBuff, species.GetSampleTypePrintString ((SampleType_t)result.sampleType, &strings), 128);
	Send( fdSer, 4, StringElement( "sampleType", cBuff));
	if ( result.age != 0 )
	{
		Send( fdSer, 4, IntElement( "age", result.age ) );
		Send( fdSer, 4, AgeUnitsElement( "ageUnits", result.ageUnits ) );
	}
	else
	{
		Send( fdSer, 4, StringElement( "age", 0 ) );
		Send( fdSer, 4, StringElement( "ageUnits", 0 ) );
	}
	switch( result.race )
	{
		case RACE_UNKNOWN:					sprintf( buff, "unknown" );					break;
		case RACE_WHITE:					sprintf( buff, "white" );					break;
		case RACE_BLACK:					sprintf( buff, "black" );					break;
		case RACE_ASIAN_PACIFIC_ISLANDER:	sprintf( buff, "asianPacificIslander" );	break;
		case RACE_NATIVE_AMERICAN:			sprintf( buff, "nativeAmerican" );			break;
		case RACE_HISPANIC:					sprintf( buff, "hispanic" );				break;
		case RACE_NONE:
		default:							buff[0] = '\0';								break;
	}
	Send( fdSer, 4, StringElement( "race", buff ) );

	switch( result.gender )
	{
		case GENDER_UNKNOWN:	sprintf( buff, "unknown" );		break;
		case GENDER_MALE:		sprintf( buff, "male" );		break;
		case GENDER_FEMALE:		sprintf( buff, "female" );		break;
		case GENDER_SPAYED:		sprintf( buff, "spayed" );		break;
		case GENDER_NEUTERED:	sprintf( buff, "neutered" );	break;
		case GENDER_NONE:
		default:				buff[0] = '\0';					break;
	}
	Send( fdSer, 4, StringElement( "gender", buff ) );
	
	Send( fdSer, 3, EndTag( "sampleInfo" ) );
    // Send analyte results and iQC results only if there is no error.
    if ( result.errNum == SERR_NONE  ) {
        // Analyte results
	    Send( fdSer, 3, StartTag( "analyteResults" ) );
	    for ( i = 1; i <= MAX_ROTOR_RESULTS; i++ )
	    {
		    for ( j = 0; j < MAX_ROTOR_RESULTS; j++ )
		    {
			    if ( result.rotorAnalyteResults[ j ].printOrder == i )
			    {
                    analyteIndex = (AnalyteResultType_t)result.rotorAnalyteResults[ j ].analyteType;
           //       analyte = &result.rotorAnalyteResults[ analyteIndex ];
                    analyte = &result.rotorAnalyteResults[ j ];
                    analyteId = analytes.GetAnalyteId( analyteIndex );

				
				    Send( fdSer, 4, StartTag( "analyte" ) );
                    wcstombs (cBuff, analytes.GetAnalysisString (analyteId , &strings), 128);
				    Send( fdSer, 5, StringElement( "name", cBuff ) );
				    Send( fdSer, 5, StringElement( "loinc", GetLoinc( analyteId ) ) );
				    GetAnalyteValue( &result, j, analyteValue, flag );
				    Send( fdSer, 5, StringElement( "value", analyteValue ) );
				    Send( fdSer, 5, StringElement( "valueMark", flag ) );
				    if ( SuppressRefRange( analyte->lowReferenceRangeLimit, analyte->highReferenceRangeLimit,
									   result.rotorId, analyteId, (SampleType_t)result.sampleType ) )
				    {
					    Send( fdSer, 5, StringElement( "lowReferenceRange", 0 ) );
					    Send( fdSer, 5, StringElement( "highReferenceRange", 0 ) );
				    }
				    else
				    {
					    Send( fdSer, 5, RefRangeElement( "lowReferenceRange", analyteId, analyte->lowReferenceRangeLimit ) );
					    Send( fdSer, 5, RefRangeElement( "highReferenceRange", analyteId, analyte->highReferenceRangeLimit ) );

				    }
                    wcstombs (cBuff, units.String( analyteId ), 128);
				    Send( fdSer, 5, StringElement( "units", cBuff ) );
				    Send( fdSer, 5, HexElement( "resultPrintFlags", analyte->resultPrintFlags, 8 ) );
				    Send( fdSer, 4, EndTag( "analyte" ) );
			    }
		    }
	    }
	    Send( fdSer, 3, EndTag( "analyteResults" ) );
        // iQC results.

	    Send( fdSer, 3, StartTag( "iqcResults" ) );
	    Send( fdSer, 4, StringElement( "controlExpired", result.controlExpired ? "YES" : "NO" ) );
	    Send( fdSer, 4, StringElement( "operatorExpired", result.operatorExpired ? "YES" : "NO" ) );
	    Send( fdSer, 4, StringElement( "operatorUnauthorized", result.operatorUnauthorized ? "YES" : "NO" ) );
	    Send( fdSer, 4, StringElement( "dilutionError", result.rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError ? "YES" : "NO" ) );
	    Send( fdSer, 4, StartTag( "endogenous" ) );
	    Send( fdSer, 5, StartTag( "hem" ) );
	    Send( fdSer, 6, EndogenousElement( "index", result.rotorInformationResults.rotorHemolyzedIndex, hemLimits ) );
	    Send( fdSer, 6, IntElement( "value", (int)( result.rotorInformationResults.rotorHemolyzedIndex + 0.5) ) );
	    Send( fdSer, 5, EndTag( "hem" ) );
	    Send( fdSer, 5, StartTag( "lip" ) );
	    Send( fdSer, 6, EndogenousElement( "index", result.rotorInformationResults.rotorLipemicIndex, lipLimits ) );
	    Send( fdSer, 6, IntElement( "value", (int)( result.rotorInformationResults.rotorLipemicIndex + 0.5) ) );
	    Send( fdSer, 5, EndTag( "lip" ) );
	    Send( fdSer, 5, StartTag( "ict" ) );
	    Send( fdSer, 6, EndogenousElement( "index", result.rotorInformationResults.rotorIctericIndex, ictLimits ) );
	    Send( fdSer, 6, IntElement( "value", (int)( result.rotorInformationResults.rotorIctericIndex + 0.5) ) );
	    Send( fdSer, 5, EndTag( "ict" ) );
	    Send( fdSer, 4, EndTag( "endogenous" ) );

	    Send( fdSer, 4, IntElement( "rqc", (int)( result.rotorAnalyteResults[ RQC ].analyteResult + 0.5) ) );
	    Send( fdSer, 4, IntElement( "acceptableMinimum", RQC_ACCEPTABLE_MINIMUM ) );
	
	    Send( fdSer, 4, StartTag( "level1" ) );
	    for ( i = 0; i < 8; i++ )
	    {
		    Send( fdSer, 5, StartTag( "test" ) );
		    sprintf( buff, "IQC %d", i+1 );
		    Send( fdSer, 6, StringElement( "name", buff ) );
		    Send( fdSer, 6, IntElement( "value", (int)( (result.rotorInformationResults.rotorIqcLevel1Results[ i ] * 100) + 0.5) ) );
		    Send( fdSer, 6, IntElement( "lowReferenceRange", 90 ) );
		    Send( fdSer, 6, IntElement( "highReferenceRange", 110 ) );
		    Send( fdSer, 5, EndTag( "test" ) );
	    }
	    Send( fdSer, 4, EndTag( "level1" ) );
	
	    Send( fdSer, 4, StartTag( "level2" ) );
	    Send( fdSer, 5, StartTag( "test" ) );
	    Send( fdSer, 6, StringElement( "name", "Precision" ) );		// GLD - do we need to translate?
	    Send( fdSer, 6, IntElement( "value", (int)( (result.rotorInformationResults.rotorIqcRatioPrecision * 100) + 0.5) ) );
	    Send( fdSer, 6, IntElement( "lowReferenceRange", 95 ) );
	    Send( fdSer, 6, IntElement( "highReferenceRange", 105 ) );
	    Send( fdSer, 5, EndTag( "test" ) );
	    for ( i = 0; i < 8; i++ )
	    {
		    Send( fdSer, 5, StartTag( "test" ) );
		    Send( fdSer, 6, StringElement( "name", wavelength[ i ] ) );
		    Send( fdSer, 6, IntElement( "value", (int)( (result.rotorInformationResults.rotorIqcLevel2Results[ i ] * 100) + 0.5) ) );
		    Send( fdSer, 6, IntElement( "lowReferenceRange", 95 ) );
		    Send( fdSer, 6, IntElement( "highReferenceRange", 105 ) );
		    Send( fdSer, 5, EndTag( "test" ) );
	    }
	    Send( fdSer, 4, EndTag( "level2" ) );
	    Send( fdSer, 3, EndTag( "iqcResults" ) );
    }
	
	Send( fdSer, 3, StartTag( "statusFlags" ) );
	Send( fdSer, 4, HexElement( "errNum", result.errNum ) );
	Send( fdSer, 4, HexElement( "systemFlags", result.rotorInformationResults.rotorSystemFlags ) );
	Send( fdSer, 4, HexElement( "beadCheck1Flags", result.rotorInformationResults.rotorBeadCheck1Flags ) );
	Send( fdSer, 4, HexElement( "beadCheck2Flags", result.rotorInformationResults.rotorBeadCheck2Flags ) );
	Send( fdSer, 4, HexElement( "distributionCheckFlags", result.rotorInformationResults.rotorDistributionCheckFlags ) );
	Send( fdSer, 4, HexElement( "dacTrimFlags", result.rotorInformationResults.rotorDacTrimFlags ) );
	Send( fdSer, 4, HexElement( "offsetErrorFlags", result.rotorInformationResults.rotorOffsetErrorFlags ) );
	Send( fdSer, 4, HexElement( "offsetSdErrorFlags", result.rotorInformationResults.rotorOffsetSdErrorFlags ) );
	Send( fdSer, 4, HexElement( "wavelengthCvFlags", result.rotorInformationResults.rotorWavelengthCvFlags ) );
	Send( fdSer, 4, HexElement( "qcFlags", result.rotorInformationResults.rotorQcFlags ) );
	Send( fdSer, 3, EndTag( "statusFlags" ) );

	Send( fdSer, 2, EndTag( "result" ) );
}

void
#ifdef QNX
Send( int fdSer, size_t indent, const char * str )
#endif
#ifdef WINCE
Send( FT_HANDLE fdSer, size_t indent, const char * str )
#endif
{
	static const char	tabs[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	static char			buff[100];
	size_t				i;
	
	if ( indent > strlen( tabs ) )
	{
		indent = strlen( tabs );
	}

	sprintf( buff, "%s%s", tabs + strlen( tabs ) - indent, str );

    WriteSerPort (buff, strlen(buff));
	
	for ( i = 0; i < strlen( buff ); i++ )
	{
 
#ifdef   NATHON_DEBUG
        // copy to debug buffer
        debugBuf [dIndex++]= buff [i]; 
        if (dIndex > MAX_DEBUG_LENGTH) {
            LogDebug ("not enough debug buffer");
            dIndex = 0;
        }
#endif
		checksum += (unsigned char)buff[i];
	}
}

bool
ReadDatabase( int index, ResultsDbEntry_t * entry )
{
// PAK: moved open/read/close to fopen/fread/fclose
	const char *	dbData;
	FILE *			fp;
	int				retVal = false;
	
	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;
	
	fp = fopen( dbData, "rb");
	if ( fp != NULL )
	{
		if ( fseek( fp, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) != -1 )
		{
			int recRead = fread((char *)entry, sizeof( ResultsDbEntry_t ),1, fp );
			if ( recRead == 1 )
			{
				retVal = true;
			}
		}
		fclose( fp );
	}
	else
	{
		LogError( "failed to open %s", dbData );
	}

	return retVal;
}


/*
 * FUNCTION
 * 	GetAnalyteValue
 * 
 * DESCRIPTION
 * 	Retrieves the analyte value, with prefix, and the flag from the formatted analyte value. Uses FormatResult, which
 * 	formats the result for printing and display, and so has everything in one string, with spaces to justify. If
 * 	FormatResult is changed, this may have to change, too (a comment to this effect is in FormatResult). The format from
 * 	FormatResult is "p rrrr f", where the prefix, p, is a '<', '>', or ' '; the result, rrrr, is the right-justified result;
 * 	and the flag, f, is '*', 'H', 'L', or 'I'.
 *
 * PARAMETERS
 * 	result			- Pointer to the record containing the result, reference range, result flags, etc., required by FormatResult.
 * 	analyteIndex	- Index of the particular analyte desired.
 * 	analyteValue	- Location to receive the analyte value string, with prefix if present, with flag, and with spaces removed.
 * 	flag			- Location to receive the flag character.
 */

void
GetAnalyteValue( ResultsDbEntry_t * result, int resultIndex, char * analyteValue, char * flag )
{
	char *	fResult;
	char *	s;
	
	// This will give us the value, with prefix, and flag (*, etc.)
    fResult = FormatResult( result, resultIndex, FMT_NORMAL, &strings );

	// Split the result between the value and the flag.
	fResult[5] = '\0';
	
	// Find the start of the result value, which is one or more characters after the first, prefix, character.
	s = fResult + 1;
	while ( *s == ' ' )
		s++;

	// Get the result value, with the prefix if there.
	if ( fResult[0] != ' ' )
	{
		sprintf( analyteValue, "%c%s", fResult[0], s );
	}
	else
	{
		strcpy( analyteValue, s );
	}

	// Get the flag.
	if ( fResult[6] != ' ' )
		flag[0] = fResult[6];
	else
		flag[0] = '\0';
	flag[1] = '\0';
}
