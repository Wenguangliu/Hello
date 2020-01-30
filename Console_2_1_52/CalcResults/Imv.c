// ********************************************************************
//
// FILENAME:  
// 
// 		Imv.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Intermediate Values implementation
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"


// intermediate values location definition by analyteType

const ImvType_t	imvTypeTable_m[MAX_ANALYTE_TYPES][MAX_INTERMEDIATE_VALUES]= 
{
	{									// idx - analyte type: 	0	SYSTEM_CHECK	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_BARCODE_FACTOR_1,			// 11 - Barcode Diluent Absorbance (Cuv 6)
		IMV_SYSCHK_SAMP_RATIO,			// 12 -	Ratio 340/850 absorbance from cuvette 28 to 11
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_SYSCHK_INSUF_SAMP_LIMIT,	// 15 - Insufficient Sample check cuvette1absorbance limit
		IMV_SYSCHK_WB_LIMIT,			// 16 - Whole Blood sample check cuvette1transmission limit
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_SYSCHK_SAMP_TO_CUV_LIMIT,	// 20 -	Sample To Cuv 28 Limit
		IMV_SYSCHK_SAMPDIL_TOGGLE,		// 21 -	Sample Check Ratio/Difference Limit
		IMV_SYSCHK_SD_RATIO_LO_LIM,		// 22 -	Sample Check Low Ratio Limit
		IMV_SYSCHK_SD_RATIO_HI_LIM,		// 23 -	Sample Check High Ratio Limit
		IMV_SYSCHK_SD_DELTA_LO_LIM,		// 24 -	Sample Check Low Difference Limit
		IMV_SYSCHK_SD_DELTA_HI_LIM,		// 25 -	Sample Check High Difference Limit
		IMV_SYSCHK_PLASTIC_ABS_6,		// 26 -	Cuv 6 Plastic Absorbance
		IMV_SYSCHK_PLASTIC_ABS_11,		// 27 -	Cuv 11 Plastic Absorbance
		IMV_SYSCHK_PLASTIC_ABS_28,		// 28 -	Cuv 28 Plastic Absorbance
		IMV_NO_IMV,						// 29  	                                              [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 30  	                                              [NGA has no path length correction IMV required]
		IMV_SYSCHK_AIR_SENSOR_OFFSET,	// 31 -	Air Sensor Offset
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_SYSCHK_TRANSMISSION_START,	// 34 -	Average Cuv 6 340/850 Transmittance
		IMV_NO_IMV,						// 35 - Average Cuv 6 405/850 Transmittance           [next transmission]
		IMV_NO_IMV,						// 36 - Average Cuv 6 467/850 Transmittance           [next transmission]
		IMV_NO_IMV,						// 37 - Average Opt Cuv 340/850 Transmittance         [next transmission]
		IMV_NO_IMV,						// 38 - Average Opt Cuv 405/850 Transmittance         [next transmission]
		IMV_NO_IMV,						// 39 - Average Opt Cuv 467/850 Transmittance         [next transmission] 
		IMV_NO_IMV,						// 40 - Average Cuv 11 340/850 Transmittance          [next transmission]
		IMV_NO_IMV,						// 41 - Average Cuv 28 340/850 Transmittance          [next transmission]
		IMV_NO_IMV,						// 42 - Ratio Cuv 6 To Opt Cuv 340/850 Transmittance  [next transmission]
		IMV_NO_IMV,						// 43 - Ratio Cuv 6 To Opt Cuv 405/850 Transmittance  [next transmission]
		IMV_NO_IMV,						// 44 - Ratio Cuv 6 To Opt Cuv 467/850 Transmittance  [next transmission]
		IMV_NO_IMV,						// 45 - Ratio Cuv 11 To Opt Cuv 340/850 Transmittance [next transmission]
		IMV_NO_IMV,						// 46 - Ratio Cuv 28 To Opt Cuv 340/850 Transmittance [next transmission]
		IMV_SYSCHK_ABSORBANCE_START,	// 47 -	Cuv 6 340/850 Absorbance
		IMV_NO_IMV,						// 48 - Cuv 6 405/850 Absorbance                      [next absorbance]
		IMV_NO_IMV,						// 49 - Cuv 6 467/850 Absorbance                      [next absorbance]
		IMV_NO_IMV,						// 50 - Cuv 11 340/850 Absorbance                     [next absorbance]
		IMV_NO_IMV,						// 51 - Cuv 28 340/850 Absorbance                     [next absorbance]
		IMV_SYSCHK_DIL_ABS,				// 52 -	Cuv 6 340/850 Corrected Diluent Absorbance
		IMV_SYSCHK_SAMP11,				// 53 -	Cuv 11 340/850 Corrected Sample Absorbance
		IMV_SYSCHK_SAMP28,				// 54 -	Cuv 28 340/850 Corrected Sample Absorbance
		IMV_SYSCHK_WB_CUV_1_TRANS,		// 55 - Whole Blood check cuvette1transmission
		IMV_SYSCHK_INSUF_SAMP_CUV_1_ABS,// 56 - Insufficient Sample check cuvette1absorbance
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	1	SAMPLE_BLANK	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_NO_IMV,						// 2   	                                            [NGA has no path length correction IMV required]
		IMV_SB_CONST_LIP_340,			// 3  -	340 Lipemic Constant
		IMV_SB_CONST_HEM_340,			// 4  -	340 Hemolysis Constant
		IMV_SB_CONST_ICT_340,			// 5  -	340 Icteric Constant
		IMV_SB_CONST_LIP_405,			// 6  -	405 Lipemic Constant
		IMV_SB_CONST_HEM_405,			// 7  -	405 Hemolysis Constant
		IMV_SB_CONST_ICT_405,			// 8  -	405 Icteric Constant
		IMV_SB_CONST_LIP_467,			// 9  -	467 Lipemic Constant
		IMV_SB_CONST_HEM_467,			// 10 -	467 Hemolysis Constant
		IMV_SB_CONST_ICT_467,			// 11 -	467 Icteric Constant
		IMV_SB_CONST_LIP_SLOPE,			// 12 -	Lipemic Slope
		IMV_SB_CONST_LIP_INT,			// 13 -	Lipemic Intercept
		IMV_SB_CONST_HEM_SLOPE,			// 14 -	Hemolysis Slope
		IMV_SB_CONST_HEM_INT,			// 15 -	Hemolysis Intercept
		IMV_SB_CONST_ICT_SLOPE,			// 16 -	Icteric Slope
		IMV_SB_CONST_ICT_INT,			// 17 -	Icteric Intercept
		IMV_TEST_BEAD_MIX_LIMIT,		// 18 - Bead Mix Limit                              [SB bead mix limit]
		IMV_850_OPTICAL_TRANSMISSION,	// 19 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 20 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 21 -	Test Bead Mix
		IMV_TEST_START_ABS_LIMIT,		// 22 - High Absorbance Limit                       [SB high start abs limit] 
		IMV_NO_IMV,						// 23 - 340/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 24 - 405/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 25 - 467/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 26 - 500/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 27 - 515/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 28 - 550/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_NO_IMV,						// 29 - 600/850 Pass 1 Absorbance                   [next SB start abs, decr]
		IMV_SB_START_ABS_START,			// 30 -	630/850 Pass 1 Absorbance
		IMV_SB_ABS_AVG_START,			// 31 -	340/850 Diluent Blank Average Absorbance
		IMV_NO_IMV,						// 32 - 405/850 Diluent Blank Average Absorbance    [next SB avg abs, incr]
		IMV_NO_IMV,						// 33 - 467/850 Diluent Blank Average Absorbance    [next SB avg abs, incr]
		IMV_NO_IMV,						// 34 - 340/850 Blank Average Absorbance            [next SB avg abs, incr]
		IMV_NO_IMV,						// 35 - 405/850 Blank Average Absorbance            [next SB avg abs, incr]
		IMV_NO_IMV,						// 36 - 467/850 Blank Average Absorbance            [next SB avg abs, incr]
		IMV_NO_IMV,						// 37 - 340/850 Corrected Blank Average Absorbance  [next SB avg abs, incr]
		IMV_NO_IMV,						// 38 - 405/850 Corrected Blank Average Absorbance  [next SB avg abs, incr]
		IMV_NO_IMV,						// 39 - 467/850 Corrected Blank Average Absorbance  [next SB avg abs, incr] 
		IMV_SB_340_TURB,				// 40 -	Lipemic Average Absorbance
		IMV_SB_405_HEME,				// 41 -	Hemolysis Average Absorbance
		IMV_SB_467_ICT,					// 42 -	Icteric Average Absorbance
		IMV_SB_HB_INDEX,				// 43 -	Hemoglobin Level
		IMV_SB_LIPEMIC_INDEX,			// 44 -	Lipemia level
		IMV_SB_ICTERIC_INDEX,			// 45 -	Icteric level
		IMV_SB_MIN_TEMP_ALLOWED,		// 46 -	Minimum Temperature Allowed
		IMV_SB_MAX_TEMP_ALLOWED,		// 47 -	Maximum Temperature Allowed
		IMV_SB_TEMP_SWING_LIMIT,		// 48 -	Temperature Swing Limit
		IMV_SB_FIRST_SAMP_TEMP,			// 49 -	Pass 1 Temperature Reading
		IMV_SB_MID_SAMP_TEMP,			// 50 -	Pass 7 Temperature Reading
		IMV_SB_LAST_SAMP_TEMP,			// 51 -	Pass 13 Temperature Reading
		IMV_SB_AVG_SAMP_TEMP,			// 52 -	Average System Temperature
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	2	RQC	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Base Abs                            [BC_RQC_BASE_ABS]	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Low Limit                           [BC_RQC_LOW_LIMIT]
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Decay Constant                      [BC_RQC_DECAY_CONST]
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_RQC_RATIO_SUPPRESS_LIMIT,	// 25 -	Ratio Suppress Limit
		IMV_RQC_HIGH_SUPPRESS_LIMIT,	// 26 -	High Suppress Limit
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_RQC_AVG_ABS,				// 41 -	515/600 Average
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_WL_0_CORRECTED_AVG,			// 46 -	515/600 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_ROTOR_AGE_IN_DAYS,			// 48 -	Rotor Age (in Days)
		IMV_RQC_QC_TIME,				// 49 - Rotor Age (in Months)                       [rotor age in months]
		IMV_RQC_EXPONENT_VALUE,			// 50 - Arrhenius Correction Exponent               [rqc arrhenius correction exponent]
		IMV_RQC_E_TO_THE_X,				// 51 - Arrhenius Correction Factor                 [rqc arrhenius correction factor]
		IMV_RQC_EXPECTED_QC,			// 52 -	RQC Expected
		IMV_RQC_ABS_START_PASS,			// 53 -	515/600 Pass 9 Absorbance
		IMV_NO_IMV,						// 54 - 515/600 Pass 10 Absorbance                  [next RQC abs]
		IMV_NO_IMV,						// 55 - 515/600 Pass 11 Absorbance                  [next RQC abs]
		IMV_RQC_LOW_LIMIT,				// 56 -	Low Limit
		IMV_RQC_HIGH_LIMIT,				// 57 -	High Limit
		IMV_RQC_ABUSED_LIMIT,			// 58 -	Abused Limit
		IMV_RQC_ABUSED,					// 59 -	Result Abused
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_RQC_RESULT,					// 63 - RQC Result
	},

	{									// idx - analyte type: 	3	IQC	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_IQC_ABS_LIMIT,				// 2  -	Absorbance Limit
		IMV_IQC_PRECISION_LIMIT,		// 3  -	Precision Limit
		IMV_IQC_RATIO_SIGMA,			// 4  -	Ratio Sigma
		IMV_IQC_RATIO_LIMIT,			// 5  -	Ratio Limit
		IMV_IQC_LEVEL1_MIN,				// 6  -	Level 1 Minimum
		IMV_IQC_LEVEL1_MAX,				// 7  -	Level 1 Maximum
		IMV_IQC_LEVEL1_RNG,				// 8  -	Level 1 Range
		IMV_IQC_LEVEL2_MIN,				// 9  -	Level 2 Minimum
		IMV_IQC_LEVEL2_MAX,				// 10 -	Level 2 Maximum
		IMV_IQC_LEVEL2_RNG,				// 11 -	Level 2 Range
		IMV_IQC_PRECISION_MIN,			// 12 -	Precision Minimum
		IMV_IQC_PRECISION_MAX,			// 13 -	Precision Maximum
		IMV_IQC_PRECISION_RNG,			// 14 -	Precision Range
		IMV_IQC_LEVEL_1_START,			// 15 -	IQC 1: DAC TRIM Min [%]
		IMV_NO_IMV,						// 16 - IQC 1: Lamp Intensity Max [%]               [next level 1]
		IMV_NO_IMV,						// 17 - IQC 1: Black Offset SD Max [%]              [next level 1] 
		IMV_NO_IMV,						// 18 - IQC 1: Optical Cuvette SD Max Light [%]     [next level 1] 
		IMV_NO_IMV,						// 19 - IQC 1: Black Offset Max [%]                 [next level 1] 
		IMV_NO_IMV,						// 20 - IQC 1: System Flash Range Max [%]           [next level 1] 
		IMV_NO_IMV,						// 21 - IQC 1: Lamp Intensisty Min [%]              [next level 1] 
		IMV_NO_IMV,						// 22 - IQC 1: Filter Curves [%]                    [next level 1] 
		IMV_IQC_ABS_A_START,			// 23 -	Absorbance A 340
		IMV_NO_IMV,						// 24 - Absorbance A 405                            [next abs A] 
		IMV_NO_IMV,						// 25 - Absorbance A 467                            [next abs A]
		IMV_NO_IMV,						// 26 - Absorbance A 500                            [next abs A]
		IMV_NO_IMV,						// 27 - Absorbance A 515                            [next abs A]
		IMV_NO_IMV,						// 28 - Absorbance A 550                            [next abs A]
		IMV_NO_IMV,						// 29 - Absorbance A 600                            [next abs A]
		IMV_NO_IMV,						// 30 - Absorbance A 630                            [next abs A]
		IMV_IQC_ABS_B_START,			// 31 -	Absorbance B 340
		IMV_NO_IMV,						// 32 - Absorbance B 405                            [next abs B]
		IMV_NO_IMV,						// 33 - Absorbance B 467                            [next abs B]
		IMV_NO_IMV,						// 34 - Absorbance B 500                            [next abs B]
		IMV_NO_IMV,						// 35 - Absorbance B 515                            [next abs B]
		IMV_NO_IMV,						// 36 - Absorbance B 550                            [next abs B]
		IMV_NO_IMV,						// 37 - Absorbance B 600                            [next abs B]
		IMV_NO_IMV,						// 38 - Absorbance B 630                            [next abs B]
		IMV_IQC_RATIO_START,			// 39 - Ratio 340
		IMV_NO_IMV,						// 40 - Ratio 405                                   [next ratio]
		IMV_NO_IMV,						// 41 - Ratio 467                                   [next ratio]
		IMV_NO_IMV,						// 42 - Ratio 500                                   [next ratio]
		IMV_NO_IMV,						// 43 - Ratio 515                                   [next ratio]
		IMV_NO_IMV,						// 44 - Ratio 550                                   [next ratio]
		IMV_NO_IMV,						// 45 - Ratio 630                                   [next ratio]
		IMV_NO_IMV,						// 46 - Ratio 850                                   [next ratio]
		IMV_IQC_LEVEL_2_START,			// 47 -	Level 2 Result 340
		IMV_NO_IMV,						// 48 - Level 2 Result 405                          [next level 2]
		IMV_NO_IMV,						// 49 - Level 2 Result 467                          [next level 2]
		IMV_NO_IMV,						// 50 - Level 2 Result 500                          [next level 2]
		IMV_NO_IMV,						// 51 - Level 2 Result 515                          [next level 2]
		IMV_NO_IMV,						// 52 - Level 2 Result 550                          [next level 2]
		IMV_NO_IMV,						// 53 - Level 2 Result 630                          [next level 2]
		IMV_NO_IMV,						// 54 - Level 2 Result 850                          [next level 2]
		IMV_IQC_RATIO_AVG,				// 55 -	Average of all Wavelength Ratios
		IMV_IQC_RATIO_STD,				// 56 -	SD of all Wavelength Ratios
		IMV_IQC_RATIO_PRECISION,		// 57 -	CV of all Wavelength Ratios
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	4	AG	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	5	ALB_BCG	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_HEM_LIMIT,					// 2  - HEM Limit
		IMV_LIP_LIMIT,					// 3  - LIP Limit
		IMV_ICT_LIMIT,					// 4  - ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  - Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  - Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  - High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  - High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  - Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_TEST_START_ABS_LIMIT_B,		// 11 - 405/500 Start Absorbance Limit
		IMV_BARCODE_FACTOR_3,			// 12 - Barcode Cat Slope 
		IMV_BARCODE_FACTOR_4,			// 13 - Barcode Cat  Intercept
		IMV_BARCODE_FACTOR_1,			// 14 - Barcode Dog Slope
		IMV_BARCODE_FACTOR_2,			// 15 - Barcode Dog  Intercept
		IMV_BARCODE_FACTOR_7,			// 16 - Barcode Horse Slope
		IMV_BARCODE_FACTOR_8,			// 17 - Barcode Horse  Intercept
		IMV_BARCODE_FACTOR_5,			// 18 - Barcode Other Slope
		IMV_BARCODE_FACTOR_6,			// 19 - Barcode Other  Intercept
		IMV_FLATNESS_TOGGLE,			// 20 - Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23 											    [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_TURB_BLANKED_ABS_START,		// 26 - Turbidity Check Blanked Abs value at Start Pass 
		IMV_TURB_BLANKED_ABS_END,		// 27 - Turbidity Check Blanked Abs value at End   Pass
		IMV_TURB_CORR_LIMIT,			// 28 - Turbidity Check Pass Correctoin Limit
		IMV_TURB_AVG_START_PASS,		// 29 - Turbidity Avg Abs Start Pass Used
		IMV_TURB_AVG_END_PASS,			// 30 - Turbidity Avg Abs End   Pass Used
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 630/850 Starting Absorbance 
		IMV_STARTING_ABS_B_BLANK,		// 40 - 405/500 Starting Absorbance 
		IMV_WL_1_BLANKED_AVG,			// 41 - 630/500 Average                             [legacy prime:   index [1] for numToAvg]  
		IMV_WL_1_BLANKED_MIN,			// 42 - 630/500 Minimum                             [legacy prime:   index [1] for numToAvg]
		IMV_WL_1_BLANKED_MAX,			// 43 - 630/500 Maximum                             [legacy prime:   index [1] for numToAvg]
		IMV_WL_1_FLATNESS,				// 44 - 630/500 Rate In Endpoint (Noise) Absolute   [legacy prime:   index [1] for numToAvg]
		IMV_WL_1_FLATNESS_AVG,			// 45 - 630/500 Rate In Endpoint (Noise) Ratio      [legacy prime:   index [1] for numToAvg]
		IMV_WL_1_CORRECTED_AVG,			// 46 - 630/500 Corrected Average                   [legacy prime:   index [1] for numToAvg]
		IMV_WL_0_BLANKED_AVG,			// 47 - 630/405 Average                             [legacy derived: index [0] for numToAvg]
		IMV_WL_0_BLANKED_MIN,			// 48 - 630/405 Minimum                             [legacy derived: index [0] for numToAvg]
		IMV_WL_0_BLANKED_MAX,			// 49 - 630/405 Maximum                             [legacy derived: index [0] for numToAvg]
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_WL_0_CORRECTED_AVG,			// 52 - 630/405 Corrected Average                   [legacy derived: index [0] for numToAvg]
		IMV_WL_1_BLANKED_START_PASS,	// 53 - 630/500 Pass 1 Blanked                      [legacy prime:   index [1] for numToAvg]
		IMV_NO_IMV,						// 54 - 630/500 Pass 2 Blanked                      [next pass]
		IMV_NO_IMV,						// 55 - 630/500 Pass 3 Blanked                      [next pass]
		IMV_WL_0_BLANKED_START_PASS,	// 56 - 630/405 Pass 1 Blanked                      [legacy derived: index [0] for numToAvg]
		IMV_NO_IMV,						// 57 - 630/405 Pass 2 Blanked                      [next pass]
		IMV_NO_IMV,						// 58 - 630/405 Pass 3 Blanked                      [next pass]
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	6	ALB_BCP	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode 550 Slope                           
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode 550  Intercept
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode 630 Slope
		IMV_BARCODE_FACTOR_4,			// 15 - Barcode 630 Intercept
		IMV_NO_IMV,						// 16 
		IMV_NO_IMV,						// 17  
		IMV_NO_IMV,						// 18  
		IMV_NO_IMV,						// 19  
		IMV_FLATNESS_TOGGLE,			// 20 - Rate In Endpoint (Noise) Toggle 
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - Rate In Endpoint (Noise) Ratio Limit 
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - Rate In Endpoint (Noise) Absolute Limit 
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_DUAL_WL_HI_DELTA_LIMIT,		// 25 - Wavelength Result Difference High Limit
		IMV_DUAL_WL_LO_DELTA_LIMIT,		// 26 - Wavelength Result Difference Low Limit
		IMV_DUAL_WL_0_RESULT_WEIGHT,	// 27 - 600/550 Weighting Factor
		IMV_DUAL_WL_1_RESULT_WEIGHT,	// 28 - 600/630 Weighting Factor
		IMV_NO_IMV,						// 29
		IMV_WL_RATIO_LIMIT,				// 30 - Ratio 600/850 to 630/850 Limit
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 600/850 Starting Absorbance 
		IMV_NO_IMV,						// 40 
		IMV_WL_0_BLANKED_AVG,			// 41 - 600/550 Average 
		IMV_WL_0_BLANKED_MIN,			// 42 - 600/550 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 - 600/550 Maximum
		IMV_WL_0_FLATNESS,				// 44 - 600/550 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 - 600/550 Rate In Endpoint (Noise) Ratio0
		IMV_WL_0_CORRECTED_AVG,			// 46 - 600/550 Corrected Average
		IMV_WL_1_BLANKED_AVG,			// 47 - 600/630 Average
		IMV_WL_1_BLANKED_MIN,			// 48 - 600/630 Minimum
		IMV_WL_1_BLANKED_MAX,			// 49 - 600/630 Maximum
		IMV_WL_1_FLATNESS,				// 50 - 600/630 Rate In Endpoint (Noise) Absolute
		IMV_WL_1_FLATNESS_AVG,			// 51 - 600/630 Rate In Endpoint (Noise) Ratio
		IMV_WL_1_CORRECTED_AVG,			// 52 - 600/630 Corrected Average 
		IMV_WL_0_BLANKED_START_PASS,	// 53 - 600/550 Pass 1 Blanked 
		IMV_NO_IMV,						// 54 - 600/550 Pass 2 Blanked                      [next pass] 
		IMV_NO_IMV,						// 55 - 600/550 Pass 3 Blanked                      [next pass] 
		IMV_WL_1_BLANKED_START_PASS,	// 56 - 600/630 Pass 1 Blanked 
		IMV_NO_IMV,						// 57 - 600/630 Pass 2 Blanked                      [next pass] 
		IMV_NO_IMV,						// 58 - 600/630 Pass 3 Blanked                      [next pass] 
		IMV_WL_0_RESULT,				// 59 - 600/550 Result
		IMV_WL_1_RESULT,				// 60 - 600/630 Result
		IMV_WL_COMP_RESULT_RATIO,		// 61 - Wavelength Result Difference
		IMV_WL_RATIO,					// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	7	ALP	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 - Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 - Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 - Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 - Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 - High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 - Decay Correction Factor
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 - 500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 - 405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature 
		IMV_RATE_EXPONENT_VALUE,		// 40 - Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 - Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 - Number of points
		IMV_SLOPE_RATE,					// 43 - Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 - Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 - Result (No Decay Correction)
		IMV_STARTING_ABS_B_BLANK,		// 51 - 405/850 Starting Absorbance                 [rate Window 1, same as idx 38]
		IMV_ABSORBANCE_WINDOW_0,		// 52 - 405/850 Pass 3 Absorbance(Window 5)         [rate Window 5]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 4 Absorbance(Window 4)         [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 54 - 405/850 Pass 6 Absorbance(Window 3)         [rate Window 3]
		IMV_ABSORBANCE_WINDOW_3,		// 55 - 405/850 Pass 11 Absorbance(Window 2)        [rate Window 2]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	8	ALT_1	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Range
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 - Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 - Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24                                               [NGA has no path length correction IMV required]
		IMV_ARRH_FACTOR,				// 25 - Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 - Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 - Window2 Threshold
		IMV_WINDOW_THRESHOLD_MULT,		// 28 - Threshold Multiplier
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 - High Dynamic Adjust
		IMV_RESULT_FALSE_RATE_MULT,		// 30 - False Rate Multiplier
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 - 340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 - 405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature 				   
		IMV_RATE_EXPONENT_VALUE,		// 40 - Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 - Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 - Number of points
		IMV_SLOPE_RATE,					// 43 - Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 - Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 - 405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_4,		// 51 - Pass 2 Absorbance(Window 5)                 [rate Window 5]
		IMV_ABSORBANCE_WINDOW_3,		// 52 - Pass 3 Absorbance(Window 4)                 [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 53 - Pass 4 Absorbance(Window 3)                 [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 54 - Pass 7 Absorbance(Window 2)                 [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 55 - Pass 13 Absorbance(Window 1)                [rate Window 1]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	9	AMY	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28	
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 -	Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_STARTING_ABS_B_BLANK,		// 51 - 405/850 Starting Absorbance                 [rate Window 1, same as idx 38]
		IMV_ABSORBANCE_WINDOW_0,		// 52 - 405/850 Pass 3 Absorbance(Window 5)         [rate Window 5]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 4 Absorbance(Window 4)         [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 54 - 405/850 Pass 6 Absorbance(Window 3)         [rate Window 3]
		IMV_ABSORBANCE_WINDOW_3,		// 55 - 405/850 Pass 11 Absorbance(Window 2)        [rate Window 2]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	10	AST_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Range
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 -	Window2 Threshold
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 -	Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 340/405 Pass 2 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 340/405 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 340/405 Pass 6 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 340/405 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	11	BA_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Limit
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Limit
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 - Window Threshold                            [threshold]
		IMV_END_ABS_THRESHOLD,			// 27 - Window  3 Threshold                         [BA_THRESHOLD]
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_2,		// 51 - 405/850 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 52 - 405/850 Pass 7 Absorbance (Window 2)        [rate Window 2: blank corrected]
		IMV_ABSORBANCE_WINDOW_0,		// 53 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1: blank corrected]
		IMV_NO_IMV,						// 54
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_ANALYTE_RESULT,				// 62 - Cuv Result
		IMV_RATE_WINDOW_USED,			// 63 - Window Used
	},

	{									// idx - analyte type: 	12	BA_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Limit
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Limit
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 - Window Threshold                            [threshold]
		IMV_END_ABS_THRESHOLD,			// 27 - Window  3 Threshold                         [BA_THRESHOLD]
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature 
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_2,		// 51 - 405/850 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 52 - 405/850 Pass 7 Absorbance (Window 2)        [rate Window 2: blank corrected]
		IMV_ABSORBANCE_WINDOW_0,		// 53 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1: blank corrected]
		IMV_NO_IMV,						// 54
		IMV_2_CUV_RESULT_1,				// 55 - BA1 Result
		IMV_2_CUV_RESULT_2,				// 56 - BA2 Result
		IMV_2_CUV_RESULT_DIFF,			// 57 - Result Difference
		IMV_2_CUV_BLEND_START_LIMIT,	// 58 - Start Blend                                 [match legacy BA_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_RANGE,			// 59 - Blend Range                                 [match legacy BA_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_END_LIMIT,		// 60 - End Blend                                   [match legacy BA_2 cuvette intermediate value]
		IMV_2_CUV_BLEND,				// 61 - Cuv 1 Blend Ratio                           [match legacy BA_2 cuvette intermediate value]
		IMV_ANALYTE_RESULT,				// 62 - Cuv Result
		IMV_RATE_WINDOW_USED,			// 63 - Window Used
	},

	{									// idx - analyte type: 	13	BUN_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 - Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 -	Window2 Threshold
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Factor
		IMV_FALSE_RATE_RATIO_LIMIT,		// 32 -	False Rate Ratio Limit
		IMV_FALSE_RATE_ABSOLUTE_LIMIT,	// 33 -	False Rate Absolute Limit
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_4,		// 51 - 340/405 Pass 2 Absorbance (Window 5)        [rate Window 5]
		IMV_ABSORBANCE_WINDOW_3,		// 52 - 340/405 Pass 3 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 53 - 340/405 Pass 4 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 54 - 340/405 Pass 7 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 55 - 340/405 Pass 9 Absorbance (Window 1)        [rate Window 1]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	14	CA	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope A1 405 
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Slope A2 405
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Slope A3 405
		IMV_BARCODE_FACTOR_4,			// 15 - Barcode Slope A1 600
		IMV_BARCODE_FACTOR_5,			// 16 - Barcode Slope A2 600
		IMV_BARCODE_FACTOR_6,			// 17 - Barcode Slope A3 600 
		IMV_NO_IMV,						// 18  
		IMV_NO_IMV,						// 19  
		IMV_NO_IMV,						// 20                                                   [no IMV_FLATNESS_TOGGLE]
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - 600/850 Rate In Endpoint (Noise) Ratio Limit    [used for wavelength index [1]
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - 405/467 Rate In Endpoint (Noise) Absolute Limit [used for wavelength index [0]
		IMV_NO_IMV,						// 23                                                   [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 405/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 - 600/850 Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 - 405/467 Corrected Average                       [legacy derived:   index [0] for numToAvg] 
		IMV_WL_0_BLANKED_MIN,			// 42 - 405/467 Corrected Minimum                       [legacy derived:   index [0] for numToAvg
		IMV_WL_0_BLANKED_MAX,			// 43 - 405/467 Corrected Maximum                       [legacy derived:   index [0] for numToAvg
		IMV_WL_0_FLATNESS,				// 44 - 405/467 Rate In Endpoint (Noise) Absolute       [legacy derived:   index [0] for numToAvg
		IMV_WL_0_FLATNESS_AVG,			// 45 - 405/467 Rate In Endpoint (Noise) Ratio          [legacy derived:   index [0] for numToAvg
		IMV_WL_0_CORRECTED_AVG,			// 46 - 405/467 Corrected Average                       [legacy derived:   index [0] for numToAvg
		IMV_WL_1_BLANKED_AVG,			// 47 - 600/850 Blanked Average                         [legacy tert:      index [1] for numToAvg
		IMV_WL_1_BLANKED_MIN,			// 48 - 600/850 Blanked Minimum                         [legacy tert:      index [1] for numToAvg
		IMV_WL_1_BLANKED_MAX,			// 49 - 600/850 Blanked Maximum                         [legacy tert:      index [1] for numToAvg
		IMV_WL_1_FLATNESS,				// 50 - 600/850 Rate In Endpoint (Noise) Absolute       [legacy tert:      index [1] for numToAvg
		IMV_WL_1_FLATNESS_AVG,			// 51 - 600/850 Rate In Endpoint (Noise) Ratio          [legacy tert:      index [1] for numToAvg
		IMV_WL_1_CORRECTED_AVG,			// 52 - 600/850 Corrected Average                       [legacy tert:      index [1] for numToAvg 
		IMV_WL_0_RESULT,				// 53 - 405/467 Result                                  [legacy derived:   index [0] for numToAvg 
		IMV_WL_1_RESULT,				// 54 - 600/850 Result                                  [legacy tert:      index [1] for numToAvg
		IMV_CA_PRE_CORRECTION_RESULT,	// 55 - Calcium result before potential lot correction 
		IMV_CA_RESULT_CORRECTION,		// 56 - Calcium result correction for applicable lot 
		IMV_NO_IMV,						// 57 
		IMV_NO_IMV,						// 58 
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_WL_COMP_RESULT_RATIO,		// 63 - Wavelength Result Ratio                         [legacy wavelength comp result ratio]
	},

	{									// idx - analyte type: 	15	CHOL	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_SLOPE_USED,			// 12 - Barcode Slope                               [used in result calc]	
		IMV_BARCODE_INTERCEPT_USED,		// 13 - Barcode Intercept                           [used in result calc]
		IMV_BARCODE_FACTOR_1,			// 14 - Barcode Slope Base                          [SLOPE     BASE]
		IMV_BARCODE_FACTOR_3,			// 15 - Barcode Intercept Base                      [INTERCEPT BASE]
		IMV_BARCODE_FACTOR_2,			// 16 - Barcode Slope Fine                          [SLOPE     FINE]
		IMV_BARCODE_FACTOR_4,			// 17 - Barcode Intercept Fine                      [INTERCEPT FINE]
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	340/850 Blank Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 -	340/405 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	340/405 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	340/405 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	340/405 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	340/405 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	340/405 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_WL_0_BLANKED_START_PASS,	// 51 - 340/405 Pass 1, 10 or 11  Blanked           [start pass: 1,  10, 11  (based on rotor ROC file)]
		IMV_NO_IMV,						// 52 - 340/405 Pass 2, 11 or 12 Blanked            [next pass:  2,  11, 12]
		IMV_NO_IMV,						// 53 - 340/405 Pass 3, 12 or 13 Blanked            [next pass:  3,  12, 13]
		IMV_NO_IMV,						// 54 - 340/405 Pass 4 or 13 Blanked (Lipid only)   [next pass:  4,  13]
		IMV_NO_IMV,						// 55 - 340/405 Pass 5 (Lipid only)                 [next pass:  5]
		IMV_NO_IMV,						// 56 - 340/405 Pass 6 (Lipid only)                 [next pass:  6]
		IMV_NO_IMV,						// 57 - 340/405 Pass 7 (Lipid only)                 [next pass:  7]
		IMV_NO_IMV,						// 58 - 340/405 Pass 8 (Lipid only)                 [next pass:  8]
		IMV_NO_IMV,						// 59 - 340/405 Pass 9 (Lipid only)                 [next pass:  9]
		IMV_NO_IMV,						// 60 - 340/405 Pass 10 (Lipid only)                [next pass:  10]
		IMV_NO_IMV,						// 61 - 340/405 Pass 11 (Lipid only)                [next pass:  11]
		IMV_NO_IMV,						// 62 - 340/405 Pass 12 (Lipid only)                [next pass:  12]
		IMV_NO_IMV,						// 63 - 340/405 Pass 13 (Lipid only)                [next pass:  13]
	},

	{									// idx - analyte type: 	16	CK_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_RESULT_FALSE_RATE_MULT,		// 30 -	405/850 False Rate Multiplier
		IMV_NO_IMV,						// 31
		IMV_RESULT_ENDOGENOUS_MULT,		// 32 -	Hemolysis Index Rate Multiplier
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 340/850 Pass 3 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 340/850 Pass 4 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 340/850 Pass 7 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 340/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	17	CL_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23 
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Correction Factor
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33 
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	405/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	500/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 - Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47 
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 405/850 Pass 5 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 405/850 Pass 7 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 10 Absorbance (Window 2)       [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_RATE_WINDOW_USED,			// 56 -	Window Used
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	18	CL_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23 
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Correction Factor
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32 - Result Difference Limit                     [match legacy CL_2 cuvette intermediate value]
		IMV_NO_IMV,						// 33 
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	405/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	500/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47 
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 405/850 Pass 5 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 405/850 Pass 7 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 10 Absorbance (Window 2)       [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_RATE_WINDOW_USED,			// 56 -	Window Used
		IMV_2_CUV_RESULT_1,				// 57 - CL1 Result                                  [match legacy CL_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - CL2 Result                                  [match legacy CL_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy CL_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	19	CRE_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Curve
		IMV_BARCODE_FACTOR_4,			// 15 - Barcode Curve Offset
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_CRE_WET_TIME,				// 25 -	Wet Time
		IMV_STABILITY_RATE,				// 26 -	Decay Correction Factor
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 550/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	550/850 Blank Starting Absorbance
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 -	550/600 Pass 8 Blanked
		IMV_NO_IMV,						// 54 - 550/600 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 550/600 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56 - 550/600 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 57 - 550/600 Pass 12 Blanked                     [next pass blanked abs]
		IMV_CRE_BLANKED_AVG,			// 58 -	550/600 Average
		IMV_WL_0_CORRECTED_AVG,			// 59 -	550/600 Corrected Average
		IMV_RESULT_NO_DECAY,			// 60 -	Result No Decay
		IMV_CRE_RATE,					// 61 -	Rate
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	20	DBIL	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_LOW_SYSTEM_LIMIT,			// 4  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 5  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 6  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 7  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 8  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 9  -	Bead Mix Limit
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_STABILITY_RATE,				// 26 -	Decay Correction Factor
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 467/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	467/850 Blank Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 -	467/550 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	467/550 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	467/550 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	467/550 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	467/550 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	467/550 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 -	467/550 Pass 10 Blanked
		IMV_NO_IMV,						// 54 - 467/550 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 467/550 Pass 12 Blanked                     [next pass blanked abs]
		IMV_RESULT_NO_DECAY,			// 56 -	Result (No Decay Correction)
		IMV_ANALYTE_RESULT,				// 57 -	Result
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	21	GGT	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_RESULT_FALSE_RATE_MULT,		// 30 -	500/850 False Rate Multiplier
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 405/500 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	500/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_0,		// 51 - 405/850 Pass 3 Absorbance (Window 5)        [rate Window 5]
		IMV_ABSORBANCE_WINDOW_1,		// 52 - 405/850 Pass 4 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 53 - 405/850 Pass 7 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_3,		// 54 - 405/850 Pass 13 Absorbance (Window 2)       [rate Window 2]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	22	GLOB	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	23	GLU	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 340/850 Starting Absorbance
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	340/850 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	340/850 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	340/850 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	340/850 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	340/850 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	340/850 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 -	340/850 Pass 8  Blanked
		IMV_NO_IMV,						// 54 - 340/850 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 340/850 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	24	HDL_1	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope                               [derived)] 
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept                           [(derived)]
		IMV_BARCODE_FACTOR_7,			// 14 - Barcode ICT Factor                          [ICT FACTOR]
		IMV_HDL_PRIM_TIME_CORR_SLOPE,	// 15 - 550 Decay A1 
		IMV_HDL_PRIM_TIME_CORR_INT,		// 16 - 550 Decay A0
		IMV_HDL_SEC_TIME_CORR_SLOPE,	// 17 - 467 Decay A1 
		IMV_HDL_SEC_TIME_CORR_INT,		// 18 - 467 Decay A0  
		IMV_NO_IMV,						// 19  
		IMV_FLATNESS_TOGGLE,			// 20 - Rate In Endpoint (Noise) Toggle             [used for derived]
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - Rate In Endpoint (Noise) Ratio Limit        [used for derived]
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - Rate In Endpoint (Noise) Absolute Limit     [used for derived]
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_BARCODE_FACTOR_3,			// 29                                               [550 Decay A0
		IMV_BARCODE_FACTOR_4,			// 30                                               [550 Decay A1]
		IMV_BARCODE_FACTOR_5,			// 31                                               [467 Decay A0]
		IMV_BARCODE_FACTOR_6,			// 32                                               [467 Decay A1]
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 550/630 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 - 550/630 Blank Starting Absorbance
		IMV_HDL_DERIVED_BLANKED_AVG,	// 41 - Derived Blanked Average                     [derived / ICT corrected from prim & sec] 
		IMV_HDL_DERIVED_BLANKED_MIN,	// 42 - Derived Blanked Minimum
		IMV_HDL_DERIVED_BLANKED_MAX,	// 43 - Derived Blanked Maximum
		IMV_HDL_DERIVED_FLATNESS,		// 44 - Derived Rate In Endpoint (Noise) Absolute
		IMV_HDL_DERIVED_FLATNESS_AVG,	// 45 - Derived Rate In Endpoint (Noise) Ratio
		IMV_HDL_DERIVED_CORRECTED_AVG,	// 46 - Derived Corrected Average
		IMV_NO_IMV,						// 47
		IMV_WL_0_BLANKED_AVG,			// 48 - 550/630 Average                             [prim 550/630} 
		IMV_WL_0_BLANKED_MIN,			// 49 - 550/630 Minimum
		IMV_WL_0_BLANKED_MAX,			// 50 - 550/630 Maximum
		IMV_WL_0_FLATNESS,				// 51 - 550/630 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 52 - 550/630 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 53 - 550/630 Corrected Average 
		IMV_WL_1_BLANKED_AVG,			// 54 - 467/630 Average                             [sec  467/630]  
		IMV_WL_1_CORRECTED_AVG,			// 55 - 467/630 Corrected Average
		IMV_HDL_PRIM_TIME_CORR_AVG,		// 56 - 550/630 Time Corrected Average              [prim 550/630]
		IMV_HDL_SEC_TIME_CORR_AVG,		// 57 - 467/630 Time Corrected Average              [sec  467/630] 
		IMV_HDL_DERIVED_TIME_CORR_AVG,	// 58 - Derived Time Corrected Average
		IMV_HDL_RESULT_NO_DECAY,		// 59 - Result (No Decay Correction)
		IMV_HDL_RESULT_DECAY,			// 60 -	Result (Decay Correction)
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63 
	},

	{									// idx - analyte type: 	25	HDL_2	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope                               [(derived)] 
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept                           [(derived)]
		IMV_BARCODE_FACTOR_7,			// 14 - Barcode ICT Factor                          [ICT FACTOR]
		IMV_HDL_PRIM_TIME_CORR_SLOPE,	// 15 - 550 Decay A1 
		IMV_HDL_PRIM_TIME_CORR_INT,		// 16 - 550 Decay A0
		IMV_HDL_SEC_TIME_CORR_SLOPE,	// 17 - 467 Decay A1 
		IMV_HDL_SEC_TIME_CORR_INT,		// 18 - 467 Decay A0  
		IMV_NO_IMV,						// 19  
		IMV_FLATNESS_TOGGLE,			// 20 - Rate In Endpoint (Noise) Toggle             [used for derived]
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - Rate In Endpoint (Noise) Ratio Limit        [used for derived]
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - Rate In Endpoint (Noise) Absolute Limit     [used for derived]
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_2_CUV_BLEND_START_LIMIT,	// 25 - Start Blend                                 [match legacy HDL_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_RANGE,			// 26 - Blend Range                                 [match legacy HDL_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_END_LIMIT,		// 27 - End Blend                                   [match legacy HDL_2 cuvette intermediate value]
		IMV_NO_IMV,						// 28
		IMV_BARCODE_FACTOR_3,			// 29                                               [550 Decay A0]
		IMV_BARCODE_FACTOR_4,			// 30                                               [550 Decay A1]
		IMV_BARCODE_FACTOR_5,			// 31                                               [467 Decay A0]
		IMV_BARCODE_FACTOR_6,			// 32                                               [467 Decay A1]
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 -	550/630 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 - 550/630 Blank Starting Absorbance
		IMV_HDL_DERIVED_BLANKED_AVG,	// 41 - Derived Blanked Average                     [derived / ICT corrected from prim & sec] 
		IMV_HDL_DERIVED_BLANKED_MIN,	// 42 - Derived Blanked Minimum
		IMV_HDL_DERIVED_BLANKED_MAX,	// 43 - Derived Blanked Maximum
		IMV_HDL_DERIVED_FLATNESS,		// 44 - Derived Rate In Endpoint (Noise) Absolute
		IMV_HDL_DERIVED_FLATNESS_AVG,	// 45 - Derived Rate In Endpoint (Noise) Ratio
		IMV_HDL_DERIVED_CORRECTED_AVG,	// 46 - Derived Corrected Average
		IMV_2_CUV_RESULT_DIFF,			// 47 - Result Difference                           [match legacy HDL_2 cuvette intermediate value]
		IMV_WL_0_BLANKED_AVG,			// 48 - 550/630 Average                             [prim 550/630] 
		IMV_WL_0_BLANKED_MIN,			// 49 - 550/630 Minimum
		IMV_WL_0_BLANKED_MAX,			// 50 - 550/630 Maximum
		IMV_WL_0_FLATNESS,				// 51 - 550/630 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 52 - 550/630 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 53 - 550/630 Corrected Average 
		IMV_WL_1_BLANKED_AVG,			// 54 - 467/630 Average                             [sec  467/630]  
		IMV_WL_1_CORRECTED_AVG,			// 55 - 467/630 Corrected Average 
		IMV_HDL_PRIM_TIME_CORR_AVG,		// 56 - 550/630 Time Corrected Average              [prim 550/630]
		IMV_HDL_SEC_TIME_CORR_AVG,		// 57 - 467/630 Time Corrected Average              [sec  467/630] 
		IMV_HDL_DERIVED_TIME_CORR_AVG,	// 58 - Derived Time Corrected Average
		IMV_HDL_RESULT_NO_DECAY,		// 59 - Result (No Decay Correction)
		IMV_HDL_RESULT_DECAY,			// 60 -	Result (Decay Correction)
		IMV_2_CUV_BLEND,				// 61 - Cuv 1 Blend Ratio                           [match legacy HDL_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_1,				// 62 - HDL1 Result                                 [match legacy HDL_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 63 - HDL2 Result                                 [match legacy HDL_2 cuvette intermediate value] 
	},

	{									// idx - analyte type: 	26	IBIL	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	27	K	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope								[SLOPE]
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept                           [ANCHOR]
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Decay                               [DECAY]
		IMV_BARCODE_FACTOR_4,			// 15 - Barcode Slope Decay                         [SLOPE DECAY]
		IMV_BARCODE_FACTOR_5,			// 16 - Barcode Intercept Decay                     [ANCHOR DECAY]
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	340/405 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_NO_IMV,						// 42
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_NO_IMV,						// 44
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_RATE_SLOPE_DELTA_START,		// 51 - Delta Absorbance 1 
		IMV_NO_IMV,						// 52 - Delta Absorbance 2                          [next slope delta]
		IMV_NO_IMV,						// 53 - Delta Absorbance 3                          [next slope delta]
		IMV_NO_IMV,						// 54 - Delta Absorbance 4                          [next slope delta]
		IMV_NO_IMV,						// 55 - Delta Absorbance 5                          [next slope delta]
		IMV_NO_IMV,						// 56 - Delta Absorbance 6                          [next slope delta]
		IMV_NO_IMV,						// 57 - Delta Absorbance 7                          [next slope delta]
		IMV_NO_IMV,						// 58 - Delta Absorbance 8                          [next slope delta]
		IMV_NO_IMV,						// 59 - Delta Absorbance 9                          [next slope delta]
		IMV_NO_IMV,						// 60 - Delta Absorbance 10                         [next slope delta]
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	28	LDH	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 - Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_FALSE_RATE_CORR_TOGGLE,		// 32 -	False Rate Ratio Limit    (4.86) ; False Rate Toggle (4.88)
		IMV_FALSE_RATE_CORR_FACTOR,		// 33 -	False Rate Absolute Limit (4.86) ; False Rate Factor (4.88)
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	630/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 500/630 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 500/630 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 500/630 Rate                      [temperature bfd corrected rate]
		IMV_FALSE_RATE_CORR_RATE,		// 46 - Corrected 630/850 False Rate (4.88)         [false rate corrected (temperature bfd corrected rate)]
		IMV_SLOPE_FALSE_RATE,			// 47 -	630/850 False Rate
		IMV_FALSE_RATE_CORRECTION,		// 48 - False Rate Correction factor (4.88)         [false rate correction value (0.0 if falseRate is <= toggle limit)]
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_2,		// 51 - 500/850 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 52 - 500/850 Pass 6 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 53 - 500/850 Pass 12 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 54
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	29	LDL	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	30	MG	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_FALSE_RATE_RATIO_LIMIT,		// 32 -	False Rate Ratio Limit
		IMV_FALSE_RATE_ABSOLUTE_LIMIT,	// 33 -	False Rate Absolute Limit
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Opticial Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_2,		// 51 - 340/850 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 52 - 340/850 Pass 6 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 53 - 340/850 Pass 12 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 54
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	31	NA_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_RATE_CORR_NOISE_MULT,		// 15 - Rate Correction Noise Method Multiplier
		IMV_RATE_CORR_RATE_LIMIT,		// 16 - Rate Correction Rate Method Limit
		IMV_RATE_CORR_ABS_RATE,			// 17 - Rate Correction Rate Method abs compare rate
		IMV_RATE_CORR_RATE_MULT,		// 18 - Rate Correction Rate Method Multiplier
		IMV_RATE_CORR_CORRECTION,		// 19 - Rate Correction Calculated
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio High Limit    [rate noise ratio limit high]
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NOISE_RATIO_LIMIT_LOW,		// 23 - Rate Not Linear (Noise) Ratio Low Limit     [rate noise ratio limit low]
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Correction Factor
		IMV_NO_IMV,						// 32
		IMV_PRE_END_ABS_LIMIT,			// 33 - End Absorbance Limit                        [rate pre end abs limit]
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	405/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	500/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_PRE_SLOPE_CORRECTION,		// 43 - Uncorrected 405/500 Rate                    [slope rate prior to slopeRateCorrection]
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46 
		IMV_SLOPE_RATE,					// 47 - Corrected Rate                              [slope rate after slopeRateCorrection]
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 405/850 Pass 5 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 405/850 Pass 7 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 10 Absorbance (Window 2)       [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_RATE_WINDOW_USED,			// 56 -	Window Used
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_RESULT_NA_UNCORRECTED,		// 63 - NA_1 Result prior to NA stability decay rate correction
	},

	{									// idx - analyte type: 	32	NA_2	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_RATE_CORR_NOISE_MULT,		// 15 - Rate Correction Noise Method Multiplier
		IMV_RATE_CORR_RATE_LIMIT,		// 16 - Rate Correction Rate Method Limit
		IMV_RATE_CORR_ABS_RATE,			// 17 - Rate Correction Rate Method abs compare rate
		IMV_RATE_CORR_RATE_MULT,		// 18 - Rate Correction Rate Method Multiplier
		IMV_RATE_CORR_CORRECTION,		// 19 - Rate Correction Calculated
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio High Limit    [rate noise ratio limit high]
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NOISE_RATIO_LIMIT_LOW,		// 23 - Rate Not Linear (Noise) Ratio Low Limit     [rate noise ratio limit low]
		IMV_NO_IMV,						// 24 
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Correction Factor
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32 - Result Difference Limit                     [match legacy NA_2 cuvette intermediate value]
		IMV_PRE_END_ABS_LIMIT,			// 33 - End Absorbance Limit                        [rate pre end abs limit]
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	405/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	500/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_PRE_SLOPE_CORRECTION,		// 43 - Uncorrected 405/500 Rate                    [slope rate prior to slopeRateCorrection]
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 405/500 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 405/500 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_RATE,					// 47 - Corrected Rate                              [slope rate after slopeRateCorrection]
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 405/850 Pass 5 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 405/850 Pass 7 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 405/850 Pass 10 Absorbance (Window 2)       [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 405/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_RATE_WINDOW_USED,			// 56 - Window Used
		IMV_2_CUV_RESULT_1,				// 57 - NA1 Result                                  [match legacy NA_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - NA2 Result                                  [match legacy NA_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy NA_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_RESULT_NA_UNCORRECTED,		// 63 - NA_2 Result prior to NA stability decay rate correction
	},

	{									// idx - analyte type: 	33	PHOS	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_PHOS_PASS_START_IN_MINUTES,	// 25 - Rate In Endpoint Correction Start           [PHOS rate in endpoint correction start time computed]
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 340/850 Starting Absorbance
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	340/405 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	340/405 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	340/405 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	340/405 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	340/405 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	340/405 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_WL_0_BLANKED_START_PASS,	// 48 -	340/405 Pass 6 Blanked
		IMV_NO_IMV,						// 49 - 340/405 Pass 7 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 50 - 340/405 Pass 8 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 51 - 340/405 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 52 - 340/405 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 53 - 340/405 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 54 - 340/405 Pass 12 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_PHOS_DELTA_7_TO_13,			// 60 - Slope Pass 6 - 12                           [PHOS slope pass 7 to 13]
		IMV_PHOS_ABS_FRACTION_LIMIT,	// 61 - Pass 1 Absorbance Divide By 20              [PHOS pass 2 absorbance / 20.0]
		IMV_PHOS_RATE,					// 62 - 340/405 Rate                                [PHOS 340/405 rate/slope]
		IMV_PHOS_INTERCEPT,				// 63 - 340/405 Intercept                           [PHOS 340/405 intercept]
	},

	{									// idx - analyte type: 	34	T4	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  - Start Absorbance Limit 
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit                              [T4_BEAD_MIX_LIMIT]
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12  	
		IMV_NO_IMV,						// 13  
		IMV_NO_IMV,						// 14
		IMV_T4_G6PDH_EXPT_RATE,			// 15 -	Barcode G6PDH Expected Rate
		IMV_T4_SLOPE,					// 16 -	Barcode T4 Slope
		IMV_T4_INTERCEPT,				// 17 -	Barcode T4 Intercept
		IMV_T4_TOP,						// 18 -	Barcode T4 Top
		IMV_T4_BOTTOM,					// 19 -	Barcode T4 Bottom
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_T4_NOISE_LIMIT,				// 22 - Noise Limit
		IMV_NO_IMV,						// 23
		IMV_T4_G6_ARRH_FACTOR,			// 24 - G6PDH Arrhenius                             [G6_ARRH]
		IMV_T4_ARRH_FACTOR,				// 25 - T4 Arrhenius                                [T4_ARRH]
		IMV_T4_G6_MAX_DELTA,			// 26 - G6PDH Max Delta                             [G6_MAX_DELTA]
		IMV_BLANK_BEAD_MIX_LIMIT,		// 27 - G6PDH Bead Mix Limit                        [G6_BEAD_MIX_LIMIT]
		IMV_T4_HEM_COMBO_LIMIT,			// 28 - HEM Limit for HEM/LIP Combo Limit           [T4_HEM_COMBO_LIMIT]
		IMV_T4_LIP_COMBO_LIMIT,			// 29 - LIP Limit for HEM/LIP Combo Limit           [T4_LIP_COMBO_LIMIT]
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_T4_FALSE_RATE_LIMIT,		// 32 - 340/850 False Rate Limit                    [T4_FALSE_RATE_LIMIT]
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Opticial Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm T4  Test Bead Transmission            [t850_T4]
		IMV_NO_IMV,						// 36
		IMV_850_BLANK_TRANSMISSION,		// 37 - 850nm G6PDH  Test Bead Transmission         [t850_G6PDH]
		IMV_AVG_SENSOR_TEMPERATURE,		// 38 -	Average System Temperature
		IMV_T4_BFD_ARRH_ADJUST,			// 39 - T4 Temperature and BFD Correction
		IMV_T4_G6_BFD_ARRH_ADJUST,		// 40 -	G6PDH Temperature and BFD Correction
		IMV_T4_T4L1_RATE,				// 41 -	340/405 T4 Rate
		IMV_T4_T4L1_RATE_CORR,			// 42 - 340/405 T4 Rate (Temperature Adjusted)      [temperature & BFD correction]
		IMV_T4_T4L1_STD_ERROR,			// 43 -	340/405 T4 Rate Not Linear
		IMV_T4_T4L1_FALSE_RATE,			// 44 -	340/850 False Rate
		IMV_T4_G6P1_RATE,				// 45 -	340/405 G6PDH Rate
		IMV_T4_G6P1_RATE_CORR,			// 46 - 340/405 G6PDH Rate(Temperature Adjusted)    [temperature & BFD correction]
		IMV_NO_IMV,						// 47
		IMV_T4_G6P1_DEVIATION,			// 48 - Actual & Expected G6PDH Ratio               [actual & expected G6PDH ratio]
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_T4_ABS_AVG_SEC_CUV_4,		// 52 -	340/850 Cuv 4 Average Sample Absorbance
		IMV_T4_ABS_AVG_TERT_CUV_4,		// 53 -	405/850 Cuv 4 Average Sample Absorbance
		IMV_T4_ABS_AVG_SEC_CUV_27,		// 54 - 340/850 Cuv 27 Average Sample Absorbance
		IMV_T4_ABS_AVG_TERT_CUV_27,		// 55 - 405/850 Cuv 27 Average Sample Absorbance
		IMV_T4_CUV_4_TOTAL,				// 56 -	Sum Cuv 4 340/850 &  340/850 Wavelegth Absorbances
		IMV_T4_CUV_27_TOTAL,			// 57 -	Sum Cuv 27 340/850 &  340/850 Wavelegth Absorbances
		IMV_T4_SAMPLE_CHECK,			// 58 -	Ratio Cuv 27 Total To Cuv 4 Total
		IMV_T4_SAMPLE_CHECK_LIMIT,		// 59 -	Sample Check Limit
		IMV_T4_T4L1_NUMERATOR,			// 60 - Numerator For Transforming Rate             [numerator for transformed rate]
		IMV_T4_T4L1_DENOMINATOR,		// 61 - Denominator For Transforming Rate           [denominator for tranformed rate]
		IMV_T4_T4L1_TRANSFORMED,		// 62 - Transformed Rate                            [transformed rate]
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	35	TBIL	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_LOW_SYSTEM_LIMIT,			// 4  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 5  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 6  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 7  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 8  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 9  -	Bead Mix Limit
		IMV_NO_IMV,						// 10 
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate Not Linear (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 467/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	467/850 Blank Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 -	467/550 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	467/550 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	467/550 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	467/550 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	467/550 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	467/550 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 -	467/550 Pass 9  Blanked
		IMV_NO_IMV,						// 54 - 467/550 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 467/550 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	36	TC/H	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	37	TCO2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TCO2_PRI_HI_START_ABS,		// 9  -	High Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_TCO2_PRI_LO_START_ABS,		// 11 -	Low Start Absorbance Limit
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode A0	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode A1
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode A2
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_TEST_START_ABS_LIMIT,		// 19 -	Start Absorbance Limit
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_TCO2_DELTA_SLOPE_2_MIN,		// 24 -	Delta Slope 2 Minimum
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_TCO2_DELTA_SLOPE_2_MAX,		// 27 -	Delta Slope 2 Maximum
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_TCO2_DELTA_SLOPE_MIN,		// 30 -	Delta Slope Minimum
		IMV_TCO2_DELTA_SLOPE_MAX,		// 31 -	Delta Slope Maximum
		IMV_TCO2_START_INDEX,			// 32 -	Start Index
		IMV_TCO2_STOP_INDEX,			// 33 -	Stop Index
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Opticial Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	340/405 Starting Absorbance
		IMV_RATE_EXPONENT_VALUE,		// 39 - Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 40 -	Arrhenius Correction Factor
		IMV_TCO2_NUM_POINTS,			// 41 -	Number of points
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_TCO2_AVG_DELTA_SLOPE_CHANGE,// 50 -	Average Delta Slope Change
		IMV_TCO2_SUM_DELTA_SLOPE,		// 51 - Sum Delta Slope 
		IMV_RATE_SLOPE_DELTA_START,		// 52 - Delta Slope 1
		IMV_NO_IMV,						// 53 - Delta Slope 2                               [next slope delta]
		IMV_NO_IMV,						// 54 - Delta Slope 3                               [next slope delta]
		IMV_NO_IMV,						// 55 - Delta Slope 4                               [next slope delta]
		IMV_NO_IMV,						// 56 - Delta Slope 5                               [next slope delta]
		IMV_NO_IMV,						// 57 - Delta Slope 6                               [next slope delta]
		IMV_NO_IMV,						// 58 - Delta Slope 7                               [next slope delta]
		IMV_NO_IMV,						// 59 - Delta Slope 8                               [next slope delta]
		IMV_NO_IMV,						// 60 - Delta Slope 9                               [next slope delta]
		IMV_NO_IMV,						// 61 - Delta Slope 10                              [next slope delta]
		IMV_NO_IMV,						// 62 - Delta Slope 11                              [next slope delta]
		IMV_NO_IMV,						// 63 - Delta Slope 12                              [next slope delta]
	},

	{									// idx - analyte type: 	38	TP	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_BLANK_BEAD_MIX_LIMIT,		// 11 -	Blank Bead Mix Limit
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 550/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	550/850 Blank Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 -	550/850 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	550/850 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	550/850 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	550/850 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	550/850 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	550/850 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48                                               [Legacy has IMV_WL_0_ENDG_CORRECTED_AVG erroneously here, TP has no endogenous correction & not used]
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 - 550/850 Pass 10  Blanked
		IMV_NO_IMV,						// 54 - 550/850 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 550/850 Pass 12 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	39	TRIG	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_BLANK_BEAD_MIX_LIMIT,		// 11 -	Blank Bead Mix Limit
		IMV_BARCODE_SLOPE_USED,			// 12 - Barcode Slope                               [SLOPE     (used in result calc)]	
		IMV_BARCODE_INTERCEPT_USED,		// 13 - Barcode Intercept                           [INTERCEPT (used in result calc)]
		IMV_BARCODE_FACTOR_1,			// 14 - Barcode Slope Base                          [SLOPE     BASE]
		IMV_BARCODE_FACTOR_3,			// 15 - Barcode Intercept Base                      [INTERCEPT BASE]
		IMV_BARCODE_FACTOR_2,			// 16 - Barcode Slope Fine                          [SLOPE     FINE]
		IMV_BARCODE_FACTOR_4,			// 17 - Barcode Intercept Fine                      [INTERCEPT FINE]
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               
		IMV_NO_IMV,						// 24
		IMV_BLANK_START_ABS_LIMIT,		// 25 -	Blank Start Absorbance Limit
		IMV_TRIG_RATE_TRIG_LIMIT,		// 26 -	High Rate Limit
		IMV_TRIG_RATE_LIP_LIMIT,		// 27 -	High LIP Limit
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Test Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 500/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	500/850 Blank Starting Absorbance
		IMV_WL_0_BLANKED_AVG,			// 41 -	500/850 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	500/850 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	500/850 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	500/850 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	500/850 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	500/850 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_WL_0_BLANKED_START_PASS,	// 51 - 500/850 Pass 1 or 10  Blanked               [start pass: 1,  10  (based on rotor ROC file)]
		IMV_NO_IMV,						// 52 - 500/850 Pass 2 or 11 Blanked                [next pass:  2,  11]
		IMV_NO_IMV,						// 53 - 500/850 Pass 3 or 12 Blanked                [next pass:  3,  12]
		IMV_NO_IMV,						// 54 - 500/850 Pass 4 or 13 Blanked                [next pass:  4,  13]
		IMV_NO_IMV,						// 55 - 500/850 Pass 5 Blanked                      [next pass:  5]
		IMV_NO_IMV,						// 56 - 500/850 Pass 6 Blanked                      [next pass:  6]
		IMV_NO_IMV,						// 57 - 500/850 Pass 7 Blanked                      [next pass:  7]
		IMV_NO_IMV,						// 58 - 500/850 Pass 8 Blanked                      [next pass:  8]
		IMV_NO_IMV,						// 59 - 500/850 Pass 9 Blanked                      [next pass:  9]
		IMV_NO_IMV,						// 60 - 500/850 Pass 10 Blanked                     [next pass:  10]
		IMV_NO_IMV,						// 61 - 500/850 Pass 11 Blanked                     [next pass:  11]
		IMV_NO_IMV,						// 62 - 500/850 Pass 12 Blanked                     [next pass:  12]
		IMV_NO_IMV,						// 63 - 500/850 Pass 13 Blanked                     [next pass:  13]
	},

	{									// idx - analyte type: 	40	UA	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_ICT_BLANK_CORRECTION,		// 14 - ICT Blank Correction
		IMV_ICT_TEST_CORRECTION,		// 15 - ICT Test Correction
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_ICT_BLANK_FACTOR,			// 23 -	ICT Blank Factor
		IMV_ICT_TEST_FACTOR_2,			// 24 -	ICT Test Factor 2
		IMV_ICT_TEST_FACTOR_1,			// 25 -	ICT Test Factor 1
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 515/850 Starting Absorbance
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	515/600 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	515/600 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	515/600 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	515/600 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	515/600 Rate In Endpoint (Noise) Ratio
		IMV_WL_1_CORRECTED_AVG,			// 46 - 515/600 Corrected Average                   [IMV_WL_0_CORRECTED_AVG repeated in IMV_WL_1_CORRECTED_AVG slot] 
		IMV_WL_0_CORRECTED_AVG,			// 47 - 515/600 Endogenous Corrected Average        [IMV_WL_0_ENDG_CORRECTED_AVG (ICT corrected) in IMV_WL_0_CORRECTED_AVG slot]
		IMV_WL_0_AVG_TEST,				// 48 - 515/600 Average
		IMV_WL_0_AVG_BLANK,				// 49 - 515/500 Average Sample Blank Absorbance
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 -	515/600 Pass 9  Blanked
		IMV_NO_IMV,						// 54 - 515/600 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 515/600 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	41	VLDL	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	42	CTBIL	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_HEM_LIMIT,					// 2  - HEM Limit
		IMV_LIP_LIMIT,					// 3  - LIP Limit
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5  - Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  - Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  - High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  - High System Limit
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_BARCODE_SLOPE_USED,			// 12 - Barcode Slope
		IMV_BARCODE_INTERCEPT_USED,		// 13 - Barcode Intercept
		IMV_CTBIL_HORSE_SLOPE,			// 14 - Barcode Horse Slope
		IMV_CTBIL_HORSE_INT,			// 15 - Barcode Horse Intercept
		IMV_CTBIL_OTHER_SLOPE,			// 16 - Barcode Other Slope
		IMV_CTBIL_OTHER_INT,			// 17 - Barcode Other Intercept
		IMV_BARCODE_FACTOR_1,			// 18                                               [TBIL slope     or CTBIL slope adjust]
		IMV_BARCODE_FACTOR_2,			// 19                                               [TBIL intercept or CTBIL intercept adjust]
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_CTBIL_SB_467_ICT,			// 41 - icteric value (467 Absorbance)
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	43	BA	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_2_CUV_BLEND_START_LIMIT,	// 25
		IMV_2_CUV_BLEND_RANGE,			// 26
		IMV_2_CUV_BLEND_END_LIMIT,		// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32 - IMV_2_CUV_DIFF_LIMIT_CONST not used	since not check
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59 - IMV_2_CUV_RESULT_DIFF not checked for inconsistent results
		IMV_NO_IMV,						// 60
		IMV_2_CUV_BLEND,				// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	44	CL	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	45	HDL	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_2_CUV_BLEND_START_LIMIT,	// 25
		IMV_2_CUV_BLEND_RANGE,			// 26
		IMV_2_CUV_BLEND_END_LIMIT,		// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_2_CUV_BLEND,				// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	46	NA	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	47	ALT_2	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Range
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 14 - Result Difference Limit                     [match legacy type ALT_2 cuvette intermediate value]
		IMV_NO_IMV,						// 15 
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 16 - Result Difference Check Toggle              [new ALT_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_CHK_MAX,			// 17 - Result Difference Check Maximum             [new ALT_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 18 - Result Difference Limit Scale               [new ALT_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_USED,		// 19 - Result Difference Limit Used                [new ALT_2 cuvette intermediate value]
		IMV_NOISE_TOGGLE,				// 20 - Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 - Rate Not Linear (Noise) Ratio Limit 
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 - Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24                                               [NGA has no path length correction IMV required]
		IMV_ARRH_FACTOR,				// 25 - Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 - Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 - Window2 Threshold
		IMV_WINDOW_THRESHOLD_MULT,		// 28 - Threshold Multiplier
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 - High Dynamic Adjust
		IMV_RESULT_FALSE_RATE_MULT,		// 30 - False Rate Multiplier
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32 - Result Difference Limit                     [match legacy type ALT_2 cuvette intermediate value
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 - 340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 - 405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature 
		IMV_RATE_EXPONENT_VALUE,		// 40 - Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 - Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 - Number of points
		IMV_SLOPE_RATE,					// 43 - Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 - Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 - 405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_4,		// 51 - Pass 2 Absorbance(Window 5)                 [rate Window 5]
		IMV_ABSORBANCE_WINDOW_3,		// 52 - Pass 3 Absorbance(Window 4)                 [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 53 - Pass 4 Absorbance(Window 3)                 [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 54 - Pass 7 Absorbance(Window 2)                 [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 55 - Pass 13 Absorbance(Window 1)                [rate Window 1]
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57 - ALT1 Result                                 [match legacy type ALT_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - ALT2 Result                                 [match legacy type ALT_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy type ALT_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	48	ALT	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 34
		IMV_2_CUV_DIFF_CHK_MAX,			// 35
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 36
		IMV_2_CUV_DIFF_LIMIT_USED,		// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_2_CUV_DIFF_CHK_MASK_MSB,	// 62
		IMV_2_CUV_DIFF_CHK_MASK_LSB,	// 63
	},

	{									// idx - analyte type: 	49	BUN_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 14 - Result Difference Limit                     [match legacy type BUN_2 cuvette intermediate value]
		IMV_NO_IMV,						// 15 
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 16 - Result Difference Check Toggle              [new BUN_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_CHK_MAX,			// 17 - Result Difference Check Maximum             [new BUN_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 18 - Result Difference Limit Scale               [new BUN_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_USED,		// 19 - Result Difference Limit Used                [new BUN_2 cuvette intermediate value]
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 -	Window2 Threshold
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_STABILITY_RATE,				// 31 -	Decay Factor
		IMV_FALSE_RATE_RATIO_LIMIT,		// 32 -	False Rate Ratio Limit
		IMV_FALSE_RATE_ABSOLUTE_LIMIT,	// 33 -	False Rate Absolute Limit
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_ABSORBANCE_WINDOW_4,		// 51 - 340/405 Pass 2 Absorbance (Window 5)        [rate Window 5]
		IMV_ABSORBANCE_WINDOW_3,		// 52 - 340/405 Pass 3 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 53 - 340/405 Pass 4 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 54 - 340/405 Pass 7 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 55 - 340/405 Pass 9 Absorbance (Window 1)        [rate Window 1]
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57 - BUN1 Result                                 [match legacy type BUN_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - BUN2 Result                                 [match legacy type BUN_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy type BUN_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	50	BUN	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 34
		IMV_2_CUV_DIFF_CHK_MAX,			// 35
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 36
		IMV_2_CUV_DIFF_LIMIT_USED,		// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_2_CUV_DIFF_CHK_MASK_MSB,	// 62
		IMV_2_CUV_DIFF_CHK_MASK_LSB,	// 63
	},

	{									// idx - analyte type: 	51	CRE_2	
		IMV_PARAGRAPH,					// 0  - Paragraph 
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Curve
		IMV_BARCODE_FACTOR_4,			// 15 - Barcode Curve Offset
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_CRE_WET_TIME,				// 25 -	Wet Time
		IMV_STABILITY_RATE,				// 26 -	Decay Correction Factor
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32 - Result Difference Limit                                            [match legacy type CRE_2 cuvette intermediate value]
		IMV_NO_IMV,						// 33 
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 -	850nm Blank Bead Transmission
		IMV_850_BLANK_BEAD_MIX,			// 38 -	Blank Bead Mix
		IMV_STARTING_ABS_A,				// 39 - 550/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 40 -	550/850 Blank Starting Absorbance
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_2_CUV_RESULT_1,				// 48 - CRE1 Result                                 [match legacy type CRE_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 49 - CRE2 Result                                 [match legacy type CRE_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 50 - Result Difference                           [match legacy type CRE_2 cuvette intermediate value]
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_WL_0_BLANKED_START_PASS,	// 53 - 550/600 Pass 8 Blanked
		IMV_NO_IMV,						// 54 - 550/600 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 550/600 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 56 - 550/600 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 57 - 550/600 Pass 12 Blanked                     [next pass blanked abs]
		IMV_CRE_BLANKED_AVG,			// 58 -	550/600 Average								 
		IMV_WL_0_CORRECTED_AVG,			// 59 -	550/600 Corrected Average
		IMV_RESULT_NO_DECAY,			// 60 -	Result No Decay
		IMV_CRE_RATE,					// 61 -	Rate
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	52	CRE	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_2_CUV_DIFF_CHK_MASK_MSB,	// 62
		IMV_2_CUV_DIFF_CHK_MASK_LSB,	// 63
	},

	{									// idx - analyte type: 	53	CHW	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  - Version
		IMV_HEM_LIMIT,					// 2  - HEM Limit
		IMV_LIP_LIMIT,					// 3  - LIP Limit
		IMV_ICT_LIMIT,					// 4  - ICT Limit
		IMV_POS_NEG_LO_RATIO_LIMIT,		// 5  - Low Net Ratio Limit                         [for NET RATIO check]
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_POS_NEG_HI_RATIO_LIMIT,		// 8  - High Net Ratio Limit                        [for NET RATIO check]
		IMV_POS_NEG_START_ABS_HI_LIMIT,	// 9  - Start Absorbance High Limit                 [for 515/850, 600/850, 550/850 CHW CUV & OPT CUV]
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Test Bead Mix Limit
		IMV_BLANK_BEAD_MIX_LIMIT,		// 11 - Optical Bead Mix Limit                      [OPT CUV 5 limit]
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Ratio Cutoff                        [CHW Ratio Cutoff]
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode WB Offset	                        [CHW WB Offset for calcs]
		IMV_BARCODE_FACTOR_3,			// 14 - Barcode Conjugate Type                      [CHW Conjugate Type for calc logic]
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16 
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_POS_NEG_START_ABS_LO_LIMIT,	// 19 - 550/850 Start Absorbance Low Limit          [550/850 only CHW CUV & OPT CUV]
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_POS_NEG_RATE_LIMIT,			// 22 - Rate Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Instrument Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_850_BLANK_TRANSMISSION,		// 37 - 850nm Optical Bead Transmission             [OPT CUV 5 transmission]
		IMV_850_BLANK_BEAD_MIX,			// 38 - Optical Bead Mix                            [OPT CUV 5 bead mix]
		IMV_POS_NEG_TEST_RATIO_0,		// 39 - Test 600nm to 515nm OD Ratio (pass 1-5)
		IMV_POS_NEG_OPT_RATIO_0,		// 40 -	Optical 600nm to 515nm OD Ratio (pass 1-5)
		IMV_POS_NEG_NET_RATIO_0,		// 41 -	Net 600nm to 515nm OD Ratio (pass 1-5)
		IMV_POS_NEG_TEST_RATIO_1,		// 42 - Test 600nm to 515nm OD Ratio (pass 10-13)
		IMV_POS_NEG_OPT_RATIO_1,		// 43 -	Optical 600nm to 515nm OD Ratio (pass 10-13)
		IMV_POS_NEG_NET_RATIO_1,		// 44 -	Net 600nm to 515nm OD Ratio (pass 10-13)
		IMV_NO_IMV,						// 45
		IMV_POS_NEG_WL_0_RATE,			// 46 - 515/850 Net Rate                            [515/850 rate]
		IMV_POS_NEG_WL_1_RATE,			// 47 - 600/850 Net Rate                            [600/850 rate]
		IMV_NO_IMV,						// 48
		IMV_POS_NEG_NET_RATIO_SELECTED,	// 49 - Net Ratio Selected 
		IMV_POS_NEG_FINAL_RATIO_RESULT,	// 50 - Final Ratio Result
		IMV_POS_NEG_START_ABS_START,	// 51 - Test 515/850 Starting Absorbance            [start idx: CHW TEST 515/850 to high start abs limit]
		IMV_NO_IMV,						// 52 - Test 600/850 Starting Absorbance            [next  idx: CHW TEST 600/850 to high start abs limit]
		IMV_NO_IMV,						// 53 - Test 550/850 Starting Absorbance            [next  idx: CHW TEST 550/850 to high start abs limit]
		IMV_NO_IMV,						// 54                                               [next  idx: CHW TEST 550/850 to low  start abs limit]
		IMV_NO_IMV,						// 55 - Opt 515/850 Starting Absorbance             [next  idx: CHW OPT  515/850 to high start abs limit] 
		IMV_NO_IMV,						// 56 - Opt 600/850 Starting Absorbance             [next  idx: CHW OPT  600/850 to high start abs limit]
		IMV_NO_IMV,						// 57 - Opt 550/850 Starting Absorbance             [next  idx: CHW OPT  515/850 to high start abs limit]
		IMV_NO_IMV,						// 58                                               [next  idx: CHW OPT  515/850 to low  start abs limit]
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	54	ALB_BCP_A	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 - Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12 
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_BARCODE_FACTOR_1,			// 16 - Barcode 600 Slope 
		IMV_BARCODE_FACTOR_2,			// 17 - Barcode 600 Intercept  
		IMV_NO_IMV,						// 18  
		IMV_NO_IMV,						// 19   
		IMV_FLATNESS_TOGGLE,			// 20 - Rate In Endpoint (Noise) Toggle 
		IMV_FLATNESS_RATIO_LIMIT,		// 21 - Rate In Endpoint (Noise) Ratio Limit 
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 - Rate In Endpoint (Noise) Absolute Limit 
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_DUAL_WL_HI_DELTA_LIMIT,		// 25 - Wavelength Result Difference High Limit
		IMV_DUAL_WL_LO_DELTA_LIMIT,		// 26 - Wavelength Result Difference Low Limit
		IMV_DUAL_WL_0_RESULT_WEIGHT,	// 27 - 600/550 Weighting Factor
		IMV_DUAL_WL_1_RESULT_WEIGHT,	// 28 - 600/630 Weighting Factor
		IMV_NO_IMV,						// 29
		IMV_WL_RATIO_LIMIT,				// 30 - Ratio 600/850 to 630/850 Limit
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 - 850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 - 850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 - Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 600/850 Starting Absorbance 
		IMV_NO_IMV,						// 40 
		IMV_WL_0_BLANKED_AVG,			// 41 - 600/550 (or 600/850) Average 
		IMV_WL_0_BLANKED_MIN,			// 42 - 600/550 (or 600/850) Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 - 600/550 (or 600/850) Maximum
		IMV_WL_0_FLATNESS,				// 44 - 600/550 (or 600/850) Rate In Endpoint Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 - 600/550 (or 600/850) Rate In Endpoint Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 - 600/550 (or 600/850) Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52 
		IMV_WL_0_BLANKED_START_PASS,	// 53 - 600/550 (or 600/850) Pass 1 Blanked   
		IMV_NO_IMV,						// 54 - 600/550 (or 600/850) Pass 2 Blanked         [next pass] 
		IMV_NO_IMV,						// 55 - 600/550 (or 600/850) Pass 3 Blanked         [next pass] 
		IMV_NO_IMV,						// 56 
		IMV_NO_IMV,						// 57 
		IMV_NO_IMV,						// 58 
		IMV_WL_0_RESULT,				// 59 - 600/550 (or 600/850) Result
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_WL_RATIO,					// 62 - Ratio 600/850 to 630/850
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	55	CRP_1	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12 	
		IMV_NO_IMV,						// 13 
		IMV_BARCODE_A0_USED,			// 14 - Barcode A0 									[used - computed from base + fine for CRP_1]
		IMV_BARCODE_A1_USED,			// 15 - Barcode A1 									[used - computed from base + fine for CRP_1]
		IMV_BARCODE_A2_USED,			// 16 - Barcode A2 									[used - computed from base + fine for CRP_1]
		IMV_NO_IMV,						// 17 
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_WL_0_OVER_RANGE_START_PASS,	// 26 - Over System Range Start Pass 
		IMV_WL_0_OVER_RANGE_END_PASS,	// 27 - Over System Range End Pass
		IMV_WL_0_OVER_RANGE_LIMIT,		// 28 - Over System Range Limit
		IMV_WL_0_OVER_RANGE_SLOPE,		// 29 - Over System Range Slope
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	850nm Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 630/850 Starting Absorbances
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	630/850 Blanked Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	630/850 Blanked Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	630/850 Blanked Maximum
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_WL_0_CORRECTED_AVG,			// 46 -	630/850 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_WL_0_BLANKED_START_PASS,	// 51 -	630/850 Pass 1 Blanked
		IMV_NO_IMV,						// 52 - 630/850 Pass 2 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 53 - 630/850 Pass 3 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 54 - 630/850 Pass 4 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 630/850 Pass 5 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 56 - 630/850 Pass 6 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 57 - 630/850 Pass 7 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 58 - 630/850 Pass 8 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 59 - 630/850 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 60 - 630/850 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 61 - 630/850 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 62 - 630/850 Pass 12 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 63 - 630/850 Pass 13 Blanked                     [next pass blanked abs]
	},

	{									// idx - analyte type: 	56	CRP_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11 
		IMV_BARCODE_SLOPE_USED,			// 12 - Barcode Slope                               [used - computed from base + fine for CRP_2]	
		IMV_BARCODE_INTERCEPT_USED,		// 13 - Barcode Intercept							[used - computed from base + fine for CRP_2]
		IMV_BARCODE_A0_USED,			// 14 - Barcode A0 									[used - computed from base + fine for CRP_1]
		IMV_BARCODE_A1_USED,			// 15 - Barcode A1 									[used - computed from base + fine for CRP_1]
		IMV_BARCODE_A2_USED,			// 16 - Barcode A2 									[used - computed from base + fine for CRP_1]
		IMV_NO_IMV,						// 17 
		IMV_NO_IMV,						// 18 
		IMV_2_CUV_BLEND_ZONE_1,			// 19 - CRP2 Cuvette Blend Zone 1                   [CRP_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_ZONE_2,			// 20 - CRP2 Cuvette Blend Zone 2                   [CRP_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_START_LIMIT,	// 21 - CRP2 Cuvette Blend Start Limit              [CRP_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_RANGE,			// 22 - CRP2 Cuvette Blend Range                    [CRP_2 cuvette intermediate value]
		IMV_2_CUV_BLEND_END_LIMIT,		// 23 - CRP2 Cuvette Blend End Limit                [CRP_2 cuvette intermediate value]
		IMV_2_CUV_BLEND,				// 24 - CRP2 Cuvette Blend                          [CRP_2 cuvette intermediate value]
		IMV_NO_IMV,						// 25
		IMV_WL_0_OVER_RANGE_START_PASS,	// 26 - Over System Range Start Pass 
		IMV_WL_0_OVER_RANGE_END_PASS,	// 27 - Over System Range End Pass
		IMV_WL_0_OVER_RANGE_LIMIT,		// 28 - Over System Range Limit
		IMV_WL_0_OVER_RANGE_SLOPE,		// 29 - Over System Range Slope
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32 - CRP2 Cuvette Difference Limit               [CRP_2 cuvette intermediate value]
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	850nm Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 630/850 Starting Absorbances
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	630/850 Blanked Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	630/850 Blanked Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	630/850 Blanked Maximum
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_WL_0_CORRECTED_AVG,			// 46 -	630/850 Corrected Average
		IMV_2_CUV_RESULT_1,				// 47 - CRP1 Result                                 [CRP_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 48 - CRP2 Result                                 [CRP_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 49 - Result Difference                           [CRP_2 cuvette intermediate value]
		IMV_NO_IMV,						// 50
		IMV_WL_0_BLANKED_START_PASS,	// 51 -	630/850 Pass 1 Blanked
		IMV_NO_IMV,						// 52 - 630/850 Pass 2 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 53 - 630/850 Pass 3 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 54 - 630/850 Pass 4 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 630/850 Pass 5 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 56 - 630/850 Pass 6 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 57 - 630/850 Pass 7 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 58 - 630/850 Pass 8 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 59 - 630/850 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 60 - 630/850 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 61 - 630/850 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 62 - 630/850 Pass 12 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 63 - 630/850 Pass 13 Blanked                     [next pass blanked abs]
	},

	{									// idx - analyte type: 	57	CRP	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_2_CUV_BLEND_ZONE_1,			// 23
		IMV_2_CUV_BLEND_ZONE_2,			// 24
		IMV_2_CUV_BLEND_START_LIMIT,	// 25
		IMV_2_CUV_BLEND_RANGE,			// 26
		IMV_2_CUV_BLEND_END_LIMIT,		// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_2_CUV_BLEND,				// 61
		IMV_2_CUV_BLEND_MASK_MSB,		// 62
		IMV_2_CUV_BLEND_MASK_LSB,		// 63
	},

	{									// idx - analyte type: 	58	EGFR	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	59	NACL	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NACL_NA_CONF_LIMIT,			// 63
	},

	{									// idx - analyte type: 	60	LAC	
		IMV_PARAGRAPH,					// 0  - Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14 
		IMV_ICT_TEST_CORRECTION,		// 15 - ICT Test Correction
		IMV_ICT_TEST_FACTOR_1,			// 16 -	ICT Test Factor 1
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_FALSE_RATE_RATIO_LIMIT,		// 32 -	False Rate Ratio Limit
		IMV_FALSE_RATE_ABSOLUTE_LIMIT,	// 33 -	False Rate Absolute Limit
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Opticial Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	515/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	600/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 515/600 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 515/600 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 515/600 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	600/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_RESULT_NO_DECAY,			// 50 -	Result No Decay
		IMV_NO_IMV,						// 51 
		IMV_RATE_WINDOW_USED,			// 52 - 515/850 window used in rate algorithm        
		IMV_ABSORBANCE_WINDOW_0,		// 53 - 515/850 Pass 13 Absorbance (Window 1)       
		IMV_ABSORBANCE_WINDOW_1,		// 54 - 515/850 Pass 12 Absorbance (Window 2)       
		IMV_ABSORBANCE_WINDOW_2,		// 55 - 515/850 Pass 11 Absorbance (Window 3)       
		IMV_ABSORBANCE_WINDOW_3,		// 56 - 515/850 Pass 10 Absorbance (Window 4)       
		IMV_ABSORBANCE_WINDOW_4,		// 57 - 515/850 Pass 9  Absorbance (Window 5)       
		IMV_ABSORBANCE_WINDOW_5,		// 58 - 515/850 Pass 8  Absorbance (Window 6)       
		IMV_ABSORBANCE_WINDOW_6,		// 59 - 515/850 Pass 7  Absorbance (Window 7)       
		IMV_ABSORBANCE_WINDOW_7,		// 60 - 515/850 Pass 6  Absorbance (Window 8)       
		IMV_ABSORBANCE_WINDOW_8,		// 61 - 515/850 Pass 5  Absorbance (Window 9)       
		IMV_ABSORBANCE_WINDOW_9,		// 62 - 515/850 Pass 4  Absorbance (Window 10)      
		IMV_ABSORBANCE_WINDOW_10,		// 63 - 515/850 Pass 3  Absorbance (Window 11)      
	},

	{									// idx - analyte type: 	61	HB	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_FLATNESS_TOGGLE,			// 20 -	Rate In Endpoint (Noise) Toggle
		IMV_FLATNESS_RATIO_LIMIT,		// 21 -	Rate In Endpoint (Noise) Ratio Limit
		IMV_FLATNESS_ABSOLUTE_LIMIT,	// 22 -	Rate In Endpoint (Noise) Absolute Limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_STARTING_ABS_A,				// 39 - 630/850 Starting Absorbance
		IMV_NO_IMV,						// 40
		IMV_WL_0_BLANKED_AVG,			// 41 -	630/850 Average
		IMV_WL_0_BLANKED_MIN,			// 42 -	630/850 Minimum
		IMV_WL_0_BLANKED_MAX,			// 43 -	630/850 Maximum
		IMV_WL_0_FLATNESS,				// 44 -	630/850 Rate In Endpoint (Noise) Absolute
		IMV_WL_0_FLATNESS_AVG,			// 45 -	630/850 Rate In Endpoint (Noise) Ratio
		IMV_WL_0_CORRECTED_AVG,			// 46 -	630/850 Corrected Average
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_WL_0_BLANKED_START_PASS,	// 51 -	630/850 Pass 1 Blanked
		IMV_NO_IMV,						// 52 - 630/850 Pass 2 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 53 - 630/850 Pass 3 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 54 - 630/850 Pass 4 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 55 - 630/850 Pass 5 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 56 - 630/850 Pass 6 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 57 - 630/850 Pass 7 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 58 - 630/850 Pass 8 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 59 - 630/850 Pass 9 Blanked                      [next pass blanked abs]
		IMV_NO_IMV,						// 60 - 630/850 Pass 10 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 61 - 630/850 Pass 11 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 62 - 630/850 Pass 12 Blanked                     [next pass blanked abs]
		IMV_NO_IMV,						// 63 - 630/850 Pass 13 Blanked                     [next pass blanked abs]
	},

	{									// idx - analyte type: 	62	HCT	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_HCT_HB_LOW_LIMIT,			// 61
		IMV_HCT_HB_HIGH_LIMIT,			// 62
		IMV_HCT_HB_RESULT_DIVISOR,		// 63
	},

	{									// idx - analyte type: 	63	NHDLC	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	64	AST_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Range
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 14 - Result Difference Limit                     [match legacy type AST_2 cuvette intermediate value]
		IMV_NO_IMV,						// 15 
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 16 - Result Difference Check Toggle              [new AST_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_CHK_MAX,			// 17 - Result Difference Check Maximum             [new AST_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 18 - Result Difference Limit Scale               [new AST_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_USED,		// 19 - Result Difference Limit Used                [new AST_2 cuvette intermediate value]
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute limit
		IMV_NO_IMV,						// 23                                               [NGA has no path length correction IMV required]
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window1 Threshold
		IMV_END_ABS_THRESHOLD,			// 27 -	Window2 Threshold
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 -	Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 340/405 Pass 2 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 340/405 Pass 3 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 340/405 Pass 6 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 340/405 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57 - AST1 Result                                 [match legacy type AST_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - AST2 Result                                 [match legacy type AST_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy type AST_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	65	AST	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 34
		IMV_2_CUV_DIFF_CHK_MAX,			// 35
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 36
		IMV_2_CUV_DIFF_LIMIT_USED,		// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_2_CUV_DIFF_CHK_MASK_MSB,	// 62
		IMV_2_CUV_DIFF_CHK_MASK_LSB,	// 63
	},

	{									// idx - analyte type: 	66	CK_2	
		IMV_PARAGRAPH,					// 0  -	Paragraph
		IMV_VERSION,					// 1  -	Version
		IMV_HEM_LIMIT,					// 2  -	HEM Limit
		IMV_LIP_LIMIT,					// 3  -	LIP Limit
		IMV_ICT_LIMIT,					// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,			// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,			// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,			// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,			// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,		// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,		// 10 -	Bead Mix Limit
		IMV_NO_IMV,						// 11
		IMV_BARCODE_FACTOR_1,			// 12 - Barcode Slope	
		IMV_BARCODE_FACTOR_2,			// 13 - Barcode Intercept
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 14 - Result Difference Limit                     [match legacy type CK_2 cuvette intermediate value]
		IMV_NO_IMV,						// 15 
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 16 - Result Difference Check Toggle              [new CK_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_CHK_MAX,			// 17 - Result Difference Check Maximum             [new CK_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 18 - Result Difference Limit Scale               [new CK_2 cuvette intermediate value]
		IMV_2_CUV_DIFF_LIMIT_USED,		// 19 - Result Difference Limit Used                [new CK_2 cuvette intermediate value]
		IMV_NOISE_TOGGLE,				// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,			// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,		// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_ARRH_FACTOR,				// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,		// 26 -	Window Threshold
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_HIGH_DYNAMIC_RANGE_ADJUST,	// 29 -	High Dynamic Adjust
		IMV_RESULT_FALSE_RATE_MULT,		// 30 -	405/850 False Rate Multiplier
		IMV_NO_IMV,						// 31
		IMV_RESULT_ENDOGENOUS_MULT,		// 32 -	Hemolysis Index Rate Multiplier
		IMV_NO_IMV,						// 33
		IMV_850_OPTICAL_TRANSMISSION,	// 34 -	850nm Optical Transmission
		IMV_850_TEST_TRANSMISSION,		// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,			// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,				// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,		// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,		// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,		// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,			// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,			// 42 -	Number of points
		IMV_SLOPE_RATE,					// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,			// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,		// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,						// 46
		IMV_SLOPE_FALSE_RATE,			// 47 -	405/850 False Rate
		IMV_NO_IMV,						// 48
		IMV_RATE_NOT_LINEAR_RATIO,		// 49
		IMV_NO_IMV,						// 50
		IMV_ABSORBANCE_WINDOW_3,		// 51 - 340/850 Pass 3 Absorbance (Window 4)        [rate Window 4]
		IMV_ABSORBANCE_WINDOW_2,		// 52 - 340/850 Pass 4 Absorbance (Window 3)        [rate Window 3]
		IMV_ABSORBANCE_WINDOW_1,		// 53 - 340/850 Pass 7 Absorbance (Window 2)        [rate Window 2]
		IMV_ABSORBANCE_WINDOW_0,		// 54 - 340/850 Pass 13 Absorbance (Window 1)       [rate Window 1]
		IMV_NO_IMV,						// 55
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57 - CK1 Result                                  [match legacy type CK_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_2,				// 58 - CK2 Result                                  [match legacy type CK_2 cuvette intermediate value]
		IMV_2_CUV_RESULT_DIFF,			// 59 - Result Difference                           [match legacy type CK_2 cuvette intermediate value]
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	67	CK	
		IMV_PARAGRAPH,					// 0
		IMV_VERSION,					// 1
		IMV_HEM_LIMIT,					// 2
		IMV_LIP_LIMIT,					// 3
		IMV_ICT_LIMIT,					// 4
		IMV_LOW_SYSTEM_LIMIT,			// 5
		IMV_LOW_DYNAMIC_RANGE,			// 6
		IMV_HIGH_DYNAMIC_RANGE,			// 7
		IMV_HIGH_SYSTEM_LIMIT,			// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_2_CUV_TYPE,					// 30 - 2 cuvette algorithm method
		IMV_NO_IMV,						// 31
		IMV_2_CUV_DIFF_LIMIT_CONST,		// 32
		IMV_NO_IMV,						// 33
		IMV_2_CUV_DIFF_CHK_TOGGLE,		// 34
		IMV_2_CUV_DIFF_CHK_MAX,			// 35
		IMV_2_CUV_DIFF_LIMIT_SCALE,		// 36
		IMV_2_CUV_DIFF_LIMIT_USED,		// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_2_CUV_RESULT_1,				// 57
		IMV_2_CUV_RESULT_2,				// 58
		IMV_2_CUV_RESULT_DIFF,			// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_2_CUV_DIFF_CHK_MASK_MSB,	// 62
		IMV_2_CUV_DIFF_CHK_MASK_LSB,	// 63
	},

	{										// idx - analyte type: 	68	PHB	
		IMV_PARAGRAPH,						// 0  - Paragraph
		IMV_VERSION,						// 1  -	Version
		IMV_HEM_LIMIT,						// 2  -	HEM Limit
		IMV_LIP_LIMIT,						// 3  -	LIP Limit
		IMV_ICT_LIMIT,						// 4  -	ICT Limit
		IMV_LOW_SYSTEM_LIMIT,				// 5  -	Low System Limit
		IMV_LOW_DYNAMIC_RANGE,				// 6  -	Low Dynamic Range
		IMV_HIGH_DYNAMIC_RANGE,				// 7  -	High Dynamic Range
		IMV_HIGH_SYSTEM_LIMIT,				// 8  -	High System Limit
		IMV_TEST_START_ABS_LIMIT,			// 9  -	Start Absorbance Limit
		IMV_TEST_BEAD_MIX_LIMIT,			// 10 -	Bead Mix Limit
		IMV_NO_IMV,							// 11
		IMV_BARCODE_FACTOR_1,				// 12 - Barcode A0 Base	
		IMV_BARCODE_FACTOR_2,				// 13 - Barcode A0 Fine
		IMV_BARCODE_FACTOR_3,				// 14 - Barcode A1 Base 
		IMV_BARCODE_FACTOR_4,				// 15 - Barcode A1 Fine
		IMV_BARCODE_FACTOR_5,				// 16 -	Barcode A2 Base
		IMV_BARCODE_FACTOR_6,				// 17 -	Barcode A2 Fine
		IMV_NO_IMV,							// 18 
		IMV_NO_IMV,							// 19 
		IMV_NOISE_TOGGLE,					// 20 -	Rate Not Linear (Noise) Toggle
		IMV_NOISE_RATIO_LIMIT,				// 21 -	Rate Not Linear (Noise) Ratio Limit
		IMV_NOISE_ABSOLUTE_LIMIT,			// 22 -	Rate Not Linear (Noise) Absolute Limit
		IMV_NO_IMV,							// 23 
		IMV_NO_IMV,							// 24 
		IMV_ARRH_FACTOR,					// 25 -	Arrhenius Constant
		IMV_WINDOW_ABS_THRESHOLD,			// 26 -	Window Threshold
		IMV_NO_IMV,							// 27 
		IMV_BARCODE_A0_USED,				// 28 - Barcode A0 used in recovery 									
		IMV_BARCODE_A1_USED,				// 29 - Barcode A1 used in recovery									
		IMV_BARCODE_A2_USED,				// 30 - Barcode A2 used in recovery 									
		IMV_NO_IMV,							// 31 
		IMV_FALSE_RATE_RATIO_LIMIT,			// 32 -	False Rate Ratio Limit
		IMV_FALSE_RATE_ABSOLUTE_LIMIT,		// 33 -	False Rate Absolute Limit
		IMV_850_OPTICAL_TRANSMISSION,		// 34 -	850nm Opticial Transmission
		IMV_850_TEST_TRANSMISSION,			// 35 -	850nm Test Bead Transmission
		IMV_850_TEST_BEAD_MIX,				// 36 -	Test Bead Mix
		IMV_STARTING_ABS_A,					// 37 -	340/850 Starting Absorbance
		IMV_STARTING_ABS_B_BLANK,			// 38 -	405/850 Starting Absorbance
		IMV_AVG_SENSOR_TEMPERATURE,			// 39 - Average System Temperature
		IMV_RATE_EXPONENT_VALUE,			// 40 -	Arrhenius Correction Exponent
		IMV_RATE_E_TO_THE_X,				// 41 -	Arrhenius Correction Factor
		IMV_SLOPE_NUM_POINTS,				// 42 -	Number of points
		IMV_SLOPE_RATE,						// 43 -	Uncorrected 340/405 Rate
		IMV_SLOPE_STD_ERROR,				// 44 -	Standard Error of the 340/405 Rate
		IMV_TEMP_CORRECTED_RATE,			// 45 - Corrected 340/405 Rate                      [temperature bfd corrected rate]
		IMV_NO_IMV,							// 46
		IMV_SLOPE_FALSE_RATE,				// 47 -	405/850 False Rate
		IMV_NO_IMV,							// 48
		IMV_RATE_NOT_LINEAR_RATIO,			// 49
		IMV_RESULT_NO_DECAY,				// 50 -	Result No Decay
		IMV_NO_IMV,							// 51 
		IMV_RATE_WINDOW_USED,				// 52 - 340/850 window used in rate algorithm        
		IMV_ABSORBANCE_WINDOW_0,			// 53 - 340/850 Pass 13 Absorbance (Window 1)       
		IMV_ABSORBANCE_WINDOW_1,			// 54 - 340/850 Pass 12 Absorbance (Window 2)       
		IMV_ABSORBANCE_WINDOW_2,			// 55 - 340/850 Pass 11 Absorbance (Window 3)       
		IMV_ABSORBANCE_WINDOW_3,			// 56 - 340/850 Pass 10 Absorbance (Window 4)       
		IMV_ABSORBANCE_WINDOW_4,			// 57 - 340/850 Pass 9  Absorbance (Window 5)       
		IMV_ABSORBANCE_WINDOW_5,			// 58 - 340/850 Pass 8  Absorbance (Window 6)       
		IMV_ABSORBANCE_WINDOW_6,			// 59 - 340/850 Pass 7  Absorbance (Window 7)       
		IMV_ABSORBANCE_WINDOW_7,			// 60 - 340/850 Pass 6  Absorbance (Window 8)       
		IMV_ABSORBANCE_WINDOW_8,			// 61 - 340/850 Pass 5  Absorbance (Window 9)       
		IMV_ABSORBANCE_WINDOW_9,			// 62 - 340/850 Pass 4  Absorbance (Window 10)      
		IMV_ABSORBANCE_WINDOW_10,			// 63 - 340/850 Pass 3  Absorbance (Window 11)      
	},

	{									// idx - analyte type: 	69	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	70	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	71	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	72	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	73	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	74	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	75	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	76	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	77	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	78	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	79	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	80	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	81	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	82	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	83	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	84	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	85	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	86	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	87	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	88	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	89	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	90	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	91	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	92	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	93	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	94	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	95	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	96	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	97	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	98	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	99	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	100	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	101	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	102	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	103	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	104	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	105	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	106	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	107	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	108	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	109	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	110	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	111	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	112	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	113	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	114	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	115	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	116	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	117	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	118	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	119	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	120	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	121	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	122	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	123	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	124	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	125	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	126	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

	{									// idx - analyte type: 	127	SPARE	
		IMV_NO_IMV,						// 0
		IMV_NO_IMV,						// 1
		IMV_NO_IMV,						// 2
		IMV_NO_IMV,						// 3
		IMV_NO_IMV,						// 4
		IMV_NO_IMV,						// 5
		IMV_NO_IMV,						// 6
		IMV_NO_IMV,						// 7
		IMV_NO_IMV,						// 8
		IMV_NO_IMV,						// 9
		IMV_NO_IMV,						// 10
		IMV_NO_IMV,						// 11
		IMV_NO_IMV,						// 12
		IMV_NO_IMV,						// 13
		IMV_NO_IMV,						// 14
		IMV_NO_IMV,						// 15
		IMV_NO_IMV,						// 16
		IMV_NO_IMV,						// 17
		IMV_NO_IMV,						// 18
		IMV_NO_IMV,						// 19
		IMV_NO_IMV,						// 20
		IMV_NO_IMV,						// 21
		IMV_NO_IMV,						// 22
		IMV_NO_IMV,						// 23
		IMV_NO_IMV,						// 24
		IMV_NO_IMV,						// 25
		IMV_NO_IMV,						// 26
		IMV_NO_IMV,						// 27
		IMV_NO_IMV,						// 28
		IMV_NO_IMV,						// 29
		IMV_NO_IMV,						// 30
		IMV_NO_IMV,						// 31
		IMV_NO_IMV,						// 32
		IMV_NO_IMV,						// 33
		IMV_NO_IMV,						// 34
		IMV_NO_IMV,						// 35
		IMV_NO_IMV,						// 36
		IMV_NO_IMV,						// 37
		IMV_NO_IMV,						// 38
		IMV_NO_IMV,						// 39 
		IMV_NO_IMV,						// 40
		IMV_NO_IMV,						// 41
		IMV_NO_IMV,						// 42
		IMV_NO_IMV,						// 43
		IMV_NO_IMV,						// 44
		IMV_NO_IMV,						// 45
		IMV_NO_IMV,						// 46
		IMV_NO_IMV,						// 47
		IMV_NO_IMV,						// 48
		IMV_NO_IMV,						// 49
		IMV_NO_IMV,						// 50
		IMV_NO_IMV,						// 51
		IMV_NO_IMV,						// 52
		IMV_NO_IMV,						// 53
		IMV_NO_IMV,						// 54 
		IMV_NO_IMV,						// 55 
		IMV_NO_IMV,						// 56
		IMV_NO_IMV,						// 57
		IMV_NO_IMV,						// 58
		IMV_NO_IMV,						// 59
		IMV_NO_IMV,						// 60
		IMV_NO_IMV,						// 61
		IMV_NO_IMV,						// 62
		IMV_NO_IMV,						// 63
	},

};



//	bead mix data local definitions & storage

#define	MAX_BEAD_MIX_CHECKS		2
 
typedef struct
{
	float	beadMixLimit;
	float	transmission;
	float	optical;
	float	beadMix;
} BeadMixParams_t;

BeadMixParams_t	beadMixParams_m[ MAX_BEAD_MIX_CHECKS ];


typedef struct
{
	ImvType_t	beadMixLimit;
	ImvType_t	transmission;
	ImvType_t	optical;
	ImvType_t	beadMix;
}
BeadMixImvTypes_t;	


//	start abs data local definitions & storage

#define	MAX_START_ABS_CHECKS	8
 
typedef struct
{
	float	startAbsSample;
	float	startAbsLimit;
} StartAbsParams_t;

StartAbsParams_t	startAbsParams_m[ MAX_START_ABS_CHECKS ];


typedef struct
{
	ImvType_t	startAbsSample;
	ImvType_t	startAbsLimit;
}
StartAbsImvTypes_t;	


// endpoint average abs & flatness data local definitions & storage 

typedef struct
{
	float	flatness;
	float	flatnessAvg;
} FlatnessParams_t;

FlatnessParams_t	flatnessParams_m[ MAX_TO_AVG ];


// endpoint over system range data local definitions & storage 

typedef struct
{
	float	startPass;
	float	endPass;
	float	limit;
	float	slope;
} OverSystemRangeParams_t;

OverSystemRangeParams_t	overSystemRangeParams_m[ MAX_TO_AVG ];


typedef struct
{
	ImvType_t	blankedStartPass;
	ImvType_t	blankedAvg;
	ImvType_t	blankedMin;
	ImvType_t	blankedMax;

	ImvType_t	flatnessToggle;
	ImvType_t	flatnessRatioLimit;
	ImvType_t	flatnessAbsoluteLimit;
	ImvType_t	flatness;
	ImvType_t	flatnessAvg;

	ImvType_t	overRangeStartPass;
	ImvType_t	overRangeEndPass;
	ImvType_t	overRangeLimit;
	ImvType_t	overRangeSlope;

	ImvType_t	avgAnalyte;
	ImvType_t	avgBlank;
}
AvgAbsImvTypes_t;	


//	bfd corrected avg or rate data local definitions & storage

#define WL_1_CORRECTED_AVG_IDX	1

#define	MAX_BFD_CORRECTED		2

typedef struct
{
	float	bfdCorrectedAvgOrRate;
} BfdCorrectedParams_t;

BfdCorrectedParams_t	bfdCorrectedParams_m[ MAX_BFD_CORRECTED ];


typedef struct
{
	ImvType_t	bfdCorrected;
}
BfdCorrectedImvTypes_t;	


//	barcode factors local definitions & storage

#define	MAX_BARCODE_PARAMS		10

typedef struct
{
	float	barcodeParam;
} BarcodeParams_t;

BarcodeParams_t	barcodeParams_m[ MAX_BARCODE_PARAMS ];


typedef struct
{
	ImvType_t	barcodeParam;
}
BarcodeImvTypes_t;	


//	dual wavelength results and comparison local definitions & storage


typedef struct
{
	float	wl0Result;
	float	wl1Result;

	float	wlCompare;
} EndpointDualWl_t;

EndpointDualWl_t	endpointDualWl_m;


//	rate algorithm window absorbance compare values

float	rateWindowAbs_m[ MAX_SELECT_WINDOWS ];

float	rateWindow_m;


// rate algorithm temperature correction parameters

typedef struct
{
	float	arrhFactor;
	float	rotorAverageTemp;
	float	tempCorrectionExp;
	float	eToCorrectionExp;
} RateTemperatureCorrection_t;

RateTemperatureCorrection_t	rateTemperatureCorrection_m;


//	system check alogrithm absorbance / transmission parameters intermediate value storage

#define	MAX_SYSCHK_ABSORBANCES		5

#define	MAX_SYSCHK_TRANSMISSIONS	13

#define SYSCHK_TRANS_06_340			0
#define SYSCHK_TRANS_06_405			1
#define SYSCHK_TRANS_06_467			2
#define SYSCHK_TRANS_OPT_340		3
#define SYSCHK_TRANS_OPT_405		4
#define SYSCHK_TRANS_OPT_467		5
#define SYSCHK_TRANS_11_340			6
#define SYSCHK_TRANS_28_340			7

#define SYSCHK_RATIO_06_340			8
#define SYSCHK_RATIO_06_405			9
#define SYSCHK_RATIO_06_467			10
#define SYSCHK_RATIO_11_340			11
#define SYSCHK_RATIO_28_340			12

float	transmissionSystemCheck_m[ MAX_SYSCHK_TRANSMISSIONS ];


//	sample blank alogrithm average absorbance 340, 405, 467 parameters intermediate value storage

#define	MAX_SB_AVG_ABS				9

float	avgAbsSampleBlank_m[ MAX_SB_AVG_ABS ];


//	general intermediate value counters / indices used in storing intermediate values
 
typedef struct
{
	unsigned		beadMixIdx;
	unsigned		startAbsIdx;
	unsigned		flatnessIdx;
	unsigned		overRangeIdx;
	unsigned		bfdCorrectedIdx;
	unsigned		barcodeIdx;
	unsigned		nextImvIdx;			// counter index used in IMV_INCR_SET_METHOD			
} ImvCounters_t;

ImvCounters_t		imvCounters_m;		// imv counters init for each analyte and incr'd in case of more than 1 imv data (e.g. 2 values for bead mix if a blank, start abs if 2 wavelengths, etc.)


// ********************************************************************
// FUNCTION:
//
//		GetImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		get the imv from the rotorIntermediateValues table based on imv index  
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

float GetImv( unsigned char analyteType, unsigned imvIdx )
{
	if ( imvIdx < MAX_INTERMEDIATE_VALUES )
	{
		return (rotorResults_g->rotorIntermediateValues[ analyteType ][ imvIdx ]); 
	}
	else
	{
		return NOVAL_NEG_FLOAT;
	}
}


// ********************************************************************
// FUNCTION:
//
//		GetImvIdx() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Scan the imvTypeTable_m at analyteType for imvType to return imv index (0 to 63) to store the imv.
//		If imvType not found in imvTypeTable_m, imv index is returned as MAX_INTERMEDIATE_VALUES (64). 
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

unsigned GetImvIdx( unsigned char analyteType, unsigned imvIdx, ImvType_t imvType )
{
	while (	imvIdx < MAX_INTERMEDIATE_VALUES )
	{
		if ( imvTypeTable_m[ analyteType ][ imvIdx ] == imvType )
		{
			return imvIdx;
		}

		imvIdx++;
	}

	return imvIdx;
}


// ********************************************************************
// FUNCTION:
//
//		SetImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set the imv in the rotorIntermediateValues table.  
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

void SetImv( unsigned char analyteType, unsigned imvIdx, float imv )
{
	if ( imvIdx < MAX_INTERMEDIATE_VALUES )
	{
		rotorResults_g->rotorIntermediateValues[ analyteType ][ imvIdx ] = imv; 
	}
}


// ********************************************************************
// FUNCTION:
//
//		StoreIntermediateValue() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store an intermediate value based on analyteType, intermediate value ID,
//		and special intermediate value storage flag (e.g. auto incr, etc.) 
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

void StoreIntermediateValue( unsigned char analyteType, ImvType_t imvType, ImvMethod_t imvMethod, float imv )
{
	unsigned	imvIdx;

	imvIdx = 0;

	if ( analyteType < MAX_ANALYTE_TYPES )
	{
		switch (imvMethod)
		{
			case IMV_DIRECT_SET_METHOD:
				while ( imvIdx < MAX_INTERMEDIATE_VALUES )
				{
					imvIdx = GetImvIdx( analyteType, imvIdx, imvType );
					SetImv( analyteType, imvIdx, imv );

					imvIdx++;
				}
				break;

			case IMV_INCR_SET_METHOD:
				if ( imvCounters_m.nextImvIdx == 0 )
				{
					imvIdx = GetImvIdx( analyteType, 0, imvType );
					imvCounters_m.nextImvIdx = imvIdx + 1;
				}
				else
				{
					imvIdx = imvCounters_m.nextImvIdx++;
				}
				SetImv( analyteType, imvIdx, imv );
				break;

			case IMV_DECR_SET_METHOD:
				if ( imvCounters_m.nextImvIdx == 0 )
				{
					imvIdx = GetImvIdx( analyteType, 0, imvType );
					imvCounters_m.nextImvIdx = imvIdx - 1;
				}
				else
				{
					imvIdx = imvCounters_m.nextImvIdx--;
				}
				SetImv( analyteType, imvIdx, imv );
				break;

			case IMV_INCR_INIT_METHOD:
				imvCounters_m.nextImvIdx = 0;
				break;

			default:
				break;
		}
	}
}


// ********************************************************************
// FUNCTION:
//
//		InitNextIdxImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Clear imvCounters_m.nextImvIdx for StoreIntermediateValue IMV_INCR_SET_METHOD  
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

void InitNextIdxImv( void )
{
	imvCounters_m.nextImvIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		InitRotorImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Initialize Imv data storage for each rotor
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

void InitRotorImv( void )
{
	unsigned	i;
	unsigned	j;
	
	for ( i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		for ( j = 0; j < MAX_INTERMEDIATE_VALUES; j++)
		{
			rotorResults_g->rotorIntermediateValues[ i ][ j ] = NOVAL_NEG_FLOAT;
		} 
	}
}


// ********************************************************************
// FUNCTION:
//
//		InitAnalyteImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Initialize Imv counters/data for each analyte set 
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

void InitAnalyteImv( void )
{
	unsigned	i;


	// init IMV_INCR_SET_METHOD counter

	imvCounters_m.nextImvIdx = 0;


	// init for bead mix imv storage

	imvCounters_m.beadMixIdx = 0;

	for ( i = 0; i <  MAX_BEAD_MIX_CHECKS; i++ )
	{
		beadMixParams_m[i].beadMixLimit		= NOVAL_NEG_FLOAT;
		beadMixParams_m[i].transmission		= NOVAL_NEG_FLOAT;
		beadMixParams_m[i].optical			= NOVAL_NEG_FLOAT;
		beadMixParams_m[i].beadMix			= NOVAL_NEG_FLOAT;
	}


	// init for start abs imv storage

	imvCounters_m.startAbsIdx = 0;

	for ( i = 0; i <  MAX_START_ABS_CHECKS; i++ )
	{
		startAbsParams_m[i].startAbsSample	= NOVAL_NEG_FLOAT;
		startAbsParams_m[i].startAbsLimit	= NOVAL_NEG_FLOAT;
	}


	// init for endpoint flatness imv storage

	imvCounters_m.flatnessIdx = 0;

	for ( i = 0; i <  MAX_TO_AVG; i++ )
	{
		flatnessParams_m[i].flatness		= NOVAL_NEG_FLOAT;
		flatnessParams_m[i].flatnessAvg		= NOVAL_NEG_FLOAT;
	}


	// init for endpoint over system range imv storage

	imvCounters_m.overRangeIdx = 0;

	for ( i = 0; i <  MAX_TO_AVG; i++ )
	{
		overSystemRangeParams_m[i].startPass	= NOVAL_NEG_FLOAT;
		overSystemRangeParams_m[i].endPass		= NOVAL_NEG_FLOAT;
		overSystemRangeParams_m[i].limit		= NOVAL_NEG_FLOAT;
		overSystemRangeParams_m[i].slope		= NOVAL_NEG_FLOAT;
	}


	// init for bfd corrected avg or rate imv storage

	imvCounters_m.bfdCorrectedIdx = 0;

	for ( i = 0; i <  MAX_BFD_CORRECTED; i++ )
	{
		bfdCorrectedParams_m[i].bfdCorrectedAvgOrRate	= NOVAL_NEG_FLOAT;
	}


	// init for bfd corrected avg or rate imv storage

	imvCounters_m.barcodeIdx = 0;

	for ( i = 0; i <  MAX_BARCODE_PARAMS; i++ )
	{
		barcodeParams_m[i].barcodeParam	= 	NOVAL_NEG_FLOAT;
	}


	// init for endpoint dual wavelength results and compare imv storage

	endpointDualWl_m.wl0Result = NOVAL_NEG_FLOAT;
	endpointDualWl_m.wl1Result = NOVAL_NEG_FLOAT;

	endpointDualWl_m.wlCompare = NOVAL_NEG_FLOAT;


	// init for rate window abs compare

	for ( i = 0; i <  MAX_SELECT_WINDOWS; i++ )
	{
		rateWindowAbs_m[ i ] = NOVAL_NEG_FLOAT;
	}

	rateWindow_m = NOVAL_NEG_FLOAT;


	// init rate temperature correction params

   rateTemperatureCorrection_m.arrhFactor        = NOVAL_NEG_FLOAT;
   rateTemperatureCorrection_m.rotorAverageTemp  = NOVAL_NEG_FLOAT;
   rateTemperatureCorrection_m.tempCorrectionExp = NOVAL_NEG_FLOAT;
   rateTemperatureCorrection_m.eToCorrectionExp  = NOVAL_NEG_FLOAT;
}


// ********************************************************************
// FUNCTION:
//
//		StoreAlgParagraphNumImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store algorithm paragraph number 
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

void StoreAlgParagraphNumImv( unsigned char analyteType, float algParagraphNum )
{
	StoreIntermediateValue( analyteType, IMV_PARAGRAPH, IMV_DIRECT_SET_METHOD, algParagraphNum );
}


// ********************************************************************
// FUNCTION:
//
//		StoreAlgVersionNumImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store algorithm paragraph number 
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

void StoreAlgVersionNumImv( unsigned char analyteType )
{
	const float		legacyAlgorithmVersion[ MAX_ANALYTE_TYPES ] = 
	{
		4.84,				//  0:   SYSTEM_CHECK
		4.86,				//  1:   SAMPLE_BLANK 
		4.83,				//  2:   RQC 
		4.83,				//  3:   IQC    
		4.83,				//  4:   AG    
		4.86,				//  5:   ALB_BCG    
		4.86,				//  6:   ALB_BCP    
		4.86,				//  7:   ALP 
		4.86,				//  8:	 ALT_1
		4.86,				//  9:	 AMY
		4.86,				//  10:	 AST_1
		4.87,				//  11:  BA_1 
		4.87,				//  12:	 BA_2
		4.86,				//  13:	 BUN_1
		4.95,				//  14:	 CA
		4.86,				//  15:	 CHOL
		4.86,				//  16:	 CK_1
		4.95,				//  17:	 CL_1
		4.95,				//  18:	 CL_2
		4.86,				//  19:	 CRE_1
		4.84,				//  20:	 DBIL
		4.86,				//  21:	 GGT
		4.84,				//  22:	 GLOB
		4.86,				//  23:	 GLU
		4.86,				//  24:	 HDL_1
		4.86,				//  25:	 HDL_2
		4.84,				//  26:	 IBIL
		4.86,				//  27:	 K
		4.88,				//  28:  LDH
		4.84,				//  29:  LDL
		4.86,				//  30:	 MG
		4.86,				//  31:  NA_1 
		4.86,				//  32:	 NA_2
		4.86,				//  33:	 PHOS
		4.86,				//  34:	 T4
		4.84,				//  35:	 TBIL
		4.83,				//  36:	 TCH
		4.86,				//  37:	 TCO2
		4.86,				//  38:	 TP
		4.86,				//  39:	 TRIG
		4.86,				//  40:	 UA
		4.83,				//  41:	 VLDL
		4.91,				//  42:	 CTBIL
		4.87,				//  43:	 BA
		4.86,				//  44:	 CL
		4.86,				//  45:	 HDL
		4.86,				//  46:	 NA
		4.86,				//  47:	 ALT_2
		4.86,				//  48:  ALT
		4.86,				//  49:  BUN_2
		4.86,				//  50:	 BUN
		4.86,				//  51:  CRE_2 
		4.86,				//  52:	 CRE
		9.01,				//  53:	 CHW
		4.94,				//  54:	 ALB_BCP_A
		9.01,				//  55:	 CRP_1
		9.01,				//  56:	 CRP_2
		9.01,				//  57:	 CRP
		9.01,				//  58:  EGFR
		9.01,				//  59:	 NACL
		9.01,				//  60:	 LAC
		9.01,				//  61:	 HB
		9.01,				//  62:	 HCT
		9.01,				//  63:	 NHDLC
		4.86,				//  64:	 AST_2
		4.86,				//  65:	 AST
		4.86,				//  66:	 CK_2
		4.86,				//  67:	 CK
		9.01,				//  68:	 PHB
		NOVAL_NEG_FLOAT,	//  69:	 SPARE
		NOVAL_NEG_FLOAT,	//  70:	 SPARE
		NOVAL_NEG_FLOAT,	//  71:	 SPARE
		NOVAL_NEG_FLOAT,	//  72:	 SPARE
		NOVAL_NEG_FLOAT,	//  73:	 SPARE
		NOVAL_NEG_FLOAT,	//  74:	 SPARE
		NOVAL_NEG_FLOAT,	//  75:	 SPARE
		NOVAL_NEG_FLOAT,	//  76:	 SPARE
		NOVAL_NEG_FLOAT,	//  77:	 SPARE
		NOVAL_NEG_FLOAT,	//  78:	 SPARE
		NOVAL_NEG_FLOAT,	//  79:	 SPARE
		NOVAL_NEG_FLOAT,	//  80:	 SPARE
		NOVAL_NEG_FLOAT,	//  81:	 SPARE
		NOVAL_NEG_FLOAT,	//  82:	 SPARE
		NOVAL_NEG_FLOAT,	//  83:	 SPARE
		NOVAL_NEG_FLOAT,	//  84:	 SPARE
		NOVAL_NEG_FLOAT,	//  85:	 SPARE
		NOVAL_NEG_FLOAT,	//  86:	 SPARE
		NOVAL_NEG_FLOAT,	//  87:	 SPARE
		NOVAL_NEG_FLOAT,	//  88:	 SPARE
		NOVAL_NEG_FLOAT,	//  89:	 SPARE
		NOVAL_NEG_FLOAT,	//  90:	 SPARE
		NOVAL_NEG_FLOAT,	//  91:	 SPARE
		NOVAL_NEG_FLOAT,	//  92:	 SPARE
		NOVAL_NEG_FLOAT,	//  93:	 SPARE
		NOVAL_NEG_FLOAT,	//  94:	 SPARE
		NOVAL_NEG_FLOAT,	//  95:	 SPARE
		NOVAL_NEG_FLOAT,	//  96:	 SPARE
		NOVAL_NEG_FLOAT,	//  97:	 SPARE
		NOVAL_NEG_FLOAT,	//  98:	 SPARE
		NOVAL_NEG_FLOAT,	//  99:	 SPARE
		NOVAL_NEG_FLOAT,	//  100: SPARE
		NOVAL_NEG_FLOAT,	//  101: SPARE
		NOVAL_NEG_FLOAT,	//  102: SPARE
		NOVAL_NEG_FLOAT,	//  103: SPARE
		NOVAL_NEG_FLOAT,	//  104: SPARE
		NOVAL_NEG_FLOAT,	//  105: SPARE
		NOVAL_NEG_FLOAT,	//  106: SPARE
		NOVAL_NEG_FLOAT,	//  107: SPARE
		NOVAL_NEG_FLOAT,	//  108: SPARE
		NOVAL_NEG_FLOAT,	//  109: SPARE
		NOVAL_NEG_FLOAT,	//  110: SPARE
		NOVAL_NEG_FLOAT,	//  111: SPARE
		NOVAL_NEG_FLOAT,	//  112: SPARE
		NOVAL_NEG_FLOAT,	//  113: SPARE
		NOVAL_NEG_FLOAT,	//  114: SPARE
		NOVAL_NEG_FLOAT,	//  115: SPARE
		NOVAL_NEG_FLOAT,	//  116: SPARE
		NOVAL_NEG_FLOAT,	//  117: SPARE
		NOVAL_NEG_FLOAT,	//  118: SPARE
		NOVAL_NEG_FLOAT,	//  119: SPARE
		NOVAL_NEG_FLOAT,	//  120: SPARE
		NOVAL_NEG_FLOAT,	//  121: SPARE
		NOVAL_NEG_FLOAT,	//  122: SPARE
		NOVAL_NEG_FLOAT,	//  123: SPARE
		NOVAL_NEG_FLOAT,	//  124: SPARE
		NOVAL_NEG_FLOAT,	//  125: SPARE
		NOVAL_NEG_FLOAT,	//  126: SPARE
		NOVAL_NEG_FLOAT,	//  127: SPARE
	};								 

	StoreIntermediateValue( analyteType, IMV_VERSION, IMV_DIRECT_SET_METHOD, legacyAlgorithmVersion[ analyteType ] );
}


// ********************************************************************
// FUNCTION:
//
//		StoreCommonAnalyteImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store common analyte intermediate values at paragraph 1 time 
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

void StoreCommonAnalyteImv( unsigned char analyteType, CommonFactorsLimits_t *factorsLimitsPtr )
{
	StoreAlgParagraphNumImv( analyteType, 1.0 );

	StoreIntermediateValue( analyteType, IMV_HEM_LIMIT,          IMV_DIRECT_SET_METHOD, factorsLimitsPtr->hemolyzedLimit[MAX_ENDOGENOUS_ZONE_IDX] );
	StoreIntermediateValue( analyteType, IMV_LIP_LIMIT,          IMV_DIRECT_SET_METHOD, factorsLimitsPtr->lipemicLimit[MAX_ENDOGENOUS_ZONE_IDX] );
	StoreIntermediateValue( analyteType, IMV_ICT_LIMIT,          IMV_DIRECT_SET_METHOD, factorsLimitsPtr->ictericLimit[MAX_ENDOGENOUS_ZONE_IDX] );
	StoreIntermediateValue( analyteType, IMV_LOW_SYSTEM_LIMIT,   IMV_DIRECT_SET_METHOD, factorsLimitsPtr->lowSystemLimit );
	StoreIntermediateValue( analyteType, IMV_LOW_DYNAMIC_RANGE,  IMV_DIRECT_SET_METHOD, factorsLimitsPtr->lowDynamicRange );
	StoreIntermediateValue( analyteType, IMV_HIGH_DYNAMIC_RANGE, IMV_DIRECT_SET_METHOD, factorsLimitsPtr->highDynamicRange );
	StoreIntermediateValue( analyteType, IMV_HIGH_SYSTEM_LIMIT,  IMV_DIRECT_SET_METHOD, factorsLimitsPtr->highSystemLimit );
}


// ********************************************************************
// FUNCTION:
//
//		SetImvBeadMixParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set common bead mix params for storage 
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

void SetImvBeadMixParams( float sampleTransmission850, float OpticalBlankTransmission850, float beadMixCheckLimit )
{
	beadMixParams_m[ imvCounters_m.beadMixIdx ].beadMixLimit = beadMixCheckLimit; 
	beadMixParams_m[ imvCounters_m.beadMixIdx ].transmission = sampleTransmission850; 
	beadMixParams_m[ imvCounters_m.beadMixIdx ].optical      = OpticalBlankTransmission850; 
	beadMixParams_m[ imvCounters_m.beadMixIdx ].beadMix      = sampleTransmission850 / OpticalBlankTransmission850; 

	imvCounters_m.beadMixIdx++;
}


// ********************************************************************
// FUNCTION:
//
//		StoreBeadMixImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store common bead mix params intermediate values 
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

void StoreBeadMixImv( unsigned char analyteType )
{
	unsigned	i;

	const BeadMixImvTypes_t	beadMixImvTypes[ MAX_BEAD_MIX_CHECKS ] = 
	{
		{
			IMV_TEST_BEAD_MIX_LIMIT,
			IMV_850_TEST_TRANSMISSION,
			IMV_850_OPTICAL_TRANSMISSION,
			IMV_850_TEST_BEAD_MIX,
		},

		{
			IMV_BLANK_BEAD_MIX_LIMIT,
			IMV_850_BLANK_TRANSMISSION,
			IMV_850_OPTICAL_TRANSMISSION,
			IMV_850_BLANK_BEAD_MIX,
		},
	};

	// store bead mix for test or blank 

	for ( i = 0; i < imvCounters_m.beadMixIdx; i++ )
	{
		StoreIntermediateValue( analyteType, beadMixImvTypes[i].beadMixLimit, IMV_DIRECT_SET_METHOD, beadMixParams_m[i].beadMixLimit );
		StoreIntermediateValue( analyteType, beadMixImvTypes[i].transmission, IMV_DIRECT_SET_METHOD, beadMixParams_m[i].transmission );
		StoreIntermediateValue( analyteType, beadMixImvTypes[i].optical,      IMV_DIRECT_SET_METHOD, beadMixParams_m[i].optical );
		StoreIntermediateValue( analyteType, beadMixImvTypes[i].beadMix,      IMV_DIRECT_SET_METHOD, beadMixParams_m[i].beadMix );
	}

	imvCounters_m.beadMixIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvStartAbsParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set common starting abs params for storage 
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

void SetImvStartAbsParams( float startAbsSample, float startAbsLimit )
{
	startAbsParams_m[ imvCounters_m.startAbsIdx ].startAbsSample = startAbsSample; 
	startAbsParams_m[ imvCounters_m.startAbsIdx ].startAbsLimit  = startAbsLimit; 

	imvCounters_m.startAbsIdx++;
}


// ********************************************************************
// FUNCTION:
//
//		StoreStartAbsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store common starting abs params intermediate values 
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

void StoreStartAbsImv( unsigned char analyteType )
{
	unsigned	i;

	const StartAbsImvTypes_t	startAbsImvTypes[ MAX_START_ABS_CHECKS ] = 
	{
		{
			IMV_STARTING_ABS_A,				// endpoint wavelength A       or rate wavelength A
			IMV_TEST_START_ABS_LIMIT,
		},

		{
			IMV_STARTING_ABS_B_BLANK,		// endpoint blank wavelength A or rate wavelength B
			IMV_BLANK_START_ABS_LIMIT,
		},

		{
			IMV_STARTING_ABS_B_BLANK,		// endpoint wavelength B  (writes over blank data for wavelength B)
			IMV_TEST_START_ABS_LIMIT_B,		// e.g. ALB_BCG, ALB_BCP, ALB_BCP_A, CA, HDL_1, HDL_2
		},

		{
			IMV_NOT_ENABLED,				// endpoint blank wavelength B - not enabled, since not stored as imv for any analyte			 
			IMV_NOT_ENABLED,				// 
		},

		{
			IMV_NOT_ENABLED,				// endpoint ALB_BCG - not enabled, since only used to flag and not stored as imv 			 
			IMV_NOT_ENABLED,				// 
		},

		{
			IMV_NOT_ENABLED,				// spare 			 
			IMV_NOT_ENABLED,				// 
		},

		{
			IMV_NOT_ENABLED,				// spare 			 
			IMV_NOT_ENABLED,				// 
		},

		{
			IMV_NOT_ENABLED,				// spare 			 
			IMV_NOT_ENABLED,				// 
		},
	};

	// store starting abs for test or blank - note 2nd idx could be for endpoint blank or rate 2nd wavelength, 3rd idx could be for endpoint 2nd wavelength 

	for ( i = 0; i < imvCounters_m.startAbsIdx; i++ )
	{
		StoreIntermediateValue( analyteType, startAbsImvTypes[i].startAbsSample, IMV_DIRECT_SET_METHOD, startAbsParams_m[i].startAbsSample );
		StoreIntermediateValue( analyteType, startAbsImvTypes[i].startAbsLimit,  IMV_DIRECT_SET_METHOD, startAbsParams_m[i].startAbsLimit  );
	}

	imvCounters_m.startAbsIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		StorePosNegStartAbsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store pos/neg algorithm starting abs params intermediate values 
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

void StorePosNegStartAbsImv( unsigned char analyteType )
{
	unsigned	i;

	// store starting abs for pos/neg wavelength pairs 

	for ( i = 0; i < imvCounters_m.startAbsIdx; i++ )
	{
		StoreIntermediateValue( analyteType, IMV_POS_NEG_START_ABS_START, IMV_INCR_SET_METHOD, startAbsParams_m[i].startAbsSample );
	}

	imvCounters_m.startAbsIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvEndpointFlatnessParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set endpoint flatness params checked for rate in endpoint 
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

void SetImvEndpointFlatnessParams( float flatness, float flatnessAvg )
{
	flatnessParams_m[ imvCounters_m.flatnessIdx ].flatness = flatness;
	flatnessParams_m[ imvCounters_m.flatnessIdx ].flatnessAvg = flatnessAvg;

	imvCounters_m.flatnessIdx = (imvCounters_m.flatnessIdx + 1) % MAX_TO_AVG;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvEndpointOverSystemRangeParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set endpoint params checked over system range limit 
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

void SetImvEndpointOverSystemRangeParams( float startPass, float endPass, float limit, float slope )
{
	overSystemRangeParams_m[ imvCounters_m.overRangeIdx ].startPass = startPass;
	overSystemRangeParams_m[ imvCounters_m.overRangeIdx ].endPass   = endPass;
	overSystemRangeParams_m[ imvCounters_m.overRangeIdx ].limit     = limit;
	overSystemRangeParams_m[ imvCounters_m.overRangeIdx ].slope     = slope;

	imvCounters_m.overRangeIdx = (imvCounters_m.overRangeIdx + 1) % MAX_TO_AVG;
}


// ********************************************************************
// FUNCTION:
//
//		StoreEndpointAvgAbsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store common endpoint average absorbance data intermediate values for wavelength i 
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

void StoreEndpointAvgAbsImv( unsigned char analyteType, unsigned char i, EndpointAlgRecord_t *endpointAlgPtr, AvgAbs_t *avgAbs )
{
	unsigned char	j;

	float			flatnessToggle;
	float			flatnessRatioLimit;
	float			flatnessAbsoluteLimit;

	const AvgAbsImvTypes_t	avgAbsImvTypes[MAX_TO_AVG] = 
	{
		{
			IMV_WL_0_BLANKED_START_PASS,
			IMV_WL_0_BLANKED_AVG,
			IMV_WL_0_BLANKED_MIN,
			IMV_WL_0_BLANKED_MAX,

			IMV_FLATNESS_TOGGLE,
			IMV_FLATNESS_RATIO_LIMIT,
			IMV_FLATNESS_ABSOLUTE_LIMIT,
			IMV_WL_0_FLATNESS,
			IMV_WL_0_FLATNESS_AVG,

			IMV_WL_0_OVER_RANGE_START_PASS,	
			IMV_WL_0_OVER_RANGE_END_PASS,	
			IMV_WL_0_OVER_RANGE_LIMIT,		
			IMV_WL_0_OVER_RANGE_SLOPE,
			
			IMV_WL_0_AVG_TEST,
			IMV_WL_0_AVG_BLANK,
		},

		{
			IMV_WL_1_BLANKED_START_PASS,
			IMV_WL_1_BLANKED_AVG,
			IMV_WL_1_BLANKED_MIN,
			IMV_WL_1_BLANKED_MAX,

			IMV_FLATNESS_TOGGLE,
			IMV_FLATNESS_RATIO_LIMIT,
			IMV_FLATNESS_ABSOLUTE_LIMIT,
			IMV_WL_1_FLATNESS,
			IMV_WL_1_FLATNESS_AVG,

			IMV_WL_1_OVER_RANGE_START_PASS,	
			IMV_WL_1_OVER_RANGE_END_PASS,	
			IMV_WL_1_OVER_RANGE_LIMIT,		
			IMV_WL_1_OVER_RANGE_SLOPE,
					
			IMV_WL_1_AVG_TEST,
			IMV_WL_1_AVG_BLANK,
		},
	};


	// store endpoint bead mix params

	StoreBeadMixImv( analyteType );
	

	// store avg abs endpoint data

	for ( j = 0; j <= endpointAlgPtr->endpointAvgRecord.avgEndPass - endpointAlgPtr->endpointAvgRecord.avgStartPass; j++ )
	{ 
		StoreIntermediateValue( analyteType, avgAbsImvTypes[i].blankedStartPass, IMV_INCR_SET_METHOD, avgAbs[i].blankedAbs[j] );
	}

	imvCounters_m.nextImvIdx = 0;

	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].blankedAvg,  IMV_DIRECT_SET_METHOD, avgAbs[i].avg );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].blankedMin,  IMV_DIRECT_SET_METHOD, avgAbs[i].min );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].blankedMax,  IMV_DIRECT_SET_METHOD, avgAbs[i].max );

	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].avgAnalyte,  IMV_DIRECT_SET_METHOD, avgAbs[i].avgAnalyte );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].avgBlank,    IMV_DIRECT_SET_METHOD, avgAbs[i].avgAnalyteBlank );


	// store endpoint flatness limits and flatness values

	flatnessToggle = endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessToggle;

	if ( (flatnessToggle != NOVAL_NEG_FLOAT) && (flatnessToggle != NOVAL_POS_FLOAT) )
	{ 	
		StoreIntermediateValue( analyteType, avgAbsImvTypes[i].flatnessToggle, IMV_DIRECT_SET_METHOD, flatnessToggle );
	}


	flatnessRatioLimit = endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessRatioLimit;

	if ( (flatnessRatioLimit != NOVAL_NEG_FLOAT) && (flatnessRatioLimit != NOVAL_POS_FLOAT) )
	{ 	
		StoreIntermediateValue( analyteType, avgAbsImvTypes[i].flatnessRatioLimit, IMV_DIRECT_SET_METHOD, flatnessRatioLimit );
	}


	flatnessAbsoluteLimit = endpointAlgPtr->endpointAvgRecord.avgParams[i].flatnessAbsoluteLimit;

	if ( (flatnessAbsoluteLimit != NOVAL_NEG_FLOAT) && (flatnessAbsoluteLimit != NOVAL_POS_FLOAT) )
	{ 	
		StoreIntermediateValue( analyteType, avgAbsImvTypes[i].flatnessAbsoluteLimit, IMV_DIRECT_SET_METHOD, flatnessAbsoluteLimit );
	}


	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].flatness,    IMV_DIRECT_SET_METHOD, flatnessParams_m[i].flatness );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].flatnessAvg, IMV_DIRECT_SET_METHOD, flatnessParams_m[i].flatnessAvg );


	// store endpoint over system range limits and values

	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].overRangeStartPass, IMV_DIRECT_SET_METHOD, overSystemRangeParams_m[i].startPass );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].overRangeEndPass,   IMV_DIRECT_SET_METHOD, overSystemRangeParams_m[i].endPass );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].overRangeLimit,     IMV_DIRECT_SET_METHOD, overSystemRangeParams_m[i].limit );
	StoreIntermediateValue( analyteType, avgAbsImvTypes[i].overRangeSlope,     IMV_DIRECT_SET_METHOD, overSystemRangeParams_m[i].slope );
}


// ********************************************************************
// FUNCTION:
//
//		SetImvBfdCorrectedParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set bfd corrected avg or rate params checked used by rate and endpoint algs 
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

void SetImvBfdCorrectedParams( float bfdCorrectedAvgOrRate )
{
	bfdCorrectedParams_m[ imvCounters_m.bfdCorrectedIdx ].bfdCorrectedAvgOrRate = bfdCorrectedAvgOrRate;

	imvCounters_m.bfdCorrectedIdx = (imvCounters_m.bfdCorrectedIdx + 1) % MAX_BFD_CORRECTED;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvWl1BfdCorrected() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set bfd corrected average for 2 wavelength endpoint alg, wavelength index 1 
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

void SetImvWl1BfdCorrected( AvgAbs_t *avgAbs, EndpointAlgRecord_t *endpointAlgPtr )
{
	// set value for IMV_WL_1_CORRECTED_AVG bfd corrected average

	if  ( endpointAlgPtr->endpointAvgRecord.numToAvg == MAX_TO_AVG )
	{
		BfdCalAdjust(avgAbs[WL_1_CORRECTED_AVG_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[WL_1_CORRECTED_AVG_IDX]);
	}
}


// ********************************************************************
// FUNCTION:
//
//		StoreBfdCorrectedImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store bfd corrected avg or rate intermediate values 
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

void StoreBfdCorrectedImv( unsigned char analyteType )
{
	unsigned	i;

	const BfdCorrectedImvTypes_t	bfdCorrectedImvTypes[ MAX_BFD_CORRECTED ] = 
	{
		{
			IMV_WL_0_CORRECTED_AVG,				
		},

		{
			IMV_WL_1_CORRECTED_AVG,
		},
	};

	// store bfd corrected avg or rate 

	for ( i = 0; i < MAX_BFD_CORRECTED; i++ )
	{
		StoreIntermediateValue( analyteType, bfdCorrectedImvTypes[i].bfdCorrected, IMV_DIRECT_SET_METHOD, bfdCorrectedParams_m[i].bfdCorrectedAvgOrRate );
	}

	imvCounters_m.bfdCorrectedIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvBarcodeParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set barcode params used by rate and endpoint algs 
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

void SetImvBarcodeParams( float barcodeParam  )
{
	barcodeParams_m[ imvCounters_m.barcodeIdx ].barcodeParam = barcodeParam;

	imvCounters_m.barcodeIdx++;
}


// ********************************************************************
// FUNCTION:
//
//		SetPatientTypeBarcodeParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set patient type barcode params used in endpoint algs 
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

void SetImvPatientTypeBarcodeParams( unsigned char analyteType  )
{
	unsigned	i;

	// DOG_SLOPE_IDX:       0 -- IMV_BARCODE_FACTOR_1 
	// DOG_INTERCEPT_IDX:   1 -- IMV_BARCODE_FACTOR_2
	// CAT_SLOPE_IDX:       2 -- IMV_BARCODE_FACTOR_3 
	// CAT_INTERCEPT_IDX:   3 -- IMV_BARCODE_FACTOR_4 
	// OTHER_SLOPE_IDX:     4 -- IMV_BARCODE_FACTOR_5 
	// OTHER_INTERCEPT_IDX: 5 -- IMV_BARCODE_FACTOR_6
	// HORSE_SLOPE_IDX:     6 -- IMV_BARCODE_FACTOR_7 
	// HORSE_INTERCEPT_IDX: 7 -- IMV_BARCODE_FACTOR_8

	for ( i = 0; i < 8; i++ )	// DOG SLOPE = 0, 
	{
		GetBarcodeParameter( analyteType, i );
	}
}


// ********************************************************************
// FUNCTION:
//
//		StoreBarcodeParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store barcode parameters intermediate values 
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

void StoreBarcodeParamsImv( unsigned char analyteType )
{
	unsigned	i;

	const BarcodeImvTypes_t	barcodeImvTypes[ MAX_BARCODE_PARAMS ] = 
	{
		{
			IMV_BARCODE_FACTOR_1,		// typically SLOPE				
		},

		{
			IMV_BARCODE_FACTOR_2,		// typically INT
		},

		{
			IMV_BARCODE_FACTOR_3,				
		},

		{
			IMV_BARCODE_FACTOR_4,
		},

		{
			IMV_BARCODE_FACTOR_5,				
		},

		{
			IMV_BARCODE_FACTOR_6,
		},

		{
			IMV_BARCODE_FACTOR_7,				
		},

		{
			IMV_BARCODE_FACTOR_8,
		},

		{
			IMV_NOT_ENABLED,				
		},

		{
			IMV_NOT_ENABLED,
		},
	};

	// store bfd corrected avg or rate 

	for ( i = 0; i < imvCounters_m.barcodeIdx; i++ )
	{
		StoreIntermediateValue( analyteType, barcodeImvTypes[i].barcodeParam, IMV_DIRECT_SET_METHOD, barcodeParams_m[i].barcodeParam );
	}

	imvCounters_m.barcodeIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvEndpointDualWavelengthResults() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set dual wavelength idx 0 and idx 1 results intermediate values 
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

void SetImvEndpointDualWavelengthResults( float wl0Result, float wl1Result )
{
	endpointDualWl_m.wl0Result = wl0Result;
	endpointDualWl_m.wl1Result = wl1Result;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvEndpointDualWavelengthResultCompare() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set dual wavelength results comparison intermediate value 
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

void SetImvEndpointDualWavelengthResultCompare( float wlCompare )
{
	endpointDualWl_m.wlCompare = wlCompare;
}


// ********************************************************************
// FUNCTION:
//
//		StoreEndpointDualWavelengthResultsCompareImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store endpoint dual wavelength results and compare intermediate values 
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

void StoreEndpointDualWavelengthResultsCompareImv( unsigned char analyteType )
{
	StoreIntermediateValue( analyteType, IMV_WL_0_RESULT, IMV_DIRECT_SET_METHOD, endpointDualWl_m.wl0Result );
	StoreIntermediateValue( analyteType, IMV_WL_1_RESULT, IMV_DIRECT_SET_METHOD, endpointDualWl_m.wl1Result );

	StoreIntermediateValue( analyteType, IMV_WL_COMP_RESULT_RATIO, IMV_DIRECT_SET_METHOD, endpointDualWl_m.wlCompare );
}


// ********************************************************************
// FUNCTION:
//
//		StoreEndpointResultCalcParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store endpoint results calculation params intermediate values 
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

void StoreEndpointResultCalcParamsImv( unsigned char analyteType )
{
	StoreStartAbsImv( analyteType );

	StoreBfdCorrectedImv( analyteType );

	StoreBarcodeParamsImv( analyteType);

	StoreEndpointDualWavelengthResultsCompareImv( analyteType );
}


// ********************************************************************
// FUNCTION:
//
//		SetImvRateCompareWindowAbs() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set rate algorithm window abs compare value for selected pass index
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

void SetImvRateCompareWindowAbs( unsigned char windowAbsIdx, float windowAbs )
{
	rateWindowAbs_m[ windowAbsIdx ] = windowAbs;

	rateWindow_m = (float) (windowAbsIdx + 1);
}


// ********************************************************************
// FUNCTION:
//
//		StoreRateSlopeWindowLimitsParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store standard rate algorithm slope, window selection, limits parameters
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

void StoreRateSlopeWindowLimitsParamsImv( unsigned char analyteType, RateAlgRecord_t *rateAlgPtr, SlopeParams_t *slopeParamsPtr )
{
	StoreBeadMixImv( analyteType );

	StoreStartAbsImv( analyteType );

	// store standard rate constants / limits

	StoreIntermediateValue( analyteType, IMV_NOISE_TOGGLE,              IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.rateNoiseToggle );
	StoreIntermediateValue( analyteType, IMV_NOISE_RATIO_LIMIT,         IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.rateNoiseRatioLimit );
	StoreIntermediateValue( analyteType, IMV_NOISE_ABSOLUTE_LIMIT,      IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.rateNoiseAbsoluteLimit );

	StoreIntermediateValue( analyteType, IMV_NOISE_RATIO_LIMIT_LOW,     IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.slopeRateCorrectionLimit );
	StoreIntermediateValue( analyteType, IMV_RATE_CORR_NOISE_MULT,     	IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.slopeRateCorrectionMultiplier );
	StoreIntermediateValue( analyteType, IMV_RATE_CORR_RATE_LIMIT,     	IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.slopeRateCorrectionRateLimit );
	StoreIntermediateValue( analyteType, IMV_RATE_CORR_RATE_MULT,     	IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.slopeRateCorrectionRateMultiplier );

	StoreIntermediateValue( analyteType, IMV_WINDOW_THRESHOLD_MULT,     IMV_DIRECT_SET_METHOD, rateAlgPtr->windowSelectRecord.derivedSelectThresholdMultiplier );
	StoreIntermediateValue( analyteType, IMV_WINDOW_ABS_THRESHOLD,      IMV_DIRECT_SET_METHOD, rateAlgPtr->windowSelectRecord.selectParams[0].selectAbsorbanceThreshold );
	StoreIntermediateValue( analyteType, IMV_END_ABS_THRESHOLD,         IMV_DIRECT_SET_METHOD, rateAlgPtr->windowSelectRecord.postEndAbsLimit );
	StoreIntermediateValue( analyteType, IMV_PRE_END_ABS_LIMIT,         IMV_DIRECT_SET_METHOD, rateAlgPtr->windowSelectRecord.preEndAbsLimit );
	StoreIntermediateValue( analyteType, IMV_RATE_WINDOW_USED,          IMV_DIRECT_SET_METHOD, rateWindow_m );

	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_RATIO_LIMIT,    IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.falseRateRatioLimit );
	StoreIntermediateValue( analyteType, IMV_FALSE_RATE_ABSOLUTE_LIMIT, IMV_DIRECT_SET_METHOD, rateAlgPtr->rateCalcRecord.falseRateAbsoluteLimit );

	StoreIntermediateValue( analyteType, IMV_SLOPE_NUM_POINTS,          IMV_DIRECT_SET_METHOD, (float) slopeParamsPtr->numPoints );
	StoreIntermediateValue( analyteType, IMV_SLOPE_RATE,           		IMV_DIRECT_SET_METHOD, slopeParamsPtr->slope );
	StoreIntermediateValue( analyteType, IMV_SLOPE_STD_ERROR,           IMV_DIRECT_SET_METHOD, slopeParamsPtr->stdError );
	StoreIntermediateValue( analyteType, IMV_SLOPE_FALSE_RATE,          IMV_DIRECT_SET_METHOD, slopeParamsPtr->falseRate );

	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_0,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[0]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_1,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[1]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_2,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[2]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_3,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[3]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_4,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[4]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_5,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[5]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_6,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[6]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_7,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[7]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_8,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[8]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_9,       IMV_DIRECT_SET_METHOD, rateWindowAbs_m[9]  );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_10,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[10] );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_11,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[11] );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_12,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[12] );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_13,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[13] );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_14,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[14] );
	StoreIntermediateValue( analyteType, IMV_ABSORBANCE_WINDOW_15,      IMV_DIRECT_SET_METHOD, rateWindowAbs_m[15] );
}


// ********************************************************************
// FUNCTION:
//
//		SetImvRateTemperatureCorrectionParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set rate temperature correction parameters
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

void SetImvRateTemperatureCorrectionParams( float arrhTemperatureCorrectionFactor, float rotorAverageTemp, float temperatureCorrectionExp, float eToCorrectionExp )
 
{
	rateTemperatureCorrection_m.arrhFactor        = arrhTemperatureCorrectionFactor;
	rateTemperatureCorrection_m.rotorAverageTemp  = rotorAverageTemp;
	rateTemperatureCorrection_m.tempCorrectionExp = temperatureCorrectionExp;
	rateTemperatureCorrection_m.eToCorrectionExp  = eToCorrectionExp;
}


// ********************************************************************
// FUNCTION:
//
//		StoreRateTemperatureCorrectionParamsIMV() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store rate temperature correction parameters
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

void StoreRateTemperatureCorrectionParamsIMV( unsigned char analyteType )
 
{
	StoreIntermediateValue( analyteType, IMV_ARRH_FACTOR,            IMV_DIRECT_SET_METHOD, rateTemperatureCorrection_m.arrhFactor );
	StoreIntermediateValue( analyteType, IMV_AVG_SENSOR_TEMPERATURE, IMV_DIRECT_SET_METHOD, rateTemperatureCorrection_m.rotorAverageTemp );
	StoreIntermediateValue( analyteType, IMV_RATE_EXPONENT_VALUE,    IMV_DIRECT_SET_METHOD, rateTemperatureCorrection_m.tempCorrectionExp );
	StoreIntermediateValue( analyteType, IMV_RATE_E_TO_THE_X,        IMV_DIRECT_SET_METHOD, rateTemperatureCorrection_m.eToCorrectionExp );
}


// ********************************************************************
// FUNCTION:
//
//		StoreRateResultCalcParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store generic rate result calculation related parameters
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

void StoreRateResultCalcParamsImv( unsigned char analyteType )
{
	StoreRateTemperatureCorrectionParamsIMV( analyteType );

	StoreBfdCorrectedImv( analyteType );

	StoreBarcodeParamsImv( analyteType );
}


// ********************************************************************
// FUNCTION:
//
//		Store2CuvAlgParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store generic 2 cuvette algorithm calculation related parameters
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

void Store2CuvAlgParamsImv( unsigned char analyteType, unsigned char analyteTypeCuv2, unsigned long analyteFlags, float analyteResult1, float analyteResult2, float analyteResultDiff, float blendFactor, float inconsistentResultDiffLimitUsed, Calc2CuvRecord_t *calc2CuvPtr )
{
	StoreAlgParagraphNumImv( analyteType, 14.0 );	// for end of 2 cuvette algorithm


	// set cuvette blend ratio based on 2 cuvette algorithm type

	switch ( calc2CuvPtr->calc2CuvType ) 
	{
		case CALC_2_CUV_BLEND:
			if ( (analyteFlags & (BLEND_CUVETTE_1_USED | BLEND_CUVETTE_2_USED)) == BLEND_CUVETTE_1_USED )
			{
				blendFactor = 1.0;	
			}
			else if ( (analyteFlags & (BLEND_CUVETTE_1_USED | BLEND_CUVETTE_2_USED)) == BLEND_CUVETTE_2_USED )
			{
				blendFactor = 0.0;	
			}
			break;

		case CALC_2_CUV_BLEND_CUV_2:
		case CALC_2_CUV_BLEND_ZONES_CUV_2:
			if ( (analyteFlags & (BLEND_CUVETTE_1_USED | BLEND_CUVETTE_2_USED)) == BLEND_CUVETTE_2_USED )
			{
				blendFactor = 1.0;	
			}
			else if ( (analyteFlags & (BLEND_CUVETTE_1_USED | BLEND_CUVETTE_2_USED)) == BLEND_CUVETTE_1_USED )
			{
				blendFactor = 0.0;	
			}
			break;

		default:
			break;
	}


	// store intermediate values for 2 cuvette chemistry analyte type

	StoreIntermediateValue( analyteType, IMV_2_CUV_TYPE,              IMV_DIRECT_SET_METHOD, (float) calc2CuvPtr->calc2CuvType );
	StoreIntermediateValue( analyteType, IMV_2_CUV_RESULT_1,          IMV_DIRECT_SET_METHOD, analyteResult1 );
	StoreIntermediateValue( analyteType, IMV_2_CUV_RESULT_2,          IMV_DIRECT_SET_METHOD, analyteResult2 );

	StoreIntermediateValue( analyteType, IMV_2_CUV_RESULT_DIFF,       IMV_DIRECT_SET_METHOD, analyteResultDiff );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_LIMIT_CONST,  IMV_DIRECT_SET_METHOD, calc2CuvPtr->inconsistentResultDiffLimit );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_CHK_MASK_MSB, IMV_DIRECT_SET_METHOD, (float) ( calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask >> 16 ) );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_CHK_MASK_LSB, IMV_DIRECT_SET_METHOD, (float) ( calc2CuvPtr->avg1CuvOkCheckData.cuvOkSuppressionMask & 0x0000ffff) );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_CHK_TOGGLE,   IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckToggle );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_CHK_MAX,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckMax );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_LIMIT_SCALE,  IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.resultDiffLimitScale );
	StoreIntermediateValue( analyteType, IMV_2_CUV_DIFF_LIMIT_USED,   IMV_DIRECT_SET_METHOD, inconsistentResultDiffLimitUsed );

	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_MASK_MSB,    IMV_DIRECT_SET_METHOD, (float) ( calc2CuvPtr->blend1CuvOkCheckData.cuvSuppressionMask >> 16 ) );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_MASK_LSB,    IMV_DIRECT_SET_METHOD, (float) ( calc2CuvPtr->blend1CuvOkCheckData.cuvSuppressionMask & 0x0000ffff ) );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_ZONE_1,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->blend1CuvOkCheckData.zone1Limit );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_ZONE_2,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->blend1CuvOkCheckData.zone2Limit );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_START_LIMIT, IMV_DIRECT_SET_METHOD, calc2CuvPtr->startCuvBlendLimit );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_END_LIMIT,   IMV_DIRECT_SET_METHOD, calc2CuvPtr->endCuvBlendLimit );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND,             IMV_DIRECT_SET_METHOD, blendFactor );
	StoreIntermediateValue( analyteType, IMV_2_CUV_BLEND_RANGE,       IMV_DIRECT_SET_METHOD, calc2CuvPtr->endCuvBlendLimit - calc2CuvPtr->startCuvBlendLimit );


	// store intermediate values for CHEM_2 cuvette analyteType	as selected

	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_TYPE,              IMV_DIRECT_SET_METHOD, (float) calc2CuvPtr->calc2CuvType );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_RESULT_1,          IMV_DIRECT_SET_METHOD, analyteResult1 );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_RESULT_2,          IMV_DIRECT_SET_METHOD, analyteResult2 );

	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_RESULT_DIFF,       IMV_DIRECT_SET_METHOD, analyteResultDiff );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_DIFF_LIMIT_CONST,  IMV_DIRECT_SET_METHOD, calc2CuvPtr->inconsistentResultDiffLimit );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_DIFF_CHK_TOGGLE,   IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckToggle );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_DIFF_CHK_MAX,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.inconsistentCheckMax );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_DIFF_LIMIT_SCALE,  IMV_DIRECT_SET_METHOD, calc2CuvPtr->avg1CuvOkCheckData.resultDiffLimitScale );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_DIFF_LIMIT_USED,   IMV_DIRECT_SET_METHOD, inconsistentResultDiffLimitUsed );

	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND_ZONE_1,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->blend1CuvOkCheckData.zone1Limit );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND_ZONE_2,      IMV_DIRECT_SET_METHOD, calc2CuvPtr->blend1CuvOkCheckData.zone2Limit );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND_START_LIMIT, IMV_DIRECT_SET_METHOD, calc2CuvPtr->startCuvBlendLimit );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND_END_LIMIT,   IMV_DIRECT_SET_METHOD, calc2CuvPtr->endCuvBlendLimit );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND,             IMV_DIRECT_SET_METHOD, blendFactor );
	StoreIntermediateValue( analyteTypeCuv2, IMV_2_CUV_BLEND_RANGE,       IMV_DIRECT_SET_METHOD, calc2CuvPtr->endCuvBlendLimit - calc2CuvPtr->startCuvBlendLimit );
}


