/*
 * FILENAME
 *  AmsGui.c
 *
 * MODULE DESCRIPTION
 *  Process  AMS Gui command 
 *
 * AUTHOR
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>


#include "Serial.h"
#include "XmlData.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "CommonDefines.h"
#include "State.h"
#include "Global.h"


HANDLE   AmsGuiQueue = 0;

/* 
 * init ams Gui message queue
 */
bool
CreateAmsGuiQueue (void)
{
    

    /* create engine comm message queue */
    AmsGuiQueue = CreateMessageQueue (AMS_GUI_QUEUE_NAME, false);
    if (AmsGuiQueue == NULL) {
        return false;
    }
    return true;
}

void
CloseAmsGuiQueue (void)
{
    if (AmsGuiQueue != NULL) {
        CloseMsgQueue (AmsGuiQueue);
    }
}

/* 
 * send message to Gui process
 */

void
SendGuiMessage (int msgId, int amsStatus, unsigned short errorCode)
{
    AmsGuiMsg_t amsMsg;

    amsMsg.msgId     = msgId;
    amsMsg.amsStatus = (AmsStatus_t)amsStatus;
    amsMsg.errorCode = errorCode;
    
    if (AmsGuiQueue != 0) {
       WriteMsgQueue (AmsGuiQueue,  &amsMsg, sizeof (AmsGuiMsg_t) + 2, INFINITE, NULL);
     //  WriteMsgQueue (AmsGuiQueue,  &amsMsg, sizeof (AmsGuiMsg_t) + 10, INFINITE, NULL);

    }
}
