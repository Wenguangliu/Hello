#include "StartupGlobal.h"

// Process pids
static int                  analysisPid     = 0;
static int                  calcResultsPid  = 0;
static int                  engineCommPid   = 0;
static int                  errLoggerPid    = 0;
static int                  guiPid          = 0;
static int                  heaterControlPid= 0;
static int                  recallPid       = 0;
static int                  settingsPid     = 0;
static int                  printerPid      = 0;
static int                  randDPid        = 0;
static int                  astmPid         = 0;
static int                  serialCommPid   = 0;
static int                  hl7Pid          = 0;
static int                  amsPid          = 0;
static int                  endPid          = 0;
static int                  errorProcessPid = 0;


// Comm queues
HANDLE                      analysisQueue;
HANDLE                      astmQueue;
HANDLE                      calcResultsQueue;
HANDLE                      errLoggerQueue;
HANDLE                      heaterControlQueue;
HANDLE                      recallQueue;
HANDLE                      serialCommQueue;
HANDLE                      settingsQueue;
HANDLE                      startupQueue;
HANDLE                      watchdogQueue;
HANDLE                      startupTimeoutQueue;
HANDLE                      engineCommQueue;
HANDLE                      guiQueue;
HANDLE                      errorProcessQueue;


// Shared memory
BarcodeData_t *             barcodeData;
CalibrationData_t *         calData;
CounterData_t *             counterData;
FactoryData_t *             factoryData;
HardwareData_t *            hardwareData;
OrdersList_t *              ordersList;
RawData_t *                 rawData;
RotorConfig_t *             rotorConfig;
RotorResults_t *            results;
SettingsData_t *            settings;
SystemData_t *              systemData;
ResultsDbChanged_t *        dbChanged;
ExpiryData_t *              expiryData;
RefRangeData_t *            refRanges;
UnitsConfig_t *             unitsConfig;
VersionData_t *             versionData;
AmsData_t   *               amsData;
Hl7Configuration_t *        hl7Config;
Hl7ConfigurationFileList_t* hl7FileList;

//#define NETWORK_TEST_PROGRAM  "/Hard Disk/Abaxis/bin/networkTest.exe"



// process Id table
typedef struct {
    int         processId;
    char        processName [128];
} ProcessName_t;

ProcessName_t ProcessNameTable [] = 
    {
        // processId        process name
        {analysisPid,       ANALYSIS_PROGRAM},
        {calcResultsPid,    CALCRESULTS_PROGRAM},
        {engineCommPid,     ENGINECOMM_PROGRAM},
        {errLoggerPid,      ERRLOGGER_PROGRAM},
        {guiPid,            GUI_PROGRAM},
        {heaterControlPid,  HEATERCONTROL_PROGRAM},
        {recallPid,         RECALL_PROGRAM},
        {settingsPid,       SETTINGS_PROGRAM},
        {printerPid,        PRINTER_PROGRAM},
        {randDPid,          RANDD_PROGRAM},
        {astmPid,           ASTM_PROGRAM},
        {hl7Pid,            HL7_PROGRAM},
        {serialCommPid,     SERIALCOMM_PROGRAM},
        {amsPid,            AMS_PROGRAM},
        {errorProcessPid,   ERROR_PROCESS_PROGRAM},
        {endPid,            NULL}
    };


// start process and assign process Id for process killing 
int
CreateProcess (const char * processName)
{
    ProcessName_t  *processNamePtr;
    BOOL           found = FALSE;

    processNamePtr = &ProcessNameTable [0];

    while (processNamePtr->processName != NULL) {
        if (strcmp (processNamePtr->processName, processName) == 0) {
            found = TRUE; 
            break;
        } 
        processNamePtr++;
    }

    if (found == TRUE) {
        processNamePtr->processId = StartProcess (processNamePtr->processName);
    } else {
        LogError ("Fail to findout process Name %s", processName);
    }
    return processNamePtr->processId; 
}

