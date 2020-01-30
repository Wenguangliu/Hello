#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "RandDStates.h"
#include "DrawerTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "ParseManufacturingTestSet.h"
#include "LogErr.h"

AStrings    strings;

// error information display 
InfoText_t    infoTextTable []  = 
                                 {
                                    // type                     title                       message                                         info
                                    {DRAWER_TEST_TYPE,          L"Drawer Test Error",            L"\0",                                   L"Drawer or Cam jammed"},
                                    {MOTOR_TEST_TYPE,           L"Motor Test Error",             L"\0",                                   L"Spindle motor test fail"},
                                    {BARCODE_TEST_TYPE,         L"Barcode Test Error",           L"\0",                                   L"Fail to read barcode"},
                                    {TEMPERATURE_TEST_TYPE,     L"Temperature Test Error",       L"\0",                                   L"Temperature beyond limit"},
                                    {TRIM_DAC_CALI_TYPE,        L"Trim Dac Cali Error",          L"\0",                                   L"Fail to calibration trim DAC value"},
                                    {CUVETTE_TEST_TYPE,         L"Cuvette Test Error",           L"\0",                                   L"Fail to calibration cuvette"},
                                    {NETWORK_IP_TYPE,           L"IP  Error",                    L"IP configueration error",              L"Fail to configure device IP Address"},
                                    {NETWORK_DHCP_TYPE,         L"DHCP Error",                   L"DHCP configueration error",            L"Fail to configure Ethernet device DHCP"},
                                    {NETWORK_SOCKET_TYPE,       L"Socket Error",                 L"Socket configuration error",           L"Fail to configure device socket"},
                                    {NETWORK_HOST_NAME_TYPE,    L"Sysem Host Name Error",        L"Invalidate host name",                 L"Invalidate Host Name"},
                                    {AMS_MOTOR_TEST_TYPE,       L"Motor Test Error",             L"\0",                                   L"Motor test fail"}, 
                                    {AMS_NDXT_TEST_TYPE,        L"NDXT Test Error",              L"\0",                                   L"NDXT test fail"},
                                    {AMS_TEMP_TEST_TYPE,        L"Temperature Test Error",       L"\0",                                   L"Temperature test fail"},
                                    {AMS_DELAY_TEST_TYPE,       L"Delay Test Error",             L"\0",                                   L"Cuvette delay test fail"}, 
                                    {AMS_BARCODE_TEST_TYPE,     L"Barcode Test Error",           L"\0",                                   L"Barcode read test fail"}, 
                                    {AMS_ALIGNMENT_TEST_TYPE,   L"Alignment Test Error",         L"\0",                                   L"Alignment alignment test fail"}, 
                                    {AMS_CUV_TEST_TYPE,         L"Cuvette Test Error",           L"\0",                                   L"Cuvette test fail"}, 
                                    {END_OF_INFO_TYTE,            NULL,                          L"\0",                                   NULL}
                                 };


// RandD static variables
RawData_t *             StateRandD::rawData = (RawData_t *)MapSharedMem (RAW_SHMEM_NAME, sizeof (RawData_t));
CalibrationData_t *     StateRandD::calibrationData = (CalibrationData_t *)MapSharedMem( CAL_DATA_SHMEM_NAME, sizeof (CalibrationData_t));
FactoryData_t     *     StateRandD::factoryData    = (FactoryData_t *)MapSharedMem (FACTORY_SHMEM_NAME, sizeof (FactoryData_t));
BarcodeData_t *         StateRandD::barcodeData    = (BarcodeData_t *)MapSharedMem (BARCODE_SHMEM_NAME, sizeof( BarcodeData_t ));
SettingsData_t *        StateRandD::settings       = (SettingsData_t *)MapSharedMem (SETTINGS_SHMEM_NAME, sizeof(SettingsData_t));
SystemData_t   *        StateRandD::systemData     = (SystemData_t *)MapSharedMem (SYSTEM_SHMEM_NAME, sizeof(SystemData_t));
SystemErrorNumber_t     StateRandD::analyzeError = SERR_NONE;
HANDLE                  StateRandD::timerThread  = NULL;
ManufacturingFlags_t    StateRandD::manufacturingFlags;
ManufacturingTestSet_t  StateRandD::manufacturingTestSet;
InfoText_t              StateRandD::infoText;
int                     StateRandD::progBarValue = 0;
int                     StateRandD::warmupDoneFlag = 0;
wchar_t                 StateRandD::displayText [4000];
const wchar_t *         StateRandD::infoMsg = L"";
char                    StateRandD::testCount   = 0;


