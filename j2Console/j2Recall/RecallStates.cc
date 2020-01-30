/*
 * FILENAME
 * 	RecallStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Recall state machine state classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "RecallStates.h"
#include "RecallMessages.h"
#include "AnalysisMessages.h"
#include "SerialCommMessages.h"
#include "Shmem.h"
#include "ParseCounters.h"
#include "LogErr.h"
#include "ServiceArchive.h"
#include "ClinicalArchive.h"
#include "GetThumbKeyMode.h"

AStrings    strings;

ResultsDbIndex_t	StateRecall::resultsIndex;
int					StateRecall::totalCount = 0;
int					StateRecall::listCount = 0;
int					StateRecall::patientCount = 0;
int					StateRecall::controlCount = 0;
int					StateRecall::topEntry = 0;
int					StateRecall::selectedItem = -1;
bool				StateRecall::selectedItemIsError = false;
bool				StateRecall::selectedItemIsPatient = true;
int					StateRecall::lastRunIndex = 0;
bool				StateRecall::lastRunIsError = false;
bool				StateRecall::lastRunIsPatient = true;
RecallReportType_t	StateRecall::reportType = REPORT_NONE;
bool				StateRecall::printSelected = true;
bool				StateRecall::transmitSelected = false;
RecallSortOrder_t	StateRecall::sortOrder = SORT_NONE;
CounterData_t *		StateRecall::counterData = (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
FactoryData_t *		StateRecall::factoryData = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SettingsData_t *	StateRecall::settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
SystemData_t *		StateRecall::systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
SystemErrNum_t		StateRecall::errNum = SERR_NONE;
const wchar_t *		StateRecall::infoText = L"";
HANDLE				StateRecall::archiveThread;
bool				StateRecall::serviceArchiveDisabled = false;
DateRange_t			StateRecall::dateRange;
DateRangeMode_t		StateRecall::dateRangeMode;
HANDLE				StateRecall::timerThread = 0;
Expirys				StateEnterOperatorId::expirys;
SYSTEMTIME			StateRecall::tm;


/*
 * class StateRecall
 */

/*
 * FUNCTION
 * 	StateRecall::ProcessOtherMessages
 * 
 * DESCRIPTION
 * 	Processes messages that aren't handled directly by the individual states.
 * 
 * PARAMETERS
 * 	newSortOrder - the sort order to apply.
 */

void
StateRecall::
ProcessOtherMessages( int msgId, char * data )
{
	// Currently there's nothing to do.
}

/*
 * FUNCTION
 * 	StateRecall::Sort
 * 
 * DESCRIPTION
 * 	Sorts the list of entries in the results index held in memory.  (Does not
 * 	change the index file.)
 * 
 * PARAMETERS
 * 	newSortOrder - the sort order to apply.
 */

void
StateRecall::
Sort()
{
	int (*func)(const void *, const void *);

	
	switch ( sortOrder )
	{
		case SORT_DATE:
			func = CompareDate;
			listCount = totalCount;
			break;
				
		case SORT_ID:
			func = CompareId;
			listCount = totalCount;
			break;
				
		case SORT_PATIENT:
			func = ComparePatient;
			listCount = patientCount;
			break;
				
		case SORT_CONTROL:
			func = CompareControl;
			listCount = controlCount;
			break;
				
		default:
			func = 0;
			break;
	}
		
	if ( func != 0 )
	{
		qsort( &resultsIndex.entries, totalCount, sizeof( ResultsDbIndexEntry_t ), func );
	}
}
 
/*
 * FUNCTION
 * 	StateRecall::CompareDate
 * 
 * DESCRIPTION
 * 	Compares the dates for two index table entries.  Called by qsort().
 * 
 * PARAMETERS
 * 	a, b - Pointers to the index table entries.
 * 
 * RETURNS
 * 	<0 if the date for the first entry is greater than that for the second,
 *   0 if the dates are the same, or
 *  >0 if the date for the first entry is less than that for the second.
 */

int
StateRecall::
CompareDate( const void * a, const void * b )
{
	FILETIME timeA = ((ResultsDbIndexEntry_t *)a)->dateTime;
	FILETIME timeB = ((ResultsDbIndexEntry_t *)b)->dateTime;

	//return timeB - timeA;	
	return CompareFileTime(&timeB, &timeA);
}

/*
 * FUNCTION
 * 	StateRecall::CompareId
 * 
 * DESCRIPTION
 * 	Compares the sample IDs for two index table entries.  Called by qsort().
 * 
 * PARAMETERS
 * 	a, b - Pointers to the index table entries.
 * 
 * RETURNS
 * 	>0 if the ID for the first is greater than that for the second,
 *  >0 if the two IDs are the same, but the date for the first is greater than for the second,
 *   0 if the IDs and dates are the same,
 *  <0 if the ID for the first entry is less than that for the second, or
 *  <0 if the two IDs are the same, but the date for the first is less than for the second.
 */

int
StateRecall::
CompareId( const void * a, const void * b )
{
	char * idA = ((ResultsDbIndexEntry_t *)a)->patientControlId;
	char * idB = ((ResultsDbIndexEntry_t *)b)->patientControlId;
	
	if ( strcmp( idA, idB ) != 0 )
	{
		return strcmp( idA, idB );
	}
	else
	{
		return CompareDate( a, b );
	}
}

/*
 * FUNCTION
 * 	StateRecall::ComparePatient
 * 
 * DESCRIPTION
 * 	Compares the patient flags and dates for two index table entries.  Called by qsort().
 * 
 * PARAMETERS
 * 	a, b - Pointers to the index table entries.
 * 
 * RETURNS
 * 	>0 if the second entry is a patient but the first one isn't,
 *  >0 if both entries are patients and the first date is greater than the second,
 *   0 if both entries are patients and their dates are the same,
 *  <0 if the first entry is a patient but the second one isn't, or
 *  <0 if both entries are patients and the first date is less than the second.
 *  Return value if neither entry is a patient is unspecified.
 */

int
StateRecall::
ComparePatient( const void * a, const void * b )
{
	bool aIsPatient = ((ResultsDbIndexEntry_t *)a)->patientControl != 'C';
	bool bIsPatient = ((ResultsDbIndexEntry_t *)b)->patientControl != 'C';
	
	if ( aIsPatient && ! bIsPatient )
		return -1;
	else if ( bIsPatient && ! aIsPatient )
		return 1;
	else
		return CompareDate( a, b );
}

/*
 * FUNCTION
 * 	StateRecall::CompareControl
 * 
 * DESCRIPTION
 * 	Compares the patient flags and dates for two index table entries.  Called by qsort().
 * 
 * PARAMETERS
 * 	a, b - Pointers to the index table entries.
 * 
 * RETURNS
 * 	>0 if the second entry is a control but the first one isn't,
 *  >0 if both entries are controls and the first date is greater than the second,
 *   0 if both entries are controls and their dates are the same,
 *  <0 if the first entry is a control but the second one isn't, or
 *  <0 if both entries are controls and the first date is less than the second.
 *  Return value if neither entry is a control is unspecified.
 */

int
StateRecall::
CompareControl( const void * a, const void * b )
{
	bool aIsControl = ((ResultsDbIndexEntry_t *)a)->patientControl == 'C';
	bool bIsControl = ((ResultsDbIndexEntry_t *)b)->patientControl == 'C';
	
	if ( aIsControl && ! bIsControl )
		return -11;
	else if ( bIsControl && ! aIsControl )
		return 1;
	else
		return CompareDate( a, b );
}

/*
 * FUNCTION
 * 	StateRecall::GetResultType
 * 
 * DESCRIPTION
 * 	Gets the result type (Patient/Control/Error) for the indicated entry.
 * 
 * PARAMETERS
 * 	index - The entry's index in the index table.
 * 
 * RETURNS
 * 	'E' if an error, whether Patient or Control
 *  'P' if a Patient and no error, or
 *  'C' if a Control and no error.
 */

const char *
StateRecall::
GetResultType( int index )
{
	if ( resultsIndex.entries[index].errorFlag )
	{
		return "E";
	}
	else if ( resultsIndex.entries[index].patientControl == 'C' )
	{
		return "C";
	}
	else
	{
		return "P";
	}
}

/*
 * FUNCTION
 * 	StateRecall::GetResultDate
 * 
 * DESCRIPTION
 * 	Gets the date for a given result.
 * 
 * PARAMETERS
 * 	index - Index of the entry in the index table.
 * 
 * RETURNS
 * 	Date string in dd mmm format.
 */

