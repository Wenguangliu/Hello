
// ********************************************************************
//
// FILENAME:  
// 
// 		Astm.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		ASTM process
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//

#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <msgqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <atlbase.h>
#include <winsock2.h>
#include <notify.h>

#include <FtdiConfig.h>

#include "AlgGlobals.h"
#include "Astm.h"
#include "AstmData.h"
#include "AstmOrders.h"
#include "AstmConstants.h"
#include "AstmMessages.h"
#include "AstmQuery.h"
#include "AstmInput.h"
#include "LogErr.h"
#include "ResultsData.h"
#include "SettingsData.h"
#include "Shmem.h"

#define ASTM_BAUD_RATE				9600

#define SERR_ASTM_RX_BAD_COUNT		0x4fff						// port read error

#define RECONNECT_INTERVAL			15000							// TCP/IP socket reconnect interval in seconds

#define NO_ERRORS		0


// ASTM interface definitions       

#define ASTM_RX_SER_TIMEOUT			1							// 0.1 seconds serial port readcond() timeout
#define ASTM_RX_SER_TIMEOUT_MS		100						// 0.1 seconds serial port readcond() timeout, in milliseconds
#define ASTM_RX_SER_TIMEOUT_US		100000						// 0.1 seconds serial port readcond() timeout, in microseconds
#define ASTM_RX_SER_TIMEOUT_SEC		0
#define ASTM_RX_TCPIP_TIMEOUT_US	100000						// 0.1 seconds socket read timeout
#define ASTM_RX_TCPIP_TIMEOUT_SEC	0
#define ASTM_RX_BUFFER_SIZE			1000				
#define ASTM_TX_BUFFER_SIZE			1000				

// ASTM timeout definitions (time in seconds + 1)       

#define ASTM_TIMED_OUT				0							// timer timed out/disabled       
#define ASTM_1S_TIMEOUT				(1+1)						//  1 to  2 seconds			      
#define ASTM_10S_TIMEOUT			(10+1)						// 10 to 11 seconds			           
#define ASTM_15S_TIMEOUT			(15+1)						// 15 to 16 seconds			      	
#define ASTM_30S_TIMEOUT			(30+1)						// 30 to 31 seconds			      

#define NO_RX_CHAR						0xff					// no char received due to readcond timeout
#define	NOT_VALID_ASTM_CHAR				0xff					// no valid ASTM 7 bit ASCII character received       

#define	ABAXIS_ASTM_TX_MSG_TRANSMITTED	true					// for Abaxis astm message transmit done   		      
#define	ABAXIS_ASTM_TX_MSG_NOT_DONE		false					// for Abaxis astm message transmit not done   	      
#define	MAX_ASTM_TX_RETRIES_ALLOWED		6						// maximum astm transmit retries allowed   		      

#define ASTM_TX_C_SKIP					0x80					// set in astm_tx_C_sequence_number to signal skip of C record       

// abaxis receive msg type definitions       

#define	ASTM_RX_QUERY_MSG				1						// abaxisAstmRxMsgToProcess: query message (H,Q,L)       
#define ASTM_RX_MFG_MSG                 2                       // abaxisAstmRxMsgToProcess: manufacturers status message (H,M,L)
#define ASTM_RX_ORDER_MSG               3                       // abaxisAstmRxMsgToProcess: patient order download message (H,P,O,L)

#define	MAX_RX_MSG_RECORDS				10						// max rx records in abaxis astm rx msg		       

#define MAX_INVALID_RX_FIELDS			127						// max number of fields for invalid record detected


// abaxis Query message states
	
typedef enum
{
	ASTM_QUERY_INIT,											// astm query state machine definitions
	ASTM_QUERY_NO_RECORDS, 		
	ASTM_QUERY_NEXT_RECORD,
	ASTM_QUERY_COMPLETED,       
} AstmQueryState_t;
		

// abaxis transmit range message states
	
typedef enum
{
	ASTM_TRANSMIT_RANGE_INIT,									// astm transmit range state machine definitions
	ASTM_TRANSMIT_RANGE_NO_RECORDS, 		
	ASTM_TRANSMIT_RANGE_NEXT_RECORD,
	ASTM_TRANSMIT_RANGE_COMPLETED,       
} AstmTransmitRangeState_t;
		

// abaxis (H C1 L) comment message type definitions       

typedef enum
{
	ASTM_POWER_UP_SIGN_ON,										// abaxis comment message sent at power up        
																// NO_ERRORS used if no astm 1394 rx irregularity       
	ASTM_1394_RX_DELIMITER_MISMATCH,							// astm 1394 field delimiter does not match expected delimiter       
	ASTM_1394_RX_FIELD_SIZE_TOO_BIG,							// astm 1394 field size is greater than maximum field size allowed       
	ASTM_1394_RX_Q_SEQUENCE_NUM_BAD,							// astm 1394 bad (not = 1) Q record sequence number value received       
	ASTM_1394_RX_L_SEQUENCE_NUM_BAD,							// astm 1394 bad (not = 1) L record sequence number value received       
	ASTM_1394_RX_INVALID_RECORD_TYPE,							// astm 1394 not a valid record type       
	ASTM_1394_RX_TOO_MANY_FIELDS_IN_RECORD,						// astm 1394 too many fields for this record type       
	ASTM_1394_RX_RECORD_OUT_OF_SEQUENCE,						// astm 1394 record not between H ... L sequence       	
	ASTM_1394_RX_MESSAGE_NOT_SUPPORTED,							// astm 1394 record not between H ... L sequence       	
    ASTM_1394_RX_P_SEQUENCE_NUM_BAD,                            // astm 1394 bad (not = 1) P record sequence number value received
    ASTM_1394_RX_O_SEQUENCE_NUM_BAD,                            // astm 1394 bad (not = 1) O record sequence number value received
    ASTM_1394_RX_M_SEQUENCE_NUM_BAD,                            // astm 1394 bad (not = 1) M record sequence number value received

} Astm1394CommentType_t;

#define ASTM_RESULT_FLAGS_COMMENT_MASK		( MASK_RESULTS_SUPPRESSED | MASK_LOW_SYSTEM_LIMIT | MASK_HIGH_SYSTEM_LIMIT |	MASK_LOW_DYNAMIC_LIMIT | MASK_HIGH_DYNAMIC_LIMIT | MASK_HEMOLYZED | MASK_HEMOLYZED_LOW | MASK_HEMOLYZED_HIGH | MASK_HEMOLYZED_SUPPRESSED |MASK_LIPEMIC | MASK_LIPEMIC_LOW | MASK_LIPEMIC_HIGH | MASK_LIPEMIC_SUPPRESSED | MASK_ICTERIC | MASK_ICTERIC_LOW | MASK_ICTERIC_HIGH | MASK_ICTERIC_SUPPRESSED | MASK_DILUTION_CHECK )


// abaxis transmit queue definitions       
typedef struct
{
	AbaxisSendType_t	msgSendType;
	unsigned long long	msgParam1;
	char				msgParam2;
	char				msgParam3[15];
	char				msgParam4[15];
    AstmInput_t         msgParam5; 
} AbaxisSendQ_t;


// ASTM 1381-02 / interface processor state  definitions       

typedef enum
{
	NO_ASTM_STATE,

	ASTM_INIT_STATE,
	ASTM_IDLE_STATE,

	ASTM_TX_DATA_TO_SEND_STATE,
	ASTM_TX_WAITING_CONNECTION_STATE,
	ASTM_TX_CONTENTION_OR_COMPUTER_BUSY_STATE,
	ASTM_TX_SETUP_NEXT_FRAME_STATE,
	ASTM_TX_FRAME_READY_STATE,
	ASTM_TX_WAITING_ACK_STATE,
	ASTM_TX_SETUP_LAST_FRAME_STATE,

	ASTM_RX_AWAKE_STATE,
	ASTM_RX_WAITING_STATE,
	ASTM_RX_FRAME_RECEIVED_STATE,
} AstmState_t;


typedef struct
{
	unsigned long	analyteFlagMask;
	const char *	astmRnC1Text;
} AstmRnC1TextData_t;

#define NUM_RN_C1_MASKS					24


unsigned			astmTxBufferIdx_m;							// index to tx buffer & tx message size	
char				astmTxBuffer_m[ASTM_TX_BUFFER_SIZE];		//
unsigned			astmRxBufferIdx_m;							// index to rx buffer       
char				astmRxBuffer_m[ASTM_RX_BUFFER_SIZE];		//
unsigned			astmTimeoutSeconds_m;						// astm timeout second counter/flag       	
AbaxisSendQ_t		abaxisSendQueue_m[ABAXIS_SEND_QUEUE_SIZE];	// astm tx message queue       
unsigned			abaxisSendQueueInputIdx_m;					// astm tx queue input index       
unsigned			abaxisSendQueueOutputIdx_m;					// astm tx queue output index       
AbaxisSendType_t	abaxisSendType_m;							// Abaxis send msg type    (dequeued from transmit Q      
unsigned long long	abaxisSendParam1_m;							// Abaxis send parameter 1 (dequeued from transmit Q
char				abaxisSendParam2_m;							// Abaxis send parameter 2 (dequeued from transmit Q
char				abaxisSendParam3_m[15];						// Abaxis send parameter 3 (dequeued from transmit Q) - transmit range start date/time       
char				abaxisSendParam4_m[15];						// Abaxis send parameter 4 (dequeued from transmit Q) - transmit range end date/time       
AstmTxMsgType_t		astmTxMsgType_m;							// 0: no data, code: Abaxis ASTM message to send       
unsigned			abaxisAstmRxMsgToProcess_m;					// 0: no msg to process, code: Abaxis message to process       
bool				astmRxBusy_m;								// Abaxis instrument astm receiver busy flag       
unsigned			astmTxFrameNumber_m;						// mod 8 frame number incremented for each frame/record transmitted       
unsigned			astmRxFrameNumber_m;						// mod 8 frame number incremented for each frame/record received       
unsigned			astmTxRetryCount_m;							// transmit retry count (max 6)       
unsigned			astmAnalyteIdx_m;							// result index
char				astmTxCtype_m;								// astm transmit record comment type: R or O       
bool				astm1394ErrorDuringRxMsg_m;					// flag that astm 1394 type issue occured during receiving astm 1381-02 acked records       
unsigned			abaxisAstmHclMessageType_m;					// message type for abaxis special comment (H C1 L) messages  
unsigned			astm1394ErrorType_m;						// astm 1394 type issue detected during receiving astm 1381-02 acked record       
char				astm1394ErrorRecordType_m;					// record type saved for (H C1 L) message for astm 1394 type issue       
bool				gotAstmRxHeader_m;							// got first rx header flag       
char				astmRxFieldDelimiter_m;						// receive header field delimiter       
char				astmRxRepeatDelimiter_m;					// receive header repeat delimiter       
char				astmRxComponentDelimiter_m;					// receive header component delimiter       
char				astmRxEscapeDelimiter_m;					// receive header escape delimiter       
char *				abaxisTxMsgDefPtr_m;						// pointer to abaxis message definition working index to current setup record (frame) template       
unsigned			abaxisRxMsgRecordIdx_m;						// index to abaxis rx msg record type array       
char				abaxisRxMsgRecords_m[MAX_RX_MSG_RECORDS];	       
unsigned			astmTxOsequenceNumber_m;					// O order   record current sequence number       
unsigned			astmTxRsequenceNumber_m;					// R result  record current sequence number        
unsigned            astmTxMsequenceNumber_m;                    // M manufacturer  record current sequence number
unsigned			astmTxCsequenceNumber_m;					// C comment record current sequence number        
unsigned			last_astm_tx_iqc_r_record_m;					// last R IQC record to process based on checking if VS2 or xPress
char				astmRxQueryStartId_m[33];					// Q message start id string       
char				astmRxQueryBeginDateTime_m[15];				// Q message begin date/time string       
char				astmRxQueryEndDateTime_m[15];				// Q message end date/time string       
unsigned			astmQueryState_m;							// Q message tx response current query processing state       
unsigned			astmTransmitRangeState_m;					// Q message tx response current transmit range processing state       
AstmRecord_t *		astmRecord_m;
AstmOrders_t        astmOrders_m;
InstrumentInfo_t *	instrumentInfo_m;
struct sockaddr_in	serverSocket_m = { 0 };
int					fd_m = -1;
FT_HANDLE			fd_ser = NULL;
SettingsData_t *	settingsData;
FactoryData_t *     factoryData;
SystemData_t *      systemData;
OrdersList_t *      ordersList;
static ULONGLONG    lastConnectAttempt = 0;
static char         reConnectionReq = 0;
bool				pauseTimer = false;

AstmInput_t         astmInput;                                  // uploaded order message exact ASCII contents
AstmInput_t *       astmInput_m = &astmInput;                   // uploaded order message exact ASCII contents
AstmInput_t         abaxisSendParam5_m;                         // dequeued input record contents - order data invalid send type


/*
 * FUNCTION
 * 	TimerThread
 * 
 * DESCRIPTION
 * 	Sends a message to this ASTM process every 1 second.
 * 
 * PARAMETERS
 *	arg - Standard thread parameter - not used.
 * 
 * RETURN
 * 	0
 */

void *
TimerThread( void * arg )
{
	const int msg = MSG_ASTM_TIMER;

	static HANDLE queue = NULL;
	
	if ( queue == NULL )
	{
		//queue = mq_open( ASTM_QUEUE_NAME, O_WRONLY );
		MSGQUEUEOPTIONS		options;

		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;
		queue = CreateMsgQueue(ASTM_QUEUE_NAME, &options);
	}
	
	if ( queue == NULL )
	{
		LogError( "failure to open ASTM queue in TimerThread" );
		exit( -1 );
	}

	while ( 1 )
	{
		Sleep( 1000 );
	
		if ( !pauseTimer )
		{
			//mq_send( queue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
			WriteMsgQueue(queue, (LPVOID)&msg, sizeof( int ), INFINITE, NULL);
		}
	}
	
	return 0;
}

/*
 * FUNCTION
 * 	SocketConnect
 * 
 * DESCRIPTION
 * 	Attempts to connect to the server's TCP/IP socket. If successful, sets the read timeout to
 * 	one second and sets fd_m to the socket ID. If not, sets fd_m to -1. A timer is used so that
 * 	attempts to connect are done at a maximum of once every RECONNECT_INTERVAL seconds.
 *  in the case of reConnectionReq =1, always to  connect 
 * PARAMETERS
 *	None
 * 
 * RETURN
 * 	None
 */

