#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <math.h>


#include "RandDStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "AmsDelayTestStates.h"
#include "Shmem.h"
#include "LogErr.h"

DelayErrorReport_t  delayErrorReportTable [] = 
                    {
                        {"Global Delay Test Pass!!!",           NO_DELAY_ERROR_REPORT, " "},        
                        {"Divide By 0",                         ERR_ZERO_DENOM, " "},
                        {"Dumy Data Used",                      ERR_DUMMY_DATA, " "},
                        {"No Maximum Found",                    ERR_NO_MAX,     " "},
                        {"Setsl Diffdrence 0",                  ERR_SETSL_DIFF_0, " "},
                        {"Interpolation Beyond Defined Setsl",  ERR_BAD_INTERPOLATION, " "},
                        {"Other delay erros",                   ERR_OTHERS, " "},
                        {"End of erros",                        END_DELAY_ERROR, " "},
                        
                    };

DelayOtherErrorReport_t  delayOtherErrorTable [] = 
                    {
                        {"Err: NX[%d] <= Avg - %f\n",               ERR_AVG1, " "},     // two variables
                        {"Err: NX[%d] >= Avg + %f\n",               ERR_AVG2, " "},
                        {"Err: NX[%d] >  NI[%d] + %f\n",            ERR_N01, " "},      // three variable
                        {"Err: NX[%d] <  NI[%d] - %f\n",            ERR_N02, " "},
                        {"Err: NX[%d] < MinSamp\n",                 ERR_SAMP1, " "},   // on variable 
                        {"Err: NX[%d] > MaxSamp\n",                 ERR_SAMP2, " "},
                        {"Err: SetAv[nz][%d] < SetAv[nz-5][%d]\n",  ERR_SETAV1, " "},   // two varialbes
                        {"Err: SetAv[nz][%d] < SetAv[nz+5][%d]\n",  ERR_SETAV2, " "},
                        {"Err: SetSl[%d][%d] <  MinDeriv\n",        ERR_MIN1, " "},     // two variables
                        {"Err: SetSl[%d][%d] > -MinDeriv\n",        ERR_MIN1, " "},
                        {"Err: SetSl[%d][%d] >  MaxDeriv\n",        ERR_MAX1, " "},
                        {"Err: SetSl[%d][%d] < -MaxDeriv\n",        ERR_MAX2, ""},
                        {"Err: SetSl[%d][%d] < MinSlope \n",        ERR_MINSLOPE, " "},   // three variables
                        {"Err: SetSl[%d][%d] > MaxSlope)\n",        ERR_MAXSLOPE, " "}
                    };

//Ams Global Delay Test Results Calculation
void
StateAmsDelayTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"AMS Delay Test Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();

    // do test data processing
    GetAverageData ();
    ProcessData ();
    CheckData ();
    FormatTestReport ();

    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_DELAY_TEST_CAL_COMPLETE);
}

state_t
StateAmsDelayTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_DELAY_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_DELAY_TEST_CAL_COMPLETE:
            retVal = STATE_AMS_DELAY_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

// get raw data from W850 and W340
// Note: W340 repeat 3 times
// save average date to data file
void
StateAmsDelayTestResultsCal::
GetAverageData (void)
{
    unsigned char           flashNumber, waveLength;
    RawFlashWavelengths_t   *flashData;
    unsigned char           delay;
    unsigned long           sum [MAX_CHANNEL], offsetSum;
    unsigned short          wavelengthAdcOffset [10];
    FILE                    *fp;
    char                    buf [128];


    delayErrorReport.error = NO_DELAY_ERROR_REPORT;
    // calculate blackoffset

    // calculate black offset
    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        offsetSum = 0;
        // sum raw readings for number of flashes (skip flash 0)
        for (flashNumber = 1; flashNumber <= NUM_BLACK_OFFSETS_FLASHES; flashNumber++) {
            offsetSum += rawData->rotorRawReadings.blackOffsets[flashNumber].rawFlashReadings [waveLength];
        }
        wavelengthAdcOffset [waveLength] = (unsigned short)(offsetSum / NUM_BLACK_OFFSETS_FLASHES);
    }
