#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsDelayTestStates.h"
#include "HeaterControlMessages.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// create global flag 
AmsDelayTestFlags_t StateAmsDelayTest::amsDelayTestFlags;
unsigned short      StateDelayTestGlobal::avgData [MAX_DELAY_NUM][MAX_CHANNEL];
unsigned int        StateDelayTestGlobal::error1 [MAX_CHANNEL];
unsigned int        StateDelayTestGlobal::error2 [MAX_DELAY_NUM][MAX_CHANNEL]; 
DelayErrorReport_t  StateDelayTestGlobal::delayErrorReport;

// static variables
int             StateAmsDelayTestShowReports::pageNumber = 0;
int             StateAmsDelayTestShowReports::totalPages = 0;
int             StateAmsDelayTestShowReports::totallReadLen = 0;
//wchar_t         StateAmsDelayTestShowReports::delayMsg [AMS_PAGE_SIZE];
int             StateAmsDelayTestShowReports::fileSize;
int             StateAmsDelayTestShowReports::lastPgLen;
char            StateAmsDelayTestShowReports:: mPage = 0;


#define DELAY_TEST_RIS_FILE              "/Hard Disk/Abaxis/Ris/delay.ris"


//Ams  Test process  
void
StateAmsDelayTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Global Delay Test";
    wchar_t                 *str = L"Load test rotor";
    
    memset (&amsDelayTestFlags, 0, sizeof AmsDelayTestFlags_t);

    SetTopLabel (topLable);

    // try to get current drawer status
    SendDrawerMessage ('S');

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons (strings[BSTR_BACK], false, L"Start", false,strings [BSTR_CLOSE], true);
}

// Ams test process
state_t
StateAmsDelayTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_DELAY_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsDelayTestFlags.startTestFlag == 0) {
                        SendDrawerMessage ('M');
                        retVal = stateStack.Pop();
                        //retVal = STATE_MANUFACTURING;
                    }
                break;

                case 1: // start temperature test
                    if ((amsDelayTestFlags.startTestFlag == 0) && 
                        (amsDelayTestFlags.drawerReadyFlag == 1)) {
                         amsDelayTestFlags.startTestFlag = 1;
                        stateStack.Push (STATE_AMS_DELAY_TEST);
                        //LogDebug ("goto rotor id");
                        retVal = STATE_AMS_DELAY_TEST_IN_PROGRESS;
                    }
                break;

                case 2: //open close drawer
                    if (amsDelayTestFlags.startTestFlag == 0) {
                        if (amsDelayTestFlags.drawerOpenFlag) {
                            SendDrawerMessage ('M');
                        } else {
                            SendDrawerMessage ('O');
                        }
                    }
                break;
                
                default:
                break;
            }

        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'D':  // get Drawer command 
                    status = engineCommMsg->message[8];
                    if (status & 0x80) {//drawer jammed
                        analyzeError = SERR_DRAWER_CLOSE_JAM;
                        LogError("drawer jammed");
                        wcscpy (msg.data.middleMessage.message, L"Drawer Jammed");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                    } else if (status & 0x10) {//drawer closed
                        if (status & 0x40) {//rotor present
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Ready to start test");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                             amsDelayTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsDelayTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsDelayTestFlags.drawerOpenFlag = 1;
                        amsDelayTestFlags.drawerReadyFlag = 0;
                    }
                break;

                default:
                    ProcessOtherMessages (msgId, data);
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        infoText.type = AMS_DELAY_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

SystemErrNum_t
StateAmsDelayTestInProgress::
StartDelayTest (void)
{
    SystemErrNum_t err;

    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);


    // download Ris file
    err = SendRisFile (DELAY_TEST_RIS_FILE);
    if (err != SERR_NONE) {
        return err;
    }

    // disable watchdog before start test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    // start  test
    SendAMessage ('M');

    //enable timer for progress bar update 
    SetTimer (15);

    return SERR_NONE;
}

//Ams temperature Test progress 
void
StateAmsDelayTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Global Delay Test Progress";
    
    //start progress bar at  0
    progBarValue = 0;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"Global Delay Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    StartDelayTest ();
     

}

