/*
 * FILENAME
 * 	ParseExpirys.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse the expirys XML file.
 * 
 * 	This code relies on the expat library, which comes with QNX, but is not
 * 	automatically linked to a program - the library must be added to the project.
 * 
 * 	It also uses the Crc code from the common library.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>

#include "ParseExpirys.h"
#include "CommonDefines.h"
#include "Crc32.h"	// CRC routines from the Common library
#include "LogErr.h"
#include "EncodeString.h"


// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE					8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static unsigned 		version_m;
static unsigned			receivedCrc_m;
static unsigned			calculatedCrc_m;
static bool				inRecords_m;
static bool				inParam_m;
static bool				inAdmin_m;
static bool				inOp_m;
static bool				inCrc_m;
static bool				inVersion_m;
static Expiry_t *		expiry_m;

static ExpiryData_t	 *	expiryData_m;


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
	if ( inParam_m ) {
		if ( (_stricmp( paramName_m, "controlHigh" ) == 0) && (expiry_m == 0) ) {
				expiryData_m->controlExpiry.dwHighDateTime =  strtoul (paramValue_m, NULL, 0);
		} else if ((_stricmp( paramName_m, "controlLow" ) == 0) && (expiry_m == 0) ) {
				expiryData_m->controlExpiry.dwLowDateTime = strtoul (paramValue_m, NULL, 0);
        } else if ( expiry_m != 0 ) {
			if ( _stricmp( paramName_m, "id" ) == 0 ) {
				strcpy( expiry_m->id, paramValue_m );
			} else if ( _stricmp( paramName_m, "password" ) == 0 ) {
				strcpy( expiry_m->password, paramValue_m ); 
            } else if ( _stricmp( paramName_m, "expiryHigh" ) == 0 ) {
				expiry_m->expiration.dwHighDateTime = strtoul (paramValue_m, NULL, 0);
			} else if (_stricmp( paramName_m, "expiryLow") == 0) {
				expiry_m->expiration.dwLowDateTime = strtoul  (paramValue_m, NULL, 0);
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
	 * We're in the Records section of the file, where the actual data is stored.
	 */

	if ( inRecords_m )
	{
		if ( _stricmp( el, "Admin" ) == 0 )
		{
			inAdmin_m = true;
			expiry_m = &expiryData_m->admins[ expiryData_m->numAdmins ];
			expiryData_m->numAdmins++;
		}
		else if ( _stricmp( el, "Op" ) == 0 )
		{
			inOp_m = true;
			expiry_m = &expiryData_m->ops[ expiryData_m->numOps ];
			expiryData_m->numOps++;
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
	else if ( _stricmp( el, "Records" ) == 0 )
	{
		inRecords_m = true;
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
	 * We're in the recods section (between the "Record" and "/Record" tags), so
	 * we've either got the end of data for a parameter or the end of the data
	 * section.
	 */
	if ( inRecords_m )
	{
		// End of data section.
		if ( _stricmp( el, "Records" ) == 0 )
		{
			inRecords_m = false;
		}
		
		else if ( _stricmp( el, "Admin" ) == 0 )
		{
			inAdmin_m = false;
			expiry_m = 0;
		}
		
		else if ( _stricmp( el, "Op" ) == 0 )
		{
			inOp_m = false;
			expiry_m = 0;
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
 * 	ParseExpirys
 * 
 * DESCRIPTION
 * 	Parses the given config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 *
 * PARAMETERS
 * 	expiryData - Pointer to the location where the expiry data should be stored.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseExpirys( ExpiryData_t * expiryData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the data structure we're filling.
	 */
	 expiryData_m = expiryData;
	 
	 /*
	  * Initialize the data.
	  */
	memset( expiryData, 0, sizeof( ExpiryData_t ) );
	 
	/*
	 * Open the XML file.
	 */
	fp = fopen( EXPIRYS_CONFIG_FILE_NAME, "rb" );
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
	inRecords_m = false;
	inParam_m = false;
	inAdmin_m = false;
	inOp_m = false;
	inCrc_m = false;
	calculatedCrc_m = Crc32Init();
	
	expiry_m = 0;

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
 * 	WriteExpirys
 * 
 * DESCRIPTION
 * 	Writes expirys file.
 * 
 * PARAMETERS
 * 	expiryData - The expiry data to write to the file.
 * 
 * RETURNS
 * 	CFG_ERR_NONE if the write goes OK, else an error indication.
 */

CfgError_t
WriteExpirys( ExpiryData_t * expiryData )
{
	FILE *			fp;
	unsigned		crc = 0;
	char			dataBuff[ 2000 ];
	int				i;

	// Open the file.
	fp = fopen( EXPIRYS_CONFIG_FILE_NAME, "wb" );
	if ( fp == 0 )
	{
		return CFG_ERR_FILE_OPEN;
	}
	
	// Initialize the CRC.
	crc = Crc32Init();

	// Write the file header.
	fprintf( fp, "<?xml version='1.0' encoding='UTF-8'?>\n" );
	fprintf( fp, "<!-- Expirys -->\n" );
	fprintf( fp, "<Expirys>\n" );
	sprintf( dataBuff, "%u", EXPIRYS_FILE_VERSION );
	fprintf( fp, "  <Version>%s</Version>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	// Write the parameters.
	fprintf( fp, "  <Records>\n" );
	
	// Write the control expiration date.
	sprintf( dataBuff, "%lu", expiryData->controlExpiry.dwHighDateTime);
	fprintf( fp, "    <controlHigh>%s</controlHigh>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

	sprintf( dataBuff, "%lu", expiryData->controlExpiry.dwLowDateTime);
	fprintf( fp, "    <controlLow>%s</controlLow>\n", dataBuff );
	crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	
	// Write the administrator expiry data.
	for ( i = 0; i < expiryData->numAdmins; i++ )
	{
	    fprintf( fp, "    <Admin>\n" );

		EncodeString( dataBuff, expiryData->admins[ i ].id );
		fprintf( fp, "     <id>%s</id>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)expiryData->admins[ i ].id, strlen( expiryData->admins[ i ].id ) );

		EncodeString( dataBuff, expiryData->admins[ i ].password );
		fprintf( fp, "     <password>%s</password>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)expiryData->admins[ i ].password, strlen( expiryData->admins[ i ].password ) );

		sprintf( dataBuff, "%lu", expiryData->admins[ i ].expiration.dwLowDateTime);
		fprintf( fp, "     <expiryLow>%s</expiryLow>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

		sprintf( dataBuff, "%lu", expiryData->admins[ i ].expiration.dwHighDateTime);
		fprintf( fp, "     <expiryHigh>%s</expiryHigh>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	    fprintf( fp, "    </Admin>\n" );

	}

	// Write the operator expiry data.
	for ( i = 0; i < expiryData->numOps; i++ )
	{
	    fprintf( fp, "    <Op>\n" );

		EncodeString( dataBuff, expiryData->ops[ i ].id );
		fprintf( fp, "      <id>%s</id>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)expiryData->ops[ i ].id, strlen( expiryData->ops[ i ].id ) );

		EncodeString( dataBuff, expiryData->ops[ i ].password );
		fprintf( fp, "      <password>%s</password>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)expiryData->ops[ i ].password, strlen( expiryData->ops[ i ].password ) );

		sprintf( dataBuff, "%lu", expiryData->ops[ i ].expiration.dwLowDateTime);
		fprintf( fp, "      <expiryLow>%s</expiryLow>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );

		sprintf( dataBuff, "%lu", expiryData->ops[ i ].expiration.dwHighDateTime);
		fprintf( fp, "     <expiryHigh>%s</expiryHigh>\n", dataBuff );
		crc = Crc32Update( crc, (unsigned char *)dataBuff, strlen( dataBuff ) );
	    fprintf( fp, "    </Op>\n" );

	}

	fprintf( fp, "  </Records>\n" );

	// Write the file signature.
	fprintf( fp, "  <Signature>%lu</Signature>\n", Crc32Finalize( crc ) );
	fprintf( fp, "</Expirys>\n" );
	
	fclose( fp );

	return CFG_ERR_NONE;
}
