// ********************************************************************
//
// FILENAME:  
// 
// 		Ldl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		LDL calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define TRIG_LIMIT							400.0

#define TRIG_STATUS_FLAG_MASK				0xfffffffc		// Mask status flags for rate in endpoint and start absorbance 
#define HDL_STATUS_FLAG_MASK				0xfffffffc		// Mask status flags cuvette used 
#define HDL_TRIG_ERROR_MASK					0x7ffffffc		// Mask TRIG status flags and Low Dynamic Range 

#define SUPPRESS_RESULT_FOR_CHOL_COMP		0x00000008		// Result was suppressed for CHOL Comparison 
#define SUPPRESS_RESULT_FOR_TRIG			0x00000004		// Result was suppressed for TRIG errors 
#define SUPPRESS_RESULT_FOR_HDL				0x00000002		// Result was suppressed for HDL errors 
#define SUPPRESS_RESULT_FOR_CHOL			0x00000001		// Result was suppressed for CHOL errors 


// ********************************************************************
// FUNCTION:
//
//		LipidsResultsFlagsChecks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Verify lipid analytes results & flags before computing LDL, TC/H or VLDL
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

unsigned char LipidsResultsFlagsChecks(void)
{
	unsigned char	hdlType;
	unsigned long	tchFlags;
	unsigned long	ldlFlags;
	unsigned long	vldlFlags;
	unsigned long	cholFlags;
	unsigned long	hdlFlags;
	unsigned long	trigFlags;

	float			cholResult;
	float			hdlResult;
	float			trigResult;

	// process HDL type algs flags/result (typically HDL for 2 cuvette alg)

	hdlType = HDL;

	if (rotorResults_g->rotorAnalyteResult[HDL_1].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for HDL_1 algorithm this rotor

		hdlType = HDL_1;
	}
	else if (rotorResults_g->rotorAnalyteResult[HDL_2].printOrder != NO_PRINT_ORDER_FLAG)
	{
		// only 1 cuvette processed for HDL_2 algorithm this rotor

		hdlType = HDL_2;
	}

	// set up compare analyte results & flags

	tchFlags   = rotorResults_g->rotorAnalyteResult[TCH].analyteFlags;
	ldlFlags   = rotorResults_g->rotorAnalyteResult[LDL].analyteFlags;
	vldlFlags  = rotorResults_g->rotorAnalyteResult[VLDL].analyteFlags;
	cholFlags  = rotorResults_g->rotorAnalyteResult[CHOL].analyteFlags;
	hdlFlags   = rotorResults_g->rotorAnalyteResult[hdlType].analyteFlags;
	trigFlags  = rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags;

	cholResult = rotorResults_g->rotorAnalyteResult[CHOL].analyteResult;
	hdlResult  = rotorResults_g->rotorAnalyteResult[hdlType].analyteResult;
	trigResult = rotorResults_g->rotorAnalyteResult[TRIG].analyteResult;

	// Do we have any TRIG errors? 

	if (trigFlags & TRIG_STATUS_FLAG_MASK)		// Supress the results depending on TRIG analyte flags
	{											
		vldlFlags |= SUPPRESS_RESULT_FOR_TRIG;
		ldlFlags  |= SUPPRESS_RESULT_FOR_TRIG;
	}


	if (trigFlags & HDL_TRIG_ERROR_MASK)		// If any TRIG error other than Low Dynamic Range then supress the HDL result
	{											
												   
		hdlFlags |= SUPPRESS_RESULT_FOR_TRIG;	
	}

	// Is TRIG result above TRIG limit? 

	if (trigResult > TRIG_LIMIT)				// Supress analyte flags for TRIG Limit 
	{										
		hdlFlags  |= LIPEMIC_LIMIT_ERROR;
		rotorResults_g->rotorAnalyteResult[hdlType].resultPrintFlags |= MASK_LIPEMIC_SUPPRESSED;

		vldlFlags |= LIPEMIC_LIMIT_ERROR;
		rotorResults_g->rotorAnalyteResult[VLDL].resultPrintFlags |= MASK_LIPEMIC_SUPPRESSED;

		ldlFlags  |= LIPEMIC_LIMIT_ERROR;
		rotorResults_g->rotorAnalyteResult[LDL].resultPrintFlags |= MASK_LIPEMIC_SUPPRESSED;
	}										

	// Do we have any CHOL errors				// Supress the results that depend on CHOL  

	if (cholFlags)
	{											
		tchFlags |= SUPPRESS_RESULT_FOR_CHOL;
		ldlFlags |= SUPPRESS_RESULT_FOR_CHOL;
	}

	// Do we have any HDL errors 

	if (hdlFlags & HDL_STATUS_FLAG_MASK)		// Supress the results that depend on HDL 
	{											
		tchFlags |= SUPPRESS_RESULT_FOR_HDL;
		ldlFlags |= SUPPRESS_RESULT_FOR_HDL;
	}

	// Check for TCH flags set to check CHOL versus HDL result 

	if(!tchFlags)
	{											
		if (cholResult < hdlResult)				// Supress all results that depend on CHOL & HDL
		{										
			cholFlags |= SUPPRESS_RESULT_FOR_CHOL_COMP;
			hdlFlags  |= SUPPRESS_RESULT_FOR_CHOL_COMP;
			tchFlags  |= SUPPRESS_RESULT_FOR_CHOL_COMP;
			ldlFlags  |= SUPPRESS_RESULT_FOR_CHOL_COMP;
		}
	}

	// restore analyte flags in rotor result record

	rotorResults_g->rotorAnalyteResult[TCH].analyteFlags     = tchFlags;
	rotorResults_g->rotorAnalyteResult[LDL].analyteFlags     = ldlFlags;
	rotorResults_g->rotorAnalyteResult[VLDL].analyteFlags    = vldlFlags;
	rotorResults_g->rotorAnalyteResult[CHOL].analyteFlags    = cholFlags;
	rotorResults_g->rotorAnalyteResult[hdlType].analyteFlags = hdlFlags;
	rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags    = trigFlags;

	return	hdlType;
}


// ********************************************************************
// FUNCTION:
//
//		CalcLdl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		LDL calculation algorithm 
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

void	CalcLdl(void)
{
	unsigned long	ldlFlags;
	float			ldlResult;
	
	float			cholResult;
	float			hdlResult;
	float			trigResult;

	unsigned char	hdlType;

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(LDL, DERIVED_CALC_ALG);

	// init result & flags values

	ldlResult	= NOVAL_NEG_FLOAT;

	hdlType = LipidsResultsFlagsChecks();

	ldlFlags = rotorResults_g->rotorAnalyteResult[LDL].analyteFlags;

	cholResult = rotorResults_g->rotorAnalyteResult[CHOL].analyteResult;
	hdlResult = rotorResults_g->rotorAnalyteResult[hdlType].analyteResult;
	trigResult = rotorResults_g->rotorAnalyteResult[TRIG].analyteResult;


	// Do we have any LDL errors? 
	
	if (!ldlFlags)
	{
		// no errors so compute ldh result recovery	using CHOL, HDL & TRIG
													
		ldlResult = cholResult - hdlResult - (trigResult / VLDL_TRIG_RESULT_DIVISOR);

		// check LDL result limits

		ResultLimitsCheck(LDL, ldlResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &ldlFlags);
	}

	// save LDL results

	StoreAnalyteResultsRecord(LDL, ldlResult, ldlFlags, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("LDL invoked\n");
#endif
}
