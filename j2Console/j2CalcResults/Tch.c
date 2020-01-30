// ********************************************************************
//
// FILENAME:  
// 
// 		Tch.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		TCH calculation algorithm
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
//		CalcTch() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		TCH calculation algorithm 
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

void	CalcTch(void)
{
	unsigned long	tchFlags;
	float			tchResult;	   

	float			cholResult;
	float			hdlResult;

	unsigned char	hdlType;

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(TCH, DERIVED_CALC_ALG);

	// init result & flags values

	tchResult	= NOVAL_NEG_FLOAT;

	hdlType = LipidsResultsFlagsChecks();

	tchFlags = rotorResults_g->rotorAnalyteResult[TCH].analyteFlags;

	cholResult = rotorResults_g->rotorAnalyteResult[CHOL].analyteResult;
	hdlResult = rotorResults_g->rotorAnalyteResult[hdlType].analyteResult;

	// Check for TCH flags set to check CHOL versus HDL result for TC/H recovery calc OK
	if(!tchFlags)
	{											
		if (cholResult >= hdlResult)
		{									   
			// no divide by zero allowed

			if (hdlResult == 0.0)
			{
				tchFlags |= DIVIDE_BY_ZERO;
			}
			else
			{
				// calculate TC/H result

				tchResult = cholResult / hdlResult;

				// check TCH result limits

				ResultLimitsCheck(TCH, tchResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &tchFlags);
			}
		}
	}

	// save TCH results

	StoreAnalyteResultsRecord(TCH, tchResult, tchFlags, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("TC/H invoked\n");
#endif
}