// ********************************************************************
// FUNCTION:
//
//		SetImvSystemCheckTransmissionParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set SYSTEM_CHECK intermediate value transmission parameters
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

void SetImvSystemCheckTransmissionParams( unsigned i, float transmission )
{
	transmissionSystemCheck_m[i] = transmission;
}


// ********************************************************************
// FUNCTION:
//
//		StoreSystemCheckTransmissionParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store SYSTEM_CHECK intermediate value transmission parameters
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

void StoreSystemCheckTransmissionParamsImv( void )
{
	unsigned 	i;

	transmissionSystemCheck_m[SYSCHK_RATIO_06_340] = transmissionSystemCheck_m[SYSCHK_TRANS_06_340] / transmissionSystemCheck_m[SYSCHK_TRANS_OPT_340];	
	transmissionSystemCheck_m[SYSCHK_RATIO_06_405] = transmissionSystemCheck_m[SYSCHK_TRANS_06_405] / transmissionSystemCheck_m[SYSCHK_TRANS_OPT_405];	
	transmissionSystemCheck_m[SYSCHK_RATIO_06_467] = transmissionSystemCheck_m[SYSCHK_TRANS_06_467] / transmissionSystemCheck_m[SYSCHK_TRANS_OPT_467];	
	transmissionSystemCheck_m[SYSCHK_RATIO_11_340] = transmissionSystemCheck_m[SYSCHK_TRANS_11_340] / transmissionSystemCheck_m[SYSCHK_TRANS_OPT_340];	
	transmissionSystemCheck_m[SYSCHK_RATIO_28_340] = transmissionSystemCheck_m[SYSCHK_TRANS_28_340] / transmissionSystemCheck_m[SYSCHK_TRANS_OPT_340];	

	for ( i = 0; i < MAX_SYSCHK_TRANSMISSIONS; i++ )
	{
		StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_TRANSMISSION_START, IMV_INCR_SET_METHOD, transmissionSystemCheck_m[i] );
	}

	imvCounters_m.nextImvIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		StoreSystemCheckAbsorbanceParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store SYSTEM_CHECK intermediate value transmission parameters
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

