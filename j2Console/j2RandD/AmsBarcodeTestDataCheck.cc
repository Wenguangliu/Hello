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
#include "AmsBarcodeTestStates.h"
#include "Shmem.h"
#include "LogErr.h"


BarcodeErrorReport_t  barcodeErrorReportTable [] = 
                    {
                        {"Barcode Test Pass!!!",                        NO_BARCODE_ERROR_REPORT,      SERR_NONE },        
                        {"Barcode End Transition Error",                BARCODE_END_TRANSITION_ERR,   SERR_BC_END_TRANSITION},
                        {"Barcode Transition Error",                    BARCODE_TRANSITIONS_ERR,      SERR_BC_TRANSITIONS},
                        {"Barcode Start Code Error",                    BARCODE_NO_START_ERR,         SERR_BC_NO_START},
                        {"Barcode Stop Code Error",                     BARCODE_NO_STOP_ERR,          SERR_BC_NO_STOP},
                        {"Barcode Decode Error",                        BARCODE_BAD_DECODE_ERR,       SERR_BC_BAD_DECODE},
                        {"Barode Quiet Zone Error",                     BARCODE_QUIET_ZONE_ERR,       SERR_BC_QUIET_ZONE},
                        {"Barcode Calibration Current Error",           BARCODE_SET_CURRENT_ERR,      SERR_BC_SET_CURRENT},
                        {"Barcode Read Timeout Error",                  BARCODE_READ_TIMEOUT_ERR,     SERR_BC_READ_TIMEOUT},
                        {"Barcode Calibration Error",                   BARCODE_CALIBRATION_ERR,      SERR_BC_CALIBRATION_TIMEOUT},
                        {"Barcode Unkonw Error",                        BARCODE_UNKNOWN_ERR,          SERR_NONE},
                        {"End of Barcode Error",                        END_BARCODE_ERROR,            SERR_NONE}
                        
                    };


//Ams Global Delay Test Results Calculation
void
StateAmsBarcodeTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();

    FormatTestReport ();

    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_BARCODE_TEST_CAL_COMPLETE);
}

state_t
StateAmsBarcodeTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_BARCODE_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_BARCODE_TEST_CAL_COMPLETE:
            retVal = STATE_AMS_BARCODE_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

#define MAX_BARCODE_REPORT_STR_LENGTH 4000

void
StateAmsBarcodeTestResultsCal::
FormatTestReport (void)
{
    char                *pBuff;
    wchar_t             *dBuff;
    char                *reportStr;
    FILE                *fp; 
    int                 i;
    SYSTEMTIME          time;

        // alloc report string memory
    reportStr = (char *)malloc (MAX_BARCODE_REPORT_STR_LENGTH);
    if (reportStr == NULL) {
        LogError ("Fail to allocate barcode report memory");
        wsprintf (displayText, L"%s", L"Fail to allocate report memory");
        return;
    }
    memset (reportStr, 0, sizeof (MAX_BARCODE_REPORT_STR_LENGTH));

    dBuff = displayText;
    pBuff = reportStr;


     // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    LogDebug ("%s",pBuff);
    pBuff += strlen (pBuff);

    if (barcodeErrorReport.error == NO_BARCODE_ERROR_REPORT) {
        sprintf (pBuff, "Barcode Current DAC: %d\n", barcodeCurDacValue);
        LogDebug ("%s",pBuff);
        pBuff += strlen (pBuff);
        sprintf (pBuff, "Barcode Read Test  : Pass\n\n");
        LogDebug ("%s",pBuff);
        pBuff += strlen (pBuff);
        wcscpy (dBuff,  CstrToWstr (reportStr));
        dBuff += wcslen (dBuff);
#if 0
        sprintf (pBuff, "Barcode Code Digital String\n");
        pBuff += strlen (pBuff);
#endif
        // now dump barcode Digital 
        fp = fopen (AMS_BARCODE_DATA_FILE, "rb");
        if (fp == NULL) {
            LogError ("Fail to open %s", AMS_BARCODE_DATA_FILE);
            wcscpy (displayText, L"Fail to open Ams barcode data file");
            free (reportStr);
            return;
        }
        sprintf (pBuff, "Barcode Parameters:\n");
        pBuff += strlen (pBuff);
        for (i = 0; i < NUM_BARCODE_PARAMETERS; i++) {
            fgets (pBuff, 7, fp);
            pBuff += strlen (pBuff);
            sprintf (pBuff, " ");
            pBuff += strlen (pBuff);
        }
        sprintf (pBuff, "\n\n");
        pBuff += strlen (pBuff);

        sprintf (pBuff, "Barcode Code Digital String\n");
        pBuff += strlen (pBuff);
        //format barcode 110 digials to be 11 line, each line has 10 digitals 
        while ((fgets (pBuff, 11, fp)) != NULL) {
            pBuff += strlen (pBuff);
            sprintf (pBuff,"\n");
            pBuff += strlen (pBuff);
        }
        fclose (fp);
    } else {
        // display barcode test error 
        for (i = 0; i < END_BARCODE_ERROR; i++) {
            if (barcodeErrorReport.error == barcodeErrorReportTable [i].error) {
                sprintf (pBuff, "%s\n", barcodeErrorReportTable [i].str);
                pBuff += strlen (pBuff);
                sprintf (pBuff, "Barocde Error Code : 0x%x\n", barcodeErrorReportTable [i].sysError);
                pBuff += strlen (pBuff);
                sprintf (pBuff, "%s\n","Barcode Test: Fail");
                pBuff += strlen (pBuff);
                break;
            }
        } 
        // get barcode test fail time
        wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay, time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);
        dBuff += wcslen (dBuff);
        // copy for test results display
        wcscpy (dBuff, L"Barcode Test: Fail"); 
        LogDebug ("%s",WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    }
    fp = fopen (AMS_BARCODE_REPORT_FILE, "wb");
    if (fp == NULL) {
        free (reportStr);
        LogError ("Fail to Open Barcode Report File");
        wsprintf (displayText, L"%s", L"Fail to open Ams barcode report file");
        return;
    }
    fwrite (reportStr, 1, strlen (reportStr), fp);
    fclose (fp);
    free (reportStr);
} 
