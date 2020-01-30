#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>

#include "AnalysisMessages.h"
#include "GuiMessages.h"
#include "RecallMessages.h"
#include "SettingsMessages.h"
#include "Shmem.h"
#include "CreateDisplay.h"
#include "LogErr.h"
#include "StateErrorProcess.h"
#include "ViewLog.h"

static State *  states [STATE_COUNT];
static state_t  currentState;
void   Receiver (void);

void
InitialState (void)
{
    states [STATE_BACKGROUND]       = new StateBackground ();
    states [STATE_ACTION_WARNING]   = new StateActionWarning ();
    states [STATE_ACTION_ABORT]     = new StateActionAbort ();
    states [STATE_ACTION_SHUTDOWN]  = new StateActionShutdown ();
    states [STATE_INFO]             = new StateInfo ();
    states [STATE_VIEW_LOG]         = new StateViewLog ();
}

int
main (int argc, char ** argv)
{
    InitialState ();

    currentState = STATE_BACKGROUND;

    states [currentState]->Enter ();

    Receiver();                         // Process messages and state changes.

    return -1;


    return 0;
}

// process received message 
void
Receiver (void)
{
    DWORD           receivedSize = 0;
    ConsoleMsg_t    msg;
    MSGQUEUEOPTIONS options;
    DWORD           flags;
    state_t         newState;
    DWORD           lastError;
    BOOL            result;
    static HANDLE   queue = 0;


    /*
     * Open the input queue and get its size.
     */
    options.dwSize = sizeof (MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = TRUE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;

    queue = CreateMsgQueue (ERROR_PROCESS_QUEUE_NAME, &options);
    lastError = GetLastError ();
    if (queue == NULL) {
        LogError ("failure to open %s message queue, errno: %d", ERROR_PROCESS_QUEUE_NAME, lastError);
        exit( -1 );
    }

    while (1) {
        result = ReadMsgQueue(queue, &msg, sizeof(ConsoleMsg_t), &receivedSize, INFINITE, &flags);
        lastError = GetLastError();
        if (result != FALSE) {
            if (receivedSize > 0) {
                newState = states [currentState]->ProcessMessage (msg.msgId, msg.data);
                if (newState >= 0 && newState != currentState && newState < STATE_COUNT) {
                    states [newState]->Enter();
                    currentState = newState;
                }
            }
        } else {
            LogError ("message receive error, errno: %d", lastError);
            break;
        }
    }
}