// background state. Set Gui and EngineComm processes talk to Analuysis process
void
StateBackground::
Enter()
{
    SendAnalysisMessage (MSG_ANALYSIS_GOTO_FOREGROUND);
}

// waiting for message from Analysis process 
// if forgrond message is received, send request to Gui and EngineComm to ask for 
// talk to Gui and EngineComm Processes  directly
state_t
StateBackground::
ProcessMessage (int msgId, char * data)
{
    state_t retVal = STATE_BACKGROUND;
    
    switch (msgId) {

        case MSG_MANUFACTURE_GOTO_FOREGROUND:
            //register GUI queue 
            InitGuiComm (MANUFACTURE_QUEUE_NAME);
            //register Engine Queue 
            InitEngineComm (MANUFACTURE_QUEUE_NAME);
            stateStack.Push (STATE_BACKGROUND);
            memset (&manufacturingFlags, 0, sizeof (ManufacturingFlags_t));
            retVal = STATE_MAIN_SELECTION;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

// enter main selection menu
void
StateMainSelection::
Enter (void)
{

    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Analyzer Test Selection";
    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Troubleshooting");
    wcscpy (data->buttonText[1], L"Manufacturing");
 
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_HOME], true);

}

state_t
StateMainSelection ::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_MAIN_SELECTION;
    int     idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {
                case 0: // troubshooting
                    stateStack.Push (STATE_MAIN_SELECTION);
                    retVal = STATE_TROUBLE_SHOOTING;
                break;

                case 1: // Manufacturing
                    stateStack.Push (STATE_MAIN_SELECTION);
                    retVal = STATE_MANUFACTURING_SELECTION;
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal =  stateStack.Pop();
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    return retVal;
}



// get error info title and text
void
StateRandDError::
GetInfoString (void)
{
    // search table
    int i = 0;
    while (infoTextTable [i].type != END_OF_INFO_TYTE) {
        if (infoTextTable [i].type == infoText.type) {
            wcscpy (infoText.title,     infoTextTable [i].title);
            // drawer test, need display conters
            if (infoText.type != DRAWER_TEST_TYPE) {
                wcscpy (infoText.text,      infoTextTable[i].text);
            }
            if (infoTextTable[i].message [0] != '\0') {
                wcscpy (infoText.message,   infoTextTable[i].message);
            } else {
                memset (infoText.message, 0, sizeof (infoText.message));
            }
            break;
        }
        i++;
    }
    // something wrong
    if (infoTextTable [i].type == END_OF_INFO_TYTE) {
        wcscpy (infoText.title, L"No Title");
        wcscpy (infoText.text,  L"No Debug message");
        wcscpy (infoText.message, L"No Debug message");
    }
}

// error handling 
void
StateRandDError::
Enter ()
{
    GuiReceiveMsg_t     msg;
    StringId_t          msgStr;

    // get error infomation string
    GetInfoString ();
    msgStr = systemError.GetScreenText (analyzeError);
    // record infoText
    msg.msgId = MSG_GUI_WIN_WARNING1;
    if (infoText.message [0] != '\0') {
        wcscpy (msg.data.winError.topLabel, infoText.title);
        wsprintf(msg.data.winError.message, L"%s\n",infoText.message);
    } else {
        wcscpy (msg.data.winError.topLabel, L"Test Error");
        wsprintf(msg.data.winError.message, L"%x\n%s\n\n", analyzeError, strings [msgStr]);
    }
    wcscpy (msg.data.winError.buttonText, strings[BSTR_CONTINUE]);
    SendGuiMessage (&msg, sizeof (GuiWinWarningData_t));
}

