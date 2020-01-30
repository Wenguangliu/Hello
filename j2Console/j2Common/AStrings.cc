#include <windows.h>
//#include <tlhelp32.h>
#include <string.h>

#include "AStrings.h"
#include "LogErr.h"
#include "Shmem.h"
//#include "TestString.h"
#include "AsiaStrings.d"
#include "WesternStrings.h"


#if 0 
// Find out current process name for debug only
wchar_t *
AStrings::
GetProcName(void)
{
    HANDLE proctable;
    static PROCESSENTRY32 pe;
    BOOL success;
    DWORD cPid;
    int Q_quiet = 0;

    // get current processId
    cPid = GetCurrentProcessId ();

    // create tool
    proctable = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (proctable == INVALID_HANDLE_VALUE) {
        if (!Q_quiet) printf("CreateToolhelp32Snapshot Failed\n");
        return  NULL;
    }

    pe.dwSize = sizeof (PROCESSENTRY32);
    success=Process32First(proctable, &pe);
    while (success){
        //wprintf(L"%d\t%s\n", pe.th32ProcessID, pe.szExeFile);
        if (pe.th32ProcessID  == cPid) {
            return(pe.szExeFile);
        }
        success=Process32Next(proctable, &pe);
    }
    return NULL;
}
#endif


AStrings::
AStrings()
{
	int	i;
    static char count = 0;


	memset(humanStrings, 0, sizeof(humanStrings));
	memset(vetStrings,   0, sizeof(vetStrings));
    
    //memset (wHumanStrings, 0, sizeof (wHumanStrings));
   // memset (wVetStrings,   0, sizeof (wVetStrings));

//  printf ("humanStrings =%lu vetStrings =%ld\n", sizeof (humanStrings), sizeof (vetStrings));
 // printf ("wHumanStrings=%lu wVetStrings=%lu\n", sizeof (wHumanStrings), sizeof (wVetStrings));

	// Initialize both the human and vet string sets to the human values.
	for ( i = 0; HUMAN_WESTERN_STRINGS[ i ].id != STR_NONE; i++ )
	{
	//	memcpy (humanStrings[ HUMAN_STRINGS[i].id ],    HUMAN_STRINGS[i].str, sizeof (HUMAN_STRINGS[i].str));
    //	memcpy (vetStrings[ HUMAN_STRINGS[i].id ],      HUMAN_STRINGS[i].str, sizeof( HUMAN_STRINGS[i].str));
 
        // add western union code string address to able 
        memcpy (humanStrings [HUMAN_WESTERN_STRINGS [ i ].id], HUMAN_WESTERN_STRINGS [i].str, sizeof (HUMAN_WESTERN_STRINGS [i].str));
        memcpy (vetStrings   [HUMAN_WESTERN_STRINGS [ i ].id], HUMAN_WESTERN_STRINGS [i].str, sizeof (HUMAN_WESTERN_STRINGS [i].str));

        // add Chinese character string address to table 
        memcpy (&humanStrings [HUMAN_ASIA_STRINGS [i].id][LANGUAGE_CHINESE], &HUMAN_ASIA_STRINGS [i].str [1], sizeof (&HUMAN_ASIA_STRINGS [i].str [1]));
        memcpy (&vetStrings   [HUMAN_ASIA_STRINGS [i].id][LANGUAGE_CHINESE], &HUMAN_ASIA_STRINGS [i].str [1], sizeof (&HUMAN_ASIA_STRINGS [i].str [1]));
 
	}


	// Change the differing strings in the vet string set to the correct values.
	for (i = 0; VET_WESTERN_STRINGS [i].id != STR_NONE; i++) {
		memcpy (vetStrings[ VET_WESTERN_STRINGS [i].id], VET_WESTERN_STRINGS [i].str, sizeof (VET_WESTERN_STRINGS [i].str));
        // add Chinese character string address to table 
        memcpy (&vetStrings [VET_ASIA_STRINGS [i].id][LANGUAGE_CHINESE], &VET_ASIA_STRINGS [i].str [1], sizeof (&VET_ASIA_STRINGS [i].str [1]));
	}

	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
    count++;
	//printf ("AString copy %d\n", count);
	//printf ("Process :%S Call AString copy %d\n",GetProcName (), count);
}

const wchar_t *
AStrings::
operator[]( StringId_t id )
{
	const wchar_t * str;
	
	if ( (factoryData != 0 ) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		str = humanStrings[ id ][ settingsData->language ];
	}
	else
	{
		str = vetStrings[ id ][ settingsData->language ];
	}
	
	if ( str == 0 )
	{
		LogError( "string %d undefined in AStrings", id );

		if ( (factoryData != 0) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			str = humanStrings[ id ][ LANGUAGE_ENGLISH ];
		}
		else
		{
			str = vetStrings[ id ][ LANGUAGE_ENGLISH ];
		}
		
		if ( str == 0 )
		{
			str = L"xxx";
		}
	}
	
	return str;
}

const wchar_t *
AStrings::
ChineseString( StringId_t id )
{
	const wchar_t * str;
	
	if ( (factoryData != 0 ) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		str = humanStrings[ id ][ LANGUAGE_CHINESE ];
	}
	else
	{
		str = vetStrings[ id ][ LANGUAGE_CHINESE];
	}
	
	if ( str == 0 )
	{
		LogError( "string %d undefined in AStrings", id );

		if ( (factoryData != 0) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			str = humanStrings[ id ][ LANGUAGE_ENGLISH ];
		}
		else
		{
			str = vetStrings[ id ][ LANGUAGE_ENGLISH ];
		}
		
		if ( str == 0 )
		{
			str = L"xxx";
		}
	}
	
	return str;
}

