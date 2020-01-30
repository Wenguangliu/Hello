#ifndef __RQCDATA_H
#define __RQCDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


typedef struct
{
	unsigned char	analyteType;
	char			versionNumber[ALG_VERSION_SIZE];

	unsigned char	sampleTypeAnalyte;
	float			beadMixCheckLimit;
	unsigned char	beadMixCheckPass;

	unsigned char	avgWavelengthPair;
	unsigned char	avgStartPass;
	unsigned char	avgEndPass;

	unsigned char	bfdCalibrationFactorIndex;

	float			rqcRatioSuppressLimit;
	float			rqcHighSuppressLimit;

	float			beadMissingCheckLimit;
	float			distributionCheckLimit;
} RqcConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __RQCDATA_H
