// ********************************************************************
//
// FILENAME:  
// 
// 		Tbil.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		TBIL calculation algorithm
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
//		CalcTbil() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		TBIL calculation algorithm 
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

void	CalcTbil(void)
{
	// invoke standard endpoint processor for TBIL with analyte blank verification set

	ProcessStdEndpointAlgorithm(TBIL, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("TBIL invoked\n");
#endif
}
