/*
 * FILENAME
 * 	ErrLoggerMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the ErrLogger process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ERRLOGGERMESSAGES_H_INCLUDED
#define __ERRLOGGERMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define ERR_LOGGER_QUEUE_NAME	"/ErrLogger"

/*
 * Message IDs for messages received by the ErrLogger process.
 */
typedef enum
{
	MSG_ERR_LOGGER_MESSAGE = MSG_FIRST_ERR_LOGGER_ID,
	MSG_ERR_LOGGER_RENAME_LOG_FILES,
} ErrLoggerMsgId_t;

typedef struct
{
	ErrLoggerMsgId_t	msgId;
	char				message[200];
} ErrLoggerMsg_t;

#endif	/* __ERRLOGGERMESSAGES_H_INCLUDED */

