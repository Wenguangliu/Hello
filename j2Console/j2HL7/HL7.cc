
// ********************************************************************
//
// FILENAME:  
// 
// 		Hl7.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		HL7 process
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
#include <WS2tcpip.h>

#include <math.h>
#include <pm.h>


#include <FtdiConfig.h>

#include "AlgGlobals.h"
#include "Hl7Data.h"
#include "Hl7Constants.h"
#include "Hl7Messages.h"
#include "Hl7Query.h"
#include "LogErr.h"
#include "ResultsData.h"
#include "SettingsData.h"
#include "Shmem.h"
#include "ErrorProcessMessages.h"
#include "SystemErrorNums.h"
#include "ErrorReport.h"
#include "LogErr.h"
#include "ParseHL7Configuration.h"



#define NO_ERRORS		0

#undef HL7_DEBUG_ENABLE


// HL7 interface definitions       

#define HL7_RX_BUFFER_SIZE			1000				
#define HL7_TX_BUFFER_SIZE			1000				

// HL7 timeout definitions (time in seconds + 1)       

#define HL7_15S_TIMEOUT			(15+1)						// 15 to 16 seconds			      	

#define NO_RX_CHAR						0xff				// no char received due to readcond timeout
#define	NOT_VALID_HL7_CHAR				0xff				// no valid HL7 7 bit ASCII character received       

#define	ABAXIS_HL7_TX_MSG_TRANSMITTED	true				// for Abaxis hl7 message transmit done   		      
#define	ABAXIS_HL7_TX_MSG_NOT_DONE		false				// for Abaxis hl7 message transmit not done   	      

#define HL7_TX_NTE_SKIP					0x80				// set in hl7_tx_NTE_sequence_number to signal skip of C record       

// abaxis receive msg type definitions       

#define	HL7_RX_QUERY_MSG				1					// abaxisHl7RxMsgToProcess: query message (H,Q,L)       

#define MAX_INVALID_RX_FIELDS			127					// max number of fields for invalid record detected

#define	PATIENT_SEQUENCE_NUMBER			"1"						// Always 1

#define SB                              0x0B                // Set Block
#define EB                              0x1C                // End Block

// abaxis Query message states
	
typedef enum
{
	HL7_QUERY_INIT,											// hl7 query state machine definitions
	HL7_QUERY_NO_RECORDS, 		
	HL7_QUERY_NEXT_RECORD,
	HL7_QUERY_COMPLETED,       
} Hl7QueryState_t;
		

// abaxis transmit range message states
	
typedef enum
{
	HL7_TRANSMIT_RANGE_INIT,									// hl7 transmit range state machine definitions
	HL7_TRANSMIT_RANGE_NO_RECORDS, 		
	HL7_TRANSMIT_RANGE_NEXT_RECORD,
	HL7_TRANSMIT_RANGE_COMPLETED,       
} Hl7TransmitRangeState_t;
		

// abaxis (H C1 L) comment message type definitions       

typedef enum
{
	HL7_POWER_UP_SIGN_ON,								// abaxis comment message sent at power up        
	HL7_RX_DELIMITER_MISMATCH,							// hl7 field delimiter does not match expected delimiter 
	HL7_RX_FIELD_SIZE_TOO_BIG,							// hl7 filed size is too big
	HL7_RX_INVALID_RECORD_TYPE,							// hl7 not a valid record type       
	HL7_RX_TOO_MANY_FIELDS_IN_RECORD,					// hl7 too many fields for this record type       
	HL7_RX_RECORD_OUT_OF_SEQUENCE,						// hl7 record not between H Q sequence
	HL7_RX_UNSUPPORTED_HL7_VERSION						// Query other than SUPPORTED_HL7_VERSION (2.3)
} Hl7CommentType_t;

#define HL7_RESULT_FLAGS_COMMENT_MASK		( MASK_RESULTS_SUPPRESSED | MASK_LOW_SYSTEM_LIMIT | MASK_HIGH_SYSTEM_LIMIT |	MASK_LOW_DYNAMIC_LIMIT | MASK_HIGH_DYNAMIC_LIMIT | MASK_HEMOLYZED | MASK_HEMOLYZED_LOW | MASK_HEMOLYZED_HIGH | MASK_HEMOLYZED_SUPPRESSED |MASK_LIPEMIC | MASK_LIPEMIC_LOW | MASK_LIPEMIC_HIGH | MASK_LIPEMIC_SUPPRESSED | MASK_ICTERIC | MASK_ICTERIC_LOW | MASK_ICTERIC_HIGH | MASK_ICTERIC_SUPPRESSED | MASK_DILUTION_CHECK )


// abaxis transmit queue definitions       

typedef enum
{
	ABAXIS_SEND_DONE_TYPE,										// abaxis send done        
	ABAXIS_SEND_COMMENT_MSG_TYPE,								// abaxis hl7 comment type      
	ABAXIS_SEND_ROTOR_RUN_DONE_TYPE,							// abaxis last rotor run done & results database record available
	ABAXIS_SEND_RESULTS_QUERY_TYPE,								// abaxis hl7 query type       
	ABAXIS_SEND_RESULTS_RANGE_TYPE,								// abaxis hl7 results range type       
} AbaxisSendType_t; 

#define ABAXIS_SEND_QUEUE_SIZE			100						// max number of abaxis hl7 transmit queue entries	      


typedef struct
{
	AbaxisSendType_t	msgSendType;
	unsigned int		msgParam1;
	char				msgParam2;
	char				msgParam3[15];
	char				msgParam4[15];
} AbaxisSendQ_t;


// HL7 / interface processor state  definitions       

typedef enum
{
	NO_HL7_STATE,
	HL7_INIT_STATE,
	HL7_IDLE_STATE,
	HL7_TX_DATA_TO_SEND_STATE,
	HL7_TX_SETUP_NEXT_FRAME_STATE,
	HL7_TX_FRAME_READY_STATE,
} Hl7State_t;


typedef struct
{
	unsigned long	analyteFlagMask;
	const char *	hl7RnC1Text;
} Hl7RnC1TextData_t;

typedef struct {
char				hl7RxMSHFieldDelimiter;					// receive header field delimiter       
char				hl7RxMSHRepeatDelimiter;				// receive header repeat delimiter       
char				hl7RxMSHComponentDelimiter;				// receive header component delimiter       
char				hl7RxMSHSubComponentDelimiter;			// receive header component delimiter       
char				hl7RxMSHEscapeCharacter;				// receive header escape character
char				hl7RxMSHSendingApplication[180];
char				hl7RxMSHSendingFacility[180];
char				hl7RxMSHReceivingApplication[180];
char				hl7RxMSHReceivingFacility[180];
char				hl7RxMSHMsgDateTime[26];
char				hl7RxMSHSecurity[40];
char				hl7RxMSHMessageType[8];
char				hl7RxMSHMessageControlID[20];
char				hl7RxMSHProcessingID[3];
char				hl7RxMSHVersionID[8];

char				hl7RxQRDDateTime[26];
char				hl7RxQRDFormatCode;
char				hl7RxQRDPriority;
char				hl7RxQRDID[10];
char				hl7RxQRDDeferredResponseType;
char				hl7RxQRDDeferredResponseDate;
char				hl7RxQRDQualityRequest;
char				hl7RxQRDWhoSubjectFilter[60];
char				hl7RxQRDWhatSubjectFilter[60];
char				hl7RxQRDWhatDataCodeValue[60];
char				hl7RxQRDWhatDataCodeValueQual[20];
char				hl7RxQueryBeginDateTime[15];			// Query message begin date/time string       
char				hl7RxQueryEndDateTime[15];				// Query message end date/time string
}Hl7RxQueryData_t;



#define NUM_RN_C1_MASKS					24


unsigned			hl7TxBufferIdx_m;							// index to tx buffer & tx message size	
char				hl7TxBuffer_m[HL7_TX_BUFFER_SIZE];			//
unsigned			hl7RxBufferIdx_m;							// index to rx buffer       
char				hl7RxBuffer_m[HL7_RX_BUFFER_SIZE];			//
unsigned			hl7TimeoutSeconds_m;						// hl7 timeout second counter/flag       	
AbaxisSendQ_t		abaxisSendQueue_m[ABAXIS_SEND_QUEUE_SIZE];	// hl7 tx message queue       
unsigned			abaxisSendQueueInputIdx_m;					// hl7 tx queue input index       
unsigned			abaxisSendQueueOutputIdx_m;					// hl7 tx queue output index       
AbaxisSendType_t	abaxisSendType_m;							// Abaxis send msg type    (dequeued from transmit Q      
unsigned			abaxisSendParam1_m;							// Abaxis send parameter 1 (dequeued from transmit Q
char				abaxisSendParam2_m;							// Abaxis send parameter 2 (dequeued from transmit Q
char				abaxisSendParam3_m[15];						// Abaxis send parameter 3 (dequeued from transmit Q) - transmit range start date/time       
char				abaxisSendParam4_m[15];						// Abaxis send parameter 4 (dequeued from transmit Q) - transmit range end date/time       
Hl7TxMsgType_t		hl7TxMsgType_m;								// 0: no data, code: Abaxis HL7 message to send       
unsigned			abaxisHl7RxMsgToProcess_m;					// 0: no msg to process, code: Abaxis message to process       
bool				hl7RxBusy_m;								// Abaxis instrument hl7 receiver busy flag       
unsigned			hl7TxFrameNumber_m;							// mod 8 frame number incremented for each frame/record transmitted       
unsigned			hl7RxFrameNumber_m;							// mod 8 frame number incremented for each frame/record received       
unsigned			hl7TxRetryCount_m;							// transmit retry count (max 6)       
unsigned			hl7AnalyteIdx_m;							// result index
char				hl7TxNTEtype_m;								// hl7 transmit record comment type: R or O       
bool				hl7ErrorDuringRxMsg_m;						// flag that hl7 type issue occured during receiving hl7 1381-02 acked records       
unsigned			abaxisHl7HclMessageType_m;					// message type for abaxis special comment (H C) messages  
unsigned			hl7ErrorType_m;								// hl7 type issue detected during receiving hl7 acked record       
bool				gotHl7RxHeader_m;							// got first rx header flag       
char *				abaxisTxMsgDefPtr_m;						// pointer to abaxis message definition working index to current setup record (frame) template       
unsigned			abaxisRxMsgRecordIdx_m;						// index to abaxis rx msg record type array       
unsigned			hl7TxOBRsequenceNumber_m;					// OBR order   record current sequence number       
unsigned			hl7TxOBXsequenceNumber_m;					// OBX result  record current sequence number        
unsigned			hl7TxNTEsequenceNumber_m;					// NTE comment record current sequence number        
unsigned			last_hl7_tx_iqc_r_record_m;					// last R IQC record to process based on checking if VS2 or xPress

unsigned			hl7QueryState_m;							// Q message tx response current query processing state       
unsigned			hl7TransmitRangeState_m;					// Q message tx response current transmit range processing state       
Hl7Record_t *		hl7Record_m;
InstrumentInfo_t *	instrumentInfo_m;
struct sockaddr_in	serverSocket_m = { 0 };
int					fd_m = -1;
int					listen_fd_m = -1;
SettingsData_t *	settingsData;

Hl7Configuration_t * hl7Configuration_m;						//Pointer to configuration shared memory

Hl7RxQueryData_t *	hl7RxQueryData_m;
Hl7RxQueryData_t	query;


static ULONGLONG	lastConnectAttempt = 0;
bool				pauseTimer = false;


/*
 * FUNCTION
 * 	TimerThread
 * 
 * DESCRIPTION
 * 	Sends a message to this HL7 process every 1 second.
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
	const int msg = MSG_HL7_TIMER;

	static HANDLE queue = NULL;
	
	if ( queue == NULL )
	{
		//queue = mq_open( HL7_QUEUE_NAME, O_WRONLY );
		MSGQUEUEOPTIONS		options;

		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;
		queue = CreateMsgQueue(HL7_QUEUE_NAME, &options);
	}
	
	if ( queue == NULL )
	{
		LogError( "failure to open HL7 queue in TimerThread" );
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
 * 
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
	static bool			connectedLastCall = true;
	struct sockaddr_in	sin;
	int					optval = 1;

	if (hl7TimeoutSeconds_m == 0) {
        if (fd_m != -1) {
		    closesocket( fd_m );
		    fd_m = -1;
#ifdef HL7_DEBUG_ENABLE 
            LogDebug ("Close socket");
#endif

        }

#ifdef HL7_DEBUG_ENABLE 
            LogDebug ("Open socket");
#endif
		// Open the local socket.

		fd_m = socket( AF_INET, SOCK_STREAM, 0 );
		if ( fd_m == INVALID_SOCKET ) {
			LogError( "socket() failed" );
		}
		else {
			sin.sin_family = AF_INET;
			sin.sin_port = htons( settingsData->comm.serverSocket );
			sin.sin_addr.s_addr = INADDR_ANY;
			if ( bind( fd_m, (struct sockaddr *)&sin, sizeof( sin ) ) == SOCKET_ERROR ) {
				LogError( "bind() failed" );
			}
		
			// Connect to the server socket.
			else {			
				/*
				 * We pause the timer while we're connecting, because a failed connect takes about 74 seconds,
				 * and we don't want the message queue to jam up. There's no need for a lock because the timer only
				 * reads the flag, and it won't hurt if it misreads the value while we're writing.
				 */

				pauseTimer = true;
				status = connect( fd_m, (struct sockaddr *)&serverSocket_m, sizeof( serverSocket_m ) );
				pauseTimer = false;

				if ( status == SOCKET_ERROR ) {
					if ( connectedLastCall ) {
						LogError( "connect() failed" );
						connectedLastCall = false;
					}
					shutdown(fd_m, SD_BOTH); 
					closesocket( fd_m );
					fd_m = -1;
				}
				else {
					optval = 1;
					setsockopt( fd_m, SOL_SOCKET, SO_REUSEADDR/*SO_RCVTIMEO (not supported)*/, (char *)&optval, sizeof( optval ) );

					if ( !connectedLastCall ) {
						LogDebug( "connect() succeeded" );
						connectedLastCall = true;
					}
				}
			}
		}
		if (fd_m == -1) {
			hl7TimeoutSeconds_m = HL7_15S_TIMEOUT;
		}

	}
}
/*
 * FUNCTION
 * 	WritePort
 * 
 * DESCRIPTION
 * 	Attempts to write to the open socket.
 * 
 * PARAMETERS
 *	Buffer pointer
 *  count - number of bytes to be written
 * 
 * RETURN
 * 	None
 */

