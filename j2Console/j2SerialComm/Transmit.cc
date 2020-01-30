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
#include <string.h>
#include <windows.h>
#include <msgqueue.h>
#include <commctrl.h>
#include <FtdiConfig.h>

#include "SerialComm.h"
#include "Transmit.h"
#include "LogErr.h"
#include "Shmem.h"
#include "CreateDisplay.h"
#include "SetSerialPortSpeed.h"

#define MAX_SEND_RETRIES	3

FT_HANDLE		Transmit::fdSer = J2_FD_INVALID;
int				Transmit::sendThreadDone = 1; 
HANDLE			Transmit::sendThread;
SystemData_t *	Transmit::systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
FactoryData_t *	Transmit::factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SendArg_t		Transmit::sendArg;

bool			cancelSend = false;

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
Run( FT_HANDLE _fdSer )
{
	DWORD			receivedSize = 0;
	SerialCommMsg_t	msg;
	HANDLE			queue;
	MSGQUEUEOPTIONS	options;
	DWORD			flags;
	BOOL			result;

	this->fdSer = _fdSer;

	// Map the settings shared memory, so we can get our comm protocol.
	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );

	// Map the reference ranges shared memory for transmitting reference ranges.
	refRanges = (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );

	/*
	 * Open the queue and get its size.
	 */
	//mqd_t queue = mq_open( SERIAL_COMM_QUEUE_NAME, O_RDONLY );
	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = TRUE;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = MAX_QUEUE_LENGTH;

	queue = CreateMsgQueue(SERIAL_COMM_QUEUE_NAME, &options);
	
	if ( queue == NULL )
	{
		LogError( "failure to open message queue" );
		return;
	}
	
	/*if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes" );
		return;
	}*/

    SetTimeout (SER_RX_TIMEOUT, SER_TX_TIMEOUT);
	while ( 1 )
	{
		// Wait for a message.
		//receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );
		result = ReadMsgQueue(queue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);
		if ( result )
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
					case MSG_SERIAL_COMM_SEND_REPORT:
						if ( systemData->inResearchMode == true )
						{
							SetPortForRad( /*&saveTerm*/ );
							SendRadFile( msg.reportData.radFileName );
                            RecoverPrevSpeed ();
							RecoverPrevFlow();
						}
						else if ( settingsData->comm.commInterface == COMM_ASCII )
						{
							SendAsciiReport( msg.reportData );
						}
						else if ( settingsData->comm.commInterface == COMM_XML )
						{
							SendXmlReport( fdSer, msg.reportData.resultIndex );
						}
						break;
												
					case MSG_SERIAL_COMM_SEND_RANGE:
						// Save the current port settings.
						sendArg.fdSer = fdSer;
						sendArg.startDate = msg.rangeData.startDate;
						sendArg.endDate = msg.rangeData.endDate;
						
						cancelSend = false;

						if ( systemData->inResearchMode == true )
						{
							sendThreadDone = 0;
							sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendRadRange, &sendArg, 0, NULL);
						}
						else if ( settingsData->comm.commInterface == COMM_ASCII )
						{
							sendThreadDone = 0;
							sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendAsciiRange, &sendArg, 0, NULL);
						}
						else if ( settingsData->comm.commInterface == COMM_XML )
						{
							sendThreadDone = 0;
							sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendXmlRange, &sendArg, 0, NULL);
						}
						break;
					
					case MSG_SERIAL_COMM_SEND_ALL:
						if ( systemData->inResearchMode == false )
						{
							// Signal send all.
							sendArg.startDate.dwHighDateTime = 0;
							sendArg.startDate.dwLowDateTime = 0;
							sendArg.endDate.dwHighDateTime = 0;
							sendArg.endDate.dwLowDateTime = 0;

							sendArg.fdSer = fdSer;

							cancelSend = false;

							if ( settingsData->comm.commInterface == COMM_ASCII )
							{
								sendThreadDone = 0;
								sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendAsciiRange, &sendArg, 0, NULL);
							}
							else if ( settingsData->comm.commInterface == COMM_XML )
							{
								sendThreadDone = 0;
								sendThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendXmlRange, &sendArg, 0, NULL);
							}
						}
						break;
												
					case MSG_SERIAL_COMM_CANCEL_SEND:
						if ( !sendThreadDone )
						{
							cancelSend = true;
						}
						break;

					case MSG_SERIAL_COMM_SEND_REF_RANGES:
						if ( (systemData->inResearchMode == false) && (settingsData->comm.commInterface == COMM_ASCII) )
						{
							SendRefRanges( msg.sampleType );
						}
						break;

                    case MSG_SERIAL_COMM_CLOSE_PORT:
                        //close serial port before kill process
                        SerialPortClose ();
                    break;
						
					default:
						break;
				}
			}
		}
		else
		{
			LogError( "message receive error" );
		}
	}
}

