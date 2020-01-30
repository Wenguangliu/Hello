// ********************************************************************
//
// FILENAME:  
// 
// 		Chol.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CHOL calculation algorithm
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
//		CalcChol() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CHOL calculation algorithm 
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

void	CalcChol(void)
{
	// invoke standard endpoint processor for CHOL with analyte blank verification set

	ProcessStdEndpointAlgorithm(CHOL, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("CHOL invoked\n");
#endif
}
