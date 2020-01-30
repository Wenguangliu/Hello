// ********************************************************************
//
// FILENAME:  
// 
// 		AlbBcg.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ALB_BCG algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// ALB_BCG custom defines

#define ALB_BCG_TERT_IDX		        1           // 405/500 index for check wavelength pair
#define ALG_BCG_TERT_START_ABS_LIMIT    1.0         // 405/500 start abs limit



// ********************************************************************
// FUNCTION:
//
//		CalcAlbBcg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		ALB_BCG calculation algorithm 
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

void	CalcAlbBcg(void)
{
    unsigned char   sampleTypeAnalyte;
    unsigned char   sampleTypeOpticalBlank;
    unsigned char   startAbsCheckWavelengthPair;
    unsigned char   startAbsCheckPass;
    unsigned char   sampleTypeAnalyteBlank;
    unsigned char   analyteCuvetteNumber;
    unsigned char   analyteBlankCuvetteNumber;
    float           desiredPathLength;
    float           analyteAbs;
    float           analyteBlankAbs;
    float           blankedAbs;
    float           startAbsLimit;

    // process result using standard endpoint algorithm

    ProcessStdEndpointAlgorithm(ALB_BCG, ANALYTE_BLANK_CHK_ON);


    // ALB_BCG custom code

    // check starting ABS for blanked tertiary only & set ALB_DOUBLE_BEAD & START_ABS_BAD
    // analyte flags for ALB_BCG, if blanked tertiary 405/500 over limit & update IMV, etc.

    sampleTypeOpticalBlank      = rotorConfig_g->analyteCalcRecord[ALB_BCG].sampleTypeOpticalBlank;
    sampleTypeAnalyteBlank      = rotorConfig_g->analyteCalcRecord[ALB_BCG].sampleTypeAnalyteBlank;
    analyteCuvetteNumber        = rotorConfig_g->analyteCalcRecord[ALB_BCG].analyteCuvetteNumber;
    analyteBlankCuvetteNumber   = rotorConfig_g->analyteCalcRecord[ALB_BCG].analyteBlankCuvetteNumber;
    desiredPathLength           = rotorConfig_g->analyteCalcRecord[ALB_BCG].analyteDesiredPathLength;

    sampleTypeAnalyte           = endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte;
    startAbsCheckWavelengthPair = endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[ALB_BCG_TERT_IDX];
    startAbsCheckPass           = endpointConfig_g.endpointAlgRecord.startAbsCheckPass;

    startAbsLimit               = ALG_BCG_TERT_START_ABS_LIMIT;

    // compute blanked abs for tert 405/500

    analyteAbs      = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, startAbsCheckPass, startAbsCheckWavelengthPair, desiredPathLength, analyteCuvetteNumber, &rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags);

    analyteBlankAbs = CalcSampleAbsorbance(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, startAbsCheckPass, startAbsCheckWavelengthPair, desiredPathLength, analyteBlankCuvetteNumber, &rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags);

    blankedAbs = analyteAbs - analyteBlankAbs;

    if (rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags & commonAlgGlobals_g->calcErrorMask)
    {
        rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteResult = NOVAL_NEG_FLOAT;
    }

    // check if blanked abs for tert 405/500 is over starting abs limit & set flags/suppress result

    if (blankedAbs > startAbsLimit)
    {
        rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags |= ( START_ABS_BAD | ALB_DOUBLE_BEAD );
        rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteResult = NOVAL_NEG_FLOAT;
    }

    // update IMV for blanked abs 405/500

    StoreIntermediateValue( ALB_BCG, IMV_STARTING_ABS_B_BLANK,   IMV_DIRECT_SET_METHOD, blankedAbs );
    StoreIntermediateValue( ALB_BCG, IMV_TEST_START_ABS_LIMIT_B, IMV_DIRECT_SET_METHOD, startAbsLimit );


    // set ALB_BCG_METHOD in bcg analyteFlags to signal BCG type of ALB result otherwise is BCP type

    rotorResults_g->rotorAnalyteResult[ALB_BCG].analyteFlags |= ALB_BCG_METHOD;


#ifndef UNDER_CE
	printf("ALB_BCG invoked\n");
#endif
}
