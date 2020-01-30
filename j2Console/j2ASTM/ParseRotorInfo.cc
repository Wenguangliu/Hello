/*
 * FILENAME
 * 	ParseRotorInfo.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse a rotor info configuration file.
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
#include <errno.h>

#include "ParseRotorInfo.h"
#include "CommonDefines.h"
#include "Crc32.h"	// CRC routines from the Common library
#include "LogErr.h"


// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE		8196

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
static bool				inRotor_m;
static RotorInfo_t *	rotorInfo_m = 0;
static RotorInfo_t		rotorRecord_m;
static int				rotorNumber_m;


static void
SetRotorParam()
{
	if ( _stricmp( paramName_m, "rotorNumber" ) == 0 )
	{
		rotorNumber_m = atoi( paramValue_m );
	}
	else if (_stricmp( paramName_m, "rotorProductCode" ) == 0 )
	{
		rotorRecord_m.rotorProductCode = paramValue_m[ 0 ];
	}
	else if ( _stricmp( paramName_m, "rotorScreenNameLine1" ) == 0 )
	{
		strncpy( rotorRecord_m.rotorNameLine1, paramValue_m, sizeof( rotorRecord_m.rotorNameLine1) );
	}
	else if ( _stricmp( paramName_m, "rotorScreenNameLine2" ) == 0 )
	{
		strncpy( rotorRecord_m.rotorNameLine2, paramValue_m, sizeof( rotorRecord_m.rotorNameLine2) );
	}
	else if ( _stricmp( paramName_m, "rotorScreenNameFontColor" ) == 0 )
	{
		rotorRecord_m.rotorNameFontColor = (unsigned)atoi( paramValue_m );;
	}
	else if ( _stricmp( paramName_m, "rotorScreenNameBackgroundColor" ) == 0 )
	{
		rotorRecord_m.rotorNameBackgroundColor = (unsigned)atoi( paramValue_m );;
	}
}

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
	if ( inRotor_m )
	{
		SetRotorParam();
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
	 * If we're in the data section (between the "Records" tags), this is a
	 * parameter in the file.  We activate the character data
	 * handler to receive the tag data.
	 */
	if ( inData_m )
	{
		if ( _stricmp( el, "rotor" ) == 0 )
		{
			inRotor_m = true;
			memset( &rotorRecord_m, 0, sizeof( RotorInfo_t ) );
			rotorNumber_m = 0;
			
		}
		
		else
		{
			paramName_m = _strdup( el );
			XML_SetCharacterDataHandler( parser_m, CharacterHandler );
			paramValue_m[0] = '\0';
			inParam_m = true;
		}
	}
	
	/*
	 * A "Records" tag indicates we've started the section with the parameter
	 * data.
	 */
	else if ( _stricmp( el, "records" ) == 0 )
	{
		inData_m = true;
	}
	
	/*
	 * A "Version" tag indicates this is the file version.  We activate the
	 * character data handler to receive the version number.
	 */
	else if ( _stricmp( el, "version" ) == 0 )
	{
		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inVersion_m = true;
	}
	
	/*
	 * A "Signature" tag indicates this is the file signature (CRC).  We
	 * activate the character data handler to receive the CRC.
	 */
	else if ( _stricmp( el, "signature" ) == 0 )
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
		if ( _stricmp( el, "records" ) == 0 )
		{
			inData_m = false;
		}
		
		else if ( _stricmp( el, "rotor" ) == 0 )
		{
			inRotor_m = false;

			if ( rotorNumber_m > 0 && rotorNumber_m <= MAX_ROTOR_NUM )
			{
				memcpy( &rotorInfo_m[ rotorNumber_m ], &rotorRecord_m, sizeof( RotorInfo_t ) );
			}
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
 * 	ParseRotorInfo
 * 
 * DESCRIPTION
 * 	Parses the given config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 * 
 * PARAMETERS
 * 	fileName - Name of the demo data file.
 *  rotorInfo - Pointer to location to store the rotor info data read from the file.
 * 				This is assumed to be an array of MAX_ROTOR_NUM+1 entries.
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseRotorInfo( RotorInfo_t * rotorInfo )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the data structure we're filling.
	 */
	rotorInfo_m = rotorInfo;
	 
	 /*
	  * Init the rotor info data to default values.
	  */
	memset( rotorInfo_m, 0, sizeof( RotorInfo_t ) * (MAX_ROTOR_NUM+1) );
	 
	/*
	 * Open the rotor info file.
	 */
	fp = fopen( ROTOR_INFO_FILE_NAME, "r" );
	if ( fp == 0 )
	{
	    return CFG_ERR_FILE_OPEN;
	}
	
	/*
	 * Create the parser and install the start and end tag callbacks.
	 */
	parser_m = XML_ParserCreate( NULL );
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
	inVersion_m = false;
	inRotor_m = false;
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
//	if ( calculatedCrc_m != receivedCrc_m )
//	{
//		return CFG_ERR_CRC;
//	}

	return CFG_ERR_NONE;
}
