/*
 * FILENAME
 * 	ParseDemo.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code to a demo results data file.
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
#include <xmlparse.h>
#include <ctype.h>
#include <errno.h>

#include "ParseDemo.h"
#include "CommonDefines.h"
#include "Crc32.h"	// CRC routines from the Common library
#include "LogErr.h"


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
static bool				inResult_m;
static int				resultNdx_m;
static bool				inAnalyte_m;
static int				analyteNdx_m;
static DemoData_t *		demoData_m = 0;

static void
ParseLotNumber( const char * str, MfgLot_t * lotNum )
{
	int	len = strlen( str );

	memset( lotNum, 0, sizeof( MfgLot_t ) );
	
	if ( (len > 0) && isdigit( str[0] ) )
		lotNum->lsdOfYear = str[0] - '0';
		
	if ( (len > 2) && isdigit( str[1] ) && isdigit( str[2] ) )
		lotNum->weekOfYear = (str[1] - '0') * 10 + (str[2] - '0');
		
	if ( (len > 3) && isdigit( str[3] ) )
		lotNum->dayOfWeek = str[3] - '0';

	if ( len > 4 )
		strncpy( lotNum->code, str + 4, 3 );
}

static int
GetAnalyteIndex( const char * str )
{
	const struct
	{
		const char *	name;
		int				analyteId;
	} ANALYTE_LIST[] = {
		{ "AG"		, AG		}, { "ALB"	, ALB_BCP_A	}, { "ALP"	, ALP		},
		{ "ALT"		, ALT		}, { "AMY"	, AMY		}, { "AST"	, AST		},
		{ "BA"		, BA		}, { "BUN"	, BUN		}, { "CA"	, CA		},
		{ "CHOL"	, CHOL		}, { "CHW"	, CHW		}, { "CK"	, CK		}, { "CL-"	, CL		},
		{ "CL"		, CL		}, { "CRE"	, CRE		}, { "CRP"	, CRP		}, { "DBIL"	, DBIL		},
		{ "EGFR"	, EGFR		}, { "GGT"	, GGT		}, { "GLOB" , GLOB		}, { "GLU"	, GLU		},
		{ "HB"		, HB		}, { "HCT"	, HCT		}, { "HDL"	, HDL		}, { "IBIL" , IBIL		},
		{ "LAC" 	, LAC		}, { "K+"	, K			}, { "K"	, K			}, { "LD"	, LDH		},
		{ "LDL"		, LDL		}, { "MG"	, MG		}, { "NA+"	, NA		}, { "NA"	, NA		},
		{ "nHDLc"	, NHDLC		}, { "PHB"	, PHB		}, { "PHOS", PHOS		}, { "T4"	, T4		},
		{ "TBIL"	, TBIL		},
		{ "TC/H"	, TCH		}, { "TCH"	, TCH		}, { "tCO2"	, TCO2		},
		{ "TP"		, TP		}, { "TRIG"	, TRIG		}, { "UA"	, UA		},
		{ "VLDL"	, VLDL		},
		{ ""		, 0			}
	};

	int	i;
	
	for ( i = 0; ANALYTE_LIST[ i ].name[0] != '\0'; i++ )
	{
		if ( stricmp( str, ANALYTE_LIST[ i ].name ) == 0 )
		{
			return ANALYTE_LIST[ i ].analyteId;
		}
	}
	
	return -1;
}

static void
SetAnalyteParam( DemoResult_t *	dr)
{
	if ( analyteNdx_m < DEMO_MAX_ANALYTES )
	{
		DemoAnalyteInfo_t *	ai = &dr->analytes[ analyteNdx_m ];

		if ( stricmp( paramName_m, "name" ) == 0 )
		{
			ai->analyteIndex = GetAnalyteIndex( paramValue_m );
			if ( ai->analyteIndex == 0xFF )
			{
				LogError( "unrecognized analyte name (%s) in demo file", paramName_m );
			}
		}
		else if ( stricmp( paramName_m, "value" ) == 0 )
		{
			ai->result = (float)atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "resultPrintFlags" ) == 0 )
		{
			ai->resultPrintFlags = strtol( paramValue_m, 0, 0 );
		}
		else if ( stricmp( paramName_m, "posNegValue" ) == 0 )
		{
			ai->posNegValue = (atoi( paramValue_m ) != 0) ? true : false;
		}
		else if ( stricmp( paramName_m, "lowDynamicRangeLimit" ) == 0 )
		{
			ai->lowDynamicRangeLimit = strtod( paramValue_m, 0 );
		}
		else if ( stricmp( paramName_m, "highDynamicRangeLimit" ) == 0 )
		{
			ai->highDynamicRangeLimit = strtod( paramValue_m, 0 );
		}
		else if ( stricmp( paramName_m, "lowSystemRangeLimit" ) == 0 )
		{
			ai->lowSystemRangeLimit = strtod( paramValue_m, 0 );
		}
		else if ( stricmp( paramName_m, "highSystemRangeLimit" ) == 0 )
		{
			ai->highSystemRangeLimit = strtod( paramValue_m, 0 );
		}
	}
}

static void
SetResultParam()
{
	DemoResult_t *	dr;
	
	if ( resultNdx_m < DEMO_MAX_RESULTS )
	{
		dr = &demoData_m->results[ resultNdx_m ];
		if ( inAnalyte_m )
		{
			SetAnalyteParam( dr );
		}
		
		else if ( stricmp( paramName_m, "rotorName" ) == 0 )
		{
			strncpy( dr->rotorName, paramValue_m, sizeof( dr->rotorName) );
		}
		else if ( stricmp( paramName_m, "rotorId" ) == 0 )
		{
			dr->rotorId = atoi( paramValue_m );
		}
		else if ( stricmp( paramName_m, "rotorFormat" ) == 0 )
		{
			dr->rotorFormat = atoi( paramValue_m );
		}
		else if ( stricmp( paramName_m, "lotNumber" ) == 0 )
		{
			ParseLotNumber( paramValue_m, &dr->lotNum );
		}
		else if ( stricmp( paramName_m, "hemValue" ) == 0 )
		{
			dr->hemValue = atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "lipValue" ) == 0 )
		{
			dr->lipValue = atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "ictValue" ) == 0 )
		{
			dr->ictValue = atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "RQC" ) == 0 )
		{
			dr->rqcValue = atof( paramValue_m );
		}
		else if ( stricmp( paramName_m, "acceptableMinimum" ) == 0 )
		{
			dr->acceptableMinimum = atof( paramValue_m );
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
	if ( inResult_m )
	{
		SetResultParam();
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
		if ( stricmp( el, "result" ) == 0 )
		{
			inResult_m = true;
			analyteNdx_m = 0;
		}
		
		else if ( inResult_m && (stricmp( el, "analyte" ) == 0) )
		{
			inAnalyte_m = true;
		}

		else
		{
			paramName_m = strdup( el );
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
			demoData_m->numResults = resultNdx_m;
		}
		
		else if ( stricmp( el, "result" ) == 0 )
		{
			inResult_m = false;
			demoData_m->results[ resultNdx_m ].numAnalytes = analyteNdx_m;
			resultNdx_m++;
		}
		
		else if ( inResult_m && (stricmp( el, "analyte" ) == 0) )
		{
			inAnalyte_m = false;
			analyteNdx_m++;
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
 * 	Init
 * 
 * DESCRIPTION
 * 	Sets default values in the demo results data structure.  This ensures that any values
 *  that are inadvertently not set in the config file will have valid values, and
 *  also initializes unused records to signal they should be ignored.  Also, set the
 * 	indices.
 */

void
Init()
{
	memset( demoData_m, 0, sizeof( DemoResult_t ) );
	
	resultNdx_m = 0;
}

/*
 * FUNCTION
 * 	ParseDemo
 * 
 * DESCRIPTION
 * 	Parses the given config file, looking for the data fields identified.  If
 * 	a given parameter is not found in the file, this will not modify the
 *  associated data field.  No indication will be made if a parameter is not
 * 	found other than that the field has not been modified.
 * 
 * PARAMETERS
 * 	fileName - Name of the demo data file.
 *  demoData - Pointer to location to store the demo data read from the file.
 * RETURNS
 * 	CFG_ERR_NONE if there are no errors parsing the file, else an error value.
 */

CfgError_t
ParseDemo( const char * fileName, DemoData_t * demoData )
{
	FILE *		fp;
	int			done;
	char		buff[ BUFF_SIZE ];
	
	/*
	 * Save the pointer to the data structure we're filling.
	 */
	 demoData_m = demoData;
	 
	 /*
	  * Init the results data to default values, and init the indices.
	  */
	Init();
	 
	/*
	 * Open the demo results file.
	 */
	fp = fopen( fileName, "r" );
	if ( fp == 0 )
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
