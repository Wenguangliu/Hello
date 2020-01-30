#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>

#include "ManufacturingStates.h"
#include "TroubleShootingStates.h"
#include "DrawerTestStates.h"
#include "HeaterTestStates.h"
#include "LampTestStates.h"
#include "MotorTestStates.h"
#include "AmsNdxtTestStates.h"
#include "AmsMotorTestStates.h"
#include "AmsTempTestStates.h"
#include "AmsDelayTestStates.h"
#include "AmsBarcodeTestStates.h"
#include "AmsAlignmentTestStates.h"
#include "AmsCuvetteTestStates.h"
#include "BarcodeTestStates.h"
#include "TsCalibrationStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "ViewLog.h"
#include "CuvetteTestStates.h"
#include "BarcodeTestStates.h"
#include "TsCalibrationStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "ViewLog.h"
#include "CuvetteTestStates.h"
#include "NetworkTestStates.h"
#include "AmsTestReportDisplay.h"

void    Receiver();
State * states [STATE_COUNT];
state_t currentState;

// initial state machine
void
InitialState (void)
{
    states [STATE_BACKGROUND]                   = new StateBackground();
    states [STATE_MANUFACTURING_ERROR]          = new StateRandDError ();
    states [STATE_MAIN_SELECTION]               = new StateMainSelection ();
    states [STATE_MANUFACTURING_SELECTION]      = new StateManufacturingSelection ();
    states [STATE_MANUFACTURING]                = new StateManufacturingTest ();
    states [STATE_MANUFACTURING_2]              = new StateManufacturingTest2 ();
    states [STATE_INFO]                         = new StateInfo ();
    states [STATE_DT_DRAWER_IS_OPENING]         = new StateDrawerIsOpening ();
    states [STATE_DT_DRAWER_IS_OPENED]          = new StateDrawerIsOpened ();
    states [STATE_DT_DRAWER_IS_CLOSING]         = new StateDrawerIsClosing ();
    states [STATE_DT_CONFIRM_CANCEL]            = new StateDrawerTestCancelConfirm ();
    states [STATE_DT_CANCELING]                 = new StateDrawerTestCancelling ();
    states [STATE_DT_WAIT_FOR_NEXT_CYCLE]       = new StateDrawerWatingForNextCycle ();
    states [STATE_DT_TEST_COMPLETED]            = new StateDrawerTestCompleted ();
    states [STATE_DT_TEST_COUNTER_DISPLAY]      = new StateDrawerTestCounterDisplay ();
    states [STATE_DT_READ_BARCODE]              = new StateReadBarcode ();
    states [STATE_MANUFACTURING_INFO]           = new StateRandDInfo ();
    states [STAET_VIEW_LOG_FILE]                = new StateViewLog ();
    states [STATE_TROUBLE_SHOOTING]             = new StateTroubleShooting ();
    states [STATE_TROUBLE_SHOOTING_2]           = new StateTroubleShooting2 ();
    states [STATE_HEATER_TEST]                  = new StateHeateTest ();
    states [STATE_WAIT_ANALY_WARM_UP]           = new StateWaitWarmup ();
    states [STATE_LAMP_TEST]                    = new StateStartLampTest ();
    states [STATE_TRIM_DAC_CALI]                = new StateStartLampTrimDacCali ();
    states [STATE_TRIM_DAC_CALI_IN_PROGRESS]    = new StateLampTrimDacCaliProgress ();
    states [STATE_MOTOR_TEST]                   = new StateMotorTest ();
    states [STATE_BARCODE_READ_TEST]            = new StateBarcodeTest ();
    states [STATE_TOUCH_SCREEN_CALI]            = new StateTsCalibration ();
    states [STATE_CUVETTE_TEST]                 = new StateCuvetteTest ();
    states [STATE_AMS_MOTOR_TEST]               = new StateAmsMotorTest ();
    states [STATE_AMS_MOTOR_TEST_IN_PROGRESS]   = new StateAmsMotorTestInProgress ();
    states [STATE_AMS_MOTOR_TEST_CANCEL]        = new StateAmsMotorTestInCanceling ();
    states [STATE_AMS_MOTOR_TEST_RESULTS_CAL]   = new StateAmsMotorTestResultsCal ();
    states [STATE_LAMP_TEST]                    = new StateStartLampTest ();
    states [STATE_TRIM_DAC_CALI]                = new StateStartLampTrimDacCali ();
    states [STATE_TRIM_DAC_CALI_IN_PROGRESS]    = new StateLampTrimDacCaliProgress ();
    states [STATE_MOTOR_TEST]                   = new StateMotorTest ();
    states [STATE_BARCODE_READ_TEST]            = new StateBarcodeTest ();
    states [STATE_TOUCH_SCREEN_CALI]            = new StateTsCalibration ();
    states [STATE_CUVETTE_TEST]                 = new StateCuvetteTest ();
    states [STATE_AMS_MOTOR_TEST]               = new StateAmsMotorTest ();
    states [STATE_AMS_MOTOR_TEST_IN_PROGRESS]   = new StateAmsMotorTestInProgress ();
    states [STATE_AMS_MOTOR_TEST_CANCEL]        = new StateAmsMotorTestInCanceling ();
    states [STATE_AMS_MOTOR_TEST_RESULTS_CAL]   = new StateAmsMotorTestResultsCal ();
    states [STATE_AMS_MOTOR_TEST_SHOW_RESULTS]  = new StateAmsMotorTestShowResults ();
    states [STATE_AMS_NDXT_TEST]                = new StateAmsNdxtTest ();
    states [STATE_AMS_NDXT_TEST_IN_PROGRESS]    = new StateAmsNdxtTestInProgress ();
    states [STATE_AMS_NDXT_TEST_CANCEL]         = new StateAmsNdxtTestInCanceling ();
    states [STATE_AMS_NDXT_TEST_RESULTS_CAL]    = new StateAmsNdxtTestResultsCal ();
    states [STATE_AMS_NDXT_TEST_SHOW_RESULTS]   = new StateAmsNdxtTestShowResults ();
    states [STATE_AMS_NDXT_TEST_SHOW_REPORTS]   = new StateAmsNdxtTestShowReports (); 
    states [STATE_AMS_MOTOR_TEST_SHOW_REPORTS]  = new StateAmsMotorTestShowReports ();

    states [STATE_AMS_TEMP_TEST]                = new StateAmsTempTest ();
    states [STATE_AMS_WAIT_ANALY_WARM_UP]       = new StateAmsAnalyWaitWarmup ();
    states [STATE_AMS_TEMP_TEST_INPUT_ROTOR_ID] = new StateAmsTempTestInputRotorId (); 
    states [STATE_AMS_TEMP_TEST_WRONG_ROTOR_ID] = new StateAmsTempTestWrongRotorId (); 
    states [STATE_AMS_TEMP_TEST_IN_PROGRESS]    = new StateAmsTempTestInProgress (); 
    states [STATE_AMS_TEMP_TEST_CANCEL]         = new StateAmsTempTestInCanceling (); 
    states [STATE_AMS_INPUT_ROTOR_OHMS]         = new StateAmsTempTestInputOhms (); 
    states [STATE_AMS_TEMP_TEST_RESULTS_CAL]    = new StateAmsTempTestResultsCal (); 
    states [STATE_AMS_TEMP_TEST_SHOW_RESULTS]   = new StateAmsTempTestShowResults (); 
    states [STATE_AMS_TEMP_TEST_SHOW_REPORTS]   = new StateAmsTempTestShowReports (); 

    states [STATE_AMS_DELAY_TEST]               = new StateAmsDelayTest (); 
    states [STATE_AMS_DELAY_TEST_IN_PROGRESS]   = new StateAmsDelayTestInProgress (); 
    states [STATE_AMS_DELAY_TEST_CANCEL]        = new StateAmsDelayTestInCanceling (); 
    states [STATE_AMS_DELAY_TEST_SHOW_RESULTS]  = new StateAmsDelayTestShowResults (); 
    states [STATE_AMS_DELAY_TEST_SHOW_REPORTS]  = new  StateAmsDelayTestShowReports (); 
    states [STATE_AMS_DELAY_TEST_RESULTS_CAL]   = new StateAmsDelayTestResultsCal () ; 

    states [STATE_AMS_BARCODE_TEST]             = new StateAmsBarcodeTest (); 
    states [STATE_AMS_BARCODE_TEST_IN_PROGRESS] = new StateAmsBarcodeTestInProgress (); 
    states [STATE_AMS_BARCODE_TEST_CANCEL]      = new StateAmsBarcodeTestInCanceling (); 
    states [STATE_AMS_BARCODE_TEST_SHOW_RESULTS]= new StateAmsBarcodeTestShowResults (); 
    states [STATE_AMS_BARCODE_TEST_SHOW_REPORTS]= new  StateAmsBarcodeTestShowReports (); 
    states [STATE_AMS_BARCODE_TEST_RESULTS_CAL] = new StateAmsBarcodeTestResultsCal () ; 

    states [STATE_AMS_ALIGNMENT_TEST]             = new StateAmsAlignmentTest (); 
    states [STATE_AMS_ALIGNMENT_TEST_IN_PROGRESS] = new StateAmsAlignmentTestInProgress (); 
    states [STATE_AMS_ALIGNMENT_TEST_CANCEL]      = new StateAmsAlignmentTestInCanceling (); 
    states [STATE_AMS_ALIGNMENT_TEST_SHOW_RESULTS]= new StateAmsAlignmentTestShowResults (); 
    states [STATE_AMS_ALIGNMENT_TEST_SHOW_REPORTS]= new  StateAmsAlignmentTestShowReports (); 
    states [STATE_AMS_ALIGNMENT_TEST_RESULTS_CAL] = new StateAmsAlignmentTestResultsCal () ; 

    states [STATE_AMS_CUV_TEST]                 = new StateAmsCuvetteTest ();
    states [STATE_AMS_CUV_TEST_INPUT_ROTOR_ID]  = new StateAmsCuvetteTestInputRotorId (); 
    states [STATE_AMS_CUV_TEST_WRONG_ROTOR_ID]  = new StateAmsCuvetteTestWrongRotorId (); 
    states [STATE_AMS_CUV_TEST_IN_PROGRESS]     = new StateAmsCuvetteTestInProgress (); 
    states [STATE_AMS_CUV_TEST_CANCEL]          = new StateAmsCuvetteTestInCanceling (); 
    states [STATE_AMS_CUV_TEST_RESULTS_CAL]     = new StateAmsCuvetteTestResultsCal (); 
    states [STATE_AMS_CUV_TEST_SHOW_RESULTS]    = new StateAmsCuvetteTestShowResults (); 
    states [STATE_AMS_CUV_TEST_SHOW_REPORTS]    = new StateAmsCuvetteTestShowReports (); 

    states [STATE_NETWORK_TEST]                 = new StateNetworkTest (); 
    states [STATE_NETWORK_CONFIG]               = new StateNetworkConfig (); 
    states [STATE_NETWORK_CONFIGURING]          = new StateNetworkConfiguring (); 
    states [STATE_ENTER_ANALYZER_IP]            = new StateEnterAnalyzerIp (); 
    states [STATE_ENTER_ANALYZER_MASK]          = new StateEnterAnalyzerMask (); 
    states [STATE_ENTER_GATEWAY]                = new StateEnterGateway (); 
    states [STATE_ENTER_SERVER_IP]              = new StateEnterServerIp (); 
    states [STATE_ENTER_SERVER_SOCKET]          = new StateEnterServerSocket (); 
    states [STATE_ENTER_HOST_NAME]              = new StateEnterHostname (); 
    states [STATE_NETWORK_CONFIG_ERROR]         = new StateNetworkConfigError (); 
    states [STATE_TELNET_CONFIG]                = new StateTelnetConfig (); 
    states [STATE_ENGINE_SERIAL_DEBUG_PORT_SET] = new StateSetEngineSerialDebugPort (); 
    states [STATE_AMS_TEST_REPORT]              = new StateAmsTestReport (); 
    states [STATE_AMS_TEST_REPORT_2]            = new StateAmsTestReport2 (); 
    states [STATE_AMS_DISPLAY_REPORT]           = new StateDisplayAmsReport(); 
    
}


