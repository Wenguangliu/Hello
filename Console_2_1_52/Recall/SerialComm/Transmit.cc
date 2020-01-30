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
#include <pthread.h>
#include <mqueue.h>
#include <string.h>
#include <sys/neutrino.h>
#include <dirent.h>

#include "SerialComm.h"
#include "Transmit.h"
#include "LogErr.h"
#include "Shmem.h"
#include "ResultsPrint.h"

#define MAX_SEND_RETRIES	3

int				Transmit::fdSer = -1;
pthread_t		Transmit::sendThread = -1;
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
Run( int _fdSer )
{
	ssize_t			receivedSize = 0;
	SerialCommMsg_t	msg;
	struct mq_attr	mqstat;
	termios			saveTerm;

	this->fdSer = _fdSer;

	// Map the settings shared memory, so we can get our comm protocol.
	settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );

	// Map the reference ranges shared memory for transmitting reference ranges.
	refRanges = (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );

	/*
	 * Open the queue and get its size.
	 */
	mqd_t queue = mq_open( SERIAL_COMM_QUEUE_NAME, O_RDONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open message queue" );
		return;
	}
	
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes" );
		return;
	}

	while ( 1 )
	{
		// Wait for a message.
		receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );
		if ( receivedSize != -1 )
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
					case MSG_SERIAL_COMM_SEND_REPORT:
						if ( systemData->inResearchMode == true )
						{
							SetPortForRad( &saveTerm );
							SendRadFile( msg.reportData.radFileName );
							tcsetattr( fdSer, TCSADRAIN, &saveTerm );
						}
						else if ( settingsData->comm.interface == COMM_ASCII )
						{
							SendAsciiReport( msg.reportData );
						}
						else if ( settingsData->comm.interface == COMM_XML )
						{
							SendXmlReport( fdSer, msg.reportData.resultIndex );
						}
						break;
												
					case MSG_SERIAL_COMM_SEND_RANGE:
						// Save the current port settings.
						tcgetattr( fdSer, &saveTerm );

						sendArg.fdSer = fdSer;
						sendArg.startDate = msg.rangeData.startDate;
						sendArg.endDate = msg.rangeData.endDate;
						
						cancelSend = false;

						if ( systemData->inResearchMode == true )
						{
							pthread_create( &sendThread, 0, SendRadRange, &sendArg );
						}
						else if ( settingsData->comm.interface == COMM_ASCII )
						{
							pthread_create( &sendThread, 0, SendAsciiRange, &sendArg );
						}
						else if ( settingsData->comm.interface == COMM_XML )
						{
							pthread_create( &sendThread, 0, SendXmlRange, &sendArg );
						}
						break;
					
					case MSG_SERIAL_COMM_SEND_ALL:
						if ( systemData->inResearchMode == false )
						{
							// Save the current port settings.
							tcgetattr( fdSer, &saveTerm );
							
							// Signal send all.
							sendArg.startDate = sendArg.endDate = 0;
							sendArg.fdSer = fdSer;

							cancelSend = false;

							if ( settingsData->comm.interface == COMM_ASCII )
							{
								pthread_create( &sendThread, 0, SendAsciiRange, &sendArg );
							}
							else if ( settingsData->comm.interface == COMM_XML )
							{
								pthread_create( &sendThread, 0, SendXmlRange, &sendArg );
							}
						}
						break;
												
					case MSG_SERIAL_COMM_CANCEL_SEND:
						if ( sendThread != -1 )
						{
							cancelSend = true;
/*
							pthread_cancel( sendThread );
							sendThread = -1;

							// Restore the port settings.
							tcsetattr( fdSer, TCSANOW, &saveTerm );
*/
						}
						break;

					case MSG_SERIAL_COMM_SEND_REF_RANGES:
						if ( (systemData->inResearchMode == false) && (settingsData->comm.interface == COMM_ASCII) )
						{
							SendRefRanges( msg.sampleType );
						}
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
	SendArg_t		sendArg = *(SendArg_t *)arg;
	DIR *			dirp;
	struct dirent *	direntp;
	struct stat		fileStat;
	char *			ext;
	int				count = 0;
	int				numSent = 0;
	RecallMsg_t		msg;
	termios			saveTerm;

	// Set the port and save the current setting.
	SetPortForRad( &saveTerm );

	chdir( RAD_FILE_DIRECTORY );
	dirp = opendir( "." );
	if ( dirp != 0 )
	{
		// Get the file count.
		for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
		{
			ext = strrchr( direntp->d_name, '.' );
			if ( (ext != 0) && (strcmp( ext, ".rad" ) == 0) )
			{
				if ( stat( direntp->d_name, &fileStat ) != -1 )
				{
					if ( (fileStat.st_mtime >= sendArg.startDate) && (fileStat.st_mtime <= sendArg.endDate) )
					{
						count++;
					}
				}
			}			
		}

		// Send the files.
		if ( count > 0 )
		{
			msg.msgId = MSG_RECALL_SEND_PROGRESS;

			rewinddir( dirp );
			for ( direntp = readdir( dirp ); !cancelSend && (direntp != 0); direntp = readdir( dirp ) )
			{
				ext = strrchr( direntp->d_name, '.' );
				if ( (ext != 0) && (strcmp( ext, ".rad" ) == 0) )
				{
					if ( stat( direntp->d_name, &fileStat ) != -1 )
					{
						if ( (fileStat.st_mtime >= sendArg.startDate) && (fileStat.st_mtime <= sendArg.endDate) )
						{
							SendRadFile( direntp->d_name );
							tcsetattr( fdSer, TCSADRAIN, &saveTerm );
		
							msg.idata = (++numSent * 100) / count;
							SendRecallMessage( &msg, sizeof( int ) );
						}
					}
				}			
			}
		}

		closedir( dirp );
	}
	
	if ( !cancelSend )
	{
		msg.msgId = MSG_RECALL_SEND_RESULTS_COMPLETE;
		SendRecallMessage( &msg, 0 );
	}

	// Signal that we're done.
	sendThread = -1;

	// Reset the port.
	tcsetattr( fdSer, TCSADRAIN, &saveTerm );

	return 0;
}


