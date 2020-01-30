#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <tlhelp32.h>

#include "AnalysisMessages.h"
#include "GuiMessages.h"
#include "RecallMessages.h"
#include "SettingsMessages.h"
#include "Shmem.h"
#include "CreateDisplay.h"
#include "StateErrorProcess.h"
#include "GetThumbKeyMode.h"
#include "LogErr.h"


AStrings    strings;

SystemData_t    *StateErrorProcess::systemData     = (SystemData_t *)MapSharedMem (SYSTEM_SHMEM_NAME, sizeof(SystemData_t));

unsigned int    StateErrorProcess::errorNumber = 0;
char            StateErrorProcess::processName [120];
unsigned int    StateErrorProcess::foreGroundMsgId = 0;
char            StateErrorProcess::processKilledFlag = 0;


ProcessForegroundMsgIdTable_t  foregroundMsgIdTable [] = 
                                {
                                    {STARTUP_PROGRAM,    MSG_STARTUP_GOTO_FOREGROUND},
                                    {ANALYSIS_PROGRAM,   MSG_ANALYSIS_GOTO_FOREGROUND},
                                    {RECALL_PROGRAM,     MSG_RECALL_GOTO_FOREGROUND},
                                    {SETTINGS_PROGRAM,   MSG_SETTINGS_GOTO_FOREGROUND},
                                    {RANDD_PROGRAM,      MSG_MANUFACTURE_GOTO_FOREGROUND},
                                    {HL7_PROGRAM,        MSG_HL7_GOTO_FOREGROUND},
                                    {NULL,               0}
                                };

// in the case of shutdown, the following process needs to be killed
char *KillProcessNameTable []  = {
                                    "j2Analysis.exe",
                                    "j2Startup.exe",
                                    "j2Recall.exe",
                                    "j2Settings.exe",
                                    "J2RandD.exe",
                                    "j2HL7.exe",
                                    "j2CalcResults.exe",
                                    "j2Printer.exe",
                                    NULL
                                 };

// get process related foreground msgId for recoer that GUI process display
unsigned int
StateBackground::
GetForegroundMsgId (char *processName)
{
    char *ptr;
    int  found = 0;
    int  i = 0;

    ptr = processName;

    while (foregroundMsgIdTable [i].processName != NULL) {
        if ((strcmp (ptr, foregroundMsgIdTable [i].processName) == 0)) {
            found = 1;
            break; 
        }
        i++;
    }
    if (found == 1) {
        return foregroundMsgIdTable [i].foreGroundMsgId;
    } 

    LogDebug ("Fail to find out %s foregroud msgId", processName);
    return 0;
}  

int
StateBackground::
SendForegroundMsg (int msgId)
{
    switch (msgId) {

        case MSG_STARTUP_GOTO_FOREGROUND:
            SendStartupMessage (MSG_STARTUP_GOTO_FOREGROUND);
        break;
        case MSG_ANALYSIS_GOTO_FOREGROUND:
            SendAnalysisMessage (MSG_ANALYSIS_GOTO_FOREGROUND);
        break;

        case MSG_RECALL_GOTO_FOREGROUND:
            SendRecallMessage (MSG_RECALL_GOTO_FOREGROUND);
        break;

        case MSG_SETTINGS_GOTO_FOREGROUND:
            SendSettingsMessage (MSG_SETTINGS_GOTO_FOREGROUND);
        break;

        case MSG_MANUFACTURE_GOTO_FOREGROUND:
            SendManufactureMessage (MSG_MANUFACTURE_GOTO_FOREGROUND);
        break;

        default:
            LogDebug ("Does not support %d MsgId foregroud sending");
        break;
    } 
    return 0;
}

// wait for warning or error message from other processes
void 
StateBackground ::
Enter (void)
{
    // recover other process GUI int the warning cases
    if (foreGroundMsgId != 0) {
        SendForegroundMsg (foreGroundMsgId);        
    } else {
        // Register to GUI Comm .
//   InitGuiComm (ERROR_PROCESS_QUEUE_NAME);
    }
} 

state_t
StateBackground ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_BACKGROUND;
    ErrorProcessMsgData_t       iData = *(ErrorProcessMsgData_t *)data;
    SystemErrorAction_t         errorAction;
    // processing 
    switch (msgId) {
        case MSG_WARNING_REPORT:
        break; 

        case MSG_ERROR_REPORT:
            errorNumber = iData.errorNumber;
            errorAction = systemError.GetAction (errorNumber);
            strcpy (processName, iData.processName);
            LogDebug ("process name =%s error number =%x action =%d",processName,  errorNumber, errorAction);
            foreGroundMsgId = GetForegroundMsgId (processName);
            if (foreGroundMsgId != 0) {
                // now get action based on error number
                switch (errorAction) {
                    case SERR_ACTION_NONE:
                        retVal = STATE_BACKGROUND;
                    break;

                    case SERR_ACTION_WARNING:
                        retVal  = STATE_ACTION_WARNING;
                        stateStack.Push (STATE_BACKGROUND);
                    break;

                    case SERR_ACTION_ABORT:
                        retVal = STATE_ACTION_ABORT;
                        stateStack.Push (STATE_BACKGROUND);
                    break;

                    case SERR_ACTION_SHUTDOWN:
                        // register to GUI
                        InitGuiComm (ERROR_PROCESS_QUEUE_NAME);
                        retVal = STATE_ACTION_SHUTDOWN;
                        stateStack.Push (STATE_BACKGROUND);
                    break;

                    default:
                    break;
                }
            } 

        break;

        default:
        break;

        
    }
    return retVal;
}