void
SocketConnect()
{
	int					status;
	struct timeval		t;
	SYSTEMTIME			currentST;
	FILETIME			currentFT;
	ULONGLONG			elapsedTime;
	static bool			connectedLastCall = true;
	struct sockaddr_in	sin;
	int					optval = 1;

	GetLocalTime(&currentST);
	SystemTimeToFileTime(&currentST, &currentFT);
	elapsedTime = (((ULONGLONG) currentFT.dwHighDateTime) << 32) + currentFT.dwLowDateTime - lastConnectAttempt;

	if (elapsedTime > RECONNECT_INTERVAL)
	{
        reConnectionReq = 0;
		//lastConnectAttempt = time( 0 );
		lastConnectAttempt = (((ULONGLONG) currentFT.dwHighDateTime) << 32) + currentFT.dwLowDateTime;
		closesocket( fd_m );
		fd_m = -1;

		// Open the local socket.
		fd_m = socket( AF_INET, SOCK_STREAM, 0 );
		if ( fd_m == INVALID_SOCKET )
		{
			LogError( "socket() failed" );
		}
		else
		{
			sin.sin_family = AF_INET;
			sin.sin_port = htons( settingsData->comm.serverSocket );
			sin.sin_addr.s_addr = INADDR_ANY;
			if ( bind( fd_m, (struct sockaddr *)&sin, sizeof( sin ) ) == SOCKET_ERROR )
			{
				LogError( "bind() failed" );
			}
		
			// Connect to the server socket.
			else
			{			
				/*
				 * We pause the timer while we're connecting, because a failed connect takes about 74 seconds,
				 * and we don't want the message queue to jam up. There's no need for a lock because the timer only
				 * reads the flag, and it won't hurt if it misreads the value while we're writing.
				 */

				pauseTimer = true;
				status = connect( fd_m, (struct sockaddr *)&serverSocket_m, sizeof( serverSocket_m ) );
				pauseTimer = false;

				if ( status == SOCKET_ERROR )
				{
					if ( connectedLastCall )
					{
						LogError( "connect() failed" );
						connectedLastCall = false;
					}
					shutdown(fd_m, SD_BOTH); 
					closesocket( fd_m );
					fd_m = -1;
				}
				else
				{
					optval = 1;
					setsockopt( fd_m, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof( optval ) );
					if ( !connectedLastCall )
					{
						LogDebug( "connect() succeeded" );
						connectedLastCall = true;
					}
				}
			}
		}
	}
}


int
ReadPort( void * buffer )
{
	int			readCount;
	int			error;
	static bool	readOkLastTime = true;
	timeval		tv = { ASTM_RX_TCPIP_TIMEOUT_SEC, ASTM_RX_TCPIP_TIMEOUT_US };
	fd_set		read_fs;
	FD_ZERO(&read_fs);

	// TCP/IP
	if ( settingsData->comm.protocol == COMM_TCPIP )
	{
		// If no connection, attempt to reconnect.
		if ( fd_m == -1 )
		{
			SocketConnect();	// Only tries once every RECONNECT_INTERVAL seconds.
			if ( fd_m == -1 )
			{
				return 0;
			}
		}
		
		// Attempt to read the socket. If the connection was reset or lost, attempt to
		// reconnect. We'll wait until next time to try another read, since reads happen
		// at least once per second.
		//readCount = read( fd_m, buffer, 1 );
		FD_SET(fd_m, &read_fs);
		readCount = select(0, &read_fs, 0, 0, &tv);
		error = WSAGetLastError();
		if ( readCount != SOCKET_ERROR && readCount != 0 )
		{
			readCount = recv( fd_m, (char *)buffer, 1, NULL );
			error = WSAGetLastError();
		}
		if ( ((readCount == SOCKET_ERROR) && (error == WSAECONNRESET)) )	// Connection reset
			 // Note: returns readCount < 1 and errno == EAGAIN.
		{
			if ( readOkLastTime )
			{
				LogError( "read() failed, error = %u", error );
				readOkLastTime = false;
			}
			shutdown(fd_m, SD_BOTH);
			closesocket( fd_m );

			fd_m = -1;
			SocketConnect();

			return 0;
		}
		else
		{
			readOkLastTime = true;
			return readCount > 0 ? readCount : 0;
		}
	}

	// USB/Serial connection, so just do a read with timeout.
	else
	{
		// Conditionally read a character and return the count.
		//return readcond( fd_m, buffer, 1, 1, 0, ASTM_RX_SER_TIMEOUT );
		return (ReceiveMsgFromSerialPort (fd_ser, (unsigned char*)buffer, 1, ASTM_RX_SER_TIMEOUT_MS));
	}
}

void
WritePort( char * ch, int count )
{
	int			sent;
	int			error;
	static bool	writeOkLastTime = true;

	// If there's currently no connection, try to connect before giving up.
	if ( (settingsData->comm.protocol == COMM_TCPIP) && (fd_m == -1) )
	{
		SocketConnect();
		if ( fd_m == -1 )
		{
			return;
		}
	}

	// Write the buffer.

	// TCP/IP
	if ( settingsData->comm.protocol == COMM_TCPIP )
	{
		//sent = write( fd_m, ch, count );
		sent = send(fd_m, ch, count, NULL);
		error = WSAGetLastError();
		if ( sent < count )
		{
			if ( writeOkLastTime )
			{
				LogError( "write() failed, error = %u", error );
				writeOkLastTime = false;
			}

			// If we're using TCP/IP, make sure our end of the socket is closed down and
			// make an attempt to reconnect.
			if ( settingsData->comm.protocol == COMM_TCPIP )
			{
				shutdown(fd_m, SD_BOTH);
				closesocket( fd_m );
				fd_m = -1;
				SocketConnect();
			}
		}
		else
		{
			writeOkLastTime = true;
		}
	}
	else // USB/Serial connection, so just do a read with timeout.
	{
		sent = SendMsgToSerialPort (fd_ser, (unsigned char*)ch, count);
		error = GetLastError();
		if ( sent < count ) {
			if ( writeOkLastTime )
			{
				LogError( "write() failed, error = %u", error );
				writeOkLastTime = false;
			} 
    
		}
		else
		{
			writeOkLastTime = true;
		}
	}
}

// ********************************************************************
// FUNCTION:
//
//		QueueAstmTxMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Queue up an Abaxis ASTM message for sent to LIS  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void QueueAstmTxMsg( AbaxisSendType_t msgSendType, unsigned long long msgParam1 = 0, char msgParam2 = '\0', const char *msgParam3 = "", const char *msgParam4 = "", const AstmInput_t * msgParam5 = 0 )
{
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgSendType = msgSendType;
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam1   = msgParam1;
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam2   = msgParam2;
    strcpy( abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam3, msgParam3 );
    strcpy( abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam4, msgParam4 );
    if ( msgParam5 != 0 )
    {
        memcpy( &abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam5, msgParam5, sizeof( AstmInput_t ) );
    }

	abaxisSendQueueInputIdx_m	= (abaxisSendQueueInputIdx_m + 1) % ABAXIS_SEND_QUEUE_SIZE;
}

void FillAstmOrdersList()
{
    int i;

    // Make sure all unused entries are nulls.
    memset( astmOrders_m.mPendingOrders, 0, sizeof( astmOrders_m.mPendingOrders ) );

    for ( i = 0; i < ordersList->numOrders; i++ )
    {
        strcpy( astmOrders_m.mPendingOrders[ i ].mnOrderDateTime, ordersList->orders[ ordersList->index[ i ] ].orderDateTime );
        strcpy( astmOrders_m.mPendingOrders[ i ].mnOrderPatientControlId, ordersList->orders[ ordersList->index[ i ] ].patientId );
        sprintf( astmOrders_m.mPendingOrders[ i ].mnOrderRotorNumber, "%u", ordersList->orders[ ordersList->index[ i ] ].rotorId );
        strcpy( astmOrders_m.mPendingOrders[ i ].mnOrderLaboratoryField1, ordersList->orders[ ordersList->index[ i ] ].orderLabel );
    }
}


// ********************************************************************
// FUNCTION:
//
//		WaitForMsg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits for QNX message received for:
//			- 1 second timeout message 	
//			- last rotor run results record available 		
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

AstmMsgId_t WaitForMsg(void)
{
	AstmMsg_t				msg;
	static HANDLE			queue = 0;
	//static struct mq_attr	mqstat;
	DWORD					receivedSize = 0;
	char					startDate[20];
	char					endDate[20];
	DWORD					flags;
	BOOL					result;

	if ( queue == 0 )
	{
		MSGQUEUEOPTIONS			options;
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = TRUE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;
		queue = CreateMsgQueue(ASTM_QUEUE_NAME, &options);

		//queue = mq_open( ASTM_QUEUE_NAME, O_RDONLY );
	
		if ( queue == NULL )
		{
			LogError( "failure to open ASTM queue" );
			exit( -1 );
		}
		
		/*if ( mq_getattr( queue, & mqstat ) == -1 )
		{
			LogError( "failure to get ASTM queue attributes" );
			exit( -1 );
		}*/
	}
		
	//receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );
	result = ReadMsgQueue(queue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);

	if ( result )
	{
		if ( receivedSize > 0 )
		{
			switch ( msg.msgId )
			{
				case MSG_ASTM_TIMER:
					// decrement astm 1 second timer if non-zero
					if ( astmTimeoutSeconds_m > 0 )
					{
						astmTimeoutSeconds_m--;
					}
					break;

				case MSG_ASTM_LAST_ROTOR_AVAILABLE:
					// queue ASTM message for last rotor run record available by database record ID
					QueueAstmTxMsg( ABAXIS_SEND_ROTOR_RUN_DONE_TYPE, msg.data );
					break;

				case MSG_ASTM_SEND_RANGE:
					FormatDateTime( msg.rangeData.startDate, startDate );
					FormatDateTime( msg.rangeData.endDate, endDate );
					QueueAstmTxMsg( ABAXIS_SEND_RESULTS_RANGE_TYPE, 0, 0, startDate, endDate );
					break;

                case MSG_ASTM_CANCEL_ORDER:
                    QueueAstmTxMsg( ABAXIS_SEND_ORDER_CANCELLATION_TYPE,
                                    msg.cancelData.rotorId,
                                    msg.cancelData.cancelType,
                                    msg.cancelData.patientControlId,
                                    msg.cancelData.orderDateTime );
                    break;


				default:
					LogWarning( "ASTM received unexpected message %d", msg.msgId );
					return (AstmMsgId_t)-1;
					break;
			}
		}
	}
	else
	{
		LogError( "message receive error" );
		return (AstmMsgId_t)-1;
	}

	return msg.msgId;
}	


// ********************************************************************
// FUNCTION:
//
//		AstmTimedOut() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits for a message (1 second timeout or from Analysis process)
//		and returns flag status for ASTM 1381-02 state machine time out  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char AstmTimedOut()
{
	WaitForMsg();

	return ( astmTimeoutSeconds_m == 0 );
}


//      	
//	generic functions follow that support overall ASTM message and interface processing including:
//		- sending 	chars direct to   ASTM port
//		- receiving chars direct from ASTM port


// ********************************************************************
// FUNCTION:
//
//		SendAstmChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Send 1 char output to ASTM port  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SendAstmChar( char astmTxChar )
{
	WritePort( &astmTxChar, 1 );
}


// ********************************************************************
// FUNCTION:
//
//		ReceivedAstmRxChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns ASCII char received from ASTM port buffer or returns NO_RX_CHAR
//		NO_RX_CHAR (0xff) if no char in ASTM port buffer in timeout period (100 ms)
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char ReceivedAstmRxChar( unsigned short *errorNumber )
{
	char	astmRxChar = (char) NO_RX_CHAR;
	int		count = 1;
	
	count = ReadPort( &astmRxChar );

	if (( count < 0 ) || (count > 1))				// error in port system call returns NO_RX_CHAR
	{
		*errorNumber = SERR_ASTM_RX_BAD_COUNT;
	}
	else if ( count == 1 )							// got 1 valid char, so mask off MSB for ASCII
	{
		astmRxChar &= 0x7f;
	}

	// implicit else for ( count == 0 ) which is for ReadPort() timeout condition returns NO_RX_CHAR

	return	astmRxChar; 
}


//      	
//	functions follow that support sending ASTM 1394-97 tx frames:
//		- output tx char and running checksum processing
//		- output tx ESC method data processing including running checksum for frame
//		- ASTM 1394-97 tx frame overall processing:
//			SetupTxMessageFrames() -- called from ProcessAstmState() state machine


// ********************************************************************
// FUNCTION:
//
//		PutAstmTxChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Put a ASCII char in ASTM frame buffer for write to ASTM port  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void PutAstmTxChar( char ch )
{
	astmTxBuffer_m[astmTxBufferIdx_m++] = ch;
}


// ********************************************************************
// FUNCTION:
//
//		PutAstmTxChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Put a ASCII char in ASTM frame buffer for write to ASTM port  
//		and return running checksum including ASCII char put in buffer 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

unsigned char PutAstmTxCharAndChecksum( char ch, unsigned char checksum )
{
	PutAstmTxChar( ch );			// set char in astm tx buffer       

	return ( checksum + ch );			// compute running checksum and return       
}


// ********************************************************************
// FUNCTION:
//
//		SetAstmSeqNum() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set ASCII sequence number for ASTM frame buffer 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SetAstmSeqNum(unsigned seqNum,char *textPtr)
{
	// max sequence number processed is 99 which is much greater than anticipated       

	if (seqNum / 10)
	{
		*textPtr++ = (seqNum / 10) + '0';
	}

	*textPtr = (seqNum % 10) + '0';
}	


// ********************************************************************
// FUNCTION:
//
//		SuppressBlanksFromAstmMethodString() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Suppress leading & trailing blanks in method fields for ASTM frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char *SuppressBlanksFromAstmMethodString(char *sourcePtr, char *suppressedPtr)
{
	bool	gotNonBlank;
	char	*suppressedStr;

	// save pointer to final suppressed text for return       

	suppressedStr = suppressedPtr;

	// suppress leading blanks and copy back to suppressed string for non-empty strings       

	gotNonBlank = false;

	while (*sourcePtr)								// process source method string for non null chars       
	{
		if (*sourcePtr != ' ')						// not a null char or a blank char        
		{
			gotNonBlank = true;
			*suppressedPtr++ = *sourcePtr;			// set in suppressed text string       
		}
		else if (gotNonBlank)						// blank char but not leading blank, so keep it       
		{
			*suppressedPtr++ = *sourcePtr;			// set in suppressed text string       
		}

		// default else: leading blanks not copied to suppressed string       

		sourcePtr++;								// in all cases incr for next source method string char       
	}

	// for strings with non blank chars, suppress any trailing blanks in method text string       

	if (gotNonBlank)								// if source method text string has non blank char       
	{
		*suppressedPtr-- = '\0';					// set delimiter and point to last char of suppressed text       

		while (*suppressedPtr == ' ')	
		{
			*suppressedPtr-- = '\0';				// if trailing blank, suppress with nulls       
		}
	}

	return suppressedStr;
}


