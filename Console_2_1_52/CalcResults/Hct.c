// ********************************************************************
//
// FILENAME:  
// 
// 		Hct.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HCT calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define HCT_HB_LOW_LIMIT			11.0
#define HCT_HB_HIGH_LIMIT			18.0

#define HCT_HB_SUPPRESSED			0x00000080			// HCT suppressed due to HB suppressed
#define	HCT_HB_OUT_OF_RANGE			0x00000040			// HCT suppressed due to HB out of range 


// ********************************************************************
// FUNCTION:
//
//		CalcHct() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		HCT calculation algorithm 
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

void	CalcHct(void)
{

	unsigned long	hbFlags;
	unsigned long	hbMask;
	float			hbResult;
	
	unsigned long	hctFlags;
	float			hctResult;	   
		   

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(HCT, DERIVED_CALC_ALG);

	// init HCT result & flags values

	hctResult	= NOVAL_NEG_FLOAT;
	hctFlags   	= 0;

	// get HB results 

	hbResult = rotorResults_g->rotorAnalyteResult[HB].analyteResult;
	hbFlags  = rotorResults_g->rotorAnalyteResult[HB].analyteFlags;
	hbMask   = rotorResults_g->rotorAnalyteResult[HB].analyteSuppressionMask;

	// process HCT for HB  

	if ( ( hbFlags & hbMask ) != NO_ANALYTE_ERROR )
	{
		// suppress HCT for HB suppressed

		hctFlags |= HCT_HB_SUPPRESSED;


		// IMV - naResult not suppressed to here

		StoreAlgParagraphNumImv( HCT, 2.0 );

		// end IMV


	}
	else if ( ( hbResult < HCT_HB_LOW_LIMIT ) || ( hbResult >= HCT_HB_HIGH_LIMIT ) )
	{
		// suppress HCT for HB out of range

		hctFlags |= HCT_HB_OUT_OF_RANGE;


		// hb out of range but compute HCT result from HB result anyway - no results limits chk

		hctResult = hbResult / HCT_HB_RESULT_DIVISOR;


		// IMV - naResult not suppressed to here

		StoreAlgParagraphNumImv( HCT, 3.0 );

		// end IMV


	}
	else
	{
		// not suppressed & in range so compute HCT result from HB result

		hctResult = hbResult / HCT_HB_RESULT_DIVISOR;

		// check GLOB result limits

		ResultLimitsCheck(HCT, hctResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &hctFlags);


		// IMV - naResult not suppressed to here

		StoreAlgParagraphNumImv( HCT, 4.0 );

		// end IMV


	}

	// save HCT results

	StoreAnalyteResultsRecord(HCT, hctResult, hctFlags, factorsLimitsPtr);


	// IMV - HCT alg constants used

	StoreIntermediateValue( HCT, IMV_HCT_HB_LOW_LIMIT,      IMV_DIRECT_SET_METHOD, HCT_HB_LOW_LIMIT );
	StoreIntermediateValue( HCT, IMV_HCT_HB_HIGH_LIMIT,     IMV_DIRECT_SET_METHOD, HCT_HB_HIGH_LIMIT );
	StoreIntermediateValue( HCT, IMV_HCT_HB_RESULT_DIVISOR, IMV_DIRECT_SET_METHOD, HCT_HB_RESULT_DIVISOR );

	// end IMV


#ifndef __QNX__
	printf("HCT invoked\n");
#endif
}
