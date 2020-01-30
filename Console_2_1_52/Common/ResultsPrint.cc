#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <math.h>

#include "ResultsPrint.h"
#include "ResultsDb.h"
#include "Shmem.h"
#include "LogErr.h"
#include "Species.h"
#include "Analytes.h"
#include "Units.h"
#include "ParseCounters.h"
#include "SystemError.h"
#include "FormatResults.h"

#define BILLION				1000000000LL;

const char *		endogenousStrings[] = { "0", "1+", "2+", "3+" };
const float			hemLimits[] = { 20.0, 45.0, 181.0 };
const float			lipLimits[] = { 150.0, 221.0, 301.0 };
const float			ictLimits[] = { 2.8, 6.0, 12.1 };

const char *		wavelengthStrings[] = { "340", "405", "467", "500", "515", "550", "600", "630" };

int					ResultsPrint::fdSer = -1;
int					ResultsPrint::fdUsb = -1;
time_t				ResultsPrint::lastReportTime = time( 0 );
AStrings			ResultsPrint::strings;
Analytes			ResultsPrint::analytes;
Units				ResultsPrint::units;
Species				ResultsPrint::species;

CounterData_t *		ResultsPrint::counterData	= (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
FactoryData_t *		ResultsPrint::factoryData	= (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SettingsData_t *	ResultsPrint::settings		= (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
SystemData_t *		ResultsPrint::systemData	= (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
RefRangeData_t *	ResultsPrint::refRanges		= (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );
char *				ResultsPrint::version		= (char *)MapSharedMem( VERSION_SHMEM_NAME, MAX_VERSION_LEN + 1 );

// GLD
// This function was created in an attempt to monitor the status of the paper and
// not write if the printer reported that the paper was out.  However, the Seiko
// printer reports paper out even if there's paper there in some cases.
int
ResultsPrint::
WriteSer( const void * buf, size_t nbytes )
{
	int				retVal;

	retVal = write( fdSer, buf, nbytes );

	return retVal;
}

SystemErrNum_t
ResultsPrint::
TestExternalPrinter()
{
	char			buff[100];
	int				i, j;
	char			c;
	const char *	header;

	if ( fdUsb == -1 )
	{
		InitExternalPrinterPort();
		if ( fdUsb == -1 )
		{
			return SERR_EXT_PTR_NOT_FOUND;
		}
	}

	// Write the header - centered in a 60-character-wide line.
	header = strings[ PSTR_PRINTER_TEST ];
	i = strlen( header );
	sprintf( buff, "%*s\r\n", i + (60 - i) / 2, header );
	write( fdUsb, buff, strlen( buff ) );
	write( fdUsb, "............................................................\r\n", 62 );


	// Write the test lines.
	for ( i = 0, c = '!'; i < 35; i++, c++ )
	{
		for ( j = 0; j < 60; j++ )
		{
			buff[ j ] = c + j;
		}
		buff[60] = '\r';
		buff[61] = '\n';
		write( fdUsb, buff, 62 );
	}

	// Move the paper up.
	write( fdUsb, "\f", 1 );
	
	return SERR_NONE;
}

SystemErrNum_t
ResultsPrint::
TestInternalPrinter()
{
	char	buff[100];
	int		i, j;
	char	c;

	if ( fdSer == -1 )
	{
		InitInternalPrinterPort();
		if ( fdSer == -1 )
		{
// GLD
// Some other error?
			return SERR_NONE;
		}
	}

	// Write the header.
	CenterText( strings[ PSTR_PRINTER_TEST ], buff );
	WriteSer( buff, strlen( buff ) );
	WriteSer( "..............................\r\n", 32 );

	// Write the test lines.
	for ( i = 0, c = '!'; i < 65; i++, c++ )
	{
		for ( j = 0; j < 30; j++ )
		{
			buff[ j ] = c + j;
		}
		buff[30] = '\r';
		buff[31] = '\n';
		WriteSer( buff, 32 );
	}

	// Move the paper up.
	WriteSer( "\n\n\n", 3 );
	counterData->printCount++;
	WriteCounters( counterData );
	
	return SERR_NONE;
}

/*
 * FUNCTION
 * 	ResultsPrint::PrintResult
 * 
 * DESCRIPTION
 * 	Prints a result to the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 */

SystemErrNum_t
ResultsPrint::
PrintResult( int index )
{
	SystemErrNum_t		errNum = SERR_NONE;
	ResultsDbEntry_t	entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		errNum = PrintResult( &entry );
	}
// GLD
// What if we don't find the result - a database error should be returned.

	return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintResult( ResultsDbEntry_t * entry )
{
	char buff[ 2000 ];
	
	CreateResultDisplay( entry, buff );

	return PrintReport( buff );
}

/*
 * FUNCTION
 * 	ResultsPrint::PrintErrorReport
 * 
 * DESCRIPTION
 * 	Prints an error report to the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 * 	abbreviated - If true, omit the results.
 */

SystemErrNum_t
ResultsPrint::
PrintErrorReport( int index, bool abbreviated )
{
	SystemErrNum_t		errNum = SERR_NONE;
	ResultsDbEntry_t	entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		errNum = PrintErrorReport( &entry, abbreviated );
	}
// GLD
// Return error if GetResult fails.

	return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintErrorReport( ResultsDbEntry_t * entry, bool abbreviated )
{
	char buff[ 2000 ];
	
	CreateErrorReportDisplay( entry, buff, abbreviated );

	return PrintReport( buff );
}

/*
 * FUNCTION
 * 	ResultsPrint::PrintIqc
 * 
 * DESCRIPTION
 * 	Prints an IQC report to the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired sample in the results database.
  */

SystemErrNum_t
ResultsPrint::
PrintIqc( int index )
{
	SystemErrNum_t		errNum = SERR_NONE;
	ResultsDbEntry_t	entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		errNum = PrintIqc( &entry );
	}
// DB error if not found.

	return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintIqc( ResultsDbEntry_t * entry )
{
	char buff[ 2000 ];
	
	CreateIqcDisplay( entry, buff );

	return PrintReport( buff );
}

/*
 * FUNCTION
 * 	ResultsPrint::PrintRefRanges
 * 
 * DESCRIPTION
 * 	Prints reference ranges to the printer.
 *
 * PARAMETERS
 * 	sampleType - ID for the desired sample type, or STYPE_ALL to print all.
 */

SystemErrNum_t
ResultsPrint::
PrintRefRanges( SampleType_t sampleType )
{
	char			buff[ 2000 ];
	int				i, j;
	SystemErrNum_t	errNum = SERR_NONE;
	
	if ( sampleType == STYPE_ALL )
	{
		for ( i = 0; i < STYPE_NUM_SPECIES && errNum == SERR_NONE; i++ )
		{
			if ( refRanges->isDefined[ i ] )
			{
				// All three genders for piccolo if not a control.
				if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
		 			 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
				{
					for ( j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++ )
					{
						CreateRefRangesDisplay( (SampleType_t)i, (Gender_t)j, buff );
						errNum = PrintReport( buff );
					}
				}
				else
				{
					CreateRefRangesDisplay( (SampleType_t)i, GENDER_UNKNOWN, buff );
					errNum = PrintReport( buff );
				}
			}
		}
	}
	else
	{
		// All three genders for piccolo if not a control.
		if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
 			 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
		{
			for ( j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++ )
			{
				CreateRefRangesDisplay( sampleType, (Gender_t)j, buff );
				errNum = PrintReport( buff );
			}
		}
		else
		{
			CreateRefRangesDisplay( sampleType, GENDER_UNKNOWN, buff );
			errNum = PrintReport( buff );
		}
	}

	return errNum;
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateHeaderDisplay
 * 
 * DESCRIPTION
 * 	Creates the header for use by the other display routines.  Formats the
 * 	data for display on the front panel.  Also used to format the display for
 * 	the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the buffer.
 */

char *
ResultsPrint::
CreateHeaderDisplay( int index, char * buff )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		return CreateHeaderDisplay( &entry, buff );
	}
	
	return 0;
}

char *
ResultsPrint::
CreateHeaderDisplay( ResultsDbEntry_t * entry, char * buff )
{
	char *			pBuff = buff;
	StringId_t		sid;
	const char *	str;
	int				startPad, endPad;

// GLD		
// System-dependent
	pBuff = CenterText( strings[ STR_PRODUCT_NAME ], pBuff );
		
	if ( systemData->inDemoMode )
	{
		pBuff = CenterText( strings[ RSTR_DEMONSTRATION_SOFTWARE ], pBuff );
	}
	else if ( systemData->inResearchMode )
	{
		sprintf( pBuff, "%-21.21s%9s\r\n", strings[ RSTR_RESEARCH_SOFTWARE ], version );
		pBuff += strlen( pBuff );
	}

	pBuff = CenterText( entry->rotorName, pBuff );
	
	sprintf( pBuff, "%s           %s\r\n", GetDate( &entry->dateTime ), GetTime( &entry->dateTime ) );
	pBuff += strlen( pBuff );

	sprintf( pBuff, "%-13.13s%17.17s\r\n", strings[ RSTR_SAMPLE_TYPE ], species[ (SampleType_t)entry->sampleType ] );
	pBuff += strlen( pBuff );
	
	if ( entry->controlExpired == true )
	{
		str = strings[ RSTR_CONTROL_RENEWAL ];
		startPad = (30 - strlen( str )) / 2;
		endPad = 30 - startPad - strlen( str );
		sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
		pBuff += strlen( pBuff );
	}

	if ( entry->sampleType >= STYPE_CONTROL && entry->sampleType <= STYPE_LAST_CONTROL )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_CONTROL_ID ], entry->patientControlId );
	}
	else
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_PATIENT_ID ], entry->patientControlId );
	}
	pBuff += strlen( pBuff );

	if ( entry->alternateId[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_ALTERNATE_ID ], entry->alternateId );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->sampleId[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_SAMPLE_ID ], entry->sampleId );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->doctorId[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_DOCTOR_ID ], entry->doctorId );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->location[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_LOCATION ], entry->location );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->phoneNumber[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_PHONE_NUM ], entry->phoneNumber );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->admissionOwnerId[0] != '\0' )
	{
		const char * prompt;
		
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
			prompt = strings[ RSTR_ADMISSION_ID ];
		else
			prompt = strings[ RSTR_OWNER_ID ];
		sprintf( pBuff, "%-19.19s%11.11s\r\n", prompt, entry->admissionOwnerId );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->dateOfBirth.year != 0 )
	{
		sprintf( pBuff, "%-19.19s%11.11s\r\n", strings[ RSTR_DOB ], GetDate( &entry->dateOfBirth ) );
		pBuff += strlen( pBuff );
	}
	
	if ( (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) && (entry->lastVaccination.year != 0) )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_DOV ], GetDate( &entry->lastVaccination ) );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->gender != GENDER_NONE )
	{
		switch ( entry->gender )
		{
			case GENDER_FEMALE:		sid = GSTR_FEMALE;		break;
			case GENDER_MALE:		sid = GSTR_MALE;		break;
			case GENDER_SPAYED:		sid = GSTR_SPAYED;		break;
			case GENDER_NEUTERED:	sid = GSTR_NEUTERED;	break;
			case GENDER_UNKNOWN:	sid = GSTR_UNKNOWN;		break;
			default:				sid = GSTR_UNKNOWN;		break;
		}
		
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_GENDER ], strings[ sid ] );
		pBuff += strlen( pBuff );
	}

	if ( entry->age > 0 )
	{
		char ageBuff[20];
		
		if ( entry->age == 1 )
		{
			switch ( entry->ageUnits )
			{
				case AGE_UNITS_DAYS:	sid = RSTR_DAY;		break;
				case AGE_UNITS_WEEKS:	sid = RSTR_WEEK;	break;
				case AGE_UNITS_MONTHS:	sid = RSTR_MONTH;	break;
				case AGE_UNITS_YEARS:	sid = RSTR_YEAR;	break;
				default:				sid = RSTR_YEAR;	break;
			}
		}
		else
		{
			switch ( entry->ageUnits )
			{
				case AGE_UNITS_DAYS:	sid = RSTR_DAYS;	break;
				case AGE_UNITS_WEEKS:	sid = RSTR_WEEKS;	break;
				case AGE_UNITS_MONTHS:	sid = RSTR_MONTHS;	break;
				case AGE_UNITS_YEARS:	sid = RSTR_YEARS;	break;
				default:				sid = RSTR_YEARS;	break;
			}
		}
		
		sprintf( ageBuff, "%d %s", entry->age, strings[ sid ] );
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_AGE ], ageBuff );
		pBuff += strlen( pBuff );
	}
	
	if ( entry->race != RACE_NONE )
	{
		switch ( entry->race )
		{
			case RACE_UNKNOWN:					str = strings[ RSTR_UNKNOWN ];					break;
			case RACE_WHITE:					str = strings[ RSTR_WHITE ];					break;
			case RACE_BLACK:					str = strings[ RSTR_BLACK ];					break;
			case RACE_ASIAN_PACIFIC_ISLANDER:	str = strings[ RSTR_ASIAN_PACIFIC_ISLANDER ];	break;
			case RACE_NATIVE_AMERICAN:			str = strings[ RSTR_NATIVE_AMERICAN ];			break;
			case RACE_HISPANIC:					str = strings[ RSTR_HISPANIC ];					break;
			default:							str = strings[ RSTR_UNKNOWN ];					break;
		}

		sprintf( pBuff, "%-8.8s%22.22s\r\n", strings[ RSTR_RACE ], str );
		pBuff += strlen( pBuff );
	}

	if ( entry->operatorId[0] != '\0' )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_OPERATOR_ID ], entry->operatorId );
		pBuff += strlen( pBuff );
	}

	if ( entry->operatorUnauthorized == true )
	{
		str = strings[ RSTR_UNAUTHORIZED_OPERATOR ];
		startPad = (30 - strlen( str )) / 2;
		endPad = 30 - startPad - strlen( str );
		sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
		pBuff += strlen( pBuff );
	}

	if ( entry->operatorExpired == true )
	{
		str = strings[ RSTR_OPERATOR_RENEWAL ];
		startPad = (30 - strlen( str )) / 2;
		endPad = 30 - startPad - strlen( str );
		sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
		pBuff += strlen( pBuff );
	}

	sprintf( pBuff, "%-23.23s%7.7s\r\n", strings[ RSTR_ROTOR_LOT_NUM ], entry->lotNumber );
	pBuff += strlen( pBuff );

	sprintf( pBuff, "%-20.20s%10.10s\r\n", strings[ RSTR_SERIAL_NUM ], factoryData->instrumentSerialNumber );
	pBuff += strlen( pBuff );
		
	sprintf( pBuff, "..............................\r\n" );
	pBuff += strlen( pBuff );
	
	return pBuff;
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateRefRangesHeaderDisplay
 * 
 * DESCRIPTION
 * 	Creates the header for a reference ranges listing.  Formats the
 * 	data for display on the front panel.  Also used to format the display for
 * 	the printer.
 *
 * PARAMETERS
 * 	sampleType - ID to indicate the desired species.
 *  genter - Gender for piccolo xpress (male/female/unknown).
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the  buffer.
 */

