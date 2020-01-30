/*
 * FILENAME
 * 	AstmInput.cc
 * 
 * MODULE DESCRIPTION
 * 	Contains functions to validate order input data received by ASTM, and send it on to Analysis.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <fcntl.h>
#include <mqueue.h>
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "AnalysisMessages.h"
#include "Species.h"
#include "LogErr.h"
#include "CommonDefines.h"
#include "Astm.h"
#include "AstmInput.h"
#include "AstmQuery.h"
#include "RotorInfo.h"
#include "hmac_sha2.h"

// Limit for mismatch between message date and analyzer date is 2 days, and we need
// the value in seconds.
const double	DateMismatchLimit = (double)(2 * 24 * 60 * 60 );

RotorInfo	rotorInfo;

// GLD
// Once it's all working OK, move the string-to-SampleType_t conversion code to Species.cc/h?
typedef struct
{
	const char *	label;
	SampleType_t	sampleType;
} SpeciesConversion_t;

SpeciesConversion_t	speciesTable[] = {
	{"african gray"		, STYPE_AFRICAN_GRAY},
	{"alligator"		, STYPE_ALLIGATOR},
	{"amazon"			, STYPE_AMAZON},
	{"amphibian"		, STYPE_AMPHIBIAN},
	{"antelope"			, STYPE_ANTELOPE},
	{"ape"				, STYPE_APE},
	{"baboon"			, STYPE_BABOON},
	{"bat"				, STYPE_BAT},
	{"bear"				, STYPE_BEAR},
	{"bearded dragon"	, STYPE_BEARDED_DRAGON},
	{"bird"				, STYPE_BIRD},
	{"bison"			, STYPE_BISON},
	{"boa constrictor"	, STYPE_BOA_CONSTRICTOR},
	{"bobcat"			, STYPE_BOBCAT},
	{"buffalo"			, STYPE_BUFFALO},
	{"bull"				, STYPE_BULL},
	{"camel"			, STYPE_CAMEL},
	{"cat"				, STYPE_CAT},
	{"chameleon"		, STYPE_CHAMELEON},
	{"cheetah"			, STYPE_CHEETAH},
	{"chicken"			, STYPE_CHICKEN},
	{"chimpanzee"		, STYPE_CHIMPANZEE},
	{"chinchilla"		, STYPE_CHINCHILLA},
	{"cockatiel"		, STYPE_COCKATIEL},
	{"cockatoo"			, STYPE_COCKATOO},
	{"conure"			, STYPE_CONURE},
	{"cow"				, STYPE_COW},
	{"coyote"			, STYPE_COYOTE},
	{"crocodile"		, STYPE_CROCODILE},
	{"deer"				, STYPE_DEER},
	{"dingo"			, STYPE_DINGO},
	{"dog"				, STYPE_DOG},
	{"donkey"			, STYPE_DONKEY},
	{"dove"				, STYPE_DOVE},
	{"duck"				, STYPE_DUCK},
	{"elephant"			, STYPE_ELEPHANT},
	{"ferret"			, STYPE_FERRET},
	{"flamingo"			, STYPE_FLAMINGO},
	{"fox"				, STYPE_FOX},
	{"frog"				, STYPE_FROG},
	{"gecko"			, STYPE_GECKO},
	{"gerbil"			, STYPE_GERBIL},
	{"giraffe"			, STYPE_GIRAFFE},
	{"goat"				, STYPE_GOAT},
	{"goose"			, STYPE_GOOSE},
	{"guinea pig"		, STYPE_GUINEA_PIG},
	{"hamster"			, STYPE_HAMSTER},
	{"hedgehog"			, STYPE_HEDGEHOG},
	{"horse"			, STYPE_HORSE},
	{"iguana"			, STYPE_IGUANA},
	{"jackal"			, STYPE_JACKAL},
	{"jaguar"			, STYPE_JAGUAR},
	{"kangaroo"			, STYPE_KANGAROO},
	{"large mammal"		, STYPE_LARGE_MAMMAL},
	{"lemur"			, STYPE_LEMUR},
	{"leopard"			, STYPE_LEOPARD},
	{"lion"				, STYPE_LION},
	{"lizard"			, STYPE_LIZARD},
	{"llama"			, STYPE_LLAMA},
	{"lovebird"			, STYPE_LOVEBIRD},
	{"lynx"				, STYPE_LYNX},
	{"macaw"			, STYPE_MACAW},
	{"mink"				, STYPE_MINK},
	{"monitor"			, STYPE_MONITOR},
	{"monkey"			, STYPE_MONKEY},
	{"mouse"			, STYPE_MOUSE},
	{"mynah"			, STYPE_MYNAH},
	{"opossum"			, STYPE_OPOSSUM},
	{"ostrich"			, STYPE_OSTRICH},
	{"other"			, STYPE_OTHER},
	{"ox"				, STYPE_OX},
	{"panther"			, STYPE_PANTHER},
	{"parakeet"			, STYPE_PARAKEET},
	{"parrot"			, STYPE_PARROT},
	{"pig"				, STYPE_PIG},
	{"pony"				, STYPE_PONY},
	{"prairie dog"		, STYPE_PRAIRIE_DOG},
	{"puma"				, STYPE_PUMA},
	{"python"			, STYPE_PYTHON},
	{"rabbit"			, STYPE_RABBIT},
	{"raccoon"			, STYPE_RACCOON},
	{"rat"				, STYPE_RAT},
	{"reptile"			, STYPE_REPTILE},
	{"salamander"		, STYPE_SALAMANDER},
	{"sea lion"			, STYPE_SEA_LION},
	{"seal"				, STYPE_SEAL},
	{"sheep"			, STYPE_SHEEP},
	{"skunk"			, STYPE_SKUNK},
	{"small mammal"		, STYPE_SMALL_MAMMAL},
	{"snake"			, STYPE_SNAKE},
	{"squirrel"			, STYPE_SQUIRREL},
	{"steer"			, STYPE_STEER},
	{"tiger"			, STYPE_TIGER},
	{"toad"				, STYPE_TOAD},
	{"tortoise"			, STYPE_TORTOISE},
	{"toucan"			, STYPE_TOUCAN},
	{"turtle"			, STYPE_TURTLE},
	{"water buffalo"	, STYPE_WATER_BUFFALO},
	{"wolf"				, STYPE_WOLF},
	{"zebra"			, STYPE_ZEBRA},
};

int
TableCompare( const void * keyval, const void * datum )
{
	int	retVal;
	
	retVal = stricmp( (char *)keyval, ((SpeciesConversion_t *)datum)->label );
	return retVal;
}

unsigned long long
ConvertStringToSampleType( const char * str, int * sampleType, ActionCode_t actionCode )
{
	unsigned long long		retVal = AIE_NO_ERROR;
	unsigned				index;
	char					indexStr[3];
	SpeciesConversion_t *	s;
	const int				tableLen = sizeof( speciesTable ) / sizeof( SpeciesConversion_t );

	// Default value for sample type if there's an input error.
	*sampleType = STYPE_NONE;
	
	if ( strnicmp( str, "special ", 8 ) == 0 )
	{
		// This sample type not allowed for controls.
		if ( actionCode == ACTION_CODE_Q )
		{
			retVal = AIE_SAMPLE_TYPE_INVALID;
		}

		// This sample type only allowed on piccolos.
		else if ( factoryData->abaxisProductCode != PICCOLO_PRODUCT_CODE )
		{
			retVal = AIE_SAMPLE_TYPE_INVALID;
		}

		else
		{
			strncpy( indexStr, str + 8, 2 );
			indexStr[2] = 0;
			index = atoi( indexStr );
			if ( (index > 0) && (index <= 99) )
			{
				*sampleType = STYPE_FIRST_SPECIES + index - 1;
			}
			else
			{
				retVal = AIE_SAMPLE_TYPE_INVALID;
			}
		}
	}

	else if ( strnicmp( str, "control ", 8 ) == 0 )
	{
		// This sample type only allowed for controls.
		if ( actionCode != ACTION_CODE_Q )
		{
			retVal = AIE_SAMPLE_TYPE_INVALID;
		}

		else
		{
			strncpy( indexStr, str + 8, 2 );
			indexStr[2] = 0;
			index = atoi( indexStr );
	
			if ( (index > 0) && (index <= 99) )
			{
				*sampleType = STYPE_CONTROL + index;
			}
			else
			{
				retVal = AIE_SAMPLE_TYPE_INVALID;
			}
		}
	}
	
	// If not special or control, then it must be a named species, which is only
	// allowed on VetScans.
	else if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	{
		s = (SpeciesConversion_t *)bsearch( str, speciesTable, tableLen, sizeof( SpeciesConversion_t), TableCompare );
		if ( s == 0 )
		{
			retVal = AIE_SAMPLE_TYPE_INVALID;
		}
		else
		{
			*sampleType = s->sampleType;
		}
	}
	
	else
	{
		retVal = AIE_SAMPLE_TYPE_INVALID;
	}
	
	return retVal;
}

unsigned long long
ParseActionCode ( char * s, ActionCode_t * actionCode )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	// Default value for action code if there's an input error.
	*actionCode = ACTION_CODE_NONE;

	if ( s[0] == 0 )
	{
		retVal = AIE_ACTION_CODE_MISSING;
	}

	else
	{
		switch ( s[0] )
		{
			case 'N':
			case 'n': *actionCode = ACTION_CODE_N; break;
			case 'Q':
			case 'q': *actionCode = ACTION_CODE_Q; break;
			case 'C':
			case 'c': *actionCode = ACTION_CODE_C; break;
			default: retVal = AIE_ACTION_CODE_INVALID; break;
		}
	}

	return retVal;
}

unsigned long long
ParseRace( char * s, Race_t * race )
{
	unsigned long long	retVal = AIE_NO_ERROR;

	if ( s[0] == 0 )
	{
		if ( settingsData->egfrEnabled && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			retVal |= AIE_RACE_MISSING;
		}
	}

	else
	{
		switch ( s[0] )
		{
			case 'W':
			case 'w': *race = RACE_WHITE; break;
			case 'B':
			case 'b': *race = RACE_BLACK; break;
			case 'O':
			case 'o': *race = RACE_ASIAN_PACIFIC_ISLANDER; break;
			case 'N':
			case 'n': *race = RACE_NATIVE_AMERICAN; break;
			case 'H':
			case 'h': *race = RACE_HISPANIC; break;
			case 0:   *race = RACE_UNKNOWN; break;
			default:
				*race = RACE_UNKNOWN;
				retVal |= AIE_RACE_BAD_FORMAT;
				break;
		}
	}
	
	return retVal;
}

unsigned long long
ParseGender( char * s, Gender_t * gender )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( s[0] == 0 )
	{
		if ( settingsData->egfrEnabled && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			retVal |= AIE_GENDER_MISSING;
		}
	}

	else
	{
		switch ( s[0] )
		{
			case 'M':
			case 'm': *gender = GENDER_MALE; break;
			case 'F':
			case 'f': *gender = GENDER_FEMALE; break;
			case 'U':
			case 'u': *gender = GENDER_UNKNOWN; break;
			default:
				retVal |= AIE_GENDER_BAD_FORMAT;
				break;
		}
	}
	
	return retVal;
}

unsigned long long
ParseDate( char * s, Date_t * date, unsigned long long errCode )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	int					i;

	memset( date, 0, sizeof( Date_t ) );

	if ( s[0] != 0 )
	{
		if ( strlen( s ) != 8 )
		{
			retVal |= errCode;
		}
		
		else
		{
			for ( i = 0; i < 8; i++ )
			{
				if ( !isdigit( s[ i ] ) )
				{
					retVal |= errCode;
					break;
				}
			}
			if ( retVal == AIE_NO_ERROR )
			{
				date->year = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + s[3] - '0';
				date->month = (s[4] - '0') * 10 + s[5] - '0' - 1;	// Stored range is 0 to 11.
				date->day = (s[6] - '0') * 10 + s[7] - '0';
				if ( (date->month > 11) || (date->day < 1) || (date->day > 31) )
				{
					retVal |= errCode;
				}
			}
		}
	}

	return retVal;	
}


unsigned long long
ParseBirthdate( char * s, Date_t * date )
{
	unsigned long long	retVal = AIE_NO_ERROR;

	if ( s[0] == 0 )
	{
		if ( settingsData->egfrEnabled && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			retVal = AIE_BIRTHDATE_MISSING;
		}
	}

	else
	{
		retVal = ParseDate( s, date, AIE_BIRTHDATE_BAD_FORMAT );
	}
	
	return retVal;	
}

/*
 * FUNCTION
 * 	ParsePatientSpecial1
 * 
 * DESCRIPTION
 * 	Parses the field into three parts:
 *   1. Owner/Admission ID - up to ID_LEN characters long
 *   2. Patient age - up to 3 decimal digits
 *   3. Date of last vaccination - 8 digits
 *
 * 	Fields are separated by the '^' character. All fields are optional.
 * 
 * RETURNS
 * 	Pointer to a data structure holding the needed instrument information.
 */

