#include <mqueue.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ErrLoggerMessages.h"
#include "CommonDefines.h"

#define LOG_FILE_VERSION		"1.1"
#define LOG_FILE_DIRECTORY		"/Abaxis/"
#define LOG_FILE_CURRENT_EXT	"log.current"
#define LOG_FILE_OLD_EXT		"log.old"
#define LOG_FILE_CURRENT		LOG_FILE_DIRECTORY LOG_FILE_CURRENT_EXT
#define LOG_FILE_OLD			LOG_FILE_DIRECTORY LOG_FILE_OLD_EXT
#define LOG_FILE_MAX_SIZE		1000000

static void LogMessage( char * message );
static void	ErrExit( const char * message );
static void RenameLogFiles();

int main( int argc, char *argv[] )
{
	ErrLoggerMsg_t			msg;
	mqd_t					queue;
	static struct mq_attr	mqstat;
	ssize_t					receivedSize = 0;
	FILE *					fp;

	// Open the input queue and get its attributes.
	queue = mq_open( ERR_LOGGER_QUEUE_NAME, O_RDONLY );
	if ( queue == -1 )
	{
		ErrExit( "ErrLogger: failure to open ErrLogger queue" );
	}
		
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		ErrExit( "ErrLogger: failure to get ErrLogger queue attributes" );
	}

	// Make sure both log files exist.
	fp = fopen( LOG_FILE_CURRENT, "a" );
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: unable to open current log file" );
	}
	fclose( fp );
	fp = fopen( LOG_FILE_OLD, "a" );
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: unable to open old log file" );
	}
	fclose( fp );

	// Loop waiting for and processing messages.		
	while ( 1 )
	{
		receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );
	
		if ( receivedSize != -1 )
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
					case MSG_ERR_LOGGER_MESSAGE:
						LogMessage( msg.message );
						break;

					case MSG_ERR_LOGGER_RENAME_LOG_FILES:
						RenameLogFiles();
						break;

					default:
						break;
				}
			}
		}
		else
		{
			slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), _SLOG_WARNING, "ErrLogger: message receive error" );
		}
	}

	return 0;
}

void
LogMessage( char * message )
{
	char		msgBuff[ 256 ];
	char		hdrBuff[ 256 ];
	time_t		t;
	struct tm *	tp;
	struct stat	statBuff;
	FILE *		fp;
	bool		doHeader = false;
	
	// Get the current date & time.
	t = time(0);
	tp = gmtime( &t );

	// Create the full error message with date and time.
	strftime( msgBuff, 30, "%m/%d/%y,%H:%M:%S,", tp );
	strncat( msgBuff, message, 220 );
	strcat( msgBuff, "\r\n" );
	
	// Get the size of the current log file.
	if ( stat( LOG_FILE_CURRENT, &statBuff ) == -1 )
	{ 
		ErrExit( "ErrLogger: unable to stat current log file" );
	}
	
	// If the current log file has reached the limit, delete the old log file
	// and rename the current log file to the old one.
	if ( statBuff.st_size >= LOG_FILE_MAX_SIZE )
	{
		if ( unlink( LOG_FILE_OLD ) != 0 )
		{
			ErrExit( "ErrLogger: unable to delete old log file" );
		}
		
		if ( rename( LOG_FILE_CURRENT, LOG_FILE_OLD ) != 0 )
		{
			ErrExit( "ErrLogger: unable to rename current log file" );
		}
		
		// Need a header on the file.
		doHeader = true;
	}
	
	// If we're just starting to log, we have a new file and need a header on the file.
	else if ( statBuff.st_size == 0 )
	{
		doHeader = true;
	}
	
	// Open the current log file for appending.
	fp = fopen( LOG_FILE_CURRENT, "a" );
	
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: failed to open current log file" );
	}

	// Put in the header lines, if needed.
	if ( doHeader )
	{
		const char HEADER[] = "Date, Time, Process, Message\r\n";

		fwrite( HEADER, 1, strlen( HEADER ), fp );
		strftime( hdrBuff, 30, "%m/%d/%y,%H:%M:%S,", tp );
		strcat( hdrBuff, ERRLOGGER_PROGRAM",Start log version "LOG_FILE_VERSION"\r\n" );
		fwrite( hdrBuff, 1, strlen( hdrBuff ), fp );
	}
	
	// Write the message to the log file.
	fwrite( msgBuff, 1, strlen( msgBuff ), fp );
	
	fclose( fp );
}

void
ErrExit( const char * msg )
{
	slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), _SLOG_WARNING, msg );
	exit( -1 );
}

void
RenameLogFiles()
{
	time_t		t;
	struct tm *	tp;
	static char nameTime[ 30 ];
	static char newName[ 120 ];
	FILE *		fp;

	// Get the current date & time, and put it into a string.
	t = time(0);
	tp = gmtime( &t );
	strftime( nameTime, 30, "%Y%m%d%H%M%S", tp );

	// Create the new name for the current log file.
	sprintf( newName, "%s%s.%s", LOG_FILE_DIRECTORY, nameTime, LOG_FILE_CURRENT_EXT );

	// Rename the current log file.
	if ( rename( LOG_FILE_CURRENT, newName ) != 0 )
	{
		// Failed, so report and stop.
		slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), _SLOG_WARNING, "ErrLogger: failed to rename current log file" );
	}
	
	else
	{
		// Create a new current log file.
		fp = fopen( LOG_FILE_CURRENT, "a" );
		if ( fp == 0 )
		{
			ErrExit( "ErrLogger: unable to open current log file" );
		}
		fclose( fp );
		
		// Create the new name for the old log file.
		sprintf( newName, "%s%s.%s", LOG_FILE_DIRECTORY, nameTime, LOG_FILE_OLD_EXT );

		// Rename the old log file.
		if ( rename( LOG_FILE_OLD, newName ) != 0 )
		{
			// Failed, so report and stop.
			slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), _SLOG_WARNING, "ErrLogger: failed to rename old log file" );
		}
		
		else
		{
			// Create a new old log file.
			fp = fopen( LOG_FILE_OLD, "a" );
			if ( fp == 0 )
			{
				ErrExit( "ErrLogger: unable to open old log file" );
			}
			fclose( fp );
		}
	}
}
