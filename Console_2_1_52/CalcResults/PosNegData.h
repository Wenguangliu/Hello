#ifndef __POSNEGDATA_H
#define __POSNEGDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


#define POS_NEG_START_ABS_CHECKS		8
#define POS_NEG_NUM_RATIO_WAVELENGTHS	2
#define POS_NEG_NUM_RATIO_RESULTS		2
#define POS_NEG_NUM_VALUES_FOR_PASSES	3

typedef struct
{
	unsigned char		startAbsCheckMethodHigh;
	unsigned char		startAbsSampleType;
	unsigned char		startAbsCheckWavelengthPair;
	float				startAbsLimit;
} StartAbsCheck_t;


typedef struct
{
	unsigned char 		blankCorrectionOrderAnalyte;
	unsigned char 		blankCorrectionOrderOpt;
	unsigned char		ratioWavelengthPair[POS_NEG_NUM_RATIO_WAVELENGTHS];

	unsigned char		ratioAvgStartPass[POS_NEG_NUM_RATIO_RESULTS];
	unsigned char		ratioAvgEndPass[POS_NEG_NUM_RATIO_RESULTS];

	float				ratioHighLimit;
	float				ratioLowLimit;
} PosNegCalcRecord_t;


typedef struct
{
	unsigned char		sampleTypeAnalyte;

	unsigned char		sampleTypeOpt;
	unsigned char		optCuvetteNumber;

	float				beadMixCheckLimitAnalyte;
	float				beadMixCheckLimitOpt;
	unsigned char		beadMixCheckPass;

	StartAbsCheck_t		startAbsCheck[POS_NEG_START_ABS_CHECKS];
	unsigned char		startAbsCheckPass;

	PosNegCalcRecord_t	posNegCalcRecord;

	float				beadMissingCheckLimitAnalyte;
	float				beadMissingCheckLimitOpt;
	float				distributionCheckLimitAnalyte;
	float				distributionCheckLimitOpt;
} PosNegAlgRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	PosNegAlgRecord_t		posNegAlgRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} PosNegConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __POSNEGDATA_H