#if 0
    // ZZZ for debug only
    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        LogDebug ("Blackoffet [%d] = %d", waveLength, wavelengthAdcOffset [waveLength]);
    }
#endif
    // get raw data and calculate average data
    for (delay = 0; delay < MAX_DELAY_NUM - 1; delay++) {
        memset (sum, 0, sizeof (sum));
        for (flashNumber =1;  flashNumber < 21; flashNumber++) {
            flashData = &rawData->rotorRawReadings.cuvetteDelay[delay][flashNumber];
            sum [0] += flashData->rawFlashReadings [W850] -  wavelengthAdcOffset [850];
            sum [1] += flashData->rawFlashReadings [W340] -  wavelengthAdcOffset [340];
            sum [2] += flashData->rawFlashReadings [W340] -  wavelengthAdcOffset [340];
            sum [3] += flashData->rawFlashReadings [W340] -  wavelengthAdcOffset [340];
        }
        avgData [delay][0] = (unsigned short)(sum [0] / MAS_FLASH_NUM);
        avgData [delay][1] = (unsigned short)(sum [1] / MAS_FLASH_NUM);
        avgData [delay][2] = (unsigned short)(sum [2] / MAS_FLASH_NUM);
        avgData [delay][3] = (unsigned short)(sum [3] / MAS_FLASH_NUM);
    }
    // save average data to file
    fp = fopen (AMS_DELAY_DATA_FILE, "wb");
    if (fp == NULL) {
        LogDebug ("Fail to open file %s", AMS_DELAY_DATA_FILE);
    } else {
        for (delay =0; delay < MAX_DELAY_NUM - 1; delay++) {
            memset (buf, 0, sizeof (buf));
          //  LogDebug ("avg = %d %d %d %d", avgData [delay][0], avgData [delay][1], avgData [delay][2], avgData [delay][3]);
            sprintf (buf, "%u %u %u %u\n", avgData [delay][0], avgData [delay][1], avgData [delay][2], avgData [delay][3]); 
           // LogDebug ("%s", buf);
            fwrite (buf, 1, strlen (buf),fp);
        }
        fclose (fp);
    }

}

int
StateAmsDelayTestResultsCal::
ProcessData (void)
{

    int i,j,k;
    float sum, f, denom;


    delayErrorReport.error = NO_DELAY_ERROR_REPORT;

    // clear memory
    memset (setav,0,sizeof (setav));
    memset (setsl,0,sizeof (setsl));
    memset (intc,0,sizeof (intc));
    memset (n0,0,sizeof (n0));
    memset (ni,0,sizeof (ni));
    memset (nx,0,sizeof (nx));

    for (i = 2; i <= 72; i++) {
        for (j = 0; j <= 3; j++) {
            for (sum=0., k = i-2; k <= i+2; k++) {
                sum += (float)avgData [k][j];
            }
            setav[i][j] = sum/5.0;
            if (i > 2) {
                f= setsl[i][j] = setav[i][j] - setav[i-1][j];
                if ((f < 0.0) && (setsl[i-1][j] >= 0.0)) {
                    intc[j]++;
                    n0[j] +=i;
                }
            }
        }
    }

    for(j = 0; j < 4; j++) {

        if (intc [j] == 0) {
            delayErrorReport.error = ERR_NO_MAX;
            return 1;
        }
        ni[j] = n0[j]/intc[j] + 0.5;    // get middle delay number /*divide first, add second */

        k = ni[j] - 3;

        if ((k + 6) > 72) {
            delayErrorReport.error = ERR_BAD_INTERPOLATION;
            return 1;
        }
        denom = setsl [k][j] - setsl [k+6][j];

        if ((denom > 0.0) && (denom < manufacturingTestSet.delaySmall)) {
            delayErrorReport.error = ERR_SETSL_DIFF_0;
            return 1;
        }

        if ((denom < 0.0) && (denom > (-1 * manufacturingTestSet.delaySmall ))) {
            delayErrorReport.error = ERR_SETSL_DIFF_0;
            return 1;
        }
        nx[j] = k + 6 * setsl[k][j] / denom;
    }
    for(avg = 0.0, j = 0; j < 4; j++) {
        avg += nx[j];
    } 
    avg /= 4.;
    nz = (int)(avg + 0.5);
    globalDelay  =  manufacturingTestSet.delayMulti * nz + manufacturingTestSet.delayOffset;

    return 0;
}

