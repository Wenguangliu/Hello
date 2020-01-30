// ********************************************************************
//
// FILENAME:  
// 
// 		BeadCheck1.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm processing for 
// 		checking full rotor scan raw data from the Engine at bead check 1 time
//		while outputting associated flags/parameters to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define INSUFFICIENT_SAMPLE_CUVETTE		1
#define INSUFFICIENT_SAMPLE_LIMIT		0.03


// ********************************************************************
// FUNCTION:
//
//		InsufficientSampleCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for checking for insufficient sample in cuvette 1
//		at bead 1 check time at 340/850
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

void InsufficientSampleCheck(void)
{
	unsigned long	cuvetteError;
	float			cuvetteAbsorbance;

	cuvetteError = 0;

	cuvetteAbsorbance = CalcCuvetteCheckAbsorbance(BEAD1_READINGS, INSUFFICIENT_SAMPLE_CUVETTE, WP_340_850, NUM_BEAD_CHECK_FLASHES, &cuvetteError);
	
	if (cuvetteError == 0)
	{
		if (cuvetteAbsorbance <	INSUFFICIENT_SAMPLE_LIMIT)
		{
			ReportSystemError(INSUFFICIENT_SAMP_1, SERR_INSUFFICIENT_SAMPLE);
		}
	}
        // IMV - init all IMV & store insufficient sample check parameters in SYSCHK IMV area

    InitRotorImv();

    StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_INSUF_SAMP_LIMIT,     IMV_DIRECT_SET_METHOD, INSUFFICIENT_SAMPLE_LIMIT );
    StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_INSUF_SAMP_CUV_1_ABS, IMV_DIRECT_SET_METHOD, cuvetteAbsorbance );

    // end IMV

}


// ********************************************************************
// FUNCTION:
//
//		SystemBeadChecks1() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for error checking after bead check 1 full rotor scan time
//		from Engine raw system cuvette readings & updating results flags/parameters
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

unsigned short SystemBeadChecks1(void)
{
	unsigned	rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// legacy did not check bead 1 limits or for empty cuvette 
	// so just process for insufficient sample check at system bead 1 time
	// only for customer rotors (rotor 1 is manufacturing test rotor)

	if (barcodeData_g->rotorId != 1)
	{
		InsufficientSampleCheck();
	}

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



