#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>

#include "SettingsData.h"
#include "CfgErr.h"
#include "LogErr.h"
#include "ParseFuseSettings.h"

#define BUFF_SIZE   512

static XML_Parser       parser_m;
static char             paramValue_m[ BUFF_SIZE ];
static char *           paramName_m;
static char *           attributes_m[2];
static unsigned *       version_m;
static unsigned         receivedCrc_m;
static unsigned         calculatedCrc_m;
static bool             inData_m;
static bool             inParam_m;
static bool             inCrc_m;
static bool             inVersion_m;
static unsigned         version = 0;

static SettingsData_t * settingsData_m;

void
SetParam ()
{
    if ( _stricmp( paramName_m, "commInterface" ) == 0 )
    {
        settingsData_m->comm.commInterface = (CommInterface_t)atoi( paramValue_m );
    }
   
    else if ( _stricmp( paramName_m, "commProtocol" ) == 0 )
    {
        settingsData_m->comm.protocol = (CommProtocol_t)atoi( paramValue_m );
    }
    else if ( _stricmp( paramName_m, "commServerIp" ) == 0 )
    {
        strcpy( settingsData_m->comm.serverIp, paramValue_m );
    }
    else if ( _stricmp( paramName_m, "commServerSocket" ) == 0 )
    {
        settingsData_m->comm.serverSocket = atoi( paramValue_m );
    }
    else if ( _stricmp( paramName_m, "dhcpEnabled" ) == 0 )
    {
        settingsData_m->comm.dhcpEnabled = atoi( paramValue_m ) == 0 ? false : true;
    }
    else if ( _stricmp( paramName_m, "astmOrderMethod" ) == 0 )
    {
        settingsData_m->astmOrderMethod = (AstmOrderMethod_t)atoi( paramValue_m );
    }

}

void
CharacterHandler( void * userData, const XML_Char * s, int len )
{
    strncat( paramValue_m, s, len );
}


void
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
}


void
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
//            calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );

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
//        calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );
        inVersion_m = false;
    }

}


CfgError_t
ParseFuseSettings (SettingsData_t * settingsData)
{
    FILE *      fp;
    int         done;
    char        buff [BUFF_SIZE];
    unsigned    _version;

    // If no address for version # storage is passed in, use the local storage.
    version_m = &version;

    // Init the module settingsData pointer.
    settingsData_m = settingsData;

     /*
     * Open the fuse settings file.
     */
    fp = fopen (FUSE_SETTINGS_FILE_NAME, "rb");
    if (fp == 0) {
        LogDebug ("Fail to open file %s", FUSE_SETTINGS_FILE_NAME);
        return CFG_ERR_FILE_OPEN;

    }

    /*
     * Create the parser and install the start and end tag callbacks.
     */
    parser_m = XML_ParserCreate (0);
    if (parser_m == 0) {
        fclose (fp);
        return CFG_ERR_PARSER_CREATE;
    }
    XML_SetElementHandler (parser_m, StartTag, EndTag);

    inData_m = false;
    inParam_m = false;

     /*
     * Repeatedly read from the file and parse what's read.  If the buffer is
     * at least as large as the file, this will only happen once.
     */
    for (done = 0; !done;) {
        int len;

        // Read the file into the buffer.
        len = fread (buff, 1, BUFF_SIZE, fp);
        if (ferror (fp )) {
            fclose( fp );
            return CFG_ERR_FILE_READ;
        }

        // EOF, so done.
        done = feof (fp);

        // Parse the buffer.
        if (XML_Parse( parser_m, buff, len, done ) == 0) {
            fclose (fp);
            return CFG_ERR_SYNTAX;
        }
    }

    fclose( fp );

    return CFG_ERR_NONE;
}
