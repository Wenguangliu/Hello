// ********************************************************************
//
// FILENAME:  
// 
// 		Glu.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		GLU calculation algorithm
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
//		CalcGlu() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		GLU calculation algorithm 
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

void CalcGlu(void)
{
	// invoke standard endpoint processor for GLU with analyte blank verification set

	ProcessStdEndpointAlgorithm(GLU, ANALYTE_BLANK_CHK_ON);

#ifndef UNDER_CE
	printf("GLU invoked\n");
#endif
}

//
//
// Note: 	could modify to allow for "void CalcGlu(unsigned char analyteType, unsigned algType)"
//			& modify algorithms.c to pass parameters --- may do later with algType added to function
//			call table as a struct parameter.			
//