unsigned long long
ParsePatientSpecial1( char * inStr, UploadedOrder_t * order )
{
	char				subField[ 3 ][ ASTM_RX_P_PATIENT_SPECIAL1_LEN + 1 ];
	unsigned			i, j, k;
	unsigned long long	retVal = AIE_NO_ERROR;
	
	// Init all subfields to nulls.
	memset( subField, 0, 3 * (ASTM_RX_P_PATIENT_SPECIAL1_LEN + 1) );

	// All fields are optional, so no error if the entire field is empty.
	if ( inStr[0] != 0 )
	{
		// Separate into three subfields.
		for ( i = 0, j = 0, k = 0;
			  (i < ASTM_RX_P_PATIENT_SPECIAL1_LEN + 1) && (inStr[ i ] != 0) && (k < 3);
			  i++ )
		{
			if ( inStr[ i ] == '^' )
			{
				j = 0;
				k++;
			}
			else
			{
				subField[ k ][ j ] = inStr[ i ];
				j++;
			}
		}

		// Now, handle the subfields.

		// Owner/Admission ID. Maximum length is ID_LEN, and no CRs are allowed.
		if ( strchr( subField[ 0 ], '\r' ) != 0 )
		{
			retVal |= AIE_OWNER_ADMISSION_ID_BAD_FORMAT;
		}
		else if ( strlen( subField[ 0 ] ) > ID_LEN )
		{
			retVal |= AIE_OWNER_ADMISSION_ID_TOO_LONG;
		}
		else
		{
			strcpy( order->admissionOwnerId, subField[ 0 ] );
		}

		// Patient age.
		order->ageUnits = AGE_UNITS_YEARS;
		for ( i = 0; i < strlen( subField[ 1 ] ); i++ )
		{
			if ( !isdigit( subField[ 1 ][ i ] ) || (i > 2) )
			{
				retVal |= AIE_AGE_BAD_FORMAT;
				break;
			}
		}
		order->age = atoi( subField[ 1 ] );
		
		// Date of last vaccination.
		if ( subField[ 2 ][ 0 ] != 0 )
		{
			retVal |= ParseDate( subField[ 2 ], &order->lastVaccination, AIE_DATE_OF_VACCINATION_BAD_FORMAT );
		}
	}
	
	return retVal;
}