char *
StateRecall::
GetResultDate( int index )
{
	static char dateBuff[10];
	SYSTEMTIME tm;

	FileTimeToSystemTime(&resultsIndex.entries[index].dateTime, &tm);
	sprintf( dateBuff, "%02d %S %02d", tm.wDay, strings.PrintMonth( tm.wMonth), tm.wYear % 100 );		
	return dateBuff;
}

/*
 * FUNCTION
 * 	StateRecall::ReadIndexFile
 * 
 * DESCRIPTION
 * 	Reads the index file into memory.
 */

void
StateRecall::
ReadIndexFile()
{
	FILE *			fp;
	int				i;
	const char *	dbIndex;
	
	// Init the results data to all 0xFF.
	memset( &resultsIndex, 0xFF, sizeof( ResultsDbIndex_t ) );
	
	// Read the file.
	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;
	fp = fopen( dbIndex, "rb" );
	if ( fp != 0 )
	{
		fread( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, fp );
		fclose( fp );
		
		// Check the version.
		if ( resultsIndex.dbVersion != RESULTS_DB_VERSION )
		{
			LogInfo( "read non-current db version of %d", resultsIndex.dbVersion );
			memset( &resultsIndex, 0xFF, sizeof( ResultsDbIndex_t ) );
		}			
	}
	
	// Get the number of results in the file.
	for ( totalCount = 0; totalCount < RESULTS_MAX; totalCount++ )
	{
		if ( resultsIndex.entries[ totalCount ].patientControlId[0] == (char)0xFF )
			break;
	}
    LogDebug ("totalCount =%d", totalCount);
	
	// Get the number of patient and control results in the file. In the case where
	// there was an error before the operator could enter patient or control, the
	// flag is not set. In that case, we assume patient.
	patientCount = 0;
	controlCount = 0;
	for ( i = 0; i < totalCount; i++ )
	{
		resultsIndex.entries[i].patientControl == 'C' ? controlCount++ : patientCount++;
	}
	
	// Init the selected results count to the total results count.
	listCount = totalCount;
	
	// Save the database index, patient/control flag and error flag of the last-run result.
	lastRunIndex = resultsIndex.entries[ resultsIndex.lastEntry ].index;
	lastRunIsPatient = resultsIndex.entries[ resultsIndex.lastEntry ].patientControl != 'C';
	lastRunIsError = resultsIndex.entries[ resultsIndex.lastEntry ].errorFlag;
}

void
StateRecall::
GetRadFileName( char * buff, int index )
{
	FILE *				fd;
	ResultsDbEntry_t	resultsEntry;
	const char *		dbData;
	
	if ( IN_CUSTOMER_MODE )
		dbData = CUSTOMER_DB_DATA;
	else
		dbData = OTHER_DB_DATA;
	
	// Init file name buffer, in case there's an error.
	buff[0] = '\0';

	// Open the results database file, get the specified entry, and copy the
	// RAD file name into the file name buffer.
	//fd = open( dbData, O_RDONLY, 0 );
	fd = fopen( dbData, "rb" );

	if ( fd != NULL )
	{
		//if ( lseek( fd, index * sizeof( ResultsDbEntry_t ), 0 ) != -1 )
		if ( fseek( fd, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) == 0 )
		{
			//int bytesRead = read( fd, (char *)&resultsEntry, sizeof( ResultsDbEntry_t ) );
			int bytesRead = fread( &resultsEntry, 1, sizeof( ResultsDbEntry_t ), fd );
			if ( bytesRead == sizeof( ResultsDbEntry_t ) )
			{
				strcpy( buff, resultsEntry.radFileName );
			}
		}
		fclose( fd );
	}
}

/*
 * FUNCTION
 * 	StateRecall::ProcessDates
 * 
 * DESCRIPTION
 * 	Processes the start and end dates so the end date is always after the start date, and the start time is 00:00:00
 * 	and the end time is 23:59:59
 * 
 * PARAMETERS
 * 	dateRange - Structure containing the two dates.
 */

void
StateRecall::
ProcessDates( DateRange_t * dateRange )
{
	//struct tm * tm;
	SYSTEMTIME tm;
	
	// If the end date is after the start date, reverse them.
	if ( /*dateRange->end < dateRange->start*/CompareFileTime(&dateRange->end, &dateRange->start) < 0 )
	{
		FILETIME temp = dateRange->start;
		dateRange->start = dateRange->end;
		dateRange->end = temp;
	}
	
	//tm = gmtime( &dateRange->start );
	FileTimeToSystemTime(&dateRange->start, &tm);
	//tm->tm_hour = 0;
	tm.wHour = 0;
	//tm->tm_min = 0;
	tm.wMinute = 0;
	//tm->tm_sec = 0;
	tm.wSecond = 0;
	//dateRange->start = mktime( tm );
	SystemTimeToFileTime(&tm, &dateRange->start);
	
	//tm = gmtime( &dateRange->end );
	FileTimeToSystemTime(&dateRange->end, &tm);
	//tm->tm_hour = 23;
	tm.wHour = 23;
	//tm->tm_min = 59;
	tm.wMinute = 59;
	//tm->tm_sec = 59;
	tm.wSecond = 59;
	//dateRange->end = mktime( tm );
	SystemTimeToFileTime(&tm, &dateRange->end);
}

/*
 * FUNCTION
 * 	StateRecall::DateRangeInit
 * 
 * DESCRIPTION
 * 	Initializes the date range start and end times to the current time, and puts the instrument serial number
 * 	into the serial number field.
 */

void
StateRecall::
InitDateRange()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	//dateRange.start = dateRange.end = time( 0 );
	SystemTimeToFileTime(&time, &dateRange.start);
	SystemTimeToFileTime(&time, &dateRange.end);
	dateRange.serNum = factoryData->instrumentSerialNumber;
}

/*
 * FUNCTION
 * 	StateRecall::RadFilesFound
 * 
 * DESCRIPTION
 * 	Determines whether there are any RAD files in the given date range to be archived.
 * 
 * PARAMETERS
 * 	dateRange - Range of dates.
 * 
 * RETURNS
 * 	TRUE if at least one file found, else FALSE.
 */

bool
StateRecall::
RadFilesFound( DateRange_t * dateRange )
{
	wchar_t				filename[255];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	BOOL				bFinished = FALSE;
    FILETIME            localTime;


	wsprintf(filename, L"%S/*.rad", RAD_FILE_DIRECTORY);
	hSearch = FindFirstFile(filename, &fileData);
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
            FileTimeToLocalFileTime (&fileData.ftLastWriteTime, &localTime);
			if ( (CompareFileTime(&localTime, &dateRange->start) >= 0) 
					&& (CompareFileTime(&localTime, &dateRange->end) <= 0) )
			{
				//closedir( dirp );
				LogDebug("StateRecall::RadFilesFound | found a rad file");
				FindClose(hSearch);
				return true;
			}
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("StateRecall::RadFilesFound | no more rad files");
				}
				else
				{
					LogDebug("StateRecall::RadFilesFound | unable to find files");
				}
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("StateRecall::RadFilesFound | INVALID_HANDLE_VALUE");
		return false;
	}

	LogDebug("StateRecall::RadFilesFound | no rad file found");
	FindClose(hSearch);
	return false;
}

/*
 * FUNCTION
 * 	StateRecall::ResultsFound
 * 
 * DESCRIPTION
 * 	Determines whether there are any results in the given date range to be archived.
 * 
 * PARAMETERS
 * 	dateRange - Range of dates.
 * 
 * RETURNS
 * 	TRUE if at least one result found, else FALSE.
 */

bool
StateRecall::
ResultsFound( DateRange_t * dateRange )
{
	FILE *				fp;
	int					i;
	const char *		dbIndex;
	static ResultsDbIndex_t	resultsIndex;

	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;

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
		return false;
	}
	
	// Look for a result in range.
	for ( i = 0; i < RESULTS_MAX; i++ )
	{
		if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
		{
			//if ( (resultsIndex.entries[i].dateTime >= dateRange->start) && (resultsIndex.entries[i].dateTime <= dateRange->end) )
			if ( (CompareFileTime(&resultsIndex.entries[i].dateTime, &dateRange->start) >= 0) 
					&& (CompareFileTime(&resultsIndex.entries[i].dateTime, &dateRange->end) <= 0) )
			{
				return true;
			}
		}
	}

	return false;
}

/*
 * FUNCTION
 * 	StateRecall::TimerThread
 * 
 * DESCRIPTION
 * 	Sleeps the given length of time, then sends a message to signal the main thread.
 * 
 * PARAMETERS
 *	arg - Pointer to the delay time, in seconds.
 */
