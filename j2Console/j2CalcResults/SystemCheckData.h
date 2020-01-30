#ifndef __SYSTEMCHECKDATA_H
#define __SYSTEMCHECKDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


typedef struct
{
	unsigned char	analyteType;
	char			versionNumber[ALG_VERSION_SIZE];

	unsigned char	sampleTypeMixCuv11;
	unsigned char	sampleTypeFillCuv28;

	float			sampleDiluentLowRatioLimit;			
	float			sampleDiluentHighRatioLimit;	
	float			sampleDiluentLowDeltaLimit;		
	float			sampleDiluentHighDeltaLimit;	
	float			plasticAbsCuv28;				
	float			plasticAbsCuv11;				
	float			plasticAbsCuv6;					
	float			sampleDiluentToggle;			
	float			sampleCuvLimit;

	float			wbCheckLimit;

	short			offsetLevelMin;
	short			offsetLevelMax;
	unsigned short	offsetLevelMask;
	float			offsetSdLimit;
	float			opticalDacCvLimit;

	float			temperatureSwingLimit;
	float			maxTemperatureAllowed;
	float			minTemperatureAllowed;
	unsigned		temperatureCheckStartIdx;
	unsigned		temperatureCheckMidIdx;
	unsigned		temperatureCheckEndIdx;
} SystemCheckConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __SYSTEMCHECKDATA_H
