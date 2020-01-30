/*
 * FILENAME
 * 	ParseUnits.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse and write the units config file.
 * 
 * 	This code uses a library that is not automatically linked to, expat, which
 *  comes with QNX.
 *
 * 	It also links in the Crc object file from the Common library.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>

#include "Crc32.h"	// CRC routines from the Common library
#include "CommonDefines.h"
#include "ParseUnits.h"


// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE		8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static char *			attributes_m[2];
static unsigned *		version_m;
static unsigned			receivedCrc_m;
static unsigned			calculatedCrc_m;
static bool				inData_m;
static bool				inParam_m;
static bool				inCrc_m;
static bool				inVersion_m;

static UnitsConfig_t *	unitsConfig_m;

static const char *			GetAnalyteTag( AnalyteId_t analyte );
static const AnalyteId_t	GetAnalyteFromTag( const char * tag );
static const char *			GetUnitTag( UnitId_t unit );
static const UnitId_t		GetUnitFromTag( const char * tag );
	
/*
 * FUNCTION
 * 	SetParam
 * 
 * DESCRIPTION
 * 	Looks for the given parameter in the list of parameters we're looking for.
 *  If found, sets the value.
 * 
 * RETURNS
 * 	void
 */

static void
SetParam()
{
	AnalyteId_t	analyte;
	UnitId_t	unit;
	
	analyte = GetAnalyteFromTag( paramName_m );
	if ( analyte != AID_NONE )
	{
		unit = GetUnitFromTag( paramValue_m );
		if ( unit != UID_NONE )
		{
			unitsConfig_m->current[ analyte ] = unit;
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
	 * parameter in the file.  We save the name and activate the character data
	 * handler to receive the tag data.
	 */
	if ( inData_m )
	{
		paramName_m = _strdup( el );
		if ( attr[0] != 0 )
		{
			attributes_m[0] = _strdup( attr[0] );
		
			if ( attr[1] != 0 )
			{
				attributes_m[1] = _strdup( attr[1] );
			}
		}

		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inParam_m = true;
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
	
		// End of data for a parameter.
		else if ( inParam_m )
		{
			// Update the calculated CRC value.  (NOTE: Do this before calling
			// SetParam(), which may modify paramValue_m. )
			calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );

			// Save the value if it's a parameter we're looking for.
			SetParam();

			// Free the memory we allocated for storing the parameter name and
			// attribute, and signal we're no longer in a parameter.
			free( paramName_m );
			paramName_m = 0;
			if ( attributes_m[0] != 0 )
			{
				free( attributes_m[0] );
				attributes_m[0] = 0;
			}
			if ( attributes_m[1] != 0 )
			{
				free( attributes_m[1] );
				attributes_m[1] = 0;
			}
			inParam_m = false;
		}
	}
	
	/*
	 * If we're in the Version, convert the string to a version number and save
	 * it.  Update the CRC.
	 */
	else if ( inVersion_m )
	{
		sscanf( paramValue_m, "%u", version_m );
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
 * 	ParseUnits
 * 
 * DESCRIPTION
 * 	Parses the units config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	unitsData - Pointer to the data structure where the parsed data should be
 * 					stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseUnits( UnitsConfig_t * unitsConfig )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	unsigned	_version;
	
	unitsConfig_m = unitsConfig;
	version_m = &_version;
	
	/*
	 * Open the config file.
	 */
	fp = fopen( UNITS_CONFIG_FILE_NAME, "rb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	/*
	 * Create the parser and install the start and end tag callbacks.
	 */
	parser_m = XML_ParserCreate( 0 );
	if ( parser_m == 0 )
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


/*
 * FUNCTION
 * 	WriteUnits
 * 
 * DESCRIPTION
 * 	Writes the units config file.
 * 
 * PARAMETERS
 * 	unitsData - The units data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteUnits( UnitsConfig_t * unitsConfig )
{
	FILE *			fp;
	unsigned		crc = 0;
	char			dataBuff[ 2000 ];
	int				i;
	const char *	a;
	const char *	u;
	
	// Open the file.
	fp = fopen( UNITS_CONFIG_FILE_NAME, "wb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Units Data -->\n" );
	fprintf( fp, "<Units>\n" );
	sprintf( dataBuff, "%u", UNITS_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	for ( i = 0; i < AID_NUM_ANALYTES; i++ )
	{
		u = GetUnitTag( unitsConfig->current[ i ] );
		a = GetAnalyteTag( (AnalyteId_t)i );
		if ( a != 0 && u != 0 )
		{
			fprintf( fp, "    <%s>%s</%s>\n", a, u, a );
			crc = Crc32Update( crc, (unsigned char *)u, strlen( u ) );
		}
	}

	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</Units>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}

// GLD
// This structure and associated code were copied directly from ParseRefRanges.cc.
// Ideally, we should combine somewhere.
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

typedef struct
{
	UnitId_t		unit;
	const char *	tag;
} UnitMatch_t;

static UnitMatch_t unitMatches[] =
{
    { UID_UL,       "UL" },
    { UID_GDL,      "GDL" },
    { UID_GL,       "GL" },
    { UID_MEQL,     "MEQL" },
    { UID_MGDL,     "MGDL" },
    { UID_UGDL,     "UGDL" },
    { UID_MGL,      "MGL" },
    { UID_UGL,      "UGL" },
    { UID_UGML,     "UGML" },
    { UID_UKATL,    "UKATL" },
    { UID_MMOLL,    "MMOLL" },
    { UID_UMOLL,    "UMOLL" },
    { UID_NKATL,    "NKATL" },
    { UID_NMOLL,    "NMOLL" },
    { UID_MLMIN,    "MLMIN" },
    { UID_PCT,      "PCT" },
    { UID_FRAC,     "FRAC" },
    { UID_NONE,     "" },

};
	
const char *
GetUnitTag( UnitId_t unit )
{
	int i;
	
	for ( i = 0; unitMatches[ i ].unit != UID_NONE; i++ )
	{
		if ( unitMatches[ i ].unit == unit )
		{
			return unitMatches[ i ].tag;
		}
	}
	
	return 0;
}

const UnitId_t
GetUnitFromTag( const char * tag )
{
	int i;
	
	for ( i = 0; unitMatches[ i ].unit != UID_NONE; i++ )
	{
		if ( _stricmp( unitMatches[ i ].tag, tag ) == 0 )
		{
			return unitMatches[ i ].unit;
		}
	}
	
	return UID_NONE;
}
