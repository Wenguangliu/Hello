// ********************************************************************
//
// FILENAME:  
// 
// 		Ldh.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		LDH calculation algorithm
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
//		CalcLdh() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		LDH calculation algorithm 
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

void	CalcLdh(void)
{
	ProcessStdRateAlgorithm(LDH);


#ifndef __QNX__
	printf("LDH invoked\n");
#endif
}