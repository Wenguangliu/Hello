// ********************************************************************
//
// FILENAME:  
// 
// 		CommonAlgFuncs.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Common Algorithm functions for Analyte Algorithms
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include <math.h>				// required standard C library 
#include <string.h>
#include <time.h>				// required standard C library 

#include "Statistics.h"
#include "Algorithms.h"			// required project file headers
#include "SharedRaw.h"
#include "SharedRoc.h"
#include "SharedFactory.h"
#include "SharedCal.h"
#include "SharedBarcode.h"

#include "SystemCheckData.h"
#include "SampleBlankData.h"
#include "RqcData.h"
#include "IqcData.h"
#include "EndpointData.h"
#include "RateData.h"
#include "PosNegData.h"
#include "T4Data.h"
#include "DerivedData.h"
#include "Calc2CuvData.h"


// IMV - external function calls

#include "Imv.h"

// end IMV


// flash defs

#define	FLASH_0							0			// only flash 0 for min lamp intensity checking


// time conversion defs

#define	MILLISECONDS_PER_MINUTE					60000.0	// conversion factor for engineTime to minutes
#define	ENGINE_MILLISECONDS_PER_BIT				5.0			// microseconds per bit in engineTime
#define ENGINE_TIME_MINUTE_CONVERT_MULT			(ENGINE_MILLISECONDS_PER_BIT / MILLISECONDS_PER_MINUTE)

// sample transmission definitions/limits

#define MAX_TRANSMISSION_DIVISOR			1000000.0

#define MIN_SAMPLE_TRIM_FLASHES_ALLOWED		9

#define ADC_REFERENCE_OFFSET_LIMIT			0xffff

// standard calc alg definitions

#define STANDARD_SLOPE_INT				0		// use bfd, standard barcode slope int for endpoint & rate result calc
#define BASE_FINE_SLOPE_INT				1		// use bfd, base/fine barcode computed slope int for endpoint result calc
#define PATIENT_TYPE_SLOPE_INT			2		// use bfd, patient type based barcode slope int for endpoint result calc
#define BFD_ICTERIC_SLOPE_INT			3		// use bfd, icteric corrected average and standard barcode slope int for rate result calc
#define BASE_FINE_EXP					4		// use bfd, base/fine barcode computed exponential result calc (A0, A1, A2 terms)  
#define STANDARD_QUADRATIC				5		// use bfd, standard quadratic equation (A0, A1, A2 terms) for A0*rate*rate + A1*rate + A2
#define SIGMOID_LOGIT_LOG4				6		// use bfd, sigmoid logit/log4 result calc

#define	STD_SLOPE1_IDX					0
#define	STD_INTERCEPT1_IDX				1
#define	STD_SLOPE2_IDX					2
#define	STD_INTERCEPT2_IDX				3

#define	SLOPE_BASE_IDX					0
#define	INTERCEPT_BASE_IDX				1
#define	SLOPE_FINE_IDX					2
#define	INTERCEPT_FINE_IDX				3

#define DOG_SLOPE_IDX					0
#define DOG_INTERCEPT_IDX				1
#define CAT_SLOPE_IDX					2
#define CAT_INTERCEPT_IDX				3
#define OTHER_SLOPE_IDX					4
#define OTHER_INTERCEPT_IDX				5
#define HORSE_SLOPE_IDX					6
#define HORSE_INTERCEPT_IDX				7

#define PARABOLA1_A1					0
#define PARABOLA1_A2					1
#define PARABOLA1_A3					2
#define PARABOLA2_A1					3
#define PARABOLA2_A2					4
#define PARABOLA2_A3					5

#define A0_BASE_IDX						0
#define A0_FINE_IDX						1
#define A1_BASE_IDX						2
#define A1_FINE_IDX						3
#define A2_BASE_IDX						4
#define A2_FINE_IDX						5

#define POS_NEG_RATIO_CUTOFF_IDX		0
#define POS_NEG_WB_OFFSET_IDX			1
#define POS_NEG_CONJ_TYPE_IDX			2

#define QUADRATIC_A0_IDX				0
#define QUADRATIC_A1_IDX				1
#define QUADRATIC_A2_IDX				2

#define	SIGMOID_LOGIT_LOG4_SLOPE_BASE_IDX		0
#define	SIGMOID_LOGIT_LOG4_INTERCEPT_BASE_IDX	1
#define	SIGMOID_LOGIT_LOG4_SLOPE_FINE_IDX		2
#define	SIGMOID_LOGIT_LOG4_INTERCEPT_FINE_IDX	3
#define	SIGMOID_LOGIT_LOG4_TOP_BASE_IDX			4
#define	SIGMOID_LOGIT_LOG4_TOP_FINE_IDX			5
#define	SIGMOID_LOGIT_LOG4_BOTTOM_BASE_IDX		6
#define	SIGMOID_LOGIT_LOG4_BOTTOM_FINE_IDX		7

// miscellaneous defs

#define SLOPE_CALC_POINTS_TOGGLE		3

#define	CELSIUS_TO_KELVIN				273.0

#define COMPARE_LT_LIMIT				TRUE

#define	AVG1_IDX						0
#define	AVG2_IDX						1

#define	RESULT_COMPARE_RATIO			1
#define	RESULT_COMPARE_DELTA			2

#define ENDPOINT_2PARABOLA_METHOD		1
#define ENDPOINT_2AVG_CALC_METHOD		2

#define	WINDOW_LINEAR_REGRESSION_METHOD		0
#define DELTA_SUM_MULTIPLIER_METHOD			1


#define NO_ENDOGENOUS_RESULT_CORRECTION		0
#define HEMOLYZED_RESULT_CORRECTION			1
#define LIPEMIC_RESULT_CORRECTION			2
#define ICTERIC_RESULT_CORRECTION			3

#define ENDOGENOUS_RESULT_CORRECTION_IDX	0


#define ICTERIC_BLANK_FACTOR_IDX			0
#define ICTERIC_ANALYTE_FACTOR_1_IDX		1
#define ICTERIC_ANALYTE_FACTOR_2_IDX		2


#define ENDOGENOUS_NO_ERROR		0		// below endogenous limit 1 so no error                : no  pre char,  <result>  ,  no post char  
#define ENDOGENOUS_RESULT		1		// above endogenous limit 1 & below endogenous limit 2 : no  pre char,  <result>  , 'H', 'L' or 'I' post char
#define ENDOGENOUS_LT_RESULT	2		// above endogenous limit 2 & below endogenous limit 3 : '<' pre char,  <result>  , 'H', 'L' or 'I' post char
#define ENDOGENOUS_GT_RESULT	3		// above endogenous limit 2 & below endogenous limit 3 : '>' pre char,  <result>  , 'H', 'L' or 'I' post char
#define ENDOGENOUS_SUPPRESSED	4		// above endogenous limit 3                            : over-write     <result>  with "HEM", "LIP" or "ICT", no pre char, no post char  




/* **** Special Constants to correct for Dog calibration issues for CA analyte **** */

#define	CORRECTION_A1					   -0.015	/* Correction Square Coefficient  */
#define	CORRECTION_A2						0.4616	/* Correction Linear Coefficient  */
#define	CORRECTION_A3					   -2.8615	/* Correction Intercept Coefficient  */


#define	MAX_NA_RATE_CORRECTION_LEVELS		5		// Maximum NA_1 and NA_2 stability rate correction levels

#define	JULIAN_11_05_2017					2458062	// Julian time compute for 11/05/2017 beginning of range to check NA rotor lots manufactured
#define	JULIAN_03_05_2018					2458182	// Julian time compute for 11/05/2017 beginning of range to check NA rotor lots manufactured
#define NA_IDENTIFIED_LOT_OFFSET_ADJUST		2.0		// Offset adjustment to subract from NA_1 or NA_2 recovery for indentified lots in timeframe


#define SEX_SPECIES_MASK	0x0e000000		// rotorSystemFlags sex/species mask


typedef struct
{
	unsigned		slopeIdx;
	unsigned		interceptIdx;
	unsigned long	systemFlags;
} PatientTypeBarcodeCorrection_t;


typedef struct
{
	float				beadMixCheckLimit;
	float				beadMissingCheckLimit;
	float				distributionCheckLimit;
	float				startAbsLimit;
} BlankLimits_t;


CommonFactorsLimits_t	factorsLimits_m;
BlankLimits_t			blankLimits_m;


// algorithm analyte configuration data

SystemCheckConfig_t		systemCheckConfig_g;
SampleBlankConfig_t		sampleBlankConfig_g;
RqcConfig_t				rqcConfig_g;
IqcConfig_t				iqcConfig_g;
EndpointConfig_t		endpointConfig_g;
RateConfig_t			rateConfig_g;
PosNegConfig_t			posNegConfig_g;
T4Config_t				t4Config_g;
DerivedConfig_t			derivedConfig_g;
Calc2CuvConfig_t		calc2CuvConfig_g;


// xref table to match legacy bit set for bead missing & distribution check cuvette errors for TRB3 program

const unsigned long beadMissingDistCheckFlagsByCuvette_m[NUM_CUVETTES] =
{
	0x00000001,		// bit mask: cuvette 0
	0x00002000,		// bit mask: cuvette 1
	0x04000000,		// bit mask: cuvette 2
	0x00000200,		// bit mask: cuvette 3
	0x00400000,		// bit mask: cuvette 4
	0x00000020,		// bit mask: cuvette 5
	0x00040000,		// bit mask: cuvette 6
	0x00000002,		// bit mask: cuvette 7
	0x00004000,		// bit mask: cuvette 8
	0x08000000,		// bit mask: cuvette 9
	0x00000400,		// bit mask: cuvette 10
	0x00800000,		// bit mask: cuvette 11
	0x00000040,		// bit mask: cuvette 12
	0x00080000,		// bit mask: cuvette 13
	0x00000004,		// bit mask: cuvette 14
	0x00008000,		// bit mask: cuvette 15
	0x10000000,		// bit mask: cuvette 16
	0x00000800,		// bit mask: cuvette 17
	0x01000000,		// bit mask: cuvette 18
	0x00000080,		// bit mask: cuvette 19
	0x00100000,		// bit mask: cuvette 20
	0x00000008,		// bit mask: cuvette 21
	0x00010000,		// bit mask: cuvette 22
	0x20000000,		// bit mask: cuvette 23
	0x00001000,		// bit mask: cuvette 24
	0x02000000,		// bit mask: cuvette 25
	0x00000100,		// bit mask: cuvette 26
	0x00200000,		// bit mask: cuvette 27
	0x00000010,		// bit mask: cuvette 28
	0x00020000,		// bit mask: cuvette 29
};


// xref table to match wavelength DAC trim & DAC CV and Offset Level & Offset SD error flags for TRB3 program

const unsigned short wavelengthErrorFlags_g[NUM_WAVELENGTHS] =
{
	0x0100,		// bit mask 0: 340
	0x0080,		// bit mask 1: 405
	0x0040,		// bit mask 2: 467
	0x0020,		// bit mask 3: 500
	0x0004,		// bit mask 4: 515
	0x0010,		// bit mask 5: 550
	0x0008,		// bit mask 6: 600
	0x0002,		// bit mask 7: 630
	0x0001,		// bit mask 8: 850
	0x0200,		// bit mask 9: WHITE
};


// cuvette hole path length correction table

const float		cuvettePathLengths_g[NUM_CUVETTES] = 
{
	5.0,		// cuvette 0: not used, SYSTEM - BLOCKED (BLACK_OFFSETS)
	0.4,		// cuvette 1: SYSTEM - SAMPLE CHECK 
	5.0,		// cuvette 2: not used, SYSTEM - DUMP
	5.0,		// cuvette 3: used    , RQC
	5.0,		// cuvette 4: used    , IQC_A
	4.3,		// cuvette 5: used    , IQC_B
	4.3,		// cuvette 6: used    , SYSTEM_CHECK
	5.0,		// cuvette 7: not used, SYSTEM - DUMP
	4.3,		// cuvette 8
	4.3,		// cuvette 9
	3.1,		// cuvette 10
	3.1,		// cuvette 11: used   , SYSTEM_CHECK
	4.3,		// cuvette 12
	2.1,		// cuvette 13
	4.3,		// cuvette 14
	4.3,		// cuvette 15
	2.1,		// cuvette 16
	4.3,		// cuvette 17
	4.3,		// cuvette 18
	4.3,		// cuvette 19
	5.0,		// cuvette 20
	1.7,		// cuvette 21
	4.3,		// cuvette 22
	4.3,		// cuvette 23
	4.3,		// cuvette 24
	4.3,		// cuvette 25
	4.3,		// cuvette 26
	5.0,		// cuvette 27
	4.3,		// cuvette 28: used    , SYSTEM_CHECK
	5.0			// cuvette 29: not used, SYSTEM - HOLE (OPTICAL_BLANK)
};	


// ********************************************************************
// FUNCTION:
//
//		ConvertEngineTimeToMinutes() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Convert engine time in 0.8 microseconds per bit to minutes  
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

float ConvertEngineTimeToMinutes(unsigned long	engineTime)
{
	// each tick of engineTime value is equivalent to 0.8 microseconds of real time
	
//	return (((float)engineTime * ENGINE_MICROSECONDS_PER_BIT) / MICROSECONDS_PER_MINUTE);
	return (float)engineTime * ENGINE_TIME_MINUTE_CONVERT_MULT;
}

// ********************************************************************
// FUNCTION:
//
//		GetSampleFlashTime() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get sample flash time for rate algorithm calculation  
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

float GetSampleFlashTime(unsigned char sampleType, unsigned char samplePass)
{
	unsigned long	engineTime;

	// get sample flash time in minutes sychronized to flash 0 of each flash set 

	engineTime = rotorRawReadings_g->rotorAnalyteSamples[sampleType][samplePass-1][FLASH_0].engineTime;		 

	return	ConvertEngineTimeToMinutes(engineTime);
}


// ********************************************************************
// FUNCTION:
//
//		RqcAbusedCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set analyte error flag for rotor RQC abused  
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

unsigned long RqcAbusedCheck(void)
{
	// check flag that CalcRqc set

	if (rotorResults_g->rotorInformationResults.rotorRqcAbused)
	{
		return RQC_ERROR;
	}
	
	return NO_ANALYTE_ERROR;	 
}


// ********************************************************************
// FUNCTION:
//
//		SetAlgorithmErrorNumber() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set for 1st system rotor error number found in algorithm  
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

unsigned short SetAlgorithmErrorNumber(unsigned rotorErrorNumberIndex)
{
	if (rotorResults_g->rotorInformationResults.rotorErrorNumberIndex == rotorErrorNumberIndex)
	{
		// no rotor error number set in this algorithm since index was not bumped

		return SERR_NONE;
	}

	// set for 1st rotor error number set for this algorithm

	return rotorResults_g->rotorInformationResults.rotorErrorNumber[rotorErrorNumberIndex];
}


// ********************************************************************
// FUNCTION:
//
//		ReportSystemError() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Report rotor system Error in rotor results record  
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

void ReportSystemError(unsigned long rotorSystemFlags, unsigned short rotorErrorNumber)
{
	rotorResults_g->rotorInformationResults.rotorSystemFlags |= rotorSystemFlags;

	if ((rotorResults_g->rotorInformationResults.rotorErrorNumberIndex < MAX_ROTOR_ERROR_NUMBERS) && (rotorErrorNumber != SERR_NONE))
	{
		rotorResults_g->rotorInformationResults.rotorErrorNumber[rotorResults_g->rotorInformationResults.rotorErrorNumberIndex++] = rotorErrorNumber;
	}
}

 
// ********************************************************************
// FUNCTION:
//
//		ProcessSystemCalcErrors() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process generic system errors for flash range, divide by 0, illegal log parameters &
//		report errors for rotor results  
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

unsigned ProcessSystemCalcErrors(unsigned long systemCalcError)
{
	if (systemCalcError & FLASH_RANGE_ERROR)
	{
		ReportSystemError(SYS_FLASH_RANGE, SERR_FLASH_RANGE); 
		return	TRUE;
	}
	else if	(systemCalcError & DIVIDE_BY_ZERO)
	{
		ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0); 
		return	TRUE;
	}
	else if	(systemCalcError & ILLEGAL_LOG_PARAM)
	{
		ReportSystemError(SYS_BAD_LOG_PARAM, SERR_ILLEGAL_LOG_PARAM); 
		return	TRUE;
	}

	return	FALSE;		// no system calc errors found
}


// ********************************************************************
// FUNCTION:
//
//		FlashMinIntensityCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update & Check flash at 850 wavelength only versus flash minimum intensity limit		 
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

unsigned long FlashMinIntensityCheck(RawFlashWavelengths_t *rawPtr)
{
	unsigned short rawFlash0Reading850;

	// update & check min lamp intensity only if optical blank cuvette 29

	if (rawPtr->cuvetteNumber == OPTICAL_CUVETTE)
	{ 
		rawFlash0Reading850 = rawPtr->rawFlashReadings[WL_850];

		// update rotor minimum intensity flash reading checked

		if (rawFlash0Reading850 < rotorResults_g->rotorInformationResults.rotorFlashLampIntensityMin)
		{
			rotorResults_g->rotorInformationResults.rotorFlashLampIntensityMin = rawFlash0Reading850;
		}
														    
		// check to set error condition for rotor minimum flash below allowable limit

		if (rawFlash0Reading850 < MINIMUM_LAMP_INTENSITY)
		{
			ReportSystemError(LAMP_NOT_FLASHING, SERR_LAMP_INTENSITY); 
			return	FLASH_BELOW_MIN_INTENSITY;
		}
	}

	return	NO_ANALYTE_ERROR;
}


// ********************************************************************
// FUNCTION:
//
//		FlashMaxSaturationCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update & Check flash reading for maximum saturation		 
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

unsigned long FlashMaxSaturationCheck(unsigned short rawFlashReading)
{
	// update rotor maximum saturation flash reading checked

	if (rawFlashReading > rotorResults_g->rotorInformationResults.rotorFlashLampSaturationMax)
	{
		rotorResults_g->rotorInformationResults.rotorFlashLampSaturationMax = rawFlashReading;
	}
														    
	// check to set error condition for rotor minimum flash below allowable limit

	if (rawFlashReading > LAMP_SATURATION_THRESHOLD)
	{
		ReportSystemError(WAVELENGTH_SATURATED, SERR_FLASH_SATURATION); 
		return	FLASH_ABOVE_MAX_SATURATION;
	}

	return	NO_ANALYTE_ERROR;
}


// ********************************************************************
// FUNCTION:
//
//		GetRawFlashReading() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get raw reading for raw flash readings at wavelength &
//		update/check for maximum lamp saturation		 
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

unsigned short GetRawFlashReading(RawFlashWavelengths_t *rawPtr, unsigned char wavelength, unsigned long *analyteError)
{
	unsigned short	rawFlashReading;

	// get reading for this wavelength

	rawFlashReading = rawPtr->rawFlashReadings[wavelength];

	*analyteError |= FlashMaxSaturationCheck(rawFlashReading);

	return	rawFlashReading;	
}


// ********************************************************************
// FUNCTION:
//
//		GetAdcReferenceOffset() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get ADC reference bias corrected reading for cuvette checks 
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

unsigned short GetAdcReferenceOffset(void)
{
	unsigned short	adcReferenceOffset;
	
	// get ADC bias reference offset and verify versus maximum limit allowed

	adcReferenceOffset = rotorRawReadings_g->adcBias.adcReferenceOffset;
	
	if (adcReferenceOffset > ADC_REFERENCE_OFFSET_LIMIT)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_ADC_REFERENCE_OFFSET); 
	}
	 
	return	adcReferenceOffset;
}


// ********************************************************************
// FUNCTION:
//
//		GetCuvetteCheckReading() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get ADC reference bias corrected reading for cuvette checks 
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

long GetCuvetteCheckReading(RawFlashWavelengths_t *rawPtr, unsigned char wavelength, unsigned long *analyteError)
{
	// correct raw flash cuvette reading with ADC bias reference offset 

	return	( (long)GetRawFlashReading(rawPtr, wavelength, analyteError) - (long)GetAdcReferenceOffset() );
}


// ********************************************************************
// FUNCTION:
//
//		GetSampleReading() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get offset corrected floating point reading for sampleType transmission calc
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

float GetSampleReading(RawFlashWavelengths_t *rawPtr, unsigned char wavelength, unsigned long *analyteError)
{
	// correct raw flash sample reading with rotor calibrated offset and convert to floating point

	return	(float)((long) GetRawFlashReading(rawPtr, wavelength, analyteError) - (long)rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[wavelength]);
}


// ********************************************************************
// FUNCTION:
//
//		CalcTrimmedFlashTransmissionAvg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate trimmed transmission average for sampleType
//		(for 8 of 10 flashes minus max/min flash readings)		 
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

float CalcTrimmedFlashTransmissionAvg(float *flashTransmissionPtr, unsigned numValidSampleFlashes)
{
	unsigned	i;
	unsigned	j;
	float 		sortSwap;
	float		averageTransmission;

	// do bubble sort on transmission data for all sample flashes from lowest to highest value

	for (i = 0; i < numValidSampleFlashes - 1; i++)
	{
		for (j = 0; j < numValidSampleFlashes - (i+1); j++)
		{
			if (flashTransmissionPtr[j] > flashTransmissionPtr[j+1])
			{
				sortSwap = flashTransmissionPtr[j+1];
				flashTransmissionPtr[j+1] = flashTransmissionPtr[j];
				flashTransmissionPtr[j] = sortSwap;
			}
		}
	}

	// Trim the sorted array by overwriting the lowest of the 10 transmission values 
	// resulting in only using 8 values in the transmission average calculation

	averageTransmission = 0.0;

	for (i = 0; i < numValidSampleFlashes-2; i++)
	{
		flashTransmissionPtr[i] = flashTransmissionPtr[i+1];
		averageTransmission += flashTransmissionPtr[i];
	}

	averageTransmission /= numValidSampleFlashes-2;	// average sample transmission value for check/return

	return averageTransmission;
}


// ********************************************************************
// FUNCTION:
//
//		TransmissionFlashRangeCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Perform sanity check of transmission value for sampleType and 
//		verify flash range of optics for sampling		 
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

unsigned long TransmissionFlashRangeCheck(float sampleTransmission, float *flashTransmissionPtr, float *flashRange, unsigned numValidSampleFlashes)
{
	float	trimmedFlashRange;

	// verify sanity of sample tranmission value

	if ((sampleTransmission == 0.0) || (sampleTransmission > MAX_TRANSMISSION_DIVISOR))
	{
		// do not allow divide by zeros extremely small values (millionths)

		return DIVIDE_BY_ZERO;
	}

	if (sampleTransmission < 0.0)
	{
		// cannot be negative as input to -log10() absorbance calculations

		return  ILLEGAL_LOG_PARAM;
	}

	// do flash range checks based on sample transmission for trimmed 8 flash values:
	// 	 	flashTransmission values sorted lower to higher - 
	//	 	so highest value for flashTransmissionPtr[(NUM_SAMPLE_FLASHES-2)-1] 
	// 	 	should always be > than lowest value for flashTransmissionPtr[0]

	trimmedFlashRange = flashTransmissionPtr[(numValidSampleFlashes-2)-1] - flashTransmissionPtr[0];

	if (sampleTransmission > 1.0)
	{
		sampleTransmission = 1.0 / sampleTransmission;
		trimmedFlashRange = sampleTransmission * sampleTransmission * trimmedFlashRange;
	}

	// set up return of flash range for SYS_CHECK flash range max calculations
	// which also uses sample transmission average value returned by CalcSampleTransmission()

	*flashRange = trimmedFlashRange;

	if ((sampleTransmission >= FLASH_RANGE_ERROR_TOGGLE) && (trimmedFlashRange > FLASH_RANGE_MAX))
	{
		return FLASH_RANGE_ERROR;
	}
	else if ((sampleTransmission < FLASH_RANGE_ERROR_TOGGLE) && (trimmedFlashRange > (FLASH_RANGE_MAX / 10)))
	{
		return FLASH_RANGE_ERROR;
	}
	
	return NO_ANALYTE_ERROR;		
}



// ********************************************************************
// FUNCTION:
//
//		CalcSampleTransmission() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate transmission value for sampleType		 
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

float CalcSampleTransmission(unsigned char sampleType, unsigned char samplePass, unsigned char wavelengthPair, float *flashRange, unsigned long *analyteError) 
{
	unsigned	i;
	unsigned	numValidSampleFlashes;
	unsigned	transmissionIndex;
	float		transmissionRatioDenominator;
	float		flashTransmission[NUM_SAMPLE_FLASHES];
	float		sampleTransmission;

	RawFlashWavelengths_t	*rawPtr;

	// initialize number of valid sample flashes to trim and index to store trim transmissions 

	numValidSampleFlashes = NUM_SAMPLE_FLASHES;
	transmissionIndex = 0;

	// update & check min lamp intensity for burnt flash 0 at 850 nm only if optical blank cuvette 29

	rawPtr = &rotorRawReadings_g->rotorAnalyteSamples[sampleType][samplePass-1][FLASH_0];

	*analyteError |= FlashMinIntensityCheck(rawPtr);

	// start at index 1 in raw readings to skip first flash raw readings

	for (i = 1; i <= NUM_SAMPLE_FLASHES; i++)
	{
		// get transmission as float value of raw reading for selected wavelength pair numerator this flash

		rawPtr = &rotorRawReadings_g->rotorAnalyteSamples[sampleType][samplePass-1][i];		 

		// check for a missing flash
		// if more than one missing flash, then flag analyte error for FLASH_RANGE_ERROR & FLASH_ABOVE_MAX_SATURATION

		if (rawPtr->engineTime != MISSING_FLASH_TIME)
		{
			// flash not missing so calc transmission for flash

			flashTransmission[transmissionIndex] = GetSampleReading(rawPtr, wavelengthPair / 16, analyteError);

			if ((wavelengthPair & 0x0f)	< NUM_WAVELENGTHS)
			{
				// calc transmission/ratio for wavelength pair if wavelength pair denominator defined
				// (can disable ratio if wavelengthPair least significant nibble = 0x0f)

				transmissionRatioDenominator = GetSampleReading(rawPtr, wavelengthPair & 0x0f, analyteError);

				if (transmissionRatioDenominator != 0.0)
				{
					flashTransmission[transmissionIndex] /= transmissionRatioDenominator;
				}
				else
				{
					*analyteError |= DIVIDE_BY_ZERO;
					return	NOVAL_POS_FLOAT;
				} 
			}

			// increment index to storage for next valid transmission value for trim set

			transmissionIndex++;
		}
		else
		{
			// allow for at least 9 valid flashes to proceed
			// no storage of transmission value for trim if missing

			numValidSampleFlashes--;

			if (numValidSampleFlashes < MIN_SAMPLE_TRIM_FLASHES_ALLOWED)
			{
				// less than 9 valid transmissions so bail

				*analyteError |= (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION);
				return	NOVAL_NEG_FLOAT;
			}
		}
	}

	// sort transmission/ratio values from lowest to highest
	// remove lowest & highest values and calculate sample transmission average for 8 remaining 

	sampleTransmission = CalcTrimmedFlashTransmissionAvg(flashTransmission, numValidSampleFlashes);
	 
	// do sanity check on sample transmission value & check for flash range errors  	

	*analyteError |= TransmissionFlashRangeCheck(sampleTransmission, flashTransmission, flashRange, numValidSampleFlashes);

	return sampleTransmission;
}


// SQUEEZER II - only

// ********************************************************************
// FUNCTION:
//
//		CalcTransmission() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate transmission value for sampleType
//		(used by SQUEEZER II only)		 
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

float CalcTransmission(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char checkPass, unsigned char wavelengthPair,unsigned long *analyteError)

{
	float	flashRange;
    float	sampleTransmission;
    float	opticalBlankTransmission;     

    // get transmission for sampleType & sampleTypeOpticalBlank with no reference wavelength for checkPass

    sampleTransmission = CalcSampleTransmission(sampleType, checkPass, wavelengthPair, &flashRange, analyteError);

    opticalBlankTransmission = CalcSampleTransmission(sampleTypeOpticalBlank, checkPass, wavelengthPair, &flashRange, analyteError);

	if (*analyteError != 0)
    {
		return (NOVAL_POS_FLOAT);
	}
	else
	{
		return(sampleTransmission / opticalBlankTransmission);
	}        
}


// ********************************************************************
// FUNCTION:
//
//		CalcFullRotorScanTransmission() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate Full Rotor Scan cuvette transmission
// 
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

