#include <stdio.h>

#include "Units.h"
#include "Shmem.h"
#include "LogErr.h"
#include "CfgErr.h"
#include "ParseUnits.h"

typedef struct
{
	AnalyteId_t		analyte;
	UnitData_t		unitData[5];
} UnitEntry_t;

// Initialization data for options array.
// NOTE: Index 0 has the default non-SI units and index 1 has the default SI units.
// NOTE: The Settings code assumes that mmol/L will be in index 0 and mEq/L will
//		 be in index 3.
UnitEntry_t UNITS[] =
{
    { AID_AG,       { { UID_MMOLL, X, 1 },  { UID_MMOLL, X, 1 },        { UID_MEQL, X, 1 },         {},                         {}                          } },
    { AID_ALB,      { { UID_GDL, Xx, 1 },   { UID_GL, X, 10 },          { UID_MGDL, X00, 1000 },    { UID_MGL, X0, 10000 },     { UID_UMOLL, X00, 151.515 } } },
    { AID_ALP,      { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_NKATL, X0, 16.667 },  { UID_UKATL, Xx, 0.01667 }, {}                          } },
    { AID_ALT,      { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_NKATL, X0, 16.667 },  { UID_UKATL, Xx, 0.01667 }, {}                          } },
    { AID_AMY,      { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_UKATL, Xx, 0.01667 }, {},                         {}                          } },
    { AID_AST,      { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_NKATL, X0, 16.667 },  { UID_UKATL, Xx, 0.01667 }, {}                          } },
    { AID_BA,       { { UID_UMOLL, X, 1 },  { UID_UMOLL, X, 1 },        { UID_MGL, X, 0.4085 },     { UID_UGML, X0, 408.5 },    { UID_MMOLL, Xxxx, 0.001 }  } },
    { AID_BUN,      { { UID_MGDL, X, 1 },   { UID_MMOLL, Xx, 0.357 },   {},                         {},                         {}                          } },
    { AID_CA,       { { UID_MGDL, Xx, 1 },  { UID_MMOLL, Xxx, 0.25 },   { UID_MEQL, Xx, 0.5 },      {},                         {}                          } },
    { AID_CHOL,     { { UID_MGDL, X, 1 },   { UID_MMOLL, Xx, 0.0259 },  { UID_MGL, X, 10 },         {},                         {}                          } },
    { AID_CHW,      { { UID_NONE, X, 1000 },{ UID_NONE, X, 1000, },     {},                         {},                         {}                          } },
    { AID_CK,       { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_UKATL, Xx, 0.01667 }, {},                         {}                          } },
    { AID_CL,       { { UID_MMOLL, X, 1 },  { UID_MMOLL, X, 1 },        { UID_MEQL, X, 1 },         {},                         {}                          } },
    { AID_CRE,      { { UID_MGDL, Xx, 1 },  { UID_UMOLL, X, 88.4 },     {},                         {},                         {}                          } },
    { AID_CRP,      { { UID_MGL, Xx, 1 },   { UID_MGL, Xx, 1 },         { UID_MGDL, Xxx, 0.1 },     {},                         {}                          } },
    { AID_DBIL,     { { UID_MGDL, Xx, 1 },  { UID_UMOLL, X, 17.1 },     {},                         {},                         {}                          } },
    { AID_EGFR,     { { UID_MLMIN, X, 1 },  { UID_MLMIN, X, 1, },       {},                         {},                         {}                          } },
    { AID_GGT,      { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_NKATL, X0, 16.667 },  { UID_UKATL, Xx, 0.01667 }, {}                          } },
    { AID_GLOB,     { { UID_GDL, Xx, 1 },   { UID_GL, X, 10 },          {},                         {},                         {}                          } },
    { AID_GLU,      { { UID_MGDL, X, 1 },   { UID_MMOLL, Xx, 0.05551 }, { UID_MGL, X0, 10 },        {},                         {}                          } },
    { AID_HB,       { { UID_GDL, Xx, 1 },   { UID_GL, X, 10 },          { UID_MMOLL, Xx, 0.6206 },  {},                         {}                          } },
    { AID_HCT,      { { UID_PCT, X, 1 },    { UID_FRAC, Xxx, 0.01 },    {},                         {},                         {}                          } },
    { AID_HDL,      { { UID_MGDL, X, 1 },   { UID_MMOLL, Xxx, 0.0259 }, { UID_MGL, X0, 10 },        {},                         {}                          } },
    { AID_IBIL,     { { UID_MGDL, Xx, 1 },  { UID_UMOLL, X, 17.1 },     {},                         {},                         {}                          } },
    { AID_K,        { { UID_MMOLL, Xx, 1 }, { UID_MMOLL, Xx, 1 },       { UID_MEQL, Xx, 1 },        {},                         {}                          } },
    { AID_LAC,      { { UID_MMOLL, Xxx, 1 },{ UID_MMOLL, Xxx, 1 },      { UID_MGDL, Xx, 9.009009 }, {},                         {}                          } },
    { AID_LD,       { { UID_UL, X, 1 },     { UID_UL, X, 1 },           { UID_UKATL, Xx, 0.01667 }, {},                         {}                          } },
    { AID_LDL,      { { UID_MGDL, X, 1 },   { UID_MMOLL, Xx, 0.0259 },  {},                         {},                         {}                          } },
    { AID_MG,       { { UID_MGDL, Xx, 1 },  { UID_MMOLL, Xxx, 0.4114 }, { UID_MEQL, Xxx, 0.822 },   {},                         {}                          } },
    { AID_NA,       { { UID_MMOLL, X, 1 },  { UID_MMOLL, X, 1 },        { UID_MEQL, X, 1 },         {},                         {}                          } },
    { AID_NHDLC,    { { UID_MGDL, X, 1 },   { UID_MMOLL, Xx, 0.0259 },  { UID_MGL, X, 10 },         {},                         {}                          } },
    { AID_PHB,      { { UID_UGML, Xx, 1 },  { UID_UMOLL, X, 4.31 },   {},                         {},                         {}                          } },
    { AID_PHOS,     { { UID_MGDL, Xx, 1 },  { UID_MMOLL, Xxx, 0.3229 }, {},                         {},                         {}                          } },
    { AID_T4,       { { UID_UGDL, Xx, 1 },  { UID_NMOLL, X, 12.9 },     { UID_UGL, X, 10 },         {},                         {}                          } },
    { AID_TBIL,     { { UID_MGDL, Xx, 1 },  { UID_UMOLL, X, 17.1 },     {},                         {},                         {}                          } },
    { AID_TCH,      { { UID_NONE, Xx, 1 },  { UID_NONE, Xx, 1 },        {},                         {},                         {}                          } },
    { AID_TCO2,     { { UID_MMOLL, X, 1 },  { UID_MMOLL, X, 1 },        { UID_MEQL, X, 1 },         {},                         {}                          } },
    { AID_TP,       { { UID_GDL, Xx, 1 },   { UID_GL, X, 10 },          {},                         {},                         {}                          } },
    { AID_TRIG,     { { UID_MGDL, X, 1 },   { UID_MMOLL, Xxx, 0.0113 }, {},                         {},                         {}                          } },
    { AID_UA,       { { UID_MGDL, Xx, 1 },  { UID_UMOLL, X, 59.5 },     {},                         {},                         {}                          } },
    { AID_VLDL,     { { UID_MGDL, X, 1 },   { UID_MMOLL, Xxx, 0.0254 }, { UID_MGL, X0, 10 },        {},                         {}                          } },

};