// how to process error ????
state_t
StateRandDError::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_MANUFACTURING_ERROR;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:     // Continue
            // clear Error flag
            analyzeError = SERR_NONE;
            retVal = stateStack.Pop();
        break;

        case MSG_GUI_OUT_INFO:
            stateStack.Push (STATE_MANUFACTURING_ERROR);
            retVal = STATE_MANUFACTURING_INFO;
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}

// error information window
void
StateRandDInfo::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    SetTopLabel (infoText.title);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    wcscpy (msg.data.middleViewResults.text, infoText.text);
    SendGuiMessage (&msg, sizeof(GuiReceiveMsg_t));
    SetBottomButtons (strings[BSTR_BACK], true);
}

state_t
StateRandDInfo::
ProcessMessage (int msgId, char * data)
{
     int     retVal = STATE_MANUFACTURING_INFO;
     switch (msgId) {

        case MSG_GUI_OUT_BOTTOM_BUTTON:     //back 
             retVal = stateStack.Pop();
        break;

        default:
             ProcessOtherMessages (msgId, data);
        break;
        
     }

     return retVal;
}

// other message process
void
StateRandD::
ProcessOtherMessages (int msgId, char * data)
{
    EngineCommMessageData_t *   engineMsg;
    SystemErrorNumber_t         errNum;
    int                         count;

    errNum = SERR_NONE;

    switch (msgId) {
        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
            engineMsg = (EngineCommMessageData_t *)data;
            switch (engineMsg->message[0]) {
                 // Error number sent by the Engine.
                case 'E':
                    errNum = (SystemErrorNumber_t)*(short *)&engineMsg->message[8];
                    LogError( "got Engine error %x", errNum );
                break;

                // Engine debug message.
                case '?':
                    count = *(unsigned short *)&engineMsg->message[2];
                    LogInfo ("Engine ? msg: %*.*s", count, count, &engineMsg->message[4]);
                break;

                // Indicates the Engine has reset asynchronously.
                case 'Z':
                    errNum = SERR_ENGINE_RESET;
                break;

                default:
                break;
            }
        break;

        case MSG_MANUFACTURE_TIMER:
        break;

        // analzyer warmup is done
        case MSG_MANUFACTURE_WARMUP_DONE:
            warmupDoneFlag = 1;
            LogDebug ("RandD Received Warmup Done Status");
        break;

        default:
        break;
    }
    if (errNum != SERR_NONE) {
        // Set the global error flag.
        analyzeError = errNum;
    }
}

// create test file sub folders
void
StateRandD::
CreateTestDirectory (char *dir)
{
    wchar_t             wDirectroyName [128];
    HANDLE              hSearch;
    WIN32_FIND_DATA     fileData;

    /* check directory */
    wsprintf (wDirectroyName, L"%S",  dir);
    hSearch = FindFirstFile (wDirectroyName, &fileData);
    /* directory does not exist, create one */
    if (hSearch == INVALID_HANDLE_VALUE) {
        if (CreateDirectory (wDirectroyName, NULL) == 0) {
            LogError ("Fail to create %S directory");
        }
    } else {
        FindClose (hSearch);
    }
}

void
StateRandD::
SendBarcodeTestMessage (char command, unsigned char curDac)
{
     EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'B';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 4;      // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5] = 0;
    msg.data.engineMsg.message[6] = curDac; // barcode Dac value
    msg.data.engineMsg.message[7] = 0;
    msg.data.engineMsg.message[8] = 0;
    msg.data.engineMsg.length = 9;

    // Send the message.
    SendEngineCommMessage (&msg, sizeof (EngineCommReceiveMsg_t));

}

// barcode alignment test
void
StateRandD::
SendAlignmentTestMessage (char command, unsigned char curDac)
{
     EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'I';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 4;      // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5] = 0;
    msg.data.engineMsg.message[6] = curDac; // barcode Dac value
    msg.data.engineMsg.message[7] = 0;
    msg.data.engineMsg.message[8] = 0;
    msg.data.engineMsg.length = 9;

    // Send the message.
    SendEngineCommMessage (&msg, sizeof (EngineCommReceiveMsg_t));

}

