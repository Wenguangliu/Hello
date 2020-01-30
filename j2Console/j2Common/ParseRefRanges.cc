/*
 * FILENAME
 * 	ParseRefRanges.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse RefRanges XML files.
 * 
 * 	This code relies on the expat library, which comes with QNX, but is not
 * 	automatically linked to a program - the library must be added to the project.
 * 
 * 	It also uses the Crc code from the common library.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>

#include "ParseRefRanges.h"
#include "CommonDefines.h"
#include "Crc32.h"	// CRC routines from the Common library
#include "LogErr.h"


// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE					8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static unsigned 		version_m;
static unsigned			receivedCrc_m;
static unsigned			calculatedCrc_m;
static bool				inData_m;
static bool				inParam_m;
static bool				inCrc_m;
static bool				inVersion_m;
static Gender_t			gender_m;

//static RefRange_t **	rangePtr_m;	// ranges[ { STYPE_NUM_SPECIES ][ AID_NUM_ANALYTES ];
static SampleType_t		sampleType_m;
static bool				inSampleType_m;

static RefRangeData_t *	refRanges_m = 0;

static const char *			GetSampleTypeTag( SampleType_t sampleType );
static const SampleType_t	GetSampleTypeFromTag( const char * tag );
static const char *			GetAnalyteTag( AnalyteId_t analyte );
static const AnalyteId_t	GetAnalyteFromTag( const char * tag );



/*
 * FUNCTION
 * 	SetParam
 * 
 * DESCRIPTION
 * 	Looks for the given parameter in the list of parameters we're looking for.
 *  If found, sets the value.
 * 
 * PARAMETERS
 * 	paramName - Name of the parameter found in the XML file.
 * 	value - Associated value of the parameter from the XML file.
 * 
 * RETURNS
 * 	void
 */

static void
SetParam()
{
	if ( inParam_m )	// Should always be true?
	{
		if ( _stricmp( paramName_m, "lastModifiedAnalyte" ) == 0 )
		{
			refRanges_m->lastModifiedAnalyte = GetAnalyteFromTag( paramValue_m );
		}
		else if ( _stricmp( paramName_m, "lastModifiedSampleType" ) == 0 )
		{
			refRanges_m->lastModifiedSampleType = GetSampleTypeFromTag( paramValue_m );
		}
		else
		{
			AnalyteId_t	analyte;
			char *		s;
			
			analyte = GetAnalyteFromTag( paramName_m );
			if ( analyte != AID_NONE )
			{
				s = strtok( paramValue_m, "-" );
				refRanges_m->ranges[ sampleType_m ][ analyte ][ gender_m - 1 ].low = (float)atof( s );
				s = strtok( 0, "" );
				refRanges_m->ranges[ sampleType_m ][ analyte ][ gender_m - 1 ].high = (float)atof( s );
			}
		}
	}
}


/*
 * FUNCTION
 * 	CharacterHandler
 * 
 * DESCRIPTION
 * 	Callback function for the expat parser.  This is called whenever there is
 * 	character data between XML tags that we're interested in.  We concatenate it
 * 	to the list of characters we might already have read in (this is called more
 * 	than once if the character string spans more than one read from the XML
 * 	file.
 * 
 * PARAMETERS
 * 	userData - Pointer to application-defined user data.  We don't use it here.
 * 	s - String of characters the parser has located.
 *  len - Length of s.
 * 
 * RETURNS
 * 	void
 */

static void
CharacterHandler( void * userData, const XML_Char * s, int len )
{
	strncat( paramValue_m, s, len );
}


/*
 * FUNCTION
 * 	StartTag
 * 
 * DESCRIPTION
 * 	Callback function for the expat parser.  This is called when a start tag is
 * 	located.
 * 
 * PARAMETERS
 * 	data - Pointer to application-defined user data.  We don't use it here.
 * 	el - The tag name.
 * 	attr - List of attribute name/value pairs.  We don't use them.
 * 
 * RETURNS
 * 	void
 */

