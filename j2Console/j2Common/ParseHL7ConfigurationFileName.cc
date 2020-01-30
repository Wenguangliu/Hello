#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#include <errno.h>

#include "Commondefines.h"
#include "Crc32.h"  // CRC routines from the Common library
#include "HL7Configuration.h"
#include "ParseHL7Configuration.h"
#include "LogErr.h"
#include "CfgErr.h"

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE        4096 

static XML_Parser       parser_m;
static char             paramValue_m [BUFF_SIZE];
static char *           paramName_m;
static char *           attributes_m[2];
static unsigned *       version_m;
static unsigned         receivedCrc_m;
static unsigned         calculatedCrc_m;
static bool             inData_m;
static bool             inParam_m;
static bool             inCrc_m;
static bool             inVersion_m;
static unsigned         wCrc_m;

typedef enum
{
    NO_READ_FIELD,
    INDEX_FIELD,
    FILE_FIELD
} ReadField_t;


static Hl7ConfigurationFileList_t       *hl7ConfigurationFileList_m;
static ReadField_t                      readField = NO_READ_FIELD;

#define HL7_CONFIG_FILE_DIRECTORY       "/Hard Disk/Abaxis/cfg/hl7/"

unsigned short                          fileIndex = 0, totallIndex = 0;

Hl7ConfigurationFile_t hl7ConfigurationFileDefaultTable [] = 
    {
        //Index    file name                  display text
        {0,   "hl7Configuration.xml",      "Generic HL7 Format"},
        {1,   "hl7MllpConfiguration.xml",  "MLLP HL7 Format"},
        {2,   "hl7EpicConfiguration.xml",  "EPIC HL7 Format"},
        {3,   "hl7EpicConfiguration.xml",  "Parctice Fusion HL7 Format"},
        
    };


// Initial default file name list 
void
InitDefaultFileName (Hl7ConfigurationFileList_t *hl7ConfigurationFileList)
{
    unsigned short     tIndex, i;

    hl7ConfigurationFileList_m->tIndex = 4;
    tIndex = hl7ConfigurationFileList_m->tIndex;

    for (i = 0; i < tIndex; i++) {
        hl7ConfigurationFileList_m->hl7FileName [i].index              =  hl7ConfigurationFileDefaultTable[i].index;
        strcpy (hl7ConfigurationFileList_m->hl7FileName [i].fileName,   HL7_CONFIG_FILE_DIRECTORY);
        strcat (hl7ConfigurationFileList_m->hl7FileName [i].fileName,   hl7ConfigurationFileDefaultTable[i].fileName);
        strcpy (hl7ConfigurationFileList_m->hl7FileName [i].displayText,hl7ConfigurationFileDefaultTable[i].displayText);
     //   LogDebug ("fileName = %s", hl7ConfigurationFileList_m->hl7FileName [i].fileName);
     //   LogDebug ("Text = %s", hl7ConfigurationFileList_m->hl7FileName [i].displayText);
    } 
}

// get totally index  number
void
IndexField (void)
{
   if (_stricmp (paramName_m, "tIndex") == 0) {
        hl7ConfigurationFileList_m->tIndex = atoi (paramValue_m);
        totallIndex = hl7ConfigurationFileList_m->tIndex;
        //LogDebug ("Get tIndex =%d",  hl7ConfigurationFileList_m->tIndex );

   }
}

// get file name lists
void
FileField (void)
{
    if (fileIndex < totallIndex) {
        if (_stricmp (paramName_m, "index") == 0) {
            hl7ConfigurationFileList_m->hl7FileName [fileIndex].index =  atoi (paramValue_m);;
            //LogDebug ("file index =%d", hl7ConfigurationFileList_m->hl7FileName [fileIndex].index);
        }
        if (_stricmp (paramName_m, "fileName") == 0) {
            strcpy (hl7ConfigurationFileList_m->hl7FileName [fileIndex].fileName, HL7_CONFIG_FILE_DIRECTORY);
            strcat (hl7ConfigurationFileList_m->hl7FileName [fileIndex].fileName, paramValue_m);
            //LogDebug ("fileName = %s", hl7ConfigurationFileList_m->hl7FileName [fileIndex].fileName);
        } 
        if (_stricmp (paramName_m, "displayText") == 0) {
            strcpy (hl7ConfigurationFileList_m->hl7FileName [fileIndex].displayText, paramValue_m);
            //LogDebug ("Text = %s", hl7ConfigurationFileList_m->hl7FileName [fileIndex].displayText);
            // end of file field, increase index
            fileIndex++;
        }
    }
}

static void
SetParam()
{
    
    if (readField == INDEX_FIELD) {
        IndexField ();
    } else if (readField == FILE_FIELD) {
        FileField ();
    } else {
        LogDebug ("Undefined file read field");
    }
}

// set reading flag
void
SetReadFieldFlag (const char *el) 
{
    if (_stricmp (el, "indexField") == 0) {
        readField = INDEX_FIELD;
    } else if (_stricmp (el, "fileNameField") == 0) {
        readField = FILE_FIELD;
    }
}
    
static void
CharacterHandler (void * userData, const XML_Char * s, int len)
{
    strncat (paramValue_m, s, len);
}


