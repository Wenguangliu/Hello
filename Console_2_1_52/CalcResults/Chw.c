// ********************************************************************
//
// FILENAME:  
// 
// 		Chw.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CHW algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// ********************************************************************
// FUNCTION:
//
//		Chw() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CHW calculation algorithm 
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

void	CalcChw(void)
{
	// process standard POSITIVE / NEGATIVE type result & flags - with ratio / rate values 

	ProcessStdPosNegAlgorithm(CHW); 


	// suppress result as "~~~" if not canine species or not a control sample

	if ( commonAlgGlobals_g->controlType != TRUE )
	{
		if ( commonAlgGlobals_g->patientType != PATIENT_TYPE_DOG_FEMALE )
		{
			rotorResults_g->rotorAnalyteResult[CHW].analyteFlags |= CHW_NOT_CANINE_OR_CONTROL;
		}
	}


#ifndef __QNX__
	printf("CHW invoked\n");
#endif
}