static void
StartTag( void *data, const char *el, const char **attr )
{
	/*
	 * If we're in the data section (between the "Record" tags), this is a
	 * parameter in the file.  If we're inside a sample type, it should be an
	 * analyte reference range.  We save the name and activate the character
	 * data handler to receive the tag data.
GLD - we could just set a variable with the analyte ID here?
	 */
	if ( inData_m )
	{
		gender_m = GENDER_UNKNOWN;
		if ( attr[0] != 0 && attr[1] != 0 )
		{
			if ( _stricmp( attr[0], "gender" ) == 0 )
			{
				if ( _stricmp( attr[1], "Male" ) == 0 )
				{
					gender_m = GENDER_MALE;
				}
				else if ( _stricmp( attr[1], "Female" ) == 0 )
				{
					gender_m = GENDER_FEMALE;
				}
			}
			else if ( attr[2] != 0 && _stricmp( attr[2], "gender" ) == 0 && attr[3] != 0 )
			{
				if ( _stricmp( attr[3], "Male" ) == 0 )
				{
					gender_m = GENDER_MALE;
				}
				else if ( _stricmp( attr[3], "Female" ) == 0 )
				{
					gender_m = GENDER_FEMALE;
				}
			}
		}

		// This handles the sample types (species, control, demographic, etc.)
		if ( inSampleType_m )
		{
			paramName_m = _strdup( el );
			XML_SetCharacterDataHandler( parser_m, CharacterHandler );
			paramValue_m[0] = '\0';
			inParam_m = true;
		}

		// Start of a Control or Species entry, or else something like lastModifiedAnalyte.
		else
		{
			// Control
			if ( _stricmp( el, "Control" ) == 0 )
			{
				if ( attr[0] != 0 && _stricmp( attr[0], "index" ) == 0 && attr[1] != 0 )
				{
					inSampleType_m = true;
					sampleType_m = (SampleType_t)(STYPE_CONTROL + atoi( attr[1] ));
					refRanges_m->isDefined[ sampleType_m ] = true;
				}
			}

			// Species
			else if ( _stricmp( el, "Species" ) == 0 )
			{
				if ( attr[0] != 0 && attr[1] != 0 )
				{
					if ( _stricmp( attr[0], "index" ) == 0 )
					{
						inSampleType_m = true;
						sampleType_m = (SampleType_t)(STYPE_OTHER + atoi( attr[1] ));
						refRanges_m->isDefined[ sampleType_m ] = true;
					}
					else if ( _stricmp( attr[0], "name" ) == 0 )
					{
						sampleType_m = GetSampleTypeFromTag( attr[1] );
						if ( sampleType_m != STYPE_NONE )
						{
							inSampleType_m = true;
							refRanges_m->isDefined[ sampleType_m ] = true;
						}
					}
				}
			}

			// One of the others
			else
			{
				paramName_m = _strdup( el );
				XML_SetCharacterDataHandler( parser_m, CharacterHandler );
				paramValue_m[0] = '\0';
				inParam_m = true;
			}
		}
	}
	
	/*
	 * A "Records" tag indicates we've started the section with the parameter
	 * data.
	 */
	else if ( _stricmp( el, "Records" ) == 0 )
	{
		inData_m = true;
	}
	
	/*
	 * A "Version" tag indicates this is the file version.  We activate the
	 * character data handler to receive the version number.
	 */
	else if ( _stricmp( el, "Version" ) == 0 )
	{
		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inVersion_m = true;
	}
	
	/*
	 * A "Signature" tag indicates this is the file signature (CRC).  We
	 * activate the character data handler to receive the CRC.
	 */
	else if ( _stricmp( el, "Signature" ) == 0 )
	{
		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inCrc_m = true;
	}
}


