#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsCuvetteTestStates.h"
#include "HeaterControlMessages.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// create global flag 
AmsCuvetteTestFlags_t       StateAmsCuvetteTest::amsCuvetteTestFlags;
CuvetteRotor_t              StateCuvetteTestGlobal::cuvetteRotor;
unsigned short              StateCuvetteTestGlobal::cuvetteData [MAX_CUVETTE_LOOP];
CuvetteCalResults_t         StateCuvetteTestGlobal::cuvetteCalResults;
CuvetteFinalErrorReport_t   StateCuvetteTestGlobal::cuvetteFinalReport [END_CUV_ERROR];

CuvetteRotor_t        defaultCuvetteRotorTable  [] = 
                        {
                            {33,                0.938,  68.500},
                            {40,                1.032,  -19.450},
                            {41,                1.023,  -22.181},
                            {42,                1.032,  -34.312}, 
                            {43,                1.014,   9.189},
                            {44,                1.023,  -15.636},
                            {45,                1.014,  14.054},
                            {46,                1.023,  5.636},
                            {47,                1.014,  12.432},
                            {48,                1.022,  8.909},
                            {49,                1.004,  43.214},
                            {50,                0.987,  16.316},
                            {51,                1.014,  20.540},
                            {52,                1.051,  -13.831},
                            {53,                1.023,  2.363},
                            {54,                1.061,  -16.038},
                            {55,                1.014,  2.702},
                            {56,                1.000,  22.478},
                            {57,                1.023,  -0.909},
                            {58,                1.041,  -8.333},
                            {59,                1.023,  22.000},
                            {60,                1.000,  20.884},
                            {61,                1.032,  11.927},
                            {62,                1.042, -20.000},
                            {63,                1.051, -15.514},
                            {MAX_CUVETTE_ROTOR, 0.000,  0.000},
                        };


//Ams  Test process  
void
StateAmsCuvetteTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test";
    wchar_t                 *str = L"Please load rotor";
    
    memset (&amsCuvetteTestFlags, 0, sizeof AmsCuvetteTestFlags_t);

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
StateAmsCuvetteTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_CUV_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsCuvetteTestFlags.cuvetteStartTestFlag == 0) {
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                       // retVal = STATE_MANUFACTURING;
                    }
                break;

                case 1: // start cuvette test
                    if ((amsCuvetteTestFlags.cuvetteStartTestFlag == 0) && 
                        (amsCuvetteTestFlags.drawerReadyFlag == 1)) {
                         amsCuvetteTestFlags.cuvetteStartTestFlag = 1;
                        stateStack.Push (STATE_AMS_CUV_TEST);
                        retVal = STATE_AMS_CUV_TEST_INPUT_ROTOR_ID;
                    }
                break;

                case 2: //open close drawer
                    if (amsCuvetteTestFlags.cuvetteStartTestFlag == 0) {
                        if (amsCuvetteTestFlags.drawerOpenFlag) {
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
                             amsCuvetteTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsCuvetteTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        wcscpy (msg.data.middleMessage.message,  L"Please load rotor");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsCuvetteTestFlags.drawerOpenFlag = 1;
                        amsCuvetteTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_CUV_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


//input valid  test rotor id 
void
StateAmsCuvetteTestInputRotorId::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Input NDXT Test Rotor ID";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
    msg.data.middleEnterId.isPassword = 0;
    wcscpy (msg.data.middleEnterId.initialValue, L"");
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIdData_t));
    SetBottomButtons (strings[BSTR_BACK], false, strings [BSTR_DONE], true);
}

