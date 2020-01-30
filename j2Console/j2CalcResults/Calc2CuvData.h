#ifndef __CALC2CUVDATA_H
#define __CALC2CUVDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


// 2 Cuvette calculation types

#define	CALC_2_CUV_AVG					0
#define CALC_2_CUV_BLEND				1
#define	CALC_2_CUV_AVG_1_CUV_OK			2
#define CALC_2_CUV_BLEND_CUV_2			3
#define	CALC_2_CUV_BLEND_ZONES_CUV_2	4


typedef struct
{
	unsigned long			cuvOkSuppressionMask;

	float					inconsistentCheckToggle;
	float					inconsistentCheckMax;
	float					resultDiffLimitScale;
} Avg1CuvOkCheckData_t;


typedef struct
{
	unsigned long			cuvSuppressionMask;

	float					zone1Limit;
	float					zone2Limit;
} Blend1CuvOkCheckData_t;


typedef struct
{
	unsigned char			calc2CuvType;

	float					startCuvBlendLimit;
	float					endCuvBlendLimit;

	unsigned char			inconsistentResultCheckEnabled;
	float					inconsistentResultDiffLimit;

	Avg1CuvOkCheckData_t	avg1CuvOkCheckData;

	Blend1CuvOkCheckData_t	blend1CuvOkCheckData;
} Calc2CuvRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	Calc2CuvRecord_t		calc2CuvRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} Calc2CuvConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __CALC2CUVDATA_H
