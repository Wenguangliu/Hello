/*
 * FILENAME
 * 	HeaterControlMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the HeaterControl process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __HEATERCONTROLMESSAGES_H_INCLUDED
#define __HEATERCONTROLMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define HEATER_CONTROL_QUEUE_NAME	"/HeaterControl"

/*
 * Message IDs for messages received by the HeaterControl process.
 */
typedef enum
{
	MSG_HEATER_CONTROL_TIMER = MSG_FIRST_HEATER_CONTROL_ID,
	MSG_HEATER_CONTROL_TEMPERATURES,
	MSG_HEATER_CONTROL_START_ANALYSIS,
	MSG_HEATER_CONTROL_MIX_DONE,
	MSG_HEATER_CONTROL_ANALYSIS_DONE,
	
//	MSG_HEATER_CONTROL_OUT_TEMPERATURE,
//	MSG_HEATER_CONTROL_OUT_ERROR,
//	MSG_HEATER_CONTROL_OUT_WARMUP_DONE,
//	MSG_HEATER_CONTROL_OUT_OVERTEMP,
} HeaterControlMsgId_t;

typedef struct
{
	HeaterControlMsgId_t	msgId;
	char					data[4096 - sizeof( HeaterControlMsgId_t )];
} HeaterControlMsg_t;

#endif	/* __HEATERCONTROLMESSAGES_H_INCLUDED */

