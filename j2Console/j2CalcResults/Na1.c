// ********************************************************************
//
// FILENAME:  
// 
// 		Na1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		NA_1 calculation algorithm
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
//		CalcNa1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		NA_1 calculation algorithm 
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

void	CalcNa1(void)
{
	ProcessStdRateAlgorithm(NA_1);


#ifndef UNDER_CE
	printf("NA_1 invoked\n");
#endif
}
