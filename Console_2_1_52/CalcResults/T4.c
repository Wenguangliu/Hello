// ********************************************************************
//
// FILENAME:  
// 
// 		T4.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		T4 calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <math.h>				// required standard C library 

#include "CommonAlgFuncs.h"
#include "Statistics.h"


// T4 sampletype definitions

#define T4_G6P1		56
#define T4_L1		57

// T4 barcode factor indices

#define	T4_G6P1_EXP_RATE		0
#define	T4_SLOPE_BASE			1
#define	T4_INTERCEPT_BASE		2
#define	T4_SLOPE_FINE			3
#define	T4_INTERCEPT_FINE		4
#define	T4_TOP_BASE				5
#define	T4_TOP_FINE				6
#define	T4_BOTTOM_BASE			7
#define	T4_BOTTOM_FINE			8


// T4 specific analyte flags
#define	SAMPLE_CHECK_LIMIT			1.02

#define	G6_RATE_DEVIATION			0x00000020	// T4_G6P1 deviation too large 
#define	T4_RATE_TOO_HIGH			0x00000010	// T4_L1 Rate too high 
#define	T4_RATE_TOO_LOW				0x00000008	// T4_L1 Rate too low 
#define	T4_INTERFERENCE				0x00000004	// multiple	interferences, suppress	


// ********************************************************************
// FUNCTION:
//
//		ProcessT4Checks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Do T4 checks for bead mix, bead missing, distribution & starting absorbance 
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

