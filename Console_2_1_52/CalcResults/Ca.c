// ********************************************************************
//
// FILENAME:  
// 
// 		Ca.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CA calculation algorithm
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
//		CalcCa() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CA calculation algorithm 
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

void	CalcCa(void)
{
	ProcessStdEndpointAlgorithm(CA, ANALYTE_BLANK_CHK_ON);

#ifndef __QNX__
	printf("CA invoked\n");
#endif
}