// ********************************************************************
//
// FILENAME:  
// 
// 		SystemCheck.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		SYSTEM_CHECK calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <math.h>				// required standard C library 

#include "CommonAlgFuncs.h"
#include "SharedFactory.h"
#include "SharedCal.h"
#include "Temperature.h"
#include "Statistics.h"


#define MAX_TRANSMISSION_CALCS	8
#define MAX_ABSORBANCE_CALCS	3
#define	SYSTEM_CHECK_PASS		1

#define	SYS_CHK_CUV6			6
#define	SYS_CHK_CUV11			11
#define	SYS_CHK_CUV28			28

#define ABS_CUV6				0
#define ABS_CUV11				1
#define ABS_CUV28				2

#define SYS_CHK_DIL_ABS_IDX		0


typedef struct
{
	unsigned char	*sampleType;
	unsigned char	wavelengthPair;	
} transmissionParam_t;


typedef struct
{
	unsigned char	*sampleType;
	unsigned char	cuvetteNumber;	
} absorbanceParam_t;


// ********************************************************************
// FUNCTION:
//
//		UpdateRotorFlashRangeMax() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Update flash range maximum for rotor run to be used by IQC algorithm
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

float UpdateRotorFlashRangeMax(float transmission, float flashRange, float flashRangeMax)
{
	if (transmission >= FLASH_RANGE_ERROR_TOGGLE)
	{
		if (flashRange > flashRangeMax)
		{
			flashRangeMax = flashRange;
		}
	}
	else
	{
		if ((flashRange * 10) > flashRangeMax)
		{
			flashRangeMax = flashRange * 10;
		}
	}

	return flashRangeMax;
}


// ********************************************************************
// FUNCTION:
//
//		SystemCuvetteChecks() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Perform common system cuvette checks (6,11,28) to be used by IQC algorithm
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