void
Transmit::
SetPortForRad( termios * saveTerm )
{
	termios	term;

	// Get the current serial port configuration and save the values we're going
	// to change.
	tcgetattr( fdSer, saveTerm );
	term = *saveTerm;
	
	// Set the baud rate and handshaking for RAD file transfer.
	term.c_ispeed = BAUD_RATE_RAD_XFER;
	term.c_ospeed = BAUD_RATE_RAD_XFER;
	term.c_cflag |= IHFLOW | OHFLOW;	// Enable RTS/CTS.
	tcsetattr( fdSer, TCSADRAIN, &term );
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
	struct stat		fileStat;
	int				fdRad;
	int				count;
	static char		buff[4096];		// Static because thread stack is small.
	
	// Get the RAD file's size.
	if ( stat( radFileName, &fileStat ) == -1 )
	{
		LogError( "could not stat file %s", radFileName );
		return;
	}
	
	// Open the RAD file.
	fdRad = open( radFileName, O_RDONLY, 0 );
	if ( fdRad == -1 )
	{
		LogError( "could not open file %s", radFileName );
		return;
	}

	// Send the file size.
	write( fdSer, &fileStat.st_size, sizeof( fileStat.st_size ) );

	// Send the file.
	while ( ( count = read( fdRad, buff, 4096 )) > 0 )
	{
		write( fdSer, buff, count );
	}

	close( fdRad );
}

void
Transmit::
SendAsciiReport( SendReportData_t reportData )
{
	termios	term, saveTerm;
	char	buff[ 2000 ];

	// Get the current serial port configuration and save the values we're going
	// to change.
	tcgetattr( fdSer, &saveTerm );
	term = saveTerm;

	// Set the baud rate and handshaking for report transfer.
	term.c_ispeed = BAUD_RATE_ASCII_DUMP;
	term.c_ospeed = BAUD_RATE_ASCII_DUMP;
	term.c_cflag &= ~(IHFLOW | OHFLOW);	// Disable RTS/CTS.
	tcsetattr( fdSer, TCSADRAIN, &term );

	// Write each requested report, preceding each with a start-of-report character.
	if ( reportData.sendResults )
	{
		write( fdSer, "\x0C", 1 );
		ResultsPrint::CreateResultDisplay( reportData.resultIndex, buff );
		write( fdSer, buff, strlen( buff ) );
	}
	
	if ( reportData.sendIqc )
	{
		write( fdSer, "\x0C", 1 );
		ResultsPrint::CreateIqcDisplay( reportData.resultIndex, buff );
		write( fdSer, buff, strlen( buff ) );
	}

	if ( reportData.sendErrorReport )
	{
		write( fdSer, "\x0C", 1 );
		ResultsPrint::CreateErrorReportDisplay( reportData.resultIndex, buff, reportData.abbreviatedErrorReport );
		write( fdSer, buff, strlen( buff ) );
	}

	// Finish with the end-of-transmission character.
	write( fdSer, "\x1A", 1 );
	
	// Reset the serial port to its previous configuration.
	tcsetattr( fdSer, TCSADRAIN, &saveTerm );
}