/*
 * FUNCTION
 * 	EndTag
 * 
 * DESCRIPTION
 * 	Callback function for the expat parser.  This is called when an end tag is
 * 	located.
 * 
 * PARAMETERS
 * 	data - Pointer to application-defined user data.  We don't use it here.
 * 	el - The tag name.
 * 
 * RETURNS
 * 	void
 */

static void
EndTag( void *data, const char *el )
{
	// Turn off the character data handler.
	XML_SetCharacterDataHandler( parser_m, 0 );
	
	/*
	 * We're in the data section (between the "Record" and "/Record" tags), so
	 * we've either got the end of data for a parameter or the end of the data
	 * section.
	 */
	if ( inData_m )
	{
		// End of data section.
		if ( _stricmp( el, "Records" ) == 0 )
		{
			inData_m = false;
		}
		
		else if ( _stricmp( el, "Control" ) == 0 )
		{
			inSampleType_m = false;
		}
		
		else if ( _stricmp( el, "Species" ) == 0 )
		{
			inSampleType_m = false;
		}
		
		// End of data for a parameter.
		else if ( inParam_m )
		{
			// Update the calculated CRC value.  (NOTE: Do this before calling
			// SetParam(), which may modify paramValue_m.)
			calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );

			// Save the value if it's a parameter we're looking for.
			SetParam();

			// Free the memory we allocated for storing the parameter name and
			// signal we're no longer in a parameter.
			free( paramName_m );
			paramName_m = 0;
			inParam_m = false;
		}
	}
	
	/*
	 * If we're in the Version, convert the string to a version number and save
	 * it.  Update the CRC.
	 */
	else if ( inVersion_m )
	{
		sscanf( paramValue_m, "%u", &version_m );
		calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );
		inVersion_m = false;
	} 
	
	/*
	 * If we're in the CRC, convert the string to a CRC number and save it.
	 */
	else if ( inCrc_m )
	{
		receivedCrc_m = strtoul( paramValue_m, 0, 0 );
		inCrc_m = false;
	} 
}

/*
 * FUNCTION
 * 	ParseRefRanges
 * 
 * DESCRIPTION
 * 	Parses the given config file, looking for the data fields identified.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseRefRanges( const char * fileName, RefRangeData_t * refRanges )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the data structure we're filling.
	 */
	 refRanges_m = refRanges;
	 
	/*
	 * Open the XML file.
	 */
	fp = fopen( fileName, "rb" );
	if ( fp == NULL )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	/*
	 * Create the parser and install the start and end tag callbacks.
	 */
	parser_m = XML_ParserCreate(NULL);
	if ( parser_m == NULL )
	{
		fclose( fp );
		return CFG_ERR_PARSER_CREATE;
	}
	XML_SetElementHandler( parser_m, StartTag, EndTag );

	/*
	 * Clear flags and init the CRC.
	 */
	inData_m = false;
	inParam_m = false;
	inCrc_m = false;
	calculatedCrc_m = Crc32Init();

	/*
	 * Repeatedly read from the file and parse what's read.  If the buffer is
	 * at least as large as the file, this will only happen once.
	 */
	for ( done = 0; !done; )
	{
		int len;

		// Read the file into the buffer.
		len = fread( buff, 1, BUFF_SIZE, fp );
		if ( ferror( fp ) )
		{
			fclose( fp );
			return CFG_ERR_FILE_READ;
		}

		// EOF, so done.
		done = feof( fp );
		
		// Parse the buffer.			
		if ( XML_Parse( parser_m, buff, len, done ) == 0 )
		{
			fclose( fp );
			return CFG_ERR_SYNTAX;
		}
	}
	
	fclose( fp );
	
	// Finalize the CRC calculation and compare it to the CRC read from the file.
	calculatedCrc_m = Crc32Finalize( calculatedCrc_m );
	if ( calculatedCrc_m != receivedCrc_m )
	{
		return CFG_ERR_CRC;
	}

	return CFG_ERR_NONE;
}

