// ********************************************************************
//
// FILENAME:  
// 
// 		Ba1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		BA_1 calculation algorithm
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
//		CalcBa1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		BA_1 calculation algorithm 
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

void	CalcBa1(void)
{
	// process standard rate result & flags for 1st cuvette analyteType	of 2 cuvette assay

	ProcessStdRateAlgorithm(BA_1);


#ifndef UNDER_CE
	printf("BA_1 invoked\n");
#endif
}
