/*
 * FILENAME
 * 	Hl7Messages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the j2Hl7 process.
 * 
 * AUTHOR
 * 	Dennis Rubenacker
 */
 
#ifndef __HL7MESSAGES_H_INCLUDED
#define __HL7MESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define HL7_QUEUE_NAME	L"/Hl7"

/*
 * Message IDs for messages received by the j2HL7 process.
 */
typedef enum
{
    MSG_HL7_GOTO_FOREGROUND = MSG_FIRST_HL7_ID,
	MSG_HL7_TIMER,
	MSG_HL7_LAST_ROTOR_AVAILABLE,
	MSG_HL7_SEND_RANGE,
} Hl7MsgId_t;

typedef struct
{
	Hl7MsgId_t			msgId;
	union
	{
		unsigned int	data;
		SendRangeData_t	rangeData;
	};
} Hl7Msg_t;



#endif	/* __HL7MESSAGES_H_INCLUDED */

