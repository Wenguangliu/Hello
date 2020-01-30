// ********************************************************************
//
// FILENAME:  
// 
// 		DacTrims.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm processing for 
// 		setting wavelength DAC trim values & associated checks using raw data from the Engine
//		while outputting DAC trims to calibration.cfg & flags/parameters to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "SharedCal.h"
#include "SharedFactory.h"


// DAC trim calc parameters

#define	TARGET_DAC_TRIM_85			0xD998			// 85% of DAC trim range for compare to DAC trim cuvette 29 reading at MIN_DAC_TRIM
#define	TARGET_DAC_TRIM_X_16		891264.0		// 16 * TARGET_DAC_TRIM_85

// DAC trim calculation flags (all < MIN_DAC_TRIM allowed) for Engine to check prior to setting DAC

#define	DAC_SATURATION				0x08
#define	DAC_TOO_MUCH_SIGNAL			0x04
#define	DAC_NO_SIGNAL				0x02
#define	DAC_WEAK_SIGNAL				0x01

// must have at least 9 valid flashes for good DAC trim calcs

#define MIN_DAC_TRIM_FLASHES_ALLOWED	9


// ********************************************************************
// FUNCTION:
//
//		CalcWavelengthDacTrim() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a setting for wavelength DAC trim from average of 
//		Engine raw cuvette 29 optical readings for a wavelength
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

unsigned char CalcWavelengthDacTrim(unsigned long dacTrimReadingAvg, unsigned short wavelengthLampThreshold)
{
	unsigned char	dacTrim;
	float			rawDac;

	//  sanity check for dac trim average above or below limits

	if ((dacTrimReadingAvg > 0) && (dacTrimReadingAvg < TARGET_DAC_TRIM_85))
	{
		// reading for cuvette 29 in range so calculate correct trim value 

		rawDac = TARGET_DAC_TRIM_X_16 / (float)dacTrimReadingAvg;

		// correct for trim values that are not in the legal range
			 
		if (rawDac < (float) MIN_DAC_TRIM)
		{
			dacTrim = DAC_TOO_MUCH_SIGNAL;
		}
		else if (rawDac >= (float) MAX_DAC_TRIM)
		{
			// DAC is wide open so check for enough counts versus factory thresholds 

			if ((dacTrimReadingAvg * 16) < (unsigned long)wavelengthLampThreshold)
			{
				dacTrim = DAC_WEAK_SIGNAL;
			}
			else
			{
				dacTrim = MAX_DAC_TRIM;
			}
		}
		else
		{
			// calculated DAC trim value in range

			dacTrim = (unsigned char) rawDac;
		}
	}
	else if (dacTrimReadingAvg == 0)
	{
		// no signal  

	 	dacTrim = DAC_NO_SIGNAL;	
	}
	else
	{
		// too much signal 

		dacTrim = DAC_SATURATION;	
	}

	return dacTrim;
}


// ********************************************************************
// FUNCTION:
//
//		SetWavelengthDacTrims() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for setting wavelength DAC trims in calibration.cfg
//		from Engine raw cuvette 29 optical readings & updating results flags/parameters
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

unsigned short SetWavelengthDacTrims(void)
{
	unsigned char	i;
	unsigned		j;
	unsigned		numValidDacTrimFlashes;
	unsigned char	dacTrim;
	unsigned short	dacTrimFlags;
	unsigned long	dacTrimError;
	unsigned long	dacTrimSumAvg[NUM_WAVELENGTHS];
	unsigned short	wavelengthLampThreshold;

	RawFlashWavelengths_t	*rawPtr;

	unsigned	rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// init for no system abort errors

	dacTrimError = 0;

	dacTrimFlags = 0;

	// compute average DAC trim readings & calc dac trim setting for each wavelength

	for (i = 0; i < WL_WHITE; i++)
	{
		dacTrimSumAvg[i] = 0;
		wavelengthLampThreshold = factoryData_g->flashLampThresholds[i];

		// sum raw readings for number of flashes (skip flash 0)

		numValidDacTrimFlashes = NUM_OPTICAL_DACS_FLASHES;

		for (j = 1; j <= NUM_OPTICAL_DACS_FLASHES; j++)
		{
			rawPtr = &rotorRawReadings_g->opticalDacs[j];

			// check for a missing flash
			// if more than one missing flash, then flag error for FLASH_RANGE_ERROR & FLASH_ABOVE_MAX_SATURATION

			if (rawPtr->engineTime != MISSING_FLASH_TIME)
			{
				// flash not missing so use flash

				dacTrimSumAvg[i] += (unsigned long) GetRawFlashReading(rawPtr, i, &dacTrimError);
			}
			else
			{
				// allow for at least 9 valid flashes to proceed
				// no summing of ADC value for DAC trim if missing

				numValidDacTrimFlashes--;

				if (numValidDacTrimFlashes < MIN_DAC_TRIM_FLASHES_ALLOWED)
				{
					// less than 9 valid ADC flashes so bail

					dacTrimError |= (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION);
					ReportSystemError(SYS_FLASH_RANGE, SERR_FLASH_RANGE);
					break;
				}
			}
		}

		// set average of readings for wavelength (will be valid for 9 or 10 flashes only)

		dacTrimSumAvg[i] /= numValidDacTrimFlashes;

		if (!dacTrimError)
		{
			// calc DAC trim setting and check if flag for too low if no reading errors

			dacTrim = CalcWavelengthDacTrim(dacTrimSumAvg[i], wavelengthLampThreshold);

			// set flag for too low trim for all wavelengths except WHITE

			if (dacTrim < MIN_DAC_TRIM)
			{
				dacTrimFlags |= wavelengthErrorFlags_g[i];
			}

			// save setting for DAC trim for this wavelength for calibration configuration

 			calibrationData_g->wavelengthDacTrims[i] = dacTrim;
		}
		else
		{
			break;
		}
	}

	// save setting for WHITE wavelength DAC trim to default minimum (0x10) in calibration.xml

 	calibrationData_g->wavelengthDacTrims[WL_WHITE] = MIN_DAC_TRIM;

	if (!dacTrimError)
	{
		rotorResults_g->rotorInformationResults.rotorDacTrimFlags = dacTrimFlags;

		if (dacTrimFlags)
		{
			ReportSystemError(NO_SYSTEM_FLAG, SERR_DAC_SETTING);
		}
	}

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