void StoreSystemCheckAbsorbanceParamsImv( float *systemCheckAbs, float absCuv06W405, float absCuv06W467 )
{
	unsigned 	i;
	float		absSystemCheck[ MAX_SYSCHK_ABSORBANCES ];

	absSystemCheck[0] = systemCheckAbs[0];	
	absSystemCheck[1] = absCuv06W405;	
	absSystemCheck[2] = absCuv06W467;	
	absSystemCheck[3] = systemCheckAbs[1];	
	absSystemCheck[4] = systemCheckAbs[2];	

	for ( i = 0; i < MAX_SYSCHK_ABSORBANCES; i++ )
	{
		StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_ABSORBANCE_START, IMV_INCR_SET_METHOD, absSystemCheck[i] );
	}

	imvCounters_m.nextImvIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		StoreIqcAbsRatioParamsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store IQC intermediate value absorbance / ratio parameters
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

void StoreIqcAbsRatioParamsImv( float *iqcACuvetteAbs, float *iqcBCuvetteAbs, float *iqcAbsRatio, float iqcAbsRatioAvg )
{
	unsigned 	i;

	// IQC A absorbances

	for ( i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++ )
	{
		StoreIntermediateValue( IQC, IMV_IQC_ABS_A_START, IMV_INCR_SET_METHOD, iqcACuvetteAbs[i] );
	}

	imvCounters_m.nextImvIdx = 0;


	// IQC B absorbances

	for ( i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++ )
	{
		StoreIntermediateValue( IQC, IMV_IQC_ABS_B_START, IMV_INCR_SET_METHOD, iqcBCuvetteAbs[i] );
	}

	imvCounters_m.nextImvIdx = 0;


	// IQC absorbance ratios

	for ( i = 0; i < MAX_IQC_LEVEL2_CHECKS; i++ )
	{
		StoreIntermediateValue( IQC, IMV_IQC_RATIO_START, IMV_INCR_SET_METHOD, iqcAbsRatio[i] );
	}

	imvCounters_m.nextImvIdx = 0;


	// IQC absorbance ratio average
	
	StoreIntermediateValue( IQC, IMV_IQC_RATIO_AVG, IMV_DIRECT_SET_METHOD, iqcAbsRatioAvg );
}


