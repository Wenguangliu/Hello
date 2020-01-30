// ********************************************************************
//
// FILENAME:  
// 
// 		Na2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		NA_2 calculation algorithm
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
//		CalcNa2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		NA_2 calculation algorithm 
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

void	CalcNa2(void)
{
	// process standard rate result & flags for 2nd cuvette analyteType	of 2 cuvette assay

	ProcessStdRateAlgorithm(NA_2);


#ifndef __QNX__
	printf("NA_2 invoked\n");
#endif
}