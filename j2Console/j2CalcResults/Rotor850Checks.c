// ********************************************************************
//
// FILENAME:  
// 
// 		Rotor850Checks.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Functions to process rotor bead and rotor distribution error checks
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "RotorResults.h"


// ********************************************************************
// FUNCTION:
//
//		UpdateBeadMissingDistributionErrorCounts() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Increment bead missing counter & distribution error counter	only if
//			- analyte is suppressed by other analyte flags set
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

unsigned long UpdateBeadMissingDistributionErrorCounts(unsigned long analyteFlags, unsigned long analyteSuppressionMask)
{
	// first, check if analyte result is suppressed (per legacy logic) other than for bead missing or distribution error

	if (analyteFlags & ((analyteSuppressionMask | ANALYTE_RESULT_LIMITS_ERR_MASK) & ~(CHEM_BEAD_MISSING | CHEM_DIST_ERROR)))
	{
		// mask to insure T4 & derived calcs have no bead missing or distribution error bits set

		if ((analyteFlags & analyteSuppressionMask) & CHEM_BEAD_MISSING)
		{
			// increment count for bead missing on rotor
				
			rotorResults_g->rotorInformationResults.rotorBeadMissingCheckCount++;
		}

		if ((analyteFlags & analyteSuppressionMask) & CHEM_DIST_ERROR)
		{
			// increment count for distribution error on rotor
				
			rotorResults_g->rotorInformationResults.rotorDistributionCheckCount++;
		}
	}
	else
	{
		// clear CHEM_BEAD_MISSING and CHEM_DIST_ERROR, since no other suppression flags set in analyteFlags
				
		analyteFlags &= ~(CHEM_BEAD_MISSING | CHEM_DIST_ERROR); 
	}

	return analyteFlags;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRotorAnalyteBeadMissingDistributionErrors() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update bead missing & distribution error counters for
//			- analytes on rotor with valid print order
//		Check bead missing & distribution check error counts versus rotor ROC limits
//			- report rotor error number if limits exceeded
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

void ProcessRotorAnalyteBeadMissingDistributionErrors(unsigned char *rotorAnalyteTypesByCalcIndex)
{
	// init rotor ROC check counters

	rotorResults_g->rotorInformationResults.rotorBeadMissingCheckCount  = 0;
	rotorResults_g->rotorInformationResults.rotorDistributionCheckCount = 0;

	// scan to incr bead missing or distribution error counters for analytes with valid printOrder

	ScanUpdateRotorAnalyteResultFlags(SCAN_BEAD_DISTRIBUTION_COUNTS, rotorAnalyteTypesByCalcIndex); 

	// check counters to report rotor errors if any

	if (rotorResults_g->rotorInformationResults.rotorBeadMissingCheckCount > rotorConfig_g->rotorCalculationRecord.rotorBeadMissingCheckLimit)
	{
		ReportSystemError(BEAD_MISSING_2, SERR_BEAD_MISSING); 
	}
	else if (rotorResults_g->rotorInformationResults.rotorDistributionCheckCount > rotorConfig_g->rotorCalculationRecord.rotorDistributionCheckLimit)
	{
		ReportSystemError(DIST_CHECK_FAILED, SERR_DISTRIBUTION); 
	}
	else if ((rotorResults_g->rotorInformationResults.rotorSystemFlags & SAMP_DIL_MIX_ERROR) && (rotorConfig_g->rotorNumber != 40))
	{
		ReportSystemError(SAMP_DIL_MIX_ERROR, SERR_SAMPLE_DIL_MIX); 
	}
}




