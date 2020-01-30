#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsMotorTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// create share memory
AmsMotorTestFlags_t StateAmsMotorTest::amsMotorTestFlags;

#define MOTOR_TEST_RIS_FILE     "/Hard Disk/Abaxis/Ris/motor.ris"

SystemErrNum_t
StateAmsMotorTest::
StartMotorTest (void)
{
    SystemErrNum_t err;

    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);

    progBarValue = 0;

    // download Ris file
    err = SendRisFile (MOTOR_TEST_RIS_FILE);
    if (err != SERR_NONE) {
        return err;
    }

    // disable watchdog before start motor test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    // start motor test
    SendAMessage ('M');

    return SERR_NONE;
}

//Ams Motor Test process  
void
StateAmsMotorTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test";
    wchar_t                 *str = L"Load rotor";
    
    memset (&amsMotorTestFlags, 0, sizeof AmsMotorTestFlags_t);

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
StateAmsMotorTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_MOTOR_TEST;
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
                    if (amsMotorTestFlags.motorStartTestFlag == 0) {
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                       // retVal = STATE_MANUFACTURING;
                    }
                break;

                case 1: // start motor test1
#ifndef MOTOR_DEBUG 
                    if ((amsMotorTestFlags.motorStartTestFlag == 0) && 
                        (amsMotorTestFlags.drawerReadyFlag == 1)) {
                         amsMotorTestFlags.motorStartTestFlag = 1;
                         errNum = StartMotorTest ();
                         if (errNum != SERR_NONE) {
                            analyzeError = errNum;
                         } else {
                             stateStack.Push (STATE_AMS_MOTOR_TEST);
                            // goto next state
                            retVal = STATE_AMS_MOTOR_TEST_IN_PROGRESS;
                         }
                    }
#else
                    retVal = STATE_AMS_MOTOR_TEST_RESULTS_CAL;
#endif
                break;

                case 2: //open close drawer
                    if (amsMotorTestFlags.motorStartTestFlag == 0) {
                        if (amsMotorTestFlags.drawerOpenFlag) {
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
                             amsMotorTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsMotorTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsMotorTestFlags.drawerOpenFlag = 1;
                        amsMotorTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_MOTOR_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


//Ams Motor Test progress 
void
StateAmsMotorTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test Progress";
    
    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"Spindle Motor Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);

}

// Ams test process
state_t
StateAmsMotorTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_MOTOR_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_MOTOR_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':  //progress command
                    switch (engineCommMsg->message[8]) {
                        case MOTOR_TEST_DONE:
                            progBarValue = 100;
                            msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
                            msg.data.middleProgBarMessage.progBarValue = progBarValue;
                            SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
                            // enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            LogDebug ("Motor test done");
                            // open drawer
                            SendDrawerMessage ('O');
                            retVal = STATE_AMS_MOTOR_TEST_RESULTS_CAL;
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

        case MSG_ENGINE_COMM_OUT_MESSAGE_DATA:
            //update progress bar 
            progBarValue += 4;
            msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
            msg.data.middleProgBarMessage.progBarValue = progBarValue;
            SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        // something wrong, open the drawer
        SendDrawerMessage ('O');
        infoText.type = AMS_MOTOR_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams Motor Test Canceling 
void
StateAmsMotorTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsMotorTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_MOTOR_TEST_CANCEL;
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
        infoText.type = AMS_MOTOR_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams Motor Test Results Calculation 
void
StateAmsMotorTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test Results Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();
    MotorDataCheck (rawData);
    // ZZZ call calculation function here
    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_MOTOR_TEST_CAL_COMPLETE);
}

state_t
StateAmsMotorTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_MOTOR_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_MOTOR_TEST_CAL_COMPLETE:
            retVal = STATE_AMS_MOTOR_TEST_SHOW_RESULTS;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}


//Ams Motor Test Results display 
void
StateAmsMotorTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t					*topLable = L"Motor Test Results";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    //msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsMotorTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_MOTOR_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
              switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;

                case 1:
                    stateStack.Push (STATE_AMS_MOTOR_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_MOTOR_TEST_SHOW_REPORTS;
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


void
StateAmsMotorTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test Reports";
    FILE                    *fp;
	int						len;
    char                    buff [4000];

    SetTopLabel (topLable);

    fp = fopen (AMS_MOTOR_TEST_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_MOTOR_TEST_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open motor test report file");
    } else {
        memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
        memset (buff, 0, 4000);
        len = fread (buff, sizeof(char), 4000, fp);
        mbstowcs (msg.data.middleViewResults.text, buff, 4000);
        //fread (msg.data.middleViewResults.text, 4000, 1, fp);
        fclose (fp);
    }
  //  msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true);

}

state_t
StateAmsMotorTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_MOTOR_TEST_SHOW_RESULTS;

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
