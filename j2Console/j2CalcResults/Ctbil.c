// ********************************************************************
//
// FILENAME:  
// 
// 		Ctbil.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CTBIL calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// ROC file barcode factor slope/intercept indices

#define CTBIL_SLOPE_IDX			0
#define CTBIL_INTERCEPT_IDX		1


// index to ROC file algorithmFlag use of CTBIL adjust barcode slope/intercept factors 
// to adjust HORSE/OTHER slope/intercept in CTBIL calc
// versus using TBIL slope/intercept directly in CTBIL calc

#define CTBIL_ADJUST_FLAG_IDX		0

#define CTBIL_PATIENT_TYPE_ADJUST	1	//	adjust HORSE/OTHER with CTBIL barcode slope/int adjust factors 		


// HORSE/OTHER slope/intercept to use if CTBIL barcode adjust factors are selected by ROC file

#define	CTBIL_HORSE_SLOPE				42.13
#define	CTBIL_HORSE_INT					0.15
#define	CTBIL_OTHER_SLOPE				57.6
#define	CTBIL_OTHER_INT					0.4


// ********************************************************************
// FUNCTION:
//
//		CalcCtbil() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CTBIL calculation algorithm 
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

void	CalcCtbil(void)
{
	unsigned long	ctbilFlags;
	float			ctbilResult;

	float			ctbilSlope;
	float			ctbilIntercept;
	
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(CTBIL, DERIVED_CALC_ALG);

	// init result & flags values

	ctbilResult	= NOVAL_NEG_FLOAT;
	ctbilFlags = NO_ANALYTE_ERROR;

	//	Check endogenous limits from sample blank results and T4 special hemolyzed & lipemic limits

	EndogenousLimitsCheck(CTBIL, factorsLimitsPtr, &ctbilFlags);

	// Get barcode slope & intercept parameters this rotor from ROC file for CTBIL result calc
	// TBIL barcode slope & intercept used if algorithmFlags[CTBIL_ADJUST_FLAG_IDX] is not CTBIL_PATIENT_TYPE_ADJUST

	ctbilSlope     = GetBarcodeParameter(CTBIL, CTBIL_SLOPE_IDX);
	ctbilIntercept = GetBarcodeParameter(CTBIL, CTBIL_INTERCEPT_IDX);

	if (rotorConfig_g->analyteCalcRecord[CTBIL].algorithmFlags[CTBIL_ADJUST_FLAG_IDX]== CTBIL_PATIENT_TYPE_ADJUST)
	{
		// CTBIL adjust barcode slope/intercept used if algorithmFlags[CTBIL_ADJUST_FLAG_IDX] is set to CTBIL_PATIENT_TYPE_ADJUST

		if (commonAlgGlobals_g->patientType == PATIENT_TYPE_HORSE)
		{
			// adjust slope/intercept for HORSE

			ctbilSlope     = CTBIL_HORSE_SLOPE * ctbilSlope;
			ctbilIntercept = CTBIL_HORSE_INT + ctbilIntercept;
		}
		else
		{
			// adjust slope/intercept for not HORSE

			ctbilSlope     = CTBIL_OTHER_SLOPE * ctbilSlope;
			ctbilIntercept = CTBIL_OTHER_INT + ctbilIntercept;
		}
	}

	// Calculate CTBIL result from sample blank icteric 467 nm intermediate results 

	ctbilResult = (rotorResults_g->rotorInformationResults.sampleBlankCalcs.sampleBlankIcteric467 * ctbilSlope) + ctbilIntercept;


	// IMV - rate paragraph 16

	StoreIntermediateValue( CTBIL, IMV_BARCODE_SLOPE_USED,     IMV_DIRECT_SET_METHOD, ctbilSlope );
	StoreIntermediateValue( CTBIL, IMV_BARCODE_INTERCEPT_USED, IMV_DIRECT_SET_METHOD, ctbilIntercept );

	StoreIntermediateValue( CTBIL, IMV_CTBIL_HORSE_SLOPE, IMV_DIRECT_SET_METHOD, CTBIL_HORSE_SLOPE );
	StoreIntermediateValue( CTBIL, IMV_CTBIL_HORSE_INT,   IMV_DIRECT_SET_METHOD, CTBIL_HORSE_INT );
	StoreIntermediateValue( CTBIL, IMV_CTBIL_OTHER_SLOPE, IMV_DIRECT_SET_METHOD, CTBIL_OTHER_SLOPE );
	StoreIntermediateValue( CTBIL, IMV_CTBIL_OTHER_INT,   IMV_DIRECT_SET_METHOD, CTBIL_OTHER_INT );

	StoreBarcodeParamsImv( CTBIL );

	StoreIntermediateValue( CTBIL, IMV_CTBIL_SB_467_ICT,  IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.sampleBlankCalcs.sampleBlankIcteric467 );

	// end IMV


	// check CTBIL result limits

	ResultLimitsCheck(CTBIL, ctbilResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &ctbilFlags);

	// save CTBIL results

	StoreAnalyteResultsRecord(CTBIL, ctbilResult, ctbilFlags, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("CTBIL invoked\n");
#endif
}
