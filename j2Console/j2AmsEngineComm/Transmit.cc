#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <msgqueue.h>
#include <ctype.h>


#include "Global.h"
#include "Transmit.h"
#include "LogErr.h"
#include "AmsMessages.h"
#include "FtdiConfig.h"

#define MAX_SEND_RETRIES	3


typedef enum {
	TX_IDLE,
	TX_SEND_ENQ,
	TX_WAIT_FOR_ACK_OF_ENQ,
	TX_SEND_MESSAGE,
	TX_WAIT_FOR_ACK_OF_FRAME,
	TX_MESSAGE_SENT
} TxState_t;


void
Transmit::
ProcessEngComSendMsg (EngineCommReceiveMsg_t *msg)
{
   char msgType, command; 

    msgType = msg->data.engineMsg.message[0];
    command = msg->data.engineMsg.message[1]; 

    switch (msgType) {
        case 'A':
            if (command == 'M') {
                receive->InitAmsData ();
                receive->GetDateTime ();
            }
        break;

        default:
        break;
        
    }
    /* send out msg to Engine board */
    SendRs232Message (&msg->data.engineMsg);
    
}

// Timeout for response from the Engine once transmission of a frame has begun.
#define TX_TIMEOUT	2000	// 2000 msec = 2 sec

/*
 * FUNCTION
 * 	Transmit::Run
 * 
 * DESCRIPTION
 * 	Opens the input queue, and then processes received messages; either
 * 	sending them on to the Engine or to the Receive thread.
 * 
 * PARAMETERS
 * 	fdSer - Handle for the serial port.
 */

void
Transmit::
Run( FT_HANDLE fdSer )
{
    HANDLE                  queue;
	DWORD					receivedSize = 0;
	EngineCommReceiveMsg_t	msg;
    MSGQUEUEOPTIONS         options;
    DWORD                   flags;
    BOOL                    result;

	
	this->fdSer = fdSer;
	
    // Open the input queue and get its attributes.
    options.dwSize = sizeof (MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = TRUE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;
    queue = CreateMsgQueue(AMS_ENGINE_COMM_QUEUE_NAME, &options);

	
	if ( queue == NULL )
	{
        LogError( "failure to open message queue, errno: %d", GetLastError() );
		return;
	}
	

	while ( 1 )
	{
		// Wait for a message telling us we've got something to send.
        result = ReadMsgQueue(queue, &msg, sizeof(EngineCommReceiveMsg_t), &receivedSize, INFINITE, &flags);

		if (result)
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
                    case MSG_ENGINE_COMM_SEND_MESSAGE:
                        ProcessEngComSendMsg (&msg);
                    break;
#if 0
					case MSG_ENGINE_COMM_SEND_MESSAGE:
						SendRs232Message( &msg.data.engineMsg );
					break;
					case MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER:
						receive->SetMessageReceiver( msg.data.messageReceiver.name );
                        /* ZZZ for test purpose*/
                        printf ("get ams register \n");
                        msg.msgId = MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED;
                        mq_send (amsQueue, (const char *)&msg, sizeof (int) + sizeof (EngineCommReceiveMsg_t),
                                 MQ_PRIO_MAX - 2); 
				    break;
#endif
                    default:
                         LogError( "EngineComm: message receive unexpect msgId =%d",  msg.msgId);
                    break;
				}
			}
		}
		else
		{
            LogError( "EngineComm: message receive error" );
		}
	}
}

/*
 * FUNCTION
 * 	Transmit::SendRs232Message
 * 
 * DESCRIPTION
 * 	Sends a message received from an external process to the Engine.
 * 
 * PARAMETERS
 * 	message - Message to send.
 */