void 
StateActionWarning ::
Enter (void)
{
    // needs to register GUI communication
} 

state_t
StateActionWarning ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_ACTION_WARNING;
    return retVal;
}

void 
StateActionAbort ::
Enter (void)
{
} 

state_t
StateActionAbort ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_ACTION_ABORT;
    return retVal;
}



// Get process name based on process name
DWORD
StateActionShutdown ::
GetProcId (TCHAR *procname)
{
    HANDLE proctable;
    PROCESSENTRY32 pe;
    BOOL success;

    proctable = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (proctable == INVALID_HANDLE_VALUE) {
        return -1;
    }

    pe.dwSize = sizeof (PROCESSENTRY32);
    success = Process32First(proctable, &pe);
    while (success){
        if (_tcsicmp(pe.szExeFile, procname) == 0) {
            return (pe.th32ProcessID);
        }
        success=Process32Next(proctable, &pe);
    }
    return -1;
}

// Kill process based on PID
int
StateActionShutdown ::
KillProcId (DWORD id)
{
    HANDLE process;
    DWORD exitCode;

    process = OpenProcess (0,0,id);
    if (process == INVALID_HANDLE_VALUE) {
        printf("OpenProcess Failed\n");
        return -1;
    }

    GetExitCodeProcess(process, &exitCode);
    if (!TerminateProcess(process, exitCode)) {
        printf("TerminateProcess Failed\n");
        return -1;

    }

    return 0;
}


// Kill process based on process table
void
StateActionShutdown ::
KillCurentProcesses (void)
{
    int i = 0;
    DWORD   pid = -1;
    TCHAR   wName [128];
    char    *processName;

    processName = KillProcessNameTable [i++];

    while (processName != NULL) {
       // printf ("kill process %s\n", processName);
        mbstowcs (wName, processName , 128);
        pid = GetProcId (wName);
        if (pid != -1) {
            KillProcId (pid);
        }
        processName = KillProcessNameTable [i++];
    }
}


// critical error action
void 
StateActionShutdown ::
Enter (void)
{
    GuiReceiveMsg_t     guiMsg;
    StringId_t          msgStr;
    const wchar_t *    topText;

    if (processKilledFlag == 0) {
        processKilledFlag = 1;
        KillCurentProcesses ();
    }

     // Flush all file buffers before displaying the error message.
    fflush (0);

    // Get the message text.
    msgStr = systemError.GetScreenText (errorNumber);
    guiMsg.msgId = MSG_GUI_WIN_CRITICAL_STOP;
    topText = strings [TSTR_CRITICAL_STOP];
    wcscpy (guiMsg.data.winError.topLabel, topText);

    // Add the error number to the beginning of the error message string.
    wsprintf (guiMsg.data.winError.message, L"%x\n%s", errorNumber, strings[msgStr]);

    wcscpy (guiMsg.data.winError.buttonText, strings [BSTR_CONTINUE]);
    SendGuiMessage (&guiMsg, sizeof (GuiWinWarningData_t));

} 

state_t
StateActionShutdown ::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_ACTION_SHUTDOWN;

    switch (msgId) {
        case MSG_GUI_OUT_INFO:
            // in the case R&D key plug in, show log file
            GetThumbKeyMode (systemData);
            if (systemData->inRandDMode) {
               retVal = STATE_VIEW_LOG;
            } else {
                retVal = STATE_INFO;
            }
            stateStack.Push (STATE_ACTION_SHUTDOWN);
            break;

        default:
            // Ignore all other input if we've reached a critical stop.
        break; 

    }
    return retVal;
}


// display warning/error information text
void
StateInfo::
Enter ()
{
    GuiReceiveMsg_t msg;

    SetTopLabel (strings [TSTR_INFORMATION]);

    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    wcscpy (msg.data.middleViewResults.text,  strings [systemError.GetInfoText(errorNumber)]);
    SendGuiMessage (&msg, sizeof (GuiMiddleViewResultsData_t));

    SetBottomButtons (strings[BSTR_BACK], true);
}

state_t
StateInfo::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_INFO;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break;

        default:
        break;
    }
    return retVal;
    
}

#if 0
void
StateViewLog::
Enter ()
{

    LogDebug ("View log file");
    SetBottomButtons (strings[BSTR_BACK], true);
}

state_t
StateViewLog::
ProcessMessage (int msgId, char * data)
{
    int     retVal = STATE_VIEW_LOG;

     switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break; 

        default:
        break;
    }

    return retVal;
}
#endif
