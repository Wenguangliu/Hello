// ********************************************************************
//
// FILENAME:  
// 
// 		Na.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		NA calculation algorithm derived using NA_1 and NA_2 result
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
//		CalcNa() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		NA calculation algorithm derived from NA_1 & NA_2 results
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

void	CalcNa(void)
{
	// process standard 2 cuvette algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(NA_1, NA_2, NA);


#ifndef __QNX__
	printf("NA invoked\n");
#endif
}
