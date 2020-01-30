// ********************************************************************
//
// FILENAME:  
// 
// 		RotorDilutionChecks.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Functions to process rotor dilution error checks
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <math.h>				// required standard C library 

#include "CommonAlgFuncs.h"
#include "RotorResults.h"


// ********************************************************************
// FUNCTION:
//
//		UpdateRotorDilutionCheckProbs() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update short and long sample probabilities for each analyte result if enabled and 
//			- analyte is NOT suppressed by other analyte flags set
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

float CalcAnalyteDilutionProb(float analtyeResult, unsigned equationSelect, float mean, float sigma)
{
	float	exponent;

	exponent = (analtyeResult - mean) / sigma;

	if (equationSelect == GAUSS_EQU)	
	{
		// probability = e**(-0.5 * ((analtyeResult - mean) / sigma)**2) for Guassian equation

		exponent = -0.5 * exponent * exponent;
	}
	else			  // EXP_EQU 
	{
		// probability = e**(-1.0 * ((analtyeResult - mean) / sigma)) for Exponential equation 

		exponent = -1.0 * exponent;		
	}

	return exp(exponent); 		
}


// ********************************************************************
// FUNCTION:
//
//		UpdateRotorDilutionCheckProbs() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update short and long sample probabilities for each analyte result if enabled and 
//			- analyte is NOT suppressed by other analyte flags set
//			- print order is not 255 (only process for printed analytes)
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

