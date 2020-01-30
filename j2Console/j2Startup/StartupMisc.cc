#include "StartupGlobal.h"


// debug flag
#undef                      STARTUP_DEBUG   // for debug message


// create message queue
HANDLE
CreateMessageQueue (const wchar_t * queueName, BOOL read)
{
    HANDLE                  queue;
    MSGQUEUEOPTIONS         options;

    options.dwSize          = sizeof(MSGQUEUEOPTIONS);
    options.dwFlags         = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess     = read;
    options.dwMaxMessages   = 0;
    options.cbMaxMessage    = MAX_QUEUE_LENGTH;

    queue = CreateMsgQueue (queueName, &options);

    if (queue == NULL) {
        LogError ("failed to create message queue %s", queueName);
        return 0;
    }

    return queue;
}

// send factroy setting message to Engine board 
void
SendFactoryMessage (FactoryData_t * factoryData)
{
    EngineCommReceiveMsg_t  msg;
    int                     i;

    // Create the message.
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'F';
    msg.data.engineMsg.message[1] = ESC;
    msg.data.engineMsg.message[2] = 32;     // LSB of binary count.
    msg.data.engineMsg.message[3] = 0;      // MSB of binary count.

    msg.data.engineMsg.message[4] = factoryData->globalCuvetteDelay & 0xFF;
    msg.data.engineMsg.message[5] = factoryData->globalCuvetteDelay >> 8;

    for (i = 0; i < 30; i++) {
        msg.data.engineMsg.message[i+6] = factoryData->cuvetteDelays[i];
    }

    msg.data.engineMsg.length = 36;

    if (engineCommQueue != NULL) {
        WriteMsgQueue(engineCommQueue, &msg, sizeof (EngineCommReceiveMsg_t), INFINITE, NULL);
    }
}

// send Gui message to GUI process
void
SendGuiMessage (GuiReceiveMsg_t * msg, int msgDataLen)
{
    BOOL result;
    if (guiQueue != NULL) {
        result = WriteMsgQueue (guiQueue, msg, msgDataLen + sizeof (GuiMsgId_t), INFINITE, NULL);
    }
}

/*
 * Inform EngineComm that we are to receive the Engine messages.  We'll use the
 * GUI's message queue since the GUI hasn't been started yet anyway.
 */
void
SendMessageReceiverMessage ()
{
    EngineCommReceiveMsg_t msg;

    if (engineCommQueue != 0) {
        // Create the message.
        msg.msgId = MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER;
        sprintf(msg.data.messageReceiver.name, "%S", STARTUP_QUEUE_NAME);

        // Send the message.
        WriteMsgQueue(engineCommQueue, &msg, sizeof( int ) + sizeof( EngineCommMessageReceiverData_t ), INFINITE, NULL);
    }
}

// display critical error messages
void
SendCriticalErrorMessage (SystemErrorNumber_t errNum)
{
    wchar_t			buff[500];
    GuiReceiveMsg_t msg;

    
    wcscpy (msg.data.winError.topLabel, (*strings)[ TSTR_CRITICAL_STOP ]);
    wsprintf( buff, L"%x\n%s", errNum, (*strings)[systemError->GetScreenText( errNum)]);
    wcscpy (msg.data.winError.message, buff);
    wcscpy (msg.data.winError.buttonText, (*strings)[ BSTR_RESTART ] );
    msg.msgId = MSG_GUI_WIN_CRITICAL_STOP2;
    SendGuiMessage(&msg, sizeof(msg));

}

// Register to talk to Gui process
void
RegisterGuiComm (const wchar_t * queueName)
{
    GuiReceiveMsg_t msg;

    msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
    //strcpy( msg.data.inputReceiver.name, queueName );
    wsprintf(msg.data.inputReceiver.name, L"%s", queueName);

    SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );

}