void *
StateRecall::
TimerThread( void * arg )
{
	const int		msg = MSG_RECALL_TIMER;
	//static mqd_t	queue = mq_open( RECALL_QUEUE_NAME, O_WRONLY );
	static HANDLE	queue = NULL;
	unsigned		time = *(unsigned *)arg;

	if (queue == NULL) {
		MSGQUEUEOPTIONS			options;
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;

		queue = CreateMsgQueue(RECALL_QUEUE_NAME, &options);
		
		if ( queue == NULL )
		{
			LogError( "failed to open queue %s", RECALL_QUEUE_NAME );
			return 0;
		}
	}

	Sleep( time * 1000);

	//mq_send( queue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
	WriteMsgQueue(queue, (void *)&msg, sizeof( int ), INFINITE, NULL);

	return 0;
}

/*
 * Class StateBackground
 */

/*
 * FUNCTION
 * 	StateBackground::Enter
 * 
 * DESCRIPTION
 * 	Sets up the GUI and condition variables upon entrance to this state.
 * 
 * NOTE
 * 	This header applies to all Enter() methods for the following subclasses of
 * 	class State.
 */

void
StateBackground::
Enter()
{
	SendAnalysisMessage( MSG_ANALYSIS_GOTO_FOREGROUND );
}

/*
 * FUNCTION
 * 	StateBackground::ProcessMessage
 * 
 * DESCRIPTION
 * 	Handles a message received by this state.
 * 
 * PARAMETERS
 * 	msgId - ID for the received message.
 * 	data - Data field of the received message.
 * 
 * RETURNS
 * 	State to proceed to, depending on the result of the message processing.  If
 * 	a change in state isn't called for, the current state is returned so a
 * 	state change won't take place.
 *
 * NOTE
 * 	This header applies to all ProcessMessage() methods for the following subclasses of
 * 	class State.
 */

state_t
StateBackground::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_BACKGROUND;
	
	switch ( msgId )
	{
		case MSG_RECALL_GOTO_FOREGROUND:
			GuiReceiveMsg_t msg;
	
			// Inform the GUI that we want input now.
			msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
			//strcpy( msg.data.inputReceiver.name, RECALL_QUEUE_NAME );
			wsprintf(msg.data.inputReceiver.name, L"%s", RECALL_QUEUE_NAME);
			SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );

			// Reenable the service archive button, in case it was disabled due
			// to insufficient space earlier.
			serviceArchiveDisabled = false;

			// Read the results index file so we've got our results index.
			ReadIndexFile();
	
			// Change to the next state.
			if ( listCount < 1 )
			{
				retVal = STATE_NO_RESULTS;
			
			}
			else if ( (settingsData->recallLockoutEnabled) && !LockoutKeyInstalled() )
			{
				retVal = STATE_ENTER_OPERATOR_ID;
			}
			else
			{
				retVal = STATE_SELECT_RECALL_TYPE;
			}
			stateStack.Push( STATE_BACKGROUND );
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

	
/*
 * StateClinicalArchiveTransmitting
 */

void
StateClinicalArchiveTransmitting::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel( strings[ TSTR_CLINICAL_ARCHIVE ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;

	wcscpy( data->progBarText, strings[ MSTR_CLINICAL_ARCHIVE_ELLIPSIS ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;		// transparent
	data->fontColor = 0x004E4E5E;			// our default color
	data->message[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_CANCEL ], true );

	//pthread_create( &archiveThread, 0, ClinicalArchive, (void *)&dateRange );
	archiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClinicalArchive, (void *)&dateRange, 0, NULL);
}

state_t
StateClinicalArchiveTransmitting::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_CLINICAL_ARCHIVE_TRANSMITTING;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	msg;
	
	switch ( msgId )
	{
		case MSG_RECALL_ARCHIVE_PROGRESS:
			msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			msg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_ARCHIVE_COMPLETE:
		case MSG_RECALL_ARCHIVE_DISCONNECT:
			retVal = stateStack.Pop();
			break;
			
		case MSG_RECALL_ARCHIVE_INSUFF_SPACE:
			errNum = SERR_INSUFF_STORAGE;
			serviceArchiveDisabled = true;
			retVal = STATE_ERROR;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Cancel
			stateStack.Push( STATE_CLINICAL_ARCHIVE_TRANSMITTING );
			retVal = STATE_CONFIRM_CANCEL_ARCHIVE;
			break;
	}
	
	return retVal;
}
	

/*
 * class StateConfirmCancelArchive
 */
 
void
StateConfirmCancelArchive::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_CANCEL_ARCHIVE ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_NO ], true );
}

int
StateConfirmCancelArchive::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_CONFIRM_CANCEL_ARCHIVE;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	guiMsg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			
			if ( idata == 0 )		// Cancel confirmed.
			{
				//pthread_cancel( archiveThread );
				TerminateThread(archiveThread, 0);
				CloseHandle(archiveThread);
				stateStack.Pop();			// Throw away previous state.
				retVal = stateStack.Pop();	// Return to state before that.
			}
			else if ( idata == 1 )	// No cancel, so return to previous state.
			{
				retVal = stateStack.Pop();
			}
			break;

		case MSG_RECALL_ARCHIVE_PROGRESS:
			guiMsg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			guiMsg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_ARCHIVE_COMPLETE:
		case MSG_RECALL_ARCHIVE_DISCONNECT:
			// We're done archiving, so we remove the previous state from the
			// stack.  However, we stay in this window until the user presses
			// Confirm or No.  Otherwise, it's a bit unsettling for the window
			// to go away on its own.  Plus, the user may press a button just as
			// the window is going away and the button press would get directed
			// to the wrong window.
			stateStack.Pop();
			break;
					
		case MSG_RECALL_ARCHIVE_INSUFF_SPACE:
			errNum = SERR_INSUFF_STORAGE;
			serviceArchiveDisabled = true;
			retVal = STATE_ERROR;
			stateStack.Pop();		// Return to main archive screen.
			break;
	}
	
	return retVal;
}


/*
 * class StateConfirmCancelTransmit
 */
 
void
StateConfirmCancelTransmit::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_CANCEL_TRANSMISSION ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_NO ], true );
}

int
StateConfirmCancelTransmit::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_CONFIRM_CANCEL_TRANSMIT;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	guiMsg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			
			if ( idata == 0 )		// Cancel confirmed.
			{
				SendSerialCommMessage( MSG_SERIAL_COMM_CANCEL_SEND );
				stateStack.Pop();			// Throw away previous state.
				retVal = stateStack.Pop();	// Return to state before that.
			}
			else if ( idata == 1 )	// No cancel, so return to previous state.
			{
				retVal = stateStack.Pop();
			}
			break;

		case MSG_RECALL_SEND_PROGRESS:
			guiMsg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			guiMsg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_SEND_RESULTS_COMPLETE:
			// We're done transmitting, so we remove the previous state from the
			// stack.  However, we stay in this window until the user presses
			// Confirm or No.  Otherwise, it's a bit unsettling for the window
			// to go away on its own.  Plus, the user may press a button just as
			// the window is going away and the button press would get directed
			// to the wrong window.
			stateStack.Pop();
			break;
	}
	
	return retVal;
}


/*
 * class StateConfirmTransmitAll
 */
 
void
StateConfirmTransmitAll::
Enter()
{
    GuiReceiveMsg_t msg;
    
    SetTopLabel( strings[ TSTR_CONFIRM ] );
    
    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy ( msg.data.middleMessage.message, strings[ MSTR_CONFIRM_TRANSMIT_ALL ] );
    SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
    
    SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_YES ], true );
}

int
StateConfirmTransmitAll::
ProcessMessage( int msgId, char * data )
{
    int             retVal = STATE_CONFIRM_TRANSMIT_ALL;
    int             idata = *(int *)data;
    
    switch ( msgId )
    {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            if ( idata == 0 )       // Back
            {
                retVal = stateStack.Pop();  // Return to state before that.
            }
            else if ( idata == 1 )  // Home
            {
                retVal = stateStack.Home();
            }
            else if ( idata == 2 )  // Yes
            {
                switch (settings->comm.commInterface)
                {
                    case COMM_ASTM:
                        retVal = STATE_TRANSMIT_RANGE_QUEUED;
                        dateRange.start.dwHighDateTime = 0;
                        dateRange.start.dwLowDateTime = 0;
                        dateRange.end.dwHighDateTime = ULONG_MAX;
                        dateRange.end.dwLowDateTime = ULONG_MAX;
                        break;
                    case COMM_ASCII:    // fall-thru intentional
                    case COMM_XML:
                        retVal = STATE_TRANSMIT_ALL;
                        break;
                    default:
                        break;
                }
            }
            break;
    }
    
    return retVal;
}

