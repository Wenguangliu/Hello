#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "AmsTempTestStates.h"
#include "HeaterControlMessages.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// create global flag 
AmsTempTestFlags_t StateAmsTempTest::amsTempTestFlags;
TempRotor_t        StateTempTestGlobal::tempRotor;
float              StateTempTestGlobal::tempData [MAX_TEMP_DATA];
TempCalResults_t   StateTempTestGlobal::tempCalResults;
TempInitValues_t   StateTempTestGlobal::tempInitValues;
TempFinalErrorReport_t StateTempTestGlobal::tempFinalReport [END_TEMP_ERROR];
char               StateTempTestGlobal::errorReport = 0;
unsigned short     StateTempTestGlobal::tempDefaultDac [2];

TempRotor_t        defaultTempRotorTable  [] = 
                        {
                            {35,  +1.286040000, 0}, {40,  -0.551158000, 0}, {41,  -4.225544000, 0}, {49,  -2.388351000, 0},
                            {56,  -6.062736000, 0}, {57,  +4.960420000, 0}, {58,  -6.062736000, 0}, {71,  -3.306947000, 0},
                            {72,  -1.837193000, 0}, {74,  -6.062736000, 0}, {76,  +1.286040000, 0}, {77,  +6.797610000, 0},
                            {78,  -6.062736000, 0}, {79,  -0.551158000, 0}, {80,  -0.551158000, 0}, {81,  -4.225544000, 0},
                            {83,  -2.388351000, 0}, {85,  +3.123230000, 0}, {87,  -6.062736000, 0}, {89,  -4.225544000, 0},
                            {91,  -4.225544000, 0}, {94,  -6.062736000, 0}, {95,  -0.551158000, 0}, {96,  +3.123230000, 0},
                            {100, +4.960420000, 0}, {105, -1.653474000, 0}, {106, -1.469754000, 0}, {107, -4.225544000, 0},
                            {108, -6.062736000, 0}, {109, +3.123230000, 0}, {110, -4.225544000, 0}, {111, -4.225544000, 0},
                            {117, +4.960420000, 0}, {118, +4.960420000, 0}, {119, -2.388351000, 0}, {120, -0.551158000, 0},
                            {121, +1.286040000, 0}, {122, +4.960420000, 0}, {123, +3.123230000, 0}, {124, +4.960420000, 0},
                            {125, +3.123230000, 0}, {126, -0.551580000, 0}, {127, +4.960420000, 0}, {128, -2.388351000, 0},
                            {129, +1.286040000, 0}, {130, -0.551158000, 0}, {131, +4.960420000, 0}, {132, +1.286040000, 0},
                            {133, -0.551158000, 0}, {134, +1.286040000, 0}, {135, +3.123230000, 0}, {136, -0.551158000, 0},
                            {300,0.000, 0}
                        };

#define TEMP_TEST_RIS_FILE              "/Hard Disk/Abaxis/Ris/temp.ris"



// in the case of analzyer is on the warming state (HeaterControl Proess)
// don't allow to enter temperature state
void
StateAmsAnalyWaitWarmup::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    wchar_t				*topLable =  L"Analyzer Warming Up";
    wchar_t             *str = L"Analyzer warming up, please wait ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));

    SetBottomButtons (strings[BSTR_BACK], true);
}

state_t
StateAmsAnalyWaitWarmup::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_AMS_WAIT_ANALY_WARM_UP;
   
    switch (msgId) {
            case MSG_MANUFACTURE_WARMUP_DONE:
                warmupDoneFlag = 1;
                LogDebug ("RandD Received Warmup Done Status");
                retVal = STATE_AMS_TEMP_TEST;
            break;

            case MSG_GUI_OUT_BOTTOM_BUTTON:
               // retVal = STATE_MANUFACTURING;
                retVal = stateStack.Pop();
            break;

            default:
                ProcessOtherMessages (msgId, data);
            break;
    }
    return retVal;
}


//Ams  Test process  
void
StateAmsTempTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperature Calibration";
    wchar_t                 *str = L"Load calibration rotor";
    wchar_t                 *fStr= L"calibration fail\n\n";
    wchar_t                 buf [256];
    wchar_t                 *testCountStr [] = {
                                                    L"No Test",
                                                    L"First ",
                                                    L"Second ",
                                               };
    
    memset (&amsTempTestFlags, 0, sizeof AmsTempTestFlags_t);

    SetTopLabel (topLable);

    // try to get current drawer status
    SendDrawerMessage ('S');