unsigned SystemCuvetteChecks(void)
{
	unsigned		i;
	unsigned char	sampleTypeDiluentCuv6;
	unsigned char	sampleTypeMixCuv11;
	unsigned char	sampleTypeFillCuv28;
	unsigned char	sampleTypeOpticalBlank;			
	unsigned long	systemCheckError;
	float			desiredPathLength;
	float			transmission;
	float			flashRange;
	float			flashRangeMax;
	float			diluentBlankAbs;
	float			sampleDiluentHighDeltaLimit;
	float			sampleDiluentToggle;
	float			correctedAbsCuv11;
	float			correctedAbsCuv28;
	float			sampleDiluentRatio;
	float			sampleDiluentDiff;
	float			systemCheckAbs[MAX_ABSORBANCE_CALCS];
	float			cuvette1Transmission;

	const	transmissionParam_t	transmissionParam[MAX_TRANSMISSION_CALCS] = 
	{
		{ &sampleTypeDiluentCuv6,  WP_340_850 },
		{ &sampleTypeDiluentCuv6,  WP_405_850 },
		{ &sampleTypeDiluentCuv6,  WP_467_850 },

		{ &sampleTypeOpticalBlank, WP_340_850 },
		{ &sampleTypeOpticalBlank, WP_405_850 },
		{ &sampleTypeOpticalBlank, WP_467_850 },

		{ &sampleTypeMixCuv11,     WP_340_850 },
		{ &sampleTypeFillCuv28,    WP_340_850 },
	};

	const	absorbanceParam_t	absorbanceParam[MAX_ABSORBANCE_CALCS] = 
	{
		{ &sampleTypeDiluentCuv6, SYS_CHK_CUV6 },
		{ &sampleTypeMixCuv11,    SYS_CHK_CUV11 },
		{ &sampleTypeFillCuv28,   SYS_CHK_CUV28 },
	};
	

	// IMV - SYSTEM_CHECK paragraph 1 store

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 1.0 );

	// end IMV


	// init system check algorithm related rotor error flags for no errors

	systemCheckError = 0;

	// set up common parameters from rotor ROC fle & SYSTEM_CHECK ROC file & results file

	sampleTypeDiluentCuv6  = rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].sampleTypeAnalyteBlank;	
	sampleTypeOpticalBlank = rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].sampleTypeOpticalBlank;	
	desiredPathLength	   = rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].analyteDesiredPathLength;

	sampleTypeMixCuv11     = systemCheckConfig_g.sampleTypeMixCuv11;	
	sampleTypeFillCuv28    = systemCheckConfig_g.sampleTypeFillCuv28;
	sampleDiluentToggle    = systemCheckConfig_g.sampleDiluentToggle;
	
	flashRangeMax = 0.0;		

	// update & save result for flash range max value for IQC algorithm processing

	for (i = 0; i < MAX_TRANSMISSION_CALCS; i++)
	{
		transmission = CalcSampleTransmission(*transmissionParam[i].sampleType, SYSTEM_CHECK_PASS, transmissionParam[i].wavelengthPair, &flashRange, &systemCheckError);


		// IMV - SYSTEM_CHECK set transmission params

		SetImvSystemCheckTransmissionParams( i, transmission );

		// end IMV


		if (ProcessSystemCalcErrors(systemCheckError))
		{
			return TRUE;
		}

		flashRangeMax = UpdateRotorFlashRangeMax(transmission, flashRange, flashRangeMax);
	}


	// IMV - store transmission params / SYSTEM_CHECK paragraph 2 store

	StoreSystemCheckTransmissionParamsImv( );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 2.0 );

	// end IMV


	rotorResults_g->rotorInformationResults.rotorFlashRangeMax = flashRangeMax;


	// IMV - SYSTEM_CHECK paragraph 3 store

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 3.0 );

	// end IMV


	// calc abs for cuv 6, cuv 11, cuv 28 at 340/850 for system diluent, mix, fill checks

	for (i = 0; i < MAX_ABSORBANCE_CALCS; i++)
	{
		systemCheckAbs[i] = CalcSampleAbsorbance(*absorbanceParam[i].sampleType, sampleTypeOpticalBlank, SYSTEM_CHECK_PASS, WP_340_850, desiredPathLength, absorbanceParam[i].cuvetteNumber, &systemCheckError);	

		if (ProcessSystemCalcErrors(systemCheckError))
		{
			return TRUE;
		}
	}


	// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 4 store

	StoreSystemCheckAbsorbanceParamsImv( systemCheckAbs, CalcSampleAbsorbance(sampleTypeDiluentCuv6, sampleTypeOpticalBlank, SYSTEM_CHECK_PASS, WP_405_850, desiredPathLength, SYS_CHK_CUV6, &systemCheckError), CalcSampleAbsorbance(sampleTypeDiluentCuv6, sampleTypeOpticalBlank, SYSTEM_CHECK_PASS, WP_467_850, desiredPathLength, SYS_CHK_CUV6, &systemCheckError) );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 4.0 );

	// end IMV


	// calc plastic corrected diluent blank abs for cuv 6 & check if too high versus ROC file bar code limit

	diluentBlankAbs = systemCheckAbs[ABS_CUV6] - systemCheckConfig_g.plasticAbsCuv6;

	if (systemCheckAbs[ABS_CUV6] > GetBarcodeParameter(SYSTEM_CHECK, SYS_CHK_DIL_ABS_IDX))
	{
		ReportSystemError(DIL_ABS_TOO_HIGH, SERR_DILUENT_ABS); 
		return TRUE;
	}

	// low absorbance check for rotor 40 only

	if (rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].algorithmFlags[ROTOR_ROC_SYSTEM_CHECK_T4_FLAG])	
	{
		if (systemCheckAbs[ABS_CUV6] < ( GetBarcodeParameter(SYSTEM_CHECK, SYS_CHK_DIL_ABS_IDX)/ 2.0 ) )
		{
			ReportSystemError(DIL_ABS_TOO_HIGH, SERR_DILUENT_ABS); 
			return TRUE;
		}
	}


	// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 5 store

	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_DIL_ABS,       IMV_DIRECT_SET_METHOD, diluentBlankAbs );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_PLASTIC_ABS_6, IMV_DIRECT_SET_METHOD, systemCheckConfig_g.plasticAbsCuv6 );
	StoreBarcodeParamsImv( SYSTEM_CHECK );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 5.0 );

	// end IMV


	// calc cuv 28 plastic/diluent blank corrected abs & process T4 check to set limit (rotor 40) or check for cuvette fill error (not rotor 40) 
	
	correctedAbsCuv28 = systemCheckAbs[ABS_CUV28] - systemCheckConfig_g.plasticAbsCuv28 - diluentBlankAbs; 		

	sampleDiluentHighDeltaLimit = systemCheckConfig_g.sampleDiluentHighDeltaLimit;
	 
	if (rotorConfig_g->analyteCalcRecord[SYSTEM_CHECK].algorithmFlags[ROTOR_ROC_SYSTEM_CHECK_T4_FLAG])	
	{
		sampleDiluentHighDeltaLimit *= 1.0 + (correctedAbsCuv28 / sampleDiluentToggle);
	}
	else if (correctedAbsCuv28 < systemCheckConfig_g.sampleCuvLimit)
	{
		ReportSystemError(CUV_FILL_ERROR, SERR_SAMPLE_TO_CUV); 
		return TRUE;
	}


	// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 6 store

	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMP28,            IMV_DIRECT_SET_METHOD, correctedAbsCuv28 );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_PLASTIC_ABS_28,    IMV_DIRECT_SET_METHOD, systemCheckConfig_g.plasticAbsCuv28 );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SD_DELTA_HI_LIM,   IMV_DIRECT_SET_METHOD, sampleDiluentHighDeltaLimit );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMPDIL_TOGGLE,    IMV_DIRECT_SET_METHOD, sampleDiluentToggle );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMP_TO_CUV_LIMIT, IMV_DIRECT_SET_METHOD, systemCheckConfig_g.sampleCuvLimit );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 6.0 );

	// end IMV


	// calc cuv 11 plastic/diluent blank corrected abs & check for sample/diluent mix error

	correctedAbsCuv11  = systemCheckAbs[ABS_CUV11] - systemCheckConfig_g.plasticAbsCuv11 - diluentBlankAbs;
		

	// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 7 store

	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMP11,               IMV_DIRECT_SET_METHOD, correctedAbsCuv11 );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_PLASTIC_ABS_11,       IMV_DIRECT_SET_METHOD, systemCheckConfig_g.plasticAbsCuv11 );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 7.0 );

	// end IMV


	if (correctedAbsCuv28 > sampleDiluentToggle)
	{
		sampleDiluentRatio = correctedAbsCuv28 / correctedAbsCuv11;


		// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 8 store

		StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMP_RATIO, IMV_DIRECT_SET_METHOD, sampleDiluentRatio );

		// end IMV


		if ((sampleDiluentRatio < systemCheckConfig_g.sampleDiluentLowRatioLimit) || (sampleDiluentRatio > systemCheckConfig_g.sampleDiluentHighRatioLimit))
		{
			ReportSystemError(SAMP_DIL_MIX_ERROR, SERR_NONE); 
		}
	}
	else
	{
		sampleDiluentDiff  = correctedAbsCuv28 - correctedAbsCuv11;


		// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 8 store

		StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SAMP_RATIO, IMV_DIRECT_SET_METHOD, sampleDiluentDiff );

		// end IMV


		if ((sampleDiluentDiff < systemCheckConfig_g.sampleDiluentLowDeltaLimit) || (sampleDiluentDiff > sampleDiluentHighDeltaLimit))
		{
			ReportSystemError(SAMP_DIL_MIX_ERROR, SERR_NONE); 
		}
	}


	// IMV - SYSTEM_CHECK result / SYSTEM_CHECK paragraph 8 store

	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SD_RATIO_LO_LIM,   IMV_DIRECT_SET_METHOD, systemCheckConfig_g.sampleDiluentLowRatioLimit );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SD_RATIO_HI_LIM,   IMV_DIRECT_SET_METHOD, systemCheckConfig_g.sampleDiluentHighRatioLimit );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_SD_DELTA_LO_LIM,   IMV_DIRECT_SET_METHOD, systemCheckConfig_g.sampleDiluentLowDeltaLimit );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_AIR_SENSOR_OFFSET, IMV_DIRECT_SET_METHOD, factoryData_g->airTemperatureOffset );

	StoreAlgParagraphNumImv( SYSTEM_CHECK, 8.0 );

	// end IMV


	// calculate cuvette 1 transmission & check if whole blood

	cuvette1Transmission = CalcCuvetteCheckTransmission(DISTRIBUTION_READINGS, 1, OPTICAL_CUVETTE, WP_850_850, NUM_DIST_CHECK_FLASHES, &systemCheckError);

	if (ProcessSystemCalcErrors(systemCheckError))
	{
		return TRUE;
	}

	if ( cuvette1Transmission < systemCheckConfig_g.wbCheckLimit )
	{
		rotorResults_g->rotorInformationResults.rotorSystemFlags |= SAMPLE_WHOLE_BLOOD;
	}


	// IMV - store whole blood sample check parameters

	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_WB_LIMIT,       IMV_DIRECT_SET_METHOD, systemCheckConfig_g.wbCheckLimit );
	StoreIntermediateValue( SYSTEM_CHECK, IMV_SYSCHK_WB_CUV_1_TRANS, IMV_DIRECT_SET_METHOD, cuvette1Transmission );

	// end IMV


	return FALSE;
}

