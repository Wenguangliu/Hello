// ********************************************************************
//
// FILENAME:  
// 
// 		Alt2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ALT_2 calculation algorithm
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
//		CalcAlt2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		ALT_2 calculation algorithm 
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

void	CalcAlt2(void)
{

	ProcessStdRateAlgorithm(ALT_2);

#ifndef __QNX__
	printf("ALT_2 invoked\n");
#endif
}