// ********************************************************************
// FUNCTION:
//
//		SetupOrecordCtypeGtext() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up O1 & O2 record comment strings for ASTM frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char *SetupOrecordCtypeGtext( char *commentTextPtr )
{
	if (astmTxOsequenceNumber_m == 1) 
	{
		switch ( astmTxCsequenceNumber_m )
		{
			case 1:		// O record 1: C record 1: "INST QC: OK    CHEM: OK"
				commentTextPtr = (char *) qcOkTextString;
				break;

			case 2:		// O record 1: C record 2: "HEM X ,  LIP Y ,  ICT Z"
				sprintf( commentTextPtr, "%s %s  %s %s  %s %s", hemTextString, astmRecord_m->o1C2HemString, lipTextString, astmRecord_m->o1C2LipString, ictTextString, astmRecord_m->o1C2IctString );      
				break;

			case 3:		// O record 1: C record 3: "@ CONFIRM LOW RECOVERIES @", if error condition
				if (astmRecord_m->o1C3RotorDilutionCheckError) 
				{ 
					commentTextPtr = (char *) rotorDiluteCheckFailedString;
				}
				break;

			case 4:		// O record 1: C record 4: operator expired or operator unauthorized string, if security issue
					commentTextPtr = astmRecord_m->o1C4OperatorSecurityStatus;
				break;

			case 5:		// O record 1: C record 5: control expired string, if control issue
					commentTextPtr = astmRecord_m->o1C5ControlExpired;
				break;

			default:
				// not supported then string copy for O1 comment        
				break;
		}
	}
	else
	{
		if (astmTxCsequenceNumber_m == 1)				// O record 2: C record 1: "CHEMISTRY QC:         X"       
		{
			sprintf( commentTextPtr, "%s %s", chemQcTextString, astmRecord_m->o2C1RqcResultString );      
		}
		else											// O record 2: C record 2: "ACCEPTABLE MINIMUM   YY"       
		{
			sprintf( commentTextPtr, "%s %s", acceptableMinTextString, astmRecord_m->o2C2RqcLowLimitString );      
		}
	}

	return commentTextPtr;
}


// ********************************************************************
// FUNCTION:
//
//		SetupRnC1Code() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up Rn records C1 comment code string for result analyte flags 
//		for ASTM frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SetupRnC1Code( char *methodTextPtr ) 
{
	unsigned long	analyteFlags;

	analyteFlags = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnC1AnalyteFlags;

	sprintf( methodTextPtr, "%4.4lX %4.4lX", analyteFlags >> 16, analyteFlags & 0xFFFF );				
}


// ********************************************************************
// FUNCTION:
//
//		SetupRnC1Text() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up Rn records C1 comment text string for result analyte flag type 
//		for ASTM frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char *SetupRnC1Text( void )
{
	unsigned		i;
	unsigned long	analyteFlags;

	const AstmRnC1TextData_t	astmRnC1TextData[NUM_RN_C1_MASKS] =
	{
		{ HEMOLYZED_LIMIT_ERROR,       hemolyzedLimitErrorString27   },		// highest priority
		{ LIPEMIC_LIMIT_ERROR,         lipemicLimitErrorString26     },
		{ ICTERIC_LIMIT_ERROR,         ictericLimitErrorString25     },
		{ RQC_ERROR,                   rqcErrorString13              },
		{ CHEM_BEAD_MISSING,           chemBeadMissingString11       },
		{ CHEM_DIST_ERROR,             chemDistErrorString10         },
		{ FLASH_RANGE_ERROR,           flashRangeErrorString31       },
		{ DIVIDE_BY_ZERO,              divideByZeroString30          },
		{ ILLEGAL_LOG_PARAM,           illegalLogParamString15       },
		{ START_ABS_BAD,               startingAbsBadString24        },
		{ TEST_BEAD_MIX_ERROR,         testBeadMixErrorString29      },
		{ BLANK_BEAD_MIX_ERROR,        blankBeadMixErrorString28     },
		{ END_ABS_BAD,                 endingAbsBadString23          },
		{ RATE_NOT_LINEAR,             rateNotLinearString20         },
		{ FALSE_RATE,                  falseRateString21             },
		{ RATE_IN_ENDPOINT,            rateInEndpointString22        },
		{ RATE_TOO_LOW,                rateTooLowString12            },
		{ INCONSISTENT_RESULT,         inconsistentResultString14    },
		{ DILUTION_VERIFICATION_ERROR, chemDiluteCheckFailedString09 },
		{ ABOVE_SYSTEM_RANGE,          aboveSystemRangeString19      },
		{ BELOW_SYSTEM_RANGE,          belowSystemRangeString18      },
		{ HIGH_DYNAMIC_RANGE,          highDynamicRangeString17      },
		{ LOW_DYNAMIC_RANGE,           lowDynamicRangeString16       },
		{ 0x000000ff,                  chemSpecificErrorString07To00 },		// lowest priority
	};	 

	analyteFlags = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnC1AnalyteFlags;

	// check masked result flags in order of priority and set up returned ptr to comment text        

	for ( i = 0; i < NUM_RN_C1_MASKS; i++ )
	{
		if ( (analyteFlags & astmRnC1TextData[i].analyteFlagMask) != 0 )
		{
			return (char*) astmRnC1TextData[i].astmRnC1Text;
		}
	}	

	return (char*) chemUnusedErrorString08; 								// spare bit: no supposed to be set       
}


// ********************************************************************
// FUNCTION:
//
//		SetupAbortComment() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up comment text string for aborted error rotor results record
//		by comment type for ASCII frame buffer method 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SetupAbortComment(unsigned commentNumber, char *commentTextPtr)
{
	switch ( commentNumber )
	{
		case 1:		// for aborted Error Rotor Message O1C1 (run count & abort count)

			sprintf( commentTextPtr, "^%s^%s\\^%s^%s", astmRecord_m->o1C1RunCount, astmRunCountString, astmRecord_m->o1C1AbortCount, astmAbortCountString );      
			break;

		case 2:		// for aborted Error Rotor Message O1C2 (print count & flash count)
			sprintf( commentTextPtr, "^%s^%s\\^%s^%s", astmRecord_m->o1C2PrintCount, astmPrintCountString, astmRecord_m->o1C2FlashCount, astmFlashCountString );      
			break;

		case 3:		// for aborted Error Rotor Message O1C3 (DAC trim flags & offset error flags & system flags)
			sprintf( commentTextPtr, "^%4.4X^%s\\^%4.4X^%s\\^%4.4lX %4.4lX^%s", astmRecord_m->o1C3DacTrimFlags, astmDacTrimFlagsString, astmRecord_m->o1C3OffsetErrorFlags, astmOffsetErrFlagsString, astmRecord_m->o1C3SystemFlags >> 16, astmRecord_m->o1C3SystemFlags & 0xFFFF, astmSystemFlagsString );      
			break;

		case 4:		// for aborted Error Rotor Message O1C4 (bead check 1 flags and bead check 2 flags)
			sprintf( commentTextPtr, "^%4.4lX %4.4lX^%s\\^%4.4lX %4.4lX^%s", astmRecord_m->o1C4BeadCheck1Flags >> 16, astmRecord_m->o1C4BeadCheck1Flags & 0xFFFF, astmBeadCheck1String, astmRecord_m->o1C4BeadCheck2Flags >> 16, astmRecord_m->o1C4BeadCheck2Flags & 0xFFFF, astmBeadCheck2String );      
			break;

		case 5:		// for aborted Error Rotor Message O1C5	(empty cuvette flags & distribution check flags)
			sprintf( commentTextPtr, "^%4.4lX %4.4lX^%s\\^%4.4lX %4.4lX^%s", astmRecord_m->o1C5EmptyCuvFlags >> 16, astmRecord_m->o1C5EmptyCuvFlags & 0xFFFF, astmEmptyCuvFlagsString, astmRecord_m->o1C5DistCheckFlags >> 16, astmRecord_m->o1C5DistCheckFlags & 0xFFFF, astmDistCheckFlagsString );      
			break;

		case 6:		// for aborted Error Rotor Message O1C6	(quality control flags & offset SD error flags & wavelength CV flags)
			sprintf( commentTextPtr, "^%4.4X^%s\\^%4.4X^%s\\^%4.4X^%s", astmRecord_m->o1C6QcFlags, astmQcFlagsString, astmRecord_m->o1C6OffsetSdErrorFlags, astmOffsetSdErrString, astmRecord_m->o1C6DacWavelengthCvFlags, astmWavelengthCvString);      
			break;

		case 7:		// for aborted Error Rotor Message O1C7	(system error number & system error string - short text)
			sprintf( commentTextPtr, "^%4.4X^%s", astmRecord_m->o1C7SystemErrorNumber, astmRecord_m->o1C7SystemErrorString );      
			break;

		default:
			// not supported then no characters in tx        
			break;
	}
}


// ********************************************************************
// FUNCTION:
//
//		SetupNoQueryresultsCommentText() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up parameters for no query results comment ASTM frame buffer method  														
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SetupNoQueryresultsCommentText(char *commentTextPtr) 
{
	sprintf( commentTextPtr, "%s:%s:%s:%s", astmRxQueryStartId_m, astmRxQueryBeginDateTime_m, astmRxQueryEndDateTime_m, astmNoQueryResultString);      
}

void
SendTranslate (char * text)
{
    const unsigned char xlate[] = {
        0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA8,
        0x20, 0x20, 0x20, 0x20, 0x8E, 0x8F, 0x92, 0x20, 0x20, 0x90, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0xA5, 0x20, 0x20, 0x20, 0x20, 0x99, 0x20, 0x20, 0x20, 0x20, 0x20, 0x9A, 0x20, 0x20, 0xE1,
        0x85, 0xA0, 0x83, 0x20, 0x84, 0x86, 0x91, 0x87, 0x8A, 0x82, 0x88, 0x89, 0x8D, 0xA1, 0x8C, 0x8B,
        0x20, 0xA4, 0x95, 0xA2, 0x93, 0x20, 0x94, 0x20, 0x20, 0x97, 0xA3, 0x96, 0x81, 0x20, 0x20, 0x98
    };

    if ((unsigned char)(*text) >= 0xB0) {
        *text = xlate [(unsigned char)(*text) - 0xB0];
    }
   
}

// ********************************************************************
// FUNCTION:
//
//		ProcessTxEscMethod() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process method for formatting ASCII data in ASTM transmit message frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