// ********************************************************************
// FUNCTION:
//
//		OffsetLevelCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check black offset readings from cuvette 0 
//		for wavelength levels versus ranges 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		rotorResults_g.rotorInformationResults.rotorOffsetErrorFlags
//
//		bit 0:	850 offset level error, cause rotor abort
//		bit 1:	630 offset level error
//		bit 2:	515 offset level error
//		bit 3:	600 offset level error
//		bit 4:	550 offset level error
//		bit 5:	500 offset level error
//		bit 6:	467 offset level error
//		bit 7:	405 offset level error
//		bit 8:	340 offset level error
//
//		bit 9:	WHT offset level error, no rotor abort
// 
// ********************************************************************/

unsigned OffsetLevelCheck()
{
	unsigned		i;
	unsigned short	wavelengthOffset;
	unsigned short	offsetLevelErrorFlags;

	offsetLevelErrorFlags = 0;

	for (i = 0; i < WL_WHITE; i++)
	{
		wavelengthOffset = rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[i];

		if ((wavelengthOffset < systemCheckConfig_g.offsetLevelMin) || (wavelengthOffset > systemCheckConfig_g.offsetLevelMax))
		{
			offsetLevelErrorFlags |= wavelengthErrorFlags_g[i];
		}
	}

	rotorResults_g->rotorInformationResults.rotorOffsetErrorFlags = offsetLevelErrorFlags;

	if (offsetLevelErrorFlags & systemCheckConfig_g.offsetLevelMask)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_OFFSET_LEVEL);
		return	TRUE;
	}
	
	return FALSE; 
}


