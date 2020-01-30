#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsAlignmentTestStates.h"
#include "HeaterControlMessages.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"

AlignmentErrorReport_t    StateAmsAlignmentTestGlobal::alignmentErrorReport;
unsigned    char          StateAmsAlignmentTestGlobal::alignmentDac;
unsigned    char          StateAmsAlignmentTestGlobal::barCount[10];


//Ams Test process  
void
StateAmsAlignmentTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Ams Alignment Test";
    wchar_t                 *str = L"Load test rotor";
    
    memset (&amsAlignmentTestFlags, 0, sizeof AmsAlignmentTestFlags_t);

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
StateAmsAlignmentTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_ALIGNMENT_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsAlignmentTestFlags.startTestFlag == 0) {
                        SendDrawerMessage ('C');
                        //retVal = STATE_MANUFACTURING;
                         retVal =  stateStack.Pop();
                    }
                break;

                case 1: // start test
                    if ((amsAlignmentTestFlags.startTestFlag == 0) && 
                        (amsAlignmentTestFlags.drawerReadyFlag == 1)) {
                         amsAlignmentTestFlags.startTestFlag = 1;
                        stateStack.Push (STATE_AMS_ALIGNMENT_TEST);
                        retVal = STATE_AMS_ALIGNMENT_TEST_IN_PROGRESS;
                    }
                break;

                case 2: //open close drawer
                    if (amsAlignmentTestFlags.startTestFlag == 0) {
                        if (amsAlignmentTestFlags.drawerOpenFlag) {
                            SendDrawerMessage ('C');
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
                             amsAlignmentTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsAlignmentTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsAlignmentTestFlags.drawerOpenFlag = 1;
                        amsAlignmentTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_ALIGNMENT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

SystemErrNum_t
StateAmsAlignmentTestInProgress::
StartAlignmentTest (void)
{
      

    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);

    // disable watchdog before start test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);

    // start barcode alignment calibration and reading
    SendAlignmentTestMessage ('C', 0);

    return SERR_NONE;
}

//Ams temperature Test progress 
void
StateAmsAlignmentTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Alignment Test Progress";
    
    progBarValue = 0;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message,  L"Barcode Alignment Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    StartAlignmentTest ();
    SetTimer (1);
}

// Ams temperature test process
state_t
StateAmsAlignmentTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_ALIGNMENT_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;
    int                         i;
    unsigned short              *barCountMin;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_ALIGNMENT_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'I':  //get reading feed back
                    // enable heater polling
                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    memset (&alignmentErrorReport, 0 , sizeof (AlignmentErrorReport_t));
                    // get dac value
                    alignmentDac = engineCommMsg->message[5];
                    //get code alignment bar count
                    barCountMin = &manufacturingTestSet.alignmentF1Min;
                    for (i = 0; i < MAX_BAR_ACOUNT; i++) {
                        barCount [i] = engineCommMsg->message[8 +i];
                        if (barCount [i] < *barCountMin) {
                            // errors
                            alignmentErrorReport.error |= (1 << i);
                        }
                        if (i == (MAX_BAR_ACOUNT /2) -1){ 
                            barCountMin = &manufacturingTestSet.alignmentF1Min;
                        } else {
                            barCountMin++;
                        }
                    } 
                    SendDrawerMessage ('O');
                    retVal = STATE_AMS_ALIGNMENT_TEST_RESULTS_CAL;
                break;

                default:
                    ProcessOtherMessages (msgId, data);
                break;
            }

        break;

        case MSG_MANUFACTURE_TIMER:
            updateProgBar = 1;
            progBarValue += 30;
            //enable timer for progress bar update
            SetTimer (2);
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
        // someting wrong, open drawer
        SendDrawerMessage ('O');
        infoText.type = AMS_ALIGNMENT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams  Test Canceling 
void
StateAmsAlignmentTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Alignment Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsAlignmentTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_ALIGNMENT_TEST_CANCEL;
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
                            //open the drawer
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
        infoText.type = AMS_ALIGNMENT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams  Test Results display 
void
StateAmsAlignmentTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Alignment Test Results";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    //msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsAlignmentTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_ALIGNMENT_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_ALIGNMENT_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_ALIGNMENT_TEST_SHOW_REPORTS;
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


//Ams  Test Results display
void
StateAmsAlignmentTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Alignment Test Reports";
    FILE                    *fp;
    int                     len;
	char					buff [4000];

    SetTopLabel (topLable);

    fp = fopen (AMS_ALIGNMENT_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_ALIGNMENT_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open ams alignment report file");
    } else {
		memset (buff, 0, 4000);
        len = fread (buff, sizeof(char), 4000, fp);
		mbstowcs (msg.data.middleViewResults.text, buff, 4000);
        //len = fread (msg.data.middleViewResults.text, sizeof(char), 4000, fp);
        LogDebug ("Len = %d", len);
        fclose (fp);
    }

   // msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT;
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);
}

state_t
StateAmsAlignmentTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_ALIGNMENT_TEST_SHOW_REPORTS;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;

    }
    return retVal;
}