typedef struct
{
	SampleType_t	sampleType;
	const char *	tag;
} SampleTypeMatch_t;

/*
 * NOTE: When adding a species, always choose a *new* tag - do not move around
 * 		 and reassign tags or the ref ranges files will be hosed.
 */

static SampleTypeMatch_t sampleMatches[] =
{
	// Canines
	{ STYPE_DOG,				"D1" },
	{ STYPE_COYOTE,				"D2" },
	{ STYPE_DINGO,				"D3" },
	{ STYPE_FOX,				"D4" },
	{ STYPE_JACKAL,				"D5" },
	{ STYPE_WOLF,				"D6" },

	// Felines
	{ STYPE_CAT,				"C1" },
	{ STYPE_BOBCAT,				"C2" },
	{ STYPE_CHEETAH,			"C3" },
	{ STYPE_JAGUAR,				"C4" },
	{ STYPE_LEOPARD,			"C5" },
	{ STYPE_LION,				"C6" },
	{ STYPE_LYNX,				"C7" },
	{ STYPE_PANTHER,			"C8" },
	{ STYPE_PUMA,				"C9" },
	{ STYPE_TIGER,				"C10" },
	
	// Equines
	{ STYPE_HORSE,				"E1" },
	{ STYPE_DONKEY,				"E2" },
	{ STYPE_PONY,				"E3" },
	{ STYPE_ZEBRA,				"E4" },
	
	// Bovines
	{ STYPE_COW,				"B1" },
	{ STYPE_WATER_BUFFALO,		"B2" },
	{ STYPE_STEER,				"B3" },
	{ STYPE_OX,					"B4" },
	{ STYPE_BULL,				"B5" },
	{ STYPE_BUFFALO,			"B6" },
	{ STYPE_BISON,				"B7" },
	
	// Avians
	{ STYPE_BIRD,				"A1" },
	{ STYPE_AFRICAN_GRAY,		"A2" },
	{ STYPE_AMAZON,				"A3" },
	{ STYPE_CHICKEN,			"A4" },
	{ STYPE_COCKATIEL,			"A5" },
	{ STYPE_COCKATOO,			"A6" },
	{ STYPE_CONURE,				"A7" },
	{ STYPE_DOVE,				"A8" },
	{ STYPE_DUCK,				"A9" },
	{ STYPE_FLAMINGO,			"A10" },
	{ STYPE_GOOSE,				"A11" },
	{ STYPE_LOVEBIRD,			"A12" },
	{ STYPE_MACAW,				"A13" },
	{ STYPE_MYNAH,				"A14" },
	{ STYPE_OSTRICH,			"A15" },
	{ STYPE_PARAKEET,			"A16" },
	{ STYPE_PARROT,				"A17" },
	{ STYPE_TOUCAN,				"A18" },
	
	// Reptiles/amphibians
	{ STYPE_REPTILE,			"R1" },
	{ STYPE_ALLIGATOR,			"R2" },
	{ STYPE_AMPHIBIAN,			"R3" },
	{ STYPE_BEARDED_DRAGON,		"R4" },
	{ STYPE_BOA_CONSTRICTOR,	"R5" },
	{ STYPE_CHAMELEON,			"R6" },
	{ STYPE_CROCODILE,			"R7" },
	{ STYPE_FROG,				"R8" },
	{ STYPE_GECKO,				"R9" },
	{ STYPE_IGUANA,				"R10" },
	{ STYPE_LIZARD,				"R11" },
	{ STYPE_MONITOR,			"R12" },
	{ STYPE_PYTHON,				"R13" },
	{ STYPE_SALAMANDER,			"R14" },
	{ STYPE_SNAKE,				"R15" },
	{ STYPE_TOAD,				"R16" },
	{ STYPE_TORTOISE,			"R17" },
	{ STYPE_TURTLE,				"R18" },
	
	// Small mammals
	{ STYPE_SMALL_MAMMAL,		"S1" },
	{ STYPE_BAT,				"S2" },
	{ STYPE_CHINCHILLA,			"S3" },
	{ STYPE_FERRET,				"S4" },
	{ STYPE_GERBIL,				"S5" },
	{ STYPE_GUINEA_PIG,			"S6" },
	{ STYPE_HAMSTER,			"S7" },
	{ STYPE_HEDGEHOG,			"S8" },
	{ STYPE_LEMUR,				"S9" },
	{ STYPE_MINK,				"S10" },
	{ STYPE_MOUSE,				"S11" },
	{ STYPE_OPOSSUM,			"S12" },
	{ STYPE_PRAIRIE_DOG,		"S13" },
	{ STYPE_RACCOON,			"S14" },
	{ STYPE_RAT,				"S15" },
	{ STYPE_SKUNK,				"S16" },
	{ STYPE_SQUIRREL,			"S17" },
	{ STYPE_RABBIT,				"S18" },
	
	// Large mammals
	{ STYPE_LARGE_MAMMAL,		"L1" },
	{ STYPE_ANTELOPE,			"L2" },
	{ STYPE_APE,				"L3" },
	{ STYPE_BABOON,				"L4" },
	{ STYPE_BEAR,				"L5" },
	{ STYPE_CAMEL,				"L6" },
	{ STYPE_CHIMPANZEE,			"L7" },
	{ STYPE_DEER,				"L8" },
	{ STYPE_ELEPHANT,			"L9" },
	{ STYPE_GIRAFFE,			"L10" },
	{ STYPE_GOAT,				"L11" },
	{ STYPE_KANGAROO,			"L12" },
	{ STYPE_LLAMA,				"L13" },
	{ STYPE_MONKEY,				"L14" },
	{ STYPE_PIG,				"L15" },
	{ STYPE_SEA_LION,			"L16" },
	{ STYPE_SEAL,				"L17" },
	{ STYPE_SHEEP,				"L18" },

	// End marker
	{ STYPE_NONE,				"" },
};

