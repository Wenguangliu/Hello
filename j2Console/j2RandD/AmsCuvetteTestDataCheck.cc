#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <math.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "AmsCuvetteTestStates.h"
#include "Shmem.h"
#include "LogErr.h"

CuvetteFinalErrorReport_t  CuvetteFinalErrorReportTable [] = 
                    {
                        {"Cuvette Test Pass!!!",                    NO_CUV_ERROR_REPORT},        
                        {"Cuvette Slope less than minimal",         CUV_SLOPE_LESS_THAN_MIN},
                        {"Cuvette Slope large than maximal",        CUV_SLOPE_LARGE_THAN_MAX},
                        {"Cuvette Offset less than minimal",        CUV_OFFSET_LESS_THAN_MIN},
                        {"Cuvette OFFSET large than maximal",       CUV_OFFSET_LARGE_THAN_MAX},
                        {" ",                                       END_CUV_ERROR}
                        
                    };

//Ams  Test Results Calculation
void
StateAmsCuvetteTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test Results Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();

    // calculate test results
    CalculateTestResults ();

    // generate test report
    FormatTestReport ();


    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_CUVETTE_TEST_CAL_COMPLETE);
}

state_t
StateAmsCuvetteTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_CUV_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_CUVETTE_TEST_CAL_COMPLETE:
            retVal = STATE_AMS_CUV_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}


void
StateAmsCuvetteTestResultsCal::
CalculateTestResults (void)
{
    float   offset;
    float   slope;

    // calculate offset and slope
    offset = (DAC120_SET_POINT - (cuvetteData [DAC_120] * cuvetteRotor.slope))  + cuvetteRotor.intercept;
    slope  = (DAC120_SET_POINT - (cuvetteData [DAC_70]  * cuvetteRotor.slope))  + cuvetteRotor.intercept + offset;
    slope /= 50;
    cuvetteCalResults.offset = (int)offset;
    cuvetteCalResults.slope  = slope;
    //clear error flag
    memset (cuvetteCalResults.error, 0, sizeof (cuvetteCalResults.error));
    LogDebug ("offset=%d slop =%f", cuvetteCalResults.offset,  cuvetteCalResults.slope);
}


void
StateAmsCuvetteTestResultsCal::
FormatTestReport ()
{
    char                *reportStr;
    char                *pBuff;
    wchar_t              *dBuff;
    int                 i;
    FILE                *fp;
    int                 errorReport = 0;        
    SYSTEMTIME          time;

   reportStr = (char *)malloc (MAX_CUV_REPORT_STR_LENGTH);
   if (reportStr == NULL) {
        LogError ("Fail to allocate cuvette report memory");
        wcscpy  (displayText, L"Fail to alloc cuvette report memory");
        return ;
    }
    memset (reportStr, 0, MAX_CUV_REPORT_STR_LENGTH);
    pBuff = reportStr;
    dBuff = displayText;

    // get courrent time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    pBuff += strlen (pBuff);

    wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    LogDebug ("%s", WstrToCstr (dBuff));
    dBuff += wcslen (dBuff);


    sprintf (pBuff, "          --Index Cuvette Width--\n");
    pBuff += strlen (pBuff);

    for (i = 0; i < MAX_CUVETTE_LOOP; i++) {
        sprintf (pBuff, "Current DAC%-3d: %d\n", (50 + i *10), cuvetteData [i]);    
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff, "\n");
    pBuff += strlen (pBuff);


    sprintf (pBuff, "          --Calculation Results--\n");
    pBuff += strlen (pBuff);

    if (cuvetteCalResults.offset < manufacturingTestSet.cuvOffsetMin) {
        sprintf (pBuff, "Offset: %d* < Offset Minimal: %d\n", cuvetteCalResults.offset, manufacturingTestSet.cuvOffsetMin);    
        pBuff += strlen (pBuff);
        cuvetteCalResults.error [CUV_OFFSET_LESS_THAN_MIN] = 1;
        errorReport = 1;
        
    } else if (cuvetteCalResults.offset > manufacturingTestSet.cuvOffsetMax) {
        sprintf (pBuff, "Offset: %d* > Offset Maximal: %d\n", cuvetteCalResults.offset, manufacturingTestSet.cuvOffsetMax);  
        pBuff += strlen (pBuff);
        cuvetteCalResults.error [CUV_OFFSET_LARGE_THAN_MAX] = 1;
        errorReport = 1;
    } else {
        sprintf (pBuff, "Offset: %d\n", cuvetteCalResults.offset);
        pBuff += strlen (pBuff);
    }

    if (cuvetteCalResults.slope <  manufacturingTestSet.cuvSlopeMin) {
        sprintf (pBuff, "Slope : %.3f* < Slope Minimal: %.3f\n", cuvetteCalResults.slope, manufacturingTestSet.cuvSlopeMin);
        pBuff += strlen (pBuff);
        cuvetteCalResults.error [CUV_SLOPE_LESS_THAN_MIN] = 1;
        errorReport = 1;
    } else if (cuvetteCalResults.slope >  manufacturingTestSet.cuvSlopeMax) {
        sprintf (pBuff, "Slope : %.3f* > Slope Maximal: %.3f\n", cuvetteCalResults.slope, manufacturingTestSet.cuvSlopeMax);
        pBuff += strlen (pBuff);
        cuvetteCalResults.error [CUV_SLOPE_LESS_THAN_MIN] = 1;
        errorReport = 1;
    } else {
        sprintf (pBuff, "Slope : %.3f\n", cuvetteCalResults.slope);
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff, "\n");
    pBuff += strlen (pBuff);

    sprintf (pBuff, "          --Final Test Result--\n");
    pBuff += strlen (pBuff);

    // no error
    if (errorReport == 0) {
        // for report
        sprintf (pBuff,"Cuvette Test: Pass");
        pBuff += strlen (pBuff);
        // for results display
        wsprintf  (dBuff,L"Cuvette Test: Pass");
        LogDebug ("%s", WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    } else {
        // for report
        sprintf (pBuff,"Cuvette Test: Fail");
        pBuff += strlen (pBuff);
        // for results display
        wsprintf  (dBuff,L"Cuvette Test: Fail");
        LogDebug ("%s", WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    }


    // save to file
    fp = fopen (AMS_CUVETTE_REPORT_FILE, "wb");
    if (fp != NULL) {
        fwrite (reportStr, 1 ,strlen (reportStr), fp);
        fclose (fp);
    } else {
        swprintf  (displayText, L"Fail to open %s", AMS_CUVETTE_REPORT_FILE);
    }
    free (reportStr);
}
