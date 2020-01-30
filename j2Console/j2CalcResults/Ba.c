// ********************************************************************
//
// FILENAME:  
// 
// 		Ba.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		BA calculation algorithm derived using BA_1 and BA_2 result
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
//		CalcBa() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		BA calculation algorithm derived from BA_1 & BA_2 results
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

void	CalcBa(void)
{
	// process standard 2 cuvette algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(BA_1, BA_2, BA);


#ifndef UNDER_CE
	printf("BA invoked\n");
#endif
}