unsigned char ProcessTxEscMethod(char escMethod, unsigned char checksum)
{
	char	*methodTextPtr;
	char	methodText[200];

	// init  methodText[] temp working buffer with working ptr to all '\0' null chars       

	memset(methodText, '\0', sizeof( methodText ));
	 
	methodTextPtr = methodText;


	// construct method text string based on method type for identified E ASTM 1394-97 field       

	// E ASTM 1394-97: 7. Message Header Record  (variable fields)       

	switch (escMethod)
	{
		case ASTM_TX_H_ANALYZER_TYPE:						// 7.1.5.2
			methodTextPtr = instrumentInfo_m->hAnalyzerName;					       
			break;

		case ASTM_TX_H_SOFTWARE_VERSION:					// 7.1.5.3       
			methodTextPtr = instrumentInfo_m->hSoftwareVersion;
			break;

		case ASTM_TX_H_SERIAL_NUMBER:						// 7.1.5.4       
			methodTextPtr = instrumentInfo_m->hSerialNumber;										       
			break;

		case ASTM_TX_H_PROCESSING_ID:						// 7.1.12       
			methodText[0] = instrumentInfo_m->hProcessingId;
			break;

		case ASTM_TX_H_DATE_TIME_OF_MESSAGE:				// 7.1.14       
			methodTextPtr = GetCurrentDateTime();						// date/time when message sent (H record)						       
			break;


	// E ASTM 1394-97: 8. Patient Information Record  (variable fields)       

		case ASTM_TX_P_SETUP:								// Abaxis implementation specific       
			astmTxOsequenceNumber_m = 1;								// init for first O record       
			astmTxCtype_m = '\0';										// init for no comment type set       
			break;

		case ASTM_TX_P_PATIENT_CONTROL_ID:					// 8.1.3       
			methodTextPtr = astmRecord_m->pPatientControlId;			// get id for patient or control       
			break;

		case ASTM_TX_NO_QUERY_RESULT_P_ID:					// 8.1.3       
			methodTextPtr = astmRxQueryStartId_m;						// loop back Q start ID       				
			break;

		case ASTM_TX_P_SAMPLE_ID:							// 8.1.4       
			methodTextPtr = astmRecord_m->pSampleId;					// get sample id        
			break;

		case ASTM_TX_P_ALTERNATE_ID:						// 8.1.5       
			methodTextPtr = astmRecord_m->pAlternateId;					// get alternate id        
			break;

		case ASTM_TX_P_BIRTHDATE:							// 8.1.8       
			methodTextPtr = astmRecord_m->pDateOfBirth;					// get birthdate, patient date of birth (DOB)        
			break;

		case ASTM_TX_P_PATIENT_SEX:							// 8.1.9       
			methodText[0] = astmRecord_m->pGender;						// get patient sex, gender						       
			break;

		case ASTM_TX_P_PATIENT_RACE:						// 8.1.10       
			methodTextPtr = astmRecord_m->pRace;						// get patient race-ethnic origin						       
			break;

		case ASTM_TX_P_PHONE_NUMBER:						// 8.1.13       
			methodTextPtr = astmRecord_m->pPhoneNumber;					// get patient phone number						       
			break;

		case ASTM_TX_P_SPECIAL1:							// 8.1.15       
			sprintf( methodTextPtr, "%s^%s^%s", astmRecord_m->pOwnerAdmissionId, astmRecord_m->pAge, astmRecord_m->pLastVaccination );       
			break;

		case ASTM_TX_P_SPECIAL2:							// 8.1.16       
		   	methodTextPtr = astmRecord_m->pReferenceRangeLabel;			// get reference range label, special 2						       
			break;

		case ASTM_TX_P_LOCATION:							// 8.1.16       
		   	methodTextPtr = astmRecord_m->pLocation;					// get location (e.g. kennel number, room number)						       
			break;


	//       E ASTM 1394-97: 9. Test Order Record  (variable fields)       

		case ASTM_TX_O_SEQ_NUM:								// 9.4.2       
			SetAstmSeqNum(astmTxOsequenceNumber_m, methodTextPtr);	
			break;

		case ASTM_TX_O_ROTOR_IQC_NAME:						// 9.4.5.4       
			if (astmTxOsequenceNumber_m == 1)							        
			{
				methodTextPtr = astmRecord_m->oRotorName;				// sample rotor disc name
			}
			else														       
			{
				methodTextPtr = (char *) iqcReportHeaderTextString;		// Quality Control Report header (in rotor name field)
			}
			break;

		case ASTM_TX_O_LOT_NUM:								// 9.4.5.4       
			methodTextPtr = astmRecord_m->oRotorLotNumber;				        
			break;

		case ASTM_TX_O_RUN_DATE_TIME:						// 9.4.7       
			methodTextPtr = astmRecord_m->oRunDateTime;					// date/time when rotor run						       
			break;

		case ASTM_TX_O_OPERATOR_ID:							// 9.4.11       
			methodTextPtr = astmRecord_m->oOperatorId;					// set collector ID (e.g. operator)       			
			break;

		case ASTM_TX_O_PHYSICIAN_ID:						// 9.4.17       
			methodTextPtr = astmRecord_m->oDoctorId;					// set alternate ID (e.g. physician)       			
			break;

		case ASTM_TX_O_REPORT_TYPE:							// 9.4.26       
			if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE)		// processing results query sends       	
			{
				methodText[0] = 'Q';									// set Q flag       			
			}
			else														// for all other results types       	
			{
				methodText[0] = 'F';									// set F flag       			
			}
			break;

		case ASTM_TX_ABORT_O_ROTOR_NAME_LOT:				// 9.4.5	(for aborted rotors)
			if (( astmRecord_m->oRotorName[0] != '\0' )	|| ( astmRecord_m->oRotorLotNumber[0] != '\0' ))
			{
				sprintf( methodTextPtr, "^^^%s: %s", astmRecord_m->oRotorName, astmRecord_m->oRotorLotNumber );       
			} 
			break;

		case ASTM_TX_O_SETUP_RESULTS:						// Abaxis implementation specific       
		case ASTM_TX_ABORT_O_SETUP:							       
			astmAnalyteIdx_m = 0;										// init index to analyte results info struct in AstmRecord_t      

			astmTxCtype_m = 'O';										// init for O record C record type
			astmTxCsequenceNumber_m = 1;								// init for first C record       

			astmTxRsequenceNumber_m = 1;								// init for first R record       
			break;


	// E ASTM 1394-97: 10. Result Record  (variable fields)       

		case ASTM_TX_R_SEQ_NUM:								// 10.1.2       
			SetAstmSeqNum(astmTxRsequenceNumber_m, methodTextPtr);	
			break;

		case ASTM_TX_R_LOINC_STRING:						// 10.1.3       
			if (astmTxOsequenceNumber_m == 1)							// set astm analyte name: O record 1         
			{
				methodTextPtr = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnUniversalTestId;
			}
			else														// set IQC level name:    O record 2       
			{
				methodTextPtr = (char *) iqcAstmLevelNameStrings[astmTxRsequenceNumber_m - 1];
			}
			break;

		case ASTM_TX_R_VALUE:								// 10.1.4       
			if (astmTxOsequenceNumber_m == 1)							// set analyte result value:   O record 1        
			{
				methodTextPtr = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnResultString;
			}
			else														// set IQC level result value: O record 2       
			{
				methodTextPtr = astmRecord_m->rIqcResultsStrings[astmTxRsequenceNumber_m - 1];
			}
			break;

		case ASTM_TX_R_UNITS:								// 10.1.5       
			if (astmTxOsequenceNumber_m == 1)							// set analyte result units: O record 1 only        
			{
				methodTextPtr = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnUnitsString;
			}

			// no units sent for Quality Control Report (IQC levels)        
			break;

		case ASTM_TX_R_REFERENCE_RANGE:						// 10.1.6       
			if (astmTxOsequenceNumber_m == 1)							// set astm analyte result reference range: O record 1        
			{
				if (( astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnLoRefRangeString[0] != '\0' ) && ( astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnHiRefRangeString[0] != '\0' ))
				{
					sprintf( methodTextPtr, "%s to %s", astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnLoRefRangeString, astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnHiRefRangeString );       
				}
			}
			else														// set astm IQC level limit range:          O record 2       
			{
				if (astmTxRsequenceNumber_m < 9)						// set LEVEL 1 limits       
				{
					methodTextPtr = (char *) iqcLimitStringLevel1;
				}
				else													// set LEVEL 2 PRE & wavelength limits       
				{
					methodTextPtr = (char *) iqcLimitStringLevel2;
				}
			}
			break;

		case ASTM_TX_R_ABNORMAL_FLAG:						// 10.1.7       
			if (astmTxOsequenceNumber_m == 1)							// process analyte result flags: O record 1 only       
			{
				methodText[0] = astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnAbnormalFlag;
			}

			// no flags processed for Quality Control Report (IQC levels)        
			break;

		case ASTM_TX_R_RESULT_STATUS:						// 10.1.9       
			if (astmTxOsequenceNumber_m == 1)							// process analyte result previously sent status: O record 1 only       
			{
				if ( astmRecord_m->astmResultRecordSent != 0 )
				{
					methodText[0] = 'R';	// this result was previously transmitted
				}
				else
				{
					methodText[0] = 'F';	// final results
				}
			}

			// no flags processed for Quality Control Report (IQC levels)        
			break;

		case ASTM_TX_R_SETUP_NEXT:							// Abaxis implementation specific       
			astmTxCtype_m = 'R';
			astmTxCsequenceNumber_m = 1;								// init for first C record       

			if (astmTxOsequenceNumber_m == 1)							// process C record: O record 1 only       
			{
				// process RnC1 record only if error condition flags for final result
				 
				if ((astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnC1ResultPrintFlags & ASTM_RESULT_FLAGS_COMMENT_MASK) == 0)
				{
					astmTxCsequenceNumber_m |= ASTM_TX_C_SKIP;
				}
			}
			break;


	// E ASTM 1394-97: 11. Comment Record  (variable fields)       

		case ASTM_TX_C_SEQ_NUM:								// 11.1.2       
			SetAstmSeqNum(astmTxCsequenceNumber_m, methodTextPtr);	
			break;

		case ASTM_TX_C_COMMENT_RESULT_RAW:					// 11.1.4       
			if (astmTxCtype_m == 'R')
			{
				sprintf( methodTextPtr, "%f", astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnC1ResultRaw );      
			}

			// not set for O record 1 C record 1, O record 1 C record 2, O record 2 C record 1, O record 2 C record 2       
			break;

		case ASTM_TX_C_COMMENT_CODE:						// 11.1.4       
			if (astmTxCtype_m == 'R')
			{
				SetupRnC1Code( methodTextPtr );
			}

			// not set for O record 1 C record 1, O record 1 C record 2, O record 2 C record 1, O record 2 C record 2       
			break;

		case ASTM_TX_C_COMMENT_TEXT:						// 11.1.4       
			if ((astmTxCtype_m == 'O') && (astmTxOsequenceNumber_m < 3))
			{
				// for O record 1 C record 1, O record 1 C record 2, O record 1 C record 3, O record 1 C record 4, O record 1 C record 5, O record 2 C record 1, O record 2 C record 2       

				methodTextPtr = SetupOrecordCtypeGtext( methodTextPtr );
			}
			else
			{
				if (astmTxCtype_m == 'R')
				{
					methodTextPtr = SetupRnC1Text();
				}
			}
			break;

		case ASTM_TX_ABORT_C_COMMENT_TEXT:					// 11.1.4
			// process rotor aborted error incr C record seq number

			SetupAbortComment(astmTxCsequenceNumber_m++, methodTextPtr);
			break;

		case ASTM_TX_NO_QUERY_RESULT_C_TEXT:				// 11.1.4       
			SetupNoQueryresultsCommentText( methodTextPtr );
			break;

		case ASTM_TX_1394_C_CODE:							// 11.1.4       
			methodText[0] = abaxisSendParam2_m;
			break;

		case ASTM_TX_1394_C_TEXT:							// 11.1.4       
            methodTextPtr = (char *) astm1394CommentStrings[(unsigned) abaxisSendParam1_m];
			break;

		case ASTM_TX_C_COMMENT_TYPE:						// 11.1.5       
			if ((astmTxCtype_m == 'O') && (astmTxOsequenceNumber_m < 3))
			{
				// for O record 1 C record 1, O record 1 C record 2, O record 2 C record 1, O record 2 C record 2       

				methodText[0] = 'G'; 
			}
			else
			{
				// for all other O records and R records       

				methodText[0] = 'I';
			}
		 
			astmTxCsequenceNumber_m++;									// incr C record seq number	      
			break;


	// E ASTM 1394-97: 13. Message Terminator Record  (variable fields)       

		case ASTM_TX_L_TERMINATION_CODE:					// 13.1.3       
			if (astmTxMsgType_m == ASTM_TX_NO_QUERY_RESULTS_MSG)
			{	 
				methodText[0] = 'I';									// set to "no information available from last query"       
			}
			else
			{
				methodText[0] = 'N';									// set to "normal termination"       
			}
		
			// 'F' & 'Q' not supported since Note 7 indicates that these flags terminate a request       
			// thus only flags understood to be handled by Abaxis instrument are supported 			         
			break;


	// E ASTM 1394-97: 15. Message Manufacturer Type 1 (Q loop back) Record  (variable fields)       

		case ASTM_TX_M1_FIELD3:								// 15.1.3       
			methodTextPtr = astmRxQueryStartId_m;						// loop back Q start ID       				
			break;


		case ASTM_TX_M1_FIELD4:								// 15.1.4       
			methodTextPtr = astmRxQueryBeginDateTime_m;					// loop back Q begin date/time       				
			break;


		case ASTM_TX_M1_FIELD5:								// 15.1.5       
			methodTextPtr = astmRxQueryEndDateTime_m;					// loop back Q end date/time       				
			break;

         // E ASTM 1394-97: 15. Message Manufacturer Type Record(Requested Abaxis Analyzer Status)   (variable fields)

        case ASTM_TX_M_RECORD_IDENTIFIER:                   // 15.1.3
            methodTextPtr = abaxisSendParam3_m;                         // Abaxis record identifier = "AS"
            break;

         // E ASTM 1394-97: 15. Message Manufacturer Type Record(Requested Abaxis AS: Analyzer Status or PO: Pending Orders)   (variable fields)    
        case ASTM_TX_M_SEQ_NUM:                             // 15.1.2
            sprintf( methodTextPtr, "%d", astmTxMsequenceNumber_m );    // Abaxis manufacturer record sequence number
            break;


        case ASTM_TX_M_ANALYZER_STATUS:                     // 15.1.4
            methodText[0] = abaxisSendParam2_m;                         // Abaxis analyzer status = '0','1','2'
            break;

        case ASTM_TX_M_ORDER_DATE_TIME:                     // 15.1.4
            methodTextPtr = astmOrders_m.mPendingOrders[ astmTxMsequenceNumber_m - 1 ].mnOrderDateTime;
            break;

        case ASTM_TX_M_ORDER_PATIENT_CONTROL_ID:            // 15.1.5
            methodTextPtr = astmOrders_m.mPendingOrders[ astmTxMsequenceNumber_m - 1 ].mnOrderPatientControlId;
            break;

        case ASTM_TX_M_ORDER_ROTOR_NUMBER:                  // 15.1.6
            methodTextPtr = astmOrders_m.mPendingOrders[ astmTxMsequenceNumber_m - 1 ].mnOrderRotorNumber;
            break;

        case ASTM_TX_M_ORDER_LABORATORY_FIELD_1:            // 15.1.7
            methodTextPtr = astmOrders_m.mPendingOrders[ astmTxMsequenceNumber_m - 1 ].mnOrderLaboratoryField1;
            break;

    // E ASTM 1394-97: Order Data Invalid P Record / O Record / C Record methods

        case ASTM_TX_INVALID_P_PATIENT_CONTROL_ID:          // 8.1.3
            methodTextPtr = abaxisSendParam5_m.pPatientControlId;
            break;

        case ASTM_TX_INVALID_P_SAMPLE_ID:                   // 8.1.4
            methodTextPtr = abaxisSendParam5_m.pSampleId;
            break;

        case ASTM_TX_INVALID_P_ALTERNATE_ID:                // 8.1.5
            methodTextPtr = abaxisSendParam5_m.pAlternateId;
            break;

        case ASTM_TX_INVALID_P_BIRTHDATE:                   // 8.1.8
            methodTextPtr = abaxisSendParam5_m.pBirthdate;
            break;

        case ASTM_TX_INVALID_P_PATIENT_SEX:                 // 8.1.9
            methodTextPtr = abaxisSendParam5_m.pPatientSex;
            break;

        case ASTM_TX_INVALID_P_PATIENT_RACE:                // 8.1.10
            methodTextPtr = abaxisSendParam5_m.pPatientRace;
            break;

        case ASTM_TX_INVALID_P_PHONE_NUMBER:                // 8.1.13
            methodTextPtr = abaxisSendParam5_m.pPatientPhoneNumber;
            break;

        case ASTM_TX_INVALID_P_SPECIAL1:                    // 8.1.15
            methodTextPtr = abaxisSendParam5_m.pPatientSpecial1;
            break;

        case ASTM_TX_INVALID_P_SPECIAL2:                    // 8.1.16
            methodTextPtr = abaxisSendParam5_m.pPatientSpecial2;
            break;

        case ASTM_TX_INVALID_P_LOCATION:                    // 8.1.16
            methodTextPtr = abaxisSendParam5_m.pPatientLocation;
            break;

        case ASTM_TX_INVALID_O_ROTOR_NUMBER:                // 9.4.5
            methodTextPtr = abaxisSendParam5_m.oRotorNumber;
            break;

        case ASTM_TX_INVALID_O_ORDER_DATE_TIME:             // 9.4.7
            methodTextPtr = abaxisSendParam5_m.oOrderDateTime;
            break;

        case ASTM_TX_INVALID_O_ACTION_CODE:                 // 9.4.12
            methodTextPtr = abaxisSendParam5_m.oActionCode;
            break;
        case ASTM_TX_INVALID_O_PHYSICIAN_ID:                // 9.4.17
            methodTextPtr = abaxisSendParam5_m.oPhysicianId;
            break;

        case ASTM_TX_INVALID_O_REPORT_TYPE:                 // 9.4.26
            methodText[0] = 'X';
            break;

        case ASTM_TX_INVALID_C_CODE:                        // 11.1.4
            sprintf( methodTextPtr, "%4.4X %4.4X %4.4X %4.4X",
                                                              (unsigned)( ( abaxisSendParam1_m >> 48 ) & 0xFFFF ),
                                                              (unsigned)( ( abaxisSendParam1_m >> 32 ) & 0xFFFF ),
                                                              (unsigned)( ( abaxisSendParam1_m >> 16 ) & 0xFFFF ),
                                                              (unsigned)  ( abaxisSendParam1_m         & 0xFFFF ) );
            break;

        case ASTM_TX_INVALID_C_TEXT:                        // 11.1.4
            methodTextPtr = (char *) astmOrderDataInvalidString;        // component3 ESC order data invalid free text
            break;


    // E ASTM 1394-97: Order Cancellation P Record / O Record / C Record methods

        case ASTM_TX_CANCEL_P_PATIENT_CONTROL_ID:           // 8.1.3
            methodTextPtr = abaxisSendParam3_m;
            break;

        case ASTM_TX_CANCEL_O_ROTOR_NUMBER:                 // 9.4.5
            sprintf( methodTextPtr, "%d", (unsigned) abaxisSendParam1_m );
            break;

        case ASTM_TX_CANCEL_O_ORDER_DATE_TIME:              // 9.4.7
            methodTextPtr = abaxisSendParam4_m;
            break;

        case ASTM_TX_CANCEL_O_REPORT_TYPE:                  // 9.4.26
            switch ( abaxisSendParam2_m )
            {
                case 'X':
                case 'I':
                case 'Y':
                    methodText[0] = abaxisSendParam2_m;
                    break;
                default:
                    break;
            }
            break;
        case ASTM_TX_CANCEL_C_CODE:                         // 11.1.4
            methodText[0] = abaxisSendParam2_m;                         // component2 ESC order cancellation code - character
            break;

        case ASTM_TX_CANCEL_C_TEXT:                         // 11.1.4
            switch ( abaxisSendParam2_m )                               // component3 ESC order cancellation free text
            {
                case 'X':
                    methodTextPtr = (char *) astmOrderCancellationXString;
                    break;
                case 'I':
                    methodTextPtr = (char *) astmOrderCancellationIString;
                    break;
                case 'Y':
                    methodTextPtr = (char *) astmOrderCancellationYString;
                    break;
                default:
                    break;
            }
            break;

	// ASTM Method not supported

		default:
			// not supported then no characters in tx        
			break;

	}	// end switch


	// suppress leading & trailing blanks in methodTextPtr string - result in methodText[]       

	methodTextPtr = SuppressBlanksFromAstmMethodString(methodTextPtr, methodText);

	// set method text in astm tx buffer while updating checksum       
	while(*methodTextPtr != '\0')		
	{

		checksum = PutAstmTxCharAndChecksum(*methodTextPtr++, checksum);
	}

	return (checksum);
}


