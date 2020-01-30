/*
 * FILENAME
 * 	ParseRoc.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to parse ROC files.
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
#ifdef __QNX__
#include <xmlparse.h>
#else
#include "xmlparse.h"
#endif
#include <ctype.h>
#include <errno.h>

#include "ParseRoc.h"
#include "CommonDefines.h"

#ifdef __QNX__
#include "Crc32.h"	// CRC routines from the Common library
#else
#include "Crc.h"

#define true			1
#define false			0
#define Crc32Init		CrcInit
#define Crc32Update		CrcUpdate
#define Crc32Finalize	CrcFinalize
#endif

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE					8196

static XML_Parser		parser_m;
static char				paramValue_m[ BUFF_SIZE ];
static char *			paramName_m;
static char *			attributes_m[2];
static unsigned 		version_m;
static unsigned			receivedCrc_m;
static unsigned			calculatedCrc_m;
static bool				inData_m;
static bool				inParam_m;
static bool				inCrc_m;
static bool				inVersion_m;
static bool				inAnalyteCalculationRecord_m;
static bool				inBarcodeFactor_m;
static bool				inRotorCalculationRecord_m;
static int				analyteCalculationRecordIndex_m;
static int				barcodeFactorIndex_m;
static RotorConfig_t *	rocData_m = 0;

static const float		defaultCuvettePathLengths[ NUM_CUVETTES ] = // Default cuvette hole path length correction values
{
	5.0,		// cuvette 0: not used, SYSTEM - BLOCKED (BLACK_OFFSETS)
	0.4,		// cuvette 1: SYSTEM - SAMPLE CHECK 
	5.0,		// cuvette 2: not used, SYSTEM - DUMP
	5.0,		// cuvette 3: used    , RQC
	5.0,		// cuvette 4: used    , IQC_A
	4.3,		// cuvette 5: used    , IQC_B
	4.3,		// cuvette 6: used    , SYSTEM_CHECK
	5.0,		// cuvette 7: not used, SYSTEM - DUMP
	4.3,		// cuvette 8
	4.3,		// cuvette 9
	3.1,		// cuvette 10
	3.1,		// cuvette 11: used   , SYSTEM_CHECK
	4.3,		// cuvette 12
	2.1,		// cuvette 13
	4.3,		// cuvette 14
	4.3,		// cuvette 15
	2.1,		// cuvette 16
	4.3,		// cuvette 17
	4.3,		// cuvette 18
	4.3,		// cuvette 19
	5.0,		// cuvette 20
	1.7,		// cuvette 21
	4.3,		// cuvette 22
	4.3,		// cuvette 23
	4.3,		// cuvette 24
	4.3,		// cuvette 25
	4.3,		// cuvette 26
	5.0,		// cuvette 27
	4.3,		// cuvette 28: used    , SYSTEM_CHECK
	5.0			// cuvette 29: not used, SYSTEM - HOLE (OPTICAL_BLANK)
};

static void
ParseFloatArray( float array[], int maxCount )
{
	int		i;
	char *	s;
	
	s = strtok( paramValue_m, "," );
	for ( i = 0; i < maxCount && s != 0; i++ )
	{
		array[ i ] = (float)atof( s );
		s = strtok( 0, "," );
	}
}

static void
ParseUcharArray( unsigned char array[], int maxCount )
{
	int		i;
	char *	s;
	
	s = strtok( paramValue_m, "," );
	for ( i = 0; i < maxCount && s != 0; i++ )
	{
		array[ i ] = (unsigned char)atoi( s );
		s = strtok( 0, "," );
	}
}

static void
ParseUnsignedArray( unsigned array[], int maxCount )
{
	int		i;
	char *	s;
	
	s = strtok( paramValue_m, "," );
	for ( i = 0; i < maxCount && s != 0; i++ )
	{
		array[ i ] = (unsigned)atoi( s );
		s = strtok( 0, "," );
	}
}

static void
SetRotorCalculationRecordParam()
{
	RotorCalcRecord_t *		rcr = &rocData_m->rotorCalculationRecord;
	int						index;


	if ( stricmp( paramName_m, "hemolyzed340InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->hemolyzed340InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "lipemic340InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->lipemic340InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "icteric340InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->icteric340InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "hemolyzed405InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->hemolyzed405InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "lipemic405InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->lipemic405InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "icteric405InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->icteric405InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "hemolyzed467InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->hemolyzed467InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "lipemic467InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->lipemic467InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "icteric467InvertedRatioMatrix" ) == 0 )
	{
		ParseFloatArray( rcr->icteric467InvertedRatioMatrix, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "hemolyzedSlope" ) == 0 )
	{
		ParseFloatArray( rcr->hemolyzedSlope, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "hemolyzedIntercept" ) == 0 )
	{
		ParseFloatArray( rcr->hemolyzedIntercept, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "lipemicSlope" ) == 0 )
	{
		ParseFloatArray( rcr->lipemicSlope, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "lipemicIntercept" ) == 0 )
	{
		ParseFloatArray( rcr->lipemicIntercept, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "ictericSlope" ) == 0 )
	{
		ParseFloatArray( rcr->ictericSlope, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "ictericIntercept" ) == 0 )
	{
		ParseFloatArray( rcr->ictericIntercept, NUM_SB_MATRIX );
	}
	else if ( stricmp( paramName_m, "rotorBeadMissingCheckLimit" ) == 0 )
	{
		rcr->rotorBeadMissingCheckLimit = (unsigned)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorDistributionCheckLimit" ) == 0 )
	{
		rcr->rotorDistributionCheckLimit = (unsigned)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorDilutionCheckEnabled" ) == 0 )
	{
		rcr->rotorDilutionCheckEnabled = (unsigned)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorShortSampleProbThreshold" ) == 0 )
	{
		rcr->rotorShortSampleProbThreshold = (float)atof( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorLongSampleProbThreshold" ) == 0 )
	{
		rcr->rotorLongSampleProbThreshold = (float)atof( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorFlags" ) == 0 )
	{
		ParseUnsignedArray( rcr->rotorFlags, NUM_ROTOR_FLAGS_LIMITS );
	}
	else if ( stricmp( paramName_m, "rotorFactorsLimits" ) == 0 )
	{
		ParseFloatArray( rcr->rotorFactorsLimits, NUM_ROTOR_FLAGS_LIMITS );
	}
	else if ( stricmp( paramName_m, "cuvettePathLength" ) == 0 )
	{
		index = -1;
		if ( attributes_m[0] != 0 && stricmp( attributes_m[0], "index" ) == 0 && attributes_m[1] != 0 )
		{
			index = atoi( attributes_m[1] );
		}
		if ( (index >= 0) && (index < NUM_CUVETTES) )
		{		
		
			rcr->rotorCuvettePathLengths[ index ] = atof( paramValue_m );
		}
	}

}

static void
SetBarcodeFactorParam( AnalyteCalcRecord_t * acr )
{
	if ( barcodeFactorIndex_m < NUM_BARCODE_FACTORS )
	{
		if ( stricmp( paramName_m, "index" ) == 0 )
		{
			acr->barcodeFactor[ barcodeFactorIndex_m ].index = (unsigned char)atoi( paramValue_m );
		}
		else if ( stricmp( paramName_m, "scale" ) == 0 )
		{
			acr->barcodeFactor[ barcodeFactorIndex_m ].scale = (float)atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "offset" ) == 0 )
		{
			acr->barcodeFactor[ barcodeFactorIndex_m ].offset = (float)atof( paramValue_m );
		}
	}
}

static void
SetAnalyteCalculationRecordParam()
{
	AnalyteCalcRecord_t	*	acr;
	
	if ( analyteCalculationRecordIndex_m < MAX_ANALYTE_TYPES )
	{
		acr = &rocData_m->analyteCalcRecord[ analyteCalculationRecordIndex_m ];
		if ( inBarcodeFactor_m )
		{
			SetBarcodeFactorParam( acr );
		}
		
		else
		{
			if ( stricmp( paramName_m, "analyteType" ) == 0 )
			{
				acr->analyteType = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "versionNumber" ) == 0 )
			{
				strncpy( acr->versionNumber, paramValue_m, sizeof( acr->versionNumber) );
			}
			else if ( stricmp( paramName_m, "calculationIndex" ) == 0 )
			{
				acr->calculationIndex = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "printOrder" ) == 0 )
			{
				acr->printOrder = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "analyteCuvetteNumber" ) == 0 )
			{
				acr->analyteCuvetteNumber = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "analyteBlankCuvetteNumber" ) == 0 )
			{
				acr->analyteBlankCuvetteNumber = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "analyteDesiredPathLength" ) == 0 )
			{
				acr->analyteDesiredPathLength = (float)atof( paramValue_m );
			}
			else if ( stricmp( paramName_m, "sampleTypeAnalyteBlank" ) == 0 )
			{
				acr->sampleTypeAnalyteBlank = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "sampleTypeOpticalBlank" ) == 0 )
			{
				acr->sampleTypeOpticalBlank = (unsigned char)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "algorithmFlags" ) == 0 )
			{
				ParseUcharArray( acr->algorithmFlags, MAX_CORRECTION_FACTORS );
			}
			else if ( stricmp( paramName_m, "analyteSuppressionMask" ) == 0 )
			{
				acr->rotorFactorsLimits.analyteSuppressionMask = (unsigned)atoi( paramValue_m );
			}
			else if ( stricmp( paramName_m, "hemolyzedLimit" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.hemolyzedLimit, NUM_ENDOGENOUS_ZONES );
			}
			else if ( stricmp( paramName_m, "lipemicLimit" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.lipemicLimit, NUM_ENDOGENOUS_ZONES );
			}
			else if ( stricmp( paramName_m, "ictericLimit" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.ictericLimit, NUM_ENDOGENOUS_ZONES );
			}
			else if ( stricmp( paramName_m, "hemolyzedCorrectionFactors" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.hemolyzedCorrectionFactors, MAX_CORRECTION_FACTORS );
			}
			else if ( stricmp( paramName_m, "lipemicCorrectionFactors" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.lipemicCorrectionFactors, MAX_CORRECTION_FACTORS );
			}
			else if ( stricmp( paramName_m, "ictericCorrectionFactors" ) == 0 )
			{
				ParseFloatArray( acr->rotorFactorsLimits.ictericCorrectionFactors, MAX_CORRECTION_FACTORS );
			}
			else if ( stricmp( paramName_m, "lowSystemLimit" ) == 0 )
			{
				acr->rotorFactorsLimits.lowSystemLimit = (float)atof( paramValue_m );
			}
			else if ( stricmp( paramName_m, "lowDynamicRange" ) == 0 )
			{
				acr->rotorFactorsLimits.lowDynamicRange = (float)atof( paramValue_m );
			}
			else if ( stricmp( paramName_m, "highDynamicRange" ) == 0 )
			{
				acr->rotorFactorsLimits.highDynamicRange = (float)atof( paramValue_m );
			}
			else if ( stricmp( paramName_m, "highSystemLimit" ) == 0 )
			{
				acr->rotorFactorsLimits.highSystemLimit = (float)atof( paramValue_m );
			}
		}
	}
}

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
	if ( inRotorCalculationRecord_m )
	{
		SetRotorCalculationRecordParam();
	}
	else if ( inAnalyteCalculationRecord_m )
	{
		SetAnalyteCalculationRecordParam();
	}
	else if ( stricmp( paramName_m, "rotorNumber" ) == 0 )
	{
		rocData_m->rotorNumber = (unsigned char)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorFormat" ) == 0 )
	{
		rocData_m->rotorFormat = (unsigned char)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorProductCode" ) == 0 )
	{
		rocData_m->rotorProductCode = paramValue_m[0];
	}
	else if ( stricmp( paramName_m, "rotorPrinterName" ) == 0 )
	{
		strncpy( rocData_m->rotorPrinterName, paramValue_m, sizeof( rocData_m->rotorPrinterName ) );
	}
	else if ( stricmp( paramName_m, "rotorScreenNameLine1" ) == 0 )
	{
		strncpy( rocData_m->rotorScreenNameLine1, paramValue_m, sizeof( rocData_m->rotorScreenNameLine1 ) );
	}
	else if ( stricmp( paramName_m, "rotorScreenNameLine2" ) == 0 )
	{
		strncpy( rocData_m->rotorScreenNameLine2, paramValue_m, sizeof( rocData_m->rotorScreenNameLine2 ) );
	}
	else if ( stricmp( paramName_m, "rotorScreenNameFontColor" ) == 0 )
	{
		rocData_m->rotorScreenNameFontColor = (unsigned)atoi( paramValue_m );
	}
	else if ( stricmp( paramName_m, "rotorScreenNameBackgroundColor" ) == 0 )
	{
		rocData_m->rotorScreenNameBackgroundColor = (unsigned)atoi( paramValue_m );
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
	 * Handle the top level tags, plus barcodeFactor and cuvettePath, which have index parameters.
	 */
	if ( inData_m )
	{
		if ( stricmp( el, "rotorCalculationRecord" ) == 0 )
		{
			inRotorCalculationRecord_m = true;
		}
		
		else if ( stricmp( el, "analyteCalculationRecord" ) == 0 )
		{
			if ( attr[0] != 0 && stricmp( attr[0], "index" ) == 0 && attr[1] != 0 )
			{
				analyteCalculationRecordIndex_m = atoi( attr[1] );
				inAnalyteCalculationRecord_m = true;
			}
		}

		else if ( stricmp( el, "barcodeFactor" ) == 0 )
		{
			if ( attr[0] != 0 && stricmp( attr[0], "index" ) == 0 && attr[1] != 0 )
			{
				barcodeFactorIndex_m = atoi( attr[1] );
				inBarcodeFactor_m = true;
			}
		}

		else
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
		
		else if ( stricmp( el, "rotorCalculationRecord" ) == 0 )
		{
			inRotorCalculationRecord_m = false;
		}
		
		else if ( stricmp( el, "analyteCalculationRecord" ) == 0 )
		{
			inAnalyteCalculationRecord_m = false;
		}
		
		else if ( stricmp( el, "barcodeFactor" ) == 0 )
		{
			inBarcodeFactor_m = false;
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
 * 	SetDefaults
 * 
 * DESCRIPTION
 * 	Sets default values in the ROC data structure.  This ensures that any values
 *  that are inadvertently not set in the ROC file will have valid values, and
 *  also initializes unused analyteCalcRecords to signal they should be ignored.
 */

void
SetDefaults()
{
	int i, j;
	
	// Start by setting everything to 0.
	memset( rocData_m, 0, sizeof( RotorConfig_t ) );

	// Make sure we have a valid rotor number, format and product code.
	rocData_m->rotorNumber = 52;
	rocData_m->rotorFormat = 2;
	rocData_m->rotorProductCode = HUMAN_ROTOR;
	
	// calculationIndex == 255 means don't calculate this one.
	// printOrder == 255 means don't print this one.
	// barcodeFactor[x].index == 0 is not valid, so set to 1 instead.
	for ( i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		rocData_m->analyteCalcRecord[i].calculationIndex = 255;
		rocData_m->analyteCalcRecord[i].printOrder = 255;
		for ( j = 0; j < NUM_BARCODE_FACTORS; j++ )
		{
			rocData_m->analyteCalcRecord[i].barcodeFactor[j].index = 1;
		}
	}
	
	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		rocData_m->rotorCalculationRecord.rotorCuvettePathLengths[i] = defaultCuvettePathLengths[i];
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
ParseRoc( char * fileName, RotorConfig_t * rocData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the ROC data structure we're filling.
	 */
	 rocData_m = rocData;
	 
	 /*
	  * Init the ROC data to default values.
	  */
	 SetDefaults();
	 
	/*
	 * Open the ROC file.
	 */
	fp = fopen( fileName, "r" );
	if ( fp == NULL )
	{
		if ( errno == ENOENT )
		{
			return CFG_ERR_FILE_DOES_NOT_EXIST;
		}
		else
		{
			return CFG_ERR_FILE_OPEN;
		}
	}
	
	/*
	 * Create the parser and install the start and end tag callbacks.
	 */
	parser_m = XML_ParserCreate( NULL );
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
