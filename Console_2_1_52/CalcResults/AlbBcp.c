// ********************************************************************
//
// FILENAME:  
// 
// 		AlbBcp.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ALB_BCP algorithm
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
//		CalcAlbBcp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		ALB_BCP calculation algorithm 
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

void	CalcAlbBcp(void)
{
	ProcessStdEndpointAlgorithm(ALB_BCP, ANALYTE_BLANK_CHK_ON);

#ifndef __QNX__
	printf("ALB_BCP invoked\n");
#endif
}
