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
#include "AmsAlignmentTestStates.h"
#include "Shmem.h"
#include "LogErr.h"


//Ams Test Results Calculation
void
StateAmsAlignmentTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"AMS Alignment Test Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();

    FormatTestReport ();

    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_ALIGNMENT_TEST_CAL_COMPLETE);
}

state_t
StateAmsAlignmentTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_ALIGNMENT_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_ALIGNMENT_TEST_CAL_COMPLETE:
            retVal = STATE_AMS_ALIGNMENT_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

#define MAX_ALIGNMENT_REPORT_STR_LENGTH 4000

void
StateAmsAlignmentTestResultsCal::
FormatTestReport (void)
{
    char				*pBuff;
    wchar_t             *dBuff;
    char				*reportStr;
    FILE				*fp; 
    int					i;
    unsigned    short   *barCountMin;
    SYSTEMTIME			time;



   // alloc report string memory
    reportStr = (char *)malloc (MAX_ALIGNMENT_REPORT_STR_LENGTH);
    if (reportStr == NULL) {
        LogError ("Fail to allocate alignment report memory");
        wsprintf (displayText, L"%s", L"Fail to allocate report memory");
        return;
    }
    memset (reportStr, 0, sizeof (MAX_ALIGNMENT_REPORT_STR_LENGTH));

    pBuff = reportStr;
    dBuff = displayText;

    //write to test data file
    fp = fopen (AMS_ALIGNMENT_DATA_FILE, "wb");
     if (fp == NULL) {
        LogError ("Fail to Open alignment data File");
        wsprintf (displayText, L"%s", L"Fail to open Ams alignment data file");
        sprintf (reportStr, "%s", "Fail to open Ams alignment data file");
        free (reportStr);
        return;
    }
    for (i = 0; i < MAX_BAR_ACOUNT; i++) {
        sprintf (pBuff, "%d\n", barCount [i]);
        fwrite (pBuff, 1, strlen (reportStr), fp);
    }
    fclose (fp);

    // clear memroy again 
    memset (reportStr, 0, sizeof (MAX_ALIGNMENT_REPORT_STR_LENGTH));
    pBuff = reportStr;

    // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);
    pBuff += strlen (pBuff);
    // format results display 
    wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);
    LogDebug ("%s", WstrToCstr (dBuff));
    dBuff += wcslen (dBuff);
    
    // for F1 count
    barCountMin = &manufacturingTestSet.alignmentF1Min;
    for (i = 0; i < MAX_BAR_ACOUNT/2; i++) {
        if (barCount [i] < *barCountMin) {
            sprintf (pBuff, "F1[%d] = %3d  < Limit %2d *\n", i, barCount [i], *barCountMin); 
        } else {
            sprintf (pBuff, "F1[%d] = %3d  > Limit %2d \n",  i, barCount [i], *barCountMin); 
        }
        pBuff += strlen (pBuff);
        barCountMin++;
    }
    // for F2 count
    barCountMin = &manufacturingTestSet.alignmentF1Min;
    for (i = 0; i < MAX_BAR_ACOUNT/2; i++) {
        if (barCount [i + 5] < *barCountMin) {
            sprintf (pBuff, "F2[%d] = %3d  < Limit %2d *\n", i, barCount [i + 5], *barCountMin); 
        } else {
            sprintf (pBuff, "F2[%d] = %3d  > Limit %2d \n",  i, barCount [i + 5], *barCountMin); 
        }
        pBuff += strlen (pBuff);
        barCountMin++;
    }

    if (alignmentErrorReport.error != NO_ALIGNMENT_ERROR_REPORT) {
        sprintf (pBuff, "Barcode Alignment Test :Fail\n"); 
        pBuff += strlen (pBuff);
        wcscpy (dBuff, L"Barcode Alignment Test :Fail\n");
        LogDebug ("%s", WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    } else {
        sprintf (pBuff, "Barcode Alignment Test :Pass\n"); 
        pBuff += strlen (pBuff);
        wcscpy (dBuff, L"Barcode Alignment Test :Pass\n");
        LogDebug ("%s", WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    }
    fp = fopen (AMS_ALIGNMENT_REPORT_FILE, "wb");
    if (fp == NULL) {
        free (reportStr);
        LogError ("Fail to Open Alignment Report File");
        wsprintf (displayText, L"%s", L"Fail to open Ams alignment report file");
        return;
    }
    fwrite (reportStr, 1, strlen (reportStr), fp);
    fclose (fp);
    free (reportStr);
} 
