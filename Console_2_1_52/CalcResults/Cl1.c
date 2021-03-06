// ********************************************************************
//
// FILENAME:  
// 
// 		Cl1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CL_1 calculation algorithm
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
//		CalcCl1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CL_1 calculation algorithm 
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

void	CalcCl1(void)
{
	// process standard rate result & flags for 1st cuvette analyteType	of 2 cuvette assay

	ProcessStdRateAlgorithm(CL_1);

#ifndef __QNX__
	printf("CL_1 invoked\n");
#endif

}