// Ams temperature test process
state_t
StateAmsDelayTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_DELAY_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_DELAY_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':  //progress command
                    switch (engineCommMsg->message[8]) {
                        case OPTICAL_DACS:
                            // send queue to Calculation process to get trim dac value
                            SendCalcResultsMessage (MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE_MFP);
                            LogDebug ("Get Trim Dac Raw data");
                        break;

                        case BLACK_OFFSETS:
                            LogDebug ("Black Offset Done");
                        break;

                        case CUV_DELAY_TEST_DONE:
                            // disable timer
                            CloseTimer ();
                            updateProgBar = 1;
                            progBarValue = 100;
                            // enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            LogDebug ("Temperature test done");
                            // open drawer
                            SendDrawerMessage ('O');
                            retVal = STATE_AMS_DELAY_TEST_RESULTS_CAL;
                        break;

                        default:
                            ProcessOtherMessages (msgId, data);
                        break;
                    }
                break;

                default:
                    ProcessOtherMessages (msgId, data);
                break;

            }

        break;

         case MSG_MANUFACTURE_DAC_TRIMS_COMPLETE:
            //calculation process send back with new trim dac values
            SendCalibrationFactorsMessage ();
            LogDebug ("Finish Trim Dac Calibration");
        break;


        case MSG_MANUFACTURE_TIMER:
            updateProgBar = 1;
            progBarValue += 8;
            //enable timer for progress bar update
            SetTimer (5);
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // update progress bar
    if (updateProgBar == 1) {
        updateProgBar = 0;
        //LogDebug ("progBarVaule =%d", progBarValue);
        msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
        msg.data.middleProgBarMessage.progBarValue = progBarValue;
        SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        // error, open the drawer 
        SendDrawerMessage ('O');
        infoText.type = AMS_DELAY_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams Global Delay Test Canceling 
void
StateAmsDelayTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Global Delay Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsDelayTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_DELAY_TEST_CANCEL;
    EngineCommMessageData_t *   engineCommMsg;
  
    switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':  //progress command
                    switch (engineCommMsg->message[8]) {
                        case CANCEL_DONE:
                            // Enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            // open drawer
                            SendDrawerMessage ('O');
                            retVal = stateStack.Pop();
                        break;

                        default:
                            ProcessOtherMessages (msgId, data);
                        break;
                    }
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        infoText.type = AMS_DELAY_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams Delay Test Results display 
void
StateAmsDelayTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Global Delay Test Results";
	 
    SetTopLabel (topLable);

    //msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsDelayTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_DELAY_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_DELAY_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_DELAY_TEST_SHOW_REPORTS;
                break;

                default:
                break;
            }
        break;


        default:
            ProcessOtherMessages (msgId, data);
        break;


    }
    return retVal;
}


// send AMS global delay test  report to console
void
StateAmsDelayTestShowReports::
SendDelayMessage (void)
{
    GuiReceiveMsg_t     msg;
    FILE                *fp;
    int                 len;
    char                buff [4000];

    // clear buffer
    memset (buff, 0, sizeof (buff));

    fp = fopen (AMS_DELAY_REPORT_FILE, "rb");

    if (fp == NULL) {
        LogError ("Fail to open %s", AMS_DELAY_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Could not be able to open ams delay report file");
    } else {
        //set file read point
        if (fseek (fp, totallReadLen, SEEK_SET) == 0) {
            len = fread (buff, 1, AMS_PAGE_SIZE, fp);
            totallReadLen += len;
            if (totallReadLen >= fileSize) {
                // records last page length
                lastPgLen = len;
            }
        } else {
            LogError ("Fail to set %s postion %d", AMS_DELAY_REPORT_FILE, totallReadLen);
            wcscpy (msg.data.middleViewResults.text, L"Fail to set ams delay report file position");
        }
        fclose (fp);
    }

    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    //wcsncpy (msg.data.middleViewResults.text, delayMsg, len);
    mbstowcs (msg.data.middleViewResults.text, buff, 4000);
    /* send message to Gui */
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
}

//Ams Delay Test Results display
void
StateAmsDelayTestShowReports::
Enter (void)
{
  
    wchar_t                    topLabel [128];
     

    // in the case of error report is larger than max display buffer
    // display should be changed to seperate pages
    fileSize = GetFileSize (AMS_DELAY_REPORT_FILE);
    printf ("filesize = %d\n", fileSize);

    totallReadLen = 0;
    mPage         = 0;

    if (fileSize > AMS_PAGE_SIZE) {
        mPage = 1;
        lastPgLen = fileSize % AMS_PAGE_SIZE;
        totalPages = (fileSize / AMS_PAGE_SIZE) + 1;
        pageNumber = 1;
        totallReadLen = 0;
        wsprintf (topLabel,L"%s      %d/%d", L"Global Delay Test Reports", pageNumber, totalPages);
    } else {
        wsprintf (topLabel,L"%s", L"Global Delay Test Reports");
    }

    // set top lable 
	SetTopLabel (topLabel);

    // set bottom buttons
    if (mPage == 0) {
        SetBottomButtons (strings [BSTR_BACK], true);
    } else {
        SetBottomButtons (strings [BSTR_BACK], false, L"PgUp", true, L"PgDn", true);
    }

    SendDelayMessage ();
}

state_t
StateAmsDelayTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int     retVal = STATE_AMS_DELAY_TEST_SHOW_REPORTS;
    int     idata = ((GuiSendMsgData_t *)data)->idata;
    wchar_t topLabel [128];

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            if (mPage == 0) {
                retVal = stateStack.Pop();
            } else {
                // more than one pages to display error messages
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
                                if (totallReadLen > AMS_PAGE_SIZE) {
                                    totallReadLen -= AMS_PAGE_SIZE;
                                } else {
                                    totallReadLen = 0;
                                }
                            }
                            /* move back one page */
                            if (totallReadLen > AMS_PAGE_SIZE) {
                                totallReadLen -= AMS_PAGE_SIZE;
                            } else {
                                totallReadLen = 0;
                            }
                            if (pageNumber > 1) {
                                pageNumber--;
                            }
                            wsprintf (topLabel,L"%s      %d/%d", L"Global Delay Test Reports", pageNumber, totalPages);
                            SetTopLabel (topLabel);
                            SendDelayMessage ();
                        }
                    break;
                    
                    case 2:     //page down
                        if (totallReadLen < fileSize) {
                            if (pageNumber < totalPages) {
                                pageNumber++;
                            }
                            wsprintf (topLabel,L"%s      %d/%d", L"Global Delay Test Reports", pageNumber, totalPages);
                            SetTopLabel (topLabel);
                            SendDelayMessage ();
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