// Given a SampleType_t, return the tag that corresponds to it, or null if it
// can't be found.  For controls, we use Xy, where y is 0 thru 100.  For unnamed
// species, we use Zy, where y is 0 thru 100.
const char *
GetSampleTypeTag( SampleType_t sampleType )
{
	int			i;
	static char	buff[5];
	
	if ( sampleType >= STYPE_CONTROL && sampleType <= STYPE_LAST_CONTROL )
	{
		sprintf( buff, "X%d", sampleType - STYPE_CONTROL );
		return buff;
	}
	
	else if ( sampleType >= STYPE_OTHER && sampleType <= STYPE_LAST_SPECIES )
	{
		sprintf( buff, "Z%d", sampleType - STYPE_OTHER );
		return buff;
	}

	for ( i = 0; sampleMatches[ i ].sampleType != STYPE_NONE; i++ )
	{
		if ( sampleMatches[ i ].sampleType == sampleType )
		{
			return sampleMatches[ i ].tag;
		}
	}
	
	return 0;
}

// Given a sample type tag, return the SampleType_t that corresponds to it, or
// STYPE_NONE if it can't be found.
const SampleType_t
GetSampleTypeFromTag( const char * tag )
{
	int i;
	
	if ( tag[0] == 'X' )
	{
		return (SampleType_t)(STYPE_CONTROL + atoi( tag + 1 ));
	}
	
	else if ( tag[0] == 'Z' )
	{
		return (SampleType_t)(STYPE_OTHER + atoi( tag + 1 ));
	}
	
	else
	{
		for ( i = 0; sampleMatches[ i ].sampleType != STYPE_NONE; i++ )
		{
			if ( _stricmp( sampleMatches[ i ].tag, tag ) == 0 )
			{
				return sampleMatches[ i ].sampleType;
			}
		}
	}
	
	return STYPE_NONE;
}

