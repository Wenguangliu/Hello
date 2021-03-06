#ifndef __IMVDATA_H
#define __IMVDATA_H

#if defined(__cplusplus)
extern "C" {
#endif



// imv methods (methods used to store imv based on type)

typedef enum
{
		IMV_DIRECT_SET_METHOD,
		IMV_INCR_SET_METHOD,
		IMV_DECR_SET_METHOD,
		IMV_INCR_INIT_METHOD,
} ImvMethod_t;



// imv types (identifier type used to scan match to select storage index)

typedef enum
{
		// standard endpoint / rate ImvType_t

		IMV_PARAGRAPH					= 0,
		IMV_VERSION 					= 1,
		IMV_HEM_LIMIT					= 2,
		IMV_LIP_LIMIT					= 3,
		IMV_ICT_LIMIT					= 4,
		IMV_LOW_SYSTEM_LIMIT			= 5,
		IMV_LOW_DYNAMIC_RANGE			= 6,
		IMV_HIGH_DYNAMIC_RANGE			= 7,
		IMV_HIGH_SYSTEM_LIMIT			= 8,

		IMV_TEST_START_ABS_LIMIT		= 9,

		IMV_TEST_BEAD_MIX_LIMIT			= 10,
		IMV_BLANK_BEAD_MIX_LIMIT		= 11,

		IMV_TEST_START_ABS_LIMIT_B		= 64,		// for special case (2 wavelength endpoint, wavelength b)

		IMV_BARCODE_SLOPE_USED			= 23,		// slope used calculated from barcode parameters
		IMV_BARCODE_INTERCEPT_USED		= 24,		// intercept used calculated from barcode parameters
		IMV_BARCODE_A0_USED				= 2000,		// A0 used calculated from barcode parameters
		IMV_BARCODE_A1_USED				= 2001,		// A0 used calculated from barcode parameters
		IMV_BARCODE_A2_USED				= 2002,		// A0 used calculated from barcode parameters

		IMV_BARCODE_FACTOR_1			= 12,		// typically SLOPE
		IMV_BARCODE_FACTOR_2			= 13,		// typically INT
		IMV_BARCODE_FACTOR_3			= 14,
		IMV_BARCODE_FACTOR_4			= 15,
		IMV_BARCODE_FACTOR_5			= 16,
		IMV_BARCODE_FACTOR_6			= 17,
		IMV_BARCODE_FACTOR_7			= 18,
		IMV_BARCODE_FACTOR_8			= 19,

		IMV_FLATNESS_TOGGLE				= 20,
		IMV_FLATNESS_RATIO_LIMIT		= 21,
		IMV_FLATNESS_ABSOLUTE_LIMIT		= 22,

		IMV_BLANK_START_ABS_LIMIT		= 25,

		IMV_850_OPTICAL_TRANSMISSION	= 34,
		IMV_850_TEST_TRANSMISSION		= 35,
		IMV_850_TEST_BEAD_MIX			= 36,
		IMV_850_BLANK_TRANSMISSION		= 37,
		IMV_850_BLANK_BEAD_MIX			= 38,

		IMV_STARTING_ABS_A				= 39, 
		IMV_STARTING_ABS_B_BLANK		= 40, 

		IMV_WL_0_AVG_TEST				= 26,
		IMV_WL_0_AVG_BLANK				= 27,

		IMV_WL_0_BLANKED_AVG			= 41,
		IMV_WL_0_BLANKED_MIN			= 42,
		IMV_WL_0_BLANKED_MAX			= 43,
		IMV_WL_0_FLATNESS				= 44,
		IMV_WL_0_FLATNESS_AVG			= 45,

		IMV_WL_0_CORRECTED_AVG			= 46,

		IMV_WL_0_ENDG_CORRECTED_AVG		= 65,		// for special case endogenous correction

		IMV_WL_1_AVG_TEST				= 31,
		IMV_WL_1_AVG_BLANK				= 32,

		IMV_WL_1_BLANKED_AVG			= 47,
		IMV_WL_1_BLANKED_MIN			= 48,
		IMV_WL_1_BLANKED_MAX			= 49,
		IMV_WL_1_FLATNESS				= 50,
		IMV_WL_1_FLATNESS_AVG			= 51,

		IMV_WL_1_CORRECTED_AVG			= 52,

		IMV_WL_0_BLANKED_START_PASS		= 53,

		IMV_WL_0_RESULT					= 66,
		IMV_WL_1_RESULT					= 67,

		IMV_WL_1_BLANKED_START_PASS		= 56,

		IMV_WL_COMP_RESULT_RATIO		= 63,
		IMV_WL_RATIO_LIMIT				= 29,
		IMV_WL_RATIO					= 33,

		IMV_WL_0_OVER_RANGE_START_PASS	= 91,
		IMV_WL_0_OVER_RANGE_END_PASS	= 92,
		IMV_WL_0_OVER_RANGE_LIMIT		= 93,
		IMV_WL_0_OVER_RANGE_SLOPE		= 94,
		IMV_WL_1_OVER_RANGE_START_PASS	= 95,
		IMV_WL_1_OVER_RANGE_END_PASS	= 96,
		IMV_WL_1_OVER_RANGE_LIMIT		= 97,
		IMV_WL_1_OVER_RANGE_SLOPE		= 98,

		IMV_NOISE_TOGGLE				= 68,
		IMV_NOISE_RATIO_LIMIT			= 69,
		IMV_NOISE_ABSOLUTE_LIMIT		= 70,
		IMV_NOISE_RATIO_LIMIT_LOW		= 57,

		IMV_ARRH_FACTOR					= 71,

		IMV_WINDOW_ABS_THRESHOLD		= 72,		// for window select pass abs compare
		IMV_WINDOW_THRESHOLD_MULT		= 28,		// derived window selection threshold multiplier 
		IMV_PRE_END_ABS_LIMIT			= 55,		// for pre end abs limit
		IMV_END_ABS_THRESHOLD			= 73,		// for end abs limit
		IMV_RESULT_FALSE_RATE_MULT		= 30,		// false rate result correction
		IMV_RESULT_ENDOGENOUS_MULT		= 60,		// endogenous result correction

		IMV_HIGH_DYNAMIC_RANGE_ADJUST	= 74,

		IMV_STABILITY_RATE				= 75,

		IMV_FALSE_RATE_RATIO_LIMIT		= 76,
		IMV_FALSE_RATE_ABSOLUTE_LIMIT	= 77,

		IMV_AVG_SENSOR_TEMPERATURE		= 78,

		IMV_RATE_EXPONENT_VALUE			= 79,
		IMV_RATE_E_TO_THE_X				= 80,

		IMV_SLOPE_NUM_POINTS			= 81,
		IMV_SLOPE_RATE					= 82,		// uncorrected rate 
		IMV_SLOPE_STD_ERROR				= 83,
		IMV_SLOPE_FALSE_RATE			= 84,
		IMV_PRE_SLOPE_CORRECTION		= 58,		// slopeRate prior to slopeRateCorrection
		IMV_RATE_WINDOW_USED			= 59,		// rate alg window used
		
		IMV_TEMP_CORRECTED_RATE			= 46,		// bfd temperature corrected rate, same as IMV_WL_0_CORRECTED_AVG

		IMV_RATE_NOT_LINEAR_RATIO		= 61,		// stdError / fabs(slopeRate)

		IMV_CA_PRE_CORRECTION_RESULT	= 1500,
		IMV_CA_RESULT_CORRECTION		= 1501,

        IMV_RESULT_NA_UNCORRECTED       = 1502,     // NA_1 or NA_2 result prior to NA stability decay rate correction

		IMV_RESULT_NO_DECAY				= 85,
		
		IMV_ABSORBANCE_WINDOW_0			= 86,		// rate windowAbs index set 
		IMV_ABSORBANCE_WINDOW_1			= 87, 
		IMV_ABSORBANCE_WINDOW_2			= 88, 
		IMV_ABSORBANCE_WINDOW_3			= 89, 
		IMV_ABSORBANCE_WINDOW_4			= 90,
		IMV_ABSORBANCE_WINDOW_5			= 10091,	// expanded for LAC - up to 16 windows
		IMV_ABSORBANCE_WINDOW_6			= 10092,
		IMV_ABSORBANCE_WINDOW_7			= 10093,
		IMV_ABSORBANCE_WINDOW_8			= 10094,
		IMV_ABSORBANCE_WINDOW_9			= 10095,
		IMV_ABSORBANCE_WINDOW_10		= 10096,
		IMV_ABSORBANCE_WINDOW_11		= 10097,
		IMV_ABSORBANCE_WINDOW_12		= 10098,
		IMV_ABSORBANCE_WINDOW_13		= 10099,
		IMV_ABSORBANCE_WINDOW_14		= 10100,
		IMV_ABSORBANCE_WINDOW_15		= 10101,
		
		IMV_RATE_SLOPE_DELTA_START		= 54,
		
		IMV_ANALYTE_RESULT				= 99, 



		// analyte type specific ImvType_t

		IMV_PHOS_DELTA_7_TO_13			= 100,		// PHOS
		IMV_PHOS_ABS_FRACTION_LIMIT		= 101,
		IMV_PHOS_RATE					= 102,
		IMV_PHOS_INTERCEPT				= 103,
		IMV_PHOS_PASS_START_IN_MINUTES	= 104,


		IMV_T4_G6PDH_EXPT_RATE			= 150,		// T4
		IMV_T4_SLOPE					= 151,			
		IMV_T4_INTERCEPT				= 152,			
		IMV_T4_TOP						= 153,			
		IMV_T4_BOTTOM					= 154,
		IMV_T4_NOISE_LIMIT				= 155,			
		IMV_T4_G6_ARRH_FACTOR			= 156,
		IMV_T4_ARRH_FACTOR				= 157,
		IMV_T4_G6_MAX_DELTA				= 158,
		IMV_T4_HEM_COMBO_LIMIT			= 159,
		IMV_T4_LIP_COMBO_LIMIT			= 160,
		IMV_T4_FALSE_RATE_LIMIT			= 161,
		IMV_T4_BFD_ARRH_ADJUST			= 162,
		IMV_T4_G6_BFD_ARRH_ADJUST		= 163,
		IMV_T4_T4L1_RATE				= 164,
		IMV_T4_T4L1_RATE_CORR			= 165,
		IMV_T4_T4L1_STD_ERROR			= 166,
		IMV_T4_T4L1_FALSE_RATE			= 167,
		IMV_T4_G6P1_RATE				= 168,
		IMV_T4_G6P1_RATE_CORR			= 169,
		IMV_T4_G6P1_DEVIATION			= 170,
		IMV_T4_ABS_AVG_SEC_CUV_4		= 171,
		IMV_T4_ABS_AVG_TERT_CUV_4		= 172,
		IMV_T4_ABS_AVG_SEC_CUV_27		= 173,
		IMV_T4_ABS_AVG_TERT_CUV_27		= 174,
		IMV_T4_CUV_4_TOTAL				= 175,
		IMV_T4_CUV_27_TOTAL				= 176,
		IMV_T4_SAMPLE_CHECK				= 177,
		IMV_T4_SAMPLE_CHECK_LIMIT		= 178,
		IMV_T4_T4L1_NUMERATOR			= 179,
		IMV_T4_T4L1_DENOMINATOR			= 180,
		IMV_T4_T4L1_TRANSFORMED			= 181,


		IMV_CRE_RATE					= 200,		// CRE
		IMV_CRE_BLANKED_AVG				= 201,
		IMV_CRE_WET_TIME				= 202,


		IMV_CTBIL_HORSE_SLOPE			= 250,		// CTBIL
		IMV_CTBIL_HORSE_INT				= 251,
		IMV_CTBIL_OTHER_SLOPE			= 252,
		IMV_CTBIL_OTHER_INT				= 253,
		IMV_CTBIL_SB_467_ICT			= 254,


		IMV_TCO2_PRI_HI_START_ABS		= 300,		// TCO2
		IMV_TCO2_PRI_LO_START_ABS		= 301,
		IMV_TCO2_DELTA_SLOPE_MAX		= 302,
		IMV_TCO2_DELTA_SLOPE_MIN		= 303,
		IMV_TCO2_DELTA_SLOPE_2_MAX		= 304, 
		IMV_TCO2_DELTA_SLOPE_2_MIN		= 305,
		IMV_TCO2_START_INDEX			= 306,
		IMV_TCO2_STOP_INDEX				= 307,
		IMV_TCO2_NUM_POINTS				= 308,
		IMV_TCO2_AVG_DELTA_SLOPE_CHANGE = 309,
		IMV_TCO2_SUM_DELTA_SLOPE 		= 310,


		IMV_TRIG_RATE_TRIG_LIMIT		= 400,		// TRIG
		IMV_TRIG_RATE_LIP_LIMIT			= 401,


		IMV_HDL_PRIM_TIME_CORR_SLOPE	= 450,		// HDL_1, HDL_2
		IMV_HDL_PRIM_TIME_CORR_INT		= 451,
		IMV_HDL_SEC_TIME_CORR_SLOPE		= 452,
		IMV_HDL_SEC_TIME_CORR_INT		= 453,
		IMV_HDL_DERIVED_BLANKED_AVG		= 454,
		IMV_HDL_DERIVED_BLANKED_MIN		= 455,
		IMV_HDL_DERIVED_BLANKED_MAX		= 456,
		IMV_HDL_DERIVED_FLATNESS		= 457,
		IMV_HDL_DERIVED_FLATNESS_AVG	= 458,
		IMV_HDL_DERIVED_CORRECTED_AVG	= 459,
		IMV_HDL_PRIM_CORRECTED_AVG		= 460,
		IMV_HDL_SEC_CORRECTED_AVG		= 461,
		IMV_HDL_PRIM_TIME_CORR_AVG		= 462,
		IMV_HDL_SEC_TIME_CORR_AVG		= 463,
		IMV_HDL_DERIVED_TIME_CORR_AVG	= 464,
		IMV_HDL_RESULT_NO_DECAY			= 465,
		IMV_HDL_RESULT_DECAY			= 466,


		IMV_SB_CONST_LIP_340			= 500,		// SAMPLE_BLANK
		IMV_SB_CONST_HEM_340			= 501,
		IMV_SB_CONST_ICT_340			= 502,
		IMV_SB_CONST_LIP_405			= 503,
		IMV_SB_CONST_HEM_405			= 504,
		IMV_SB_CONST_ICT_405			= 505,
		IMV_SB_CONST_LIP_467			= 506,
		IMV_SB_CONST_HEM_467			= 507,
		IMV_SB_CONST_ICT_467			= 508,
		IMV_SB_CONST_LIP_SLOPE			= 509,
		IMV_SB_CONST_LIP_INT			= 510,
		IMV_SB_CONST_HEM_SLOPE			= 511,
		IMV_SB_CONST_HEM_INT			= 512,
		IMV_SB_CONST_ICT_SLOPE			= 513,
		IMV_SB_CONST_ICT_INT			= 514,
		IMV_SB_START_ABS_START			= 515,
		IMV_SB_ABS_AVG_START			= 516,
		IMV_SB_340_TURB					= 517,
		IMV_SB_405_HEME					= 518,
		IMV_SB_467_ICT					= 519,
		IMV_SB_HB_INDEX					= 520,
		IMV_SB_LIPEMIC_INDEX			= 521,
		IMV_SB_ICTERIC_INDEX			= 522,
		IMV_SB_MIN_TEMP_ALLOWED			= 523,
		IMV_SB_MAX_TEMP_ALLOWED			= 524,
		IMV_SB_TEMP_SWING_LIMIT			= 525,
		IMV_SB_FIRST_SAMP_TEMP			= 526,
		IMV_SB_MID_SAMP_TEMP			= 527,
		IMV_SB_LAST_SAMP_TEMP			= 528,
		IMV_SB_AVG_SAMP_TEMP			= 529,


		IMV_SYSCHK_SAMP_RATIO			= 600,		// SYSTEM_CHECK
		IMV_SYSCHK_SAMP_TO_CUV_LIMIT	= 601,
		IMV_SYSCHK_SAMPDIL_TOGGLE		= 602,
		IMV_SYSCHK_SD_RATIO_LO_LIM		= 603,
		IMV_SYSCHK_SD_RATIO_HI_LIM		= 604,
		IMV_SYSCHK_SD_DELTA_LO_LIM		= 605,
		IMV_SYSCHK_SD_DELTA_HI_LIM		= 606,
		IMV_SYSCHK_PLASTIC_ABS_6		= 607,
		IMV_SYSCHK_PLASTIC_ABS_11		= 608,
		IMV_SYSCHK_PLASTIC_ABS_28		= 609,
		IMV_SYSCHK_AIR_SENSOR_OFFSET	= 610,
		IMV_SYSCHK_TRANSMISSION_START	= 611,
		IMV_SYSCHK_ABSORBANCE_START		= 612,
		IMV_SYSCHK_DIL_ABS				= 613,
		IMV_SYSCHK_SAMP11				= 614,
		IMV_SYSCHK_SAMP28				= 615,
		IMV_SYSCHK_INSUF_SAMP_LIMIT		= 616,
		IMV_SYSCHK_INSUF_SAMP_CUV_1_ABS	= 617,
		IMV_SYSCHK_WB_LIMIT				= 618,
		IMV_SYSCHK_WB_CUV_1_TRANS		= 619,

		IMV_IQC_ABS_LIMIT				= 700,		// IQC
		IMV_IQC_PRECISION_LIMIT			= 701,
		IMV_IQC_RATIO_SIGMA				= 702,
		IMV_IQC_RATIO_LIMIT				= 703,
		IMV_IQC_LEVEL1_MIN				= 704,
		IMV_IQC_LEVEL1_MAX				= 705,
		IMV_IQC_LEVEL1_RNG				= 706,
		IMV_IQC_LEVEL2_MIN				= 707,
		IMV_IQC_LEVEL2_MAX				= 708,
		IMV_IQC_LEVEL2_RNG				= 709,
		IMV_IQC_PRECISION_MIN			= 710,
		IMV_IQC_PRECISION_MAX			= 711,
		IMV_IQC_PRECISION_RNG			= 712,
		IMV_IQC_LEVEL_1_START			= 713,
		IMV_IQC_ABS_A_START				= 714,
		IMV_IQC_ABS_B_START				= 715,
		IMV_IQC_RATIO_START				= 716,
		IMV_IQC_LEVEL_2_START			= 717,
		IMV_IQC_RATIO_AVG				= 718,
		IMV_IQC_RATIO_STD				= 719,
		IMV_IQC_RATIO_PRECISION			= 720,


		IMV_RQC_RATIO_SUPPRESS_LIMIT	= 800,		// RQC
		IMV_RQC_HIGH_SUPPRESS_LIMIT		= 801,
		IMV_RQC_AVG_ABS					= 802,
		IMV_ROTOR_AGE_IN_DAYS			= 803,
		IMV_RQC_QC_TIME					= 804,
		IMV_RQC_EXPONENT_VALUE			= 805,
		IMV_RQC_E_TO_THE_X				= 806,
		IMV_RQC_EXPECTED_QC				= 807,
		IMV_RQC_ABS_START_PASS			= 808,
		IMV_RQC_LOW_LIMIT				= 809,
		IMV_RQC_HIGH_LIMIT				= 810,
		IMV_RQC_ABUSED_LIMIT			= 811,
		IMV_RQC_ABUSED					= 812,
		IMV_RQC_RESULT					= 813,


		IMV_2_CUV_TYPE					= 900,		// 2 cuvette generic algorithm
		IMV_2_CUV_DIFF_LIMIT_CONST		= 901,
		IMV_2_CUV_DIFF_CHK_MASK_MSB		= 902,
		IMV_2_CUV_DIFF_CHK_MASK_LSB		= 917,
		IMV_2_CUV_DIFF_CHK_TOGGLE		= 903,
		IMV_2_CUV_DIFF_CHK_MAX			= 904,
		IMV_2_CUV_DIFF_LIMIT_SCALE		= 905,
		IMV_2_CUV_DIFF_LIMIT_USED		= 906,
		IMV_2_CUV_BLEND_MASK_MSB		= 907,
		IMV_2_CUV_BLEND_MASK_LSB		= 918,
		IMV_2_CUV_BLEND_ZONE_1			= 908,
		IMV_2_CUV_BLEND_ZONE_2			= 909,
		IMV_2_CUV_BLEND_START_LIMIT		= 910,
		IMV_2_CUV_BLEND_END_LIMIT		= 911,
		IMV_2_CUV_BLEND					= 912,
		IMV_2_CUV_BLEND_RANGE			= 913,
		IMV_2_CUV_RESULT_1				= 914,
		IMV_2_CUV_RESULT_2				= 915,
		IMV_2_CUV_RESULT_DIFF			= 916,


		IMV_POS_NEG_LO_RATIO_LIMIT		= 1000,		// pos neg generic algorithm
		IMV_POS_NEG_HI_RATIO_LIMIT		= 1001,
		IMV_POS_NEG_RATE_LIMIT			= 1002,
		IMV_POS_NEG_TEST_RATIO_0		= 1003,
		IMV_POS_NEG_OPT_RATIO_0			= 1004,
		IMV_POS_NEG_NET_RATIO_0			= 1005,
		IMV_POS_NEG_WL_0_RATE			= 1006,
		IMV_POS_NEG_WL_1_RATE			= 1007,
		IMV_POS_NEG_START_ABS_HI_LIMIT	= 1008,
		IMV_POS_NEG_START_ABS_LO_LIMIT	= 1009,
		IMV_POS_NEG_START_ABS_START		= 1010,
		IMV_POS_NEG_TEST_RATIO_1		= 1011,
		IMV_POS_NEG_OPT_RATIO_1			= 1012,
		IMV_POS_NEG_NET_RATIO_1			= 1013,
		IMV_POS_NEG_NET_RATIO_SELECTED	= 1014,
		IMV_POS_NEG_FINAL_RATIO_RESULT	= 1015,


		IMV_ICT_BLANK_CORRECTION		= 1100,		// ICT correction
		IMV_ICT_TEST_CORRECTION			= 1101,
		IMV_ICT_BLANK_FACTOR			= 1102,
		IMV_ICT_TEST_FACTOR_2			= 1103,
		IMV_ICT_TEST_FACTOR_1			= 1104,


		IMV_DUAL_WL_HI_DELTA_LIMIT		= 1200,		// ENDPOINT dual wavelength params & limits
		IMV_DUAL_WL_LO_DELTA_LIMIT		= 1201,
		IMV_DUAL_WL_0_RESULT_WEIGHT		= 1202,
		IMV_DUAL_WL_1_RESULT_WEIGHT		= 1203,


		IMV_FALSE_RATE_CORR_TOGGLE		= 1300,		// RATE false rate, rate correction params & limits
		IMV_FALSE_RATE_CORR_FACTOR		= 1301,
		IMV_FALSE_RATE_CORRECTION		= 1302,
		IMV_FALSE_RATE_CORR_RATE		= 1303,


		IMV_NACL_NA_CONF_LIMIT			= 1400,		// NACL algorithm - NA confidence limit


		IMV_HCT_HB_LOW_LIMIT			= 1600,		// HCT algorithm - range & divisor constants
		IMV_HCT_HB_HIGH_LIMIT			= 1601,
		IMV_HCT_HB_RESULT_DIVISOR		= 1602,


		IMV_RATE_CORR_NOISE_MULT		= 1700,		// RATE slopeRate correction params & limits
		IMV_RATE_CORR_RATE_LIMIT		= 1701,
		IMV_RATE_CORR_ABS_RATE			= 1702,
		IMV_RATE_CORR_RATE_MULT			= 1703,
		IMV_RATE_CORR_CORRECTION		= 1704,


		IMV_TURB_BLANKED_ABS_START		= 1800,		// Turbidity Correction specific params & limits
		IMV_TURB_BLANKED_ABS_END		= 1801,
		IMV_TURB_CORR_LIMIT				= 1802,
		IMV_TURB_AVG_START_PASS			= 1803,
		IMV_TURB_AVG_END_PASS			= 1804,
										
		IMV_SIGMOID_LOGIT_LOG4_SLOPE		= 1900,
		IMV_SIGMOID_LOGIT_LOG4_INTERCEPT	= 1901,
		IMV_SIGMOID_LOGIT_LOG4_TOP			= 1902,
		IMV_SIGMOID_LOGIT_LOG4_BOTTOM		= 1903,
		IMV_SIGMOID_LOGIT_LOG4_NUMERATOR	= 1904,
		IMV_SIGMOID_LOGIT_LOG4_DENOMINATOR	= 1905,
		IMV_SIGMOID_LOGIT_LOG4_TRANSFORM	= 1906,

		
		// table filler ImvType_t 

		IMV_NOT_ENABLED					= 0x80000000,	// not used so do not define as type in function call or table
		IMV_NO_IMV						= 0xffffffff,
} ImvType_t;



#if defined(__cplusplus)
}
#endif

#endif	// __IMVDATA_H