unsigned long long
ParseString( char * iStr, char * oStr, int errCode )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( strlen( iStr ) != 0 )
	{
		if ( strchr( iStr, '\r' ) != 0 )
		{
			retVal |= errCode;
		}
		else
		{
			strcpy( oStr, iStr );
		}
	}

	return retVal;
}

unsigned long long
ParseOrderLabel( AstmInput_t * astmInput, char * oStr )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( strchr( astmInput->oLaboratoryField1, '\r' ) != 0 )
	{
		retVal |= AIE_ORDER_LABEL_BAD_FORMAT;
	}

	// If we've got an order label, use it. Otherwise, use the patient ID.
	else
	{
		if ( strlen( astmInput->oLaboratoryField1 ) > 0 )
		{
			strcpy( oStr, astmInput->oLaboratoryField1 );
		}
		else
		{
			strcpy( oStr, astmInput->pPatientControlId );
		}
	}

	return retVal;
}

unsigned long long
ParsePatientId( char * iStr, char * oStr )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( strlen( iStr ) > 0 )
	{
		if ( strchr( iStr, '\r' ) != 0 )
		{
			retVal |= AIE_PATIENT_CONTROL_ID_BAD_FORMAT;
		}
		else
		{
			strcpy( oStr, iStr );
		}
	}
	else
	{
		retVal |= AIE_PATIENT_CONTROL_ID_MISSING;
	}

	return retVal;
}

