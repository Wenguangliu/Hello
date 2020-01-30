#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsNdxtTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// create global flag 
AmsNdxtTestFlags_t StateAmsNdxtTest::amsNdxtTestFlags;

#define NDXT_TEST_RIS_FILE     "/Hard Disk/Abaxis/Ris/ndxt.ris"

SystemErrNum_t
StateAmsNdxtTest::
StartNdxtTest (void)
{
    SystemErrNum_t err;

    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);

    // start progress bar at 6
    progBarValue = 0;

    // download Ris file
    err = SendRisFile (NDXT_TEST_RIS_FILE);
    if (err != SERR_NONE) {
        return err;
    }

    // disable watchdog before start test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    // start  test
    SendAMessage ('M');

    return SERR_NONE;
}

//Ams  Test process  
void
StateAmsNdxtTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                    *topLable = L"NDXT Test";
    wchar_t                    *str = L"Load NDXT rotor";
    
    memset (&amsNdxtTestFlags, 0, sizeof AmsNdxtTestFlags_t);

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
StateAmsNdxtTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_NDXT_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    SystemErrorNumber_t         errNum;
	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsNdxtTestFlags.ndxtStartTestFlag == 0) {
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                       // retVal = STATE_MANUFACTURING;
                    }
                break;

                case 1: // start ndxt test
                    if ((amsNdxtTestFlags.ndxtStartTestFlag == 0) && 
                        (amsNdxtTestFlags.drawerReadyFlag == 1)) {
                         amsNdxtTestFlags.ndxtStartTestFlag = 1;
                         errNum = StartNdxtTest ();
                         if (errNum != SERR_NONE) {
                            LogDebug ("Start error =%d", errNum);
                            analyzeError = errNum;
                         } else {
                             stateStack.Push (STATE_AMS_NDXT_TEST);
                            // goto next state
                            retVal = STATE_AMS_NDXT_TEST_IN_PROGRESS;
                         }
                    }
                break;

                case 2: //open close drawer
                    if (amsNdxtTestFlags.ndxtStartTestFlag == 0) {
                        if (amsNdxtTestFlags.drawerOpenFlag) {
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
                             amsNdxtTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsNdxtTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsNdxtTestFlags.drawerOpenFlag = 1;
                        amsNdxtTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_NDXT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


//Ams NDXT Test progress 
void
StateAmsNdxtTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"NDXT Test Progress";
    
    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"NDXT Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);

}

// Ams test process
state_t
StateAmsNdxtTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_NDXT_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_NDXT_TEST_CANCEL;
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
                            updateProgBar = 1;
                            progBarValue += 3;
                        break;

                        case BLACK_OFFSETS:
                            updateProgBar = 1;
                            progBarValue += 3;
                            LogDebug ("Black Offset done");
                        break;

                        case NDXT_TEST_DONE:
                            updateProgBar = 1;
                            progBarValue = 100;
                            // enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            //LogDebug ("NDXT test done");
                            SendDrawerMessage ('O');
                            retVal = STATE_AMS_NDXT_TEST_RESULTS_CAL;
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

        case MSG_ENGINE_COMM_OUT_MESSAGE_DATA:
            //update progress bar 
            progBarValue += 3;
            updateProgBar = 1;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // update progress bar
    if (updateProgBar == 1) {
        updateProgBar = 0;
        msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
        msg.data.middleProgBarMessage.progBarValue = progBarValue;
        SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        // in the case of error, open the drawer
         SendDrawerMessage ('O');
        infoText.type = AMS_NDXT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams NDXT Test Canceling 
void
StateAmsNdxtTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"NDXT Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsNdxtTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_NDXT_TEST_CANCEL;
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
        infoText.type = AMS_NDXT_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams NDXT Test Results Calculation 
void
StateAmsNdxtTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"NDXT Test Results Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();
    NdxtTestDataCheck (rawData);
    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_NDXT_TEST_CAL_COMPLETE);
}

state_t
StateAmsNdxtTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_NDXT_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_NDXT_TEST_CAL_COMPLETE:
            FormatTestReport ();
            retVal = STATE_AMS_NDXT_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}


//Ams NDXT Test Results display 
void
StateAmsNdxtTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"NDXT Test Results";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    //msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsNdxtTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_NDXT_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_NDXT_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_NDXT_TEST_SHOW_REPORTS;
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


//Ams NDXT Test Results display
void
StateAmsNdxtTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"NDXT Test Reports";
    FILE                    *fp;
    int                     len;
    char                    buff [4000];

    SetTopLabel (topLable);

    fp = fopen (AMS_NDXT_DISP_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_NDXT_DISP_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open NDXT test report file");
    } else {
        memset (buff, 0, 4000);
        len = fread (buff, sizeof(char), 4000, fp);
        mbstowcs (msg.data.middleViewResults.text, buff, 4000);
        //len = fread (msg.data.middleViewResults.text, sizeof(char), 4000, fp);
        fclose (fp);
    }

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT;
// msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);

}

state_t
StateAmsNdxtTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_NDXT_TEST_SHOW_REPORTS;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
#ifndef NDXT_DEBUG
            retVal = stateStack.Pop();
#else
            retVal = STATE_MANUFACTURING;
#endif
        break;


        default:
            ProcessOtherMessages (msgId, data);
        break;


    }
    return retVal;
}