char *
ResultsPrint::
CreateRefRangesHeaderDisplay( SampleType_t sampleType, Gender_t gender, char * buff )
{
	char *	pBuff = buff;
	time_t	currentTime = time( 0 );

	pBuff = CenterText( strings[ STR_PRODUCT_NAME ], pBuff );
		
	if ( systemData->inDemoMode )
	{
		pBuff = CenterText( strings[ RSTR_DEMONSTRATION_SOFTWARE ], pBuff );
	}
	else if ( systemData->inResearchMode )
	{
		pBuff = CenterText( strings[ RSTR_RESEARCH_SOFTWARE ], pBuff );
	}

	sprintf( pBuff, "%s           %s\r\n", GetDate( &currentTime ), GetTime( &currentTime ) );
	pBuff += strlen( pBuff );

	sprintf( pBuff, "%-13.13s%17.17s\r\n", strings[ RSTR_RANGE_LABEL ], species[ sampleType ] );
	pBuff += strlen( pBuff );
		
	// Gender entry only if piccolo xpress and not a control.
	if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
		 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
	{
		sprintf( pBuff, "%-16.16s%14.14s\r\n", strings[ RSTR_GENDER ], strings.Gender( gender ) );
		pBuff += strlen( pBuff );
	}
	
	sprintf( pBuff, "%-20.20s%10.10s\r\n", strings[ RSTR_SERIAL_NUM ], factoryData->instrumentSerialNumber );
	pBuff += strlen( pBuff );
		
	sprintf( pBuff, "..............................\r\n" );
	pBuff += strlen( pBuff );
	
	return pBuff;
}

