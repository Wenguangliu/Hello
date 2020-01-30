#include <windows.h>
#include <msgqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <tlhelp32.h>

#include "ErrLoggerMessages.h"
#include "CommonDefines.h"
#include "Shmem.h"
#include "FactoryData.h"
#include "LogErr.h"

#define LOG_FILE_VERSION	"1.1"
#define LOG_FILE_DIR        "/Hard Disk/Abaxis/"
#define CURRENT_LOG_FILE    "log.current"
#define OLD_LOG_FILE        "log.old"
#define TMP_LOG_FILE        "log.tmp"
#define LOG_FILE_CURRENT	"/Hard Disk/Abaxis/log.current"
#define LOG_FILE_OLD		"/Hard Disk/Abaxis/log.old"
#define LOG_FILE_TEMP       "/Hard Disk/Abaxis/log.tmp"
#define LOG_FILE_MAX_SIZE	10000000

static void LogMessage( char * message );
static void	ErrExit( const char * message );
//static int RenameLogFile (wchar_t *logFileName);
static int RenameLogFile (char *fileName);
FactoryData_t *  factoryData      = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );

int main( int argc, char *argv[] )
{
	ErrLoggerMsg_t			msg;
	HANDLE					queue;
	MSGQUEUEOPTIONS			options;
	DWORD					receivedSize = 0;
	DWORD					flags;
	BOOL					result;
	FILE *					fp;
    HANDLE                  LogSemHandle; 

    //Create Log file semaphore
    LogSemHandle = CreateSemaphore (NULL, 1, 1, LogSem);
    if (LogSemHandle == NULL) {
        printf ("Fail to crate log file semaphore\n\r");
		ErrExit("ErrLogger: failure to create Log file semaphore" );
    }

    
	// Open the input queue and get its attributes.
	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = TRUE;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(ErrLoggerMsg_t)*/;
	queue = CreateMsgQueue(ERR_LOGGER_QUEUE_NAME, &options);
	if ( queue == NULL )
	{
		ErrExit( "ErrLogger: failure to open ErrLogger queue" );
	}
		
	// Make sure both log files exist.
	fp = fopen( LOG_FILE_CURRENT, "ab" );
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: unable to open current log file" );
	}
	fclose( fp );
	fp = fopen( LOG_FILE_OLD, "ab" );
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: unable to open old log file" );
	}
	fclose( fp );


	// Loop waiting for and processing messages.		
	while ( 1 )
	{
		result = ReadMsgQueue(queue, &msg, sizeof(ErrLoggerMsg_t), &receivedSize, INFINITE, &flags);
		if (result)
		{
			if ( receivedSize > 0 )
			{
				switch ( msg.msgId )
				{
					case MSG_ERR_LOGGER_MESSAGE:
                        // acquire semaphore
                        WaitForSingleObject (LogSemHandle, INFINITE);
						LogMessage( msg.message );
                        // release semaphore
                        if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
                            printf ("semaphore release error %d\n\r",GetLastError ());
                            ErrExit( "ErrLogger: Fail to release Log Semaphore" );
                        }
						break;

                    case MSG_ERR_LOGGER_RENAME_LOG_FILES:
                        // acquire semaphore
                        WaitForSingleObject (LogSemHandle, INFINITE);
                        // backup current log file and create new log file
                        // RenameLogFile (TEXT (LOG_FILE_CURRENT));
                        //RenameLogFile (TEXT (LOG_FILE_OLD));
                        //RenameLogFile (CURRENT_LOG_FILE);
                        RenameLogFile (TMP_LOG_FILE);
                        RenameLogFile (OLD_LOG_FILE);
                        if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
                            printf ("semaphore release error %d\n\r",GetLastError ());
                            ErrExit( "ErrLogger: Fail to release Log Semaphore" );
                        }
                        break;
	
					default:
						break;
				}
			}
		}
		else
		{
			printf("ErrLogger: message receive error\r\n");
		}
	}

	return 0;
}