// start process and assign process Id for process killing
int
CreateProcess (const char * processName,  char * const argv[])
{
    ProcessName_t  *processNamePtr;
    BOOL           found = FALSE;

    processNamePtr = &ProcessNameTable [0];

    while (processNamePtr->processName != NULL) {
        if (strcmp (processNamePtr->processName, processName) == 0) {
            found = TRUE;
            break;
        }
        processNamePtr++;
    }
    if (found == TRUE) {
        processNamePtr->processId = StartProcess (processNamePtr->processName, argv);
    } else {
        LogError ("Fail to findout process Name %s", processName);
    }
    return processNamePtr->processId; 
}


// kill one porcess based on process name 
void
KillProcessByName (char *processName)
{

    DWORD           exitCode;
    ProcessName_t   *processNamePtr;
    BOOL            found = FALSE;
    HANDLE          process;

    processNamePtr = &ProcessNameTable [0];

    while (processNamePtr->processName != NULL) {
        if (strcmp (processNamePtr->processName, processName) == 0) {
            found = TRUE;
            break;
        }
        processNamePtr++;
    }
    if (found == TRUE) {
        process = OpenProcess (0,FALSE, processNamePtr->processId);
    }
    if (process != NULL) {
        GetExitCodeProcess(process, &exitCode);
        TerminateProcess(process, exitCode);
        CloseHandle(process);
    }
}

// kill one porcess based on PID
void
KillProcessById (DWORD pid)
{

    DWORD       exitCode;
    HANDLE      process;

    process = OpenProcess (0, FALSE, pid);
   
    if (process != NULL) {
        GetExitCodeProcess(process, &exitCode);
        TerminateProcess(process, exitCode);
        CloseHandle(process);
    }
}


// kill all processes
// Flag = WATCHDOG_STOP,       system timeout, need keep printer process alive
// Flag = GOTO_MANUFACTURE,    kill all current processes
// Flag = GOTO_REBOOT          kill all current processes
void
KillAll (killAllFlag_t flag)
{

    KillProcessByName (ANALYSIS_PROGRAM);
    KillProcessByName (CALCRESULTS_PROGRAM);
    KillProcessById   (systemData->commPid);      // Either ASTM or SerialComm or HL7, depending on which was started.
    KillProcessByName (RECALL_PROGRAM);
    KillProcessByName (SETTINGS_PROGRAM);
    KillProcessByName (HEATERCONTROL_PROGRAM);
    KillProcessByName (RANDD_PROGRAM);
    KillProcessByName (ENGINECOMM_PROGRAM);
    KillProcessByName (ERROR_PROCESS_PROGRAM);
    //Watch dog timeout, keep printer process to test 
    //printer port  
    if (flag != WATCHDOG_STOP) {
        KillProcessByName (PRINTER_PROGRAM);
    }

    if ((flag == GOTO_MANUFACTURE)  || (flag == GOTO_REBOOT)){
        KillProcessByName (ERRLOGGER_PROGRAM);
        KillProcessByName (GUI_PROGRAM);
        CloseMsgQueue (analysisQueue);
        CloseMsgQueue (astmQueue);
        CloseMsgQueue (calcResultsQueue);
        CloseMsgQueue (engineCommQueue);
        CloseMsgQueue (errLoggerQueue);
        CloseMsgQueue (guiQueue);
        CloseMsgQueue (heaterControlQueue);
        CloseMsgQueue (recallQueue);
        CloseMsgQueue (serialCommQueue);
        CloseMsgQueue (settingsQueue);
        CloseMsgQueue (startupQueue);
        CloseMsgQueue (startupTimeoutQueue);
        CloseMsgQueue (errorProcessQueue);

        // Delete the shared memory.
        UnmapViewOfFile (barcodeData);
        UnmapViewOfFile (calData);
        UnmapViewOfFile (counterData);
        UnmapViewOfFile (factoryData);
        UnmapViewOfFile (rawData);
        UnmapViewOfFile (results);
        UnmapViewOfFile (rotorConfig);
        UnmapViewOfFile (settings);
        UnmapViewOfFile (systemData);
        UnmapViewOfFile (hl7Config);
        UnmapViewOfFile (ordersList);
    } 
    
}

