// ********************************************************************
//
// FILENAME:  
// 
// 		Ag.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		AG calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	AG_NA_ABORT			0x00000080			// AG suppressed due to ALB abort 
#define	AG_K_ABORT			0x00000040			// AG suppressed due to TP  abort 
#define	AG_TCO2_ABORT		0x00000020			// AG suppressed due to TP  abort 
#define	AG_CL_ABORT			0x00000010			// AG suppressed due to TP  abort 


// ********************************************************************
// FUNCTION:
//
//		CalcAg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		AG calculation algorithm 
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

void	CalcAg(void)
{
	unsigned		analyteType;
	float			naResult;	   
	float			kResult;	   
	float			tcO2Result;	   
	float			clResult;
		   
	unsigned long	agFlags;
	float			agResult;	   

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(AG, DERIVED_CALC_ALG);

	// init result & flags values

	agResult  = NOVAL_NEG_FLOAT;
	agFlags   = 0;

	// process NA type algs flags/result (typically NA for 2 cuvette alg)

	analyteType = NA;

	if (rotorResults_g->rotorAnalyteResult[NA_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for NA_1 algorithm this rotor

		analyteType = NA_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[NA_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for NA_2 algorithm this rotor

		analyteType = NA_2;
	}

	naResult = rotorResults_g->rotorAnalyteResult[analyteType].analyteResult;

	if (rotorResults_g->rotorAnalyteResult[analyteType].analyteFlags)
	{
		agFlags |= AG_NA_ABORT;
	}

	// process K flags/result 

	kResult = rotorResults_g->rotorAnalyteResult[K].analyteResult;

	if (rotorResults_g->rotorAnalyteResult[K].analyteFlags)
	{
		agFlags |= AG_K_ABORT;
	}

	// process TC02 flags/result 

	tcO2Result = rotorResults_g->rotorAnalyteResult[TCO2].analyteResult;

	if (rotorResults_g->rotorAnalyteResult[TCO2].analyteFlags)
	{
		agFlags |= AG_TCO2_ABORT;
	}

	// process CL type algs flags/result (typically CL for 2 cuvette alg)

	analyteType = CL;

	if (rotorResults_g->rotorAnalyteResult[CL_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for CL_1 algorithm this rotor

		analyteType = CL_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[CL_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for CL_2 algorithm this rotor

		analyteType = CL_2;
	}

	clResult = rotorResults_g->rotorAnalyteResult[analyteType].analyteResult;	

	if (rotorResults_g->rotorAnalyteResult[analyteType].analyteFlags)
	{
		agFlags |= AG_CL_ABORT;
	}
		
	// calc the AG result if no errors in NA, K, CL & TCO2 result components

	if (!agFlags)
	{
		agResult = (naResult + kResult) - (clResult + tcO2Result);

		// check AG result limits

		ResultLimitsCheck(AG, agResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &agFlags);
	}

	// save AG results

	StoreAnalyteResultsRecord(AG, agResult, agFlags, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("AG invoked\n");
#endif
}
