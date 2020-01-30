// ********************************************************************
//
// FILENAME:  
// 
// 		Hdl1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HDL_1 calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// abs indices

#define	HDL_MAX_TO_AVG			3

#define PRIME_BLANKED			0
#define SEONDARY_BLANKED		1
#define DERIVED_AVG				2

#define RATE_IN_ENDPOINT_IDX	PRIME_BLANKED

// barcode parameter indices

#define	NUM_HDL_BC_FACTORS		7

#define HDL_SLOPE				0
#define HDL_INT					1
#define HDL_550_DECAY_A0		2
#define HDL_550_DECAY_A1		3
#define HDL_467_DECAY_A0		4
#define HDL_467_DECAY_A1		5
#define HDL_ICT_FACTOR			6

#define ROTOR_AGE_DIVISOR		60.0


// ********************************************************************
// FUNCTION:
//
//		CalcHdlCuv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculation algorithm for a HDL cuvette (for HDL_1 or used by HDL_2) 
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

void	CalcHdlCuv(unsigned char analyteType, unsigned checkAnalyteBlank)
{
	float				hdlResult;
	unsigned long		hdlError;

	unsigned			i;
	float				derivedPrime;
	float				derivedSecondary;
	float				derivedBlanked;
	float				primeCorrectedAvg;
	float				secondaryCorrectedAvg;
	float				primeTimeCorrectedSlope;
	float				primeTimeCorrectedInt;
	float				secondaryTimeCorrectedSlope;
	float				secondaryTimeCorrectedInt;
	float				primeTimeCorrectedAvg;
	float				secondaryTimeCorrectedAvg;
	float				derivedTimeCorrectedAvg;
	float 				bcFactor[NUM_HDL_BC_FACTORS];

	EndpointAlgRecord_t		*algPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	AvgAbs_t 	avgAbs[HDL_MAX_TO_AVG];

	// init endpoint result & flags for this HDL cuvette calculated

	hdlResult = NOVAL_NEG_FLOAT;
	hdlError  = NO_ANALYTE_ERROR;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, ENDPOINT);
	
	algPtr = &endpointConfig_g.endpointAlgRecord;		

	// process endpoint standard average absorbance algorithm for up to 2 wavelength pairs if enabled

	if (!ProcessStdEndpointAvgAbs(analyteType, checkAnalyteBlank, algPtr, factorsLimitsPtr, avgAbs, &hdlError))
	{
		// custom algorithm for HDL type cuvette (HDL_1 or HDL_2 sample type) recovery
		// get barcode factors

		for (i = 0; i < NUM_HDL_BC_FACTORS; i++)
		{
			bcFactor[i] = GetBarcodeParameter(analyteType, i);
		}

		// compute primary, secondary & derived average data for checks and recovery

		avgAbs[DERIVED_AVG].min = NOVAL_POS_FLOAT;
		avgAbs[DERIVED_AVG].max = NOVAL_NEG_FLOAT;
		
		for (i = algPtr->endpointAvgRecord.avgStartPass - 1; i < algPtr->endpointAvgRecord.avgEndPass; i++)
		{
			derivedPrime     = BfdCalAdjust(avgAbs[PRIME_BLANKED].blankedAbs[i],    algPtr->endpointCalcRecord.bfdCalibrationFactorIndex[PRIME_BLANKED]);
			derivedSecondary = BfdCalAdjust(avgAbs[SEONDARY_BLANKED].blankedAbs[i], algPtr->endpointCalcRecord.bfdCalibrationFactorIndex[SEONDARY_BLANKED]);
			derivedBlanked   = derivedPrime + (bcFactor[HDL_ICT_FACTOR] * derivedSecondary);

			if (derivedBlanked > avgAbs[DERIVED_AVG].max)
			{
				avgAbs[DERIVED_AVG].max = derivedBlanked;	
			}
			if (derivedBlanked < avgAbs[DERIVED_AVG].min)
			{
				avgAbs[DERIVED_AVG].min = derivedBlanked;	
			}
		}		


		// IMV - endpoint paragraph 6

		StoreAlgParagraphNumImv( analyteType, 6.0 );

		// end IMV


		primeCorrectedAvg     = BfdCalAdjust(avgAbs[PRIME_BLANKED].avg,    algPtr->endpointCalcRecord.bfdCalibrationFactorIndex[PRIME_BLANKED]);
		secondaryCorrectedAvg = BfdCalAdjust(avgAbs[SEONDARY_BLANKED].avg, algPtr->endpointCalcRecord.bfdCalibrationFactorIndex[SEONDARY_BLANKED]);

		avgAbs[DERIVED_AVG].avg = primeCorrectedAvg + (bcFactor[HDL_ICT_FACTOR] * secondaryCorrectedAvg);

		// do rate in endpoint check using derived corrected avg and limits from analyte ROC (use primary values)

		if (!EndpointRateErrorCheck(&avgAbs[DERIVED_AVG], algPtr->endpointAvgRecord.avgParams[RATE_IN_ENDPOINT_IDX].flatnessToggle, algPtr->endpointAvgRecord.avgParams[RATE_IN_ENDPOINT_IDX].flatnessRatioLimit, algPtr->endpointAvgRecord.avgParams[RATE_IN_ENDPOINT_IDX].flatnessAbsoluteLimit, algPtr->endpointAvgRecord.avgParams[RATE_IN_ENDPOINT_IDX].rateInEndpointErrorMask, &hdlError))
		{
			// calculate HDL cuvette recovery
			// get rotor ROC file parameters

			primeTimeCorrectedSlope     = bcFactor[HDL_550_DECAY_A1] * barcodeData_g->rotorAgeInDays;
			primeTimeCorrectedInt       = bcFactor[HDL_550_DECAY_A0] * CalcExponentialRotorAgeDecay(ROTOR_AGE_DIVISOR);
			secondaryTimeCorrectedSlope = bcFactor[HDL_467_DECAY_A1] * barcodeData_g->rotorAgeInDays;
			secondaryTimeCorrectedInt   = bcFactor[HDL_467_DECAY_A0] * CalcExponentialRotorAgeDecay(ROTOR_AGE_DIVISOR);

			primeTimeCorrectedAvg     = primeCorrectedAvg     + primeTimeCorrectedSlope     + primeTimeCorrectedInt;
			secondaryTimeCorrectedAvg = secondaryCorrectedAvg - secondaryTimeCorrectedSlope - secondaryTimeCorrectedInt;

			derivedTimeCorrectedAvg = primeTimeCorrectedAvg + (bcFactor[HDL_ICT_FACTOR] * secondaryTimeCorrectedAvg);

			hdlResult = (derivedTimeCorrectedAvg * bcFactor[HDL_SLOPE]) + bcFactor[HDL_INT];		
		

			// IMV - endpoint paragraph 7

			StoreAlgParagraphNumImv( analyteType, 7.0 );

			// end IMV


			ResultLimitsCheck(analyteType, hdlResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &hdlError);
		}
	}


	// IMV - store HDL cuvette intermediate value specific params / HDL cuvette paragraph 8

	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_BLANKED_AVG,   IMV_DIRECT_SET_METHOD, avgAbs[PRIME_BLANKED].avg + (bcFactor[HDL_ICT_FACTOR] * avgAbs[SEONDARY_BLANKED].avg) );
	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_BLANKED_MIN,   IMV_DIRECT_SET_METHOD, avgAbs[DERIVED_AVG].min );
	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_BLANKED_MAX,   IMV_DIRECT_SET_METHOD, avgAbs[DERIVED_AVG].max );
	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_CORRECTED_AVG, IMV_DIRECT_SET_METHOD, avgAbs[DERIVED_AVG].avg );

	EndpointRateErrorCheck(&avgAbs[PRIME_BLANKED], algPtr->endpointAvgRecord.avgParams[PRIME_BLANKED].flatnessToggle, algPtr->endpointAvgRecord.avgParams[PRIME_BLANKED].flatnessRatioLimit, algPtr->endpointAvgRecord.avgParams[PRIME_BLANKED].flatnessAbsoluteLimit, NO_ANALYTE_ERROR, &hdlError);
	StoreHdlStartAbsFlatnessParmsImv( analyteType, algPtr );

	StoreBfdCorrectedImv( analyteType );
	StoreBarcodeParamsImv( analyteType);
	StoreIntermediateValue( analyteType, IMV_HDL_PRIM_TIME_CORR_SLOPE, IMV_DIRECT_SET_METHOD, primeTimeCorrectedSlope );
	StoreIntermediateValue( analyteType, IMV_HDL_PRIM_TIME_CORR_INT,   IMV_DIRECT_SET_METHOD, primeTimeCorrectedInt );
	StoreIntermediateValue( analyteType, IMV_HDL_SEC_TIME_CORR_SLOPE,  IMV_DIRECT_SET_METHOD, secondaryTimeCorrectedSlope );
	StoreIntermediateValue( analyteType, IMV_HDL_SEC_TIME_CORR_INT,    IMV_DIRECT_SET_METHOD, secondaryTimeCorrectedInt );

	StoreIntermediateValue( analyteType, IMV_HDL_PRIM_TIME_CORR_AVG,    IMV_DIRECT_SET_METHOD, primeTimeCorrectedAvg );
	StoreIntermediateValue( analyteType, IMV_HDL_SEC_TIME_CORR_AVG,     IMV_DIRECT_SET_METHOD, secondaryTimeCorrectedAvg );
	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_TIME_CORR_AVG, IMV_DIRECT_SET_METHOD, derivedTimeCorrectedAvg );
	StoreIntermediateValue( analyteType, IMV_HDL_RESULT_NO_DECAY,       IMV_DIRECT_SET_METHOD, (avgAbs[DERIVED_AVG].avg * bcFactor[HDL_SLOPE]) + bcFactor[HDL_INT] );
	StoreIntermediateValue( analyteType, IMV_HDL_RESULT_DECAY,          IMV_DIRECT_SET_METHOD, hdlResult );

	StoreAlgParagraphNumImv( analyteType, 8.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, hdlResult, hdlError, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		CalcHdl1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		HDL_1 calculation algorithm 
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

void	CalcHdl1(void)
{
	// process the HDL_1 cuvette using custom endpoint algorithm & check analyte blank

	CalcHdlCuv(HDL_1, ANALYTE_BLANK_CHK_ON);


#ifndef __QNX__
	printf("HDL_1 invoked\n");
#endif
}
