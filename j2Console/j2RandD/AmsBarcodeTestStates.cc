#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "RotorBarcode.h"
#include "AmsBarcodeTestStates.h"
#include "HeaterControlMessages.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"

BarcodeData_t           StateAmsBarcodeTestGlobal::barcodeData;
unsigned char           StateAmsBarcodeTestGlobal::barcodeCurDacValue;
BarcodeErrorReport_t    StateAmsBarcodeTestGlobal::barcodeErrorReport;


SystemErrMappingBarcodeErr_t SystemErrMappingBarcodeErrTable [] = 
                {
                    {SERR_NONE,                      NO_BARCODE_ERROR_REPORT},
                    {SERR_BC_END_TRANSITION,         BARCODE_END_TRANSITION_ERR},
                    {SERR_BC_TRANSITIONS,            BARCODE_TRANSITIONS_ERR},
                    {SERR_BC_NO_START,               BARCODE_NO_START_ERR},
                    {SERR_BC_NO_STOP,                BARCODE_NO_STOP_ERR},
                    {SERR_BC_BAD_DECODE,             BARCODE_BAD_DECODE_ERR},
                    {SERR_BC_QUIET_ZONE,             BARCODE_QUIET_ZONE_ERR},
                    {SERR_BC_SET_CURRENT,            BARCODE_SET_CURRENT_ERR},
                    {SERR_BC_READ_TIMEOUT,           BARCODE_READ_TIMEOUT_ERR},
					{SERR_BC_CALIBRATION_TIMEOUT,    BARCODE_CALIBRATION_ERR},
                };

//Ams Test process  
void
StateAmsBarcodeTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Read Test";
    wchar_t                 *str = L"Load test rotor";
    
    memset (&amsBarcodeTestFlags, 0, sizeof AmsBarcodeTestFlags_t);

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
StateAmsBarcodeTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_BARCODE_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsBarcodeTestFlags.startTestFlag == 0) {
                        SendDrawerMessage ('C');
                       // retVal = STATE_MANUFACTURING;
                        retVal =  stateStack.Pop();
                    }
                break;

                case 1: // start test
                    if ((amsBarcodeTestFlags.startTestFlag == 0) && 
                        (amsBarcodeTestFlags.drawerReadyFlag == 1)) {
                         amsBarcodeTestFlags.startTestFlag = 1;
                        stateStack.Push (STATE_AMS_BARCODE_TEST);
                        retVal = STATE_AMS_BARCODE_TEST_IN_PROGRESS;
                    }
                break;

                case 2: //open close drawer
                    if (amsBarcodeTestFlags.startTestFlag == 0) {
                        if (amsBarcodeTestFlags.drawerOpenFlag) {
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
                             amsBarcodeTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsBarcodeTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsBarcodeTestFlags.drawerOpenFlag = 1;
                        amsBarcodeTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_BARCODE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

SystemErrNum_t
StateAmsBarcodeTestInProgress::
StartBarcodeTest (void)
{


    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);

    // disable watchdog before start test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);

    // start barcode calibration and reading
    SendBarcodeTestMessage ('C', 0);

    return SERR_NONE;
}

//Ams temperature Test progress 
void
StateAmsBarcodeTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test Progress";
    
    progBarValue = 0;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
	wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"Barcode Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    StartBarcodeTest ();
    SetTimer (1);
}

BarcodeError_t
StateAmsBarcodeTestInProgress::
BarcodeErrorMappings (SystemErrorNumber_t   sysErr)
{
    int i; 

    for (i = 0; i < END_BARCODE_ERROR -1; i++) {
        if (SystemErrMappingBarcodeErrTable[i].sysError == sysErr) {
            return SystemErrMappingBarcodeErrTable[i].bcError;
        }
    }
    LogDebug ("Fail to map barcode error %x", sysErr);
    return BARCODE_UNKNOWN_ERR;
}
// Ams temperature test process
state_t
StateAmsBarcodeTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_BARCODE_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;
    SystemErrorNumber_t         barcodeErrorNum;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_BARCODE_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {

                case 'B':  //get barcode reading feed back
                    // enable heater polling
                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    barcodeErrorNum = ProcessRotorBarcode ((const unsigned short *)(engineCommMsg->message + BARCODE_DATA_OFFSET), 
                                                            (unsigned short *)(engineCommMsg->message + BARCODE_PARAMETER_OFFSET), 
                                                            &barcodeData, AMS_BARCODE_DATA_FILE);
                    barcodeCurDacValue = (unsigned char)engineCommMsg->message[5];
                    // ingor date error
                    if  ((barcodeErrorNum == SERR_BC_ILLEGAL_DATE) ||
                         (barcodeErrorNum == SERR_IMPROPER_DATE)   ||
                         (barcodeErrorNum == SERR_EXPIRED_ROTOR)) {
                            barcodeErrorNum = SERR_NONE;
                            // record calibration barcode DAC value
                    }
                   barcodeErrorReport.error = BarcodeErrorMappings (barcodeErrorNum);
                   barcodeErrorReport.sysError = barcodeErrorNum;
                   SendDrawerMessage ('O');
                   retVal = STATE_AMS_BARCODE_TEST_RESULTS_CAL;
                break;

                case 'E':
                    // enable heater polling
                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    // error code from Engine
                    barcodeErrorNum = (SystemErrorNumber_t)*(short *)&engineCommMsg->message[8];
                    barcodeErrorReport.error = BarcodeErrorMappings (barcodeErrorNum);
                    barcodeErrorReport.sysError = barcodeErrorNum;
                    SendDrawerMessage ('O');
                    retVal = STATE_AMS_BARCODE_TEST_RESULTS_CAL;
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
        // something wrong, open the drawer
        SendDrawerMessage ('O');
        infoText.type = AMS_BARCODE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams Barcode Test Canceling 
void
StateAmsBarcodeTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsBarcodeTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_BARCODE_TEST_CANCEL;
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
        infoText.type = AMS_BARCODE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams Barcode Test Results display 
void
StateAmsBarcodeTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test Results";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsBarcodeTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_BARCODE_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_BARCODE_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_BARCODE_TEST_SHOW_REPORTS;
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


//Ams Barcode Test Results display
void
StateAmsBarcodeTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test Reports";
    FILE                    *fp;
    int                     len;
    char                    buff [4000];


    SetTopLabel (topLable);

    fp = fopen (AMS_BARCODE_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_BARCODE_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open ams barcode report file");
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
StateAmsBarcodeTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_BARCODE_TEST_SHOW_REPORTS;

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