#if 0
    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    memset (buf, 0, sizeof (buf));
    if ((testCount != 0) && (testCount < 3)) {
        strcpy (buf, testCountStr [testCount]); 
        strcat (buf, fStr); 
        strcat (buf, str);
        strcpy (msg.data.middleMessage.message, buf);
    } else {
        strcpy (msg.data.middleMessage.message, str);
    }
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
#else
    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, L"Waiting for drawer status");
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    
    memset (displayText,  0, sizeof (displayText));
    if ((testCount != 0) && (testCount < 3)) {
        wcscpy (displayText, testCountStr [testCount]); 
        wcscat (displayText, fStr); 
        wcscat (displayText, str);
    } else {
        wcscpy (displayText, str);
    }
    
#endif

    SetBottomButtons (strings[BSTR_BACK], false, L"Start", false,strings [BSTR_CLOSE], true);
}

// Ams test process
state_t
StateAmsTempTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_TEMP_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
 	unsigned char				status;
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    //Back, always close drawer
                    if (amsTempTestFlags.tempStartTestFlag == 0) {
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                       // retVal = STATE_MANUFACTURING;
                    }
                break;

                case 1: // start temperature test
                    if ((amsTempTestFlags.tempStartTestFlag == 0) && 
                        (amsTempTestFlags.drawerReadyFlag == 1)) {
                         amsTempTestFlags.tempStartTestFlag = 1;
                        stateStack.Push (STATE_AMS_TEMP_TEST);
                        //LogDebug ("goto rotor id");
                        retVal = STATE_AMS_TEMP_TEST_INPUT_ROTOR_ID;
                    }
                break;

                case 2: //open close drawer
                    if (amsTempTestFlags.tempStartTestFlag == 0) {
                        if (amsTempTestFlags.drawerOpenFlag) {
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
                    LogDebug ("get status =%x", status);
                    if (status & 0x80) {//drawer jammed
                        analyzeError = SERR_DRAWER_CLOSE_JAM;
                        LogError("drawer jammed");
                        msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                        wcscpy (msg.data.middleMessage.message, L"Drawer Jammed");
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                    } else if (status & 0x10) {//drawer closed
                        if (status & 0x40) {//rotor present
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Ready to start test");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                             amsTempTestFlags.drawerReadyFlag = 1;
                        } else {
                             msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                             wcscpy (msg.data.middleMessage.message, L"Please open drawer and load rotor");
                             SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        }
                        amsTempTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_OPEN], true);
                    } else if (status & 0x20) { // drawer open
                        msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
                        //strcpy (msg.data.middleMessage.message,  "Please load clibration rotor");
                        wcscpy (msg.data.middleMessage.message,  displayText);
                        SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, strings [BSTR_CLOSE], true);
                        amsTempTestFlags.drawerOpenFlag = 1;
                        amsTempTestFlags.drawerReadyFlag = 0;
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
        infoText.type = AMS_TEMP_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


//input valid temperature test rotor id 
void
StateAmsTempTestInputRotorId::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Input Temperature Test Rotor ID";

    //LogDebug ("input rotor id");
    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
    msg.data.middleEnterId.isPassword = 0;
    wcscpy (msg.data.middleEnterId.initialValue, L"");
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIdData_t));
    SetBottomButtons (strings[BSTR_BACK], false, strings [BSTR_DONE], true);
}

