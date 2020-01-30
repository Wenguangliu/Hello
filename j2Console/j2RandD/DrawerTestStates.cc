#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "DrawerTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"

DrawerErrorStr_t  drawerTestErrorStr [] = 
        {
            {SERR_NONE,              "No Error"},
            {SERR_DRAWER_OPEN_JAM,   "Drawer Open Jammed"},
            {SERR_DRAWER_CLOSE_JAM,  "Drawer Close Jammed"},
            {SERR_CAM_UP_JAM,        "Cam Up Jammed"},
            {SERR_CAM_DOWN_JAM,      "Cam Down Jammed"},
            {0xFFFF,                 "Not Valild Error"},
        };

// static variable
DrawerTestCounters_t    StateDrawerTest::drawerTestCounters;

// do nothing
void
StateDrawerTest::
Enter (void)
{
}

// drawer test, currently do nothing
state_t
StateDrawerTest::
ProcessMessage (int msgId, char *data)
{
    state_t    retVal = STATE_DRAWER_TEST;

    return retVal;
}


// copy test report to infoText buffer for error 
// information display
void
StateDrawerTest::
GetDrawerTestReport ()
{
    FILE                *fp;
    size_t              bufSize;

    bufSize =  sizeof (infoText.text);
    // clear buffer
    memset (infoText.text, 0, bufSize);
    fp = fopen (AMS_DRAWER_TEST_REPORT_FILE, "rb");
    if (fp == NULL) {
        wcscpy (infoText.text, L"Test Error Information is not available");
    } else {
        fread (infoText.text, 1, bufSize, fp);
        fclose (fp);
    }
}

// display drawer test coounter during drawer test
void
StateDrawerTest::
DisplayDrawerTestCounter (void)
{
    GuiReceiveMsg_t     msg;
    GuiMiddleListData_t *data = &msg.data.middleList;
    char                i, numItems = 4;
    InfoItem_t          info [NUM_INFO_ITEMS];

    // display drawer counters
    strcpy (info[0].label, "Open Counter");
    sprintf (info[0].value,"%u",drawerTestCounters.openCounter);
    strcpy (info[1].label, "Close Counter");
    sprintf (info[1].value,"%u",drawerTestCounters.closeCounter);
    strcpy (info[2].label, "Up Counter");
    sprintf (info[2].value,"%u",drawerTestCounters.upCounter);
    strcpy (info[3].label, "Down Counter");
    sprintf (info[3].value,"%u",drawerTestCounters.downCounter);
    msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;
    memset (data, 0, sizeof (GuiMiddleListData_t));
    //copy the counter to info
    for (i =0; i < numItems; i++) {
        wcscpy (data->items[i][0], CstrToWstr (info[i].label));
        wcscpy (data->items[i][1], CstrToWstr (info[i].value));
    }
    data->isSelectable = 0;
    data->hasUpDown = 0;
    SendGuiMessage (&msg, sizeof(GuiMiddleListData_t));
}


#define MAX_DRAWER_TEST_REPORT_LENGTR       1024

//format test counter and test results 
//saved to file
void
StateDrawerTest::
FormatTestReport (void)
{
    char                *reportStr;
    FILE                *fp;
    char                *pBuff;
    SYSTEMTIME          time;
    int                 i;

    // alloc report string memory
    reportStr = (char *)malloc (MAX_DRAWER_TEST_REPORT_LENGTR);
    if (reportStr == NULL) {
        LogError ("Fail to allocate drawer test report memory");
        return;
    }
    memset (reportStr, 0, sizeof (MAX_DRAWER_TEST_REPORT_LENGTR));
    pBuff = reportStr;
    // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);
    pBuff += strlen (pBuff);

    sprintf (pBuff, "Drawer Open  Counter: %u\n", drawerTestCounters.openCounter);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Drawer Close Counter: %u\n", drawerTestCounters.closeCounter);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Cam    Up    Counter: %u\n", drawerTestCounters.upCounter);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Cam    Down  Counter: %u\n\n", drawerTestCounters.downCounter);
    pBuff += strlen (pBuff);

    
    // try to find out error string
    i = 0;
    while (drawerTestErrorStr [i].errorCode != 0xFFFF) {
        if (drawerTestErrorStr [i].errorCode == analyzeError) {
            break;
        }
        i++;
    }
    sprintf (pBuff, "Drawer Test Result: %s\n", drawerTestErrorStr [i].errStr);
    pBuff += strlen (pBuff);

    // write the test counter to file
    fp = fopen (AMS_DRAWER_TEST_REPORT_FILE, "wb");
    if (fp == NULL) {
        free (reportStr);
        LogError ("Fail to Open Drawer Test Report File");
        return;
    }
    fwrite (reportStr, 1, strlen (reportStr), fp);
    fclose (fp);
    free (reportStr);
}



