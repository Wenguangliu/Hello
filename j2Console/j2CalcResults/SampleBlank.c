// ********************************************************************
//
// FILENAME:  
// 
// 		SampleBlank.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		SAMPLE_BLANK calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


#define AVG_340_850		0
#define AVG_405_850		1
#define AVG_467_850		2

#define	MAX_STARTING_ABS_CHECK_PAIRS	8
#define	MAX_AVG_ABS_PAIRS				3
#define	SAMPLE_QUALITY_ERROR_CHECK_IDX	5
#define	SAMPLE_BLANK_LESS_DILUENT		0


const unsigned char	startingAbsWavelengthPair_m[MAX_STARTING_ABS_CHECK_PAIRS] = 
{
	WP_630_850,		// 0	SB_START_ABS_ERROR for rotorErrorNumber
	WP_600_850,		// 1
	WP_550_850,		// 2
	WP_515_850,		// 3
	WP_500_850,		// 4

	WP_467_850,		// 5	SAMPLE_QUALITY for rotorErrorNumber	
	WP_405_850,		// 6
	WP_340_850		// 7
};


// ********************************************************************
// FUNCTION:
//
//		ProcessSampleBlankError() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process error found during sample blank algorithm 
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

unsigned ProcessSampleBlankError(unsigned long sampleBlankError, unsigned startAbsIdx)
{
	if (ProcessSystemCalcErrors(sampleBlankError))
	{
		return	TRUE;
	}

	if (sampleBlankError & BLANK_BEAD_MIX_ERROR)
	{
		ReportSystemError(SB_BEAD_NOT_MIXED, SERR_SB_BEAD_MIX); 
		return	TRUE;
	}
	else if (sampleBlankError & CHEM_BEAD_MISSING)
	{
		ReportSystemError(BEAD_MISSING_2, SERR_BEAD_MISSING); 
		return	TRUE;
	}
	else if (sampleBlankError & CHEM_DIST_ERROR)
	{
		ReportSystemError(DIST_CHECK_FAILED, SERR_DISTRIBUTION); 
		return	TRUE;
	}
	else if (sampleBlankError & START_ABS_BAD)
	{

		if (startAbsIdx < SAMPLE_QUALITY_ERROR_CHECK_IDX)
		{
			// 500, 515, 550, 600, 630 start abs error number

			ReportSystemError(HIGH_SB_START_ABS, SERR_SB_START_ABS); 
		}
		else
		{
			// 467, 405, 340 start abs error flag set as sample quality number

			ReportSystemError(HIGH_SB_START_ABS, SERR_SAMPLE_QUALITY); 
		}

		return	TRUE;
	}

	return	FALSE;		// no errors found during sample blank algorithm processing
}


// ********************************************************************
// FUNCTION:
//
//		ProcessEndogenousIndices() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate and save results for endogenous hemolyzed, lipemic & 
//		icteric indices for rotor sample limit checks for patient type
//		index using constants in rotor ROC file
// 
//			PATIENT_TYPE_DOG_FEMALE  = 0
//			PATIENT_TYPE_CAT_MALE    = 1
//			PATIENT_TYPE_HORSE       = 2
//			PATIENT_TYPE_UNSPECIFIED = 3
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