/*
 * FUNCTION
 * 	ResultsPrint::CenterText
 * 
 * DESCRIPTION
 * 	Centers a string in a field 30 characters wide, with a CR/LF appended to the
 * 	end (CR/LF not included in the 30 characters).
 *
 * PARAMETERS
 * 	str - String to center.
 * 	buff - Buffer to accept the field.
 * 
 * RETURNS
 * 	Pointer to the next position in the buffer after the added text.
 */

char *
ResultsPrint::
CenterText( const char * str, char * buff )
{
	const char	blanks[] = "                              ";
	
	// If the string is too long, just copy it to the buffer.
	if ( strlen( str ) >= 30 )
	{
		strcpy( buff, str );
	}
	else
	{
		strcpy( buff, blanks );
		memcpy( buff + (30 - strlen( str )) / 2, str, strlen( str ) );
	}
	
	strcat( buff, "\r\n" );
	
	return buff + strlen( buff );
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateResultDisplay
 * 
 * DESCRIPTION
 * 	Creates a display string for the indicated sample result.  Formats the
 * 	data for display on the front panel.  Also used to format the display for
 * 	the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateResultDisplay( int index, char * buff )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateResultDisplay( &entry, buff );
	}
}

void
ResultsPrint::
CreateResultDisplay( ResultsDbEntry_t * entry, char * buff )
{
	char *				pBuff = buff;
	static SystemError	systemError;
	AnalyteResultType_t	analyteIndex;
	
	// Header text.
	pBuff = CreateHeaderDisplay( entry, pBuff );

	// Results text.
	for ( int i = 1; i <= MAX_ROTOR_RESULTS; i++ )
	{
		for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
		{
			if ( entry->rotorAnalyteResults[ j ].printOrder == i )
			{
				analyteIndex = (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType;
				AnalyteId_t	analyteId = analytes.GetAnalyteId( analyteIndex );

				sprintf( pBuff, "%-5.5s %s %s %6.6s\r\n",
					analytes[ analyteId ],
					FormatResult( entry, j, FMT_NORMAL ),
					FormatRefRange( entry, j ),
					units.String( analyteId ) );

				pBuff += strlen( pBuff );
				break;
			}
		}
	}
	
	// Line of blanks after the results.
	sprintf( pBuff, "%30s\r\n", " " );
	pBuff += strlen( pBuff );	

	// Footer
	sprintf( pBuff, "%-30.30s\r\n", strings[ RSTR_QC_OK ] );
	pBuff += strlen( pBuff );
		
	sprintf( pBuff, "HEM %-4.4s  LIP %-4.4s  ICT %-4.4s  \r\n",
		GetEndogenousString( entry->rotorInformationResults.rotorHemolyzedIndex, hemLimits ),
		GetEndogenousString( entry->rotorInformationResults.rotorLipemicIndex, lipLimits ),
		GetEndogenousString( entry->rotorInformationResults.rotorIctericIndex, ictLimits ) );
	pBuff += strlen( pBuff );
	
	// Warning line, only if Piccolo.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		if ( (entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0)
			 && (systemError.GetAction( entry->errNum ) != SERR_ACTION_ABORT) )
		{
			sprintf( pBuff, "%-30.30s\r\n", strings[ RSTR_CONFIRM_LOW_RECOVERIES ] );
			pBuff += strlen( pBuff );	
		}
		else
		{
			sprintf( pBuff, "%30s\r\n", " " );
			pBuff += strlen( pBuff );	
		}
	}

	// CR/LF pair to signal the end of the report.
	strcpy( pBuff, "\r\n" );
	pBuff += strlen( pBuff );	
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateErrorReportDisplay
 * 
 * DESCRIPTION
 * 	Creates an error report display string for the indicated test.  Formats
 *	the data for display on the front panel.  Also used to format the display
 *	for the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 * 	buff - Buffer large enough to hold the result string.
 * 	abbreviated - If true, omit the results.
 * 
 * RETURNS
 * 	Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateErrorReportDisplay( int index, char * buff, bool abbreviated )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateErrorReportDisplay( &entry, buff, abbreviated );
	}
}