void
StateRandD::
SendCalibrationFactorsMessage()
{
    EngineCommReceiveMsg_t  msg;
    int                     i;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'C';
    msg.data.engineMsg.message[1] = ESC;
    msg.data.engineMsg.message[2] = 10;     // LSB of binary count.
    msg.data.engineMsg.message[3] = 0;      // MSB of binary count.
    for (i = 0; i < NUM_WAVELENGTHS; i++) {
        msg.data.engineMsg.message[4+i] = calibrationData->wavelengthDacTrims[i];
    }
    msg.data.engineMsg.length = 14;
    // Send the message.
    SendEngineCommMessage( &msg, sizeof( EngineCommMessageData_t ) );
}

// send command to Engien for cuvette detection test
void
StateRandD::
SendCuvetteDetectionCommand (char command)
{
    EngineCommReceiveMsg_t  msg;


    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'Q';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 14;     // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5] = 50;     // Ams cuvette test start current (DAC from 50 to 120)
    msg.data.engineMsg.message[6] = 0x20;   // Ams cuvette test threshold
    msg.data.engineMsg.length = 19;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}


// set timer by crate timerThread
// time = second
// Sleeps the given length of time, then sends a message to signal the main thread
void
StateRandD::
SetTimer(int time)
{
    static int  _time;

    _time = time;

    // Cancel the current timer, if there is one.
    if (timerThread != NULL) {
        TerminateThread (timerThread, 0);
        CloseHandle (timerThread);
    }
    timerThread = NULL;

    // Create a new timer.
    if (_time >= 0) {
        timerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, &_time, 0, NULL);
    }
}

void
StateRandD::
CloseTimer (void)
{
    // Cancel the current timer, if there is one.
    if (timerThread != NULL) {
        TerminateThread (timerThread, 0);
        CloseHandle (timerThread);
    }
    timerThread = NULL;
}

// timerThread
void *
StateRandD::
TimerThread (void * arg)
{
    const int       msg = MSG_MANUFACTURE_TIMER;

    static HANDLE   queue = NULL;

    unsigned        time = *(unsigned *)arg;

    if (queue == NULL) {
        MSGQUEUEOPTIONS         options;
        options.dwSize = sizeof (MSGQUEUEOPTIONS);
        options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess = FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage = MAX_QUEUE_LENGTH;

        queue = CreateMsgQueue(MANUFACTURE_QUEUE_NAME, &options);

        if (queue == NULL) {
            LogError( "failed to open queue %s", ANALYSIS_QUEUE_NAME );
            return 0;
        }
    }
    Sleep (time * 1000);
    WriteMsgQueue(queue, (void *)&msg, sizeof( int ), INFINITE, NULL);

    return 0;
}

//send Enging Drawer command
void
StateRandD::
SendDrawerMessage (char command)
{
    EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'D';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.length = 2;

    // Send the message.
    SendEngineCommMessage (&msg, sizeof( EngineCommMessageData_t));
}

// send Engine Heater Command
void
StateRandD::
SendHeaterCommand (char command, unsigned short tDac, unsigned short bDac, float airOffset)
{
    EngineCommReceiveMsg_t  msg;

    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'H';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 10;                 // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;                  // MSB of binary count
    /* add extra value for debug purpose */
    memcpy (&msg.data.engineMsg.message[5], &tDac, 2);  //top heater setting
    memcpy (&msg.data.engineMsg.message[7], &bDac, 2);  //bottom heater setting
    msg.data.engineMsg.message[9] = 0;                  // fan settings
    msg.data.engineMsg.message[10] = airOffset;
    msg.data.engineMsg.length = 15;
    /* Send the message */
    SendEngineCommMessage (&msg, sizeof( EngineCommMessageData_t ));
}



// send analysis A command
void
StateRandD::
SendAMessage (char command)
{
    EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'A';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.length = 2;

    // Send the message.
    SendEngineCommMessage (&msg, sizeof(EngineCommMessageData_t));
}