// ********************************************************************
// FUNCTION:
//
//		StoreSampleBlankStartAbsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set SAMPLE_BLANK starting absorbance parameters intermediate values
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

void StoreSampleBlankStartAbsImv( void )
{
	unsigned	i;

	StoreIntermediateValue( SAMPLE_BLANK, IMV_TEST_START_ABS_LIMIT, IMV_DIRECT_SET_METHOD, startAbsParams_m[0].startAbsLimit );

	// store starting abs value for 8 wavelength pairs 630, 600, 550, 515, 500, 467, 405, 340

	for ( i = 0; i < imvCounters_m.startAbsIdx; i++ )
	{
		StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_START_ABS_START, IMV_DECR_SET_METHOD, startAbsParams_m[i].startAbsSample );
	}

	imvCounters_m.nextImvIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		SetImvSampleBlankAvgAbsParams() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set SAMPLE_BLANK average absorbance parameters intermediate values
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

void SetImvSampleBlankAvgAbsParams( unsigned i, AvgAbs_t *avgAbs )
{
	avgAbsSampleBlank_m[i]     = avgAbs->avgAnalyteBlank;
	avgAbsSampleBlank_m[i + 3] = avgAbs->avgAnalyte;
	avgAbsSampleBlank_m[i + 6] = avgAbs->avg;
}