Units::
Units()
{
	unsigned	i;
	int			count;
	
	// Map the shared memory that holds our currently-selected units options.
	unitsConfig = (UnitsConfig_t *)MapSharedMem( UNITS_SHMEM_NAME, sizeof( UnitsConfig_t ) );
	
	for ( i = 0; i < sizeof( UNITS ) / sizeof( UnitEntry_t ); i++ )
	{
		for ( count = 0; count < 5; count++ )
		{
			if ( UNITS[ i ].unitData[ count ].unitId == (UnitId_t)0 )
			{
				break;
			}
		}
		
		//options[ UNITS[ i ].analyte ].units = UNITS[ i ].unitData;
		memcpy (options[ UNITS[ i ].analyte ].units, UNITS[ i ].unitData, sizeof (options[ UNITS[ i ].analyte ].units));
// GLD
// Should probably ditch this.
		options[ UNITS[ i ].analyte ].current = unitsConfig->current[ UNITS[ i ].analyte ];
		options[ UNITS[ i ].analyte ].count = count;
	}
}

/*
 * Returns the UnitData_t, which is the unit ID, display format and multiplier
 * for the given analyte's current unit option.
 */
UnitData_t
Units::
operator[]( AnalyteId_t analyteId )
{
	int i;
	
	for ( i = 0; i < options[ analyteId ].count; i++ )
	{
		if ( options[ analyteId ].units[ i ].unitId == unitsConfig->current[ analyteId ] )
		{
			return options[ analyteId ].units[ i ];
		}
	}
	
	// Should never get here.
	return options[ analyteId ].units[ 0 ];
}

UnitOptions_t *
Units::
Options( AnalyteId_t analyteId )
{
	// Make sure we've got latest current option.
	options[ analyteId ].current = unitsConfig->current[ analyteId ];

	return &options[ analyteId ];
}