// get all test waiting time duing boot up from configuration xml file
void
StateRandD::
ReadManufacturingTestTimeSet (void)
{
    // clear to be zero
    memset (&manufacturingTestSet, 0, sizeof (ManufacturingTestSet_t));
    // get fanufacturing test set parameters
    ParseManufacturingTestSet (&manufacturingTestSet);
#if 0
    // ZZZ debug
    int i;
    printf ("dec4000Min =%d\n", manufacturingTestSet.dec4000Min);
    printf ("dec3000Min =%d\n", manufacturingTestSet.dec3000Min);
    printf ("max offset = %d\n", manufacturingTestSet.maxBlackOffset);
    printf ("nd05Exabs:");
    for (i = 0;  i < 9; i++) {
        printf ("%.3f, ", manufacturingTestSet.nd05ExpectedAbs [i]);
    }
    printf ("\n");
    printf ("nd10Exabs:");
    for (i = 0;  i < 9; i++) {
        printf ("%.3f, ", manufacturingTestSet.nd10ExpectedAbs [i]);
    }
    printf ("\n");
    printf ("nd20Exabs:");
    for (i = 0;  i < 9; i++) {
        printf ("%.3f, ", manufacturingTestSet.nd20ExpectedAbs [i]);
    }
    printf ("\n");
    printf ("bandBlock:");
    for (i = 0;  i < 9; i++) {
        printf ("%.3f, ", manufacturingTestSet.bandBlockLimits [i]);
    }
    printf ("\n");
    printf ("nd05DiffLimit = %.3f\n", manufacturingTestSet.nd05DiffLimit);
    printf ("nd10DiffLimit = %.3f\n", manufacturingTestSet.nd10DiffLimit);
    printf ("nd20DiffLimit = %.3f\n", manufacturingTestSet.nd20DiffLimit);
    printf ("cuvSlopeMin  =%.3f\n", manufacturingTestSet.cuvSlopeMin);
    printf ("cuvSlopeMax  =%.3f\n", manufacturingTestSet.cuvSlopeMax);
    printf ("cuvOffseeMin  =%d\n",  manufacturingTestSet.cuvOffsetMin);
    printf ("cuvOffsetMax  =%d\n",  manufacturingTestSet.cuvOffsetMax);
#endif
}

// ask barcode test 
void
StateRandD::
SendBarcodeMessage (char command)
{
    EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'B';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 4;      // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5] = 120;
    msg.data.engineMsg.message[6] = 120;
    msg.data.engineMsg.message[7] = 0;
    msg.data.engineMsg.message[8] = 0;
    msg.data.engineMsg.length = 9;

    // Send the message.
    SendEngineCommMessage (&msg, sizeof (EngineCommReceiveMsg_t));
}

// send request message to engine board
void
StateRandD::
SendTestRequestMessage (char command,  char *parmeters, unsigned char parameLen)
{
    EngineCommReceiveMsg_t  msg;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'G';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 16;                 // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;                  // MSB of binary count.
    memset (&msg.data.engineMsg.message[5], 0, 16);     // 16 bytes parameters
    if ((parmeters != NULL) && (parameLen != 0))  {
        if (parameLen > 16) {
            parameLen = 16;
        }
        memcpy (&msg.data.engineMsg.message[5], parmeters, parameLen);
    }
    msg.data.engineMsg.length = 21;
    // Send the message.
    SendEngineCommMessage (&msg, sizeof (EngineCommReceiveMsg_t));
}

