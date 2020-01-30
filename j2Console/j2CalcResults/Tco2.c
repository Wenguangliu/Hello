// ********************************************************************
//
// FILENAME:  
// 
// 		Tco2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		TCO2 calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <string.h>
#include <math.h>				// required standard C library 
#include "CommonAlgFuncs.h"


#define	TCO2_BAD_RATE					0x00000020
#define	TCO2_BAD_POINTS					0x00000010

#define	PASS1_IDX						0

#define DELTA_INCR_SLOPE_NOISE_TOGGLE	4
#define	TCO2_BAD_RATE_TOGGLE			6
#define TCO2_ABORT_CALC_POINTS_TOGGLE	3

#define	TCO2_A0							0
#define	TCO2_A1							1
#define	TCO2_A2							2

// ********************************************************************
// FUNCTION:
//
//		DeltaIncrSlopeIndexCorrection() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct index & start index using 2nd derivative delta incremental slope data  
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

unsigned char DeltaIncrSlopeIndexCorrection(unsigned char i, float *deltaIncrSlope, float deltaIncrSlopeMin, float deltaIncrSlopeMax, unsigned char lastDeltaIncrSlopeIdx, unsigned char *startIdx) 
{
	while (((deltaIncrSlope[i] <= deltaIncrSlopeMin) || (deltaIncrSlope[i] >= deltaIncrSlopeMax)) && (i < (lastDeltaIncrSlopeIdx-1)))		
	{
		i++;

		// i equals first good data point or last index
		// since i points to first good data point in 2nd derivative, increment start index to remove contribution from prior bad point

		*startIdx = i + 1;
	}

	// start index updated so scan for first index after start index out of range in 2nd derivative incremental slope data

	i = *startIdx;
	
	while ((deltaIncrSlope[i] > deltaIncrSlopeMin) && (deltaIncrSlope[i] < deltaIncrSlopeMax) && (i < lastDeltaIncrSlopeIdx))		
	{
		i++;
	}

	return	i;
}


// ********************************************************************
// FUNCTION:
//
//		NumPointsToCalcSearch() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Search for number of points and start and stop indices to use for calcs 
//		from slope rate (1st derivative) and slope incremental (2nd derivative) arrays  
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

unsigned char NumPointsToCalcSearch(float *deltaSlopeRate, float *deltaIncrSlope, RateCalcRecord_t *calcPtr, unsigned char lastDeltaSlopeRateIdx, unsigned char lastDeltaIncrSlopeIdx, unsigned char *startIdx, unsigned char *stopIdx)
{
	unsigned char	i;
	float			deltaSlopeRateMin;
	float			deltaSlopeRateMax;
	float			deltaIncrSlopeMin;	
	float			deltaIncrSlopeMax;
	
	deltaSlopeRateMin = calcPtr->deltaSumMultRecord.deltaSlopeRateMin;	
	deltaSlopeRateMax = calcPtr->deltaSumMultRecord.deltaSlopeRateMax;	
	deltaIncrSlopeMin = calcPtr->deltaSumMultRecord.deltaIncrSlopeMin;	
	deltaIncrSlopeMax = calcPtr->deltaSumMultRecord.deltaIncrSlopeMax;
	
	// scan for start index in delta slope 1st derivative range

	i = *startIdx;
	
	while (((deltaSlopeRate[i] <= deltaSlopeRateMin) || (deltaSlopeRate[i] >= deltaSlopeRateMax)) && (i < lastDeltaSlopeRateIdx))		
	{
		i++;
	}

	*startIdx = i;

	// scan for stop index in delta slope 1st derviative range

	i = *stopIdx;
	
	while (((deltaSlopeRate[i] <= deltaSlopeRateMin) || (deltaSlopeRate[i] >= deltaSlopeRateMax)) && (i > *startIdx))		
	{
		i--;
	}

	*stopIdx = i;

	// update start index by scanning for first within range in 2nd derivative incremental slope data
	
	i = DeltaIncrSlopeIndexCorrection(*startIdx, deltaIncrSlope, deltaIncrSlopeMin, deltaIncrSlopeMax, lastDeltaIncrSlopeIdx, startIdx); 

	// start found before noise toggle index, so update start index again using 2nd derivative data

	if (i < DELTA_INCR_SLOPE_NOISE_TOGGLE)
	{
		i = DeltaIncrSlopeIndexCorrection(i, deltaIncrSlope, deltaIncrSlopeMin, deltaIncrSlopeMax, lastDeltaIncrSlopeIdx, startIdx); 
	}

	// update the stop index based on 2nd dervative noise scanning

	if (i < *stopIdx)
	{
		*stopIdx = i;
	}

	// if at last point in 2nd derivative data, check for update of stop index for calc 

	if ((i == lastDeltaIncrSlopeIdx) && (deltaIncrSlope[i] > deltaIncrSlopeMin) && (deltaIncrSlope[i] < deltaIncrSlopeMax))
	{
		// update for last point in 1st derivative delta slope rate data for calcs
		 
		*stopIdx = i + 1;
	}

	// calc number of points for return

	return	(*stopIdx - *startIdx + 1);
}


