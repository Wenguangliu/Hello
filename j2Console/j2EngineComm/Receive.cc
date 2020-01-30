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

#include <ctype.h>

#include "RawData.h"
#include "CommonDefines.h"
#include "MsgConstants.h"
#include "Global.h"
#include "Receive.h"
#include "HeaterControlMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "FtdiConfig.h"
#include "StartupMessages.h"

#define  TEMP_DATA_DIRECTORY    "/Hard Disk/Abaxis/TempData"
// Timeout value for receiving a character within a message.
const int RX_TIMEOUT = 1000;	// 1 sec

// Maximum binary byte count for receive.
const unsigned MAX_BINARY_COUNT = ENGINE_COMM_MAX_MSG - 2;

FactoryData_t *Receive::factoryData = (FactoryData_t *)MapSharedMem (FACTORY_SHMEM_NAME, sizeof (FactoryData_t));
//AmsData_t     *Receive::amsData = (AmsData_t *)MapSharedMem(AMS_SHMEM_NAME, sizeof(AmsData_t));

Receive::
Receive()
{
	MSGQUEUEOPTIONS			options;

	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = FALSE;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = MAX_QUEUE_LENGTH;

	// Set the message receive queue to uninitialized.
	mqueue = NULL;

	// Open the HeaterControl queue.
	heaterControlQueue = CreateMsgQueue(HEATER_CONTROL_QUEUE_NAME, &options);
	if ( heaterControlQueue == NULL )
	{
		LogError( "failure to open HeaterControl queue" );
	}
    // Open start up queue for watchdog
    watchdogQueue = CreateMsgQueue (WATCHDOG_QUEUE_NAME, &options);
    if (watchdogQueue == NULL) 
    {
		LogError( "failure to open watchdogQueue queue" );
    }
}

/*
 * FUNCTION
 * 	Receive::Run
 * 
 * DESCRIPTION
 * 	Receives messages from the Engine.
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

	this->fdSer = fdSer;

	InitSharedMemory();
		
	while ( 1 )
	{
		// Wait until we get an ENQ, which starts a message.
		state = WaitForEnq();

		// Receive the rest of the message.
		while ( state != RX_IDLE )
		{
			count = ReceiveMsgFromSerialPort(fdSer, &ch, 1, RX_TIMEOUT);
// GLD
// count == -1 means the errno has the info (got a signal, etc.)
// count == 0 means we timed out
			if ( count < 1 )
			{
				SendNak();
				state = RX_IDLE;
				LogWarning( "timeout, returning to RX_IDLE" );
			}
			else
			{
				if ( rxCount < RX_FRAME_SIZE )
				{
					*rxFramePtr = ch;
					rxFramePtr++;
					rxCount++;
				}
				
				else
				{
					SendNak();
					state = RX_IDLE;
					LogError( "too many characters for frame (max = %d)", RX_FRAME_SIZE );
				}
			}
			
			switch ( state )
			{
				// Idle state, so break out.
				case RX_IDLE:												break;
					
				// ASCII data - includes end-of-frame and end-of-message characters.
				case RX_DATA:			state = HandleAsciiChar( ch );		break;

				// LSB of binary data count.
				case RX_BINARY_LSB:		state = HandleBinaryCountLsb( ch );	break;

				// MSB of binary data count.
				case RX_BINARY_MSB:		state = HandleBinaryCountMsb( ch );	break;
						
				// Binary data.
				case RX_BINARY_DATA:	state = HandleBinaryData( ch );		break;
			}
		}
		
		// We're done receiving the message, so clear the transmit/receive flag
		// and let the transmit thread know.
		if ( transmitReceiveState == RECEIVE_ACTIVE )
		{
			EnterCriticalSection(&csTransmitReceiveState);
			transmitReceiveState = NEITHER_ACTIVE;
			LeaveCriticalSection(&csTransmitReceiveState);
		}
		SetEvent(hTransmitReceiveState);
	}
}

/*
 * FUNCTION
 * 	Receive::InitSharedMemory
 * 
 * DESCRIPTION
 * 	Creates shared memory for storing the assay raw data.
 */