void
WritePort( char * ch, int count )
{
	int			sent;
	int			error;
	static bool	writeOkLastTime = true;

	// If there's currently no connection, try to connect before giving up.
	if ( fd_m == -1 ) {
		SocketConnect();
		if ( fd_m == -1 ) {
			return;
		}
	}

	// Write the buffer.
	sent = send(fd_m, ch, count, NULL);
#ifdef HL7_DEBUG_ENABLE
    ch [count] = '\0';
    LogDebug ("%s", ch);
#endif
	error = WSAGetLastError();
	if ( sent < count ) {
		if ( writeOkLastTime ) {
			LogError( "write() failed, error = %u", error );
			writeOkLastTime = false;
		}

		// If we're using TCP/IP, make sure our end of the socket is closed down and
		// make an attempt to reconnect.
		if ( settingsData->comm.protocol == COMM_TCPIP ) {
			shutdown(fd_m, SD_BOTH);
			closesocket( fd_m );
			fd_m = -1;
			SocketConnect();
		}
	}
	else {
		writeOkLastTime = true;
	}
}

// ********************************************************************
// FUNCTION:
//
//		QueueHl7TxMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Queue up an Abaxis HL7 message for sent to LIS  
// _____________________________________________________________________
// PARAMETERS:
//
//		msgSendType
//		message parameters
// ____________________________________________________________________
// RETURNED:
//
//		none
// ********************************************************************/      

