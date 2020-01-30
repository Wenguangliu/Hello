#include <windows.h>
#include <string.h>
#include <time.h>
#include <msgqueue.h>

#include "RecallMessages.h"
#include "LogErr.h"
#include "CommonDefines.h"

#include <stdio.h>	// printf

static const char *	fileList[] = { "Results/Results.ndx",
								   "Results/Results.db",
                                   "Results/OtherResults.ndx",
								   "Results/OtherResults.db",
								   "RefRanges/CurrentRefRanges.xml",
								   "cfg/calibration.xml",
								   "cfg/counters.xml",
								   "cfg/expirys.xml",
								   "cfg/factory.xml",
								   "cfg/hardware.xml",
								   "cfg/settings.xml",
								   "cfg/system.xml",
								   "cfg/timeouts.xml",
								   "cfg/units.xml",
								   "log.current",
								   "log.old",
								   0 };

static bool		ArchiveFile( const char * destDirName, const char * fileName );
static int		GetArchiveSize();
static char *	GetArchiveDirName( char * serNum );
static bool		CreateArchiveDirectories( const char * destDirName );
static bool		CopyFiles( const char * destDirName );
static void		SendRecallMessage( RecallMsgId_t msgId, int data );

static unsigned long long	totalSize = 0;
static unsigned long long	archivedSize = 0;

static char		currentPath[256];
static HANDLE   LogSemHandle = NULL;    //log file semaphore

// service archive thread
void *
ServiceArchive( void * arg )
{
	char				destDirName[256];
	char *				serNum = (char *)arg;
	const char *		path;

	ULARGE_INTEGER		availableFreeBytes;
	ULARGE_INTEGER		totalBytes;
	ULARGE_INTEGER		totalFreeBytes;

    // create semaphore
    if (LogSemHandle == NULL) {
        //Create Log file semaphore with initial value 0
        //the semaphore should be released by LogError process
        LogSemHandle = CreateSemaphore (NULL, 0, 1, LogSem);
        if (LogSemHandle == NULL) {
            LogError ("Fail to crate Archive  semaphore");
            return 0;
        }
    }

	sprintf(currentPath, "/Hard Disk/Abaxis");

	// See if we've got a USB drive attached.
	if ( GetDiskFreeSpaceEx(L"/Hard Disk2", &availableFreeBytes, &totalBytes, &totalFreeBytes) != 0 )					// Development system booting to hard disk
	{
		path = "/Hard Disk2";
	}
	else
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}

	// Get the size of the files to back up, and clear the progress amount.
	totalSize = GetArchiveSize();
	archivedSize = 0;

	// Error if not enough space available.
	if ( totalSize > availableFreeBytes.QuadPart)
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}

	sprintf( destDirName, "%s/%s", path, GetArchiveDirName( serNum ) );

	// Create the archive directories.
	if ( CreateArchiveDirectories( destDirName ) == false )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_DISCONNECT, 0 );
		return 0;
	}

	// Copy the files.
	if ( CopyFiles( destDirName ) == false )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_DISCONNECT, 0 );
		return 0;
	}

	// Signal that we're done.
	SendRecallMessage( MSG_RECALL_ARCHIVE_COMPLETE, 0 );

	return 0;
}

bool
ArchiveFile( const char * destDir, const char * file )
{
	FILE *	source;
	FILE *	dest;
	int		count, i;
	char	buff[ 32768 ];
	char	destFileName[ 200 ];
	char	filePath[ 200 ];
    char    encrypt = 0;
    char    semReleaseFlag = 0;

    // access to log current, need to get semaphore to prevent 
    // both LogError process and recall process access log.current file
    // at same time
    if (strcmp (file, "log.current") == 0) {
        // waiting for semaphore
        WaitForSingleObject (LogSemHandle, INFINITE);
        semReleaseFlag = 1;
    }
	sprintf( filePath, "%s/%s", currentPath, file );
	source = fopen( filePath, "rb" );

	if ( source == NULL )
	{
        // release semaphore
        if (semReleaseFlag == 1) {
            if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
                LogError ("sema release error %d\n\r",GetLastError ());
            }
        }
		LogDebug("ArchiveFile | opening file %s failed", filePath);
		return false;
	}

    // expirys file should be encrypted adn file name changed to be bin
    if (strcmp (file, "cfg/expirys.xml") == 0) {
        encrypt = 1;
	    sprintf( destFileName, "%s/%s", destDir, "cfg/expirys.bin");
    } else {
	    sprintf( destFileName, "%s/%s", destDir, file );
    }
	dest = fopen( destFileName, "wb" );

	if ( dest == NULL )
	{
		fclose( source );
        // release semaphore
        if (semReleaseFlag == 1) {
            if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
                LogError ("sema release error %d\n\r",GetLastError ());
            }
        }
		LogDebug("ArchiveFile | opening file %s failed", destFileName);
		return false;
	}

	while ( (count = fread( buff, sizeof(char), 32768, source )) > 0 )
	{
        // encrypt file
        if (encrypt == 1) {
            for (i = 0; i < count; i++) {
                buff [i] ^= 0xFF;
            }
        }  
		if ( fwrite( buff, sizeof(char), count, dest ) != count )
		{
			fclose( source );
			fclose( dest );
            // release semaphore
            if (semReleaseFlag == 1) {
                if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
                    LogError ("sema release error %d\n\r",GetLastError ());
                }
            }
            LogDebug("ArchiveFile | writing file %s failed", destFileName);
			return false;
		}
		else
		{
			archivedSize += count;
			SendRecallMessage( MSG_RECALL_ARCHIVE_PROGRESS, archivedSize * 100 / totalSize );
		}
        
	}

	fclose( source );
	fclose( dest );

    // release semaphore
    if (semReleaseFlag == 1) {
        if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
            LogError ("sema release error %d\n\r",GetLastError ());
        }
    }

	return true;
}

