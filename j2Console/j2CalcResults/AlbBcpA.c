// ********************************************************************
//
// FILENAME:  
// 
// 		AlbBcpA.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ALB_BCP_A algorithm
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
//		CalcAlbBcpA() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		ALB_BCP_A calculation algorithm 
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

void	CalcAlbBcpA(void)
{
	ProcessStdEndpointAlgorithm(ALB_BCP_A, ANALYTE_BLANK_CHK_ON);

	// set ALB_BCP_A_METHOD in ALB_BCP_A analyteFlags to signal ALB BCP type A calc method result

	rotorResults_g->rotorAnalyteResult[ALB_BCP_A].analyteFlags |= ALB_BCP_A_METHOD;


#ifndef UNDER_CE
	printf("ALB_BCP_A invoked\n");
#endif
}
