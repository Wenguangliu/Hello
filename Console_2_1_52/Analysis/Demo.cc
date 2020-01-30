#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Demo.h"
#include "CommonDefines.h"
#include "Analytes.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "Shmem.h"
#include "ParseDemo.h"
#include "LogErr.h"


static int				ndx = 0;
static DemoData_t		demoData;
static bool				firstTime = true;	// Flag to signal the need to populate the demo results list.

DemoData_t humanDemoBuiltIn =
{
	1,											// Number of rotors
	{
		{
			"Metlyte 8 Panel*",					// Rotor name
			43,	4,								// Rotor ID, Rotor format
			{ 4, 30, 0, "D" },					// Lot number
			-32.0, 210.0, 5.0, 100.0, 50.0, 	// Hem value, Lip value, Ict value, RQC value, Acceptable minium
			8,									// Number of analytes
			{
				{ GLU,		95.5,	0, 0 },		// Analyte index, result, Result print flags, Pos/neg value (!= 0 -> POS)
				{ BUN,		14.5,	0, 0 },
				{ CRE,		0.9,	0, 0 },
				{ CK,		114.5,	0, 0 },
				{ NA,		136.5,	0, 0 },
				{ K,		4.0,	0, 0 },
				{ CL,		102.57,	0, 0 },
				{ TCO2,		25.5,	0, 0 },
				{ NHDLC,	175,	0, 0 },
			},
		},
	},
};

DemoData_t vetDemoBuiltIn =
{
	1,
	{
		{
			"Critical Care Profile*",
			29, 5,
			{ 0, 02, 9, "A" },
			62.3, 283.9, 7.5, 100.00, 50.0, 7,
			{
				{ ALT,		20.1,	0, 0 },
				{ GLU,		89.51,	0, 0 },
				{ BUN,		18.45,	0, 0 },
				{ CRE,		0.851,	0, 0 },
				{ NA,		144.2,	0, 0 },
				{ K,		4.504,	0, 0 },
				{ TCO2,		22.22,	0, 0 },
			},
		},
	},
};


void
PopulateDemoList()
{
	CfgError_t				cfgErr;
	bool					demoParsed = false;
	static FactoryData_t *	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	static SystemData_t *	systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );

	/*
	 * Parse the appropriate demo file. If a key file indicated we're to use the demo
	 * file on the key drive, attempt to do so. If that fails, or if we're not to use
	 * the demo file from a key drive, use the demo file from the local drive. If that
	 * fails, use the built-in demo data.
	 * */

	if ( systemData->demoFromKey )
	{
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
		{
			cfgErr = ParseDemo( THUMB_DRIVE_PATH "/" PICCOLO_DEMO_FILE_NAME, &demoData );
		}
		else
		{
			cfgErr = ParseDemo( THUMB_DRIVE_PATH "/" VETSCAN_DEMO_FILE_NAME, &demoData );
		}
		
		if ( cfgErr == CFG_ERR_NONE )
		{
			demoParsed = true;
		}
		else
		{
			LogError( "error %d parsing demo data file on thumb drive", cfgErr );
		}
	}
	
	if ( !demoParsed )
	{
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
		{
			cfgErr = ParseDemo( DEMO_FILE_DIRECTORY "/" PICCOLO_DEMO_FILE_NAME, &demoData );
		}
		else
		{
			cfgErr = ParseDemo( DEMO_FILE_DIRECTORY "/" VETSCAN_DEMO_FILE_NAME, &demoData );
		}
	}

	// If there was an error parsing the demo file (which includes the file not being there), use the built-in demo entry.
	if ( cfgErr != CFG_ERR_NONE )
	{
		LogError( "error %d parsing demo data file", cfgErr );
		if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
		{
			memcpy( &demoData, &humanDemoBuiltIn, sizeof( DemoData_t ) );
		}
		else
		{
			memcpy( &demoData, &vetDemoBuiltIn, sizeof( DemoData_t ) );
		}
	}

	// Initialize the random number generator.
	srand( time( 0 ) );
}