void QueueHl7TxMsg( AbaxisSendType_t msgSendType, unsigned int msgParam1 = 0, char msgParam2 = '\0', const char *msgParam3 = "", const char *msgParam4 = "" )
{
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgSendType = msgSendType;
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam1   = msgParam1;
	abaxisSendQueue_m[abaxisSendQueueInputIdx_m].msgParam2   = msgParam2;
	strncpy( abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam3, msgParam3, sizeof(abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam3)-1 );
	strncpy( abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam4, msgParam4, sizeof(abaxisSendQueue_m[abaxisSendQueueOutputIdx_m])-1 );
		

	abaxisSendQueueInputIdx_m	= (abaxisSendQueueInputIdx_m + 1) % ABAXIS_SEND_QUEUE_SIZE;
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

void WaitForMsg(void)
{
	Hl7Msg_t				msg;
	static HANDLE			queue = 0;
	//static struct mq_attr	mqstat;
	DWORD					receivedSize = 0;
	char					startDate[20];
	char					endDate[20];
	DWORD					flags;
	BOOL					result;

	if ( queue == 0 ) {
		MSGQUEUEOPTIONS			options;
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = TRUE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;
		queue = CreateMsgQueue(HL7_QUEUE_NAME, &options);
	
		if ( queue == NULL ) {
			LogError( "failure to open HL7 queue" );
			exit( -1 );
		}
	}
		
	result = ReadMsgQueue(queue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);

	if ( result ) {
		if ( receivedSize > 0 ) {
			switch ( msg.msgId )
			{
				case MSG_HL7_TIMER:
					// decrement hl7 1 second timer if non-zero
					if ( hl7TimeoutSeconds_m > 0 ) {
						hl7TimeoutSeconds_m--;
					}
					break;

				case MSG_HL7_LAST_ROTOR_AVAILABLE:
					// queue HL7 message for last rotor run record available by database record ID
					QueueHl7TxMsg( ABAXIS_SEND_ROTOR_RUN_DONE_TYPE, msg.data );
					break;

				case MSG_HL7_SEND_RANGE:
					FormatDateTime( msg.rangeData.startDate, startDate, sizeof(startDate) );
					FormatDateTime( msg.rangeData.endDate, endDate,sizeof(endDate) );
					QueueHl7TxMsg( ABAXIS_SEND_RESULTS_RANGE_TYPE, 0, 0, startDate, endDate );
					break;

				default:
					LogWarning( "HL7 received unexpected message %d", msg.msgId );
					break;
			}
		}
	}
	else
	{
		LogError( "message receive error" );
	}
}	


// ********************************************************************
// FUNCTION:
//
//		PutHl7TxChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Put a ASCII char in HL7 frame buffer for write to HL7 port  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		next character
// 
// ********************************************************************/      

void PutHl7TxChar( char ch )
{
	hl7TxBuffer_m[hl7TxBufferIdx_m++] = ch;
}



// ********************************************************************
// FUNCTION:
//
//		SetHl7SeqNum() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set ASCII sequence number for HL7 frame buffer 
// _____________________________________________________________________
// PARAMETERS:
//
//		sequence number
// ____________________________________________________________________
// RETURNED:
//
//		Pointer to new sequence number
// 
// ********************************************************************/      

void SetHl7SeqNum(unsigned seqNum,char *textPtr)
{
	// max sequence number processed is 99 which is much greater than anticipated       

	if (seqNum / 10) {
		*textPtr++ = (seqNum / 10) + '0';
	}
	*textPtr = (seqNum % 10) + '0';
}	


// ********************************************************************
// FUNCTION:
//
//		SuppressBlanksFromHl7MethodString() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Suppress leading & trailing blanks in method fields for HL7 frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		Source string
// ____________________________________________________________________
// RETURNED:
//
//		String with suppressed spaces
// 
// ********************************************************************/      

char *SuppressBlanksFromHl7MethodString(char *sourcePtr, char *suppressedPtr)
{
	bool	gotNonBlank;
	char	*suppressedStr;

	// save pointer to final suppressed text for return       

	suppressedStr = suppressedPtr;

	// suppress leading blanks and copy back to suppressed string for non-empty strings       

	gotNonBlank = false;

	while (*sourcePtr)								// process source method string for non null chars       
	{
		if (*sourcePtr != ' ')	{					// not a null char or a blank char        
			gotNonBlank = true;
			*suppressedPtr++ = *sourcePtr;			// set in suppressed text string       
		}
		else if (gotNonBlank)	{					// blank char but not leading blank, so keep it       
			*suppressedPtr++ = *sourcePtr;			// set in suppressed text string       
		}

		// default else: leading blanks not copied to suppressed string       

		sourcePtr++;								// in all cases incr for next source method string char       
	}

	// for strings with non blank chars, suppress any trailing blanks in method text string       

	if (gotNonBlank)	{							// if source method text string has non blank char       
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
//		Set up OBR1 & OBR2 record comment strings for HL7 frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		Comment string
// 
// ********************************************************************/      

char *SetupOrecordCtypeGtext( char *commentTextPtr )
{
	if (hl7TxOBRsequenceNumber_m == 1) {
		switch ( hl7TxNTEsequenceNumber_m )
		{
			case 1:		// OBR record 1: NTE record 1: "INST QC: OK    CHEM: OK"
				commentTextPtr = (char *) qcOkTextString;
				break;

			case 2:		// OBR record 1: NTE record 2: "HEM X ,  LIP Y ,  ICT Z"
				_snprintf( commentTextPtr,(METHOD_TEXT_SIZE-1), "%s %s  %s %s  %s %s", hemTextString, hl7Record_m->o1C2HemString, lipTextString, hl7Record_m->o1C2LipString, ictTextString, hl7Record_m->o1C2IctString );      
				break;

			case 3:		// OBR record 1: NTE record 3: "@ CONFIRM LOW RECOVERIES @", if error condition
				if (hl7Record_m->o1C3RotorDilutionCheckError) 
				{ 
					commentTextPtr = (char *) rotorDiluteCheckFailedString;
				}
				break;

			case 4:		// OBR record 1: NTE record 4: operator expired or operator unauthorized string, if security issue
					commentTextPtr = hl7Record_m->o1C4OperatorSecurityStatus;
				break;

			case 5:		// OBR record 1: NTE record 5: control expired string, if control issue
					commentTextPtr = hl7Record_m->o1C5ControlExpired;
				break;

			default:
				// not supported then string copy for OBR1 comment        
				break;
		}
	}
	else
	{
		if (hl7TxNTEsequenceNumber_m == 1)	{			// OBR record 2: NTE record 1: "CHEMISTRY QC:         X"       
			_snprintf( commentTextPtr, (METHOD_TEXT_SIZE-1), "%s %s", chemQcTextString, hl7Record_m->o2C1RqcResultString );      
		}
		else	{										// OBR record 2: NTE record 2: "ACCEPTABLE MINIMUM   YY"       
			_snprintf( commentTextPtr, (METHOD_TEXT_SIZE-1), "%s %s", acceptableMinTextString, hl7Record_m->o2C2RqcLowLimitString );      
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
//		Set up OBXn records NTE1 comment code string for result analyte flags 
//		for HL7 frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		NTE comment string
// 
// ********************************************************************/      

void SetupRnC1Code( char *methodTextPtr ) 
{
	unsigned long	analyteFlags;

	analyteFlags = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnC1AnalyteFlags;

	_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%4.4lX %4.4lX", analyteFlags >> 16, analyteFlags & 0xFFFF );				
}


// ********************************************************************
// FUNCTION:
//
//		SetupRnC1Text() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up OBXn records NTE1 comment text string for result analyte flag type 
//		for HL7 frame buffer methods  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		The record string
// 
// ********************************************************************/      

char *SetupRnC1Text( void )
{
	unsigned		i;
	unsigned long	analyteFlags;

	const Hl7RnC1TextData_t	hl7RnC1TextData[NUM_RN_C1_MASKS] =
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



	analyteFlags = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnC1AnalyteFlags;

	// check masked result flags in order of priority and set up returned ptr to comment text        

	for ( i = 0; i < NUM_RN_C1_MASKS; i++ )
	{
		if ( (analyteFlags & hl7RnC1TextData[i].analyteFlagMask) != 0 ) {
			return (char*) hl7RnC1TextData[i].hl7RnC1Text;
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
//		The abort comment string
// 
// ********************************************************************/      

void SetupAbortComment(unsigned commentNumber, char *commentTextPtr, int size)
{
	switch ( commentNumber )
	{
		case 1:		// for aborted Error Rotor Message OBR1NTE1 (run count & abort count)

			_snprintf( commentTextPtr, size, "%c%s%c%s%c%c%s%c%s",hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																  hl7Record_m->o1C1RunCount,
																  hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																  hl7RunCountString,
																  hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																  hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																  hl7Record_m->o1C1AbortCount,
																  hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																  hl7AbortCountString );      
			break;

		case 2:		// for aborted Error Rotor Message OBR1NTE2 (print count & flash count)
			_snprintf( commentTextPtr, size, "%c%s%c%s%c%c%s%c%s",	hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																	hl7Record_m->o1C2PrintCount,
																	hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																	hl7PrintCountString,
																	hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																	hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																	hl7Record_m->o1C2FlashCount,
																	hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																	hl7FlashCountString );      
			break;

		case 3:		// for aborted Error Rotor Message OBR1NTE3 (DAC trim flags & offset error flags & system flags)
			_snprintf( commentTextPtr, size, "%c%4.4X%c%s%c%c%4.4X%c%s%c%c%4.4lX %4.4lX%c%s", hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7Record_m->o1C3DacTrimFlags,
																							hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7DacTrimFlagsString,
																							hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																							hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7Record_m->o1C3OffsetErrorFlags,
																							hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7OffsetErrFlagsString,
																							hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																							hl7Record_m->o1C3SystemFlags >> 16,
																							hl7Record_m->o1C3SystemFlags & 0xFFFF,
																							hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																							hl7SystemFlagsString );      
			break;

		case 4:		// for aborted Error Rotor Message OBR1NTE4 (bead check 1 flags and bead check 2 flags)
			_snprintf( commentTextPtr, size, "%c%4.4lX %4.4lX%c%s%c%c%4.4lX %4.4lX%c%s", hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					 hl7Record_m->o1C4BeadCheck1Flags >> 16,
																					 hl7Record_m->o1C4BeadCheck1Flags & 0xFFFF,
																					 hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					 hl7BeadCheck1String,
																					 hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																					 hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					 hl7Record_m->o1C4BeadCheck2Flags >> 16,
																					 hl7Record_m->o1C4BeadCheck2Flags & 0xFFFF,
																					 hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					 hl7BeadCheck2String );      
			break;

		case 5:		// for aborted Error Rotor Message OBR1NTE5	(empty cuvette flags & distribution check flags)
			_snprintf( commentTextPtr, size, "%c%4.4lX %4.4lX%c%s%c%c%4.4lX %4.4lX%c%s", hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																						hl7Record_m->o1C5EmptyCuvFlags >> 16,
																						hl7Record_m->o1C5EmptyCuvFlags & 0xFFFF,
																						hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																						hl7EmptyCuvFlagsString,
																						hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																						hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																						hl7Record_m->o1C5DistCheckFlags >> 16,
																						hl7Record_m->o1C5DistCheckFlags & 0xFFFF,
																						hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																						hl7DistCheckFlagsString );      
			break;

		case 6:		// for aborted Error Rotor Message OBR1NTE6	(quality control flags & offset SD error flags & wavelength CV flags)
			_snprintf( commentTextPtr, size, "%c%4.4X%c%s%c%c%4.4X%c%s%c%c%4.4X%c%s", hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7Record_m->o1C6QcFlags,
																					hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7QcFlagsString,
																					hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																					hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7Record_m->o1C6OffsetSdErrorFlags,
																					hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7OffsetSdErrString,
																					hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter,
																					hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7Record_m->o1C6DacWavelengthCvFlags,
																					hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																					hl7WavelengthCvString);      
				break;

		case 7:		// for aborted Error Rotor Message OBR1NTE7	(system error number & system error string - short text)
			_snprintf( commentTextPtr, size, "%c%4.4X%c%s", hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
														  hl7Record_m->o1C7SystemErrorNumber,
														  hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
														  hl7Record_m->o1C7SystemErrorString );      
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
//		Set up parameters for no query results comment HL7 frame buffer method  														
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		The comment string
// 
// ********************************************************************/      

void SetupNoQueryresultsCommentText(char *commentTextPtr) 
{
	_snprintf( commentTextPtr, (METHOD_TEXT_SIZE-1), "%s:%s:%s:%s", hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue, hl7RxQueryData_m->hl7RxQueryBeginDateTime, hl7RxQueryData_m->hl7RxQueryEndDateTime, hl7NoQueryResultString);      
}

// ********************************************************************
// FUNCTION:
//
//		ExtractTxConfigFields() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Lookup the configuration file which fileld should be used  
// _____________________________________________________________________
// PARAMETERS:
//
//		Next field to be processed
// ____________________________________________________________________
// RETURNED:
//
//		Field to be used
// 
// ********************************************************************/      

unsigned short ExtractTxConfigFields(Hl7EscMethod_t method)
{
unsigned short retMethod = method;
	switch (method)
	{
		case HL7_MSH_DATE_TIME_OF_MESSAGE:		//MSH-7
			retMethod = hl7Configuration_m->segmentMSHOut.mshTimeofMessage;
			break;
		case HL7_MSH_MESSAGE_TYPE:				//MSH-9
			retMethod = hl7Configuration_m->segmentMSHOut.mshMessageType;
			break;
		case HL7_MSH_MESSAGE_CONTROL_ID:		//MSH-10
			retMethod = hl7Configuration_m->segmentMSHOut.mshMessageControlID;
			break;
		case HL7_MSH_PROCESSING_ID:				//MSH-11
			retMethod = hl7Configuration_m->segmentMSHOut.mshProcessingID;
			break;

		case HL7_PID_SET_ID:					//PID-1 PID patient record methods
			retMethod = hl7Configuration_m->segmentPID.pidSetID;
			break;
		case HL7_PID_EXTERNAL_ID:				//PID-2
			retMethod = hl7Configuration_m->segmentPID.pidPatientIDExternal;
			break;
		case HL7_PID_INTERNAL_ID:				//PID-3
			retMethod = hl7Configuration_m->segmentPID.pidPatientIDInternal;
			break;
		case HL7_PID_ALTERNATE_ID:				//PID-4
			retMethod = hl7Configuration_m->segmentPID.pidAlternatePatientID;
			break;
		case HL7_PID_PATIENT_NAME:				//PID-5
			retMethod = hl7Configuration_m->segmentPID.pidPatientName;
			break;
		case HL7_PID_BIRTHDATE:					//PID-7
			retMethod = hl7Configuration_m->segmentPID.pidDateTimeofBirth;
			break;
		case HL7_PID_PATIENT_SEX:				//PID-8
			retMethod = hl7Configuration_m->segmentPID.pidSex;
			break;
		case HL7_PID_PATIENT_RACE:				//PID-10
			retMethod = hl7Configuration_m->segmentPID.pidRace;
			break;
		case HL7_PID_PHONE_NUMBER:				//PID-13
			retMethod = hl7Configuration_m->segmentPID.pidPhoneNumberHome;
			break;

		case HL7_PV1_ASSIGNED_LOCATION:			//PV1-3
			retMethod = hl7Configuration_m->segmentPV1.pv1AssignedPatientLocation;
			break;
		case HL7_PV1_ADMITTING_DOCTOR:			//PV1-19
			retMethod = hl7Configuration_m->segmentPV1.pv1VisitNumber;
			break;
		case HL7_OBR_SET_ID:					//OBR-1 OBR order record methods       	
			retMethod = hl7Configuration_m->segmentOBR.obrSetID;
			break;
		case HL7_OBR_REQUESTED_DATE_TIME:		//OBR-6		
			retMethod = hl7Configuration_m->segmentOBR.obrRequestedDateTime;
			break;
		case HL7_OBR_OBSERVATION_START_TIME:	//OBR-7
			retMethod = hl7Configuration_m->segmentOBR.obrObservationDateTime;
			break;
		case HL7_OBR_OBSERVATION_END_TIME:		//OBR-8
			retMethod = hl7Configuration_m->segmentOBR.obrObservationEndDateTime;
			break;
		case HL7_OBR_COLLECTOR_ID:				//OBR-10
			retMethod = hl7Configuration_m->segmentOBR.obrCollectorIdentifier;
			break;
		case HL7_OBR_PHYSICIAN_ID:				//OBR-16
			retMethod = hl7Configuration_m->segmentOBR.obrOrderingProvider;
			break;
		case HL7_OBR_RESULT_STATUS:				//OBR-25
			retMethod = hl7Configuration_m->segmentOBR.obrResultStatus;
			break;

		case HL7_OBX_SET_ID:					//OBX-1 OBX result record methods       	
			retMethod = hl7Configuration_m->segmentOBX.obxSetID;
			break;
		case HL7_OBX_ID:						//OBX-3
			retMethod = hl7Configuration_m->segmentOBX.obxObservationIdentifier;
			break;
		case HL7_OBX_VALUE:						//OBX-5
			retMethod = hl7Configuration_m->segmentOBX.obxObservationValue;
			break;
		case HL7_OBX_UNITS:						//OBX-6
			retMethod = hl7Configuration_m->segmentOBX.obxUnits;
			break;
		case HL7_OBX_REFERENCE_RANGE:			//OBX-7
			retMethod = hl7Configuration_m->segmentOBX.obxReferenceRange;
			break;
		case HL7_OBX_ABNORMAL_FLAG:				//OBX-8
			retMethod = hl7Configuration_m->segmentOBX.obxAbnormalFlags;
			break;
		case HL7_OBX_RESULT_STATUS:				//OBX-11
			retMethod = hl7Configuration_m->segmentOBX.obxObservResultStatus;
			break;
		case HL7_OBX_OBSERVATION_METHOD:		//OBX-17
			retMethod = hl7Configuration_m->segmentOBX.obxObservationMethod;
			break;

		case HL7_NTE_SET_ID:					//NTE-1 NTE comment record methods - result
			retMethod = hl7Configuration_m->segmentNTE.nteSetID;
			break;
		case HL7_NTE_COMMENT_RESULT_RAW:		//NTE-2
			retMethod = hl7Configuration_m->segmentNTE.nteCommentSource;
			break;
		case HL7_NTE_COMMENT_TEXT:				//NTE-3
			retMethod = hl7Configuration_m->segmentNTE.nteCommentText;
			break;
		case HL7_NTE_COMMENT_TYPE:				//NTE-4
			retMethod = hl7Configuration_m->segmentNTE.nteCommentType;
			break;

		case HL7_QRD_DATE_TIME:					//QRD-1 QRD record methods - handled as generic field strings       
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryDateTime;
			break;
		case HL7_QRD_FORMAT_CODE:				//QRD-2
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryFormatCode;
			break;
		case HL7_QRD_PRIORITY:					//QRD-3
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryPriority;
			break;
		case HL7_QRD_ID:						//QRD-4
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryID;
			break;
		case HL7_QRD_DEFERRED_RESPONSE_TYPE:	//QRD-5
			retMethod = hl7Configuration_m->segmentQRD.qrdDeferredResponseType;
			break;
		case HL7_QRD_DEFERRED_RESPONSE_DATE:	//QRD-6
			retMethod = hl7Configuration_m->segmentQRD.qrdDeferredResponseDateTime;
			break;
		case HL7_QRD_QUALITY_REQUEST:			//QRD-7
			retMethod = hl7Configuration_m->segmentQRD.qrdQuantityLimitedRequest;
			break;
		case HL7_QRD_WHO_SUBJECT_FILTER:		//QRD-8
			retMethod = hl7Configuration_m->segmentQRD.qrdWhoSubjectFilter;
			break;
		case HL7_QRD_WHAT_SUBJECT_FILTER:		//QRD-9
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatSubjectFilter;
			break;
		case HL7_QRD_WHAT_DEPT_DATA_CODE:		//QRD-10
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatDepartmentDataCode;
			break;
		case HL7_QRD_WHAT_DEPT_VALUE_QUAL:		//QRD-11
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatDataCodeValueQual;
			break;

		//All Fields unsupported by the configuration file
		default:	
			break;
	}
	return retMethod;
}

// ********************************************************************
// FUNCTION:
//
//		ExtractRxConfigFields() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Lookup the configuration file which field should be used  
// _____________________________________________________________________
// PARAMETERS:
//
//		Next field to be processed
// ____________________________________________________________________
// RETURNED:
//
//		The field to be  used
// 
// ********************************************************************/      
unsigned short ExtractRxConfigFields(unsigned short method)
{
unsigned short retMethod = method;
	switch (method)
	{
		case HL7_MSH_SENDING_APPLICATION:			//MSH-3
			retMethod = hl7Configuration_m->segmentMSHIn.mshSendingApplication;
			break;
		case HL7_MSH_SENDING_FACILITY:				//MSH-4
			retMethod = hl7Configuration_m->segmentMSHIn.mshSendingFacility;
			break;
		case HL7_MSH_DATE_TIME_OF_MESSAGE:			//MSH-7
			retMethod = hl7Configuration_m->segmentMSHIn.mshTimeofMessage;
			break;
		case HL7_MSH_MESSAGE_TYPE:					//MSH-9
			retMethod = hl7Configuration_m->segmentMSHIn.mshMessageType;
			break;
		case HL7_MSH_MESSAGE_CONTROL_ID:			//MSH-10
			retMethod = hl7Configuration_m->segmentMSHIn.mshMessageControlID;
			break;
		case HL7_MSH_PROCESSING_ID:					//MSH-11
			retMethod = hl7Configuration_m->segmentMSHIn.mshProcessingID;
			break;
		case HL7_QRD_DATE_TIME:						//QRD-1 record methods - handled as generic field strings       
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryDateTime;
			break;
		case HL7_QRD_FORMAT_CODE:					//QRD-2
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryFormatCode;
			break;
		case HL7_QRD_PRIORITY:						//QRD-3
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryPriority;
			break;
		case HL7_QRD_ID:							//QRD-4
			retMethod = hl7Configuration_m->segmentQRD.qrdQueryID;
			break;
		case HL7_QRD_DEFERRED_RESPONSE_TYPE:		//QRD-5
			retMethod = hl7Configuration_m->segmentQRD.qrdDeferredResponseType;
			break;
		case HL7_QRD_DEFERRED_RESPONSE_DATE:		//QRD-6
			retMethod = hl7Configuration_m->segmentQRD.qrdDeferredResponseDateTime;
			break;
		case HL7_QRD_QUALITY_REQUEST:				//QRD-7
			retMethod = hl7Configuration_m->segmentQRD.qrdQuantityLimitedRequest;
			break;
		case HL7_QRD_WHO_SUBJECT_FILTER:			//QRD-8
			retMethod = hl7Configuration_m->segmentQRD.qrdWhoSubjectFilter;
			break;
		case HL7_QRD_WHAT_SUBJECT_FILTER:			//QRD-9
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatSubjectFilter;
			break;
		case HL7_QRD_WHAT_DEPT_DATA_CODE:			//QRD-10
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatDepartmentDataCode;
			break;
		case HL7_QRD_WHAT_DEPT_VALUE_QUAL:			//QRD-11
			retMethod = hl7Configuration_m->segmentQRD.qrdWhatDataCodeValueQual;
			break;
		default:
			break;
	}
	//Process unsuported fields that may arrive from Host with an content
	if (retMethod == 0) {
		retMethod = method;
	}

	return retMethod;
}

// ********************************************************************
// FUNCTION:
//
//		ProcessTxEscMethod() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process method for formatting ASCII data in HL7 transmit message frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		escMethod - next field to be processed
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessTxEscMethod(Hl7EscMethod_t escMethod)
{
	char		*methodTextPtr;
	char		methodText[METHOD_TEXT_SIZE];
	int			retValue = 0;
	unsigned short convertedEscMethod;

	// init  methodText[] temp working buffer with working ptr to all '\0' null chars      
	memset(methodText, '\0', sizeof( methodText ));
	 
	methodTextPtr = methodText;
	//Check the configuration file which method to use
	convertedEscMethod = ExtractTxConfigFields(escMethod);

	// construct method text string based on method type for identified HL7 field       

	// HL7 Message Header Record  (variable fields)       
	switch (convertedEscMethod)
	{
		case HL7_MSH_FIELD_DELIMITER:						//MSH-1
			methodText[0] = hl7Configuration_m->segmentMSHOut.mshFieldDelimiter;
			break;

		case HL7_MSH_COMPONENT_DELIMITER:					//MSH-2
			methodText[0] = hl7Configuration_m->segmentMSHOut.mshComponentDelimiter;
			break;

		case HL7_MSH_REPEAT_DELIMITER:						//MSH-2
			methodText[0] = hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter;
			break;

		case HL7_MSH_ESCAPE_CHARACTER:						//MSH-2
			methodText[0] = hl7Configuration_m->segmentMSHOut.mshEscapeCharacter;
			break;

		case HL7_MSH_SUB_COMPONENT_DELIMITER:				//MSH-2
			methodText[0] = hl7Configuration_m->segmentMSHOut.mshSubComponentDelimiter;
			break;

		case HL7_MSH_ANALYZER_TYPE:							//MSH-4
			methodTextPtr = instrumentInfo_m->hAnalyzerName;					       
			break;

		case HL7_MSH_SOFTWARE_VERSION:						//MSH-4
			methodTextPtr = instrumentInfo_m->hSoftwareVersion;
			break;

		case HL7_MSH_SERIAL_NUMBER:							//MSH-4
			methodTextPtr = instrumentInfo_m->hSerialNumber;										       
			break;

		case HL7_MSH_DATE_TIME_OF_MESSAGE:      			//MSH-7
			methodTextPtr = GetCurrentDateTime();						// date/time when message sent (MSH record)						       
			break;

		case HL7_MSH_MESSAGE_TYPE:							//MSH-9
			if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE) {
				_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%s%c%s",	MSH3_RESULT_RESPONSE_ORF,
																			hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																			MSH3_RESULT_RESPONSE_R04 );       
			}
			else {
				_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%s%c%s",	MSH3_RESULT_UNSOLICITED_RESPONSE_ORU,
																			hl7Configuration_m->segmentMSHOut.mshComponentDelimiter,
																			MSH3_RESULT_UNSOLICITED_RESPONSE_R01 );       
			}
			break;

		case HL7_MSH_MESSAGE_CONTROL_ID:					//MSH-10
			//Not implemented in ASTM, however this is a HL7 required fied
			methodTextPtr = hl7RxQueryData_m->hl7RxMSHMsgDateTime;
			break;

		case HL7_MSH_PROCESSING_ID:       					//MSH-11
			methodText[0] = instrumentInfo_m->hProcessingId;
			break;

		case 	HL7_MSA_ACK_CODE:							//MSA-1				// Message acknowledge record
			if (hl7ErrorDuringRxMsg_m == false) {
				methodTextPtr = APPLICATION_ACCEPT;
			}
			else {
				methodTextPtr = APPLICATION_ERROR;
			}
			break;

		case 	HL7_MSA_MSG_CONTROL_ID:						//MSA-2
			//Not supported by Abaxis or ASTM, but MSA-2 is a required field
			methodTextPtr = hl7RxQueryData_m->hl7RxMSHMsgDateTime;
			break;

		case 	HL7_MSA_TEXT_MSG:
			if (hl7ErrorDuringRxMsg_m == true) {
				methodTextPtr = (char *) hl7CommentStrings[hl7ErrorType_m];
			}
			break;

		case 	HL7_MSA_ERROR_CONDITION:					//MSA-6
			break;


		case 	HL7_QRD_DATE_TIME:							//QRD-1		QRD record methods - handled as generic field strings       
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDDateTime;
			break;

		case 	HL7_QRD_FORMAT_CODE:						//QRD-2
			methodText[0] = hl7RxQueryData_m->hl7RxQRDFormatCode;
			break;

		case 	HL7_QRD_PRIORITY:							//QRD-3
			methodText[0] = hl7RxQueryData_m->hl7RxQRDPriority;
			break;

		case 	HL7_QRD_ID:									//QRD-4
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDID;
			break;

		case 	HL7_QRD_DEFERRED_RESPONSE_TYPE:				//QRD-5
			methodText[0] = hl7RxQueryData_m->hl7RxQRDDeferredResponseType;
			break;

		case 	HL7_QRD_DEFERRED_RESPONSE_DATE:				//QRD-6
			methodText[0] = hl7RxQueryData_m->hl7RxQRDDeferredResponseDate;
			break;

		case 	HL7_QRD_QUALITY_REQUEST:					//QRD-7
			methodText[0] = hl7RxQueryData_m->hl7RxQRDQualityRequest;
			break;

		case 	HL7_QRD_WHO_SUBJECT_FILTER:					//QRD-8
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDWhoSubjectFilter;
			break;

		case 	HL7_QRD_WHAT_SUBJECT_FILTER:				//QRD-9
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDWhatSubjectFilter;
			break;

		case 	HL7_QRD_WHAT_DEPT_DATA_CODE:				//QRD-10
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue;
			break;

		case 	HL7_QRD_WHAT_DEPT_VALUE_QUAL:				//QRD-11
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDWhatDataCodeValueQual;
			break;

		case HL7_PID_SET_ID:					//PID-1
			hl7TxOBRsequenceNumber_m = 1;		// init for first OBR record       
			hl7TxNTEtype_m = '\0';				// init for no comment type set       
			methodTextPtr = PATIENT_SEQUENCE_NUMBER;// always 1
			break;

		case HL7_NO_QUERY_RESULT_PID_ID:		//PID-3
			methodTextPtr = hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue;       				
			break;

		case HL7_OBR_SET_ID:					//OBR-1			     
			SetHl7SeqNum(hl7TxOBRsequenceNumber_m, methodTextPtr);	
			break;

		case HL7_OBR_SETUP_RESULTS:										// Abaxis implementation specific       
		case HL7_ABORT_OBR_SETUP:							       
			hl7AnalyteIdx_m = 0;										// init index to analyte results info struct in Hl7Record_t      

			hl7TxNTEtype_m = 'O';										// init for OBR record NTE record type
			hl7TxNTEsequenceNumber_m = 1;								// init for first NTE record       

			hl7TxOBXsequenceNumber_m = 1;								// init for first OBX record       
			break;


		case HL7_NTE_SET_ID:					//NTE-1
			SetHl7SeqNum(hl7TxNTEsequenceNumber_m, methodTextPtr);	
			break;

		case HL7_NO_QUERY_RESULT_NTE_TEXT:		//NTE-3
			SetupNoQueryresultsCommentText( methodTextPtr );
			break;

		case HL7_NTE_CODE:						//NTE-3
			methodText[0] = abaxisSendParam2_m;
			break;

		case HL7_NTE_TEXT:						//NTE-3
			methodTextPtr = (char *) hl7CommentStrings[abaxisSendParam1_m];
			break;

		case HL7_NTE_COMMENT_TYPE:				//NTE-4
			if ((hl7TxNTEtype_m == 'O') && (hl7TxOBRsequenceNumber_m < 3)) {
				// for O record 1 NTE record 1, OBR record 1 NTE record 2, OBR record 2 NTE record 1, OBR record 2 NTE record 2       
				methodText[0] = 'G'; 
			}
			else {
				// for all other OBR records and OBX records       
				methodText[0] = 'I';
			}
			hl7TxNTEsequenceNumber_m++;							// incr NTE record seq number	      
			break;


		default:

			//Process records containing query information
			if (hl7Record_m != NULL) {
				switch (convertedEscMethod)
				{
					case HL7_PID_EXTERNAL_ID:				//PID-2
						methodTextPtr = hl7Record_m->pSampleId;	       
						break;
					case HL7_PID_INTERNAL_ID:				//PID-3
						methodTextPtr = hl7Record_m->pPatientControlId;											       
						break;

					case HL7_PID_ALTERNATE_ID:				//PID-4
						methodTextPtr = hl7Record_m->pAlternateId;				       
						break;

					case HL7_PID_BIRTHDATE:					//PID-7
						methodTextPtr = hl7Record_m->pDateOfBirth;					// get birthdate, patient date of birth (DOB)        
						break;

					case HL7_PID_PATIENT_SEX:				//PID-8				
						methodText[0] = hl7Record_m->pGender;						// get patient sex, gender						       
						break;

					case HL7_PID_PATIENT_RACE:				//PID-10
						methodTextPtr[0] = hl7Record_m->pRace;						// get patient race-ethnic origin						       
						break;

					case HL7_PID_PHONE_NUMBER:				//PID-13	      
						methodTextPtr = hl7Record_m->pPhoneNumber;					// get patient phone number						       
						break;

					//       HL7 Patient Visit Record  (variable fields)       
					case HL7_PV1_ASSIGNED_LOCATION:			//PV1-3		     
						methodTextPtr = hl7Record_m->pLocation;							       
						break;

					case HL7_PV1_ADMITTING_DOCTOR:			//PV1-19			     
						_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%s%c%s%c%s", hl7Record_m->pOwnerAdmissionId,hl7Configuration_m->segmentMSHOut.mshComponentDelimiter, hl7Record_m->pAge, hl7Configuration_m->segmentMSHOut.mshComponentDelimiter, hl7Record_m->pLastVaccination );       
						break;
						
						
					//       HL7 Test Order Record  (variable fields)       

					case HL7_OBR_ROTOR_IQC_NAME:			//OBR-4
						if (hl7TxOBRsequenceNumber_m == 1) {
							methodTextPtr = hl7Record_m->oRotorName;				// sample rotor disc name
						}
						else	{													       
							methodTextPtr = (char *) iqcReportHeaderTextString;		// Quality Control Report header (in rotor name field)
						}
						break;

					case HL7_OBR_LOT_NUM:					//OBR-4
						methodTextPtr = hl7Record_m->oRotorLotNumber;				        
						break;

					case HL7_ABORT_OBR_ROTOR_NAME_LOT:		//OBR-4				// (for aborted rotors)
						if (( hl7Record_m->oRotorName[0] != '\0' )	|| ( hl7Record_m->oRotorLotNumber[0] != '\0' )) {
							_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%c%c%c%s: %s",hl7Configuration_m->segmentMSHOut.mshComponentDelimiter, hl7Configuration_m->segmentMSHOut.mshComponentDelimiter, hl7Configuration_m->segmentMSHOut.mshComponentDelimiter, hl7Record_m->oRotorName, hl7Record_m->oRotorLotNumber );       
						} 
						break;

					case HL7_OBR_REQUESTED_DATE_TIME:		//OBR-6
						methodTextPtr = hl7Record_m->oRunDateTime;					// date/time when rotor run						       
						break;

					case HL7_OBR_OBSERVATION_START_TIME:	//OBR-7
						methodTextPtr = observationStartTime;
						break;

					case HL7_OBR_OBSERVATION_END_TIME:		//OBR-8
						methodTextPtr = observationEndTime;
						break;

					case HL7_OBR_COLLECTOR_ID:				//OBR-10
						methodTextPtr = hl7Record_m->oOperatorId;					// set collector ID (e.g. operator)       			
						break;

					case HL7_OBR_PHYSICIAN_ID: 				//OBR-16
						methodTextPtr = hl7Record_m->oDoctorId;					// set alternate ID (e.g. physician)       			
						break;

					case HL7_OBR_RESULT_STATUS:				//OBR-25
						if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE)		// processing results query sends       	
						{
							methodText[0] = 'Q';									// set Q flag       			
						}
						else														// for all other results types       	
						{
							methodText[0] = 'F';									// set F flag       			
						}
						break;

				// HL7 Result Record  (variable fields)       
	
					case HL7_OBX_SET_ID:					//OBX-1
						SetHl7SeqNum(hl7TxOBXsequenceNumber_m, methodTextPtr);	
						break;

					case HL7_OBX_ID:						//OBX-3
						if (hl7TxOBRsequenceNumber_m == 1)	{						// set hl7 analyte name: OBR record 1         
							methodTextPtr = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnUniversalTestId;
						}
						else	{													// set IQC level name:    OBR record 2       
							methodTextPtr = (char *) hl7Record_m->iqcHl7LevelNameStrings[hl7TxOBXsequenceNumber_m - 1];
						}
						break;

					case HL7_OBX_VALUE:						//OBX-5
						if (hl7TxOBRsequenceNumber_m == 1)	{						// set analyte result value:   OBR record 1        
							methodTextPtr = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnResultString;
						}
						else	{													// set IQC level result value: OBR record 2       
							methodTextPtr = hl7Record_m->rIqcResultsStrings[hl7TxOBXsequenceNumber_m - 1];
						}
						break;

					case HL7_OBX_UNITS:						//OBX-6
						if (hl7TxOBRsequenceNumber_m == 1)	{						// set analyte result units: OBR record 1 only        
							methodTextPtr = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnUnitsString;
						}

						// no units sent for Quality Control Report (IQC levels)        
						break;

					case HL7_OBX_REFERENCE_RANGE:			//OBX-7
						if (hl7TxOBRsequenceNumber_m == 1)	{						// set hl7 analyte result reference range: OBR record 1        
							if (( hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnLoRefRangeString[0] != '\0' ) && ( hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnHiRefRangeString[0] != '\0' )) {
								_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%s to %s", hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnLoRefRangeString, hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnHiRefRangeString );       
							}
						}
						else														// set hl7 IQC level limit range:          OBR record 2       
						{
							if (hl7TxOBXsequenceNumber_m < 9)	{				// set LEVEL 1 limits       
								methodTextPtr = (char *) iqcLimitStringLevel1;
							}
							else{												// set LEVEL 2 PRE & wavelength limits       
								methodTextPtr = (char *) iqcLimitStringLevel2;
							}
						}
						break;

					case HL7_OBX_ABNORMAL_FLAG:				//OBX-8
						if (hl7TxOBRsequenceNumber_m == 1)	{						// process analyte result flags: OBR record 1 only       
							methodText[0] = hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnAbnormalFlag;
						}

						// no flags processed for Quality Control Report (IQC levels)        
						break;

					case HL7_OBX_RESULT_STATUS:				//OBX-11

						if (hl7TxOBRsequenceNumber_m == 1)	{						// process analyte result previously sent status: OBR record 1 only       
							if ( hl7Record_m->hl7ResultRecordSent != 0 ) {
								methodText[0] = 'R';		// this result was previously transmitted
							}
							else {
								methodText[0] = 'F';		// final results
							}
						}
						else {
								methodText[0] = 'F';		// final results
						}

						// no flags processed for Quality Control Report (IQC levels)        
						break;
					case HL7_OBX_OBSERVATION_METHOD:		//OBX-17
		   				methodTextPtr = hl7Record_m->pReferenceRangeLabel;			// get reference range label, special 2						       
						break;

					case HL7_OBX_SETUP_NEXT:										// Abaxis implementation specific       
						hl7TxNTEtype_m = 'R';
						hl7TxNTEsequenceNumber_m = 1;								// init for first NTE record       

						if (hl7TxOBRsequenceNumber_m == 1)	{						// process NTE record: OBR record 1 only       
							// process OBXnNTE1 record only if error condition flags for final result
							 
							if ((hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnC1ResultPrintFlags & HL7_RESULT_FLAGS_COMMENT_MASK) == 0) {
								hl7TxNTEsequenceNumber_m |= HL7_TX_NTE_SKIP;
							}
						}
						break;

				// HL7 Comment Record  (variable fields)       
					case HL7_NTE_COMMENT_RESULT_RAW:		//NTE-2
						if (hl7TxNTEtype_m == 'R') {
							_snprintf( methodTextPtr, (METHOD_TEXT_SIZE-1), "%f", hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnC1ResultRaw );      
						}

						// not set for OBR record 1 NTE record 1, OBR record 1 NTE record 2, OBR record 2 NTE record 1, OBR record 2 NTE record 2       
						break;

					case HL7_NTE_COMMENT_CODE:				//NTE-3
						if (hl7TxNTEtype_m == 'R') {
							SetupRnC1Code( methodTextPtr );
						}

						// not set for OBR record 1 NTE record 1, OBR record 1 NTE record 2, OBR record 2 NTE record 1, OBR record 2 NTE record 2       
						break;

					case HL7_NTE_COMMENT_TEXT:				//NTE-3
						if ((hl7TxNTEtype_m == 'O') && (hl7TxOBRsequenceNumber_m < 3)) {
							// for O record 1 NTE record 1, OBR record 1 NTE record 2, OBR record 1 NTE record 3, OBR record 1 NTE record 4, OBR record 1 NTE record 5, OBR record 2 NTE record 1, OBR record 2 NTE record 2       

							methodTextPtr = SetupOrecordCtypeGtext( methodTextPtr );
						}
						else {
							if (hl7TxNTEtype_m == 'R') {
								methodTextPtr = SetupRnC1Text();
							}
						}
						break;

					case HL7_ABORT_NTE_COMMENT_TEXT:		//NTE-3
						// process rotor aborted error incr NTE record seq number
						SetupAbortComment(hl7TxNTEsequenceNumber_m, methodTextPtr, (sizeof (methodText)-1));
						break;



					default:
						//Illegal field, keep it as blank
						break;
				}//end of switch
		}// end of if (hl7Record_m != NULL)
			break;
	}	// end switch


	// suppress leading & trailing blanks in methodTextPtr string - result in methodText[]       

	methodTextPtr = SuppressBlanksFromHl7MethodString(methodTextPtr, methodText);

	while(*methodTextPtr != '\0')		
	{

		PutHl7TxChar(*methodTextPtr++);
	}


}


// ********************************************************************
// FUNCTION:
//
//		NextHl7ResultString() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get pointer to next HL7 result ASCII string  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		The next result
// 
// ********************************************************************/      

char NextHl7ResultString(void)
{
	return hl7Record_m->rAnalyteResults[hl7AnalyteIdx_m].rnResultString[0];
}


// ********************************************************************
// FUNCTION:
//
//		ProcessRepeatResultCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to process method to repeat frame for HL7 transmit message records  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		The next record
// 
// ********************************************************************/      

char *ProcessRepeatResultCheck(void)
{
	if (hl7TxOBRsequenceNumber_m == 1)	{							// O record 1        
		hl7AnalyteIdx_m++;											// incr to next chem analyte results index

		// check if done with analyte R records since next result is NULL string or end of results

		if ( (NextHl7ResultString() == '\0') || (hl7AnalyteIdx_m >= MAX_RESULTS) ) {
			hl7TxOBRsequenceNumber_m++;								// set for next O record when R records all done       

			abaxisTxMsgDefPtr_m++;									// skip past this record       
		}
		else	{													// not done with R records yet       
			abaxisTxMsgDefPtr_m--;									// back past C record       
			abaxisTxMsgDefPtr_m--;									// and for next R record       
		}
	}
	else	{														// O record 2       
		if (hl7TxOBXsequenceNumber_m == last_hl7_tx_iqc_r_record_m)	{ // done with all QC R records       	
			abaxisTxMsgDefPtr_m++;									// skip past this record       
		}
		else	{													// not done with R records yet       
			abaxisTxMsgDefPtr_m--;									// decr back for next QC R record       
		}
	}		

	hl7TxOBXsequenceNumber_m++;										// incr for next R record seq num and/or QC index       

	return abaxisTxMsgDefPtr_m;							
}


// ********************************************************************
// FUNCTION:
//
//		ProcessSkipCommentCheck() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check to process method to skip comment for HL7 transmit message 'R' records  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		The next record
// 
// ********************************************************************/      

char *ProcessSkipCommentCheck(void)
{
	if (hl7TxNTEsequenceNumber_m & HL7_TX_NTE_SKIP) {
		abaxisTxMsgDefPtr_m++;						// skip past this NTE record       
	}		

	return abaxisTxMsgDefPtr_m;							
}


//
// ********************************************************************
// FUNCTION:
//
//		ProcessTxField() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check the delimiters defined in the configuration file
// _____________________________________________________________________
// PARAMETERS:
//
//		chazacter to be processed
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessTxCharacter(char frameChar)
{
	switch (frameChar)
	{
		case TEMPLATE_FIELD_DELIMITER:
			if (frameChar != hl7Configuration_m->segmentMSHOut.mshFieldDelimiter) {
				PutHl7TxChar(hl7Configuration_m->segmentMSHOut.mshFieldDelimiter);
			}
			else {
				PutHl7TxChar(frameChar);
			}
			break;
		case TEMPLATE_COMPONENT_DELIMITER:
			if (frameChar != hl7Configuration_m->segmentMSHOut.mshComponentDelimiter) {
				PutHl7TxChar(hl7Configuration_m->segmentMSHOut.mshComponentDelimiter);
			}
			else {
				PutHl7TxChar(frameChar);
			}
			break;
		case TEMPLATE_REPEAT_DELIMITER:
			if (frameChar != hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter) {
				PutHl7TxChar(hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter);
			}
			else {
				PutHl7TxChar(frameChar);
			}
			break;
		case TEMPLATE_ESCAPE_CHARACTER:
			if (frameChar != hl7Configuration_m->segmentMSHOut.mshEscapeCharacter) {
				PutHl7TxChar(hl7Configuration_m->segmentMSHOut.mshEscapeCharacter);
			}
			else {
				PutHl7TxChar(frameChar);
			}
			break;
		case TEMPLATE_SUB_COMPONENT_DELIMITER:
			if (frameChar != hl7Configuration_m->segmentMSHOut.mshSubComponentDelimiter) {
				PutHl7TxChar(hl7Configuration_m->segmentMSHOut.mshSubComponentDelimiter);
			}
			else {
				PutHl7TxChar(frameChar);
			}
			break;
		default:
			PutHl7TxChar(frameChar);
			break;
	}
}

// ********************************************************************
// FUNCTION:
//
//		SetupTxMessageFrames() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up an HL7 frame buffer for a record in an Abaxis transmit message  
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


	// process Abaxis message record flags       


	if (*abaxisTxMsgDefPtr_m == HL7_TX_COMMENT)	{							// handle NTE record skip sequencing       
		abaxisTxMsgDefPtr_m = ProcessSkipCommentCheck();
	}

	if (*abaxisTxMsgDefPtr_m == HL7_TX_RESULTS_REPEAT)	{					// handle variable R record sequencing       
		abaxisTxMsgDefPtr_m = ProcessRepeatResultCheck(); 
	}

	if (*abaxisTxMsgDefPtr_m == HL7_TX_MESSAGE_DONE)	{	                 // no more record templates to set up for this abaxis message	
        if (hl7Configuration_m->segmentBLK.blockEnd) {
            //ZZZ end of  block, add <EB> <CR>
            hl7TxBufferIdx_m = 0;
            hl7TxRetryCount_m = 0;
            PutHl7TxChar (EB);
            PutHl7TxChar ('\r');
#ifdef HL7_DEBUG_ENABLE
        LogDebug ("block end = 0x%2x 0x%2x", EB, '\r');
#endif
            WritePort (hl7TxBuffer_m, hl7TxBufferIdx_m);                        // transmit End of block
        }
        hl7TxBufferIdx_m = 0;
        hl7TxRetryCount_m = 0;
		return (ABAXIS_HL7_TX_MSG_TRANSMITTED);
	}

	//  get pointer to frame template to set up for this record 
	//	and increment pointer to message definition index for next frame setup/check       
	frameTemplatePtr = (char *) hl7TxRecordTemplatePtrs_m[ (int)*abaxisTxMsgDefPtr_m++ ];

    if (frameTemplatePtr == &hl7TxMSHrecord[0]) {
        // ZZZ start of block, add <SB> and <CR>
        if (hl7Configuration_m->segmentBLK.blockStart) {
            PutHl7TxChar (SB);
            //PutHl7TxChar ('\r');
        }
    }
	
	// Next, set up *** HL7 *** record "text" data in hl7 transmit buffer using template data       

	while(*frameTemplatePtr != '\0')											// process hl7 "text" till end of template       
	{
		frameTemplateChar = *frameTemplatePtr++;								// get this "text" char to process and incr pointer to next char       

		if (frameTemplateChar == ESC)	{										// process ESC method ?        
			//  Process ESC method and set "text" in hl7 tx buffer and incr ptr to next char in template       

			ProcessTxEscMethod((Hl7EscMethod_t )*frameTemplatePtr++);
		}
		else	{																// not an ESC method, just set this text char is hl7 transmit buffer 
			ProcessTxCharacter(frameTemplateChar);
		}
	}
	PutHl7TxChar('\r');						// set CR as last "text" char in hl7 frame       

	// Now, signal not done setting up all frames (records) for this abaxis message and exit       

	return (ABAXIS_HL7_TX_MSG_NOT_DONE);		
}


//
// ********************************************************************
// FUNCTION:
//
//		ReceivedHl7FrameChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Return ASCII char from HL7 received frame buffer  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		ASCII character
// 
// ********************************************************************/      

char ReceivedHl7FrameChar(void)
{
	return hl7RxBuffer_m[hl7RxBufferIdx_m++] & 0x7f;
}


// ********************************************************************
// FUNCTION:
//
//		GetValidRxChar() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Get a valid char from the HL7 received frame buffer
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

char GetValidRxChar()
{
	char	rxChar;

	// check to see if valid HL7 char				      
	//       														      
	// HL7 specifies char allowed in record	      

	rxChar = ReceivedHl7FrameChar();

	if ( (rxChar != CR) && ((rxChar < ' ') || (rxChar >= DEL)) ) {
		return (char) NOT_VALID_HL7_CHAR;
	}
	return (rxChar);
}


// ********************************************************************
// FUNCTION:
//
//		GetRxFrameTemplate() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Find and return a valid HL7 received frame template for 
//		received frame processing  
// _____________________________________________________________________
// PARAMETERS:
//
//		recordType - find template for the record type
// ____________________________________________________________________
// RETURNED:
//
//		maxFieldsPtr - maximum number of fioelds for this record
//		Null if template not found, else pointer to template
// 
// ********************************************************************/      

char	*GetRxFrameTemplate(char recordType, char *maxFieldsPtr)
{
	int		i;
	char	*recordTemplatePtr;

	// scan list of record template ptrs for current hl7 record type and	      
	// set maximum fields for current record type								      

	i = 0;

	while (i < NUM_RX_FRAME_TEMPLATES)
	{
		recordTemplatePtr = (char*) hl7RxRecordTemplatePtrs_m[i++];

		*maxFieldsPtr = *recordTemplatePtr++;				// set max fields this record       

		if (*recordTemplatePtr++ == recordType)  {
			return (recordTemplatePtr);						// found so return ptr to template for this record       
		}
	}

	return (NULL);											// exit with NULL ptr for no record of this type found in list       												
}

// ********************************************************************
// FUNCTION:
//
//		SetHl7RecordTypeMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set HL7 type for transmit as either valid results data type or
//		aborted error rotor results data type message  
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		message type
// 
// ********************************************************************/      

Hl7TxMsgType_t SetHl7RecordTypeMsg( void )
{
	Hl7TxMsgType_t MsgType;
	if (hl7ErrorDuringRxMsg_m == true) {
		MsgType = HL7_TX_ERROR_MSG;
	}
	else if (hl7Record_m->hl7RecordType == HL7_RECORD_RESULTS ) {
		if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE) {
			MsgType = HL7_TX_RESULTS_MSG;
		}
		else {
			MsgType = HL7_TX_UNSOLICITED_RESULTS_MSG;
		}
	}
	else {
		MsgType = HL7_TX_ABORT_MSG;
	}
	return MsgType;
}


// ********************************************************************
// FUNCTION:
//
//		ExtractRxFrameFields() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Extract and verify ASCII fields for HL7 received frame message  
// _____________________________________________________________________
// PARAMETERS:
//
//		method - field method
//		validRxChar - first character in the field string
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

char ExtractRxFrameFields(char method, char validRxChar)
{
	char	fieldIdx;
	char	fieldString[MAX_HL7_RX_FIELD];
	char	tempString[MAX_HL7_RX_FIELD];
	char	*fieldStringPtr;
	char	*token;
	int		fieldSize;
	unsigned short convertedMethod;


	// extract/handle text string based on method for identified HL7 field       
	// first handle single char methods (uses passed validRxChar only and exits) 
	convertedMethod = ExtractRxConfigFields(method);
	// generic method: do exact match to current "field terminator"	for non Header records       
	switch (convertedMethod)
	{

		// HL7 Message Header Record  (variable fields)       
		case HL7_MSH_FIELD_DELIMITER:			//MSH-1
			if (validRxChar != hl7Configuration_m->segmentMSHIn.mshFieldDelimiter) {
				// bad field delimiter so set up for tx of abaxis comment message 
				// Use the default from the configuration file
				hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
				hl7ErrorDuringRxMsg_m = true;
				LogError("Bad MSH_FIELD_DELIMITER = %c should be %c",validRxChar,hl7Configuration_m->segmentMSHIn.mshFieldDelimiter);
			}
			else {
				hl7RxQueryData_m->hl7RxMSHFieldDelimiter =  validRxChar;
			}
			break;

		case HL7_MSH_REPEAT_DELIMITER:			//MSH-2  
			if (validRxChar != hl7Configuration_m->segmentMSHIn.mshRepeatDelimiter) {
				// bad repeat delimiter so set up for tx of abaxis comment message 
				// Use the default from the configuration file
				hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
				hl7ErrorDuringRxMsg_m = true;
				LogError("Bad MSH_REPEAT_DELIMITER = %c should be %c",validRxChar,hl7Configuration_m->segmentMSHIn.mshRepeatDelimiter);
			}
			hl7RxQueryData_m->hl7RxMSHRepeatDelimiter =  validRxChar;
			break;

		case HL7_MSH_COMPONENT_DELIMITER:		//MSH-2  
			if (validRxChar != hl7Configuration_m->segmentMSHIn.mshComponentDelimiter) {
				// bad component delimiter so set up for tx of abaxis comment message 
				// Use the default from the configuration file
				hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
				hl7ErrorDuringRxMsg_m = true;
				LogError("Bad MSH_COMPONENT_DELIMITER = %c should be %c",validRxChar,hl7Configuration_m->segmentMSHIn.mshComponentDelimiter);
			}
			hl7RxQueryData_m->hl7RxMSHComponentDelimiter =  validRxChar;
			break;

		case HL7_MSH_SUB_COMPONENT_DELIMITER:	//MSH-2  	    
			if (validRxChar != hl7Configuration_m->segmentMSHIn.mshSubComponentDelimiter) {
				// bad sub component delimiter so set up for tx of abaxis comment message 
				// Use the default from the configuration file
				hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
				hl7ErrorDuringRxMsg_m = true;
				LogError("Bad SUB_COMPONENT_DELIMITER = %c should be %c",validRxChar,hl7Configuration_m->segmentMSHIn.mshSubComponentDelimiter);
			}
			hl7RxQueryData_m->hl7RxMSHSubComponentDelimiter =  validRxChar;
			break;

		case HL7_MSH_ESCAPE_CHARACTER:			//MSH-2  
			if (validRxChar != hl7Configuration_m->segmentMSHIn.mshEscapeCharacter) {
				// bad field delimiter so set up for tx of abaxis comment message 
				// Use the default from the configuration file
				hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
				hl7ErrorDuringRxMsg_m = true;
				LogError("Bad MSH_ESCAPE_CHARACTER = %c should be %c",validRxChar,hl7Configuration_m->segmentMSHIn.mshEscapeCharacter);
			}
			hl7RxQueryData_m->hl7RxMSHEscapeCharacter =  validRxChar;
			break;



		//	 next handle all other methods: extract strings into temp buffer for processing
		//	 of abaxis hl7 message specific parameters 

		default:
			// init temp field to all null chars        
			memset(fieldString, 0, sizeof( fieldString ));

			// extract char data for field until field delimiter or CR (aka last field delimiter)       

			fieldIdx = 0;													// init index to temp field storage     
			while ((validRxChar != hl7RxQueryData_m->hl7RxMSHFieldDelimiter) && (validRxChar != CR))  
			{
				// clamp field char store at (max - 1) for char not (field delim or CR)       

				if (fieldIdx < (MAX_HL7_RX_FIELD - 1)) 	{
					fieldString[ (int)fieldIdx++ ] = validRxChar;	// valid char so store in temp field storage       	
				}

				validRxChar = GetValidRxChar(); 		// get next char       

				if (validRxChar == (char)NOT_VALID_HL7_CHAR)  {
					return (NAK);											// in-valid hl7 char in rx frame text so NAK       
				}
			}
			for (int i=0; i < MAX_RX_SIZE_FIELDS; i++)
			{
				if (hl7RxFieldSizes[i].method == convertedMethod) {
					fieldSize = hl7RxFieldSizes[i].size;
					break;
				}
			}
			if (fieldIdx > fieldSize)
			{
				// field size too big for abaxis so set up for tx of abaxis comment message       
				//Don't overwrite the last error
				if (hl7ErrorDuringRxMsg_m == false) {
					hl7ErrorType_m = HL7_RX_FIELD_SIZE_TOO_BIG;
					hl7ErrorDuringRxMsg_m = true;
					LogError("Segment field size too big method =%d size = %d should be %d",method,fieldIdx,fieldSize);
				}
			}

			// init field ptr to beginning of field string for use by methods       

			fieldStringPtr = fieldString;
			
			// HL7 Request Information Record  (variable fields)       

			switch (convertedMethod)
			{
				case HL7_MSH_SENDING_APPLICATION:	//MSH-3
					strncpy(hl7RxQueryData_m->hl7RxMSHSendingApplication, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHSendingApplication)-1);
					break;

				case HL7_MSH_SENDING_FACILITY:		//MSH-4
					strncpy(hl7RxQueryData_m->hl7RxMSHSendingApplication, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHSendingApplication)-1);
					break;

				case HL7_MSH_DATE_TIME_OF_MESSAGE:	//MSH-7
 					strncpy(hl7RxQueryData_m->hl7RxMSHMsgDateTime, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHMsgDateTime)-1);
					break;

				case HL7_MSH_SECURITY:				//MSH-8
					strncpy(hl7RxQueryData_m->hl7RxMSHSecurity, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHSecurity)-1);
					break;

				case HL7_MSH_MESSAGE_TYPE:			//MSH-9
					strncpy(hl7RxQueryData_m->hl7RxMSHMessageType, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHMessageType)-1);
					//The message type can only be "QRY^R02"
					if ((strlen (hl7RxQueryData_m->hl7RxMSHMessageType) > 7) ||
						(strstr(hl7RxQueryData_m->hl7RxMSHMessageType,MSH3_RESULT_QUERY_QRY) == 0) ||
						(strstr(hl7RxQueryData_m->hl7RxMSHMessageType,MSH3_RESULT_QUERY_R02) == 0) ||
						(strchr(hl7RxQueryData_m->hl7RxMSHMessageType,hl7Configuration_m->segmentMSHIn.mshComponentDelimiter) == 0) )
					{
						// field size too big for abaxis so set up for tx of abaxis comment message       
						//Don't overwrite the last error
						if (hl7ErrorDuringRxMsg_m == false) {
							hl7ErrorType_m = HL7_RX_INVALID_RECORD_TYPE;
							hl7ErrorDuringRxMsg_m = true;
							LogError("Wrong message type = %s should be %s%c%s",
													hl7RxQueryData_m->hl7RxMSHMessageType,
													MSH3_RESULT_QUERY_QRY,
													hl7Configuration_m->segmentMSHIn.mshSubComponentDelimiter,
													MSH3_RESULT_QUERY_R02);
						}
					}

					break;

				case HL7_MSH_MESSAGE_CONTROL_ID:	//MSH-10
					strncpy(hl7RxQueryData_m->hl7RxMSHMessageControlID, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHMessageControlID)-1);
					break;

				case HL7_MSH_PROCESSING_ID:			//MSH-11
					strncpy(hl7RxQueryData_m->hl7RxMSHProcessingID, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHProcessingID)-1);
					break;

				case HL7_MSH_VERSION_ID:			//MSH-12	
					strncpy(hl7RxQueryData_m->hl7RxMSHVersionID, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxMSHVersionID)-1);
					if (strncmp(SUPPORTED_HL7_VERSION,hl7RxQueryData_m->hl7RxMSHVersionID, sizeof(hl7RxQueryData_m->hl7RxMSHVersionID)-1) != 0) {
						//Don't everwrite the last error
						if (hl7ErrorDuringRxMsg_m == false) {
							hl7ErrorType_m = HL7_RX_UNSUPPORTED_HL7_VERSION;
							hl7ErrorDuringRxMsg_m = true;
							LogError("Bad MSH_VERSION_ID = %s should be %s",hl7RxQueryData_m->hl7RxMSHVersionID,SUPPORTED_HL7_VERSION);
						}
					}

					break;

				case HL7_QRD_DATE_TIME:   			//QRD-1
					strncpy(hl7RxQueryData_m->hl7RxQRDDateTime, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDDateTime)-1);
					break;

				case HL7_QRD_FORMAT_CODE:			//QRD-2
					hl7RxQueryData_m->hl7RxQRDFormatCode = *fieldStringPtr;
					break;

				case HL7_QRD_PRIORITY:	       		//QRD-3
					hl7RxQueryData_m->hl7RxQRDPriority = *fieldStringPtr;
					break;

				case HL7_QRD_ID:       				//QRD-4
					strncpy(hl7RxQueryData_m->hl7RxQRDID, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDID)-1);
					break;

			// HL7 Message Terminator Record        

				case HL7_QRD_WHO_SUBJECT_FILTER:	//QRD-8
					strncpy(hl7RxQueryData_m->hl7RxQRDWhoSubjectFilter, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDWhoSubjectFilter)-1);
					break;

				case HL7_QRD_WHAT_SUBJECT_FILTER:   //QRD-9
					strncpy(hl7RxQueryData_m->hl7RxQRDWhatSubjectFilter, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDWhatSubjectFilter)-1);
					break;

				case HL7_QRD_WHAT_DEPT_DATA_CODE:   //QRD-10
					strncpy(hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue)-1);
					break;

				case HL7_QRD_WHAT_DEPT_VALUE_QUAL:  //QRD-11
					strncpy(hl7RxQueryData_m->hl7RxQRDWhatDataCodeValueQual, fieldStringPtr, sizeof(hl7RxQueryData_m->hl7RxQRDWhatDataCodeValueQual)-1);
					strncpy(tempString, fieldStringPtr, sizeof(tempString)-1);
					//Check if the start/end time delimiter is present
					if (strchr(tempString,hl7Configuration_m->segmentMSHIn.mshComponentDelimiter) != NULL) {
						if (tempString[0] != hl7Configuration_m->segmentMSHIn.mshComponentDelimiter) {
							token = strtok(tempString,&hl7Configuration_m->segmentMSHIn.mshComponentDelimiter);
							if (token != NULL) {
								strncpy(hl7RxQueryData_m->hl7RxQueryBeginDateTime, token, sizeof(hl7RxQueryData_m->hl7RxQueryBeginDateTime)-1);
								//Set the time at the beggining of the day
								strncpy(&hl7RxQueryData_m->hl7RxQueryBeginDateTime[8], "000000", 6);
								token = strtok(NULL,&hl7Configuration_m->segmentMSHOut.mshComponentDelimiter);
								if ( token != NULL) {
									strncpy(hl7RxQueryData_m->hl7RxQueryEndDateTime, token, sizeof(hl7RxQueryData_m->hl7RxQueryEndDateTime)-1);
									//Set the time at the end of the day
									strncpy(&hl7RxQueryData_m->hl7RxQueryEndDateTime[8], "235959", 6);
								}
								else {
									hl7RxQueryData_m->hl7RxQueryEndDateTime[0] = '\0';
								}
							}
							else {
								hl7RxQueryData_m->hl7RxQueryBeginDateTime[0] = '\0';
								hl7RxQueryData_m->hl7RxQueryEndDateTime[0] = '\0';
							}
						}
						else {
							hl7RxQueryData_m->hl7RxQueryBeginDateTime[0] = '\0';
							token = strtok(tempString,&hl7RxQueryData_m->hl7RxMSHComponentDelimiter);
							if ( token != NULL) {
								strncpy(hl7RxQueryData_m->hl7RxQueryEndDateTime, token, sizeof(hl7RxQueryData_m->hl7RxQueryEndDateTime)-1);
								//Set the time at the end of the day
								strncpy(&hl7RxQueryData_m->hl7RxQueryEndDateTime[8], "235959", 6);
							}
							else {
								hl7RxQueryData_m->hl7RxQueryEndDateTime[0] = '\0';
							}
						}
					}
					else {
						//Don't overwrite the last error
						if (hl7ErrorDuringRxMsg_m == false) {
							// bad component delimiter so set up for tx of abaxis comment message 
							// Use the default from the configuration file
							hl7ErrorType_m = HL7_RX_DELIMITER_MISMATCH;
							hl7ErrorDuringRxMsg_m = true;
							LogError("Bad or missing MSH_COMPONENT_DELIMITER in QRD-11 should be %c",hl7Configuration_m->segmentMSHIn.mshComponentDelimiter);
						}
					}

					break;

				case HL7_RX_FIELD_DELIMITER:			    			
					break;
			
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
//		ProcessHl7RxMsg() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set up valid receipt of Abaxis HL7 supported message for processing  
// _____________________________________________________________________
// PARAMETERS:
//
//		recordType HSM or QRD record
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/      