void
Transmit::
SendRs232Message( EngineCommMessageData_t * message )
{
	TxState_t				state = TX_IDLE;
	unsigned char			ch;
	int						retryCount = 0;

	state = TX_SEND_ENQ;

	while ( state != TX_MESSAGE_SENT )
	{
		switch ( state )
		{
			case TX_SEND_ENQ:
			{
				// Go to transmit state.  Wait for receive to complete, if necessary.
				while ( transmitReceiveState == RECEIVE_ACTIVE )
				{
                    WaitForSingleObject(hTransmitReceiveState, INFINITE);
				}
                EnterCriticalSection(&csTransmitReceiveState);
                transmitReceiveState = TRANSMIT_ACTIVE;
                LeaveCriticalSection(&csTransmitReceiveState);

				// Send the ENQ.
				ch = ENQ;
				SendMsgToSerialPort ( fdSer, &ch, 1 );
					
				state = TX_WAIT_FOR_ACK_OF_ENQ;
				break;
			}
				
			case TX_WAIT_FOR_ACK_OF_ENQ:
			{
				ch = WaitForAck();

				// If timeout, try again.
				if ( ch == 0 )
				{
					state = TX_SEND_ENQ;
				}
				
				// If ACK received, send the message.
				else if ( ch == ACK )
				{
					state = TX_SEND_MESSAGE;
				}
					
				// Could be ENQ - contention, NAK - Engine busy, or unrecognized character.
				else
				{
                    // Leave transmit state.
                    EnterCriticalSection(&csTransmitReceiveState);
                    transmitReceiveState = NEITHER_ACTIVE;
                    LeaveCriticalSection(&csTransmitReceiveState);

	
					// Wait two seconds.
					Sleep( 2000 );
						
					// Try again.
					state = TX_SEND_ENQ;
				}
				break;
			}
						
			case TX_SEND_MESSAGE:
			{
				SendMessage( message );
				state = TX_WAIT_FOR_ACK_OF_FRAME;
				break;
			}
				
			case TX_WAIT_FOR_ACK_OF_FRAME:
			{
				ch = WaitForAck();
				if ( ch == ACK )
				{
					ch = EOT;
                    SendMsgToSerialPort (fdSer, &ch, 1);
					state = TX_MESSAGE_SENT;
	
                      // Leave transmit state.
                    EnterCriticalSection(&csTransmitReceiveState);
                    transmitReceiveState = NEITHER_ACTIVE;
                    LeaveCriticalSection(&csTransmitReceiveState);

				}
				else
				{
                    if ( ch == NAK )
                    {
	                    LogError( "EngineComm: received NAK after '%c' message send", message->message[0] );
                    }
					else if ( ch == 0 ) 
					{
	                    LogError ( "EngineComm: timeout after message send" );
					}
					else 
					{
	                    LogError ("EngineComm: received %d instead of ACK/NAK after message send", ch );
					}
					retryCount++;
					if ( retryCount < MAX_SEND_RETRIES )
					{
						state = TX_SEND_ENQ;
					}
					else
					{
						state = TX_MESSAGE_SENT;
                        LogError( "EngineComm: retried send max times, message not sent" );
					}
				}
				break;
			}
				
			default:
				break;
		}
	}
}

/*
 * FUNCTION
 * 	Transmit::WaitForAck
 * 
 * DESCRIPTION
 * 	Waits for an ACK to be received from the Engine.
 * 
 * RETURNS
 * 	The character received, or 0 if the Engine did not respond within the
 * 	timeout period.
 */

unsigned char
Transmit::
WaitForAck()
{
	unsigned char	ch;
	int				ret;
	
	// Wait for a response.
    ret = WaitForSingleObject(hReceivedChar, TX_TIMEOUT);
    EnterCriticalSection(&csReceivedChar);
    ch = receivedChar;
    LeaveCriticalSection(&csReceivedChar);

	
	if ( ret == WAIT_TIMEOUT )
	{
		ch = 0;
	}
	
	return ch;
}

/*
 * FUNCTION
 * 	Transmit::SendMessage
 * 
 * DESCRIPTION
 * 	Sends the message body, as received from the external process, to the
 * 	Engine.
 * 
 * PARAMETERS
 * 	message - Message to send.
 */

void
Transmit::
SendMessage( EngineCommMessageData_t * msg )
{
	int				i;
	unsigned char	checksum;
	unsigned char	ch;
	unsigned char	frameNumber = '1';

	// Start with STX, to turn on checksumming.
	ch = STX;
    SendMsgToSerialPort(fdSer, &ch, 1);

	// Send the frame number;
	checksum = frameNumber;
    SendMsgToSerialPort(fdSer, &frameNumber, 1);

	// Send the message body.
	for ( i = 0; i < msg->length; i++ )
	{
		checksum += msg->message[i];
	}
    SendMsgToSerialPort(fdSer, msg->message, msg->length);
		
	// Send the ETX, to turn off checksumming.
	checksum += ETX;
	ch = ETX;
    SendMsgToSerialPort(fdSer, &ch, 1);

	
	// Send the checksum.
	ch = HexToAscii( checksum >> 4 );
	SendMsgToSerialPort( fdSer, &ch, 1 );
	ch = HexToAscii( checksum & 0x0F );
	SendMsgToSerialPort( fdSer, &ch, 1 );
	
	// Send the CR & LF to end the frame.
	ch = CR;
	SendMsgToSerialPort( fdSer, &ch, 1 );
	ch = LF;
	SendMsgToSerialPort( fdSer, &ch, 1 );
}
