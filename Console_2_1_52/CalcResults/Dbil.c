// ********************************************************************
//
// FILENAME:  
// 
// 		Dbil.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		DBIL calculation algorithm
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
//		CalcDbil() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		DBIL calculation algorithm 
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

void	CalcDbil(void)
{
	// invoke standard endpoint processor for TBIL with analyte blank verification set

	ProcessStdEndpointAlgorithm(DBIL, ANALYTE_BLANK_CHK_ON);


#ifndef __QNX__
	printf("DBIL invoked\n");
#endif
}
