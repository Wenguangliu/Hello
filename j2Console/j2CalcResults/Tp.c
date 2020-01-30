// ********************************************************************
//
// FILENAME:  
// 
// 		Tp.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		TP calculation algorithm
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
//		CalcTp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		TP calculation algorithm 
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

void	CalcTp(void)
{
	// invoke standard endpoint processor for TP with analyte blank verification set

	ProcessStdEndpointAlgorithm(TP, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("TP invoked\n");
#endif
}
