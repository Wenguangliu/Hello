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
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <mqueue.h>

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

// Timeout value for receiving a character within a message.
const unsigned	RX_CHAR_TIMEOUT = 50;	// 10 * 0.1 sec = 5 sec

// Timeout value for receiving a file packet.
const unsigned	RX_FILE_TIMEOUT = 100;	// 100 * 0.1 sec = 10 sec

// File packet size. Does not include the checksum.
const unsigned	RX_FILE_PACKET_SIZE = 1000;

// File checksum size.
const unsigned	RX_FILE_CHECKSUM_SIZE = 4;

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
Run( int fdSer )
{
	RxState_t		state = RX_IDLE;
	int				count;
	unsigned char	ch;

	this->fdSer = fdSer;

	// Flush the port, so we ignore anything that may have been in the buffer.
	do
	{
		count = readcond( fdSer, &ch, 1, 1, 0, 1 );
	} while ( count > 0 );

	while ( 1 )
	{
		// Wait until we get an SOH, which starts a message.
		state = WaitForSoh();

		// Receive the rest of the message.
		while ( state != RX_IDLE )
		{
			count = readcond( fdSer, &ch, 1, 1, 0, RX_CHAR_TIMEOUT );

			// count == -1 means errno has the info (got a signal, etc.)
			// count == 0 means we timed out
			if ( count < 1 )
			{
				LogWarning( "timeout, returning to RX_IDLE" );
				state = RX_IDLE;
			}
			else
			{
				if ( ch == ETX )
				{
					ProcessFrame();
					state = RX_IDLE;
				}
				else
				{
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

	// Wait for a character.
	count = read( fdSer, &ch, 1 );
	if ( count < 1 )
		return RX_IDLE;

	if ( ch != SOH )
	{
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
InitFrame( void )
{
	crc = 0;
		
	// Clear the frame, set the input pointer to the beginning, and clear
	// the input count.
	memset( rxFrame, RX_FRAME_SIZE,0 );
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
	if ( rxCount < RX_FRAME_SIZE )
	{
		*rxFramePtr = ch;
		rxFramePtr++;
		rxCount++;
	}
	
	else
	{
		LogError( "got too many characters for frame (max = %d)", RX_FRAME_SIZE );
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
	char					buff[30];
	char					fileName[ UPLOADED_ROTOR_NAME_LEN + 1 ];
	unsigned				fileSize;
	unsigned				checksum;

	switch ( rxFrame[0] )
	{
		case 'C':	// Request serial number - regular or DLD+.
			if ( strnicmp( rxFrame, serNumString, strlen( serNumString ) ) == 0 )
			{
				SendAck();
				
				// Delay so the PC can separate the messages.
				sleep( 1 );
				
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
			if ( strnicmp( rxFrame, verString, strlen( verString ) ) == 0 )
			{
				SendAck();
				
				sleep( 1 );
				
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

		case 'O':		// ROC file is next. Frame is 'O' + STX + Size[5] + Label[20].
			if ( systemData->inResearchMode )
			{
				strncpy( buff, rxFrame + 2, 5 );
				buff[5] = '\0';
				fileSize = strtoul( buff, 0, 16 );
				strncpy( fileName, rxFrame + 7, UPLOADED_ROTOR_NAME_LEN );
				fileName[20] = '\0';
				LogDebug( "Got 'O' packet, fileSize = %d, name = %s", fileSize, fileName );
	
				SendAck();
	
				// Receive the file and save it to disk (/tmp directory). If successful, let Analysis know.
				if ( ReceiveFile( fileSize, UPLOADED_ROTOR_ROC_NAME ) == 0 )
				{
					SendAnalysisMessage( fileName );
				}
			}
			else
			{
				SendNak();
			}
			break;
		
		case 'A':		// Analyte ROC file is next. Frame is 'A' + STX + Size[5] + Name[20] + Checksum[4].
			break;
		
		case 'I':		// RIS file is next. Frame is 'I' + STX + Size[5].
			if ( systemData->inResearchMode )
			{
				strncpy( buff, rxFrame + 2, 5 );
				buff[5] = '\0';
				fileSize = strtoul( buff, 0, 16 );
				strncpy( fileName, rxFrame + 7, UPLOADED_ROTOR_NAME_LEN );
				fileName[20] = '\0';
				LogDebug( "Got 'I' packet, fileSize = %d, fileName = %s", fileSize, fileName );
				SendAck();
				ReceiveFile( fileSize, UPLOADED_ROTOR_RIS_NAME );
			}
			else
			{
				SendNak();
			}
			break;

		case 'B':		// Barcode data. Frame is 'B' + STX + checksum.
			if ( systemData->inResearchMode )
			{
				strncpy( buff, rxFrame + 2, 4 );
				buff[4] = '\0';
				checksum = strtoul( buff, 0, 16 );
				LogDebug( "Got 'B' packet, checksum = %x", checksum );
				SendAck();
				ReceiveBarcodeData( checksum );
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
	const char *	ackString = "\001\006\002\003";
	
	write( fdSer, ackString, strlen( ackString) );
}

void
Receive::
SendNak()
{
							   // SOH NAK STX ETX
	const char *	nakString = "\001\025\002\003";
	
	write( fdSer, nakString, strlen( nakString) );
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
	write( fdSer, msgBuff, strlen( msgBuff) );
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
	write( fdSer, msgBuff, strlen( msgBuff) );
}

int
Receive::
ReceiveFile( unsigned fileSize, char * fileName )
{
	unsigned char	buff[ RX_FILE_PACKET_SIZE + RX_FILE_CHECKSUM_SIZE ];	// Data packet plus checksum.
	unsigned		i;
	unsigned		receiveCount;
	unsigned		expectCount;
	unsigned		calcChecksum = 0;
	unsigned		sentChecksum;
	FILE *			fp;
	
	// Open the file. Overwrite if it already exists.
	fp = fopen( fileName, "w" );
	if ( fp == 0 )
	{
		LogError( "Error opening file %s", fileName );
		SendNak();
		return -2;
	}	

	// Receive the file.
	while ( fileSize > 0 )
	{
		// Determine the expected data packet size plus checksum.
		expectCount = fileSize > RX_FILE_PACKET_SIZE ? RX_FILE_PACKET_SIZE : fileSize;
		expectCount += RX_FILE_CHECKSUM_SIZE;

		// Get the data frame.
		receiveCount = readcond( fdSer, buff, expectCount, expectCount, RX_CHAR_TIMEOUT, RX_FILE_TIMEOUT );

		// receiveCount < expectCount indicates a timeout error. Send a NAK and remove the file.
		if ( receiveCount < expectCount )
		{
			LogError( "Timeout receiving file %s, %u bytes received", fileName, receiveCount );
			SendNak();
			fclose( fp );
			unlink( fileName );
			return -1;
		}
		else
		{
			// Get the checksum from the end of the frame, and null-terminate.
			memcpy( &sentChecksum, buff + receiveCount - RX_FILE_CHECKSUM_SIZE, sizeof( unsigned ) );
			
			// Calculate the checksum for the data sent.
			for ( i = 0, calcChecksum = 0; i < receiveCount - 4; i++ )
			{
				calcChecksum += buff[i];
			}

			// Checksums don't match, so send a NAK. In this case, we wait for a resend.
			if ( calcChecksum != sentChecksum )
			{
				LogError( "Checksum mismatch receiving file %s. Calculated %x, received %x", fileName, calcChecksum, sentChecksum );
				SendNak();
			}
			
			// Checksums match, so ACK and write the data to the file.
			else
			{
				SendAck();
				fwrite( buff, receiveCount - RX_FILE_CHECKSUM_SIZE, 1, fp );
				
				// Decrement the count of characters still to receive.
				fileSize -= (receiveCount - RX_FILE_CHECKSUM_SIZE);
			}
		}
	}
	
	fclose( fp );

	LogDebug( "File %s successfully received", fileName );
	return 0;
}

int
Receive::
ReceiveBarcodeData( unsigned checksum )
{
	unsigned		i;
	int				count;
	unsigned		calcChecksum = 0;
	FILE *			fp;
	unsigned char *	p;
	BarcodeData_t	barcodeData;

	p = (unsigned char *)&barcodeData;

	// We wait an extended period for the first character, in case it takes a while for the PC to set up.
	count = readcond( fdSer, p, 1, 1, 0, RX_FILE_TIMEOUT );
	if ( count < 1 )
	{
		LogError( "Timeout receiving start of barcode data" );
		SendNak();
		return -1;
	}
	else
	{
		calcChecksum += *p;
	}

	// Now, get the rest of the data.
	for ( i = 1, ++p; i < sizeof( BarcodeData_t ); i++, p++ )
	{
		count = readcond( fdSer, p, 1, 1, 0, RX_CHAR_TIMEOUT );
		if ( count < 1 )
		{
			LogError( "Timeout receiving barcode data, received count = %x", i );
			SendNak();
			return -3;
		}
		else
		{
			calcChecksum += *p;
		}
	}

	if ( calcChecksum != checksum )
	{
		LogError( "Checksum mismatch receiving barcode data. Calculated %x", calcChecksum );
		SendNak();
		return -4;
	}

	fp = fopen( UPLOADED_ROTOR_BARCODE_NAME, "w" );
	if ( fp == 0 )
	{
		LogError( "Error opening barcode data file %s", UPLOADED_ROTOR_BARCODE_NAME );
		SendNak();
		return -2;
	}	

	fwrite( &barcodeData, 1, sizeof( BarcodeData_t ), fp );
	fclose( fp );

	LogDebug( "Barcode data successfully received" );


// GLD
// temp
	LogDebug( "rotorId = %x", barcodeData.rotorId );
	LogDebug( "rotorAgeInDays = %f", barcodeData.rotorAgeInDays );
	LogDebug( "rotorMfgLot.lsdOfYear = %x", barcodeData.rotorMfgLot.lsdOfYear );
	LogDebug( "rotorMfgLot.weekOfYear = %x", barcodeData.rotorMfgLot.weekOfYear );
	LogDebug( "rotorMfgLot.dayOfWeek = %x", barcodeData.rotorMfgLot.dayOfWeek );
	LogDebug( "rotorMfgLot.code[0] = %x", barcodeData.rotorMfgLot.code[0] );
	LogDebug( "rotorMfgLot.code[1] = %x", barcodeData.rotorMfgLot.code[1] );
	LogDebug( "rotorMfgLot.code[2] = %x", barcodeData.rotorMfgLot.code[2] );
	LogDebug( "rotorMfgLot.code[3] = %x", barcodeData.rotorMfgLot.code[3] );
	LogDebug( "rotorAnalyteFactor[0] = %f", barcodeData.rotorAnalyteFactor[0] );
	LogDebug( "rotorAnalyteFactor[43] = %f", barcodeData.rotorAnalyteFactor[43] );

	SendAck();
	return 0;
}

void
Receive::
SendAnalysisMessage( char * fileName )
{
	static mqd_t	queue = 0;
	AnalysisMsg_t	msg;

	if ( queue == 0 )
	{
		queue = mq_open( ANALYSIS_QUEUE_NAME, O_WRONLY );
	
		if ( queue == -1 )
		{
			LogError( "failure to open Analysis queue" );
			return;
		}
	}
	
	msg.msgId = MSG_ANALYSIS_UPLOADED_ROTOR;
	strcpy( msg.rotorName, fileName );
	mq_send( queue, (const char *)&msg, sizeof( AnalysisMsg_t ), MQ_PRIO_MAX-1 );
}
