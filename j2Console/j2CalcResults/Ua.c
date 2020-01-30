// ********************************************************************
//
// FILENAME:  
// 
// 		Ua.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		UA calculation algorithm
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
//		CalcUa() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		UA calculation algorithm 
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

void	CalcUa(void)
{
	// invoke standard endpoint processor for GLU with analyte blank verification set

	ProcessStdEndpointAlgorithm(UA, ANALYTE_BLANK_CHK_ON);


#ifndef UNDER_CE
	printf("UA invoked\n");
#endif
}
