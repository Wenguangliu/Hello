#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "LampTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


//test
char            StateStartLampTest::testStartFlag = 0;
unsigned short  StateStartLampTest:: avg [NUM_WAVELENGTHS];


// display string table
typedef struct
{
    wchar_t         waveLength [7];
    unsigned int    adcValue; 
} MotorDisplayItem_t;

MotorDisplayItem_t   motorDisplayItem [] = 
                                            {
                                                {L"340nm", 0},
                                                {L"405nm", 0},
                                                {L"457nm", 0},
                                                {L"500nm", 0},
                                                {L"515nm", 0},
                                                {L"550nm", 0},
                                                {L"600nm", 0},
                                                {L"630nm", 0},
                                                {L"850nm", 0},
                                                {L"White ", 0},
                                            };

// get sum of raw data
void
StateStartLampTest::
GetRawDataAvg (void)
{
    unsigned char         flasNmuber, waveLength;
    unsigned long         sum [NUM_WAVELENGTHS];
    

    memset (sum, 0, sizeof (sum));

     //sum 20 times flashes  skip flash number 0
    for (flasNmuber = 1; flasNmuber < 21; flasNmuber++) {
        for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
            sum [waveLength] += rawData->rotorRawReadings.blackOffsets [flasNmuber].rawFlashReadings [waveLength];
        }
    }

    // got average
    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        avg [waveLength] = (unsigned short)(sum [waveLength]/20);
    }
}

// display test results
void
StateStartLampTest::
DisplayAdcValues ( )
{
    GuiReceiveMsg_t       msg;
    char                  i,j,numItems = 5;
    GuiMiddleListData_t   *data = &msg.data.middleList;


    // format display string
    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;
    memset (data, 0, sizeof (GuiMiddleListData_t));
    for (i =0, j = 0; i < numItems; i++) {
        wsprintf (data->items[i][0], L"%s    %5u", motorDisplayItem[j].waveLength, avg [j++]);
        wsprintf (data->items[i][1], L"%s    %5u", motorDisplayItem[j].waveLength, avg [j++]);
    }
    data->isSelectable = 0;
    data->hasUpDown = 0;
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
} 

// Lamp Test state  
void
StateStartLampTest::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    wchar_t            *topLable = L"Lamp Test";
    char                i,j,numItems = 5;
    GuiMiddleListData_t *data = &msg.data.middleList;

    // disable heater polling
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;
    memset (data, 0, sizeof (GuiMiddleListData_t));
    j = 0;
    for (i = 0; i < numItems; i++) {
        wsprintf (data->items[i][0], L"%s       %5u", motorDisplayItem[j].waveLength, motorDisplayItem[j++].adcValue);
        wsprintf (data->items[i][1], L"%s       %5u", motorDisplayItem[j].waveLength, motorDisplayItem[j++].adcValue);
    }
    data->isSelectable = 0;
    data->hasUpDown = 0;
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
    SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Cali", false);
    testStartFlag = 0;
}

