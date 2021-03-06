// ********************************************************************
//
// FILENAME:  
// 
// 		Ck2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CK_2 calculation algorithm
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
//		CalcCk2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CK_2 calculation algorithm 
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

void	CalcCk2(void)
{
	ProcessStdRateAlgorithm(CK_2);

#ifndef UNDER_CE
	printf("CK_2 invoked\n");
#endif

}
