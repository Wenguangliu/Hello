/*
 * FILENAME
 * 	StartupMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the Startup process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __STARTUPMESSAGES_H_INCLUDED
#define __STARTUPMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define STARTUP_QUEUE_NAME	    L"/Startup"
#define WATCHDOG_QUEUE_NAME     L"/WatchDog"


/*
 * Message IDs for messages received by the CalcResults process.
 */
typedef enum
{
	MSG_STARTUP_GOTO_AMS = MSG_FIRST_STARTUP_ID,
	MSG_STARTUP_GUI_READY,
	MSG_STARTUP_ERROR,
    MSG_STARTUP_WATCH_DOG,
    MSG_STARTUP_HOME_READY,
    MSG_STARTUP_GOTO_FOREGROUND,
    MSG_STARTUP_RESTART,
} StartupMsgId_t;

typedef struct
{
	StartupMsgId_t	msgId;
} StartupMsg_t;

#endif	// __STARTUPMESSAGES_H_INCLUDED