/*
 * StateDateRangeEnd
 */

void
StateDateRangeEnd::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;

	SetTopLabel( strings[ TSTR_ENTER_END_DATE ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

	wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
	wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

	//tm = gmtime( &dateRange.end );
	FileTimeToSystemTime(&dateRange.end, &tm);

	wsprintf( data->textLeft, L"%d", tm.wDay /*tm->tm_mday*/ );
	wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth /*tm->tm_mon*/ ) );
	wsprintf( data->textRight, L"%d", tm.wYear /*tm->tm_year + 1900*/ );
	msg.data.middleEnterDate.buttonText[0] = L'\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

state_t
StateDateRangeEnd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_DATE_RANGE_END;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0:
					//tm->tm_mday++;
					tm.wDay++;
					if ( tm.wDay > 31 /*tm->tm_mday > 31*/ )
					{
						/*tm->tm_mday = 1;*/
						tm.wDay = 1;
					}
					updateDay = true;
					break;
				case 1:
					//tm->tm_mon++;
					tm.wMonth++;
					if ( tm.wMonth > 12 /*tm->tm_mon > 11*/ )
					{
						/*tm->tm_mon = 0;*/
						tm.wMonth = 1;
					}
					updateMonth = true;
					break;
				case 2:
					//tm->tm_year++;
					tm.wYear++;
					if ( tm.wYear < 0 /*tm->tm_year < 0*/ )
					{
						/*tm->tm_year = 0;*/
						tm.wYear = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0:
					/*tm->tm_mday--;*/
					tm.wDay--;
					if ( tm.wDay < 1/*tm->tm_mday < 1*/ )
					{
						/*tm->tm_mday = 31;*/
						tm.wDay = 31;
					}
					updateDay = true;
					break;
				case 1:
					/*tm->tm_mon--;*/
					tm.wMonth--;
					if ( tm.wMonth < 1 /*tm->tm_mon < 0*/ )
					{
						/*tm->tm_mon = 11;*/
						tm.wMonth = 12;
					}
					updateMonth = true;
					break;
				case 2:
					/*tm->tm_year--;*/
					tm.wYear--;
					if ( tm.wYear < 0 /*tm->tm_year < 0*/ )
					{
						/*tm->tm_year = 0;*/
						tm.wYear = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:

			// We save the time regardless of whether we're going on or returning to the start date screen. If we
			// go back, then when we come back here we want the time to stay at what we last set.
			//dateRange.end = mktime( tm );
			SystemTimeToFileTime(&tm, &dateRange.end);
				
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
					
				case 1:		// Done
					stateStack.Pop();				// Pop the Start Date screen.
					ProcessDates( &dateRange );		// Reverse the order, if necessary.

					if ( dateRangeMode == DATE_RANGE_CLINICAL_ARCHIVE )
					{
						retVal = STATE_CLINICAL_ARCHIVE_TRANSMITTING;

						if ( RadFilesFound( &dateRange ) == false )
						{
							errNum = SERR_NO_FILES_TO_ARCHIVE;
							retVal = STATE_ERROR;
						}
					}
					else	// DATE_RANGE_TRANSMIT
					{
						if (( settings->comm.commInterface == COMM_ASTM ) || (settings->comm.commInterface == COMM_HL7)) 
						{
							retVal = STATE_TRANSMIT_RANGE_QUEUED;
						}
						else
						{						
							retVal = STATE_TRANSMIT_RANGE_TRANSMITTING;
						}

						if ( systemData->inResearchMode )
						{
							if ( RadFilesFound( &dateRange ) == false )
							{
								errNum = SERR_NO_FILES_TO_TRANSMIT;
								retVal = STATE_ERROR;
							}
						}
						else
						{
							if ( ResultsFound( &dateRange ) == false )
							{
								errNum = SERR_NO_RESULTS_TO_TRANSMIT;
								retVal = STATE_ERROR;
							}
						}
					}
					break;
			}
			break;

		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.wDay /*tm->tm_mday*/ );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth /*tm->tm_mon*/ ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.wYear /*tm->tm_year + 1900*/ );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	return retVal;
}


/*
 * StateDateRangeStart
 */

void
StateDateRangeStart::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;

	SetTopLabel( strings[ TSTR_ENTER_START_DATE ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

	wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
	wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

	//tm = gmtime( &dateRange.start );
	FileTimeToSystemTime(&dateRange.start, &tm);

	wsprintf( data->textLeft, L"%d", tm.wDay /*tm->tm_mday*/ );
	wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth /*tm->tm_mon*/ ) );
	wsprintf( data->textRight, L"%d", tm.wYear /*tm->tm_year + 1900*/ );
	msg.data.middleEnterDate.buttonText[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

state_t
StateDateRangeStart::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_DATE_RANGE_START;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0:
					/*tm->tm_mday++;*/
					tm.wDay++;
					if ( tm.wDay > 31 /*tm->tm_mday > 31*/ )
					{
						/*tm->tm_mday = 1;*/
						tm.wDay = 1;
					}
					updateDay = true;
					break;
				case 1:
					//tm->tm_mon++;
					tm.wMonth++;
					if ( tm.wMonth > 12 /*tm->tm_mon > 11*/ )
					{
						/*tm->tm_mon = 0;*/
						tm.wMonth = 1;
					}
					updateMonth = true;
					break;
				case 2:
					//tm->tm_year++;
					tm.wYear++;
					if ( tm.wYear < 0 /*tm->tm_year < 0*/ )
					{
						/*tm->tm_year = 0;*/
						tm.wYear = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0:
					/*tm->tm_mday--;*/
					tm.wDay--;
					if ( tm.wDay < 1 /*tm->tm_mday < 1*/ )
					{
						/*tm->tm_mday = 31;*/
						tm.wDay = 31;
					}
					updateDay = true;
					break;
				case 1:
					/*tm->tm_mon--;*/
					tm.wMonth--;
					if ( tm.wMonth < 1 /*tm->tm_mon < 0*/ )
					{
						/*tm->tm_mon = 11;*/
						tm.wMonth = 12;
					}
					updateMonth = true;
					break;
				case 2:
					/*tm->tm_year--;*/
					tm.wYear--;
					if ( tm.wYear < 0 /*tm->tm_year < 0*/ )
					{
						/*tm->tm_year = 0;*/
						tm.wYear = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
					
				case 1:		// Done
					stateStack.Push( STATE_DATE_RANGE_START );
					//dateRange.start = mktime( tm );
					SystemTimeToFileTime(&tm, &dateRange.start);
					retVal = STATE_DATE_RANGE_END;
					break;
			}
			break;

		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.wDay /*tm->tm_mday*/ );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth /*tm->tm_mon*/ ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.wYear /*tm->tm_year + 1900*/ );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	return retVal;
}


/*
 * Class StateEnterDate
 */
 
