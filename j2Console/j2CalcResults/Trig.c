// ********************************************************************
//
// FILENAME:  
// 
// 		Trig.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		TRIG calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define	TRIG_RATE_TRIG_LIMIT		500.0			
#define	TRIG_RATE_LIPEMIC_LIMIT		1000.0			

#define TRIG_FLASH_RANGE_MASK		0x7ffffffc		/* Mask off flags and flash range error */
#define TRIG_FLASH_RANGE_SUPP		0x7fffffff		/* Mask flash range error */



// ********************************************************************
// FUNCTION:
//
//		CheckTrigRateInEndpoint() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process for TRIG rate in endpoint condition to set appropriate analyteError flag 
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

void	CheckTrigRateInEndpoint(void)
{
	// if TRIG rate in endpoint determine which analyteError flag should be set

	if (rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags & TRIG_RATE_IN_ENDPOINT)
	{
		if ((rotorResults_g->rotorAnalyteResult[TRIG].analyteResult > TRIG_RATE_TRIG_LIMIT)	&& (rotorResults_g->rotorInformationResults.rotorLipemicIndex > TRIG_RATE_LIPEMIC_LIMIT))
		{
			rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags |= HIGH_DYNAMIC_RANGE;
			rotorResults_g->rotorAnalyteResult[TRIG].resultPrintFlags |= MASK_HIGH_DYNAMIC_LIMIT;
		}
		else
		{
			rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags |= RATE_IN_ENDPOINT;
		}
	}
}

// ********************************************************************
// FUNCTION:
//
//		ProcessTrigFlashRangeError() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process for TRIG flash range errors versus dynamic range error condition
//		clear flash range error analyteError flag 
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

void	ProcessTrigFlashRangeError(void)
{
	// High Dynamic Range Error?

	if (rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags & HIGH_DYNAMIC_RANGE)					 
	{
		// Just flash range and high dynamic range errors?

		if (!(rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags & TRIG_FLASH_RANGE_MASK & ~HIGH_DYNAMIC_RANGE))	 
		{
			// Yes, mask off flash range error
			// Will take this path if just high dynamic range
			// (no harm in clearing flash range error flag even if not set
																
			rotorResults_g->rotorAnalyteResult[TRIG].analyteFlags &= TRIG_FLASH_RANGE_SUPP;			 
		}													 
	}														 
}


// ********************************************************************
// FUNCTION:
//
//		CalcTrig() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		TRIG calculation algorithm 
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

void	CalcTrig(void)
{
	// invoke standard endpoint processor for TRIG with analyte blank verification set

	ProcessStdEndpointAlgorithm(TRIG, ANALYTE_BLANK_CHK_ON);


	// IMV - store TRIG analyte specific result check constants for TRIG rate in endpoint

	StoreIntermediateValue( TRIG, IMV_TRIG_RATE_TRIG_LIMIT, IMV_DIRECT_SET_METHOD, TRIG_RATE_TRIG_LIMIT );
	StoreIntermediateValue( TRIG, IMV_TRIG_RATE_LIP_LIMIT,  IMV_DIRECT_SET_METHOD, TRIG_RATE_LIPEMIC_LIMIT );
		
	// end IMV


	// Process TRIG error flags for rate in endpoint

	CheckTrigRateInEndpoint();

	// Process flash range errors versus high dynamic range condition

	ProcessTrigFlashRangeError();


#ifndef UNDER_CE
	printf("TRIG invoked\n");
#endif
}
