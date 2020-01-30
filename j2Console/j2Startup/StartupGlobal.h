#ifndef _STARTUP_GLOBAL_H
#define _STARTUP_GLOGAL_H
#include "StartupIncludeFiles.h"

// Comm queues
extern HANDLE               analysisQueue;
extern HANDLE               astmQueue;
extern HANDLE               calcResultsQueue;
extern HANDLE               errLoggerQueue;
extern HANDLE               heaterControlQueue;
extern HANDLE               recallQueue;
extern HANDLE               serialCommQueue;
extern HANDLE               settingsQueue;
extern HANDLE               startupQueue;
extern HANDLE               watchdogQueue;
extern HANDLE               startupTimeoutQueue;
extern HANDLE               engineCommQueue;
extern HANDLE               guiQueue;             
extern AStrings             *strings;
extern SystemError          *systemError;

// Shared memory
extern BarcodeData_t *                  barcodeData;
extern CalibrationData_t *              calData;
extern CounterData_t *                  counterData;
extern FactoryData_t *                  factoryData;
extern HardwareData_t *                 hardwareData;
extern OrdersList_t *                   ordersList;
extern RawData_t *                      rawData;
extern RotorConfig_t *                  rotorConfig;
extern RotorResults_t *                 results;
extern SettingsData_t *                 settings;
extern Hl7ConfigurationFileList_t *     hl7FileList;
extern SystemData_t *       systemData;
extern ResultsDbChanged_t * dbChanged;
extern ExpiryData_t *       expiryData;
extern RefRangeData_t *     refRanges;
extern UnitsConfig_t *      unitsConfig;
extern VersionData_t *      versionData;
extern AmsData_t   *        amsData;
extern Hl7Configuration_t * hl7Config;

extern bool                 guiInitFlag;

// Global functions
extern void
ActiveDefaultIpAddress (SettingsData_t *settings, SystemData_t *systemData);
extern  void *
DeviceNotificationHandler (void * arg);
extern void
StreamDeviceNotification (void);
extern void
SendFactoryMessage (FactoryData_t * factoryData);
extern HANDLE
CreateMessageQueue (const wchar_t * queueName, BOOL read);
extern void
SendGuiMessage (GuiReceiveMsg_t * msg, int msgDataLen);
extern void
SendMessageReceiverMessage ();
extern void
SendCriticalErrorMessage (SystemErrorNumber_t errNum);
extern void
RegisterGuiComm (const wchar_t * queueName);
extern void
SendFanMessage(char cmd, unsigned char dac);
extern void
SendPrinterMessage (PrinterMsg_t *msg);
extern void SetLogFileSize (void);
extern DWORD
StartProcess (const char * processName);
extern DWORD
StartProcess (const char * processName, char * const argv[]);
extern void
CreateResultsDatabaseFiles (const char * indexFileName, const char * dataFileName);
extern void *
CreateResultsDatabase();
extern void
SendErrorMessage (SystemErrorNumber_t errNum);
extern void
SendSettingsMessage (SettingsMsgId_t msgId);
extern void
ErrorStop (SystemErrorNumber_t errNum, SystemData_t *systemData);
extern int
ErrorInfo (SystemErrorNumber_t errNum);
extern void
GotoManufacturing ();
extern void
GotoRestart (bool shutdownFlag);
extern void
KillAll (killAllFlag_t flag);
extern int
CreateProcess (const char * processName);
extern int
CreateProcess (const char * processName,  char * const argv[]);
extern void
CreateShareMemories (void);
extern void
CreateMessageQueues (void);
extern void
SoftwareErrorState (SystemErrorNumber_t errNum);
extern void
ExpireDateCheck ();
extern void
SendTestRequestMessage (char command);
extern void
InitialGui (void);
extern void *
NetworkDetectionThread (void * arg);
#if 0
extern void
OutputNotifications (void);
extern void
DisableEventNotifications (void);
extern void
CleanDatabaseEvents (void);
#endif

#endif
