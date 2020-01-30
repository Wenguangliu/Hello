// ********************************************************************
//
// FILENAME:  
// 
// 		Hdl2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HDL_2 calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"

#include "Hdl1.h"


// ********************************************************************
// FUNCTION:
//
//		CalcHdl2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		HDL_2 calculation algorithm 
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

void	CalcHdl2(void)
{
	// process the HDL_2 cuvette using custom endpoint algorithm & check analyte blank

	CalcHdlCuv(HDL_2, ANALYTE_BLANK_CHK_ON);


#ifndef UNDER_CE
	printf("HDL_2 invoked\n");
#endif
}