char *
GetArchiveDirName( char * serNum )
{
	static char dirName[50];
	SYSTEMTIME		tm;

	GetLocalTime(&tm);

	sprintf( dirName, "ServiceArchive_%s_%04d%02d%02d%02d%02d%02d",	serNum,
																	tm.wYear/*tm->tm_year + 1900*/,
																	tm.wMonth/*tm->tm_mon + 1*/,
																	tm.wDay/*tm->tm_mday*/,
																	tm.wHour/*tm->tm_hour*/,
																	tm.wMinute/*tm->tm_min*/,
																	tm.wSecond/*tm->tm_sec*/ );

	return dirName;
}

int
GetArchiveSize()
{
	int				size = 0;
	int				i;
	wchar_t				filename[255];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	HANDLE				hFile;
	BOOL				bFinished = FALSE;
    BOOL                semReleaseFlag = FALSE;



    // find out Rad file index file size
	wsprintf(filename, L"%S/Rad/*.ndx", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	if (hSearch != INVALID_HANDLE_VALUE)
	{
		while ( !bFinished )
		{
			size += fileData.nFileSizeLow;
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::GetArchiveSize() | no more ndx files");
				}
				else
				{
					LogDebug("ServiceArchive::GetArchiveSize() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize()| INVALID_HANDLE_VALUE searching for ndx file");
	}

    // find out Rad file size
	bFinished = FALSE;

	wsprintf(filename, L"%S/Rad/*.rad", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			size += fileData.nFileSizeLow;
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::GetArchiveSize() | no more rad files");
				}
				else
				{
					LogDebug("ServiceArchive::GetArchiveSize() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize() | INVALID_HANDLE_VALUE searching for rad files");
	}

    // find out log archive size
	bFinished = FALSE;

	wsprintf (filename, L"%S/*.log.*", currentPath);
	hSearch = FindFirstFile (filename, &fileData);
	if (hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			size += fileData.nFileSizeLow;
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::GetArchiveSize() | no more backup log files");
				}
				else
				{
					LogDebug("ServiceArchive::GetArchiveSize() | unable to find backup log files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize() | INVALID_HANDLE_VALUE searching for Log backup files");
	}

    // find out update.log size
    bFinished = FALSE;
	wsprintf (filename, L"%S/*.log", currentPath);
	hSearch = FindFirstFile (filename, &fileData);
	if (hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			size += fileData.nFileSizeLow;
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::GetArchiveSize() | no more backup log files");
				}
				else
				{
					LogDebug("ServiceArchive::GetArchiveSize() | unable to find backup log files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize() | INVALID_HANDLE_VALUE searching for Log backup files");
	}


	for ( i = 0; fileList[ i ] != 0; i++ )
	{
		wsprintf(filename, L"%S/%S", currentPath, fileList[i]);
        if (strcmp (fileList[i], "log.current") == 0) {
             // waiting for semaphore
            WaitForSingleObject (LogSemHandle, INFINITE);
            semReleaseFlag = TRUE;
        }
		hFile = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			size += GetFileSize(hFile,NULL);
			CloseHandle(hFile);
		}
		else
		{
			LogDebug("ServiceArchive::GetArchiveSize() | %S not found", filename);
		}
	}
    if (semReleaseFlag == TRUE) {
        semReleaseFlag = FALSE;
        if (ReleaseSemaphore (LogSemHandle, 1, NULL) == 0) {
            LogError ("sema release error %d\n\r",GetLastError ());
        }
    }

	return size;
}

bool
CreateArchiveDirectories( const char * destDirName )
{
	wchar_t			wDestDirName[256];

	wsprintf(wDestDirName, L"%S", destDirName);
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destDirName, 0777 )*/ == 0 )
	{
		return false;
	}

	wsprintf(wDestDirName, L"%S/%S", destDirName, "Rad" );
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destSubDirName, 0777 )*/ == 0 )
	{
		return false;
	}

	wsprintf(wDestDirName, L"%S/%S", destDirName, "Results" );
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destSubDirName, 0777 )*/ == 0 )
	{
		return false;
	}

	wsprintf(wDestDirName, L"%S/%S", destDirName, "RefRanges" );
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destSubDirName, 0777 )*/ == 0 )
	{
		return false;
	}

	wsprintf(wDestDirName, L"%S/%S", destDirName, "cfg" );
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destSubDirName, 0777 )*/ == 0 )
	{
		return false;
	}

	return true;
}