// ********************************************************************
// FUNCTION:
//
//		NextAstmResultString() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get pointer to next ASTM result ASCII string  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char NextAstmResultString(void)
{
	return astmRecord_m->rAnalyteResults[astmAnalyteIdx_m].rnResultString[0];
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRepeatResultCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to process method to repeat frame for ASTM transmit message records  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char *ProcessRepeatResultCheck(void)
{
	if (astmTxOsequenceNumber_m == 1)								// O record 1        
	{
		astmAnalyteIdx_m++;											// incr to next chem analyte results index

		// check if done with analyte R records since next result is NULL string or end of results

		if ( (NextAstmResultString() == '\0') || (astmAnalyteIdx_m >= MAX_RESULTS) )						        
		{
			astmTxOsequenceNumber_m++;								// set for next O record when R records all done       

			abaxisTxMsgDefPtr_m++;									// skip past this record       
		}
		else														// not done with R records yet       
		{
			abaxisTxMsgDefPtr_m--;									// back past C record       
			abaxisTxMsgDefPtr_m--;									// and for next R record       
		}
	}
	else															// O record 2       
	{
		if (astmTxRsequenceNumber_m == last_astm_tx_iqc_r_record_m)	// done with all QC R records       	
		{
			abaxisTxMsgDefPtr_m++;									// skip past this record       
		}
		else														// not done with R records yet       
		{
			abaxisTxMsgDefPtr_m--;									// decr back for next QC R record       
		}
	}		

	astmTxRsequenceNumber_m++;										// incr for next R record seq num and/or QC index       

	return abaxisTxMsgDefPtr_m;							
}



// ********************************************************************
// FUNCTION:
//
//      NextAstmMpoString()
// _____________________________________________________________________
// DESCRIPTION:
//
//      Get pointer to next ASTM M n PO ASCII string
// _____________________________________________________________________
// PARAMETERS:
//
//      none
// ____________________________________________________________________
// RETURNED:
//
//      none
//
// ********************************************************************/

char NextAstmMpoString(void)
{
    return astmOrders_m.mPendingOrders[ astmTxMsequenceNumber_m ].mnOrderDateTime[ 0 ];
}


// ********************************************************************
// FUNCTION:
//
//      ProcessRepeatMpoCheck()
// _____________________________________________________________________
// DESCRIPTION:
//
//      Check to process method to repeat frame for ASTM transmit message manufacturing M n PO records
// _____________________________________________________________________
// PARAMETERS:
//
//      none
// ____________________________________________________________________
// RETURNED:
//
//      none
//
// ********************************************************************/

char *ProcessRepeatMpoCheck(void)
{
    if ( (NextAstmMpoString() == '\0') || (astmTxMsequenceNumber_m >= MAX_QUEUED_ORDERS ) )
    {
        abaxisTxMsgDefPtr_m++;                                  // skip past this record
    }
    else                                                        // not done with R records yet
    {
        abaxisTxMsgDefPtr_m--;                                  // back for next M PO record
    }

    astmTxMsequenceNumber_m++;                                  // incr for next M n PO record sequence number

    return abaxisTxMsgDefPtr_m;
}

// ********************************************************************
// FUNCTION:
//
//		ProcessSkipCommentCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to process method to skip comment for ASTM transmit message 'R' records  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char *ProcessSkipCommentCheck(void)
{
	if (astmTxCsequenceNumber_m & ASTM_TX_C_SKIP)
	{
		abaxisTxMsgDefPtr_m++;						// skip past this C record       
	}		

	return abaxisTxMsgDefPtr_m;							
}



// ********************************************************************
// FUNCTION:
//
//		SetupTxMessageFrames() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up an ASTM frame buffer for a record in an Abaxis transmit message  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char SetupTxMessageFrames(void)
{
	char			*frameTemplatePtr;
	char			frameTemplateChar;
	char			checksumString[3];
	unsigned char	checksum;


	// process Abaxis message record flags       

	if (*abaxisTxMsgDefPtr_m == ASTM_TX_MESSAGE_DONE)							// no more record templates to set up for this abaxis message       
	{
		return (ABAXIS_ASTM_TX_MSG_TRANSMITTED);
	}

	if (*abaxisTxMsgDefPtr_m == ASTM_TX_COMMENT)								// handle C record skip sequencing       
	{
		abaxisTxMsgDefPtr_m = ProcessSkipCommentCheck();
	}

	if (*abaxisTxMsgDefPtr_m == ASTM_TX_RESULTS_REPEAT)							// handle variable R record sequencing       
	{
		abaxisTxMsgDefPtr_m = ProcessRepeatResultCheck(); 
	}
    
    if (*abaxisTxMsgDefPtr_m == ASTM_TX_MANUFACTURER_PO_REPEAT)                 // handle variable M n PO record sequencing
    {
        abaxisTxMsgDefPtr_m = ProcessRepeatMpoCheck();
    }

	//  get pointer to frame template to set up for this record 
	//	and increment pointer to message definition index for next frame setup/check       

	frameTemplatePtr = (char *) astmTxRecordTemplatePtrs_m[ (int)*abaxisTxMsgDefPtr_m++ ];

	
	// Setup next *** ASTM E 1381-02 *** frame in astm transmit buffer       
	
	// First, set up *** ASTM E 1381-02 *** header characters in astm transmit buffer       

	PutAstmTxChar(STX);															// set STX as first char in astm frame       


	checksum = 0;																// init checksum &       	


	// set ASCII FN as next char in astm frame and include in checksum

	checksum = PutAstmTxCharAndChecksum( (char) (astmTxFrameNumber_m + '0'), checksum);	       
	 

	// Next, set up *** ASTM E 1394-97 *** record "text" data in astm transmit buffer using template data       

	while(*frameTemplatePtr != '\0')											// process astm "text" till end of template       
	{
		frameTemplateChar = *frameTemplatePtr++;								// get this "text" char to process and incr pointer to next char       

		if (frameTemplateChar == ESC)											// process ESC method ?        
		{
			//  Process ESC method and set "text" in astm tx buffer and incr ptr to next char in template       

			checksum = ProcessTxEscMethod(*frameTemplatePtr++, checksum);
		}
		else																	// not an ESC method, just set this text char is astm transmit buffer       
		{
			checksum = PutAstmTxCharAndChecksum(frameTemplateChar, checksum);
		}
	}

	checksum = PutAstmTxCharAndChecksum('\r', checksum);						// set CR as last "text" char in astm frame       


	//       Last, set up *** ASTM E 1381-02 *** frame termination characters in astm transmit buffer       

	checksum = PutAstmTxCharAndChecksum(ETX,checksum);							// set ETX (never ETB) in astm frame       

	sprintf(checksumString, "%2.2X", checksum);									// convert checksum for astm frame       
	PutAstmTxChar(checksumString[0]);											// set C1 (checksum MSN) in astm frame       
	PutAstmTxChar(checksumString[1]);											// set C2 (checksum LSN) in astm frame       

	PutAstmTxChar('\r');														// set CR as second to last char in astm frame       

	PutAstmTxChar('\n');														// set STX as last char in astm frame       


	// Now, signal not done setting up all frames (records) for this abaxis message and exit       

	return (ABAXIS_ASTM_TX_MSG_NOT_DONE);		
}


//      	
//
//	functions follow that support receiving ASTM 1394-97 rx frames:
//
//		- receive rx char at saved ptr to frame
//		- check for valid rx char and compute running checksum for frame
//		- get ptr to template for record type with max fields expected for record
//		- process rx extract method for rx data from frame while maintaining running checksum
//		- set up abaxis rx msg for ACKed frames (from H record to L record)
//		- ASTM 1394-97 rx frame overall processing:
//			ProcessRxMessageFrames() -- called from ProcessAstmState() state machine


// ********************************************************************
// FUNCTION:
//
//		ReceivedAstmFrameChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Return ASCII char from ASTM received frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char ReceivedAstmFrameChar(void)
{
	return astmRxBuffer_m[astmRxBufferIdx_m++] & 0x7f;
}


// ********************************************************************
// FUNCTION:
//
//		GetValidRxCharAndChecksum() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get a valid char from the ASTM received frame buffer and include
//		in received running checksum value  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char GetValidRxCharAndChecksum(unsigned char *checksumPtr)
{
	char	rxChar;

	// check to see if valid ASTM 1394-97 char				      
	//       														      
	// ASTM 1394-97 specifies char allowed in record &		      
	//      is superset to ASTM 1381-02						      

	rxChar = ReceivedAstmFrameChar();

	if ( (rxChar != CR) && ((rxChar < ' ') || (rxChar >= DEL)) )
	{
		return (char) NOT_VALID_ASTM_CHAR;
	}
	 
	// compute running checksum for valid chars       

	*checksumPtr += rxChar;

	return (rxChar);
}


// ********************************************************************
// FUNCTION:
//
//		GetRxFrameTemplate() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Find and return a valid ASTM received frame template for 
//		received frame processing  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char	*GetRxFrameTemplate(char recordType, char *maxFieldsPtr)
{
	int		i;
	char	*recordTemplatePtr;

	// scan list of record template ptrs for current astm record type and	      
	// set maximum fields for current record type								      

	i = 0;

	while (i < NUM_RX_FRAME_TEMPLATES)
	{
		recordTemplatePtr = (char*) astmRxRecordTemplatePtrs_m[i++];

		*maxFieldsPtr = *recordTemplatePtr++;				// set max fields this record       

		if (*recordTemplatePtr++ == recordType)
		{
			return (recordTemplatePtr);						// found so return ptr to template for this record       
		}
	}

	return (NULL);											// exit with NULL ptr for no record of this type found in list       												
}


// ********************************************************************
// FUNCTION:
//
//		ExtractRxFrameFields() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Extract and verify ASCII fields for ASTM received frame message  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char ExtractRxFrameFields(char method, char validRxChar, unsigned char *checksumPtr)
{
	char	fieldIdx;
	char	fieldString[MAX_ASTM_RX_FIELD];
	char	*fieldStringPtr;
	char	maxFieldSize;

	// extract/handle text string based on method for identified E ASTM 1394-97 field       
	// first handle single char methods (uses passed validRxChar only and exits) 

	// generic method: do exact match to current "field terminator"	for non Header records       

	switch (method)
	{
		case ASTM_RX_FIELD_DELIMITER: 				
			if (validRxChar != astmRxFieldDelimiter_m)
			{
				// bad field delimiter so set up for tx of abaxis comment message       

				astm1394ErrorType_m = ASTM_1394_RX_DELIMITER_MISMATCH;	
			}
			break;

		// E ASTM 1394-97: 7. Message Header Record  (fixed fields)       

		case ASTM_RX_H_FIELD_DELIMITER:							// 7.1.2       			
			astmRxFieldDelimiter_m =  validRxChar;
			break;

		case ASTM_RX_H_REPEAT_DELIMITER:						// 7.1.2       
			astmRxRepeatDelimiter_m =  validRxChar;
			break;

		case ASTM_RX_H_COMPONENT_DELIMITER:						// 7.1.2       
			astmRxComponentDelimiter_m =  validRxChar;
			break;

		case ASTM_RX_H_ESCAPE_DELIMITER:						// 7.1.2       
			astmRxEscapeDelimiter_m =  validRxChar;
			break;

		//	 next handle all other methods: extract strings into temp buffer for processing
		//	 of abaxis astm message specific parameters 

		default:
			// init temp field to all null chars        

			memset(fieldString, 0, sizeof( fieldString ));

			// extract char data for field until field delimiter or CR (aka last field delimiter)       

			fieldIdx = 0;													// init index to temp field storage       

			while ((validRxChar != astmRxFieldDelimiter_m) && (validRxChar != CR))  
			{
				// clamp field char store at (max - 1) for char not (field delim or CR)       

				if (fieldIdx < (MAX_ASTM_RX_FIELD - 1)) 	
				{
					fieldString[ (int)fieldIdx++ ] = validRxChar;	// valid char so store in temp field storage       	
				}

				validRxChar = GetValidRxCharAndChecksum(checksumPtr); 		// get next char       

				if (validRxChar == (char)NOT_VALID_ASTM_CHAR) 
				{
					return (NAK);											// in-valid astm char in rx frame text so NAK       
				}
			}

			// check if field size extracted > max field size allowed       

			maxFieldSize = maxAstmRxFieldSize_m[ (int)method ];

			if (fieldIdx > maxFieldSize)
			{
				// field size too big for abaxis so set up for tx of abaxis comment message       

				astm1394ErrorType_m = ASTM_1394_RX_FIELD_SIZE_TOO_BIG;	
			}

			// init field ptr to beginning of field string for use by methods       

			fieldStringPtr = fieldString;
			
            // process an rx method for an identified record field

			switch (method)
			{

                 // E ASTM 1394-97: 7. Message Header Record  (variable fields)

                case ASTM_RX_H_MESSAGE_CONTROL_ID:              // 7.1.3
                    strncpy(astmInput_m->hMessageControlId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_H_ACCESS_PASSWORD:                 // 7.1.4
                    strncpy(astmInput_m->hAccessPassword, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_H_SENDER_NAME_OR_ID:               // 7.1.5
                    strncpy(astmInput_m->hSenderNameOrId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_H_RECEIVER_ID:                     // 7.1.10
                    strncpy(astmInput_m->hReceiverId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_H_MESSAGE_DATE_TIME:               // 7.1.14
                    strncpy(astmInput_m->hMessageDateAndTime, fieldStringPtr, maxFieldSize);
                    break;

                // E ASTM 1394-97: 12. Request Information Record  (variable fields)

				case ASTM_RX_Q_SEQUENCE_NUMBER:					// 12.1.2       
					if (*fieldStringPtr != '1')								// should always be '1'       
					{
						// Q record sequence number not 1 so set up for tx of abaxis comment message       

						astm1394ErrorType_m = ASTM_1394_RX_Q_SEQUENCE_NUM_BAD;	
					}
					break;

				case ASTM_RX_Q_STARTING_RANGE_ID:				// 12.1.3       		  
					strncpy(astmRxQueryStartId_m, fieldStringPtr, maxFieldSize);
					break;

				case ASTM_RX_Q_BEGIN_RESULTS_DATE_TIME:			// 12.1.7       
					strncpy(astmRxQueryBeginDateTime_m, fieldStringPtr, maxFieldSize);
					break;

				case ASTM_RX_Q_END_RESULTS_DATE_TIME:			// 12.1.8       
					strncpy(astmRxQueryEndDateTime_m, fieldStringPtr, maxFieldSize);
					break;

			// E ASTM 1394-97: 13. Message Terminator Record        

				case ASTM_RX_L_SEQUENCE_NUMBER:					// 13.1.2       
					if (*fieldStringPtr != '1')								// should always be '1'       
					{
						// L record sequence number not 1 so set up for tx of abaxis comment message       

						astm1394ErrorType_m = ASTM_1394_RX_L_SEQUENCE_NUM_BAD;	
					}
					break;
                            // E ASTM 1394-97: 8. Patient Information Record  (variable fields)

                case ASTM_RX_P_SEQUENCE_NUMBER:                 // 8.1.2
                    strncpy(astmInput_m->pSequenceNumber, fieldStringPtr, maxFieldSize);
                    if (*fieldStringPtr != '1')                             // should always be '1'
                    {
                        // P record sequence number not 1 so set up for tx of abaxis comment message

                        astm1394ErrorType_m = ASTM_1394_RX_P_SEQUENCE_NUM_BAD;
                    }
                    break;

                case ASTM_RX_P_PATIENT_CONTROL_ID:              // 8.1.3
                    strncpy(astmInput_m->pPatientControlId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_SAMPLE_ID:                       // 8.1.4
                    strncpy(astmInput_m->pSampleId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_ALTERNATE_ID:                    // 8.1.5
                    strncpy(astmInput_m->pAlternateId, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_BIRTHDATE:                       // 8.1.8
                    strncpy(astmInput_m->pBirthdate, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_SEX:                     // 8.1.9
                    strncpy(astmInput_m->pPatientSex, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_RACE:                    // 8.1.10
                    strncpy(astmInput_m->pPatientRace, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_PHONE_NUMBER:            // 8.1.13
                    strncpy(astmInput_m->pPatientPhoneNumber, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_SPECIAL1:                // 8.1.15
                    strncpy(astmInput_m->pPatientSpecial1, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_SPECIAL2:                // 8.1.16
                    strncpy(astmInput_m->pPatientSpecial2, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_P_PATIENT_LOCATION:                // 8.1.26
                    strncpy(astmInput_m->pPatientLocation, fieldStringPtr, maxFieldSize);
                    break;


            // E ASTM 1394-97: 9. Test Order Record  (variable fields)

                case ASTM_RX_O_SEQUENCE_NUMBER:                 // 9.4.2
                    strncpy(astmInput_m->oSequenceNumber, fieldStringPtr, maxFieldSize);
                    if (*fieldStringPtr != '1')                             // should always be '1'
                    {
                        // O record sequence number not 1 so set up for tx of abaxis comment message

                        astm1394ErrorType_m = ASTM_1394_RX_O_SEQUENCE_NUM_BAD;
                    }
                    break;

                case ASTM_RX_O_ROTOR_NUMBER:                    // 9.4.5
                    strncpy(astmInput_m->oRotorNumber, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_O_ORDER_DATE_TIME:                 // 9.4.7
                    strncpy(astmInput_m->oOrderDateTime, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_O_ACTION_CODE:                     // 9.4.12
                    strncpy(astmInput_m->oActionCode, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_O_PHYSICIAN_ID:                    // 9.4.17
                    strncpy(astmInput_m->oPhysicianId, fieldStringPtr, maxFieldSize);
                    break;
            
                 case ASTM_RX_O_LABORATORY_FIELD_1:              // 9.4.21
                    strncpy(astmInput_m->oLaboratoryField1, fieldStringPtr, maxFieldSize);
                    break;

                case ASTM_RX_O_REPORT_TYPE:                     // 9.4.26
                    strncpy(astmInput_m->oReportType, fieldStringPtr, maxFieldSize);
                    break;

            // E ASTM 1394-97: 15. Manufacturer Information Record  (variable fields)

                case ASTM_RX_M_SEQUENCE_NUMBER:                 // 15.1.2
                    strncpy(astmInput_m->mSequenceNumber, fieldStringPtr, maxFieldSize);
                    if (*fieldStringPtr != '1')                             // should always be '1'
                    {
                        // M record sequence number not 1 so set up for tx of abaxis comment message

                        astm1394ErrorType_m = ASTM_1394_RX_M_SEQUENCE_NUM_BAD;
                    }
                    break;

                case ASTM_RX_M_RECORD_IDENTIFIER:               // 15.1.3 - Abaxis defined
                    strncpy(astmInput_m->mRecordIdentifier, fieldStringPtr, maxFieldSize);
                    break;
		
			//	ASTM_RX_EMPTY_FIELD generic method and all "UNDEFINED" methods
			//		fall through here to ignore any data in field &
			//		to return current received field delimiter only
			 
				default:
					// not supported rx fields        
					break;
			}
			break;		
	}

	return validRxChar;														// return validRxChar passed or last validRxChar extracted       
}


// ********************************************************************
// FUNCTION:
//
//		SetAstmRxMsgRecordTypes() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set for verified receipt of ASTM message frame record type
//		(e.g. 'H', 'Q', 'L', etc.)  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      
void SetAstmRxMsgRecordTypes(char recordType)
{
	if (abaxisRxMsgRecordIdx_m < MAX_RX_MSG_RECORDS)
	{
		abaxisRxMsgRecords_m[abaxisRxMsgRecordIdx_m++] = recordType; 	// set record type in msg record array       
	}
}


// ********************************************************************
// FUNCTION:
//
//		SetupAbaxisAstmRxMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up valid receipt of Abaxis ASTM supported message for processing  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void SetupAbaxisAstmRxMsg(char recordType)
{
	int	i;

	// if found astm 1394 issue, set up for tx of (H C1 L) message for first issue found       

	if ((astm1394ErrorType_m != NO_ERRORS) && (!astm1394ErrorDuringRxMsg_m))
	{
		astm1394ErrorDuringRxMsg_m = true;							// set for astm 1394 issue during message reception       

		// set up for tx of (H C1 L) comment message with error type and record type parameters       

		abaxisAstmHclMessageType_m = astm1394ErrorType_m;
		astm1394ErrorRecordType_m = recordType;	
	}			

	// if got good astm 1381-02 header, flag for next records and to use delimiters from received frame       

	if ((recordType == 'H') && (!gotAstmRxHeader_m))		
	{
		gotAstmRxHeader_m = true;									// K to process more records for abaxis msg       

		memset(abaxisRxMsgRecords_m, '\0', sizeof( abaxisRxMsgRecords_m ));

		abaxisRxMsgRecordIdx_m = 0;

		astmRxQueryStartId_m[0] = '\0';								// make Q message start id string empty       
		astmRxQueryBeginDateTime_m[0] = '\0';						// make Q message begin date/time string empty       
		astmRxQueryEndDateTime_m[0] = '\0';							// make Q message end date/time string empty       

		SetAstmRxMsgRecordTypes( 'H' );								// set first record in msg array
	}

	// for valid astm 1381-02 records (between H & L records), set type of record in abaxis astm msg record coalation array       

	else if ((recordType != 'H') && (recordType != 'L') && gotAstmRxHeader_m)	
	{
		SetAstmRxMsgRecordTypes( recordType );						// set middle records in msg array
	}

	// if got good astm 1391-02 terminator, check for processing of allowed abaxis astm rx message       

	else if ((recordType == 'L') && gotAstmRxHeader_m)	
	{
		gotAstmRxHeader_m = false;									//  reset to make sure get a header record for next abaxis msg      

		SetAstmRxMsgRecordTypes( 'L' );								// set last record in msg array

		// check if astm 1394 issues with any records prior to or during reception of abaxis terminated message       

		if (astm1394ErrorDuringRxMsg_m)
		{
			QueueAstmTxMsg( ABAXIS_SEND_1394_COMMENT_MSG_TYPE, abaxisAstmHclMessageType_m, astm1394ErrorRecordType_m );			       
		}
			
		// scan to see if valid abaxis rx msg and set to process if so       

		i = 0;														// init idx to abaxis defined msg records ptr       

		while (i < NUM_ABAXIS_ASTM_RX_MSGS)							// scan til checked all valid abaxis rx msgs       
		{
			if ( strcmp( abaxisAstmRxMsgPtrs_m[i++], abaxisRxMsgRecords_m) == 0 )			       
			{
				// got an abaxis rx msg match

				abaxisAstmRxMsgToProcess_m = i;						// set abaxis rx msg type to process 			      
				astmRxBusy_m = true;								// set rx now busy til done processing message 	      

				astm1394ErrorDuringRxMsg_m = false;					// re-init for next abaxis message       
				return;												// and exit with found abaxis supported rx msg       
			}
		}

		// not a supported abaxis rx message so tx issue only if no other errors detected       

		if (!astm1394ErrorDuringRxMsg_m)
		{
			// set up for tx of H C1 L comment message with error type and record type parameters       

			QueueAstmTxMsg( ABAXIS_SEND_1394_COMMENT_MSG_TYPE, ASTM_1394_RX_MESSAGE_NOT_SUPPORTED );	       
		}

		astm1394ErrorDuringRxMsg_m = false;							// re-init for next abaxis message       
	}

	// for all other valid astm 1381-02 records (not betweeen H & L records), if no other errors detected set up for tx for out of sequence       

	else if (!astm1394ErrorDuringRxMsg_m)
	{
		astm1394ErrorDuringRxMsg_m = true;							// set for astm 1394 issue during message reception       

		// set up for tx of H C1 L comment message with error type and record type parameters       

		abaxisAstmHclMessageType_m = ASTM_1394_RX_RECORD_OUT_OF_SEQUENCE;
		astm1394ErrorRecordType_m = recordType;				
	}
}


// ********************************************************************
// FUNCTION:
//
//		AsciiToHex() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Returns hex value for ASCII char  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

unsigned char AsciiToHex( char ascii )
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


// ********************************************************************
// FUNCTION:
//
//		ProcessRxMessageFrames() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process ASTM message frames for inclusion in Abaxis supported
//		ASTM messages and return verified ACK/NAK status of frame  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char ProcessRxMessageFrames(void)
{
	unsigned char	checksum;
	unsigned char	*checksumPtr;

	char	recordType;
	char	maxFields;
	char	*maxFieldsPtr;
	char	numFields;
	char	rxFrameChar;
	char	*templatePtr;
	char	templateTerminator = '\0';

	// init abaxis astm comment message type for no 1394-97 record data error type       

	astm1394ErrorType_m = NO_ERRORS;
	astmRxBufferIdx_m = 0;

	// expect STX as first received frame char per ASTM 1381-02	      

	if (ReceivedAstmFrameChar() != STX)							
	{
		return (NAK);				// 1st char in rx buffer is not STX so NAK       
	}

	// init for rx frame number as first char in running checksum computed & 				      
	// check that received rx frame number matches expected frame number per ASTM 1381-02 	      		

	checksumPtr = &checksum;		
	*checksumPtr = 0;				// init checksum for this rx frame       

	if ( GetValidRxCharAndChecksum( checksumPtr ) != ( (char) (astmRxFrameNumber_m + '0') ) )
	{
		return (NAK);				// received frame number does not match expected frame number       
	}

	// get record type per ASTM 1394-97 (e.g. H,Q,L,M,P,O,R,C,S) for this frame (start frame record text)       

	maxFieldsPtr = &maxFields;		// set up to get max number fields in this frame record type       

	recordType = GetValidRxCharAndChecksum(checksumPtr);		

    // if there's an error and recordType is one of the delimiters, change recordType to null so
    // the replay message isn't corrupted

    if ( ( recordType == astmRxFieldDelimiter_m ) ||
         ( recordType == astmRxRepeatDelimiter_m ) ||
         ( recordType == astmRxComponentDelimiter_m ) ||
         ( recordType == astmRxEscapeDelimiter_m ) )
    {
        recordType = 0;
    }

	templatePtr = GetRxFrameTemplate(recordType, maxFieldsPtr);

	if (templatePtr == NULL)
	{
		// not a valid astm 1394 record type so set up for tx of abaxis comment message       

		templatePtr = &templateTerminator;
		maxFields = MAX_INVALID_RX_FIELDS;       

		astm1394ErrorType_m = ASTM_1394_RX_INVALID_RECORD_TYPE;	
	}

	// init num_fields to compare to max_fields for possible NAK for this record frame	      

	numFields = 0;						

	//  process frame text for this record type until CR (end of frame text)			      
	//  - if template at '\0' then ignore (do not check) rest of char thru 1st <CR>	      
	//    except for incr of num_fields if "field delimiter" found			      								
	//  - extract record data using special method sequences defined in template	      
	//    num_fields incremented based on special method processing for field	      

	do
	{
		rxFrameChar = GetValidRxCharAndChecksum(checksumPtr);

		if (rxFrameChar == (char)NOT_VALID_ASTM_CHAR)
		{
			return (NAK);			// in-valid astm char in rx frame text so NAK       
		}

		// continue processing only if already got a header frame or if this frame			      
		//   is a header record so that delimiters are set up for rx frames from host	      
		//   otherwise ignore all chars till CR and flush but still checksum chars		      
		//   and check for num_fields													      

		if (gotAstmRxHeader_m || (recordType == 'H'))
		{
			if (*templatePtr != '\0')			// NULL at end of template       
			{
				rxFrameChar = ExtractRxFrameFields(*templatePtr++, rxFrameChar, checksumPtr);

				if (rxFrameChar == NAK)
				{
					return (NAK);	// in-valid astm char in rx frame method processing so NAK       
				}
			}
		}

		if ((rxFrameChar == astmRxFieldDelimiter_m) || (rxFrameChar == CR))
		{
			numFields++;

			// check to make sure num_fields is less than max for this frame       

			if (numFields > maxFields)
			{
				// too many fields for this astm 1394 record type so set up for tx of abaxis comment message       

				astm1394ErrorType_m = ASTM_1394_RX_TOO_MANY_FIELDS_IN_RECORD;	
			}
		}
			
	} while (rxFrameChar != CR);

	// make sure there is an ETX and include in checksum per ASTM 1381-02	      

	if (ReceivedAstmFrameChar() != ETX)
	{
		return (NAK);				// char after CR is not ETX	so NAK       
	}
	else
	{
		checksum += ETX;			// ETX so include as last char in checksum        
	}

	//  then get checksum chars and compare to rx computed checksum per ASTM 1381-02	      

	rxFrameChar  = AsciiToHex( ReceivedAstmFrameChar() ) << 4;		// get checksum msn from rx frame       
	rxFrameChar += AsciiToHex( ReceivedAstmFrameChar() );			// get checksum lsn from rx frame and merge with msn       

	if ( (unsigned char) rxFrameChar != checksum )
	{
		return (NAK);				// checksum received does not match computed frame checksum so NAK       
	}

	//  then verify got <CR> <LF> in sequence as end of ASTM 1381-02       

	if (ReceivedAstmFrameChar() != CR)
	{
		return (NAK);				// char after checksum chars is not CR so NAK       
	}

	if (ReceivedAstmFrameChar() != LF)
	{
		return (NAK);				// char after last CR is not LF so NAK       
	}


	// got good astm 1381-02 frame, but now process astm rx frames for correct record synchronization / coalation  	       
	//   and to see if got complete abaxis astm rx msg without astm 1394 errors									      
	//   if got complete msg without astm 1394 errors, then sets to process message type							      	

	SetupAbaxisAstmRxMsg(recordType);


	// all OK so ACK this frame for astm state machine       

	return (ACK);					// valid astm rx frame received so ACK       
}


	// functions follow that support overall ASTM message and interface processing including:
	//
	//	- ASTM 1394-97 abaxis tx and rx hi level message processing:
	//		ProcessAbaxisAstmMsgs() called by ProcessAstmState() during idle state
	//
	//	- ASTM 1381-02 state machine implementation:
	//		ProcessAstmState() 
						
      
// ********************************************************************
// FUNCTION:
//
//		SetAstmRecordTypeMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set ASTM type for transmit as either valid results data type or
//		aborted error rotor results data type message  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

AstmTxMsgType_t SetAstmRecordTypeMsg( void )
{
	if (astmRecord_m->astmRecordType == ASTM_RECORD_RESULTS )
	{
		return ASTM_TX_RESULTS_MSG;
	}
	else
	{
		return ASTM_TX_ABORT_MSG;
	}
}


// ********************************************************************
// FUNCTION:
//
//		ProcessSendResultsQuery() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		State machine set up to process query requests from LIS 
//		for potentially multiple Abaxis results transmit messages 
//		to be sent to LIS 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessSendResultsQuery(void)
{
	// process ASTM Query message request transmit states

	switch ( astmQueryState_m )
	{
		case ASTM_QUERY_INIT:
			if (astmRxQueryStartId_m[0] == '\0') 							// check to handle no ID at all       
			{
				astmQueryState_m = ASTM_QUERY_NO_RECORDS;					// set up to send no query results feedback message       
			}
			else
			{
				astmRecord_m = SearchAstmResultsRecord( astmRxQueryStartId_m, astmRxQueryBeginDateTime_m, astmRxQueryEndDateTime_m );
				
				if (astmRecord_m != NULL)
				{
					astmTxMsgType_m = SetAstmRecordTypeMsg();				// at least 1 record exists

					astmQueryState_m = ASTM_QUERY_NEXT_RECORD;				// set up to search for next database ASTM record       
				}
				else
				{
					astmQueryState_m = ASTM_QUERY_NO_RECORDS;				// set up to send no query results feedback message       
				}
			}
			break;											
	       
		case ASTM_QUERY_NEXT_RECORD:
			// get next record	
			astmRecord_m = GetNextAstmResultsRecord();

			if (astmRecord_m != NULL)
			{
				astmTxMsgType_m = SetAstmRecordTypeMsg();
			}
			else
			{
				astmQueryState_m = ASTM_QUERY_COMPLETED;					// set up to send no query results feedback message       
			}
			break;

		case ASTM_QUERY_NO_RECORDS:
			astmTxMsgType_m = ASTM_TX_NO_QUERY_RESULTS_MSG;					// send no query results message  

			// and complete abaxis send query state machine

		case ASTM_QUERY_COMPLETED:
		default:
			// if query of all astm results selected have been processed then cleanup and exit

			astmRxBusy_m = false;											// handshake to now allow more host rx messages       

			abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;						// set for abaxis de-queued query type send done       
			break;	
	}	
}


// ********************************************************************
// FUNCTION:
//
//		ProcessTransmitRangeResultsRecords() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		State machine set up to process transmit of results records range 
//		for potentially multiple Abaxis results transmit messages initiated  
//		by operator to be sent to LIS 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessTransmitRangeResultsRecords(void)
{
	// process ASTM transmit range message states

	switch ( astmTransmitRangeState_m )
	{
		case ASTM_TRANSMIT_RANGE_INIT:
			astmRecord_m = SearchAstmResultsRecord( "ALL", abaxisSendParam3_m, abaxisSendParam4_m );
				
			if (astmRecord_m != NULL)
			{
				astmTxMsgType_m = SetAstmRecordTypeMsg();						// at least 1 record exists

				astmTransmitRangeState_m = ASTM_TRANSMIT_RANGE_NEXT_RECORD;		// set up to search for next database ASTM record       
			}
			else
			{
				astmTransmitRangeState_m = ASTM_TRANSMIT_RANGE_NO_RECORDS;		// set up to send no query results feedback message       
			}
			break;											
	       
		case ASTM_TRANSMIT_RANGE_NEXT_RECORD:
			// get next record	
			astmRecord_m = GetNextAstmResultsRecord();

			if (astmRecord_m != NULL)
			{
				astmTxMsgType_m = SetAstmRecordTypeMsg();
			}
			else
			{
				astmTransmitRangeState_m = ASTM_TRANSMIT_RANGE_COMPLETED;			       
			}
			break;

		case ASTM_TRANSMIT_RANGE_NO_RECORDS:
		case ASTM_TRANSMIT_RANGE_COMPLETED:
		default:
			// if transmit of all astm results in range have been processed then cleanup and exit

			MarkTransmitResultsRecordsDone();

			abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;							// set for abaxis de-queued transmit range type send done       
			break;	
	}	
}


// ********************************************************************
// FUNCTION:
//
//		ProcessAbaxisAstmMsgs() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sets up and handles 
//			- transmit of messages in Abaxis ASTM send message queue
//			- receipt of messages from LIS (e.g. result queries)
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void	ProcessAbaxisAstmMsgs()
{
	// process abaxis astm tx messages       

	if (abaxisSendType_m != ABAXIS_SEND_DONE_TYPE)					// send is in process       
	{
		// if a query message send is in process, do not de-queue new tx message

		if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE)		// process results query sends - potential multiple H..L messages       	
		{
			ProcessSendResultsQuery();
		}
		else if (abaxisSendType_m == ABAXIS_SEND_RESULTS_RANGE_TYPE)	// process results range sends - potential multiple H..L messages       	
		{
			ProcessTransmitRangeResultsRecords();
		}
		else // process finish of de-queued abaxis messages with 1 H..L message sequence only       
		{
			if (astmTxMsgType_m == ASTM_TX_NO_MSGS)					// single de-queued message complete       
			{
				abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;			// set for abaxis send done       	
			}
		}
	}
	else 																       
	{
		// abaxis send done so check if another tx message type in queue to send

		if (abaxisSendQueueOutputIdx_m != abaxisSendQueueInputIdx_m)
		{
			abaxisSendType_m    = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgSendType;
			abaxisSendParam1_m  = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam1;
			abaxisSendParam2_m  = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam2;
			strcpy( abaxisSendParam3_m, abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam3 );
			strcpy( abaxisSendParam4_m, abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam4 );
            memcpy( &abaxisSendParam5_m, &abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam5, sizeof( AstmInput_t ));
		
			abaxisSendQueueOutputIdx_m = (abaxisSendQueueOutputIdx_m + 1) % ABAXIS_SEND_QUEUE_SIZE;

			// process tx send type       
			switch ( abaxisSendType_m )
			{
				case ABAXIS_SEND_1394_COMMENT_MSG_TYPE:
                    //ZZZ
                    //LogDebug ("set astmTxMsgType_m = ASTM_TX_1394_COMMENT_MSG"); 
					astmTxMsgType_m = ASTM_TX_1394_COMMENT_MSG;
					break;

				case ABAXIS_SEND_ROTOR_RUN_DONE_TYPE:
					// rotor run completed so get newest rotor result record & check to set for result or error

                    astmRecord_m = GetAstmResultsRecord( (unsigned) abaxisSendParam1_m );

					if ( astmRecord_m != 0 )
					{
						astmTxMsgType_m = SetAstmRecordTypeMsg();
					}
					break;

				case ABAXIS_SEND_RESULTS_QUERY_TYPE:
					astmQueryState_m = ASTM_QUERY_INIT;
					break;

				case ABAXIS_SEND_RESULTS_RANGE_TYPE:
					astmTransmitRangeState_m = ASTM_TRANSMIT_RANGE_INIT;
					break;
					
				case ABAXIS_SEND_M1_TYPE:
					astmTxMsgType_m = ASTM_TX_MANUFACTURER_TYPE1_MSG;
					break;

                case ABAXIS_SEND_ORDER_DATA_INVALID_TYPE:
                    astmTxMsgType_m = ASTM_TX_ORDER_DATA_INVALID_MSG;
                    break;

                case ABAXIS_SEND_ORDER_CANCELLATION_TYPE:
                    astmTxMsgType_m = ASTM_TX_ORDER_CANCELLATION_MSG;
                    break;
                
                case ABAXIS_SEND_M_AS_TYPE:
                    astmTxMsgType_m = ASTM_TX_MANUFACTURER_AS_MSG;      // M - AS analyzer status
                    break;

                case ABAXIS_SEND_M_PO_TYPE:
                    astmTxMsgType_m = ASTM_TX_MANUFACTURER_PO_MSG;      // M - PO pending orders
                    astmTxMsequenceNumber_m = 1;
                    break;


				default: 												// not valid abaxisSendType_m       
					abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;
					break;
			}	
		}
	} 

	// process abaxis astm rx messages       
    switch ( abaxisAstmRxMsgToProcess_m )
    {
        case ASTM_RX_QUERY_MSG:
            // set up for astm query message processing

            QueueAstmTxMsg( ABAXIS_SEND_RESULTS_QUERY_TYPE );
            
            memset(astmInput_m, '\0', sizeof( AstmInput_t ));           // make astm rx input structure empty for next rx input

            abaxisAstmRxMsgToProcess_m = 0;                             // handshake for no more abaxis rx msg to process
            break;

        case ASTM_RX_MFG_MSG:
            // set up for astm manufacturing status request message processing

           SetMfgStatusRequest( astmInput_m );                         // Set instrument status request - analyzer status or pending order  
           memset(astmInput_m, '\0', sizeof( AstmInput_t ));           // make astm rx input structure empty for next rx input


            abaxisAstmRxMsgToProcess_m = 0;                             // handshake for no more abaxis rx msg to process
            astmRxBusy_m = false;                                       // handshake to now allow more host rx messages
            break;

        case ASTM_RX_ORDER_MSG:
            // set up for astm patient order to run rotor request processing

            SetHostPatientOrder( astmInput_m );                         // Set Host/LIS Order to run rotor request
            memset(astmInput_m, '\0', sizeof( AstmInput_t ));           // make astm rx input structure empty for next rx input

            abaxisAstmRxMsgToProcess_m = 0;                             // handshake for no more abaxis rx msg to process
            astmRxBusy_m = false;                                       // handshake to now allow more host rx messages
            break;

        default:
            break;
    }


}



// ********************************************************************
// FUNCTION:
//
//		ProcessAstmState() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Main state machine processor ASTM idle, transmit, and receive states
//			- implements main ASTM 1381-02 standard communications to/from ASTM port  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

AstmState_t ProcessAstmState(AstmState_t astmState, unsigned short *errorNumber)
{
	char	astmReceivedChar;														// temp astm character received for comparison       

	switch (astmState)
	{
		case ASTM_INIT_STATE:
			// Process ASTM_INIT_STATE: initialization state for ASTM state machine interface processing       

			abaxisSendQueueInputIdx_m = 0;											// init the astm_tx_queue indices & flags       
			abaxisSendQueueOutputIdx_m = 0;
			abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;
			astmTxMsgType_m = ASTM_TX_NO_MSGS;										// set for no Abaxis data to send on init       

			// set up abaxis information info for send of ASTM 1394 tx power up sign on comment (H C1 L) message        

			instrumentInfo_m = GetInstrumentInfo();

			QueueAstmTxMsg( ABAXIS_SEND_1394_COMMENT_MSG_TYPE, ASTM_POWER_UP_SIGN_ON );

			// set up last_astm_tx_iqc_r_record_m based on xPress or VS2 instrumentInfo_m for number of IQC records transmitted       

			if ( instrumentInfo_m->productCode == '1' )								// productCode: '1' is VS2 else xPress or both
			{
				last_astm_tx_iqc_r_record_m = LAST_TX_IQC_R_RECORD_VS2;				// set up number of VS2    instrument IQC records
			}
			else
			{
				last_astm_tx_iqc_r_record_m = LAST_TX_IQC_R_RECORD_XPRESS;			// set up number of xPress instrument IQC records
			} 

			abaxisAstmRxMsgToProcess_m = 0;											// init for no rx msg to process       		
			astmRxBusy_m = false;													// set for instrument not busy on init       
			astm1394ErrorDuringRxMsg_m = false;										// init for no astm 1394 issues during abaxis message reception       

			astmRxFieldDelimiter_m     = '|';										// default field delimiter       
			astmRxRepeatDelimiter_m    = '\\';										// default header repeat delimiter       
			astmRxComponentDelimiter_m = '^';										// default header component delimiter       
			astmRxEscapeDelimiter_m    = '&';										// default header escape delimiter       

			gotAstmRxHeader_m = false;												// init for first rx header not received       
            memset(astmInput_m, '\0', sizeof( AstmInput_t ));                       // make astm rx input structure empty on init


			astmTimeoutSeconds_m = ASTM_TIMED_OUT;									// init the astm timeout counter to off/disabled       
		 
			astmState = ASTM_IDLE_STATE;
			break;


	//	*** ASTM 1381-02 STATE MACHINE ***

		case ASTM_IDLE_STATE:
			// Process ASTM_IDLE_STATE: astm communications idle/terminated state       

			if (astmTxMsgType_m != ASTM_TX_NO_MSGS)									// Abaxis data to send so start transmit states       			
			{
				astmState = ASTM_TX_DATA_TO_SEND_STATE;	
			}
			else if (ReceivedAstmRxChar( errorNumber ) == ENQ) 						// received ENQ so start receive states       
			{
				astmState = ASTM_RX_AWAKE_STATE;
			}
			else
			{
				// process abaxis defined astm messages transmit/receive (made up of astm record frames)

				WaitForMsg();														// Wait for 1 second timer or last rotor record available message

				ProcessAbaxisAstmMsgs();											// process msg frame collection and msg verification here also
			}									       
			break;


	//	--- ASTM 1381-02 ABAXIS INSTRUMENT AS SENDING DEVICE STATES ---

		case ASTM_TX_DATA_TO_SEND_STATE:
			// Process ASTM_TX_DATA_TO_SEND_STATE: set up Abaxis data transmission state       
	 
			astmTxFrameNumber_m = 1;												// init for first transmit frame/record       

			abaxisTxMsgDefPtr_m = (char *) abaxisAstmTxMsgPtrs_m[astmTxMsgType_m];	// set pointer to this message definition record indices       

			astmTimeoutSeconds_m = ASTM_15S_TIMEOUT;								// set for 15 second timeout       

            // ZZZ
            //LogDebug ("Send ENQ");
			SendAstmChar( ENQ );													// send ENQ to establish connection to transmit       

			astmState = ASTM_TX_WAITING_CONNECTION_STATE;							// proceed to wait for connection to transmit       
			break;


		case ASTM_TX_WAITING_CONNECTION_STATE:
			// Process ASTM_TX_WAITING_CONNECTION_STATE: handle establishing data tranmission with receiver state       

			astmReceivedChar = ReceivedAstmRxChar( errorNumber );  					// get char from rx 0 buffer       

			if (astmReceivedChar == ENQ)
			{
				astmTimeoutSeconds_m = ASTM_1S_TIMEOUT;								// set for 1 second timeout       
				astmState = ASTM_TX_CONTENTION_OR_COMPUTER_BUSY_STATE;				// proceed to wait 1 s before tranmit next ENQ       			 
			}
			else if (astmReceivedChar == NAK)
			{
				astmTimeoutSeconds_m = ASTM_10S_TIMEOUT;							// set for 10 second timeout       
				astmState = ASTM_TX_CONTENTION_OR_COMPUTER_BUSY_STATE;				// proceed to wait 10 s before tranmit next ENQ       			 
			}
			else if (astmReceivedChar == ACK)
			{
				astmTxBufferIdx_m = 0;
				astmTxRetryCount_m = 0;												// clear transmit retry count       
				astmState = ASTM_TX_SETUP_NEXT_FRAME_STATE;							// proceed to process first frame/record of Abaxis message       			 
			}
			else if ( AstmTimedOut() )												// no ENQ, NAK, or ACK in 15 seconds?       		
			{
                //ZZZ
                //LogDebug ("Send EOT");
				SendAstmChar( EOT );												// send EOT       
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}
			break;
	 

		case ASTM_TX_CONTENTION_OR_COMPUTER_BUSY_STATE:
			// Process ASTM_TX_CONTENTION_OR_COMPUTER_BUSY_STATE: set up Abaxis to handle transmission contention or receiver busy state       

			if ( AstmTimedOut() )
			{
				astmState = ASTM_IDLE_STATE;										// proceed to idle to transmit another ENQ       
			}
			break;
	 

		case ASTM_TX_SETUP_NEXT_FRAME_STATE:
			// Process ASTM_TX_SETUP_NEXT_FRAME_STATE: set up next frame to transmit/determine if done transmitting Abaxis data state       

			if (SetupTxMessageFrames() == ABAXIS_ASTM_TX_MSG_TRANSMITTED)			// set up frame to transmit if not done with message        
			{
				if (( astmTxMsgType_m == ASTM_TX_RESULTS_MSG ) || ( astmTxMsgType_m == ASTM_TX_ABORT_MSG ))
				{
					MarkResultsRecordSent( astmRecord_m->recordId );
				}

				astmTxMsgType_m = ASTM_TX_NO_MSGS;									// message sent       
				SendAstmChar( EOT );												// send EOT       
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}
			else
			{
				astmState = ASTM_TX_FRAME_READY_STATE;								// proceed to transmit frame out UART0       
			}
			break;
	 

		case ASTM_TX_FRAME_READY_STATE:
			// Process ASTM_TX_FRAME_READY_STATE: frame ready so tranmit/retransmit state       
			WritePort (astmTxBuffer_m, astmTxBufferIdx_m);							// start transmit of frame here with byte count = astmTxBufferIdx_m
			astmTimeoutSeconds_m = ASTM_15S_TIMEOUT;								// frame transmitted, so set for 15 second timeout	      
			astmState = ASTM_TX_WAITING_ACK_STATE;									// proceed to wait for ACK from receiver			      
			break;
	 

		case ASTM_TX_WAITING_ACK_STATE:
			// Process ASTM_TX_WAITING_ACK_STATE: wait for acknowledgment of tranmitted frame state       

			astmReceivedChar = ReceivedAstmRxChar( errorNumber );

			if ((astmReceivedChar == ACK) || (astmReceivedChar == EOT))				// accept ACK, ignore EOT       
			{
				astmTxFrameNumber_m = (astmTxFrameNumber_m + 1) % 8;				// incr tx frame number       

				astmTxBufferIdx_m = 0;
				astmTxRetryCount_m = 0;												// clear transmit retry count       
				astmState = ASTM_TX_SETUP_NEXT_FRAME_STATE;							// proceed to process next frame/record of Abaxis message       			 
			}
			else if (astmReceivedChar != (char)NO_RX_CHAR)							// NAK or (ANY other valid character) received       
			{
				astmTxRetryCount_m++;
				astmState = ASTM_TX_SETUP_LAST_FRAME_STATE;							// proceed to wait 10 s before tranmit next ENQ       			 
			}
			else if ( AstmTimedOut() )												// no chars in 15 seconds?       		
			{
				SendAstmChar( EOT );												// send EOT       
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}
			break;
	 

		case ASTM_TX_SETUP_LAST_FRAME_STATE:
			// Process ASTM_TX_SETUP_LAST_FRAME_STATE: set up frame for retry/retransmit state       

			if (astmTxRetryCount_m == MAX_ASTM_TX_RETRIES_ALLOWED)					// too many NAKS so terminate comm       
			{
                // ZZZ fix bug 
                astmTxMsgType_m = ASTM_TX_NO_MSGS;                                  // message sent
				SendAstmChar( EOT );												// send EOT       
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}
			else
			{
				astmState = ASTM_TX_FRAME_READY_STATE;								// proceed to transmit frame out serial port       
			}
			break;
	 

	//	--- ASTM 1381-02 ABAXIS INSTRUMENT AS RECEIVING DEVICE STATES ---

		case ASTM_RX_AWAKE_STATE:
			// Process ASTM_RX_AWAKE_STATE: set up to acknowledge established reception state       

			if (astmRxBusy_m)
			{
				SendAstmChar( NAK );												// send NAK to computer for instrument busy       
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}
			else
			{
				astmRxFrameNumber_m = 1;											// init for first receive frame/record expected       

				SendAstmChar( ACK );												// send ACK to establish connection with computer       

				astmRxBufferIdx_m = 0;												// init index to receive buffer       

				astmTimeoutSeconds_m = ASTM_30S_TIMEOUT;							// set for 30 second timeout       

				astmState = ASTM_RX_WAITING_STATE;									// proceed to wait to receive frame from computer       
			}
			break;
	 

		case ASTM_RX_WAITING_STATE:
			// Process ASTM_RX_WAITING_STATE: wait for received frame state       

			astmReceivedChar = ReceivedAstmRxChar( errorNumber );				        
		
			if (astmReceivedChar != (char)NO_RX_CHAR)
			{
				if (astmRxBufferIdx_m < ASTM_RX_BUFFER_SIZE)
				{
					astmRxBuffer_m[astmRxBufferIdx_m++] = astmReceivedChar;			// save char in receive buffer
				}

				if (astmReceivedChar == LF)
				{
					astmState = ASTM_RX_FRAME_RECEIVED_STATE;						// proceed to process frame data       			 
				}
				else if (astmReceivedChar == EOT)				
				{												
					astmState = ASTM_IDLE_STATE;									// proceed to idle       
				}																	// for EOT char
			}																		        
			else if ( AstmTimedOut() )				
			{												
				astmState = ASTM_IDLE_STATE;										// proceed to idle       
			}																		// for no frame in 30 seconds received from sender        
			break;


		case ASTM_RX_FRAME_RECEIVED_STATE:
			// Process ASTM_RX_FRAME_RECEIVED_STATE: check and acknowledge/not ack proper frame received state       

			if (ProcessRxMessageFrames() == ACK)									// process ACK for good frame       
			{
				astmRxFrameNumber_m = (astmRxFrameNumber_m + 1) % 8;				// incr rx frame number       

				SendAstmChar( ACK );												// send ACK to establish connection with computer       
			}
			else 																	// process NAK for bad frame       
			{
				SendAstmChar( NAK );												// send NAK to computer        
			}

			astmRxBufferIdx_m = 0;													// init index to receive buffer for receive of next frame      

			astmTimeoutSeconds_m = ASTM_30S_TIMEOUT;								// set for 30 second timeout       
			astmState = ASTM_RX_WAITING_STATE;										// proceed to wait to receive frame from computer       
			break;


	//	ASTM 1381-02 ABAXIS INSTRUMENT INVALID STATE ERROR
	      
		default:
			astmState = ASTM_INIT_STATE;											// re-initialize state machine       
		
			//       add any other error handling as required here       
				
			break;
	}

	return astmState;
}


// ********************************************************************
// FUNCTION:
//
//		main() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console ASTM process top level main function 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/

int main( int argc, char **argv )
{
	unsigned short		errorNumber;
	AstmState_t			astmState;
	unsigned			i=0;
	unsigned char		checksum = 0;
	int					c = 0;
	int					port = 2;
	long				speed = ASTM_BAUD_RATE;
	int					status = 0;
	FT_STATUS			ret;

	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
    factoryData = (FactoryData_t *)  MapSharedMem( FACTORY_SHMEM_NAME,  sizeof( FactoryData_t ) );
    systemData  = (SystemData_t *)   MapSharedMem( SYSTEM_SHMEM_NAME,   sizeof( SystemData_t ) );
    ordersList = (OrdersList_t *)    MapSharedMem( ORDERS_SHMEM_NAME,   sizeof( OrdersList_t ) );

	
	// Get the command line options.
	while ( i < argc )
	{
		if (argv[i] == "-p")
		{
			port = atoi(argv[++i]);
			LogDebug("SerialComm| argv[%u]=%s",i,port);
		}
		else if (argv[i] == "-b")
		{
			speed = atoi(argv[++i]);
			LogDebug("SerialComm| argv[%u]=%s",i,port);
		}

		i++;
	}
	
	/*
	 *  Open the connection to the ASTM server.
	 */
	 
	// TCP/IP connection.
	if ( (settingsData != 0) && (settingsData->comm.protocol == COMM_TCPIP) )
	{
		serverSocket_m.sin_family = AF_INET;
		serverSocket_m.sin_port = htons( settingsData->comm.serverSocket );

		//status = inet_aton( settingsData->comm.serverIp, &serverSocket_m.sin_addr );
		serverSocket_m.sin_addr.S_un.S_addr = inet_addr( settingsData->comm.serverIp );
		if ( serverSocket_m.sin_addr.S_un.S_addr == 0 )
		{
			LogError( "inet_addr() failed" );
			return -1;
		}
		SocketConnect();
	}

	// USB/Serial connection.
	else
	{
		fd_ser = OpenSerialPort(port);
		if ( fd_ser == INVALID_HANDLE_VALUE )
		{
			LogError( "failed to open %s", port );
			return -1;
		}
	
		// Set the serial port's baud rate and handshaking.
		if (SetSerialPortBaudRate (fd_ser, speed, port) == -1) {
			LogError("ASTM: Failed to change port%d baud rate %d", port, speed);
			return -1;
		}
		ret = FT_SetFlowControl(fd_ser, FT_FLOW_NONE, 0, 0);
		if (ret != FT_OK)
		{
			LogError("ASTM: Failed to set flow control %d", ret);
			return -1;
		}
	}

    LogDebug( "ASTM_INIT_STATE" );

	// Start the 1 second timer thread.
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);

	// Set up for Windows debugger testing of simulated 'HQL' message received
	strcpy(	astmRxBuffer_m, "\xff\x02""1H|\\^&\x0d\x03""E5\x0d\x0a\xff\x02""2Q|1|ALL\x0d\x03""95\x0d\x0a\xff\x02""3L|1\x0d\x03""3C\x0d\x0a\xff" );
	for (i = 2; i < 0x0a; i++)
	{
		checksum += astmRxBuffer_m[i];
	}

	errorNumber = SERR_NONE;
	astmState = ASTM_INIT_STATE;

	// main ASTM process
	while ( errorNumber == SERR_NONE )
	{
		astmState = ProcessAstmState( astmState, &errorNumber );
	}

	LogError( "ASTM: exiting with error %X", errorNumber );

	return 0;
}


