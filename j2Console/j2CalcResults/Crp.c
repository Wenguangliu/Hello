// ********************************************************************
//
// FILENAME:  
// 
// 		Crp.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CRP calculation algorithm derived using CRP_1 and CRP_2 result
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
//		CalcCrp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CRP calculation algorithm derived from CRP_1 & CRP_2 results
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

void	CalcCrp(void)
{
	// process standard 2 cuvette algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(CRP_1, CRP_2, CRP);


#ifndef UNDER_CE
	printf("CRP invoked\n");
#endif
}
