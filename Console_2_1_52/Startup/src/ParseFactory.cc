/*
 * FILENAME
 * 	ParseFactory.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse the factory.xml config file.
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
#include <xmlparse.h>
#include <ctype.h>

#include "Crc32.h"	// CRC routines from the Common library
#include "FactoryData.h"
#include "ParseFactory.h"

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

static FactoryData_t *	factoryData_m;
	
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
	int		i;
	char *	s;

	if ( stricmp( paramName_m, "airTemperatureOffset" ) == 0 )
	{
		factoryData_m->airTemperatureOffset = (float)atof( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "cuvetteTransmitLimit" ) == 0 )
	{
		factoryData_m->cuvetteTransmitLimit = (float)atof( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "wavelengthFilterCurves" ) == 0 )
	{
		int wavelength = -1;
		
		if ( attributes_m[0] != 0 && stricmp( attributes_m[0], "wavelength" ) == 0 && attributes_m[1] != 0 )
		{
			wavelength = atoi( attributes_m[1] );
		}
		
		if ( wavelength >= 0 && wavelength < NUM_WAVELENGTHS - 1 )
		{
			s = strtok( paramValue_m, "," );
			for ( i = 0; i < MAX_BFD_CALS && s != 0; i++ )
			{
				factoryData_m->wavelengthFilterCurves[ wavelength ][ i ] = (unsigned short)strtoul( s, 0, 0 );
				s = strtok( 0, "," );
			}
		}
	}
	
	else if ( stricmp( paramName_m, "globalCuvetteDelay" ) == 0 )
	{
		factoryData_m->globalCuvetteDelay = strtoul( paramValue_m, 0, 0 );
	}
	
	else if ( stricmp( paramName_m, "cuvetteDelays" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_CUVETTES && s != 0; i++ )
		{
			factoryData_m->cuvetteDelays[ i ] = (unsigned short)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
		}
	}
	
	else if ( stricmp( paramName_m, "flashLampThresholds" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_WAVELENGTHS && s != 0; i++ )
		{
			factoryData_m->flashLampThresholds[ i ] = (unsigned short)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
		}
	}
	
	else if ( stricmp( paramName_m, "heaterDacSettings" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_HEATER_DAC_SETTINGS && s != 0; i++ )
		{
			factoryData_m->heaterDacSettings[ i ] = (unsigned char)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
		}
	}
	
	else if ( stricmp( paramName_m, "fanTemperatureSetting" ) == 0 )
	{
		factoryData_m->fanTemperatureSetting = strtoul( paramValue_m, NULL, 0 );
	}
	
	else if ( stricmp( paramName_m, "instrumentSerialNumber" ) == 0 )
	{
// GLD
// Need a define in CommonDefines.h
		strncpy( factoryData_m->instrumentSerialNumber, paramValue_m, 11 );
		factoryData_m->instrumentSerialNumber[ 11 ] = '\0';
	}
	
	else if ( stricmp( paramName_m, "abaxisProductCode" ) == 0 )
	{
		factoryData_m->abaxisProductCode = paramValue_m[ 0 ];
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
		if ( attr[0] != 0 )
		{
			attributes_m[0] = strdup( attr[0] );
		
			if ( attr[1] != 0 )
			{
				attributes_m[1] = strdup( attr[1] );
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
 * 	ParseFactory
 * 
 * DESCRIPTION
 * 	Parses the factory config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	factoryData - Pointer to the data structure where the parsed data should be
 * 					stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseFactory( FactoryData_t * factoryData )
{
	unsigned	version;

	return ParseFactory( factoryData, &version );
}

CfgError_t
ParseFactory( FactoryData_t * factoryData, unsigned * version )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	version_m = version;	
	factoryData_m = factoryData;

	/*
	 * Open the config file.
	 */
	fp = fopen( FACTORY_CONFIG_FILE_NAME, "r" );
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