void
ResultsPrint::
FormatCounter( char * str, uint32_t value )
{
	char	buff[20];
	int		i;
	
	sprintf( buff, "%08X", ~value + 1 );
	for ( i = 0; i < 4; i++ )
	{
		str[ i ] = buff[ 7 - i ];
	}
	str[ 4 ] = ' ';
	str[ 5 ] = ' ';
	for ( i = 4; i < 8; i++ )
	{
		str[ i + 2 ] = buff[ 7 - i ];
	}
	str[ 10 ] = '\0';
}

void
ResultsPrint::
CreateErrorReportDisplay( ResultsDbEntry_t * entry, char * buff, bool abbreviated )
{
	char *				pBuff = buff;
	char				cBuffA[20];
	char				cBuffB[20];
	unsigned			flags;
	AnalyteId_t			analyteId;
	int 				endogenousIndex;
	ResultFormatType_t	formatType;
	static SystemError	systemError;
	AnalyteResultType_t	analyteIndex;

	// Header.
	pBuff = CreateHeaderDisplay( entry, pBuff );

	// Flags.
	FormatCounter( cBuffA, counterData->rotorRunCount );
	FormatCounter( cBuffB, counterData->rotorAbortCount );
	sprintf( pBuff, "01 %s  %s     \r\n", cBuffA, cBuffB );
	pBuff += strlen( pBuff );

	FormatCounter( cBuffA, counterData->printCount );
	FormatCounter( cBuffB, counterData->flashCount );
	sprintf( pBuff, "02 %s  %s     \r\n", cBuffA, cBuffB );
	pBuff += strlen( pBuff );

	sprintf( pBuff, "03 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		entry->rotorInformationResults.rotorDacTrimFlags,
		entry->rotorInformationResults.rotorOffsetErrorFlags,
		entry->rotorInformationResults.rotorOffsetSdErrorFlags,
		entry->rotorInformationResults.rotorWavelengthCvFlags );
	pBuff += strlen( pBuff );

	uint32_t bc1 = entry->rotorInformationResults.rotorBeadCheck1Flags;
	uint32_t bc2 = entry->rotorInformationResults.rotorBeadCheck2Flags;
	sprintf( pBuff, "04 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		bc1 >> 16, bc1 & 0xFFFF, bc2 >> 16, bc2 & 0xFFFF );
	pBuff += strlen( pBuff );

	uint32_t dc = entry->rotorInformationResults.rotorDistributionCheckFlags;
	uint32_t sf = entry->rotorInformationResults.rotorSystemFlags;
	sprintf( pBuff, "05 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		dc >> 16, dc & 0xFFFF, sf >> 16, sf & 0xFFFF );
	pBuff += strlen( pBuff );

	sprintf( pBuff, "06 %4.4X%23s\r\n",
		entry->rotorInformationResults.rotorQcFlags,
		" " );
	pBuff += strlen( pBuff );

	// Results.
	if ( !abbreviated )
	{
		for ( int i = 1; i <= MAX_ROTOR_RESULTS; i++ )
		{
			for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
			{
				if ( entry->rotorAnalyteResults[ j ].printOrder == i )
				{
					if ( systemData->inDemoMode )
						flags = entry->rotorAnalyteResults[ j ].resultPrintFlags;
					else
						flags = entry->rotorAnalyteResults[ j ].analyteFlags;

					analyteIndex = (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType;

					analyteId = analytes.GetAnalyteId( analyteIndex );

					// Result format depends on whether we're in research mode or not.
					formatType = systemData->inResearchMode ? FMT_ERROR_RESEARCH : FMT_ERROR;

					sprintf( pBuff, "%-5.5s %s %4.4X %4.4X %6.6s\r\n",
						analytes[ analyteId ],
						FormatResult( entry, j, formatType ),
						flags >> 16, flags & 0xFFFF,
						units.String( analyteId ) );
					pBuff += strlen( pBuff );
				}
			}
		}
	}

	// Line of blanks after the results.
	sprintf( pBuff, "%30s\r\n", " " );
	pBuff += strlen( pBuff );	

	/*
	 * Footer.
	 */
// GLD - Start of issue #71 change. If removed, change CalcResults initialization, too.
	// RQC result.
	if ( entry->rotorAnalyteResults[ RQC ].analyteResult < 0 )
	{
		sprintf( pBuff, "RQC: ~~~%22s\r\n", " " );
	}
	else
	{
		int rqcResult = (int)round( entry->rotorAnalyteResults[ RQC ].analyteResult );
		sprintf( pBuff, "RQC: %3d%22s\r\n", rqcResult, " " );
	}
	pBuff += strlen( pBuff );

	// Endogenous values.

	// HEM
	if ( entry->rotorInformationResults.rotorHemolyzedIndex < -999.0 )
	{
		sprintf( pBuff, "HEM ~~~   " );
	}
	else
	{
		if ( entry->rotorInformationResults.rotorHemolyzedIndex >= 0 )
		{
			endogenousIndex = (int)round( entry->rotorInformationResults.rotorHemolyzedIndex );
		}
		else
		{
			endogenousIndex = (int)round(entry->rotorInformationResults.rotorHemolyzedIndex );
		}
		sprintf( pBuff, "HEM %-4d  ", endogenousIndex );
	}
	pBuff += strlen( pBuff );
	
	// LIP
	if ( entry->rotorInformationResults.rotorLipemicIndex < -999.0 )
	{
		sprintf( pBuff, "LIP ~~~   " );
	}
	else
	{
		endogenousIndex = (int)round( entry->rotorInformationResults.rotorLipemicIndex );
		sprintf( pBuff, "LIP %-4d  ", endogenousIndex );
	}
	pBuff += strlen( pBuff );
	
	// ICT
	if ( entry->rotorInformationResults.rotorIctericIndex < -999.0 )
	{
		sprintf( pBuff, "ICT ~~~   \r\n" );
	}
	else
	{
		endogenousIndex = (int)round( entry->rotorInformationResults.rotorIctericIndex );
		sprintf( pBuff, "ICT %-4d  \r\n", endogenousIndex );
	}
	pBuff += strlen( pBuff );

	// Warning line, only if Piccolo.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		if ( (entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0)
			 && (systemError.GetAction( entry->errNum ) != SERR_ACTION_ABORT) )
		{
			sprintf( pBuff, "%-30.30s\r\n", strings[ RSTR_CONFIRM_LOW_RECOVERIES ] );
			pBuff += strlen( pBuff );	
		}
		else
		{
			sprintf( pBuff, "%30s\r\n", " " );
			pBuff += strlen( pBuff );	
		}
	}

	// Software version.
	sprintf( pBuff, "%-21.21s%9s\r\n", strings[ MSTR_VERSION ], version );
	pBuff += strlen( pBuff );

	// Error text.
	if ( entry->errNum != SERR_NONE )
	{
		SystemError	systemError;

		sprintf( pBuff, "%04X %25s\r\n", entry->errNum, strings[ systemError.GetScreenText( entry->errNum ) ] );
		pBuff += strlen( pBuff );
	}

	// CR/LF pair to signal the end of the report.
	strcpy( pBuff, "\r\n" );
	pBuff += strlen( pBuff );	
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateIqcDisplay
 * 
 * DESCRIPTION
 * 	Creates an IQC display string for the indicated test.  Formats the
 * 	data for display on the front panel.  Also used to format the display for
 * 	the printer.
 *
 * PARAMETERS
 * 	index - Index of the desired result in the results database.
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateIqcDisplay( int index, char * buff )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateIqcDisplay( &entry, buff );
	}
}