void ProcessHl7RxMsg(char recordType)
{

	// if found hl7 issue, set up for tx o      

	if (hl7ErrorType_m != NO_ERRORS) {
		hl7ErrorDuringRxMsg_m = true;							// set for hl7 issue during message reception       
	}			

	// if got good hl7 header, flag for next records and to use delimiters from received frame       

	if ((recordType == 'H') && (!gotHl7RxHeader_m))	{
		gotHl7RxHeader_m = true;								// K to process more records for abaxis msg       

		abaxisRxMsgRecordIdx_m = 0;

		hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue[0] = '\0';	// make QRD message start id string empty       
		hl7RxQueryData_m->hl7RxQueryBeginDateTime[0] = '\0';	// make Query*- message begin date/time string empty       
		hl7RxQueryData_m->hl7RxQueryEndDateTime[0] = '\0';		// make Query message end date/time string empty       

	}
	else if ((recordType == 'Q') && gotHl7RxHeader_m)	{
		gotHl7RxHeader_m = false;
		// scan to see if valid abaxis rx msg and set to process if so       
		abaxisHl7RxMsgToProcess_m = HL7_RX_QUERY_MSG;			// set abaxis rx msg type to process 			      
	}
	else  {
		gotHl7RxHeader_m = false;
		// scan to see if valid abaxis rx msg and set to process if so       
		abaxisHl7RxMsgToProcess_m = HL7_RX_QUERY_MSG;			// set abaxis rx msg type to process 			      
		//Don't overwrite the last error
		if (hl7ErrorDuringRxMsg_m == false) {
			hl7ErrorType_m = HL7_RX_RECORD_OUT_OF_SEQUENCE;
			hl7ErrorDuringRxMsg_m = true;						// set for hl7 issue during message reception       
			LogError("Segments out of sequence, got header =%d recordType = %d",gotHl7RxHeader_m,recordType);
		}
	}
}



