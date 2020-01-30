// ********************************************************************
//
// FILENAME:  
// 
// 		TestCuvetteAbs.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Calculate rotor 1 test cuvette absorbances for RAD file algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "SystemCheck.h"
#include "Statistics.h"


#ifdef UNDER_CE
#include "LogErr.h"
#endif


// rotor 1 test sample types for abs calcs for rotor sample passes

#define OPTICAL_BLANK_0		0
#define OPTICAL_BLANK_1		1
#define OPTICAL_BLANK_2		2
#define OPTICAL_BLANK_3		3
#define OPTICAL_BLANK_4		4

#define SAMPLE_BLANK_0		10
#define SAMPLE_BLANK_1		11
#define SAMPLE_BLANK_2		12
#define SAMPLE_BLANK_3		13
#define SAMPLE_BLANK_4		14

#define CUV_00_ABS			98
#define CUV_01_ABS			99
#define CUV_02_ABS			100
#define CUV_03_ABS			101
#define CUV_04_ABS			102
#define CUV_05_ABS			103
#define CUV_06_ABS			104
#define CUV_07_ABS			105
#define CUV_08_ABS			106
#define CUV_09_ABS			107
#define CUV_10_ABS			108
#define CUV_11_ABS			109
#define CUV_12_ABS			110
#define CUV_13_ABS			111
#define CUV_14_ABS			112
#define CUV_15_ABS			113
#define CUV_16_ABS			114
#define CUV_17_ABS			115
#define CUV_18_ABS			116
#define CUV_19_ABS			117
#define CUV_20_ABS			118
#define CUV_21_ABS			119
#define CUV_22_ABS			120
#define CUV_23_ABS			121
#define CUV_24_ABS			122
#define CUV_25_ABS			123
#define CUV_26_ABS			124
#define CUV_27_ABS			125
#define CUV_28_ABS			126
#define CUV_29_ABS			127

#define CUV_ABS_DONE		0xff

#define NUM_TEST_PASSES		13


// DAC wavelength CV test data

#define	ROTOR_1_OPTICAL_DAC_CV_LIMIT	3.00


// structure for sampleType & sampleTypeOpticalBlank used in rotor 1 abs test

typedef struct
{
	unsigned char	sampleTypeCuv;
	unsigned char	sampleTypeOpticalBlank;
	unsigned char	cuvetteNumber;	
} sampleTypeParam_t;


// ********************************************************************
// FUNCTION:
//
//		Rotor1DacWavelengthCvNoiseCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check manufacturing test Rotor 1 open hole readings for cuvette 29 used to set 
//		DAC wavelength trim	using coefficient of variance (CV) for noise 
//		in each used wavelength transmission
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

void Rotor1DacWavelengthCvNoiseCheck()
{
	unsigned		i;
	unsigned		j;
	unsigned short	wavelengthCvFlags;
	float			opticalDacCv[NUM_WAVELENGTHS];
	float			opticalDacAdc[NUM_OPTICAL_DACS_FLASHES];

	wavelengthCvFlags = 0;

	// process all optical DAC wavelengths for rotor 1 test

	for (i = 0; i <= WL_WHITE; i++)
	{
		// calc optical transmission to 850 reference for each wavelength at optical DAC cuvette & skip 1st flash

		for (j = 0; j < NUM_OPTICAL_DACS_FLASHES; j++)
		{
			opticalDacAdc[j] = (float) rotorRawReadings_g->opticalDacs[j+1].rawFlashReadings[i];
		}
		
		opticalDacCv[i] = PercentCv(opticalDacAdc, NUM_OPTICAL_DACS_FLASHES);

		if (opticalDacCv[i] > ROTOR_1_OPTICAL_DAC_CV_LIMIT)
		{
			wavelengthCvFlags |= wavelengthErrorFlags_g[i];
		}
	}

	if (wavelengthCvFlags)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_WAVE_CV_UNREF);

#ifdef UNDER_CE

		LogError( "4214 - Flags  0x%4X - 340  %f - 405  %f - 467  %f - 500  %f - 515  %f - 550  %f - 600  %f - 630  %f - 850  %f - WHITE  %f", 
				  wavelengthCvFlags,
				  opticalDacCv[0],
				  opticalDacCv[1],
				  opticalDacCv[2],
				  opticalDacCv[3],
				  opticalDacCv[4],
				  opticalDacCv[5],
				  opticalDacCv[6],
				  opticalDacCv[7],
				  opticalDacCv[8],
				  opticalDacCv[9] );

#endif
	}
}


// ********************************************************************
// FUNCTION:
//
//		CalcTestCuvetteAbs() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate rotor 1 test cuvette absorbances for RAD file algorithm 
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

