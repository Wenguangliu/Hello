#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>

#include "LogErr.h"
#include "ManufacturingStates.h"
#include "ParseManufacturingTestSet.h"

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE       8196

static XML_Parser       parser_m;
static char             paramValue_m[ BUFF_SIZE ];
static char *           paramName_m;
static unsigned         version_m;
static bool             inData_m;
static bool             inParam_m;
static bool             inVersion_m;

static ManufacturingTestSet_t   *manufacturingTestSet_m;
double  defaultNd05ExpectedAbs[9] = {0.448,0.474,0.489,0.493,0.494,0.495,0.495,0.493,0.480};
double  defaultNd10ExpectedAbs[9] = {1.071,1.085,1.079,1.071,1.067,1.056,1.040,1.031,0.986};
double  defaultNd20ExpectedAbs[9] = {1.774,1.742,1.688,1.650,1.631,1.591,1.543,1.516,1.402};
double  defaultBlockLimits[9]     = {.003,.003,.003,.005,.005,.009,.005,.005,.015};
double  defaultDiffLimit = 6.0;
unsigned short defaultMaxBlackOffset = 950;



/*
 * FUNCTION
 *  SetParam
 *
 * DESCRIPTION
 *  Looks for the given parameter in the list of parameters we're looking for.
 *  If found, sets the value.
 *
 * RETURNS
 *  void
 */

static void
SetParam()
{
    int     i;
    char    *s;
    if (_stricmp (paramName_m, "nextDrawerTestWaitTime") == 0) {
        manufacturingTestSet_m->nextDrawerTestWaitTime = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "countDisplayTime") == 0) {
        manufacturingTestSet_m->countDisplayTime = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "motor7000MinSpeed") == 0) {
        manufacturingTestSet_m->motor7000MinSpeed = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "acc5000Min") == 0) {
        manufacturingTestSet_m->acc5000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "acc4000Min") == 0) {
        manufacturingTestSet_m->acc4000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "acc3000Min") == 0) {
        manufacturingTestSet_m->acc3000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "acc2000Min") == 0) {
        manufacturingTestSet_m->acc2000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "acc1000Min") == 0) {
        manufacturingTestSet_m->acc1000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "dec4000Min") == 0) {
        manufacturingTestSet_m->dec4000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "dec3000Min") == 0) {
        manufacturingTestSet_m->dec3000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "dec2000Min") == 0) {
        manufacturingTestSet_m->dec2000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "dec1000Min") == 0) {
        manufacturingTestSet_m->dec1000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "dec0000Min") == 0) {
        manufacturingTestSet_m->dec0000Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "maxBlackOffset") == 0) {
        manufacturingTestSet_m->maxBlackOffset = atoi (paramValue_m);
    } else if (_stricmp (paramName_m, "nd05ExpectedAbs") == 0) {
        s = strtok (paramValue_m, ",");
        for (i = 0; i < 9 && s != 0; i++) {
            manufacturingTestSet_m->nd05ExpectedAbs [i] = atof (s);
            s = strtok( 0, "," );
        }
    } else if (_stricmp (paramName_m, "nd10ExpectedAbs") == 0) {
        s = strtok (paramValue_m, ",");
        for (i = 0; i < 9 && s != 0; i++) {
            manufacturingTestSet_m->nd10ExpectedAbs [i] = atof (s);
            s = strtok( 0, "," );
        }
    } else if (_stricmp (paramName_m, "nd20ExpectedAbs") == 0) {
        s = strtok (paramValue_m, ",");
        for (i = 0; i < 9 && s != 0; i++) {
            manufacturingTestSet_m->nd20ExpectedAbs [i] = atof (s);
            s = strtok( 0, "," );
        }
    } else if (_stricmp (paramName_m, "bandBlockLimits") == 0) {
        s = strtok (paramValue_m, ",");
        for (i = 0; i < 9 && s != 0; i++) {
            manufacturingTestSet_m->bandBlockLimits [i] = atof (s);
            s = strtok( 0, "," );
        }
    } else if (_stricmp (paramName_m, "nd05DiffLimit") == 0) {
        manufacturingTestSet_m->nd05DiffLimit = atof (paramValue_m);
    } else if (_stricmp (paramName_m, "nd10DiffLimit") == 0) {
        manufacturingTestSet_m->nd10DiffLimit  = atof (paramValue_m);
    } else if (_stricmp (paramName_m, "nd20DiffLimit") == 0) {
        manufacturingTestSet_m->nd20DiffLimit  = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempMin")== 0) {
        manufacturingTestSet_m->rotorTempMin = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempMax")== 0) {
        manufacturingTestSet_m->rotorTempMax = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempInitLow")== 0) {
        manufacturingTestSet_m->rotorTempInitLow = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempInitHigh")== 0) {
        manufacturingTestSet_m->rotorTempInitHigh = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempAveLow")== 0) {
        manufacturingTestSet_m->rotorTempAveLow = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempAveHigh")== 0) {
        manufacturingTestSet_m->rotorTempAveHigh = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempOffsetMin")== 0) {
        manufacturingTestSet_m->rotorTempOffsetMin = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempOffsetMax")== 0) {
        manufacturingTestSet_m->rotorTempOffsetMax = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempstdDevMin")== 0) {
        manufacturingTestSet_m->rotorTempstdDevMin = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"rotorTempstdDevMax")== 0) {
        manufacturingTestSet_m->rotorTempstdDevMax = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayOffset")== 0) {
        manufacturingTestSet_m->delayOffset = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMinSamp")== 0) {
        manufacturingTestSet_m->delayMinSamp = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMaxSamp")== 0) {
        manufacturingTestSet_m->delayMaxSamp = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMulti")== 0) {
        manufacturingTestSet_m->delayMulti = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delaySmall")== 0) {
        manufacturingTestSet_m->delaySmall = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMinSlop")== 0) {
        manufacturingTestSet_m->delayMinSlop = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMaxSlop")== 0) {
        manufacturingTestSet_m->delayMaxSlop = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"okHalfWitdh")== 0) {
        manufacturingTestSet_m->okHalfWitdh = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMinDerive")== 0) {
        manufacturingTestSet_m->delayMinDerive = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"delayMaxDerive")== 0) {
        manufacturingTestSet_m->delayMaxDerive = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"alignmentF1Min")== 0) {
        manufacturingTestSet_m->alignmentF1Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"alignmentF2Min")== 0) {
        manufacturingTestSet_m->alignmentF2Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"alignmentF3Min")== 0) {
        manufacturingTestSet_m->alignmentF3Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"alignmentF4Min")== 0) {
        manufacturingTestSet_m->alignmentF4Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"alignmentF5Min")== 0) {
        manufacturingTestSet_m->alignmentF5Min = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"cuvOffsetMin")== 0) {
        manufacturingTestSet_m->cuvOffsetMin = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"cuvOffsetMax")== 0) {
        manufacturingTestSet_m->cuvOffsetMax = atoi (paramValue_m);
    } else if (_stricmp (paramName_m,"cuvSlopeMin")== 0) {
        manufacturingTestSet_m->cuvSlopeMin = atof (paramValue_m);
    } else if (_stricmp (paramName_m,"cuvSlopeMax")== 0) {
        manufacturingTestSet_m->cuvSlopeMax = atof (paramValue_m);
    }
}