// ********************************************************************
// FUNCTION:
//
//		Tco2ResultsCalc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Final TCO2 custom results/errors calculation equations/checks 
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

float Tco2ResultsCalc(unsigned char analyteType, unsigned char numPoints, unsigned char startIdx, unsigned char stopIdx, float *deltaSlopeRate, float *deltaIncrSlope, RateCalcRecord_t *calcPtr, unsigned char lastDeltaSlopeRateIdx, unsigned char lastDeltaIncrSlopeIdx, unsigned long *tco2Error)  
{
	unsigned char 	i;
	unsigned char	j;
	float			deltaIncrSlopeAvg;
	float			sumDeltaSlope;
	float			sumDeltaSlopeExpFactor;
	float			tco2ResultA0;
	float			tco2ResultA1;
	float			tco2ResultA2;


	// IMV - TCO2 paragraph 14 store

	StoreAlgParagraphNumImv( TCO2, 14.0 );

	// end IMV


	// flag for too few points for optimal calc

	if (numPoints < TCO2_BAD_RATE_TOGGLE)
	{
		*tco2Error |= TCO2_BAD_RATE;
	}


	// IMV - TCO2 paragraph 15 store

	StoreAlgParagraphNumImv( TCO2, 15.0 );

	// end IMV


	// flag and exit with no result if not enough points

	if (numPoints < TCO2_ABORT_CALC_POINTS_TOGGLE)
	{
		*tco2Error |= TCO2_BAD_POINTS;

		return NOVAL_NEG_FLOAT;
	}
	
	// enough points to calculate final TCO2 result	so calc avg for 2nd derivative correction factor
	
	deltaIncrSlopeAvg = 0.0;
	j = 0;
	
	for (i = startIdx; (i < stopIdx) && (i <= lastDeltaIncrSlopeIdx); i++)
	{
		deltaIncrSlopeAvg += deltaIncrSlope[i];
		j++;
	}
	
	if (j)		// no divide by 0 in delta incremental slope 2 derivative avg calc allowed
	{
		deltaIncrSlopeAvg /= ((float)(2 * j));
	}
	

	// IMV - TCO2 paragraph 16 store

	StoreIntermediateValue( TCO2, IMV_TCO2_AVG_DELTA_SLOPE_CHANGE, IMV_DIRECT_SET_METHOD, deltaIncrSlopeAvg );

	StoreAlgParagraphNumImv( TCO2, 16.0 );

	// end IMV


	// compute slope rate for final calc using 1st derivative delta slope rate data corrected by 2nd derivative avg
	
	sumDeltaSlope = 0.0;

	for (i = 0; (i <= lastDeltaSlopeRateIdx) && (i < MAX_PASSES); i++)
	{
		if (i < startIdx)
		{
			deltaSlopeRate[i] = deltaSlopeRate[startIdx] - ((float)(startIdx - i) * deltaIncrSlopeAvg);
		}

		if (i > stopIdx)
		{
			deltaSlopeRate[i] = deltaSlopeRate[stopIdx]  + ((float)(i - stopIdx)  * deltaIncrSlopeAvg);
		}


		// IMV - store TCO2 deltaSlopeRate[i] intermediate values

		StoreIntermediateValue( TCO2, IMV_RATE_SLOPE_DELTA_START, IMV_INCR_SET_METHOD, deltaSlopeRate[i] );

		// end IMV


		sumDeltaSlope += (calcPtr->deltaSumMultRecord.deltaSumMulitiplier[i] * deltaSlopeRate[i]);
	}


	// IMV - store sumDeltaSlope intermediate value

	StoreIntermediateValue( TCO2, IMV_TCO2_SUM_DELTA_SLOPE, IMV_DIRECT_SET_METHOD, sumDeltaSlope );

	// end IMV


	// flag if last delta slope too low	for calc
	
	if (deltaSlopeRate[lastDeltaSlopeRateIdx] >= 0.0)
	{
		*tco2Error |= RATE_TOO_LOW;
	}
	
	// get result calc bar code factors from ROC file

	tco2ResultA0 = GetBarcodeParameter(analyteType, TCO2_A0);
	tco2ResultA1 = GetBarcodeParameter(analyteType, TCO2_A1);
	tco2ResultA2 = GetBarcodeParameter(analyteType, TCO2_A2);


	// create sum of delta slopes calc factor as per e**(fabs(TCO2_A1 * sumDeltaSlope))
	
	sumDeltaSlopeExpFactor = exp(fabs(tco2ResultA1 * sumDeltaSlope));
	

	// IMV - store exp-value and e_to_the_x	intermediate values

	StoreBarcodeParamsImv( TCO2 );
	StoreIntermediateValue( TCO2, IMV_RATE_EXPONENT_VALUE, IMV_DIRECT_SET_METHOD, fabs(tco2ResultA1 * sumDeltaSlope) );
	StoreIntermediateValue( TCO2, IMV_RATE_E_TO_THE_X,     IMV_DIRECT_SET_METHOD, exp(fabs(tco2ResultA1 * sumDeltaSlope)) );

	// end IMV


	// calc corrected slope rate exponential factor by BFD

	sumDeltaSlopeExpFactor = BfdCalAdjust(sumDeltaSlopeExpFactor, calcPtr->bfdCalibrationFactorIndex);
			
	// return final TCO2 result

	return ((tco2ResultA0 * sumDeltaSlopeExpFactor) + tco2ResultA2); 		 
}			 


