// ********************************************************************
//
// FILENAME:  
// 
// 		Hb.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HB calculation algorithm
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
//		CalcHb() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		HB calculation algorithm 
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

void	CalcHb(void)
{
	// invoke standard endpoint processor for HB with analyte blank verification set

	ProcessStdEndpointAlgorithm(HB, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("HB invoked\n");
#endif
}