// ********************************************************************
// FUNCTION:
//
//		ProcessRxMessageFrames() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process HL7 message frames for inclusion in Abaxis supported
//		HL7 messages and return verified ACK/NAK status of frame  
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
	char	recordType;
	char	maxFields;
	char	*maxFieldsPtr;
	char	numFields;
	char	rxFrameChar;
	char	*templatePtr;
	char	templateTerminator = '\0';

	// init abaxis hl7 comment message type for no record data error type       

	hl7RxBufferIdx_m = 0;

	// init for rx frame number as first char  & 				      
	// check that received rx frame number matches expected frame number per HL7


	// get record type per HL7(e.g. H,Q) for this frame (start frame record text)       

	maxFieldsPtr = &maxFields;		// set up to get max number fields in this frame record type       

    //ZZZ for MLLP mode, ignor SB
    if ((hl7Configuration_m->segmentBLK.blockStart) && (hl7RxBuffer_m[hl7RxBufferIdx_m] == SB)) { 
            // ZZZ
            // just ingor the SB
            hl7RxBufferIdx_m++;
#ifdef HL7_DEBUG_ENABLE
            LogDebug ("ignor SB");
#endif
    }
    if ((hl7RxBuffer_m[hl7RxBufferIdx_m] == EB)  && (hl7Configuration_m->segmentBLK.blockEnd)){
#ifdef HL7_DEBUG_ENABLE
        LogDebug ("Get End of Block message 0x%x, just return", hl7RxBuffer_m[hl7RxBufferIdx_m]);
#endif
        return (NAK);
    }

	if (strncmp (&hl7RxBuffer_m[hl7RxBufferIdx_m], MSH_RECORD,3) == 0) {
#ifdef HL7_DEBUG_ENABLE
        LogDebug ("rx Msg:: %s", &hl7RxBuffer_m [hl7RxBufferIdx_m] );
#endif
		recordType = 'H';
		//Clear stale data
		memset(hl7RxQueryData_m,0,sizeof(Hl7RxQueryData_t));
		hl7ErrorDuringRxMsg_m = false;
		hl7ErrorType_m = NO_ERRORS;
	}
	else if (strncmp (&hl7RxBuffer_m[hl7RxBufferIdx_m], QRD_RECORD,3) == 0) {
#ifdef HL7_DEBUG_ENABLE
        LogDebug ("rx Msg:: %s", &hl7RxBuffer_m [hl7RxBufferIdx_m]);
#endif
		recordType = 'Q';
	}
	hl7RxBufferIdx_m += 3;


	templatePtr = GetRxFrameTemplate(recordType, maxFieldsPtr);

	if (templatePtr == NULL) {
		// not a valid hl7 record type so set up for tx of abaxis comment message       

		templatePtr = &templateTerminator;
		maxFields = MAX_INVALID_RX_FIELDS;       

		//Don't overwrite the last error
		if (hl7ErrorDuringRxMsg_m == false) {
			hl7ErrorType_m = HL7_RX_INVALID_RECORD_TYPE;
			hl7ErrorDuringRxMsg_m = true;
			LogError("Invalid segment type, got header =%d recordType = %d",gotHl7RxHeader_m,recordType);
		}
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
		rxFrameChar = GetValidRxChar();

		if (rxFrameChar == NOT_VALID_HL7_CHAR) {
			return (NAK);			// in-valid hl7 char in rx frame text so NAK       
		}

		// continue processing only if already got a header frame or if this frame			      
		//   is a header record so that delimiters are set up for rx frames from host	      
		//   otherwise ignore all chars till CR and flush 		      
		//   and check for num_fields													      

		if (gotHl7RxHeader_m || (recordType == 'H')) {
			if (*templatePtr != '\0')	{		// NULL at end of template       

				rxFrameChar = ExtractRxFrameFields(*templatePtr++, rxFrameChar);

				if (rxFrameChar == NAK) {
					return (NAK);	// in-valid hl7 char in rx frame method processing so NAK       
				}
			}
		}

		if ((rxFrameChar == hl7RxQueryData_m->hl7RxMSHFieldDelimiter) || (rxFrameChar == CR)) {
			numFields++;

			// check to make sure num_fields is less than max for this frame       

			if (numFields > maxFields) {
				// too many fields for this hl7 record type so set up for tx of abaxis comment message       

				//Don't overwrite the last error
				if (hl7ErrorDuringRxMsg_m == false) {
					hl7ErrorType_m = HL7_RX_TOO_MANY_FIELDS_IN_RECORD;
					hl7ErrorDuringRxMsg_m = true;
					LogError("Too many seqment fields =%d maxfields =%d",numFields,maxFields);
				}
			}
		}
			
	} while (rxFrameChar != CR);

	// got good hl7 frame, but now process hl7 rx frames for correct record synchronization / coalation  	       
	//   and to see if got complete abaxis hl7 rx msg without hl7 errors									      
	//   if got complete msg without hl7 errors, then sets to process message type							      	

	ProcessHl7RxMsg(recordType);


	// all OK so ACK this frame for hl7 state machine       

	return (ACK);					// valid hl7 rx frame received so ACK       
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
	// process HL7 Query message request transmit states

	switch ( hl7QueryState_m )
	{
		case HL7_QUERY_INIT:

			if (hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue[0] == '\0') 	{						// check to handle no ID at all       
				hl7QueryState_m = HL7_QUERY_NO_RECORDS;					// set up to send no query results feedback message       
			}
			else {
				hl7Record_m = SearchHl7ResultsRecord( hl7RxQueryData_m->hl7RxQRDWhatDataCodeValue, hl7RxQueryData_m->hl7RxQueryBeginDateTime, hl7RxQueryData_m->hl7RxQueryEndDateTime );
				if (hl7ErrorDuringRxMsg_m == true) {
					clearResultList();
				}

				if (hl7Record_m != NULL) {
					hl7TxMsgType_m = SetHl7RecordTypeMsg();				// at least 1 record exists

					hl7QueryState_m = HL7_QUERY_NEXT_RECORD;				// set up to search for next database HL7 record       
				}
				else {
					hl7QueryState_m = HL7_QUERY_NO_RECORDS;				// set up to send no query results feedback message       
				}
			}
			break;											
	       
		case HL7_QUERY_NEXT_RECORD:
			// get next record

			hl7Record_m = GetNextHl7ResultsRecord();

			if (hl7Record_m != NULL) {
				hl7TxMsgType_m = SetHl7RecordTypeMsg();
			}
			else {
				hl7QueryState_m = HL7_QUERY_COMPLETED;					// set up to send no query results feedback message       
				hl7ErrorDuringRxMsg_m = false;					// re-init for next abaxis message       
				hl7ErrorType_m = NO_ERRORS;
			}
			break;

		case HL7_QUERY_NO_RECORDS:
			hl7TxMsgType_m = HL7_TX_NO_QUERY_RESULTS_MSG;					// send no query results message  

			// and complete abaxis send query state machine

		case HL7_QUERY_COMPLETED:
		default:
			// if query of all hl7 results selected have been processed then cleanup and exit

			hl7RxBusy_m = false;											// handshake to now allow more host rx messages       

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
	// process HL7 transmit range message states

	switch ( hl7TransmitRangeState_m )
	{
		case HL7_TRANSMIT_RANGE_INIT:
			hl7Record_m = SearchHl7ResultsRecord( "ALL", abaxisSendParam3_m, abaxisSendParam4_m );
			if (hl7ErrorDuringRxMsg_m == true) {
				clearResultList();
			}
			if (hl7Record_m != NULL) {
				hl7TxMsgType_m = SetHl7RecordTypeMsg();						// at least 1 record exists

				hl7TransmitRangeState_m = HL7_TRANSMIT_RANGE_NEXT_RECORD;		// set up to search for next database HL7 record       
			}
			else {
				hl7TransmitRangeState_m = HL7_TRANSMIT_RANGE_NO_RECORDS;		// set up to send no query results feedback message       
			}
			break;											
	       
		case HL7_TRANSMIT_RANGE_NEXT_RECORD:
			// get next record
			hl7Record_m = GetNextHl7ResultsRecord();

			if (hl7Record_m != NULL) {
				hl7TxMsgType_m = SetHl7RecordTypeMsg();
			}
			else {
				hl7TransmitRangeState_m = HL7_TRANSMIT_RANGE_COMPLETED;			       
				hl7ErrorDuringRxMsg_m = false;					// re-init for next abaxis message       
				hl7ErrorType_m = NO_ERRORS;
			}

			break;

		case HL7_TRANSMIT_RANGE_NO_RECORDS:
		case HL7_TRANSMIT_RANGE_COMPLETED:
		default:
			// if transmit of all hl7 results in range have been processed then cleanup and exit

			MarkTransmitResultsRecordsDone();

			abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;							// set for abaxis de-queued transmit range type send done       
			break;	
	}	
}