void
StateEnterDate::
Enter()
{
	GuiReceiveMsg_t	msg;
	//time_t			t;
	//struct tm * 	currentTime;
	SYSTEMTIME		currentTime;

	SetTopLabel( strings[ TSTR_ENTER_DATE ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
	
	wcscpy( msg.data.middleEnterDate.labelLeft, strings[ MSTR_DAY ] );
	wcscpy( msg.data.middleEnterDate.labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( msg.data.middleEnterDate.labelRight, strings[ MSTR_YEAR ] );
	
	// Get the current date.
	/*t = time( 0 );
	currentTime = gmtime( &t );*/
	GetLocalTime(&currentTime);
	
	// Save the current date values.
	day = currentTime.wDay; /*currentTime->tm_mday;*/
	month = currentTime.wMonth; /*currentTime->tm_mon;*/
	year = currentTime.wYear; /*currentTime->tm_year + 1900*/;
	
	wsprintf( msg.data.middleEnterDate.textLeft, L"%d", day );
	wcscpy( msg.data.middleEnterDate.textMiddle, strings.Month( month ) );
	wsprintf(msg.data.middleEnterDate.textRight, L"%d", year );
	
	msg.data.middleEnterDate.buttonText[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t
StateEnterDate::
ProcessMessage( int msgId, char * data )
{
	state_t			retVal = STATE_ENTER_DATE;
	bool			updateDay = false;
	bool			updateMonth = false;
	bool			updateYear = false;
	GuiReceiveMsg_t	updateMsg;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0: day++;		updateDay = true;	break;
				case 1:	month++;	updateMonth = true;	break;
				case 2: year++;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0: day--;		updateDay = true;	break;
				case 1:	month--;	updateMonth = true;	break;
				case 2: year--;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )
			{
				retVal = stateStack.Pop();
			}
			else
			{
				retVal = STATE_LIST;
				sortOrder = SORT_DATE;
				Sort();
				topEntry = FindSelectedDate();
				stateStack.Push( STATE_ENTER_DATE );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}

	if ( updateDay )
	{
		if ( day > 31 )
			day = 1;
		else if ( day < 1 )
			day = 31;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( updateMsg.data.middleEnterDate.textLeft, L"%d", day );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		if ( month > 12 )
			month = 1;
		else if ( month < 1 )
			month = 12;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wcscpy( updateMsg.data.middleEnterDate.textMiddle, strings.Month( month ) );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
		
	}
	else if ( updateYear )
	{
		if ( year < 1970 )
			year = 1970;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( updateMsg.data.middleEnterDate.textRight, L"%d", year );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}
		
	return retVal;
}

int
StateEnterDate::
FindSelectedDate()
{
	SYSTEMTIME	tm;
	FILETIME	date;
    LONG        tCpm;
    const int  daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
    
	memset( &tm, 0, sizeof ( SYSTEMTIME ) );

    if (day >= daysInMonth [month - 1]) {
        tm.wDay = 1;
        if (month == 12) {
            tm.wMonth = 1;
            tm.wYear = year + 1;
        }  else {
            tm.wMonth = month +1;
            tm.wYear = year;
        }
    } else {
	    tm.wDay = day +1;
	    tm.wMonth = month ;
	    tm.wYear = year;
    }
	SystemTimeToFileTime(&tm, &date);
	
	// Look for the first dateTime value less than or equal to the entered date.
	for ( int i = 0; i < listCount; i++ )
	{
        
		if ((CompareFileTime(&resultsIndex.entries[i].dateTime, &date) <= 0))
		{
            return i;
        }
	}
	
	// All the dates are greater than the one entered, so we point at the end of the list.
	return listCount > 5 ? listCount-5 : 0;
}

/*
 * class StateEnterOperatorId
 */
 
void
StateEnterOperatorId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_OPERATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterOperatorId::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_OPERATOR_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	Expiry_t *		expiry;
	FILETIME		currentFT;
	SYSTEMTIME		time;
    bool            idOk = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Done, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:

            if (strcmp( sdata, LOCKOUT_BACKDOOR ) == 0 )
            {
                idOk = true;
            } 
            else 
            {

			    expiry = expirys.GetOp( sdata );
			    GetLocalTime(&time);
			    SystemTimeToFileTime(&time, &currentFT);

			    // Invalid operator ID.
			    if ( expiry == 0 )
			    {
				    errNum = SERR_UNAUTHORIZED_OPERATOR;
				    stateStack.Push( STATE_ENTER_OPERATOR_ID );
				    retVal = STATE_WARNING;
			    }
			    // Operator ID expired.
			    else if ( (expiry->expiration.dwLowDateTime != 0 || expiry->expiration.dwHighDateTime != 0) && (CompareFileTime(&(expiry->expiration), &currentFT) == -1) )
			    {
				    errNum =  SERR_OPERATOR_RENEWAL_REQUIRED;
				    retVal = STATE_WARNING;
			    }

			    // Operator ID OK.
			    else
			    {
				    idOk = true;	
			    }
            }
            if ( idOk )
            {
                retVal = STATE_SELECT_RECALL_TYPE;
            }

			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * Class StateEnterPatientControlId
 */
 
void 
StateEnterPatientControlId::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_ENTER_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t 
StateEnterPatientControlId::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ENTER_PATIENT_CONTROL_ID;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	char *	sdata = ((GuiSendMsgData_t *)data)->sdata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_ACTIVATE:
			GetId();
			break;

		case MSG_GUI_OUT_ID_TEXT:
			retVal = STATE_LIST;
			sortOrder = SORT_ID;
			Sort();
			topEntry = FindSelectedId( sdata );
			stateStack.Push( STATE_ENTER_PATIENT_CONTROL_ID );
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )
			{
				retVal =  stateStack.Pop();
			}
			else
			{
				GetId();
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

void
StateEnterPatientControlId::
GetId()
{
	GuiReceiveMsg_t	msg;

	msg.msgId = MSG_GUI_GET_ID_TEXT;
	SendGuiMessage( &msg, 0 );
}

int
StateEnterPatientControlId::
FindSelectedId( char * patientControlId )
{
	for ( int i = 0; i < listCount; i++ )
	{
		int cmp = strcmp( patientControlId, resultsIndex.entries[i].patientControlId );

		// Found.
		if ( cmp == 0 )
		{
			return i;
		}
		
		// Not found, but not at end of list before we found an ID that's
		// greater, so display section of list with IDs around the requested one.
		else if ( cmp < 0 )
		{
			return i > 2 ? i-2 : 0;
		}
	}
	
	// We didn't find it, and all the IDs are less than the one entered,
	// so we point at the end of the list.
	return listCount > 5 ? listCount-5 : 0;
}

/*
 * class StateError
 */
 
void
StateError::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	StringId_t			msgStr;
	const wchar_t *		topText;
	
	// Get the message text.
	msgStr = systemError.GetScreenText( errNum );

	guiMsg.msgId = MSG_GUI_WIN_WARNING1;
	topText = strings[ TSTR_ERROR ];

	wcscpy( guiMsg.data.winError.topLabel, topText );

	// Add the error number to the beginning of the error message.
	wsprintf( guiMsg.data.winError.message, L"%x\n%s", errNum, strings[ msgStr ] );

	wcscpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateError::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_ERROR;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			errNum = SERR_NONE;
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( errNum ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ERROR );
			break;
				
		default:
			break;
	}
	
	return retVal;
}

/*
 * Class StateInfo
 */

void
StateInfo::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_INFORMATION ] );
	
	msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
	wcscpy( msg.data.middleViewResults.text, infoText );
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], true );
}

state_t
StateInfo::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_INFO;
//	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
//			switch ( idata )
//			{
//				case 0:
					retVal = stateStack.Pop();
//					break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * Class StateNoResults
 */

void
StateNoResults::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_NO_RESULTS ] );
		
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_NO_RESULTS ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_HOME ], true );
}

state_t
StateNoResults::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SELECT_RECALL_TYPE;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Home
			retVal = stateStack.Home();
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateList
 */

