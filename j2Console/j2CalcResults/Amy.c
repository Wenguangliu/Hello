// ********************************************************************
//
// FILENAME:  
// 
// 		Amy.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		AMY calculation algorithm
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
//		CalcAmy() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		AMY calculation algorithm 
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

void	CalcAmy(void)
{
	ProcessStdRateAlgorithm(AMY);


#ifndef UNDER_CE
	printf("AMY invoked\n");
#endif
}