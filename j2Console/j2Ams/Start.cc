#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>


#include "Global.h"
#include "Serial.h"
#include "CommonDefines.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "FtdiConfig.h"
#include "Shmem.h"
#include "Epcs.h"
#include "ErrLoggerMessages.h"
#include "LogErr.h"

#include "ErrLoggerMessages.h"

static DWORD AmsEngineCommPid;
static DWORD AmsGuiPid;
static DWORD AmsPid;
static DWORD HeaterTestPid;
static DWORD ErrorLogPid;

/*
 * FUNCTION
 *   CreateMessageQueue
 *
 * DESCRIPTION
 *   created a message queue
 *
 * PARAMETERS
 *   const char * --- Queue name
 *
 * RETURNS
 *   mqd_t  --- message queue
 */

HANDLE
CreateMessageQueue (const WCHAR * queueName, BOOL read)
{
    HANDLE queue; 
    MSGQUEUEOPTIONS         options;

    // MSGQUEUEOPTIONS structure size
    options.dwSize          = sizeof (MSGQUEUEOPTIONS);
    // not free buffer
    options.dwFlags         = MSGQUEUE_ALLOW_BROKEN;
    // true = read
    options.bReadAccess     = read;
    // unlimited number of queue
    options.dwMaxMessages   = 0;
    // always 4096 bytes
    options.cbMaxMessage    = MAX_QUEUE_LENGTH;

    queue = CreateMsgQueue (queueName, &options);
    if (queue == NULL) {
        LogError ("failed to create message queue %s", queueName);
    }

    return queue;
}


/*
 * FUNCTION
 *  StartProcess 
 *
 * DESCRIPTION
 *  folk child process
 *
 * RETURNS
 *   pid    -- child process id
 */

DWORD
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

/* initial AMS serial port2 to talk to AMS PC station  */
int
InitSerialPort  (unsigned short portNum, FT_HANDLE *fdAms)
{


//    *fdAms = OpenSerialPort (portNum);
    FT_Open (portNum, fdAms); 

    if (*fdAms == INVALID_HANDLE_VALUE) {
        LogError ("Fail to open serial port %d", portNum);
		return -1;
    } else {
        SetSerialPortBaudRate (*fdAms, AMS_BAUD_RATE, portNum);
    }
    return 0;
}

/* change port baud rate */
int
ChangeSerialPortBaudRate (FT_HANDLE fdAms,  int BAUD_RATE, unsigned short portNum)
{

    printf ("Change baud rate to %d\n", BAUD_RATE);
    if (SetSerialPortBaudRate (fdAms, BAUD_RATE, portNum) == -1) {
        LogError ("Fail to change port%d barud rate %d", portNum, BAUD_RATE);
        return -1;
    } 
    return 0;
    
}

/* create share memory */
void *
CreateSharedMem (const char * sharedMemName, DWORD size)
{
    return MapSharedMem (sharedMemName, size);
}


void
StartupLogErrorProcess (void)
{
    ErrorLogPid = StartProcess (ERRLOGGER_PROGRAM);
}

/* start all process */
void
StartupAllProcess (void)
{
    /* start up AmsEngineComm process */
     AmsEngineCommPid =  StartProcess (AMS_ENGINGCOMM_PROGRAM);
   /* start console process */
    AmsGuiPid =  StartProcess (AMS_GUI_PROGRAM);
    

}

/* kill process */
void
KillProcess (DWORD pid)
{
    DWORD exitCode;
    HANDLE process = OpenProcess(0,FALSE, pid);
    if (process != NULL) {
        GetExitCodeProcess(process, &exitCode);
        TerminateProcess(process, exitCode);
        CloseHandle(process);
    }
}

#if 0
/* kill all processes */
void
KillAllProcess (StartProcess_t startProcess)
{
    /* kill process */
    KillProcess (AmsEngineCommPid);
    KillProcess (AmsGuiPid);
    KillProcess (ErrorLogPid);

    /* remove message queue */


    // kill old one, start new process
    if (startProcess == START_ANALYZER) {
        StartProcess (STARTUP_PROGRAM);
    } else if (startProcess == START_AMS) {
        StartProcess (AMS_PROGRAM);
    } else if (startProcess == START_ENGINE_TEST) {
        StartProcess (ENGING_TEST_PROGRAM);
    } else if (startProcess == START_NONE) {
        /* do not kill itself */
        return;
    }
    ExitProcess (0);
}
#else
void
KillAllProcess (void)
{
    /* kill all process */
    KillProcess (AmsEngineCommPid);
    KillProcess (AmsGuiPid);
    KillProcess (ErrorLogPid);

}
#endif

void
RemoveAllMsgQueue (void)
{
    CloseAmsQueue ();
    CloseAmsTxQueue ();
    CloseAmsEngineQueue ();
    CloseAmsGuiQueue ();
    //CloseHeateQueue ();
    CloseLogQueue ();
}


/* kill process based on process name */
void
SlayProcess (ProcessName_t processName)
{
    if (processName == DRAWER_PROCESS) {
        KillProcess (AmsGuiPid);
    } else if (processName == ENGCOMM_PROCESS) {
        KillProcess (AmsEngineCommPid);
    } else if (processName == ERRORLOG_PROCESS) {
        KillProcess (ErrorLogPid);
    } else if (processName == AMS_PROCESS) {
        exit (0);
    }
}

/* re-start process */
void
ReStartProcess (ProcessName_t processName)
{
    if (processName == DRAWER_PROCESS) {
        AmsGuiPid =   StartProcess (AMS_GUI_PROGRAM);
    } else if (processName == ENGCOMM_PROCESS) {
        AmsEngineCommPid =   StartProcess (AMS_ENGINGCOMM_PROGRAM);
    } else if (processName == AMS_PROCESS) {
        AmsPid =  StartProcess (AMS_PROGRAM);
    } else if (processName == HEATERTEST_PROCESS) {
        HeaterTestPid = StartProcess (HEATER_TEST_PROGRAM);
      
    }
}


/* exit AMS process */
void
ExitAms (StartProcess_t startProcess)
{
    RemoveAllMsgQueue ();
    DeleteAllShareMemory ();
    KillAllProcess ();

    // kill old one, start new process
    if (startProcess == START_ANALYZER) {
        StartProcess (STARTUP_PROGRAM);
    } else if (startProcess == START_AMS) {
        StartProcess (AMS_PROGRAM);
    } else if (startProcess == START_ENGINE_TEST) {
        StartProcess (ENGING_TEST_PROGRAM);
    } else if (startProcess == START_NONE) {
        /* do not kill itself */
        return;
    }
    ExitProcess (0);

}