// main program
int
main (int argc, char ** argv)
{
    // initial state
    InitialState ();
    // get manufacturing test timing set
    StateRandD::ReadManufacturingTestTimeSet();
    // create ams test data directory
    StateRandD::CreateTestDirectory (AMS_DATA_DIRECTARY);
    currentState = STATE_BACKGROUND;
    // default at background state
    states [currentState]->Enter();
    // wait for messages to process
    Receiver ();
    // should be here except errors
    return -1;
}

// receive loop
void
Receiver (void)
{
    DWORD               receivedSize = 0;
    ConsoleMsg_t        msg;
    state_t             newState;
    HANDLE              queue;
    MSGQUEUEOPTIONS     options;
    DWORD               flags;
    BOOL                result;


    /*
     * Open the manufacturing receive queue and get its size.
     */
    options.dwSize = sizeof (MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = TRUE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;

    queue = CreateMsgQueue (MANUFACTURE_QUEUE_NAME, &options);
   
    if (queue == NULL) {
        LogError( "failure to open manufacturing queue, error: %d",  GetLastError());
        return;
    }
   
    /*
     * Receive messages and handle state changes.
     */
    while (1) {
        result = ReadMsgQueue(queue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);
        if (result) {
            if (receivedSize > 0) {
                newState = states [currentState]->ProcessMessage(msg.msgId, msg.data);
                if (newState >= 0
                     && newState != currentState
                     && newState < STATE_COUNT
                     && states[newState] != 0) {
                    //goto new state
                    states [newState]->Enter();
                    currentState = newState;
                }
            }
        } else {
            LogError ("manufacturing message receive error, error: %d", GetLastError()) ;
            break;
        }
    }
}