void
StateList::
Enter()
{
	switch ( sortOrder )
	{
		case SORT_DATE:
			SetTopLabel( strings[ TSTR_ALL_RESULTS_BY_DATE ] );
			break;
		case SORT_ID:
			SetTopLabel( strings[ TSTR_ALL_RESULTS_BY_ID ] );
			break;
		case SORT_CONTROL:
			SetTopLabel( strings[ TSTR_CONTROL_BY_DATE ] );
			break;
		case SORT_PATIENT:
			SetTopLabel( strings[ TSTR_PATIENT_BY_DATE ] );
			break;
		default:
			SetTopLabel( L"" );
			break;
	}

// GLD
// Code could me moved up here.
	SetMiddleList();
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t
StateList::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_LIST;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t	msg;
	int				i, j;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			selectedItem = resultsIndex.entries[ topEntry + idata ].index;
			selectedItemIsPatient = resultsIndex.entries[ topEntry + idata ].patientControl != 'C';
			selectedItemIsError = resultsIndex.entries[ topEntry + idata ].errorFlag;
			retVal = STATE_VIEW_RESULT;
			stateStack.Push( STATE_LIST );
			break;

		case MSG_GUI_OUT_UP:
			if ( topEntry > 0 )
			{
				topEntry = (topEntry -= 4) < 0 ? 0 : topEntry;
				
				msg.msgId = MSG_GUI_UPDATE_LIST_RESULTS;

				for ( i = 0, j = topEntry; i < 5 && j < listCount; i++, j++ )
				{	
					wcscpy( msg.data.middleList.items[i][0], CstrToWstr (GetResultType( j )) );
					wcscpy( msg.data.middleList.items[i][1], CstrToWstr (GetResultDate( j ) ));
					wcscpy( msg.data.middleList.items[i][2], CstrToWstr (resultsIndex.entries[j].patientControlId ));
				}
				
				for ( ; i < 5; i++ )
				{
					msg.data.middleList.items[i][0][0] = 0;
					msg.data.middleList.items[i][0][1] = 0;
					msg.data.middleList.items[i][0][2] = 0;
				}

				msg.data.middleList.isSelectable = 1;
				msg.data.middleList.hasUpDown = 1;
	
				SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
			}
			break;

		case MSG_GUI_OUT_DOWN:
			if ( topEntry + 5 < listCount )
			{
				topEntry += 4;
				
				msg.msgId = MSG_GUI_UPDATE_LIST_RESULTS;

				for ( i = 0, j = topEntry; i < 5 && j < listCount; i++, j++ )
				{	
					wcscpy( msg.data.middleList.items[i][0], CstrToWstr (GetResultType( j )) );
					wcscpy( msg.data.middleList.items[i][1], CstrToWstr (GetResultDate( j )) );
					wcscpy( msg.data.middleList.items[i][2], CstrToWstr (resultsIndex.entries[j].patientControlId));
				}
				
				for ( ; i < 5; i++ )
				{
					msg.data.middleList.items[i][0][0] = 0;
					msg.data.middleList.items[i][0][1] = 0;
					msg.data.middleList.items[i][0][2] = 0;
				}

				msg.data.middleList.isSelectable = 1;
				msg.data.middleList.hasUpDown = 1;
	
				SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = idata == 0 ? stateStack.Pop() : stateStack.Home();
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * FUNCTION
 * 	StateList::SetMiddleList
 * 
 * DESCRIPTION
 * 	Sets the contents of entries list in the GUI.
 */

void
StateList::
SetMiddleList()
{
	GuiReceiveMsg_t	msg;
	int				i, j;
	
	msg.msgId = MSG_GUI_MIDDLE_LIST_RESULTS;

	for ( i = 0, j = topEntry; i < 5 && j < listCount; i++, j++ )
	{	
		wcscpy( msg.data.middleList.items[i][0], CstrToWstr (GetResultType( j )) );
		wcscpy( msg.data.middleList.items[i][1], CstrToWstr (GetResultDate( j )) );
		wcscpy( msg.data.middleList.items[i][2], CstrToWstr (resultsIndex.entries[j].patientControlId) );
	}
	
	for ( ; i < 5; i++ )
	{
		msg.data.middleList.items[i][0][0] = 0;
		msg.data.middleList.items[i][0][1] = 0;
		msg.data.middleList.items[i][0][2] = 0;
	}
		
	msg.data.middleList.isSelectable = 1;
	msg.data.middleList.hasUpDown = 1;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
}

/*
 * Class StateSearchUsing
 */
 
void 
StateSearchUsing::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_SEARCH ] );
	
	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	wcscpy( msg.data.middleButtons.buttonText[0], strings[ MSTR_PATIENT_CONTROL_ID ] );
	wcscpy( msg.data.middleButtons.buttonText[1], strings[ MSTR_DATE ] );
	wcscpy( msg.data.middleButtons.buttonText[2], strings[ MSTR_PATIENTS ] );
	wcscpy( msg.data.middleButtons.buttonText[3], strings[ MSTR_CONTROLS ] );
	msg.data.middleButtons.buttonText[4][0] = 0;
	msg.data.middleButtons.buttonText[5][0] = 0;
	msg.data.middleButtons.buttonHighlighted[0] = 1;
	msg.data.middleButtons.buttonHighlighted[1] = 0;
	msg.data.middleButtons.buttonHighlighted[2] = 0;
	msg.data.middleButtons.buttonHighlighted[3] = 0;
	msg.data.middleButtons.buttonHighlighted[4] = 0;
	msg.data.middleButtons.buttonHighlighted[5] = 0;

	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t 
StateSearchUsing::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SEARCH_USING;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:
					retVal = STATE_ENTER_PATIENT_CONTROL_ID;
					stateStack.Push( STATE_SEARCH_USING );
					break;
					
				case 1:
					retVal = STATE_ENTER_DATE;
					stateStack.Push( STATE_SEARCH_USING );
					break;
					
				case 2:
					retVal = STATE_LIST;
					sortOrder = SORT_PATIENT;
					Sort();
					topEntry = 0;
					stateStack.Push( STATE_SEARCH_USING );
					break;
					
				case 3:
					retVal = STATE_LIST;
					sortOrder = SORT_CONTROL;
					Sort();
					topEntry = 0;
					stateStack.Push( STATE_SEARCH_USING );
					break;
					
				default:
					break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = idata == 0 ? stateStack.Pop() : stateStack.Home();
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * Class StateSelectOutputType
 */

void 
StateSelectOutputType::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_SELECT_REPORT ] );

	msg.msgId = MSG_GUI_MIDDLE_SELECT_TYPE;
	
	memset( &msg.data.middleSelectType, 0, sizeof( GuiMiddleSelectTypeData_t ) );
	wcscpy( msg.data.middleSelectType.buttonText[0], strings[ MSTR_ERROR_REPORT ] );
	wcscpy( msg.data.middleSelectType.buttonText[4], strings[ MSTR_PRINT ] );
	wcscpy( msg.data.middleSelectType.buttonText[5], strings[ MSTR_XMIT ] );
	msg.data.middleSelectType.buttonChecked[0] = printSelected ? 1 : 0;
	msg.data.middleSelectType.buttonChecked[1] = transmitSelected ? 1 : 0;

	SendGuiMessage( &msg, sizeof( GuiMiddleSelectTypeData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t 
StateSelectOutputType::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SELECT_OUTPUT_TYPE;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			reportType = REPORT_ERROR;
			retVal = STATE_SENDING_REPORT;
			stateStack.Pop();
			break;

		case MSG_GUI_OUT_TOGGLE:
			switch ( idata )
			{
				case 4:		printSelected = false;		break;
				case 0xF4:	printSelected = true;		break;
				case 5:		transmitSelected = false;	break;
				case 0xF5:	transmitSelected = true;	break;
				default:								break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = idata == 0 ? stateStack.Pop() : stateStack.Home();
			break;
		
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}


/*
 * Class StateSelectRecallType
 */

void 
StateSelectRecallType::
Enter()
{
	GuiReceiveMsg_t	msg;
	//struct stat		statBuf;
	ULARGE_INTEGER		availableFreeBytes;
	ULARGE_INTEGER		totalBytes;
	ULARGE_INTEGER		totalFreeBytes;

	SetTopLabel( strings[ TSTR_RECALL ] );
	
	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	memset( &msg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );

	wcscpy( msg.data.middleButtons.buttonText[0], strings[ MSTR_LAST_ROTOR ] );
	wcscpy( msg.data.middleButtons.buttonText[1], strings[ MSTR_SEARCH ] );
	wcscpy( msg.data.middleButtons.buttonText[2], strings[ MSTR_BROWSE ] );
	wcscpy( msg.data.middleButtons.buttonText[3], strings[ MSTR_TRANSMIT_DATE_RANGE ] );

	if ( !systemData->inResearchMode )
	{
		wcscpy( msg.data.middleButtons.buttonText[4], strings[ MSTR_TRANSMIT_ALL ] );
	}
	
	if ((!serviceArchiveDisabled)	/*&&
		 ((stat("/dev/hd1t6", &statBuf ) == 0)  ||
		  (stat("/dev/hd1t11", &statBuf ) == 0) ||
		  (stat("/dev/hd1t12", &statBuf ) == 0) ||
          (stat("/fs/hdumass10-dos-1",&statBuf) ==0))*/)
	{
		if (GetDiskFreeSpaceEx(L"/Hard Disk2", &availableFreeBytes, &totalBytes, &totalFreeBytes) != 0)
		{
			LogDebug("StateSelectRecallType::Enter | Service Archive enabled && Hard Disk2 Found");
			if ( systemData->inResearchMode )
			{
				wcscpy( msg.data.middleButtons.buttonText[5], strings[ MSTR_CLINICAL_ARCHIVE ] );
			}
			else
			{
				wcscpy( msg.data.middleButtons.buttonText[5], strings[ MSTR_SERVICE_ARCHIVE ] );
			}
		}
	}

	msg.data.middleButtons.buttonHighlighted[0] = 1;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );
		
		
	printSelected = true;
	transmitSelected = false;

	SetBottomButtons( strings[ BSTR_HOME ], true );
}

state_t 
StateSelectRecallType::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SELECT_RECALL_TYPE;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
// gld
// look for spots where we're always pushing the same thing.
			switch ( idata )
			{
				case 0:		// Last Disc
					retVal = STATE_VIEW_RESULT;
					selectedItem = lastRunIndex;
					selectedItemIsPatient = lastRunIsPatient;
					selectedItemIsError = lastRunIsError;
					stateStack.Push( STATE_SELECT_RECALL_TYPE );
					break;
					
				case 1:		// Search
					retVal = STATE_SEARCH_USING;
					stateStack.Push( STATE_SELECT_RECALL_TYPE );
					break;
					
				case 2:		// Browse
					retVal = STATE_LIST;
					sortOrder = SORT_DATE;
					Sort();
					topEntry = 0;
					stateStack.Push( STATE_SELECT_RECALL_TYPE );
					break;

				case 3:		// Transmit Date Range
					retVal = STATE_DATE_RANGE_START;
					InitDateRange();
					dateRangeMode = DATE_RANGE_TRANSMIT;
					stateStack.Push( STATE_SELECT_RECALL_TYPE );
					break;
					
				case 4:		// Transmit All
                    stateStack.Push( STATE_SELECT_RECALL_TYPE );
                    retVal = STATE_CONFIRM_TRANSMIT_ALL;
					break;

				case 5:		// Service Archive / Clinical Archive
					if ( systemData->inResearchMode )
					{
						retVal = STATE_DATE_RANGE_START;	// Clinical Archive
						dateRangeMode = DATE_RANGE_CLINICAL_ARCHIVE;
						InitDateRange();
					}
					else
					{
						retVal = STATE_SERVICE_ARCHIVE;		// Service Archive
					}
					stateStack.Push( STATE_SELECT_RECALL_TYPE );
					break;

				default:
					break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Home
			retVal = stateStack.Home();
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * Class StateSelectReportType
 */

void 
StateSelectReportType::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_SELECT_REPORT ] );

	if ( !systemData->inResearchMode )
	{
		msg.msgId = MSG_GUI_MIDDLE_SELECT_TYPE;
		
		wcscpy( msg.data.middleSelectType.buttonText[0], strings[ MSTR_ALL ] );
		wcscpy( msg.data.middleSelectType.buttonText[1], strings[ MSTR_RESULTS ] );
		wcscpy( msg.data.middleSelectType.buttonText[2], strings[ MSTR_IQC ] );
		wcscpy( msg.data.middleSelectType.buttonText[3], strings[ MSTR_ERROR_REPORT ] );
		wcscpy( msg.data.middleSelectType.buttonText[4], strings[ MSTR_PRINT ] );
		wcscpy( msg.data.middleSelectType.buttonText[5], strings[ MSTR_XMIT ] );
		msg.data.middleSelectType.buttonChecked[0] = printSelected ? 1 : 0;
		msg.data.middleSelectType.buttonChecked[1] = transmitSelected ? 1 : 0;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleSelectTypeData_t ) );
	}
	
	else
	{
		msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
		
		memset( &msg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
		wcscpy( msg.data.middleButtons.buttonText[0], strings[ MSTR_ALL ] );
		wcscpy( msg.data.middleButtons.buttonText[1], strings[ MSTR_RESULTS ] );
		wcscpy( msg.data.middleButtons.buttonText[2], strings[ MSTR_IQC ] );
		wcscpy( msg.data.middleButtons.buttonText[3], strings[ MSTR_ERROR_REPORT ] );
		msg.data.middleButtons.buttonHighlighted[0] = 1;

		if (( settings->comm.commInterface != COMM_ASTM ) && (settings->comm.commInterface != COMM_HL7))
		{
			wcscpy( msg.data.middleButtons.buttonText[4], L"Send RAD" );
		}
		
		SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );
		
		printSelected = true;
		transmitSelected = false;
	}
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false );
}