// ********************************************************************
// FUNCTION:
//
//		ProcessHl7Msgs() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sets up and handles 
//			- transmit of messages in Abaxis HL7 send message queue
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

void	ProcessHl7Msgs()
{
	// process abaxis hl7 tx messages       

	if (abaxisSendType_m != ABAXIS_SEND_DONE_TYPE)					// send is in process       
	{
		// if a query message send is in process, do not de-queue new tx message

		if (abaxisSendType_m == ABAXIS_SEND_RESULTS_QUERY_TYPE)	{	// process results query sends - potential multiple H..L messages       	
			ProcessSendResultsQuery();
		}
		else if (abaxisSendType_m == ABAXIS_SEND_RESULTS_RANGE_TYPE) {	// process results range sends - potential multiple H..L messages       	
			ProcessTransmitRangeResultsRecords();
		}
		else {// process finish of de-queued abaxis messages with 1 message sequence only       
			if (hl7TxMsgType_m == HL7_TX_NO_MSGS)	{				// single de-queued message complete       
				abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;			// set for abaxis send done       	
			}
		}
	}
	else 																       
	{
		// abaxis send done so check if another tx message type in queue to send


		if (abaxisSendQueueOutputIdx_m != abaxisSendQueueInputIdx_m) {
			abaxisSendType_m    = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgSendType;
			abaxisSendParam1_m  = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam1;
			abaxisSendParam2_m  = abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam2;
			strncpy( abaxisSendParam3_m, abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam3, sizeof(abaxisSendParam3_m)-1 );
			strncpy( abaxisSendParam4_m, abaxisSendQueue_m[abaxisSendQueueOutputIdx_m].msgParam4, sizeof(abaxisSendParam4_m)-1 );
		
			abaxisSendQueueOutputIdx_m = (abaxisSendQueueOutputIdx_m + 1) % ABAXIS_SEND_QUEUE_SIZE;

			// process tx send type       
			switch ( abaxisSendType_m )
			{
				case ABAXIS_SEND_COMMENT_MSG_TYPE:
					hl7TxMsgType_m = HL7_TX_COMMENT_MSG;
					break;

				case ABAXIS_SEND_ROTOR_RUN_DONE_TYPE:
					// rotor run completed so get newest rotor result record & check to set for result or error

					hl7Record_m = GetHl7ResultsRecord( abaxisSendParam1_m );

					if ( hl7Record_m != 0 ) {
						hl7TxMsgType_m = SetHl7RecordTypeMsg();
					}
					break;
				case ABAXIS_SEND_RESULTS_QUERY_TYPE:
					hl7QueryState_m = HL7_QUERY_INIT;
					break;

				case ABAXIS_SEND_RESULTS_RANGE_TYPE:
					hl7TransmitRangeState_m = HL7_TRANSMIT_RANGE_INIT;
					break;					

				default: 												// not valid abaxisSendType_m       
					abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;
					break;
			}	
		}
	} 

	// process abaxis hl7 rx messages       

	// check if process ABAXIS QUERY message from host       



	if (abaxisHl7RxMsgToProcess_m == HL7_RX_QUERY_MSG) {
		// set up for hl7 query message processing        

		QueueHl7TxMsg( ABAXIS_SEND_RESULTS_QUERY_TYPE );

		abaxisHl7RxMsgToProcess_m = 0;									// handshake for no more abaxis rx msg to process       
	}
}



