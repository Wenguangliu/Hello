#include <sys/stat.h>
#include <sys/mount.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <mqueue.h>
#include <sys/neutrino.h>

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

void *
ClinicalArchive( void * arg )
{
	struct statvfs		vfsBuf;
	char				destDirName[256];
	unsigned long long	freeSpace;
	DateRange_t	*		dateRange = (DateRange_t *)arg;

	// Make RAD_FILE_DIRECTORY our current working directory so we don't need to worry about the path.
	chdir( RAD_FILE_DIRECTORY );

	// Unmount the thumb drive in case it's already mounted.
	umount( THUMB_DRIVE_PATH, _MOUNT_FORCE );

	// Mount the thumb drive. Try the FAT partition first. If that fails, try
	// the FAT32 partitions.
	if ( mount( "/dev/hd1t6", THUMB_DRIVE_PATH, 0, "dos", 0, 0 ) == -1 )
	{
		if ( mount( "/dev/hd1t11", THUMB_DRIVE_PATH, 0, "dos", 0, 0 ) == -1 )
		{
			if ( mount( "/dev/hd1t12", THUMB_DRIVE_PATH, 0, "dos", 0, 0 ) == -1 )
			{
				SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
				return 0;
			}
		}
	}

	// Get the free space on the thumb drive.
	if ( statvfs( THUMB_DRIVE_PATH, &vfsBuf ) == -1 )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}
	freeSpace = vfsBuf.f_bfree * vfsBuf.f_bsize;
	
	// Get the size of the files to back up, and clear the progress amount.
	totalSize = GetArchiveSize( dateRange );
	archivedSize = 0;
	
	// Error if not enough space.
	if ( totalSize > freeSpace )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}
	
	sprintf( destDirName, THUMB_DRIVE_PATH"/%s", GetArchiveDirName( dateRange->serNum ) );
	if ( mkdir( destDirName, 0777 ) != 0 )
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
	int		source, dest;
	int		count;
	char	buff[ 32768 ];
	char	destFileName[ 200 ];

	
	source = open( file, O_RDONLY );
	
	if ( source == -1 )
	{
		return false;
	}
	
	sprintf( destFileName, "%s/%s", destDir, file );
	dest = open( destFileName, O_WRONLY | O_CREAT, 0777 );
		
	if ( dest == -1 )
	{
		close( source );
		return false;
	}
		
	while ( (count = read( source, buff, 32768 )) > 0 )
	{
		if ( write( dest, buff, count ) == -1 )
		{
			return false;
		}
		else
		{
			archivedSize += count;
			SendRecallMessage( MSG_RECALL_ARCHIVE_PROGRESS, archivedSize * 100 / totalSize );
		}
	}
		
	close( source );
	close( dest );
	
	return true;
}

char *
GetArchiveDirName( char * serNum )
{
	static char dirName[50];
	time_t		t;
	struct tm *	tm;
	
	t = time( 0 );
	tm = gmtime( &t );
	
	sprintf( dirName, "ClinicalArchive_%s_%04d%02d%02d%02d%02d%02d",	serNum,
																	tm->tm_year + 1900,
																	tm->tm_mon + 1,
																	tm->tm_mday,
																	tm->tm_hour,
																	tm->tm_min,
																	tm->tm_sec );
	
	return dirName;
}

int
GetArchiveSize( DateRange_t * dateRange )
{
	DIR *			dirp;
	struct dirent *	direntp;
	struct stat		statBuf;
	char *			ext;
	int				size = 0;

	dirp = opendir( "." );
	if ( dirp != 0 )
	{
		for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
		{
			ext = strrchr( direntp->d_name, '.' );
			if ( (ext != 0) && (strcmp( ext, ".rad" ) == 0) )
			{
				if ( stat( direntp->d_name, &statBuf ) == 0 )
				{
					if ( (statBuf.st_mtime >= dateRange->start) && (statBuf.st_mtime <= dateRange->end) )
					{
						size += statBuf.st_size;
					}
				}
			}			
		}
		closedir( dirp );
	}

	return size;
}

bool
CopyFiles( const char * destDirName, DateRange_t * dateRange )
{
	DIR *			dirp;
	struct dirent *	direntp;
	char 			buff[100];
	char *			ext;
	struct stat		statBuf;

	dirp = opendir( "." );
	if ( dirp == 0 )
	{
		return false;
	}

	for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
	{
		ext = strrchr( direntp->d_name, '.' );
		if ( (ext != 0) && (strcmp( ext, ".rad" ) == 0) )
		{
			sprintf( buff, "%s", direntp->d_name );
			if ( stat( buff, &statBuf ) == 0 )
			{
				if ( (statBuf.st_mtime >= dateRange->start) && (statBuf.st_mtime <= dateRange->end) )
				{
					if ( ArchiveFile( destDirName, buff ) == false )
					{
						closedir( dirp );
						return false;
					}
				}
			}
		}			
	}
	closedir( dirp );

/*
	for ( i = 0; fileList[ i ] != 0; i++ )
	{
// GLD
// Currently, ArchiveFile returns false if the source file doesn't exist, as well
// as if the thumb drive is disconnected. Therefore, we'll ignore the return
// value until such time as it's changed to give us a false only upon a disconnect.
// Currently, Recall treats both completion and disconnect messages the same
// anyway.
		if ( ArchiveFile( destDirName, fileList[ i ] ) == false )
		{
//			return false;
			;
		}
	}
*/	
	return true;
}

void
SendRecallMessage( RecallMsgId_t msgId, int data )
{
	static	mqd_t	queue = 0;
	RecallMsg_t		msg;

	if ( queue == 0 )
	{
		queue = mq_open( RECALL_QUEUE_NAME, O_WRONLY );
	
		if ( queue == -1 )
		{
			LogError( "failure to open "RECALL_QUEUE_NAME" queue." );
			queue = 0;
		}
	}
	
	if ( queue != 0 )
	{
		msg.msgId = msgId;
		msg.idata = data;
		mq_send( queue, (const char *)&msg, sizeof( RecallMsg_t ), MQ_PRIO_MAX-1 );
	}
}
