// ********************************************************************
//
// FILENAME:  
// 
// 		Rqc.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		RQC calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <math.h>				// required standard C library 
#include "CommonAlgFuncs.h"


#define RQC_TOO_LOW				0x0040
#define RQC_BEAD_NOT_MIXED		0x0010

#define	RQC_HIGH				0x00000002
#define	RQC_LOW					0x00000001

#define RQC_BASE_ABS			0
#define RQC_LOW_LIMIT			1
#define RQC_DECAY_CONST			2


// ********************************************************************
// FUNCTION:
//
//		ReportRqcError() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Got RQC type error so check to report insufficient diluent system error
//		else report RQC type system error 
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

void ReportRqcError(unsigned long rqcSystemFlags, unsigned short rqcErrorNumber)
{
	// insufficient diluent error number has priority

	if (rotorResults_g->rotorInformationResults.rotorSystemFlags & INSUFFICIENT_DIL)
	{
		rqcErrorNumber = SERR_INSUFFICIENT_DILUENT;
	}

	ReportSystemError(rqcSystemFlags, rqcErrorNumber);
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRqcError() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process errors detected in RQC algorithm 
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

unsigned ProcessRqcError(unsigned long rqcError)
{
	// check for calc type system errors (flash range, divide by 0, illegal log param)

	if (ProcessSystemCalcErrors(rqcError))
	{
		return	TRUE;
	}
	else if (rqcError & TEST_BEAD_MIX_ERROR)
	{
		rotorResults_g->rotorInformationResults.rotorQcFlags |= RQC_BEAD_NOT_MIXED;
		ReportRqcError(NO_SYSTEM_FLAG, SERR_QC_MIX);
		return	TRUE;
	}
	else if (rqcError & RQC_LOW)
	{
		rotorResults_g->rotorInformationResults.rotorQcFlags |= RQC_TOO_LOW;
		ReportRqcError(NO_SYSTEM_FLAG, SERR_RQC_OUT_OF_RANGE);
		return	TRUE;
	}
	else if (rqcError & RQC_HIGH)
	{
		ReportRqcError(RQC_TOO_HIGH, SERR_RQC_OUT_OF_RANGE);
		return	TRUE;
	}
	 
	return FALSE;		// no errors found
}


// ********************************************************************
// FUNCTION:
//
//		CalcRqc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		RQC calculation algorithm 
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

void	CalcRqc(void)
{
	float			rqcResult;
	unsigned long	rqcError;

	unsigned char	i;
	unsigned char	sampleTypeAnalyte;
	unsigned char	sampleTypeOpticalBlank;			
	unsigned char	analyteCuvetteNumber;
	float			desiredPathLength;

	unsigned char	avgStartPass;
	unsigned char	avgEndPass;
	float			rqcAvgAbs;
	float			rqcCorrectedAvg;
	float			expectedQc;
	float			rqcResultBaseAbs;
	float			rqcResultLowLimit;
	float			rqcResultDecayConst;
					
	CommonFactorsLimits_t	factorsLimits;


	// IMV - RQC paragraph 2 store

	StoreAlgParagraphNumImv( RQC, 2.0 );

	// end IMV


	// init rqc algorithm result & flags

	rqcResult = NOVAL_NEG_FLOAT;
	rqcError = 0;

	rotorResults_g->rotorInformationResults.rotorRqcAbused = FALSE;

	factorsLimits.analyteSuppressionMask = 0;
	factorsLimits.lowSystemLimit   = 0.0;
	factorsLimits.lowDynamicRange  = 0.0;
	factorsLimits.highDynamicRange = 0.0;
	factorsLimits.highSystemLimit  = 0.0;

	commonAlgGlobals_g->derivedResult = FALSE;

    rqcCorrectedAvg = NOVAL_NEG_FLOAT;
    expectedQc = NOVAL_NEG_FLOAT;


	// RQC analyte ROC data may not be needed here if already loaded during rotor system bead & distribution checks

	LoadAnalyteAlgData(RQC);

	// set up common parameters from ROC files

	sampleTypeAnalyte      = rqcConfig_g.sampleTypeAnalyte;
	sampleTypeOpticalBlank = rotorConfig_g->analyteCalcRecord[RQC].sampleTypeOpticalBlank;	
	analyteCuvetteNumber   = rotorConfig_g->analyteCalcRecord[RQC].analyteCuvetteNumber;     
	desiredPathLength	   = rotorConfig_g->analyteCalcRecord[RQC].analyteDesiredPathLength;
	
	avgStartPass 	  = rqcConfig_g.avgStartPass;
	avgEndPass	 	  = rqcConfig_g.avgEndPass;


	// IMV - RQC paragraph 4 store

	StoreAlgParagraphNumImv( RQC, 4.0 );

	// end IMV


	// check bead mix of RQC cuvette

	BeadMixCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, rqcConfig_g.beadMixCheckPass, rqcConfig_g.beadMixCheckLimit, TEST_BEAD_MIX_ERROR, &rqcError);


	// IMV - store bead mix params / RQC paragraph 5 store

	StoreBeadMixImv( RQC );

	StoreAlgParagraphNumImv( RQC, 5.0 );

	// end IMV


	// compute average absorbance for RQC checks

	rqcAvgAbs = 0.0;

	for (i = avgStartPass; i <= avgEndPass; i++)
	{
		rqcAvgAbs += CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, i, rqcConfig_g.avgWavelengthPair, desiredPathLength, analyteCuvetteNumber, &rqcError);	


		// IMV - RQC pass abs store

		StoreIntermediateValue( RQC, IMV_RQC_ABS_START_PASS, IMV_INCR_SET_METHOD, CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, i, rqcConfig_g.avgWavelengthPair, desiredPathLength, analyteCuvetteNumber, &rqcError) );

		// end IMV


	}

	rqcAvgAbs /= ((avgEndPass - avgStartPass) + 1);	
	
	// check for system calc errors or RQC bead mix error

	if (!ProcessRqcError(rqcError))
	{
		// correct avg using bfd from RQC ROC file

		rqcCorrectedAvg = BfdCalAdjust(rqcAvgAbs, rqcConfig_g.bfdCalibrationFactorIndex);

		// get result calc bar code factors from rotor ROC file

		rqcResultBaseAbs    = GetBarcodeParameter(RQC, RQC_BASE_ABS);
		rqcResultLowLimit   = GetBarcodeParameter(RQC, RQC_LOW_LIMIT);
		rqcResultDecayConst = GetBarcodeParameter(RQC, RQC_DECAY_CONST);


		// IMV - store abs params / store barcode params / RQC paragraph 6 store

		StoreIntermediateValue( RQC, IMV_RQC_AVG_ABS, IMV_DIRECT_SET_METHOD, rqcAvgAbs );
		StoreBfdCorrectedImv( RQC );
		StoreBarcodeParamsImv( RQC );

		StoreAlgParagraphNumImv( RQC, 6.0 );

		// end IMV


		// save RQC_LOW_LIMIT in percent in rotor information result area

		rotorResults_g->rotorInformationResults.rotorRqcLowLimit = (rqcResultLowLimit * 100.0);

		// calc expected QC value for limit comparisons

		expectedQc = exp(-rqcResultDecayConst * (barcodeData_g->rotorAgeInDays/DAYS_PER_MONTH)) * rqcResultBaseAbs;


		// IMV - RQC decay params / RQC paragraph 7 store

		StoreIntermediateValue( RQC, IMV_ROTOR_AGE_IN_DAYS,  IMV_DIRECT_SET_METHOD, barcodeData_g->rotorAgeInDays );
		StoreIntermediateValue( RQC, IMV_RQC_QC_TIME,        IMV_DIRECT_SET_METHOD, barcodeData_g->rotorAgeInDays/DAYS_PER_MONTH );
		StoreIntermediateValue( RQC, IMV_RQC_EXPONENT_VALUE, IMV_DIRECT_SET_METHOD, -rqcResultDecayConst * (barcodeData_g->rotorAgeInDays/DAYS_PER_MONTH) );
		StoreIntermediateValue( RQC, IMV_RQC_E_TO_THE_X,     IMV_DIRECT_SET_METHOD, exp(-rqcResultDecayConst * (barcodeData_g->rotorAgeInDays/DAYS_PER_MONTH)) );
		StoreIntermediateValue( RQC, IMV_RQC_EXPECTED_QC,    IMV_DIRECT_SET_METHOD, expectedQc );

		StoreAlgParagraphNumImv( RQC, 7.0 );

		// end IMV


		if (rqcCorrectedAvg < (expectedQc * rqcConfig_g.rqcRatioSuppressLimit))
		{
			rqcError |= RQC_LOW;
		}
		else if (rqcCorrectedAvg > (expectedQc * rqcConfig_g.rqcHighSuppressLimit))
		{
			rqcError |= RQC_HIGH;
		}


		// IMV - RQC low & high limits params / RQC paragraph 8 store

		StoreIntermediateValue( RQC, IMV_RQC_RATIO_SUPPRESS_LIMIT,  IMV_DIRECT_SET_METHOD, rqcConfig_g.rqcRatioSuppressLimit );
		StoreIntermediateValue( RQC, IMV_RQC_LOW_LIMIT,             IMV_DIRECT_SET_METHOD, expectedQc * rqcConfig_g.rqcRatioSuppressLimit );
		StoreIntermediateValue( RQC, IMV_RQC_HIGH_SUPPRESS_LIMIT,   IMV_DIRECT_SET_METHOD, rqcConfig_g.rqcHighSuppressLimit );
		StoreIntermediateValue( RQC, IMV_RQC_HIGH_LIMIT,            IMV_DIRECT_SET_METHOD, expectedQc * rqcConfig_g.rqcHighSuppressLimit );

		StoreAlgParagraphNumImv( RQC, 8.0 );

		// end IMV


		if (!ProcessRqcError(rqcError))
		{
			// check for RQC abused flag for input to affected analytes

			if (rqcCorrectedAvg < (expectedQc * rqcResultLowLimit))
			{
				rotorResults_g->rotorInformationResults.rotorRqcAbused = TRUE;
			}


			// IMV - RQC abused params / RQC paragraph 9 store

			StoreIntermediateValue( RQC, IMV_RQC_ABUSED_LIMIT, IMV_DIRECT_SET_METHOD, expectedQc * rqcResultLowLimit );
			StoreIntermediateValue( RQC, IMV_RQC_ABUSED,       IMV_DIRECT_SET_METHOD, (float) rotorResults_g->rotorInformationResults.rotorRqcAbused );

			StoreAlgParagraphNumImv( RQC, 9.0 );

			// end IMV


			// compute RQC result percentage

			rqcResult = (rqcCorrectedAvg / expectedQc) * 100.0;
		}
	}


	// IMV - RQC result / RQC paragraph 10 store

    StoreIntermediateValue( RQC, IMV_RQC_RESULT, IMV_DIRECT_SET_METHOD, rqcResult );

	StoreAlgParagraphNumImv( RQC, 10.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(RQC, rqcResult, rqcError, &factorsLimits);

#ifndef UNDER_CE
	printf("RQC invoked\n");
#endif
}