void
ResultsPrint::
CreateIqcDisplay( ResultsDbEntry_t * entry, char * buff )
{
	char *			pBuff = buff;
	const char *	wavelength[] = { "340", "405", "467", "500", "515", "550", "600", "630" };

	// Header text.
	pBuff = CreateHeaderDisplay( entry, pBuff );
		
	// Body text.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		sprintf( pBuff, "%-27s%3d\r\n",
			strings[ RSTR_INSTRUMENT_QC ],
			(int)round( entry->rotorInformationResults.rotorIqcRatioPrecision * 100 ) );
		pBuff += strlen( pBuff );

		for ( int i = 0; i < 8; i++ )
		{
			sprintf( pBuff, "iQC %d:        %3d  %s nm: %3d\r\n",
				i+1,
				(int)round( entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100 ),
				wavelength[ i ],
				(int)round( entry->rotorInformationResults.rotorIqcLevel2Results[ i ] * 100 ) );
			pBuff += strlen( pBuff );
		}

		// Range.
		sprintf( pBuff, "%-11s90-110       95-105\r\n", strings[ RSTR_RANGE ] );
		pBuff += strlen( pBuff );
	}
	
	else
	{
		for ( int i = 0; i < 8; i++ )
		{
			sprintf( pBuff, "iQC %d:        %3d%13s\r\n",
				i+1,
				(int)round( entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100 ),
				" " );
			pBuff += strlen( pBuff );
		}

		// Range.
		sprintf( pBuff, "%-11s90-110             \r\n", strings[ RSTR_RANGE ] );
		pBuff += strlen( pBuff );

	}
	
	// Space row.
	strcpy( pBuff, "\r\n" );
	pBuff += strlen( pBuff );
	
	// Footer text.
	sprintf( pBuff, "%-27s%3d\r\n",
		strings[ RSTR_CHEMISTRY_QC ],
		(int)round( entry->rotorAnalyteResults[ RQC ].analyteResult ) );
	pBuff += strlen( pBuff );