void UpdateRotorDilutionCheckProbs(unsigned char analyteType, unsigned long analyteFlags, unsigned long analyteSuppressionMask, float analyteResult)
{
	float	analyteShortSampleProbability;
	float	analyteLongSampleProbability;

	DilutionCheckFactors_t	*dilutionFactorsPtr;
	DilutionCheckResults_t	*dilutionResultsPtr;

	dilutionFactorsPtr = &commonAlgGlobals_g->dilutionCheckFactors[analyteType];
	dilutionResultsPtr = &rotorResults_g->rotorInformationResults.dilutionCheckResults;


	// do not suppress dilution check for NA type result if NACL_NA_LOW_RECOVERY flag set
	// in NA type analyte flags from NACL algorithm, so mask off NACL_NA_LOW_RECOVERY flag
	// before using analyteSuppressionMask to check if NA type result should be suppressed

	if ( ( analyteType == NA ) || ( analyteType == NA_1 ) || ( analyteType == NA_2 ) )
	{
		analyteSuppressionMask &= ( ~NACL_NA_LOW_RECOVERY );
	}


	// process analyte in dilution check alg only if analyeType enabled

	if (dilutionFactorsPtr->dilutionCheckEnabled)
	{
		// check for analyte suppression here as per legacy
		// may want to modify (e.g. by adding system range error to mask)
		// passed flags & mask for ease of modification (could have used resultPtr->resultPrintFlags.resultSuppressed otherwise)

		if ((analyteFlags & analyteSuppressionMask) != NO_ANALYTE_ERROR)
		{
			// result suppressed by other analyte error (except for system & dynamic range per legacy) so incr suppressed count

			dilutionResultsPtr->rotorDilutionCheckNumberSuppressed++;
		}
		else
		{
			// process short sample probability	for analyte result on rotor

			if (analyteResult > dilutionFactorsPtr->shortSampleMean)
			{
				analyteShortSampleProbability = 1.0;
			}
			else
			{
				analyteShortSampleProbability = CalcAnalyteDilutionProb(analyteResult, dilutionFactorsPtr->shortSampleEquationSelect, dilutionFactorsPtr->shortSampleMean, dilutionFactorsPtr->shortSampleSigma);
			}

			dilutionResultsPtr->rotorShortSampleProbability += analyteShortSampleProbability;


			// process long sample probability	for analyte result on rotor

			if (analyteResult < dilutionFactorsPtr->longSampleMean)
			{
				analyteLongSampleProbability = 1.0;
			}
			else
			{
				analyteLongSampleProbability = CalcAnalyteDilutionProb(analyteResult, dilutionFactorsPtr->longSampleEquationSelect, dilutionFactorsPtr->longSampleMean, dilutionFactorsPtr->longSampleSigma);
			}

			dilutionResultsPtr->rotorLongSampleProbability += analyteLongSampleProbability;
		}

		// incr number of enabled analytes checked on rotor

		dilutionResultsPtr->rotorDilutionCheckNumberChecked++;
	}
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRotorDilutionChecks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check total rotor short and long sample probability versus rotor ROC limits	and
//		if limits are exceeded:
//			- set rotor dilution check error flag if limits in rotorInformationResults
//			- set dilutionCheckError flag in resultPrintFlags for each analyte printed for rotor
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

void ProcessRotorDilutionChecksErrors(unsigned char *rotorAnalyteTypesByCalcIndex)
{
	unsigned	rotorDilutionLowDynamicRangeCheckEnabled;
	unsigned	rotorAnalyteLowDynamicRangeCountLimit;

	float	analytesUsedInProbabilityCalc;

	DilutionCheckResults_t	*dilutionResultsPtr;

	dilutionResultsPtr = &rotorResults_g->rotorInformationResults.dilutionCheckResults;

	// init dilution check accummulators and flags for no error

	dilutionResultsPtr->rotorDilutionCheckError = FALSE;

	dilutionResultsPtr->rotorDilutionCheckNumberSuppressed = 0;
	dilutionResultsPtr->rotorDilutionCheckNumberChecked    = 0;
	dilutionResultsPtr->rotorShortSampleProbability = 0.0;
	dilutionResultsPtr->rotorLongSampleProbability  = 0.0;

	// only process if rotor ROC enables standard dilution check and not a control rotor

	if ((rotorConfig_g->rotorCalculationRecord.rotorDilutionCheckEnabled) && ( commonAlgGlobals_g->controlType == FALSE ))
	{
		// scan to update short & long sample counters and probability accummulators for analytes with valid printOrder

		ScanUpdateRotorAnalyteResultFlags(SCAN_SHORT_LONG_SAMPLE_PROBS, rotorAnalyteTypesByCalcIndex); 

		// only one supressed chemistry allowed to continue dilution verification for rotor 

		if ( dilutionResultsPtr->rotorDilutionCheckNumberSuppressed <= 1 )	
		{
			// compute number of analytes used in short & long sample probability calculations

			analytesUsedInProbabilityCalc = (float)(dilutionResultsPtr->rotorDilutionCheckNumberChecked - dilutionResultsPtr->rotorDilutionCheckNumberSuppressed);

			// compute to compare against rotor thresholds as		 			
			// 		1 - (SUM(sampleProbability) - analytesUsedInProbabilityCalc)		
			//		for both short sample & long sample checks					

			if (((1 - (dilutionResultsPtr->rotorShortSampleProbability / analytesUsedInProbabilityCalc)) > rotorConfig_g->rotorCalculationRecord.rotorShortSampleProbThreshold) ||
         		((1 - (dilutionResultsPtr->rotorLongSampleProbability  / analytesUsedInProbabilityCalc)) > rotorConfig_g->rotorCalculationRecord.rotorLongSampleProbThreshold))
			{
				// rotor dilution verification failed, so set dilution verification failed for rotor  

				dilutionResultsPtr->rotorDilutionCheckError = TRUE;

				// and scan to set dilutionCheckError for resultPrintFlags for each analyte on rotor
		
				ScanUpdateRotorAnalyteResultFlags(SCAN_SET_DILUTION_CHECK_FLAGS, rotorAnalyteTypesByCalcIndex); 
			}
		}
	}


	// only process if rotorDilutionLowDynamicRangeCheckEnabled is set in ROC file and not a control rotor

	rotorDilutionLowDynamicRangeCheckEnabled = rotorConfig_g->rotorCalculationRecord.rotorFlags[ 0 ];
	rotorAnalyteLowDynamicRangeCountLimit    = rotorConfig_g->rotorCalculationRecord.rotorFlags[ 1 ];

	if (( rotorDilutionLowDynamicRangeCheckEnabled ) && ( commonAlgGlobals_g->controlType == FALSE ))
	{
		if ( commonAlgGlobals_g->numAnalytesBelowDynamicRange >= rotorAnalyteLowDynamicRangeCountLimit )
		{
			// rotor dilution verification failed (too many anaylte results below dynamic range), so set dilution verification failed for rotor  

			dilutionResultsPtr->rotorDilutionCheckError = TRUE;

			// and scan to set dilutionCheckError for resultPrintFlags for each analyte on rotor
		
			ScanUpdateRotorAnalyteResultFlags(SCAN_SET_DILUTION_CHECK_FLAGS, rotorAnalyteTypesByCalcIndex); 
		} 
	}
}




