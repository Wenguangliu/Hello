// ********************************************************************
//
// FILENAME:  
// 
// 		ErrorReport.cc	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains functions to send system error message
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>

#include "CommonDefines.h"
#include "ErrorProcessMessages.h"
#include "SystemErrorNums.h"
#include "ErrorReport.h"
#include "LogErr.h"

// send message queue to ErrorProcess
void
SendErrorProcessMessage (ErrorProcessMsg_t *msg, int msgDataLen)
{
    static HANDLE           queue = 0;
    MSGQUEUEOPTIONS         options;

    if (queue == NULL) {
        options.dwSize =        sizeof (MSGQUEUEOPTIONS);
        options.dwFlags =       MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess =   FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage =  MAX_QUEUE_LENGTH;
        queue = CreateMsgQueue  (ERROR_PROCESS_QUEUE_NAME, &options);
    }
    if (queue != NULL) {
        WriteMsgQueue (queue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
    } else {
       LogDebug ("Fail to create Error Process  Queue");
    }
}

// generate error report 
void
SendErrorReport (unsigned int errorNumber)
{
    ErrorProcessMsg_t   msg;

    msg.msgId   = MSG_ERROR_REPORT;
    strncpy (msg.data.processName, HL7_PROGRAM,sizeof(msg.data.processName)-1);
    msg.data.errorNumber = errorNumber;
    msg.data.retState    = 0;
    SendErrorProcessMessage (&msg, sizeof (ErrorProcessMsgData_t));
}

