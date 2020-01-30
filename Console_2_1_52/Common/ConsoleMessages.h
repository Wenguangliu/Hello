/*
 * FILENAME
 * 	ConsoleMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for interprocess messages on the Console.  This
 * 	is the top-level header for interprocess messaging.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __MESSAGES_H_INCLUDED
#define __MESSAGES_H_INCLUDED

#include <time.h>

/*
 * This is a generic message type.  It may be used to receive any of the message
 * types, and then be cast to the appropriate type upon examination of the
 * message ID.  Note that the data is set to 4096 less the size of the message
 * ID, so that the total message size will be 4096 bytes, the standard for a
 * POSIX message.
 */
typedef struct{
	int msgId;
	char data[4096 - sizeof( int )];
} ConsoleMsg_t;

// Date range, used by both ASTM and SerialComm.
typedef struct
{
	time_t	startDate;
	time_t	endDate;
} SendRangeData_t;


/*
 * These defines are used to set the starting value for the message ID for the
 * messages defined by the various processes.  They are set so that no two
 * messages will use the same message ID.
 */
#define	MSG_FIRST_GUI_ID			0
#define MSG_FIRST_ANALYSIS_ID		1000
#define MSG_FIRST_RECALL_ID			2000
#define MSG_FIRST_SETTINGS_ID		3000
#define MSG_FIRST_ENGINE_COMM_ID	4000
#define MSG_FIRST_CALC_RESULTS_ID	5000
#define MSG_FIRST_AMS_ID			6000
#define MSG_FIRST_HEATER_CONTROL_ID	7000
#define MSG_FIRST_STARTUP_ID		8000
#define MSG_FIRST_SERIAL_COMM_ID	9000
#define MSG_FIRST_ASTM_ID			10000
#define MSG_FIRST_ERR_LOGGER_ID		11000
#define MSG_FIRST_MGUI_ID			12000
#define MSG_FIRST_MFG_ID			13000
#define MSG_FIRST_BEAK_TEST_ID      14000
#endif	/* __MESSAGES_H_INCLUDED */

