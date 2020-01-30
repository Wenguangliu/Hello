#ifndef __ROCDATA_H
#define __ROCDATA_H


#include "CommonDefines.h"


#define NUM_ROTOR_FLAGS_LIMITS		8

#define NUM_ENDOGENOUS_ZONES		3
#define MAX_ENDOGENOUS_ZONE_IDX		2
#define MID_ENDOGENOUS_ZONE_IDX		1
#define MIN_ENDOGENOUS_ZONE_IDX		0

#define ENDOGENOUS_LT				1
#define ENDOGENOUS_GT				2

#define	MAX_CORRECTION_FACTORS		8

#define	NUM_BARCODE_FACTORS			16
#define NUM_ALG_FLAGS				16

#define NUM_SB_MATRIX				4


typedef struct
{
	float				hemolyzed340InvertedRatioMatrix[NUM_SB_MATRIX];
	float				lipemic340InvertedRatioMatrix[NUM_SB_MATRIX];
	float				icteric340InvertedRatioMatrix[NUM_SB_MATRIX];
	float				hemolyzed405InvertedRatioMatrix[NUM_SB_MATRIX];
	float				lipemic405InvertedRatioMatrix[NUM_SB_MATRIX];
	float				icteric405InvertedRatioMatrix[NUM_SB_MATRIX];
	float				hemolyzed467InvertedRatioMatrix[NUM_SB_MATRIX];
	float				lipemic467InvertedRatioMatrix[NUM_SB_MATRIX];
	float				icteric467InvertedRatioMatrix[NUM_SB_MATRIX];
	float				hemolyzedSlope[NUM_SB_MATRIX];
	float				hemolyzedIntercept[NUM_SB_MATRIX];
	float				lipemicSlope[NUM_SB_MATRIX];
	float				lipemicIntercept[NUM_SB_MATRIX];
	float				ictericSlope[NUM_SB_MATRIX];
	float				ictericIntercept[NUM_SB_MATRIX];

	unsigned 			rotorBeadMissingCheckLimit;					
	unsigned 			rotorDistributionCheckLimit;

	unsigned			rotorDilutionCheckEnabled;
	float				rotorShortSampleProbThreshold;
	float				rotorLongSampleProbThreshold;

	unsigned			rotorFlags[NUM_ROTOR_FLAGS_LIMITS];
	float				rotorFactorsLimits[NUM_ROTOR_FLAGS_LIMITS];
	float				rotorCuvettePathLengths[NUM_CUVETTES];
} RotorCalcRecord_t;

typedef struct
{
	unsigned char	index;
	float			scale;
	float			offset;
} BarcodeFactor_t;

typedef struct
{
	unsigned 			analyteSuppressionMask;
	float				hemolyzedLimit[NUM_ENDOGENOUS_ZONES];
	unsigned			hemolyzedDirection;										// DLR - for resultPrintFlags '<' or '>' (no rotor ROC support yet)
	float				lipemicLimit[NUM_ENDOGENOUS_ZONES];
	unsigned			lipemicDirection;										// DLR - for resultPrintFlags '<' or '>' (no rotor ROC support yet)
	float				ictericLimit[NUM_ENDOGENOUS_ZONES];
	unsigned			ictericDirection;										// DLR - for resultPrintFlags '<' or '>' (no rotor ROC support yet)
	float				hemolyzedCorrectionFactors[MAX_CORRECTION_FACTORS];
	float				lipemicCorrectionFactors[MAX_CORRECTION_FACTORS];
	float				ictericCorrectionFactors[MAX_CORRECTION_FACTORS];
	float				lowSystemLimit;
	float				lowDynamicRange;
	float				highDynamicRange;
	float				highSystemLimit;
} CommonFactorsLimits_t;

typedef struct
{
	unsigned char			analyteType;
	char					versionNumber[ALG_VERSION_SIZE];
	unsigned char			calculationIndex;
	unsigned char			printOrder;
	unsigned char			analyteCuvetteNumber;
	unsigned char			analyteBlankCuvetteNumber;
	float					analyteDesiredPathLength;
	unsigned char			sampleTypeAnalyteBlank;
	unsigned char			sampleTypeOpticalBlank;
	BarcodeFactor_t			barcodeFactor[NUM_BARCODE_FACTORS];
	unsigned char			algorithmFlags[NUM_ALG_FLAGS];
	CommonFactorsLimits_t	rotorFactorsLimits;
} AnalyteCalcRecord_t;


typedef struct
{
	unsigned char		rotorNumber;
	unsigned char		rotorFormat;
	char				rotorProductCode;
	char				rotorPrinterName[31];
	char				rotorScreenNameLine1[26];
	char				rotorScreenNameLine2[26];
	unsigned			rotorScreenNameFontColor;
	unsigned			rotorScreenNameBackgroundColor;
	RotorCalcRecord_t	rotorCalculationRecord;
	AnalyteCalcRecord_t	analyteCalcRecord[MAX_ANALYTE_TYPES];
} RotorConfig_t;

#endif	// __ROCDATA_H
