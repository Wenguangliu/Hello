// ********************************************************************
//
// FILENAME:  
// 
// 		Ast.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		AST calculation algorithm
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
//		CalcAst() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		AST calculation algorithm 
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

void	CalcAst(void)
{
	// process standard 2 cuvette/data algorithm to combine analyte results & flags 

	Process2CuvAlgorithm(AST_1, AST_2, AST);

#ifndef __QNX__
	printf("AST invoked\n");
#endif
}
