#ifndef __ALGGLOBALS_H
#define __ALGGLOBALS_H

#include <stdio.h>	// for terminal window debugging - to be removed later

#include "CommonDefines.h"
#include "SystemErrorNums.h"

#define	TRUE						1
#define	FALSE						0

#define	NOVAL_NEG_FLOAT				-999999.0
#define	NOVAL_POS_FLOAT				999999.0

#define NO_ANALYTE_CALC				255

#define MAX_BFD_CALS				32

#define	ANALYTE_BLANK_CHK_ON		TRUE
#define	ANALYTE_BLANK_CHK_OFF		FALSE

#define	BLANK_BEAD_MIX_CHK			TRUE
#define	ANALYTE_BEAD_MIX_CHK		FALSE

#define	SB_AVG_ABS_PAIRS			3

// min & max for range of possible actual trim settings for output to DAC wavelength channel

#define	MIN_DAC_TRIM				0x10
#define	MAX_DAC_TRIM				0xFF


// wavelength indices to raw sample ADC readings for NUMBER_WAVELENGTHS

#define	WL_340			0
#define	WL_405			1
#define	WL_467			2
#define	WL_500			3
#define	WL_515			4
#define	WL_550			5
#define	WL_600			6
#define	WL_630			7
#define	WL_850			8
#define	WL_WHITE		9

#define WL_NULL			0x0f

#define MAX_WP_850		10

// value used to detect a missing flash since 'R' messages set data all default to 0xffffffff

#define MISSING_FLASH_TIME				0xffffffff
#define MAX_MISSING_FLASHES_ALLOWED		1

// wavelength pairs for bead mix check transmission check wavelength pair absorbance calcs

#define WP_850_NULL		(WL_850*16)+WL_NULL	// 0x8f: use 850 nm average only for bead mix check transmission	


#define WP_340_405		(WL_340*16)+WL_405		// 0x01: 340/405 wavelengthPair transmission/absorbance
#define WP_340_850		(WL_340*16)+WL_850		// 0x08: 340/850 wavelengthPair transmission/absorbance
#define WP_405_467		(WL_405*16)+WL_467		// 0x12: 405/467 wavelengthPair transmission/absorbance
#define WP_405_500		(WL_405*16)+WL_500		// 0x13: 405/500 wavelengthPair transmission/absorbance
#define WP_405_850		(WL_405*16)+WL_850		// 0x18: 405/850 wavelengthPair transmission/absorbance
#define WP_467_405		(WL_467*16)+WL_405		// 0x21: 467/405 wavelengthPair transmission/absorbance
#define WP_467_550		(WL_467*16)+WL_550		// 0x25: 467/550 wavelengthPair transmission/absorbance
#define WP_467_630		(WL_467*16)+WL_630		// 0x27: 467/630 wavelengthPair transmission/absorbance
#define WP_467_850		(WL_467*16)+WL_850		// 0x28: 467/850 wavelengthPair transmission/absorbance
#define WP_500_600		(WL_500*16)+WL_600		// 0x36: 500/600 wavelengthPair transmission/absorbance
#define WP_500_630		(WL_500*16)+WL_630		// 0x37: 500/630 wavelengthPair transmission/absorbance
#define WP_500_850		(WL_500*16)+WL_850		// 0x38: 500/850 wavelengthPair transmission/absorbance
#define WP_515_600		(WL_515*16)+WL_600		// 0x46: 515/600 wavelengthPair transmission/absorbance
#define WP_515_850		(WL_515*16)+WL_850		// 0x48: 515/850 wavelengthPair transmission/absorbance
#define WP_550_600		(WL_550*16)+WL_600		// 0x56: 550/600 wavelengthPair transmission/absorbance
#define WP_550_630		(WL_550*16)+WL_630		// 0x57: 550/630 wavelengthPair transmission/absorbance
#define WP_550_850		(WL_550*16)+WL_850		// 0x58: 550/830 wavelengthPair transmission/absorbance
#define WP_600_550		(WL_600*16)+WL_550		// 0x65: 600/550 wavelengthPair transmission/absorbance
#define WP_600_630		(WL_600*16)+WL_630		// 0x67: 600/630 wavelengthPair transmission/absorbance
#define WP_600_850		(WL_600*16)+WL_850		// 0x68: 600/850 wavelengthPair transmission/absorbance
#define WP_630_405		(WL_630*16)+WL_405		// 0x71: 630/405 wavelengthPair transmission/absorbance
#define WP_630_500		(WL_630*16)+WL_500		// 0x73: 630/500 wavelengthPair transmission/absorbance
#define WP_630_850		(WL_630*16)+WL_850		// 0x78: 630/850 wavelengthPair transmission/absorbance
#define WP_850_850		(WL_850*16)+WL_850		// 0x88: 630/850 wavelengthPair transmission/absorbance