unsigned long long
ParseRotorId( char * iStr, UploadedOrder_t * order )
{
	char *					s;
	unsigned				i;
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( (strlen( iStr ) == 0) || (strcmp( iStr, "^^^" ) == 0) )
	{
		retVal |= AIE_ROTOR_NUMBER_MISSING;
	}
	
	else
	{
		// Rotor ID is the fourth subfield, and the other subfields should be empty.
		if ( strncmp( iStr, "^^^", 3 ) != 0 )
		{
			retVal |= AIE_ROTOR_NUMBER_INVALID;
		}
		
		else
		{
			for ( i = 0, s = iStr + 3; i < strlen( s ); i++ )
			{
				if ( !isdigit( s[ i ] ) )
				{
					retVal |= AIE_ROTOR_NUMBER_INVALID;
					break;
				}
			}
	
			if ( i == strlen( s ) )
			{
				order->rotorId = atoi( s );
				
				if ( rotorInfo.RotorProductCode( order->rotorId ) != factoryData->abaxisProductCode )
				{
					retVal |= AIE_ROTOR_NUMBER_INVALID;
				}
				else
				{
					strncpy( order->rotorNameLine1, rotorInfo.RotorNameLine1( order->rotorId ), sizeof( order->rotorNameLine1 ) );
					strncpy( order->rotorNameLine2, rotorInfo.RotorNameLine2( order->rotorId ), sizeof( order->rotorNameLine2 ) );
					order->rotorNameFontColor = rotorInfo.RotorNameFontColor( order->rotorId );
					order->rotorNameBackgroundColor = rotorInfo.RotorNameBackgroundColor( order->rotorId );
				}
			}
		}
	}
	
	return retVal;
}