// ********************************************************************
// FUNCTION:
//
//		OffsetSdNoiseCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check black offset readings from cuvette 0 
//		using standard deviation (SD) for noise in wavelengths
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		rotorResults_g.rotorInformationResults.rotorOffsetSdErrorFlags
//
//		bit 0:	850 SD noise error, cause rotor abort
//		bit 1:	630 SD noise error
//		bit 2:	515 SD noise error
//		bit 3:	600 SD noise error
//		bit 4:	550 SD noise error
//		bit 5:	500 SD noise error
//		bit 6:	467 SD noise error
//		bit 7:	405 SD noise error
//		bit 8:	340 SD noise error
//
//		bit 9:	WHT SD noise error, no rotor abort
// 
// ********************************************************************/

unsigned OffsetSdNoiseCheck()
{
	unsigned		i;
	unsigned		j;
	unsigned short	offsetSdErrorFlags;
	float			wavelengthOffsetSd;
	float			offsetSdMax;
	float			wavelengthOffsets[NUM_BLACK_OFFSETS_FLASHES];

	offsetSdErrorFlags = 0;
	offsetSdMax = 0.0;

	// check all wavelengths except white wavelenth channel

	for (i = 0; i < WL_WHITE; i++)
	{
		// get set of offset readings for standard deviation noise check & skip 1st flash

		for (j = 0; j < NUM_BLACK_OFFSETS_FLASHES; j++)
		{
			wavelengthOffsets[j] = (float) rotorRawReadings_g->blackOffsets[j+1].rawFlashReadings[i];
		}
		
		wavelengthOffsetSd = StandardDeviation(wavelengthOffsets, NUM_BLACK_OFFSETS_FLASHES);

		if (wavelengthOffsetSd > systemCheckConfig_g.offsetSdLimit)
		{
			offsetSdErrorFlags |= wavelengthErrorFlags_g[i];
		}
		
		// update offset SD wavelength maximum for this rotor for IQC checks

		if (wavelengthOffsetSd  > offsetSdMax)
		{
			offsetSdMax = wavelengthOffsetSd;
		} 
	}

	rotorResults_g->rotorInformationResults.rotorOffsetSdMax = offsetSdMax;
	rotorResults_g->rotorInformationResults.rotorOffsetSdErrorFlags = offsetSdErrorFlags;

	if (offsetSdErrorFlags)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_OFFSET_SD);
		return	TRUE;
	}
	
	return FALSE; 
}