unsigned ProcessT4Checks(unsigned char sampleTypeOpticalBlank, T4AlgRecord_t *t4AlgPtr, CommonFactorsLimits_t *factorsLimitsPtr, unsigned long *t4Error)
{
/*	ignore bead missing and bead distribution checks for now by commenting out logic
	unsigned	i;
*/

	//	Check endogenous limits from sample blank results and T4 special hemolyzed & lipemic limits

	EndogenousLimitsCheck(T4, factorsLimitsPtr, t4Error);

	if ((rotorResults_g->rotorInformationResults.rotorHemolyzedIndex > t4AlgPtr->t4HemComboLimit) && (rotorResults_g->rotorInformationResults.rotorLipemicIndex > t4AlgPtr->t4LipComboLimit))
	{
		*t4Error |= T4_INTERFERENCE;
	}


	// IMV - T4 paragraph 2

	StoreAlgParagraphNumImv( T4, 2.0 );

	// end IMV


	//	Check bead mix of T4_L1 & T4_G6P1 sampleType

	if (BeadMixCheck(T4_L1,  sampleTypeOpticalBlank, t4AlgPtr->beadMixCheckPass, t4AlgPtr->beadMixCheckLimitT4L1,  TEST_BEAD_MIX_ERROR,  t4Error))
	{
		return	TRUE;		// got a calc type error (divide by zero, etc.)
	}

	if (BeadMixCheck(T4_G6P1, sampleTypeOpticalBlank, t4AlgPtr->beadMixCheckPass, t4AlgPtr->beadMixCheckLimitG6P1, BLANK_BEAD_MIX_ERROR, t4Error))
	{
		return	TRUE;		// got a calc type error (divide by zero, etc.)
	}


/*	ignore bead missing and bead distribution checks for now by commenting out logic

	for (i = 0; i < NUM_BEAD_DIST_CUV; i++)
	{
		//	Check for bead missing errors

		if (BeadMissingCheck(t4AlgPtr->t4BeadDistCheck[i].cuvetteNumber, t4AlgPtr->t4BeadDistCheck[i].beadMissingCheckLimit, t4Error))
		{
			return	TRUE;		// got a calc type error (divide by zero, etc.)
		}

		//	Check for distribution check error

		if (BeadDistributionCheck(t4AlgPtr->t4BeadDistCheck[i].cuvetteNumber, t4AlgPtr->t4BeadDistCheck[i].distributionCheckLimit, t4Error))
		{
			return	TRUE;		// got a calc type error (divide by zero, etc.)
		}
	}

*/


	// check starting absorbance for wavelength pair 340/850 of T4_L1 & T4_G6P1 sampleType

	if (StartingAbsorbanceCheck(T4_L1,   sampleTypeOpticalBlank, t4AlgPtr->startAbsCheckPass, WP_340_850, t4AlgPtr->startAbsLimit, START_ABS_BAD, t4Error))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	if (StartingAbsorbanceCheck(T4_G6P1, sampleTypeOpticalBlank, t4AlgPtr->startAbsCheckPass, WP_340_850, t4AlgPtr->startAbsLimit, START_ABS_BAD, t4Error))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}


	// IMV - store bead mix params & starting abs limit / T4 paragraph 3

	StoreIntermediateValue( T4, IMV_T4_HEM_COMBO_LIMIT, IMV_DIRECT_SET_METHOD, t4AlgPtr->t4HemComboLimit );
	StoreIntermediateValue( T4, IMV_T4_LIP_COMBO_LIMIT, IMV_DIRECT_SET_METHOD, t4AlgPtr->t4LipComboLimit );

	StoreBeadMixImv( T4 );
	StoreStartAbsImv( T4 );

	StoreAlgParagraphNumImv( T4, 3.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessT4L1Rate() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate T4_L1 rate for recovery and process error conditions 
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

unsigned ProcessT4L1Rate(unsigned char sampleTypeOpticalBlank, T4AlgRecord_t *t4AlgPtr, unsigned char analyteCuvetteNumber, float desiredPathLength, float *rateT4L1, unsigned long *t4Error)
{
	unsigned char	startPass;
	unsigned char	endPass;
	unsigned char	numPoints;
	float			intercept;
	float			stdErrorT4L1;
	float			falseRateT4L1;

	AvgAbs_t 		absData;
	AvgAbs_t 		*absPtr = &absData;


	// IMV - declare intermediate value storage for T4 rate

	float			uncorrectedT4L1Rate;

	// end IMV


	// init common params

	startPass = t4AlgPtr->t4CalcRecord.startPass;
	endPass   = t4AlgPtr->t4CalcRecord.endPass;
	numPoints = (endPass - startPass) + 1;

	// get T4_L1 abs & time points for number of passes at rate/noise wavelength (does not blank or use average, max or min)
	 
	if (CalcAverageAbsorbance(absPtr, T4_L1, NO_BLANK_CORRECTION, sampleTypeOpticalBlank, startPass, endPass, t4AlgPtr->t4CalcRecord.rateStdErrorWavelengthPair, desiredPathLength, analyteCuvetteNumber, NO_BLANK_CORRECTION, NO_BLANK_CORRECTION, t4Error))
	{
		return	TRUE;						// abort algorithm processing flag
	}


	// IMV - T4 paragraph 4

	StoreAlgParagraphNumImv( T4, 4.0 );

	// end IMV


	// compute slope used to calculate T4_L1 rate analyte result for multiple points 

	if (LinearRegression(absPtr->flashTime, absPtr->blankedAbs, numPoints, rateT4L1, &intercept))
	{
		*t4Error |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag
	}
		 

	// IMV - set uncorrectedT4L1Rate / store T4 paragraph 5

	uncorrectedT4L1Rate = *rateT4L1;

	StoreAlgParagraphNumImv( T4, 5.0 );

	// end IMV


	// compute T4_L1 standard error (noise) of computed slope for noise error checks

	if (StandardError(absPtr->flashTime, absPtr->blankedAbs, numPoints, *rateT4L1, intercept, &stdErrorT4L1))
	{
		*t4Error |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag
	}
		 
	// get T4_L1 abs & time points for number of passes at false rate wavelength (does not blank or use average, max or min)
	 
	if (CalcAverageAbsorbance(absPtr, T4_L1, NO_BLANK_CORRECTION, sampleTypeOpticalBlank, startPass, endPass, t4AlgPtr->t4CalcRecord.falseRateWavelengthPair, desiredPathLength, analyteCuvetteNumber, NO_BLANK_CORRECTION, NO_BLANK_CORRECTION, t4Error))
	{
		return	TRUE;						// abort algorithm processing flag
	}


	// IMV - T4 paragraph 6

	StoreAlgParagraphNumImv( T4, 6.0 );

	// end IMV


	// compute T4_L1 false rate slope 

	if (LinearRegression(absPtr->flashTime, absPtr->blankedAbs, numPoints, &falseRateT4L1, &intercept))
	{
		*t4Error |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag 
	}


	// IMV - T4 paragraph 7

	StoreAlgParagraphNumImv( T4, 7.0 );

	// end IMV


	// Perform temperature correction on the T4_L1 rate 

	*rateT4L1 *= CalcTemperatureCorrection(t4AlgPtr->t4CalcRecord.arrhTemperatureCorrectionFactorT4L1);

	// calc corrected T4_L1 rate by BFD
			
	*rateT4L1 = BfdCalAdjust(*rateT4L1, t4AlgPtr->t4CalcRecord.bfdCalibrationFactorIndex);		


	// IMV - T4 paragraph 9

	StoreAlgParagraphNumImv( T4, 9.0 );

	// end IMV


	// check for T4_L1 false rate error

	if (fabs(falseRateT4L1) > t4AlgPtr->t4CalcRecord.falseRateAbsoluteLimit)
	{
		*t4Error |= FALSE_RATE;
	}


	// IMV - T4 paragraph 10

	StoreAlgParagraphNumImv( T4, 10.0 );

	// end IMV


	// check for noise in T4_L1 rate calc

	if (stdErrorT4L1 > t4AlgPtr->t4CalcRecord.rateNoiseAbsoluteLimit) 
	{
		*t4Error |= RATE_NOT_LINEAR;
	}


	// IMV - T4 paragraph 11	  IMV_T4_ARRH_FACTOR

	StoreIntermediateValue( T4, IMV_T4_NOISE_LIMIT,      IMV_DIRECT_SET_METHOD, t4AlgPtr->t4CalcRecord.rateNoiseAbsoluteLimit );
	StoreIntermediateValue( T4, IMV_T4_ARRH_FACTOR,      IMV_DIRECT_SET_METHOD, t4AlgPtr->t4CalcRecord.arrhTemperatureCorrectionFactorT4L1 );
	StoreIntermediateValue( T4, IMV_T4_FALSE_RATE_LIMIT, IMV_DIRECT_SET_METHOD, t4AlgPtr->t4CalcRecord.falseRateAbsoluteLimit );

	StoreIntermediateValue( T4, IMV_AVG_SENSOR_TEMPERATURE, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.rotorAverageTemperature );

	StoreIntermediateValue( T4, IMV_T4_BFD_ARRH_ADJUST, IMV_DIRECT_SET_METHOD, *rateT4L1 / uncorrectedT4L1Rate );
	StoreIntermediateValue( T4, IMV_T4_T4L1_RATE,       IMV_DIRECT_SET_METHOD, uncorrectedT4L1Rate );
	StoreIntermediateValue( T4, IMV_T4_T4L1_RATE_CORR,  IMV_DIRECT_SET_METHOD, *rateT4L1 );
	StoreIntermediateValue( T4, IMV_T4_T4L1_STD_ERROR,  IMV_DIRECT_SET_METHOD, stdErrorT4L1 );
	StoreIntermediateValue( T4, IMV_T4_T4L1_FALSE_RATE, IMV_DIRECT_SET_METHOD, falseRateT4L1 );

	StoreAlgParagraphNumImv( T4, 11.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessG6P1Rate() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate T4_G6P1 rate and process error conditions 
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

unsigned ProcessG6P1Rate(unsigned char sampleTypeOpticalBlank, T4AlgRecord_t *t4AlgPtr, unsigned long *t4Error)
{
	unsigned char	numPoints;
	float			intercept;
	float			rateT4G6P1;
	float			expRateT4G6P1;
	float			deviationT4G6P1;

	AvgAbs_t 		absData;
	AvgAbs_t 		*absPtr = &absData;


	// IMV - declare intermediate value storage for G6P1 rate

	float			uncorrectedT4G6P1Rate;

	// end IMV


	// init common params

	numPoints = (t4AlgPtr->t4CalcRecord.endPass - t4AlgPtr->t4CalcRecord.startPass) + 1;

	// get T4_G6P1 abs & time points for number of passes at rate wavelength (does not blank or use average, max or min)
	 
	if (CalcAverageAbsorbance(absPtr, T4_G6P1, NO_BLANK_CORRECTION, sampleTypeOpticalBlank, t4AlgPtr->t4CalcRecord.startPass, t4AlgPtr->t4CalcRecord.endPass, t4AlgPtr->t4CalcRecord.rateStdErrorWavelengthPair, t4AlgPtr->t4CalcRecord.desiredPathLengthT4G6P1, t4AlgPtr->t4CalcRecord.cuvetteNumberT4G6P1, NO_BLANK_CORRECTION, NO_BLANK_CORRECTION, t4Error))
	{
		return	TRUE;						// abort algorithm processing flag
	}

	// compute rate for T4_G6P1 for checks

	if (LinearRegression(absPtr->flashTime, absPtr->blankedAbs, numPoints, &rateT4G6P1, &intercept))
	{
		*t4Error |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag
	}


	// IMV - set uncorrectedT4G6P1Rate / store T4 paragraph 12

	uncorrectedT4G6P1Rate = rateT4G6P1;

	StoreAlgParagraphNumImv( T4, 12.0 );

	// end IMV


	// Perform temperature correction on the T4_G6P1 rate 

	rateT4G6P1 *= CalcTemperatureCorrection(t4AlgPtr->t4CalcRecord.arrhTemperatureCorrectionFactorT4G6P1);

	// calc corrected T4_G6P1 rate by BFD
			
	rateT4G6P1 = BfdCalAdjust(rateT4G6P1, t4AlgPtr->t4CalcRecord.bfdCalibrationFactorIndex);		


	// IMV - T4 paragraph 13

	StoreAlgParagraphNumImv( T4, 13.0 );

	// end IMV


	// calculate deviation for T4_G6P1 to compare against max delta limit using barcode param

	expRateT4G6P1 = GetBarcodeParameter(T4, T4_G6P1_EXP_RATE);

	deviationT4G6P1 = (rateT4G6P1 / expRateT4G6P1) - 1.0;


	// IMV - T4 paragraph 14

	StoreAlgParagraphNumImv( T4, 14.0 );

	// end IMV


	if (fabs(deviationT4G6P1) > t4AlgPtr->t4CalcRecord.maxDeltaLimitT4G6P1)
	{
		*t4Error |= G6_RATE_DEVIATION;
	}


	// IMV - store G6P1 params & T4 paragraph 16  t4AlgPtr->t4CalcRecord.maxDeltaLimitT4G6P1

	StoreIntermediateValue( T4, IMV_T4_G6PDH_EXPT_RATE, IMV_DIRECT_SET_METHOD, expRateT4G6P1 );
	StoreIntermediateValue( T4, IMV_T4_G6_ARRH_FACTOR,  IMV_DIRECT_SET_METHOD, t4AlgPtr->t4CalcRecord.arrhTemperatureCorrectionFactorT4G6P1 );
	StoreIntermediateValue( T4, IMV_T4_G6_MAX_DELTA,    IMV_DIRECT_SET_METHOD, t4AlgPtr->t4CalcRecord.maxDeltaLimitT4G6P1 );

	StoreIntermediateValue( T4, IMV_T4_G6_BFD_ARRH_ADJUST, IMV_DIRECT_SET_METHOD, rateT4G6P1 / uncorrectedT4G6P1Rate );
	StoreIntermediateValue( T4, IMV_T4_G6P1_RATE,          IMV_DIRECT_SET_METHOD, uncorrectedT4G6P1Rate );
	StoreIntermediateValue( T4, IMV_T4_G6P1_RATE_CORR,     IMV_DIRECT_SET_METHOD, rateT4G6P1 );
	StoreIntermediateValue( T4, IMV_T4_G6P1_DEVIATION,     IMV_DIRECT_SET_METHOD, deviationT4G6P1 );

	StoreAlgParagraphNumImv( T4, 16.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		CalcT4Result() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate T4 final recover Result 
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

unsigned CalcT4Result(float rateT4L1, float *t4Result, unsigned long *t4Error)
{
	float		bcT4Top;
	float		bcT4Bottom;
	float		bcT4Slope;
	float		bcT4Intercept;
	float		numeratorT4L1;
	float		denominatorT4L1;
	float		transformT4L1;

	bcT4Top    = GetBarcodeParameter(T4, T4_TOP_BASE)    + GetBarcodeParameter(T4, T4_TOP_FINE);	
	bcT4Bottom = GetBarcodeParameter(T4, T4_BOTTOM_BASE) + GetBarcodeParameter(T4, T4_BOTTOM_FINE);	
																				   
	bcT4Slope     = GetBarcodeParameter(T4, T4_SLOPE_BASE)     + GetBarcodeParameter(T4, T4_SLOPE_FINE);	
	bcT4Intercept = GetBarcodeParameter(T4, T4_INTERCEPT_BASE) + GetBarcodeParameter(T4, T4_INTERCEPT_FINE);	

	numeratorT4L1   = bcT4Top  - rateT4L1;
	denominatorT4L1 = rateT4L1 - bcT4Bottom;
		 
	// check for low rate

	if (denominatorT4L1 <= 0.0)
	{
		*t4Error |= (T4_RATE_TOO_LOW | BELOW_SYSTEM_RANGE);
		rotorResults_g->rotorAnalyteResult[ T4 ].resultPrintFlags |= MASK_LOW_SYSTEM_LIMIT;
		return TRUE;
	}

	// check for high rate

	if (numeratorT4L1 <= 0.0)
	{
		*t4Error |= (T4_RATE_TOO_HIGH | ABOVE_SYSTEM_RANGE);
		rotorResults_g->rotorAnalyteResult[ T4 ].resultPrintFlags |= MASK_HIGH_SYSTEM_LIMIT;
		*t4Result = NOVAL_POS_FLOAT;
		return TRUE;
	}

	transformT4L1 = log(numeratorT4L1 / denominatorT4L1);

	*t4Result = -bcT4Slope * transformT4L1 + bcT4Intercept;


	// IMV - store result calc params / T4 paragraph 17

	StoreIntermediateValue( T4, IMV_T4_SLOPE,     IMV_DIRECT_SET_METHOD, bcT4Slope );
	StoreIntermediateValue( T4, IMV_T4_INTERCEPT, IMV_DIRECT_SET_METHOD, bcT4Intercept );
	StoreIntermediateValue( T4, IMV_T4_TOP,       IMV_DIRECT_SET_METHOD, bcT4Top );
	StoreIntermediateValue( T4, IMV_T4_BOTTOM,    IMV_DIRECT_SET_METHOD, bcT4Bottom );

	StoreIntermediateValue( T4, IMV_T4_T4L1_NUMERATOR,   IMV_DIRECT_SET_METHOD, numeratorT4L1 );
	StoreIntermediateValue( T4, IMV_T4_T4L1_DENOMINATOR, IMV_DIRECT_SET_METHOD, denominatorT4L1 );
	StoreIntermediateValue( T4, IMV_T4_T4L1_TRANSFORMED, IMV_DIRECT_SET_METHOD, transformT4L1 );

	StoreAlgParagraphNumImv( T4, 17.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		T4SampleCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Perform T4 sample validity check 
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

unsigned T4SampleCheck(unsigned char sampleTypeOpticalBlank, T4AlgRecord_t *t4AlgPtr, unsigned long *t4Error)
{
	unsigned	i;
	float		avgCheckAbs[4];
	float		totalT4OR1;
	float		totalT4OR2;
	float		sampleCheck;

	AvgAbs_t 	avgAbs;
	AvgAbs_t 	*absPtr = &avgAbs;

	// init avg params

	// get averages for check: 
	//   avgCheckAbs[0]	- T4_OR1 secondary
	//   avgCheckAbs[1]	- T4_OR1 tertiary
	//   avgCheckAbs[2]	- T4_OR2 secondary
	//   avgCheckAbs[3]	- T4_OR2 tertiary

	for (i = 0; i < NUM_SAMPLE_CHECK_AVG; i++)
	{
		if (CalcAverageAbsorbance(absPtr, t4AlgPtr->t4CalcRecord.t4SampleCheckAvg[i].sampleType, NO_BLANK_CORRECTION, sampleTypeOpticalBlank, t4AlgPtr->t4CalcRecord.startPass, t4AlgPtr->t4CalcRecord.endPass, t4AlgPtr->t4CalcRecord.t4SampleCheckAvg[i].wavelengthPair, t4AlgPtr->t4CalcRecord.t4SampleCheckAvg[i].desiredPathLength, t4AlgPtr->t4CalcRecord.t4SampleCheckAvg[i].cuvetteNumber, NO_BLANK_CORRECTION, NO_BLANK_CORRECTION, t4Error))
		{
			return	TRUE;						// abort algorithm processing flag
		}

		avgCheckAbs[i] = absPtr->avg;
	}

	totalT4OR2 = avgCheckAbs[3];	// cuvette 27
	totalT4OR1 = avgCheckAbs[1];	// cuvette 4

	if (totalT4OR1 == 0.0)
	{
		*t4Error |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag
	}

	sampleCheck = totalT4OR2 / totalT4OR1;

	if (sampleCheck < SAMPLE_CHECK_LIMIT)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_SAMPLE_TO_CUV);
		return	TRUE;						// abort algorithm processing flag
	} 


	// IMV - store sample check params / T4 paragraph 18

	StoreIntermediateValue( T4, IMV_T4_ABS_AVG_SEC_CUV_4,   IMV_DIRECT_SET_METHOD, avgCheckAbs[0] );
	StoreIntermediateValue( T4, IMV_T4_ABS_AVG_TERT_CUV_4,  IMV_DIRECT_SET_METHOD, avgCheckAbs[1] );
	StoreIntermediateValue( T4, IMV_T4_ABS_AVG_SEC_CUV_27,  IMV_DIRECT_SET_METHOD, avgCheckAbs[2] );
	StoreIntermediateValue( T4, IMV_T4_ABS_AVG_TERT_CUV_27, IMV_DIRECT_SET_METHOD, avgCheckAbs[3] );

	StoreIntermediateValue( T4, IMV_T4_CUV_4_TOTAL,  IMV_DIRECT_SET_METHOD, totalT4OR1 );
	StoreIntermediateValue( T4, IMV_T4_CUV_27_TOTAL, IMV_DIRECT_SET_METHOD, totalT4OR2 );

	StoreIntermediateValue( T4, IMV_T4_SAMPLE_CHECK,       IMV_DIRECT_SET_METHOD, sampleCheck );
	StoreIntermediateValue( T4, IMV_T4_SAMPLE_CHECK_LIMIT, IMV_DIRECT_SET_METHOD, SAMPLE_CHECK_LIMIT );

	StoreAlgParagraphNumImv( T4, 18.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		CalcT4() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		T4 calculation algorithm 
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

void	CalcT4(void)
{
	float			t4Result;
	unsigned long	t4Error;

	unsigned char	analyteType;
	unsigned char	sampleTypeOpticalBlank;			
	unsigned char	analyteCuvetteNumber;
	float			desiredPathLength;

	float			rateT4L1;

	CommonFactorsLimits_t	*factorsLimitsPtr;

	T4AlgRecord_t	*t4AlgPtr;

	// init rate result & flags

	t4Result = NOVAL_NEG_FLOAT;
	t4Error = NO_ANALYTE_ERROR;

	analyteType = T4;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, T4_CALC_ALG);

	t4AlgPtr = &t4Config_g.t4AlgRecord;		

	// set up common parameters from rotor & algorithm specific configuration data

	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	if (ProcessT4Checks(sampleTypeOpticalBlank, t4AlgPtr, factorsLimitsPtr, &t4Error)== FALSE)
	{
		if (ProcessT4L1Rate(sampleTypeOpticalBlank, t4AlgPtr, analyteCuvetteNumber, desiredPathLength, &rateT4L1, &t4Error) == FALSE)
		{
			if (ProcessG6P1Rate(sampleTypeOpticalBlank, t4AlgPtr, &t4Error) == FALSE)
			{
				if (CalcT4Result(rateT4L1, &t4Result, &t4Error) == FALSE)
				{
					if (T4SampleCheck(sampleTypeOpticalBlank, t4AlgPtr, &t4Error) == FALSE)
					{
						// Check standard result for within system & dynamic range limits

						ResultLimitsCheck(analyteType, t4Result, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &t4Error);
					}
				}
			} 
		} 
	}


	// IMV - T4 paragraph 19

	StoreAlgParagraphNumImv( T4, 19.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, t4Result, t4Error, factorsLimitsPtr);


	// Special Case to set RQC value = 100.0 only if T4 is on rotor and if RQC value == 0.0	(i.e. implies no RQC bead on rotor, since RQC init value = 0)

	if ( rotorResults_g->rotorAnalyteResult[RQC].analyteResult == 0.0 )
	{
		rotorResults_g->rotorAnalyteResult[RQC].analyteResult = 100.00;
	}


#ifndef __QNX__
	printf("T4 invoked\n");
#endif
}