#define WP_WHITE_850	(WL_WHITE*16)+WL_850	// 0x98: 630/850 wavelengthPair transmission/absorbance


// analyte algorithm error flags

typedef enum
{
	FLASH_RANGE_ERROR			  =	0x80000000,	// abort algorithm, flash range exceeded
	DIVIDE_BY_ZERO				  =	0x40000000,	// abort algorithm, div by 0 in calcs 
	TEST_BEAD_MIX_ERROR			  =	0x20000000,	// improper mixing of sample bead
	BLANK_BEAD_MIX_ERROR		  =	0x10000000,	// improper mixing sample blank
	HEMOLYZED_LIMIT_ERROR		  =	0x08000000,	// sample hemolyzed 
	LIPEMIC_LIMIT_ERROR			  =	0x04000000,	// sample lipemic 
	ICTERIC_LIMIT_ERROR			  =	0x02000000,	// sample icteric 
	START_ABS_BAD				  =	0x01000000,	// starting abs too high 
	END_ABS_BAD					  =	0x00800000,	// ending abs too high	
	RATE_IN_ENDPOINT			  =	0x00400000,	// endpoint shows rate	
	FALSE_RATE					  =	0x00200000,	// illegal rate 
	RATE_NOT_LINEAR				  =	0x00100000,	// non-linear rate	
	ABOVE_SYSTEM_RANGE			  =	0x00080000,	// suppress, result	above system range
	BELOW_SYSTEM_RANGE			  =	0x00040000,	// suppress, result	below system range
	HIGH_DYNAMIC_RANGE			  =	0x00020000,	// notification, out of	dynamic	range 
	LOW_DYNAMIC_RANGE			  =	0x00010000,	// notification, out of	dynamic	range 
	ILLEGAL_LOG_PARAM			  =	0x00008000,	// abort algorithm, illegal log param encountered 
	INCONSISTENT_RESULT			  =	0x00004000,	// inconsistent	result between cuvettes	
	RQC_ERROR					  =	0x00002000,	// RQC value suppression for this method 
	RATE_TOO_LOW				  =	0x00001000,	// Rate is too low to report a value 
	CHEM_BEAD_MISSING			  =	0x00000800,	// BEAD_MISSING_2 3A error in specific sample blank 
	CHEM_DIST_ERROR				  =	0x00000400,	// DIST_CHECK_FAILED 3B error in specific sample blank 
	DILUTION_VERIFICATION_ERROR	  = 0x00000200,	// rotor dilution verification error - set for all analytes if rotor dilution check error

	// flash min & max flags are used in conjunction with error numbers that abort rotor and are for internal algorithm flagging only

	FLASH_BELOW_MIN_INTENSITY	  =	0x00000200,	// flash at optical cuvette 29, index 0 (burnt flash), WL_850 reading < minimum flash intensity limit error
	FLASH_ABOVE_MAX_SATURATION	  =	0x00000100, // flash at optical cuvette 29, index 0 (burnt flash), WL_850 reading < minimum flash intensity limit error

	// analyte algorithm method specific flags

	RATE_STARTING_ABS_80		  =	0x00000080 | START_ABS_BAD,		// rate starting abs error mask	(usually for second cuvette checked, first wavelength pair checked)
	RATE_STARTING_ABS_40		  =	0x00000040 | START_ABS_BAD,		// rate starting abs error mask	(usually for first cuvette checked, first wavelength pair checked)
	RATE_STARTING_ABS_20		  =	0x00000020 | START_ABS_BAD,		// rate starting abs error mask	(usually for second cuvette checked, second wavelength pair checked)
	RATE_STARTING_ABS_10		  =	0x00000010 | START_ABS_BAD,		// rate starting abs error mask	(usually for first wavelength pair checked/first cuvette)
	RATE_STARTING_ABS_08		  =	0x00000008 | START_ABS_BAD,		// rate starting abs error mask (usually for second wavelength pair checked)

	RATE_ENDING_ABS_02			  =	0x00000002 | END_ABS_BAD,		// rate ending abs error mask (usually for second cuvette checked)
	RATE_ENDING_ABS_01			  =	0x00000001 | END_ABS_BAD,		// rate ending abs error mask (usually for first cuvette checked)

	RATE_NOT_LINEAR_08			  =	0x00000008 | RATE_NOT_LINEAR,	// rate ending abs error mask (usually for second cuvette checked)
	RATE_NOT_LINEAR_04			  =	0x00000004 | RATE_NOT_LINEAR,	// rate ending abs error 0mask (usually for first cuvette checked)

	RATE_WINDOW_1				  =	0x00000000,						// rate window for most points in slope
	RATE_WINDOW_2				  =	0x00000020,						// rate window for next most points
	RATE_WINDOW_3				  =	0x00000040,						// rate window for next most points
	RATE_WINDOW_4				  =	0x00000060,						// rate window for next most points
	RATE_WINDOW_5				  =	0x00000080,						// rate window for least points in slope

	RATE_UNEXPECTED_RESULT		  =	0x00000004,						// rate number of points < 3, unexpected rate value	

	ENDPOINT_LOW_ABS_ERROR		  =	0x00000080,						// endpoint low absorbance error flag
	WAVELENGTH_COMPARE_ERROR	  =	0x00000040,						// endpoint error if 2 wavelengths calculated result out of limits

	RATE_IN_ENDPOINT_20			  =	0x00000020 | RATE_IN_ENDPOINT,	// rate in endpoint 2 wavelengths flag 20
	RATE_IN_ENDPOINT_10			  =	0x00000010 | RATE_IN_ENDPOINT,	// rate in endpoint 2 wavelengths flag 10
	RATE_IN_ENDPOINT_08			  =	0x00000008 | RATE_IN_ENDPOINT,	// rate in endpoint 2 wavelengths flag 08

	ALB_DOUBLE_BEAD				  = 0x00000004,						// set in flags for ALB_BCG	double bead detect from special starting abs check
	ALB_BCG_METHOD				  =	0x00000001,						// set in flags for ALB_BCG algorithm type
	ALB_BCP_A_METHOD			  =	0x00000002,						// set in flags for ALB_BCP_A algorithm type

	BLEND_CUVETTE_1_USED		  =	0x00000001,						// set in flags for 2 cuvette blend algorithm for cuvette 1 used
	BLEND_CUVETTE_2_USED		  =	0x00000002,						// set in flags for 2 cuvette blend algorithm for cuvette 2 used

	CALC_2_CUVETTE_1_USED		  =	0x00000001,						// set in flags for 2 cuvette algorithm for cuvette 1 used
	CALC_2_CUVETTE_2_USED		  =	0x00000002,						// set in flags for 2 cuvette algorithm for cuvette 2 used

	TRIG_RATE_IN_ENDPOINT		  = 0x00000002,						// Flag for rate in endpoint error 			
	TRIG_BLANK_START_ABS_BAD	  = 0x00000001,						// Flag for seperating blank from test start abs error 			

	POS_NEG_RATIO_ERROR			  =	0x00000080,						// positive / negative std alg ratio error
	POS_NEG_RATE_TRENDING_ERROR	  =	0x00000040,						// positive / negative std alg rate	trending error
	POS_NEG_WB					  = 0x00000002,						// positive / negative std Whole Blood (WB) flag if set
	POS_NEG_RESULT_POSITIVE		  =	0x00000001,						// positive / negative std alg result positive flag / else negative result

	CHW_NOT_CANINE_OR_CONTROL	  =	0x00000020,						// CHW is suppressed if not canine species or not a control sample

	NACL_NA_LOW_RECOVERY		  = RATE_IN_ENDPOINT,				// NACL algorithm suppress NA for low recovery [ share RATE_IN_ENDPONT flag ]

    SIGMOID_LOGIT_LOG4_RATE_HIGH  = 0x00000002,                     // sigmoid logit/log4 Rate too high
    SIGMOID_LOGIT_LOG4_RATE_LOW   = 0x00000001,                     // sigmoid logit/log4 Rate too low

	NO_ANALYTE_ERROR			  =	0
} AnalyteFlags_t;


