/*
 * FILENAME
 * 	RecallMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the Recall process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __RECALLMESSAGES_H_INCLUDED
#define __RECALLMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define RECALL_QUEUE_NAME	"/Recall"

/*
 * Message IDs for messages received by the Recall process.
 */
typedef enum
{
	MSG_RECALL_GOTO_FOREGROUND = MSG_FIRST_RECALL_ID,
	MSG_RECALL_TRIGGER,
	MSG_RECALL_SEND_RESULTS_COMPLETE,
	MSG_RECALL_SEND_PROGRESS,
	MSG_RECALL_ARCHIVE_PROGRESS,
	MSG_RECALL_ARCHIVE_COMPLETE,
	MSG_RECALL_ARCHIVE_INSUFF_SPACE,
	MSG_RECALL_ARCHIVE_DISCONNECT,
	MSG_RECALL_TIMER,
} RecallMsgId_t;

typedef struct
{
	RecallMsgId_t	msgId;
	int				idata;
} RecallMsg_t;

#endif	/* __RECALLMESSAGES_H_INCLUDED */