// send Fan Idle speed to engine
void
SendFanMessage(char cmd, unsigned char dac)
{
    EngineCommReceiveMsg_t  msg;

    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0]  = 'H';
    msg.data.engineMsg.message[1]  = cmd;    // cmd = F, set fan idle speed
    msg.data.engineMsg.message[2]  = ESC;
    msg.data.engineMsg.message[3]  = 10;     // LSB of binary count.
    msg.data.engineMsg.message[4]  = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5]  = 0;      // Top DAC, two bytes for 10 bit  DAC
    msg.data.engineMsg.message[6]  = 0;
    msg.data.engineMsg.message[7]  = 0;      // Bootom DAC, two bytes for 10 bit DAC
    msg.data.engineMsg.message[8]  = 0;
    msg.data.engineMsg.message[9]  = dac;    // Fan idle speed, low = 0, high = 1 
    msg.data.engineMsg.message[10] = 0;      // Fan Speed Dac 
    msg.data.engineMsg.message[11] = 0;      // air temperature offset 4 bytes
    msg.data.engineMsg.message[12] = 0;
    msg.data.engineMsg.message[13] = 0;
    msg.data.engineMsg.message[14] = 0;
    msg.data.engineMsg.length = 15;

    if (engineCommQueue != 0) {
        WriteMsgQueue(engineCommQueue, &msg, sizeof (EngineCommReceiveMsg_t), INFINITE, NULL);
    }
}

// send printer message
void
SendPrinterMessage (PrinterMsg_t *msg)
{
    static HANDLE   printerQueue = NULL;

    if (printerQueue == NULL) {
        printerQueue = CreateMessageQueue (PRINTER_QUEUE_NAME, FALSE);
    }

    if (printerQueue != NULL) {
        WriteMsgQueue(printerQueue, msg, sizeof (PrinterMsg_t) + sizeof (int), INFINITE, NULL);
    } else {
        LogError ("Fail to create print queue");
    }
}

/*
 * read log file size to be used by tail utility
 */
void
SetLogFileSize (void)
{
    wchar_t             wFileName [256];
    WIN32_FIND_DATA     fileData;
    HANDLE              hSearch;
    char                *fileName = "/Hard Disk/Abaxis/logOffset.dat";
    FILE                *fp;


    fp =fopen (fileName,"wb");
    if (fp == NULL) {
        return;
    }

    wsprintf (wFileName, L"%S",  "/Hard Disk/Abaxis/log.current");
    hSearch = FindFirstFile (wFileName, &fileData);
    if (hSearch == INVALID_HANDLE_VALUE) {
        fileData.nFileSizeLow = 0;
    } else {
        FindClose (hSearch);
    }
    // write the current log file size to logOffset.dat
    fwrite (&fileData.nFileSizeLow, sizeof (fileData.nFileSizeLow), 1, fp);
    fclose (fp);

    return;
}