// Ams test process
state_t
StateAmsTempTestInputRotorId::
ProcessMessage (int msgId, char *data)
{
    int     retVal = STATE_AMS_TEMP_TEST_INPUT_ROTOR_ID;
    int     idata = *(int *)data;
    char *  sdata = (char *)data;
    char    rotorIdStr[10];
    HeaterValues_t  heaterValues;

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
            tempRotor.rotorId = atoi (rotorIdStr); 
            LogDebug ("rotorId= %d", tempRotor.rotorId);
            //now confirm valid rotor id number
            if (CheckValidRotor ()  == -1) {
                stateStack.Push (STATE_AMS_TEMP_TEST_INPUT_ROTOR_ID);
                retVal = STATE_AMS_TEMP_TEST_WRONG_ROTOR_ID;
            } else {
                // send message to heaterProcess to get initial temperature
                SendHeaterControlMessage (MSG_HEATER_GET_TEMPERATURE_VALUES);
            } 
        break;

        case  MSG_HEATER_SEND_TEMPERATURE_VALUES:
            //wait for Heater control process send initial temperaturs
            memcpy ((char *)&heaterValues, data, sizeof (HeaterValues_t));
            LogDebug ("Get initial temperatures from HeaterControl process");
            tempInitValues.airInitTemp = heaterValues.rotorTemperature;    
            tempInitValues.ambInitTemp = heaterValues.ambientTemperature;
            tempInitValues.topInitTemp = heaterValues.topPlateTemperature;
            tempInitValues.botInitTemp = heaterValues.bottomPlateTemperature;
#ifdef TEMP_SIMULATION
            // ZZZ
            SendDrawerMessage ('O');
            retVal = STATE_AMS_INPUT_ROTOR_OHMS;  
#else
            retVal = STATE_AMS_TEMP_TEST_IN_PROGRESS;
#endif
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

// check if it is valid tempperature rotor  
int
StateAmsTempTestInputRotorId::
CheckValidRotor (void)
{

    int i;

    for (i = 0; i < MAX_TEMP_ROTOR; i++) {
        if ((defaultTempRotorTable [i].rotorId == tempRotor.rotorId) || 
            (defaultTempRotorTable [i].rotorId == MAX_TEMP_ROTOR)) {
            break;
        }
    }
    if (defaultTempRotorTable [i].rotorId != MAX_TEMP_ROTOR) {
        tempRotor.offset = defaultTempRotorTable [i].offset;
        return 0;
    } else {
        LogDebug ("Fail to find out temperature rotor Id: %d", tempRotor.rotorId);
    }
    return -1;
}

//input wrong temperature test rotor id 
void
StateAmsTempTestWrongRotorId::
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
StateAmsTempTestWrongRotorId::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_AMS_TEMP_TEST_WRONG_ROTOR_ID;

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
StateAmsTempTestInProgress::
StartTempTest (void)
{
    SystemErrNum_t err;

    // first time test temperatures, set default and initial DAC values
    if (testCount ==0) {
        tempInitValues.initDac [0]          = DEFAULT_AMS_TEMP_TEST_DAC;
        tempInitValues.initDac [1]          = DEFAULT_AMS_TEMP_TEST_DAC;
        tempDefaultDac [0]   = DEFAULT_AMS_TEMP_TEST_DAC;
        tempDefaultDac [1]   = DEFAULT_AMS_TEMP_TEST_DAC;

    }
    // always set offset = 0 for temperature calibration
    tempInitValues.offset         = 0;
    LogDebug ("Set AMS Heater topDAC = %d, botDac= %d offset =%d", tempInitValues.initDac [0],
                                                                   tempInitValues.initDac [1],
                                                                   tempInitValues.offset);

    // setting Ams default temperature test Dac values and offset = 0
    SendHeaterCommand ('S', tempInitValues.initDac [0], 
                                tempInitValues.initDac [1], 
                                tempInitValues.offset);

    // Initialize EngineComm and CalcResults for a new analysis run.
    SendEngineCommMessage (MSG_ENGINE_COMM_INIT_ANALYSIS);


    // download Ris file
    err = SendRisFile (TEMP_TEST_RIS_FILE);
    if (err != SERR_NONE) {
        return err;
    }

    // disable watchdog before start test
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    // start  test
    SendAMessage ('M');

    //enable timer for progress bar update 
    SetTimer (27);

    return SERR_NONE;
}

//Ams temperature Test progress 
void
StateAmsTempTestInProgress::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperature Calibration Progress";
    
    SetTopLabel (topLable);

    // start progress bar at 6
    progBarValue = 0;
    msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
    wcscpy (msg.data.middleProgBarMessage.progBarText, L"Calibrating ...");
    wcscpy (msg.data.middleProgBarMessage.message, L"Temperature Calibration");
    // temperatur calibration rotor, background white color, font black color
    msg.data.middleProgBarMessage.fontColor =  0x004E4E5E;
    msg.data.middleProgBarMessage.backgroundColor = 0x00FFFFFF;
    msg.data.middleProgBarMessage.progBarValue = progBarValue;
    SendGuiMessage (&msg, sizeof(GuiMiddleProgBarMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    StartTempTest ();
     

}

// Ams temperature test process
state_t
StateAmsTempTestInProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_TEMP_TEST_IN_PROGRESS;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    int                         updateProgBar = 0;
     

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // Tell the Engine to cancel.
            SendAMessage( 'C' );
            retVal = STATE_AMS_TEMP_TEST_CANCEL;
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':  //progress command
                    switch (engineCommMsg->message[8]) {
                        case TEMPERATURE_TEST_DONE:
                            // disable timer
                            CloseTimer ();
                            updateProgBar = 1;
                            progBarValue = 100;
                            // enable watchdog
                            SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                            LogDebug ("Temperature test done");
                            //recover original factory dac settings 
                            SendHeaterCommand ('S', factoryData->heaterDacSettings [0], 
                                                    factoryData->heaterDacSettings [2], 
                                                    factoryData->airTemperatureOffset);
                            // open drawer
                            SendDrawerMessage ('O');
                            retVal = STATE_AMS_INPUT_ROTOR_OHMS;
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

        case MSG_MANUFACTURE_TIMER:
            updateProgBar = 1;
            progBarValue += 4;
            //enable timer for progress bar update
            SetTimer (26);
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
         // someting wrong, open the drawer
         SendDrawerMessage ('O');
        infoText.type = AMS_TEMP_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



//Ams Temperature Test Canceling 
void
StateAmsTempTestInCanceling::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperature Test Canceling";
    wchar_t                 *str = L"Canceling ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));

    SetBottomButtons ();

}

state_t
StateAmsTempTestInCanceling::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_TEMP_TEST_CANCEL;
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
        infoText.type = AMS_TEMP_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

//input valid temperature test rotor id
void
StateAmsTempTestInputOhms::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Input Test Rotor Ohms";

   // SendDrawerMessage ('O'); 

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
    msg.data.middleEnterId.isPassword = 0;
    wcscpy (msg.data.middleEnterId.initialValue, L"");
    SendGuiMessage (&msg, sizeof (GuiMiddleEnterIdData_t));
    SetBottomButtons (strings [BSTR_DONE], true);
}

// Ams test process
state_t
StateAmsTempTestInputOhms::
ProcessMessage (int msgId, char *data)
{
    int     retVal = STATE_AMS_INPUT_ROTOR_OHMS;
    char *  sdata = (char *)data;
    char    rotorOhmStr[10];

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            // done, get input string
            SendGuiMessage (MSG_GUI_GET_ID_TEXT);
        break;

        case MSG_GUI_OUT_ID_TEXT:
            // get ohms 
            memset (rotorOhmStr, 0 , 10);
            strcpy (rotorOhmStr, sdata);
            tempRotor.rotorOhms = atoi (rotorOhmStr);
            if ((tempRotor.rotorOhms > 10000) || (tempRotor.rotorOhms < 4000)) {
                LogError ("Resistance %d out of range", tempRotor.rotorOhms); 
                tempFinalReport[ROTOR_OHMS_OUT_RANGE].error = ROTOR_OHMS_OUT_RANGE;
                retVal = STATE_AMS_TEMP_TEST_RESULTS_CAL;
            } else {
                retVal = STATE_AMS_TEMP_TEST_RESULTS_CAL;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

//Ams TEMP Test Results display 
void
StateAmsTempTestShowResults::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperature Test Results";

    SetTopLabel (topLable);

    //msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    wcsncpy (msg.data.middleViewResults.text, displayText, wcslen (displayText));
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true, L"Rept", false);

}

state_t
StateAmsTempTestShowResults::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_TEMP_TEST_SHOW_RESULTS;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
             switch (idata) {
                case 0:
                    // avoid disply loading rotor display state
                    stateStack.Pop();
                    retVal = stateStack.Pop();
                break;
                
                case 1:
                    stateStack.Push (STATE_AMS_TEMP_TEST_SHOW_RESULTS);
                    retVal = STATE_AMS_TEMP_TEST_SHOW_REPORTS;
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





//Ams TEMP Test Results display
void
StateAmsTempTestShowReports::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperatuer Test Reports";
    FILE                    *fp;
    int                     len;
	char					buff [4000];

    SetTopLabel (topLable);

    fp = fopen (AMS_TEMPERATURE_REPORT_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_TEMPERATURE_REPORT_FILE);
        wcscpy (msg.data.middleViewResults.text, L"Fail to open temperature test report file");
    } else {memset (buff, 0, 4000);
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
StateAmsTempTestShowReports::
ProcessMessage (int msgId, char *data)
{
    int  retVal = STATE_AMS_TEMP_TEST_SHOW_REPORTS;

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
