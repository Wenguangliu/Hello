/*
 * FILENAME
 * 	ParseCounters.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse and write the counters.xml config file.
 * 
 * 	This code uses a library that is not automatically linked to, expat, which
 *  comes with QNX.
 *
 * 	It also links in the Crc object file from the Common library.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>

#include "Crc32.h"	// CRC routines from the Common library
#include "CommonDefines.h"
#include "CounterData.h"
#include "ParseCounters.h"

#define COUNTERS_FILE_VERSION	1

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

static CounterData_t *	counterData_m;
	
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
	if ( _stricmp( paramName_m, "rotorRunCount" ) == 0 )
	{
		counterData_m->rotorRunCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "rotorAbortCount" ) == 0 )
	{
		counterData_m->rotorAbortCount = (int)atoi( paramValue_m );
	}
	else if ( _stricmp( paramName_m, "rotorAbortCount" ) == 0 )
	{
		counterData_m->rotorAbortCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "printCount" ) == 0 )
	{
		counterData_m->printCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "flashCount" ) == 0 )
	{
		counterData_m->flashCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "missedFlashCount" ) == 0 )
	{
		counterData_m->missedFlashCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "camCycleCount" ) == 0 )
	{
		counterData_m->camCycleCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "drawerCycleCount" ) == 0 )
	{
		counterData_m->drawerCycleCount = (int)atoi( paramValue_m );
	}
	
	else if ( _stricmp( paramName_m, "spindleRevolutionCount" ) == 0 )
	{
		counterData_m->spindleRevolutionCount = (int)atoi( paramValue_m );
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
 * 	ParseCounters
 * 
 * DESCRIPTION
 * 	Parses the counters config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	counterData - Pointer to the data structure where the parsed data should be
 * 					stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseCounters( CounterData_t * counterData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	unsigned	_version;
	
	counterData_m = counterData;
	version_m = &_version;
	
	/*
	 * Open the config file.
	 */
	fp = fopen( COUNTERS_CONFIG_FILE_NAME, "rb" );
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


/*
 * FUNCTION
 * 	WriteCounters
 * 
 * DESCRIPTION
 * 	Writes the counters.xml file.
 * 
 * PARAMETERS
 * 	counterData - The counters data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteCounters( CounterData_t * counterData )
{
	FILE *		fp;
	unsigned	crc = 0;
	char		dataBuff[ 2000 ];
	
	// Open the file.
	fp = fopen( COUNTERS_CONFIG_FILE_NAME, "wb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Counters Data -->\n" );
	fprintf( fp, "<Counters>\n" );
	sprintf( dataBuff, "%u", COUNTERS_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	// rotorRunCount
	sprintf( dataBuff, "%d", counterData->rotorRunCount );
	fprintf( fp, "    <rotorRunCount>%s</rotorRunCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// rotorAbortCount
	sprintf( dataBuff, "%d", counterData->rotorAbortCount );
	fprintf( fp, "    <rotorAbortCount>%s</rotorAbortCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// printCount
	sprintf( dataBuff, "%d", counterData->printCount );
	fprintf( fp, "    <printCount>%s</printCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// flashCount
	sprintf( dataBuff, "%d", counterData->flashCount );
	fprintf( fp, "    <flashCount>%s</flashCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// missedFlashCount
	sprintf( dataBuff, "%d", counterData->missedFlashCount );
	fprintf( fp, "    <missedFlashCount>%s</missedFlashCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// camCycleCount
	sprintf( dataBuff, "%d", counterData->camCycleCount );
	fprintf( fp, "    <camCycleCount>%s</camCycleCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// drawerCycleCount
	sprintf( dataBuff, "%d", counterData->drawerCycleCount );
	fprintf( fp, "    <drawerCycleCount>%s</drawerCycleCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// spindleRevolutionCount
	sprintf( dataBuff, "%d", counterData->spindleRevolutionCount );
	fprintf( fp, "    <spindleRevolutionCount>%s</spindleRevolutionCount>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</Counters>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}
