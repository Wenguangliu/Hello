#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "AmsTestReportDisplay.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// static variables
DisplayParameters_t  StateAmsTestReportGlobal::displayParameters;

int             StateDisplayAmsReport::pageNumber = 0;
int             StateDisplayAmsReport::totalPages = 0;
int             StateDisplayAmsReport::totallReadLen = 0;
wchar_t         StateDisplayAmsReport::displayMsg [DIS_PAGE_SIZE];
int             StateDisplayAmsReport::fileSize;
int             StateDisplayAmsReport::lastPgLen;
char            StateDisplayAmsReport:: mPage = 0;


void
StateAmsTestReportGlobal::
foo (void)
{
}


//Ams Test Report Display 
void
StateAmsTestReport::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Ams Test Reports";
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    
    SetTopLabel (topLable);

    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Drawer Test Report");
    wcscpy (data->buttonText[1], L"Motor Test Report");
    wcscpy (data->buttonText[2], L"NDXT Test Report");
    wcscpy (data->buttonText[3], L"Temperature Test Report");
    wcscpy (data->buttonText[4], L"Global Delay Test Report");
    wcscpy (data->buttonText[5], L"Cuvette Test Report");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true, strings [BSTR_NEXT]);
}

// Ams test report  process
state_t
StateAmsTestReport::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_AMS_TEST_REPORT;

    int idata = *(int *)data;

     switch (msgId) {
        case MSG_GUI_OUT_BUTTON:

            stateStack.Push (STATE_AMS_TEST_REPORT);
            retVal = STATE_AMS_DISPLAY_REPORT;
            memset (&displayParameters.topLabel [0], 0, sizeof (DisplayParameters_t));

            switch (idata) {
                case 0: // drawer test
                    wcscpy (displayParameters.topLabel, L"Drawer Test Report"); 
                    displayParameters.fileName = AMS_DRAWER_TEST_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                case 1: // motor test
                    wcscpy (displayParameters.topLabel, L"Motor Test Report"); 
                    displayParameters.fileName = AMS_MOTOR_TEST_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                case 2: // ndxt test
                    wcscpy (displayParameters.topLabel, L"NDXT Test Report"); 
                    displayParameters.fileName = AMS_NDXT_DISP_REPORT_FILE;
                    displayParameters.font  = SMALL;
                break;

                case 3: // temperature test
                    wcscpy (displayParameters.topLabel, L"Temperature Test Report"); 
                    displayParameters.fileName = AMS_TEMPERATURE_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                case 4: // cuvette global delay test
                    wcscpy (displayParameters.topLabel, L"Global Delay Test Report"); 
                    displayParameters.fileName = AMS_DELAY_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                case 5: // cuvette test
                    wcscpy (displayParameters.topLabel, L"Cuvette Test Report");
                    displayParameters.fileName = AMS_CUVETTE_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                default:
                    stateStack.Pop(); 
                    retVal = STATE_AMS_TEST_REPORT;
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    retVal =  stateStack.Pop();
                break;

                case 1:
                   stateStack.Push (STATE_AMS_TEST_REPORT);
                   retVal = STATE_AMS_TEST_REPORT_2;
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}


//Ams Test Report Display 
void
StateAmsTestReport2::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Ams Test Reports";
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    
    SetTopLabel (topLable);

    memset (data, 0, sizeof (GuiMiddleButtonsData_t));

    wcscpy (data->buttonText[0], L"Barcode Test Report");
    wcscpy (data->buttonText[1], L"Alignment Test Report");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);
}

