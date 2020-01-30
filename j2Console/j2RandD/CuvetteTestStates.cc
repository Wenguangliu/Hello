#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "BarcodeTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "RotorBarcode.h"
#include "ParseCalibration.h"
#include "CuvetteTestStates.h"
#include "PrinterMessages.h"
#include "LogErr.h"

#define             CUVETTE_TEST_RESULTS_FILE   "Hard Disk/Abaxis/cfg/cuvTest.dat"

unsigned char       StateCuvetteTest::cuvCurrentDacValue;
unsigned char       StateCuvetteTest::cuvThresholdDacValue;
CuvetteTestFlags_t  StateCuvetteTest::cuvetteTestFlags;

CuvetteResultStrings_t resultStr [] = 
                           {    {SERR_NONE,                 "Cuvette calibration successful"},
                                {SERR_SPINDLE_MOTOR_STUCK,  ":Spindle motor stuck"},
                                {SERR_CUVETTE_CURRENT,      ":Cuvette current cali error"},
                                {SERR_CUVETTE_THRESHOLD,    ":Cuvette threshold cali error"},
                                {SERR_MISSED_CUV_WINDOW,    ":Cuvette miss window err"},
                                {SERR_INDEX_CUV_TOO_LONG,   ":Cuvette index too long error"},
                                {SERR_INDEX_CUV_TOO_SHORT,  ":Cuvette index too short error"},
                                {SERR_CUV_TOO_SHORT,        ":Cuvette too short error"},
                                {SERR_CUV_TOO_LONG,         ":Cuvette too long error"},
                                {SERR_CUV_MISSING,          ":Cuvette miss error"},
                                {(SystemErrorNumber_t)0xFFFF,":Cuvette end string"} 

                           };
                          

#define GUI_ENABLE


char   *
StateCuvetteTest::
FindCuvetteTestResultString (SystemErrorNumber_t errorNum)
{
    CuvetteResultStrings_t  *cuvetteResultStrings;

    cuvetteResultStrings = &resultStr [0];

    while (cuvetteResultStrings->errorNum != 0xFFFF) {
        if (cuvetteResultStrings->errorNum == errorNum) {
            break;
        }
        cuvetteResultStrings++;
    }
    if (cuvetteResultStrings->errorNum != 0xFFFF) {
        return cuvetteResultStrings->resultStr;
    } 
    
    return ("Unknown Cuvette Error");
}

// Cuvette Test process  
void
StateCuvetteTest::
Enter (void)
{
    GuiReceiveMsg_t                 msg;
    wchar_t                         *topLable = L"Cuvette Test";
    GuiMiddleCuvetteTestData_t      *middleCuvetteTestData;
    int                             i;
    
    SetTopLabel (topLable);

    // clear test flags
    memset (&cuvetteTestFlags, 0, sizeof (CuvetteTestFlags_t));

    // try to get drawer status
    SendDrawerMessage ('S');

    middleCuvetteTestData =  &msg.data.middleCuvetteTestData;
    memset (middleCuvetteTestData, 0, sizeof (GuiMiddleCuvetteTestData_t));
    
    // initial cuvette test data strings
    wcscpy (&middleCuvetteTestData->resultsText[0],  L" ");
    for (i =0; i < 6; i++) {
        wsprintf (&middleCuvetteTestData->valueText [i][0], L"%u", 0); 
    }
    msg.msgId = MSG_GUI_MIDDLE_CUVETTE_DATA;

#ifdef GUI_ENABLE
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
#endif
    SetBottomButtons (strings[BSTR_BACK], true, L"Start", false, strings [BSTR_OPEN], false);
}

