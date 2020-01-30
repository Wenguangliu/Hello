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



#include "CommonDefines.h"
#include "Global.h"
#include "Receive.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "Shmem.h"
#include "AmsData.h"
#include "LogErr.h"
#include "FtdiConfig.h"

// Timeout value for receiving a character within a message.
const int RX_TIMEOUT = 1000;	// 1000ms = 1 sec

// Maximum binary byte count for receive.
const unsigned MAX_BINARY_COUNT = ENGINE_COMM_MAX_MSG - 2;

FactoryData_t *Receive::factoryData = (FactoryData_t *)MapSharedMem (FACTORY_SHMEM_NAME, sizeof (FactoryData_t));
#define  TEMP_DATA_DIRECTORY    "/Hard Disk/Abaxis/TempData"

Receive::
Receive()
{

    MSGQUEUEOPTIONS         options;

    options.dwSize = sizeof(MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = FALSE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;

    /* intial ams queue for forwarding engine message*/
    mqueue = CreateMsgQueue (AMS_QUEUE_NAME, &options);
    if (mqueue == NULL) {
        LogError ("Failure to open queue %s", AMS_QUEUE_NAME); 
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
			if ( count < 1 )
			{
                SendNak();
				state = RX_IDLE;
				LogError( "EngineComm: timeout, returning to RX_IDLE" );
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
				case RX_IDLE:												
                break;
					
				// ASCII data 
                // includes end-of-frame and end-of-message characters.
				case RX_DATA:			
                    state = HandleAsciiChar( ch );		
                break;

				// LSB of binary data count.
				case RX_BINARY_LSB:		
                    state = HandleBinaryCountLsb( ch );	
                break;

				// MSB of binary data count.
				case RX_BINARY_MSB:		
                    state = HandleBinaryCountMsb( ch );	
                break;
						
				// Binary data.
				case RX_BINARY_DATA:	
                    state = HandleBinaryData( ch );		
                break;

                default:
                    LogError ("Wrong Engine Rx State");
                break;
    
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
 * 	intial shared memory for transfering data.
 */

void
Receive::
InitSharedMemory()
{

	amsData = (AmsData_t *)MapSharedMem(AMS_SHMEM_NAME, sizeof(AmsData_t));

	if  (amsData == NULL) {
		LogError ( "Receive:Failed to map shared memory" );
	}
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
    wchar_t         localWSTR[256];
    MSGQUEUEOPTIONS         options;

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
	count =  ReceiveMsgFromSerialPort(fdSer, &ch, 1, 0);
	if ( count < 1 )
		return RX_IDLE;
		
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
	SendMsgToSerialPort (fdSer, &ch, 1);
		
	// Initialize the receive state and receive frame.
	state = RX_DATA;
	InitFrame ();
	
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
	RxState_t state = RX_DATA;
    int         nakCount = 0;
	
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
			SendMsgToSerialPort ( fdSer,(unsigned char *)&ch, 1 );

			// Process the frame.
			if (ch == ACK) 
            {
				ProcessFrame();
			}
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
	RxState_t state = RX_IDLE;
	
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
		LogError ( "EngineComm: Binary count > %d", MAX_BINARY_COUNT );
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
		LogError ( "EngineComm: got too many characters for frame (max = %d)", RX_FRAME_SIZE );
	}
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
        LogError ("CR/LF not where expected");
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
	switch (rxFrame[2])
	{
		case 'A':	// Analysis status.
			HandleAnalysisStatusMessage();
			forwardMessage = true;
			break;
		
		case 'R':	// Photometric data.
			HandlePhotometricData();
			break;
		
		case 'T':	// Temperature readings.
            // first message, send to Ams process
            if (temperatureCount == 0) {
                forwardMessage = true;
            }
			HandleTemperatureData();
			break;

        case 'L':
            // for AMS, only suuport motor test raw data dump
             switch (rxFrame [3]) {
                case 'T':
                break;

                case 'M':
                    HandleMotorTestMessage ();
                break;

                case 'S':
                //    LogDebug ("type =%d  status =%u", rxFrame [10], (unsigned short)rxFrame [12]);
                break;

                default:
                break;
            }

        break;

        case 'W':
            //ignor watchdog command
            break;
		default:
			forwardMessage = true;
			break;
	}
	
	if (forwardMessage) {

		if (mqueue != 0) {
			msg.msgId = MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED;
			
			// Data is rxFrame[2] thru rxFrame[ rxCount - 6 ]
			memcpy( msg.data.message, rxFrame + 2, rxCount - 7 );
			msg.data.length = rxCount - 7;
            WriteMsgQueue(mqueue, &msg, sizeof( int ) + msg.data.length, INFINITE, NULL);

		}
	}		
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
 * 	Receive::HandlePhotometricData
 * 
 * DESCRIPTION
 * 	Handles the photometer data - stores it in the appropriate place in the
 * 	raw data memory.
 */

void
Receive::
HandlePhotometricData ()
{
	RawFlashWavelengths_t * r = NULL;
	int i;
	unsigned char cuvetteNumber = rxFrame[10];
	unsigned char sampleType    = rxFrame[11];
	unsigned char flashNumber   = rxFrame[12];

	if (cuvetteNumber < NUM_CUVETTES) {
		// Analyte data.
		if (sampleType < 128) {
            if (flashNumber <= NUM_SAMPLE_FLASHES) {
			    r = &amsData->rotorAnalyteSamples[sampleType][passCount][flashNumber];
            } else {
                LogError( "flash # of %d received for analyte data", flashNumber);
            }
		} else {
			switch (sampleType) {

				case BEAD_CHK_1:
					if (flashNumber <= NUM_BEAD_CHECK_FLASHES) {
						r = &amsData->beadCheck1[cuvetteNumber][flashNumber];
					} else {
                        LogError( "flash # of %d received for BEAD_CHK_1", flashNumber );

                    }
					break;
	
				case BEAD_CHK_2:
					if (flashNumber <= NUM_BEAD_CHECK_FLASHES) {
						r = &amsData->beadCheck2[cuvetteNumber][flashNumber];
					} else {
                        LogError( "flash # of %d received for BEAD_CHK_2", flashNumber );

                    }
					break;
	
				case DISTRIBUTION_CHK:
					if (flashNumber <= NUM_DIST_CHECK_FLASHES) {
						r = &amsData->distributionCheck[cuvetteNumber][flashNumber];
					} else {
                          LogError( "flash # of %d received for DISTRIBUTION_CHK", flashNumber );

                    }
                    
					break;
	
				case PRE_SAMPLE:
					if (flashNumber <= NUM_FRS_SAMPLE_FLASHES) {
						r = &amsData->preSampleFullRotorScan[cuvetteNumber][flashNumber];
					} else {
                         LogError( "flash # of %d received for PRE_SAMPLE", flashNumber );

                    }
					break;
	
				case POST_SAMPLE:
					if (flashNumber <= NUM_FRS_SAMPLE_FLASHES) {
						r = &amsData->postSampleFullRotorScan[cuvetteNumber][flashNumber];
					} else {
                         LogError( "flash # of %d received for POST_SAMPLE", flashNumber );

                    }
					break;
	
				case OPTICAL_DACS:
					if (flashNumber <= NUM_OPTICAL_DACS_FLASHES) {
						r = &amsData->opticalDacs[flashNumber];
					} else {
                        LogError( "flash # of %d received for OPTICAL_DACS", flashNumber );

                    }
					break;
	
				case BLACK_OFFSETS:
					if (flashNumber <= NUM_BLACK_OFFSETS_FLASHES) {
						r = &amsData->blackOffsets[flashNumber];
					} else {
                        LogError( "flash # of %d received for BLACK_OFFSETS", flashNumber );

                    }
					break;

                
                case  CUV_DELAY_SAMPLE:

                    if (flashNumber < 21) {
                        r = &amsData->cuvetteDealy[cuvetteDelayCount][flashNumber];
                        /* next cuvette delay data */
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
                        r = &amsData->ndxtSamples [cuvetteNumber][flashNumber];
                    } else {
                         LogError( "flash # of %d received for NDXT", flashNumber );
                    }
                break;
	
				default:
	//				LogError ("EngineComm: 'R' message, cuvette # %d, sample type %d", cuvetteNumber, sampleType);
					break;
			}
		}
	} else {
		LogError( "EngineComm: 'R' message with cuvette # %d", cuvetteNumber );
	}
				
	// Save the data to the appropriate location.
	if (r != NULL) {
		memcpy (&r->engineTime, &rxFrame[6], 4);
		r->cuvetteNumber = cuvetteNumber;
		r->sampleType = sampleType;
		r->flashNumber = flashNumber;
	    for (i = 0; i < 10; i++) {
			    memcpy( &r->rawFlashReadings[i], &rxFrame[14 + 2*i], 2 );
	   }				
	}
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
HandleTemperatureData ()
{
	// Save the temperature readings.
	RawTemperatures_t * t = &amsData->temperatures[ temperatureCount ];
    if (temperatureCount < (MAX_TEMPERATURES - 1)) { 
	    temperatureCount++;
    }
	memcpy( &t->engineTime, &rxFrame[6], 4 );
	memcpy( &t->rotorTemperature, &rxFrame[10], 2 );
	memcpy( &t->ambientTemperature, &rxFrame[12], 2 );
	memcpy( &t->topPlateTemperature, &rxFrame[14], 2 );
	memcpy( &t->bottomPlateTemperature, &rxFrame[16], 2 );
	memcpy( &t->topPlateCurrent, &rxFrame[18], 2 );
	memcpy( &t->bottomPlateCurrent, &rxFrame[20], 2 );
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
	// Save the status message.
	AnalysisStatus_t * s = &amsData->analysisStatus[statusCount];

    if (statusCount < MAX_ANALYSIS_STATUS) {
	    statusCount++;
	    memcpy (&s->engineTime, &rxFrame[6], 4);
	    s->analysisStatus = rxFrame[10];
	    s->analysisParameter = rxFrame[11];
    }
	if (rxFrame[10] == PASS_DONE) {
        if (passCount < (MAX_PASSES -1)) {
		    passCount++;
        }
	}
}		

/*
 * FUNCTION
 *  HandleMotorTestMessage	
 * 
 * DESCRIPTION
 *  Stores motor test position data  in raw data memory
 * 
 */
#if 0 
void
Receive::
HandleMotorTestMessage ()
{
    // save motor positon data
    RawMotorTestData_t *m =  &amsData->motorTestData [motorCount];

    if (motorCount < MAX_MOTOR_TEST_DATA) {
        motorCount++;
    }
    memcpy (&m->position, &rxFrame[6], 2);
}
#else
void
Receive::
HandleMotorTestMessage ()
{

    unsigned short  length;
    unsigned short  packageNum;
    RawMotorTestData_t *m =  &amsData->motorTestData [motorCount];


    /* get totally data length */
    memcpy (&length, &rxFrame [6], 2);

    /* get package number */
    memcpy (&packageNum, &rxFrame [8], 2);

    // take out package number
    length -=2;

    //LogDebug ("length = %d pakNum =%d cont =%d",length, packageNum, motorCount);

    // copy position to share memory
    memcpy (&m->position, &rxFrame[10], length);

    // two bytes per data
    length /=2;

    if (motorCount < (MAX_MOTOR_TEST_DATA - length)) {
        motorCount += length;
    }
}
#endif
	
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
AsciiToHex(char ascii)
{
	unsigned char hex = 0xFF;
	
	if (isxdigit( ascii )) {
		hex = ascii & 0x0f;
		if (ascii > '9') {
			hex += 9;
		}
	}

	return hex;
}

/* initial ams raw data count to be zero */
void
Receive::
InitAmsData (void)
{

    passCount           = 0;
    temperatureCount    = 0;
    statusCount         = 0;
    cuvetteDelayCount   = 0;
    motorCount          = 0;
}

void
Receive::
SendNak()
{
    unsigned char ch = NAK;
    SendMsgToSerialPort(fdSer, &ch, 1);
}

