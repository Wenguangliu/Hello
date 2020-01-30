// ********************************************************************
//
// FILENAME:  
// 
// 		Iqc.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		IQC calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "SharedFactory.h"
#include "SharedCal.h"
#include "Statistics.h"


// QC error	flags
 
#define	IQC_340_ERROR			0x8000
#define	IQC_DIV_BY_0			0x0080		// Divide by 0 in IQC calcs	 
#define	IQC_BAD_LOG_PARAM		0x0008		// IQC encountered bad log param error  
#define	IQC_FLASH_RANGE			0x0004		// IQC encountered flash range error  
#define	IQC_ABORTED_ROTOR		0x0002		// One of the IQC Level 1 checks Aborted the rotor  
#define	IQC_PRECISION_ERROR		0x0001		// IQC error occurred in %cv across	wavelengths	 


// misc defined

#define PASS_1					1


typedef struct
{
	float			*measured;
	float			*minLimit;
	float			*maxLimit;
	unsigned short	errorNumber;
} iqcLevel1Param_t;


// ********************************************************************
// FUNCTION:
//
//		SearchMinimumDacTrim() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Search for minimum dac trim used in calibration.cfg
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float SearchMinimumDacTrim(void)
{
	unsigned 		i;
	unsigned char	minimumDacTrim;
	unsigned char	dacTrim;

	minimumDacTrim = MAX_DAC_TRIM;
		
	for (i = 0; i < WL_WHITE; i++)
	{
		dacTrim = calibrationData_g->wavelengthDacTrims[i];

		if (dacTrim < minimumDacTrim)
		{
			minimumDacTrim = dacTrim;
		}
	}
	
	return ((float) minimumDacTrim);
}


// ********************************************************************
// FUNCTION:
//
//		SearchMaximumOffset() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Search for maximum offset used in calibration.cfg
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

float SearchMaximumOffset(void)
{
	unsigned 		i;
	unsigned short	maximumOffset;
	unsigned short	offset;

	maximumOffset = 0;
		
	for (i = 0; i < WL_WHITE; i++)
	{
		offset = rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[i];

		if (offset > maximumOffset)
		{
			maximumOffset = offset;
		}
	}
	
	return ((float) maximumOffset);
}

// ********************************************************************
// FUNCTION:
//
//		NormalizeIqcResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a normalized iqc result
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned NormalizeIqcResult(float measured, float minLimit, float maxLimit, unsigned short errorNumber, float printMin, float printMax, float *result)
{
	float	range;
	float	printRange;

	range = maxLimit - minLimit;

	printRange = printMax - printMin; 
	
	// limits are passed so check for divide by zero 

	if (range == 0.0)
	{
		*result = range;

		rotorResults_g->rotorInformationResults.rotorQcFlags |= IQC_DIV_BY_0;

		ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);

		return TRUE;
	}
	else
	{
		// Calculate the result using the general formula:
		//
		//		result =  Print Range * (measured - minimum allowed measured value)
		//      -----------------------------------------------------------------  + minimum print value
		//		(maximum allowed measured value - minimum allowed measured value)
		//

		*result = (printRange * (measured - minLimit) / range) + printMin;
	
		if ((*result < printMin) || (*result > printMax))
		{
			// 	Value is outside the acceptable range. Set flag to abort rotor for
			//	the correct error.  Do Not report an IQC value outside of allowable
			//	limit without aborting rotor! 
			//
		
			rotorResults_g->rotorInformationResults.rotorQcFlags |= IQC_ABORTED_ROTOR;

			ReportSystemError(NO_SYSTEM_FLAG, errorNumber);

			return TRUE;
		}
	}
	
	return FALSE;
} 		


