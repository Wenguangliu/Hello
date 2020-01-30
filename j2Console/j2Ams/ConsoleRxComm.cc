/*
 * FILENAME
 *  ConsoleRxComm.c 
 *
 * MODULE DESCRIPTION
 *  Process console command 
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
#include "AmsComm.h"
#include "LogErr.h"
#include "Global.h"


/*
 * FUNCTION
 *   ProcessConsoleRxMsg 
 *
 * DESCRIPTION
 *  Process console command from GUI
 *
 * PARAMETERS
 *   char *msgPtr   --- Engine message queue pointer
 *   int  length    --- message length
 *
 * RETURNS
 *  none
 */

AmsError_t
ProcessConsoleRxMsg (char *msgPtr, int length)
{
     char            command;

      /* get command */
    command   = msgPtr [0];
#ifdef CONSOLE_RX_DEBUG
    printf ("ConsoleRxComm:  command %c\n",command);
#endif

    switch (command) {
        case 'D':
            /* send drawer control message to engine */
            SendDrawerMessage (msgPtr[1]);
        break;

        case 'C':
            /* calibration touch screen */        
            TouchScreenCal ();
        break;
        
        case 'A':
            /* process cancel command */
            SendAnalysisCommand (msgPtr[1]);
            /* set gui display */
            SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_CONCEL, 0);
        break;

        case 'H':
        /* process heater test command */
            HeaterTest ();
        break;

        case 'R':
        /* process drawer test command */
            CamDrawerTest ();
        break;

        default:
        break;
    }

    return NO_ERR;

}