int
StateAmsDelayTestResultsCal::
CheckData (void)
{
    float   halfWidth;
    int     i,j;
    float   f, g;
    char    errors = 0;

    memset (error1, 0, sizeof (error1));
    memset (error2, 0, sizeof (error2));


    halfWidth = manufacturingTestSet.okHalfWitdh;

    for(j = 0; j < 4; j++) {
        f = nx [j];
        if (f <= (avg - halfWidth)) {
            error1[j] |= ERR_AVG1;
            errors = 1;
        } 
        if (f >= (avg + halfWidth)) {
            error1[j] |= ERR_AVG2;
            errors = 1;
        }
        if (f > (ni[j] + halfWidth)) {
            error1[j] |= ERR_N01;
            errors = 1;
        }
        if (f < (ni[j] - halfWidth)) {
            error1[j] |= ERR_N02;
            errors = 1;
        }
        if (f <= manufacturingTestSet.delayMinSamp) {
            error1[j] |= ERR_SAMP1;
            errors = 1;
        }
        if (f >= manufacturingTestSet.delayMaxSamp) {
            error1[j] |= ERR_SAMP2;
            errors = 1;
        }
        f =  setav[nz][j];
        if (f <  setav[nz-5][j]) {
            error1[j] |= ERR_SETAV1;
            errors = 1;
        }
        if (f < setav[nz+5][j]) {
            error1[j] |= ERR_SETAV2;
            errors = 1;
        }
        
    }

    for(i = 3; i <= 72; i++) {
        for(j = 0; j < 4; j++) {
            f = setsl[i][j];
            if ((i <= nz - 14) && (f < manufacturingTestSet.delayMinDerive)) {
                error2[i][j] |= ERR_MIN1;
                errors = 1;
            }
            if ((i >= nz + 14) && (f > (-1 * manufacturingTestSet.delayMinDerive))) {
                error2[i][j] |= ERR_MIN2;
                errors = 1;
            }

            if ((i <= nz - 6) && (f > manufacturingTestSet.delayMaxDerive)) {
                error2[i][j] |= ERR_MAX1;
                errors = 1;
            }
            if ((i >= nz + 6) && (f < (-1 * manufacturingTestSet.delayMaxDerive))) {
                error2[i][j] |= ERR_MAX2;
                errors = 1;
            }
            g = manufacturingTestSet.delayMinSlop - 46.0 * (i - nz + 14);
            if ((i > nz -14) && (i < nz +16) && (f < g)) {
                error2[i][j] |= ERR_MINSLOPE;
                errors = 1;
            }
            g = manufacturingTestSet.delayMaxSlop - 36.0 * (i - nz + 6);
            if ((i > nz -6) && (i < nz +14) && (f > g)) {
                error2[i][j] |= ERR_MAXSLOPE;
                errors = 1;
            }
        
        }
    }
    if (errors) {
        delayErrorReport.error = ERR_OTHERS;
    }
    return 0;
}

#define MAX_DELAY_REPORT_STR_LENGTH 20280