void *
Transmit::
SendRadRange( void * arg )
{
	SendArg_t		    sendArg = *(SendArg_t *)arg;
	int				    count = 0;
	int				    numSent = 0;
	RecallMsg_t		    msg;
	wchar_t				filename[256];
	char				currentfile[256];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	BOOL				bFinished = FALSE;

	// Set the port and save the current setting.
	SetPortForRad( /*&saveTerm*/ );

	wsprintf(filename, L"%S/*.rad", RAD_FILE_DIRECTORY);

	hSearch = FindFirstFile(filename, &fileData);
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		// Get the file count.
		while ( !bFinished && !cancelSend)
		{
			if ( (CompareFileTime(&fileData.ftLastWriteTime, &sendArg.startDate) >= 0) 
					&& (CompareFileTime(&fileData.ftLastWriteTime, &sendArg.endDate) <= 0) )
			{
				count++;
			}
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("Transmit::SendRadRange | no more rad files");
				}
				else
				{
					LogDebug("Transmit::SendRadRange | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("Transmit::SendRadRange | INVALID_HANDLE_VALUE");
	}
	
	if ( count > 0 )
	{
		bFinished = FALSE;
		msg.msgId = MSG_RECALL_SEND_PROGRESS;
		
		hSearch = FindFirstFile(filename, &fileData);
		if ( hSearch != INVALID_HANDLE_VALUE ) 
		{
			// Send the files.
			while ( !bFinished )
			{
				if ( (CompareFileTime(&fileData.ftLastWriteTime, &sendArg.startDate) >= 0) 
						&& (CompareFileTime(&fileData.ftLastWriteTime, &sendArg.endDate) <= 0) )
				{
					sprintf(currentfile, "%s/%S", RAD_FILE_DIRECTORY, fileData.cFileName);
					SendRadFile( currentfile );
		
					msg.idata = (++numSent * 100) / count;
					SendRecallMessage( &msg, sizeof( msg ) );
				}
			
				if (!FindNextFile(hSearch, &fileData)) 
				{
					if (GetLastError () == ERROR_NO_MORE_FILES)
					{
						LogDebug("Transmit::SendRadRange | no more rad files");
					}
					else
					{
						LogDebug("Transmit::SendRadRange | unable to find files");
					}
					FindClose(hSearch);
					bFinished = TRUE;
				}
			}
		}
		else
		{
			LogDebug("Transmit::SendRadRange | INVALID_HANDLE_VALUE");
		}
	}
	
	if ( !cancelSend )
	{
		msg.msgId = MSG_RECALL_SEND_RESULTS_COMPLETE;
		SendRecallMessage( &msg, 0 );
	}

	// Signal that we're done.
	// PAK: pthread_t is opaque, should not set
	sendThreadDone = 1;

	// Reset the port.
    RecoverPrevSpeed ();
	RecoverPrevFlow();

	return 0;
}


void
Transmit::
SetPortForRad( /*termios * saveTerm*/ )
{

    SetSerialPortSpeed (BAUD_RATE_RAD_XFER);
	SetFlowControl (FT_FLOW_RTS_CTS);
}


/*
 * FUNCTION
 * 	Transmit::SendRadFile
 * 
 * DESCRIPTION
 * 	Sends a RAD file out the serial port.
 * 
 * PARAMETERS
 * 	radFileName - Name of the RAD file.
 */

void
Transmit::
SendRadFile( char * radFileName )
{
	wchar_t			filename[256];
	FILE *			fdRad;
	int				count;
    int             sumCount = 0;
    int             wCount, sumWCount = 0;
	static char		buff[4096];		// Static because thread stack is small.
	HANDLE			hFile;
	DWORD			filesize;
    int             reTry;
	
	wsprintf (filename, L"%S", radFileName);
	hFile = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		filesize = GetFileSize(hFile,NULL);
		CloseHandle(hFile);
	} else {
		LogError ("could not open file %s", radFileName);
		return;
	}
	
	// Open the RAD file.
	fdRad = fopen (radFileName, "rb");
	if (fdRad == NULL) {
		LogError( "could not open file %s", radFileName );
		return;
	}

	LogInfo ("Rad File Size =%d", filesize);
	wCount = WriteSerPort (&filesize, sizeof (filesize));
	LogInfo ("first write =%d", wCount);
    

	// Send the file.
    reTry = 0;
	while (( count = fread( buff, 1, 4096, fdRad )) > 0) {
        while (1) {
            wCount =  WriteSerPort (buff, count);
            sumCount += count;
            sumWCount += wCount;
            if (wCount == 0) {
                LogError ("Tx timeout, wbyte =%d", wCount);
                reTry++;
                if (reTry == 4) {
                    LogError ("Retry = %d, Fail to transimt Rad file", reTry);
                    fclose( fdRad );
                    return;
                }
            } else if (wCount != count) {
                LogError ("Fail to transimt Rad file rByte=%d wByte=%d", count, wCount);
                fclose( fdRad );
                return;
            } else {
                reTry = 0;
                break;
            }
        }

	}
    LogInfo ("rbyte= %d wbyte=%d", sumCount, sumWCount);

	fclose( fdRad );
}

