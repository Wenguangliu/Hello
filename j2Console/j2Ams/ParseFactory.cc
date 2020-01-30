/*
 * FILENAME
 *  Data.c
 *
 * MODULE DESCRIPTION
 *  write and read data to/from xml file
 *
 * AUTHOR
 */
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>


#include "XmlData.h"
#include "Crc32.h"    /*CRC routines from the Common library*/
#include "Serial.h"
#include "LogErr.h"
#include "Global.h"


#define     BUFF_SIZE        8196      

static XML_Parser       parser_m;
static char             paramValue_m[ BUFF_SIZE ];
static char *           paramName_m;
static char *           attributes_m[2];
static unsigned long *  version_m;
static unsigned long    receivedCrc_m;
static unsigned long    calculatedCrc_m;
static bool             inData_m;
static bool             inParam_m;
static bool             inCrc_m;
static bool             inVersion_m;

    
static FactoryData_t *  factoryData_m;
/*
 * FUNCTION
 * WriteFactoryXml
 *
 * DESCRIPTION
 *  Writes the factory calibration xml file.
 *
 * PARAMETERS
 * fp           --- file  pointer  
 * calibration  --- factory calibration data
 *
 * RETURNS
 * void   
 */

void 
WriteFactoryXml (FILE * fp, FactoryData_t * calibration)
{
    unsigned short          i, j;
    unsigned char           last;
    unsigned long           crc;
    char                    numBuff[20];
    char                    crcStr [5000];
    char                    *c =",";
    unsigned long           version = 1;

    /* Initialize the CRC */
    crc = Crc32Init();

    /* header */
    fprintf(fp, "<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf(fp, "<!-- Factory configuration -->\n");
    fprintf(fp, "<Factory>\n");

    /* version */
    sprintf(numBuff, "%lu", version);
    fprintf(fp, "    <Version>%s</Version>\n", numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    /* data */
    fprintf(fp, "  <Records>\n");

    /* air temperature */
    sprintf(numBuff, "%f", calibration->airTemperatureOffset);
    fprintf(fp, "    <airTemperatureOffset>%s</airTemperatureOffset>\n",
                            numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    /* cuvette transmit */
    sprintf(numBuff, "%f",  calibration->cuvetteTransmitLimit);
    fprintf(fp, "    <cuvetteTransmitLimit>%s</cuvetteTransmitLimit>\n",
                            numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    /* bfd calibration */
    fprintf(fp, "    <bfdCalibrationFactors>");
    
    last = 31;
    crcStr[0]= '\0';
    for (i = 0; i < 32; i++) {
        sprintf(numBuff, "%f",  calibration->bfdCalibrationFactors[i]);
        strncat (crcStr,numBuff, strlen(numBuff));
        if (i != last) {
            fprintf (fp, "%s,", numBuff);
            strncat (crcStr,c, 1);
        } else {
            fprintf (fp, "%s", numBuff);
        }
    }
    fprintf(fp, "</bfdCalibrationFactors>\n");
    crc = Crc32Update (crc, (unsigned char *)crcStr, strlen(crcStr));

    sprintf(numBuff, "%f",  calibration->bfdAvgCurveFactors);
    fprintf(fp, "    <bfdAvgCurveFactors>%s</bfdAvgCurveFactors>\n",numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    last = 31;
    for (i = 0; i < 9; i++) {
        sprintf(numBuff, "%u",i);
        fprintf(fp, "    <wavelengthFilterCurves wavelength=%c%s%c>",'"',numBuff,'"');
        crcStr[0]= '\0';
        for (j = 0; j < 32; j++) {
            sprintf(numBuff, "%u", calibration->wavelengthFilterCurves[i][j]);
            strncat (crcStr,numBuff, strlen(numBuff));
            if (j != last) {
                fprintf (fp, "%s,", numBuff);
                strncat (crcStr,c, 1);
            } else {
                fprintf (fp, "%s", numBuff);
            }
        }
        fprintf(fp, "</wavelengthFilterCurves>\n");
        crc = Crc32Update (crc, (unsigned char *)crcStr, strlen(crcStr));
    }

    sprintf(numBuff, "%u",  calibration->globalCuvetteDelay);
    fprintf(fp, "    <globalCuvetteDelay>%s</globalCuvetteDelay>\n",
                      numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    fprintf(fp, "    <cuvetteDelays>");
    last = 29;
    crcStr[0]= '\0';
    for (i = 0; i < 30; i++) {
        sprintf(numBuff, "%u",  calibration->cuvetteDelays[i]);
        strncat (crcStr,numBuff, strlen(numBuff));
        if (i != last) {    
            fprintf (fp, "%s,", numBuff);
            strncat (crcStr,c, 1);
        } else {
            fprintf (fp, "%s", numBuff);
        }
    }
    fprintf(fp, "</cuvetteDelays>\n");
    crc = Crc32Update (crc, (unsigned char *)crcStr, strlen(crcStr));

    fprintf(fp, "    <flashLampThresholds>");
    last = 9;
    crcStr[0]= '\0';
    for (i = 0; i < 10; i++) {
        sprintf(numBuff, "%u",  calibration->flashLampThresholds[i]);
        strncat (crcStr,numBuff, strlen(numBuff));
        if (i != last) {
            fprintf (fp, "%s,", numBuff);
            strncat (crcStr,c, 1);
        } else {
           fprintf (fp, "%s", numBuff);
        }
    }
    fprintf(fp, "</flashLampThresholds>\n");
    crc = Crc32Update (crc, (unsigned char *)crcStr, strlen(crcStr));

    fprintf(fp, "    <heaterDacSettings>");
    last = 3;
    crcStr[0]= '\0';
    for (i = 0; i < 4; i++) {
        sprintf(numBuff, "%u",  calibration->heaterDacSettings[i]);
        strncat (crcStr,numBuff, strlen(numBuff));
        if (i != last) {
            fprintf (fp, "%s,", numBuff);
            strncat (crcStr,c, 1);
        } else {
            fprintf (fp, "%s", numBuff);
        }
    }
    fprintf(fp, "</heaterDacSettings>\n");
    crc = Crc32Update (crc, (unsigned char *)crcStr, strlen(crcStr));
    
    sprintf(numBuff, "%u",  calibration->fanTemperatureSetting);
    fprintf(fp, "    <fanTemperatureSetting>%s</fanTemperatureSetting>\n",
                     numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));

    fprintf( fp, "    <instrumentSerialNumber>%s</instrumentSerialNumber>\n", 
                        calibration->instrumentSerialNumber );
    crc = Crc32Update( crc, (unsigned char *)calibration->instrumentSerialNumber,
                     strlen(calibration->instrumentSerialNumber));

    sprintf(numBuff, "%c",  calibration->abaxisProductCode);
    fprintf(fp, "    <abaxisProductCode>%s</abaxisProductCode>\n",
                      numBuff);
    crc = Crc32Update (crc, (unsigned char *)numBuff, strlen(numBuff));
    

    fprintf( fp,"  </Records>\n" );

    crc = Crc32Finalize(crc);
    fprintf(fp, "     <Signature>%lu</Signature>\n", crc);
    
    fprintf( fp, "</Factory>\n" );
}


/*
 * FUNCTION
 *  WriteData 
 *
 * DESCRIPTION
 *  Writes the data to file.
 *
 * PARAMETERS
 *  fileName    filename to be written 
 *  data        pointer to data 
 * RETURNS
 * MsgStatus_t write status
 */


AmsError_t
WriteData (char *fileName, char *data)
{
    FILE    *fp;
    WCHAR   wFileName [128]; 
    DWORD   error;
   
    // convert to wchar file
    wsprintf (wFileName, L"%S", fileName);
    // change file permit
    if (SetFileAttributes (wFileName, FILE_ATTRIBUTE_NORMAL) == 0) {
      error = GetLastError ();
      LogError ("Fail to change file %s write permit ", wFileName);
      return OPEN_FACTORY_FILE_FAIL;
    }

    fp = fopen (fileName, "wb");

    if (fp == NULL) {
        printf("Error opening file <%s>\n", fileName);
        return OPEN_FACTORY_FILE_FAIL;
    }

    WriteFactoryXml (fp, (FactoryData_t *)data);

    fclose (fp);

    // change file permit to be read only
    if (SetFileAttributes (wFileName, FILE_ATTRIBUTE_READONLY) == 0) {
      error = GetLastError ();
      LogError ("Fail to change file %s write permit ", wFileName);
      return OPEN_FACTORY_FILE_FAIL;

    }

    return NO_ERR;
}


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
    char *  s;

    if ( _stricmp (paramName_m, "airTemperatureOffset" ) == 0) 
    {
        factoryData_m->airTemperatureOffset = (float)atof (paramValue_m);
    }

    else if ( _stricmp(paramName_m, "cuvetteTransmitLimit") == 0 )
    {
        factoryData_m->cuvetteTransmitLimit = (float)atof( paramValue_m );
    }

    else if ( _stricmp( paramName_m, "bfdCalibrationFactors" ) == 0 )
    {
        s = strtok( paramValue_m, "," );
        for ( i = 0; i < MAX_BFD_CALS && s != 0; i++ )
        {
            factoryData_m->bfdCalibrationFactors[ i ] = (float)atof( s );
            s = strtok( 0, "," );
        }
    }

    else if ( _stricmp( paramName_m, "bfdAvgCurveFactors" ) == 0 )
    {
        factoryData_m->bfdAvgCurveFactors = (float)atof( paramValue_m );
    }
   else if ( _stricmp( paramName_m, "wavelengthFilterCurves" ) == 0 )
    {
        int wavelength = -1;

        if ( attributes_m[0] != 0 && _stricmp( attributes_m[0], "wavelength" ) == 0 && attributes_m[1] != 0 )
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
    else if ( _stricmp( paramName_m, "globalCuvetteDelay" ) == 0 )
    {
        factoryData_m->globalCuvetteDelay = strtoul( paramValue_m, 0, 0 );
    }

    else if ( _stricmp( paramName_m, "cuvetteDelays" ) == 0 )
    {
        s = strtok( paramValue_m, "," );
        for ( i = 0; i < NUM_CUVETTES && s != 0; i++ )
        {
            factoryData_m->cuvetteDelays[ i ] = (unsigned short)strtoul( s, 0, 0 );
            s = strtok( 0, "," );
        }
    }
 else if ( _stricmp( paramName_m, "flashLampThresholds" ) == 0 )
    {
        s = strtok( paramValue_m, "," );
        for ( i = 0; i < NUM_WAVELENGTHS && s != 0; i++ )
        {
            factoryData_m->flashLampThresholds[ i ] = (unsigned short)strtoul( s, 0, 0 );
            s = strtok( 0, "," );
        }
    }

    else if ( _stricmp( paramName_m, "heaterDacSettings" ) == 0 )
    {
        s = strtok( paramValue_m, "," );
        for ( i = 0; i < NUM_HEATER_DAC_SETTINGS && s != 0; i++ )
        {
            factoryData_m->heaterDacSettings[ i ] = (unsigned short)strtoul( s, 0, 0 );
            s = strtok( 0, "," );
        }
    }
    else if ( _stricmp( paramName_m, "fanTemperatureSetting" ) == 0 )
    {
        factoryData_m->fanTemperatureSetting = strtoul( paramValue_m, NULL, 0 );
    }

    else if ( _stricmp( paramName_m, "instrumentSerialNumber" ) == 0 )
    {
// GLD
// Need a define in CommonDefines.h
        strncpy( factoryData_m->instrumentSerialNumber, paramValue_m, SERIAL_NUMBER_LEN );
        factoryData_m->instrumentSerialNumber[ SERIAL_NUMBER_LEN ] = '\0';
    }

    else if ( _stricmp( paramName_m, "abaxisProductCode" ) == 0 )
    {
        factoryData_m->abaxisProductCode = paramValue_m[ 0 ];
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
        sscanf( paramValue_m, "%lu", version_m );
        calculatedCrc_m = Crc32Update( calculatedCrc_m, (unsigned char *)paramValue_m, strlen( paramValue_m ) );
        inVersion_m = false;
    }

    /*
     * If we're in the CRC, convert the string to a CRC number and save it.
     */
    else if ( inCrc_m )
    {
#if 0
        receivedCrc_m = strtoul( paramValue_m, 0, 0 );
#else
        sscanf( paramValue_m, "%lu", &receivedCrc_m);
#endif
        inCrc_m = false;
    }
}

#if 0
/*
 * FUNCTION
 *  ParseFactoryXml
 *
 * DESCRIPTION
 *  Parses the given factory file, looking for the data fields identified.  If
 *  a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 *  found other than that the field has not been modified.
 *
 * PARAMETERS
 *  FileName - Name of the config file.
 *
 * RETURNS
 *  MsgStatus_t - Xml Parse status
 */

AmsError_t
ParseFactoryXml (char *FileName)
{
    FILE *          fp;
    char            buff[BUFF_SIZE];
    int             len, done ;

    fp = fopen(FileName, "rb");

    if (fp == NULL) {
        PrintMsgStatus (OPEN_FACTORY_FILE_FAIL);
        return FACTORY_XML_OPEN_FAIL;
    }

    factoryData_m = &FactoryData;
    version_m     = &FactoryData.version;
    /*
     * Create the parser and install the start and end tag callbacks.
     */
    parser_m = XML_ParserCreate(NULL);
    if (parser_m == NULL) {
        fclose(fp);
        return CREATE_PARSER_FAIL;
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
        // Read the file into the buffer.
        len = fread( buff, 1, BUFF_SIZE, fp );
        if ( ferror( fp ) )
        {
            fclose( fp );
            return XML_READ_ERR;
        }

        // EOF, so done.
        done = feof( fp );

        // Parse the buffer.
        if ( XML_Parse( parser_m, buff, len, done ) == 0 )
        {
            fclose( fp );
            return XML_SYNTAX_ERR;
        }
    }

    fclose( fp );

   // Finalize the CRC calculation and compare it to the CRC read from the file.
    calculatedCrc_m = Crc32Finalize(calculatedCrc_m);
    if (calculatedCrc_m != receivedCrc_m)
    {
        return XML_CRC_ERR;
    }

    return NO_ERR;
}
#endif


/*
 * FUNCTION
 *  ParseFactory
 *
 * DESCRIPTION
 *  Parses the factory config file, looking for the data fields identified.  If
 *  a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 *  found other than that the field has not been modified.
 *
 * PARAMETERS
 *  factoryData - Pointer to the data structure where the parsed data should be
 *          stored.
 *
 * RETURNS
 */

AmsError_t
ParseFactory (FactoryData_t * factoryData, unsigned long * version)
{
  FILE *    fp;
  int       done;
  char      buff [BUFF_SIZE];

  version_m = version;
  factoryData_m = factoryData;

  /*
   * Open the config file.
   */
  fp = fopen (FACTORY_CONFIG_FILE_NAME, "rb");
  if (fp == NULL) {
      PrintMsgStatus (OPEN_FACTORY_FILE_FAIL);
      return FACTORY_XML_OPEN_FAIL;
  }

  /*
   * Create the parser and install the start and end tag callbacks.
   */
  parser_m = XML_ParserCreate (NULL);

  if (parser_m == NULL) {
      fclose (fp);
      return CREATE_PARSER_FAIL;
  }

  XML_SetElementHandler (parser_m, StartTag, EndTag);

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
  for (done = 0; !done;) {
    int len;

    // Read the file into the buffer.
    len = fread (buff, 1, BUFF_SIZE, fp);
    if (ferror(fp)) {
        fclose (fp);
        return XML_READ_ERR;
    }

    // EOF, so done.
    done = feof(fp);

    // Parse the buffer.
    if (XML_Parse (parser_m, buff, len, done) == 0) {
        fclose (fp);
        return XML_SYNTAX_ERR;
    }
  }

  fclose(fp);

  // Finalize the CRC calculation and compare it to the CRC read from the file.
  calculatedCrc_m = Crc32Finalize (calculatedCrc_m);
  if (calculatedCrc_m != receivedCrc_m) {
    return XML_CRC_ERR;
  }

  return NO_ERR;
}

AmsError_t
ParseFactoryXml (FactoryData_t * factoryData)
{
  unsigned long version;

  return ParseFactory (factoryData, &version);
}