float CalcFullRotorScanTransmission(unsigned readingsType, unsigned char targetCuvetteNumber, unsigned char referenceCuvetteNumber, unsigned char wavelengthPair, unsigned long *analyteError)
{
	unsigned char	i;
	unsigned		numMissingFlashes;
	long			targetSum;
	long			referenceSum;

	RawFlashWavelengths_t	*rawFlash0Reading850Ptr = 0;
	RawFlashWavelengths_t	*targetPtr = 0;
	RawFlashWavelengths_t	*referencePtr = 0;

	targetSum = 0;
	referenceSum = 0;

	numMissingFlashes = 0;

	// start at index 1 in raw readings to skip first flash raw readings

	for (i = 1; i <= NUM_FRS_SAMPLE_FLASHES; i++)
	{
        switch (readingsType)
        {
            case PRE_SAMPLE_FRS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->preSampleFullRotorScan[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->preSampleFullRotorScan[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->preSampleFullRotorScan[referenceCuvetteNumber][i];
				break;
            case POST_SAMPLE_FRS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->postSampleFullRotorScan[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->postSampleFullRotorScan[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->postSampleFullRotorScan[referenceCuvetteNumber][i];
				break;
			default:
				break;
		}

		// check for missing flashes
		// if more than one missing flash, then flag analyte error for FLASH_RANGE_ERROR & FLASH_ABOVE_MAX_SATURATION

		if ((targetPtr->engineTime == MISSING_FLASH_TIME) || (referencePtr->engineTime == MISSING_FLASH_TIME)) 
		{
			numMissingFlashes++;

			if (numMissingFlashes > MAX_MISSING_FLASHES_ALLOWED)
			{
				// more than 1 missing flash

				*analyteError |= (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION);
				return	NOVAL_NEG_FLOAT;
			}
		}
		else
		{
			// flash not missing so sum target and reference ADC values for transmission calcs

			targetSum    += GetSampleReading(targetPtr,    wavelengthPair / 16,   analyteError);
			referenceSum += GetSampleReading(referencePtr, wavelengthPair & 0x0f, analyteError);
		}
	}

	if (!referenceSum)
	{
		*analyteError |= DIVIDE_BY_ZERO;
		return	NOVAL_POS_FLOAT;
	}

	// update & check min lamp intensity for burnt flash 0 at 850 nm only if optical blank cuvette 29

	*analyteError |= FlashMinIntensityCheck(rawFlash0Reading850Ptr);

	// compute transmission	for 850 wavelength for cuvette

	return	((float)targetSum / (float)referenceSum);
}


// ********************************************************************
// FUNCTION:
//
//		CalcFullRotorScanAbsorbance() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate Full Rotor Scan cuvette absorbance 
//		
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

float CalcFullRotorScanAbsorbance(unsigned readingsType, unsigned char cuvetteNumber, unsigned char wavelengthPair, unsigned long *cuvetteError)
{
	float	cuvetteTransmission;
	float	opticalTransmission;
	float	cuvetteAbsorbance;

	cuvetteAbsorbance = NOVAL_NEG_FLOAT;

	cuvetteTransmission = CalcFullRotorScanTransmission(readingsType, cuvetteNumber,   cuvetteNumber,   wavelengthPair, cuvetteError);
	opticalTransmission = CalcFullRotorScanTransmission(readingsType, OPTICAL_CUVETTE, OPTICAL_CUVETTE, wavelengthPair, cuvetteError);

	if (*cuvetteError & CALC_ERROR_MASK)
	{
		// got a calc error in transmission calcs
			
		if (*cuvetteError & DIVIDE_BY_ZERO)
		{
			ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
		}
		else if (*cuvetteError & (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION))
		{
			ReportSystemError(SYS_FLASH_RANGE, SERR_FLASH_RANGE);
		}
	}
	else if (opticalTransmission == 0.0)
	{
		// got a optical tranmission divisor of 0

		ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
	}
	else if ((cuvetteTransmission / opticalTransmission) <= 0.0)
	{
		// got a negative illgal transmission ratio	for log

		ReportSystemError(SYS_BAD_LOG_PARAM, SERR_ILLEGAL_LOG_PARAM);
	}
	else
	{
		// optical transmission not 0 with no calc errors, then calc absorbance for cuvette

		cuvetteAbsorbance = -log10(cuvetteTransmission / opticalTransmission);
	}

	return cuvetteAbsorbance;
}


// end SQUEEZER II


// ********************************************************************
// FUNCTION:
//
//		StoreSampleAbsorbance() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store each sampleAbsorbance value calculated in next record for RAD file		 
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

void StoreSampleAbsorbance(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char samplePass, unsigned char wavelengthPair, float desiredPathLength, unsigned char cuvetteNumber, unsigned long analyteError, float sampleAbsorbance)
{
	SampleAbsRecord_t	*absPtr;

	absPtr = &rotorResults_g->rotorAbsorbances.sampleAbsRecords[rotorResults_g->rotorAbsorbances.numberAbsRecords++];

	absPtr->engineMinutesTime	   = GetSampleFlashTime(sampleType, samplePass);
	absPtr->sampleType			   = sampleType;
	absPtr->sampleTypeOpticalBlank = sampleTypeOpticalBlank;
	absPtr->samplePass 			   = samplePass;
	absPtr->wavelengthPair 		   = wavelengthPair;
	absPtr->desiredPathLength	   = desiredPathLength;
	absPtr->cuvetteNumber		   = cuvetteNumber;
	absPtr->analyteError		   = analyteError;
	absPtr->sampleAbsorbance	   = sampleAbsorbance;
}


// ********************************************************************
// FUNCTION:
//
//		CalcSampleAbsorbance() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate path length corrected absorbance value for sampleType		 
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

float CalcSampleAbsorbance(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char samplePass, unsigned char wavelengthPair, float desiredPathLength, unsigned char cuvetteNumber, unsigned long *analyteError)	
{
	float	sampleTransmission;
	float	OpticalBlankTransmission;
	float	sampleAbsorbance;
	float	flashRange;

	// get transmission for sampleType & sampleTypeOpticalBlank for wavelengthPair at samplePass

	sampleTransmission = CalcSampleTransmission(sampleType, samplePass, wavelengthPair, &flashRange, analyteError);

	OpticalBlankTransmission = CalcSampleTransmission(sampleTypeOpticalBlank, samplePass, wavelengthPair, &flashRange, analyteError);

	if (*analyteError & commonAlgGlobals_g->calcErrorMask)
	{
		sampleAbsorbance = NOVAL_POS_FLOAT;
	}
	else
	{
		// no errors in transmissions, then calc absorbance for sample

		sampleAbsorbance = -log10(sampleTransmission / OpticalBlankTransmission);

		// do path length correction for absorbance if cuvetteNumber is valid 
		// (can disable path length correction if cuvetteNumber = 0xff)

		if (cuvetteNumber < NUM_CUVETTES)
		{
			sampleAbsorbance *= (desiredPathLength / rotorConfig_g->rotorCalculationRecord.rotorCuvettePathLengths[cuvetteNumber]);
		}
	}

	StoreSampleAbsorbance(sampleType, sampleTypeOpticalBlank, samplePass, wavelengthPair, desiredPathLength, cuvetteNumber, *analyteError, sampleAbsorbance);

	return	sampleAbsorbance;
}


// ********************************************************************
// FUNCTION:
//
//		BeadMixCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see of the bead in cuvette is mixed within limits		 
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

unsigned long BeadMixCheck(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char checkPass, float beadMixCheckLimit, unsigned long analyteErrorMask, unsigned long *analyteError)
{
	float			flashRange;
	float			sampleTransmission850;
	float			OpticalBlankTransmission850;

	// get transmission for sampleType & sampleTypeOpticalBlank at 850 nm with no reference wavelength for checkPass
	 
	sampleTransmission850 = CalcSampleTransmission(sampleType, checkPass, WP_850_NULL, &flashRange, analyteError);

	OpticalBlankTransmission850 = CalcSampleTransmission(sampleTypeOpticalBlank, checkPass, WP_850_NULL, &flashRange, analyteError);


	// IMV - bead mix set

	SetImvBeadMixParams( sampleTransmission850, OpticalBlankTransmission850, beadMixCheckLimit );
	 
	// end IMV


	if (!(*analyteError & commonAlgGlobals_g->calcErrorMask))
	{
		// no errors in transmissions, then check bead mix of analyte & process error
		
		if ((sampleTransmission850 / OpticalBlankTransmission850) < beadMixCheckLimit)
		{
				*analyteError |= analyteErrorMask;
		}
	}

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		CalcCuvetteCheckTransmission() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate cuvette transmission for cuvette checks such as
//		- bead
//		- distribution
//		- insufficient sample
//		- insufficient diluent 	 
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

float CalcCuvetteCheckTransmission(unsigned readingsType, unsigned char targetCuvetteNumber, unsigned char referenceCuvetteNumber, unsigned char wavelengthPair, unsigned numFlashes, unsigned long *analyteError)
{
	unsigned char	i;
	unsigned		numMissingFlashes;
	long			targetSum;
	long			referenceSum;

	RawFlashWavelengths_t	*rawFlash0Reading850Ptr = 0;
	RawFlashWavelengths_t	*targetPtr = 0;
	RawFlashWavelengths_t	*referencePtr = 0;

	targetSum = 0;
	referenceSum = 0;

	numMissingFlashes = 0;

	// start at index 1 in raw readings to skip first flash raw readings

	for (i = 1; i <= numFlashes; i++)
	{
        switch (readingsType)
        {
            case BEAD1_READINGS:
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->beadCheck1[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->beadCheck1[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->beadCheck1[referenceCuvetteNumber][i];
				break;
            case BEAD2_READINGS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->beadCheck2[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->beadCheck2[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->beadCheck2[referenceCuvetteNumber][i];
				break;
            case DISTRIBUTION_READINGS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->distributionCheck[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->distributionCheck[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->distributionCheck[referenceCuvetteNumber][i];
				break;
            case PRE_SAMPLE_FRS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->preSampleFullRotorScan[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->preSampleFullRotorScan[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->preSampleFullRotorScan[referenceCuvetteNumber][i];
				break;
            case POST_SAMPLE_FRS:     
            	rawFlash0Reading850Ptr = &rotorRawReadings_g->postSampleFullRotorScan[OPTICAL_CUVETTE][FLASH_0];     
				targetPtr    = &rotorRawReadings_g->postSampleFullRotorScan[targetCuvetteNumber][i];
				referencePtr = &rotorRawReadings_g->postSampleFullRotorScan[referenceCuvetteNumber][i];
				break;
			default:
				break;
		}

		// check for missing flashes
		// if more than one missing flash, then flag analyte error for FLASH_RANGE_ERROR & FLASH_ABOVE_MAX_SATURATION

		if ((targetPtr->engineTime == MISSING_FLASH_TIME) || (referencePtr->engineTime == MISSING_FLASH_TIME)) 
		{
			numMissingFlashes++;

			if (numMissingFlashes > MAX_MISSING_FLASHES_ALLOWED)
			{
				// more than 1 missing flash

				*analyteError |= (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION);
				return	NOVAL_NEG_FLOAT;
			}
		}
		else
		{
			// flash not missing so sum target and reference ADC values for transmission calcs

			targetSum    += GetCuvetteCheckReading(targetPtr,    wavelengthPair / 16,   analyteError);
			referenceSum += GetCuvetteCheckReading(referencePtr, wavelengthPair & 0x0f, analyteError);
		}
	}

	if (!referenceSum)
	{
		*analyteError |= DIVIDE_BY_ZERO;
		return	NOVAL_POS_FLOAT;
	}

	// update & check min lamp intensity for burnt flash 0 at 850 nm only if optical blank cuvette 29

	*analyteError |= FlashMinIntensityCheck(rawFlash0Reading850Ptr);

	// compute transmission	for 850 wavelength for cuvette

	return	((float)targetSum / (float)referenceSum);
}


// ********************************************************************
// FUNCTION:
//
//		CalcCuvetteCheckAbsorbance() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate cuvette absorbance for cuvette checks such as
//		- insufficient sample
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

float CalcCuvetteCheckAbsorbance(unsigned readingsType, unsigned char cuvetteNumber, unsigned char wavelengthPair, unsigned numFlashes, unsigned long *cuvetteError)
{
	float	cuvetteTransmission;
	float	opticalTransmission;
	float	cuvetteAbsorbance;

	cuvetteAbsorbance = NOVAL_NEG_FLOAT;

	cuvetteTransmission = CalcCuvetteCheckTransmission(readingsType, cuvetteNumber,   cuvetteNumber,   wavelengthPair, numFlashes, cuvetteError);
	opticalTransmission = CalcCuvetteCheckTransmission(readingsType, OPTICAL_CUVETTE, OPTICAL_CUVETTE, wavelengthPair, numFlashes, cuvetteError);

	if (*cuvetteError & CALC_ERROR_MASK)
	{
		// got a calc error in transmission calcs
			
		if (*cuvetteError & DIVIDE_BY_ZERO)
		{
			ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
		}
		else if (*cuvetteError & (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION))
		{
			ReportSystemError(SYS_FLASH_RANGE, SERR_FLASH_RANGE);
		}
	}
	else if (opticalTransmission == 0.0)
	{
		// got a optical tranmission divisor of 0

		ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
	}
	else if ((cuvetteTransmission / opticalTransmission) <= 0.0)
	{
		// got a negative illgal transmission ratio	for log

		ReportSystemError(SYS_BAD_LOG_PARAM, SERR_ILLEGAL_LOG_PARAM);
	}
	else
	{
		// optical transmission not 0 with no calc errors, then calc absorbance for cuvette

		cuvetteAbsorbance = -log10(cuvetteTransmission / opticalTransmission);
	}

	return cuvetteAbsorbance;
}


// ********************************************************************
// FUNCTION:
//
//		SetBeadMissingDistributionCheckFlag() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set flag for cuvette bead missing or distribution check error in
//		respective error flags		 
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

void SetBeadMissingDistributionCheckFlag(unsigned char cuvetteNumber, unsigned long *beadMissingDistCheckFlags)
{
	*beadMissingDistCheckFlags |= beadMissingDistCheckFlagsByCuvette_m[cuvetteNumber];
}


// ********************************************************************
// FUNCTION:
//
//		BeadMissingCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see of the bead in an analyte cuvette and an analyte blank cuvette if enabled		 
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

unsigned long BeadMissingCheck(unsigned char cuvetteNumber, float beadMissingCheckLimit, unsigned long *analyteError)
{
	float			cuvetteTransmission;
		

	// IMV - store this cuvette bead check limit for RAD intermediate value

	rotorResults_g->rotorCuvetteBeadLimits[ cuvetteNumber ] = beadMissingCheckLimit;

	// end IMV


	// get transmission for analyte cuvette 850 nm / optical cuvette 850 nm from bead check 2 data 

	cuvetteTransmission = CalcCuvetteCheckTransmission(BEAD2_READINGS, cuvetteNumber, OPTICAL_CUVETTE, WP_850_850, NUM_BEAD_CHECK_FLASHES, analyteError);

	if (cuvetteTransmission > beadMissingCheckLimit)
	{
		*analyteError |= CHEM_BEAD_MISSING;

		SetBeadMissingDistributionCheckFlag(cuvetteNumber, &rotorResults_g->rotorInformationResults.rotorBeadCheck2Flags);
	}

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		BeadDistributionCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see of the bead in cuvette is mixed within limits		 
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

unsigned long BeadDistributionCheck(unsigned char cuvetteNumber, float beadDistributionCheckLimit, unsigned long *analyteError)
{
	float			cuvetteTransmission;
		

	// IMV - store this cuvette distribution check limit for RAD intermediate value

	rotorResults_g->rotorCuvetteDistributionLimits[ cuvetteNumber ] = beadDistributionCheckLimit;

	// end IMV


	// get transmission for analyte cuvette 850 nm / optical cuvette 850 nm from bead check 2 data 

	cuvetteTransmission = CalcCuvetteCheckTransmission(DISTRIBUTION_READINGS, cuvetteNumber, OPTICAL_CUVETTE, WP_850_850, NUM_DIST_CHECK_FLASHES, analyteError);

	if (cuvetteTransmission < beadDistributionCheckLimit)
	{
		*analyteError |= CHEM_DIST_ERROR;

		SetBeadMissingDistributionCheckFlag(cuvetteNumber, &rotorResults_g->rotorInformationResults.rotorDistributionCheckFlags);
	}

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		SystemBeadMissingDistCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see if system level error numbers for bead missing at bead 2 time
//		or distribution error at distribution time		 
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

void SystemBeadMissingDistCheck(unsigned readingsType, const BeadDistCheck_t *beadDistChecks, unsigned long beadDistSystemFlag, unsigned short beadDistErrorNumber)  
{
	unsigned		i;
	unsigned long	beadDistCheckError;
	unsigned long	beadDistCheckMask;
	unsigned long	errorFlag;

	// init for no cuvette errors

	beadDistCheckError = 0;
	errorFlag = 0;
	
	// perform selected bead or distribution check for all cuvettes in list	passed

	i = 0;				

	while ((beadDistChecks[i].cuvetteNumber != BEAD_DIST_CHK_DONE) && (!errorFlag))
	{
		if (readingsType == BEAD2_READINGS)
		{
			beadDistCheckMask = CHEM_BEAD_MISSING;
			errorFlag = BeadMissingCheck(beadDistChecks[i].cuvetteNumber, beadDistChecks[i].beadDistCheckLimit, &beadDistCheckError);
		}
		else
		{
			if ( ((rotorConfig_g->rotorNumber == 60) && ( beadDistChecks[i].cuvetteNumber == 5 )) == FALSE )  // disable system distribution check for rotor 60, optical cuvette 5 (i.e. not IQC) used in POS/NEG type algorithm (e.g. CHW)
			{
				beadDistCheckMask = CHEM_DIST_ERROR;
				errorFlag = BeadDistributionCheck(beadDistChecks[i].cuvetteNumber, beadDistChecks[i].beadDistCheckLimit, &beadDistCheckError);
			}
		}

		if (errorFlag)
		{
			if (beadDistCheckError & DIVIDE_BY_ZERO)
			{
				ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
				return;
			}
		}
		else if (beadDistCheckError & beadDistCheckMask)
		{
			ReportSystemError(beadDistSystemFlag, beadDistErrorNumber);
			return;
		}	

		beadDistCheckError = 0;
		i++;
	}
}


// ********************************************************************
// FUNCTION:
//
//		StartingAbsorbanceCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see of the starting absorbance is within limits		 
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

unsigned long StartingAbsorbanceCheck(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char checkPass, unsigned char wavelengthPair, float startingAbsLimit, unsigned long analyteErrorMask, unsigned long *analyteError)
{
	unsigned	startingAbsCheckError;
	float		startingAbsSample;

	// get starting absorbance of analyte 

	startingAbsCheckError = FALSE;

	startingAbsSample = CalcSampleAbsorbance(sampleType, sampleTypeOpticalBlank, checkPass, wavelengthPair, 1.0, 0xff, analyteError);


	// IMV - starting abs set

	SetImvStartAbsParams( startingAbsSample, startingAbsLimit );

	// end IMV


	// if calc error in analyte absorbance then don't check starting abs

	if (!(*analyteError & commonAlgGlobals_g->calcErrorMask))
	{
		// no errors in absorbance
		
		if (startingAbsSample > startingAbsLimit)
		{
			startingAbsCheckError = TRUE;
			*analyteError |= analyteErrorMask;
		}
	}

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask) | startingAbsCheckError;		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		StartingAbsorbanceCheckLow() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to see of the starting absorbance is within low limits		 
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

unsigned long StartingAbsorbanceCheckLow(unsigned char sampleType, unsigned char sampleTypeOpticalBlank, unsigned char checkPass, unsigned char wavelengthPair, float startingAbsLimit, unsigned long analyteErrorMask, unsigned long *analyteError)
{
	unsigned	startingAbsCheckError;
	float		startingAbsSample;

	// get starting absorbance of analyte 

	startingAbsCheckError = FALSE;

	startingAbsSample = CalcSampleAbsorbance(sampleType, sampleTypeOpticalBlank, checkPass, wavelengthPair, 1.0, 0xff, analyteError);


	// IMV - starting abs set

	SetImvStartAbsParams( startingAbsSample, startingAbsLimit );

	// end IMV


	// if calc error in analyte absorbance then don't check starting abs

	if (!(*analyteError & commonAlgGlobals_g->calcErrorMask))
	{
		// no errors in absorbance
		
		if (startingAbsSample < startingAbsLimit)
		{
			startingAbsCheckError = TRUE;
			*analyteError |= analyteErrorMask;
		}
	}

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask) | startingAbsCheckError;		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		CalcAverageAbsorbance() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate average analyte blanked absorbance for endpoint algorithm 
//		and min & max blanked absorbances in average		 
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

unsigned long CalcAverageAbsorbance(AvgAbs_t *absPtr, unsigned char sampleTypeAnalyte, unsigned char sampleTypeAnalyteBlank, unsigned char sampleTypeOpticalBlank, unsigned char startPass, unsigned char endPass, unsigned char wavelengthPair, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned char analyteBlankCuvetteNumber, unsigned char blankCorrectionOrder, unsigned long *analyteError)
{
	unsigned char	i;
	float			analyteAbs;
	float			analyteBlankAbs;
	float			blankedAbs;

	// init blanked averages, abs sum and blanked minimum & maximum abs in average

	analyteBlankAbs = 0.0;

	absPtr->min = NOVAL_POS_FLOAT;
	absPtr->max = NOVAL_NEG_FLOAT;
	absPtr->avg = 0.0;

	absPtr->avgAnalyte      = 0.0;
	absPtr->avgAnalyteBlank = 0.0;

	for (i = startPass; i <= endPass; i++)
	{
		// calc absorbance for analyte sample type and analyte blank sample type for this pass

		analyteAbs = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, i, wavelengthPair, desiredPathLength, analyteCuvetteNumber, analyteError);

		if (blankCorrectionOrder != NO_BLANK_CORRECTION)
		{
			analyteBlankAbs = CalcSampleAbsorbance(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, i, wavelengthPair, desiredPathLength, analyteBlankCuvetteNumber, analyteError);
		}
		// compute blanked absorbance based on order for this pass

		if (blankCorrectionOrder == ANALYTE_MINUS_BLANK)
		{
			blankedAbs = analyteAbs - analyteBlankAbs;		// 0: "analyte - blank"
		}
		else if	(blankCorrectionOrder == BLANK_MINUS_ANALYTE)
		{
			blankedAbs = analyteBlankAbs - analyteAbs;		// 1: "blank - analyte"
		}
		else
		{
			blankedAbs = analyteAbs;						// 0xff: "analyte" ; no NO_BLANK_CORRECTION
		}
		
		if (blankedAbs < absPtr->min)		// update min blanked abs value
		{
			absPtr->min = blankedAbs;
		}
		
		if (blankedAbs > absPtr->max)		// update max blanked abs value
		{
			absPtr->max = blankedAbs;
		}

		absPtr->avg             += blankedAbs;			// sum blanked abs value for this pass 
		absPtr->avgAnalyte      += analyteAbs;			// sum analyte abs value for this pass 
		absPtr->avgAnalyteBlank += analyteBlankAbs;		// sum analyte blank abs value for this pass 

		// save flash time & blank corrected abs for each pass in avg

		absPtr->flashTime[i - startPass]  = GetSampleFlashTime(sampleTypeAnalyte, i);
		absPtr->blankedAbs[i - startPass] = blankedAbs;		
	}

	// calculate average blanked abs for all passes and for analyte abs & analyte blank abs

	absPtr->avg             /= ((endPass - startPass) + 1);
	absPtr->avgAnalyte      /= ((endPass - startPass) + 1);
	absPtr->avgAnalyteBlank /= ((endPass - startPass) + 1);

	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag 
}


// ********************************************************************
// FUNCTION:
//
//		EndpointTurbidityPassCorrection() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check for turbidity for pass correction for abs averaging		 
//
//		Note: assume enabled for only one wavelength to determine if correction needed		 
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

void EndpointTurbidityPassCorrection( AvgAbs_t *avgAbsTurb, unsigned char turbidityPassCorrectionStartPass, unsigned char turbidityPassCorrectionEndPass, unsigned long turbidityPassCorrectionSexSpecies, float turbidityPassCorrectionLimit, float turbidityCorrectedStartPassAvg, float turbidityCorrectedEndPassAvg )
{
	float	startBlankedAbs;
	float	endBlankedAbs;
	float	netAbs;


//	compute net blanked abs from end pass blanked abs - start pass blanked abs

	startBlankedAbs = avgAbsTurb->blankedAbs[ turbidityPassCorrectionStartPass - 1 ];
	endBlankedAbs   = avgAbsTurb->blankedAbs[ turbidityPassCorrectionEndPass   - 1 ];

	netAbs = startBlankedAbs - endBlankedAbs;

// correct only if species match AND net blankd abs over limit

	if ( ( ( rotorResults_g->rotorInformationResults.rotorSystemFlags & SEX_SPECIES_MASK ) == turbidityPassCorrectionSexSpecies ) && ( netAbs > turbidityPassCorrectionLimit ) )
	{
		endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = turbidityCorrectedStartPassAvg;
		endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass   = turbidityCorrectedEndPassAvg;
	}


// set appropriate IMV

	StoreIntermediateValue( ALB_BCG, IMV_TURB_BLANKED_ABS_START , IMV_DIRECT_SET_METHOD, startBlankedAbs );
	StoreIntermediateValue( ALB_BCG, IMV_TURB_BLANKED_ABS_END   , IMV_DIRECT_SET_METHOD, endBlankedAbs );

	StoreIntermediateValue( ALB_BCG, IMV_TURB_CORR_LIMIT        , IMV_DIRECT_SET_METHOD, turbidityPassCorrectionLimit );

	StoreIntermediateValue( ALB_BCG, IMV_TURB_AVG_START_PASS    , IMV_DIRECT_SET_METHOD, (float) endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass );
	StoreIntermediateValue( ALB_BCG, IMV_TURB_AVG_END_PASS      , IMV_DIRECT_SET_METHOD, (float) endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass );


}


// ********************************************************************
// FUNCTION:
//
//		EndpointRateErrorCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check for rate in endpoint based on magnitude of average abs toggle
//		and limits for flatness ratio and absolute flatness
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

unsigned long EndpointRateErrorCheck(AvgAbs_t *absPtr, float flatnessToggle, float flatnessRatioLimit, float flatnessAbsoluteLimit, unsigned long rateInEndpointErrorMask, unsigned long *analyteError)
{
	float	flatness;
	float	flatnessAvg;

	flatness 	= NOVAL_NEG_FLOAT;
	flatnessAvg	= NOVAL_NEG_FLOAT;

	// check abs avg for divide by zero error 

	if (absPtr->avg == 0.0)
	{
		*analyteError |= DIVIDE_BY_ZERO;
	}
	else
	{
		// check for rate in endpoint

		flatness = absPtr->max - absPtr->min;
		flatnessAvg = flatness / fabs(absPtr->avg);

		if (fabs(absPtr->avg) > flatnessToggle)
		{
			if (flatnessAvg > flatnessRatioLimit)
			{
				*analyteError |= rateInEndpointErrorMask;
			}
		}
		else
		{
			if (flatness > flatnessAbsoluteLimit)
			{
				*analyteError |= rateInEndpointErrorMask;
			}
		}
	}


	// IMV - endpoint flatness set

	SetImvEndpointFlatnessParams(flatness, flatnessAvg);

	// end IMV


	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		OverSystemRangeErrorCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check for over system range by performing linear regression of 
//		absorbance data during the reaction to determine if 
//		slope would indicate that the result might be over system range		 
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

unsigned long OverSystemRangeErrorCheck(AvgAbs_t *absPtr, unsigned char overSystemRangeStartPass, unsigned char overSystemRangeEndPass, unsigned char overSystemRangeLessThanLimit, float overSystemRangeLimit, unsigned long overSystemRangeErrorMask, unsigned long *analyteError)
{
	unsigned char	numPoints;
	float			slope;
	float			intercept;


	numPoints = (overSystemRangeEndPass - overSystemRangeStartPass) + 1;

	// compute slope used to calculate T4_L1 rate analyte result for multiple points 

	if (LinearRegression(absPtr->flashTime, absPtr->blankedAbs, numPoints, &slope, &intercept))
	{
		*analyteError |= DIVIDE_BY_ZERO;
		return	TRUE;						// abort algorithm processing flag
	}

	if ( (overSystemRangeLessThanLimit && ( slope < overSystemRangeLimit )) || (!overSystemRangeLessThanLimit && ( slope > overSystemRangeLimit )) )		
	{
		*analyteError |= overSystemRangeErrorMask;						// set for overSystemRange based on rate in endpoint
	}


	// IMV - endpoint flatness set

	SetImvEndpointOverSystemRangeParams( (float) overSystemRangeStartPass, (float) overSystemRangeEndPass, overSystemRangeLimit, slope );

	// end IMV


	return	(*analyteError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag
}


// ********************************************************************
// FUNCTION:
//
//		GetBarcodeParameter() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get rotor barcode analyte algorithm parameter from barcode data & ROC file		 
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

float GetBarcodeParameter(unsigned char analyteType, unsigned algParamIdx)
{
	unsigned	index;
	float		scale;
	float		offset;	

	index  = rotorConfig_g->analyteCalcRecord[analyteType].barcodeFactor[algParamIdx].index;
	scale  = rotorConfig_g->analyteCalcRecord[analyteType].barcodeFactor[algParamIdx].scale;
	offset = rotorConfig_g->analyteCalcRecord[analyteType].barcodeFactor[algParamIdx].offset;


	// IMV - barcode factor set: GetBarcodeParameter()

	SetImvBarcodeParams( (barcodeData_g->rotorAnalyteFactor[index-1] * scale) + offset );

	// end IMV


	return	(barcodeData_g->rotorAnalyteFactor[index-1] * scale) + offset;
}


// ********************************************************************
// FUNCTION:
//
//		bfdCalAdjust() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct algorithm parameter with bfd calibration adjustment
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

float BfdCalAdjust(float algParameter, unsigned bfdIdx)
{
	if ( bfdIdx >= MAX_BFD_CALS )
	{

		// IMV - bfd corrected endpoint avg / rate slope set: invalid bfd

		SetImvBfdCorrectedParams( algParameter * 1.0  );
		
		// end IMV


		return ( algParameter * 1.0 );
	}
	else
	{

		// IMV - bfd corrected endpoint avg / rate slope set: valid bfd

		SetImvBfdCorrectedParams( algParameter * factoryData_g->bfdCalibrationFactors[bfdIdx] );
		
		// end IMV


		return	algParameter * factoryData_g->bfdCalibrationFactors[bfdIdx];
	}
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdStdBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic endpoint or rate analyte result value using absorbance average or slope
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

float CalcBfdStdBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType, unsigned slopeIdx, unsigned interceptIdx )
{
	float	bfdCorrected;
	float	slope;
	float	intercept;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	slope     = GetBarcodeParameter(analyteType, slopeIdx);
	intercept = GetBarcodeParameter(analyteType, interceptIdx);

	return	(bfdCorrected * slope) + intercept;
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdBaseFineBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic endpoint or rate analyte result value using absorbance average or slope
//		using slope & intercept from base/fine barcode parameters
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

float CalcBfdBaseFineBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType)
{
	float	bfdCorrected;
	float	slope;
	float	intercept;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	slope     = GetBarcodeParameter(analyteType, SLOPE_BASE_IDX)     + GetBarcodeParameter(analyteType, SLOPE_FINE_IDX);
	intercept = GetBarcodeParameter(analyteType, INTERCEPT_BASE_IDX) + GetBarcodeParameter(analyteType, INTERCEPT_FINE_IDX);


	// IMV - set final slope & intercept intermediate values used in calculation

	StoreIntermediateValue( analyteType, IMV_BARCODE_SLOPE_USED,     IMV_DIRECT_SET_METHOD, slope );
	StoreIntermediateValue( analyteType, IMV_BARCODE_INTERCEPT_USED, IMV_DIRECT_SET_METHOD, intercept );
		
	// end IMV


	return	(bfdCorrected * slope) + intercept;
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdExpBaseFineBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic analyte result value using absorbance average 
//		using exponential recovery calc using  A0, A1, A2 base/fine barcode parameters  
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

float CalcBfdExpBaseFineBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType)
{
	float	bfdCorrected;
	float	A0;
	float	A1;
	float	A2;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	A0 = GetBarcodeParameter(analyteType, A0_BASE_IDX) + GetBarcodeParameter(analyteType, A0_FINE_IDX);
	A1 = GetBarcodeParameter(analyteType, A1_BASE_IDX) + GetBarcodeParameter(analyteType, A1_FINE_IDX);
	A2 = GetBarcodeParameter(analyteType, A2_BASE_IDX) + GetBarcodeParameter(analyteType, A2_FINE_IDX);


	// IMV - set final A0, A1, A2 intermediate values used in calculation

	StoreIntermediateValue( analyteType, IMV_BARCODE_A0_USED, IMV_DIRECT_SET_METHOD, A0 );
	StoreIntermediateValue( analyteType, IMV_BARCODE_A1_USED, IMV_DIRECT_SET_METHOD, A1 );
	StoreIntermediateValue( analyteType, IMV_BARCODE_A2_USED, IMV_DIRECT_SET_METHOD, A2 );
		
	// end IMV


	return	( ( A0 * exp( A1 * bfdCorrected ) ) + A2 );
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdPatientTypeBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic endpoint or rate analyte result value using absorbance average or slope
//		using barcode slope & intercept based on patient type selected
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

float CalcBfdPatientTypeBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType)
{
	unsigned		slopeIdx;
	unsigned		interceptIdx;
	unsigned long	systemFlags;
	float			bfdCorrected;
	float			slope;
	float			intercept;

	const PatientTypeBarcodeCorrection_t patientTypeBarcodeCorrection[] = 
	{
		{DOG_SLOPE_IDX,   DOG_INTERCEPT_IDX,   USED_DOG_FACTORS  },	// DOG_FEMALE_PATIENT_TYPE
		{CAT_SLOPE_IDX,   CAT_INTERCEPT_IDX,   USED_CAT_FACTORS  },	// CAT_MALE_PATIENT_TYPE
		{HORSE_SLOPE_IDX, HORSE_INTERCEPT_IDX, USED_HORSE_FACTORS},	// HORSE_PATIENT_TYPE
		{OTHER_SLOPE_IDX, OTHER_INTERCEPT_IDX, USED_OTHER_FACTORS},	// UNSPECIFIED_PATIENT_TYPE
	};
	
	
	// IMV - barcode factors set: 8 patient type factors 

	SetImvPatientTypeBarcodeParams( analyteType  );

	// end IMV	


	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	// report barcode factors used in rotor system flags with no error

	systemFlags =  patientTypeBarcodeCorrection[commonAlgGlobals_g->patientType].systemFlags;
	ReportSystemError(systemFlags, SERR_NONE); 

	// get slope & index for result calc based on patient type

	slopeIdx     = patientTypeBarcodeCorrection[commonAlgGlobals_g->patientType].slopeIdx;
	interceptIdx = patientTypeBarcodeCorrection[commonAlgGlobals_g->patientType].interceptIdx;

	slope     = GetBarcodeParameter(analyteType, slopeIdx);
	intercept = GetBarcodeParameter(analyteType, interceptIdx);

	return	(bfdCorrected * slope) + intercept;
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdIctericBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate endpoint analyte result value using analyte abs average & 
//		analyte blank abs average for computing icteric bfd correction 
//		using standard barcode slope & intercept  
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

float CalcBfdIctericBarcodeCorrectedResult(float avgAnalyte, float avgAnalyteBlank, unsigned bfdIdx, unsigned char analyteType, CommonFactorsLimits_t *factorsLimitsPtr)
{
	float	blankFactor;
	float	analyteFactor1;
	float	analyteFactor2;
	float	blankCorrection;
	float	analyteCorrection;
	float	avgIctericCorrected;

	// compute false rate correction on bfd corrected rate value

	blankCorrection = 0.0;
	analyteCorrection = 0.0;

	if (rotorResults_g->rotorInformationResults.rotorIctericIndex > 0.0)
	{
		blankFactor    = factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_BLANK_FACTOR_IDX];
		analyteFactor1 = factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_1_IDX];
		analyteFactor2 = factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_2_IDX];

		blankCorrection   = blankFactor * rotorResults_g->rotorInformationResults.rotorIctericIndex;
		analyteCorrection = ((analyteFactor2 * rotorResults_g->rotorInformationResults.rotorIctericIndex) + analyteFactor1) * rotorResults_g->rotorInformationResults.rotorIctericIndex;
	}

	avgIctericCorrected = (avgAnalyte / (1 + analyteCorrection)) - (avgAnalyteBlank - blankCorrection);


	// IMV - store ICT correction params

	StoreIntermediateValue( analyteType, IMV_ICT_BLANK_CORRECTION, IMV_DIRECT_SET_METHOD, blankCorrection );
	StoreIntermediateValue( analyteType, IMV_ICT_TEST_CORRECTION,  IMV_DIRECT_SET_METHOD, analyteCorrection );
		
	StoreIntermediateValue( analyteType, IMV_ICT_BLANK_FACTOR,  IMV_DIRECT_SET_METHOD, factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_BLANK_FACTOR_IDX] );
	StoreIntermediateValue( analyteType, IMV_ICT_TEST_FACTOR_2, IMV_DIRECT_SET_METHOD, factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_2_IDX] );
	StoreIntermediateValue( analyteType, IMV_ICT_TEST_FACTOR_1, IMV_DIRECT_SET_METHOD, factorsLimitsPtr->ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_1_IDX] );

	// end IMV


	return	CalcBfdStdBarcodeCorrectedResult(avgIctericCorrected, bfdIdx, analyteType, STD_SLOPE1_IDX, STD_INTERCEPT1_IDX);
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdFalseRateBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate rate analyte result value using absorbance slope rate
//		by computing false rate correction on bfd corrected rate &
//		using standard barcode slope & intercept  
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

float CalcBfdFalseRateBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType, float bfdFalseRateCorrectionLimit, float bfdFalseRateCorrectionFactor, float falseRate)
{
	float	bfdCorrected;
	float	slope;
	float	intercept;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	// compute false rate correction on bfd corrected rate value

	if (falseRate > bfdFalseRateCorrectionLimit)
	{
		bfdCorrected +=  bfdFalseRateCorrectionFactor * falseRate;
	}


	// IMV - store false rate, rate correction params

	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_CORR_TOGGLE, IMV_DIRECT_SET_METHOD, bfdFalseRateCorrectionLimit );
	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_CORR_FACTOR, IMV_DIRECT_SET_METHOD, bfdFalseRateCorrectionFactor );

	if (falseRate <= bfdFalseRateCorrectionLimit)
	{
		bfdFalseRateCorrectionFactor = 0.0;
	}

	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_CORRECTION, IMV_DIRECT_SET_METHOD, bfdFalseRateCorrectionFactor * falseRate );
	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_CORR_RATE,  IMV_DIRECT_SET_METHOD, bfdCorrected );

	// end IMV


	slope     = GetBarcodeParameter(analyteType, STD_SLOPE1_IDX);
	intercept = GetBarcodeParameter(analyteType, STD_INTERCEPT1_IDX);

	return	(bfdCorrected * slope) + intercept;
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdQuadraticBarcodeCorrectedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic endpoint or rate analyte result value using quadratic for absorbance average or slope
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

float CalcBfdQuadraticBarcodeCorrectedResult(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType)
{
	float	bfdCorrected;
	float	A0;
	float	A1;
	float	A2;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	A0 = GetBarcodeParameter(analyteType, QUADRATIC_A0_IDX);
	A1 = GetBarcodeParameter(analyteType, QUADRATIC_A1_IDX);
	A2 = GetBarcodeParameter(analyteType, QUADRATIC_A2_IDX);


	// IMV - set final A0, A1, A2 intermediate values used in calculation

	StoreIntermediateValue( analyteType, IMV_BARCODE_A0_USED, IMV_DIRECT_SET_METHOD, A0 );
	StoreIntermediateValue( analyteType, IMV_BARCODE_A1_USED, IMV_DIRECT_SET_METHOD, A1 );
	StoreIntermediateValue( analyteType, IMV_BARCODE_A2_USED, IMV_DIRECT_SET_METHOD, A2 );
		
	// end IMV


	return	( (A0 * bfdCorrected * bfdCorrected) + (A1 * bfdCorrected) + A2 );
}


// ********************************************************************
// FUNCTION:
//
//		CalcBfdSigmoidLogitLog4Result() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate generic rate analyte result value using sigmoid logit/log4 method
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

float CalcBfdSigmoidLogitLog4Result(float absAvgOrRate, unsigned bfdIdx, unsigned char analyteType, unsigned long *rateError)
{
	float	bfdCorrected;
	float	bcTop;
	float	bcBottom;
	float	bcSlope;
	float	bcIntercept;
	float	numerator;
	float	denominator;
	float	transform;
	float	sigmoidLogitLog4esult;

	bfdCorrected = BfdCalAdjust(absAvgOrRate, bfdIdx);

	bcTop       = GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_TOP_BASE_IDX)       + GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_TOP_FINE_IDX);	
	bcBottom    = GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_BOTTOM_BASE_IDX)    + GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_BOTTOM_FINE_IDX);	
																				   
	bcSlope     = GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_SLOPE_BASE_IDX)     + GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_SLOPE_FINE_IDX);	
	bcIntercept = GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_INTERCEPT_BASE_IDX) + GetBarcodeParameter(analyteType, SIGMOID_LOGIT_LOG4_INTERCEPT_FINE_IDX);	

	numerator   = bcTop  - bfdCorrected;
	denominator = bfdCorrected - bcBottom;
		 
	// check for low rate

	if (denominator <= 0.0)
	{
		*rateError |= (SIGMOID_LOGIT_LOG4_RATE_LOW | BELOW_SYSTEM_RANGE);
		rotorResults_g->rotorAnalyteResult[ analyteType ].resultPrintFlags |= MASK_LOW_SYSTEM_LIMIT;
		return NOVAL_NEG_FLOAT;
	}

	// check for high rate

	if (numerator <= 0.0)
	{
		*rateError |= (SIGMOID_LOGIT_LOG4_RATE_HIGH | ABOVE_SYSTEM_RANGE);
		rotorResults_g->rotorAnalyteResult[ analyteType ].resultPrintFlags |= MASK_HIGH_SYSTEM_LIMIT;
		return NOVAL_POS_FLOAT;
	}

	transform = log(numerator / denominator);

	sigmoidLogitLog4esult = -bcSlope * transform + bcIntercept;


	// IMV - store result calc params 

	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_SLOPE,       IMV_DIRECT_SET_METHOD, bcSlope );
	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_INTERCEPT,   IMV_DIRECT_SET_METHOD, bcIntercept );
	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_TOP,         IMV_DIRECT_SET_METHOD, bcTop );
	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_BOTTOM,      IMV_DIRECT_SET_METHOD, bcBottom );

	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_NUMERATOR,   IMV_DIRECT_SET_METHOD, numerator );
	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_DENOMINATOR, IMV_DIRECT_SET_METHOD, denominator );
	StoreIntermediateValue( analyteType, IMV_SIGMOID_LOGIT_LOG4_TRANSFORM,   IMV_DIRECT_SET_METHOD, transform );


	// end IMV


	return sigmoidLogitLog4esult;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRotorEndogenousZonesIndex() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check endogenous limits for this analyte based on endogenous indices
