// ********************************************************************
//
// FILENAME:  
// 
// 		Crp1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CRP_1 algorithm
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
//		CalcCrp1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CRP_1 calculation algorithm 
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

void	CalcCrp1(void)
{
	ProcessStdEndpointAlgorithm(CRP_1, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("CRP_1 invoked\n");
#endif
}