// drawer open state  
void
StateDrawerIsOpened::
Enter (void)
{
    GuiReceiveMsg_t msg;
    wchar_t         *str = L"Close drawer to start test";
    wchar_t         *topLable = L"Drawer Test";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons (strings[BSTR_BACK], false, strings [BSTR_CLOSE], true);
}

// close drawer. in the case of first run,  
// close drawer shall start test 
state_t
StateDrawerIsOpened ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_DT_DRAWER_IS_OPENED;
    int                         idata = *(int *)data;
     
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: //back
                    // drawer must be closed before exit drawer test mode
                    SendDrawerMessage ('C');
                    retVal = stateStack.Pop();
                break;

                case 1://close drawer and start test
                    // set test flag to be true
                    manufacturingFlags.drawerTestFlag = true;
                    retVal = STATE_DT_DRAWER_IS_CLOSING;
                break;

                default:
                break;
            }
        break;
        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        GetDrawerTestReport ();
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}


// enter drawer test open state  
void
StateDrawerIsOpening::
Enter (void)
{
    GuiReceiveMsg_t msg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, strings[MSTR_OPENING_DRAWER]);
    SendGuiMessage (&msg, sizeof (GuiMiddleMessageData_t ));

    SetBottomButtons ();

    SendDrawerMessage('O');

}

// drawer is openging
state_t
StateDrawerIsOpening ::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_DRAWER_IS_OPENING;
    EngineCommMessageData_t *   engineCommMsg;

    switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            if (engineCommMsg->message[0] == 'D') {
                unsigned char status = engineCommMsg->message[8];
                if (status & 0x80) { // drawer jammed
                    analyzeError = SERR_DRAWER_OPEN_JAM;
                    LogError( "Drawer Test: drawer jammed");
                    infoText.type = DRAWER_TEST_TYPE;
                    //retVal = STATE_MANUFACTURING_ERROR;
                } else if (status & 0x20) { // drawer opened
                    if (manufacturingFlags.drawerTestFlag == true) {
                        drawerTestCounters.openCounter++;
                        // test have been started, goto waiting time state 
                        retVal = STATE_DT_WAIT_FOR_NEXT_CYCLE;
                    } else {
                        // test not started yet, goto drawer closing state
                        retVal = STATE_DT_DRAWER_IS_OPENED;
                        memset (&drawerTestCounters, 0, sizeof (DrawerTestCounters_t));
                    }
                } else {
                    LogError ("unexpected drawer status: %x", status);
                    retVal = STATE_DRAWER_TEST;
                }
            } else { // process other command
                 ProcessOtherMessages (msgId, data);
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        GetDrawerTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }

    return retVal;
}

// enter drawer test open state
void
StateDrawerIsClosing::
Enter (void)
{
    GuiReceiveMsg_t msg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, strings[MSTR_CLOSING_DRAWER]);
    SendGuiMessage (&msg, sizeof (GuiMiddleMessageData_t ));

    msg.msgId = MSG_GUI_BOTTOM_NONE;
    SendGuiMessage( &msg, 0 );
    SendDrawerMessage('C');
}