void
Transmit::
SendAsciiReport( SendReportData_t reportData )
{


	char	buff [ 2000 ];
	wchar_t	wBuff[ 2000 ];

    SetSerialPortSpeed (BAUD_RATE_ASCII_DUMP);

	// Write each requested report, preceding each with a start-of-report character.
	if ( reportData.sendResults )
	{
        WriteSerPort ("\x0C", 1);
		CreateDisplay::CreateResultDisplay( reportData.resultIndex, wBuff, &strings );
        wcstombs (buff, wBuff, 2000);
		WriteSerPort (buff, strlen (buff));
	}
	
	if ( reportData.sendIqc )
	{
		WriteSerPort ("\x0C", 1);
		CreateDisplay::CreateIqcDisplay( reportData.resultIndex, wBuff, &strings );
        wcstombs (buff, wBuff, 2000);
		WriteSerPort (buff, strlen( buff ));
	}

	if ( reportData.sendErrorReport )
	{
	    WriteSerPort ("\x0C", 1);
		CreateDisplay::CreateErrorReportDisplay( reportData.resultIndex, wBuff, reportData.abbreviatedErrorReport,&strings );
        wcstombs (buff, wBuff, 2000);
		WriteSerPort (buff, strlen( buff ));
	}

	// Finish with the end-of-transmission character.
	WriteSerPort ("\x1A", 1);
	
    RecoverPrevSpeed ();
}

void
Transmit::
SendRefRanges( SampleType_t sampleType )
{
	char	buff[ 2000 ];
	wchar_t	wBuff [2000];
	int		i, j;

	// Get the current serial port configuration and save the values we're going
	// to change.
    SetSerialPortSpeed (BAUD_RATE_ASCII_DUMP);

	if ( sampleType == STYPE_ALL )
	{
		for ( i = 0; i < STYPE_NUM_SPECIES; i++ )
		{
			if ( refRanges->isDefined[ i ] )
			{
				// All three genders for piccolo if not a control.
				if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
		 			 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
				{
                    if (i == 0) {
                         WriteSerPort ("\x0C", 1);
                         CreateDisplay::CreateRefRangesDisplay( (SampleType_t)i, GENDER_UNKNOWN, wBuff, &strings );
						 wcstombs (buff, wBuff, 2000);
                         WriteSerPort (buff, strlen( buff ) );
                    } else {
					    for ( j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++ )
					    {
						    WriteSerPort ("\x0C", 1);
						    CreateDisplay::CreateRefRangesDisplay( (SampleType_t)i, (Gender_t)j, wBuff, &strings );
							wcstombs (buff, wBuff, 2000);
						    WriteSerPort (buff, strlen( buff ) );
					    }
                    }
				}
				else
				{
					WriteSerPort ("\x0C", 1);
					CreateDisplay::CreateRefRangesDisplay( (SampleType_t)i, GENDER_UNKNOWN, wBuff, &strings );
					wcstombs (buff, wBuff, 2000);
					WriteSerPort (buff, strlen( buff ));
				}
			}
		}
	}
	else
	{
		// All three genders for piccolo if not a control.
		if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
			 (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
		{
			for ( j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++ )
			{
				WriteSerPort ("\x0C", 1);
				CreateDisplay::CreateRefRangesDisplay( sampleType, (Gender_t)j, wBuff, &strings );
				wcstombs (buff, wBuff, 2000);
				WriteSerPort (buff, strlen( buff ));
			}
		}
		else
		{
			WriteSerPort ("\x0C", 1);
			CreateDisplay::CreateRefRangesDisplay( sampleType, GENDER_UNKNOWN, wBuff, &strings );
			wcstombs (buff, wBuff, 2000);
			WriteSerPort (buff, strlen( buff ));
		}
	}

	// Finish with the end-of-transmission character.
	WriteSerPort ("\x1A", 1);
	
    RecoverPrevSpeed ();
}

void *
Transmit::
SendAsciiRange( void * arg )
{
	SendArg_t				sendArg = *(SendArg_t *)arg;
	FILE *					fp;
	int						i;
	int						numResults;
	int						numSent;
	RecallMsg_t				msg;
	const char *			dbIndex;
	bool					abbreviated;
	bool					sendRange;
	// NOTE: The following variables made static to keep them off the stack, which
	// has a limited size.
	static char				buff[ 2000 ];
    wchar_t                 wBuff [2000];
	static ResultsDbIndex_t	resultsIndex;

	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;

	sendRange = (sendArg.startDate.dwHighDateTime != 0 || sendArg.startDate.dwLowDateTime != 0) 
				|| (sendArg.endDate.dwHighDateTime != 0 || sendArg.endDate.dwLowDateTime != 0);

	// Read the index file.
	fp = fopen( dbIndex, "rb" );
	if ( fp == 0 )
	{
		LogError( "failed to open results index" );
		return 0;
	}
	
	fread( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, fp );
	fclose( fp );
		
	// Check for the version.
	if ( resultsIndex.dbVersion != RESULTS_DB_VERSION )
	{
		return 0;
	}
	
	// Get the current serial port configuration and save the values we're going
	// to change.

	// Set the baud rate and handshaking for report transfer.
    SetSerialPortSpeed (BAUD_RATE_ASCII_DUMP);

	// Get the number of results.
	for ( i = 0, numResults = 0; i < RESULTS_MAX; i++ )
	{
		if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
		{
			if ( !sendRange
				|| ((CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg.startDate) >= 0) 
				&& (CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg.endDate) <= 0)) )
			{
				numResults++;
			}
		}
	}

    // ZZZ debug only