// ********************************************************************
// FUNCTION:
//
//		OpticalDacWavelengthCvNoiseCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check open hole readings for cuvette 29 used to set DAC wavelength trim
//		using coefficient of variance (CV) for noise in each used wavelength transmission
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

unsigned OpticalDacWavelengthCvNoiseCheck()
{
	unsigned		i;
	unsigned		j;
	unsigned short	wavelengthCvFlags;
	float			target;
	float			reference;
	float			opticalDacCv;
	float			opticalDacCvMax;
	float			opticalTransmissions[NUM_OPTICAL_DACS_FLASHES];

	wavelengthCvFlags = 0;
	opticalDacCvMax = 0.0;

	// process all wavelengths except 850 (used to calc transmission) & white (WL_WHITE)

	for (i = 0; i < WL_850; i++)
	{
		// calc optical transmission to 850 reference for each wavelength at optical DAC cuvette & skip 1st flash

		for (j = 0; j < NUM_OPTICAL_DACS_FLASHES; j++)
		{
			reference = (float) rotorRawReadings_g->opticalDacs[j+1].rawFlashReadings[WL_850];

			if (reference == 0.0)
			{
				ReportSystemError(SYS_DIV_BY_0, SERR_DIVIDE_BY_0);
				return	TRUE;
			}

			target = (float) rotorRawReadings_g->opticalDacs[j+1].rawFlashReadings[i];

			opticalTransmissions[j] = target /  reference;
		}
		
		opticalDacCv = PercentCv(opticalTransmissions, NUM_OPTICAL_DACS_FLASHES);

		if (opticalDacCv > systemCheckConfig_g.opticalDacCvLimit)
		{
			wavelengthCvFlags |= wavelengthErrorFlags_g[i];
		}
		
		// update optical DAC CV wavelength maximum for this rotor for IQC checks

		if (opticalDacCv > opticalDacCvMax)
		{
			opticalDacCvMax = opticalDacCv;
		} 
	}

	rotorResults_g->rotorInformationResults.rotorOpticalDacCvMax = opticalDacCvMax;
	rotorResults_g->rotorInformationResults.rotorWavelengthCvFlags = wavelengthCvFlags;

	if (wavelengthCvFlags)
	{
		ReportSystemError(NO_SYSTEM_FLAG, SERR_WAVE_CV);
		return	TRUE;
	}
	
	return FALSE; 
}


// ********************************************************************
// FUNCTION:
//
//		RotorSystemTemperatureCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process rotor temperature error checking & report average rotor temperature
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

