/*
 * FILENAME
 * 	EngineCommMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the EngineComm
 *  process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ENGINECOMMMESSAGES_H_INCLUDED
#define __ENGINECOMMMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define ENGINE_COMM_QUEUE_NAME	"/EngineComm"

// GLD
// 'B' message from Engine has 1200 16-bit values, plus misc.
#define RX_FRAME_SIZE			4000				// RS232 receive buffer size.
#define ENGINE_COMM_MAX_MSG		(RX_FRAME_SIZE - 7)	// Overhead bytes.

/*
 * Message IDs for messages received by the EngineComm process, plus messages
 * sent by the EngineComm process to destinations that aren't known until run-time.
 */
typedef enum
{
	MSG_ENGINE_COMM_INIT_ANALYSIS = MSG_FIRST_ENGINE_COMM_ID,
	MSG_ENGINE_COMM_SEND_MESSAGE,
	MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER,
	
	MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED,
	
	MSG_ENGINE_COMM_LAST,
} EngineCommMsgId_t;

#define IS_ENGINE_COMM_MSG( msgId ) ( msgId >= MSG_FIRST_ENGINE_COMM_ID && msgId <= MSG_ENGINE_COMM_LAST )

/*
 * The following data structures are for the data fields of EngineComm messages
 */
 
/*
 * Data field for the MSG_ENGINE_COMM_INIT_ANALYSIS message.
 */
 typedef struct
{
} EngineCommInitAnalysisData_t;

/*
 * Data field for the MSG_ENGINE_COMM_SEND_MESSAGE and
 * MSG_ENGINE_COMM_MESSAGE_RECEIVED messages.
 */
typedef struct
{
	unsigned char	message[ ENGINE_COMM_MAX_MSG ];
	int				length;
} EngineCommMessageData_t;

/*
 * Data field for the MSG_ENGINE_COMM_SET_INPUT_RECEIVER message.
 */
 typedef struct
{
	char name[100];
} EngineCommMessageReceiverData_t;

/*
 * Union of all possible data fields for messages received by EngineComm.
 */
typedef union
{
	EngineCommInitAnalysisData_t	initAnalysis;
	EngineCommMessageData_t			engineMsg;
	EngineCommMessageReceiverData_t	messageReceiver;
} EngineCommReceiveMsgData_t;

/*
 * Data structure for all messages received by EngineComm.
 */
typedef struct
{
	int							msgId;
	EngineCommReceiveMsgData_t	data;
} EngineCommReceiveMsg_t;

typedef struct
{
	int						msgId;
	EngineCommMessageData_t	data;
    void                    *amsData;
} EngineCommSendMsg_t;

#endif	/* __ENGINECOMMMESSAGES_H_INCLUDED */