unsigned long long
ParseSampleType( char * iStr, int * sampleType, ActionCode_t actionCode )
{
	unsigned long long	retVal = AIE_NO_ERROR;

	if ( iStr[0] == 0 )
	{
		if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
		{
			retVal = AIE_SAMPLE_TYPE_MISSING;
		}
		else
		{
			if ( actionCode == ACTION_CODE_Q )
			{
				*sampleType = STYPE_CONTROL;
			}
			else
			{
				*sampleType = STYPE_PATIENT;
			}
		}
	}
	else
	{
		retVal = ConvertStringToSampleType( iStr, sampleType, actionCode );
	}
	
	return retVal;
}

PatientType_t
SetPatientType( int sampleType, Gender_t gender, ActionCode_t actionCode )
{
	PatientType_t	patientType;
	
	// If the action code is Q, then the patient type is control, regardless of the
	// sample type, gender, and instrument type.
	if ( actionCode == ACTION_CODE_Q )
	{
		patientType = PATIENT_TYPE_CONTROL;
	}
	
	// If a piccolo and not a control, then the patient type is based on the gender
	// given.
	else if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		switch ( gender )
		{
			case GENDER_FEMALE:	patientType = PATIENT_TYPE_DOG_FEMALE;	break;
			case GENDER_MALE:	patientType = PATIENT_TYPE_CAT_MALE;	break;
			default:			patientType = PATIENT_TYPE_UNSPECIFIED;	break;
		}
	}

	// If a VetScan, then the patient type is based on the sample type.
	else
	{
		patientType = species.GetPatientType( (SampleType_t)sampleType);
	}
	
	return patientType;
}