void
Receive::
InitSharedMemory()
{
	RawData_t *	rawData = 0;
	
	// Map the shared memory.
	rawData = (RawData_t *)MapSharedMem( RAW_SHMEM_NAME, sizeof( RawData_t ) );

	// Point to the separate parts of the shared memory.
	rotorRawReadings = &rawData->rotorRawReadings;
	receivedDataCounter = &rawData->receivedDataCounter;

	// Initialize the shared memory to the "empty" state.
	InitRawData();
}

/*
 * FUNCTION
 * 	Receive::SetMessageReceiver
 * 
 * DESCRIPTION
 * 	Sets the POSIX message receive queue for messages that are sent when
 * 	analysis status messages and all non-analysis messages are received from
 * 	the Engine.
 * 
 * PARAMETERS
 * 	queueName - Name of the receiver queue (obtained from the external receiver
 * 				process.
 */

void
Receive::
SetMessageReceiver( char * queueName )
{
	wchar_t			localWSTR[256];
	MSGQUEUEOPTIONS			options;

	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = FALSE;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(EngineCommSendMsg_t)*/;

	if ( mqueue != NULL )
	{
		CloseMsgQueue( mqueue );
		mqueue = NULL;
	}

	if ( queueName != NULL )
	{
		wsprintf(localWSTR, L"%S", queueName);
		mqueue = CreateMsgQueue(localWSTR, &options);
		
		if ( mqueue == NULL )
		{
			LogError( "failure to open message receiver message queue" );
			mqueue = NULL;
			return;
		}
	}
}

/*
 * FUNCTION
 * 	Receive::WaitForEnq
 * 
 * DESCRIPTION
 * 	Waits for an ENQ message from the Engine.  An ENQ signals the start of a
 *  message.  There is no timeout while waiting for an ENQ.  If the Transmit
 * 	thread sets the transmit-receive flag to transmit while this thread is
 * 	waiting for a character, the character received, regardless of what it is,
 * 	is forwarded to the Transmit thread without reaction here.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in.
 */

RxState_t
Receive::
WaitForEnq()
{
	bool			transmitActive = false;
	unsigned char	ch;
	int				count;		// GLD -- don't like this name
	RxState_t		state;

	// Wait for a character.
	count = ReceiveMsgFromSerialPort(fdSer, &ch, 1, 0);
	if ( count < 1 )
	{
		return RX_IDLE;
	}
		
	/*
	 * Get the transmit/receive state.  The transmit thread must access it
	 * too, so we need to use a mutex.  If it's clear and we got an ENQ,
	 * then we set the state to receive.
	 */
	transmitActive = 0;

	// If transmit is active, we signal the transmit thread and pass the
	// character along to it.
	if ( transmitReceiveState == TRANSMIT_ACTIVE )
	{
		transmitActive = true;
		EnterCriticalSection(&csReceivedChar);
		receivedChar = ch;
		LeaveCriticalSection(&csReceivedChar);
		SetEvent(hReceivedChar);
	}

	else if ( ch == ENQ )
	{
		EnterCriticalSection(&csTransmitReceiveState);
		transmitReceiveState = RECEIVE_ACTIVE;
		LeaveCriticalSection(&csTransmitReceiveState);
	}
		
	// If transmit is active or we got an unexpected character, just ignore it.
	if ( transmitActive || ch != ENQ )
	{
		return RX_IDLE;
	}
		
	/*
	 * We've received an ENQ, and have the transmitReceive flag locked in
	 * receive mode.
	 */

	// Send ACK
	ch = ACK;
	SendMsgToSerialPort(fdSer, &ch, 1);
		
	// Initialize the receive state and receive frame.
	state = RX_DATA;
	InitFrame();
	
	return state;
}

/*
 * FUNCTION
 * 	Receive::HandleAsciiChar
 * 
 * DESCRIPTION
 * 	Handles received ASCII characters, which include the control characters in
 * 	a message.
 * 
 * PARAMETERS
 * 	ch - Received character.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in as a result of
 * 	the received character.
 */

