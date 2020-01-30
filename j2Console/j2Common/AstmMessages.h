/*
 * FILENAME
 * 	AstmMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the ASTM process.
 * 
 * AUTHOR
 * 	Dennis Rubenacker
 */
 
#ifndef __ASTMMESSAGES_H_INCLUDED
#define __ASTMMESSAGES_H_INCLUDED

#include "CommonDefines.h"
#include "ConsoleMessages.h"

#define ASTM_QUEUE_NAME	L"/Astm"

/*
 * Message IDs for messages received by the ASTM process.
 */
typedef enum
{
	MSG_ASTM_TIMER = MSG_FIRST_ASTM_ID,
	MSG_ASTM_LAST_ROTOR_AVAILABLE,
	MSG_ASTM_SEND_RANGE,
    MSG_ASTM_CANCEL_ORDER,
} AstmMsgId_t;

typedef struct
{
    unsigned    rotorId;
    char        patientControlId[ ID_LEN + 1 ];
    char        orderDateTime[ 15 ];    // GLD - ought to be a #define
    char        cancelType;
} AstmCancelData_t;

typedef struct
{
	AstmMsgId_t			    msgId;
	union
	{
		unsigned int	    data;
		SendRangeData_t	    rangeData;
        AstmCancelData_t    cancelData;
	};
} AstmMsg_t;

#endif	/* __ASTMMESSAGES_H_INCLUDED */

