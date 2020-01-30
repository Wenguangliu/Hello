// ********************************************************************
//
// FILENAME:  
// 
// 		Alt.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ALT calculation algorithm
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
//		CalcAlt() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		ALT calculation algorithm 
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

void	CalcAlt(void)
{
	// process standard 2 cuvette/data algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(ALT_1, ALT_2, ALT);


#ifndef __QNX__
	printf("ALT invoked\n");
#endif
}
