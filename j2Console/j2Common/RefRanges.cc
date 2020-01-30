#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "RefRanges.h"
#include "Species.h"
#include "LogErr.h"
#include "Shmem.h"

typedef struct
{
	AnalyteId_t	id;
	RefRange_t	range;
} InitData_t;

/*
 * Vet Defaults
 */

const InitData_t DOG_DATA[] =
{
	{ AID_ALB	, { 2.5,	4.4 } },
	{ AID_ALP	, { 20,		150 } },
	{ AID_ALT	, { 10,		118 } },
	{ AID_AMY	, { 200,	1200 } },
	{ AID_AST	, { 14,		45 } },
	{ AID_BA	, { 0,		25 } },
	{ AID_BUN	, { 7,		25 } },
	{ AID_CA	, { 8.6,	11.8 } },
	{ AID_CHOL	, { 125,	270 } },
	{ AID_CHW	, { 0.0,	0.0 } },
	{ AID_CK	, { 20,		200 } },
	{ AID_CL	, { 95,	    119 } },
	{ AID_CRE	, { 0.3,	1.4 } },
	{ AID_GGT	, { 0,		7 } },
	{ AID_GLOB	, { 2.3,	5.2 } },
	{ AID_GLU	, { 60,		110 } },
	{ AID_K		, { 3.7,	5.8 } },
	{ AID_MG	, { 1.8,	2.4 } },
	{ AID_NA	, { 138,	160 } },
    { AID_PHB   , { 10,     45 } },
	{ AID_PHOS	, { 2.9,	6.6 } },
	{ AID_T4	, { 1.1,	4.0 } },
	{ AID_TBIL	, { 0.1,	0.6 } },
	{ AID_TCO2	, { 12,		27 } },
	{ AID_TP	, { 5.4,	8.2 } },
	{ AID_UA	, { 1.0,	9.0 } },
};

const InitData_t CAT_DATA[] =
{
	{ AID_ALB	, { 2.2,	4.4 } },
	{ AID_ALP	, { 10,		90 } },
	{ AID_ALT	, { 20,		100 } },
	{ AID_AMY	, { 300,	1100 } },
	{ AID_AST	, { 12,		43 } },
	{ AID_BA	, { 0,		25 } },
	{ AID_BUN	, { 10,		30 } },
	{ AID_CA	, { 8.0,	11.8 } },
	{ AID_CHOL	, { 90,		205 } },
	{ AID_CHW	, { 0.0,	0.0 } },
	{ AID_CK	, { 50,		450 } },
	{ AID_CL	, { 99,	    122 } },
	{ AID_CRE	, { 0.3,	2.1 } },
	{ AID_GGT	, { 0,		2 } },
	{ AID_GLOB	, { 1.5,	5.7 } },
	{ AID_GLU	, { 70,		150 } },
	{ AID_K		, { 3.7,	5.8 } },
	{ AID_MG	, { 2.0,	2.5 } },
	{ AID_NA	, { 142,	164 } },
    { AID_PHB   , { 10,     45 } },
	{ AID_PHOS	, { 3.4,	8.5 } },
	{ AID_T4	, { 1.5,	4.8 } },
	{ AID_TBIL	, { 0.1,	0.6 } },
	{ AID_TCO2	, { 15,		24 } },
	{ AID_TP	, { 5.4,	8.2 } },
	{ AID_UA	, { 1.0,	9.0 } },
};