// special function created for print resutls string 
// in the case language is chinese,  English string should be printed
const wchar_t *
AStrings::
PrintString (StringId_t id)
{
    const wchar_t * str;

    if (settingsData->language == LANGUAGE_CHINESE) {
        str =  English (id);
    } else {
        if ((factoryData != 0) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE)) {
            str = humanStrings [id][settingsData->language];
        } else {
            str = vetStrings [id][settingsData->language];
        }
    }

    if (str == 0) {
        LogError("string %d undefined in AStrings", id);

        if ((factoryData != 0) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE)) {
            str = humanStrings[id][LANGUAGE_ENGLISH];
        } else {
            str = vetStrings [id][LANGUAGE_ENGLISH];
        }
        if (str == 0) {
            str = L"xxx";
        }
    }
        
    return str;
}

// in the case of chinese, gender print english
const wchar_t *
AStrings::
PrintGender (Gender_t gender)
{
    const wchar_t * str;

    if (settingsData->language == LANGUAGE_CHINESE) {
        switch (gender) {
            case GENDER_MALE:       
                str = humanStrings [MSTR_MALE][LANGUAGE_ENGLISH]; 
            break;
            case GENDER_FEMALE:     
                str = humanStrings [MSTR_FEMALE][LANGUAGE_ENGLISH];   
            break;
            case GENDER_UNKNOWN:
                str = humanStrings [MSTR_UNKNOWN][LANGUAGE_ENGLISH];  
            break;

            default:                
            break;
        }
    } else {
      str = Gender (gender);
    }
    return str;
}

const wchar_t *
AStrings::
English( StringId_t id )
{
	const wchar_t * str;
	
	if ( (factoryData != 0 ) && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		str = humanStrings[ id ][ LANGUAGE_ENGLISH ];
	}
	else
	{
		str = vetStrings[ id ][ LANGUAGE_ENGLISH ];
	}
	
	if ( str == 0 )
	{
		LogError( "string %d undefined in AStrings", id );
		str = L"xxx";
	}
	
	return str;
}

const wchar_t *
AStrings::
PrintDayOfWeek (unsigned index)
{
	const StringId_t	days[] = { STR_SUNDAY, STR_MONDAY, STR_TUESDAY,
								   STR_WEDNESDAY, STR_THURSDAY, STR_FRIDAY,
								   STR_SATURDAY };

    if ( index < 7) {
        if (settingsData->language == LANGUAGE_CHINESE) {
            return humanStrings[days[index ]][LANGUAGE_ENGLISH];
        } else {
		    return humanStrings[ days[ index ] ][ settingsData->language ];
        }
								   
	} else {
		return humanStrings[ STR_NONE ][ settingsData->language ];
	}
}

const wchar_t *
AStrings::
DayOfWeek( unsigned index )
{
	const StringId_t	days[] = { STR_SUNDAY, STR_MONDAY, STR_TUESDAY,
								   STR_WEDNESDAY, STR_THURSDAY, STR_FRIDAY,
								   STR_SATURDAY };
								   
	if ( index < 7 )
	{
		return humanStrings[ days[ index ] ][ settingsData->language ];
	}
	else
	{
		return humanStrings[ STR_NONE ][ settingsData->language ];
	}
}


const wchar_t *
AStrings::
PrintMonth(unsigned index)
{
    const  wchar_t *str;

    const StringId_t    months[] = { STR_JAN, STR_FEB, STR_MAR, STR_APR, STR_MAY,
                                     STR_JUN, STR_JUL, STR_AUG, STR_SEP, STR_OCT,
                                     STR_NOV, STR_DEC };

    index--;

    if (index < 12) {
        if (settingsData->language == LANGUAGE_CHINESE) {
            str =  humanStrings [months[index]][LANGUAGE_ENGLISH];
        } else {
            str = humanStrings[ months[ index ] ][ settingsData->language ];
        }
    } else {
		return humanStrings[ STR_NONE ][ settingsData->language ];
    }

    return str;
}


const wchar_t *
AStrings::
Month( unsigned index )
{
	const StringId_t	months[] = { STR_JAN, STR_FEB, STR_MAR, STR_APR, STR_MAY,
									 STR_JUN, STR_JUL, STR_AUG, STR_SEP, STR_OCT,
									 STR_NOV, STR_DEC };
    
    
     /* Windows time functions count from January = 1 */
    index--;
	if ( index < 12 )
	{
		return humanStrings[ months[ index ] ][ settingsData->language ];
	}
	else
	{
		return humanStrings[ STR_NONE ][ settingsData->language ];
	}
}

const wchar_t *
AStrings::
Gender( Gender_t gender )
{
	switch ( gender )
	{
		case GENDER_MALE:		
            return humanStrings[ MSTR_MALE ][ settingsData->language ];	
        break;
		case GENDER_FEMALE:		
            return humanStrings[ MSTR_FEMALE ][ settingsData->language ];	
        break;
		case GENDER_UNKNOWN:
		default:				
            return humanStrings[ MSTR_UNKNOWN ][ settingsData->language ];	
        break;
	}
}

// get language string
const wchar_t *
AStrings::
Language (Language_t languang)
{

    return  (humanStrings[ STR_LANGUAGE ] [languang]);

}