#define	CALC_ERROR_MASK					(FLASH_RANGE_ERROR | DIVIDE_BY_ZERO | ILLEGAL_LOG_PARAM | FLASH_BELOW_MIN_INTENSITY | FLASH_ABOVE_MAX_SATURATION)
#define ANALYTE_RESULT_LIMITS_ERR_MASK	(ABOVE_SYSTEM_RANGE | BELOW_SYSTEM_RANGE | HIGH_DYNAMIC_RANGE | LOW_DYNAMIC_RANGE)

#define	TRIG_CALC_ERROR_MASK			(DIVIDE_BY_ZERO | ILLEGAL_LOG_PARAM) // Only aborts for DIVIDE_BY_ZERO & ILLEGAL_LOG_PARAM


#define	GT_WINDOW_THRESHOLD				TRUE
#define LT_WINDOW_THRESHOLD				FALSE


// system error flags

typedef enum
{
    SAMPLE_WHOLE_BLOOD             = 0x80000000, // sample is whole blood type, NOT serum   
	RQC_TOO_HIGH				   = 0x40000000, // RQC value too high, test bad	
    LOW_IDLE_FAN_SET               = 0x10000000, // set low fan speed when system is idle
	USED_OTHER_FACTORS			   = 0x08000000, // used other barcode factors for UNSPECIFIED patient type
	USED_HORSE_FACTORS			   = 0x06000000, // used horse barcode factors
	USED_CAT_FACTORS			   = 0x04000000, // used cat   barcode factors 
	USED_DOG_FACTORS			   = 0x02000000, // used dog   barcode factors
	SYS_BAD_LOG_PARAM			   = 0x01000000, // bad param passed to log10() 
	LAMP_NOT_FLASHING			   = 0x00080000, // lamp intensity error	
	WAVELENGTH_SATURATED		   = 0x00010000, // wavelength saturated past limit
	SYS_FLASH_RANGE				   = 0x00008000, // system flash range error	
	SYS_DIV_BY_0				   = 0x00004000, // system divide by 0 encountered 
	TEMP_DRIFT_ERROR			   = 0x00002000, // temperature drifted too much during rotor run
	TEMP_OUT_OF_RANGE			   = 0x00001000, // temperature out of range during rotor run
	HIGH_SB_START_ABS			   = 0x00000800, // sample blank bead abs too high 
	SB_BEAD_NOT_MIXED			   = 0x00000400, // sample blank bead not mixed 
	DIST_CHECK_FAILED			   = 0x00000200, // distribution check failed at dist check time
	BEAD_MISSING_2				   = 0x00000080, // bead missing detected at check 2 time	
	CUV_FILL_ERROR				   = 0x00000020, // cuv 28 didn't fill 
	SAMP_DIL_MIX_ERROR			   = 0x00000010, // sample/diluent mix error	
	DIL_ABS_TOO_HIGH			   = 0x00000008, // diluent abs too high	
	INSUFFICIENT_DIL			   = 0x00000004, // not enough diluent, at bead check 2 time
	INSUFFICIENT_SAMP_1			   = 0x00000001, // not enough sample,  at bead check 1 time

	NO_SYSTEM_FLAG				   = 0
} RotorSystemFlags_t;


