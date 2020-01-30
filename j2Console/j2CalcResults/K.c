// ********************************************************************
//
// FILENAME:  
// 
// 		K.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		K calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <windows.h>
#include <math.h>				// required standard C library 
#include <time.h>				// required standard C library 
#include "CommonAlgFuncs.h"
#include "SharedRaw.h"
#include "SharedBarcode.h"


#define K_SLOPE			0
#define K_ANCHOR		1
#define K_DECAY			2
#define K_SLOPE_DECAY	3
#define K_ANCHOR_DECAY	4

#define	CALC_RESULT_DECAY		0

#define	ROTOR_DECAY_MULT_1		1.58198		// 1.58198 changes the BC factor for decay from total decay to the decay at one time constant 
#define	ROTOR_AGE_DIVISOR_1		20.0
#define	ROTOR_AGE_DIVISOR_2		5.0


// K+ lot correction constants

#define JULIAN_01_03_2013		2456296
#define JULIAN_03_04_2013		2456356
#define JULIAN_01_09_2013		2456302		
#define JULIAN_03_15_2013		2456367		
#define JULIAN_03_19_2013		2456371		
#define JULIAN_01_10_2013		2456303		
#define JULIAN_03_23_2013		2456375		
#define JULIAN_03_26_2013		2456378		
#define JULIAN_03_30_2013		2456382		
#define JULIAN_01_28_2013		2456321		

#define K_LOT_RESULT_DECAY			0.4
#define K_LOT_ROTOR_AGE_DIVISOR		30.0


// ********************************************************************
// FUNCTION:
//
//		KResultLotCorrectionCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		K calculation algorithm 
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

unsigned KResultLotCorrectionCheck( void )
{
	//time_t			t;
	//struct tm		*currentDate;
    SYSTEMTIME      currentDate;
	unsigned short	currentYear;
	int				lsdOfCurrentYear;
	unsigned short	mfgYear;
	unsigned		mfgJulian;
	unsigned char	rotorId;							
      

	// create Julian barcode / lot manufacturing date for K+ correction check compare
	// Get the current date, and extract the current year.

//	t = time( 0 );
//	currentDate = gmtime( &t );
//	currentYear = currentDate->tm_year + 1900;
    GetLocalTime(&currentDate);
    currentYear = currentDate.wYear;
      
	// Determine the manufacturing year.

	lsdOfCurrentYear = currentYear % 10;
	mfgYear = currentYear - ( lsdOfCurrentYear - barcodeData_g->rotorMfgLot.lsdOfYear );

	if ( lsdOfCurrentYear < barcodeData_g->rotorMfgLot.lsdOfYear )
	{
		mfgYear -= 10;
	}

	// Get the Julian date for January 1 of the manufacturing year.

	mfgJulian = JulianDate( 1, 1, mfgYear );
      
	// Add the manufacturing week and day-of-week, and subtract the day of week
	// that the manufacturing year started on.
	// Subtract 1 from weekOfYear so we get 0-based value.

	mfgJulian += (unsigned)(barcodeData_g->rotorMfgLot.weekOfYear - 1) * 7 + (unsigned)barcodeData_g->rotorMfgLot.dayOfWeek - ( ( mfgJulian % 7 ) + 1 );


	// get this rotor ID for compare / barcode / lot manufacturing / date for K+ correction check

	rotorId	= barcodeData_g->rotorId;


	// Check if any following lots to correct based on rotor ID and MFG Date match

	if ( 
	
	   	    ( ( rotorId == 27 ) && ( mfgJulian == JULIAN_01_03_2013 ) )

	|| 

	        ( ( rotorId == 42 ) && ( mfgJulian == JULIAN_03_04_2013 ) )

	|| 

	        ( ( rotorId == 43 ) && ( ( mfgJulian == JULIAN_01_09_2013 ) || ( mfgJulian == JULIAN_03_15_2013 ) || ( mfgJulian == JULIAN_03_19_2013 ) ) )

	|| 

	        ( ( rotorId == 44 ) && ( mfgJulian == JULIAN_01_10_2013 ) )


	|| 

	        ( ( rotorId == 49 ) && ( mfgJulian == JULIAN_03_15_2013 ) )

	||

	        ( ( rotorId == 52 ) && ( ( mfgJulian > JULIAN_01_03_2013 ) && ( mfgJulian < JULIAN_03_23_2013 ) ) )

	||

	        ( ( rotorId == 52 ) && ( ( mfgJulian > JULIAN_03_26_2013 ) && ( mfgJulian < JULIAN_03_30_2013 ) ) )

	|| 

	        ( ( rotorId == 64 ) && ( mfgJulian == JULIAN_01_28_2013 ) )

	    )
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;
	}
}