// ********************************************************************
// FUNCTION:
//
//		ProcessHl7State() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Main state machine processor HL7 idle and transmit states
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

Hl7State_t ProcessHl7State(Hl7State_t hl7State)
{

	//	*** HL7 TX STATE MACHINE ***

	switch (hl7State)
	{
		case HL7_INIT_STATE:
			// Process HL7_INIT_STATE: initialization state for HL7 state machine interface processing       

			abaxisSendQueueInputIdx_m = 0;											// init the hl7_tx_queue indices & flags       
			abaxisSendQueueOutputIdx_m = 0;
			abaxisSendType_m = ABAXIS_SEND_DONE_TYPE;
			hl7TxMsgType_m = HL7_TX_NO_MSGS;										// set for no Abaxis data to send on init       

			// set up abaxis information info for send of HL7 tx power up sign on comment (H C) message        
			instrumentInfo_m = GetInstrumentInfo();

			// Send a wake up message to the Host
			//!! Not implemented in this release
//			QueueHl7TxMsg( ABAXIS_SEND_COMMENT_MSG_TYPE, HL7_POWER_UP_SIGN_ON );

			if ( instrumentInfo_m->productCode == '1' )	{							// productCode: '1' is VS2 else xPress or both
				last_hl7_tx_iqc_r_record_m = LAST_TX_IQC_OBX_RECORD_VS2;			// set up number of VS2    instrument IQC records
			}
			else {
				last_hl7_tx_iqc_r_record_m = LAST_TX_IQC_OBX_RECORD_XPRESS;			// set up number of xPress instrument IQC records
			} 
			last_hl7_tx_iqc_r_record_m = LAST_TX_IQC_OBX_RECORD_XPRESS;				// set up number of xPress instrument IQC records

			abaxisHl7RxMsgToProcess_m = 0;											// init for no rx msg to process       		
			hl7RxBusy_m = false;													// set for instrument not busy on init       
			hl7ErrorDuringRxMsg_m = false;											// init for no hl7 issues during abaxis message reception       

			hl7RxQueryData_m->hl7RxMSHFieldDelimiter			= hl7Configuration_m->segmentMSHIn.mshFieldDelimiter;				// default field delimiter       
			hl7RxQueryData_m->hl7RxMSHRepeatDelimiter			=  hl7Configuration_m->segmentMSHIn.mshRepeatDelimiter;				// default header repeat delimiter       
			hl7RxQueryData_m->hl7RxMSHComponentDelimiter		=  hl7Configuration_m->segmentMSHIn.mshComponentDelimiter;			// default header component delimiter       
			hl7RxQueryData_m->hl7RxMSHEscapeCharacter			=  hl7Configuration_m->segmentMSHIn.mshEscapeCharacter;				// default header escape delimiter       
			hl7RxQueryData_m->hl7RxMSHSubComponentDelimiter		=  hl7Configuration_m->segmentMSHIn.mshSubComponentDelimiter;		// default header subcomponent delimiter       

			//Setup the query fixed or unused fields
			hl7RxQueryData_m->hl7RxQRDFormatCode = 'R';								//R  - Response is in record-oriented format
			hl7RxQueryData_m->hl7RxQRDPriority = 'I';								//This field contains the time frame in which the response is expected
			hl7RxQueryData_m->hl7RxQRDDeferredResponseType = '\0';
			hl7RxQueryData_m->hl7RxQRDDeferredResponseDate = '\0';
			hl7RxQueryData_m->hl7RxQRDQualityRequest = '\0';
			gotHl7RxHeader_m = false;												// init for first rx header not received       
			hl7State = HL7_IDLE_STATE;
			hl7RxFrameNumber_m = 1;													// init for first receive frame/record expected       
			hl7RxBufferIdx_m = 0;													// init index to receive buffer       

			break;

		case HL7_IDLE_STATE:
			// Process HL7_IDLE_STATE: hl7 communications idle/terminated state       
			if (hl7TxMsgType_m != HL7_TX_NO_MSGS)	{								// Abaxis data to send so start transmit states       			
				hl7State = HL7_TX_DATA_TO_SEND_STATE;	
			}
			else {
				// process abaxis defined hl7 messages transmit/receive (made up of hl7 record frames)

				WaitForMsg();								// Wait for 1 second timer or last rotor record available message
				ProcessHl7Msgs();							// process msg frame collection and msg verification here also
			}									       
			break;

		case HL7_TX_DATA_TO_SEND_STATE:
			// Process HL7_TX_DATA_TO_SEND_STATE: set up Abaxis data transmission state       
			hl7TxFrameNumber_m = 1;													// init for first transmit frame/record       
			abaxisTxMsgDefPtr_m = (char *) abaxisHl7TxMsgPtrs_m[hl7TxMsgType_m];	// set pointer to this message definition record indices       
			hl7State = HL7_TX_SETUP_NEXT_FRAME_STATE;								// proceed to wait for connection to transmit       
			break;

		case HL7_TX_SETUP_NEXT_FRAME_STATE:
			// Process HL7_TX_SETUP_NEXT_FRAME_STATE: set up next frame to transmit/determine if done transmitting Abaxis data state       
			if (SetupTxMessageFrames() == ABAXIS_HL7_TX_MSG_TRANSMITTED)	{		// set up frame to transmit if not done with message        
				if (( hl7TxMsgType_m == HL7_TX_RESULTS_MSG ) || ( hl7TxMsgType_m == HL7_TX_ABORT_MSG )) {
					MarkResultsRecordSent( hl7Record_m->recordId );
				}
				hl7TxMsgType_m = HL7_TX_NO_MSGS;									// message sent       
				hl7State = HL7_IDLE_STATE;											// proceed to idle       
			}
			else {
				hl7TxFrameNumber_m = (hl7TxFrameNumber_m + 1) % 8;					// incr tx frame number       
				hl7State = HL7_TX_FRAME_READY_STATE;								// proceed to process next frame/record of Abaxis message       			 
			}
			break;
	 
		case HL7_TX_FRAME_READY_STATE:
			// Process HL7_TX_FRAME_READY_STATE: frame ready so tranmit       
			WritePort (hl7TxBuffer_m, hl7TxBufferIdx_m);							// start transmit of frame here with byte count = hl7TxBufferIdx_m
			hl7TxBufferIdx_m = 0;
			hl7TxRetryCount_m = 0;													// clear transmit retry count       
			hl7State = HL7_TX_SETUP_NEXT_FRAME_STATE;								// proceed to wait for ACK from receiver			      
			break;

	//	HL7 ABAXIS INSTRUMENT INVALID STATE ERROR
		default:
			hl7State = HL7_INIT_STATE;												// re-initialize state machine       
			break;
	}
	return hl7State;
}

