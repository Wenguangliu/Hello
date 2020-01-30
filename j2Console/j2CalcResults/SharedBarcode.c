// ********************************************************************
//
// FILENAME:  
// 
// 		SharedBarcode.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Shared factory file data definition and test load code for algorithm testing
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <string.h>


#include "BarcodeData.h"


static BarcodeData_t	barcodeData;
BarcodeData_t *			barcodeData_g = &barcodeData;


// ********************************************************************
// FUNCTION:
//
//		LoadTestBarcode() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load test data to structure for algorithm testing 
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

void LoadTestBarcode(void)
{
	unsigned	i;

	char	mfgCode[] = {"AA4"};

// DLR
// set to current analyteFactors from crunch template

/*	used for original gold file testing

	float	analyteFactors[MAX_BC_FACTORS] =
	{
		27.0,		// Albumin 550 Slope
		80.0,		// Albumin 550  Int.
		0.0,		// Albumin cat factor
		27.0,		// Albumin 630 Slope
		74.0,		// Albumin 630  Int.
		0.0,		// Albumin dog factor
		56.0,		// ALP Slope
		50.0,		// ALP Intercept
		47.0,		// ALT Slope
		99.0,		// ALT Intercept
		76.0,		// CL- Slope
		90.0,		// CL- Intercept
		50.0,		// AST Slope
		90.0,		// AST Intercept
		46.0,		// TBIL Slope
		47.0,		// TBIL Intercept
		55.0,		// BUN Slope
		60.0,		// BUN Intercept
		32.0,		// CA++ 405 A1
		42.0,		// CA++ 405 A2
		50.0,		// CA++ 405 A3
		47.0,		// CA++ 600 A1
		51.0,		// CA++ 600 A2
		54.0,		// CA++ 600 A3
		43.0,		// NA+ Slope
		17.0,		// NA+ Intercept
		46.0,		// CRE Slope
		26.0,		// CRE Intercept
		50.0,		// CRE Curve
		36.0,		// CRE CurveOffset
		42.0,		// GLU Slope
		90.0,		// GLU Intercept
		74.0,		// K+ (Enz) Slope
		52.0,		// K+ (Enz) Intercept
		0.0,		// K+ (Enz) Decay
		52.0,		// tCO2 Slope
		13.0,		// tCO2 Intercept
		0.0,		// not used
		46.0,		// TP Slope
		73.0,		// TP Intercept
		39.0,		// RQC Base Abs
		70.0,		// RQC Low Limit
		14.0,		// RQC Decay Const
		50.0		// Diluent Absorbance
	};
*/

// DLR
// set to current analyteFactors from crunch template

//	used for original rotor 52 ROC file testing "gold.1613.rad" file?

	float	analyteFactors[MAX_BC_FACTORS] =
	{
		17.0,		// 1:  Albumin 550 Slope
		72.0,		// 2:  Albumin 550  Int.
		0.0,		// 3:  Albumin cat factor
		22.0,		// 4:  Albumin 630 Slope
		65.0,		// 5:  Albumin 630  Int.
		0.0,		// 6:  Albumin dog factor
		56.0,		// 7:  ALP Slope
		68.0,		// 8:  ALP Intercept
		47.0,		// 9:  ALT Slope
		49.0,		// 10: ALT Intercept
		43.0,		// 11: CL- Slope
		82.0,		// 12: CL- Intercept
		50.0,		// 13: AST Slope
		77.0,		// 14: AST Intercept
		46.0,		// 15: TBIL Slope
		47.0,		// 16: TBIL Intercept
		75.0,		// 17: BUN Slope
		61.0,		// 18: BUN Intercept
		29.0,		// 19: CA++ 405 A1
		45.0,		// 20: CA++ 405 A2
		50.0,		// 21: CA++ 405 A3
		39.0,		// 22: CA++ 600 A1
		36.0,		// 23: CA++ 600 A2
		37.0,		// 24: CA++ 600 A3
		56.0,		// 25: NA+ Slope
		52.0,		// 26: NA+ Intercept
		45.0,		// 27: CRE Slope
		34.0,		// 28: CRE Intercept
		50.0,		// 29: CRE Curve
		36.0,		// 30: CRE CurveOffset
		42.0,		// 31: GLU Slope
		81.0,		// 32: GLU Intercept
		74.0,		// 33: K+ (Enz) Slope
		36.0,		// 34: K+ (Enz) Intercept
		0.0,		// 35: K+ (Enz) Decay
		56.0,		// 36: tCO2 Slope
		14.0,		// 37: tCO2 Intercept
		0.0,		// 38: not used
		47.0,		// 39: TP Slope
		72.0,		// 40: TP Intercept
		43.0,		// 41: RQC Base Abs
		70.0,		// 42: RQC Low Limit
		14.0,		// 43: RQC Decay Const
		50.0		// 44: Diluent Absorbance
	};
//	


/*	used for original rotor 40 ROC file testing

	float	analyteFactors[MAX_BC_FACTORS] =
	{
		84.0,		// 1:  Albumin 550 Slope
		0.0,		// 2:  Albumin 550  Int.
		29.0,		// 3:  Albumin cat factor
		78.0,		// 4:  Albumin 630 Slope
		92.0,		// 5:  Albumin 630  Int.
		42.0,		// 6:  Albumin dog factor
		22.0,		// 7:  ALP Slope
		89.0,		// 8:  ALP Intercept
		58.0,		// 9:  ALT Slope
		47.0,		// 10: ALT Intercept

		43.0,		// 11: CL- Slope
		82.0,		// 12: CL- Intercept
		50.0,		// 13: AST Slope
		77.0,		// 14: AST Intercept
		46.0,		// 15: TBIL Slope
		47.0,		// 16: TBIL Intercept
		75.0,		// 17: BUN Slope
		61.0,		// 18: BUN Intercept
		29.0,		// 19: CA++ 405 A1
		45.0,		// 20: CA++ 405 A2

		50.0,		// 21: CA++ 405 A3
		38.0,		// 22: CA++ 600 A1
		93.0,		// 23: CA++ 600 A2
		12.0,		// 24: CA++ 600 A3

		56.0,		// 25: NA+ Slope
		52.0,		// 26: NA+ Intercept
		45.0,		// 27: CRE Slope
		34.0,		// 28: CRE Intercept
		50.0,		// 29: CRE Curve
		36.0,		// 30: CRE CurveOffset
		42.0,		// 31: GLU Slope
		81.0,		// 32: GLU Intercept
		74.0,		// 33: K+ (Enz) Slope
		36.0,		// 34: K+ (Enz) Intercept
		0.0,		// 35: K+ (Enz) Decay
		56.0,		// 36: tCO2 Slope
		14.0,		// 37: tCO2 Intercept
		0.0,		// 38: not used
		47.0,		// 39: TP Slope
		72.0,		// 40: TP Intercept
		43.0,		// 41: RQC Base Abs
		70.0,		// 42: RQC Low Limit
		14.0,		// 43: RQC Decay Const
		50.0		// 44: Diluent Absorbance
	};
*/
	
	// DLR
// set rotorId to 1 to test rotor 1 test abs calcs, else set to 52 for alg gold file regression testing

//	barcodeData_g->rotorId     = 1;

	barcodeData_g->rotorId     = 52;
//	barcodeData_g->rotorId     = 40;

// DLR
// set rotorFormat to 1 for rotor 1 test

//	barcodeData_g->rotorFormat = 1;

	barcodeData_g->rotorFormat = 2;
//	barcodeData_g->rotorFormat = 4;

	barcodeData_g->rotorExpiryMonth = 12;
	barcodeData_g->rotorExpiryDayOfMonth = 9;
	barcodeData_g->rotorExpiryYear = 3;

// DLR
// set rotorAgeInDays to value in current crunch template

//	barcodeData_g->rotorAgeInDays = 39.0;	// used for original gold file testing

	barcodeData_g->rotorAgeInDays = 120.0;	// used for rotor 52 ROC file testing
//	barcodeData_g->rotorAgeInDays = 120.0;	// used for rotor 40 ROC file testing
//	barcodeData_g->rotorAgeInDays = 12.0;	// used for rotor 54 ROC file testing
//	barcodeData_g->rotorAgeInDays = 31.0;	// used for rotor 52 ROC file testing

	barcodeData_g->rotorMfgLot.lsdOfYear = 3;
	barcodeData_g->rotorMfgLot.weekOfYear = 24;
	barcodeData_g->rotorMfgLot.dayOfWeek = 1;
	strcpy(barcodeData_g->rotorMfgLot.code, mfgCode);

	for (i = 0; i < MAX_BC_FACTORS; i++)
	{
		barcodeData_g->rotorAnalyteFactor[i] = analyteFactors[i];
	}
}
