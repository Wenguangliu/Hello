#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>


#include "Serial.h"
#include "XmlData.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "CommonDefines.h"
#include "State.h"
#include "AmsComm.h"
#include "EngComm.h"
#include "Global.h"

/* 
 * 85% of DAC trim range for compare to DAC trim cuvette 
 * 29 reading at MIN_DAC_TRIM
 */
#define TARGET_DAC_TRIM_85          0xD998         
/* 16 * TARGET_DAC_TRIM_85 */
#define TARGET_DAC_TRIM_X_16        891264.0        

/*  DAC trim calculation flags */
#define DAC_SATURATION              0x08
#define DAC_TOO_MUCH_SIGNAL         0x04
#define DAC_NO_SIGNAL               0x02
#define DAC_WEAK_SIGNAL             0x01

#define MIN_DAC_TRIM                0x10
#define MAX_DAC_TRIM                0xFF


/*
 *
 * Calculate Dac Trim
 */
unsigned char 
CalcWavelengthDacTrim (unsigned long dacTrimReadingAvg, unsigned short wavelengthLampThreshold)
{
    unsigned char   dacTrim;
    float           rawDac;

    //  sanity check for dac trim average above or below limits

    if ((dacTrimReadingAvg > 0) && (dacTrimReadingAvg < TARGET_DAC_TRIM_85)) {
        // reading for cuvette 29 in range so calculate correct trim value

        rawDac = (float)(TARGET_DAC_TRIM_X_16 / (float)dacTrimReadingAvg);

        // correct for trim values that are not in the legal range

        if (rawDac < (float) MIN_DAC_TRIM) {
            dacTrim = DAC_TOO_MUCH_SIGNAL;
        } else if (rawDac >= (float) MAX_DAC_TRIM) {
            // DAC is wide open so check for enough counts versus factory thresholds

            if ((dacTrimReadingAvg * 16) < (unsigned long)wavelengthLampThreshold) {
                dacTrim = DAC_WEAK_SIGNAL;
            } else {
                dacTrim = MAX_DAC_TRIM;
            }
        } else {
            // calculated DAC trim value in range

            dacTrim = (unsigned char) rawDac;
        }
    } else if (dacTrimReadingAvg == 0) {
        // no signal

        dacTrim = DAC_NO_SIGNAL;
    } else {
        // too much signal
        dacTrim = DAC_SATURATION;
    }

    return dacTrim;
}

/*
 * calculate trim DAC values and send back to Engine
 */
void
SetDacTrims (void)
{
    unsigned char   waveLength;
    unsigned        flash;
    unsigned char   dacTrim;
    unsigned long   dacTrimSumAvg[NUM_WAVELENGTHS];
    unsigned short  wavelengthLampThreshold;

    RawFlashWavelengths_t   *rawData;

    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        dacTrimSumAvg[waveLength] = 0;
        /* get flash lamp threshod */
        wavelengthLampThreshold = FactoryData.flashLampThresholds [waveLength];
        /* cummulate average of raw data */
        for (flash =1; flash <= NUM_OPTICAL_DACS_FLASHES; flash++) {
            rawData = &amsData->opticalDacs[flash]; 
            dacTrimSumAvg[waveLength] += (unsigned long)rawData->rawFlashReadings [waveLength];
        }
        /* set the average */
        dacTrimSumAvg[waveLength] /= NUM_OPTICAL_DACS_FLASHES;

        /* calculate trim setting */
        dacTrim = CalcWavelengthDacTrim (dacTrimSumAvg[waveLength], 
                                         wavelengthLampThreshold);

        /* set calibration data structure */
        CalibrationData.wavelengthDacTrims [waveLength] = dacTrim;
        /* ZZZ */
#if 0
        printf ("w = %d dacTrim = %d\n",waveLength,  dacTrim);
#endif
    }

    /* now send trim Dac back to Engine */
    SendCalibrationFactors ();

    /* update calibration.xml file */
    WriteCalibration (&CalibrationData);
}
