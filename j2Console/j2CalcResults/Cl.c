// ********************************************************************
//
// FILENAME:  
// 
// 		Cl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CL calculation algorithm derived using CL_1 and CL_2 result
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
//		CalcCl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CL calculation algorithm derived from CL_1 & CL_2 results
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

void	CalcCl(void)
{
	// process standard 2 cuvette algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(CL_1, CL_2, CL);


#ifndef UNDER_CE
	printf("CL invoked\n");
#endif
}