// GLD
	sprintf( pBuff, "%-27s%3d\r\n", strings[ RSTR_ACCEPTABLE_MINIMUM ], RQC_ACCEPTABLE_MINIMUM );
//	sprintf( pBuff, "%-27s%3d\r\n", strings[ RSTR_ACCEPTABLE_MINIMUM ], entry->rotorInformationResults.rotorRqcLowLimit );
	pBuff += strlen( pBuff );
	
	// CR/LF pair to signal the end of the report.
	strcpy( pBuff, "\r\n" );
	pBuff += strlen( pBuff );	
}

/*
 * FUNCTION
 * 	ResultsPrint::CreateRefRangesDisplay
 * 
 * DESCRIPTION
 * 	Creates a reference ranges display for the indicated sample type.  Formats
 *	the data for display on the printer.
 * 
 * 	NOTE: This does *not* do ALL sample types.  It must be called iteratively
 * 		  for each sample type if the user selected print all.
 *
 * PARAMETERS
 * 	sampleType - Sample ID for the desired species.
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the buffer.
 */

void
ResultsPrint::
CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, char * buff )
{
	char *					pBuff = buff;
	int						i;
	AnalyteId_t				analyteId;
	float					low, high;
	const AnalyteList_t *	analyteList;
	
	if ( sampleType != STYPE_ALL )
	{
		// Header text.
		pBuff = CreateRefRangesHeaderDisplay( sampleType, gender, pBuff );
			
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
		{
			analyteList = analytes.PiccoloAnalytesWithRefRanges();
		}
		else
		{
			analyteList = analytes.VetScanAnalytesWithRefRanges();
		}

		for ( i = 0; i < analyteList->len; i++ )
		{
			analyteId = analyteList->list[ i ];
			low = refRanges->ranges[ sampleType ][ analyteId ][ gender-1 ].low;
			high = refRanges->ranges[ sampleType ][ analyteId ][ gender-1 ].high;
			
			sprintf( pBuff, "%-5.5s %s %6.6s        \r\n",
						analytes[ analyteId ],
						FormatRefRange( analyteId, low, high ),
						units.String( analyteId ) );
			pBuff += strlen( pBuff );
		}

		// CR/LF pair to signal the end of the report.
		strcpy( pBuff, "\r\n" );
		pBuff += strlen( pBuff );	
	}
}

