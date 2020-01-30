// ********************************************************************
//
// FILENAME:  
// 
// 		Offsets.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console thread that implements rotor algorithm processing for 
// 		setting wavelength offset values & associated checks using raw data from the Engine
//		while outputting offsets to calibration.cfg & flags/parameters to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "SharedCal.h"

// must have at least 49 valid flashes for good offset average calcs

#define MIN_OFFSET_FLASHES_ALLOWED	49


// ********************************************************************
// FUNCTION:
//
//		SetWavelengthOffsets() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithm for setting wavelength offsets in calibration.cfg
//		from Engine raw cuvette 0 black hole readings & updating results flags/parameters
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

unsigned short SetWavelengthOffsets(void)
{
	unsigned char	i;
	unsigned		j;
	unsigned		numValidOffsetFlashes;
	unsigned long	offsetError;
	unsigned long	offsetSum[NUM_WAVELENGTHS];

	RawFlashWavelengths_t	*rawPtr;

	unsigned	rotorErrorNumberIndex;

	// save rotor error number index for 1st system error number found here

	rotorErrorNumberIndex = rotorResults_g->rotorInformationResults.rotorErrorNumberIndex;

	// init for no errors

	offsetError = 0;

	// compute average of black offset readings for each wavelength

	for (i = 0; i < WL_WHITE; i++)
	{
		offsetSum[i] = 0;

		// sum raw readings for number of flashes (skip flash 0)

		numValidOffsetFlashes = NUM_BLACK_OFFSETS_FLASHES;

		for (j = 1; j <= NUM_BLACK_OFFSETS_FLASHES; j++)
		{
			rawPtr = &rotorRawReadings_g->blackOffsets[j];

			// check for a missing flash
			// if more than one missing flash, then flag error for FLASH_RANGE_ERROR & FLASH_ABOVE_MAX_SATURATION

			if (rawPtr->engineTime != MISSING_FLASH_TIME)
			{
				// flash not missing so use flash

				offsetSum[i] += (unsigned long) GetRawFlashReading(rawPtr, i, &offsetError);
			}
			else
			{
				// allow for at least 49 valid flashes to proceed
				// no summing of ADC value for offset if missing flash

				numValidOffsetFlashes--;

				if (numValidOffsetFlashes < MIN_OFFSET_FLASHES_ALLOWED)
				{
					// less than 49 valid ADC flashes so bail

					offsetError |= (FLASH_RANGE_ERROR | FLASH_ABOVE_MAX_SATURATION);
					ReportSystemError(SYS_FLASH_RANGE, SERR_FLASH_RANGE);
					break;
				}
			}
		}

		if (!offsetError)
		{
			rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[i] = (unsigned short)(offsetSum[i] / numValidOffsetFlashes);
		}
		else
		{
			break;
		}
	}

	// save offset for WHITE wavelength to default ADC bias offset for rotor information (not used)

	rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[WL_WHITE] = rotorRawReadings_g->adcBias.adcReferenceOffset;

	return SetAlgorithmErrorNumber(rotorErrorNumberIndex);
}