const wchar_t *
Units::
String( UnitId_t unitId )
{
	switch ( unitId )
	{
		case UID_UL:	return L"U/L";
		case UID_GDL:	return L"g/dL";
		case UID_GL:	return L"g/L";
		case UID_MEQL:	return L"mEq/L";
		case UID_MGDL:	return L"mg/dL";
		case UID_UGDL:	return L"ug/dL";
		case UID_MGL:	return L"mg/L";
		case UID_UGL:	return L"ug/L";
		case UID_UGML:	return L"ug/mL";
		case UID_UKATL:	return L"ukat/L";
		case UID_MMOLL:	return L"mmol/L";
		case UID_UMOLL:	return L"umol/L";
		case UID_NKATL:	return L"nkat/L";
		case UID_NMOLL:	return L"nmol/L";
		case UID_MLMIN:	return L"mL/min";
        case UID_PCT:   return L"%";
        case UID_FRAC:  return L"";
		case UID_NONE:	return L"";
	}
	
	// We should never get here.
	return L"";
}

const wchar_t *
Units::
String( AnalyteId_t analyteId )
{
	return String( unitsConfig->current[ analyteId ] );
}

const wchar_t *
Units::
ListString( UnitId_t unitId, AStrings *strings )
{
    if ( unitId == UID_FRAC )
    {
        return (*strings)[ MSTR_FRACTION ];
    }
    else
    {
        return String( unitId );
    }
}

CfgError_t
Units::
SetAllNonSi( bool doWrite )
{
	int	i;
	
	for ( i = 0; i < AID_NUM_ANALYTES; i++ )
	{
// GLD
// Don't need to save the options[i].current?
		options[ i ].current = options[ i ].units[0].unitId;
		unitsConfig->current[ i ] = options[ i ].units[0].unitId;
	}

	if ( doWrite )
	{
		return WriteUnitsFile();
	}
	else
	{
		return CFG_ERR_NONE;
	}
}

CfgError_t
Units::
SetAllSi()
{
	int	i;
	
	for ( i = 0; i < AID_NUM_ANALYTES; i++ )
	{
// GLD
// Don't need to save the options[i].current?
		options[ i ].current = options[ i ].units[1].unitId;
		unitsConfig->current[ i ] = options[ i ].units[1].unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
Update( AnalyteId_t analyte, UnitId_t unitId )
{
// GLD
// Don't need to save the options[i].current?
	options[ analyte ].current = unitId;
	unitsConfig->current[ analyte ] = unitId;

	return WriteUnitsFile();
}

CfgError_t
Units::
SetElectrolytes( UnitId_t unitId )
{
	unsigned			i;
	const AnalyteId_t	electrolytes[] = { AID_CL, AID_K, AID_NA, AID_TCO2 };

	for ( i = 0; i < sizeof( electrolytes ) / sizeof( AnalyteId_t ); i++ )
	{
		options[ electrolytes[ i ] ].current = unitId;
		unitsConfig->current[ electrolytes[ i ] ] = unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
SetEnzymes( UnitId_t unitId )
{
	unsigned			i;
	const AnalyteId_t	enzymes[] = { AID_ALT, AID_ALP, AID_AMY, AID_AST, AID_CK, AID_GGT, AID_LD };

	for ( i = 0; i < sizeof( enzymes ) / sizeof( AnalyteId_t ); i++ )
	{
		options[ enzymes[ i ] ].current = unitId;
		unitsConfig->current[ enzymes[ i ] ] = unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
SetLipids( UnitId_t unitId )
{
	unsigned			i;
	const AnalyteId_t	lipids[] = { AID_CHOL, AID_HDL, AID_LDL, AID_NHDLC, AID_TRIG, AID_VLDL };
;

	for ( i = 0; i < sizeof( lipids ) / sizeof( AnalyteId_t ); i++ )
	{
		options[ lipids[ i ] ].current = unitId;
		unitsConfig->current[ lipids[ i ] ] = unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
SetMinerals( UnitId_t unitId )
{
	unsigned			i;
	const AnalyteId_t	minerals[] = { AID_CA, AID_MG };

	for ( i = 0; i < sizeof( minerals ) / sizeof( AnalyteId_t ); i++ )
	{
		options[ minerals[ i ] ].current = unitId;
		unitsConfig->current[ minerals[ i ] ] = unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
SetProteins( UnitId_t unitId )
{
	unsigned			i;
	const AnalyteId_t	proteins[] = { AID_ALB, AID_GLOB, AID_TP };

	for ( i = 0; i < sizeof( proteins ) / sizeof( AnalyteId_t ); i++ )
	{
		options[ proteins[ i ] ].current = unitId;
		unitsConfig->current[ proteins[ i ] ] = unitId;
	}

	return WriteUnitsFile();
}

CfgError_t
Units::
SetToDefaults( bool doWrite )
{
	return SetAllNonSi( doWrite );
}

CfgError_t
Units::
WriteUnitsFile()
{
	CfgError_t	cfgErr;
	
	cfgErr = WriteUnits( unitsConfig );
	
	if ( cfgErr != CFG_ERR_NONE )
	{
		LogError( "error writing Units file %s", UNITS_CONFIG_FILE_NAME );
	}
	return cfgErr;
}
	