bool
CopyFiles( const char * destDirName )
{
	char 			    buff[100];
	int				    i;
	wchar_t				filename[255];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	BOOL				bFinished = FALSE;
    DWORD               error;


    // copy rad index file
	wsprintf(filename, L"%S/Rad/*.ndx", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	if (hSearch != INVALID_HANDLE_VALUE)
	{
		while (!bFinished)
		{
			sprintf( buff, "Rad/%S", fileData.cFileName );
			//LogDebug("ServiceArchive::CopyFiles() | found a ndx file, archiving %s", buff);
			if ( ArchiveFile( destDirName, buff ) == false )
			{
				FindClose(hSearch);
				return false;
			}
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::CopyFiles() | no more ndx files");
				}
				else
				{
					LogDebug("ServiceArchive::CopyFiles() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::CopyFiles()| INVALID_HANDLE_VALUE searching for ndx file");
	}


    // copy rad files
	bFinished = FALSE; 

	wsprintf(filename, L"%S/Rad/*.rad", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	error = GetLastError();
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			sprintf( buff, "Rad/%S", fileData.cFileName );
			//LogDebug("ServiceArchive::CopyFiles() | found a rad file, archiving %s", buff);
			if ( ArchiveFile( destDirName, buff ) == false )
			{
				FindClose(hSearch);
				return false;
			}
						
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::CopyFiles() | no more rad files");
				}
				else
				{
					LogDebug("ServiceArchive::CopyFiles() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize()| INVALID_HANDLE_VALUE searching for rad file, error %u", error);
	}

    // copy update.log file
	bFinished = FALSE; 

    wsprintf (filename, L"%S/*.log", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	error = GetLastError();
	if  (hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			sprintf( buff, "%S", fileData.cFileName );
			if ( ArchiveFile( destDirName, buff ) == false )
			{
				FindClose(hSearch);
				return false;
			}
						
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::CopyFiles() | no more Log backup files");
				}
				else
				{
					LogDebug("ServiceArchive::CopyFiles() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize()| INVALID_HANDLE_VALUE searching for Log back file, error %u", error);
	}

    // copy log archive files
	bFinished = FALSE; 

    wsprintf (filename, L"%S/*.log.*", currentPath);
	hSearch = FindFirstFile(filename, &fileData);
	error = GetLastError();
	if  (hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
			sprintf( buff, "%S", fileData.cFileName );
			if ( ArchiveFile( destDirName, buff ) == false )
			{
				FindClose(hSearch);
				return false;
			}
						
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ServiceArchive::CopyFiles() | no more Log backup files");
				}
				else
				{
					LogDebug("ServiceArchive::CopyFiles() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ServiceArchive::GetArchiveSize()| INVALID_HANDLE_VALUE searching for Log back file, error %u", error);
	}


    // copy list table files
	for ( i = 0; fileList[ i ] != 0; i++ )
	{
		if ( ArchiveFile( destDirName, fileList[ i ] ) == false )
		{
			;
		}
	}

	return true;
}

void
SendRecallMessage( RecallMsgId_t msgId, int data )
{
	static	HANDLE	queue = 0;
	RecallMsg_t		msg;

	if ( queue == NULL )
	{
		MSGQUEUEOPTIONS			options;
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;

		//queue = mq_open( RECALL_QUEUE_NAME, O_WRONLY );
		queue = CreateMsgQueue(RECALL_QUEUE_NAME, &options);

		if ( queue == NULL )
		{
			//LogError( "failure to open "RECALL_QUEUE_NAME" queue." );
			LogError( "failure to open %s queue.", RECALL_QUEUE_NAME );
			//queue = 0;
		}
	}

	if ( queue != NULL )
	{
		msg.msgId = msgId;
		msg.idata = data;
		//mq_send( queue, (const char *)&msg, sizeof( RecallMsg_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(queue, (void *)&msg, sizeof( RecallMsg_t ), INFINITE, NULL);
	}
}
