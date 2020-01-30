/*
 * FILENAME
 * 	MfgMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the Manufacturing process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __MFGMESSAGES_H_INCLUDED
#define __MFGMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define MFG_QUEUE_NAME	"/Manufacturing"

/*
 * Message IDs for messages received by the Recall process.
 */
typedef enum
{
	MSG_MFG_TRIGGER = MSG_FIRST_MFG_ID,
	MSG_MFG_TIMER,
} MfgMsgId_t;

typedef struct
{
	MfgMsgId_t	msgId;
//	int			idata;
} MfgMsg_t;

#endif	/* __MFGMESSAGES_H_INCLUDED */