//		calculated for rotor in SAMPLE_BLANK algorithm		 
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

unsigned ProcessRotorEndogenousZonesIndex(float rotorEndogenousIndex, float *indexEndogenousZones, unsigned directionEndogenous) 
{

	// check endogenous index limits for within zones & process for analyteError & printCardFlags 

	if (rotorEndogenousIndex > indexEndogenousZones[MAX_ENDOGENOUS_ZONE_IDX])
	{
		// signal to set: no pre-char, suppressed result , no pre-char
		//                           , "HEM","LIP","ICT"
		  
		return ENDOGENOUS_SUPPRESSED;
	}
	else if (rotorEndogenousIndex > indexEndogenousZones[MID_ENDOGENOUS_ZONE_IDX]) 
	{
		if (directionEndogenous == ENDOGENOUS_GT)
		{
			// signal to set: pre-char,    result value      , post-char
			//                   '>'                           'H','L','I'

			return ENDOGENOUS_GT_RESULT;			
		}
		else
		{
			// signal to set: pre-char,    result value      , post-char
			//                   '<'                           'H','L','I'

			return ENDOGENOUS_LT_RESULT;
		} 
		 
	}
	else if (rotorEndogenousIndex > indexEndogenousZones[MIN_ENDOGENOUS_ZONE_IDX])  
	{
		// signal to set: no pre-char, result value      , post-char only
		//                                               , 'H','L','I'

		return ENDOGENOUS_RESULT;
	}

	// signal to set: no endogenous error type

	return ENDOGENOUS_NO_ERROR;
}


// ********************************************************************
// FUNCTION:
//
//		EndogenousLimitsCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check endogenous limits for this analyte based on endogenous indices
//		calculated for rotor in SAMPLE_BLANK algorithm		 
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

void EndogenousLimitsCheck(unsigned char analyteType, CommonFactorsLimits_t *factorsLimitsPtr, unsigned long *analyteError)
{
	unsigned	endogenousErrorType;

	// hemolyzed, lipemic & icteric resultPrintFlags masks tables

	const	ResultPrintFlags_t	hemolyzedPrintMasks[] = 
	{
		0,							// 0: ENDOGENOUS_NO_ERROR
		MASK_HEMOLYZED,				// 1: ENDOGENOUS_RESULT
		MASK_HEMOLYZED_LOW,			// 2: ENDOGENOUS_LT_RESULT
		MASK_HEMOLYZED_HIGH,		// 3: ENDOGENOUS_GT_RESULT
		MASK_HEMOLYZED_SUPPRESSED,	// 4: ENDOGENOUS_SUPPRESSED
	};

	const	ResultPrintFlags_t	lipemicPrintMasks[] = 
	{
		0,							// 0: ENDOGENOUS_NO_ERROR
		MASK_LIPEMIC,				// 1: ENDOGENOUS_RESULT
		MASK_LIPEMIC_LOW,			// 2: ENDOGENOUS_LT_RESULT
		MASK_LIPEMIC_HIGH,			// 3: ENDOGENOUS_GT_RESULT
		MASK_LIPEMIC_SUPPRESSED,	// 4: ENDOGENOUS_SUPPRESSED
	};

	const	ResultPrintFlags_t	ictericPrintMasks[] = 
	{
		0,							// 0: ENDOGENOUS_NO_ERROR
		MASK_ICTERIC,				// 1: ENDOGENOUS_RESULT
		MASK_ICTERIC_LOW,			// 2: ENDOGENOUS_LT_RESULT
		MASK_ICTERIC_HIGH,			// 3: ENDOGENOUS_GT_RESULT
		MASK_ICTERIC_SUPPRESSED,	// 4: ENDOGENOUS_SUPPRESSED
	};


	// DLR - EndogenousLimitsCheck() only called in 5 spots so not too bad


	// check for hemolyzed error condition
	
	endogenousErrorType = ProcessRotorEndogenousZonesIndex(rotorResults_g->rotorInformationResults.rotorHemolyzedIndex, factorsLimitsPtr->hemolyzedLimit, factorsLimitsPtr->hemolyzedDirection);

	if (endogenousErrorType != ENDOGENOUS_NO_ERROR)
	{
		*analyteError |= HEMOLYZED_LIMIT_ERROR;

		rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= hemolyzedPrintMasks[endogenousErrorType];
	}

	// check for lipemic error condition

	endogenousErrorType = ProcessRotorEndogenousZonesIndex(rotorResults_g->rotorInformationResults.rotorLipemicIndex, factorsLimitsPtr->lipemicLimit, factorsLimitsPtr->lipemicDirection);

	if (endogenousErrorType != ENDOGENOUS_NO_ERROR)
	{
		*analyteError |= LIPEMIC_LIMIT_ERROR;

		rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= lipemicPrintMasks[endogenousErrorType];
	}

	// check for icteric error condition

	endogenousErrorType = ProcessRotorEndogenousZonesIndex(rotorResults_g->rotorInformationResults.rotorIctericIndex, factorsLimitsPtr->ictericLimit, factorsLimitsPtr->ictericDirection);

	if (endogenousErrorType != ENDOGENOUS_NO_ERROR)
	{
		*analyteError |= ICTERIC_LIMIT_ERROR;

		rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= ictericPrintMasks[endogenousErrorType];
	}
}


// ********************************************************************
// FUNCTION:
//
//		ResultLimitsCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check analyteResult calculated verus limits for system and dynamic range		 
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

void ResultLimitsCheck(unsigned char analyteType, float analyteResult, float lowSystemRangeLimit, float lowDynamicRangeLimit, float highDynamicRangeLimit, float highSystemRangeLimit, unsigned long *analyteError)
{
	if (analyteResult < lowSystemRangeLimit)
	{
		*analyteError |= BELOW_SYSTEM_RANGE;
		rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_LOW_SYSTEM_LIMIT;

		// inc counter of analyte results below system range that are not derived and are printed per ROC file for rotor
		
		if (( commonAlgGlobals_g->derivedResult == FALSE ) && ( rotorConfig_g->analyteCalcRecord[analyteType].printOrder != NO_PRINT_ORDER_FLAG )) 
		{
			commonAlgGlobals_g->numAnalytesBelowDynamicRange++;
		}
	}
	else if	(analyteResult > highSystemRangeLimit)
	{
		*analyteError |= ABOVE_SYSTEM_RANGE;
		rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_HIGH_SYSTEM_LIMIT;
	}
	else if	(analyteResult < lowDynamicRangeLimit)
	{
		*analyteError |= LOW_DYNAMIC_RANGE;
 		if (!commonAlgGlobals_g->controlType)
 		{
 			rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_LOW_DYNAMIC_LIMIT;

			if ( commonAlgGlobals_g->useMaskLowDynamicSpecial == TRUE )
			{
				// if special type flag, then set resultPrintFlags for no '*' on print card
				rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_LOW_DYNAMIC_SPECIAL;
			}
 		}

		// inc counter of analyte results below dynamic range that are not derived and are printed per ROC file for rotor
		
		if (( commonAlgGlobals_g->derivedResult == FALSE ) && ( rotorConfig_g->analyteCalcRecord[analyteType].printOrder != NO_PRINT_ORDER_FLAG )) 
		{
			commonAlgGlobals_g->numAnalytesBelowDynamicRange++;
		}
	}
	else if	(analyteResult > highDynamicRangeLimit)
	{
		*analyteError |= HIGH_DYNAMIC_RANGE;
 		if (!commonAlgGlobals_g->controlType)
 		{
			rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_HIGH_DYNAMIC_LIMIT;

			if ( commonAlgGlobals_g->useMaskHighDynamicSpecial == TRUE )
			{
				// if special type flag, then set resultPrintFlags for no '*' on print card
				rotorResults_g->rotorAnalyteResult[analyteType].resultPrintFlags |= MASK_HIGH_DYNAMIC_SPECIAL;
			}
 		}
	}
}


// ********************************************************************
// FUNCTION:
//
//		LoadAnalyteAlgData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load algorithm specific configuration from analyte ROC files 
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