unsigned long long
ParseOrderDateTime( char * iStr, char * oStr )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	
	if ( strlen( iStr ) == 0 )
	{
		retVal |= AIE_ORDER_DATE_TIME_MISSING;
	}
	else
	{
		strcpy( oStr, iStr );
	}
	
	return retVal;
}

unsigned long long
ValidateMessageDateTime( char * iStr )
{
	struct tm			iDateTime;
	char				buff[ 5 ];
	time_t				messageTime;
	time_t				currentTime;
	
	memset( &iDateTime, 0, sizeof( struct tm ) );

	if ( strlen( iStr ) != 14 )
	{
		return AIE_MESSAGE_DATE_TIME_INVALID;
	}

//	The input format is YYYYMMDDHHMMSS.
	strncpy( buff, iStr, 4 );
	buff[4] = 0;
	iDateTime.tm_year = atoi( buff ) - 1900;
	
	strncpy( buff, iStr+4, 2 );
	buff[2] = 0;
	iDateTime.tm_mon = atoi( buff ) - 1;
	
	strncpy( buff, iStr+6, 2 );
	buff[2] = 0;
	iDateTime.tm_mday = atoi( buff );
	
	strncpy( buff, iStr+8, 2 );
	buff[2] = 0;
	iDateTime.tm_hour = atoi( buff );
	
	strncpy( buff, iStr+10, 2 );
	buff[2] = 0;
	iDateTime.tm_min = atoi( buff );
	
	strncpy( buff, iStr+12, 2 );
	buff[2] = 0;
	iDateTime.tm_sec = atoi( buff );
	
	messageTime = mktime( &iDateTime );
	if ( messageTime == (time_t)-1 )
	{
		return AIE_MESSAGE_DATE_TIME_INVALID;
	}
	
	currentTime = time( 0 );
	
	if ( fabs( difftime( messageTime, currentTime ) ) > DateMismatchLimit )
	{
		return AIE_MESSAGE_DATE_TIME_INCORRECT;
	}
	
	return AIE_NO_ERROR;
}
	
unsigned long long
ValidateSerialNumber( char * iStr )
{
	unsigned long long retVal = AIE_NO_ERROR;
	
	// If a s/n is received, check it against the instrument's s/n.
	if ( iStr[0] != 0 )
	{
		if ( strcmp( factoryData->instrumentSerialNumber, iStr ) != 0 )
		{
			retVal |= AIE_SERIAL_NUMBER_MISMATCH;
		}
	}

	return retVal;
}

unsigned long long
ValidateReportType( char * iStr )
{
	return (iStr[0] == 'O') ? AIE_NO_ERROR : AIE_REPORT_TYPE_INVALID;
}

unsigned long long
GetHmacIndex( char * iStr, unsigned * index )
{
	// Error if no parameter received.
	if ( iStr[0] == 0 )
	{
		return AIE_MESSAGE_CONTROL_ID_MISSING;
	}

	// Only digits (0-9) accepted.
	if ( isdigit( iStr[0] ) && ( (iStr[1] == 0) || isdigit( iStr[1] )) )
	{
		*index = atoi( iStr );

		// Valid values are 0 to 99.
		if ( *index < 100 )
		{
			return AIE_NO_ERROR;
		}
	}

	// If we got here, the value is invalid.
	return AIE_MESSAGE_CONTROL_ID_INVALID;
}

unsigned long long
ValidatePassword( char *iStr )
{
	int	i;

	// Must be the correct length.
	if ( strlen( iStr ) != ASTM_RX_H_ACCESS_PASSWORD_LEN )
	{
		return AIE_ACCESS_PASSWORD_INVALID;
	}
	
	// All characters must be hex digits.
	for ( i = 0; i < ASTM_RX_H_ACCESS_PASSWORD_LEN; i++ )
	{
		if ( !isxdigit( iStr[ i ] ) )
		{
			return AIE_ACCESS_PASSWORD_INVALID;
		}
	}
	
	return AIE_NO_ERROR;
}

