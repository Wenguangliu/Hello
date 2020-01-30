// ********************************************************************
//
// FILENAME:  
// 
// 		Cre1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CRE_1 calculation algorithm	including general CRE cuvette calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "Statistics.h"


#define	CRE_PASS11_IDX			3
#define	CRE_PASS12_IDX			4

#define	CRE_SLOPE_NUM_POINTS	5

#define CRE_WET_TIME			5.23

#define	CRE_AVG_IDX				0

#define	CRE_SLOPE				0
#define	CRE_INTERCEPT			1
#define	CRE_CURVE				2
#define	CRE_CURVE_OFFSET		3


// ********************************************************************
// FUNCTION:
//
//		CalcCreCuv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		General CRE cuvette calculation algorithm 
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

void	CalcCreCuv( unsigned char analyteType )
{
	float				creResult;
	unsigned long		creError;

	unsigned			checkAnalyteBlank;

	float				creSlopeRate;
	float				creInterceptRate;
	float				creBlankedAvg;
	float				creCorrectedAvg;
	float				creResultSlope;
	float				creResultIntercept;
	float				creResultCurve;
	float				creResultCurveOffset;

	EndpointAlgRecord_t		*endpointAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	AvgAbs_t 	avgAbs[MAX_TO_AVG];

	// init endpoint result & flags

	checkAnalyteBlank = TRUE;

	creResult =  NOVAL_NEG_FLOAT;
	creError =  NO_ANALYTE_ERROR;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, ENDPOINT);
	
	endpointAlgPtr = &endpointConfig_g.endpointAlgRecord;		

	// process endpoint standard average absorbance algorithm for up to 2 wavelength pairs if enabled

	if (!ProcessStdEndpointAvgAbs(analyteType, checkAnalyteBlank, endpointAlgPtr, factorsLimitsPtr, avgAbs, &creError))
	{
		// get CRE slope & intercept from passes with blanked abs

		if (LinearRegression(avgAbs[CRE_AVG_IDX].flashTime, avgAbs[CRE_AVG_IDX].blankedAbs, CRE_SLOPE_NUM_POINTS, &creSlopeRate, &creInterceptRate))
		{
			creError |= DIVIDE_BY_ZERO;
		}
		else
		{


			// IMV - CRE paragraph 4

			StoreAlgParagraphNumImv( analyteType, 4.0 );

			StoreIntermediateValue( analyteType, IMV_CRE_RATE, IMV_DIRECT_SET_METHOD, creSlopeRate );

			// end IMV


			// calc CRE average blanked abs	for passes 11 & 12 only

			creBlankedAvg = (avgAbs[CRE_AVG_IDX].blankedAbs[CRE_PASS11_IDX] + avgAbs[CRE_AVG_IDX].blankedAbs[CRE_PASS12_IDX]) / 2.0;  

			// calc corrected average using slope
			
			creCorrectedAvg = creBlankedAvg - (CRE_WET_TIME * creSlopeRate);		

			// calc corrected average by BFD
			
			creCorrectedAvg = BfdCalAdjust(creCorrectedAvg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[CRE_AVG_IDX]);		

			// calc CRE result using rotor bar code factors

			creResultSlope       = GetBarcodeParameter(analyteType, CRE_SLOPE);
			creResultIntercept   = GetBarcodeParameter(analyteType, CRE_INTERCEPT);
			creResultCurve       = GetBarcodeParameter(analyteType, CRE_CURVE);
			creResultCurveOffset = GetBarcodeParameter(analyteType, CRE_CURVE_OFFSET);

			creResult = (creResultSlope * (creCorrectedAvg - (creResultCurve / (creCorrectedAvg + creResultCurveOffset)))) + creResultIntercept; 


			// IMV - CRE paragraph 5 / linear time correction stability rate parameter

			StoreAlgParagraphNumImv( analyteType, 5.0 );

			StoreIntermediateValue( analyteType, IMV_CRE_WET_TIME,    IMV_DIRECT_SET_METHOD, CRE_WET_TIME );
			StoreIntermediateValue( analyteType, IMV_CRE_BLANKED_AVG, IMV_DIRECT_SET_METHOD, creBlankedAvg );
			StoreIntermediateValue( analyteType, IMV_RESULT_NO_DECAY, IMV_DIRECT_SET_METHOD, creResult );

			// end IMV


			// perform linear time correction for final result as enabled

			if (endpointAlgPtr->endpointCalcRecord.linearTimeCorrectionEnabled)
			{
				creResult *= CalcLinearTimeCorrection(analyteType, endpointAlgPtr->endpointCalcRecord.linearTimeCorrectionStabilityRate);
			}


			// IMV - CRE paragraph 6

			StoreAlgParagraphNumImv( analyteType, 6.0 );

			// end IMV


			// check if RQC error as enabled

			if (endpointAlgPtr->rqcCheckEnabled)
			{
				creError |= RqcAbusedCheck();
			}
			

			// IMV - CRE paragraph 8 / starting abs / bfd corrected / barcode factors store

			StoreAlgParagraphNumImv( analyteType, 8.0 );

			StoreEndpointResultCalcParamsImv( analyteType );

			// end IMV


			// Check standard endpointResult for within system & dynamic range limits

			ResultLimitsCheck(analyteType, creResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &creError);
		}
	}


	// IMV - CRE paragraph 9

	StoreAlgParagraphNumImv( analyteType, 9.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, creResult, creError, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		CalcCre1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CRE_1 calculation algorithm 
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

void	CalcCre1(void)
{
	CalcCreCuv( CRE_1 );


#ifndef UNDER_CE
	printf("CRE_1 invoked\n");
#endif
}