/*
 * FUNCTION
 *  CharacterHandler
 *
 * DESCRIPTION
 *  Callback function for the expat parser.  This is called whenever there is
 *  character data between XML tags that we're interested in.  We concatenate it
 *  to the list of characters we might already have read in (this is called more
 *  than once if the character string spans more than one read from the XML
 *  file.
 *
 * PARAMETERS
 *  userData - Pointer to application-defined user data.  We don't use it here.
 *  s - String of characters the parser has located.
 *  len - Length of s.
 *
 * RETURNS
 *  void
 */

static void
CharacterHandler( void * userData, const XML_Char * s, int len )
{
    strncat( paramValue_m, s, len );
}


/*
 * FUNCTION
 *  StartTag
 *
 * DESCRIPTION
 *  Callback function for the expat parser.  This is called when a start tag is
 *  located.
 *
 * PARAMETERS
 *  data - Pointer to application-defined user data.  We don't use it here.
 *  el - The tag name.
 *  attr - List of attribute name/value pairs.  We don't use them.
 *
 * RETURNS
 *  void
 */

static void
StartTag (void *data, const char *el, const char **attr)
{
    /*
     * If we're in the data section (between the "Record" tags), this is a
     * parameter in the file.  We save the name and activate the character data
     * handler to receive the tag data.
     */
    if (inData_m) {
        paramName_m = _strdup (el);

        XML_SetCharacterDataHandler (parser_m, CharacterHandler);
        paramValue_m[0] = '\0';
        inParam_m = true;
    }

    /*
     * A "Records" tag indicates we've started the section with the parameter
     * data.
     */
    else if (_stricmp( el, "Records") == 0) {
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
}


/*
 * FUNCTION
 *  EndTag
 *
 * DESCRIPTION
 *  Callback function for the expat parser.  This is called when an end tag is
 *  located.
 *
 * PARAMETERS
 *  data - Pointer to application-defined user data.  We don't use it here.
 *  el - The tag name.
 *
 * RETURNS
 *  void
 */

static void
EndTag (void *data, const char *el)
{
    // Turn off the character data handler.
    XML_SetCharacterDataHandler(parser_m, 0);

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
            // Save the value if it's a parameter we're looking for.
            SetParam ();

            // Free the memory we allocated for storing the parameter name and
            // attribute, and signal we're no longer in a parameter.
            free (paramName_m);
            paramName_m = 0;
            inParam_m = false;
        }
    }

    /*
     * If we're in the Version, convert the string to a version number and save
     * it.
     */
    else if (inVersion_m) {
        sscanf (paramValue_m, "%u", &version_m);
        inVersion_m = false;
    }
}