// rotor ROC file algorithmFlags indices definitions

#define ROTOR_ROC_SYSTEM_CHECK_T4_FLAG			0

#define ROTOR_ROC_WEIGHTED_ENDPOINT_50_50_FLAG	0

#define ROTOR_ROC_AVG_START_PASS_FLAG			0
#define ROTOR_ROC_AVG_END_PASS_FLAG				1
#define ROTOR_ROC_SLOPE_INT_METHOD_FLAG			2
#define ROTOR_ROC_RATE_THRESHOLD_FLAG			3
#define ROTOR_ROC_ENDPOINT_RATE_CORRECT_FLAG	4

#define ROTOR_ROC_NA_DECAY_CORRECT_LEVEL_FLAG   0

// cuvette numbers

#define OPTICAL_CUVETTE					29			// optical cuvette number		


// cuvette transmission calc types

#define BEAD1_READINGS					0
#define BEAD2_READINGS					1
#define DISTRIBUTION_READINGS			2
#define PRE_SAMPLE_FRS					3
#define POST_SAMPLE_FRS					4


// common limits/toggles

#define FLASH_RANGE_ERROR_TOGGLE		0.1
// DLR - update for 16 bit ADC?
//#define FLASH_RANGE_MAX					0.08
#define FLASH_RANGE_MAX					0.20