void
StateAmsDelayTestResultsCal::
FormatTestReport (void)
{
    char            *pBuff;
    wchar_t         *dBuff;
    char            *reportStr;
    FILE            *fp; 
    int             i, j, k;
    float           delayOffset;
    SYSTEMTIME      time;

    // alloc report string memory
    reportStr = (char *)malloc (MAX_DELAY_REPORT_STR_LENGTH);
    if (reportStr == NULL) {
        LogError ("Fail to allocate delay report memory");
        wsprintf (displayText, L"%s", L"Fail  to allocate report memory");
        return;
    }
    memset (reportStr, 0, sizeof (MAX_DELAY_REPORT_STR_LENGTH));

    pBuff = reportStr;
    dBuff = displayText;

    // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    LogDebug ("%s", pBuff);
    pBuff += strlen (pBuff);




    if (delayErrorReport.error == NO_DELAY_ERROR_REPORT) {
        sprintf (pBuff, "Global Delay Time  : %u usec\n", (unsigned short)globalDelay);
        LogDebug ("%s", pBuff);
        pBuff += strlen (pBuff);
        sprintf (pBuff, "Global Delay Number: %u \n", (unsigned short)(globalDelay /manufacturingTestSet.delayMulti));
        LogDebug ("%s", pBuff);
        pBuff += strlen (pBuff);
        sprintf (pBuff, "Global Delay Test  : Pass\n\n");
        LogDebug ("%s", pBuff);
        pBuff += strlen (pBuff);
        // dispaly results
        wcscpy (dBuff, CstrToWstr (reportStr));
    } else {
        // display global delay error 
        for (i = 0; i < ERR_OTHERS; i++) {
            if (delayErrorReport.error == delayErrorReportTable [i].error) {
                sprintf (pBuff, "%s\n", delayErrorReportTable [i].str);
                pBuff += strlen (pBuff);
                break;
            }
        } 
        if (delayErrorReport.error == ERR_OTHERS) {
            // search whole table
            for (j = 0; j < MAX_OTHER_ERR1; j++) {
                for (i = 0; i < MAX_CHANNEL; i++) {
                    if (error1 [i] & delayOtherErrorTable [j].error) {
                        if ( j < 2) {
                            sprintf (pBuff, delayOtherErrorTable [j].str, i, manufacturingTestSet.okHalfWitdh); 
                        } else if ((j >= 2 ) && (j < 4))  {
                            sprintf (pBuff, delayOtherErrorTable [j].str, i, i, manufacturingTestSet.okHalfWitdh); 
                        } else if ((j >= 4) && (j < 6)){
                            sprintf (pBuff, delayOtherErrorTable [j].str, i); 
                        } else {
                            sprintf (pBuff, delayOtherErrorTable [j].str, i,  manufacturingTestSet.okHalfWitdh); 
                        }
                        pBuff += strlen (pBuff);
                    }
                }
            } 
            for (j = MAX_OTHER_ERR1; j < (MAX_OTHER_ERR2 + MAX_OTHER_ERR1); j++) {
                for (i = 3;  i < MAX_DELAY_NUM -4; i++) {
                    for (k = 0; k < MAX_CHANNEL; k++) {
                        if (error2 [i][k] & delayOtherErrorTable [j].error) {
                            sprintf (pBuff,  delayOtherErrorTable [j].str, i, k);
                            pBuff += strlen (pBuff);
                        }
                    }
                } 
            }
            
        }
        sprintf (pBuff, "Global Delay Test: Fail\n\n");
        pBuff += strlen (pBuff);
        // test results display
        wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

        dBuff += wcslen (dBuff);
        wsprintf (dBuff, L"Global Delay Test: Fail\n\n");
        LogDebug ("%s", dBuff);
        dBuff += wcslen (dBuff);
    } // end of if (delayErrorReport.error == NO_DELAY_ERROR_REPORT)

    // copy for test results display
    //strcpy (displayText, reportStr); 

    sprintf (pBuff, "usec    chan1   chan2  chan3  chan4\n");
    pBuff += strlen (pBuff);
    // Average  number and delay time
    delayOffset = manufacturingTestSet.delayOffset;
    for (i = 0; i <  MAX_DELAY_NUM -1; i++) {
        sprintf (pBuff, "%4.1f  %5d  %5d  %5d  %5d\n", delayOffset, avgData [i][0], avgData [i][1], avgData [i][2], avgData [i][3]);
        pBuff += strlen (pBuff);
        delayOffset += manufacturingTestSet.delayMulti;
    }
    // now saved to file
    fp = fopen (AMS_DELAY_REPORT_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to open %s", AMS_DELAY_REPORT_FILE);
        wsprintf (displayText, L"Fail to open %s", AMS_DELAY_REPORT_FILE);
    } else {
        fwrite (reportStr, 1, strlen (reportStr), fp);
        fclose (fp);
    }
    free (reportStr);
} 
