// ********************************************************************
//
// FILENAME:  
// 
// 		Hdl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HDL calculation algorithm derived using HDL_1 and HDL_2 result
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
//		CalcHdl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		HDL calculation algorithm derived from HDL_1 & HDL_2 results
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

void	CalcHdl(void)
{
	// process standard 2 cuvette algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(HDL_1, HDL_2, HDL);


#ifndef __QNX__
	printf("HDL invoked\n");
#endif
}