typedef struct
{
	AnalyteId_t		analyte;
	const char *	tag;
} AnalyteMatch_t;

static AnalyteMatch_t analyteMatches[] =
{
    { AID_AG,   "AG" },
    { AID_ALB,  "ALB" },
    { AID_ALP,  "ALP" },
    { AID_ALT,  "ALT" },
    { AID_AMY,  "AMY" },
    { AID_AST,  "AST" },
    { AID_BA,   "BA" },
    { AID_BUN,  "BUN" },
    { AID_CA,   "CA" },
    { AID_CHOL, "CHOL" },
    { AID_CHW,  "CHW" },
    { AID_CK,   "CK" },
    { AID_CL,   "CL" },
    { AID_CRE,  "CRE" },
    { AID_CRP,  "CRP" },
    { AID_DBIL, "DBIL" },
    { AID_EGFR, "EGFR" },
    { AID_GGT,  "GGT" },
    { AID_GLOB, "GLOB" },
    { AID_GLU,  "GLU" },
    { AID_HB,   "HB" },
    { AID_HCT,  "HCT" },
    { AID_HDL,  "HDL" },
    { AID_IBIL, "IBIL" },
    { AID_K,    "K" },
    { AID_LAC,  "LAC" },
    { AID_LD,   "LD" },
    { AID_LDL,  "LDL" },
    { AID_MG,   "MG" },
    { AID_NA,   "NA" },
    { AID_NHDLC,"NHDLC" },
    { AID_PHB,  "PHB" },
    { AID_PHOS, "PHOS" },
    { AID_T4,   "T4" },
    { AID_TBIL, "TBIL" },
    { AID_TCH,  "TCH" },
    { AID_TCO2, "TCO2" },
    { AID_TP,   "TP" },
    { AID_TRIG, "TRIG" },
    { AID_UA,   "UA" },
    { AID_VLDL, "VLDL" },
    { AID_NONE, "" },

};

// Given an AnalyteId_t, return the tag that corresponds to it, or null if it
// can't be found.
const char *
GetAnalyteTag( AnalyteId_t analyte )
{
	int i;
	
	for ( i = 0; analyteMatches[ i ].analyte != AID_NONE; i++ )
	{
		if ( analyteMatches[ i ].analyte == analyte )
		{
			return analyteMatches[ i ].tag;
		}
	}
	
	return 0;
}

// Given an analyte tag, return the AnalyteId_t that corresponds to it, or
// AID_NONE if it can't be found.
const AnalyteId_t
GetAnalyteFromTag( const char * tag )
{
	int i;
	
	for ( i = 0; analyteMatches[ i ].analyte != AID_NONE; i++ )
	{
		if ( _stricmp( analyteMatches[ i ].tag, tag ) == 0 )
		{
			return analyteMatches[ i ].analyte;
		}
	}
	
	return AID_NONE;
}

