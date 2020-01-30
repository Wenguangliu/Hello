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
#include "LogErr.h"

unsigned char       StateBarcodeTest::barcodeCurrentDacValue;
BarcodeTestFlags_t  StateBarcodeTest::barcodeTestFlags;

#define             MAX_BARCODE_DAC             150 //max barcode dac could be set for jen II engine board

BarcodeResultStrings_t resultStr [] = 
                           {    {SERR_NONE,                 "Barcode read successful"},
                                {SERR_SPINDLE_MOTOR_STUCK,  ":Spindle motor stuck"},
                                {SERR_BC_END_TRANSITION,    ":Barcode no end quiet zone"},
                                {SERR_BC_TRANSITIONS,       ":Barcode end quiet zone err"},
                                {SERR_BC_NO_START,          ":Barcode no start pattern"},
                                {SERR_BC_NO_STOP,           ":Barcode no stop pattern"},
                                {SERR_BC_BAD_DECODE,        ":Barcode bad decode"},
                                {SERR_BC_QUIET_ZONE,        ":Barcode no quiet zone"},
                                {SERR_BC_SET_CURRENT,       ":Barcode set current fail"},
                                {SERR_BC_READ_TIMEOUT,      ":Barcode read timeout"},
                                {SERR_BC_CALIBRATION_TIMEOUT,":Barcode calibration timeout"},
                                {(SystemErrorNumber_t)0xFFFF,":Barcode end string"} 

                           };
                          

#define GUI_ENABLE

char   *
StateBarcodeTest::
FindBarcodeTestResultString (SystemErrorNumber_t errorNum)
{
    BarcodeResultStrings_t  *barcodeResultStrings;

    barcodeResultStrings = &resultStr [0];

    while (barcodeResultStrings->errorNum != 0xFFFF) {
        if (barcodeResultStrings->errorNum == errorNum) {
            break;
        }
        barcodeResultStrings++;
    }
    if (barcodeResultStrings->errorNum != 0xFFFF) {
        return barcodeResultStrings->resultStr;
    } 
    
    return ("Unknow Barcode Error");
}

// Barcode Test process  
void
StateBarcodeTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Barcode Test";
    GuiMiddleTestData_t     *middleTestData;
    
    // get default Barcode DAC value 
    barcodeCurrentDacValue = calibrationData->barcodeDacs [1];

    SetTopLabel (topLable);

    // clear test flags
    memset (&barcodeTestFlags, 0, sizeof (BarcodeTestFlags_t));

    // try to get current drawer status
    SendDrawerMessage ('S');

    middleTestData =  &msg.data.middleTestData;
    memset (middleTestData, 0, sizeof (GuiMiddleTestData_t));
    
    // initial barcode test data strings
    wcscpy (&middleTestData->resultsText[0],  L" ");
    wsprintf (middleTestData->valueText, L"%u", barcodeCurrentDacValue);
    wsprintf (middleTestData->middleButton0, L"%s", L"Calibration");
    wsprintf (middleTestData->middleButton1, L"%s", L"Read");
    wsprintf (middleTestData->middleButton2, L"%s", L"Open");
    msg.msgId = MSG_GUI_MIDDLE_BARCODE_DATA;

#ifdef GUI_ENABLE
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
#endif
    SetBottomButtons (strings[BSTR_BACK], true);
}