void ProcessEndogenousIndices(float *correctedAvg)
{
	float	sbLip340;
	float	sbHem405;
	float	sbIct467;

	PatientType_t		patientType;
	RotorCalcRecord_t	*rocPtr;

	patientType = commonAlgGlobals_g->patientType;
	rocPtr = &rotorConfig_g->rotorCalculationRecord;

	// compute and save intermediate lip, hem, ict result values from matrix table for rotor

	sbLip340 = (rocPtr->lipemic340InvertedRatioMatrix[patientType] * correctedAvg[AVG_340_850]) + (rocPtr->hemolyzed340InvertedRatioMatrix[patientType] * correctedAvg[AVG_405_850]) + (rocPtr->icteric340InvertedRatioMatrix[patientType] * correctedAvg[AVG_467_850]); 	
	rotorResults_g->rotorInformationResults.sampleBlankCalcs.sampleBlankLipemic340 = sbLip340;

	sbHem405 = (rocPtr->lipemic405InvertedRatioMatrix[patientType] * correctedAvg[AVG_340_850]) + (rocPtr->hemolyzed405InvertedRatioMatrix[patientType] * correctedAvg[AVG_405_850]) + (rocPtr->icteric405InvertedRatioMatrix[patientType] * correctedAvg[AVG_467_850]); 	
	rotorResults_g->rotorInformationResults.sampleBlankCalcs.sampleBlankHemolyzed405 = sbHem405;

	sbIct467 = (rocPtr->lipemic467InvertedRatioMatrix[patientType] * correctedAvg[AVG_340_850]) + (rocPtr->hemolyzed467InvertedRatioMatrix[patientType] * correctedAvg[AVG_405_850]) + (rocPtr->icteric467InvertedRatioMatrix[patientType] * correctedAvg[AVG_467_850]); 	
	rotorResults_g->rotorInformationResults.sampleBlankCalcs.sampleBlankIcteric467 = sbIct467;

	// save calculated final endogenous indices for rotor results /

	rotorResults_g->rotorInformationResults.rotorLipemicIndex   = (sbLip340 * rocPtr->lipemicSlope[patientType])   + rocPtr->lipemicIntercept[patientType];

	rotorResults_g->rotorInformationResults.rotorHemolyzedIndex = (sbHem405 * rocPtr->hemolyzedSlope[patientType]) + rocPtr->hemolyzedIntercept[patientType];

	rotorResults_g->rotorInformationResults.rotorIctericIndex   = (sbIct467 * rocPtr->ictericSlope[patientType])   + rocPtr->ictericIntercept[patientType];


	// IMV - endogenous calculated params store

	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_340_TURB,      IMV_DIRECT_SET_METHOD, sbLip340 );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_405_HEME,      IMV_DIRECT_SET_METHOD, sbHem405 );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_467_ICT,       IMV_DIRECT_SET_METHOD, sbIct467 );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_HB_INDEX,      IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.rotorHemolyzedIndex );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_LIPEMIC_INDEX, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.rotorLipemicIndex );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_ICTERIC_INDEX, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.rotorIctericIndex );

	// end IMV


}


// ********************************************************************
// FUNCTION:
//
//		CalcSampleBlank() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		SAMPLE_BLANK calculation algorithm 
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

