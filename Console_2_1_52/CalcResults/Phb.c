// ********************************************************************
//
// FILENAME:  
// 
// 		Phb.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		PHB calculation algorithm
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
//		CalcLac() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		LAC calculation algorithm 
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

void	CalcPhb(void)
{
	ProcessStdRateAlgorithm(PHB);


#ifndef __QNX__
	printf("PHB invoked\n");
#endif
}
