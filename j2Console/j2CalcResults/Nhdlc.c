// ********************************************************************
//
// FILENAME:  
// 
// 		Nhdlc.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		nHDLc calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	NHDLC_HDL_ABORT				0x00000080			// NHDLC suppressed due to HDL abort 
#define	NHDLC_CHOL_ABORT			0x00000040			// NHDLC suppressed due to CHOL abort 
#define NHDLC_HDL_GT_CHOL_ABORT		0x00000001			// NHDLC suppressed due to HDL > CHOL


// ********************************************************************
// FUNCTION:
//
//		CalcNhdlc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		nHDLc calculation algorithm 
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

void	CalcNhdlc(void)
{
	unsigned long	cholMask;
	unsigned long	cholFlags;
	float			cholResult;

	unsigned		hdlAnalyteType;
	unsigned long	hdlMask;
	unsigned long	hdlFlags;
	float			hdlResult;

	unsigned long	nhdlcFlags;
	float			nhdlcResult;	   

	CommonFactorsLimits_t	*factorsLimitsPtr;


	// IMV - start of NHDLC algorithm to here for paragraph number & alg version

	StoreAlgParagraphNumImv( NHDLC, 1.0 );

	StoreAlgVersionNumImv( NHDLC );

	// end IMV


	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(NHDLC, DERIVED_CALC_ALG);

	// init result & flags values

	nhdlcResult	= NOVAL_NEG_FLOAT;
	nhdlcFlags   = 0;


	// get CHOL flags/result 

	cholFlags  = rotorResults_g->rotorAnalyteResult[CHOL].analyteFlags;
	cholResult = rotorResults_g->rotorAnalyteResult[CHOL].analyteResult;
	cholMask   = rotorResults_g->rotorAnalyteResult[CHOL].analyteSuppressionMask | ANALYTE_RESULT_LIMITS_ERR_MASK;

	// process HDL type algs flags/result (typically HDL for 2 cuvette alg)

	hdlAnalyteType = HDL;

	if (rotorResults_g->rotorAnalyteResult[HDL_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for HDL_1 algorithm this rotor

		hdlAnalyteType = HDL_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[HDL_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for HDL_2 algorithm this rotor

		hdlAnalyteType = HDL_2;
	}

	hdlResult = rotorResults_g->rotorAnalyteResult[hdlAnalyteType].analyteResult;
	hdlFlags  = rotorResults_g->rotorAnalyteResult[hdlAnalyteType].analyteFlags;
	hdlMask   = rotorResults_g->rotorAnalyteResult[hdlAnalyteType].analyteSuppressionMask | ANALYTE_RESULT_LIMITS_ERR_MASK;


	// next look for any CHOL or HDL errors and set NHDLC abort flag if so 

	if (cholFlags & cholMask)	   
	{
		nhdlcFlags |= NHDLC_CHOL_ABORT;
	}

	if (hdlFlags & hdlMask)
	{
		nhdlcFlags |= NHDLC_HDL_ABORT;
	}

	if (hdlResult > cholResult)
	{
		nhdlcFlags |= NHDLC_HDL_GT_CHOL_ABORT;
	}

	// if there are no errors in , calc NHDLC, otherwise process the errors 

	if (!nhdlcFlags) 
	{
		// do final NHDLC result calculation

		nhdlcResult	= cholResult - hdlResult;

		// check NHDLC result limits

		ResultLimitsCheck(NHDLC, nhdlcResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &nhdlcFlags);

	}

	// save NHDLC results

	StoreAnalyteResultsRecord(NHDLC, nhdlcResult, nhdlcFlags, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("NHDLC invoked\n");
#endif
}
