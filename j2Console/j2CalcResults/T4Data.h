#ifndef __T4DATA_H
#define __T4DATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "AlgGlobals.h"


#define NUM_BEAD_DIST_CUV		3
#define NUM_SAMPLE_CHECK_AVG	4


typedef struct
{
	unsigned char		sampleType;
	unsigned char		wavelengthPair;
	unsigned char		cuvetteNumber;
	float				desiredPathLength;
} T4SampleCheckAvg_t;


typedef struct
{
	unsigned char		startPass;
	unsigned char		endPass;

	unsigned char		rateStdErrorWavelengthPair;
	unsigned char		falseRateWavelengthPair;

	unsigned char		cuvetteNumberT4G6P1;
	float				desiredPathLengthT4G6P1;

	float				rateNoiseAbsoluteLimit;
	float				falseRateAbsoluteLimit;
	float				maxDeltaLimitT4G6P1;
	float				arrhTemperatureCorrectionFactorT4L1;
	float				arrhTemperatureCorrectionFactorT4G6P1;

	unsigned char		bfdCalibrationFactorIndex;

	T4SampleCheckAvg_t	t4SampleCheckAvg[NUM_SAMPLE_CHECK_AVG];
	float				sampleCheckLimit;
} T4CalcRecord_t;


typedef struct
{
	unsigned char		cuvetteNumber;
	float				beadMissingCheckLimit;
	float				distributionCheckLimit;
} T4BeadDistCheck_t;


typedef struct
{
	float				beadMixCheckLimitT4L1;
	float				beadMixCheckLimitG6P1;
	unsigned char		beadMixCheckPass;

	float				t4HemComboLimit;
	float				t4LipComboLimit;

	unsigned char		startAbsCheckWavelengthPair;
	unsigned char		startAbsCheckPass;
	float				startAbsLimit;

	T4CalcRecord_t		t4CalcRecord;

	T4BeadDistCheck_t	t4BeadDistCheck[NUM_BEAD_DIST_CUV];
} T4AlgRecord_t;


typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	T4AlgRecord_t			t4AlgRecord;
	CommonFactorsLimits_t	algFactorsLimits;
	DilutionCheckFactors_t	dilutionCheckFactors;
} T4Config_t;


#if defined(__cplusplus)
}
#endif

#endif	// __T4DATA_H
