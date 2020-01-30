// ********************************************************************
//
// FILENAME:  
// 
// 		Glob.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		GLOB calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	GLOB_ALB_ABORT			0x00000080			// GLOB	suppressed due to ALB abort 
#define	GLOB_TP_ABORT			0x00000040			// GLOB	suppressed due to TP  abort 

#define	ALB_BCG_ABORT_MASK		(~ALB_BCG_METHOD)	// mask OFF ALB_BCG alg   flag 
#define	ALB_BCP_ABORT_MASK		0xffffffdf			// mask OFF ALB_BCP serum flag? 
#define	ALB_BCP_A_ABORT_MASK	0xffffffdd			// mask OFF ALB_BCP serum flag? & ALB_BCP_A alg flag

#define	ALB_TP_REL_ERROR		0x00000080			// ALB > TP, abort set in ALB analyteFlags	
#define	TP_ALB_REL_ERROR		0x00000080			// TP > ALB, abort set in TP  analyteFlags	


// ********************************************************************
// FUNCTION:
//
//		CalcGlob() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		GLOB calculation algorithm 
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

void	CalcGlob(void)
{
	unsigned		albType;
	unsigned long	albMask;
	unsigned long	albFlags;
	float			albResult;
	unsigned long	tpFlags;
	float			tpResult;

	unsigned long	globFlags;
	float			globResult;	   

	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up derived calc alg data & factors & limits for result store

	factorsLimitsPtr = SetupAnalyteAlgInfo(GLOB, DERIVED_CALC_ALG);

	// init result & flags values

	globResult	= NOVAL_NEG_FLOAT;
	globFlags   = 0;

	// determine type of ALB to get the correct analyte result abort mask 

	if (rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags & ALB_BCG_METHOD)
	{
		albMask = ALB_BCG_ABORT_MASK;
		albType = ALB_BCG;
	}
	else if (rotorResults_g->rotorAnalyteResult[ALB_BCP_A].analyteFlags & ALB_BCP_A_METHOD)
	{
		albMask = ALB_BCP_A_ABORT_MASK;
		albType = ALB_BCP_A;
	}
	else
	{
		albMask = ALB_BCP_ABORT_MASK;
		albType = ALB_BCP;
	}

	albFlags = rotorResults_g->rotorAnalyteResult[albType].analyteFlags;
	albResult = rotorResults_g->rotorAnalyteResult[albType].analyteResult;

	tpFlags  = rotorResults_g->rotorAnalyteResult[TP].analyteFlags;
	tpResult  = rotorResults_g->rotorAnalyteResult[TP].analyteResult;

	// next look for any ALB or TP errors and set GLOB abort flag if so 

	if (albFlags & albMask)	   
	{
		globFlags |= GLOB_ALB_ABORT;
	}

	if (tpFlags)
	{
		globFlags |= GLOB_TP_ABORT;
	}

	// if there are no errors in ALB or TP, calc GLOB if TP result > ALB result, otherwise process the errors 

	if ((!globFlags) && (tpResult > albResult))
	{
		// do final GLOB result calculation

		globResult	= tpResult - albResult;

		// check GLOB result limits

		ResultLimitsCheck(GLOB, globResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &globFlags);

	}
	else	
	{
		// got ALB error or TP error or TP <= ALB

		if(!globFlags)			 	
		{
			// no errors in ALB or TP, so inconsistent result for TP result <= ALB result

			tpFlags  |= TP_ALB_REL_ERROR;	// suppress TP 
			albFlags |= ALB_TP_REL_ERROR;	// suppress ALB 

			// update ALB & TP flags in results from GLOB

			rotorResults_g->rotorAnalyteResult[albType].analyteFlags = albFlags;
			rotorResults_g->rotorAnalyteResult[TP].analyteFlags = tpFlags;
		}
	} 

	// save GLOB results

	StoreAnalyteResultsRecord(GLOB, globResult, globFlags, factorsLimitsPtr);


#ifndef __QNX__
	printf("GLOB invoked\n");
#endif
}