const InitData_t HORSE_DATA[] =
{
	{ AID_ALB	, { 2.2,	3.7 } },
	{ AID_ALP	, { 50,		170 } },
	{ AID_ALT	, { 5,		20 } },
	{ AID_AMY	, { 5,		15 } },
	{ AID_AST	, { 175,	340 } },
	{ AID_BA	, { 0,		25 } },
	{ AID_BUN	, { 7,		25 } },
	{ AID_CA	, { 11.5,	14.2 } },
	{ AID_CHOL	, { 50,		140 } },
	{ AID_CHW	, { 0.0,	0.0 } },
	{ AID_CK	, { 120,	470 } },
	{ AID_CL	, { 92,		104 } },
	{ AID_CRE	, { 0.6,	2.2 } },
	{ AID_GGT	, { 5,		24 } },
	{ AID_GLOB	, { 2.7,	5.0 } },
	{ AID_GLU	, { 65,		110 } },
	{ AID_K		, { 2.5,	5.2 } },
	{ AID_MG	, { 1.7,	2.9 } },
	{ AID_NA	, { 126,	146 } },
	{ AID_PHOS	, { 1.9,	4.3 } },
	{ AID_T4	, { 1.0,	2.8 } },
	{ AID_TBIL	, { 0.5,	2.3 } },
	{ AID_TCO2	, { 20,		33 } },
	{ AID_TP	, { 5.7,	8.0 } },
	{ AID_UA	, { 1.0,	9.0 } },
};

/*
 * Human Defaults
 */