// set ams test setting default value in the case of xml does not exist
void
SetManufacturingTestSetDefaultValue (ManufacturingTestSet_t *testData)
{
    int i;
    //set test set default values
    testData->nextDrawerTestWaitTime  = 15;
    testData->countDisplayTime        = 15;
    testData->savedWaitTime           = 0;
    testData->motor7000MinSpeed       = 5500;
    testData->acc5000Min              = 60;
    testData->acc4000Min              = 2200;
    testData->acc3000Min              = 4400;
    testData->acc2000Min              = 5200;
    testData->acc1000Min              = 6200;
    testData->dec4000Min              = 9500;
    testData->dec3000Min              = 8000;
    testData->dec2000Min              = 7800;
    testData->dec1000Min              = 7500;
    testData->dec0000Min              = 7000;

    testData->maxBlackOffset = defaultMaxBlackOffset;

    for (i =0; i < 9; i++) {
        testData->nd05ExpectedAbs [i] = defaultNd05ExpectedAbs [i];
        testData->nd10ExpectedAbs [i] = defaultNd10ExpectedAbs [i];
        testData->nd20ExpectedAbs [i] = defaultNd20ExpectedAbs [i];
        testData->bandBlockLimits [i] = defaultBlockLimits [i];
    }

    testData->nd05DiffLimit = defaultDiffLimit;
    testData->nd10DiffLimit = defaultDiffLimit;
    testData->nd20DiffLimit = defaultDiffLimit;

    testData->rotorTempMin      = 36.5;
    testData->rotorTempMax      = 37.25;
    testData->rotorTempInitLow  = 33.0;
    testData->rotorTempInitHigh = 38.0;
    testData->rotorTempAveLow  =  34.0;
    testData->rotorTempAveHigh =  38.85;
    testData->rotorTempOffsetMin = -2.6;
    testData->rotorTempOffsetMax = 2.6;
    testData->rotorTempstdDevMin = 0; 
    testData->rotorTempstdDevMax = 1; 

    testData->delayOffset       = 1280.0;
    testData->delayMinSamp      = 5.0;
    testData->delayMaxSamp      = 68.0;
    testData->delayMulti        = 1.6;
    testData->delaySmall        = 0.0001;
    testData->delayMinSlop      = 10.0;
    testData->delayMaxSlop      = 1500.0;
    testData->okHalfWitdh       = 2.5;
    testData->delayMinDerive    = 10.0;
    testData->delayMaxDerive    = 1500.0;

    testData->alignmentF1Min    = 14;
    testData->alignmentF2Min    = 8;
    testData->alignmentF3Min    = 8; 
    testData->alignmentF4Min    = 11; 
    testData->alignmentF5Min    = 11;

    testData->cuvOffsetMin      = -200;
    testData->cuvOffsetMax      = 140;
    testData->cuvSlopeMin       = 2.8;
    testData->cuvSlopeMax       = 4.8;
    
}

// get test set parameters from xml file
CfgError_t
ParseManufacturingTestSet (ManufacturingTestSet_t *testData)
{
    FILE *      fp;
    int         done;
    char        buff[ BUFF_SIZE ];

    manufacturingTestSet_m = testData;

    SetManufacturingTestSetDefaultValue (testData);

    fp = fopen (MANUFACTURING_TEST_SET_FILE_NAME, "rb");
    if (fp == NULL) {
        LogError ("Fail to open %s", MANUFACTURING_TEST_SET_FILE_NAME);
//      SetManufacturingTestSetDefaultValue (testData);
        return CFG_ERR_FILE_OPEN;
    }
    
    parser_m = XML_ParserCreate (NULL);
    if (parser_m == NULL) {
        fclose (fp);
        return CFG_ERR_PARSER_CREATE;
    }

    XML_SetElementHandler (parser_m, StartTag, EndTag);
    /*
     * Clear flags.
     */
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
        if (ferror(fp)) {
            fclose( fp );
            return CFG_ERR_FILE_READ;
        }

        // EOF, so done.
        done = feof(fp);

        // Parse the buffer.
        if (XML_Parse( parser_m, buff, len, done ) == 0) {
            fclose (fp);
            return CFG_ERR_SYNTAX;
        }
    }

    fclose(fp);

    return CFG_ERR_NONE;
}
