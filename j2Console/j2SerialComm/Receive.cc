/*
 * FILENAME
 * 	Receive.cc
 * 
 * MODULE DESCRIPTION
 * 	Method definitions for the Receive class.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <windows.h>
#include <commctrl.h>
#include <msgqueue.h>
#include <FtdiConfig.h>
#include <ctype.h>

#include "CommonDefines.h"
#include "SerialComm.h"
#include "Receive.h"
#include "Crc16.h"
#include "Shmem.h"
#include "AnalysisMessages.h"
#include "RecallMessages.h"
#include "LogErr.h"
#include "VersionData.h"
#include "SetSerialPortSpeed.h"
#include "EngineCommMessages.h"
#include "ParseSettings.h"

// Timeout value for receiving a character within a message.
//const int RX_TIMEOUT = 5000;	// 10 * 0.1 sec = 5 sec

Receive::
Receive()
{
	// Map the factoryData shared memory so we can get the serial number.
	factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );

	// Map the systemData shared memory so we can modify the inSearchMode member.
	systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
}

/*
 * FUNCTION
 * 	Receive::Run
 * 
 * DESCRIPTION
 * 	Receives messages from the serial port.
 * 
 * PARAMETERS
 * 	fdSer - Handle for the serial port.
 */

void
Receive::
Run( FT_HANDLE fdSer )
{
	RxState_t		state = RX_IDLE;
	int				count;
	unsigned char	ch;
    unsigned long   status;

	this->fdSer = fdSer;

    // flush rx buffer
	do
	{
        count = ReadSerPort (&ch, 1, &status);
        if (status != FT_OK) {
            LogError ("Serial Port Error Exit");
            Sleep (1000);
            exit (0);
        }
	} while (count > 0);

	while (1) {
		// Wait until we get an SOH, which starts a message.
		state = WaitForSoh ();
		// Receive the rest of the message.
		while (state != RX_IDLE) {
            count = ReadSerPort (&ch, 1, &status);
            if (status != FT_OK) {
                LogError ("Serial Port Error Exit");
                Sleep (1000);
                exit (0);
            }

	        // count == -1 means errno has the info (got a signal, etc.)
		    // count == 0 means we timed out
		   if (count < 1) {
			    LogWarning( "timeout, returning to RX_IDLE" );
				    state = RX_IDLE;
		    } else {
		        if (ch == ETX) {
			        ProcessFrame ();
				    state = RX_IDLE;
				} else {
			        AddChar( ch );
			   }
		   }
        }
	}
}

/*
 * FUNCTION
 * 	Receive::WaitForSoh
 * 
 * DESCRIPTION
 * 	Waits for an SOH message from the external computer.  An SOH signals the
 *	start of a message.  There is no timeout while waiting for an SOH.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in.
 */

RxState_t
Receive::
WaitForSoh()
{
	unsigned char	ch;
	int				count;
	RxState_t		state;
    unsigned long   status;

	// Wait for a character.
	count =  ReadSerPort (&ch, 1, &status);

    if (status != FT_OK) {
        LogError ("Serial Port Error Exit");
        Sleep (1000);
        exit (0);
     }

	if (count < 1) {
		    return RX_IDLE;
     }
	if (ch != SOH) {
	   return RX_IDLE;
    }
    state = RX_DATA;
    InitFrame();
	return state;
}

/*
 * FUNCTION
 * 	InitFrame
 * 
 * DESCRIPTION
 * 	Initializes the frame buffer and associated metadata.
 */

void
Receive::
InitFrame (void)
{
	crc = 0;
		
	// Clear the frame, set the input pointer to the beginning, and clear
	// the input count.
	memset( rxFrame, SR_RX_FRAME_SIZE,0 );
	rxFramePtr = rxFrame;
	rxCount = 0;
}

/*
 * FUNCTION
 * 	AddChar
 * 
 * DESCRIPTION
 * 	Adds a character to the received frame buffer.
 * 
 * PARAMETERS
 * 	ch - Character to add to the buffer.
 */

void
Receive::
AddChar( char ch )
{
	if ( rxCount < SR_RX_FRAME_SIZE )
	{
		*rxFramePtr = ch;
		rxFramePtr++;
		rxCount++;
	}
	
	else
	{
		LogError( "got too many characters for frame (max = %d)", SR_RX_FRAME_SIZE );
	}
}

