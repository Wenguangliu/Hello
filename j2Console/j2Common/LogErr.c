#include <windows.h>
#include <kfuncs.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <msgqueue.h>


#include "LogErr.h"
#include "ErrLoggerMessages.h"
#include "CommonDefines.h"

#define	PROCESS_NAME_SIZE	256
static HANDLE				queue = NULL;


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
	wchar_t		fileName[PROCESS_NAME_SIZE];
	char		processName[PROCESS_NAME_SIZE];
	char *		processName2;
	char *		processName3;
	
	processName3 = (char*)malloc(PROCESS_NAME_SIZE);
	GetModuleFileName(NULL, fileName, PROCESS_NAME_SIZE);
	wcstombs(processName, fileName, PROCESS_NAME_SIZE);
	processName2 = strrchr(processName, '\\') + 1;
	strncpy(processName3, processName2, PROCESS_NAME_SIZE);
	
	return processName3;
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
	static char					buff[1024];
	static char *				msgStart = buff;
	static int					firstTime = 0;
	static MSGQUEUEOPTIONS		options;
	static MSGQUEUEINFO			info;
	ErrLoggerMsg_t				msg;
	BOOL						result;
    size_t                      strLen;
	
	if ( firstTime == 0 )
	{
		sprintf( buff, "%s, ", GetProcessName() );
		msgStart = buff + strlen( buff );
		firstTime = 1;

		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(ErrLoggerMsg_t)*/;

		queue = CreateMsgQueue(ERR_LOGGER_QUEUE_NAME, &options);
		if ( queue == NULL )
		{
			strcat( msgStart, "failed to open ErrLogger queue" );
			printf("Severity %d: %s\r\n", severity, buff);
            return;
		}
	}
	
	vsprintf( msgStart, fmt, args );
	printf("Severity %d: %s\r\n", severity, buff);

    // take out '\n' and '\r' at end of string
    strLen = strlen (buff);
    strLen -= 1;
    while ((buff [strLen] == '\n') || (buff [strLen] == '\r')) {
        strLen--;
    }
    buff [strLen +1] = '\0';
	
	if ( (queue != NULL) )
	{
		msg.msgId = MSG_ERR_LOGGER_MESSAGE;
		strncpy( msg.message, buff, sizeof( msg.message ) - 1 );
		msg.message[ sizeof( msg.message ) - 1 ] = '\0';
		result = WriteMsgQueue(queue, &msg, sizeof(ErrLoggerMsg_t), INFINITE, NULL);
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
	Log( 1, fmt, args);
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
	Log( 3, fmt, args);
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
	Log( 5, fmt, args);
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
	Log( 6, fmt, args);
	va_end( args );
}

// tell log process to rename old log file and 
// create new log file
void
LogRenameFiles (void)
{

    static MSGQUEUEOPTIONS      options;
    static MSGQUEUEINFO         info;
    ErrLoggerMsg_t              msg;
    BOOL                        result;


    if (queue == NULL) {
        options.dwSize = sizeof(MSGQUEUEOPTIONS);
        options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess = FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage = MAX_QUEUE_LENGTH;

        queue = CreateMsgQueue(ERR_LOGGER_QUEUE_NAME, &options);
        if (queue == NULL) {
            printf("Fail to open queue%s\n", ERR_LOGGER_QUEUE_NAME);
            return;

        }
    } else {
        msg.msgId = MSG_ERR_LOGGER_RENAME_LOG_FILES;
        result = WriteMsgQueue(queue, &msg, sizeof(ErrLoggerMsg_t), INFINITE, NULL);
    }
}