// ********************************************************************
// FUNCTION:
//
//		CalcTco2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		TCO2 calculation algorithm 
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

void	CalcTco2(void)
{
	float			tco2Result;
	unsigned long	tco2Error;

	unsigned char	analyteType;
	unsigned char	sampleTypeAnalyte;		
	unsigned char	sampleTypeOpticalBlank;
	unsigned char	analyteCuvetteNumber;
	float			desiredPathLength;

	unsigned char	i;
	unsigned char	numPoints;
	unsigned char	startIdx;
	unsigned char	stopIdx;
	unsigned char	lastDeltaSlopeRateIdx;
	unsigned char	lastDeltaIncrSlopeIdx;
	float			deltaSlopeRate[MAX_PASSES];
	float			deltaIncrSlope[MAX_PASSES];
	float			tco2SlopeRateStartAbs;

	RateAlgRecord_t			*rateAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	SlopeParams_t 			slopeParams;
	SlopeParams_t 			*slopeParamsPtr;

	SlopeAbsData_t			slopeAbsData;
	SlopeAbsData_t			*slopeAbsPtr;

	RateCalcRecord_t 		*calcPtr;

	slopeParamsPtr = &slopeParams;
	slopeAbsPtr = &slopeAbsData;

	// init rate result & flags

	analyteType = TCO2;

	tco2Result = NOVAL_NEG_FLOAT;
	tco2Error = NO_ANALYTE_ERROR;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, RATE);

	rateAlgPtr = &rateConfig_g.rateAlgRecord;		

	// set up common parameters from rotor & algorithm specific configuration data

	sampleTypeAnalyte         = rateAlgPtr->sampleTypeAnalyte;	
	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	// perform standard rate algorithm checks for TCO2

	if (!ProcessStdRateChecks(analyteType, sampleTypeAnalyte, sampleTypeOpticalBlank, analyteCuvetteNumber, rateAlgPtr, factorsLimitsPtr, &tco2Error))
	{

		// IMV - bead mix store / starting abs store / TCO2 paragraph 10 store

		StoreBeadMixImv(  TCO2 );
		StoreStartAbsImv( TCO2 );

		StoreAlgParagraphNumImv( TCO2, 10.0 );

		// end IMV


		// standard rate checks OK so get absorbance data used to calc TCO2 slope rate

		calcPtr = &rateAlgPtr->rateCalcRecord;
		slopeParamsPtr->numPoints = calcPtr->deltaSumMultRecord.slopeNumPoints;
		slopeParamsPtr->startPass = calcPtr->deltaSumMultRecord.slopeStartPass;

		if (!GetSlopeTimeAbsData(slopeParamsPtr, sampleTypeAnalyte, sampleTypeOpticalBlank, calcPtr, desiredPathLength, analyteCuvetteNumber, slopeAbsPtr, &tco2Error))
		{


			// IMV - TCO2 paragraph 11 store

			StoreAlgParagraphNumImv( TCO2, 11.0 );

			// end IMV


			// check if starting slope rate absorbance for calcs within range

			tco2SlopeRateStartAbs = slopeAbsPtr->rateStdErrorAbs[PASS1_IDX];

			if ((tco2SlopeRateStartAbs < calcPtr->deltaSumMultRecord.startAbsRangeLow) || (tco2SlopeRateStartAbs > calcPtr->deltaSumMultRecord.startAbsRangeHigh))
			{
				tco2Error |= START_ABS_BAD;
			}


			// IMV - primary abs high start abs & low start abs limits store / TCO2 paragraph 12 store

			StoreIntermediateValue( TCO2, IMV_TCO2_PRI_HI_START_ABS,  IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.startAbsRangeHigh );
			StoreIntermediateValue( TCO2, IMV_TCO2_PRI_LO_START_ABS,  IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.startAbsRangeLow );

			StoreAlgParagraphNumImv( TCO2, 12.0 );

			// end IMV


			lastDeltaSlopeRateIdx = slopeParamsPtr->numPoints - 2;
			lastDeltaIncrSlopeIdx = lastDeltaSlopeRateIdx - 1;	

			// calculate slope rate deltas from pass absorbances (1st derivative)
			 
			for (i = 0; (i <= lastDeltaSlopeRateIdx) && (i < MAX_PASSES); i++)
			{
				deltaSlopeRate[i] = (slopeAbsPtr->rateStdErrorAbs[i+1] - slopeAbsPtr->rateStdErrorAbs[i]) / (slopeAbsPtr->flashTime[i+1] - slopeAbsPtr->flashTime[i]);
			}

			// calculate incremental slope rate deltas from slope rate deltas (2nd derivative)

			for (i = 0; (i <= lastDeltaIncrSlopeIdx) && (i < MAX_PASSES); i++)
			{
				deltaIncrSlope[i] = (deltaSlopeRate[i+1] - deltaSlopeRate[i]) / (slopeAbsPtr->flashTime[i+1] - slopeAbsPtr->flashTime[i]);
			}


			// IMV - TCO2 paragraph 13 store

			StoreAlgParagraphNumImv( TCO2, 13.0 );

			// end IMV


			// next search for number of points to use in calc along with start index & stop index

			startIdx = 0;
			stopIdx  = lastDeltaSlopeRateIdx;

			numPoints = NumPointsToCalcSearch(deltaSlopeRate, deltaIncrSlope, calcPtr, lastDeltaSlopeRateIdx, lastDeltaIncrSlopeIdx, &startIdx, &stopIdx);

			// calculate final TCO2 result & flags

			tco2Result = Tco2ResultsCalc(analyteType, numPoints, startIdx, stopIdx, deltaSlopeRate, deltaIncrSlope, calcPtr, lastDeltaSlopeRateIdx, lastDeltaIncrSlopeIdx, &tco2Error);  
		}
			

		// IMV - TCO2 paragraph 18 store

		StoreIntermediateValue( TCO2, IMV_TCO2_DELTA_SLOPE_MAX,   IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.deltaSlopeRateMax );
		StoreIntermediateValue( TCO2, IMV_TCO2_DELTA_SLOPE_MIN,   IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.deltaSlopeRateMin );
		StoreIntermediateValue( TCO2, IMV_TCO2_DELTA_SLOPE_2_MAX, IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.deltaIncrSlopeMax );
		StoreIntermediateValue( TCO2, IMV_TCO2_DELTA_SLOPE_2_MIN, IMV_DIRECT_SET_METHOD, calcPtr->deltaSumMultRecord.deltaIncrSlopeMin );
		StoreIntermediateValue( TCO2, IMV_TCO2_START_INDEX,       IMV_DIRECT_SET_METHOD, (float) startIdx );
		StoreIntermediateValue( TCO2, IMV_TCO2_STOP_INDEX,        IMV_DIRECT_SET_METHOD, (float) stopIdx );
		StoreIntermediateValue( TCO2, IMV_TCO2_NUM_POINTS,        IMV_DIRECT_SET_METHOD, (float) numPoints );

		StoreAlgParagraphNumImv( TCO2, 18.0 );

		// end IMV


		// Check standard result for within system & dynamic range limits

		ResultLimitsCheck(analyteType, tco2Result, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &tco2Error);
	}


	// IMV - TCO2 paragraph 19 store

	StoreAlgParagraphNumImv( TCO2, 19.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, tco2Result, tco2Error, factorsLimitsPtr);

#ifndef UNDER_CE
	printf("TCO2 invoked\n");
#endif
}