void LoadAnalyteAlgData(unsigned char analyteType)
{
	// test code to be replaced by function call set up for XML file loader 
	//
	// - (potentially in future)
	//


	// IMV - initialize imvCounters / misc set params

	StoreAlgVersionNumImv( analyteType );

	InitAnalyteImv();

	// end IMV


	// clear use low and high dynamic range special flags to allow '*' print, by default for analtyes

	commonAlgGlobals_g->useMaskLowDynamicSpecial = FALSE;
	commonAlgGlobals_g->useMaskHighDynamicSpecial = FALSE;


	// set up default algorithm calculation error mask 

	commonAlgGlobals_g->calcErrorMask = CALC_ERROR_MASK;

	// load algorithm specific values into alg table

	switch (analyteType)
	{
		case SYSTEM_CHECK:
			systemCheckConfig_g.sampleTypeMixCuv11 = 21;
			systemCheckConfig_g.sampleTypeFillCuv28 = 22;

			systemCheckConfig_g.sampleDiluentLowRatioLimit = -1.0;	
			systemCheckConfig_g.sampleDiluentHighRatioLimit = 1.65;
			systemCheckConfig_g.sampleDiluentLowDeltaLimit = -0.025;
			systemCheckConfig_g.sampleDiluentHighDeltaLimit = 0.008;
			systemCheckConfig_g.sampleDiluentToggle = -2.0;
			systemCheckConfig_g.sampleCuvLimit = 0.01;

			//  replace system check limits for rotors with T4 analyte

			if (rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_SYSTEM_CHECK_T4_FLAG])
			{
				systemCheckConfig_g.sampleDiluentLowRatioLimit = 0.3;	
				systemCheckConfig_g.sampleDiluentHighRatioLimit = 3.0;
				systemCheckConfig_g.sampleDiluentLowDeltaLimit = -0.030;
				systemCheckConfig_g.sampleDiluentHighDeltaLimit = 0.008;
				systemCheckConfig_g.sampleDiluentToggle = 0.05;
				systemCheckConfig_g.sampleCuvLimit = 0.0;
			}

			systemCheckConfig_g.plasticAbsCuv28 = 0.0044;
			systemCheckConfig_g.plasticAbsCuv11 = 0.0128;
			systemCheckConfig_g.plasticAbsCuv6 = 0.0090;

			systemCheckConfig_g.wbCheckLimit = 0.8;

// DLR - per Audie input
//			systemCheckConfig_g.offsetLevelMin = 0;
			systemCheckConfig_g.offsetLevelMin = 100;
// DLR - per Audie input
//			systemCheckConfig_g.offsetLevelMax = 1000;
			systemCheckConfig_g.offsetLevelMax = 1200;
			systemCheckConfig_g.offsetLevelMask = 0x01ff;
// DLR - per Audie input
//			systemCheckConfig_g.offsetSdLimit = 40.0;
//			systemCheckConfig_g.offsetSdLimit = 100.0;
//			systemCheckConfig_g.offsetSdLimit = 600.0;
			systemCheckConfig_g.offsetSdLimit = 150.0;
			systemCheckConfig_g.opticalDacCvLimit = 3.7;

			systemCheckConfig_g.temperatureCheckStartIdx = 4;
			systemCheckConfig_g.temperatureCheckMidIdx = 10;
			systemCheckConfig_g.temperatureCheckEndIdx = 16;
			systemCheckConfig_g.temperatureSwingLimit = 1.0;
			systemCheckConfig_g.maxTemperatureAllowed = 39.0;
			systemCheckConfig_g.minTemperatureAllowed = 35.0;
			break;

		case SAMPLE_BLANK:
			sampleBlankConfig_g.sampleTypeDiluent = 20;
			sampleBlankConfig_g.beadMixCheckLimit = 0.45;
			sampleBlankConfig_g.beadMixCheckPass = 7;
			sampleBlankConfig_g.startAbsCheckPass = 1;
			sampleBlankConfig_g.startAbsLimit = 3.0;
			sampleBlankConfig_g.avgWavelengthPair[0] = WP_340_850;
			sampleBlankConfig_g.avgWavelengthPair[1] = WP_405_850;
			sampleBlankConfig_g.avgWavelengthPair[2] = WP_467_850;
			sampleBlankConfig_g.avgStartPass = 1;
			sampleBlankConfig_g.avgEndPass = 3;
			sampleBlankConfig_g.beadMissingCheckLimit = 0.70;
			sampleBlankConfig_g.distributionCheckLimit = 0.19;
			break;

		case RQC:
			rqcConfig_g.sampleTypeAnalyte = 23;
			rqcConfig_g.beadMixCheckLimit = 0.85;
			rqcConfig_g.beadMixCheckPass = 1;
			rqcConfig_g.avgWavelengthPair = WP_515_600;
			rqcConfig_g.avgStartPass = 9;
			rqcConfig_g.avgEndPass = 11;
			rqcConfig_g.bfdCalibrationFactorIndex = 24;
			rqcConfig_g.rqcRatioSuppressLimit = 0.5;
			rqcConfig_g.rqcHighSuppressLimit = 1.3;
			rqcConfig_g.beadMissingCheckLimit = 1.5;
			rqcConfig_g.distributionCheckLimit = 0.0;
			break;

		case IQC:
			iqcConfig_g.sampleTypeIqcA = 24;
			iqcConfig_g.iqcACuvetteNumber = 4;
			iqcConfig_g.iqcAPathLength = 5.0;

			iqcConfig_g.sampleTypeIqcB = 25;
			iqcConfig_g.iqcBCuvetteNumber = 5;
			iqcConfig_g.iqcBPathLength = 4.3;

			iqcConfig_g.iqcLevel1Min = 0.90;
			iqcConfig_g.iqcLevel1Max = 1.10;
			
			iqcConfig_g.iqcLevel2Min = 0.95;
			iqcConfig_g.iqcLevel2Max = 1.05;
			iqcConfig_g.iqcPrecisionMin = 1.00;
			iqcConfig_g.iqcPrecisionMax = 1.05;
			iqcConfig_g.iqcPrecisionLimit = 0.05;
			iqcConfig_g.iqcRatioSigma = 4.5;
			iqcConfig_g.iqcLowAbsorbanceLimit = 0.1;

			iqcConfig_g.beadMissingCheckLimitIqcA = 1.5;
			iqcConfig_g.beadMissingCheckLimitIqcB = 1.5;
			iqcConfig_g.distributionCheckLimitIqcA = 0.3;
			iqcConfig_g.distributionCheckLimitIqcB = 0.3;
			break;

		case AG:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x000000F0;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = -85.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 0.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 93.5;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 115.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case ALB_BCG:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 26;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.65;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_500;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = NOVAL_POS_FLOAT; 		// post process WP_405_500 limit/IMV = 1.0
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 3;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_630_405;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = NO_ANALYTE_ERROR;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_630_500;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = RATE_IN_ENDPOINT_08;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionWavelengthPair = WP_850_NULL;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEndPass = 13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionSexSpecies = USED_DOG_FACTORS;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionLimit = 0.02;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityCorrectedStartPassAvg = 11;			
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityCorrectedEndPassAvg = 13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 4;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 8;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = PATIENT_TYPE_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.30;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c8c;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 3200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 3200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 3200.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 10.5;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 10.5;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 10.5;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.2;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 6.5;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 10.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case ALB_BCP:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 26;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.65;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 3;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_600_550;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT_10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_600_630;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = RATE_IN_ENDPOINT_08;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 2;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 3;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResultCalcMethod = ENDPOINT_2AVG_CALC_METHOD;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult1Factor = -.3;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult2Factor = 2.3;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareType = RESULT_COMPARE_DELTA;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareLowLimit = -4.5;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareHighLimit = 3.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.checkEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.wavelengthPairs[0] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.wavelengthPairs[1] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.checkPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.blankCorrectionOrder = ANALYTE_MINUS_BLANK;
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[0], "7405AA7" );
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[1], "7493AA4" );
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[2], "NO LOT#" );
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.highMinLimit = 3.9;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.lowMinLimit = 1.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.30;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408cd8;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 4200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 4200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 4200.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.2;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 6.5;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 10.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 4.5;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 1.1;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 4.275;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.45;
			break;

		case ALB_BCP_A:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 26;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.65;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 3;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_600_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT_10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_600_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.06;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = RATE_IN_ENDPOINT_10;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;
		
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 11;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 11;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResultCalcMethod = ENDPOINT_2AVG_CALC_METHOD;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult1Factor = 2.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult2Factor = 0.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareType = RESULT_COMPARE_DELTA;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareLowLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareHighLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.checkEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.wavelengthPairs[0] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.wavelengthPairs[1] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.checkPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.blankCorrectionOrder = ANALYTE_MINUS_BLANK;
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[0], "7405AA7" );
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[1], "7493AA4" );
			strcpy( endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.mfgLots[2], "NO LOT#" );
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.highMinLimit = 3.9;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.lowMinLimit = 1.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.30;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408cd8;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 4200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 4200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 4200.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.2;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 6.5;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 10.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 4.5;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 1.1;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 4.275;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.45;
			break;

		case ALP:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 27;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.65;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = NO_ANALYTE_ERROR;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_500_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.04;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.002;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 3600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = (0.0065 / DAYS_PER_MONTH);

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 360.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.14;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef10ac1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -20.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 2400.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 4000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 73.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 17.5;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 73.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 45.0;
			break;

		case ALT_1:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 28;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.80;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = 1.38;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.25;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.06;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0031;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 4600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 4000.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 300.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2200.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 12.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 12.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 12.0;
			break;

		case ALT_2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 66;		// added sampleType for second ALT_2 cuvette

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.80;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = 1.38;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.33;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.25;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.06;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0031;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 4600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 4000.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 300.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2200.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 12.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 12.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 12.0;
			break;

		case ALT:	
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG_1_CUV_OK;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 25.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.cuvOkSuppressionMask = 0xe190801c;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckToggle = 100.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckMax = 2000.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.resultDiffLimitScale = 0.25;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90cc1c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 2200.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 2200.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 2200.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 12.5;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 12.5;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 12.0;
			break;

		case AMY:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 29;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.70;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = NO_ANALYTE_ERROR;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_500_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 2900.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 21;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 600.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.07;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef108c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 26.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 26.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 26.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 4000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5500.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 36.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 11.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 36.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 29.0;
			break;

		case AST_1:
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 30;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.80;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.30;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.02;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.001;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 5300.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 300.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 430.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 430.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 430.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1436.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1436.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1436.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 16.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 16.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 10.0;
			break;

		case AST_2:
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 76;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.80;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.41;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.30;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.02;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.001;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 5300.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 300.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 430.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 430.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 430.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1436.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1436.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1436.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 16.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 16.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 10.0;
			break;

		case AST:	
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG_1_CUV_OK;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 25.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.cuvOkSuppressionMask = 0xe190801c;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckToggle = 100.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckMax = 2000.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.resultDiffLimitScale = 0.25;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90cc1c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 430.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 430.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 430.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 1436.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 1436.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 1436.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -15.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 16.5;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 4.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 16.5;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 10.0;
			break;

		case BA_1:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 31;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 1;
			rateConfig_g.rateAlgRecord.startAbsLimit = 2.8;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_3;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 1;							// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;	// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;			// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 10;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c00;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1200.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1200.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 140.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;

			// update BA_1 pass 13 & 7 threshold based on ROC algorithm flag

			if (rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_RATE_THRESHOLD_FLAG])
			{
				rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.35;
				rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.35;
			}
			break;

		case BA_2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 32;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 1;
			rateConfig_g.rateAlgRecord.startAbsLimit = 2.8;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.35;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.35;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_3;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 1;							// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;	// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;			// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 10;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c00;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1200.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1200.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 140.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case BA:
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_BLEND;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = 45.0;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = 55.0;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = FALSE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = NOVAL_NEG_FLOAT;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef908c00;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1200.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1200.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1200.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 140.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case BUN_1:
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 33;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.70;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_40;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = RATE_WINDOW_5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.30;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.07;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0035;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_04;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 5;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = 0.1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = 0.0145;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 3600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = (0.0025 / DAYS_PER_MONTH);

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 27.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb0ac1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 4200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 4200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 4200.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -2.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 2.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 180.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 600.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 13.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 14.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 13.5;
			break;

		case BUN_2:
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 67;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.70;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_80;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_20;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 0.34;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = RATE_WINDOW_5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 0.30;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.07;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0035;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_08;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 5;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = 0.1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = 0.0145;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 3600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = (0.0025 / DAYS_PER_MONTH);

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 27.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb0ac1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 4200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 4200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 4200.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -2.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 2.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 180.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 600.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 13.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 14.5;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 13.5;
			break;

		case BUN:	
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG_1_CUV_OK;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 5.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.cuvOkSuppressionMask = 0xe1b0a01c;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckToggle = 50.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckMax = 180.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.resultDiffLimitScale = 0.2;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb0ec1c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 4200.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 4200.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 4200.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -2.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 2.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 180.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 600.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 13.5;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 14.5;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 13.5;
			break;

		case CA:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 34;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.7;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_405_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_600_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = 3.1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 4;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_405_467;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.015;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT_20;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_600_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.12;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = RATE_IN_ENDPOINT_10;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 28;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 23;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResultCalcMethod = ENDPOINT_2PARABOLA_METHOD;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult1Factor = 1.6;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult2Factor = 0.4;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareType = RESULT_COMPARE_RATIO;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareToggle = 12.5;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareLowLimit = 0.7;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareHighLimit = 1.3;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.wavelengthRatioCheck.checkEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.14;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408cf0;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2000.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2000.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1500.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 26.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 26.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 26.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -4.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 4.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 16.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 20.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 9.3;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.86;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 9.1;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.65;
			break;

		case CHOL:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 36;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.72;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.72;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_340_405;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 1.0;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.02;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.02;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
		
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.20;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.16;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff408c08;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1400.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 3900.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 3900.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 3900.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 25.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 25.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 25.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -10.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 20.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 520.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 800.0;

			// write over config record based on rotor ROC file algorithm flags

			// set pass info based on rotor ROC algorithm flags

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_AVG_START_PASS_FLAG];
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass   = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_AVG_END_PASS_FLAG];

			// set type of barcode factor slope/intercept correction based on ROC algorithm flag

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_SLOPE_INT_METHOD_FLAG];

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 178.0;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 34.0;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 178.0;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 60.0;
			break;

		case CHW:
			posNegConfig_g.posNegAlgRecord.sampleTypeAnalyte = 70;
			posNegConfig_g.posNegAlgRecord.sampleTypeOpt = 73;
			posNegConfig_g.posNegAlgRecord.optCuvetteNumber = 5;

			posNegConfig_g.posNegAlgRecord.beadMixCheckLimitAnalyte = 0.4;
			posNegConfig_g.posNegAlgRecord.beadMixCheckLimitOpt = 0.7;
			posNegConfig_g.posNegAlgRecord.beadMixCheckPass = 1;

			posNegConfig_g.posNegAlgRecord.startAbsCheck[0].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[0].startAbsSampleType = 70;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[0].startAbsCheckWavelengthPair = WP_515_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[0].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[1].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[1].startAbsSampleType = 70;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[1].startAbsCheckWavelengthPair = WP_600_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[1].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[2].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[2].startAbsSampleType = 70;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[2].startAbsCheckWavelengthPair = WP_550_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[2].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[3].startAbsCheckMethodHigh = FALSE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[3].startAbsSampleType = 70;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[3].startAbsCheckWavelengthPair = WP_550_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[3].startAbsLimit = 0.5;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[4].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[4].startAbsSampleType = 73;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[4].startAbsCheckWavelengthPair = WP_515_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[4].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[5].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[5].startAbsSampleType = 73;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[5].startAbsCheckWavelengthPair = WP_600_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[5].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[6].startAbsCheckMethodHigh = TRUE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[6].startAbsSampleType = 73;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[6].startAbsCheckWavelengthPair = WP_550_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[6].startAbsLimit = 3.0;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[7].startAbsCheckMethodHigh = FALSE;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[7].startAbsSampleType = 73;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[7].startAbsCheckWavelengthPair = WP_550_850;
			posNegConfig_g.posNegAlgRecord.startAbsCheck[7].startAbsLimit = 0.5;
			posNegConfig_g.posNegAlgRecord.startAbsCheckPass = 1;

			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.blankCorrectionOrderAnalyte = ANALYTE_MINUS_BLANK;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.blankCorrectionOrderOpt = NO_BLANK_CORRECTION;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioWavelengthPair[0] = WP_515_850;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioWavelengthPair[1] = WP_600_850;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioAvgStartPass[0] = 1;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioAvgEndPass[0] = 5;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioAvgStartPass[1] = 10;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioAvgEndPass[1] = 13;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioHighLimit = 0.61;
			posNegConfig_g.posNegAlgRecord.posNegCalcRecord.ratioLowLimit = -0.01;

			posNegConfig_g.posNegAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			posNegConfig_g.posNegAlgRecord.beadMissingCheckLimitOpt = 0.20;
			posNegConfig_g.posNegAlgRecord.distributionCheckLimitAnalyte = 0.14;
			posNegConfig_g.posNegAlgRecord.distributionCheckLimitOpt = 0.14;

			posNegConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff008cc0 | CHW_NOT_CANINE_OR_CONTROL;

			posNegConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2500.0;
			posNegConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2500.0;
			posNegConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2500.0;
			posNegConfig_g.algFactorsLimits.lipemicLimit[0]   = 3000.0;
			posNegConfig_g.algFactorsLimits.lipemicLimit[1]   = 3000.0;
			posNegConfig_g.algFactorsLimits.lipemicLimit[2]   = 3000.0;
			posNegConfig_g.algFactorsLimits.ictericLimit[0]   = 20.0;
			posNegConfig_g.algFactorsLimits.ictericLimit[1]   = 20.0;
			posNegConfig_g.algFactorsLimits.ictericLimit[2]   = 20.0;

			posNegConfig_g.algFactorsLimits.lowSystemLimit = NOVAL_NEG_FLOAT;
			posNegConfig_g.algFactorsLimits.lowDynamicRange = NOVAL_NEG_FLOAT;
			posNegConfig_g.algFactorsLimits.highDynamicRange = NOVAL_POS_FLOAT;
			posNegConfig_g.algFactorsLimits.highSystemLimit = NOVAL_POS_FLOAT;

			posNegConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case CK_1:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 37;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = FALSE;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 5150.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 2600.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = HEMOLYZED_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 2100.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.16;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef108c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 42.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 42.0;
 			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 42.0;

			rateConfig_g.algFactorsLimits.hemolyzedCorrectionFactors[ENDOGENOUS_RESULT_CORRECTION_IDX] = -0.185;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 14000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 15000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 20.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 8.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 60.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 80.0;
			break;

		case CK_2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 77;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = START_ABS_BAD;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = FALSE;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 5150.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 2600.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = HEMOLYZED_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 2100.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.16;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef108c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 42.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 42.0;
 			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 42.0;

			rateConfig_g.algFactorsLimits.hemolyzedCorrectionFactors[ENDOGENOUS_RESULT_CORRECTION_IDX] = -0.185;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 14000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 15000.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 20.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 8.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 60.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 80.0;
			break;

		case CK:	
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG_1_CUV_OK;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 50.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.cuvOkSuppressionMask = 0xe110801c;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckToggle = 200.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckMax = 5000.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.resultDiffLimitScale = 0.25;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef10cc1c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 42.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 42.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 42.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 14000.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 15000.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 20.0;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 8.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 60.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 80.0;
			break;

		case CL_1:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 38;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_40;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = RATE_ENDING_ABS_01;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_04;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 21;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = 0.000175;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 34.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 34.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 34.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 75.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 80.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 135.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 140.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 105.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 105.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 3.0;
			break;

		case CL_2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 39;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_80;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_20;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = RATE_ENDING_ABS_02;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_08;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 21;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = 0.000175;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 34.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 34.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 34.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 75.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 80.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 135.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 140.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 105.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 105.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 3.0;
			break;

		case CL:
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 13.0;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 34.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 34.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 34.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = 75.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 80.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 135.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 140.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 105.0;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 5.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 105.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 3.0;
			break;

		case CRE_1:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 41;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.60;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.60;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 8;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_550_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 8;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 12;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_550_600;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 17;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionStabilityRate = (0.007 / DAYS_PER_MONTH);

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.2;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.2;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.16;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff00ac00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 688.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 688.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 688.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 3000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 3000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 3000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 7.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 7.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 7.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.2;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 20.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 30.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 0.83;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.26;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 0.83;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.28;
			break;

		case CRE_2:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 69;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.60;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.60;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 8;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_550_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 8;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 12;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_550_600;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 17;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionStabilityRate = (0.007 / DAYS_PER_MONTH);

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.2;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.2;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.16;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff00ac00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 688.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 688.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 688.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 3000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 3000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 3000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 7.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 7.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 7.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.2;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 20.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 30.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 0.83;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.26;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 0.83;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.28;
			break;

		case CRE:	
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG_1_CUV_OK;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 2.0;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.cuvOkSuppressionMask = 0xf100a000;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckToggle = NOVAL_POS_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.inconsistentCheckMax = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.avg1CuvOkCheckData.resultDiffLimitScale = NOVAL_NEG_FLOAT;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff00ec00;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 688.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 688.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 688.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 3000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 3000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 3000.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 7.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 7.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 7.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 0.2;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 20.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 30.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 0.83;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 0.26;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 0.83;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 0.28;
			break;

		case CRP_1:
			// set flag for no '*', if < low dynamic range limit
			commonAlgGlobals_g->useMaskLowDynamicSpecial = TRUE;

			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 71;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.1;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_630_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeStartPass = 10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeEndPass = 13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeLessThanLimit = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeLimit = -0.5;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeErrorMask = ABOVE_SYSTEM_RANGE | RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = MAX_BFD_CALS;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = BASE_FINE_EXP;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 750.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 200.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case CRP_2:
			// set flag for no '*', if < low dynamic range limit
			commonAlgGlobals_g->useMaskLowDynamicSpecial = TRUE;

			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 72;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.1;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_630_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeStartPass = 10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeEndPass = 13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeLessThanLimit = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeLimit = -0.5;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeErrorMask = ABOVE_SYSTEM_RANGE | RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = MAX_BFD_CALS;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = BASE_FINE_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 750.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 200.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;

			if ( rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[0] > 0 )
			{
				endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			}
			break;

		case CRP:
			// set flag for no '*', if < low dynamic range limit
			commonAlgGlobals_g->useMaskLowDynamicSpecial = TRUE;

			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_BLEND_ZONES_CUV_2;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = 70.0;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = 90.0;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = FALSE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 20.0;
			calc2CuvConfig_g.calc2CuvRecord.blend1CuvOkCheckData.cuvSuppressionMask = 0xef408c00 | ABOVE_SYSTEM_RANGE; 
			calc2CuvConfig_g.calc2CuvRecord.blend1CuvOkCheckData.zone1Limit = 75.0; 
			calc2CuvConfig_g.calc2CuvRecord.blend1CuvOkCheckData.zone2Limit = 85.0; 

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef40cc0c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 750.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 750.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 750.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 750.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 750.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 750.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 35.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 35.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 35.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 200.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 250.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case CTBIL:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x0c000000;

			derivedConfig_g.algFactorsLimits.hemolyzedLimit[0] = 900.0;
			derivedConfig_g.algFactorsLimits.hemolyzedLimit[1] = 900.0;
			derivedConfig_g.algFactorsLimits.hemolyzedLimit[2] = 900.0;
			derivedConfig_g.algFactorsLimits.lipemicLimit[0]   = 1800.0;
			derivedConfig_g.algFactorsLimits.lipemicLimit[1]   = 1800.0;
			derivedConfig_g.algFactorsLimits.lipemicLimit[2]   = 1800.0;
			derivedConfig_g.algFactorsLimits.ictericLimit[0]   = 999999.0;
			derivedConfig_g.algFactorsLimits.ictericLimit[1]   = 999999.0;
			derivedConfig_g.algFactorsLimits.ictericLimit[2]   = 999999.0;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = -1.5;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 0.1;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 30.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 35.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case DBIL:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 43;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_467_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = BLANK_MINUS_ANALYTE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 12;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_467_550;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.08;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.063;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.005;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 13;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionStabilityRate = (0.004 / DAYS_PER_MONTH);

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.2;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.2;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.19;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xfd40ac80;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 200.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 200.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 600.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 600.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 600.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 999999.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 999999.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 999999.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -1.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.1;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 15.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 25.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 0.12;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.07;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 0.12;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.07;
			break;

		case EGFR:
			// set flag for no '*', if > high dynamic range limit
			commonAlgGlobals_g->useMaskHighDynamicSpecial = TRUE;

			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x000001ff;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = 1.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 2.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 60.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 5000.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case GGT:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 44;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.70;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = GT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = NO_ANALYTE_ERROR;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_500_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0025;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 5150.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 27;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = 5000.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = 450.0;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef108c1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 26.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 26.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 26.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 3000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 5500.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 75.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 30.0;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 75.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 45.0;
			break;

		case GLOB:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x000000c0;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = -10.5;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 0.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 13.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 15.2;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case GLU:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 45;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.74;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 9;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 8;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 10;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_340_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.08;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1790.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1790.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1790.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1950.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1950.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1950.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 37.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 37.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 10.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 700.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 1200.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 99.0;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 17.0;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 99.9;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 40.0;
			break;

		case HB:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 75;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_630_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = NO_BLANK_CORRECTION;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_630_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 16;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408000;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = NOVAL_POS_FLOAT;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = NOVAL_POS_FLOAT;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -10.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 25.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 28.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case HCT:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x000000c0;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = -10.0 / HCT_HB_RESULT_DIVISOR;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 5.0 / HCT_HB_RESULT_DIVISOR;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 25.0 / HCT_HB_RESULT_DIVISOR;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 28.0 / HCT_HB_RESULT_DIVISOR;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case HDL_1:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 46;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.74;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_550_630;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_467_630;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_550_630;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.6;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.05;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_467_630;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 6;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 14;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResultCalcMethod = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult1Factor = 0.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult2Factor = 0.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareType = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareLowLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareHighLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c0c;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 400.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = 10.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 15.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 100.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 110.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case HDL_2:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 47;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.74;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_550_630;
			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_467_630;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = NOVAL_POS_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_550_630;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.6;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.05;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_467_630;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = 0.6;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.13;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = 0.05;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointErrorMask = NO_ANALYTE_ERROR;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 6;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 14;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResultCalcMethod = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult1Factor = 0.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.weightedResult2Factor = 0.0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareType = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareToggle = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareLowLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.resultWavelengthCompareHighLimit = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c0c;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 400.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 400.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = 10.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 15.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 100.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 110.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case HDL:
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_BLEND;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = 65.0;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = 77.0;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = FALSE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = NOVAL_NEG_FLOAT;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c0c;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 400.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 400.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 400.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 1000.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = 10.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 15.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 100.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 110.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case K:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 48;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.75;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_340_405;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = DELTA_SUM_MULTIPLIER_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumIdxDiff = 2;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[0]  = 0.36;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[1]  = 0.56;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[2]  = 0.66;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[3]  = 0.66;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[4]  = 0.73;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[5]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[6]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[7]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[8]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[9]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[10] = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;

			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.16;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef008c18;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 0.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 1.5;
			rateConfig_g.algFactorsLimits.highDynamicRange = 8.5;
			rateConfig_g.algFactorsLimits.highSystemLimit = 12.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 4.15;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 0.49;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 4.4;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 0.49;
			break;

		case LAC:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 74;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_515_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_600_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 1;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_515_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.32;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 12;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 11;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 11;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 10;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 10;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 9;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 9;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].checkPass = 8;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].slopeNumPoints = 8;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].checkPass = 7;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].slopeNumPoints = 7;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].checkPass = 6;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].slopeNumPoints = 6;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].checkPass = 5;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].slopeNumPoints = 5;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].checkPass = 4;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectAbsorbanceThreshold = 1.32;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].slopeNumPoints = 4;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].checkPass = 3;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectAbsorbanceBlankCorrected = FALSE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectAbsorbanceThreshold = 3.0;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].slopeNumPoints = 3;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_515_600;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_600_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 15;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = BFD_ICTERIC_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb0ac18;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 500.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 500.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 500.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 3000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 3000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 3000.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 15.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 15.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 15.0;

			rateConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_BLANK_FACTOR_IDX]     = 0.0;
			rateConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_1_IDX] = -1.1743703E-2;
			rateConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_2_IDX] = 0.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 0.10;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 0.30;
			rateConfig_g.algFactorsLimits.highDynamicRange = 9.99;
			rateConfig_g.algFactorsLimits.highSystemLimit = 15.00;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case LDH:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 49;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_630_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_500_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;	// no mask in legacy
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 1;							// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;	// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;			// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_500_630;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_630_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.07;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0035;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 4000.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 22;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionLimit = 0.015;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionFactor = 2.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb08c18;

			rateConfig_g.algFactorsLimits.hemolyzedDirection = ENDOGENOUS_LT;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 50.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 100.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 150.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1720.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1720.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1720.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 64.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 64.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 64.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 8.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 50.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 1000.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 1800.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case LDL:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x0400000f;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = -21.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 0.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 520.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 785.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case MG:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 50;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = RATE_WINDOW_1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = RATE_WINDOW_2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = RATE_WINDOW_3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;	// no mask in legacy
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 3;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 1;							// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;	// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 3.0;			// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 1;						// not used
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.07;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0035;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 12;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = 0.1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = 0.0145;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 3600.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb0ac1c;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 1500.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 1500.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 1500.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 38.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 38.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 38.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -2.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 0.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 8.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 10.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 1.76;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 0.25;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 1.74;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 0.32;
			break;

		case NA_1:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 51;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.5;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_40;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 2.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = 0.2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = RATE_ENDING_ABS_01;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0031;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_04;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = 0.015;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = 0.83;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionWavelengthPair = WP_850_NULL;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionStartPass = 2;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEndPass = 13;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionRateLimit = -0.01;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionRateMultiplier = 0.83;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 1600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 9;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = 0.0001227;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff | NACL_NA_LOW_RECOVERY;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 25.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 25.0;
  			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 25.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 100.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 110.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 170.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 180.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 139.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 3.8;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 139.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 2.3;
			break;

		case NA_2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 52;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.5;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_500_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_80;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_20;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 2.5;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 10;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 9;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 7;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 6;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 5;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 4;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsLimit = 0.2;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 1.8;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = RATE_ENDING_ABS_02;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_405_500;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.05;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.0031;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR_08;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = TRUE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionLimit = 0.015;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionMultiplier = 0.83;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionWavelengthPair = WP_850_NULL;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionStartPass = 2;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEndPass = 13;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionRateLimit = -0.01;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionRateMultiplier = 0.83;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 1600.00;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 9;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = 0.0001227;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff | NACL_NA_LOW_RECOVERY;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2600.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2600.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 25.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 25.0;
  			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 25.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 100.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 110.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 170.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 180.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 139.0;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 3.8;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 139.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 2.3;
			break;

		case NA:
			calc2CuvConfig_g.calc2CuvRecord.calc2CuvType = CALC_2_CUV_AVG;
			calc2CuvConfig_g.calc2CuvRecord.startCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.endCuvBlendLimit = NOVAL_NEG_FLOAT;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultCheckEnabled = TRUE;
			calc2CuvConfig_g.calc2CuvRecord.inconsistentResultDiffLimit = 20.0;

			calc2CuvConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef90ccff | NACL_NA_LOW_RECOVERY;

			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2000.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[0]   = 2600.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[1]   = 2600.0;
			calc2CuvConfig_g.algFactorsLimits.lipemicLimit[2]   = 2600.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[0]   = 25.0;
			calc2CuvConfig_g.algFactorsLimits.ictericLimit[1]   = 25.0;
  			calc2CuvConfig_g.algFactorsLimits.ictericLimit[2]   = 25.0;

			calc2CuvConfig_g.algFactorsLimits.lowSystemLimit = 100.0;
			calc2CuvConfig_g.algFactorsLimits.lowDynamicRange = 110.0;
			calc2CuvConfig_g.algFactorsLimits.highDynamicRange = 170.0;
			calc2CuvConfig_g.algFactorsLimits.highSystemLimit = 180.0;

			calc2CuvConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleMean = 139.0;
			calc2CuvConfig_g.dilutionCheckFactors.shortSampleSigma = 3.8;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleMean = 139.0;
			calc2CuvConfig_g.dilutionCheckFactors.longSampleSigma = 2.3;
			break;

		case NHDLC:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x000000c1;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = 0.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 505.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 790.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case PHB:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 78;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.7;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 1;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_405_850;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_10;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_08;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 1;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.windowSelectRecord.selectWavelengthPair = WP_340_850;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectWavelengthPair = 0xff;
			rateConfig_g.rateAlgRecord.windowSelectRecord.derivedSelectThresholdMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.windowSelectRecord.compareThresholdType = LT_WINDOW_THRESHOLD;
			rateConfig_g.rateAlgRecord.windowSelectRecord.numCheckPasses = 11;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].checkPass = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceBlankCorrected = TRUE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectAbsorbanceThreshold = 1.60;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].selectedPassMask = 0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[0].slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].checkPass = 12;
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].selectedPassMask = 0;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[1].slopeNumPoints = 12;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].checkPass = 11;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[2].slopeNumPoints = 11;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].checkPass = 10;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].selectedPassMask = 0;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[3].slopeNumPoints = 10;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].checkPass = 9;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[4].slopeNumPoints = 9;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].checkPass = 8;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[5].slopeNumPoints = 8;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].checkPass = 7;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[6].slopeNumPoints = 7;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].checkPass = 6;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[7].slopeNumPoints = 6;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].checkPass = 5;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[8].slopeNumPoints = 5;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].checkPass = 4;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectAbsorbanceBlankCorrected = TRUE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectAbsorbanceThreshold = 1.60;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[9].slopeNumPoints = 4;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].checkPass = 3;							
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectAbsorbanceBlankCorrected = FALSE;	
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectAbsorbanceThreshold = 3.0;			
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].selectedPassMask = 0;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].slopeStartPass = 1;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.selectParams[10].slopeNumPoints = 3;						
			rateConfig_g.rateAlgRecord.windowSelectRecord.preEndAbsCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsCheckEnabled = TRUE;		
			rateConfig_g.rateAlgRecord.windowSelectRecord.postEndAbsLimit = 3.0;
			rateConfig_g.rateAlgRecord.windowSelectRecord.endAbsAnalyteErrorMask = END_ABS_BAD;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_405_850;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = WINDOW_LINEAR_REGRESSION_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseToggle = 0.15;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseRatioLimit = 0.060;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNoiseAbsoluteLimit = 0.009;
			rateConfig_g.rateAlgRecord.rateCalcRecord.rateNotLinearAnalyteErrorMask = RATE_NOT_LINEAR;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRatePointsToggle = 255;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateRatioLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateAbsoluteLimit = NOVAL_POS_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.arrhTemperatureCorrectionFactor = 2351.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = BASE_FINE_EXP;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateCorrectionMultiplier = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.endogenousResultCorrectionMethod = NO_ENDOGENOUS_RESULT_CORRECTION;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionEnabled = FALSE;
			rateConfig_g.rateAlgRecord.rateCalcRecord.linearTimeCorrectionStabilityRate = NOVAL_NEG_FLOAT;

			rateConfig_g.rateAlgRecord.rqcCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueCheckEnabled = FALSE;
			rateConfig_g.rateAlgRecord.unexpectedValueHighDynamicRangeAdjust = NOVAL_NEG_FLOAT;
			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xefb08c18;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1000.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1000.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 800.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 800.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 800.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 40.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 40.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 40.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = -5.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 60.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 80.00;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case PHOS:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 53;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.74;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 8;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 7;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 13;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_340_405;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.08;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.01;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 0;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1200.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1200.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1200.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1800.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1800.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1800.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 21.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 21.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 21.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.2;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 20.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 30.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 3.35;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.8;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 3.5;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 1.3;
			break;

		case T4:
			t4Config_g.t4AlgRecord.beadMixCheckLimitT4L1 = 0.70;
			t4Config_g.t4AlgRecord.beadMixCheckLimitG6P1 = 0.80;
			t4Config_g.t4AlgRecord.beadMixCheckPass = 1;

			t4Config_g.t4AlgRecord.t4HemComboLimit = 300.0;
			t4Config_g.t4AlgRecord.t4LipComboLimit = 300.0;

			t4Config_g.t4AlgRecord.startAbsCheckWavelengthPair = WP_340_850;
			t4Config_g.t4AlgRecord.startAbsCheckPass = 1;
			t4Config_g.t4AlgRecord.startAbsLimit = 2.6;

			t4Config_g.t4AlgRecord.t4CalcRecord.startPass = 1;
			t4Config_g.t4AlgRecord.t4CalcRecord.endPass = 12;
			t4Config_g.t4AlgRecord.t4CalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			t4Config_g.t4AlgRecord.t4CalcRecord.falseRateWavelengthPair = WP_405_850;
			t4Config_g.t4AlgRecord.t4CalcRecord.cuvetteNumberT4G6P1 = 8;
			t4Config_g.t4AlgRecord.t4CalcRecord.desiredPathLengthT4G6P1 = 4.3;
			t4Config_g.t4AlgRecord.t4CalcRecord.rateNoiseAbsoluteLimit = 0.002;
			t4Config_g.t4AlgRecord.t4CalcRecord.falseRateAbsoluteLimit = 0.008;
			t4Config_g.t4AlgRecord.t4CalcRecord.maxDeltaLimitT4G6P1 = 0.17;
			t4Config_g.t4AlgRecord.t4CalcRecord.arrhTemperatureCorrectionFactorT4L1 = 2100.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.arrhTemperatureCorrectionFactorT4G6P1 = 3500.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.bfdCalibrationFactorIndex = 0;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[0].sampleType = 54;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[0].wavelengthPair = WP_340_850;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[0].cuvetteNumber = 4;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[0].desiredPathLength = 5.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[1].sampleType = 54;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[1].wavelengthPair = WP_405_850;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[1].cuvetteNumber = 4;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[1].desiredPathLength = 5.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[2].sampleType = 55;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[2].wavelengthPair = WP_340_850;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[2].cuvetteNumber = 27;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[2].desiredPathLength = 5.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[3].sampleType = 55;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[3].wavelengthPair = WP_405_850;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[3].cuvetteNumber = 27;
			t4Config_g.t4AlgRecord.t4CalcRecord.t4SampleCheckAvg[3].desiredPathLength = 5.0;
			t4Config_g.t4AlgRecord.t4CalcRecord.sampleCheckLimit = 1.02;

			t4Config_g.t4AlgRecord.t4BeadDistCheck[0].cuvetteNumber = 3;	// dummy bead
			t4Config_g.t4AlgRecord.t4BeadDistCheck[0].beadMissingCheckLimit = 1.50;
			t4Config_g.t4AlgRecord.t4BeadDistCheck[0].distributionCheckLimit = 0.30;
			t4Config_g.t4AlgRecord.t4BeadDistCheck[1].cuvetteNumber = 8;	// T4_G6P1
			t4Config_g.t4AlgRecord.t4BeadDistCheck[1].beadMissingCheckLimit = 1.50;
			t4Config_g.t4AlgRecord.t4BeadDistCheck[1].distributionCheckLimit = 0.07;
			t4Config_g.t4AlgRecord.t4BeadDistCheck[2].cuvetteNumber = 26;	// sample blank
			t4Config_g.t4AlgRecord.t4BeadDistCheck[2].beadMissingCheckLimit = 0.70;
			t4Config_g.t4AlgRecord.t4BeadDistCheck[2].distributionCheckLimit = 0.07;

			t4Config_g.algFactorsLimits.analyteSuppressionMask = 0xff308024;

			t4Config_g.algFactorsLimits.hemolyzedLimit[0] = 900.0;
			t4Config_g.algFactorsLimits.hemolyzedLimit[1] = 900.0;
			t4Config_g.algFactorsLimits.hemolyzedLimit[2] = 900.0;
			t4Config_g.algFactorsLimits.lipemicLimit[0]   = 1000.0;
			t4Config_g.algFactorsLimits.lipemicLimit[1]   = 1000.0;
			t4Config_g.algFactorsLimits.lipemicLimit[2]   = 1000.0;
			t4Config_g.algFactorsLimits.ictericLimit[0]   = 15.0;
			t4Config_g.algFactorsLimits.ictericLimit[1]   = 15.0;
			t4Config_g.algFactorsLimits.ictericLimit[2]   = 15.0;

			t4Config_g.algFactorsLimits.lowSystemLimit = -1.0;
			t4Config_g.algFactorsLimits.lowDynamicRange = 0.5;
			t4Config_g.algFactorsLimits.highDynamicRange = 8.0;
			t4Config_g.algFactorsLimits.highSystemLimit = 10.0;

			t4Config_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case TBIL:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 59;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_467_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 1;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = BLANK_MINUS_ANALYTE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 9;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 11;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_467_550;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.034;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.0034;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 13;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.2;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.2;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.19;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xfd408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 420.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 420.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 420.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1500.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 999999.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 999999.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 999999.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -1.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.1;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 30.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 35.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 0.39;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 0.11;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 0.41;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.35;
			break;

		case TCH:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x4000000b;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = 0.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 1.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 35.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 80.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		case TCO2:	
			rateConfig_g.rateAlgRecord.sampleTypeAnalyte = 60;

			rateConfig_g.rateAlgRecord.beadMixCheckLimit = 0.80;
			rateConfig_g.rateAlgRecord.beadMixCheckPass = 2;

			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[0] = WP_340_850;
			rateConfig_g.rateAlgRecord.startAbsCheckWavelengthPair[1] = WP_340_405;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[0] = RATE_STARTING_ABS_80;
			rateConfig_g.rateAlgRecord.startAbsAnalyteErrorMask[1] = RATE_STARTING_ABS_40;
			rateConfig_g.rateAlgRecord.startAbsCheckPass = 2;
			rateConfig_g.rateAlgRecord.startAbsLimit = 3.0;

			rateConfig_g.rateAlgRecord.rateCalcRecord.rateStdErrorWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.falseRateWavelengthPair = WP_340_405;
			rateConfig_g.rateAlgRecord.rateCalcRecord.slopeRateCalcMethod = DELTA_SUM_MULTIPLIER_METHOD;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.slopeStartPass = 1;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.slopeNumPoints = 13;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[0]  = 0.04;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[1]  = 0.07;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[2]  = 0.14;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[3]  = 0.23;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[4]  = 0.59;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[5]  = 0.75;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[6]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[7]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[8]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[9]  = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[10] = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSumMulitiplier[11] = 1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.startAbsRangeLow  = 0.5;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.startAbsRangeHigh = 2.5;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSlopeRateMin = -1.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaSlopeRateMax = 0.0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaIncrSlopeMin = -0.22;	
			rateConfig_g.rateAlgRecord.rateCalcRecord.deltaSumMultRecord.deltaIncrSlopeMax =  0.20;	
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdCalibrationFactorIndex = 0;
			rateConfig_g.rateAlgRecord.rateCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			rateConfig_g.rateAlgRecord.rateCalcRecord.bfdFalseRateCorrectionEnabled = FALSE;

			rateConfig_g.rateAlgRecord.beadMissingCheckLimit = 0.20;
			rateConfig_g.rateAlgRecord.distributionCheckLimit = 0.19;

			rateConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef009cf0;

			rateConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1300.0;
			rateConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1300.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[0]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[1]   = 2200.0;
			rateConfig_g.algFactorsLimits.lipemicLimit[2]   = 2200.0;
			rateConfig_g.algFactorsLimits.ictericLimit[0]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[1]   = 17.0;
			rateConfig_g.algFactorsLimits.ictericLimit[2]   = 17.0;

			rateConfig_g.algFactorsLimits.lowSystemLimit = 2.0;
			rateConfig_g.algFactorsLimits.lowDynamicRange = 5.0;
			rateConfig_g.algFactorsLimits.highDynamicRange = 40.0;
			rateConfig_g.algFactorsLimits.highSystemLimit = 45.0;

			rateConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			rateConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.shortSampleMean = 25.5;
			rateConfig_g.dilutionCheckFactors.shortSampleSigma = 4.5;
			rateConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			rateConfig_g.dilutionCheckFactors.longSampleMean = 25.0;
			rateConfig_g.dilutionCheckFactors.longSampleSigma = 3.6;
			break;

		case TP:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 62;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.4;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.6;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 10;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_550_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 10;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 10;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 12;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_550_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.03;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.005;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 5;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = STANDARD_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.2;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.2;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.16;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff408c80;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 2580.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 2580.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 2000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 2000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 2000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 14.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 14.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 14.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.5;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 2.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 14.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 15.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = TRUE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 7.6;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 1.15;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 7.7;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 0.65;
			break;

		case TRIG:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 64;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = 0.75;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 1;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_500_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 10;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = 0.5;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = (START_ABS_BAD | TRIG_BLANK_START_ABS_BAD);

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_500_850;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.04;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.02;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = TRIG_RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 7;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = BASE_FINE_SLOPE_INT;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = 0.20;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.16;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = 0.16;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xff408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 750.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 750.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1000.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1000.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 42.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 42.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 42.0;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = 5.0;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 20.0;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 500.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 750.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 87.0;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 23.0;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 87.0;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 115.0;

			// write over config record based on rotor ROC file algorithm flags
			// set pass info based on rotor ROC algorithm flags

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_AVG_START_PASS_FLAG];
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass   = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_AVG_END_PASS_FLAG];

			// set special case calc error mask

			commonAlgGlobals_g->calcErrorMask = TRIG_CALC_ERROR_MASK;
			break;

		case UA:
			endpointConfig_g.endpointAlgRecord.sampleTypeAnalyte = 65;

			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitAnalyte = 0.8;
			endpointConfig_g.endpointAlgRecord.beadMixCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.beadMixCheckPass = 9;

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[0] = WP_515_850;
			endpointConfig_g.endpointAlgRecord.startAbsCheckPass = 9;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[0] = 3.0;
			endpointConfig_g.endpointAlgRecord.startAbsLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.startAbsBlankErrorMask = START_ABS_BAD;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 1;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgBlankCorrectionOrder = ANALYTE_MINUS_BLANK;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgStartPass = 9;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgEndPass = 11;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].avgWavelengthPair = WP_515_600;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointCheckEnabled = TRUE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessRatioLimit = 0.03;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit = 0.005;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].rateInEndpointErrorMask = RATE_IN_ENDPOINT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].turbidityPassCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[0].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.multipleAvgResultCalcEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[0] = 24;
			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.linearTimeCorrectionEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.rqcCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitAnalyte = 0.20;
			endpointConfig_g.endpointAlgRecord.beadMissingCheckLimitBlank = NOVAL_NEG_FLOAT;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitAnalyte = 0.19;
			endpointConfig_g.endpointAlgRecord.distributionCheckLimitBlank = NOVAL_NEG_FLOAT;

			endpointConfig_g.algFactorsLimits.analyteSuppressionMask = 0xef408c00;

			endpointConfig_g.algFactorsLimits.hemolyzedLimit[0] = 1100.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[1] = 1100.0;
			endpointConfig_g.algFactorsLimits.hemolyzedLimit[2] = 1100.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[0]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[1]   = 1500.0;
			endpointConfig_g.algFactorsLimits.lipemicLimit[2]   = 1500.0;
			endpointConfig_g.algFactorsLimits.ictericLimit[0]   = 6.5;
			endpointConfig_g.algFactorsLimits.ictericLimit[1]   = 6.5;
			endpointConfig_g.algFactorsLimits.ictericLimit[2]   = 6.5;
	
			endpointConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_BLANK_FACTOR_IDX]     = 1.538927E-3;
			endpointConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_1_IDX] = -3.269451E-2;
			endpointConfig_g.algFactorsLimits.ictericCorrectionFactors[ICTERIC_ANALYTE_FACTOR_2_IDX] = 5.093318E-4;

			endpointConfig_g.algFactorsLimits.lowSystemLimit = -0.3;
			endpointConfig_g.algFactorsLimits.lowDynamicRange = 0.3;
			endpointConfig_g.algFactorsLimits.highDynamicRange = 25.0;
			endpointConfig_g.algFactorsLimits.highSystemLimit = 50.0;

			endpointConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			endpointConfig_g.dilutionCheckFactors.shortSampleEquationSelect = GAUSS_EQU;
			endpointConfig_g.dilutionCheckFactors.shortSampleMean = 6.0;
			endpointConfig_g.dilutionCheckFactors.shortSampleSigma = 1.5;
			endpointConfig_g.dilutionCheckFactors.longSampleEquationSelect = EXP_EQU;
			endpointConfig_g.dilutionCheckFactors.longSampleMean = 6.0;
			endpointConfig_g.dilutionCheckFactors.longSampleSigma = 3.5;

			// get bfd barcode factor calc method from rotor ROC, typically icteric correction method - BFD_ICTERIC_SLOPE_INT

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.barcodeFactorSlopeIntCalcMethod = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_SLOPE_INT_METHOD_FLAG];


			// IMV - set up for actual IMV_WL_0_CORRECTED_AVG to be repeated in IMV_WL_1_CORRECTED_AVG slot with IMV_WL_0_ENDG_CORRECTED_AVG (ICT corrected) in normal IMV_WL_0_CORRECTED_AVG slot

			endpointConfig_g.endpointAlgRecord.startAbsCheckWavelengthPair[1] = WP_515_850;
			endpointConfig_g.endpointAlgRecord.startAbsLimitAnalyte[1] = NOVAL_POS_FLOAT;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.numToAvg = 2;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].avgWavelengthPair = WP_515_600;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].rateInEndpointCheckEnabled = FALSE;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessToggle = 0.167;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessRatioLimit = 0.03;
			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].flatnessAbsoluteLimit = 0.005;

			endpointConfig_g.endpointAlgRecord.endpointAvgRecord.avgParams[1].overSystemRangeCheckEnabled = FALSE;

			endpointConfig_g.endpointAlgRecord.endpointCalcRecord.bfdCalibrationFactorIndex[1] = 24;

			// end IMV


			break;

		case VLDL:
			derivedConfig_g.algFactorsLimits.analyteSuppressionMask = 0x04000004;

			derivedConfig_g.algFactorsLimits.lowSystemLimit = 1.0;
			derivedConfig_g.algFactorsLimits.lowDynamicRange = 4.0;
			derivedConfig_g.algFactorsLimits.highDynamicRange = 100.0;
			derivedConfig_g.algFactorsLimits.highSystemLimit = 150.0;

			derivedConfig_g.dilutionCheckFactors.dilutionCheckEnabled = FALSE;
			break;

		default:
			break;
	}
}	


