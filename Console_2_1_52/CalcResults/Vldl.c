// ********************************************************************
//
// FILENAME:  
// 
// 		Vldl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		VLDL calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"

#include "Ldl.h"


// ********************************************************************
// FUNCTION:
//
//		CalcVldl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		VLDL calculation algorithm 
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

void	CalcVldl(void)
{
	unsigned long	vldlFlags;
	float			vldlResult;
	
	float			trigResult;

	unsigned char	hdlType;

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(VLDL, DERIVED_CALC_ALG);

	// init result & flags values

	vldlResult	= NOVAL_NEG_FLOAT;

	hdlType = LipidsResultsFlagsChecks();

	vldlFlags = rotorResults_g->rotorAnalyteResult[VLDL].analyteFlags;

	trigResult = rotorResults_g->rotorAnalyteResult[TRIG].analyteResult;

	// Do we have any VLDL errors? 

	if (!vldlFlags)
	{											
		// no errors so compute VLDL result recovery using TRIG

		vldlResult = trigResult / VLDL_TRIG_RESULT_DIVISOR;

		// check LDL result limits

		ResultLimitsCheck(VLDL, vldlResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &vldlFlags);
	} 

	// save LDL results

	StoreAnalyteResultsRecord(VLDL, vldlResult, vldlFlags, factorsLimitsPtr);


#ifndef __QNX__
	printf("VLDL invoked\n");
#endif
}
