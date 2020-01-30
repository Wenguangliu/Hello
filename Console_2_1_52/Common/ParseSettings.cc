/*
 * FILENAME
 * 	ParseSettings.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse the settings.xml config file.
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
#include "SettingsData.h"
#include "ParseSettings.h"

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

static SettingsData_t *	settingsData_m;
	
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
	if ( stricmp( paramName_m, "language" ) == 0 )
	{
		settingsData_m->language = (Language_t)atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "keyboard" ) == 0 )
	{
		settingsData_m->keyboard = (Language_t)atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "time24Format" ) == 0 )
	{
		settingsData_m->time24Format = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "screenSaverTimeout" ) == 0 )
	{
		settingsData_m->screenSaverTimeout = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "powerSaveTimeout" ) == 0 )
	{
		settingsData_m->powerSaveTimeout = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "backlightLevel" ) == 0 )
	{
		settingsData_m->backlightLevel = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "touchClickVolume" ) == 0 )
	{
		settingsData_m->touchClickVolume = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "alertVolume" ) == 0 )
	{
		settingsData_m->alertVolume = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "statusVolume" ) == 0 )
	{
		settingsData_m->statusVolume = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commInterface" ) == 0 )
	{
		settingsData_m->comm.interface = (CommInterface_t)atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commProtocol" ) == 0 )
	{
		settingsData_m->comm.protocol = (CommProtocol_t)atoi( paramValue_m );
	}

	else if ( stricmp( paramName_m, "commAnalyzerIp" ) == 0 )
	{
		strcpy( settingsData_m->comm.analyzerIp, paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commGateway" ) == 0 )
	{
		strcpy( settingsData_m->comm.gateway, paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commIpMask" ) == 0 )
	{
		strcpy( settingsData_m->comm.ipMask, paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commServerIp" ) == 0 )
	{
		strcpy( settingsData_m->comm.serverIp, paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "commServerSocket" ) == 0 )
	{
		settingsData_m->comm.serverSocket = atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "astmOrderMethod" ) == 0 )
	{
		settingsData_m->astmOrderMethod = (AstmOrderMethod_t)atoi( paramValue_m );
	}
	
	else if ( stricmp( paramName_m, "operatorIdEnabled" ) == 0 )
	{
		settingsData_m->operatorIdEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "analysisLockoutEnabled" ) == 0 )
	{
		settingsData_m->analysisLockoutEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "recallLockoutEnabled" ) == 0 )
	{
		settingsData_m->recallLockoutEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "settingsLockoutEnabled" ) == 0 )
	{
		settingsData_m->settingsLockoutEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "sampleTypeAdvanced" ) == 0 )
	{
		settingsData_m->sampleTypeAdvanced = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "alternateIdEnabled" ) == 0 )
	{
		settingsData_m->alternateIdEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "doctorIdEnabled" ) == 0 )
	{
		settingsData_m->doctorIdEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "locationEnabled" ) == 0 )
	{
		settingsData_m->locationEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "phoneNumberEnabled" ) == 0 )
	{
		settingsData_m->phoneNumberEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "admissionOwnerIdEnabled" ) == 0 )
	{
		settingsData_m->admissionOwnerIdEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "dobEnabled" ) == 0 )
	{
		settingsData_m->dobEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "dovEnabled" ) == 0 )
	{
		settingsData_m->dovEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "sampleIdEnabled" ) == 0 )
	{
		settingsData_m->sampleIdEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "genderEnabled" ) == 0 )
	{
		settingsData_m->genderEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "ageEnabled" ) == 0 )
	{
		settingsData_m->ageEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "raceEnabled" ) == 0 )
	{
		settingsData_m->raceEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "egfrEnabled" ) == 0 )
	{
		settingsData_m->egfrEnabled = atoi( paramValue_m ) == 0 ? false : true;
	}
	
	else if ( stricmp( paramName_m, "printNumResults" ) == 0 )
	{
		settingsData_m->printNumResults = atoi( paramValue_m );
	}

	else if ( stricmp( paramName_m, "printNumIqc" ) == 0 )
	{
		settingsData_m->printNumIqc = atoi( paramValue_m );
	}

	else if ( stricmp( paramName_m, "printNumError" ) == 0 )
	{
		settingsData_m->printNumError = atoi( paramValue_m );
	}

	else if ( stricmp( paramName_m, "errorPrintAuto" ) == 0 )
	{
		settingsData_m->errorPrintAuto = atoi( paramValue_m ) == 0 ? false : true;
	}

	else if ( stricmp( paramName_m, "defaultPrinter" ) == 0 )
	{
		settingsData_m->defaultPrinter = atoi( paramValue_m );
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
 * 	ParseSettings
 * 
 * DESCRIPTION
 * 	Parses the settings config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	settingsData - Pointer to the data structure where the parsed data should be
 * 					stored.
 * 	version - Pointer to a location where the file version may be stored.  This
 * 				may be null (default).
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseSettings( SettingsData_t * settingsData, unsigned * version )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	unsigned	_version;
	
	// If no address for version # storage is passed in, use the local storage.
	version_m = version != 0 ? version : &_version;	

	// Init the module settingsData pointer.
	settingsData_m = settingsData;

	// Init the settings to the defaults.
	memcpy( settingsData, &defaultSettings, sizeof( SettingsData_t ) );
	
	/*
	 * Open the config file.
	 */
	fp = fopen( SETTINGS_CONFIG_FILE_NAME, "r" );
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
 * 	WriteSettings
 * 
 * DESCRIPTION
 * 	Writes the settings.xml file.
 * 
 * PARAMETERS
 * 	settingsData - The calibration data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteSettings( SettingsData_t * settingsData )
{
	FILE *		fp;
	unsigned	crc = 0;
	char		dataBuff[ 2000 ];
	
	// Open the file.
	fp = fopen( SETTINGS_CONFIG_FILE_NAME, "w" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Settings Data -->\n" );
	fprintf( fp, "<Settings>\n" );
	sprintf( dataBuff, "%u", SETTINGS_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	// language
	sprintf( dataBuff, "%d", settingsData->language );
	fprintf( fp, "    <language>%s</language>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// keyboard
	sprintf( dataBuff, "%d", settingsData->keyboard );
	fprintf( fp, "    <keyboard>%s</keyboard>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// time24Format
	sprintf( dataBuff, "%d", settingsData->time24Format );
	fprintf( fp, "    <time24Format>%s</time24Format>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// screenSaverTimeout
	sprintf( dataBuff, "%d", settingsData->screenSaverTimeout );
	fprintf( fp, "    <screenSaverTimeout>%s</screenSaverTimeout>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// powerSaveTimeout
	sprintf( dataBuff, "%d", settingsData->powerSaveTimeout );
	fprintf( fp, "    <powerSaveTimeout>%s</powerSaveTimeout>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// backlightLevel
	sprintf( dataBuff, "%d", settingsData->backlightLevel );
	fprintf( fp, "    <backlightLevel>%s</backlightLevel>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// touchClickVolume
	sprintf( dataBuff, "%d", settingsData->touchClickVolume );
	fprintf( fp, "    <touchClickVolume>%s</touchClickVolume>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// alertVolume
	sprintf( dataBuff, "%d", settingsData->alertVolume );
	fprintf( fp, "    <alertVolume>%s</alertVolume>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// statusVolume
	sprintf( dataBuff, "%d", settingsData->statusVolume );
	fprintf( fp, "    <statusVolume>%s</statusVolume>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// commInterface
	sprintf( dataBuff, "%d", settingsData->comm.interface );
	fprintf( fp, "    <commInterface>%s</commInterface>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// commProtocol
	sprintf( dataBuff, "%d", settingsData->comm.protocol );
	fprintf( fp, "    <commProtocol>%s</commProtocol>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// commAnalyzerIp
	sprintf( dataBuff, "%s", settingsData->comm.analyzerIp );
	fprintf( fp, "    <commAnalyzerIp>%s</commAnalyzerIp>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// commGateway
	sprintf( dataBuff, "%s", settingsData->comm.gateway );
	fprintf( fp, "    <commGateway>%s</commGateway>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// commIpMask
	sprintf( dataBuff, "%s", settingsData->comm.ipMask );
	fprintf( fp, "    <commIpMask>%s</commIpMask>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// commServerIp
	sprintf( dataBuff, "%s", settingsData->comm.serverIp );
	fprintf( fp, "    <commServerIp>%s</commServerIp>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// commServerSocket
	sprintf( dataBuff, "%d", settingsData->comm.serverSocket );
	fprintf( fp, "    <commServerSocket>%s</commServerSocket>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// astmOrderMethod
	sprintf( dataBuff, "%d", settingsData->astmOrderMethod );
	fprintf( fp, "    <astmOrderMethod>%s</astmOrderMethod>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// operatorIdEnabled
	sprintf( dataBuff, "%d", settingsData->operatorIdEnabled ? 1 : 0 );
	fprintf( fp, "    <operatorIdEnabled>%s</operatorIdEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// analysisLockoutEnabled
	sprintf( dataBuff, "%d", settingsData->analysisLockoutEnabled ? 1 : 0 );
	fprintf( fp, "    <analysisLockoutEnabled>%s</analysisLockoutEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// recallLockoutEnabled
	sprintf( dataBuff, "%d", settingsData->recallLockoutEnabled ? 1 : 0 );
	fprintf( fp, "    <recallLockoutEnabled>%s</recallLockoutEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// settingsLockoutEnabled
	sprintf( dataBuff, "%d", settingsData->settingsLockoutEnabled ? 1 : 0 );
	fprintf( fp, "    <settingsLockoutEnabled>%s</settingsLockoutEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// sampleTypeAdvanced
	sprintf( dataBuff, "%d", settingsData->sampleTypeAdvanced ? 1 : 0 );
	fprintf( fp, "    <sampleTypeAdvanced>%s</sampleTypeAdvanced>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// alternateIdEnabled
	sprintf( dataBuff, "%d", settingsData->alternateIdEnabled ? 1 : 0 );
	fprintf( fp, "    <alternateIdEnabled>%s</alternateIdEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// doctorIdEnabled
	sprintf( dataBuff, "%d", settingsData->doctorIdEnabled ? 1 : 0 );
	fprintf( fp, "    <doctorIdEnabled>%s</doctorIdEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// locationEnabled
	sprintf( dataBuff, "%d", settingsData->locationEnabled ? 1 : 0 );
	fprintf( fp, "    <locationEnabled>%s</locationEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// phoneNumberEnabled
	sprintf( dataBuff, "%d", settingsData->phoneNumberEnabled ? 1 : 0 );
	fprintf( fp, "    <phoneNumberEnabled>%s</phoneNumberEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// admissionOwnerIdEnabled
	sprintf( dataBuff, "%d", settingsData->admissionOwnerIdEnabled ? 1 : 0 );
	fprintf( fp, "    <admissionOwnerIdEnabled>%s</admissionOwnerIdEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// dobEnabled
	sprintf( dataBuff, "%d", settingsData->dobEnabled ? 1 : 0 );
	fprintf( fp, "    <dobEnabled>%s</dobEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// dovEnabled
	sprintf( dataBuff, "%d", settingsData->dovEnabled ? 1 : 0 );
	fprintf( fp, "    <dovEnabled>%s</dovEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// sampleIdEnabled
	sprintf( dataBuff, "%d", settingsData->sampleIdEnabled ? 1 : 0 );
	fprintf( fp, "    <sampleIdEnabled>%s</sampleIdEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// genderEnabled
	sprintf( dataBuff, "%d", settingsData->genderEnabled ? 1 : 0 );
	fprintf( fp, "    <genderEnabled>%s</genderEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// ageEnabled
	sprintf( dataBuff, "%d", settingsData->ageEnabled ? 1 : 0 );
	fprintf( fp, "    <ageEnabled>%s</ageEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// raceEnabled
	sprintf( dataBuff, "%d", settingsData->raceEnabled ? 1 : 0 );
	fprintf( fp, "    <raceEnabled>%s</raceEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// egfrEnabled
	sprintf( dataBuff, "%d", settingsData->egfrEnabled ? 1 : 0 );
	fprintf( fp, "    <egfrEnabled>%s</egfrEnabled>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// printNumResults
	sprintf( dataBuff, "%d", settingsData->printNumResults );
	fprintf( fp, "    <printNumResults>%s</printNumResults>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// printNumIqc
	sprintf( dataBuff, "%d", settingsData->printNumIqc );
	fprintf( fp, "    <printNumIqc>%s</printNumIqc>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// printNumError
	sprintf( dataBuff, "%d", settingsData->printNumError );
	fprintf( fp, "    <printNumError>%s</printNumError>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// errorPrintAuto
	sprintf( dataBuff, "%d", settingsData->errorPrintAuto ? 1 : 0 );
	fprintf( fp, "    <errorPrintAuto>%s</errorPrintAuto>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// defaultPrinter
	sprintf( dataBuff, "%d", settingsData->defaultPrinter );
	fprintf( fp, "    <defaultPrinter>%s</defaultPrinter>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</Settings>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}

