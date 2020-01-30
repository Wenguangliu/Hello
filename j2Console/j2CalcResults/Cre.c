// ********************************************************************
//
// FILENAME:  
// 
// 		Cre.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CRE calculation algorithm
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
//		CalcCre() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CRE calculation algorithm 
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

void	CalcCre(void)
{
	// process standard 2 cuvette/data algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(CRE_1, CRE_2, CRE);


#ifndef UNDER_CE
	printf("CRE invoked\n");
#endif
}