bool
ResultsPrint::
GetResult( ResultsDbEntry_t * entry, int index )
{
	bool 			retVal = false;
	const char *	dbData;
	
	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;
	
	int fd = open( dbData, O_RDONLY, 0 );
	if ( fd != -1 )
	{
		if ( lseek( fd, index * sizeof( ResultsDbEntry_t ), 0 ) != -1 )
		{
			int bytesRead = read( fd, (char *)entry, sizeof( ResultsDbEntry_t ) );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )
			{
				retVal = true;
			}
		}
		close( fd );
	}
	else
	{
		LogError( "failed to open %s", dbData );
	}

	return retVal;
}	

char *
ResultsPrint::
GetDate( time_t * time )
{
	static char dateBuff[15];
	struct tm * tm = gmtime( time );
	
	sprintf( dateBuff, "%02d %s %04d", tm->tm_mday, strings.Month( tm->tm_mon ), tm->tm_year + 1900 );

	return dateBuff;
}

char *
ResultsPrint::
GetDate( Date_t * date )
{
	static char dateBuff[15];

	sprintf( dateBuff, "%02d %s %04d", date->day, strings.Month( date->month), date->year );
	
	return dateBuff;
}

char *
ResultsPrint::
GetTime( time_t * time )
{
	static char		timeBuff[10];
	struct tm *		tm = gmtime( time );
	int				hour = tm->tm_hour;
	const char *	amPm;
	
	if ( settings->time24Format )
	{
		sprintf( timeBuff, "   %02d:%02d", tm->tm_hour, tm->tm_min );
	}
	else
	{
		amPm = hour < 12 ? "AM" : "PM";
		
		if ( hour == 0 )
		{
			hour = 12;
		}
		else if ( hour > 12 )
		{
			hour -= 12;
		}
	
		sprintf( timeBuff, "%02d:%02d %s", hour, tm->tm_min, amPm );
	}
	
	return timeBuff;
}


SystemErrNum_t
ResultsPrint::
PrintReport( char * resultText )
{
	time_t	currentTime;
	int		elapsedTime;
	char *	s;

	// Translate ISO-8859-1 characters to printer characters as necessary.
	PrinterTranslate( resultText );

	if ( settings->defaultPrinter == INTERNAL_PTR )
	{
		if ( fdSer == -1 )
		{
			InitInternalPrinterPort();
			if ( fdSer == -1 )
			{
// GLD
// Different error?
				return SERR_NONE;
			}
		}

		// If the screen and keyboard languages are both English, we print
		// all uppercase to the internal printer.
		if ( (settings->language == LANG_ENGLISH) && (settings->keyboard == LANG_ENGLISH) )
		{
			strupr( resultText );
		
			// We've got an exception - we leave nHDLc in mixed case. So, just convert the appropriate
			// characters back to lower case. Do as a while loop in case there are multiple occurrences.
			while ( (s = strstr( resultText, "NHDLC" )) != 0 )
			{
				s[0] = 'n';
				s[4] = 'c';
			}
		}
			
		// Get the current time.  If it's been less than 5 seconds since we
		// printed the last report, add a delay until we hit the 5 seconds.
		// We can get a negative number if the date/time was changed between
		// reports, so we also make sure the elapsed time is greater than 0.
		currentTime = time( 0 );
		elapsedTime = (int)difftime( currentTime, lastReportTime );
		if ( (elapsedTime >= 0) && (elapsedTime < 5) )
		{
			sleep( 5 - elapsedTime );
		}
		lastReportTime = currentTime;

		WriteSer( resultText, strlen( resultText ) );
		WriteSer( "\n\n\n", 3 );
	
		counterData->printCount++;
		WriteCounters( counterData );
	}
	
	else
	{
		char *	s;

		if ( fdUsb == -1 )
		{
			InitExternalPrinterPort();
			if ( fdUsb == -1 )
			{
				return SERR_EXT_PTR_NOT_FOUND;
			}
		}

		// Break the text into individual lines and print them with leading blanks.
		s = strtok( resultText, "\r\n" );
		while ( s != 0 )
		{
			write( fdUsb, "                         ", 25 );
			write( fdUsb, s, strlen( s ) );
			write( fdUsb, "\r\n", 2 );
			
			s = strtok( 0, "\r\n" );
		}	

		// Finish with a formfeed.
		write( fdUsb, "\f", 1 );
	}
	
	return SERR_NONE;
}