RxState_t
Receive::
HandleAsciiChar( char ch )
{
	RxState_t	state = RX_DATA;
	int			nakCount = 0;
	
	switch ( ch )
	{
		// End of a frame, or sequence of frames.
		case EOT:
		{
			state = RX_IDLE;
			break;
		}

		// LF signifies the end of frame data.  For this application,
		// it's probably redundant to use both LF and EOT.
		case LF:
		{
			// Verify the frame and return ACK/NAK to the sender.
			ch = VerifyFrame();
			SendMsgToSerialPort(fdSer, (unsigned char *)&ch, 1);;

			// Process the frame.
			if ( ch == ACK )
			{
				ProcessFrame();
				nakCount = 0;
			}
			
			// Report on NAK count.
			else
			{
				nakCount++;
				if ( nakCount >= 3 )
				{
					LogError( "%d consecutive NAKs sent to Engine", nakCount );
				}
			}

			// Reinit for the next frame.  This supports multiple frames received
			// with just one ENQ/ACK preamble.
			state = RX_DATA;
			InitFrame();
			break;
		}
						
		// We currently checksum the whole frame data set, so
		// we could do this implicitly without the STX and ETX
		// characters.
		case STX:	// Turn on checksumming.
		{
			calcChecksum = true;
			break;
		}
						
		case ESC:	// Switch to binary receive mode.
		{
			UpdateChecksum( ch );
			binaryCount = 0;
			state = RX_BINARY_LSB;
			break;
		}
						
		case ETX:	// Turn off checksumming.
		{
			UpdateChecksum( ch );
			calcChecksum = false;
			break;
		}
						
					
		default:	// Non-binary data.
		{
			UpdateChecksum( ch );
			break;
		}
	}
	
	return state;
}

/*
 * FUNCTION
 * 	Receive::HandleBinaryCountLsb
 * 
 * DESCRIPTION
 * 	Handles the least significant byte of the binary data count.
 * 
 * PARAMETERS
 * 	ch - Received character.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in as a result of
 * 	the received character.
 */

RxState_t
Receive::
HandleBinaryCountLsb( char ch )
{
	UpdateChecksum( ch );
	binaryCount = (unsigned char)ch;
	return RX_BINARY_MSB;
}

/*
 * FUNCTION
 * 	Receive::HandleBinaryCountMsb
 * 
 * DESCRIPTION
 * 	Handles the most significant byte of the binary data count.
 * 
 * PARAMETERS
 * 	ch - Received character.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in as a result of
 * 	the received character.
 */

RxState_t
Receive::
HandleBinaryCountMsb( char ch )
{
	RxState_t state;
	UpdateChecksum( ch );
	binaryCount += ( (unsigned)ch << 8 );
	if ( binaryCount == 0 )
	{
		state = RX_DATA;
	}

	// Exceeded the limit for binary data.
	else if ( binaryCount > MAX_BINARY_COUNT )
	{
		SendNak();
		state = RX_IDLE;
		LogError( "binary count > %d", MAX_BINARY_COUNT );
	}

	else
	{
		state = RX_BINARY_DATA;
	}

	return state;							
}

/*
 * FUNCTION
 * 	Receive::HandleBinaryData
 * 
 * DESCRIPTION
 * 	Handles received binary characters.
 * 
 * PARAMETERS
 * 	ch - Received character.
 * 
 * RETURNS
 * 	The state the Receive state machine should move to/stay in as a result of
 * 	the received character.
 */

RxState_t
Receive::
HandleBinaryData( char ch )
{
	RxState_t state = RX_BINARY_DATA;
	UpdateChecksum( ch );
	if ( --binaryCount == 0 )
	{
		state = RX_DATA;
	}

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
	// Turn off checksumming;
	calcChecksum = false;
	calculatedChecksum = 0;
		
	// Clear the frame, set the input pointer to the beginning, and clear
	// the input count.
	memset( rxFrame, RX_FRAME_SIZE,0 );
	rxFramePtr = rxFrame;
	rxCount = 0;
}

/*
 * FUNCTION
 * 	Receive::InitRawData
 * 
 * DESCRIPTION
 * 	Initializes the raw data memory before the start of an analysis run.
 */

void
Receive::
InitRawData()
{
	memset( rotorRawReadings, 0xFF, sizeof( RotorRawReadings_t ) );
	memset( receivedDataCounter, 0, sizeof( ReceivedDataCounter_t ) );
    memset( &amsTestCounter, 0, sizeof (AmsTestCounter_t));
}