// ********************************************************************
// FUNCTION:
//
//		SelectAlgFactorsLimits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Select algorithm factors & limits from rotor ROC or analyte ROC files 
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

CommonFactorsLimits_t *SelectAlgFactorsLimits(unsigned char analyteType, unsigned algType)
{
	unsigned	i;

	CommonFactorsLimits_t	*rocPtr;
	CommonFactorsLimits_t	*algPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	DilutionCheckFactors_t	*dilutionCheckPtr;

	// set up pointer to ROC, analyte files & selected factors/limits

	factorsLimitsPtr = &factorsLimits_m;

	rocPtr = &rotorConfig_g->analyteCalcRecord[analyteType].rotorFactorsLimits;

	// analyte algorithm is not derived calculation by default

	commonAlgGlobals_g->derivedResult = FALSE;
	commonAlgGlobals_g->posNegResult = FALSE;
	
	switch (algType)
	{
		case ENDPOINT:
			algPtr = &endpointConfig_g.algFactorsLimits;
			dilutionCheckPtr = &endpointConfig_g.dilutionCheckFactors;
			break;
		case RATE:
			algPtr = &rateConfig_g.algFactorsLimits;
			dilutionCheckPtr = &rateConfig_g.dilutionCheckFactors;
			break;
		case POS_NEG_ALG:
			algPtr = &posNegConfig_g.algFactorsLimits;
			dilutionCheckPtr = &posNegConfig_g.dilutionCheckFactors;
			commonAlgGlobals_g->posNegResult = TRUE;
			break;
		case DERIVED_CALC_ALG:
			algPtr = &derivedConfig_g.algFactorsLimits;
			dilutionCheckPtr = &derivedConfig_g.dilutionCheckFactors;
			commonAlgGlobals_g->derivedResult = TRUE;
			break;
		case T4_CALC_ALG:
			algPtr = &t4Config_g.algFactorsLimits;
			dilutionCheckPtr = &t4Config_g.dilutionCheckFactors;
			break;
		case CALC_2_CUV_ALG:
			algPtr = &calc2CuvConfig_g.algFactorsLimits;
			dilutionCheckPtr = &calc2CuvConfig_g.dilutionCheckFactors;
			break;
		default:
			algPtr = rocPtr;
			dilutionCheckPtr = &commonAlgGlobals_g->dilutionCheckFactors[analyteType];
			break;
	}

	// set up dilution check factors for use for rotor dilution check algorithm

	commonAlgGlobals_g->dilutionCheckFactors[analyteType] = *dilutionCheckPtr;

	// use analyte suppressin mask default unless ROC file limit specified is not 0)

	factorsLimitsPtr->analyteSuppressionMask = (rocPtr->analyteSuppressionMask != 0) ? rocPtr->analyteSuppressionMask : algPtr->analyteSuppressionMask;

	factorsLimitsPtr->hemolyzedDirection = (rocPtr->hemolyzedDirection != 0) ? rocPtr->hemolyzedDirection : algPtr->hemolyzedDirection;
	factorsLimitsPtr->lipemicDirection   = (rocPtr->lipemicDirection != 0)   ? rocPtr->lipemicDirection   : algPtr->lipemicDirection;
	factorsLimitsPtr->ictericDirection   = (rocPtr->ictericDirection != 0)   ? rocPtr->ictericDirection   : algPtr->ictericDirection;

	// use analyte limit default unless ROC file limit specified is not NOVAL_NEG_FLOAT)

	for (i = 0; i < NUM_ENDOGENOUS_ZONES; i++)
	{
		factorsLimitsPtr->hemolyzedLimit[i] = (rocPtr->hemolyzedLimit[i] != NOVAL_NEG_FLOAT) ? rocPtr->hemolyzedLimit[i] : algPtr->hemolyzedLimit[i];
		factorsLimitsPtr->lipemicLimit[i]   = (rocPtr->lipemicLimit[i]   != NOVAL_NEG_FLOAT) ? rocPtr->lipemicLimit[i]   : algPtr->lipemicLimit[i];
		factorsLimitsPtr->ictericLimit[i]   = (rocPtr->ictericLimit[i]   != NOVAL_NEG_FLOAT) ? rocPtr->ictericLimit[i]   : algPtr->ictericLimit[i];
	}

	for (i = 0; i < MAX_CORRECTION_FACTORS; i++)
	{
	factorsLimitsPtr->hemolyzedCorrectionFactors[i] = (rocPtr->hemolyzedCorrectionFactors[i] != NOVAL_NEG_FLOAT) ? rocPtr->hemolyzedCorrectionFactors[i] : algPtr->hemolyzedCorrectionFactors[i];
	factorsLimitsPtr->lipemicCorrectionFactors[i]   = (rocPtr->lipemicCorrectionFactors[i]   != NOVAL_NEG_FLOAT) ? rocPtr->lipemicCorrectionFactors[i]   : algPtr->lipemicCorrectionFactors[i];
	factorsLimitsPtr->ictericCorrectionFactors[i]   = (rocPtr->ictericCorrectionFactors[i]   != NOVAL_NEG_FLOAT) ? rocPtr->ictericCorrectionFactors[i]   : algPtr->ictericCorrectionFactors[i];
	}

	factorsLimitsPtr->lowSystemLimit   = (rocPtr->lowSystemLimit   != NOVAL_NEG_FLOAT) ? rocPtr->lowSystemLimit   : algPtr->lowSystemLimit;
	factorsLimitsPtr->lowDynamicRange  = (rocPtr->lowDynamicRange  != NOVAL_NEG_FLOAT) ? rocPtr->lowDynamicRange  : algPtr->lowDynamicRange;
	factorsLimitsPtr->highDynamicRange = (rocPtr->highDynamicRange != NOVAL_NEG_FLOAT) ? rocPtr->highDynamicRange : algPtr->highDynamicRange;
	factorsLimitsPtr->highSystemLimit  = (rocPtr->highSystemLimit  != NOVAL_NEG_FLOAT) ? rocPtr->highSystemLimit  : algPtr->highSystemLimit;


	// IMV - paragraph 1 / common limits all analytes store

	StoreCommonAnalyteImv(  analyteType, factorsLimitsPtr );

	// end IMV
	 

	return	factorsLimitsPtr;
}


// ********************************************************************
// FUNCTION:
//
//		SetupAnalyteAlgInfo() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Common setup function to:
//		- Load analyte ROC data &
//		- Select pointer to algorithm factors & limits from rotor ROC or analyte ROC files 
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

CommonFactorsLimits_t *SetupAnalyteAlgInfo(unsigned char analyteType, unsigned algType)
{
	// load configuration data from file & setup pointer common factors/limits

	LoadAnalyteAlgData(analyteType);
													
	// Select limits for rotor from ROC or from rate algorithm default

	return SelectAlgFactorsLimits(analyteType, algType);
}


// ********************************************************************
// FUNCTION:
//
//		SelectEndpointBlankLimits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Select algorithm factors & limits from rotor ROC or analyte ROC files 
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

BlankLimits_t *SelectEndpointBlankLimits(unsigned char analyteType)
{
	unsigned char analyteBlankCuvette;
	unsigned char sampleBlankCuvette;

	EndpointAlgRecord_t	*algPtr;
	BlankLimits_t		*blankLimitsPtr;

	// set up pointer to selected blank limits & analyte ROC

	algPtr = &endpointConfig_g.endpointAlgRecord;
	blankLimitsPtr = &blankLimits_m;

	analyteBlankCuvette = rotorConfig_g->analyteCalcRecord[analyteType].analyteBlankCuvetteNumber;
	sampleBlankCuvette  = rotorConfig_g->analyteCalcRecord[SAMPLE_BLANK].analyteBlankCuvetteNumber;

	// use analyte limit defaults unless ROC file specifies analyte blank is sample blank)

	blankLimitsPtr->beadMixCheckLimit      = (analyteBlankCuvette == sampleBlankCuvette) ? sampleBlankConfig_g.beadMixCheckLimit      : algPtr->beadMixCheckLimitBlank;
	blankLimitsPtr->beadMissingCheckLimit  = (analyteBlankCuvette == sampleBlankCuvette) ? sampleBlankConfig_g.beadMissingCheckLimit  : algPtr->beadMissingCheckLimitBlank;
	blankLimitsPtr->distributionCheckLimit = (analyteBlankCuvette == sampleBlankCuvette) ? sampleBlankConfig_g.distributionCheckLimit : algPtr->distributionCheckLimitBlank;
	blankLimitsPtr->startAbsLimit          = (analyteBlankCuvette == sampleBlankCuvette) ? sampleBlankConfig_g.startAbsLimit          : algPtr->startAbsLimitBlank;

	return	blankLimitsPtr;
}


// ********************************************************************
// FUNCTION:
//
//		StoreAnalyteResultsRecord() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store analyte information, result & flags into a result data record 
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

void StoreAnalyteResultsRecord(unsigned char analyteType, float analyteResult, unsigned long analyteFlags, CommonFactorsLimits_t *factorsLimitsPtr)
{
	RotorAnalyteResult_t	*resultPtr;

	resultPtr = &rotorResults_g->rotorAnalyteResult[analyteType];
	 
	resultPtr->analyteType	= analyteType;
	strcpy(resultPtr->versionNumber, rotorConfig_g->analyteCalcRecord[analyteType].versionNumber);

	resultPtr->printOrder = rotorConfig_g->analyteCalcRecord[analyteType].printOrder;

	resultPtr->analyteResult = analyteResult;
	resultPtr->analyteFlags  = analyteFlags;

	resultPtr->analyteSuppressionMask = factorsLimitsPtr->analyteSuppressionMask;
	
	if (commonAlgGlobals_g->derivedResult)
	{
		resultPtr->resultPrintFlags |= MASK_DERIVED_RESULT;
	}

	if (commonAlgGlobals_g->posNegResult)
	{
		resultPtr->resultPrintFlags |= MASK_POS_NEG_RESULT;
	}

	resultPtr->lowSystemRangeLimit = factorsLimitsPtr->lowSystemLimit;
	resultPtr->lowDynamicRangeLimit = factorsLimitsPtr->lowDynamicRange;
	resultPtr->highDynamicRangeLimit = factorsLimitsPtr->highDynamicRange;
	resultPtr->highSystemRangeLimit = factorsLimitsPtr->highSystemLimit;	
}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdEndpointAvgAbs() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic enpoint algorithm processor for average absorbance calculation 
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

unsigned ProcessStdEndpointAvgAbs(unsigned char analyteType, unsigned checkAnalyteBlank, EndpointAlgRecord_t *endpointAlgPtr, CommonFactorsLimits_t *factorsLimitsPtr, AvgAbs_t *avgAbs, unsigned long *endpointError)
{
	unsigned char		i; 
	unsigned char		sampleTypeAnalyte;		
	unsigned char		sampleTypeAnalyteBlank;		
	unsigned char		sampleTypeOpticalBlank;
	unsigned char		analyteCuvetteNumber;
	unsigned char		analyteBlankCuvetteNumber;			
	float				desiredPathLength;

	BlankLimits_t		*blankLimitsPtr;

	AvgAbs_t 			avgAbsTurb[MAX_TO_AVG];


	// set up common parameters from rotor & algorithm specific configuration data

	sampleTypeAnalyte         = endpointAlgPtr->sampleTypeAnalyte;	
	sampleTypeAnalyteBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	analyteBlankCuvetteNumber = rotorConfig_g->analyteCalcRecord[analyteType].analyteBlankCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	//	Select blank limits for endpoint

	blankLimitsPtr = SelectEndpointBlankLimits(analyteType);

	//	Check endogenous limits from sample blank results

	EndogenousLimitsCheck(analyteType, factorsLimitsPtr, endpointError);


	// IMV - endpoint paragraph 2 store

	StoreAlgParagraphNumImv( analyteType, 2.0 );

	// end IMV


	//	Check bead mix of analyte and bead mix of blank if enabled

	if (BeadMixCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, endpointAlgPtr->beadMixCheckPass, endpointAlgPtr->beadMixCheckLimitAnalyte, TEST_BEAD_MIX_ERROR, endpointError))
	{
		return	TRUE;		// got a calc type error (divide by zero, etc.)
	}
	if (checkAnalyteBlank)
	{
		if (BeadMixCheck(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, endpointAlgPtr->beadMixCheckPass, blankLimitsPtr->beadMixCheckLimit, BLANK_BEAD_MIX_ERROR, endpointError))
		{
			return	TRUE;	// got a calc type error (divide by zero, etc.)
		}
	}

	//	Check for analyte bead missing and for blank bead missing if enabled

	if (BeadMissingCheck(analyteCuvetteNumber, endpointAlgPtr->beadMissingCheckLimitAnalyte, endpointError))
	{
		return	TRUE;		// got a calc type error (divide by zero, etc.)
	}
	if (checkAnalyteBlank)
	{
		if (BeadMissingCheck(analyteBlankCuvetteNumber, blankLimitsPtr->beadMissingCheckLimit, endpointError))
		{
			return	TRUE;	// got a calc type error (divide by zero, etc.)
		} 
	}

	//	Check for analyte bead missing and for blank bead missing if enabled

	if (BeadDistributionCheck(analyteCuvetteNumber, endpointAlgPtr->distributionCheckLimitAnalyte, endpointError))
	{
		return	TRUE;		// got a calc type error (divide by zero, etc.)
	}
	if (checkAnalyteBlank)
	{
		if (BeadDistributionCheck(analyteBlankCuvetteNumber, blankLimitsPtr->distributionCheckLimit, endpointError))
		{
			return	TRUE;	// got a calc type error (divide by zero, etc.)
		} 
	}

	// calc averages for number of wavelength pairs to average

	for (i = 0; i <= endpointAlgPtr->endpointAvgRecord.numToAvg - 1; i++)
	{
		//	Check starting absorbance of analyte and blank for this wavelength to avg if enabled

		if (StartingAbsorbanceCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, endpointAlgPtr->startAbsCheckPass, endpointAlgPtr->startAbsCheckWavelengthPair[i], endpointAlgPtr->startAbsLimitAnalyte[i], START_ABS_BAD, endpointError))
		{
			return	TRUE;		// got a calc type error (divide by zero, etc.)	or starting abs error
		}
		if (checkAnalyteBlank)
		{
			if (StartingAbsorbanceCheck(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, endpointAlgPtr->startAbsCheckPass, endpointAlgPtr->startAbsCheckWavelengthPair[i], blankLimitsPtr->startAbsLimit, endpointAlgPtr->startAbsBlankErrorMask, endpointError))
			{
				return	TRUE;	// got a calc type error (divide by zero, etc.) or starting abs error
			} 
		}


		// IMV - endpoint paragraph 3 store

		StoreAlgParagraphNumImv( analyteType, 3.0 );

		// end IMV


		// Check for turbidity pass correction

		if (endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionEnabled)
		{
			// calculate absorbance using turbidity check algorithm passes & wavelength	settings

			if ( CalcAverageAbsorbance( &avgAbsTurb[i], sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionStartPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionEndPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionWavelengthPair, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, endpointAlgPtr->endpointAvgRecord.avgBlankCorrectionOrder, endpointError ) )
			{
				return	TRUE;		// got a calc type error (divide by zero, etc.)
			}

			// check for potential adjustment of passes for abs averaging for endpoint algorithm
			 
			EndpointTurbidityPassCorrection( &avgAbsTurb[i], endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionStartPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionEndPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionSexSpecies, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityPassCorrectionLimit, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityCorrectedStartPassAvg, endpointAlgPtr->endpointAvgRecord.avgParams[i].turbidityCorrectedEndPassAvg );
		}


		//	Calculate average blanked aborbance for number of passes in average

		if (CalcAverageAbsorbance(&avgAbs[i], sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, endpointAlgPtr->endpointAvgRecord.avgStartPass, endpointAlgPtr->endpointAvgRecord.avgEndPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].avgWavelengthPair, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, endpointAlgPtr->endpointAvgRecord.avgBlankCorrectionOrder, endpointError))
		{
			return	TRUE;		// got a calc type error (divide by zero, etc.)
		}


		// IMV - endpoint paragraph 4 store

		StoreAlgParagraphNumImv( analyteType, 4.0 );

		// end IMV


		//	Check blanked average for rate in endpoint

		if (endpointAlgPtr->endpointAvgRecord.avgParams[i].rateInEndpointCheckEnabled)
		{
			if (EndpointRateErrorCheck(&avgAbs[i], endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessToggle, endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessRatioLimit, endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessAbsoluteLimit, endpointAlgPtr->endpointAvgRecord.avgParams[i].rateInEndpointErrorMask, endpointError))
			{
				return	TRUE;		// got a calc type error (divide by zero, etc.)
			}
		}
		else
		{


			// IMV - endpoint flatness set - not enabled increment index & set NOVAL_NEG_FLOAT

			SetImvEndpointFlatnessParams(NOVAL_NEG_FLOAT, NOVAL_NEG_FLOAT);

			// end IMV


		}

		//	Check for slope over system range in endpoint

		if (endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeCheckEnabled)
		{
			if (OverSystemRangeErrorCheck(&avgAbs[i], endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeStartPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeEndPass, endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeLessThanLimit, endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeLimit, endpointAlgPtr->endpointAvgRecord.avgParams[i].overSystemRangeErrorMask, endpointError))
			{
				return	TRUE;		// got a calc type error (divide by zero, etc.)
			}
		}
		else
		{


			// IMV - over system range params set - not enabled increment index & set NOVAL_NEG_FLOAT

			SetImvEndpointOverSystemRangeParams( NOVAL_NEG_FLOAT, NOVAL_NEG_FLOAT, NOVAL_NEG_FLOAT, NOVAL_NEG_FLOAT );

			// end IMV


		}


		// IMV - endpoint bead mix & blanked abs / blanked avg params / flatness by wavelength params store

		StoreEndpointAvgAbsImv( analyteType, i, endpointAlgPtr, avgAbs );

		// end IMV


	}


	// IMV - endpoint paragraph 5 store

	StoreAlgParagraphNumImv( analyteType, 5.0 );

	// end IMV


	// no errors in endpoint standard average absorbance calc, so return average absorbance(s)

	return	FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		CalcEndpointWeightedResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calc final result as weighted average result from 2 wavelength averages 
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