// start/stop lamp test
state_t
StateStartLampTest ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_LAMP_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    static   char               stopButtonActive = 0;
           
    
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: //Back
                    if (testStartFlag == 0) {
                        // enable heater polling
                        SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                        retVal = stateStack.Pop();
                    }
                break;

                case 1://Start/Stop Test
                    // set test flag to be true
                    if (testStartFlag == 0) {
                        // send test start command to engine
                        SendTestRequestMessage ('P', NULL, 0);
                        testStartFlag = 1;
                        stopButtonActive = 1;
                        SetBottomButtons (strings[BSTR_BACK], false, L"Stop", false, L"Cali", true);
                    } else  if (stopButtonActive == 1){
                        // in activer stop button 
                        stopButtonActive = 0;
                        // stop flash
                        SendTestRequestMessage ('p', NULL, 0);
                        // send test stop command to engine
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Cali", false);
                        // wait 500ms before allow to re-start flash test 
                        Sleep (500);
                        testStartFlag = 0;
                   }
                break;


                case 2://Trim DAC calibration 
                    // de-active this key if lamp test started
                    if (testStartFlag == 0) {
                        stateStack.Push (STATE_LAMP_TEST); 
                        retVal = STATE_TRIM_DAC_CALI_IN_PROGRESS;
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':  // get A command 
                    switch (engineCommMsg->message[8]) {
                        case ANALYSIS_DONE:
                            // get and calculate raw data average
                            GetRawDataAvg ();
                            // start flash again
                            if (testStartFlag == 1) {
                                SendTestRequestMessage ('P', NULL, 0);
                            }
                            // display test results on Gui
                            DisplayAdcValues ();
                        break;


                        default:
                        break;
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
    if (analyzeError != SERR_NONE) {
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


// enter trim DAC windows
void
StateStartLampTrimDacCali::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    wchar_t             *topLable = L"Trim DAC Calibration";
    char                i,j,numItems = 5;
    GuiMiddleListData_t *data = &msg.data.middleList;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;
    memset (data, 0, sizeof (GuiMiddleListData_t));
    j = 0;
    for (i = 0; i < numItems; i++) {
        wsprintf (data->items[i][0], L"%sDAC  %3u", motorDisplayItem[j].waveLength,  calibrationData->wavelengthDacTrims [j++]);
        wsprintf (data->items[i][1], L"%sDAC  %3u", motorDisplayItem[j].waveLength,  calibrationData->wavelengthDacTrims [j++]);
    }

    data->isSelectable = 0;
    data->hasUpDown = 0;
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
    SetBottomButtons (strings[BSTR_BACK], false);
}


// calibrate trim dac values 
state_t
StateStartLampTrimDacCali ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_TRIM_DAC_CALI;

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




// enter trim DAC progress windows
void
StateLampTrimDacCaliProgress::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    wchar_t             *topLable = L"Trim DAC Calibrating";
    wchar_t             *str = L"Calibrating ...";

    SetTopLabel (topLable);

    wcscpy (msg.data.middleMessage.message, str);
    msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
    msg.msgId = MSG_GUI_BOTTOM_NONE;
    SendGuiMessage (&msg, 0);
    //send command to Engine to calibration trim DAC 
    SendTestRequestMessage ('C', NULL, 0);
    // set 5 seconds timeout for trim dac calibration
    SetTimer (5);
}


// waiting for calibration
// may add timeout state and timeout thread
state_t
StateLampTrimDacCaliProgress::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_TRIM_DAC_CALI_IN_PROGRESS;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    char                        parameters [16];
    int                         i, numItems = 5;;


     switch (msgId) {

         case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'A':
                    // Engine board sent raw date 
                    if (engineCommMsg->message[8] ==  OPTICAL_DACS) {
                        CloseTimer ();
                        // send message to calculation process
                        // to calculation trim DAC values
                        SendCalcResultsMessage (MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE_MFP);
                        LogDebug ("Get Trim Dac Raw data");
                        // wait for calculation process 2 seconds
                        SetTimer (2);
                    }

                break;

                default:
                    ProcessOtherMessages (msgId, data);
                break;
            }
        break;

        case MSG_MANUFACTURE_DAC_TRIMS_COMPLETE:
             // disable timer
             CloseTimer ();
            // calculation process  finishing trim DAC caculation
            // get results from share memory and send the results to Engine board
            memset (parameters, 0, sizeof (parameters));
            for (i =0; i < NUM_WAVELENGTHS; i++) {
                parameters [i] =  calibrationData->wavelengthDacTrims [i];
            }
            // send command to Engine to set DAC values
            SendTestRequestMessage ('S', &parameters[0], 10);
            // display window
            retVal = STATE_TRIM_DAC_CALI;
        break;

        case MSG_MANUFACTURE_TIMER:
            // calibration timeout , something wrong
            analyzeError = SERR_DAC_SETTING;
            LogError( "Trim Dac Calibration Fail");
            infoText.type = TRIM_DAC_CALI_TYPE;
            stateStack.Pop(); // back to lamp tester
            retVal = STATE_MANUFACTURING_ERROR;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}