// fox child process
DWORD
StateRandD::
StartProcess (const char * processName)
{
    DWORD                   pid = 0;
    PROCESS_INFORMATION     proc_info;
    int             result;
    wchar_t         localWSTR[255];

    wsprintf (localWSTR, L"%S", processName);

    result = CreateProcess(localWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
    pid = proc_info.dwProcessId;

    if (result == 0) {
            LogError ("failed to start process %s", processName);
    }
    return pid;
}

// send ris file to Engine board
SystemErrNum_t
StateRandD::
SendRisFile  (char *risFileName)
{
    FILE *                  fp;
    unsigned char           fileBuff[3000];
    unsigned                fsize;
    EngineCommReceiveMsg_t  msg;
    DWORD                   error;


    fp = fopen (risFileName, "rb");
    if (fp == NULL)  {
        error = GetLastError();
        LogError( "error opening RIS file %s", risFileName );
        if (error == ERROR_FILE_NOT_FOUND) {
            return SERR_RIS_NOT_THERE;
        } else {
            return SERR_FILE_READ;
        }
    }

    // read ris file to buffer
    fsize = fread (fileBuff, sizeof( char ), 3000, fp);
    fclose( fp );

    if (fsize > 0) {
        msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
        msg.data.engineMsg.message[0] = 'P';
        msg.data.engineMsg.message[1] = ESC;
        msg.data.engineMsg.message[2] = fsize & 0xFF;   // LSB of binary count.
        msg.data.engineMsg.message[3] = fsize >> 8;     // MSB of binary count.
        memcpy( msg.data.engineMsg.message + 4, fileBuff, fsize );
        msg.data.engineMsg.length = fsize + 4;
        SendEngineCommMessage( &msg, sizeof( EngineCommReceiveMsg_t ) );
    } else {
        LogError( "error reading RIS file %s", risFileName );
        return SERR_FILE_READ;

    }
    return SERR_NONE;
}

// function to get file size
int
StateRandD::
GetFileSize (char *fileName)
{
    int                     fileSize = 0;
    HANDLE                  hSearch;
    wchar_t                 wFileName [128];
    WIN32_FIND_DATA         fileData;

    wsprintf(wFileName, L"%S", fileName);
    hSearch = FindFirstFile(wFileName, &fileData);
    if (hSearch != INVALID_HANDLE_VALUE) {
        fileSize = fileData.nFileSizeLow;
        FindClose (hSearch);
        printf ("file size = %d\n", fileSize);
    }
    return fileSize;
}

int
StateRandD::
SaveTestResultsToUsb (char *sFileName, char *reportStr)
{
    wchar_t             wDirectroyName [128];
    HANDLE              hSearch;
    WIN32_FIND_DATA     fileData;
    char                dateTimeStr [24];
    SYSTEMTIME          sysTime;
    FILE                *fp;
    char                fileName [256];
    int                 strLen;

    // create directory 
    wsprintf (wDirectroyName, L"%S",  AMS_DATA_USB_DIRECTORY);
    // directory detection
    hSearch = FindFirstFile (wDirectroyName, &fileData);
    if (hSearch == INVALID_HANDLE_VALUE) {
        // create directory
        if (CreateDirectory (wDirectroyName, NULL) == 0) {
            LogError ("Fail to create %s directory", AMS_DATA_USB_DIRECTORY);
            FindClose (hSearch);
            return -1;
            
        }
     } else {
        FindClose (hSearch);
     }

    // get current time string
    memset (dateTimeStr, 0 ,sizeof (dateTimeStr));
    GetLocalTime(&sysTime);
    sprintf (dateTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", sysTime.wYear,
                                                      sysTime.wMonth,
                                                      sysTime.wDay,
                                                      sysTime.wHour,
                                                      sysTime.wMinute,
                                                      sysTime.wSecond);

    sprintf (fileName,"%s/%s.%s.%s.txt",AMS_DATA_USB_DIRECTORY, sFileName,factoryData->instrumentSerialNumber,dateTimeStr);

    fp = fopen (fileName, "wb");
    if (fp == NULL) { 
        return -1;
        LogError ("Fail to open file %s", fileName);
    } else {
        strLen =  fwrite (reportStr, strlen (reportStr), 1, fp);
        LogDebug ("strlen =%d\n", strlen (reportStr));
        fclose (fp);
    }


    return 0;
}


// error information 
void
StateInfo::
Enter()
{
    GuiReceiveMsg_t msg;

    SetTopLabel( strings[ TSTR_INFORMATION ] );

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    wcscpy( msg.data.middleViewResults.text, infoMsg );
    SendGuiMessage (&msg, sizeof( GuiMiddleViewResultsData_t ) );

    SetBottomButtons( strings[ BSTR_BACK ], true );
}

state_t
StateInfo::
ProcessMessage( int msgId, char * data )
{
    state_t retVal = STATE_INFO;

    switch ( msgId )
    {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
            break;

        default:
            break;
    }

    return retVal;
}