const InitData_t UNKNOWN_DATA[] = 
{
// GLD
// Do we need the non-printing ones?
	{ AID_AG	, { 0.0,	0.0 } },
	{ AID_ALB	, { 3.3,	5.5 } },
	{ AID_ALP	, { 42.0,	141.0 } },
	{ AID_ALT	, { 10.0,	47.0 } },
	{ AID_AMY	, { 14.0,	97.0 } },
	{ AID_AST	, { 11.0,	38.0 } },
	{ AID_BUN	, { 7.0,	22.0 } },
	{ AID_CA	, { 8.0,	10.3 } },
	{ AID_CHOL	, { 0.0,	0.0 } },
	{ AID_CK	, { 30.0,	380.0 } },
	{ AID_CL	, { 98.0,	108.0 } },
	{ AID_CRE	, { 0.6,	1.2 } },
	{ AID_CRP	, { 0.0,	7.5 } },
	{ AID_DBIL	, { 0.0,	0.3 } },
	{ AID_EGFR	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GGT	, { 5.0,	65.0 } },
	{ AID_GLOB	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GLU	, { 73.0,	118.0 } },
	{ AID_HB	, { 11.0,	18.0 } },
	{ AID_HCT	, { 34.0,	51.0 } },
	{ AID_HDL	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_K		, { 3.6,	5.1 } },
	{ AID_LAC	, { 0.53,	2.1 } },
	{ AID_LD	, { 99.0,	192.0 } },
	{ AID_LDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_MG	, { 1.6,	2.3 } },
	{ AID_NA	, { 128.0,	145.0 } },
	{ AID_NHDLC	, { 0.0,	0.0 } },
    { AID_PHB   , { 0.0,    0.0 } },
	{ AID_PHOS	, { 2.2,	4.1 } },
	{ AID_TBIL	, { 0.2,	1.6	 } },
	{ AID_TCH	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_TCO2	, { 18.0,	33.0 } },
	{ AID_TP	, { 6.4,	8.1	} },
	{ AID_TRIG	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_UA	, { 2.2,	8.0 } },
	{ AID_VLDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
};

const InitData_t MALE_DATA[] = 
{
	{ AID_AG	, { 0.0,	0.0 } },
	{ AID_ALB	, { 3.3,	5.5 } },
	{ AID_ALP	, { 53.0,	128.0 } },
	{ AID_ALT	, { 10.0,	47.0 } },
	{ AID_AMY	, { 14.0,	97.0 } },
	{ AID_AST	, { 11.0,	38.0 } },
	{ AID_BUN	, { 7.0,	22.0 } },
	{ AID_CA	, { 8.0,	10.3 } },
	{ AID_CHOL	, { 0.0,	0.0 } },
	{ AID_CK	, { 39.0,	380.0 } },
	{ AID_CL	, { 98.0,	108.0 } },
	{ AID_CRE	, { 0.6,	1.2 } },
	{ AID_CRP	, { 0.0,	7.5 } },
	{ AID_DBIL	, { 0.0,	0.3 } },
	{ AID_EGFR	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GGT	, { 5.0,	65.0 } },
	{ AID_GLOB	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GLU	, { 73.0,	118.0 } },
	{ AID_HB	, { 12.0,	18.0 } },
	{ AID_HCT	, { 40.0,	51.0 } },
	{ AID_HDL	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_K		, { 3.6,	5.1 } },
	{ AID_LAC	, { 0.53,	2.1 } },
	{ AID_LD	, { 99.0,	192.0 } },
	{ AID_LDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_MG	, { 1.6,	2.3 } },
	{ AID_NA	, { 128.0,	145.0 } },
	{ AID_NHDLC	, { 0.0,	0.0 } },
	{ AID_PHOS	, { 2.2,	4.1 } },
	{ AID_TBIL	, { 0.2,	1.6	 } },
	{ AID_TCH	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_TCO2	, { 18.0,	33.0 } },
	{ AID_TP	, { 6.4,	8.1	} },
	{ AID_TRIG	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_UA	, { 3.6,	8.0 } },
	{ AID_VLDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
};


const InitData_t FEMALE_DATA[] = 
{
	{ AID_AG	, { 0.0,	0.0 } },
	{ AID_ALB	, { 3.3,	5.5 } },
	{ AID_ALP	, { 42.0,	141.0 } },
	{ AID_ALT	, { 10.0,	47.0 } },
	{ AID_AMY	, { 14.0,	97.0 } },
	{ AID_AST	, { 11.0,	38.0 } },
	{ AID_BUN	, { 7.0,	22.0 } },
	{ AID_CA	, { 8.0,	10.3 } },
	{ AID_CHOL	, { 0.0,	0.0 } },
	{ AID_CK	, { 30.0,	190.0 } },
	{ AID_CL	, { 98.0,	108.0 } },
	{ AID_CRE	, { 0.6,	1.2 } },
	{ AID_CRP	, { 0.0,	7.5 } },
	{ AID_DBIL	, { 0.0,	0.3 } },
	{ AID_EGFR	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GGT	, { 5.0,	65.0 } },
	{ AID_GLOB	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_GLU	, { 73.0,	118.0 } },
	{ AID_HB	, { 11.0,	16.0 } },
	{ AID_HCT	, { 34.0,	45.0 } },
	{ AID_HDL	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_K		, { 3.6,	5.1 } },
	{ AID_LAC	, { 0.53,	2.1 } },
	{ AID_LD	, { 99.0,	192.0 } },
	{ AID_LDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_MG	, { 1.6,	2.3 } },
	{ AID_NA	, { 128.0,	145.0 } },
	{ AID_NHDLC	, { 0.0,	0.0 } },
	{ AID_PHOS	, { 2.2,	4.1 } },
	{ AID_TBIL	, { 0.2,	1.6	 } },
	{ AID_TCH	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
	{ AID_TCO2	, { 18.0,	33.0 } },
	{ AID_TP	, { 6.4,	8.1	} },
	{ AID_TRIG	, { 0.0,	0.0 } },	/* Lipid Assay, reference range not printed */
	{ AID_UA	, { 2.2,	6.6 } },
	{ AID_VLDL	, { 0.0,	0.0 } },	/* Calculated value, reference range not printed */
};


static int	RangeNameCompare( const void * first, const void * second);

void
SetRefRangesToDefaults( RefRangeData_t * refRanges, char productCode )
{
	unsigned i;
	
	memset( refRanges, 0, sizeof( RefRangeData_t ) );
	
	if ( productCode == PICCOLO_PRODUCT_CODE )
	{
		refRanges->isDefined[ STYPE_OTHER ] = true;		// Patient
		refRanges->isDefined[ STYPE_CONTROL ] = true;	// Control

		for ( i = 0; i < sizeof( UNKNOWN_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_OTHER ][ UNKNOWN_DATA[ i ].id][ GENDER_UNKNOWN-1 ] = UNKNOWN_DATA[ i ].range;
		}
		for ( i = 0; i < sizeof( MALE_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_OTHER ][ MALE_DATA[ i ].id][ GENDER_MALE-1 ] = MALE_DATA[ i ].range;
		}
		for ( i = 0; i < sizeof( FEMALE_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_OTHER ][ FEMALE_DATA[ i ].id][ GENDER_FEMALE-1 ] = FEMALE_DATA[ i ].range;
		}
	}

	else
	{
		refRanges->isDefined[ STYPE_DOG ] = true;
		refRanges->isDefined[ STYPE_CAT ] = true;
		refRanges->isDefined[ STYPE_HORSE ] = true;
		refRanges->isDefined[ STYPE_OTHER ] = true;
		
		for ( i = 0; i < sizeof( DOG_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_DOG ][ DOG_DATA[ i ].id ][0] = DOG_DATA[ i ].range;
		}
	
		for ( i = 0; i < sizeof( CAT_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_CAT ][ CAT_DATA[ i ].id ][0] = CAT_DATA[ i ].range;
		}
	
		for ( i = 0; i < sizeof( HORSE_DATA ) / sizeof( InitData_t ); i++ )
		{
			refRanges->ranges[ STYPE_HORSE ][ HORSE_DATA[ i ].id ][0] = HORSE_DATA[ i ].range;
		}
	}

	refRanges->lastModifiedAnalyte = AID_ALB;
	refRanges->lastModifiedSampleType = STYPE_OTHER;
}

void
SetRefRangesToDefaultSpecies( RefRangeData_t * refRanges, char productCode )
{
	// Start by making all ref ranges undefined.
	memset( &refRanges->isDefined, 0, STYPE_NUM_SPECIES * sizeof( bool ) );

	// Then set just the default ones to defined.
	if ( productCode == PICCOLO_PRODUCT_CODE )
	{
		refRanges->isDefined[ STYPE_OTHER ] = true;		// Patient
		refRanges->isDefined[ STYPE_CONTROL ] = true;	// Control
	}
	else
	{
		refRanges->isDefined[ STYPE_DOG ] = true;
		refRanges->isDefined[ STYPE_CAT ] = true;
		refRanges->isDefined[ STYPE_HORSE ] = true;
		refRanges->isDefined[ STYPE_OTHER ] = true;
	}
}

static AStrings *localStrings = NULL;

ActiveRangesList_t *
GetActiveRefRanges( RefRangeData_t * refRanges, AStrings *strings )
{
	int							i;
	static ActiveRangesList_t	activeRanges;
	
	// Get the active ranges.
	for ( i = 0, activeRanges.len = 0; i < STYPE_NUM_SPECIES; i++ )
	{
		if ( refRanges->isDefined[ i ] )
		{
			activeRanges.list[ activeRanges.len ] = (SampleType_t)i;
			activeRanges.len++;
		}
	}
	
    // get local string address for RangeNameCompare function used 
    if (localStrings == NULL) {
        localStrings = strings;
    }
	// Sort the list.
	qsort( &activeRanges.list, activeRanges.len, sizeof( SampleType_t ), RangeNameCompare);
	
	return &activeRanges;
}

int
RangeNameCompare( const void * _first, const void * _second )
{
	SampleType_t	first = *(SampleType_t *)_first;
	SampleType_t	second = *(SampleType_t *)_second;
	static Species	species;
	
	return wcscmp( species.GetSampleTypeString (first, localStrings),  species.GetSampleTypeString (second, localStrings) );
}

SampleType_t
GetNextSpeciesNRefRange( RefRangeData_t * refRanges )
{
	int i;
	
	// If we've issued all the Species at some point or other, we start from the
	// beginning and find the first one available.
	if ( refRanges->isDefined[ STYPE_LAST_SPECIES ] )
	{
		for ( i = STYPE_FIRST_SPECIES; i < STYPE_LAST_SPECIES; i++ )
		{
			if ( !refRanges->isDefined[ i ] )
			{
				return (SampleType_t)i;
			}
		}
		
		// We didn't find an unused Control.
		return STYPE_NONE;
	}
	
	// Look for the next Species to issue.  We start at the end and look for the
	// last one that's defined, then return next higher index.
	else
	{
		for ( i = STYPE_LAST_SPECIES; i >= STYPE_FIRST_SPECIES; i-- )
		{
			if ( refRanges->isDefined[ i ] )
			{
				return (SampleType_t)(i + 1);
			}
		}
		
		// None defined, so return the first one.
		return STYPE_FIRST_SPECIES;
	}
}

SampleType_t
GetNextControlNRefRange( RefRangeData_t * refRanges )
{
	int i;
	
	// If we've issued all the Controls at some point or other, we start from the
	// beginning and find the first one available.
	if ( refRanges->isDefined[ STYPE_LAST_CONTROL ] )
	{
		for ( i = STYPE_FIRST_CONTROL; i < STYPE_LAST_CONTROL; i++ )
		{
			if ( !refRanges->isDefined[ i ] )
			{
				return (SampleType_t)i;
			}
		}
		
		// We didn't find an unused Control.
		return STYPE_NONE;
	}
	
	// Look for the next Control to issue.  We start at the end and look for the
	// last one that's defined, then return next higher index.
	else
	{
		for ( i = STYPE_LAST_CONTROL; i >= STYPE_FIRST_CONTROL; i-- )
		{
			if ( refRanges->isDefined[ i ] )
			{
				return (SampleType_t)(i + 1);
			}
		}
		
		// None defined, so return the first one.
		return STYPE_FIRST_CONTROL;
	}
}

AvailableGroupsList_t *
GetGroupsWithAvailableRefRanges( RefRangeData_t * refRanges )
{
	static AvailableGroupsList_t	aList;
	static Species					species;
	SpeciesGroupList_t				groupList;
	int								i, j;
	
	for ( i = GTYPE_FIRST_GROUP, aList.len = 0; i <= GTYPE_LAST_GROUP; i++ )
	{
		groupList = species.GetGroupList( (SpeciesGroup_t)i );
		for ( j = 0; j < groupList.len; j++ )
		{
			if ( !refRanges->isDefined[ groupList.list[ j ] ] )
			{
				aList.list[ aList.len ] = (SpeciesGroup_t)i;
				aList.len++;
				break;
			}
		}
	}

	return &aList;
}

RefRange_t
GetDefaultRefRange( RefRangeData_t * refRanges,
					SampleType_t sampleType,
					AnalyteId_t analyteId,
					Gender_t gender )
{
	unsigned				i;
	static RefRange_t		zeroRange = { 0.0, 0.0 };
	static FactoryData_t *	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		if ( sampleType == STYPE_PATIENT )
		{
			switch ( gender )
			{
				case GENDER_UNKNOWN:
					for ( i = 0; i < sizeof( UNKNOWN_DATA ) / sizeof( InitData_t ); i++ )
					{
						if ( analyteId == UNKNOWN_DATA[ i ].id )
						{
							return UNKNOWN_DATA[ i ].range;
						}
					}
					break;

				case GENDER_MALE:
					for ( i = 0; i < sizeof( MALE_DATA ) / sizeof( InitData_t ); i++ )
					{
						if ( analyteId == MALE_DATA[ i ].id )
						{
							return MALE_DATA[ i ].range;
						}
					}
					break;

				case GENDER_FEMALE:
					for ( i = 0; i < sizeof( FEMALE_DATA ) / sizeof( InitData_t ); i++ )
					{
						if ( analyteId == FEMALE_DATA[ i ].id )
						{
							return FEMALE_DATA[ i ].range;
						}
					}
					break;

				default:
					break;
			}
		}
	}
	
	else
	{
		switch ( sampleType )
		{
			case STYPE_DOG:
				for ( i = 0; i < sizeof( DOG_DATA ) / sizeof( InitData_t ); i++ )
				{
					if ( analyteId == DOG_DATA[ i ].id )
					{
						return DOG_DATA[ i ].range;
					}
				}
				break;
				
			case STYPE_CAT:
				for ( i = 0; i < sizeof( CAT_DATA ) / sizeof( InitData_t ); i++ )
				{
					if ( analyteId == CAT_DATA[ i ].id )
					{
						return CAT_DATA[ i ].range;
					}
				}
				break;
				
			case STYPE_HORSE:
				for ( i = 0; i < sizeof( HORSE_DATA ) / sizeof( InitData_t ); i++ )
				{
					if ( analyteId == HORSE_DATA[ i ].id )
					{
						return HORSE_DATA[ i ].range;
					}
				}
				break;
				
			default:
				break;
		}
	}
	
	return zeroRange;
}
