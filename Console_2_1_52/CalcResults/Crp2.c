// ********************************************************************
//
// FILENAME:  
// 
// 		Crp2.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		CRP_2 algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// IMV - indexes for set of A0, A1, A2 intermediate values used in CRP_1 calculation in CRP_2 intermediate values to facilitate production calibration

#define IMV_CRP_1_BARCODE_A0_USED_IDX		14
#define IMV_CRP_1_BARCODE_A1_USED_IDX		15
#define IMV_CRP_1_BARCODE_A2_USED_IDX		16

// end IMV


// ********************************************************************
// FUNCTION:
//
//		CalcCrp2() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		CRP_2 calculation algorithm 
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

void	CalcCrp2(void)
{
	ProcessStdEndpointAlgorithm(CRP_2, ANALYTE_BLANK_CHK_ON);


	// IMV - set A0, A1, A2 intermediate values used in CRP_1 calculation in CRP_2 intermediate values to facilitate production calibration

	StoreIntermediateValue( CRP_2, IMV_BARCODE_A0_USED, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorIntermediateValues[ CRP_1 ][ IMV_CRP_1_BARCODE_A0_USED_IDX ] );
	StoreIntermediateValue( CRP_2, IMV_BARCODE_A1_USED, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorIntermediateValues[ CRP_1 ][ IMV_CRP_1_BARCODE_A1_USED_IDX ] );
	StoreIntermediateValue( CRP_2, IMV_BARCODE_A2_USED, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorIntermediateValues[ CRP_1 ][ IMV_CRP_1_BARCODE_A2_USED_IDX ] );
		
	// end IMV


#ifndef __QNX__
	printf("CRP_2 invoked\n");
#endif
}
