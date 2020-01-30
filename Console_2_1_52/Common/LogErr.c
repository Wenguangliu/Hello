#include <stdio.h>
#include <fcntl.h>
#include <devctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/procfs.h>
#include <errno.h>
#include <libgen.h>
#include <string.h>
#include <process.h>
#include <stdarg.h>
#include <sys/slog.h>
#include <sys/slogcodes.h>
#include <mqueue.h>

#include "LogErr.h"
#include "ErrLoggerMessages.h"

static mqd_t	queue = -1;

/*
 * FUNCTION
 *  GetProcessName 
 *
 * DESCRIPTION
 *  Returns the name of the current process.
 *
 * RETURNS
 *	A pointer to static storage with the process' name.
 */

static char *
GetProcessName()
{
	int		fd;
	char *	processName;
	char	processPath[ PATH_MAX ];
	struct
	{
		procfs_debuginfo	info;
		char				buff[ PATH_MAX ];
	} processInfo;
	
	sprintf( processPath, "/proc/%d/as", getpid() );
	fd = open( processPath, O_RDONLY );
	if ( fd == -1 )
	{
		return "";
	}
	
	if ( devctl( fd, DCMD_PROC_MAPDEBUG_BASE, &processInfo, sizeof( processInfo ), 0 ) != EOK )
	{
		processName = "";
	}
	else
	{
		processName = basename( processInfo.info.path );
	}
	
	close( fd );

	return processName;
}

/*
 * FUNCTION
 *  Log 
 *
 * DESCRIPTION
 *  Write an error/warning/information message to the system log.
 *
 * PARAMETERS
 *	severity	- Severity of the error
 *	fmt			- Format string (printf style) for the error message
 * 	args		- Additional parameters associated with fmt
 */

static void
Log( int severity, const char * fmt, va_list args )
{
	static char		buff[1024];
	static char *	msgStart = buff;
	static int		firstTime = 0;
	ErrLoggerMsg_t	msg;
	
	// If it's the first time this has been called, set the program name at the start of the buffer and point
	// the start-of-message pointer just past the program name and comma/separator.
	if ( firstTime == 0 )
	{
		sprintf( buff, "%s,", GetProcessName() );
		msgStart = buff + strlen( buff );
		firstTime = 1;
	}

	// If the ErrLogger queue hasn't been opened yet, either because this is the first time this function was called or
	// the previous attempt to open it failed, try to open the queue.
	if ( queue == -1 )
	{
		queue = mq_open( ERR_LOGGER_QUEUE_NAME, O_WRONLY );
		if ( queue == -1 )
		{
			strcat( msgStart, "failed to open ErrLogger queue" );
			slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), _SLOG_WARNING, buff );
		}
	}
	
	vsprintf( msgStart, fmt, args );
	slogf( _SLOG_SETCODE( _SLOGC_PRIVATE_START, 0 ), severity, buff );
	
	if ( (queue != -1) )
	{
		msg.msgId = MSG_ERR_LOGGER_MESSAGE;
		strncpy( msg.message, buff, sizeof( msg.message ) - 1 );
		msg.message[ sizeof( msg.message ) - 1 ] = '\0';
		mq_send( queue, (const char *)&msg, sizeof( ErrLoggerMsg_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 *  LogError 
 *
 * DESCRIPTION
 *  Write an non-terminal error message to the system log.
 *
 * PARAMETERS
 *	fmt			- Format string (printf style) for the error message
 * 	args		- Additional parameters associated with fmt
 */

void LogError( const char * fmt, ... )
{
	va_list	args;
	
	va_start( args, fmt );
	Log( _SLOG_CRITICAL, fmt, args );
	va_end( args );
}

/*
 * FUNCTION
 *  LogWarning 
 *
 * DESCRIPTION
 *  Write a warning to the system log.
 *
 * PARAMETERS
 *	fmt			- Format string (printf style) for the error message
 * 	args		- Additional parameters associated with fmt
 */

void LogWarning( const char * fmt, ... )
{
	va_list	args;
	
	va_start( args, fmt );
	Log( _SLOG_WARNING, fmt, args );
	va_end( args );
}

/*
 * FUNCTION
 *  LogInfo 
 *
 * DESCRIPTION
 *  Write an info message to the system log.
 *
 * PARAMETERS
 *	fmt			- Format string (printf style) for the error message
 * 	args		- Additional parameters associated with fmt
 */

void LogInfo( const char * fmt, ... )
{
	va_list	args;
	
	va_start( args, fmt );
	Log( _SLOG_INFO, fmt, args );
	va_end( args );
}

/*
 * FUNCTION
 *  LogShutdown 
 *
 * DESCRIPTION
 *  Write a debug error message to the system log.
 *
 * PARAMETERS
 *	fmt			- Format string (printf style) for the error message
 * 	args		- Additional parameters associated with fmt
 */

void LogDebug( const char * fmt, ... )
{
	va_list	args;
	
	va_start( args, fmt );
	Log( _SLOG_DEBUG1, fmt, args );
	va_end( args );
}

void LogRenameFiles( void )
{
	ErrLoggerMsg_t	msg;

	// If the ErrLogger queue hasn't been opened yet, either because this is the first time this function was called or
	// the previous attempt to open it failed, try to open the queue.
	if ( queue == -1 )
	{
		queue = mq_open( ERR_LOGGER_QUEUE_NAME, O_WRONLY );
	}

	// Send the message to rename the log files.
	if ( (queue != -1) )
	{
		msg.msgId = MSG_ERR_LOGGER_RENAME_LOG_FILES;
		mq_send( queue, (const char *)&msg, sizeof( msg.msgId ), MQ_PRIO_MAX-1 );
	}
	
}