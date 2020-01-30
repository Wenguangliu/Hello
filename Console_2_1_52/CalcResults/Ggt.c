// ********************************************************************
//
// FILENAME:  
// 
// 		Ggt.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		GGT calculation algorithm
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
//		CalcGgt() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		GGT calculation algorithm 
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

void	CalcGgt(void)
{
	ProcessStdRateAlgorithm(GGT);


#ifndef __QNX__
	printf("GGT invoked\n");
#endif
}
