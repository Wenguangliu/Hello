// ********************************************************************
//
// FILENAME:  
// 
// 		Ck.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CK calculation algorithm
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
//		CalcCk() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CK calculation algorithm 
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

void	CalcCk(void)
{
	ProcessStdRateAlgorithm(CK);

#ifndef UNDER_CE
	printf("CK invoked\n");
#endif

}