float CalcEndpointWeightedResult(unsigned char analyteType, AvgAbs_t *avgAbs, EndpointAlgRecord_t *endpointAlgPtr, float *endpointResult1, float *endpointResult2, unsigned long *endpointError)
{
	float	bfdCorrectedAvg1;
	float	a1SquaredAvgMult1;
	float	a2AvgMult1;
	float	a3Intercept1;
	float	weightedResult1Factor;
	float	bfdCorrectedAvg2;
	float	a1SquaredAvgMult2;
	float	a2AvgMult2;
	float	a3Intercept2;
	float	weightedResult2Factor;

	// set up final result weighting factors

	weightedResult1Factor= endpointAlgPtr->endpointCalcRecord.weightedResult1Factor;
	weightedResult2Factor= endpointAlgPtr->endpointCalcRecord.weightedResult2Factor;

	// 2 wavelength pairs averaged to be used in final result calc so choose type of final endpoint result calc

	if (endpointAlgPtr->endpointCalcRecord.weightedResultCalcMethod == ENDPOINT_2PARABOLA_METHOD)
	{
		// set up to calc 2 endpoint type results using parabolic equations	from barcode parameter terms

		a1SquaredAvgMult1 = GetBarcodeParameter(analyteType, PARABOLA1_A1);
		a2AvgMult1        = GetBarcodeParameter(analyteType, PARABOLA1_A2);
		a3Intercept1      = GetBarcodeParameter(analyteType, PARABOLA1_A3);
		a1SquaredAvgMult2 = GetBarcodeParameter(analyteType, PARABOLA2_A1);
		a2AvgMult2        = GetBarcodeParameter(analyteType, PARABOLA2_A2);
		a3Intercept2      = GetBarcodeParameter(analyteType, PARABOLA2_A3);

		bfdCorrectedAvg1 = BfdCalAdjust(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX]);
		bfdCorrectedAvg2 = BfdCalAdjust(avgAbs[AVG2_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG2_IDX]);

		if ((bfdCorrectedAvg1 >= (-a2AvgMult1 / (2 * a1SquaredAvgMult1))) && (bfdCorrectedAvg2 >= (a2AvgMult2 / (2 * a1SquaredAvgMult2))))
		{
			*endpointResult1 = (a1SquaredAvgMult1 * bfdCorrectedAvg1 * bfdCorrectedAvg1) + (a2AvgMult1 * bfdCorrectedAvg1) + a3Intercept1;
			*endpointResult2 = (a1SquaredAvgMult2 * bfdCorrectedAvg2 * bfdCorrectedAvg2) - (a2AvgMult2 * bfdCorrectedAvg2) + a3Intercept2;

			// rotor (e.g. 26 for CA) weighting factors are set to 1.0 for 50/50 averaging

			if (rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_WEIGHTED_ENDPOINT_50_50_FLAG])
			{
				weightedResult1Factor= 1.0;
				weightedResult2Factor= 1.0;
			} 
		}
		else
		{
			*endpointError |= ENDPOINT_LOW_ABS_ERROR;
			return NOVAL_NEG_FLOAT;
		}
	}
	else 
	{
		// set up to calc weighted average for 2 results computed using standard endpoint calc

		*endpointResult1 = CalcBfdStdBarcodeCorrectedResult(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType, STD_SLOPE1_IDX, STD_INTERCEPT1_IDX);
		*endpointResult2 = CalcBfdStdBarcodeCorrectedResult(avgAbs[AVG2_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG2_IDX], analyteType, STD_SLOPE2_IDX, STD_INTERCEPT2_IDX);
	}


	// IMV - store endpoint wavelength at idx 0 result & wavelength at idx 1 result

	SetImvEndpointDualWavelengthResults( *endpointResult1, *endpointResult2 );

	// end IMV


	return ((*endpointResult1 * weightedResult1Factor) + (*endpointResult2 * weightedResult2Factor)) / 2.0;
}


// ********************************************************************
// FUNCTION:
//
//		CaResultLotCorrectionCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check/Calc result corrections for CA for all lots after 05/24/10 
//
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

float CaResultLotCorrectionCheck( unsigned char analyteType, float endpointResult )
{
	unsigned char	rotorId;							
	unsigned char	rotorFormat;						
	float			calciumResultCorrection;


	if ( ( analyteType == CA ) && ( rotorResults_g->rotorProductCode == VETSCAN_PRODUCT_CODE) && ( commonAlgGlobals_g->patientType == PATIENT_TYPE_DOG_FEMALE ) && ( endpointResult != NOVAL_NEG_FLOAT ) )
	{
		// valid result for CA on VetScan rotor for DOG species type
		// initialize CA result correction for no correction

		calciumResultCorrection = 0.0;							


		// IMV - store CA pre-corrected endpoint result

		StoreIntermediateValue( analyteType, IMV_CA_PRE_CORRECTION_RESULT, IMV_DIRECT_SET_METHOD, endpointResult );

		// end IMV


		// verify that applicable rotor / barcode format is in the list

		rotorId     = barcodeData_g->rotorId;
		rotorFormat = barcodeData_g->rotorFormat;

        if (   ( rotorId == 36 && rotorFormat < 4)
            || ( rotorId == 48 && rotorFormat < 4)
            || ( rotorId == 56 && rotorFormat < 3)
            || ( rotorId == 59 && rotorFormat < 4)
            || ( rotorId == 60 && rotorFormat < 3) 
            || ( rotorId == 69 && rotorFormat < 2)
            || ( rotorId == 73 && rotorFormat < 2) 
            || ( rotorId == 75 && rotorFormat < 2) )
        {   
        	// When correction is no longer needed the bc_format number will be increased to skip the correction
                        
        	calciumResultCorrection = (CORRECTION_A1* endpointResult * endpointResult) + (CORRECTION_A2 * endpointResult) + CORRECTION_A3; 
                        
            // Only correct if correction is greater than zero
                             
            if (calciumResultCorrection > 0.0)
            {
            	endpointResult = endpointResult - calciumResultCorrection; 
        	}
        } 

				
		// IMV - store CA result correction for applicable lots

		StoreIntermediateValue( analyteType, IMV_CA_RESULT_CORRECTION, IMV_DIRECT_SET_METHOD, calciumResultCorrection );

		// end IMV


	}

	return	endpointResult; 
}


// ********************************************************************
// FUNCTION:
//
//		ResultWavelengthCompareCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Endpoint error check for result wavelength computed from compare  
//		of 2 averages by method type out of limits
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

unsigned long ResultWavelengthCompareCheck(float endpointResult, float endpointResult1, float endpointResult2, EndpointAlgRecord_t *endpointAlgPtr)
{
	float	resultCompare;
	 
	if (endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareType == RESULT_COMPARE_RATIO)
	{
		resultCompare = endpointResult2 / endpointResult1;
	}
	else
	{
		resultCompare = endpointResult2 - endpointResult1;
	}


	// IMV - store endpoint dual wavelength compare value

	SetImvEndpointDualWavelengthResultCompare( resultCompare );

	// end IMV


	if (endpointResult > endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareToggle)
	{
		if ((resultCompare < endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareLowLimit) || (resultCompare > endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareHighLimit))
		{
			return WAVELENGTH_COMPARE_ERROR;
		}
	}

	return NO_ANALYTE_ERROR;
} 


// ********************************************************************
// FUNCTION:
//
//		WavelengthRatioCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Endpoint error check for result wavelength computed from compare  
//		of 2 averages by method type out of limits
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

unsigned long WavelengthRatioCheck(unsigned char analyteType, EndpointAlgRecord_t *endpointAlgPtr, unsigned long *endpointError)
{
	unsigned		i;
	unsigned char	checkPass;
	unsigned char	wavelengthPair;
	char 			rotorMfgLot[8];
	unsigned char	sampleTypeAnalyte;		
	unsigned char	sampleTypeAnalyteBlank;		
	unsigned char	sampleTypeOpticalBlank;
	unsigned char	analyteCuvetteNumber;
	unsigned char	analyteBlankCuvetteNumber;			
	float			desiredPathLength;
	float			analyteAbs;
	float			analyteBlankAbs;
	float			ratioCheckAbs[MAX_TO_AVG];
	float			wavelengthRatio;
	float			wavelengthRatioLimit;
	 
	// set up common parameters from rotor & algorithm specific configuration data for abs calcs

	sampleTypeAnalyte         = endpointAlgPtr->sampleTypeAnalyte;	
	sampleTypeAnalyteBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	analyteBlankCuvetteNumber = rotorConfig_g->analyteCalcRecord[analyteType].analyteBlankCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	// compute wavelengthRatio for check

	checkPass = endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.checkPass;

	for (i = 0; i < MAX_TO_AVG; i++)
	{
		wavelengthPair = endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.wavelengthPairs[i];

		// calc absorbance for analyte sample type and analyte blank sample type for selected wavelength at selected pass

		analyteAbs = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, checkPass, wavelengthPair, desiredPathLength, analyteCuvetteNumber, endpointError);

		analyteBlankAbs = CalcSampleAbsorbance(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, checkPass, wavelengthPair, desiredPathLength, analyteBlankCuvetteNumber, endpointError);

		// compute blanked absorbance based on blankCorrectionOrder 

		if (endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.blankCorrectionOrder == ANALYTE_MINUS_BLANK)
		{
			ratioCheckAbs[i] = analyteAbs - analyteBlankAbs;	// 0: "analyte - blank"
		}
		else if	(endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.blankCorrectionOrder == BLANK_MINUS_ANALYTE)
		{
			ratioCheckAbs[i] = analyteBlankAbs - analyteAbs;	// 1: "blank - analyte"
		}
		else
		{
			ratioCheckAbs[i] = analyteAbs;						// 0xff: "analyte" ; no NO_BLANK_CORRECTION
		}
	}

	wavelengthRatio = ratioCheckAbs[0] / ratioCheckAbs[1];


	// select limit for wavelength ratio check based on match to any mfg lot number in list (initialized to low min limit)

	wavelengthRatioLimit = endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.lowMinLimit;

	// set up this rotor mfg lot number for compare

	sprintf( rotorMfgLot, "%d%02d%d%s", barcodeData_g->rotorMfgLot.lsdOfYear,	
								 		barcodeData_g->rotorMfgLot.weekOfYear,
								 		barcodeData_g->rotorMfgLot.dayOfWeek,
								 		barcodeData_g->rotorMfgLot.code );

	// scan for mfg lot match for setting limit to high min limit

	i = 0;

	while ( i < MAX_MFG_LOTS_TO_CHECK )
	{
		if ( endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.mfgLots[i][0] == 'N' )
		{
			i = MAX_MFG_LOTS_TO_CHECK;
		}
		else if ( strcmp( rotorMfgLot, endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.mfgLots[i]) == 0 )			       
		{
			wavelengthRatioLimit = endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.highMinLimit;

			i = MAX_MFG_LOTS_TO_CHECK;
		}

		i++;
	}	


	// IMV - store wavelength ratio check params

	StoreIntermediateValue( analyteType, IMV_WL_RATIO_LIMIT, IMV_DIRECT_SET_METHOD, wavelengthRatioLimit );
	StoreIntermediateValue( analyteType, IMV_WL_RATIO,       IMV_DIRECT_SET_METHOD, wavelengthRatio );

	// end IMV


	// check computed wavelength ratio versus selected limit
	 
	if ( wavelengthRatio < wavelengthRatioLimit )
	{
		return WAVELENGTH_COMPARE_ERROR;
	}

	return NO_ANALYTE_ERROR;
} 


// ********************************************************************
// FUNCTION:
//
//		CalcExponentialRotorAgeDecay() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate exponential rotor age decay factor as:
//		1 - e**(-rotorAgeInDays / passed rotorAgeDivisor) 
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

float CalcExponentialRotorAgeDecay(float rotorAgeDivisor) 
{
	return (1.0 - exp(-barcodeData_g->rotorAgeInDays / rotorAgeDivisor));
}


// ********************************************************************
// FUNCTION:
//
//		CalcLinearTimeCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct the rate result for linear time correction  
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

float CalcLinearTimeCorrection(unsigned char analyteType, float linearTimeCorrectionStabilityRate)
{


	// IMV - linear time correction stability rate parameter

	StoreIntermediateValue( analyteType, IMV_STABILITY_RATE, IMV_DIRECT_SET_METHOD, linearTimeCorrectionStabilityRate );

	// end IMV


	return (1.0 + (linearTimeCorrectionStabilityRate * barcodeData_g->rotorAgeInDays));
}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdEndpointAlgorithm() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic enpoint algorithm processor for enpoint analyte results & error flags 
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

