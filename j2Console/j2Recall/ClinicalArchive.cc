#include <windows.h>
//#include <sys/stat.h>
//#include <sys/mount.h>
//#include <sys/statvfs.h>
//#include <dirent.h>
#include <string.h>
//#include <unistd.h>
//#include <fcntl.h>
#include <time.h>
#include <msgqueue.h>
//#include <sys/neutrino.h>

#include "RecallMessages.h"
#include "LogErr.h"
#include "CommonDefines.h"
#include "RecallStates.h"
#include "ClinicalArchive.h"

#include <stdio.h>

static bool		ArchiveFile( const char * destDirName, const char * fileName );
static char *	GetArchiveDirName( char * serNum );
static bool		CopyFiles( const char * destDirName, DateRange_t * dateRange );
static void		SendRecallMessage( RecallMsgId_t msgId, int data );
static int		GetArchiveSize( DateRange_t * dateRange );

static unsigned long long	totalSize = 0;
static unsigned long long	archivedSize = 0;

static char		currentPath[256];

void *
ClinicalArchive( void * arg )
{
	//struct statvfs		vfsBuf;
	//struct stat			statBuf;
	char				destDirName[256];
	wchar_t				wDestDirName[256];
	//unsigned long long	freeSpace;
	DateRange_t	*		dateRange = (DateRange_t *)arg;
	const char *		path;

	ULARGE_INTEGER		availableFreeBytes;
	ULARGE_INTEGER		totalBytes;
	ULARGE_INTEGER		totalFreeBytes;

	// Make RAD_FILE_DIRECTORY our current working directory so we don't need to worry about the path.
	//chdir( RAD_FILE_DIRECTORY );
	sprintf(currentPath, "%s", RAD_FILE_DIRECTORY);

	// See if we've got a USB drive attached.
	if ( GetDiskFreeSpaceEx(L"/Hard Disk2", &availableFreeBytes, &totalBytes, &totalFreeBytes)/*stat( "/fs/usb0", &statBuf )*/ != 0 )					// Development system booting to hard disk
	{
		//path = "/fs/usb0";
		path = "/Hard Disk2";
	}
	//else if ( stat( "/fs/hdumass10-dos-1", &statBuf ) == 0 )	// Development system booting to flash drive
	//{
	//	path = "/fs/hdumass10-dos-1";
	//}
	else
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}

	// Get the free space on the thumb drive.
	/*if ( statvfs( path, &vfsBuf ) == -1 )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}
	freeSpace = vfsBuf.f_bfree * vfsBuf.f_bsize;*/

	// Get the size of the files to back up, and clear the progress amount.
	totalSize = GetArchiveSize( dateRange );
	archivedSize = 0;

	// Error if not enough space.
	if ( totalSize > availableFreeBytes.QuadPart/*freeSpace*/ )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}

	sprintf( destDirName, "%s/%s", path, GetArchiveDirName( dateRange->serNum ) );
	wsprintf(wDestDirName, L"%S", destDirName);
	if ( CreateDirectory(wDestDirName, NULL)/*mkdir( destDirName, 0777 )*/ == 0 )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_DISCONNECT, 0 );
		return 0;
	}

	// Copy the files.
	if ( CopyFiles( destDirName, dateRange ) == false )
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
	int		count;
	char	buff[ 32768 ];
	char	destFileName[ 200 ];
	char	filePath[ 200 ];


	sprintf( filePath, "%s/%s", currentPath, file );
	//source = open( file, O_RDONLY );
	source = fopen( filePath, "rb" );

	if ( source == NULL )
	{
		LogDebug("ArchiveFile | opening file %s failed", filePath);
		return false;
	}

	sprintf( destFileName, "%s/%s", destDir, file );
	//dest = open( destFileName, O_WRONLY | O_CREAT, 0777 );
	dest = fopen( destFileName, "wb" );

	if ( dest == NULL )
	{
		LogDebug("ArchiveFile | opening file %s failed", destFileName);
		fclose( source );
		return false;
	}

	//while ( (count = read( source, buff, 32768 )) > 0 )
	while ( (count = fread( buff, sizeof(char), 32768, source )) > 0 )
	{
		//if ( write( dest, buff, count ) == -1 )
		if ( fwrite( buff, sizeof(char), count, dest ) != count )
		{
			fclose( source );
			fclose( dest );
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

	return true;
}

char *
GetArchiveDirName( char * serNum )
{
	static char dirName[50];
	//time_t		t;
	//struct tm *	tm;
	SYSTEMTIME		tm;

	//t = time( 0 );
	//tm = gmtime( &t );
	GetLocalTime(&tm);

	sprintf( dirName, "ClinicalArchive_%s_%04d%02d%02d%02d%02d%02d",	serNum,
																	tm.wYear/*tm->tm_year + 1900*/,
																	tm.wMonth/*tm->tm_mon + 1*/,
																	tm.wDay/*tm->tm_mday*/,
																	tm.wHour/*tm->tm_hour*/,
																	tm.wMinute/*tm->tm_min*/,
																	tm.wSecond/*tm->tm_sec*/ );

	return dirName;
}

int
GetArchiveSize( DateRange_t * dateRange )
{
	int				    size = 0;
	wchar_t				filename[255];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	BOOL				bFinished = FALSE;
    FILETIME            localTime;

	wsprintf(filename, L"%S/*.rad", currentPath/*RAD_FILE_DIRECTORY*/);
	hSearch = FindFirstFile(filename, &fileData);
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
            //cunvert to local time
            FileTimeToLocalFileTime (&fileData.ftLastWriteTime, &localTime);
			if ( (CompareFileTime (&localTime, &dateRange->start) >= 0) 
					&& (CompareFileTime(&localTime, &dateRange->end) <= 0) )
			{
				size += fileData.nFileSizeLow;
			}
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ClinicalArchive::GetArchiveSize() | no more rad files");
				}
				else
				{
					LogDebug("ClinicalArchive::GetArchiveSize() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ClinicalArchive::GetArchiveSize() | INVALID_HANDLE_VALUE");
	}

	return size;
}