void
Transmit::
SendRefRanges( SampleType_t sampleType )
{
	termios	term, saveTerm;
	char	buff[ 2000 ];
	int		i, j;

	// Get the current serial port configuration and save the values we're going
	// to change.
	tcgetattr( fdSer, &saveTerm );
	term = saveTerm;

	// Set the baud rate and handshaking for report transfer.
	term.c_ispeed = BAUD_RATE_ASCII_DUMP;
	term.c_ospeed = BAUD_RATE_ASCII_DUMP;
	term.c_cflag &= ~(IHFLOW | OHFLOW);	// Disable RTS/CTS.
	tcsetattr( fdSer, TCSADRAIN, &term );

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
					for ( j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++ )
					{
						write( fdSer, "\x0C", 1 );
						ResultsPrint::CreateRefRangesDisplay( (SampleType_t)i, (Gender_t)j, buff );
						write( fdSer, buff, strlen( buff ) );
					}
				}
				else
				{
					write( fdSer, "\x0C", 1 );
					ResultsPrint::CreateRefRangesDisplay( (SampleType_t)i, GENDER_UNKNOWN, buff );
					write( fdSer, buff, strlen( buff ) );
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
				write( fdSer, "\x0C", 1 );
				ResultsPrint::CreateRefRangesDisplay( sampleType, (Gender_t)j, buff );
				write( fdSer, buff, strlen( buff ) );
			}
		}
		else
		{
			write( fdSer, "\x0C", 1 );
			ResultsPrint::CreateRefRangesDisplay( sampleType, GENDER_UNKNOWN, buff );
			write( fdSer, buff, strlen( buff ) );
		}
	}

	// Finish with the end-of-transmission character.
	write( fdSer, "\x1A", 1 );
	
	// Reset the serial port to its previous configuration.
	tcsetattr( fdSer, TCSADRAIN, &saveTerm );
}

void *
Transmit::
SendAsciiRange( void * arg )
{
	SendArg_t				sendArg = *(SendArg_t *)arg;
	termios					term, saveTerm;
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
	static ResultsDbIndex_t	resultsIndex;

	if ( IN_CUSTOMER_MODE )
		dbIndex = CUSTOMER_DB_INDEX;
	else
		dbIndex = OTHER_DB_INDEX;

	sendRange = (sendArg.startDate != 0) || (sendArg.endDate != 0);

	// Read the index file.
	fp = fopen( dbIndex, "r" );
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
	tcgetattr( sendArg.fdSer, &saveTerm );
	term = saveTerm;

	// Set the baud rate and handshaking for report transfer.
	term.c_ispeed = BAUD_RATE_ASCII_DUMP;
	term.c_ospeed = BAUD_RATE_ASCII_DUMP;
	term.c_cflag &= ~(IHFLOW | OHFLOW);	// Disable RTS/CTS.
	tcsetattr( fdSer, TCSADRAIN, &term );

	// Get the number of results.
	for ( i = 0, numResults = 0; i < RESULTS_MAX; i++ )
	{
		if ( resultsIndex.entries[i].patientControlId[0] != (char)0xFF )
		{
			if ( !sendRange
				 || ((resultsIndex.entries[i].dateTime >= sendArg.startDate)
					&& (resultsIndex.entries[i].dateTime <= sendArg.endDate)) )
			{
				numResults++;
			}
		}
	}

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
					 || ((resultsIndex.entries[i].dateTime >= sendArg.startDate)
						&& (resultsIndex.entries[i].dateTime <= sendArg.endDate)) )
				{
					if ( resultsIndex.entries[i].errorFlag == false )
					{
						write( sendArg.fdSer, "\x0C", 1 );	// Start-of-report signal.
						ResultsPrint::CreateResultDisplay( resultsIndex.entries[i].index, buff );
						write( sendArg.fdSer, buff, strlen( buff ) );
						
						write( sendArg.fdSer, "\x0C", 1 );
						ResultsPrint::CreateIqcDisplay( resultsIndex.entries[i].index, buff );
						write( sendArg.fdSer, buff, strlen( buff ) );
					}

					write( sendArg.fdSer, "\x0C", 1 );
	
					if ( !systemData->inResearchMode && (resultsIndex.entries[i].errorFlag == true) )
					{
						abbreviated = true;
					}
					else
					{
						abbreviated = false;
					}
					ResultsPrint::CreateErrorReportDisplay( resultsIndex.entries[i].index, buff, abbreviated );
					write( sendArg.fdSer, buff, strlen( buff ) );
					
					// Send percentage completion.
					msg.idata = (++numSent * 100) / numResults;
					SendRecallMessage( &msg, sizeof( int ) );
				}
			}
		} while ( !cancelSend && (i != resultsIndex.lastEntry) );

		// Signal end-of-transmission.
		write( fdSer, "\x1A", 1 );
	}

	// Indicate that we're done.
	sendThread = -1;
	
	if ( !cancelSend )
	{
		msg.msgId = MSG_RECALL_SEND_RESULTS_COMPLETE;
		SendRecallMessage( &msg, 0 );
	}
	
	// Restore the serial port settings.  This takes some time, since
	// the output buffer is 2K.
	tcsetattr( sendArg.fdSer, TCSADRAIN, &saveTerm );

	return 0;
}
	
void
Transmit::
SendRecallMessage( RecallMsg_t * msg, int msgDataLen )
{
	static mqd_t	recallQueue = 0;
	
	if ( recallQueue == 0 )
	{
		recallQueue = mq_open( RECALL_QUEUE_NAME, O_WRONLY );
	
		if ( recallQueue == -1 )
		{
			LogError( "failure to open %s queue", RECALL_QUEUE_NAME );
			recallQueue = 0;
		}
	}
	
	if ( recallQueue != 0 )
	{
		mq_send( recallQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

