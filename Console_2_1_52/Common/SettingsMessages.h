/*
 * FILENAME
 * 	SettingsMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the Settings process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SETTINGSMESSAGES_H_INCLUDED
#define __SETTINGSMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define SETTINGS_QUEUE_NAME	"/Settings"

/*
 * Message IDs for messages received by the Settings process.
 */
typedef enum
{
	MSG_SETTINGS_GOTO_FOREGROUND = MSG_FIRST_SETTINGS_ID,
	MSG_SETTINGS_TIMER,
	MSG_SETTINGS_CDUPDATE_ERROR,
	MSG_SETTINGS_CDUPDATE_DONE,
	MSG_SETTINGS_CDUPDATE_WARNING,
	MSG_SETTINGS_CDUPDATE_OK,
	MSG_SETTINGS_TRIGGER,
} SettingsMsgId_t;

typedef struct
{
	SettingsMsgId_t msgId;
	int				data;
} SettingsMsg_t;

#endif	/* __SETTINGSMESSAGES_H_INCLUDED */

