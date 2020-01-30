// ********************************************************************
//
// FILENAME:  
// 
// 		SharedRoc.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Shared Roc file data definition and test load code for algorithm testing
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <string.h>

#include "RocData.h"
#include "AlgGlobals.h"


static RotorConfig_t	rotorConfig;
RotorConfig_t *			rotorConfig_g = &rotorConfig;


// ********************************************************************
// FUNCTION:
//
//		LoadTestRoc() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load test ROC file data to structure for algorithm testing 
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

void LoadTestRoc(void)
{							   

/* DLR - comment out since no longer required; use for reference only & get rid of eventually

  unsigned	i;
	char		algVersion[ALG_VERSION_SIZE]= {"14.00"};	

	//	init all rotor analyte calculation indices for no calculation

	for	(i = 0;	i < MAX_ANALYTE_TYPES; i++)
	{
		rotorConfig_g->analyteCalcRecord[i].calculationIndex = NO_ANALYTE_CALC;
	}

	// init test rotor header info
	rotorConfig_g->rotorNumber = 52;
	rotorConfig_g->rotorFormat = 2;				 
	rotorConfig_g->rotorProductCode = HUMAN_ROTOR; 

	// set up test rotor specific/sample blank info

	rotorConfig_g->rotorCalculationRecord.hemolyzed340InvertedRatioMatrix = -0.3304367;
	rotorConfig_g->rotorCalculationRecord.lipemic340InvertedRatioMatrix = 1.1743131;
	rotorConfig_g->rotorCalculationRecord.icteric340InvertedRatioMatrix = 0.06910007;
	rotorConfig_g->rotorCalculationRecord.hemolyzed405InvertedRatioMatrix = 1.1859084;
	rotorConfig_g->rotorCalculationRecord.lipemic405InvertedRatioMatrix = -0.4307289;
	rotorConfig_g->rotorCalculationRecord.icteric405InvertedRatioMatrix = -0.7378163;
	rotorConfig_g->rotorCalculationRecord.hemolyzed467InvertedRatioMatrix = 0.02375283;
	rotorConfig_g->rotorCalculationRecord.lipemic467InvertedRatioMatrix = -0.4280568;
	rotorConfig_g->rotorCalculationRecord.icteric467InvertedRatioMatrix = 1.0395186;
	rotorConfig_g->rotorCalculationRecord.hemolyzedSlope = 839.2192;
	rotorConfig_g->rotorCalculationRecord.hemolyzedIntercept = -15.20343;
	rotorConfig_g->rotorCalculationRecord.lipemicSlope = 1999.165;
	rotorConfig_g->rotorCalculationRecord.lipemicIntercept = 67.28455;
	rotorConfig_g->rotorCalculationRecord.ictericSlope = 49.98997;
	rotorConfig_g->rotorCalculationRecord.ictericIntercept = 0.5225775;
	rotorConfig_g->rotorCalculationRecord.rotorBeadMissingCheckLimit = 1;
	rotorConfig_g->rotorCalculationRecord.rotorDistributionCheckLimit = 3;

	// set up test rotor analyte specific info

	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].analyteType = SYSTEM_CHECK;
	strcpy(rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].calculationIndex = 0;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].analyteCuvetteNumber = 255;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].analyteBlankCuvetteNumber = 6;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].sampleTypeAnalyteBlank = 20;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].sampleTypeOpticalBlank = 4;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].barcodeFactor[0].index = 44;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].barcodeFactor[0].scale = 0.001;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].barcodeFactor[0].offset =	0.0;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteType = SAMPLE_BLANK;
	strcpy(rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].calculationIndex = 1;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteCuvetteNumber = 6;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].sampleTypeAnalyteBlank = 14;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].sampleTypeOpticalBlank = 4;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[RQC].analyteType = RQC;
	strcpy(rotorConfig_g->analyteCalcRecord[RQC].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[RQC].calculationIndex = 2;
	rotorConfig_g->analyteCalcRecord[RQC].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[RQC].analyteCuvetteNumber = 3;
	rotorConfig_g->analyteCalcRecord[RQC].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[RQC].analyteDesiredPathLength = 5.0;
	rotorConfig_g->analyteCalcRecord[RQC].sampleTypeAnalyteBlank = 12;
	rotorConfig_g->analyteCalcRecord[RQC].sampleTypeOpticalBlank = 2;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[0].index = 41;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[0].scale = 0.002;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[0].offset =	0.05;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[1].index = 42;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[1].scale = 0.01;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[1].offset =	0.0;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[2].index = 43;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[2].scale = 0.001;
	rotorConfig_g->analyteCalcRecord[RQC].barcodeFactor[2].offset =	0.0;

	rotorConfig_g->analyteCalcRecord[IQC].analyteType = IQC;
	strcpy(rotorConfig_g->analyteCalcRecord[IQC].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[IQC].calculationIndex = 3;
	rotorConfig_g->analyteCalcRecord[IQC].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[IQC].sampleTypeOpticalBlank = 4;

	rotorConfig_g->analyteCalcRecord[ALP].analyteType = ALP;
	strcpy(rotorConfig_g->analyteCalcRecord[ALP].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[ALP].calculationIndex = 4;
	rotorConfig_g->analyteCalcRecord[ALP].printOrder = 9;
	rotorConfig_g->analyteCalcRecord[ALP].analyteCuvetteNumber = 8;
	rotorConfig_g->analyteCalcRecord[ALP].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[ALP].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[ALP].sampleTypeAnalyteBlank = 11;
	rotorConfig_g->analyteCalcRecord[ALP].sampleTypeOpticalBlank = 1;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[0].index = 7;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[0].scale = 20.0;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[0].offset =	1500.0;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[1].index = 8;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[1].scale = 0.5;
	rotorConfig_g->analyteCalcRecord[ALP].barcodeFactor[1].offset =	-35.0;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALP].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[ALT].analyteType = ALT;
	strcpy(rotorConfig_g->analyteCalcRecord[ALT].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[ALT].calculationIndex = 5;
	rotorConfig_g->analyteCalcRecord[ALT].printOrder = 10;
	rotorConfig_g->analyteCalcRecord[ALT].analyteCuvetteNumber = 20;
	rotorConfig_g->analyteCalcRecord[ALT].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[ALT].analyteDesiredPathLength = 5.0;
	rotorConfig_g->analyteCalcRecord[ALT].sampleTypeAnalyteBlank = 10;
	rotorConfig_g->analyteCalcRecord[ALT].sampleTypeOpticalBlank = 0;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[0].index = 9;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[0].scale = -50.0;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[0].offset =	-4000.0;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[1].index = 10;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[1].scale = 0.3;
	rotorConfig_g->analyteCalcRecord[ALT].barcodeFactor[1].offset =	-15.0;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALT].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[BUN].analyteType = BUN;
	strcpy(rotorConfig_g->analyteCalcRecord[BUN].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[BUN].calculationIndex = 6;
	rotorConfig_g->analyteCalcRecord[BUN].printOrder = 7;
	rotorConfig_g->analyteCalcRecord[BUN].analyteCuvetteNumber = 16;
	rotorConfig_g->analyteCalcRecord[BUN].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[BUN].analyteDesiredPathLength = 2.1;
	rotorConfig_g->analyteCalcRecord[BUN].sampleTypeAnalyteBlank = 10;
	rotorConfig_g->analyteCalcRecord[BUN].sampleTypeOpticalBlank = 0;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[0].index = 17;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[0].scale = -4.0;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[0].offset =	-200.0;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[1].index = 18;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[1].scale = 0.08;
	rotorConfig_g->analyteCalcRecord[BUN].barcodeFactor[1].offset =	-4.0;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[BUN].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[CRE].analyteType = CRE;
	strcpy(rotorConfig_g->analyteCalcRecord[CRE].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[CRE].calculationIndex = 7;
	rotorConfig_g->analyteCalcRecord[CRE].printOrder = 8;
	rotorConfig_g->analyteCalcRecord[CRE].analyteCuvetteNumber = 18;
	rotorConfig_g->analyteCalcRecord[CRE].analyteBlankCuvetteNumber = 22;
	rotorConfig_g->analyteCalcRecord[CRE].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[CRE].sampleTypeAnalyteBlank = 40;
	rotorConfig_g->analyteCalcRecord[CRE].sampleTypeOpticalBlank = 3;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[0].index = 27;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[0].scale = 0.3;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[0].offset =	25.0;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[1].index = 28;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[1].scale = 0.025;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[1].offset =	-0.8;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[2].index = 29;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[2].scale = 0.00006;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[2].offset =	-0.003;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[3].index = 30;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[3].scale = 0.00125;
	rotorConfig_g->analyteCalcRecord[CRE].barcodeFactor[3].offset =	-0.045;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CRE].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[GLU].analyteType = GLU;
	strcpy(rotorConfig_g->analyteCalcRecord[GLU].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[GLU].calculationIndex = 8;
	rotorConfig_g->analyteCalcRecord[GLU].printOrder = 5;
	rotorConfig_g->analyteCalcRecord[GLU].analyteCuvetteNumber = 21;
	rotorConfig_g->analyteCalcRecord[GLU].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[GLU].analyteDesiredPathLength = 1.7;
	rotorConfig_g->analyteCalcRecord[GLU].sampleTypeAnalyteBlank = 11;
	rotorConfig_g->analyteCalcRecord[GLU].sampleTypeOpticalBlank = 1;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[0].index = 31;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[0].scale = 3.0;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[0].offset =	200.0;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[1].index = 32;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[1].scale = 0.7;
	rotorConfig_g->analyteCalcRecord[GLU].barcodeFactor[1].offset =	-70.0;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLU].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[TP].analyteType = TP;
	strcpy(rotorConfig_g->analyteCalcRecord[TP].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[TP].calculationIndex = 9;
	rotorConfig_g->analyteCalcRecord[TP].printOrder = 14;
	rotorConfig_g->analyteCalcRecord[TP].analyteCuvetteNumber = 9;
	rotorConfig_g->analyteCalcRecord[TP].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[TP].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[TP].sampleTypeAnalyteBlank = 61;
	rotorConfig_g->analyteCalcRecord[TP].sampleTypeOpticalBlank = 2;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[0].index = 39;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[0].scale = 0.075;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[0].offset = 12.5;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[1].index = 40;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[1].scale = 0.05;
	rotorConfig_g->analyteCalcRecord[TP].barcodeFactor[1].offset = -4.0;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.lipemicLimit = 1000.0;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TP].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[TBIL].analyteType = TBIL;
	strcpy(rotorConfig_g->analyteCalcRecord[TBIL].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[TBIL].calculationIndex = 10;
	rotorConfig_g->analyteCalcRecord[TBIL].printOrder = 12;
	rotorConfig_g->analyteCalcRecord[TBIL].analyteCuvetteNumber = 12;
	rotorConfig_g->analyteCalcRecord[TBIL].analyteBlankCuvetteNumber = 26;
	rotorConfig_g->analyteCalcRecord[TBIL].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[TBIL].sampleTypeAnalyteBlank = 58;
	rotorConfig_g->analyteCalcRecord[TBIL].sampleTypeOpticalBlank = 3;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[0].index = 15;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[0].scale = 0.6;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[0].offset =	30.0;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[1].index = 16;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[1].scale = 0.03;
	rotorConfig_g->analyteCalcRecord[TBIL].barcodeFactor[1].offset =	-1.0;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TBIL].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[ALB_BCP].analyteType = ALB_BCP;
	strcpy(rotorConfig_g->analyteCalcRecord[ALB_BCP].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[ALB_BCP].calculationIndex = 11;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].printOrder = 13;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].analyteCuvetteNumber = 10;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].analyteDesiredPathLength = 3.1;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].sampleTypeAnalyteBlank = 12;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].sampleTypeOpticalBlank = 2;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[0].index = 1;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[0].scale = 0.1;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[0].offset = 7.0;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[1].index = 2;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[1].scale = 0.035;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[1].offset = -4.0;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[2].index = 4;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[2].scale = 0.1;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[2].offset = 5.0;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[3].index = 5;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[3].scale = 0.035;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].barcodeFactor[3].offset = -4.5;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[ALB_BCP].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[CA].analyteType = CA;
	strcpy(rotorConfig_g->analyteCalcRecord[CA].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[CA].calculationIndex = 12;
	rotorConfig_g->analyteCalcRecord[CA].printOrder = 6;
	rotorConfig_g->analyteCalcRecord[CA].analyteCuvetteNumber = 24;
	rotorConfig_g->analyteCalcRecord[CA].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[CA].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[CA].sampleTypeAnalyteBlank = 12;
	rotorConfig_g->analyteCalcRecord[CA].sampleTypeOpticalBlank = 2;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[0].index = 19;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[0].scale = 0.6;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[0].offset = 0.1;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[1].index = 20;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[1].scale = 0.35;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[1].offset = 10.0;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[2].index = 21;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[2].scale = 0.12;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[2].offset = 3.5;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[3].index = 22;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[3].scale = 0.125;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[3].offset = -1.0;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[4].index = 23;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[4].scale = 0.3;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[4].offset = -5.0;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[5].index = 24;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[5].scale = 0.3;
	rotorConfig_g->analyteCalcRecord[CA].barcodeFactor[5].offset = -8.0;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CA].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[AST].analyteType = AST;
	strcpy(rotorConfig_g->analyteCalcRecord[AST].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[AST].calculationIndex = 13;
	rotorConfig_g->analyteCalcRecord[AST].printOrder = 11;
	rotorConfig_g->analyteCalcRecord[AST].analyteCuvetteNumber = 19;
	rotorConfig_g->analyteCalcRecord[AST].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[AST].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[AST].sampleTypeAnalyteBlank = 10;
	rotorConfig_g->analyteCalcRecord[AST].sampleTypeOpticalBlank = 0;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[0].index = 13;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[0].scale = -50.0;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[0].offset =	-5000.0;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[1].index = 14;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[1].scale = 0.3;
	rotorConfig_g->analyteCalcRecord[AST].barcodeFactor[1].offset =	-20.0;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AST].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[K].analyteType = K;
	strcpy(rotorConfig_g->analyteCalcRecord[K].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[K].calculationIndex = 14;
	rotorConfig_g->analyteCalcRecord[K].printOrder = 2;
	rotorConfig_g->analyteCalcRecord[K].analyteCuvetteNumber = 23;
	rotorConfig_g->analyteCalcRecord[K].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[K].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[K].sampleTypeAnalyteBlank = 10;
	rotorConfig_g->analyteCalcRecord[K].sampleTypeOpticalBlank = 0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[0].index = 33;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[0].scale = 0.333;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[0].offset = -54.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[1].index = 34;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[1].scale = 0.05;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[1].offset = -4.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[2].index = 35;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[2].scale = 0.005;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[2].offset = 0.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[3].index = 1;		// factor not used this rotor
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[3].scale = 0.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[3].offset = 0.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[4].index = 1;		// factor not used this rotor
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[4].scale = 0.0;
	rotorConfig_g->analyteCalcRecord[K].barcodeFactor[4].offset = 0.0;
	rotorConfig_g->analyteCalcRecord[K].algorithmFlags[0] = 0;			// flag calc result decay type this rotor
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[K].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[GLOB].analyteType = GLOB;
	strcpy(rotorConfig_g->analyteCalcRecord[GLOB].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[GLOB].calculationIndex = 15;
	rotorConfig_g->analyteCalcRecord[GLOB].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[GLOB].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLOB].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLOB].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[GLOB].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[NA_1].analyteType = NA_1;
	strcpy(rotorConfig_g->analyteCalcRecord[NA_1].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[NA_1].calculationIndex = 16;
	rotorConfig_g->analyteCalcRecord[NA_1].printOrder = 1;
	rotorConfig_g->analyteCalcRecord[NA_1].analyteCuvetteNumber = 25;
	rotorConfig_g->analyteCalcRecord[NA_1].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[NA_1].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[NA_1].sampleTypeAnalyteBlank = 11;
	rotorConfig_g->analyteCalcRecord[NA_1].sampleTypeOpticalBlank = 1;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[0].index = 25;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[0].scale = 4.04;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[0].offset =	200.0;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[1].index = 26;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[1].scale = 0.5;
	rotorConfig_g->analyteCalcRecord[NA_1].barcodeFactor[1].offset =	35.0;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[NA_1].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[TCO2].analyteType = TCO2;
	strcpy(rotorConfig_g->analyteCalcRecord[TCO2].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[TCO2].calculationIndex = 17;
	rotorConfig_g->analyteCalcRecord[TCO2].printOrder = 3;
	rotorConfig_g->analyteCalcRecord[TCO2].analyteCuvetteNumber = 14;
	rotorConfig_g->analyteCalcRecord[TCO2].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[TCO2].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[TCO2].sampleTypeAnalyteBlank = 10;
	rotorConfig_g->analyteCalcRecord[TCO2].sampleTypeOpticalBlank = 0;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[0].index = 36;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[0].scale = 0.0505;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[0].offset = 1.0;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[1].index = 37;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[1].scale = 0.02525;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[1].offset = 0.5;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[2].index = 38;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[2].scale = 0.0;
	rotorConfig_g->analyteCalcRecord[TCO2].barcodeFactor[2].offset = 0.0;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[TCO2].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[CL_1].analyteType = CL_1;
	strcpy(rotorConfig_g->analyteCalcRecord[CL_1].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[CL_1].calculationIndex = 18;
	rotorConfig_g->analyteCalcRecord[CL_1].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[CL_1].analyteCuvetteNumber = 17;
	rotorConfig_g->analyteCalcRecord[CL_1].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[CL_1].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[CL_1].sampleTypeAnalyteBlank = 11;
	rotorConfig_g->analyteCalcRecord[CL_1].sampleTypeOpticalBlank = 1;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[0].index = 11;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[0].scale = 3.535;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[0].offset =	350.0;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[1].index = 12;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[1].scale = 0.808;
	rotorConfig_g->analyteCalcRecord[CL_1].barcodeFactor[1].offset =	-70.0;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_1].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[CL_2].analyteType = CL_2;
	strcpy(rotorConfig_g->analyteCalcRecord[CL_2].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[CL_2].calculationIndex = 19;
	rotorConfig_g->analyteCalcRecord[CL_2].printOrder = 4;
	rotorConfig_g->analyteCalcRecord[CL_2].analyteCuvetteNumber = 15;
	rotorConfig_g->analyteCalcRecord[CL_2].analyteBlankCuvetteNumber = 27;
	rotorConfig_g->analyteCalcRecord[CL_2].analyteDesiredPathLength = 4.3;
	rotorConfig_g->analyteCalcRecord[CL_2].sampleTypeAnalyteBlank = 11;
	rotorConfig_g->analyteCalcRecord[CL_2].sampleTypeOpticalBlank = 1;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[0].index = 11;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[0].scale = 3.535;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[0].offset =	350.0;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[1].index = 12;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[1].scale = 0.808;
	rotorConfig_g->analyteCalcRecord[CL_2].barcodeFactor[1].offset =	-70.0;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.hemolyzedLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.lipemicLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.ictericLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.highDynamicRange = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[CL_2].rotorFactorsLimits.highSystemLimit = NOVAL_NEG_FLOAT;

	rotorConfig_g->analyteCalcRecord[AG].analyteType = AG;
	strcpy(rotorConfig_g->analyteCalcRecord[AG].versionNumber, algVersion);
	rotorConfig_g->analyteCalcRecord[AG].calculationIndex = 20;
	rotorConfig_g->analyteCalcRecord[AG].printOrder = 255;
	rotorConfig_g->analyteCalcRecord[AG].rotorFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
	rotorConfig_g->analyteCalcRecord[AG].rotorFactorsLimits.lowDynamicRange = 0.0;
	rotorConfig_g->analyteCalcRecord[AG].rotorFactorsLimits.highDynamicRange = 100.0;
	rotorConfig_g->analyteCalcRecord[AG].rotorFactorsLimits.highSystemLimit = NOVAL_POS_FLOAT;

  */

}