/*
 * FUNCTION
 * 	ProcessFrame
 * 
 * DESCRIPTION
 * 	Parses a received frame and acts accordingly.
 */

void
Receive::
ProcessFrame()
{
	const char *			serNumString = "C\0020008000300002147";	// 'C' + STX + Size[4] + Type[4] + Offset[4] + CRC[4]
	const char *			verString = "v\002F661";				// 'v' + STX + CRC[4]
	static bool				sentVersion = false;
	static bool				gotAck = false;
	static RxFrameState_t	state = RX_NONE;

	switch ( rxFrame[0] )
	{
		case 'C':	// Request serial number - regular or DLD+.
			if ( _strnicmp( rxFrame, serNumString, strlen( serNumString ) ) == 0 )
			{
				SendAck();
				
				// Delay so the PC can separate the messages.
				Sleep( 1000/*1*/ );
				
				SendSerNum();
				state = RX_SERNUM;
				gotAck = false;
			}

			else
			{
				SendNak();
			}
			break;

		case 'v':	// Request software version number.
			if ( _strnicmp( rxFrame, verString, strlen( verString ) ) == 0 )
			{
				SendAck();
				
				Sleep( 1000/*1*/ );
				
				SendVersion();
				state = RX_VERSION;
				sentVersion = true;
				gotAck = false;
			}

			else
			{
				SendNak();
			}
			break;

		case ACK:	// ACK
			if ( state != RX_NONE )
			{
				gotAck = true;
			}
			break;

		case NAK:	// NAK - we need to resend.
			switch ( state )
			{
				case RX_SERNUM:		SendSerNum();
				case RX_VERSION:	SendVersion();
				default:			break;
			}
			break;
			
		case 't':	// End of transmission.
			if ( gotAck )
			{
				SendAck();

				// If we haven't previously sent the version, then this means DLD+ is connected, so go into Research mode.
				if ( !sentVersion && (state == RX_SERNUM) )
				{
					systemData->inResearchMode = true;
#ifndef FAN_IDLE_TEST
                    // send engine fan contrll to be idle high speed
                    SendEngineFanHighSpeed ();
#endif
				}

				// Reinit for the next time.
				state = RX_NONE;
				gotAck = false;
			}
			break;
		
		default:
			break;
	}
}

void
Receive::
SendAck()
{
							   // SOH ACK STX ETX
	char *	ackString = "\001\006\002\003";
	
	WriteSerPort (ackString, strlen( ackString));
}

void
Receive::
SendNak()
{
							   // SOH ??? STX ETX
	char *	nakString = "\001\017\002\003";
	
	WriteSerPort (nakString, strlen( nakString) );
}

void
Receive::
SendSerNum()
{
	char			msgBuff[100];
	char			charBuff[3];
	char			messageLen[3];
	char			crcString[5];
	char			encodedSn[ SERIAL_NUMBER_LEN * 2 ];
	char *			factorySn = factoryData->instrumentSerialNumber;
	char			formattedSn[ 11 ];
	const char		pad[] = "0000000000";
	unsigned short	crc = 0;
	unsigned		i;

	// Create the serial number string.
	if ( strlen( factorySn ) < 10 )
	{
		sprintf( formattedSn, "%1.*s%s", (int)(10 - strlen( factorySn )), pad, factorySn );
	}
	else
	{
		strncpy( formattedSn, factorySn, 10 );
		formattedSn[10] = '\0';
	}

	memset( encodedSn, 0, SERIAL_NUMBER_LEN * 2 );
	for ( i = 0; i < strlen( formattedSn ) && i < SERIAL_NUMBER_LEN - 1; i++ )
	{
		sprintf( charBuff, "%X%X", formattedSn[i] / 16, formattedSn[i] % 16 );
		strcat( encodedSn, charBuff );
	}

	// Create the message length string.
	sprintf( messageLen, "%X%X", ((8 + strlen( encodedSn)) / 16) & 0xF,
	                             (8 + strlen( encodedSn)) & 0xF );
	                             
	// Create the message, sans CRC and ETX.
					 // SOH + 'c' + STX + data length (00XX) + type ("0003") + offset ("0000" ) + encoded serial number
	sprintf( msgBuff, "\001c\00200%s00030000%s", messageLen, encodedSn );
	
	// Calculate the CRC.
	for ( i = 0; i < strlen( msgBuff ); i++ )
	{
		CalcCrc16( msgBuff[i], &crc );
	}

	// Create the CRC string, low byte first.
	sprintf( crcString, "%X%X%X%X", (crc / 16) & 0xF,
									crc & 0xF,
									(crc / 4096) & 0xF,
									(crc / 256) & 0xF );

	// Add the CRC and ETX to the end of the message.
	strcat( msgBuff, crcString );
	strcat( msgBuff, "\003" );	// ETX
	
	// Send the message.
	WriteSerPort (msgBuff, strlen( msgBuff) );
}