// ********************************************************************
// FUNCTION:
//
//		ProcessIqcLevel1Checks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		IQC Level 1 calcs & checks   
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned ProcessIqcLevel1Checks(void)
{
	unsigned		i;

	float			iqcLevel1Min;
	float			iqcLevel1Max;

	float			zero;
	float			minDacTrimLimit;
	float			maxDacTrimLimit;
	float			minimumDacTrim;
	float			minLampIntensityLimit;
	float			maxLampIntensityLimit;
	float			maximumLampSaturation;
	float			maxOffsetSdLimit;
	float			maximumOffsetSd;
	float			maxOpticalCvLimit;
	float			maximumOpticalCv;
	float			minOffsetLimit;
	float			maxOffsetLimit;
	float			maximumOffset;
	float			maxFlashRangeLimit;
	float			maximumFlashRange;
	float			minimumLampIntensity;
	float			maxBfdAvgCurveLimit;
	float			bfdAvgCurveFactors;			

	const iqcLevel1Param_t	iqcLevel1Param[MAX_IQC_LEVEL1_CHECKS] = 
	{
		{ &minimumDacTrim,  		&minDacTrimLimit,		&maxDacTrimLimit, 		SERR_DAC_SETTING				  },	// 0: DAC trim check

		{ &maximumLampSaturation,	&minLampIntensityLimit,	&maxLampIntensityLimit,	SERR_FLASH_SATURATION             },	// 1: Lamp saturation check 

		{ &maximumOffsetSd,  		&zero, 					&maxOffsetSdLimit, 		SERR_OFFSET_SD					  },	// 2: Offset SD check

		{ &maximumOpticalCv,  		&zero,					&maxOpticalCvLimit, 	SERR_WAVE_CV					  },	// 3: Optical DAC CV check

		{ &maximumOffset,			&minOffsetLimit,		&maxOffsetLimit,		SERR_OFFSET_LEVEL				  },	// 4: Black Offsets check

		{ &maximumFlashRange,		&zero,					&maxFlashRangeLimit,	SERR_FLASH_RANGE				  },	// 5: Flash range check

		{ &minimumLampIntensity,	&minLampIntensityLimit,	&maxLampIntensityLimit,	SERR_LAMP_INTENSITY				  },	// 6. Minimum lamp intensity check

		{ &bfdAvgCurveFactors,		&zero,					&maxBfdAvgCurveLimit,	SERR_MISSING_FILTER_CURVE_FACTORS }		// 7: Missing BFD curve factor check
	};

	// get analyte ROC file parameters 

	iqcLevel1Min      = iqcConfig_g.iqcLevel1Min;
	iqcLevel1Max      = iqcConfig_g.iqcLevel1Max;


	// IMV - store IQC level limits / IQC paragraph 2

	StoreAlgParagraphNumImv( IQC, 2.0 );

	StoreIntermediateValue( IQC, IMV_IQC_LEVEL1_MIN, IMV_DIRECT_SET_METHOD, iqcLevel1Min );
	StoreIntermediateValue( IQC, IMV_IQC_LEVEL1_MAX, IMV_DIRECT_SET_METHOD, iqcLevel1Max );
	StoreIntermediateValue( IQC, IMV_IQC_LEVEL1_RNG, IMV_DIRECT_SET_METHOD, iqcLevel1Max - iqcLevel1Min );

	// end IMV


	// initialize parameters for IQC Level 1 constant table checks

	zero = (float) 0;

	minDacTrimLimit = (float) MIN_DAC_TRIM;
	maxDacTrimLimit = (float) MAX_DAC_TRIM;
	minimumDacTrim  = SearchMinimumDacTrim();

	minLampIntensityLimit = (float) MINIMUM_LAMP_INTENSITY;
	maxLampIntensityLimit = (float) LAMP_SATURATION_THRESHOLD;
	maximumLampSaturation = rotorResults_g->rotorInformationResults.rotorFlashLampSaturationMax;

	maxOffsetSdLimit = systemCheckConfig_g.offsetSdLimit;
	maximumOffsetSd  = rotorResults_g->rotorInformationResults.rotorOffsetSdMax;

	maxOpticalCvLimit = systemCheckConfig_g.opticalDacCvLimit;
	maximumOpticalCv  = rotorResults_g->rotorInformationResults.rotorOpticalDacCvMax;

	minOffsetLimit = (float) systemCheckConfig_g.offsetLevelMin;
	maxOffsetLimit = (float) systemCheckConfig_g.offsetLevelMax;
	maximumOffset  = SearchMaximumOffset();

	minimumLampIntensity  = rotorResults_g->rotorInformationResults.rotorFlashLampIntensityMin;

	maxFlashRangeLimit = FLASH_RANGE_MAX;
	maximumFlashRange  = rotorResults_g->rotorInformationResults.rotorFlashRangeMax;

	maxBfdAvgCurveLimit = (float) (UNINITIALIZED_FILTER_CAL - 1);
	bfdAvgCurveFactors  = factoryData_g->bfdAvgCurveFactors;

	// check IQC Level 1 parameters and set in rotor results values

	for (i = 0; i < MAX_IQC_LEVEL1_CHECKS; i++)
	{
		if (NormalizeIqcResult(*iqcLevel1Param[i].measured, *iqcLevel1Param[i].minLimit, *iqcLevel1Param[i].maxLimit, iqcLevel1Param[i].errorNumber, iqcLevel1Min, iqcLevel1Max, &rotorResults_g->rotorInformationResults.rotorIqcLevel1Results[i]))
		{


			// IMV - IQC level 1 value for last paragraph set due to error

			StoreIntermediateValue( IQC, IMV_IQC_LEVEL_1_START, IMV_INCR_SET_METHOD, rotorResults_g->rotorInformationResults.rotorIqcLevel1Results[i] );

			// end IMV


			return TRUE;
		}


		// IMV - IQC level 1 value / IQC paragraph store

		StoreIntermediateValue( IQC, IMV_IQC_LEVEL_1_START, IMV_INCR_SET_METHOD, rotorResults_g->rotorInformationResults.rotorIqcLevel1Results[i] );

		StoreAlgParagraphNumImv( IQC, (float) (i + 3.0) );

		// end IMV


	}


	// IMV - reset for next incr store

	InitNextIdxImv();

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessIqcLevel2Checks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		IQC Level 2 calcs & checks   
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

unsigned ProcessIqcLevel2Checks(void)
{
	unsigned		i;
	unsigned long	iqcError;
	unsigned short	iqcLowAbsMask;

	unsigned char	sampleTypeOpticalBlank;			
	unsigned char	sampleTypeIqcA;
	unsigned char	sampleTypeIqcB;
	unsigned char	iqcACuvetteNumber;
	unsigned char	iqcBCuvetteNumber;
	float			iqcAPathLength;
	float			iqcBPathLength;

	float			iqcACuvetteAbs[MAX_IQC_LEVEL2_CHECKS];
	float			iqcBCuvetteAbs[MAX_IQC_LEVEL2_CHECKS];
	float			iqcAbsRatio[MAX_IQC_LEVEL2_CHECKS];
	float			iqcAbsRatioAvg;
	float			iqcAbsRatioSd;
	float			iqcAbsRatioPrecision;

	float			iqcLevel2Min;
	float			iqcLevel2Max;
	float			iqcPrecisionMin;
	float			iqcPrecisionMax;
	float			iqcPrecisionLimit;
	float			iqcRatioSigma;
	float			iqcRatioLimit;
	float			iqcLowAbsorbanceLimit;

	const unsigned char iqcWavelengthPair[MAX_IQC_LEVEL2_CHECKS] = 
	{
		WP_340_850,
		WP_405_850,
		WP_467_850,
		WP_500_850,
		WP_515_850,
		WP_550_850,
		WP_600_850,
		WP_630_850
	};

	// get analyte ROC file parameters & init variables

	sampleTypeOpticalBlank = rotorConfig_g->analyteCalcRecord[IQC].sampleTypeOpticalBlank;	

	sampleTypeIqcA    = iqcConfig_g.sampleTypeIqcA;
	iqcACuvetteNumber = iqcConfig_g.iqcACuvetteNumber;
	iqcAPathLength	  = iqcConfig_g.iqcAPathLength;
	sampleTypeIqcB    = iqcConfig_g.sampleTypeIqcB;
	iqcBCuvetteNumber = iqcConfig_g.iqcBCuvetteNumber;
	iqcBPathLength	  = iqcConfig_g.iqcBPathLength;

	iqcLowAbsorbanceLimit = iqcConfig_g.iqcLowAbsorbanceLimit;
	iqcLevel2Min          = iqcConfig_g.iqcLevel2Min;
	iqcLevel2Max          = iqcConfig_g.iqcLevel2Max;
	iqcPrecisionMin       = iqcConfig_g.iqcPrecisionMin;
	iqcPrecisionMax       = iqcConfig_g.iqcPrecisionMax;
	iqcPrecisionLimit     = iqcConfig_g.iqcPrecisionLimit;
	iqcRatioSigma         = iqcConfig_g.iqcRatioSigma;
	iqcRatioLimit         = iqcPrecisionLimit * iqcRatioSigma;

	// calc IQC Level 2 absorbance related values & perform associated checks

	iqcError = 0;


	// IMV - store IQC level limits / IQC paragraph 11

	StoreIntermediateValue( IQC, IMV_IQC_ABS_LIMIT,       IMV_DIRECT_SET_METHOD, iqcLowAbsorbanceLimit );
	StoreIntermediateValue( IQC, IMV_IQC_PRECISION_LIMIT, IMV_DIRECT_SET_METHOD, iqcPrecisionLimit );
	StoreIntermediateValue( IQC, IMV_IQC_RATIO_SIGMA,     IMV_DIRECT_SET_METHOD, iqcRatioSigma );
	StoreIntermediateValue( IQC, IMV_IQC_RATIO_LIMIT,     IMV_DIRECT_SET_METHOD, iqcRatioLimit );
	StoreIntermediateValue( IQC, IMV_IQC_LEVEL2_MIN,      IMV_DIRECT_SET_METHOD, iqcLevel2Min );
	StoreIntermediateValue( IQC, IMV_IQC_LEVEL2_MAX,      IMV_DIRECT_SET_METHOD, iqcLevel2Max );
	StoreIntermediateValue( IQC, IMV_IQC_LEVEL2_RNG,      IMV_DIRECT_SET_METHOD, iqcLevel2Max - iqcLevel2Min );
	StoreIntermediateValue( IQC, IMV_IQC_PRECISION_MIN,   IMV_DIRECT_SET_METHOD, iqcPrecisionMin );
	StoreIntermediateValue( IQC, IMV_IQC_PRECISION_MAX,   IMV_DIRECT_SET_METHOD, iqcPrecisionMax );
	StoreIntermediateValue( IQC, IMV_IQC_PRECISION_RNG,   IMV_DIRECT_SET_METHOD, iqcPrecisionMax - iqcPrecisionMin );

	StoreAlgParagraphNumImv( IQC, 11.0 );

	// end IMV


	iqcLowAbsMask = IQC_340_ERROR;

	iqcAbsRatioAvg = 0.0;

	for (i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++)
	{
		// get IQC A absorbances

		iqcACuvetteAbs[i] = CalcSampleAbsorbance(sampleTypeIqcA, sampleTypeOpticalBlank, PASS_1, iqcWavelengthPair[i], iqcAPathLength, iqcACuvetteNumber, &iqcError);	

		// get IQC B absorbances

		iqcBCuvetteAbs[i] = CalcSampleAbsorbance(sampleTypeIqcB, sampleTypeOpticalBlank, PASS_1, iqcWavelengthPair[i], iqcBPathLength, iqcBCuvetteNumber, &iqcError);	

		// check if absorbance calc errors

		if (ProcessSystemCalcErrors(iqcError))
		{
			return TRUE;
		}


		// IMV - IQC paragraph 12

		StoreAlgParagraphNumImv( IQC, 12.0 );

		// end IMV


		// check for absorbance low	and set IQC flag for wavelength tested if error

		if ((iqcACuvetteAbs[i] < iqcLowAbsorbanceLimit) || (iqcBCuvetteAbs[i] < iqcLowAbsorbanceLimit))
		{
			rotorResults_g->rotorInformationResults.rotorQcFlags |= iqcLowAbsMask;

			ReportSystemError(NO_SYSTEM_FLAG, SERR_IQC_BEAD);

			return TRUE;
		}


		// IMV - IQC paragraph 13

		StoreAlgParagraphNumImv( IQC, 13.0 );

		// end IMV


		iqcLowAbsMask >>= 1;

		// compute IQC ratio and accumulate ratios for ratio average calc

		if (iqcBCuvetteAbs[i] == 0.0)
		{
			rotorResults_g->rotorInformationResults.rotorQcFlags |= IQC_DIV_BY_0;

			ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);

			return TRUE;
		}

		iqcAbsRatio[i] = iqcACuvetteAbs[i] / iqcBCuvetteAbs[i];

		iqcAbsRatioAvg += iqcAbsRatio[i];
	}

	// complete IQC ratio average calc

	iqcAbsRatioAvg /= MAX_IQC_LEVEL2_CHECKS;


	// IMV - store IQC abs A, Abs B, abs ratio A/B, abs ratio avg params / IQC paragraph 14

	StoreIqcAbsRatioParamsImv( iqcACuvetteAbs, iqcBCuvetteAbs, iqcAbsRatio, iqcAbsRatioAvg );

	StoreAlgParagraphNumImv( IQC, 14.0 );

	// end IMV


	// calc the SD of the ratio average for ratio precision checks

	iqcAbsRatioSd = StandardDeviation(iqcAbsRatio, MAX_IQC_LEVEL2_CHECKS);

	iqcAbsRatioPrecision = iqcAbsRatioSd / iqcAbsRatioAvg;


	// IMV - store absorbance SD & CV

	StoreIntermediateValue( IQC, IMV_IQC_RATIO_STD,       IMV_DIRECT_SET_METHOD, iqcAbsRatioSd );
	StoreIntermediateValue( IQC, IMV_IQC_RATIO_PRECISION, IMV_DIRECT_SET_METHOD, iqcAbsRatioPrecision );

	// end IMV


	if (iqcAbsRatioPrecision > iqcPrecisionLimit)
	{
		rotorResults_g->rotorInformationResults.rotorQcFlags |= IQC_PRECISION_ERROR;

		ReportSystemError(NO_SYSTEM_FLAG, SERR_IQC_RATIO_PRECISION);

		return TRUE;
	}


	// IMV - store IQC paragraph 15

	StoreAlgParagraphNumImv( IQC, 15.0 );

	// end IMV


	// check IQC absorbance ration precision and set rotor result value

	if (NormalizeIqcResult(iqcAbsRatioPrecision, 0.0, iqcPrecisionLimit, SERR_IQC_RATIO_PRECISION, iqcPrecisionMin, iqcPrecisionMax, &rotorResults_g->rotorInformationResults.rotorIqcRatioPrecision))
	{
		rotorResults_g->rotorInformationResults.rotorQcFlags |= IQC_PRECISION_ERROR;

		return TRUE;
	}

	// check IQC Level 2 parameters and set in rotor results values

	iqcLowAbsMask = IQC_340_ERROR;

	for (i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++)
	{
		if (NormalizeIqcResult((iqcAbsRatio[i] / iqcAbsRatioAvg), (1 - iqcRatioLimit), (1 + iqcRatioLimit), SERR_IQC_RATIO, iqcLevel2Min, iqcLevel2Max, &rotorResults_g->rotorInformationResults.rotorIqcLevel2Results[i]))
		{


			// IMV - IQC level 1 value store due to error

			StoreIntermediateValue( IQC, IMV_IQC_LEVEL_2_START, IMV_INCR_SET_METHOD, rotorResults_g->rotorInformationResults.rotorIqcLevel2Results[i] );

			// end IMV


			rotorResults_g->rotorInformationResults.rotorQcFlags |= iqcLowAbsMask;

			return TRUE;
		}


		// IMV - IQC level 2 value OK store

		StoreIntermediateValue( IQC, IMV_IQC_LEVEL_2_START, IMV_INCR_SET_METHOD, rotorResults_g->rotorInformationResults.rotorIqcLevel2Results[i] );

		// end IMV


		iqcLowAbsMask >>= 1;
	}


	// IMV - store IQC paragraph 16

	StoreAlgParagraphNumImv( IQC, 16.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		CalcIqc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		IQC calculation algorithm 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

void	CalcIqc(void)
{
	// get IQC analyte ROC data

	LoadAnalyteAlgData(IQC);

	// Perform IQC Level 1 checks

	if (ProcessIqcLevel1Checks())
	{
		return;
	}

	// Perform IQC Level 2 checks only for Piccolo type rotor

	if (rotorConfig_g->rotorProductCode == HUMAN_ROTOR)
	{
		if (ProcessIqcLevel2Checks())
		{
			return;
		}
	}


#ifndef UNDER_CE
	printf("IQC invoked\n");
#endif
}