void CalcSampleBlank(void)
{
	unsigned		i;
	unsigned char	sampleTypeSampleBlank;
	unsigned char	sampleTypeOpticalBlank;			
	unsigned char	sbCuvetteNumber;
	unsigned char	diluentCuvetteNumber;			
	unsigned long	sampleBlankError;
	float			desiredPathLength;
	float			correctedAvg[MAX_AVG_ABS_PAIRS];
				
	AvgAbs_t 		avgAbs;
	AvgAbs_t 		*avgAbsPtr;


	// IMV - SAMPLE_BLANK paragraph 1 store

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 1.0 );

	// end IMV


	avgAbsPtr = &avgAbs;

	// init sample blank algorithm related rotor error flags for no errors

	sampleBlankError = 0;

	// sample blank analyte ROC data may not be needed here if already loaded during rotor system bead & distribution checks

	LoadAnalyteAlgData(SAMPLE_BLANK);


	// IMV - SAMPLE_BLANK endogenous constant store / SAMPLE_BLANK paragraph 2 store

	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_LIP_340,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.lipemic340InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_HEM_340,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.hemolyzed340InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_ICT_340,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.icteric340InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_LIP_405,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.lipemic405InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_HEM_405,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.hemolyzed405InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_ICT_405,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.icteric405InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_LIP_467,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.lipemic467InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_HEM_467,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.hemolyzed467InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_ICT_467,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.icteric467InvertedRatioMatrix[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_LIP_SLOPE, IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.lipemicSlope[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_LIP_INT,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.lipemicIntercept[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_HEM_SLOPE, IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.hemolyzedSlope[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_HEM_INT,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.hemolyzedIntercept[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_ICT_SLOPE, IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.ictericSlope[commonAlgGlobals_g->patientType] );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_CONST_ICT_INT,   IMV_DIRECT_SET_METHOD, rotorConfig_g->rotorCalculationRecord.ictericIntercept[commonAlgGlobals_g->patientType] );

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 2.0 );

	// end IMV


	// set up common parameters from ROC fle

	sampleTypeSampleBlank  = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].sampleTypeOpticalBlank;	
	sbCuvetteNumber        = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteBlankCuvetteNumber;
	diluentCuvetteNumber   = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteCuvetteNumber;     
	desiredPathLength	   = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteDesiredPathLength;


	// IMV - SAMPLE_BLANK paragraph 3 store

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 3.0 );

	// end IMV


	// check bead mix of sample blank at diluent check position

	BeadMixCheck(sampleTypeSampleBlank, sampleTypeOpticalBlank, sampleBlankConfig_g.beadMixCheckPass, sampleBlankConfig_g.beadMixCheckLimit, BLANK_BEAD_MIX_ERROR, &sampleBlankError);

	// process sample blank bead missing and distribution checks for non T4 rotors (not rotor 40)
	 
	if (rotorConfig_g->rotorNumber != 40)
	{
		BeadMissingCheck(sbCuvetteNumber, sampleBlankConfig_g.beadMissingCheckLimit, &sampleBlankError);
		BeadDistributionCheck(sbCuvetteNumber, sampleBlankConfig_g.distributionCheckLimit, &sampleBlankError);
	}


	// IMV - SAMPLE_BLANK bead tests store / SAMPLE_BLANK paragraph 4 store

	StoreBeadMixImv( SAMPLE_BLANK );

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 4.0 );

	// end IMV


	if (ProcessSampleBlankError(sampleBlankError, FALSE))
	{
		return;
	}

	// check starting absorbance for each wavelength pair with wavelength/850

	for (i = 0; i < MAX_STARTING_ABS_CHECK_PAIRS; i++)
	{
		StartingAbsorbanceCheck(sampleTypeSampleBlank, sampleTypeOpticalBlank, sampleBlankConfig_g.startAbsCheckPass, startingAbsWavelengthPair_m[i], sampleBlankConfig_g.startAbsLimit, START_ABS_BAD, &sampleBlankError);

		if (ProcessSampleBlankError(sampleBlankError, i))
		{
			return;
		}
	}


	// IMV - SAMPLE_BLANK starting abs params store / SAMPLE_BLANK paragraph 5 store

	StoreSampleBlankStartAbsImv();

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 5.0 );

	// end IMV


	// get average abs of 340/850, 405/850, 467/850 for endogenous limit computation input

	for (i = 0; i < MAX_AVG_ABS_PAIRS; i++)
	{
		CalcAverageAbsorbance(avgAbsPtr, sampleTypeSampleBlank, sampleBlankConfig_g.sampleTypeDiluent, sampleTypeOpticalBlank, sampleBlankConfig_g.avgStartPass, sampleBlankConfig_g.avgEndPass, sampleBlankConfig_g.avgWavelengthPair[i], desiredPathLength, sbCuvetteNumber, diluentCuvetteNumber, SAMPLE_BLANK_LESS_DILUENT, &sampleBlankError);

		correctedAvg[i] = avgAbs.avg;


		// IMV - SAMPLE_BLANK average absorbance set for 340, 405, 467

		SetImvSampleBlankAvgAbsParams( i, avgAbsPtr );

		// end IMV


		if (ProcessSystemCalcErrors(sampleBlankError))
		{
			return;
		}
	}


	// IMV - SAMPLE_BLANK average abs params store / SAMPLE_BLANK paragraph 7 store

	StoreSampleBlankAvgAbsImv();

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 7.0 );

	// end IMV


	// calculate and save results for endogenous hemolyzed, lipemic & icteric indices for rotor sample limit checks

	ProcessEndogenousIndices(correctedAvg);


	// IMV - SAMPLE_BLANK paragraph 8 store

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 8.0 );

	// end IMV


#ifndef UNDER_CE
	printf("SAMPLE_BLANK invoked\n");
#endif
}