// Cuvette test process
state_t
StateCuvetteTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_CUVETTE_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    GuiMiddleCuvetteTestData_t *middleCuvetteTestData; 
    unsigned char               status;
    static  char                guiUpdate = 0;
    cuvetteTestParameters_t     cuvetteTestParameters;
    PrinterMsg_t                printerMsg;

    middleCuvetteTestData =  &msg.data.middleCuvetteTestData;
    memset (middleCuvetteTestData, 0, sizeof (GuiMiddleCuvetteTestData_t));
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: // back to previous manue
                    if (cuvetteTestFlags.testStartedFlag == 0) {
                        //Back, always close drawer
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                    }
                break;

                case 1: // start cuvette test
                    if ((cuvetteTestFlags.drawerReadyFlag == 1) && (cuvetteTestFlags.testStartedFlag == 0)) {
                        cuvetteTestFlags.testStartedFlag = 1;
                        SendCuvetteTestMessage ('V');
                        wsprintf (&middleCuvetteTestData->resultsText[0],L"%s", L"Cuvette Calibrating ...");
                        ResetDisplayParameter (middleCuvetteTestData);
                        cuvetteTestFlags.guiUpdateFlag = 1; 
                    } else if (cuvetteTestFlags.testStartedFlag != 1) {
                        wsprintf (&middleCuvetteTestData->resultsText[0],L"%s", L"Please load rotor");
                        cuvetteTestFlags.guiUpdateFlag = 1; 
                    }
                break;

                case 2: //open/close drawer
                    if (cuvetteTestFlags.drawerOpenFlag) {
                        SendDrawerMessage ('C');
                    } else {
                        // only test finished, allow to open drawer
                        if (cuvetteTestFlags.testStartedFlag == 0) {
                            SendDrawerMessage ('O');
                        }
                    }
                    ResetDisplayParameter (middleCuvetteTestData);
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
                        wcscpy (&middleCuvetteTestData->resultsText[0],  L"Drawer jammed");
                    } else if (status & 0x10) {//drawer closed
                        cuvetteTestFlags.drawerOpenFlag = 0;
                        SetBottomButtons (strings[BSTR_BACK], true, L"Start", false, strings [BSTR_OPEN], false);
                        if (status & 0x40) {//rotor present
                            cuvetteTestFlags.drawerReadyFlag = 1;
                            wcscpy (&middleCuvetteTestData->resultsText[0],  L"Ready for cuvette test");
                        } else {
                            wcscpy (&middleCuvetteTestData->resultsText[0],  L"Please open drawer and load rotor");
                            cuvetteTestFlags.drawerReadyFlag = 0;
                        }
                    } else if (status & 0x20) { // drawer open
                        SetBottomButtons (strings[BSTR_BACK], true, L"Start", false, strings [BSTR_CLOSE], false);
                        wcscpy (&middleCuvetteTestData->resultsText[0],  L"Please load rotor");
                        cuvetteTestFlags.drawerReadyFlag = 0;
                        cuvetteTestFlags.drawerOpenFlag = 1;
                    }
                    cuvetteTestFlags.guiUpdateFlag = 1;
                break;

                case 'G':
                    if (engineCommMsg->message [1] == 'V') {
                        // reset flag to allow to test again
                        cuvetteTestFlags.testStartedFlag = 0;
                        // get all cuvette parameters
                        memcpy ((char *)&cuvetteTestParameters, (char *)&engineCommMsg->message[6], sizeof (cuvetteTestParameters_t));;
                        // if error
                        if (cuvetteTestParameters.cuvError != 0) {
                            wsprintf (&middleCuvetteTestData->resultsText[0],L"%x%s",cuvetteTestParameters.cuvError, 
                                                                                 CstrToWstr(FindCuvetteTestResultString ((SystemErrorNumber_t)cuvetteTestParameters.cuvError)));
                        } else {
                            wsprintf (&middleCuvetteTestData->resultsText[0],L"%s", CstrToWstr (FindCuvetteTestResultString ((SystemErrorNumber_t)cuvetteTestParameters.cuvError)));
                        }
                        wsprintf (&middleCuvetteTestData->valueText [0][0], L"%u",cuvetteTestParameters.cuvCurrentDac);
                        wsprintf (&middleCuvetteTestData->valueText [1][0], L"%u",cuvetteTestParameters.cuvThresholdDac);
                        wsprintf (&middleCuvetteTestData->valueText [2][0], L"%u",cuvetteTestParameters.totalCuvNum);
                        wsprintf (&middleCuvetteTestData->valueText [3][0], L"%u",cuvetteTestParameters.indexWidth);
                        wsprintf (&middleCuvetteTestData->valueText [4][0], L"%u",cuvetteTestParameters.maxCuvWidth);
                        wsprintf (&middleCuvetteTestData->valueText [5][0], L"%u",cuvetteTestParameters.minCuvWidth);
                        cuvetteTestFlags.guiUpdateFlag = 1;
                        cuvetteTestFlags.resetDisplayFlag = 1;
                        // save test results for print
                        SaveTestResults (&cuvetteTestParameters);
                        //print test results
                        printerMsg.msgId = MSG_PRINT_CUVETTE_TEST_RESULTS; 
                        SendPrinterMessage (&printerMsg);
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

    // send message to Gui 
    if (cuvetteTestFlags.guiUpdateFlag == 1) {
        cuvetteTestFlags.guiUpdateFlag = 0;
        msg.msgId = MSG_GUI_UPDATE_CUVETTE_DATA; 
#ifdef GUI_ENABLE
        SendGuiMessage (&msg, sizeof (GuiReceiveMsg_t));
#endif
    }
        //send command to Engine
    if (analyzeError != SERR_NONE) {
        infoText.type = CUVETTE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// send command to Engine
void
StateCuvetteTest::
SendCuvetteTestMessage (char command)
{
   char Parameter [16];
   memset (Parameter, 0, 16);

   SendTestRequestMessage (command, Parameter, 16);

}

// save test resutls to file
void
StateCuvetteTest::
SaveTestResults (cuvetteTestParameters_t *cuvetteTestParameters)
{
    FILE    *fp;

    fp = fopen (CUVETTE_TEST_RESULTS_FILE, "wb");
    if (fp != NULL) {
        fwrite (cuvetteTestParameters, sizeof (cuvetteTestParameters_t), 1, fp);
        fclose (fp);
    } else {
        LogError ("Fail to open %s", CUVETTE_TEST_RESULTS_FILE);
    }
}

// reset all display counter
void
StateCuvetteTest::
ResetDisplayParameter (GuiMiddleCuvetteTestData_t *middleCuvetteTestData)
{
    int i;

    if (cuvetteTestFlags.resetDisplayFlag == 1) {
        for (i = 0; i < 6; i++) {
            wsprintf (&middleCuvetteTestData->valueText [i][0], L"0");
        } 
        cuvetteTestFlags.resetDisplayFlag = 0;
        cuvetteTestFlags.guiUpdateFlag = 1;
    }
}