void
ResultsPrint::
InitInternalPrinterPort()
{
	struct termios	term;

	// Open the serial port.
	fdSer = open( INTERNAL_PRINTER_PORT, O_RDWR | O_NONBLOCK, 0 );
	if ( fdSer == -1 )
	{
		LogError( "failed to open internal printer port: " INTERNAL_PRINTER_PORT );
		return;
	}
	
	// Set the serial port's baud rate and handshaking.
// GLD
// These will all depend on whether we're using the Seiko or the Alps printer.
// This configuration is for the Seiko.
	tcgetattr( fdSer, &term );
	term.c_ispeed = INTERNAL_PRINTER_BAUD_RATE;
	term.c_ospeed = INTERNAL_PRINTER_BAUD_RATE;
	term.c_iflag |= (IXOFF | IXON);		// Enable Xon/Xoff
	term.c_cflag &= ~(IHFLOW | OHFLOW);	// Disable RTS/CTS.
	tcsetattr( fdSer, TCSANOW, &term );

// GLD
// We'll need to figure out if the printer is there, at least if we're using
// H/W handshaking (it seems to just dump fine w/ S/W handshaking if the printer
// is off).

// GLD
// This is printer-specific, too.
	// Set to 16-dot font size.
	WriteSer( "\x12\x46\x00", 3 );
	
// GLD
	// Set to maximum intensity (140%)
	WriteSer( "\x12\x7E\x8C", 3 );
	
	// Turn on error reporting, so we know when we're out of paper.
	WriteSer( "\x12\x65\x01", 3 );
}

void
ResultsPrint::
InitExternalPrinterPort()
{
	// Open the printer port.
	fdUsb = open( EXTERNAL_PRINTER_PORT, O_RDWR, 0 );

	// If the open fails, we restart the USB printer driver and try again.
	if ( fdUsb == -1 )
	{
		int pid;

		// Slay the driver.
		pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "devu-prn",  0 );
		if ( pid == -1 )
		{
			LogError( "failed to run 'slay devu-prn'" );
			return;
		}
	
		// Restart the driver.
		pid = spawnlp( P_WAIT, "devu-prn", "devu-prn", 0 );
		if ( pid == -1 )
		{
			LogError( "failed to start devu-prn" );
		}

		// Wait for the driver to start up.
		spawnlp( P_WAIT, "waitfor", "waitfor", EXTERNAL_PRINTER_PORT, "5", 0 );
		
		// Try again to open the printer port.
		fdUsb = open( EXTERNAL_PRINTER_PORT, O_RDWR, 0 );
		if ( fdUsb == -1 )
		{
			LogError( "failed to open external printer port %s", EXTERNAL_PRINTER_PORT );
		}
	}
}

const char *
ResultsPrint::
GetEndogenousString( float value, const float limits[] )
{
	if ( value < limits[0] )
	{
		return endogenousStrings[0];
	}
	else if ( value < limits[1] )
	{
		return endogenousStrings[1];
	}
	else if ( value < limits[2] )
	{
		return endogenousStrings[2];
	}
	else
	{
		return endogenousStrings[3];
	}
}

void
ResultsPrint::
PrinterTranslate( char * text )
{
	const char xlate[] = {
		0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA8,
		0x20, 0x20, 0x20, 0x20, 0x8E, 0x8F, 0x92, 0x20, 0x20, 0x90, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
		0x20, 0xA5, 0x20, 0x20, 0x20, 0x20, 0x99, 0x20, 0x20, 0x20, 0x20, 0x20, 0x9A, 0x20, 0x20, 0xE1,
		0x85, 0xA0, 0x83, 0x20, 0x84, 0x86, 0x91, 0x87, 0x8A, 0x82, 0x88, 0x89, 0x8D, 0xA1, 0x8C, 0x8B,
		0x20, 0xA4, 0x95, 0xA2, 0x93, 0x20, 0x94, 0x20, 0x20, 0x97, 0xA3, 0x96, 0x81, 0x20, 0x20, 0x98
	};

	for ( unsigned i = 0; i < strlen( text ); i++ )
	{
		if ( (unsigned char)(text[ i ]) >= 0xB0 )
		{
			text[ i ] = xlate[ (unsigned char)text[ i ] - 0xB0 ];
		}
	}
}