unsigned RotorSystemTemperatureCheck()
{
	unsigned	i;
	unsigned	startIdx;
	unsigned	midIdx;
	unsigned	endIdx;
	float		temperatureSwingLimit;
	float		rotorTemperature[MAX_TEMPERATURES];

	RawTemperatures_t	*rawTemperaturesPtr;

	// set up system check common ROC file params
	startIdx = systemCheckConfig_g.temperatureCheckStartIdx;
	midIdx   = systemCheckConfig_g.temperatureCheckMidIdx;
	endIdx   = systemCheckConfig_g.temperatureCheckEndIdx;
	temperatureSwingLimit = systemCheckConfig_g.temperatureSwingLimit;


	// IMV - temperature params store / SAMPLE_BLANK paragraph 9 store

	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_MIN_TEMP_ALLOWED, IMV_DIRECT_SET_METHOD, systemCheckConfig_g.minTemperatureAllowed );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_MAX_TEMP_ALLOWED, IMV_DIRECT_SET_METHOD, systemCheckConfig_g.maxTemperatureAllowed );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_TEMP_SWING_LIMIT, IMV_DIRECT_SET_METHOD, temperatureSwingLimit );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_FIRST_SAMP_TEMP,  IMV_DIRECT_SET_METHOD, CalcTemperature(ROTOR_TEMPERATURE, &rotorRawReadings_g->temperatures[startIdx]) );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_MID_SAMP_TEMP,    IMV_DIRECT_SET_METHOD, CalcTemperature(ROTOR_TEMPERATURE, &rotorRawReadings_g->temperatures[midIdx]) );
	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_LAST_SAMP_TEMP,   IMV_DIRECT_SET_METHOD, CalcTemperature(ROTOR_TEMPERATURE, &rotorRawReadings_g->temperatures[endIdx]) );

	// end IMV


	for (i = startIdx; i <= endIdx && i < MAX_TEMPERATURES; i++)
	{
		// point to rotor temperature raw reading
		rawTemperaturesPtr = &rotorRawReadings_g->temperatures[i];
		rotorTemperature[i] = CalcTemperature(ROTOR_TEMPERATURE, rawTemperaturesPtr);

		if ((rotorTemperature[i] < systemCheckConfig_g.minTemperatureAllowed) || (rotorTemperature[i] > systemCheckConfig_g.maxTemperatureAllowed))
		{
			ReportSystemError(TEMP_OUT_OF_RANGE, SERR_ANALYSIS_TEMPERATURE); 
			return TRUE;
		}
	}

	// compute average for analyte temperature correction & save in rotor results

	rotorResults_g->rotorInformationResults.rotorAverageTemperature = (rotorTemperature[startIdx] + rotorTemperature[midIdx]	+ rotorTemperature[endIdx]) / 3;


	// IMV - SAMPLE_BLANK store	rotor average temperature

	StoreIntermediateValue( SAMPLE_BLANK, IMV_SB_AVG_SAMP_TEMP, IMV_DIRECT_SET_METHOD, rotorResults_g->rotorInformationResults.rotorAverageTemperature );

	// end IMV


	// check if rotor temperature ranges are within swing limits

	if ((fabs(rotorTemperature[startIdx] - rotorTemperature[midIdx]) > temperatureSwingLimit) ||
	    (fabs(rotorTemperature[midIdx]   - rotorTemperature[endIdx]) > temperatureSwingLimit) ||
	    (fabs(rotorTemperature[startIdx] - rotorTemperature[endIdx]) > temperatureSwingLimit)   )
	{
		ReportSystemError(TEMP_DRIFT_ERROR, SERR_TEMPERATURE_SWING); 
		return TRUE;
	}


	// IMV - SAMPLE_BLANK paragraph 9 store

	StoreAlgParagraphNumImv( SAMPLE_BLANK, 9.0 );

	// end IMV


	return FALSE;			// no system rotor temperature error found
}


// ********************************************************************
// FUNCTION:
//
//		CalcSystemCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		SYSTEM_CHECK calculation algorithm 
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

void CalcSystemCheck(void)
{

	// load system check analyte ROC data 
	
	LoadAnalyteAlgData(SYSTEM_CHECK);


	// check system cuvettes 6, 11, 28 for transmission flash range, diluent absorbance, fill & mix, for all rotors except rotor 1
	
	if (barcodeData_g->rotorId != 1)
	{
		if (SystemCuvetteChecks())
		{
			return;
		}
	}

	// check cuvette 0 black offsets averages for level 

	if (OffsetLevelCheck())
	{
		return;
	}

	// check cuvette 0 black offsets raw readings for SD noise

	if (OffsetSdNoiseCheck())
	{
		return;
	}

	// check cuvette 29 DAC trim set wavelength transmission readings for CV noise

	if (OpticalDacWavelengthCvNoiseCheck())
	{
		return;
	}

	// check temperature limits and ranges for rotor run 

	RotorSystemTemperatureCheck();


#ifndef __QNX__
	printf("SYSTEM_CHECK invoked\n");
#endif
}
