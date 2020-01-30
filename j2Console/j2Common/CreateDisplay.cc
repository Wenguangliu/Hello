#include <limits.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <wchar.h>

#include "CreateDisplay.h"
#include "ResultsDb.h"
#include "Shmem.h"
#include "LogErr.h"
#include "Species.h"
#include "Analytes.h"
#include "Units.h"
#include "ParseCounters.h"
#include "SystemError.h"
#include "FormatResults.h"


const wchar_t *		endogenousStrings[] = { L"0", L"1+", L"2+", L"3+" };
const float			hemLimits[] = { 20.0, 45.0, 181.0 };
const float			lipLimits[] = { 150.0, 221.0, 301.0 };
const float			ictLimits[] = { 2.8, 6.0, 12.1 };

const wchar_t *		wavelengthStrings[] = { L"340", L"405", L"467", L"500", L"515", L"550", L"600", L"630" };

ULONGLONG			CreateDisplay::lastReportTime = 0;
//AStrings			CreateDisplay::strings;
Analytes			CreateDisplay::analytes;
Units				CreateDisplay::units;
Species				CreateDisplay::species;


CounterData_t *		CreateDisplay::counterData	= (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
FactoryData_t *		CreateDisplay::factoryData	= (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SettingsData_t *	CreateDisplay::settings		= (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
SystemData_t *		CreateDisplay::systemData	= (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
RefRangeData_t *	CreateDisplay::refRanges	= (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );
char 		*		CreateDisplay::version	= (char *)MapSharedMem( VERSION_SHMEM_NAME, MAX_VERSION_LEN + 1 );



/*
 * FUNCTION
 * 	CreateDisplay::CreateHeaderDisplay
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

wchar_t *
CreateDisplay::
CreateHeaderDisplay( int index, wchar_t * buff, AStrings *strings )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		return CreateHeaderDisplay( &entry, buff, strings );
	}
	
	return 0;
}

wchar_t *
CreateDisplay::
CreateHeaderDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings )
{
	wchar_t *		pBuff = buff;
	StringId_t		sid;
	const wchar_t *	str;
	int				startPad, endPad;
    wchar_t         wBuff [128];

    // System-dependent
	pBuff = CenterText( strings->PrintString (STR_PRODUCT_NAME), pBuff );
		
	if ( systemData->inDemoMode )
	{
	    pBuff = CenterText (strings->PrintString (RSTR_DEMONSTRATION_SOFTWARE), pBuff);
	}
	else if ( systemData->inResearchMode )
	{
		wsprintf( pBuff, L"%-21.21s%9S\r\n", strings->PrintString ( RSTR_RESEARCH_SOFTWARE), version );
		pBuff += wcslen (pBuff);
	}

    mbstowcs (wBuff,  entry->rotorName, sizeof  (entry->rotorName));
	pBuff = CenterText( wBuff, pBuff );
	
	wsprintf (pBuff, L"%s           %s\r\n", GetDate( &entry->dateTime, strings ), GetTime( &entry->dateTime ) );
	pBuff += wcslen (pBuff);

	wsprintf( pBuff, L"%-13.13s%17.17s\r\n", strings->PrintString (RSTR_SAMPLE_TYPE), species.GetSampleTypePrintString ((SampleType_t)entry->sampleType, strings ));
	pBuff += wcslen( pBuff );
	
	if ( entry->controlExpired == true )
	{
		str = strings->PrintString (RSTR_CONTROL_RENEWAL);
		startPad = (30 - wcslen( str )) / 2;
		endPad = 30 - startPad - wcslen( str );
		wsprintf( pBuff, L"%*s%s%*s\r\n", startPad, L" ", str, endPad, L" " );
		pBuff += wcslen( pBuff );
	}

	if ( entry->sampleType >= STYPE_CONTROL && entry->sampleType <= STYPE_LAST_CONTROL )
	{
	    wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_CONTROL_ID), entry->patientControlId );
	}
	else
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_PATIENT_ID), entry->patientControlId );
	}
	pBuff += wcslen( pBuff );

	if ( entry->alternateId[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_ALTERNATE_ID), entry->alternateId );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->sampleId[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_SAMPLE_ID), entry->sampleId );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->doctorId[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_DOCTOR_ID), entry->doctorId );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->location[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_LOCATION), entry->location );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->phoneNumber[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_PHONE_NUM), entry->phoneNumber );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->admissionOwnerId[0] != '\0' )
	{
		const wchar_t * prompt;
		
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE ) {
			prompt = strings->PrintString ( RSTR_ADMISSION_ID);
        } else {
			prompt = strings->PrintString (RSTR_OWNER_ID);
        }
		wsprintf( pBuff, L"%-19.19s%11.11S\r\n", prompt, entry->admissionOwnerId );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->dateOfBirth.year != 0 )
	{
		wsprintf( pBuff, L"%-19.19s%11.11s\r\n", strings->PrintString (RSTR_DOB), GetDate( &entry->dateOfBirth, strings ) );
		pBuff += wcslen( pBuff );
	}
	
	if ( (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) && (entry->lastVaccination.year != 0) )
	{
		wsprintf( pBuff, L"%-16.16s%14.14s\r\n", strings->PrintString (RSTR_DOV), GetDate( &entry->lastVaccination, strings ) );
		pBuff += wcslen( pBuff );
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
		
		wsprintf( pBuff, L"%-16.16s%14.14s\r\n", strings->PrintString (RSTR_GENDER), strings->PrintString (sid) );
		pBuff += wcslen( pBuff );
	}

	if ( entry->age > 0 )
	{
		wchar_t ageBuff[20];
		
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
		
		wsprintf( ageBuff, L"%d %s", entry->age, strings->PrintString (sid));
		wsprintf( pBuff, L"%-16.16s%14.14s\r\n", strings->PrintString (RSTR_AGE), ageBuff );
		pBuff += wcslen( pBuff );
	}
	
	if ( entry->race != RACE_NONE )
	{
		switch ( entry->race )
		{
			case RACE_UNKNOWN:					
                str = strings->PrintString (RSTR_UNKNOWN);					
            break;
			case RACE_WHITE:					
                str = strings->PrintString (RSTR_WHITE);					    
            break;
			case RACE_BLACK:					
                str = strings->PrintString ( RSTR_BLACK);					    
            break;
			case RACE_ASIAN_PACIFIC_ISLANDER:	
                str = strings->PrintString ( RSTR_ASIAN_PACIFIC_ISLANDER);	
            break;
			case RACE_NATIVE_AMERICAN:			
                str = strings->PrintString (RSTR_NATIVE_AMERICAN );			
            break;
			case RACE_HISPANIC:					
                str = strings->PrintString (RSTR_HISPANIC);					
            break;
			default:							
                str = strings->PrintString (RSTR_UNKNOWN);					
            break;
		}
		wsprintf( pBuff, L"%-8.8s%22.22s\r\n", strings->PrintString (RSTR_RACE), str );
		pBuff += wcslen ( pBuff );
	}

	if ( entry->operatorId[0] != '\0' )
	{
		wsprintf( pBuff, L"%-16.16s%14.14S\r\n", strings->PrintString (RSTR_OPERATOR_ID), entry->operatorId );
		pBuff += wcslen ( pBuff );
	}

	if ( entry->operatorUnauthorized == true )
	{
		str = strings->PrintString (RSTR_UNAUTHORIZED_OPERATOR);
		startPad = (30 - wcslen ( str )) / 2;
		endPad = 30 - startPad - wcslen ( str );
		wsprintf( pBuff, L"%*s%s%*s\r\n", startPad, L" ", str, endPad, L" " );
		pBuff += wcslen ( pBuff );
	}

	if ( entry->operatorExpired == true )
	{
		str = strings->PrintString (RSTR_OPERATOR_RENEWAL);
		startPad = (30 - wcslen ( str )) / 2;
		endPad = 30 - startPad - wcslen ( str );
		wsprintf( pBuff, L"%*s%s%*s\r\n", startPad, L" ", str, endPad, L" " );
		pBuff += wcslen ( pBuff );
	}

	wsprintf( pBuff, L"%-23.23s%7.7S\r\n", strings->PrintString (RSTR_ROTOR_LOT_NUM), entry->lotNumber );
	pBuff += wcslen ( pBuff );
    
	wsprintf( pBuff, L"%-20.20s%10.10S\r\n", strings->PrintString (RSTR_SERIAL_NUM), factoryData->instrumentSerialNumber );
	pBuff += wcslen( pBuff );
		
	wsprintf( pBuff, L"..............................\r\n" );
	pBuff += wcslen ( pBuff );
	
	return pBuff;
}

/*
 * FUNCTION
 * 	CreateDisplay::CreateRefRangesHeaderDisplay
 * 
 * DESCRIPTION
 * 	Creates the header for a reference ranges listing.  Formats the
 * 	data for display on the front panel.  Also used to format the display for
 * 	the printer.
 *
 * PARAMETERS
 * 	//ampleType - ID to indicate the desired species.
 *  genter - Gender for piccolo xpress (male/female/unknown).
 * 	buff - Buffer large enough to hold the result string.
 * 
 * RETURNS
 * 	Pointer to the  buffer.
 */

wchar_t *
CreateDisplay::
CreateRefRangesHeaderDisplay( SampleType_t sampleType, Gender_t gender, wchar_t * buff, AStrings *strings)
{
	wchar_t *	pBuff = buff;
	FILETIME	currentTime;
	SYSTEMTIME	time;
	//GetCurrentFT(&currentTime);
	GetLocalTime(&time);
	SystemTimeToFileTime(&time, &currentTime);

	pBuff = CenterText( strings->PrintString (STR_PRODUCT_NAME), pBuff );
		
	if ( systemData->inDemoMode )
	{
	   pBuff = CenterText( strings->PrintString (RSTR_DEMONSTRATION_SOFTWARE), pBuff );
	}
	else if ( systemData->inResearchMode )
	{
		pBuff = CenterText( strings->PrintString (RSTR_RESEARCH_SOFTWARE), pBuff );
	}

	wsprintf( pBuff, L"%s           %s\r\n", GetDate( &currentTime, strings ), GetTime( &currentTime ) );
	pBuff += wcslen( pBuff );
	wsprintf( pBuff, L"%-13.13s%17.17s\r\n", strings->PrintString (RSTR_RANGE_LABEL), species.GetSampleTypePrintString (sampleType, strings) );
	pBuff += wcslen( pBuff );
		
	// Gender entry only if piccolo xpress and not a control.
	if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
		 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
	{
         
		wsprintf( pBuff, L"%-16.16s%14.14s\r\n", strings->PrintString (RSTR_GENDER), strings->PrintGender( gender ) );
        
		pBuff += wcslen( pBuff );
	}
	
	wsprintf( pBuff, L"%-20.20s%10.10S\r\n", strings->PrintString (RSTR_SERIAL_NUM), factoryData->instrumentSerialNumber );
	pBuff += wcslen ( pBuff );
		
	wsprintf( pBuff, L"..............................\r\n" );
	pBuff += wcslen( pBuff );
	
	return pBuff;
}

/*
 * FUNCTION
 * 	CreateDisplay::CenterText
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

wchar_t *
CreateDisplay::
CenterText( const wchar_t * str, wchar_t * buff )
{
	const wchar_t	blanks[] = L"                              ";
	
    // If the string is too long, just copy it to the buffer.
    if ( wcslen( str ) >= 30 ) {
	    wcscpy( buff, str );
    } else {
        wcscpy( buff, blanks );
	    wmemcpy( buff + (30 - wcslen( str )) / 2, str, wcslen( str ) );
	}
    wcscat( buff, L"\r\n\0" );
	
	return buff + wcslen( buff );
}

/*
 * FUNCTION
 * 	CreateDisplay::CreateResultDisplay
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
CreateDisplay::
CreateResultDisplay( int index, wchar_t * buff, AStrings *strings )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateResultDisplay( &entry, buff, strings );
	}
}

void
CreateDisplay::
CreateResultDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings)
{
	wchar_t *	pBuff = buff;
	
	// Header text.
	pBuff = CreateHeaderDisplay( entry, pBuff, strings );

	// Results text.
	for ( int i = 1; i <= MAX_ROTOR_RESULTS; i++ )
	{
		for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
		{
			if ( entry->rotorAnalyteResults[ j ].printOrder == i )
			{
				AnalyteId_t	analyteId = analytes.GetAnalyteId( (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType );

				wsprintf( pBuff, L"%-5.5s %S %S %6.6s\r\n",
					analytes.GetAnalysisString (analyteId, strings),
					FormatResult( entry, j, FMT_NORMAL, strings ),
					FormatRefRange( entry, j ),
					units.String( analyteId ) );

				pBuff += wcslen( pBuff );
				break;
			}
		}
	}
	
	// Line of blanks after the results.
	wsprintf( pBuff, L"%30s\r\n", L" " );
	pBuff += wcslen( pBuff );	

	// Footer
	wsprintf( pBuff, L"%-30.30s\r\n", strings->PrintString (RSTR_QC_OK) );
	pBuff += wcslen( pBuff );
		
	wsprintf( pBuff, L"HEM %-4.4s  LIP %-4.4s  ICT %-4.4s  \r\n",
		GetEndogenousString( entry->rotorInformationResults.rotorHemolyzedIndex, hemLimits ),
		GetEndogenousString( entry->rotorInformationResults.rotorLipemicIndex, lipLimits ),
		GetEndogenousString( entry->rotorInformationResults.rotorIctericIndex, ictLimits ) );
	pBuff += wcslen( pBuff );
	
	// Warning line, only if Piccolo.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		if ( entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0 )
		{
			wsprintf( pBuff, L"%-30.30s\r\n", strings->PrintString (RSTR_CONFIRM_LOW_RECOVERIES));
			pBuff += wcslen( pBuff );	
		}
		else
		{
			wsprintf( pBuff, L"%30s\r\n", " " );
			pBuff += wcslen( pBuff );	
		}
	}

	// CR/LF pair to signal the end of the report.
	wcscpy( pBuff, L"\r\n" );
	pBuff += wcslen( pBuff );	
}

/*
 * FUNCTION
 * 	CreateDisplay::CreateErrorReportDisplay
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
CreateDisplay::
CreateErrorReportDisplay( int index, wchar_t * buff, bool abbreviated, AStrings *strings)
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateErrorReportDisplay( &entry, buff, abbreviated, strings );
	}
}

void
CreateDisplay::
FormatCounter( wchar_t * str, UINT32 value )
{
	wchar_t	buff[20];
	int		i;
	
	wsprintf( buff, L"%08X", ~value + 1 );
	for ( i = 0; i < 4; i++ )
	{
		str[ i ] = buff[ 7 - i ];
	}
	str[ 4 ] = L' ';
	str[ 5 ] = L' ';
	for ( i = 4; i < 8; i++ )
	{
		str[ i + 2 ] = buff[ 7 - i ];
	}
	str[ 10 ] = L'\0';
}

void
CreateDisplay::
CreateErrorReportDisplay( ResultsDbEntry_t * entry, wchar_t * buff, bool abbreviated, AStrings *strings)
{
	wchar_t *			pBuff = buff;
	wchar_t				cBuffA[20];
	wchar_t				cBuffB[20];
    wchar_t             wBuff [512];
	unsigned			flags;
	AnalyteId_t			analyteId;
	int 				endogenousIndex;
	ResultFormatType_t	formatType;

	// Header.
	pBuff = CreateHeaderDisplay( entry, pBuff, strings );

	// Flags.
	FormatCounter( cBuffA, counterData->rotorRunCount );
    FormatCounter( cBuffB, counterData->rotorAbortCount );

	wsprintf( pBuff, L"01 %s  %s     \r\n", cBuffA, cBuffB );
	pBuff += wcslen( pBuff );

	FormatCounter( cBuffA, counterData->printCount );
	FormatCounter( cBuffB, counterData->flashCount );
	wsprintf( pBuff, L"02 %s  %s     \r\n", cBuffA, cBuffB );
	pBuff += wcslen( pBuff );

	wsprintf( pBuff, L"03 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		entry->rotorInformationResults.rotorDacTrimFlags,
		entry->rotorInformationResults.rotorOffsetErrorFlags,
		entry->rotorInformationResults.rotorOffsetSdErrorFlags,
		entry->rotorInformationResults.rotorWavelengthCvFlags );
	pBuff += wcslen( pBuff );

	UINT32 bc1 = entry->rotorInformationResults.rotorBeadCheck1Flags;
	UINT32 bc2 = entry->rotorInformationResults.rotorBeadCheck2Flags;
	wsprintf( pBuff, L"04 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		bc1 >> 16, bc1 & 0xFFFF, bc2 >> 16, bc2 & 0xFFFF );
	pBuff += wcslen( pBuff );

	UINT32 dc = entry->rotorInformationResults.rotorDistributionCheckFlags;
	UINT32 sf = entry->rotorInformationResults.rotorSystemFlags;
	wsprintf( pBuff, L"05 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
		dc >> 16, dc & 0xFFFF, sf >> 16, sf & 0xFFFF );
	pBuff += wcslen( pBuff );

	wsprintf( pBuff, L"06 %4.4X%23s\r\n",
		entry->rotorInformationResults.rotorQcFlags,
		L" " );
	pBuff += wcslen( pBuff );

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

					analyteId = analytes.GetAnalyteId( (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType );

					// Result format depends on whether we're in research mode or not.
					formatType = systemData->inResearchMode ? FMT_ERROR_RESEARCH : FMT_ERROR;
                    // convert char to wchar_t string
                    mbstowcs (wBuff, FormatResult( entry, j, formatType, strings ), 512);
					wsprintf( pBuff, L"%-5.5s %s %4.4X %4.4X %6.6s\r\n",
						analytes.GetAnalysisString (analyteId, strings),
                        wBuff,
						flags >> 16, flags & 0xFFFF,
						units.String( analyteId ) );
					pBuff += wcslen( pBuff );
				}
			}
		}
	}

	// Line of blanks after the results.
	wsprintf( pBuff, L"%30s\r\n", L" " );
	pBuff += wcslen( pBuff );	

	/*
	 * Footer.
	 */
	// RQC result.
	if ( entry->rotorAnalyteResults[ RQC ].analyteResult < 0 )
	{
		wsprintf( pBuff, L"RQC: ~~~%22s\r\n", L" " );
	}
	else
	{
		int rqcResult = (int)( entry->rotorAnalyteResults[ RQC ].analyteResult + 0.5);
		wsprintf( pBuff, L"RQC: %3d%22s\r\n", rqcResult, L" " );
	}
	pBuff += wcslen( pBuff );

	// Endogenous values.

	// HEM
	if ( entry->rotorInformationResults.rotorHemolyzedIndex < -999.0 )
	{
		wsprintf( pBuff, L"HEM ~~~   " );
	}
	else
	{
		if ( entry->rotorInformationResults.rotorHemolyzedIndex >= 0 )
		{
			endogenousIndex = (int)( entry->rotorInformationResults.rotorHemolyzedIndex +0.5);
		}
		else
		{
			endogenousIndex = (int)(entry->rotorInformationResults.rotorHemolyzedIndex +0.5);
		}
		wsprintf( pBuff, L"HEM %-4d  ", endogenousIndex );
	}
	pBuff += wcslen( pBuff );
	
	// LIP
	if ( entry->rotorInformationResults.rotorLipemicIndex < -999.0 )
	{
		wsprintf( pBuff, L"LIP ~~~   " );
	}
	else
	{
		endogenousIndex = (int)( entry->rotorInformationResults.rotorLipemicIndex +0.5);
		wsprintf( pBuff, L"LIP %-4d  ", endogenousIndex );
	}
	pBuff += wcslen( pBuff );
	
	// ICT
	if ( entry->rotorInformationResults.rotorIctericIndex < -999.0 )
	{
		wsprintf( pBuff, L"ICT ~~~   \r\n" );
	}
	else
	{
		endogenousIndex = (int)( entry->rotorInformationResults.rotorIctericIndex +0.5);
		wsprintf( pBuff, L"ICT %-4d  \r\n", endogenousIndex );
	}
	pBuff += wcslen( pBuff );

	// Warning line, only if Piccolo.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		if ( entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0 )
		{
			wsprintf( pBuff, L"%-30.30s\r\n", strings->PrintString (RSTR_CONFIRM_LOW_RECOVERIES));
			pBuff += wcslen( pBuff );	
		}
		else
		{
			wsprintf( pBuff, L"%30s\r\n", " " );
			pBuff += wcslen( pBuff );	
		}
	}

	// Software version.
    mbstowcs (wBuff, version, 512);
	wsprintf( pBuff, L"%-21.21s%9s\r\n", strings->PrintString (MSTR_VERSION), wBuff );
	pBuff += wcslen( pBuff );

	// Error text.
	if ( entry->errNum != SERR_NONE )
	{
		SystemError	systemError;

		wsprintf( pBuff, L"%04X %25s\r\n", entry->errNum, strings->PrintString (systemError.GetScreenText( entry->errNum )));
		pBuff += wcslen( pBuff );
	}

	// CR/LF pair to signal the end of the report.
	wcscpy( pBuff, L"\r\n" );
	pBuff += wcslen( pBuff );	
}

/*
 * FUNCTION
 * 	CreateDisplay::CreateIqcDisplay
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
CreateDisplay::
CreateIqcDisplay( int index, wchar_t * buff, AStrings *strings )
{
	ResultsDbEntry_t entry;
	
	if ( GetResult( &entry, index ) == true )
	{
		CreateIqcDisplay( &entry, buff, strings );
	}
}

void
CreateDisplay::
CreateIqcDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings )
{
	wchar_t *		pBuff = buff;


	// Header text.
	pBuff = CreateHeaderDisplay( entry, pBuff, strings );
		
	// Body text.
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		wsprintf( pBuff, L"%-27s%3d\r\n",
			strings->PrintString (RSTR_INSTRUMENT_QC),
			(int)( (entry->rotorInformationResults.rotorIqcRatioPrecision * 100) +0.5) );
		pBuff += wcslen( pBuff );

		for ( int i = 0; i < 8; i++ )
		{
			wsprintf( pBuff, L"iQC %d:        %3d  %s nm: %3d\r\n",
				i+1,
				(int)( (entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100) +0.5 ),
				wavelengthStrings [ i ],
				(int)( (entry->rotorInformationResults.rotorIqcLevel2Results[ i ] * 100) +0.5 ) );
			pBuff += wcslen( pBuff );
		}

		// Range.
		wsprintf( pBuff, L"%-11s90-110       95-105\r\n", strings->PrintString (RSTR_RANGE));
		pBuff += wcslen( pBuff );
	}
	
	else
	{
		for ( int i = 0; i < 8; i++ )
		{
			wsprintf( pBuff, L"iQC %d:        %3d%13s\r\n",
				i+1,
				(int)( (entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100) +0.5 ),
				" " );
			pBuff += wcslen( pBuff );
		}

		// Range.
		wsprintf( pBuff, L"%-11s90-110             \r\n", strings->PrintString (RSTR_RANGE ));
		pBuff += wcslen( pBuff );

	}
	
	// Space row.
	wcscpy( pBuff, L"\r\n" );
	pBuff += wcslen( pBuff );
	
	// Footer text.
	wsprintf( pBuff, L"%-27s%3d\r\n",
		strings->PrintString ( RSTR_CHEMISTRY_QC),
		(int)( entry->rotorAnalyteResults[ RQC ].analyteResult +0.5) );
	pBuff += wcslen( pBuff );
	wsprintf( pBuff, L"%-27s%3d\r\n", strings->PrintString (RSTR_ACCEPTABLE_MINIMUM), RQC_ACCEPTABLE_MINIMUM );
	pBuff += wcslen( pBuff );
	
	// CR/LF pair to signal the end of the report.
	wcscpy( pBuff, L"\r\n" );
	pBuff += wcslen( pBuff );	
}

/*
 * FUNCTION
 * 	CreateDisplay::CreateRefRangesDisplay
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
CreateDisplay::
CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, wchar_t * buff, AStrings *strings )
{
	wchar_t *				pBuff = buff;
	int						i;
	AnalyteId_t				analyteId;
	float					low, high;
	const AnalyteList_t *	analyteList;
	
	if ( sampleType != STYPE_ALL )
	{
		// Header text.
		pBuff = CreateRefRangesHeaderDisplay( sampleType, gender, pBuff, strings );
			
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
			
			wsprintf( pBuff, L"%-5.5s %S %6.6s        \r\n",
						analytes.GetAnalysisString (analyteId, strings),
						FormatRefRange( analyteId, low, high ),
						units.String( analyteId ) );
			pBuff += wcslen( pBuff );
		}

		// CR/LF pair to signal the end of the report.
	    wcscpy ( pBuff, L"\r\n" );
		pBuff += wcslen ( pBuff );	
	}
}

bool
CreateDisplay::
GetResult( ResultsDbEntry_t * entry, int index )
{
	bool 			retVal = false;
	const char *	dbData;
	FILE *			fp;

	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;
	
	fp = fopen( dbData, "rb" );
	if ( fp != NULL )
	{
		if ( fseek( fp, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) == 0 )
		{
			int bytesRead = fread( (char *)entry, 1, sizeof( ResultsDbEntry_t ), fp );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )
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

wchar_t *
CreateDisplay::
GetDate( FILETIME * time, AStrings *strings )
{
	static wchar_t dateBuff[15];
	SYSTEMTIME	tm;

	FileTimeToSystemTime(time, &tm);
	
	wsprintf( dateBuff, L"%02d %s %04d", tm.wDay, strings->PrintMonth( tm.wMonth ), tm.wYear );

	return dateBuff;
}

wchar_t *
CreateDisplay::
GetDate( Date_t * date, AStrings *strings )
{
	static wchar_t dateBuff[15];

	wsprintf( dateBuff, L"%02d %s %04d", date->day, strings->PrintMonth (date->month), date->year );
	
	return dateBuff;
}

wchar_t *
CreateDisplay::
GetTime( FILETIME * time )
{
	static wchar_t		timeBuff[10];
	SYSTEMTIME	        tm;
    int                 hour;
	const char *	    amPm;

	FileTimeToSystemTime(time, &tm);
	hour = tm.wHour;
	
	if ( settings->time24Format )
	{
		wsprintf( timeBuff, L"   %02d:%02d", tm.wHour, tm.wMinute );
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
	
		wsprintf( timeBuff, L"%02d:%02d %S", hour, tm.wMinute, amPm );
	}
	
	return timeBuff;
}
const wchar_t *
CreateDisplay::
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
