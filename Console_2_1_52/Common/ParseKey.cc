/*
 * FILENAME
 * 	ParseKey.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse the key.xml config file.
 * 
 * 	This code uses a library that is not automatically linked to, expat, which
 *  comes with QNX.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <xmlparse.h>
#include <ctype.h>

#include "Crc32.h"
#include "ParseKey.h"

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE		8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static unsigned *		version_m;
static unsigned			receivedCrc_m;
static unsigned			calculatedCrc_m;
static bool				inData_m;
static bool				inParam_m;
static bool				inVersion_m;
static bool				inCrc_m;

static KeyData_t *		keyData_m;
	
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
	if ( stricmp( paramName_m, "serialNumber" ) == 0 )
	{
		strcpy( keyData_m->serialNumber, paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "mode" ) == 0 )
	{
		if ( stricmp( paramValue_m, "Demo" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_DEMO;
		}
		else if ( stricmp( paramValue_m, "Research" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_RESEARCH;
		}
		else if ( stricmp( paramValue_m, "Manufacturing" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_MANUFACTURING;
		}
		else if ( stricmp( paramValue_m, "Service" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_SERVICE;
		}
		else if ( stricmp( paramValue_m, "Administrator" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_ADMINISTRATOR;
		}
		else if ( stricmp( paramValue_m, "Lockout" ) == 0 )
		{
			keyData_m->mode = KEY_MODE_LOCKOUT;
		}
	}
	else if ( stricmp( paramName_m, "demoFromKey" ) == 0 )
	{
		if ( stricmp( paramValue_m, "true" ) == 0 )
		{
			keyData_m->demoFromKey = true;
		}
		else
		{
			keyData_m->demoFromKey = false;
		}
	}
	else if ( stricmp( paramName_m, "useFastDemo" ) == 0 )
	{
		if ( stricmp( paramValue_m, "true" ) == 0 )
		{
			keyData_m->useFastDemo = true;
		}
		else
		{
			keyData_m->useFastDemo = false;
		}
	}
	else if ( stricmp( paramName_m, "enableFtpTelnet" ) == 0 )
	{
		if ( stricmp( paramValue_m, "true" ) == 0 )
		{
			keyData_m->enableFtpTelnet = true;
		}
		else
		{
			keyData_m->enableFtpTelnet = false;
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
		paramName_m = strdup( el );

		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inParam_m = true;
	}
	
	/*
	 * A "Records" tag indicates we've started the section with the parameter
	 * data.
	 */
	else if ( stricmp( el, "Records" ) == 0 )
	{
		inData_m = true;
	}
	
	/*
	 * A "Version" tag indicates this is the file version.  We activate the
	 * character data handler to receive the version number.
	 */
	else if ( stricmp( el, "Version" ) == 0 )
	{
		XML_SetCharacterDataHandler( parser_m, CharacterHandler );
		paramValue_m[0] = '\0';
		inVersion_m = true;
	}
	
	/*
	 * A "Signature" tag indicates this is the file signature (CRC).  We
	 * activate the character data handler to receive the CRC.
	 */
	else if ( stricmp( el, "Signature" ) == 0 )
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
		if ( stricmp( el, "Records" ) == 0 )
		{
			inData_m = false;
		}
	
		// End of data for a parameter.
		else if ( inParam_m )
		{
			calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );

			// Save the value if it's a parameter we're looking for.
			SetParam();

			// Free the memory we allocated for storing the parameter name and
			// attribute, and signal we're no longer in a parameter.
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
 * 	ParseKey
 * 
 * DESCRIPTION
 * 	Parses the key config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	keyData - Pointer to the data structure where the parsed data should be
 * 			  stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseKey( KeyData_t * keyData )
{
	unsigned	version;

	return ParseKey( keyData, &version );
}

CfgError_t
ParseKey( KeyData_t * keyData, unsigned * version )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	version_m = version;	
	keyData_m = keyData;
	
	keyData->mode = KEY_MODE_NONE;
	keyData->demoFromKey = false;
	keyData->useFastDemo = false;
	keyData->enableFtpTelnet = false;

	/*
	 * Open the config file.
	 */
	fp = fopen( KEY_CONFIG_FILE_NAME, "r" );
	if ( fp == 0 )
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
	if ( (calculatedCrc_m != receivedCrc_m) && (receivedCrc_m != 314159265) )
	{
		return CFG_ERR_CRC;
	}

	return CFG_ERR_NONE;
}