// ********************************************************************
// FUNCTION:
//
//		ReceiveThread() 									  
// _____________________________________________________________________
// DESCRIPTION:
//
//		Opens a listen port used for the inbound traffic. The port number is taken from
//		the setting shared memory
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

void *
ReceiveThread( void * arg )
{

	static bool			connectedLastCall = true;
	int					readCount;
	int					error;
	static bool	readOkLastTime = true;
	char				buffer[4];

    struct sockaddr_in dest;								// socket info about the machine connecting to us 
    struct sockaddr_in serv;								// socket info about our server 
    int mysocket;											// socket used to listen for incoming connections 
    socklen_t socksize = sizeof(struct sockaddr_in);


	listen_fd_m = -1;
	memset(&serv, 0, sizeof(serv));							//zero the struct before filling the fields
	serv.sin_family = AF_INET;								// set the type of connection to TCP/IP
	serv.sin_addr.s_addr = htonl(INADDR_ANY);				// set our address to any interface
	serv.sin_port = htons(settingsData->comm.clientSocket);	// set the server port number
 
	mysocket = socket(AF_INET, SOCK_STREAM, 0);
	if(mysocket==-1) {
		LogError("Receive socket error");
	}
	// bind serv information to mysocket 
	else if (bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) >= 0) {
		// start listening, allowing a queue of up to 1 pending connection 
		listen(mysocket, 1);
		listen_fd_m = accept(mysocket, (struct sockaddr *)&dest, &socksize);
	}

    while(1)
    {
		readCount = recv( listen_fd_m, (char *)buffer, 1, NULL );
		error = WSAGetLastError();
		if ( ((readCount == SOCKET_ERROR) && (error == WSAECONNRESET)) ) {	// Connection reset
			 // Note: returns readCount < 1 and errno == EAGAIN.
			if ( readOkLastTime ) {
				LogError( "read() failed, error = %u", error );
				readOkLastTime = false;
			}
		}
		else if (readCount == 1) {
			buffer[0] &= 0x7f;											// got 1 valid char, so mask off MSB for ASCII
			readOkLastTime = true;
			if (hl7RxBufferIdx_m < HL7_RX_BUFFER_SIZE) {
				hl7RxBuffer_m[hl7RxBufferIdx_m++] = buffer[0];			// save char in receive buffer
			}

			if (buffer[0] == CR) {
                if (ProcessRxMessageFrames() == ACK) {
                    hl7RxFrameNumber_m = (hl7RxFrameNumber_m + 1) % 8;
#ifdef HL7_DEBUG_ENABLE
                    LogDebug ("Rx Ack");
#endif
                } else {
#ifdef HL7_DEBUG_ENABLE
                    LogDebug ("Rx Nak");
#endif
                }
                hl7RxBufferIdx_m = 0;
			}
		}
		else if (readCount == 0) {
			LogError( "port diconnected" );
			shutdown(listen_fd_m, SD_BOTH);
			closesocket( listen_fd_m );
			listen_fd_m = accept(mysocket, (struct sockaddr *)&dest, &socksize);
		}
    }
	return 0;
}


// ********************************************************************
// FUNCTION:
//
//		CheckHl7ShareMemory() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check share memory, in the case of share memory crashed, send error report (0x411b) 
// _____________________________________________________________________
// PARAMETERS:
//
//		Pointer to shared memory
// ____________________________________________________________________
// RETURNED:
//
//		-1 if error
// 
// ********************************************************************/
int
CheckHl7ShareMemory (Hl7Configuration_t  *shareMemory)
{
    unsigned int    checkSum;

    checkSum = CalculateMemoryCrc (shareMemory);
    if (checkSum != shareMemory->crcCheckSum) {
        LogDebug ("HL7 share memory check fail");
        SendErrorReport (SERR_FILE_READ);
        return -1;
    } 
    return 0;
}


// ********************************************************************
// FUNCTION:
//
//		main() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console HL7 process top level main function 
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
	Hl7State_t			hl7State;

	hl7RxQueryData_m = (Hl7RxQueryData_t *)&query;
	memset(&query,0, sizeof(Hl7RxQueryData_t));
	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );

	// Map HL7 share memory
	hl7Configuration_m = (Hl7Configuration_t *)MapSharedMem (HL7_SHEME_NAME, sizeof (Hl7Configuration_t));
#ifdef HL7_DEBUG_ENABLE
    LogDebug ("HL7 xml file name: %s",      HL7_CONFIG_FILE_NAME);
    LogDebug ("HL7 mesage queue name %S",  HL7_QUEUE_NAME);
#endif
	// check hl7 share memory during initialization
    if (CheckHl7ShareMemory (hl7Configuration_m) == -1) {
        // share memory checking fail, exit process
        exit (0);
    }

	/*
	 *  Open the connection to the HL7 server.
	 */
	 
	// TCP/IP connection.
	if ( settingsData != 0 )
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
        //ZZZ
        hl7TimeoutSeconds_m = 0;
		SocketConnect();
	}

	// Start the 1 second timer thread.
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);

	// Start the 1 second timer thread.
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveThread, NULL, 0, NULL);

	hl7State = HL7_INIT_STATE;

	// main HL7 process
	while (1)
	{
		hl7State = ProcessHl7State( hl7State);
	}


	return 0;
}
