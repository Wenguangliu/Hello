#ifndef __IQCDATA_H
#define __IQCDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


typedef struct
{
	unsigned char	analyteType;
	char			versionNumber[ALG_VERSION_SIZE];

	unsigned char	sampleTypeIqcA;
	unsigned char	iqcACuvetteNumber;
	float			iqcAPathLength;

	unsigned char	sampleTypeIqcB;
	unsigned char	iqcBCuvetteNumber;
	float			iqcBPathLength;

	float			iqcLevel1Min;
	float			iqcLevel1Max;

	float			iqcLevel2Min;
	float			iqcLevel2Max;
	float			iqcPrecisionMin;
	float			iqcPrecisionMax;
	float			iqcPrecisionLimit;
	float			iqcRatioSigma;
	float			iqcLowAbsorbanceLimit;

	float			beadMissingCheckLimitIqcA;
	float			beadMissingCheckLimitIqcB;
	float			distributionCheckLimitIqcA;
	float			distributionCheckLimitIqcB;
} IqcConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __IQCDATA_H