// Concatenates iStr onto the end of oStr, but leaving out any '^' characters in iStr.
void
FilterCat( char * oStr, char * iStr )
{
	unsigned	i;
	char *		p;
	
	for ( i = 0, p = oStr + strlen( oStr ); i < strlen( iStr ); i++ )
	{
		if ( iStr[ i ] != '^' )
		{
			*p++ = iStr[ i ];
		}	
	}
	
	// Finish up by terminating the output string.
	*p = 0;
}

void
CreateHmacInputString( AstmInput_t * astmInput, char * keyStr )
{
	strcpy( keyStr, astmInput->hReceiverId );
	strcat( keyStr, astmInput->hMessageDateAndTime );
	strcat( keyStr, astmInput->pPatientControlId );
	strcat( keyStr, astmInput->pSampleId );
	strcat( keyStr, astmInput->pAlternateId );
	strcat( keyStr, astmInput->pBirthdate );
	strcat( keyStr, astmInput->pPatientSex );
	strcat( keyStr, astmInput->pPatientRace );
	strcat( keyStr, astmInput->pPatientPhoneNumber );
	FilterCat( keyStr, astmInput->pPatientSpecial1 );
	strcat( keyStr, astmInput->pPatientSpecial2 );
	strcat( keyStr, astmInput->pPatientLocation );
	FilterCat( keyStr, astmInput->oRotorNumber );
	strcat( keyStr, astmInput->oOrderDateTime );
	strcat( keyStr, astmInput->oActionCode );
	strcat( keyStr, astmInput->oPhysicianId );
	strcat( keyStr, astmInput->oLaboratoryField1 );
	strcat( keyStr, astmInput->oReportType );
}

unsigned long long
HmacCheck( AstmInput_t * astmInput )
{
	unsigned long long	retVal = AIE_NO_ERROR;
	unsigned			keyIndex;
    char				keyStr[ 256 ];
    unsigned char		hmacStr[ 64 ];

	// Make sure we've got the index, and that it's in range.
	retVal = GetHmacIndex( astmInput->hMessageControlId, &keyIndex );
	if ( retVal != AIE_NO_ERROR )
	{
		return retVal;
	}
	
	// Make sure we've got a valid password - it's there, it's the proper length,
	// and it contains only the hexadecimal characters.
	retVal = ValidatePassword( astmInput->hAccessPassword );
	if ( retVal != AIE_NO_ERROR )
	{
		return retVal;
	}

	// Concatenate fields to create our HMAC input string.
	CreateHmacInputString( astmInput, keyStr );

	// Calculate the encoded value for our HMAC input string.
	CalculateSha256Mac( keyStr, hmacStr, 64, keyIndex);  

	// Compare calculated and received passwords.
	if ( strcmp( (char *)hmacStr, astmInput->hAccessPassword ) != 0 )
	{
		return AIE_ACCESS_PASSWORD_INCORRECT;
	}

	return retVal;
}