// Barcode test process
state_t
StateBarcodeTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_BARCODE_READ_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    GuiMiddleTestData_t         *middleTestData;
    unsigned char               status;
    static  char                guiUpdate = 0;
    SystemErrorNumber_t         errNum;

    middleTestData =  &msg.data.middleTestData;
    memset (middleTestData, 0, sizeof (GuiMiddleTestData_t));
           
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            //Back, always close drawer
            if (barcodeTestFlags.barcodeStartTestFlag == 0) {
                SendDrawerMessage ('C');
                retVal = stateStack.Pop();
            }
        break;

        case MSG_GUI_OUT_BUTTON:
            printf ("idata =%d\n\r", idata);
            switch (idata) {
                case 0:         //calibration button
                    // disable heater polling
                    if (barcodeTestFlags.drawerReadyFlag == 1) {
                        SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
                        SendBarcodeTestMessage ('C', barcodeCurrentDacValue);
                        wcscpy (middleTestData->resultsText, L"Barcode read calibrating ...");
                        barcodeTestFlags.guiUpdateFlag = 1;
                        barcodeTestFlags.barcodeStartTestFlag = 1;
                    }

                break;
                
                case 1:         //read button
                    if (barcodeTestFlags.drawerReadyFlag == 1) {
                        // disable heater polling
                        SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
                        SendBarcodeTestMessage ('R', barcodeCurrentDacValue);
                        wcscpy (middleTestData->resultsText, L"Barcode reading ...");
                        barcodeTestFlags.guiUpdateFlag = 1;
                        barcodeTestFlags.barcodeStartTestFlag = 1;
                    }
                break;

                case 2:         //open button
                    if (barcodeTestFlags.drawerOpenFlag) {
                        // drawer opened, close drawer
                        SendDrawerMessage ('C');
                    } else {
                        if (barcodeTestFlags.barcodeStartTestFlag == 0) {
                            SendDrawerMessage ('O');
                        }
                    }
        
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_UP:    // dac increase      
            if (barcodeCurrentDacValue < MAX_BARCODE_DAC) {
                barcodeCurrentDacValue++;
                wsprintf (middleTestData->valueText, L"%u", barcodeCurrentDacValue);
                barcodeTestFlags.guiUpdateFlag = 1;
            }
        break;

        case MSG_GUI_OUT_DOWN:     // dac decrease 
            if (barcodeCurrentDacValue > 0) {
                barcodeCurrentDacValue--;
                wsprintf (middleTestData->valueText, L"%u", barcodeCurrentDacValue);
                barcodeTestFlags.guiUpdateFlag = 1;
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
                        wcscpy (&middleTestData->resultsText[0],  L"Drawer jammed");
                    } else if (status & 0x10) {//drawer closed
                        barcodeTestFlags.drawerOpenFlag = 0;
                        wsprintf (middleTestData->middleButton2, L"%s", L"Open");
                        if (status & 0x40) {//rotor present
                            barcodeTestFlags.drawerReadyFlag = 1;
                            barcodeTestFlags.guiUpdateFlag = 1;
                            wcscpy (&middleTestData->resultsText[0],  L"Ready for barcode test");
                        } else {
                            wcscpy (&middleTestData->resultsText[0],  L"Please open drawer and load rotor");
                        }
                    } else if (status & 0x20) { // drawer open
                        wcscpy (&middleTestData->resultsText[0],  L"Please load rotor");
                        wsprintf (middleTestData->middleButton2, L"%s", L"Close");
                        barcodeTestFlags.drawerReadyFlag = 0;
                        barcodeTestFlags.drawerOpenFlag = 1;
                    }
                    barcodeTestFlags.guiUpdateFlag = 1;
                break;

                case 'B':  //get barcode reading feed back 
                    // enable heater polling
                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    //errNum = ProcessRotorBarcode( (const unsigned short *)(engineCommMsg->message + 8), barcodeData, BC_DIGITS_FILE);
                    errNum = ProcessRotorBarcode ((const unsigned short *)(engineCommMsg->message + BARCODE_DATA_OFFSET),
                                                   (unsigned short *)(engineCommMsg->message + BARCODE_PARAMETER_OFFSET),
                                                   barcodeData, BC_DIGITS_FILE);

                    printf ("errNum =0x%x\n\r", errNum);
                    // ingor date error
                    if  ((errNum == SERR_BC_ILLEGAL_DATE) || 
                         (errNum == SERR_IMPROPER_DATE)   ||
                         (errNum == SERR_EXPIRED_ROTOR)) {
                            errNum = SERR_NONE;
                        }
                    if (errNum != SERR_NONE) {
                        printf ("barcode read fail err=%x\n\r", errNum);
                        wsprintf (middleTestData->resultsText, L"0x%x%s",errNum, CstrToWstr (FindBarcodeTestResultString (errNum)));
                    } else {
                        UpdateBarcodeCalibrationData ((unsigned char *)engineCommMsg, middleTestData);
                        printf ("barcode read success!!!\n\r");
                        wcscpy (middleTestData->resultsText, CstrToWstr (FindBarcodeTestResultString (errNum)));
                    }
                    barcodeTestFlags.guiUpdateFlag = 1;
                    barcodeTestFlags.barcodeStartTestFlag = 0;
                break;

                case 'E':
                    // enable heater polling
                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    // error code from Engine
                    errNum = (SystemErrorNumber_t)*(short *)&engineCommMsg->message[8];
                    printf ("get barcode error =0x%x\n\r", errNum);
                    wsprintf (middleTestData->resultsText, L"0x%x%s",errNum, CstrToWstr (FindBarcodeTestResultString (errNum)));
                    barcodeTestFlags.guiUpdateFlag = 1;
                    barcodeTestFlags.barcodeStartTestFlag = 0;
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
    if (barcodeTestFlags.guiUpdateFlag == 1) {
        barcodeTestFlags.guiUpdateFlag = 0;
        msg.msgId = MSG_GUI_UPDATE_BARCODE_DATA; 
#ifdef GUI_ENABLE
        SendGuiMessage (&msg, sizeof (GuiReceiveMsg_t));
#endif
    }
        //send command to Engine
    if (analyzeError != SERR_NONE) {
        infoText.type = BARCODE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// save barcode DAC value to xml file
// update Dac value display
void
StateBarcodeTest::
UpdateBarcodeCalibrationData (unsigned char * message, GuiMiddleTestData_t *middleTestData)
{
    calibrationData->barcodeDacs[0] = message[4];
    calibrationData->barcodeDacs[1] = message[5];
    WriteCalibration (calibrationData);
    // update display DAC value
    barcodeCurrentDacValue =  message[5];
    swprintf (middleTestData->valueText, L"%u", barcodeCurrentDacValue);
}