DemoResult_t *
GetRandomDemoResult()
{
	if ( firstTime )
	{
		firstTime = false;
		PopulateDemoList();
	}

	return &demoData.results[ rand() % demoData.numResults ];
}

DemoResult_t *
GetFirstDemoResult( DemoResult_t * exclude )
{
	ndx = 0;

	return GetNextDemoResult( exclude );
}

DemoResult_t *
GetNextDemoResult( DemoResult_t * exclude )
{
	// If the current result is one we wish to exclude, skip it.
	if ( &demoData.results[ ndx ] == exclude )
	{
		ndx++;
	}

	// If we're at the end of the line, return null, else return the result record.
	if ( ndx >= demoData.numResults )
	{
		return 0;
	}
	else
	{
		return &demoData.results[ ndx++ ];
	}
}

DemoResult_t *
GetDemoResultById( int rotorId )
{
	int	i;
	
	for ( i = 0; i < demoData.numResults; i++ )
	{
		if ( demoData.results[ i ].rotorId == rotorId )
		{
			return &demoData.results[ i ];
		}
	}
	return 0;
}

void
PopulateDemoResults( DemoResult_t * demoResult, RotorResults_t * results )
{
	int						i;
	RotorAnalyteResult_t *	aResult;
	
	// In the special case where we didn't find any of the ROC files for the demo results, demoResult will be null.
	// In that case, we just use the first demo result so we've got something to print, etc.
	if ( demoResult == 0 )
	{
		demoResult = &demoData.results[0];
	}

	// Set the results to the initialized condition.
	memset( results, 0, sizeof( RotorResults_t ) );
	for (i = 0; i < MAX_ANALYTE_TYPES; i++)
	{
		results->rotorAnalyteResult[i].analyteType = 0xff;
		results->rotorAnalyteResult[i].printOrder = 0xff;
	}
	
	results->rotorNumber = demoResult->rotorId;
	results->rotorFormat = demoResult->rotorFormat;
	results->rotorInformationResults.rotorHemolyzedIndex = demoResult->hemValue;
	results->rotorInformationResults.rotorLipemicIndex = demoResult->lipValue;
	results->rotorInformationResults.rotorIctericIndex = demoResult->ictValue;
	results->rotorAnalyteResult[RQC].analyteResult = demoResult->rqcValue;
	results->rotorInformationResults.rotorRqcLowLimit = demoResult->acceptableMinimum;
	
	for ( i = 0; i < demoResult->numAnalytes; i++ )
	{
		aResult = &results->rotorAnalyteResult[ demoResult->analytes[ i ].analyteIndex ];
		
		memset( aResult, 0, sizeof ( RotorAnalyteResult_t ) );
		
		aResult->analyteType = demoResult->analytes[ i ].analyteIndex;
		aResult->printOrder = i+1;
		aResult->analyteResult = demoResult->analytes[ i ].result;
		aResult->resultPrintFlags = demoResult->analytes[ i ].resultPrintFlags;
		aResult->analyteFlags = demoResult->analytes[ i ].posNegValue ? 1 : 0;
		aResult->lowDynamicRangeLimit = demoResult->analytes[ i ].lowDynamicRangeLimit;
		aResult->highDynamicRangeLimit = demoResult->analytes[ i ].highDynamicRangeLimit;
		aResult->lowSystemRangeLimit = demoResult->analytes[ i ].lowSystemRangeLimit;
		aResult->highSystemRangeLimit = demoResult->analytes[ i ].highSystemRangeLimit;
	}
	
// GLD
// Add to xml file?
	for ( i = 0; i < 8; i++ )
	{
		results->rotorInformationResults.rotorIqcLevel1Results[ i ] = 1.0;
		results->rotorInformationResults.rotorIqcLevel2Results[ i ] = 1.0;
	}
	results->rotorInformationResults.rotorIqcRatioPrecision = 1.0;

	results->rotorAnalyteResult[ RQC ].analyteResult = 100.0;
}