// ********************************************************************
// FUNCTION:
//
//		StoreSampleBlankAvgAbsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store SAMPLE_BLANK average absorbance parameters intermediate values
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

void StoreSampleBlankAvgAbsImv( void )
{
	unsigned	i;

	// store average abs value for cuv 6 blank, sample blank cuvette, corrected (blanked) endogenous params for 340, 405, 467

	for ( i = 0; i < MAX_SB_AVG_ABS; i++ )
	{
		StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_ABS_AVG_START, IMV_INCR_SET_METHOD, avgAbsSampleBlank_m[i] );
	}

	imvCounters_m.nextImvIdx = 0;
}


// ********************************************************************
// FUNCTION:
//
//		StoreHdlStartAbsFlatnessParmsImv() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Store HDL_1 or HDL_2 non-generic starting abs and abs flatness params
//		intermediate values
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

void StoreHdlStartAbsFlatnessParmsImv( unsigned char analyteType, EndpointAlgRecord_t* algPtr )
{
	// store starting abs and limit for primary 550/630 & primary blank 550/630

	StoreIntermediateValue( analyteType, IMV_STARTING_ABS_A,       IMV_DIRECT_SET_METHOD, startAbsParams_m[0].startAbsSample );
	StoreIntermediateValue( analyteType, IMV_TEST_START_ABS_LIMIT, IMV_DIRECT_SET_METHOD, startAbsParams_m[0].startAbsLimit );
	StoreIntermediateValue( analyteType, IMV_STARTING_ABS_B_BLANK, IMV_DIRECT_SET_METHOD, startAbsParams_m[1].startAbsSample );

	// store abs flatness params for derived and prim 550/630

	StoreIntermediateValue( analyteType, IMV_FLATNESS_TOGGLE,          IMV_DIRECT_SET_METHOD, algPtr->endpointAvgRecord.avgParams[0].flatnessToggle );
	StoreIntermediateValue( analyteType, IMV_FLATNESS_RATIO_LIMIT,     IMV_DIRECT_SET_METHOD, algPtr->endpointAvgRecord.avgParams[0].flatnessRatioLimit );
	StoreIntermediateValue( analyteType, IMV_FLATNESS_ABSOLUTE_LIMIT,  IMV_DIRECT_SET_METHOD, algPtr->endpointAvgRecord.avgParams[0].flatnessAbsoluteLimit );

	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_FLATNESS,     IMV_DIRECT_SET_METHOD, flatnessParams_m[0].flatness );
	StoreIntermediateValue( analyteType, IMV_HDL_DERIVED_FLATNESS_AVG, IMV_DIRECT_SET_METHOD, flatnessParams_m[0].flatnessAvg );
	StoreIntermediateValue( analyteType, IMV_WL_0_FLATNESS,            IMV_DIRECT_SET_METHOD, flatnessParams_m[1].flatness );
	StoreIntermediateValue( analyteType, IMV_WL_0_FLATNESS_AVG,        IMV_DIRECT_SET_METHOD, flatnessParams_m[1].flatnessAvg );
}

 