void
SetHostPatientOrder( AstmInput_t * astmInput )
{
	static mqd_t		queue = 0;
	AnalysisMsg_t		msg;
	UploadedOrder_t *	order = &(msg.uploadedOrder);
	unsigned long long	errCode = AIE_NO_ERROR;
	
	// Make sure serial number matches (if given), the report type is O, and the message
	// date/time matches the analyzer's date/time with the accepted tolerance.
	errCode |= ValidateSerialNumber( astmInput->hReceiverId );
	errCode |= ValidateReportType( astmInput->oReportType );
	errCode |= ValidateMessageDateTime( astmInput->hMessageDateAndTime );

	// If this is a VetScan, perform the HMAC check. 
	if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	{
		errCode |= HmacCheck( astmInput );
	}
	
	// Initialize all fields of the order structure.
	memset( order, 0, sizeof( UploadedOrder_t ) );
	
	// Validate fields and transfer the data to our order record.
	errCode |= ParseActionCode( astmInput->oActionCode, &(order->actionCode) );
	errCode |= ParseRotorId( astmInput->oRotorNumber, order );
	errCode |= ParseOrderLabel( astmInput, order->orderLabel );
	errCode |= ParsePatientId( astmInput->pPatientControlId, order->patientId );
	errCode |= ParseOrderDateTime( astmInput->oOrderDateTime, order->orderDateTime );
	errCode |= ParseString( astmInput->pSampleId, order->sampleId, AIE_SAMPLE_ID_BAD_FORMAT );
	errCode |= ParseString( astmInput->pAlternateId, order->alternateId, AIE_ALTERNATE_ID_BAD_FORMAT );
	errCode |= ParseString( astmInput->oPhysicianId, order->doctorId, AIE_PHYSICIAN_ID_BAD_FORMAT );
	errCode |= ParseString( astmInput->pPatientLocation, order->location, AIE_LOCATION_BAD_FORMAT );
	errCode |= ParseString( astmInput->pPatientPhoneNumber, order->phoneNumber, AIE_PATIENT_PHONE_NUMBER_BAD_FORMAT );
	errCode |= ParsePatientSpecial1( astmInput->pPatientSpecial1, order );
	errCode |= ParseBirthdate( astmInput->pBirthdate, &order->dateOfBirth );
	errCode |= ParseGender( astmInput->pPatientSex, &order->gender );
	errCode |= ParseRace( astmInput->pPatientRace, &order->race );
	errCode |= ParseSampleType( astmInput->pPatientSpecial2, &order->sampleType, order->actionCode );
	
	// Determine the patient type.
	order->patientType = SetPatientType( order->sampleType, order->gender, order->actionCode );
	
	if ( errCode == AIE_NO_ERROR )
	{
		// Instrument not accepting ASTM orders, so send a cancellation message.
		if ( settingsData->astmOrderMethod == ASTM_ORDERS_DISABLED )
		{
			QueueAstmTxMsg( ABAXIS_SEND_ORDER_CANCELLATION_TYPE,
							order->rotorId,
							'X',
							order->patientId,
							order->orderDateTime );
		}
		
		// Send the order to Analysis.
		else
		{
			if ( queue == 0 )
			{
				queue = mq_open( ANALYSIS_QUEUE_NAME, O_WRONLY );
				if ( queue == -1 )
				{
					LogError( "failure to open %s queue.", ANALYSIS_QUEUE_NAME );
					queue = 0;
				}
			}
			
			if ( queue != 0 )
			{
				msg.msgId = MSG_ANALYSIS_UPLOADED_ORDER;
				mq_send( queue, (const char *)&msg, sizeof( AnalysisMsg_t ), MQ_PRIO_MAX-1 );
			}
		}
	}

	// Invalid input, so notify the host.
	else
	{
		QueueAstmTxMsg( ABAXIS_SEND_ORDER_DATA_INVALID_TYPE, errCode, 0, "", "", astmInput );
	}
}

void
SetMfgStatusRequest( AstmInput_t * astmInput )
{
	char	statusFlag;
	
	if ( stricmp( astmInput->mRecordIdentifier, "AS" ) == 0 )
	{
		switch ( systemData->systemStatus )
		{
			case SYSTEM_STATUS_BUSY:		statusFlag = '2'; break;
			case SYSTEM_STATUS_READY:		statusFlag = '1'; break;
			case SYSTEM_STATUS_NOT_READY:	statusFlag = '0'; break;
			default:						statusFlag = '1'; break;
		}

		QueueAstmTxMsg( ABAXIS_SEND_M_AS_TYPE, 0, statusFlag, "AS" );
	}
	else if ( stricmp( astmInput->mRecordIdentifier, "PO" ) == 0 )
	{
		// Fill the data array.
		FillAstmOrdersList();
		
		QueueAstmTxMsg( ABAXIS_SEND_M_PO_TYPE, 0, 0, "PO" );
	}
		
}
