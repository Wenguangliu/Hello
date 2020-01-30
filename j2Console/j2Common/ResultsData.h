#ifndef __RESULTSDATA_H_INCLUDED
#define __RESULTSDATA_H_INCLUDED

#include "CommonDefines.h"


#define MAX_ROTOR_ERROR_NUMBERS			20

#define	MAX_ABS_RECORDS					4000

#define	MAX_INTERMEDIATE_VALUES			64
 

typedef unsigned ResultPrintFlags_t;


typedef struct
{
	unsigned char		analyteType;
	char				versionNumber[ALG_VERSION_SIZE];
	unsigned char		printOrder;
	float				analyteResult;
	unsigned long		analyteFlags;
	unsigned long		analyteSuppressionMask;				
	ResultPrintFlags_t	resultPrintFlags;
	float				lowReferenceRangeLimit;
	float				highReferenceRangeLimit;
	float				lowDynamicRangeLimit;
	float				highDynamicRangeLimit;
	float				lowSystemRangeLimit;
	float				highSystemRangeLimit;
} RotorAnalyteResult_t;


typedef struct
{
	unsigned				rotorDilutionCheckError;
	unsigned				rotorDilutionCheckNumberSuppressed;
	unsigned				rotorDilutionCheckNumberChecked;
	float					rotorShortSampleProbability;
	float					rotorLongSampleProbability;
} DilutionCheckResults_t;

typedef struct
{
	float					sampleBlankHemolyzed405;
	float					sampleBlankLipemic340;
	float					sampleBlankIcteric467;
} SampleBlankCalcs_t;

typedef struct
{
	unsigned long			rotorSystemFlags;
	unsigned short			rotorErrorNumber[MAX_ROTOR_ERROR_NUMBERS];
	unsigned				rotorErrorNumberIndex;
	unsigned 				rotorBeadMissingCheckCount;
	unsigned 				rotorDistributionCheckCount;
	unsigned long			rotorBeadCheck1Flags;
	unsigned long			rotorBeadCheck2Flags;
	unsigned long			rotorDistributionCheckFlags;
	unsigned short			rotorDacTrimFlags;
	unsigned short			rotorOffsetErrorFlags;
	unsigned short			rotorOffsetSdErrorFlags;
	unsigned short			rotorWavelengthCvFlags;
	unsigned short			rotorQcFlags;
	unsigned short			rotorFlashLampSaturationMax;
	unsigned short			rotorFlashLampIntensityMin;
	float					rotorFlashRangeMax;
	float					rotorOffsetSdMax;
	float					rotorOpticalDacCvMax;
	float					rotorAverageTemperature;
	float					rotorHemolyzedIndex;
	float					rotorLipemicIndex;
	float					rotorIctericIndex;
	float					rotorRqcLowLimit;
	unsigned				rotorRqcAbused;
	float					rotorIqcRatioPrecision;
	float					rotorIqcLevel1Results[MAX_IQC_LEVEL1_CHECKS];
	float					rotorIqcLevel2Results[MAX_IQC_LEVEL2_CHECKS];
	unsigned				referenceRangeType;
	unsigned short			wavelengthAdcOffsets[NUM_WAVELENGTHS];
	DilutionCheckResults_t	dilutionCheckResults;
	SampleBlankCalcs_t		sampleBlankCalcs;
} RotorInformationResults_t;


typedef struct
{
	float			engineMinutesTime;
	unsigned char	sampleType;
	unsigned char	sampleTypeOpticalBlank;
	unsigned char 	samplePass;
	unsigned char 	wavelengthPair;
	float			desiredPathLength;
	unsigned char	cuvetteNumber;
	unsigned long	analyteError;
	float			sampleAbsorbance;
} SampleAbsRecord_t;

typedef struct
{
	unsigned long		numberAbsRecords;
	SampleAbsRecord_t	sampleAbsRecords[MAX_ABS_RECORDS];
} RotorAbsorbances_t;


typedef struct
{
	unsigned char				rotorNumber;
	unsigned char				rotorFormat;
	char						rotorProductCode;
	RotorInformationResults_t	rotorInformationResults;
	RotorAnalyteResult_t		rotorAnalyteResult[MAX_ANALYTE_TYPES];
	RotorAbsorbances_t			rotorAbsorbances;

	float						rotorIntermediateValues[MAX_ANALYTE_TYPES][MAX_INTERMEDIATE_VALUES];

	float						rotorCuvetteEmptyLimits[ NUM_CUVETTES ];
	float						rotorCuvetteBeadLimits[ NUM_CUVETTES ];
	float						rotorCuvetteDistributionLimits[ NUM_CUVETTES ];
} RotorResults_t;

#endif	// __RESULTSDATA_H_INCLUDED