void
Receive::
SendEngineCommMessage (EngineCommReceiveMsg_t * msg, int msgDataLen)
{
    static HANDLE       engineCommQueue = NULL;
    MSGQUEUEOPTIONS     options;

    if (engineCommQueue == NULL) {
        options.dwSize = sizeof(MSGQUEUEOPTIONS);
        options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess = FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage = MAX_QUEUE_LENGTH;
        engineCommQueue = CreateMsgQueue (ENGINE_COMM_QUEUE_NAME, &options);
        if (engineCommQueue == NULL) {
            LogError( "failure to open %s queue.", ENGINE_COMM_QUEUE_NAME);
            return;
        }
    }
    WriteMsgQueue(engineCommQueue, msg, sizeof(EngineCommReceiveMsg_t), INFINITE, NULL);
}

void
Receive::
SendEngineFanHighSpeed()
{
    EngineCommReceiveMsg_t  msg;

    LogDebug ("Send Fan cmd=%c dac =%d", 'F', IDLE_FAN_HIGH_SPEED);
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0]  = 'H';
    msg.data.engineMsg.message[1]  = 'F';    // cmd = F, set fan idle speed
    msg.data.engineMsg.message[2]  = ESC;
    msg.data.engineMsg.message[3]  = 10;     // LSB of binary count.
    msg.data.engineMsg.message[4]  = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5]  = 0;      // Top DAC, two bytes for 10 bit  DAC
    msg.data.engineMsg.message[6]  = 0;
    msg.data.engineMsg.message[7]  = 0;      // Bootom DAC, two bytes for 10 bit DAC
    msg.data.engineMsg.message[8]  = 0;
    msg.data.engineMsg.message[9]  = IDLE_FAN_HIGH_SPEED;    // Fan idle speed, low = 0, high = 1
    msg.data.engineMsg.message[10] = 0;      // Fan speed set
    msg.data.engineMsg.message[11] = 0;      // air temperature offset 4 bytes
    msg.data.engineMsg.message[12] = 0;
    msg.data.engineMsg.message[13] = 0;
    msg.data.engineMsg.message[14] = 0;
    msg.data.engineMsg.length = 15;
    SendEngineCommMessage ( &msg, sizeof( EngineCommReceiveMsg_t ) );
}

void
Receive::
SendVersion()
{
	char					msgBuff[100];
	char					crcString[5];
	unsigned short			crc = 0;
	unsigned				i;
	static VersionData_t *	versionData = (VersionData_t *)MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );
	
	// Create the message, sans CRC and ETX.
					 // SOH + 'V' + STX + data length ("000E") + version (14 characters, left-justified)
	sprintf( msgBuff, "\001V\002000E%-14.14s", versionData->version );
	
	// Calculate the CRC.
	for ( i = 0; i < strlen( msgBuff ); i++ )
	{
		CalcCrc16( msgBuff[i], &crc );
	}

	// Create the CRC string, low byte first.
	sprintf( crcString, "%X%X%X%X", (crc / 16) & 0xF,
									crc & 0xF,
									(crc / 4096) & 0xF,
									(crc / 256) & 0xF );

	// Add the CRC and ETX to the end of the message.
	strcat( msgBuff, crcString );
	strcat( msgBuff, "\003" );	// ETX
	
	// Send the message.
	WriteSerPort (msgBuff, strlen( msgBuff) );
}
