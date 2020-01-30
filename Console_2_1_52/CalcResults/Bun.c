// ********************************************************************
//
// FILENAME:  
// 
// 		Bun.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		BUN calculation algorithm
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
//		CalcBun() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		BUN calculation algorithm 
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

void	CalcBun(void)
{
	// process standard 2 cuvette/data algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(BUN_1, BUN_2, BUN);

#ifndef __QNX__
	printf("BUN invoked\n");
#endif
}