state_t 
StateSelectReportType::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SELECT_REPORT_TYPE;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			retVal = STATE_SENDING_REPORT;
			
			/*
			 * If All is selected, then when it's done printing/transmitting,
			 * it'll go back to whichever screen was before the View Results
			 * screen.  For the rest, it returns to this screen in case the user
			 * wishes to print/transmit something more.
			 */

			switch ( idata )
			{
				case 0:		// All
					reportType = REPORT_ALL;
					stateStack.Pop();
					break;
				case 1:		// Results
					reportType = REPORT_RESULTS;
					stateStack.Push( STATE_SELECT_REPORT_TYPE );
					break;
				case 2:		// IQC
					reportType = REPORT_IQC;
					stateStack.Push( STATE_SELECT_REPORT_TYPE );
					break;
				case 3:		// Error
					reportType = REPORT_ERROR;
					stateStack.Push( STATE_SELECT_REPORT_TYPE );
					break;
				case 4:		// Send RAD
					reportType = REPORT_RESULTS;
					printSelected = false;
					transmitSelected = true;
					break;
			}
			break;

		case MSG_GUI_OUT_TOGGLE:
			switch ( idata )
			{
				case 4:		printSelected = false;		break;
				case 0xF4:	printSelected = true;		break;
				case 5:		transmitSelected = false;	break;
				case 0xF5:	transmitSelected = true;	break;
				default:								break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = idata == 0 ? stateStack.Pop() : stateStack.Home();
			break;
		
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * Class StateSendingReport
 */
 
void 
StateSendingReport::
Enter()
{
	GuiReceiveMsg_t	guiMsg;
	SerialCommMsg_t	serialCommMsg;
	bool			abbreviated = selectedItemIsError && !systemData->inResearchMode;
    PrinterMsg_t    printerMsg;

	SetTopLabel( 0 );
	
	guiMsg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	
	wcscpy( guiMsg.data.middleMessage.message, strings[ MSTR_SENDING_REPORT ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();	


	switch ( reportType )
	{
		case REPORT_ALL:
			if ( printSelected )
			{
                printerMsg.selectedItem = selectedItem;
                printerMsg.abbreviated  = abbreviated;
                printerMsg.msgId= MSG_PRINT_RESULTS_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);
                printerMsg.msgId = MSG_PRINT_IQC_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);
                printerMsg.msgId = MSG_PRINT_ERROR_REPORT_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);
			}

			serialCommMsg.reportData.sendResults = true;
			serialCommMsg.reportData.sendIqc = true;
			serialCommMsg.reportData.sendErrorReport = true;
			serialCommMsg.reportData.abbreviatedErrorReport = abbreviated;
			break;
					
		case REPORT_RESULTS:
			if (printSelected) {
                printerMsg.selectedItem = selectedItem;
                printerMsg.abbreviated  = abbreviated;
                printerMsg.msgId= MSG_PRINT_RESULTS_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);

			}

			serialCommMsg.reportData.sendResults = true;
			serialCommMsg.reportData.sendIqc = false;
			serialCommMsg.reportData.sendErrorReport = false;
			break;
					
		case REPORT_IQC:
			if  (printSelected) {
                printerMsg.selectedItem = selectedItem;
                printerMsg.abbreviated  = abbreviated;
                printerMsg.msgId = MSG_PRINT_IQC_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);
			}

			serialCommMsg.reportData.sendResults = false;
			serialCommMsg.reportData.sendIqc = true;
			serialCommMsg.reportData.sendErrorReport = false;
			break;
					
		case REPORT_ERROR:
			if (printSelected) {
                printerMsg.selectedItem = selectedItem;
                printerMsg.abbreviated  = abbreviated;
                printerMsg.msgId = MSG_PRINT_ERROR_REPORT_BY_INDEX;
                errNum = SendPrinterMessage (&printerMsg);
			}

			serialCommMsg.reportData.sendResults = false;
			serialCommMsg.reportData.sendIqc = false;
			serialCommMsg.reportData.sendErrorReport = true;
			serialCommMsg.reportData.abbreviatedErrorReport = abbreviated;
			break;

		default:
			break;
	}
	
	if ( transmitSelected )
	{
		if ( settings->comm.commInterface == COMM_ASTM )
		{
			AstmMsg_t	astmMsg;
			
			astmMsg.msgId = MSG_ASTM_LAST_ROTOR_AVAILABLE;
			astmMsg.data = selectedItem;
			SendAstmMessage( &astmMsg, sizeof( astmMsg.data ) );
		}
        else if  ( settings->comm.commInterface == COMM_HL7 )
        {
            Hl7Msg_t   hl7Msg;

            hl7Msg.msgId = MSG_HL7_LAST_ROTOR_AVAILABLE;
            hl7Msg.data = selectedItem;
            SendHl7Message (&hl7Msg, sizeof( hl7Msg.data ));

        }
		else
		{
			serialCommMsg.msgId = MSG_SERIAL_COMM_SEND_REPORT;
			serialCommMsg.reportData.resultIndex = selectedItem;
			GetRadFileName( serialCommMsg.reportData.radFileName, selectedItem );
			SendSerialCommMessage( &serialCommMsg, sizeof( SendReportData_t ) );
		}
	}

	// This generates an event that will call StateSendingReport::ProcessMessage.
	SendRecallMessage( MSG_RECALL_TRIGGER );
}

