/*
 * FILENAME
 * 	Transmit.cc
 * 
 * MODULE DESCRIPTION
 * 	Method definitions for the Transmit class.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <mqueue.h>
#include <ctype.h>

#define MAX_SEND_RETRIES	3

#include "Global.h"
#include "Transmit.h"
#include "LogErr.h"


typedef enum {
	TX_IDLE,
	TX_SEND_ENQ,
	TX_WAIT_FOR_ACK_OF_ENQ,
	TX_SEND_MESSAGE,
	TX_WAIT_FOR_ACK_OF_FRAME,
	TX_MESSAGE_SENT
} TxState_t;


// Timeout for response from the Engine once transmission of a frame has begun.
#define TX_TIMEOUT	2000000000	// 10^9 nsec = 2 sec

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
Run( int fdSer )
{
	ssize_t					receivedSize = 0;
	EngineCommReceiveMsg_t	msg;
	struct mq_attr			mqstat;
	
	this->fdSer = fdSer;
	
	/*
	 * Open the queue and get its size.
	 */
	mqd_t queue = mq_open( ENGINE_COMM_QUEUE_NAME, O_RDONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open message queue, errno: %d", errno );
		return;
	}
	
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes, errno: %d", errno );
		return;
	}

	while ( 1 )
	{
		// Wait for a message telling us we've got something to send.
		receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );
		if ( receivedSize != -1 )
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
					case MSG_ENGINE_COMM_INIT_ANALYSIS:
					{
						receive->InitRawData();
						break;
					}
					case MSG_ENGINE_COMM_SEND_MESSAGE:
					{
						SendRs232Message( &msg.data.engineMsg );
						break;
					}
					case MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER:
					{
						receive->SetMessageReceiver( msg.data.messageReceiver.name );
						break;
					}
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
				pthread_sleepon_lock();
				while ( transmitReceiveState == RECEIVE_ACTIVE )
				{
					pthread_sleepon_wait( &transmitReceiveState );
				}
				transmitReceiveState = TRANSMIT_ACTIVE;
				pthread_sleepon_unlock();
	
				// Send the ENQ.
				ch = ENQ;
				write( fdSer, &ch, 1 );
					
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
					pthread_sleepon_lock();
					transmitReceiveState = NEITHER_ACTIVE;
					pthread_sleepon_unlock();
	
					// Wait two seconds.
					sleep( 2 );
						
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
					write( fdSer, &ch, 1 );
					state = TX_MESSAGE_SENT;
	
					// Leave transmit state.
					pthread_sleepon_lock();
					transmitReceiveState = NEITHER_ACTIVE;
					pthread_sleepon_unlock();
				}
				else
				{
					if ( ch == NAK )
					{
						LogWarning( "received NAK after '%c' message send", message->message[0] );
					}
					else if ( ch == 0 )
					{
						LogWarning( "timeout after message send" );
					}
					else
					{
						LogWarning( "received %d instead of ACK/NAK after message send", ch );
					}
					retryCount++;
					if ( retryCount < MAX_SEND_RETRIES )
					{
						state = TX_SEND_ENQ;
					}
					else
					{
						state = TX_MESSAGE_SENT;
						LogError( "retried send max times, message not sent" );
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
	pthread_sleepon_lock();
	ret = pthread_sleepon_timedwait( &receivedChar, TX_TIMEOUT );
	ch = receivedChar;
	pthread_sleepon_unlock();
	
	if ( ret == ETIMEDOUT )
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
	write( fdSer, &ch, 1 );

	// Send the frame number;
	checksum = frameNumber;
	write( fdSer, &frameNumber, 1 );

	// Send the message body.
	for ( i = 0; i < msg->length; i++ )
	{
		checksum += msg->message[i];
	}
	write( fdSer, msg->message, msg->length );
		
	// Send the ETX, to turn off checksumming.
	checksum += ETX;
	ch = ETX;
	write( fdSer, &ch, 1 );
	
	// Send the checksum.
	ch = HexToAscii( checksum >> 4 );
	write( fdSer, &ch, 1 );
	ch = HexToAscii( checksum & 0x0F );
	write( fdSer, &ch, 1 );
	
	// Send the CR & LF to end the frame.
	ch = CR;
	write( fdSer, &ch, 1 );
	ch = LF;
	write( fdSer, &ch, 1 );
}