// Ams test report  process
state_t
StateAmsTestReport2::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_AMS_TEST_REPORT_2;

    int idata = *(int *)data;

     switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            stateStack.Push (STATE_AMS_TEST_REPORT_2);
            retVal = STATE_AMS_DISPLAY_REPORT;
            memset (&displayParameters.topLabel [0], 0, sizeof (DisplayParameters_t));

            switch (idata) {

                case 0: // barcode test
                    wcscpy (displayParameters.topLabel, L"Barcode Test Report");
                    displayParameters.fileName = AMS_BARCODE_REPORT_FILE;
                    displayParameters.font  = MEDIUM;

                break;

                case 1: // barcode alignment file
                    wcscpy (displayParameters.topLabel, L"Barcode Alignment Test Report");
                    displayParameters.fileName = AMS_ALIGNMENT_REPORT_FILE;
                    displayParameters.font  = MEDIUM;
                break;

                default:
                    stateStack.Pop(); 
                    retVal = STATE_AMS_TEST_REPORT;
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal =  stateStack.Pop();
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

void
StateDisplayAmsReport::
SendReportMessage (void)
{
    GuiReceiveMsg_t     msg;
    FILE                *fp;
    int                 len;
    char                buff [DIS_PAGE_SIZE];
    char                error = 0;

    // clear buffer
    memset (displayMsg, 0, sizeof (displayMsg));
    memset (buff, 0, sizeof (buff));

    fp = fopen (displayParameters.fileName, "rb");
    if (fp == NULL) {
        wcscpy (displayMsg, L"Test report is not available");
        //len = sizeof (displayMsg);
        len = sizeof (buff);
        error = 1;
    } else {
         //set file read point
        if (fseek (fp, totallReadLen, SEEK_SET) == 0) {
            len = fread (buff, 1, DIS_PAGE_SIZE, fp);
            //len = fread (displayMsg, 1, DIS_PAGE_SIZE, fp);
            totallReadLen += len;
            if (totallReadLen >= fileSize) {
                // records last page length
                lastPgLen = len;
            }
        } else {
            LogError ("Fail to set %s postion %d", displayParameters.fileName, totallReadLen);
            wcscpy (displayMsg, L"Fail to set ams report file position");
            len     = sizeof (buff);
            //len     = sizeof (displayMsg);
            error = 1;
        }
        fclose (fp);
    }
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    if (error == 0) {
        mbstowcs (displayMsg, buff, len);
    }
    wcsncpy (msg.data.middleViewResults.text, displayMsg, len);
    /* send message to Gui */
    if (displayParameters.font == SMALL) {
        msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT;
    } else {
        msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    }
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
}

void
StateDisplayAmsReport::
Enter (void)
{
 
    wchar_t                    topLabel [128];
  
    fileSize = GetFileSize (displayParameters.fileName);
    
    printf ("filesize = %d\n", fileSize);

    totallReadLen = 0;
    mPage         = 0;

     if (fileSize > DIS_PAGE_SIZE) {
        mPage = 1;
        lastPgLen = fileSize % DIS_PAGE_SIZE;
        totalPages = (fileSize / DIS_PAGE_SIZE) + 1;
        pageNumber = 1;
        totallReadLen = 0;
        wsprintf (topLabel,L"%s      %d/%d", displayParameters.topLabel, pageNumber, totalPages);
    } else {
        wsprintf (topLabel,L"%s", displayParameters.topLabel);
    }
    SetTopLabel (topLabel);

    // set bottom buttons
    if (mPage == 0) {
        SetBottomButtons (strings [BSTR_BACK], true);
    } else {
        SetBottomButtons (strings [BSTR_BACK], false, L"PgUp", true, L"PgDn", true);
    }
    SendReportMessage ();
}

state_t
StateDisplayAmsReport::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_AMS_DISPLAY_REPORT;

    int     idata = ((GuiSendMsgData_t *)data)->idata;
    wchar_t topLabel [128];

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            if (mPage == 0) {
                retVal = stateStack.Pop();
            } else {
                // more than one pages to displa
                switch (idata) {
                    case 0:     // Back
                        retVal = stateStack.Pop();;
                    break;

                    case 1:     //page up
                        if (totallReadLen >= 0) {
                            // set current page first line
                            if (lastPgLen > 0) {
                                if (totallReadLen > lastPgLen) {
                                    totallReadLen -= lastPgLen;
                                } else {
                                    totallReadLen = 0;
                                }
                                lastPgLen = 0;
                            } else {
                                if (totallReadLen > DIS_PAGE_SIZE) {
                                    totallReadLen -= DIS_PAGE_SIZE;
                                } else {
                                    totallReadLen = 0;
                                }
                            }
                            /* move back one page */
                            if (totallReadLen > DIS_PAGE_SIZE) {
                                totallReadLen -= DIS_PAGE_SIZE;
                            } else {
                                totallReadLen = 0;
                            }
                            if (pageNumber > 1) {
                                pageNumber--;
                            }
                            wsprintf (topLabel,L"%s      %d/%d", displayParameters.topLabel, pageNumber, totalPages);
                            SetTopLabel (topLabel);
                            SendReportMessage ();
                        }
                    break;

                    case 2:     //page down
                        if (totallReadLen < fileSize) {
                            if (pageNumber < totalPages) {
                                pageNumber++;
                            }
                            wsprintf (topLabel,L"%s      %d/%d", displayParameters.topLabel, pageNumber, totalPages);
                            SetTopLabel (topLabel);
                            SendReportMessage ();
                        }
                    break;


                    default:
                    break;
                }
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;

    }
    return retVal;
}