state_t 
StateSendingReport::
ProcessMessage( int msgId, char * data )
{
	if ( msgId == MSG_RECALL_TRIGGER )
	{
		if ( errNum != SERR_NONE )
		{
			return STATE_ERROR;
		}
		else
		{
			return stateStack.Pop();
		}
	}
	else
	{
		return STATE_SENDING_REPORT;
	}
}

/*
 * class StateServiceArchive
 */
 
void
StateServiceArchive::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel( strings[ TSTR_SERVICE_ARCHIVE ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
	wcscpy( data->progBarText, strings[ MSTR_SERVICE_ARCHIVE_ELLIPSIS ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;		// transparent
	data->fontColor = 0x004E4E5E;			// our default color
	data->message[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_CANCEL ], true );

	//pthread_create( &archiveThread, 0, ServiceArchive, (void *)&factoryData->instrumentSerialNumber );
	archiveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServiceArchive, (void *)&factoryData->instrumentSerialNumber, 0, NULL);
}

state_t
StateServiceArchive::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_SERVICE_ARCHIVE;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	msg;
	
	switch ( msgId )
	{
		case MSG_RECALL_ARCHIVE_PROGRESS:
			msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			msg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_ARCHIVE_COMPLETE:
		case MSG_RECALL_ARCHIVE_DISCONNECT:
			retVal = stateStack.Pop();
			break;
			
		case MSG_RECALL_ARCHIVE_INSUFF_SPACE:
			errNum = SERR_INSUFF_STORAGE;
			serviceArchiveDisabled = true;
			retVal = STATE_ERROR;
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Cancel
			stateStack.Push( STATE_SERVICE_ARCHIVE );
			retVal = STATE_CONFIRM_CANCEL_ARCHIVE;
			break;
	}
	
	return retVal;
}

/*
 * class StateTransmitAll
 */
 
void
StateTransmitAll::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel( strings[ TSTR_TRANSMIT_ALL ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
	wcscpy( data->progBarText, strings[ MSTR_TRANSMITTING_ALL ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;		// transparent
	data->fontColor = 0x004E4E5E;			// our default color
	data->message[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_CANCEL ], true );

	if ( (settings->comm.commInterface == COMM_ASCII) || (settings->comm.commInterface == COMM_XML) )
	{
		SendSerialCommMessage( MSG_SERIAL_COMM_SEND_ALL );
	}

// Need timeout timer?
}

state_t
StateTransmitAll::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_TRANSMIT_ALL;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	msg;
	
	switch ( msgId )
	{
		case MSG_RECALL_SEND_PROGRESS:
			msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			msg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_SEND_RESULTS_COMPLETE:
			retVal = stateStack.Pop();
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Cancel
			stateStack.Push( STATE_TRANSMIT_ALL );
			retVal = STATE_CONFIRM_CANCEL_TRANSMIT;
			break;
	}
	
	return retVal;
}


/*
 * StateTransmitRangeQueued
 */

void
StateTransmitRangeQueued::
Enter()
{
	GuiReceiveMsg_t	    msg;
	AstmMsg_t		    astmMsg;
	Hl7Msg_t		    hl7Msg;
	static unsigned		time = 2;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_RESULTS_QUEUED ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );

	//pthread_create( &timerThread, 0, TimerThread, &time );
	timerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, &time, 0, NULL);
		
    if (settings->comm.commInterface == COMM_ASTM) {
	    astmMsg.msgId = MSG_ASTM_SEND_RANGE;
	    astmMsg.rangeData.startDate = dateRange.start;
	    astmMsg.rangeData.endDate = dateRange.end;
	    SendAstmMessage( &astmMsg, sizeof( SendRangeData_t ) );
    } else if (settings->comm.commInterface == COMM_HL7) {
	    hl7Msg.msgId = MSG_HL7_SEND_RANGE;
	    hl7Msg.rangeData.startDate = dateRange.start;
	    hl7Msg.rangeData.endDate = dateRange.end;
	    SendHl7Message (&hl7Msg, sizeof (SendRangeData_t));
    }
}

state_t
StateTransmitRangeQueued::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_TRANSMIT_RANGE_QUEUED;

	if ( msgId = MSG_RECALL_TIMER )
	{
		retVal = stateStack.Pop();
	}

	return retVal;
}

	
/*
 * StateTransmitRangeTransmitting
 */

void
StateTransmitRangeTransmitting::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;
	SerialCommMsg_t					serialCommMsg;

	SetTopLabel( strings[ TSTR_TRANSMIT_DATE_RANGE ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;

	wcscpy( data->progBarText, strings[ MSTR_TRANSMITTING_ELLIPSIS ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;		// transparent
	data->fontColor = 0x004E4E5E;			// our default color
	data->message[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_CANCEL ], true );
		
	serialCommMsg.msgId = MSG_SERIAL_COMM_SEND_RANGE;
	serialCommMsg.rangeData.startDate = dateRange.start;
	serialCommMsg.rangeData.endDate = dateRange.end;
	SendSerialCommMessage( &serialCommMsg, sizeof( SendRangeData_t ) );
}

state_t
StateTransmitRangeTransmitting::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_TRANSMIT_RANGE_TRANSMITTING;
	int				idata = *(int *)data;
	GuiReceiveMsg_t	msg;
	
	switch ( msgId )
	{
		case MSG_RECALL_SEND_PROGRESS:
			msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
			msg.data.middleProgBarMessage.progBarValue = idata;
			SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
			break;
			
		case MSG_RECALL_SEND_RESULTS_COMPLETE:
			retVal = stateStack.Pop();
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Cancel
			stateStack.Push( STATE_TRANSMIT_RANGE_TRANSMITTING );
			retVal = STATE_CONFIRM_CANCEL_ARCHIVE;
			break;
	}
	
	return retVal;
}

/*
 * Class StateViewResult
 */

void
StateViewResult::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_VIEW_RESULT ] );
	
	// If not a results-suppression or we're in Research mode, display all three
	// reports.
	if ( !selectedItemIsError || systemData->inResearchMode || systemData->inDemoMode )
	{
        memset (msg.data.middleViewResults.text, 0 ,sizeof (msg.data.middleViewResults.text)); 
		wchar_t * p = msg.data.middleViewResults.text;
		
		CreateDisplay::CreateResultDisplay( selectedItem, p, &strings);
		p += wcslen( p );
		CreateDisplay::CreateIqcDisplay( selectedItem, p, &strings);
		p += wcslen( p );
		CreateDisplay::CreateErrorReportDisplay( selectedItem, p, false, &strings);
#if 0
        LogDebug("text size = %d\n", wcslen (msg.data.middleViewResults.text));
        LogDebug ("msg size = %d\n", sizeof (GuiMiddleViewResultsData_t));
        printf ("%S\n", msg.data.middleViewResults.text);
#endif
	}

	// Results-suppression and we're not in Research or Demo mode, so only display the
	// error report and omit the results.
	else
	{
		CreateDisplay::CreateErrorReportDisplay( selectedItem, msg.data.middleViewResults.text, true, &strings );
	}

	msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_PRINT ], true );
}

state_t
StateViewResult::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_VIEW_RESULT;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Print
				{
					// If the results aren't for a run with an error, or we're
					// in research mode, allow all the reports.
					if ( !selectedItemIsError || systemData->inResearchMode )
					{
						retVal = STATE_SELECT_REPORT_TYPE;
					}
					// Error report only.
					else
					{
						retVal = STATE_SELECT_OUTPUT_TYPE;
					}
					stateStack.Push( STATE_VIEW_RESULT );
					break;
				}
				default: break;
			}

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateWarning
 */
 
void
StateWarning::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	StringId_t			msgStr;
	const wchar_t *		topText;
	
	// Get the message text.
	msgStr = systemError.GetScreenText( errNum );

	guiMsg.msgId = MSG_GUI_WIN_WARNING1;
	topText = strings[ TSTR_WARNING ];

	wcscpy( guiMsg.data.winError.topLabel, topText );

	// Add the error number to the beginning of the error message.
	wsprintf( guiMsg.data.winError.message, L"%x\n%s", errNum, strings[ msgStr ] );

	wcscpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateWarning::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_WARNING;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			errNum = SERR_NONE;
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( errNum ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_WARNING );
			break;
				
		default:
			break;
	}
	
	return retVal;
}