// drawer is closing
state_t
StateDrawerIsClosing ::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_DRAWER_IS_CLOSING;
    EngineCommMessageData_t *   engineCommMsg;
    switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            if (engineCommMsg->message[0] == 'D') {
                unsigned char status = engineCommMsg->message[8];
                if (status & 0x80) {//drawer jammed
                    analyzeError = SERR_DRAWER_CLOSE_JAM;
                    LogError ("drawer jammed");
                    infoText.type = DRAWER_TEST_TYPE;
                    //retVal = STATE_MANUFACTURING_ERROR;
                } else if (status & 0x10) { //drawer closed
                    if (status & 0x40) {
                        // cam is up
                        drawerTestCounters.closeCounter++;
                        drawerTestCounters.upCounter++;
                        manufacturingFlags.camFlag = true;
                        //loaded rotor Cam up, goto ris file download
                        //retVal = STATE_DT_DOWNLOAD_RIS_FILE;
                        retVal = STATE_DT_READ_BARCODE;
                    } else {
                        drawerTestCounters.closeCounter++;
                       // no rotor loaded, one cycle test finised, goto analysis state  
                        retVal = STATE_DT_TEST_COMPLETED; 
                        manufacturingFlags.camFlag = false;
                    }
                }
            } else {
                ProcessOtherMessages (msgId, data);
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        GetDrawerTestReport ();
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}



// one drawer test cycle completed (open/close)
// display test counters
void
StateDrawerTestCompleted::
Enter (void)
{
    wchar_t *label = L"Drawer Test Counters";

    SetTopLabel (label);
    // display counter
    DisplayDrawerTestCounter ();
    SetBottomButtons ( strings[BSTR_CANCEL], true);
    // enable 5 seconds timeout thread for display counter
    SetTimer (manufacturingTestSet.countDisplayTime);

    
}

// drawer is openging
state_t
StateDrawerTestCompleted::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_TEST_COMPLETED; 

    switch (msgId) {

        case MSG_MANUFACTURE_TIMER: //disply timeout, re-open the drawer
            retVal = STATE_DT_DRAWER_IS_OPENING;
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON: // test concel button pressed
            // save waiting timer in the case of recover test 
            manufacturingTestSet.savedWaitTime = manufacturingTestSet.countDisplayTime;
            // cancel timer
            SetTimer (-1);
            // drawer test cancelled 
            retVal = STATE_DT_CONFIRM_CANCEL;
            // save current state
            stateStack.Push (STATE_DT_TEST_COMPLETED);
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        GetDrawerTestReport ();
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// displlay test counter 
void
StateDrawerTestCounterDisplay::
Enter (void)
{
    wchar_t *label = L"Drawer Test Counters";

    SetTopLabel (label);
    DisplayDrawerTestCounter ();
    SetBottomButtons (strings[BSTR_BACK], true);
}

// drawer is openging
state_t
StateDrawerTestCounterDisplay::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_TEST_COUNTER_DISPLAY; 

    switch (msgId) {

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();;
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

// wait for next test
void
StateDrawerWatingForNextCycle::
Enter (void)
{
    GuiReceiveMsg_t msg;
    wchar_t            *str = L"Waiting for next cycle test";
    wchar_t            *topLabel = L"Drawer Test";

    SetTopLabel (topLabel);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons (strings[BSTR_CANCEL], true);
    SetTimer (manufacturingTestSet.nextDrawerTestWaitTime);
}

// what next to do
state_t
StateDrawerWatingForNextCycle::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_WAIT_FOR_NEXT_CYCLE;

    switch (msgId) {

        case MSG_MANUFACTURE_TIMER: //timeout, need to re-open the drawer
            retVal = STATE_DT_DRAWER_IS_CLOSING;
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
             // save waiting timer
            manufacturingTestSet.savedWaitTime = manufacturingTestSet.nextDrawerTestWaitTime;
            // cancel timer
            SetTimer (-1);
            // drawer test cancelled
            retVal = STATE_DT_CONFIRM_CANCEL;
            // save current state
            stateStack.Push (STATE_DT_WAIT_FOR_NEXT_CYCLE);
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        GetDrawerTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// confirm canceling
void
StateDrawerTestCancelConfirm::
Enter (void)
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings[TSTR_CONFIRM]);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, strings [MSTR_CANCEL_ANALYSIS]);
    SendGuiMessage (&msg, sizeof(GuiMiddleMessageData_t));

    SetBottomButtons (strings[BSTR_CANCEL], false, strings[BSTR_NO], true);
}