unsigned short CalcTestCuvetteAbs(void)
{
	unsigned char	i;
	unsigned		j;
	unsigned		k;
	unsigned		rotorErrorNumberIndex;
	unsigned char	cuvetteNumber;
	unsigned long	cuvTestError;

	// rotor 1 sampleType test table

	const sampleTypeParam_t		sampleTypesForTest[] = 
	{
		{CUV_01_ABS,		OPTICAL_BLANK_0,	1 },	
		{CUV_03_ABS,		OPTICAL_BLANK_0,	3 },	
		{CUV_08_ABS,		OPTICAL_BLANK_0,	8 },	
		{CUV_09_ABS,		OPTICAL_BLANK_0,	9 },	
		{CUV_10_ABS,		OPTICAL_BLANK_0,	10},	
		{SAMPLE_BLANK_0,	OPTICAL_BLANK_0,	26},
		{CUV_12_ABS,		OPTICAL_BLANK_1,	12},	
		{CUV_13_ABS,		OPTICAL_BLANK_1,	13},	
		{CUV_14_ABS,		OPTICAL_BLANK_1,	14},	
		{CUV_15_ABS,		OPTICAL_BLANK_1,	15},	
		{CUV_16_ABS,		OPTICAL_BLANK_1,	16},	
		{SAMPLE_BLANK_1,	OPTICAL_BLANK_1,	26},
		{CUV_17_ABS,		OPTICAL_BLANK_2,	17},	
		{CUV_18_ABS,		OPTICAL_BLANK_2,	18},	
		{CUV_19_ABS,		OPTICAL_BLANK_2,	19},	
		{CUV_20_ABS,		OPTICAL_BLANK_2,	20},	
		{CUV_21_ABS,		OPTICAL_BLANK_2,	21},	
		{SAMPLE_BLANK_2,	OPTICAL_BLANK_2,	26},
		{CUV_22_ABS,		OPTICAL_BLANK_3,	22},	
		{CUV_23_ABS,		OPTICAL_BLANK_3,	23},	
		{CUV_24_ABS,		OPTICAL_BLANK_3,	24},	
		{CUV_25_ABS,		OPTICAL_BLANK_3,	25},	
		{CUV_27_ABS,		OPTICAL_BLANK_3,	27},	
		{SAMPLE_BLANK_3,	OPTICAL_BLANK_3,	26},
		{CUV_04_ABS,		OPTICAL_BLANK_4,	4 },	
		{CUV_05_ABS,		OPTICAL_BLANK_4,	5 },	
		{CUV_11_ABS,		OPTICAL_BLANK_4,	11},	
		{CUV_28_ABS,		OPTICAL_BLANK_4,	28},	
		{CUV_06_ABS,		OPTICAL_BLANK_4,	6 },	
		{SAMPLE_BLANK_4,	OPTICAL_BLANK_4,	26},
			
		{CUV_ABS_DONE,		CUV_ABS_DONE,		CUV_ABS_DONE}
	};

	// rotor 1 wavelengthPair table for absorbance calcs
	const unsigned char	wavelengthPairsForTest[MAX_WP_850] = 
	{
		WP_340_850,
		WP_405_850,
		WP_467_850,
		WP_500_850,
		WP_515_850,
		WP_550_850,
		WP_600_850,
		WP_630_850,
		WP_850_850,
		WP_WHITE_850
	};


	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// invoke rotor 1 test to check DAC Wavelength CV within range for all ADC readings

	Rotor1DacWavelengthCvNoiseCheck();

	// invoke system checks allowed for rotor 1 to check for issues with DAC settings and offset levels

	CalcSystemCheck();

	// Calulate & store all absorbances for passes with sampleType wavelengths in RIS fileorder 

	for (i = 1; i <= NUM_TEST_PASSES; i++)
	{
		j = 0;

		while (sampleTypesForTest[j].sampleTypeCuv != CUV_ABS_DONE)
		{
			cuvetteNumber = sampleTypesForTest[j].cuvetteNumber;

			for (k = 0; k < MAX_WP_850; k++)
			{
				// reset error for each abs calculation

				cuvTestError = 0;

				CalcSampleAbsorbance(sampleTypesForTest[j].sampleTypeCuv, sampleTypesForTest[j].sampleTypeOpticalBlank, i, wavelengthPairsForTest[k], cuvettePathLengths_g[cuvetteNumber], cuvetteNumber, &cuvTestError);	
			}
			
			j++; 
		}
	}

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);


#ifndef UNDER_CE
	printf("Rotor 1 Cuvette Absorbance Test invoked\n");
#endif
}
