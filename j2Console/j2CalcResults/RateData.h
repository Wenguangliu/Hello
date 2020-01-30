#ifndef __RATEDATA_H
#define __RATEDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


#define MAX_TO_CHECK			2
#define MAX_SELECT_WINDOWS		16


typedef struct
{
	unsigned char			checkPass;
	unsigned char			selectAbsorbanceBlankCorrected;
	float					selectAbsorbanceThreshold;
	unsigned long			selectedPassMask;
	unsigned char			slopeStartPass;
	unsigned char			slopeNumPoints;
} SelectParams_t;


typedef struct
{
	unsigned char			selectWavelengthPair;
	unsigned char			derivedSelectThresholdEnabled;
	unsigned char			derivedSelectWavelengthPair;
	float					derivedSelectThresholdMultiplier;

	unsigned char			compareThresholdType;

	unsigned char			numCheckPasses;
	SelectParams_t			selectParams[MAX_SELECT_WINDOWS];

	unsigned char			preEndAbsCheckEnabled;
	float					preEndAbsLimit;
	unsigned char			postEndAbsCheckEnabled;
	float					postEndAbsLimit;
	unsigned long			endAbsAnalyteErrorMask;
} WindowSelectRecord_t;


typedef struct
{
	unsigned char			slopeStartPass;
	unsigned char			slopeNumPoints;
	unsigned char			deltaSumIdxDiff;
	float					deltaSumMulitiplier[MAX_PASSES];

	float					startAbsRangeLow;
	float					startAbsRangeHigh;
	float					deltaSlopeRateMin;
	float					deltaSlopeRateMax;
	float					deltaIncrSlopeMin;	
	float					deltaIncrSlopeMax;	
} DeltaSumMultRecord_t;


typedef struct
{
	unsigned char			rateStdErrorWavelengthPair;
	unsigned char			falseRateWavelengthPair;

	unsigned char			slopeRateCalcMethod;
	DeltaSumMultRecord_t	deltaSumMultRecord;

	float					rateNoiseToggle;
	float					rateNoiseRatioLimit;
	float					rateNoiseAbsoluteLimit;
	unsigned long			rateNotLinearAnalyteErrorMask;

	unsigned char			slopeRateCorrectionEnabled;
	float					slopeRateCorrectionLimit;
	float					slopeRateCorrectionMultiplier;
	unsigned char			slopeRateCorrectionWavelengthPair;
	unsigned char			slopeRateCorrectionStartPass;
	unsigned char			slopeRateCorrectionEndPass;
	float					slopeRateCorrectionRateLimit;
	float					slopeRateCorrectionRateMultiplier;

	unsigned char			falseRateCheckEnabled;
	unsigned char			falseRatePointsToggle;
	float					falseRateRatioLimit;
	float					falseRateAbsoluteLimit;

	float					arrhTemperatureCorrectionFactor;

	unsigned char			bfdCalibrationFactorIndex;

	unsigned char			barcodeFactorSlopeIntCalcMethod;

	unsigned char			bfdFalseRateCorrectionEnabled;
	float					bfdFalseRateCorrectionLimit;
	float					bfdFalseRateCorrectionFactor;

	unsigned char			falseRateCorrectionEnabled;
	float					falseRateCorrectionMultiplier;

	unsigned char			endogenousResultCorrectionMethod;

	unsigned char			linearTimeCorrectionEnabled;
	float					linearTimeCorrectionStabilityRate;
} RateCalcRecord_t;


typedef struct
{
	unsigned char			sampleTypeAnalyte;

	float					beadMixCheckLimit;
	unsigned char			beadMixCheckPass;

	unsigned char			startAbsCheckWavelengthPair[MAX_TO_CHECK];
	unsigned long			startAbsAnalyteErrorMask[MAX_TO_CHECK];
	unsigned char			startAbsCheckPass;
	float					startAbsLimit;

	WindowSelectRecord_t	windowSelectRecord;

	RateCalcRecord_t 		rateCalcRecord;

	unsigned char			rqcCheckEnabled;
	unsigned char			unexpectedValueCheckEnabled;
	float					unexpectedValueHighDynamicRangeAdjust;
	float					beadMissingCheckLimit;
	float					distributionCheckLimit;
} RateAlgRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	RateAlgRecord_t			rateAlgRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} RateConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __RATEDATA_H