void ProcessStdEndpointAlgorithm(unsigned char analyteType, unsigned checkAnalyteBlank)
{
	float				endpointResult;
	unsigned long		endpointError;

	float				endpointResult1;
	float				endpointResult2;

	EndpointAlgRecord_t		*endpointAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	AvgAbs_t 	avgAbs[MAX_TO_AVG];

	// init endpoint result & flags

	endpointResult =  NOVAL_NEG_FLOAT;
	endpointError =  NO_ANALYTE_ERROR;

	// get endpoint analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, ENDPOINT);
	
	endpointAlgPtr = &endpointConfig_g.endpointAlgRecord;		

	// process endpoint standard average absorbance algorithm for up to 2 wavelength pairs if enabled

	if (!ProcessStdEndpointAvgAbs(analyteType, checkAnalyteBlank, endpointAlgPtr, factorsLimitsPtr, avgAbs, &endpointError))
	{
		// Compute result for average for additional wavelength pair averaged to be included in final result calc if enabled

		if (endpointAlgPtr->endpointCalcRecord.multipleAvgResultCalcEnabled == FALSE)
		{
			switch (endpointAlgPtr->endpointCalcRecord.barcodeFactorSlopeIntCalcMethod)
			{
				case BASE_FINE_SLOPE_INT:
					endpointResult = CalcBfdBaseFineBarcodeCorrectedResult(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType);
					break;
				case BASE_FINE_EXP:
					endpointResult = CalcBfdExpBaseFineBarcodeCorrectedResult(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType);
					break;
				case PATIENT_TYPE_SLOPE_INT:
					endpointResult = CalcBfdPatientTypeBarcodeCorrectedResult(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType);
					break;
				case BFD_ICTERIC_SLOPE_INT:
					endpointResult = CalcBfdIctericBarcodeCorrectedResult(avgAbs[AVG1_IDX].avgAnalyte, avgAbs[AVG1_IDX].avgAnalyteBlank, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType, factorsLimitsPtr);
					break;
				case STANDARD_SLOPE_INT:
				default:
					endpointResult = CalcBfdStdBarcodeCorrectedResult(avgAbs[AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[AVG1_IDX], analyteType, STD_SLOPE1_IDX, STD_INTERCEPT1_IDX);
					break;
			}


			// IMV - wavelength index 1 (2nd wavelength) endpoint bfd corrected avg set

			SetImvWl1BfdCorrected( avgAbs, endpointAlgPtr );
			 
			// end IMV


		}
		else
		{
			// calculate endpoint result based on results from 2 wavelength pairs by type

			endpointResult = CalcEndpointWeightedResult(analyteType, avgAbs, endpointAlgPtr, &endpointResult1, &endpointResult2, &endpointError);


			// correct for DOG samples on VetScan rotors only with CA analyte for those rotors 

			endpointResult = CaResultLotCorrectionCheck( analyteType, endpointResult );


			// result wavelength comparison check for either weighted average or parabolic functions

			if (endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareType)
			{
				endpointError |= ResultWavelengthCompareCheck(endpointResult, endpointResult1, endpointResult2, endpointAlgPtr);
			}

			// result wavelength comparison check for either weighted average or parabolic functions

			if (endpointAlgPtr->endpointCalcRecord.wavelengthRatioCheck.checkEnabled)
			{
				endpointError |= WavelengthRatioCheck(analyteType, endpointAlgPtr, &endpointError);
			}


			// IMV - store dual wavelength calc / check params

			StoreIntermediateValue( analyteType, IMV_DUAL_WL_HI_DELTA_LIMIT,  IMV_DIRECT_SET_METHOD, endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareHighLimit );
			StoreIntermediateValue( analyteType, IMV_DUAL_WL_LO_DELTA_LIMIT,  IMV_DIRECT_SET_METHOD, endpointAlgPtr->endpointCalcRecord.resultWavelengthCompareLowLimit );
			StoreIntermediateValue( analyteType, IMV_DUAL_WL_0_RESULT_WEIGHT, IMV_DIRECT_SET_METHOD, endpointAlgPtr->endpointCalcRecord.weightedResult1Factor );
			StoreIntermediateValue( analyteType, IMV_DUAL_WL_1_RESULT_WEIGHT, IMV_DIRECT_SET_METHOD, endpointAlgPtr->endpointCalcRecord.weightedResult2Factor );

			// end IMV


		}


		// IMV - result no decay / endpoint paragraph 6

		StoreIntermediateValue( analyteType, IMV_RESULT_NO_DECAY, IMV_DIRECT_SET_METHOD, endpointResult );

		StoreAlgParagraphNumImv( analyteType, 6.0 );

		// end IMV


		// Perform linear time correction for the final endpoint result if enabled

		if (endpointAlgPtr->endpointCalcRecord.linearTimeCorrectionEnabled)
		{
			endpointResult *= CalcLinearTimeCorrection(analyteType, endpointAlgPtr->endpointCalcRecord.linearTimeCorrectionStabilityRate);
		}

		// check if RQC error if enabled

		if (endpointAlgPtr->rqcCheckEnabled)
		{
			endpointError |= RqcAbusedCheck();
		}
			

		// IMV - endpoint result / endpoint paragraph 7 / endpoint starting abs / bfd corrected / barcode factors store

		StoreIntermediateValue( analyteType, IMV_ANALYTE_RESULT, IMV_DIRECT_SET_METHOD, endpointResult );

		StoreAlgParagraphNumImv( analyteType, 7.0 );

		StoreEndpointResultCalcParamsImv( analyteType );

		// end IMV


		// Check standard endpointResult for within system & dynamic range limits

		ResultLimitsCheck(analyteType, endpointResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &endpointError);
	}


	// IMV - endpoint paragraph 8

	StoreAlgParagraphNumImv( analyteType, 8.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, endpointResult, endpointError, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		CompareWindowThreshold() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns flag for threshold check for abs >= threshold or abs <= threshold
//		based on selection in rate analyte ROC config 
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

float GetWindowThresholdAbs(unsigned char selectAbsorbanceBlankCorrected, unsigned char checkPass, unsigned char wavelengthPair, unsigned char sampleTypeAnalyte, unsigned char sampleTypeAnalyteBlank, unsigned char sampleTypeOpticalBlank, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned char analyteBlankCuvetteNumber, unsigned long *rateError)
{
	float	thresholdAbs;
	float	blankAbs;

	thresholdAbs = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, checkPass, wavelengthPair, desiredPathLength, analyteCuvetteNumber, rateError);	

	// do blank absorbance correction if enabled

	if (selectAbsorbanceBlankCorrected)
	{
		blankAbs = CalcSampleAbsorbance(sampleTypeAnalyteBlank, sampleTypeOpticalBlank, checkPass, wavelengthPair, desiredPathLength, analyteBlankCuvetteNumber, rateError);	

		thresholdAbs -= blankAbs;
	}

	return thresholdAbs;
}


// ********************************************************************
// FUNCTION:
//
//		CompareWindowThreshold() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns flag for threshold check for abs >= threshold or abs <= threshold
//		based on selection in rate analyte ROC config 
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

unsigned CompareWindowThreshold(float windowAbs, unsigned char compareThresholdType, float selectAbsorbanceThreshold)
{
	// do compare for increasing & decreasing absorbance window checks

	return ((compareThresholdType == GT_WINDOW_THRESHOLD) ? (windowAbs >= selectAbsorbanceThreshold) : (windowAbs <= selectAbsorbanceThreshold));
}



// ********************************************************************
// FUNCTION:
//
//		EndingAbsorbanceCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Checks end absorbance if enabled and compares abs < threshold or abs > threshold
//		based on selection in rate analyte ROC config 
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

unsigned EndingAbsorbanceCheck(float windowAbs, unsigned compareFlag, float endAbsLimit)
{
	// do compare for ending absorbance check based on compare type
		 				
	return ((compareFlag) ? (windowAbs < endAbsLimit) : (windowAbs > endAbsLimit));
}


// ********************************************************************
// FUNCTION:
//
//		SelectSlopeWindow() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sets start pass & number of points for slope, standard error & slope false rate
//		calculations based on absorbance pass comparisons to a threshold within the 
//		absorbance window (all passes, comparisons, thresholds defined in analyte ROC config) 
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

unsigned long SelectSlopeWindow(SlopeParams_t *slopeParamsPtr, WindowSelectRecord_t *windowPtr, unsigned char sampleTypeAnalyte, unsigned char sampleTypeAnalyteBlank, unsigned char sampleTypeOpticalBlank, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned char analyteBlankCuvetteNumber, unsigned long *rateError)
{
	unsigned char	i;
	unsigned char	selectAbsorbanceBlankCorrected;
	unsigned char	lastCheckPass;
	unsigned char	checkPass;
	unsigned		preEndAbsCheckError;
	unsigned long	endAbsAnalyteErrorMask;
	float			selectAbsorbanceThreshold;
	float			windowAbs;
	float			derivedAbs;

	// set up common params 

	preEndAbsCheckError = FALSE;
	endAbsAnalyteErrorMask = windowPtr->endAbsAnalyteErrorMask;

	// calc sample aborbance for number of window check passes defined

	lastCheckPass = windowPtr->numCheckPasses - 1;

	for (i = 0; i <= lastCheckPass; i++)
	{
		// get window absorbance this pass blank corrected if enabled or derived if enabled

		checkPass = windowPtr->selectParams[i].checkPass;
		selectAbsorbanceBlankCorrected = windowPtr->selectParams[i].selectAbsorbanceBlankCorrected;
		selectAbsorbanceThreshold = windowPtr->selectParams[i].selectAbsorbanceThreshold;

		windowAbs = GetWindowThresholdAbs(selectAbsorbanceBlankCorrected, checkPass, windowPtr->selectWavelengthPair, sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, rateError);	

		if (windowPtr->derivedSelectThresholdEnabled)
		{
			derivedAbs = GetWindowThresholdAbs(selectAbsorbanceBlankCorrected, checkPass, windowPtr->derivedSelectWavelengthPair, sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, rateError);	

			windowAbs -= (windowPtr->derivedSelectThresholdMultiplier * derivedAbs);
		}


		// IMV - rate window abs for window compare set

		SetImvRateCompareWindowAbs( i, windowAbs );
			 
		// end IMV


		// do pre window selection end abs check if enabled	& exit if end abs error

		if ((i == 0) && windowPtr->preEndAbsCheckEnabled)
		{
			if (EndingAbsorbanceCheck(windowAbs, COMPARE_LT_LIMIT, windowPtr->preEndAbsLimit))
			{
				*rateError |= endAbsAnalyteErrorMask; 
				preEndAbsCheckError = TRUE;
				i = lastCheckPass;
			}
		}

		// check absorbance against threshold to select window for slope determination

		if (CompareWindowThreshold(windowAbs, windowPtr->compareThresholdType, selectAbsorbanceThreshold) || (i == lastCheckPass))
		{
			// if  got window or last check pass, then set start point (pass) & number of points for slope / error calcs

			slopeParamsPtr->startPass = windowPtr->selectParams[i].slopeStartPass;
			slopeParamsPtr->numPoints = windowPtr->selectParams[i].slopeNumPoints;
			*rateError |= windowPtr->selectParams[i].selectedPassMask;

			// if last window check pass, then check end absorbance against limit (if enabled)

			if ((i == lastCheckPass) && windowPtr->postEndAbsCheckEnabled)
			{
				if (EndingAbsorbanceCheck(windowAbs, windowPtr->compareThresholdType, windowPtr->postEndAbsLimit))
				{
					*rateError |= endAbsAnalyteErrorMask;
				} 
			}

			// window is selected so exit window selection loop
						 
			i = lastCheckPass;
		}
	}
	 
	return	(*rateError & commonAlgGlobals_g->calcErrorMask) | preEndAbsCheckError;	// abort algorithm processing flag 
}


// ********************************************************************
// FUNCTION:
//
//		GetSlopeTimeAbsData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sets up absorbances (slope, standard error, false rate)
//		for number of passes set for slope window 
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

unsigned long GetSlopeTimeAbsData(SlopeParams_t *slopeParamsPtr, unsigned char sampleTypeAnalyte, unsigned char sampleTypeOpticalBlank, RateCalcRecord_t *calcPtr, float desiredPathLength, unsigned char analyteCuvetteNumber, SlopeAbsData_t *slopeAbsPtr, unsigned long *rateError)
{
	unsigned char	i;
	unsigned char	numPoints;
	unsigned char	startPass;

	// minimum points allowed for any slope calc is 2 

	numPoints = slopeParamsPtr->numPoints;

	if (numPoints == 1)		// flag for last window
	{
		numPoints = 2;
	}

	startPass = slopeParamsPtr->startPass;

	for (i = 0; i < numPoints; i++)
	{
		// set time and absorbance in arrays for slope calcs

		slopeAbsPtr->flashTime[i] = GetSampleFlashTime(sampleTypeAnalyte, i + startPass);

		slopeAbsPtr->rateStdErrorAbs[i] = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, i + startPass, calcPtr->rateStdErrorWavelengthPair, desiredPathLength, analyteCuvetteNumber, rateError);	

		slopeAbsPtr->falseRateAbs[i]    = CalcSampleAbsorbance(sampleTypeAnalyte, sampleTypeOpticalBlank, i + startPass, calcPtr->falseRateWavelengthPair,    desiredPathLength, analyteCuvetteNumber, rateError);	
	}
	 
	return	(*rateError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag 
}


// ********************************************************************
// FUNCTION:
//
//		SlopeNoiseErrorCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Checks for noise in slope for rate not linear 
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

unsigned long SlopeNoiseErrorCheck(float slopeRate, float stdError, RateCalcRecord_t *calcPtr)
{
	if (fabs(slopeRate) > calcPtr->rateNoiseToggle)
	{
		if ((stdError / fabs(slopeRate)) > calcPtr->rateNoiseRatioLimit)
		{
			return calcPtr->rateNotLinearAnalyteErrorMask;
		}
	}
	else
	{
		if (stdError > calcPtr->rateNoiseAbsoluteLimit)
		{
			return calcPtr->rateNotLinearAnalyteErrorMask;
		}
	}

	return NO_ANALYTE_ERROR;
}


// ********************************************************************
// FUNCTION:
//
//		CalcSlopeRateCorrection() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculates correction to slope based on false rate slope calculation and
//		empirically derived correction multiplier if noise greater than correction limit 
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

float CalcSlopeRateCorrection(unsigned char analyteType, float slopeRate, float stdError, float slopeFalseRate, RateCalcRecord_t *calcPtr, unsigned char sampleTypeAnalyte, unsigned char sampleTypeOpticalBlank, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned long *rateError)
{
	unsigned char	slopeRateCorrected;
	unsigned char	startPass;
	unsigned char	endPass;
	unsigned char	numPoints;
	float			limitCompareRate;
	float			intercept;
	float			slopeRateCorrection;

	AvgAbs_t 		absData;
	AvgAbs_t 		*absPtr = &absData;

	// init for no correction

	slopeRateCorrected  = FALSE;
	slopeRateCorrection = 0.0;

	// Noise Ratio Check Method:
	// 		Use rate and standard error already computed for analyte
	// 		if absolute value of rate > rate noise toggle AND noise ratio > correction noise ratio limit,
	// 		then correct using false rate adjusted by correction multiplier
			 
	if ( (fabs(slopeRate) > calcPtr->rateNoiseToggle) && ((stdError / fabs(slopeRate)) > calcPtr->slopeRateCorrectionLimit) )
	{
		slopeRateCorrection = slopeRate - (calcPtr->slopeRateCorrectionMultiplier * slopeFalseRate);
		slopeRateCorrected = TRUE;	
	}

	// ABS Check Method:
	// 		Compute a rate per selected wavelength pair, start pass, end pass
	// 		if rate < rate correction limit AND slopeRate NOT CORRECTED by Noise Ratio Check
	// 		then correct using false rate adjusted by correction multiplier
			 
	startPass = calcPtr->slopeRateCorrectionStartPass;
	endPass   = calcPtr->slopeRateCorrectionEndPass;
	numPoints = (endPass - startPass) + 1;
			
	// get analyte abs & time points for number of passes at rate wavelength (does not blank or use average, max or min)
	 
	if (CalcAverageAbsorbance(absPtr, sampleTypeAnalyte, NO_BLANK_CORRECTION, sampleTypeOpticalBlank, startPass, endPass, calcPtr->slopeRateCorrectionWavelengthPair, desiredPathLength, analyteCuvetteNumber, NO_BLANK_CORRECTION, NO_BLANK_CORRECTION, rateError))
	{
		return	slopeRateCorrection;			// NO correction since math error
	}

	// compute rate used to compare to limit to see if correction is needed 

	if (LinearRegression(absPtr->flashTime, absPtr->blankedAbs, numPoints, &limitCompareRate, &intercept))
	{
		*rateError |= DIVIDE_BY_ZERO;
		return	slopeRateCorrection;			// NO correction since math error
	}

	// check computed ABS selected rate versus limit and calc correction if not already corrected by Noise Ratio Check Method

	if ( (limitCompareRate < calcPtr->slopeRateCorrectionRateLimit) && (slopeRateCorrected == FALSE) )
	{
		slopeRateCorrection = slopeRate - (calcPtr->slopeRateCorrectionRateMultiplier * slopeFalseRate);
	}


	// IMV - store slopeRateCorrection IMV

	StoreIntermediateValue( analyteType, IMV_RATE_CORR_ABS_RATE,   IMV_DIRECT_SET_METHOD, limitCompareRate );
	StoreIntermediateValue( analyteType, IMV_RATE_CORR_CORRECTION, IMV_DIRECT_SET_METHOD, slopeRateCorrection );

	// end IMV


	return slopeRateCorrection;
}


// ********************************************************************
// FUNCTION:
//
//		FalseRateErrorCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Checks for false rate from alternate wavelength selected 
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

unsigned long FalseRateErrorCheck(unsigned char numPoints, float slopeRate, float slopeFalseRate, RateCalcRecord_t *calcPtr)
{
	if (numPoints != calcPtr->falseRatePointsToggle)
	{
		if ((fabs(slopeFalseRate / slopeRate)) > calcPtr->falseRateRatioLimit)
		{
			return FALSE_RATE;
		} 
	}
	else
	{
		if (fabs(slopeFalseRate) > 	calcPtr->falseRateAbsoluteLimit)
		{
			return FALSE_RATE;
		} 
	}

	return NO_ANALYTE_ERROR;
}


// ********************************************************************
// FUNCTION:
//
//		CalcSlopeParameters() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate standard Slope rate algorithm parameters using 
//		window linear regression method or delta sum multiplier method  
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

unsigned long CalcSlopeParameters(unsigned char analyteType, SlopeParams_t *slopeParamsPtr, RateCalcRecord_t *calcPtr, unsigned char sampleTypeAnalyte, unsigned char sampleTypeOpticalBlank, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned long *rateError)
{
	unsigned char	i;
	unsigned char	slopeRateCalcMethod;
	unsigned char	numPoints;
	unsigned char	deltaSumIdxDiff;
	float			slopeRate;
	float			slopeFalseRate;
	float			intercept;
	float			stdError;
	float			timeDelta;
	float			slopeDelta[MAX_PASSES];
	float			deltaMultiplier;
	float			slopeDeltaMultSum;
	float			multSum;

	SlopeAbsData_t	slopeAbsData;
	SlopeAbsData_t	*slopeAbsPtr;

	slopeAbsPtr = &slopeAbsData;

	slopeRate = 0.0;
	slopeFalseRate = 0.0;
	stdError = 0.0;

	// set up slope calc parameters for calc method

	slopeRateCalcMethod = calcPtr->slopeRateCalcMethod;

	if (slopeRateCalcMethod != WINDOW_LINEAR_REGRESSION_METHOD)
	{
		slopeParamsPtr->numPoints = calcPtr->deltaSumMultRecord.slopeNumPoints;
		slopeParamsPtr->startPass = calcPtr->deltaSumMultRecord.slopeStartPass;
	}

	// get time and absorbance values for passes used to calc slope rate

	if (GetSlopeTimeAbsData(slopeParamsPtr, sampleTypeAnalyte, sampleTypeOpticalBlank, calcPtr, desiredPathLength, analyteCuvetteNumber, slopeAbsPtr, rateError))
	{
		return	TRUE;								// abort algorithm processing flag 
	}

	if (slopeRateCalcMethod == WINDOW_LINEAR_REGRESSION_METHOD)
	{
		// abs data OK & standard rate window linear regression algorithm method, so calc slope, standard error & false rate & check for rate type analyte errors

		numPoints = slopeParamsPtr->numPoints;

		// abs data OK, so calc slope, standard error & false rate & check for rate type analyte errors

		if (numPoints < SLOPE_CALC_POINTS_TOGGLE)
		{


			// IMV - rate paragraph 10

			StoreAlgParagraphNumImv( analyteType, 10.0 );

			// end IMV


			// compute slope used to calculate rate analyte result for 2 points	or less

			timeDelta = slopeAbsPtr->flashTime[1] - slopeAbsPtr->flashTime[0];

			slopeRate =      (slopeAbsPtr->rateStdErrorAbs[1] - slopeAbsPtr->rateStdErrorAbs[0]) / timeDelta;
			slopeFalseRate = (slopeAbsPtr->falseRateAbs[1]    - slopeAbsPtr->falseRateAbs[0])    / timeDelta;
		}
		else
		{


			// IMV - rate paragraph 5

			StoreAlgParagraphNumImv( analyteType, 5.0 );

			// end IMV


			// compute slope used to calculate rate analyte result for multiple points 

			if (LinearRegression(slopeAbsData.flashTime, slopeAbsData.rateStdErrorAbs, numPoints, &slopeRate, &intercept))
			{
				*rateError |= DIVIDE_BY_ZERO;
				return	TRUE;						// abort algorithm processing flag
			}
		 

			// IMV - rate paragraph 6

			StoreAlgParagraphNumImv( analyteType, 6.0 );

			// end IMV


			// compute standard error of computed slope for noise error checks

			if (StandardError(slopeAbsData.flashTime, slopeAbsData.rateStdErrorAbs, numPoints, slopeRate, intercept, &stdError))
			{
				*rateError |= DIVIDE_BY_ZERO;
				return	TRUE;						// abort algorithm processing flag
			}
		 

			// IMV - rate paragraph 7

			StoreAlgParagraphNumImv( analyteType, 7.0 );

			// end IMV


			// compute false rate slope for false rate error checks

			if (LinearRegression(slopeAbsData.flashTime, slopeAbsData.falseRateAbs, numPoints, &slopeFalseRate, &intercept))
			{
				*rateError |= DIVIDE_BY_ZERO;
				return	TRUE;						// abort algorithm processing flag 
			}


			// IMV - rate paragraph 8

			StoreAlgParagraphNumImv( analyteType, 8.0 );

			StoreIntermediateValue( analyteType, IMV_PRE_SLOPE_CORRECTION, IMV_DIRECT_SET_METHOD, slopeRate );

			// end IMV


			// check for noisy slope
		
			*rateError |= SlopeNoiseErrorCheck(slopeRate, stdError, calcPtr);
		

			// IMV - store rate not linear ratio = stdError / fabs(slopeRate)

			StoreIntermediateValue( analyteType, IMV_RATE_NOT_LINEAR_RATIO, IMV_DIRECT_SET_METHOD, stdError / fabs(slopeRate) );

			// end IMV


			// calc slope rate correction (if enabled - for HUMAN rotors only)

			if ((calcPtr->slopeRateCorrectionEnabled) && (rotorConfig_g->rotorProductCode == HUMAN_ROTOR))
			{
				slopeRate += CalcSlopeRateCorrection(analyteType, slopeRate, stdError, slopeFalseRate, calcPtr, sampleTypeAnalyte, sampleTypeOpticalBlank, desiredPathLength, analyteCuvetteNumber, rateError);
			}


			// IMV - rate paragraph 9

			StoreAlgParagraphNumImv( analyteType, 9.0 );

			// end IMV


			// check for false rate	(if enabled)

			if (calcPtr->falseRateCheckEnabled)
			{
				*rateError |= FalseRateErrorCheck(numPoints, slopeRate, slopeFalseRate, calcPtr);
			}
		}
	}
	else
	{
		// abs data OK & calc slopeRate using delta sum multiplier algorithm method  

		deltaSumIdxDiff = calcPtr->deltaSumMultRecord.deltaSumIdxDiff;
		numPoints = slopeParamsPtr->numPoints - deltaSumIdxDiff;

		// only calculate slope using delta multiplied sum if number of passes OK
			 
		slopeDeltaMultSum = 0.0;
		multSum = 0.0;

		for (i = 0; (i < numPoints) && (i < MAX_PASSES); i++)
		{
			slopeDelta[i] = (slopeAbsPtr->rateStdErrorAbs[i+deltaSumIdxDiff] - slopeAbsPtr->rateStdErrorAbs[i]) / (slopeAbsPtr->flashTime[i+deltaSumIdxDiff] - slopeAbsPtr->flashTime[i]);
			deltaMultiplier = calcPtr->deltaSumMultRecord.deltaSumMulitiplier[i];
			slopeDeltaMultSum += slopeDelta[i] * deltaMultiplier;
			multSum += deltaMultiplier;


			// IMV - store slopeDelta[i] intermediate values

			StoreIntermediateValue( analyteType, IMV_RATE_SLOPE_DELTA_START, IMV_INCR_SET_METHOD, slopeDelta[i] );

			// end IMV


		}


		// IMV - reset for next incr store

		InitNextIdxImv();

		// end IMV


		if (multSum == 0.0)
		{
			*rateError |= DIVIDE_BY_ZERO;
			return	TRUE;							// abort algorithm processing flag
		}
		else
		{
			// calc final slope Rate for delta multiplied sums

			slopeRate = slopeDeltaMultSum / multSum; 
		} 
	}

	slopeParamsPtr->slope = slopeRate;	
	slopeParamsPtr->stdError = stdError;	
	slopeParamsPtr->falseRate = slopeFalseRate;	
	 
	return	(*rateError & commonAlgGlobals_g->calcErrorMask);		// abort algorithm processing flag 
}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdRateChecks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic rate algorithm checks for
//		endongenous limits, bead check, bead missing, distribution check & starting absorbance 
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

unsigned ProcessStdRateChecks(unsigned char analyteType, unsigned char sampleTypeAnalyte, unsigned char sampleTypeOpticalBlank, unsigned char analyteCuvetteNumber, RateAlgRecord_t *rateAlgPtr, CommonFactorsLimits_t *factorsLimitsPtr, unsigned long *rateError)
{
	//	Check endogenous limits from sample blank results

	EndogenousLimitsCheck(analyteType, factorsLimitsPtr, rateError);


	// IMV - rate paragraph 2

	StoreAlgParagraphNumImv( analyteType, 2.0 );

	// end IMV


	//	Check bead mix of analyte and bead mix of blank if enabled

	if (BeadMixCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, rateAlgPtr->beadMixCheckPass, rateAlgPtr->beadMixCheckLimit, TEST_BEAD_MIX_ERROR, rateError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check for analyte bead missing and for blank bead missing if enabled

	if (BeadMissingCheck(analyteCuvetteNumber, rateAlgPtr->beadMissingCheckLimit, rateError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check for analyte bead missing and for blank bead missing if enabled

	if (BeadDistributionCheck(analyteCuvetteNumber, rateAlgPtr->distributionCheckLimit, rateError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check starting absorbance of analyte and blank if enabled

	if (StartingAbsorbanceCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, rateAlgPtr->startAbsCheckPass, rateAlgPtr->startAbsCheckWavelengthPair[0], rateAlgPtr->startAbsLimit, rateAlgPtr->startAbsAnalyteErrorMask[0], rateError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	if (StartingAbsorbanceCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, rateAlgPtr->startAbsCheckPass, rateAlgPtr->startAbsCheckWavelengthPair[1], rateAlgPtr->startAbsLimit, rateAlgPtr->startAbsAnalyteErrorMask[1], rateError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}
	
	return FALSE;						// all checks pass OK
} 


// ********************************************************************
// FUNCTION:
//
//		ProcessStdRateSlope() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic rate algorithm processor for slope used in rate algorithm calculation 
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

float ProcessStdRateSlope(unsigned char analyteType, RateAlgRecord_t *rateAlgPtr, CommonFactorsLimits_t *factorsLimitsPtr, float *falseRate, unsigned char *numSlopePoints, unsigned long *rateError)
{ 
	unsigned char		sampleTypeAnalyte;		
	unsigned char		sampleTypeAnalyteBlank;		
	unsigned char		sampleTypeOpticalBlank;
	unsigned char		analyteCuvetteNumber;
	unsigned char		analyteBlankCuvetteNumber;			
	float				desiredPathLength;

	SlopeParams_t 		slopeParams;
	SlopeParams_t 		*slopeParamsPtr;

	slopeParamsPtr = &slopeParams;

	// set up common parameters from rotor & algorithm specific configuration data

	sampleTypeAnalyte         = rateAlgPtr->sampleTypeAnalyte;	
	sampleTypeAnalyteBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	analyteBlankCuvetteNumber = rotorConfig_g->analyteCalcRecord[analyteType].analyteBlankCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	if (ProcessStdRateChecks(analyteType, sampleTypeAnalyte, sampleTypeOpticalBlank, analyteCuvetteNumber, rateAlgPtr, factorsLimitsPtr, rateError))
	{
		return	NOVAL_NEG_FLOAT;			// got a calc type error (divide by zero, etc.) or starting abs error
	} 


	// IMV - rate paragraph 3

	StoreAlgParagraphNumImv( analyteType, 3.0 );

	// end IMV


	if (rateAlgPtr->rateCalcRecord.slopeRateCalcMethod == WINDOW_LINEAR_REGRESSION_METHOD)
	{
		if (SelectSlopeWindow(slopeParamsPtr, &rateAlgPtr->windowSelectRecord, sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, rateError))
		{
			return	NOVAL_NEG_FLOAT;		// got a calc type error (divide by zero, etc.) or pre - ending abs check error
		}
	} 


	// IMV - rate paragraph 4

	StoreAlgParagraphNumImv( analyteType, 4.0 );

	// end IMV


	if (CalcSlopeParameters(analyteType, slopeParamsPtr, &rateAlgPtr->rateCalcRecord, sampleTypeAnalyte, sampleTypeOpticalBlank, desiredPathLength, analyteCuvetteNumber, rateError))
	{
		return	NOVAL_NEG_FLOAT;		// got a calc type error (divide by zero, etc.)
	}


 	// IMV - rate standard slope / window abs / limits params

	StoreRateSlopeWindowLimitsParamsImv( analyteType, rateAlgPtr, slopeParamsPtr );
			 
	// end IMV


	// exit with slope related values for rate concentration calc & associated error checks

	*falseRate = slopeParams.falseRate;
	*numSlopePoints = slopeParams.numPoints;
	return	slopeParams.slope;
}


// ********************************************************************
// FUNCTION:
//
//		CalcTemperatureCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct the rate of slope for rotor averaage temperature  
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

float CalcTemperatureCorrection(float arrhTemperatureCorrectionFactor)
{
	float	rotorAverageTemp;

	rotorAverageTemp = rotorResults_g->rotorInformationResults.rotorAverageTemperature;


	// IMV - set temperature correction params

	SetImvRateTemperatureCorrectionParams( arrhTemperatureCorrectionFactor, rotorAverageTemp, (arrhTemperatureCorrectionFactor / (rotorAverageTemp + CELSIUS_TO_KELVIN)) - (arrhTemperatureCorrectionFactor / 310.0), exp((arrhTemperatureCorrectionFactor / (rotorAverageTemp + CELSIUS_TO_KELVIN)) - (arrhTemperatureCorrectionFactor / 310.0)) );

	// end IMV


	return	exp((arrhTemperatureCorrectionFactor / (rotorAverageTemp + CELSIUS_TO_KELVIN)) - (arrhTemperatureCorrectionFactor / 310.0));
}


// ********************************************************************
// FUNCTION:
//
//		CalcNaRotorStabilityRateCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct the slope rate for NA_1 and NA_2 rotor stability decay  
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

float CalcNaRotorStabilityRateCorrection(unsigned char analyteType, float slopeRate, RateAlgRecord_t *rateAlgPtr)
{
	unsigned char	naCorrectionLevelFlag;
	float			uncorrectedNaResult;
	float			naCorrectedRate;

	const float		naStabilityRateCorrectionFactor[MAX_NA_RATE_CORRECTION_LEVELS] =  // NA-1 & NA_2 rate linear time correction now disabled 
	{
		0.000350,		// level 0: default
		0.000220,		// level 1: rotors 49, 59, 63 	(approximate existing correction)
		0.000290,		// level 2: rotors 27, 48
		0.000420,		// level 3: rotors 64
		0.000490,		// level 4: rotors 				(none identified)
	};

	// Only correct rate for NA_1 or NA_2 analytes to adjust slopeRate for rotor stability correction

	if ( (analyteType == NA_1) || (analyteType == NA_2) )
	{
		// IMV - Store NA_1 or NA_2 uncorrected result 

		uncorrectedNaResult = CalcBfdStdBarcodeCorrectedResult(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType, STD_SLOPE1_IDX, STD_INTERCEPT1_IDX);

		StoreIntermediateValue( analyteType, IMV_RESULT_NA_UNCORRECTED, IMV_DIRECT_SET_METHOD, uncorrectedNaResult );

		// end IMV


		naCorrectionLevelFlag = rotorConfig_g->analyteCalcRecord[analyteType].algorithmFlags[ROTOR_ROC_NA_DECAY_CORRECT_LEVEL_FLAG];

		if ( naCorrectionLevelFlag >= MAX_NA_RATE_CORRECTION_LEVELS )
		{
			naCorrectionLevelFlag = 0;	// if NA rate correction level out of range, clamp to level 0 - default
		}

		naCorrectedRate = slopeRate * ( 1 + ( naStabilityRateCorrectionFactor[ naCorrectionLevelFlag ] * barcodeData_g->rotorAgeInDays ) );

		return	naCorrectedRate;
	}
	else
	{
		return	slopeRate;
	}
}


// ********************************************************************
// FUNCTION:
//
//		CalcResultFalseRateCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct the rate result for false rate  
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

float CalcResultFalseRateCorrection(float falseRate, float falseRateCorrectMultiplier)
{
	return (falseRateCorrectMultiplier * falseRate);
}


// ********************************************************************
// FUNCTION:
//
//		CalcEndogenousResultCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct the rate result for endogenous correction method  
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

float CalcEndogenousResultCorrection( unsigned char analyteType, unsigned char endogenousResultCorrectionMethod, CommonFactorsLimits_t	*factorsLimitsPtr )
{
	float	endogenousIndex;
	float	endogenousCorrectionMultiplier;

	switch (endogenousResultCorrectionMethod)
	{
		case HEMOLYZED_RESULT_CORRECTION:
			endogenousIndex = rotorResults_g->rotorInformationResults.rotorHemolyzedIndex; 
			endogenousCorrectionMultiplier = factorsLimitsPtr->hemolyzedCorrectionFactors[ENDOGENOUS_RESULT_CORRECTION_IDX]; 
			break;
		case LIPEMIC_RESULT_CORRECTION:
			endogenousIndex = rotorResults_g->rotorInformationResults.rotorLipemicIndex; 
			endogenousCorrectionMultiplier = factorsLimitsPtr->lipemicCorrectionFactors[ENDOGENOUS_RESULT_CORRECTION_IDX]; 
			break;
		case ICTERIC_RESULT_CORRECTION:
			endogenousIndex = rotorResults_g->rotorInformationResults.rotorIctericIndex; 
			endogenousCorrectionMultiplier = factorsLimitsPtr->ictericCorrectionFactors[ENDOGENOUS_RESULT_CORRECTION_IDX]; 
			break;
		default:
			endogenousIndex = 0.0;
			endogenousCorrectionMultiplier = 0.0;
			break;
	} 


	// IMV - endogenous result correction multiplier parameter store

	StoreIntermediateValue( analyteType, IMV_RESULT_ENDOGENOUS_MULT, IMV_DIRECT_SET_METHOD, endogenousCorrectionMultiplier );

	// end IMV


	return (endogenousCorrectionMultiplier * endogenousIndex);
}


// ********************************************************************
// FUNCTION:
//
//		CalcNaIdentifiedRotorLotRecoveryCorrection() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Correct to remove offset bias in the rateResult for NA_1 and NA_2 identified rotors within Julian time range   
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

float CalcNaIdentifiedRotorLotRecoveryCorrection(unsigned char analyteType, float rateResult)
{
	float			rateResultFinal;

	time_t			t;
	struct tm		*currentDate;
	unsigned short	currentYear;
	int				lsdOfCurrentYear;
	unsigned short	mfgYear;
	unsigned		mfgJulian;
	unsigned char	rotorId;							
      
	rateResultFinal = rateResult;

	// only correct for NA_1 or NA_2

	if ( (analyteType == NA_1) || (analyteType == NA_2) )
	{
		// get this rotor ID for compare / barcode / lot manufacturing / date for NA correction check

		rotorId	= barcodeData_g->rotorId;

		// only correct for identified rotors = 27,42,43,44,52,64,71

		if ( ( rotorId == 27 ) || ( rotorId == 42 ) || ( rotorId == 43 ) || ( rotorId == 44 ) || ( rotorId == 52 ) || ( rotorId == 64 ) || ( rotorId == 71 ) ) 
		{
			// create Julian barcode / lot manufacturing date for K+ correction check compare
			// Get the current date, and extract the current year.

			t = time( 0 );
			currentDate = gmtime( &t );
			currentYear = currentDate->tm_year + 1900;
      
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


			// check if identified rotor lots including NA_1 or NA_2 are within Julian time range to compute the lot correction

			if ( ( mfgJulian > JULIAN_11_05_2017 ) && ( mfgJulian < JULIAN_03_05_2018 ) )
			{
				rateResultFinal = rateResult - NA_IDENTIFIED_LOT_OFFSET_ADJUST; 
 			}
		}
	}

	return rateResultFinal;
}


// ********************************************************************
// FUNCTION:
//
//		UnexpectedResultCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set analyte error flag for unexpected analyte result value  
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

unsigned long UnexpectedResultCheck(unsigned char numSlopePoints, float result, float limit)
{
	if ((numSlopePoints < SLOPE_CALC_POINTS_TOGGLE) && (result < limit))
	{
		return RATE_UNEXPECTED_RESULT;
	}
	
	return NO_ANALYTE_ERROR;	 
}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdRateAlgorithm() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic rate algorithm processor for rate analyte results & error flags 
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

void ProcessStdRateAlgorithm(unsigned char analyteType)
{
	float				rateResult;
	unsigned long		rateError;

	float				slopeRate;
	float				falseRate;
	unsigned char		numSlopePoints;

	RateAlgRecord_t			*rateAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// init rate result & flags

	rateResult =  NOVAL_NEG_FLOAT;
	rateError =  NO_ANALYTE_ERROR;

	// get rate analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, RATE);
	
	rateAlgPtr = &rateConfig_g.rateAlgRecord;		

	// process rate standard slope algorithm

	slopeRate = ProcessStdRateSlope(analyteType, rateAlgPtr, factorsLimitsPtr, &falseRate, &numSlopePoints, &rateError);

	if (slopeRate != NOVAL_NEG_FLOAT)
	{


		// IMV - rate paragraph 11

		StoreAlgParagraphNumImv( analyteType, 11.0 );

		// end IMV


		// Perform temperature correction on the slope rate 

		slopeRate *= CalcTemperatureCorrection(rateAlgPtr->rateCalcRecord.arrhTemperatureCorrectionFactor);

		// Correct slopeRate for NA_1 & NA_2 rotor stability
		
		slopeRate = CalcNaRotorStabilityRateCorrection(analyteType, slopeRate, rateAlgPtr);  

		// Check if BFD false rate correction enabled

		if (rateAlgPtr->rateCalcRecord.bfdFalseRateCorrectionEnabled)
		{
			rateResult = CalcBfdFalseRateBarcodeCorrectedResult(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType, rateAlgPtr->rateCalcRecord.bfdFalseRateCorrectionLimit, rateAlgPtr->rateCalcRecord.bfdFalseRateCorrectionFactor, falseRate);
		}
		else
		{
			// Compute result using standard rate result using standard equation with BFD cal correction & slope/intercept barcode parameters or quadratic parameters  

			switch (rateAlgPtr->rateCalcRecord.barcodeFactorSlopeIntCalcMethod)
			{
				case STANDARD_QUADRATIC:
					rateResult = CalcBfdQuadraticBarcodeCorrectedResult(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType);
					break;
				case BASE_FINE_EXP:
					rateResult = CalcBfdExpBaseFineBarcodeCorrectedResult(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType);
					break;
				case BFD_ICTERIC_SLOPE_INT:
					rateResult = CalcBfdIctericBarcodeCorrectedResult(slopeRate, 0.0, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType, factorsLimitsPtr);
					break;
				case SIGMOID_LOGIT_LOG4:
					rateResult = CalcBfdSigmoidLogitLog4Result(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType, &rateError);
					break;
				case STANDARD_SLOPE_INT:
				default:
					rateResult = CalcBfdStdBarcodeCorrectedResult(slopeRate, rateAlgPtr->rateCalcRecord.bfdCalibrationFactorIndex, analyteType, STD_SLOPE1_IDX, STD_INTERCEPT1_IDX);
					break;
			}
		}


		// IMV - linear time correction stability rate parameter

		StoreIntermediateValue( analyteType, IMV_RESULT_NO_DECAY, IMV_DIRECT_SET_METHOD, rateResult );

		// end IMV


		// Perform false rate correction for the final rate result if enabled

		if (rateAlgPtr->rateCalcRecord.falseRateCorrectionEnabled)
		{
			rateResult += CalcResultFalseRateCorrection(falseRate, rateAlgPtr->rateCalcRecord.falseRateCorrectionMultiplier);


			// IMV - rate result false rate correction parameter

			StoreIntermediateValue( analyteType, IMV_RESULT_FALSE_RATE_MULT, IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.falseRateCorrectionMultiplier );

			// end IMV


		}

		// Perform endogenous correction for the final rate result if enabled

		if (rateAlgPtr->rateCalcRecord.endogenousResultCorrectionMethod != NO_ENDOGENOUS_RESULT_CORRECTION)
		{
			rateResult += CalcEndogenousResultCorrection(analyteType, rateAlgPtr->rateCalcRecord.endogenousResultCorrectionMethod, factorsLimitsPtr);
		}


		// IMV - rate paragraph 12

		StoreAlgParagraphNumImv( analyteType, 12.0 );

		// end IMV


		// Perform linear time correction for the final rate result if enabled

		if (rateAlgPtr->rateCalcRecord.linearTimeCorrectionEnabled)
		{
			rateResult *= CalcLinearTimeCorrection(analyteType, rateAlgPtr->rateCalcRecord.linearTimeCorrectionStabilityRate);
		}


		// Correct rateResult for NA_1 & NA_2 for identified rotor lots to remove bias
		
		rateResult = CalcNaIdentifiedRotorLotRecoveryCorrection(analyteType, rateResult);  

		// IMV - rate paragraph 13

		StoreAlgParagraphNumImv( analyteType, 13.0 );

		// end IMV


		// check if for unexpected result error if enabled

		if (rateAlgPtr->unexpectedValueCheckEnabled)
		{
			rateError |= UnexpectedResultCheck(numSlopePoints, rateResult, factorsLimitsPtr->highDynamicRange - rateAlgPtr->unexpectedValueHighDynamicRangeAdjust);


			// IMV - unexpected value high dynamic range adjust parameter

			StoreIntermediateValue( analyteType, IMV_HIGH_DYNAMIC_RANGE_ADJUST, IMV_DIRECT_SET_METHOD, rateAlgPtr->unexpectedValueHighDynamicRangeAdjust );

			// end IMV


		}
			

		// IMV - rate paragraph 14

		StoreAlgParagraphNumImv( analyteType, 14.0 );

		// end IMV


		// check if RQC error if enabled

		if (rateAlgPtr->rqcCheckEnabled)
		{
			rateError |= RqcAbusedCheck();
		}
			

		// IMV - rate paragraph 16

		StoreIntermediateValue( analyteType, IMV_ANALYTE_RESULT, IMV_DIRECT_SET_METHOD, rateResult );

		StoreAlgParagraphNumImv( analyteType, 16.0 );

		StoreRateResultCalcParamsImv( analyteType );

		// end IMV


		// Check standard result for within system & dynamic range limits

		ResultLimitsCheck(analyteType, rateResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &rateError);
	}


	// IMV - rate paragraph 17

	StoreAlgParagraphNumImv( analyteType, 17.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, rateResult, rateError, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdPosNegChecks() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic positive / negative result algorithm checks for
//		endongenous limits, bead check, bead missing, distribution check & starting absorbance 
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

unsigned ProcessStdPosNegChecks(unsigned char analyteType, unsigned char sampleTypeAnalyte, unsigned char sampleTypeOpticalBlank, unsigned char analyteCuvetteNumber, PosNegAlgRecord_t *posNegAlgPtr, CommonFactorsLimits_t *factorsLimitsPtr, unsigned long *posNegError)
{
	unsigned	i;

	//	Check endogenous limits from sample blank results

	EndogenousLimitsCheck(analyteType, factorsLimitsPtr, posNegError);

	//	Check bead mix of analyte 

	if (BeadMixCheck(sampleTypeAnalyte, sampleTypeOpticalBlank, posNegAlgPtr->beadMixCheckPass, posNegAlgPtr->beadMixCheckLimitAnalyte, TEST_BEAD_MIX_ERROR, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check bead mix of opt cuv

	if (BeadMixCheck(posNegAlgPtr->sampleTypeOpt, sampleTypeOpticalBlank, posNegAlgPtr->beadMixCheckPass, posNegAlgPtr->beadMixCheckLimitOpt, BLANK_BEAD_MIX_ERROR, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}


	// IMV - store pos/neg bead mix params / pos/neg algorithm paragraph 2

	StoreBeadMixImv( analyteType );

	StoreAlgParagraphNumImv( analyteType, 2.0 );

	// end IMV


	//	Check for analyte bead missing 

	if (BeadMissingCheck(analyteCuvetteNumber, posNegAlgPtr->beadMissingCheckLimitAnalyte, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check for opt cuv bead missing 

	if (BeadMissingCheck(posNegAlgPtr->optCuvetteNumber, posNegAlgPtr->beadMissingCheckLimitOpt, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check for analyte distribution error 

	if (BeadDistributionCheck(analyteCuvetteNumber, posNegAlgPtr->distributionCheckLimitAnalyte, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}

	//	Check for opt cuv distribution error 

	if (BeadDistributionCheck(posNegAlgPtr->optCuvetteNumber, posNegAlgPtr->distributionCheckLimitOpt, posNegError))
	{
		return	TRUE;					// got a calc type error (divide by zero, etc.)
	}


	// IMV - pos/neg algorithm paragraph 3

	StoreAlgParagraphNumImv( analyteType, 3.0 );

	// end IMV


	//	Check starting absorbance of analyte for wavelength high limit or wavelength low limit for cuvette and opt cuvette - up to 8 combinations 

	for (i = 0; i < POS_NEG_START_ABS_CHECKS; i++)
	{
		if ( posNegAlgPtr->startAbsCheck[i].startAbsCheckMethodHigh == TRUE )	// check for over limit - standard
		{


			// IMV - store IMV_POS_NEG_START_ABS_HI_LIMIT

			StoreIntermediateValue( analyteType, IMV_POS_NEG_START_ABS_HI_LIMIT, IMV_DIRECT_SET_METHOD, posNegAlgPtr->startAbsCheck[i].startAbsLimit );

			// end IMV


			if (StartingAbsorbanceCheck(posNegAlgPtr->startAbsCheck[i].startAbsSampleType, sampleTypeOpticalBlank, posNegAlgPtr->startAbsCheckPass, posNegAlgPtr->startAbsCheck[i].startAbsCheckWavelengthPair, posNegAlgPtr->startAbsCheck[i].startAbsLimit, START_ABS_BAD, posNegError))
			{
				return	TRUE;			// got a calc type error (divide by zero, etc.)	or starting abs error
			}
		}
		else	// check for under limit 
		{


			// IMV - store IMV_POS_NEG_START_ABS_LO_LIMIT

			StoreIntermediateValue( analyteType, IMV_POS_NEG_START_ABS_LO_LIMIT, IMV_DIRECT_SET_METHOD, posNegAlgPtr->startAbsCheck[i].startAbsLimit );

			// end IMV


			if (StartingAbsorbanceCheckLow(posNegAlgPtr->startAbsCheck[i].startAbsSampleType, sampleTypeOpticalBlank, posNegAlgPtr->startAbsCheckPass, posNegAlgPtr->startAbsCheck[i].startAbsCheckWavelengthPair, posNegAlgPtr->startAbsCheck[i].startAbsLimit, START_ABS_BAD, posNegError))
			{
				return	TRUE;			// got a calc type error (divide by zero, etc.) or starting abs error
			}
		}
	}


	// IMV - store pos/neg starting abs / pos/neg algorithm paragraph 4

	StorePosNegStartAbsImv( analyteType );

	StoreAlgParagraphNumImv( analyteType, 4.0 );

	// end IMV


	return FALSE;						// all checks pass OK
} 


// ********************************************************************
// FUNCTION:
//
//		CalcStdPosNegRatio() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate ratio for abs at 2 wavelengths for analyte cuv minus 
//		ratio for abs at 2 wavelengths for optical cuv 
//		for std pos / neg algorithm 
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

unsigned CalcStdPosNegRatio(unsigned char analyteType, unsigned char sampleTypeAnalyte, unsigned char sampleTypeAnalyteBlank, unsigned char sampleTypeOpticalBlank, PosNegAlgRecord_t *posNegAlgPtr, float desiredPathLength, unsigned char analyteCuvetteNumber, unsigned char analyteBlankCuvetteNumber, float *posNegRatioResult, float *posNegRatioCutoff, unsigned long *posNegError)
{
	unsigned		i;
	unsigned		j;

	unsigned char	startPass;
	unsigned char	endPass;

	float			avgData[POS_NEG_NUM_RATIO_WAVELENGTHS];
	float			avgRatioAnalyte;
	float			avgRatioOpt;
	float			netRatioResult[POS_NEG_NUM_RATIO_RESULTS];

	AvgAbs_t 		absData;
	AvgAbs_t 		*absPtr = &absData;

	float			barcodeWbOffset;
	float			barcodeConjType;

	const ImvType_t	imvPosNegPasses_m[POS_NEG_NUM_RATIO_RESULTS][POS_NEG_NUM_VALUES_FOR_PASSES]= 
	{
		{
			IMV_POS_NEG_TEST_RATIO_0,
			IMV_POS_NEG_OPT_RATIO_0,
			IMV_POS_NEG_NET_RATIO_0,
		},

		{
			IMV_POS_NEG_TEST_RATIO_1,
			IMV_POS_NEG_OPT_RATIO_1,
			IMV_POS_NEG_NET_RATIO_1,
		},
	};


	// load barcode calibration factor limits / logic for setting positive / negative result

	*posNegRatioCutoff	= GetBarcodeParameter(analyteType, POS_NEG_RATIO_CUTOFF_IDX);
	barcodeWbOffset 	= GetBarcodeParameter(analyteType, POS_NEG_WB_OFFSET_IDX);
	barcodeConjType 	= GetBarcodeParameter(analyteType, POS_NEG_CONJ_TYPE_IDX);
	
	for (j = 0; j < POS_NEG_NUM_RATIO_RESULTS; j++)
	{
		// init common params

		startPass = posNegAlgPtr->posNegCalcRecord.ratioAvgStartPass[j];
		endPass   = posNegAlgPtr->posNegCalcRecord.ratioAvgEndPass[j];

		// get average for wavelengths for analyte cuv ratio 
	 
		for (i = 0; i < POS_NEG_NUM_RATIO_WAVELENGTHS; i++)
		{
			if (CalcAverageAbsorbance(absPtr, sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, startPass, endPass, posNegAlgPtr->posNegCalcRecord.ratioWavelengthPair[i], desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, posNegAlgPtr->posNegCalcRecord.blankCorrectionOrderAnalyte, posNegError))
			{
				return	TRUE;						// abort algorithm processing flag
			}

			avgData[i] = absPtr->avg;
		}

		avgRatioAnalyte = avgData[1] / avgData[0];
		 
		// get average for wavelengths for optical cuv ratio 
	 
		for (i = 0; i < POS_NEG_NUM_RATIO_WAVELENGTHS; i++)
		{
			if (CalcAverageAbsorbance(absPtr, posNegAlgPtr->sampleTypeOpt, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, startPass, endPass, posNegAlgPtr->posNegCalcRecord.ratioWavelengthPair[i], desiredPathLength, posNegAlgPtr->optCuvetteNumber, analyteBlankCuvetteNumber, posNegAlgPtr->posNegCalcRecord.blankCorrectionOrderOpt, posNegError))
			{
				return	TRUE;						// abort algorithm processing flag
			}

			avgData[i] = absPtr->avg;
		}

		avgRatioOpt = avgData[1] / avgData[0];

		// compute positive / negative result ratio	as difference of analyte average - optical average

		netRatioResult[j] = avgRatioAnalyte - avgRatioOpt;


		// IMV - store pos/neg ratio params depending on passes used 

		StoreIntermediateValue( analyteType, imvPosNegPasses_m[j][0], IMV_DIRECT_SET_METHOD, avgRatioAnalyte );
		StoreIntermediateValue( analyteType, imvPosNegPasses_m[j][1], IMV_DIRECT_SET_METHOD, avgRatioOpt );
		StoreIntermediateValue( analyteType, imvPosNegPasses_m[j][2], IMV_DIRECT_SET_METHOD, netRatioResult[j] );

		// end IMV
	}		 

	// check barcode parameters conjugate type for logic on which passes net result to use and if cutoff needs adjustment

	if ( barcodeConjType == 0.0 )	
	{
		// use selected passes 0 results and barcode cutoff

		*posNegRatioResult = netRatioResult[0];


		// IMV - store pos/neg ratio params depending on passes used 

		StoreIntermediateValue( analyteType, IMV_POS_NEG_NET_RATIO_SELECTED, IMV_DIRECT_SET_METHOD, netRatioResult[0] );

		// end IMV
	}
	else							
	{
		// use selected passes 1 net ratio and adjust net ratio depending on cuvette 1 distribution level

		*posNegRatioResult = netRatioResult[1];
		

		// IMV - store pos/neg ratio params depending on passes used 

		StoreIntermediateValue( analyteType, IMV_POS_NEG_NET_RATIO_SELECTED, IMV_DIRECT_SET_METHOD, netRatioResult[1] );

		// end IMV


		// if whole blood sample, adjust ratio and set pos/neg analyte whole blood flag

		if ( ( rotorResults_g->rotorInformationResults.rotorSystemFlags & SAMPLE_WHOLE_BLOOD ) == SAMPLE_WHOLE_BLOOD )
		{
			*posNegRatioResult -= barcodeWbOffset;	// adjust ratio with whole blood offset barcode param

			*posNegError |= POS_NEG_WB;				// OR in whole blood flag for whole blood and ratio adjusted flag
		}
	}


	// IMV - store barcode params, final ratio result & pos/neg algorithm paragraph 6

	StoreIntermediateValue( analyteType, IMV_POS_NEG_FINAL_RATIO_RESULT, IMV_DIRECT_SET_METHOD, *posNegRatioResult );

	StoreBarcodeParamsImv( analyteType);

	StoreAlgParagraphNumImv( analyteType, 6.0 );

	// end IMV


	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		SetPosNegResultFlags() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set positive / negative result flag and error flags based on ratio & rate limits 
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

void SetPosNegResultFlags(unsigned char analyteType, float posNegRatioResult, float posNegRatioCutoff, PosNegAlgRecord_t *posNegAlgPtr, unsigned long *posNegError)
{

	// check ratio versus high and low limits for good ratio

	if (( posNegRatioResult < posNegAlgPtr->posNegCalcRecord.ratioLowLimit ) || ( posNegRatioResult > posNegAlgPtr->posNegCalcRecord.ratioHighLimit ))
	{
		*posNegError |= POS_NEG_RATIO_ERROR;	
	}

	// Check for ratio above the ratio cutoff barcode factor for a positive result

	if ( posNegRatioResult > posNegRatioCutoff )
	{
		*posNegError |= POS_NEG_RESULT_POSITIVE;	
	}


	// IMV - store pos/neg check limits & pos/neg algorithm paragraph 7

	StoreIntermediateValue( analyteType, IMV_POS_NEG_LO_RATIO_LIMIT,    IMV_DIRECT_SET_METHOD, posNegAlgPtr->posNegCalcRecord.ratioLowLimit );
	StoreIntermediateValue( analyteType, IMV_POS_NEG_HI_RATIO_LIMIT,    IMV_DIRECT_SET_METHOD, posNegAlgPtr->posNegCalcRecord.ratioHighLimit );

	StoreAlgParagraphNumImv( analyteType, 7.0 );

	// end IMV


}


// ********************************************************************
// FUNCTION:
//
//		ProcessStdPosNegAlgorithm() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Standard generic Positive / Negative type result algorithm processor with ratio / rate computed for result & flags 
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

void ProcessStdPosNegAlgorithm(unsigned char analyteType)
{
	unsigned char		sampleTypeAnalyte;		
	unsigned char		sampleTypeAnalyteBlank;		
	unsigned char		sampleTypeOpticalBlank;
	unsigned char		analyteCuvetteNumber;
	unsigned char		analyteBlankCuvetteNumber;			
	float				desiredPathLength;

	float				posNegRatioResult;
	float				posNegRatioCutoff;
	unsigned long		posNegError;

	PosNegAlgRecord_t		*posNegAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// init rate result & flags

	posNegRatioResult =  NOVAL_NEG_FLOAT;
	posNegRatioCutoff =  NOVAL_NEG_FLOAT;
	posNegError =  NO_ANALYTE_ERROR;

	// get rate analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, POS_NEG_ALG);
	
	posNegAlgPtr = &posNegConfig_g.posNegAlgRecord;		

	// set up common parameters from rotor & algorithm specific configuration data

	sampleTypeAnalyte         = posNegAlgPtr->sampleTypeAnalyte;	
	sampleTypeAnalyteBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[analyteType].sampleTypeOpticalBlank;	
	analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[analyteType].analyteCuvetteNumber;	
	analyteBlankCuvetteNumber = rotorConfig_g->analyteCalcRecord[analyteType].analyteBlankCuvetteNumber;	
	desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[analyteType].analyteDesiredPathLength;

	if (ProcessStdPosNegChecks(analyteType, sampleTypeAnalyte, sampleTypeOpticalBlank, analyteCuvetteNumber, posNegAlgPtr, factorsLimitsPtr, &posNegError) == FALSE)
	{
		if (CalcStdPosNegRatio(analyteType, sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, posNegAlgPtr, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, &posNegRatioResult, &posNegRatioCutoff, &posNegError) == FALSE)
		{
			// Set positive / negative result flag and error flags based on ratio & rate limits

			SetPosNegResultFlags(analyteType, posNegRatioResult, posNegRatioCutoff, posNegAlgPtr, &posNegError);

			ResultLimitsCheck(analyteType, posNegRatioResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &posNegError);
		}
	}


	// IMV - pos/neg algorithm paragraph 8

	StoreAlgParagraphNumImv( analyteType, 8.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, posNegRatioResult, posNegError, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		Check2CuvLimitsFlags() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Common 2 cuvette result checks for:
//			2 cuvette results inconsistent result,
//			set of resultPrintFlag for rotor hemogenous limit errors
//			2 cuvette results system & dynamic range check
//		 
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

void Check2CuvLimitsFlags(unsigned char analyteType2CuvResult, float analyteResult1, float analyteResult2, float analyteResult, CommonFactorsLimits_t *factorsLimitsPtr, unsigned char inconsistentResultCheckEnabled, float inconsistentResultDiffLimit, unsigned long *analyteFlags)
{
	// Check for inconsistent result between cuvette 1 & 2 if enabled

	if (inconsistentResultCheckEnabled)
	{
		if ( fabs(analyteResult1 - analyteResult2) >= inconsistentResultDiffLimit )
		{
			*analyteFlags |= INCONSISTENT_RESULT;
		} 
	}

	// Make sure to update resultPrintFlags for any endogenous zone limit found for 2 cuvette analtyeType results

	EndogenousLimitsCheck(analyteType2CuvResult, factorsLimitsPtr, analyteFlags);

	// Check combined 2 cuv result for within system & dynamic range limits, clear flags before check

	*analyteFlags &= (~ANALYTE_RESULT_LIMITS_ERR_MASK);

	ResultLimitsCheck(analyteType2CuvResult, analyteResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, analyteFlags);
}


// ********************************************************************
// FUNCTION:
//
//		Calc2CuvAnalyteResult() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate combined analyte result & flagsfor analyte 
//		that requires 2 cuvettes for algorithm 
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

void Calc2CuvAnalyteResult(unsigned char analyteType2CuvResult, unsigned char analyteTypeCuv2, float analyteResult1, unsigned long analyteFlags1, float analyteResult2, unsigned long analyteFlags2, Calc2CuvRecord_t *calc2CuvPtr, CommonFactorsLimits_t *factorsLimitsPtr)
{
	unsigned long	analyteFlags;
	float			analyteResult;
	float			blendFactor;
	float			startCuvBlendLimit;
	float			endCuvBlendLimit;
	float			inconsistentResultDiffLimit;
	unsigned char	inconsistentResultCheckEnabled;

	analyteResult = NOVAL_NEG_FLOAT;

	blendFactor = NOVAL_NEG_FLOAT;
	inconsistentResultDiffLimit = NOVAL_NEG_FLOAT; 

	// calc 2 cuvette results and set flags based on 2 cuvette type or calc method

	if ( calc2CuvPtr->calc2CuvType == CALC_2_CUV_AVG_1_CUV_OK )	// case CALC_2_CUV_AVG_1_CUV_OK: 2 cuvette average algorithm for results if 1 cuvette is OK
	{
		analyteResult = NOVAL_NEG_FLOAT;
		inconsistentResultCheckEnabled = FALSE;
		inconsistentResultDiffLimit = NOVAL_POS_FLOAT;

		if ( ((analyteResult1 == NOVAL_NEG_FLOAT) && (analyteResult2 == NOVAL_NEG_FLOAT)) || 
			 ((analyteFlags1 & calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask) && (analyteFlags2 & calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask)) )
		{
			// both results are bad or need to be suppressed so suppress 2 cuvette result
			analyteFlags  = (analyteFlags1 | analyteFlags2) & ~(CALC_2_CUVETTE_1_USED | CALC_2_CUVETTE_2_USED);
		}
		else
		{
			// check each cuvette for suppression to set which cuvettes used

			if ((analyteFlags1 & calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask) == NO_ANALYTE_ERROR)
			{
				// setup cuvette 1 result and flags in case only one of two cuvettes OK
				 
				analyteResult = analyteResult1;
				analyteFlags1 |= CALC_2_CUVETTE_1_USED;
			}

			if ((analyteFlags2 & calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask) == NO_ANALYTE_ERROR)
			{
				// setup cuvette 2 result and flags in case only one of two cuvettes OK

				analyteResult = analyteResult2;
				analyteFlags2 |= CALC_2_CUVETTE_2_USED;
			}

			// combine flags for final result, even though one cuvette may be suppressed

			analyteFlags  = analyteFlags1 | analyteFlags2;

			if ((analyteFlags & CALC_2_CUVETTE_1_USED) && (analyteFlags & CALC_2_CUVETTE_2_USED))
			{
				// average result since both cuvettes not suppressed

				analyteResult = (analyteResult1 + analyteResult2) / 2.0;

				// set up for inconsistent result check
				
				if (analyteResult < calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckToggle)
				{
					inconsistentResultDiffLimit = calc2CuvPtr->inconsistentResultDiffLimit;

					inconsistentResultCheckEnabled = TRUE;
				}
				else if (analyteResult < calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckMax)
				{
					inconsistentResultDiffLimit = calc2CuvPtr->inconsistentResultDiffLimit + (calc2CuvPtr->avg1CuvOkCheckData.resultDiffLimitScale * (analyteResult - calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckToggle));

					inconsistentResultCheckEnabled = TRUE;
				} 
			}

			// check 2 cuvette inconsistent result limit, set endogenous resultPrintFlags, check 2 cuvette result system & dynamic range limits
			 
			Check2CuvLimitsFlags(analyteType2CuvResult, analyteResult1, analyteResult2, analyteResult, factorsLimitsPtr, inconsistentResultCheckEnabled, inconsistentResultDiffLimit, &analyteFlags);

			// at least 1 of 2 cuvettes result is OK so set not to suppress results on rotor, unless inconsistent result for 2 cuvette average result

			if ((analyteFlags & INCONSISTENT_RESULT) == FALSE)
			{
				commonAlgGlobals_g->rotorNoSuppressAnalyteResultList[commonAlgGlobals_g->rotorNoSuppressAnalyteResultListIdx++] = analyteType2CuvResult;
			}
		}
	}
	else if ( calc2CuvPtr->calc2CuvType == CALC_2_CUV_BLEND_ZONES_CUV_2 )	// case CALC_2_CUV_BLEND_ZONES_CUV_2: 2 cuvette blend algorithm, based on cuvette 2 result / flags, blend using zones since cuvette 2 and cuvette 1 can have large difference in results
	{
		if ( ( analyteFlags2 & calc2CuvPtr->blend1CuvOkCheckData.cuvSuppressionMask ) != NO_ANALYTE_ERROR )
		{
			// cuvette 2 error: report cuvette 2 results and error info and mark for cuvette 2

			analyteResult = analyteResult2;
			analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
		}
		else if ( analyteResult2 >= calc2CuvPtr->endCuvBlendLimit )
		{
			// cuvette 2 result >= end blend limit: report cuvette 2 results and flag info and mark for cuvette 2

			analyteResult = analyteResult2;
			analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
		}
		else if ( analyteResult2 <= calc2CuvPtr->startCuvBlendLimit )
		{
			// cuvette 2 result <= start blend limit: report cuvette 1 results and flag/error info and mark for cuvette 1, even if error in cuvette 1

			analyteResult = analyteResult1;
			analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED;
		}
		else	// cuvette 2 result > start blend limit and cuvette 2 result < end blend limit: cuvette 2 result is in blend range
		{
			if ( ( analyteFlags1 & calc2CuvPtr->blend1CuvOkCheckData.cuvSuppressionMask ) != NO_ANALYTE_ERROR )
			{
				// cuvette 2 result in blend range and cuvette 1 error: report cuvette 2 results and flag info and mark for cuvette 2

				analyteResult = analyteResult2;
				analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
			}
			else	// cuvette 2 result in blend range and cuvette 1 OK: process blend & zones in blend range / inconsistend result
			{
				// calc default proportional blend result
				
				startCuvBlendLimit = calc2CuvPtr->startCuvBlendLimit;
				endCuvBlendLimit   = calc2CuvPtr->endCuvBlendLimit;

				blendFactor = ( analyteResult2 - startCuvBlendLimit) / (endCuvBlendLimit - startCuvBlendLimit);

				analyteResult = (blendFactor * analyteResult2) + ((1 - blendFactor) * analyteResult1);
				analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED | analyteFlags2 | BLEND_CUVETTE_2_USED;

				// check if cuvette 2 is more than the blend range greater than cuvette 1

				if ( fabs( analyteResult2 - analyteResult1 ) > calc2CuvPtr->inconsistentResultDiffLimit )	// cuvette 2 result in blend range and (cuvette 2 result - cuvette 1 result) > blend range: process analyte 2 result by zone
				{
					if ( analyteResult2 <= calc2CuvPtr->blend1CuvOkCheckData.zone1Limit )	// cuvette 2 result <= zone 1 limit (near bottom) - set result / flags for cuvette 1
					{
						analyteResult = analyteResult1;
						analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED;
					} 
					else if ( analyteResult2 >= calc2CuvPtr->blend1CuvOkCheckData.zone2Limit )	// cuvette 2 result >= zone 2 limit (near top) - set result / flags for cuvette 2
					{
						analyteResult = analyteResult2;
						analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
					}
					else	// cuvette 2 result > zone 1 limit and cuvette 2 result < zone 2 limit - use calculated blend result / combined flags (already processed) & set as inconsistent result
					{
						analyteFlags |= INCONSISTENT_RESULT;
					} 
				}
			}
		}

		// process endogenous limits and system/dynamic range limits for any valid answers

		if ( analyteResult != NOVAL_NEG_FLOAT )
		{  
			// Make sure to update resultPrintFlags for any endogenous zone limit found for 2 cuvette analtyeType results (preserve above system range flag since part of possible rate in endpoint condition)

			EndogenousLimitsCheck(analyteType2CuvResult, factorsLimitsPtr, &analyteFlags);

			// Check combined 2 cuv result for within system & dynamic range limits, clear flags before check

			ResultLimitsCheck(analyteType2CuvResult, analyteResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &analyteFlags);
		}
	}
	else
	{		
		// process 2 cuvette algorithm for average or blend methods (assuming both cuvettes have non NOVAL_NEG_FLOAT results to do blend or average)
		// check to see if valid analyte result for either cuvette 

		if (analyteResult1 == NOVAL_NEG_FLOAT)
		{
			// 1st cuvette invalid analyte result

			analyteResult = analyteResult1;
			analyteFlags  = analyteFlags1;
		}
		else if (analyteResult2 == NOVAL_NEG_FLOAT)
		{
			// 2nd cuvette invalid analyte result

			analyteResult = analyteResult2;
			analyteFlags  = analyteFlags2;
		}
		else
		{
			// 1st & 2nd cuvettes both valid analyte results

			if (calc2CuvPtr->calc2CuvType == CALC_2_CUV_AVG)
			{
				// case CALC_2_CUV_AVG: calc average of 2 cuvette analyte results & combine flags for both cuvettes 

				analyteResult = (analyteResult1 + analyteResult2) / 2.0;
				analyteFlags  = analyteFlags1 | analyteFlags2;
			}
			else				  
			{
				// process blend of 2 cuvette analyte results and combine flags

				startCuvBlendLimit = calc2CuvPtr->startCuvBlendLimit;
				endCuvBlendLimit   = calc2CuvPtr->endCuvBlendLimit;

				if (calc2CuvPtr->calc2CuvType == CALC_2_CUV_BLEND)	// case CALC_2_CUV_BLEND: blend based on cuvette 1
				{
					if (analyteResult1 < startCuvBlendLimit)
					{
						// use good analyte result for 1st cuvette if 1st cuvette below blend range

						analyteResult = analyteResult1;
						analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED;
					}
					else if (analyteResult1 > endCuvBlendLimit)
					{
						// use good analyte result for 2nd cuvette if 1st cuvette above blend range

						analyteResult = analyteResult2;
						analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
					}
					else
					{
						// checked cuvette within blend range, compute blend factor & combined analyte result & flags

						blendFactor = (endCuvBlendLimit - analyteResult1) / (endCuvBlendLimit - startCuvBlendLimit);

						analyteResult = (blendFactor * analyteResult1) + ((1 - blendFactor) * analyteResult2);
						analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED | analyteFlags2 | BLEND_CUVETTE_2_USED;
					}
				}
				else	// case CALC_2_CUV_BLEND_CUV_2: blend based on cuvette 2
				{
					if (analyteResult2 < startCuvBlendLimit)
					{
						// use good analyte result for 1st cuvette if 2nd cuvette below blend range

						analyteResult = analyteResult1;
						analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED;
					}
					else if (analyteResult2 > endCuvBlendLimit)
					{
						// use good analyte result for 2nd cuvette if 2nd cuvette above blend range

						analyteResult = analyteResult2;
						analyteFlags  = analyteFlags2 | BLEND_CUVETTE_2_USED;
					}
					else
					{
						// checked cuvette within blend range, compute blend factor & combined analyte result & flags

						blendFactor = ( analyteResult2 - startCuvBlendLimit) / (endCuvBlendLimit - startCuvBlendLimit);

						analyteResult = (blendFactor * analyteResult2) + ((1 - blendFactor) * analyteResult1);
						analyteFlags  = analyteFlags1 | BLEND_CUVETTE_1_USED | analyteFlags2 | BLEND_CUVETTE_2_USED;
					}
				}
			}

			// check 2 cuvette inconsistent result limit, set endogenous resultPrintFlags, check 2 cuvette result system & dynamic range limits
			 
			Check2CuvLimitsFlags(analyteType2CuvResult, analyteResult1, analyteResult2, analyteResult, factorsLimitsPtr, calc2CuvPtr->inconsistentResultCheckEnabled, calc2CuvPtr->inconsistentResultDiffLimit, &analyteFlags);
		}
	}


	// IMV - rate paragraph 16

	Store2CuvAlgParamsImv( analyteType2CuvResult, analyteTypeCuv2, analyteFlags, analyteResult1, analyteResult2, fabs(analyteResult1 - analyteResult2), blendFactor, inconsistentResultDiffLimit, calc2CuvPtr );

	// end IMV


	// Store results in results data record	for analyteType set for final results

	StoreAnalyteResultsRecord(analyteType2CuvResult, analyteResult, analyteFlags, factorsLimitsPtr);
}


// ********************************************************************
// FUNCTION:
//
//		Process2CuvAlgorithm() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process 2 cuvette algorithm for combined result & flags 
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

void Process2CuvAlgorithm(unsigned char analyteTypeCuv1, unsigned char analyteTypeCuv2, unsigned char analyteType2CuvResult)
{
	unsigned long	analyteFlags1;
	unsigned long	analyteFlags2;
	float			analyteResult1;
	float			analyteResult2;

	Calc2CuvRecord_t 		*calc2CuvPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType2CuvResult, CALC_2_CUV_ALG);

	calc2CuvPtr = &calc2CuvConfig_g.calc2CuvRecord;
	
	analyteResult1 = rotorResults_g->rotorAnalyteResult[analyteTypeCuv1].analyteResult;
	analyteFlags1  = rotorResults_g->rotorAnalyteResult[analyteTypeCuv1].analyteFlags;
	analyteResult2 = rotorResults_g->rotorAnalyteResult[analyteTypeCuv2].analyteResult;
	analyteFlags2  = rotorResults_g->rotorAnalyteResult[analyteTypeCuv2].analyteFlags;

	// process the 2 analyteType cuvettes to calculate combined analyte result & flags in result record

	Calc2CuvAnalyteResult(analyteType2CuvResult, analyteTypeCuv2, analyteResult1, analyteFlags1, analyteResult2, analyteFlags2, calc2CuvPtr, factorsLimitsPtr);
}