void LogMessage( char * message )
{
	char		msgBuff[ 256 ];
	char		hdrBuff[ 512 ];
	SYSTEMTIME	time;
	FILE *		fp;
	long		fileSize = 0;
	bool		doHeader = false;
    DWORD       attribs;
	
	// Get the current date & time.
	//GetSystemTime(&time);
	GetLocalTime(&time);

	// Create the full error message with date and time.
	sprintf(msgBuff, "%d/%d/%d,%02d:%02d:%02d,", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond);
	strncat( msgBuff, message, 220 );
	strcat( msgBuff, "\r\n" );
	
	
	// Get the size of the current log file.
	fp = fopen(LOG_FILE_CURRENT, "rb");
	if (fp == NULL) {
        //detect to see if log.current file exist or not
        attribs = GetFileAttributes (TEXT(LOG_FILE_CURRENT));
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            // somehow log.current file does not exist
            // create a new one
            fp = fopen(LOG_FILE_CURRENT, "ab");
            if (fp == 0) {

            } else {
                fclose (fp);
                //open as read only 
                fp = fopen(LOG_FILE_CURRENT, "rb");
                if (fp != NULL) {
                    fseek(fp, 0, SEEK_END);
                    fileSize = ftell(fp);
                    fclose(fp);
                } else {
                    ErrExit ("log file open fail");
                }
            }
        } else {
            printf ("log file open fail %d\n\r",  GetLastError ());
            return;
        }
	} else {
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);
		fclose(fp);
	}
	
	// If the current log file has reached the limit, delete the old log file
	// and rename the current log file to the old one.
	if ( fileSize >= LOG_FILE_MAX_SIZE )
	{
		if ( DeleteFile( TEXT(LOG_FILE_OLD) ) == FALSE )
		{
			ErrExit( "ErrLogger: unable to delete old log file" );
		}

		if ( MoveFile( TEXT(LOG_FILE_CURRENT), TEXT(LOG_FILE_OLD) ) == FALSE )
		{
			ErrExit( "ErrLogger: unable to rename current log file" );
		}
		
		// Need a header on the file.
		doHeader = true;
	}
	
	// If we're just starting to log, we have a new file and need a header on the file.
	else if ( fileSize == 0 )
	{
		doHeader = true;
	}
	
	// Open the current log file for appending.
	fp = fopen( LOG_FILE_CURRENT, "ab" );
	
	if ( fp == 0 )
	{
		ErrExit( "ErrLogger: failed to open current log file" );
	}

	// Put in the header lines, if needed.
	if ( doHeader )
	{
		const char HEADER[] = "Date, Time, Process, Message\r\n";

		fwrite( HEADER, 1, strlen( HEADER ), fp );
		sprintf(hdrBuff, "%d/%d/%d,%02d:%02d:%02d,", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond);
        strcat (hdrBuff, "Analyzer Serial Number:");
        strcat (hdrBuff,factoryData->instrumentSerialNumber);
        strcat (hdrBuff,"\r\n");
		fwrite( hdrBuff, 1, strlen( hdrBuff ), fp );
		sprintf(hdrBuff, "%d/%d/%d,%02d:%02d:%02d,", time.wMonth, time.wDay, time.wYear, time.wHour, time.wMinute, time.wSecond);
		strcat( hdrBuff, ERRLOGGER_PROGRAM",Start log version "LOG_FILE_VERSION"\r\n" );
		fwrite( hdrBuff, 1, strlen( hdrBuff ), fp );
	}
	
	// Write the message to the log file.
	fwrite( msgBuff, 1, strlen( msgBuff ), fp );

	fclose( fp );
}

void ErrExit( const char * msg )
{
	printf("%s\r\n", msg);
	exit( -1 );
}


// rename current log file
int
RenameLogFile (char *fileName)
{

    DWORD           attribs;
    SYSTEMTIME      sysTime;
    wchar_t         newFileName [64];
    wchar_t         logFileName [64];
    char            newLogFileName [64];
    FILE            *fp;

    wsprintf (logFileName, L"%S%S", LOG_FILE_DIR, fileName);
    // detect log file
    attribs = GetFileAttributes (logFileName);
    if (attribs != INVALID_FILE_ATTRIBUTES) {
        if (strcmp (fileName, TMP_LOG_FILE) == 0) {
            fileName = CURRENT_LOG_FILE;
        }
        GetLocalTime (&sysTime);
        wsprintf (newFileName, L"%S%04d%02d%02d%02d%02d%02d.%S", 
                                                   LOG_FILE_DIR,
                                                   sysTime.wYear,
                                                   sysTime.wMonth,
                                                   sysTime.wDay,
                                                   sysTime.wHour,
                                                   sysTime.wMinute,
                                                   sysTime.wSecond,
                                                   fileName);
        MoveFile (logFileName, newFileName);
        // create a new one
        sprintf (newLogFileName, "%s%s",LOG_FILE_DIR, fileName);
        fp = fopen (newLogFileName, "ab");
        fclose (fp);
    } else {
        LogDebug ("LogFile %S does not exist\n\r", logFileName);
        return -1;
    }
    return 0;

}