// start new process
DWORD
StartProcess (const char * processName)
{
    DWORD pid = 0;
    PROCESS_INFORMATION     proc_info;
    int             result;
    wchar_t         localWSTR[255];

    wsprintf(localWSTR, L"%S", processName);

    result = CreateProcess(localWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
    pid = proc_info.dwProcessId;

    if (result == 0) {
            LogError ("failed to start process %s", processName);
    }
    return pid;
}


//start new process
DWORD
StartProcess (const char * processName, char * const argv[])
{
    DWORD pid = 0;
    PROCESS_INFORMATION     proc_info;
    int             result;
    wchar_t         process[256];
    wchar_t         commandline[256];

    wsprintf(process, L"%S", processName);
    wsprintf(commandline, L"%S", argv[0]);

#ifdef STARTUP_DEBUG
    LogDebug("StartProcess | process: %S",process);
    LogDebug("StartProcess | commandline: %S",commandline);
#endif
    result = CreateProcess(process, commandline, NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
    pid = proc_info.dwProcessId;

    if (result == 0) {
            LogError ("failed to start process %s", processName);
    }
    return pid;
}

void *
CreateResultsDatabase ()
{
    HANDLE  lock;
    DWORD   dwWaitResult;

    // Lock the database.
    lock = CreateMutex(
        NULL,               // default security attributes
        FALSE,              // initially not owned
        RESULTS_LOCK);      // unnamed mutex
    if ( lock == NULL )
    {
        LogError( "unable to lock database" );
        return 0;
    }
    dwWaitResult = WaitForSingleObject( lock, INFINITE );
    if ( dwWaitResult != WAIT_OBJECT_0 )
    {
        LogError( "unable to lock database" );
        CloseHandle(lock);
        return 0;
    }

    CreateResultsDatabaseFiles( CUSTOMER_DB_INDEX, CUSTOMER_DB_DATA );
    CreateResultsDatabaseFiles( OTHER_DB_INDEX, OTHER_DB_DATA );

    ReleaseMutex(lock);
    CloseHandle(lock);

    return 0;
}

void
CreateResultsDatabaseFiles( const char * indexFileName, const char * dataFileName )
{
    int                         i;
    FILE *                      indexFile;
    FILE *                      dataFile;
    // These are static because they're large and making them static avoids having
    // to increase the size of the thread's stack.
    static  ResultsDbIndex_t    resultsIndex;
    static  ResultsDbEntry_t    resultsDbEntry;
    DWORD                       error;

    // Determine whether the index file exists.
    indexFile = fopen( indexFileName, "r+b" );
    error = GetLastError();
    if ( indexFile == NULL )
    {
        // Index file doesn't exist, so create new database files.
        if ( error == ERROR_FILE_NOT_FOUND )
        {
            LogInfo( "creating results database (%s)", dataFileName );

            // Create the index file.
            indexFile = fopen( indexFileName, "wb" );
            if ( indexFile == 0 )
            {
                LogError( "error creating results index file %s", indexFileName );
                return;
            }
            memset( &resultsIndex, 0xFF, sizeof(ResultsDbIndex_t ) );
            resultsIndex.dbVersion = RESULTS_DB_VERSION;
            resultsIndex.lastEntry = RESULTS_MAX - 1;
            fwrite( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, indexFile );
            fclose( indexFile );

            // Create the data file.
            dataFile = fopen( dataFileName, "wb" );
            if ( dataFile == 0 )
            {
                LogError( "error creating results database file %s", dataFileName );
                return;
            }

            memset( &resultsDbEntry, 0, sizeof( ResultsDbEntry_t) );

            for ( i = 0; i < RESULTS_MAX; i++ )
            {
                fwrite( &resultsDbEntry, sizeof( ResultsDbEntry_t ), 1, dataFile );
            }

            fclose( dataFile );
        }
        else
        {
            LogError( "error opening results index" );
                return;
        }
    }

    else
    {
        fclose( indexFile );
    }
}

// setting message to j2Settings process
void
SendSettingsMessage (SettingsMsgId_t msgId)
{
    if (settingsQueue != NULL) {
        WriteMsgQueue (settingsQueue, &msgId, sizeof(SettingsMsgId_t), INFINITE, NULL);
    }
}

// send error message to GUI
void
SendErrorMessage (SystemErrorNumber_t errNum)
{
    wchar_t			buff[500];
    GuiReceiveMsg_t msg;

    msg.msgId = MSG_GUI_WIN_ERROR;
    wcscpy (msg.data.winError.topLabel, (*strings)[TSTR_ERROR]);
    wsprintf( buff, L"%x\n%s", errNum, (*strings)[systemError->GetScreenText(errNum)]);
    wcscpy (msg.data.winError.message, buff);
    wcscpy (msg.data.winError.buttonText, (*strings)[BSTR_CONTINUE]);
    SendGuiMessage (&msg, sizeof(msg));
}


// send test command to Engine board to enable/disable 
// Engine board debug port
void
SendTestRequestMessage (char command)
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
    msg.data.engineMsg.length = 21;

    // Send the message.
   if (engineCommQueue != NULL) {
        WriteMsgQueue(engineCommQueue, &msg, sizeof (EngineCommReceiveMsg_t), INFINITE, NULL);
    }

}

/*
 * read eeprom to get expire flag
 */
int
GetExpireFlag (void)
{
    unsigned    i;
    char        buff[20];

    for (i = 20; i < 30; i++) {
        EepromRead (i, (char *)&buff [i - 20]);
    }

    if (strncmp( "EXPIREACT", buff, 9 ) == 0) {
        LogInfo ("Expire Time Active");
        return 1;
    }

    return -1;
}

// check expired days left during boot up
// only RandD key could disable this check
void
ExpireDateCheck ()
{
    //in the case of expire file exist,check software expire date
    if ((GetExpireFlag () == 1) && (systemData->inRandDMode == false)) {
        if (DetectExpireDateFile () == 1) {
            SYSTEMTIME currentDate; int expirdDayLeft; unsigned int expireDays;
            GetLocalTime (&currentDate);
            expirdDayLeft = CalculateExpireDateLeft (currentDate, &expireDays);
            LogDebug ("System expired day left %d", expirdDayLeft);
            if (expirdDayLeft  <= 0) {
                LogError ("Software Day Expire");
                // time expired, stop system
                ErrorStop (SERR_SOFTWARE_EXPIRED, systemData);
            } else {
                LogInfo ("Expire days left=%d", expirdDayLeft);
            }
        } else {
            LogError ("Expire date file deleted");
            ErrorStop (SERR_SOFTWARE_EXPIRED, systemData);
        }
    }

}
