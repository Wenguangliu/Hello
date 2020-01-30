// ********************************************************************
//
// FILENAME:  
// 
// 		Bun2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		BUN_2 calculation algorithm
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
//		CalcBun2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		BUN_2 calculation algorithm 
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

void	CalcBun2(void)
{

	ProcessStdRateAlgorithm(BUN_2);

#ifndef __QNX__
	printf("BUN_2 invoked\n");
#endif
}