static void
StartTag (void *data, const char *el, const char **attr)
{
    if (inData_m) {
        SetReadFieldFlag (el);
        paramName_m = _strdup (el);
        if (attr[0] != 0) {
            attributes_m[0] = _strdup (attr[0]);

            if (attr[1] != 0) {
                attributes_m[1] = _strdup( attr[1] );
            }
        }

        XML_SetCharacterDataHandler (parser_m, CharacterHandler);
        paramValue_m[0] = '\0';
        inParam_m = true;
    }

    /*
     * A "Records" tag indicates we've started the section with the parameter
     * data.
     */
    else if (_stricmp( el, "Records" ) == 0) {
        inData_m = true;
    }

    /*
     * A "Version" tag indicates this is the file version.  We activate the
     * character data handler to receive the version number.
     */
    else if (_stricmp( el, "Version" ) == 0) {
        XML_SetCharacterDataHandler (parser_m, CharacterHandler);
        paramValue_m[0] = '\0';
        inVersion_m = true;
    }

    /*
     * A "Signature" tag indicates this is the file signature (CRC).  We
     * activate the character data handler to receive the CRC.
     */
    else if (_stricmp( el, "Signature" ) == 0) {
        XML_SetCharacterDataHandler( parser_m, CharacterHandler );
        paramValue_m[0] = '\0';
        inCrc_m = true;
    }
}


static void
EndTag (void *data, const char *el)
{
    // Turn off the character data handler.
    XML_SetCharacterDataHandler (parser_m, 0);

    /*
     * We're in the data section (between the "Record" and "/Record" tags), so
     * we've either got the end of data for a parameter or the end of the data
     * section.
     */
    if (inData_m) {
        // End of data section.
        if (_stricmp( el, "Records" ) == 0) {
            inData_m = false;
        }

        // End of data for a parameter.
        else if (inParam_m) {
            // Update the calculated CRC value.  (NOTE: Do this before calling
            // SetParam(), which may modify paramValue_m. )
            calculatedCrc_m = Crc32Update (calculatedCrc_m, (unsigned char *)paramValue_m, strlen (paramValue_m));

            // Save the value if it's a parameter we're looking for.
            SetParam ();

            // Free the memory we allocated for storing the parameter name and
            // attribute, and signal we're no longer in a parameter.
            free (paramName_m);
            paramName_m = 0;
            if (attributes_m[0] != 0) {
                free (attributes_m[0]);
                attributes_m[0] = 0;
            }
            if (attributes_m[1] != 0) {
                free(attributes_m[1]);
                attributes_m[1] = 0;
            }
            inParam_m = false;
        }
    }
    /*
     * If we're in the Version, convert the string to a version number and save
     * it.  Update the CRC.
     */
    else if (inVersion_m) {
        sscanf (paramValue_m, "%u", version_m);
        calculatedCrc_m = Crc32Update (calculatedCrc_m, (unsigned char *)paramValue_m, strlen (paramValue_m));
        inVersion_m = false;
    }

    /*
     * If we're in the CRC, convert the string to a CRC number and save it.
     */
    else if (inCrc_m) {
        receivedCrc_m = strtoul (paramValue_m, 0, 0);
        inCrc_m = false;
    }
}


// get HL7 configuration file names
CfgError_t
ParseHL7ConfigurationFileName (Hl7ConfigurationFileList_t  *hl7ConfigurationFileList)
{
    FILE *      fp;
    int         done;
    char        buff [BUFF_SIZE];
    int         len;
    unsigned    _version;

    // get version address
    version_m =  &_version;

    fileIndex = 0; 
    totallIndex = 0;

    hl7ConfigurationFileList_m = hl7ConfigurationFileList;


    fp = fopen (HL7_CONFIG_FILE_NAMES, "rb");
    if (fp == NULL) {
        // in the case of name file open fail, copy default file name 
        InitDefaultFileName (hl7ConfigurationFileList);
        return CFG_ERR_FILE_DOES_NOT_EXIST;
    }

    //Create the parser and install the start and end tag callbacks.
     parser_m = XML_ParserCreate (0);
     if (parser_m == 0) {
        fclose (fp);
        return CFG_ERR_PARSER_CREATE;
     }
     XML_SetElementHandler (parser_m, StartTag, EndTag);
     //Clear flags and init the CRC.
     inData_m = false;
     inParam_m = false;
     inCrc_m = false;
     calculatedCrc_m = Crc32Init();

    /*
     * Repeatedly read from the file and parse what's read.  If the buffer is
     * at least as large as the file, this will only happen once.
     */
    for (done = 0; !done;) {
        // Read the file into the buffer.
        len = fread (buff, 1, BUFF_SIZE, fp);
        if (ferror(fp)) {
            fclose (fp);
            return CFG_ERR_FILE_READ;
        }

        // EOF, so done.
        done = feof(fp);
   
        // Parse the buffer.
        if (XML_Parse (parser_m, buff, len, done) == 0) {
            fclose (fp);
            return CFG_ERR_SYNTAX;
        }
    }

    fclose(fp);

    // Finalize the CRC calculation and compare it to the CRC read from the file.
    calculatedCrc_m = Crc32Finalize (calculatedCrc_m );
    if (calculatedCrc_m != receivedCrc_m)   {
       return CFG_ERR_CRC;
     }

    return CFG_ERR_NONE;
}
