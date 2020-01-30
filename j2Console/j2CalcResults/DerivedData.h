#ifndef __DERIVEDCALCDATA_H
#define __DERIVEDCALCDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


typedef struct
{
	unsigned 				derivedFlags[8];
	float					derivedFactorsLimits[8];
} DerivedAlgRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	DerivedAlgRecord_t		derivedAlgRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} DerivedConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __DERIVEDCALCDATA_H