/*
 * FUNCTION:
 * 		VerifyFrame
 * 
 * DESCRIPTION:
 * 		Verifies that the received frame is valid.  It does no checking to
 * 		see that the frame data matches the message type -- if it doesn't then
 * 		that is a bug, not a receive error.
 * 
 * RETURNS:
 * 		ACK if the frame is valid, else NAK
 */
  
char
Receive::
VerifyFrame( void )
{
	char retVal = ACK;
	unsigned receivedChecksum;

	// Make sure STX and ETX are in expected locations.
	if ( rxFrame[ 0 ] != STX )
	{
		LogError( "STX not where expected" );
		retVal = NAK;
	}
	if ( rxFrame[ rxCount - 5 ] != ETX )
	{
		LogError( "ETX not where expected" );
		retVal = NAK;
	}
	
	// Compare the calculated checksum with that received in the frame.
	receivedChecksum = AsciiToHex( rxFrame[ rxCount - 4 ] );
	receivedChecksum = receivedChecksum << 4;
	receivedChecksum += AsciiToHex( rxFrame[ rxCount - 3 ] );
	if ( receivedChecksum != calculatedChecksum )
	{
		LogError( "bad checksum" );
		retVal = NAK;
	}
	
	// Verify that the CR and LF are where they're expected.
	if ( rxFrame[ rxCount - 2 ] != CR || rxFrame[ rxCount - 1 ] != LF )
	{
		LogError( "CR/LF not where expected" );
		retVal = NAK;
	}
		
	return retVal;
}

/*
 * FUNCTION
 * 	ProcessFrame
 * 
 * DESCRIPTION
 * 	Takes the data from a received frame and saves it to the proper location
 * 	in the data structure.
 */

void
Receive::
ProcessFrame()
{
	bool				forwardMessage = false;
	EngineCommSendMsg_t	msg;

	// rxFrame[2] indicates frame type.
	switch ( rxFrame[2] )
	{
		case 'A':	// Analysis status.
			HandleAnalysisStatusMessage();
			forwardMessage = true;
			break;
		
		case 'R':	// Photometric data.
			HandlePhotometricData();
			break;
		
		case 'T':	// Temperature readings.
			HandleTemperatureData();
			break;
		
		case 'X':	// ADC reference offset.
			HandleAdcReferenceOffset();
			break;
		
		case 'H':	// Heater control temperature readings.
			if ( heaterControlQueue != 0 )
			{
				msg.msgId = MSG_HEATER_CONTROL_TEMPERATURES;
			
				// Data is rxFrame[2] thru rxFrame[ rxCount - 6 ]
				memcpy( msg.data.message, rxFrame + 2, rxCount - 7 );
				msg.data.length = rxCount - 7;
			
				WriteMsgQueue(heaterControlQueue, &msg, sizeof( int ) + msg.data.length, INFINITE, NULL);
			}
			break;

        case 'W':   // send message to disable watchdog  
            if (heaterControlQueue != 0) {
                msg.msgId = MSG_HEATER_CONTROL_WATCH_DOG_DISABLE;
                msg.data.length = 2;
				WriteMsgQueue(heaterControlQueue, &msg, sizeof (int) + msg.data.length, INFINITE, NULL);
                 
            }
        break;

        case 'L':
            //LogDebug ("process heater dump data");
            switch (rxFrame [3]) {
                case 'T':
                     if (HandleTemperatureDumpData () != 0) {
                        if (heaterControlQueue != 0) { // send message to enable watchdog
                            msg.msgId = MSG_HEATER_CONTROL_WATCH_DOG_ENABLE;
                            msg.data.length = 2;
                            WriteMsgQueue(heaterControlQueue, &msg, sizeof (int) + msg.data.length, INFINITE, NULL);
                        }
                    }
                break;

                case 'M':
                    HandleMotorTestMessage ();
                break;

                case 'S':
                    if (rxFrame [10] == 1) {
                        // temperature status
                    } else if (rxFrame [10] == 2) {
                        // ams motor test status
                        SendDataMessage ();
                    }
                break;

                default:
                break;
            }
    
            
        break;

		default:
			forwardMessage = true;
			break;
	}
	
	if ( forwardMessage )
	{

		if ( mqueue != 0 )
		{
			msg.msgId = MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED;
			
			// Data is rxFrame[2] thru rxFrame[ rxCount - 6 ]
			memcpy( msg.data.message, rxFrame + 2, rxCount - 7 );
			msg.data.length = rxCount - 7;
			WriteMsgQueue(mqueue, &msg, sizeof( int ) + msg.data.length, INFINITE, NULL);
		}
	}		
}

