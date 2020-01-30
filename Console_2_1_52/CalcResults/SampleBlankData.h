#ifndef __SAMPLEBLANKDATA_H
#define __SAMPLEBLANKDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


typedef struct
{
	unsigned char	analyteType;
	char			versionNumber[ALG_VERSION_SIZE];

	unsigned char	sampleTypeDiluent;
	float			beadMixCheckLimit;
	unsigned char	beadMixCheckPass;

	unsigned char	avgWavelengthPair[SB_AVG_ABS_PAIRS];
	unsigned char	startAbsCheckPass;
	float			startAbsLimit;

	unsigned char	avgStartPass;
	unsigned char	avgEndPass;

	float			beadMissingCheckLimit;
	float			distributionCheckLimit;
} SampleBlankConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __SAMPLEBLANKDATA_H
