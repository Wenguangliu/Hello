#ifndef __REFRANGES_H_INCLUDED
#define __REFRANGES_H_INCLUDED

#include "Species.h"
#include "Analytes.h"

#define REF_RANGE_FILE_VERSION			3

typedef struct
{
	float			low;
	float			high;
} RefRange_t;

typedef struct
{
	int				len;
	SampleType_t	list[ STYPE_NUM_SPECIES ];
} ActiveRangesList_t;

typedef struct
{
	int				len;
	SpeciesGroup_t	list[ GTYPE_NUM_GROUPS ];
} AvailableGroupsList_t;

typedef struct
{
	int				version;
	RefRange_t		ranges[ STYPE_NUM_SPECIES ][ AID_NUM_ANALYTES ][3];
	bool			isDefined[ STYPE_NUM_SPECIES ];
	AnalyteId_t		lastModifiedAnalyte;
	SampleType_t	lastModifiedSampleType;
} RefRangeData_t;

ActiveRangesList_t *	GetActiveRefRanges( RefRangeData_t * refRanges, AStrings *strings );
void					SetRefRangesToDefaults( RefRangeData_t * refRanges, char productCode );
void					SetRefRangesToDefaultSpecies( RefRangeData_t * refRanges, char productCode );
SampleType_t			GetNextSpeciesNRefRange( RefRangeData_t * refRanges );
SampleType_t			GetNextControlNRefRange( RefRangeData_t * refRanges );
AvailableGroupsList_t *	GetGroupsWithAvailableRefRanges( RefRangeData_t * refRanges );
RefRange_t				GetDefaultRefRange( RefRangeData_t * refRanges, SampleType_t sampleType, AnalyteId_t analyteId, Gender_t gender );

#endif	// __REFRANGES_H_INCLUDED