// send data message for update progress bar
void
Receive::
SendDataMessage (void)
{
    EngineCommSendMsg_t msg;

    if (mqueue != 0) {
        msg.msgId = MSG_ENGINE_COMM_OUT_MESSAGE_DATA;
        WriteMsgQueue(mqueue, &msg, sizeof (EngineCommSendMsg_t), INFINITE, NULL);
    }

}

/*
 * FUNCTION
 * 	Receive::HandlePhotometricData
 * 
 * DESCRIPTION
 * 	Handles the photometer data - stores it in the appropriate place in the
 * 	raw data memory.
 */

void
Receive::
HandlePhotometricData()
{
	RawFlashWavelengths_t * r = NULL;
	int i;
	unsigned char cuvetteNumber = rxFrame[10];
	unsigned char sampleType = rxFrame[11];
	unsigned char flashNumber = rxFrame[12];

	if ( cuvetteNumber < NUM_CUVETTES )
	{
		// Analyte data.
		if ( sampleType < 128 )
		{
			if ( flashNumber <= NUM_SAMPLE_FLASHES )
			{
				r = &rotorRawReadings->rotorAnalyteSamples[sampleType][receivedDataCounter->passCount][flashNumber];
				receivedDataCounter->analyteHasData[sampleType] = 1;
			}
			else
			{
				LogError( "flash # of %d received for analyte data", flashNumber );
			}
		}

		// Non-analyte data.
		else
		{
			switch ( sampleType )
			{
				case BEAD_CHK_1:
					if ( flashNumber <= NUM_BEAD_CHECK_FLASHES )
					{
						r = &rotorRawReadings->beadCheck1[cuvetteNumber][flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for BEAD_CHK_1", flashNumber );
					}
					break;
	
				case BEAD_CHK_2:
					if ( flashNumber <= NUM_BEAD_CHECK_FLASHES )
					{
						r = &rotorRawReadings->beadCheck2[cuvetteNumber][flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for BEAD_CHK_2", flashNumber );
					}
					break;
	
				case DISTRIBUTION_CHK:
					if ( flashNumber <= NUM_DIST_CHECK_FLASHES )
					{
						r = &rotorRawReadings->distributionCheck[cuvetteNumber][flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for DISTRIBUTION_CHK", flashNumber );
					}
					break;
	
				case PRE_SAMPLE:
					if ( flashNumber <= NUM_FRS_SAMPLE_FLASHES )
					{
						r = &rotorRawReadings->preSampleFullRotorScan[cuvetteNumber][flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for PRE_SAMPLE", flashNumber );
					}
					break;
	
				case POST_SAMPLE:
					if ( flashNumber <= NUM_FRS_SAMPLE_FLASHES )
					{
						r = &rotorRawReadings->postSampleFullRotorScan[cuvetteNumber][flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for POST_SAMPLE", flashNumber );
					}
					break;
	
				case OPTICAL_DACS:
					if ( flashNumber <= NUM_OPTICAL_DACS_FLASHES )
					{
						r = &rotorRawReadings->opticalDacs[flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for OPTICAL_DACS", flashNumber );
					}
					break;
	
				case BLACK_OFFSETS:
					if ( flashNumber <= NUM_BLACK_OFFSETS_FLASHES )
					{
						r = &rotorRawReadings->blackOffsets[flashNumber];
					}
					else
					{
						LogError( "flash # of %d received for BLACK_OFFSETS", flashNumber );
					}
					break;

                case CUV_DELAY_SAMPLE:
                    if (flashNumber < 21) {
                         r = &rotorRawReadings->cuvetteDelay [cuvetteDelayCount][flashNumber];
                         if (flashNumber == 20) {
                            if (cuvetteDelayCount < (MAX_DELAY_TIMES - 1)) {
                                cuvetteDelayCount++;
                            }
                        }
                    } else {
                        LogError( "flash # of %d received for CUV DELAY", flashNumber );
                    }
                break;

                case NDXT_SAMPLE:
                    if (flashNumber < 21) {
                        r = &rotorRawReadings->ndxtSamples [cuvetteNumber][flashNumber];
                    } else {
                         LogError( "flash # of %d received for NDXT", flashNumber );
                    }
                    // send message to update progress bar (repeat 30 times)
                    if ((flashNumber == 20) && (mqueue != 0)) {
                        SendDataMessage ();
                    }
                break;

                case NO_FLASH:
                    // no flash, ignor data
                break;
	
				default:
					LogError( "sample type %d received", sampleType );
					break;
			}
		}
	}
	
	else
	{
		LogError( "cuvette # of %d received", cuvetteNumber );
	}
				
	// Save the data to the appropriate location.
	if ( r != NULL )
	{
		memcpy( &r->engineTime, &rxFrame[6], 4 );
		r->cuvetteNumber = cuvetteNumber;
		r->sampleType = sampleType;
		r->flashNumber = flashNumber;
				
		for ( i = 0; i < 10; i++ )
		{
			memcpy( &r->rawFlashReadings[i], &rxFrame[14 + 2*i], 2 );
		}
		
		receivedDataCounter->flashCount++;	
	}

	// Error.
	else
	{
		;
	}
}

void
Receive::
InitialCount (void)
{
    cuvetteDelayCount = 0;
}

/* get system local time */
void
Receive::
GetDateTime ()
{
    SYSTEMTIME          sysTime;


    memset (dateTimeStr, 0 ,sizeof (dateTimeStr));

    GetLocalTime(&sysTime);

    sprintf (dateTimeStr, "%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d", sysTime.wYear,
                                                      sysTime.wMonth,
                                                      sysTime.wDay,
                                                      sysTime.wHour,
                                                      sysTime.wMinute,
                                                      sysTime.wSecond);
}

/*
 * save temp data to TempData directroy
 */
int
Receive::
HandleTemperatureDumpData (void)
{
    unsigned int        length, i, j;
    float               data, data1;
    char                buf [24]; 
    wchar_t             wDirectroyName [128];
    HANDLE              hSearch;
    WIN32_FIND_DATA     fileData;
    FILE                *fp;
    char                fileName [256];
    unsigned short      packNum;
    int                 ret = 0;

    /* create directory */
    wsprintf (wDirectroyName, L"%S",  TEMP_DATA_DIRECTORY);
    hSearch = FindFirstFile (wDirectroyName, &fileData);
    if (hSearch == INVALID_HANDLE_VALUE) {
        if (CreateDirectory (wDirectroyName, NULL) == 0) {
            LogError ("Fail to create %S directory");
        }
    } else {
        FindClose (hSearch);
    }
    sprintf (fileName,"%s/tempData.%s.%s.txt",TEMP_DATA_DIRECTORY,  factoryData->instrumentSerialNumber, dateTimeStr);

    /* open write file for file apending*/
    fp = fopen (fileName, "ab");
    if (fp == NULL) {
        LogError ("pen file %s error =%d", fileName, GetLastError ()); 
        return  -1;
    }

    /* get totally data length */
    memcpy (&length, &rxFrame [6], 2);

    //LogDebug ("length =%d", length);

    // get package number 
    memcpy (&packNum, &rxFrame [8], 2);
    //LogDebug ("packNum =%d", packNum);
    // last package
    if (packNum == 1) {
       LogDebug ("Start process temperature dump data");
    } else if (packNum == 255) {
        // last package, return 1 for enable watchdog
        ret = 1;
    }

    /* float = 4 bytes , 
     * two float data : rotor temperature and ambinet temperature 
     * totally 8 bytes
     */

    length = length/8;
    /* first data is saved offet = 6 */
    j = 10; 
    for (i = 0; i < length; i++) {
        /* get rotor tempreature */
        memcpy (&data, (float *)&rxFrame [j], 4); 
        j += 4;
        /* get ambinet temperature */
        memcpy (&data1, (float *)&rxFrame [j], 4); 
        j += 4;
        /* format  data to be saved */
        sprintf (buf, "%.3f %.3f\r\n", data, data1);
        /* save data  to file*/
        fwrite (buf, strlen (buf), 1, fp);
    }
    fclose (fp);

    return ret;
}

/*
 * FUNCTION
 * 	Receive::HandleTemperatureData
 * 
 * DESCRIPTION
 * 	Handles temperature data - stores it in the appropriate place in the
 * 	raw data memory, and increases the counter for stored temperature readings.
 */

void
Receive::
HandleTemperatureData()
{
	if ( receivedDataCounter->temperatureCount < MAX_TEMPERATURES )
	{
		// Save the temperature readings.
		RawTemperatures_t * t = &rotorRawReadings->temperatures[ receivedDataCounter->temperatureCount ];
		receivedDataCounter->temperatureCount++;
		memcpy( &t->engineTime, &rxFrame[6], 4 );
		memcpy( &t->rotorTemperature, &rxFrame[10], 2 );
		memcpy( &t->ambientTemperature, &rxFrame[12], 2 );
		memcpy( &t->topPlateTemperature, &rxFrame[14], 2 );
		memcpy( &t->bottomPlateTemperature, &rxFrame[16], 2 );
		memcpy( &t->topPlateCurrent, &rxFrame[18], 2 );
		memcpy( &t->bottomPlateCurrent, &rxFrame[20], 2 );
	}
}

/*
 * FUNCTION
 * 	Receive::HandleStatusMessage
 * 
 * DESCRIPTION
 * 	Handles analysis status messages - stores them in the appropriate place in the
 * 	raw data memory, increments the count of status messages received, and
 * 	forwards them to the controlling external process.
 */

void
Receive::
HandleAnalysisStatusMessage()
{
	if ( receivedDataCounter->statusCount < MAX_ANALYSIS_STATUS )
	{
		// Save the status message.
		AnalysisStatus_t * s = &rotorRawReadings->analysisStatus[ receivedDataCounter->statusCount ];
		receivedDataCounter->statusCount++;
		memcpy( &s->engineTime, &rxFrame[6], 4 );
		s->analysisStatus = rxFrame[10];
		s->analysisParameter = rxFrame[11];
	}

	if ( rxFrame[10] == PASS_DONE )
	{
		receivedDataCounter->passCount++;
	}
}		
	
/*
 * FUNCTION
 * 	Receive::HandleAdcReferenceOffset
 * 
 * DESCRIPTION
 * 	Handles ADC reference offset messages - stores the value in the appropriate
 *	place in the raw data memory.
 */

void
Receive::
HandleAdcReferenceOffset()
{
	memcpy( &rotorRawReadings->adcBias.engineTime, &rxFrame[6], 4 );
	memcpy( &rotorRawReadings->adcBias.adcReferenceOffset, &rxFrame[10], 2 );
}		


/*
 * FUNCTION
 *  HandleMotorTestMessage
 *
 * DESCRIPTION
 *  Stores motor test position data  in raw data memory
 *
 */
void
Receive::
HandleMotorTestMessage ()
{

    unsigned short  length;
    unsigned short  packageNum;
    RawAmsMotorTestData_t *m =  &rotorRawReadings->motorTestData [amsTestCounter.motorCount];


    /* get totally data length */
    memcpy (&length, &rxFrame [6], 2);

    /* get package number */
    memcpy (&packageNum, &rxFrame [8], 2);

    // take out package number
    length -=2; 

    // copy position to share memory
    memcpy (&m->position, &rxFrame[10], length);
    // two bytes per data
    length /=2;

    if (amsTestCounter.motorCount < (MAX_MOTOR_TEST_DATA - length)) {
        amsTestCounter.motorCount += length;
    }
}
	
/*
 * FUNCTION
 * 	AsciiToHex
 * 
 * DESCRIPTION
 * 	Converts an ASCII hexadecimal string number to a binary value.
 * 
 * PARAMETERS
 * 	ascii - ASCII hexadecimal string.
 * 
 * RETURNS
 * 	Binary value.
 */

unsigned char
Receive::
AsciiToHex( char ascii )
{
	unsigned char hex = 0xFF;
	
	if ( isxdigit( ascii ) )
	{
		hex = ascii & 0x0f;
		if ( ascii > '9' )
		{
			hex += 9;
		}
	}

	return hex;
}


void
Receive::
SendNak()
{
	unsigned char ch = NAK;
	SendMsgToSerialPort(fdSer, &ch, 1);
}