/*
 * FUNCTION
 * 	WriteRefRanges
 * 
 * DESCRIPTION
 * 	Writes a reference ranges file.
 * 
 * PARAMETERS
 *  fileName - Name of the file to write.
 * 	refRanges - The ref ranges data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteRefRanges( const char * fileName, RefRangeData_t * refRanges )
{
	FILE *			fp;
	unsigned		crc = 0;
	char			dataBuff[ 2000 ];
	char			tagBuff[ 100 ];
	int				i, j, k;
	RefRange_t		r;
	const char *	s;
	const char *	genders[] = { "Unknown", "Male", "Female" };

	// Open the file.
	fp = fopen( fileName, "wb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Reference Ranges -->\n" );
	fprintf( fp, "<RefRanges>\n" );
	sprintf( dataBuff, "%u", REF_RANGE_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	// Write the last modifed analyte and sample type.
	s = GetAnalyteTag( refRanges->lastModifiedAnalyte );
	if ( s != 0 )
	{
		fprintf( fp, "    <lastModifiedAnalyte>%s</lastModifiedAnalyte>\n", s );
		crc = Crc32Update( crc, (unsigned char *)s, strlen( s ) );
	}

	s = GetSampleTypeTag( refRanges->lastModifiedSampleType );
	if ( s != 0 )
	{
		fprintf( fp, "    <lastModifiedSampleType>%s</lastModifiedSampleType>\n", s );
		crc = Crc32Update( crc, (unsigned char *)s, strlen( s ) );
	}

	// Write any controls that are defined.
	for ( i = STYPE_CONTROL; i <= STYPE_LAST_CONTROL; i++ )
	{
		if ( refRanges->isDefined[ i ] )
		{
			fprintf( fp, "    <Control index=\"%d\">\n", i - STYPE_CONTROL );
			for ( j = 0; j < AID_NUM_ANALYTES; j++ )
			{
				r = refRanges->ranges[ i ][ j ][0];
                // HB and HCT currently deactivated.
                if ( (j != AID_HB) && (j != AID_HCT) )
				{
					s = GetAnalyteTag( (AnalyteId_t)j );
					sprintf( dataBuff, "%f-%f", r.low, r.high );
					fprintf( fp, "      <%s>%s</%s>\n", s, dataBuff, s );
					crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
				}
			}
			fprintf( fp, "\n    </Control>\n" );
		}
	}
	
	// Write any unnamed species that are defined.
	for ( i = STYPE_OTHER; i <= STYPE_LAST_SPECIES; i++ )
	{
		if ( refRanges->isDefined[ i ] )
		{
			fprintf( fp, "    <Species index=\"%d\">\n", i - STYPE_OTHER );
			for ( j = 0; j < AID_NUM_ANALYTES; j++ )
			{
				for ( k = 0; k < 3; k++ )
				{
					r = refRanges->ranges[ i ][ j ][ k ];
                    // HB and HCT currently deactivated.
                    if ( (j != AID_HB) && (j != AID_HCT) )
					{
						s = GetAnalyteTag( (AnalyteId_t)j );
						sprintf( dataBuff, "%f-%f", r.low, r.high );
						if ( k > 0 )
						{
							sprintf( tagBuff, "%s gender=\"%s\"", s, genders[ k ] );
						}
						else
						{
							strcpy( tagBuff, s );
						}
						fprintf( fp, "      <%s>%s</%s>\n", tagBuff, dataBuff, s );
						crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
					}
				}
			}
			fprintf( fp, "\n    </Species>\n" );
		}
	}

	// Write any named species that are defined.
	for ( i = STYPE_FIRST_NAMED_SPECIES; i <= STYPE_LAST_NAMED_SPECIES; i++ )
	{
		if ( refRanges->isDefined[ i ] )
		{
			fprintf( fp, "    <Species name=\"%s\">\n", GetSampleTypeTag( (SampleType_t)i ) );
			for ( j = 0; j < AID_NUM_ANALYTES; j++ )
			{
				for ( k = 0; k < 3; k++ )
				{
					r = refRanges->ranges[ i ][ j ][ k ];
                    // HB and HCT currently deactivated.
                    if ( (j != AID_HB) && (j != AID_HCT) )
					{
						s = GetAnalyteTag( (AnalyteId_t)j );
						sprintf( dataBuff, "%f-%f", r.low, r.high );
						if ( k > 0 )
						{
							sprintf( tagBuff, "%s gender=\"%s\"", s, genders[ k ] );
						}
						else
						{
							strcpy( tagBuff, s );
						}
						fprintf( fp, "      <%s>%s</%s>\n", tagBuff, dataBuff, s );
						crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
					}
				}
			}
			fprintf( fp, "\n    </Species>\n" );
		}
	}

	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</RefRanges>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}
