#ifndef __ENDPOINTDATA_H
#define __ENDPOINTDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


#define MAX_TO_AVG				2

#define MAX_MFG_LOTS_TO_CHECK	10
#define MFG_LOT_STRING_SIZE		8


typedef struct
{
	unsigned char		avgWavelengthPair;

    unsigned char       turbidityPassCorrectionEnabled;
    unsigned char       turbidityPassCorrectionWavelengthPair;
    unsigned char       turbidityPassCorrectionStartPass;
    unsigned char       turbidityPassCorrectionEndPass;
    unsigned long       turbidityPassCorrectionSexSpecies;
    float               turbidityPassCorrectionLimit;
    unsigned char       turbidityCorrectedStartPassAvg;
    unsigned char       turbidityCorrectedEndPassAvg;
   
    unsigned char       rateInEndpointCheckEnabled;
    float               flatnessToggle;
    float               flatnessRatioLimit;
    float               flatnessAbsoluteLimit;
    unsigned long       rateInEndpointErrorMask;


	unsigned char		overSystemRangeCheckEnabled;
	unsigned char		overSystemRangeStartPass;
	unsigned char		overSystemRangeEndPass;
	unsigned char		overSystemRangeLessThanLimit;
	float				overSystemRangeLimit;
	unsigned long		overSystemRangeErrorMask;
} AvgParams_t;


typedef struct
{
	unsigned char		numToAvg;
	unsigned char		avgBlankCorrectionOrder;
	unsigned char		avgStartPass;
	unsigned char		avgEndPass;

	AvgParams_t			avgParams[MAX_TO_AVG];

} EndpointAvgRecord_t;


typedef struct
{
	unsigned char		checkEnabled;
	unsigned char		wavelengthPairs[MAX_TO_AVG];
	unsigned char		checkPass;
	unsigned char		blankCorrectionOrder;

	char				mfgLots[MAX_MFG_LOTS_TO_CHECK][MFG_LOT_STRING_SIZE];

	float				highMinLimit;
	float				lowMinLimit;
} WavelengthRatioCheck_t;


typedef struct
{
	unsigned char			multipleAvgResultCalcEnabled;
	unsigned char			bfdCalibrationFactorIndex[MAX_TO_AVG];

	unsigned char			barcodeFactorSlopeIntCalcMethod;

	unsigned char			weightedResultCalcMethod;		
	float					weightedResult1Factor;			
	float					weightedResult2Factor;

	unsigned char			resultWavelengthCompareType;
	float					resultWavelengthCompareToggle;
	float					resultWavelengthCompareLowLimit;			
	float					resultWavelengthCompareHighLimit;			

	WavelengthRatioCheck_t	wavelengthRatioCheck;	

	unsigned char			linearTimeCorrectionEnabled;
	float					linearTimeCorrectionStabilityRate;
} EndpointCalcRecord_t;


typedef struct
{
	unsigned char			sampleTypeAnalyte;

	float					beadMixCheckLimitAnalyte;
	float					beadMixCheckLimitBlank;
	unsigned char			beadMixCheckPass;

	unsigned char			startAbsCheckWavelengthPair[MAX_TO_AVG];
	unsigned char			startAbsCheckPass;
	float					startAbsLimitAnalyte[MAX_TO_AVG];
	float					startAbsLimitBlank;
	unsigned long			startAbsBlankErrorMask;

	EndpointAvgRecord_t		endpointAvgRecord;

	EndpointCalcRecord_t	endpointCalcRecord;

	unsigned char			rqcCheckEnabled;

	float					beadMissingCheckLimitAnalyte;
	float					beadMissingCheckLimitBlank;
	float					distributionCheckLimitAnalyte;
	float					distributionCheckLimitBlank;
} EndpointAlgRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	EndpointAlgRecord_t		endpointAlgRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} EndpointConfig_t;


#if defined(__cplusplus)
}
#endif

#endif	// __ENDPOINTDATA_H