#define	MINIMUM_LAMP_INTENSITY			4000
#define LAMP_SATURATION_THRESHOLD		0xffe0
#define	DAYS_PER_MONTH					30.6

#define	DERIVED_CALC_ALG				0
#define	ENDPOINT						1
#define	RATE							2
#define T4_CALC_ALG						3
#define CALC_2_CUV_ALG					4
#define	POS_NEG_ALG						5

#define ANALYTE_MINUS_BLANK				0
#define BLANK_MINUS_ANALYTE				1
#define NO_BLANK_CORRECTION				0xff

#define	UNINITIALIZED_FILTER_CAL		0xffff

#define VLDL_TRIG_RESULT_DIVISOR 		5.0
#define HCT_HB_RESULT_DIVISOR           0.34

#define NO_PRINT_ORDER_FLAG				255

#define SCAN_FOR_ANALYTE_SUPPRESSED		0
#define SCAN_BEAD_DISTRIBUTION_COUNTS	1
#define SCAN_SHORT_LONG_SAMPLE_PROBS	2
#define SCAN_SET_DILUTION_CHECK_FLAGS	3


// common typedefs

typedef struct
{
	float	flashTime[MAX_PASSES];
	float	blankedAbs[MAX_PASSES];
	float	min;
	float	max;
	float	avg;
	float	avgAnalyte;
	float	avgAnalyteBlank;
} AvgAbs_t;


typedef struct
{
	unsigned char	startPass;
	unsigned char	numPoints;
	float			slope;
	float			stdError;
	float			falseRate;
} SlopeParams_t;


typedef struct
{
	float				flashTime[MAX_PASSES];
	float				rateStdErrorAbs[MAX_PASSES];
	float				falseRateAbs[MAX_PASSES];
} SlopeAbsData_t;


typedef struct
{
	unsigned char		cuvetteNumber;
	float				beadDistCheckLimit;
} BeadDistCheck_t;

#define BEAD_DIST_CHK_DONE		255


typedef struct
{
	unsigned			dilutionCheckEnabled;
	unsigned			shortSampleEquationSelect;
	float				shortSampleMean;
	float				shortSampleSigma;
	unsigned			longSampleEquationSelect;
	float				longSampleMean;
	float				longSampleSigma;
} DilutionCheckFactors_t;

#define	GAUSS_EQU				0
#define	EXP_EQU					1


#define	MAX_ROTOR_ALGORITHMS	32

typedef struct
{
	unsigned					patientType;
// GLD
// I added these three fields for eGFR.
	int							age;
	Race_t						race;
	Gender_t					gender;

	unsigned					useMaskLowDynamicSpecial;								// DLR - generic but now for CRP,  no '*' if < low  dynamic limit
	unsigned					useMaskHighDynamicSpecial;								// DLR - generic but now for eGFR, no '*' if > high dynamic limit

	unsigned					controlType;

	unsigned long				calcErrorMask;

	unsigned 					derivedResult;											// DLR - for resultPrintFlags derived calc algorithm

	unsigned 					posNegResult;											// DLR - for resultPrintFlags positive / negative result calc algorithm

	unsigned					numAnalytesBelowDynamicRange;							// DLR - track analyte results below dynamic range (printed & not derived)

	unsigned					rotorNoSuppressAnalyteResultListIdx;					// DLR - index to analyteType for no suppression of result in resultPrintFlags
	unsigned char				rotorNoSuppressAnalyteResultList[MAX_ROTOR_ALGORITHMS];	// DLR - list on rotor for analyteType to flag not to suppress result in resultPrintFlags

	DilutionCheckFactors_t		dilutionCheckFactors[MAX_ANALYTE_TYPES];
} CommonAlgGlobals_t;


#endif	// __ALGGLOBALS_H
