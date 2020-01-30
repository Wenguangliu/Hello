/*
 * FILENAME
 * 	ParseCalibration.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse and write the calibration config file.
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

#include "ParseCalibration.h"
#include "CommonDefines.h"
#include "Crc32.h"	// CRC routines from the Common library

#define CALIBRATION_FILE_VERSION	1

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


CalibrationData_t *	calibrationData_m = 0;
	
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
	int		i;
	char *	s;

	if ( _stricmp( paramName_m, "barcodeDacs" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_BARCODE_DACS && s != 0; i++ )
		{
			calibrationData_m->barcodeDacs[ i ] = (unsigned char)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
		}
	}
	
	else if ( _stricmp( paramName_m, "cuvetteMarkDacs" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_CUVETTE_MARK_DACS && s != 0; i++ )
		{
			calibrationData_m->cuvetteMarkDacs[ i ] = (unsigned char)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
		}
	}
	
	else if ( _stricmp( paramName_m, "wavelengthDacTrims" ) == 0 )
	{
		s = strtok( paramValue_m, "," );
		for ( i = 0; i < NUM_WAVELENGTHS && s != 0; i++ )
		{
			calibrationData_m->wavelengthDacTrims[ i ] = (unsigned char)strtoul( s, 0, 0 );
			s = strtok( 0, "," );
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
 * 	ParseCalibration
 * 
 * DESCRIPTION
 * 	Parses the given config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseCalibration( CalibrationData_t * calibrationData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the calibration data structure we're filling.
	 */
	 calibrationData_m = calibrationData;
	 
	 /*
	  * Init the calibration data to default values.
	  */
	 memset( calibrationData, 0, sizeof( CalibrationData_t ) );
	 
	/*
	 * Open the config file.
	 */
	fp = fopen( CALIBRATION_CONFIG_FILE_NAME, "rb" );
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
 * 	WriteCalibration
 * 
 * DESCRIPTION
 * 	Writes the calibration.xml file.
 * 
 * PARAMETERS
 * 	calibrationData - The calibration data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteCalibration( CalibrationData_t * calibrationData )
{
	FILE *		fp;
	unsigned	crc = 0;
	char		dataBuff[ 2000 ];
	char *		ptr;
	int			i;
	
	// Open the file.
	fp = fopen( CALIBRATION_CONFIG_FILE_NAME, "wb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Calibration Data -->\n" );
	fprintf( fp, "<Calibration>\n" );
	sprintf( dataBuff, "%u", CALIBRATION_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	// barcodeDacs
	for ( i = 0, ptr = dataBuff; i < NUM_BARCODE_DACS; i++ )
	{
		sprintf( ptr, "%u,", calibrationData->barcodeDacs[ i ] );
		ptr += strlen( ptr );
	}
	*--ptr = '\0';	// Remove the last ','
	fprintf( fp, "    <barcodeDacs>%s</barcodeDacs>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// cuvetteMarkDacs
	for ( i = 0, ptr = dataBuff; i < NUM_CUVETTE_MARK_DACS; i++ )
	{
		sprintf( ptr, "%u,", calibrationData->cuvetteMarkDacs[ i ] );
		ptr += strlen( ptr );
	}
	*--ptr = '\0';	// Remove the last ','
	fprintf( fp, "    <cuvetteMarkDacs>%s</cuvetteMarkDacs>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// wavelengthDacTrims
	for ( i = 0, ptr = dataBuff; i < NUM_WAVELENGTHS; i++ )
	{
		sprintf( ptr, "%u,", calibrationData->wavelengthDacTrims[ i ] );
		ptr += strlen( ptr );
	}
	*--ptr = '\0';	// Remove the last ','
	fprintf( fp, "    <wavelengthDacTrims>%s</wavelengthDacTrims>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
/*
	// wavelengthAdcOffsets
	for ( i = 0, ptr = dataBuff; i < NUM_WAVELENGTHS; i++ )
	{
		sprintf( ptr, "%u,", calibrationData->wavelengthAdcOffsets[ i ] );
		ptr += strlen( ptr );
	}
	*--ptr = '\0';	// Remove the last ','
	fprintf( fp, "    <wavelengthAdcOffsets>%s</wavelengthAdcOffsets>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
*/	
	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</Calibration>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}
