// ********************************************************************
//
// FILENAME:  
// 
// 		Ibil.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		IBIL calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	DBIL_TBIL_CUTOFF			1.5
#define DBIL_TBIL_RESULT_ADJUST		0.05

#define	DBIL_TBIL_COMP_ERROR		0x00000080
#define	DBIL_IS_TBIL_FLAG			0x00000001


// ********************************************************************
// FUNCTION:
//
//		CalcIbil() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		IBIL calculation algorithm 
//
//		NO results or flags for IBIL are stored
//		(DBIL results & flags may be updated) 
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

void	CalcIbil(void)
{
	unsigned long	dbilFlags;
	unsigned long	tbilFlags;
	float			dbilResult;
	float			tbilResult;
	 
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// set up DBIL alg data & factors & limits for potential updated result check

	factorsLimitsPtr = SetupAnalyteAlgInfo(DBIL, ENDPOINT);


	// IMV - update for DBIL endpoint paragraph 8

	StoreAlgParagraphNumImv( DBIL, 8.0 );

	// end IMV


	// get analyte results & flags from DBIL & TBIL (must precede IBIL alg in calculation idx)

	dbilFlags = rotorResults_g->rotorAnalyteResult[DBIL].analyteFlags;
	tbilFlags = rotorResults_g->rotorAnalyteResult[TBIL].analyteFlags;

	dbilResult = rotorResults_g->rotorAnalyteResult[DBIL].analyteResult;
	tbilResult = rotorResults_g->rotorAnalyteResult[TBIL].analyteResult;


	// check results only if no errors in TBIL and DBIL
	  
	if ((tbilFlags == NO_ANALYTE_ERROR) && (dbilFlags == NO_ANALYTE_ERROR))
	{
		// check for not possible for the total (TBIL result) to be less than a component (DBIL result)

		if (tbilResult < dbilResult)
		{
			// adjust DBIL result & update flags if DBIL result is too large
			 			
			if (dbilResult < DBIL_TBIL_CUTOFF )
			{
				dbilResult  = tbilResult - DBIL_TBIL_RESULT_ADJUST;
				dbilFlags  |= DBIL_IS_TBIL_FLAG;

				// check updated DBIL result versus range limits (and first mask off previous range limit error sets)

				dbilFlags  &= (~ANALYTE_RESULT_LIMITS_ERR_MASK);

				ResultLimitsCheck(DBIL, dbilResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &dbilFlags);
			}
			else
			{
				// Use TBIL, supress DBIL 

				dbilFlags  |= DBIL_TBIL_COMP_ERROR;
			}

			// update DBIL results & flags for IBIL algorithm

			rotorResults_g->rotorAnalyteResult[DBIL].analyteResult = dbilResult;
			rotorResults_g->rotorAnalyteResult[DBIL].analyteFlags  = dbilFlags;
		}
	}


#ifndef __QNX__
	printf("IBIL invoked\n");
#endif
}