bool
CopyFiles( const char * destDirName, DateRange_t * dateRange )
{
	wchar_t				filename[255];
    char                buff[100];
	WIN32_FIND_DATA		fileData;
	HANDLE				hSearch;
	BOOL				bFinished = FALSE;
    FILETIME            localTime;

	wsprintf(filename, L"%S/*.rad", currentPath/*RAD_FILE_DIRECTORY*/);
	hSearch = FindFirstFile(filename, &fileData);
	if ( hSearch != INVALID_HANDLE_VALUE ) 
	{
		while ( !bFinished )
		{
            //cunvert to local time
            FileTimeToLocalFileTime (&fileData.ftLastWriteTime, &localTime);
			if ( (CompareFileTime(&localTime, &dateRange->start) >= 0) 
					&& (CompareFileTime(&localTime, &dateRange->end) <= 0) )
			{
				sprintf( buff, "%S", fileData.cFileName );
				if ( ArchiveFile( destDirName, buff ) == false )
				{
					FindClose(hSearch);
					return false;
				}
			}
			
			if (!FindNextFile(hSearch, &fileData)) 
			{
				if (GetLastError () == ERROR_NO_MORE_FILES)
				{
					 LogDebug("ClinicalArchives::CopyFiles() | no more rad files");
				}
				else
				{
					LogDebug("ClinicalArchives::CopyFiles() | unable to find files");
				}
				FindClose(hSearch);
				bFinished = TRUE;
			}
		}
	}
	else
	{
		LogDebug("ClinicalArchives::CopyFiles() | INVALID_HANDLE_VALUE");
		return false;
	}

	LogDebug("ClinicalArchives::CopyFiles() | no rad file found");
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
