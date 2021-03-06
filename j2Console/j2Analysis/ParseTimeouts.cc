/*
 * FILENAME
 * 	ParseTimeouts.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse the timeouts.xml config file.
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
#include <expat.h>
#include <ctype.h>

#include "TimeoutData.h"
#include "ParseTimeouts.h"

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE		8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static unsigned 		version_m;
static bool				inData_m;
static bool				inParam_m;
static bool				inVersion_m;

static TimeoutData_t *	timeoutData_m;
	
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
	int	value = atoi( paramValue_m );

	// We assume that a parsed value of zero or less is an error.
	if ( value <= 0 )
	{
		return;
	}

	if ( _stricmp( paramName_m, "opticalDacs" ) == 0 )
	{
		timeoutData_m->opticalDacs = value;
	}
	
	else if ( _stricmp( paramName_m, "beadCheck1" ) == 0 )
	{
		timeoutData_m->beadCheck1 = value;
	}
	
	else if ( _stricmp( paramName_m, "beadCheck2" ) == 0 )
	{
		timeoutData_m->beadCheck2 = atoi( paramValue_m );
	}

	else if ( _stricmp( paramName_m, "distributionCheck" ) == 0 )
	{
		timeoutData_m->distributionCheck = value;
	}

	else if ( _stricmp( paramName_m, "mixDone" ) == 0 )
	{
		timeoutData_m->mixDone = value;
	}

	else if ( _stricmp( paramName_m, "blackOffsets" ) == 0 )
	{
		timeoutData_m->blackOffsets = value;
	}

	else if ( _stricmp( paramName_m, "pass" ) == 0 )
	{
		timeoutData_m->pass = value;
	}

	else if ( _stricmp( paramName_m, "analysisDone" ) == 0 )
	{
		timeoutData_m->analysisDone = value;
	}

	else if ( _stricmp( paramName_m, "resultsCalcComplete" ) == 0 )
	{
		timeoutData_m->resultsCalcComplete = value;
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
	 * it.
	 */
	else if ( inVersion_m )
	{
		sscanf( paramValue_m, "%u", &version_m );
		inVersion_m = false;
	} 
}


/*
 * FUNCTION
 * 	ParseTimeouts
 * 
 * DESCRIPTION
 * 	Parses the timeouts config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, a default value will be set.
 *	No indication will be made if a parameter is not found other than that the
 *	field will be set to the default.
 *
 * PARAMETERS
 * 	timeoutData - Pointer to the data structure where the parsed data should be
 * 				  stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseTimeouts( TimeoutData_t * timeoutData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	timeoutData_m = timeoutData;
	
	/*
	 * Set the values to defaults, so if there's an error reading the config
	 * file we can use the defaults.
	 */
	 timeoutData->opticalDacs = -1;
	 timeoutData->beadCheck1 = -1;
	 timeoutData->beadCheck2 = -1;
	 timeoutData->distributionCheck = -1;
	 timeoutData->mixDone = -1;
	 timeoutData->blackOffsets = -1;
	 timeoutData->pass = -1;
	 timeoutData->analysisDone = -1;
	 timeoutData->resultsCalcComplete = -1;

	/*
	 * Open the config file.
	 */
	fp = fopen( TIMEOUTS_CONFIG_FILE_NAME, "rb" );
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
	 * Clear flags.
	 */
	inData_m = false;
	inParam_m = false;

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
	

	return CFG_ERR_NONE;
}