// create message queues 
void
CreateMessageQueues (void)
{
     // Create the message queues.
    analysisQueue       = CreateMessageQueue (ANALYSIS_QUEUE_NAME, TRUE);
    astmQueue           = CreateMessageQueue (ASTM_QUEUE_NAME, TRUE);
    calcResultsQueue    = CreateMessageQueue (CALC_RESULTS_QUEUE_NAME, TRUE);
    engineCommQueue     = CreateMessageQueue (ENGINE_COMM_QUEUE_NAME, FALSE);
    errLoggerQueue      = CreateMessageQueue (ERR_LOGGER_QUEUE_NAME, TRUE);
    guiQueue            = CreateMessageQueue (GUI_QUEUE_NAME, FALSE);
    heaterControlQueue  = CreateMessageQueue (HEATER_CONTROL_QUEUE_NAME, TRUE);
    recallQueue         = CreateMessageQueue (RECALL_QUEUE_NAME, TRUE);
    serialCommQueue     = CreateMessageQueue (SERIAL_COMM_QUEUE_NAME, TRUE);
    settingsQueue       = CreateMessageQueue (SETTINGS_QUEUE_NAME, FALSE);
    startupQueue        = CreateMessageQueue (STARTUP_QUEUE_NAME, TRUE);
    watchdogQueue       = CreateMessageQueue (WATCHDOG_QUEUE_NAME, TRUE);
    startupTimeoutQueue = CreateMessageQueue (STARTUP_QUEUE_NAME, FALSE);
    errorProcessQueue   = CreateMessageQueue (ERROR_PROCESS_QUEUE_NAME, TRUE);

}

// create all share meomries
void
CreateShareMemories (void)
{
     // Create the shared memory.
    barcodeData     = (BarcodeData_t *)     MapSharedMem (BARCODE_SHMEM_NAME,   sizeof (BarcodeData_t));
    calData         = (CalibrationData_t *) MapSharedMem (CAL_DATA_SHMEM_NAME,  sizeof (CalibrationData_t));
    counterData     = (CounterData_t *)     MapSharedMem (COUNTERS_SHMEM_NAME,  sizeof (CounterData_t));
    dbChanged       = (ResultsDbChanged_t *)MapSharedMem (DATABASE_SHMEM_NAME,  sizeof (ResultsDbChanged_t));
    expiryData      = (ExpiryData_t *)      MapSharedMem (EXPIRY_SHMEM_NAME,    sizeof (ExpiryData_t));
    factoryData     = (FactoryData_t *)     MapSharedMem (FACTORY_SHMEM_NAME,   sizeof (FactoryData_t));
    hardwareData    = (HardwareData_t *)    MapSharedMem (HARDWARE_SHMEM_NAME,  sizeof (HardwareData_t));
    ordersList      = (OrdersList_t *)      MapSharedMem (ORDERS_SHMEM_NAME,    sizeof( OrdersList_t));
    rawData         = (RawData_t *)         MapSharedMem (RAW_SHMEM_NAME,       sizeof (RawData_t));
    refRanges       = (RefRangeData_t *)    MapSharedMem (REF_RANGES_SHMEM_NAME,sizeof (RefRangeData_t));
    results         = (RotorResults_t *)    MapSharedMem (RESULTS_SHMEM_NAME,   sizeof (RotorResults_t));
    rotorConfig     = (RotorConfig_t *)     MapSharedMem (ROC_SHMEM_NAME,       sizeof (RotorConfig_t));
    settings        = (SettingsData_t *)    MapSharedMem (SETTINGS_SHMEM_NAME,  sizeof (SettingsData_t));
    systemData      = (SystemData_t *)      MapSharedMem (SYSTEM_SHMEM_NAME,    sizeof (SystemData_t));
    unitsConfig     = (UnitsConfig_t *)     MapSharedMem (UNITS_SHMEM_NAME,     sizeof (UnitsConfig_t));
    versionData     = (VersionData_t *)     MapSharedMem (VERSION_SHMEM_NAME,   sizeof (VersionData_t));
    amsData         = (AmsData_t *)         MapSharedMem (AMS_SHMEM_NAME,       sizeof (AmsData_t));
    hl7Config       = (Hl7Configuration_t *)MapSharedMem (HL7_SHEME_NAME,       sizeof (Hl7Configuration_t));
    hl7FileList     = (Hl7ConfigurationFileList_t *)MapSharedMem (HL7_FILE_SHEME_NAME, sizeof (Hl7ConfigurationFileList_t));
}