//    LogDebug ("numResults =%d", numResults);
	if ( numResults > 0 )
	{
		// Send out all the results.
		i = resultsIndex.lastEntry;
		numSent = 0;
		msg.msgId = MSG_RECALL_SEND_PROGRESS;
		do
		{
			i = (i + 1) % RESULTS_MAX;
			
			if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
			{
				if ( !sendRange
					|| ((CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg.startDate) >= 0) 
					&& (CompareFileTime(&resultsIndex.entries[i].dateTime, &sendArg.endDate) <= 0)) )
				{
					if ( resultsIndex.entries[i].errorFlag == false )
					{
						WriteSerPort ("\x0C", 1);	// Start-of-report signal.
						CreateDisplay::CreateResultDisplay( resultsIndex.entries[i].index, wBuff, &strings );
                        wcstombs (buff, wBuff, 2000);
						WriteSerPort (buff, strlen( buff ));
						
						WriteSerPort ("\x0C", 1);
						CreateDisplay::CreateIqcDisplay( resultsIndex.entries[i].index, wBuff, &strings );
                        wcstombs (buff, wBuff, 2000);
						WriteSerPort (buff, strlen( buff ));
					}

					WriteSerPort ("\x0C", 1);
	
					if ( !systemData->inResearchMode && (resultsIndex.entries[i].errorFlag == true) )
					{
						abbreviated = true;
					}
					else
					{
						abbreviated = false;
					}
					CreateDisplay::CreateErrorReportDisplay( resultsIndex.entries[i].index, wBuff, abbreviated, &strings );
                    wcstombs (buff, wBuff, 2000);
					WriteSerPort (buff, strlen( buff ));
					// Send percentage completion.
					msg.idata = (++numSent * 100) / numResults;
					SendRecallMessage( &msg, sizeof( msg ) );
				}
			}
		} while ( !cancelSend && (i != resultsIndex.lastEntry) );

		// Signal end-of-transmission.
		WriteSerPort ("\x1A", 1);
	}

	// Indicate that we're done.
	// sendThread = -1;
	sendThreadDone = 1;

	if ( !cancelSend )
	{
		msg.msgId = MSG_RECALL_SEND_RESULTS_COMPLETE;
		SendRecallMessage( &msg, 0 );
	}
	
	// Restore the serial port settings.  This takes some time, since
	// the output buffer is 2K.
    RecoverPrevSpeed ();

	return 0;
}
	
void
Transmit::
SendRecallMessage( RecallMsg_t * msg, int msgDataLen )
{
	static HANDLE	recallQueue = NULL;
	
	if ( recallQueue == NULL )
	{
		MSGQUEUEOPTIONS		options;

		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;

		//recallQueue = mq_open( RECALL_QUEUE_NAME, O_WRONLY );
		recallQueue = CreateMsgQueue(RECALL_QUEUE_NAME, &options);
	
		if ( recallQueue == NULL )
		{
			LogError( "failure to open %s queue", RECALL_QUEUE_NAME );
			recallQueue = NULL;
		}
	}
	
	if ( recallQueue != NULL )
	{
		//mq_send( recallQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(recallQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

