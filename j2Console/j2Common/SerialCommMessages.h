/*
 * FILENAME
 * 	SerialCommMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the SerialComm
 *  process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SERIALCOMMMESSAGES_H_INCLUDED
#define __SERIALCOMMMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "Species.h"

#define SERIAL_COMM_QUEUE_NAME	L"/SerialComm"

/*
 * Message IDs for messages received by the SerialComm process, plus messages
 * sent by the SerialComm process to destinations that aren't known until run-time.
 */
typedef enum
{
	MSG_SERIAL_COMM_SEND_REPORT = MSG_FIRST_SERIAL_COMM_ID,
	MSG_SERIAL_COMM_SEND_ALL,
	MSG_SERIAL_COMM_CANCEL_SEND,
	MSG_SERIAL_COMM_SEND_REF_RANGES,
	MSG_SERIAL_COMM_SEND_RANGE,
    MSG_SERIAL_COMM_CLOSE_PORT,
} SerialCommMsgId_t;

/*
 * Data structures for the messages received by SerialComm.
 */
 
typedef struct
{
	int		resultIndex;
	bool	sendResults;
	bool	sendIqc;
	bool	sendErrorReport;
	bool	abbreviatedErrorReport;
	char	radFileName[100];
} SendReportData_t;

typedef struct
{
	int							msgId;
	union
	{
//		char				radFileName[100];
		SendReportData_t	reportData;
		SendRangeData_t		rangeData;
		SampleType_t		sampleType;
	};
} SerialCommMsg_t;

#endif	/* __SERIALCOMMMESSAGES_H_INCLUDED */