// decide cancel or not
state_t
StateDrawerTestCancelConfirm::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_DT_CONFIRM_CANCEL;
    int idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            if (idata == 0) {   // cancel confirmed
                // save the test report
                FormatTestReport ();
                retVal = STATE_DT_CANCELING;
                stateStack.Pop();   // Discard return state.
            } else if (idata == 1)  {  // no cancel 
                // recover waiting timer
                SetTimer (manufacturingTestSet.savedWaitTime);
                retVal = stateStack.Pop ();
            }
            break;

        default:
            ProcessOtherMessages (msgId, data);
            break;
    }
    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        GetDrawerTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// confirm canceling
void
StateDrawerTestCancelling::
Enter (void)
{
    GuiReceiveMsg_t msg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, strings[MSTR_CANCELING_ANALYSIS]);
    SendGuiMessage(&msg, sizeof(GuiMiddleMessageData_t));

    msg.msgId = MSG_GUI_BOTTOM_NONE;
    SendGuiMessage (&msg, 0);

    // close drawer 
    SendDrawerMessage('C');
    // clear test flag
    manufacturingFlags.drawerTestFlag = false; 
}

state_t
StateDrawerTestCancelling::
ProcessMessage (int msgId, char *data)
{
    EngineCommMessageData_t *   engineCommMsg;
    state_t retVal = STATE_DT_CANCELING;

    switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            if (engineCommMsg->message[0] == 'D') {
                unsigned char status = engineCommMsg->message[8];
                printf ("Get cancel drawer status =%x\n\r", status);
                if (status & 0x10) { // drawer closed
                    // display final drawer test counters
                    retVal = STATE_DT_TEST_COUNTER_DISPLAY;
                } else if (status & 0x80) {//drawer jammed
                    analyzeError = SERR_DRAWER_CLOSE_JAM;
                    LogError ("drawer jammed");
                    infoText.type = DRAWER_TEST_TYPE;
                    //retVal = STATE_MANUFACTURING_ERROR;
                }
            } else { // process other command
                ProcessOtherMessages (msgId, data);
            }

        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        GetDrawerTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// read barcode during drawer test
void
StateReadBarcode::
Enter (void) 
{
    GuiReceiveMsg_t msg;

    SetTopLabel (0);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, L"Reading Barcode ...");
    SendGuiMessage (&msg, sizeof (GuiMiddleMessageData_t ));
    // bottom none buttton
    SetBottomButtons ();
    // disable watchdog during barcode read
    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
    // read barcode 
    SendBarcodeMessage ('R');
}

state_t
StateReadBarcode::
ProcessMessage (int msgId, char *data)
{
    EngineCommMessageData_t *   engineCommMsg;
    state_t retVal = STATE_DT_READ_BARCODE;
     switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineCommMsg = (EngineCommMessageData_t *)data;
            if (engineCommMsg->message[0] == 'B') {
                // enable watchdog during barcode read
                SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                //get barcode read done
                drawerTestCounters.downCounter++;
                retVal = STATE_DT_TEST_COMPLETED;
            } else {
                 ProcessOtherMessages (msgId, data);
            }
        break;

        default:
             ProcessOtherMessages (msgId, data);
        break;
    }

    if (analyzeError != SERR_NONE) {
        // save the test report
        FormatTestReport ();
        GetDrawerTestReport ();
        infoText.type = DRAWER_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}
