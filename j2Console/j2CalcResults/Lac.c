// ********************************************************************
//
// FILENAME:  
// 
// 		Lac.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		LAC calculation algorithm
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
//		CalcLac() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		LAC calculation algorithm 
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

void	CalcLac(void)
{
	ProcessStdRateAlgorithm(LAC);


#ifndef UNDER_CE
	printf("LAC invoked\n");
#endif
}