// Ams test process
state_t
StateAmsCuvetteTestInputRotorId::
ProcessMessage (int msgId, char *data)
{
    int     retVal = STATE_AMS_CUV_TEST_INPUT_ROTOR_ID;
    int     idata = *(int *)data;
    char *  sdata = (char *)data;
    char    rotorIdStr[10];

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {

                case 0:     // Back
                    retVal = stateStack.Pop();
                break;

                case 1:     // Done
                    SendGuiMessage (MSG_GUI_GET_ID_TEXT);
                break;

                default:
                break;
            }
            break;
        break;

        case MSG_GUI_OUT_ID_TEXT:
            // get RotorId
            memset (rotorIdStr, 0 , 10);
            strcpy (rotorIdStr, sdata);
            cuvetteRotor.rotorId = atoi (rotorIdStr); 
            //now confirm valid rotor id number
            if (CheckValidRotor ()  == -1) {
                stateStack.Push (STATE_AMS_CUV_TEST_INPUT_ROTOR_ID);
                retVal = STATE_AMS_CUV_TEST_WRONG_ROTOR_ID;
            }  else {
                retVal = STATE_AMS_CUV_TEST_IN_PROGRESS;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

// check if it is valid tempperature rotor  
int
StateAmsCuvetteTestInputRotorId::
CheckValidRotor (void)
{

    int i;

    for (i = 0; i < MAX_CUVETTE_ROTOR; i++) {
        if ((defaultCuvetteRotorTable [i].rotorId == cuvetteRotor.rotorId) || 
            (defaultCuvetteRotorTable [i].rotorId == MAX_CUVETTE_ROTOR)) {
            break;
        }
    }
    if (defaultCuvetteRotorTable [i].rotorId != MAX_CUVETTE_ROTOR) {
        cuvetteRotor.slope = defaultCuvetteRotorTable [i].slope;
        cuvetteRotor.intercept = defaultCuvetteRotorTable [i].intercept;
        return 0;
    } else {
        LogDebug ("Fail to find out cuvette rotor Id: %d", cuvetteRotor.rotorId);
    }
    return -1;
}

//input wrong test rotor id 
void
StateAmsCuvetteTestWrongRotorId::
Enter (void)
{
    GuiReceiveMsg_t         msg;

    msg.msgId = MSG_GUI_WIN_WARNING1;
    wcscpy (msg.data.winError.topLabel, L"Rotor ID Error");
    wsprintf(msg.data.winError.message, L"%s\n", L"Input Wrong Rotor ID");
    wcscpy (msg.data.winError.buttonText, strings[BSTR_CONTINUE]);
    SendGuiMessage (&msg, sizeof (GuiWinWarningData_t));
}

//  
state_t
StateAmsCuvetteTestWrongRotorId::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_AMS_CUV_TEST_WRONG_ROTOR_ID;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:     // Continue to ip input state
            retVal =  stateStack.Pop();
        break;
        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

SystemErrNum_t
StateAmsCuvetteTestInProgress::
StartCuvetteTest (void)
{

    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);

    SendCuvetteDetectionCommand ('K');

    // reset loop counter
    testLoopCount = 0;

    return SERR_NONE;
}

//Ams test in progress 
void
StateAmsCuvetteTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test Progress";
    
    SetTopLabel (topLable);

    // reset progress bar to be zero
    progBarValue = 0;
    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Testing ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"Cuvette Test");
    // rotor1 white color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    StartCuvetteTest ();
     

}

// Ams test in process
state_t
StateAmsCuvetteTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_CUV_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;
    unsigned char               loop;
    FILE                        *fp;
    char                        buf [64];
    CuvetteTestData_t           cuvetteTestData;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_CUV_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'Q':  //progress command
                    loop = engineCommMsg->message[6];
                    if (loop < MAX_CUVETTE_LOOP) {
                        memcpy (&cuvetteTestData, &engineCommMsg->message[6], sizeof (CuvetteTestData_t));
                        // multiple by 1.6 to  convet real ns time
                        cuvetteData [loop] = (unsigned short)(cuvetteTestData.cuvetteWidth [0] * 1.6);
                        progBarValue = loop * 13;
                        updateProgBar = 1;
                        if (loop == MAX_CUVETTE_LOOP - 1) {
                            // enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            LogDebug ("Cuvette test done");
                            updateProgBar = 1;
                            progBarValue = 100;
                            // save raw data
                            fp = fopen (AMS_CUVETTE_DATA_FILE, "wb");
                            if (fp != NULL) {
                                // save test data
                                for (loop = 0; loop < MAX_CUVETTE_LOOP; loop++) {
                                    memset (buf, 0, sizeof (buf));
                                    sprintf (buf, "%u\n", cuvetteData [loop]); 
                                    fwrite (buf, 1, strlen (buf), fp);
                                }
                                fclose (fp);
                            } else {
                                LogError ("Fail to open %s", AMS_CUVETTE_DATA_FILE);
                            }
                            // open drawer
                            SendDrawerMessage ('O');
                            retVal = STATE_AMS_CUV_TEST_RESULTS_CAL;
                        }
                    } else {
                        wsprintf (displayText, L"Get invalid loop number %d", loop);
                        retVal = STATE_AMS_CUV_TEST_SHOW_RESULTS;
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

    // update progress bar
    if (updateProgBar == 1) {
        updateProgBar = 0;
        msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
        msg.data.middleProgBarMessage.progBarValue = progBarValue;
        SendGuiMessage (&msg, sizeof( GuiMiddleProgBarMessageData_t));
    }

    // something wrong
    if (analyzeError != SERR_NONE) {
        // something wrong, open the drawer
        SendDrawerMessage ('O');
        infoText.type = AMS_CUV_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams Cuvette Test Canceling 
void
StateAmsCuvetteTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsCuvetteTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_CUV_TEST_CANCEL;
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
        infoText.type = AMS_CUV_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//Ams Cuvette Test Results display 
void
StateAmsCuvetteTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test Results";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsCuvetteTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_CUV_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_CUV_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_CUV_TEST_SHOW_REPORTS;
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



//Ams  Cuvette Test Report display
void
StateAmsCuvetteTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Cuvette Test Reports";
    FILE                    *fp;
    int                     len;
	char					buff [4000];

    SetTopLabel (topLable);

    fp = fopen (AMS_CUVETTE_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_CUVETTE_DATA_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open cuvette test report file");
    } else {
		memset (buff, 0, 4000);
        len = fread (buff, sizeof(char), 4000, fp);
		mbstowcs (msg.data.middleViewResults.text, buff, 4000);
        //len = fread (msg.data.middleViewResults.text, sizeof(char), 4000, fp);
        fclose (fp);
    }

   // msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT;
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);

}

state_t
StateAmsCuvetteTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_CUV_TEST_SHOW_REPORTS;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        //    retVal = STATE_MANUFACTURING;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;

    }
    return retVal;
}