// ********************************************************************
// FUNCTION:
//
//		CalcK() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		K calculation algorithm 
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

void	CalcK(void)
{
	float				kResult;
	unsigned long		kError;

	unsigned char		analyteType;
	unsigned char		kNumSlopePoints;
	float				kSlopeRate;
	float				kFalseRate;
	float				kCorrectedSlopeRate;
	float				kResultSlope;
	float				kResultAnchor;
	float				kResultDecay;
	float				kResultSlopeDecay;
	float				kResultAnchorDecay;
	float				expRotorAgeDecayFactor;

	RateAlgRecord_t			*rateAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// init K result & flags

	analyteType = K;

	kResult =  NOVAL_NEG_FLOAT;
	kError =  NO_ANALYTE_ERROR;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, RATE);
	
	rateAlgPtr = &rateConfig_g.rateAlgRecord;		

	// process rate standard slope algorithm

	kSlopeRate = ProcessStdRateSlope(analyteType, rateAlgPtr, factorsLimitsPtr, &kFalseRate, &kNumSlopePoints, &kError);

	if (kSlopeRate != NOVAL_NEG_FLOAT)
	{


		// IMV - K paragraph 4 store

		StoreAlgParagraphNumImv( K, 4.0 );

		// end IMV


		// Perform temperature correction on the slope rate 

		kSlopeRate *= CalcTemperatureCorrection(rateAlgPtr->rateCalcRecord.arrhTemperatureCorrectionFactor);

		// calc corrected slope rate by BFD
			
		kCorrectedSlopeRate = BfdCalAdjust(kSlopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex);		


		// IMV - K paragraph 5 store

		StoreAlgParagraphNumImv( K, 5.0 );

		// end IMV


		// get result calc bar code factors from ROC file
		 
		kResultSlope       = GetBarcodeParameter(analyteType, K_SLOPE);
		kResultAnchor      = GetBarcodeParameter(analyteType, K_ANCHOR);
		kResultDecay       = GetBarcodeParameter(analyteType, K_DECAY);
		kResultSlopeDecay  = GetBarcodeParameter(analyteType, K_SLOPE_DECAY);
		kResultAnchorDecay = GetBarcodeParameter(analyteType, K_ANCHOR_DECAY);

		// check result flag set in ROC file bar code factors for type of result calc

		if (rotorConfig_g->analyteCalcRecord[K].algorithmFlags[CALC_RESULT_DECAY])
		{
			// Rotor 42, 43, 44 and 49 result calc with decay correction

			expRotorAgeDecayFactor = CalcExponentialRotorAgeDecay(ROTOR_AGE_DIVISOR_2);

			kResult = (kCorrectedSlopeRate * (kResultSlope + ((kResultSlopeDecay - kResultSlope) * expRotorAgeDecayFactor))) 
				   	  + (kResultAnchor + ((kResultAnchorDecay - kResultAnchor) * expRotorAgeDecayFactor)); 
		}
		else
		{
			// All other Rotors result calc with decay correction

			expRotorAgeDecayFactor = CalcExponentialRotorAgeDecay(ROTOR_AGE_DIVISOR_1);

			kResult = (kCorrectedSlopeRate * kResultSlope) + kResultAnchor - (kResultDecay * ROTOR_DECAY_MULT_1 * expRotorAgeDecayFactor);
		}


		// Adjust K result only for selected rotor lots

		if ( KResultLotCorrectionCheck() == TRUE )
		{
			expRotorAgeDecayFactor = CalcExponentialRotorAgeDecay(K_LOT_ROTOR_AGE_DIVISOR);

			kResult -= ( K_LOT_RESULT_DECAY * expRotorAgeDecayFactor );
		}


		// IMV - result no decay store / K paragraph 7 store / K standard rate calc params store

		StoreIntermediateValue( analyteType, IMV_RESULT_NO_DECAY, IMV_DIRECT_SET_METHOD, (kCorrectedSlopeRate * kResultSlope) + kResultAnchor );

		StoreAlgParagraphNumImv( K, 7.0 );

		StoreRateResultCalcParamsImv( analyteType );

		// end IMV


		// Check standard result for within system & dynamic range limits

		ResultLimitsCheck(analyteType, kResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &kError);
	}


	// IMV - K paragraph 8 store

	StoreAlgParagraphNumImv( K, 8.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, kResult, kError, factorsLimitsPtr);


#ifndef UNDER_CE
	printf("K invoked\n");
#endif
}
