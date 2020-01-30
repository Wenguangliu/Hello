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
#include "MountThumbDrive.h"

#include <stdio.h>	// printf

static const char *	fileList[] = { "Results/Results.ndx",
								   "Results/Results.db",
								   "RefRanges/CurrentRefRanges.xml",
								   "cfg/calibration.xml",
								   "cfg/counters.xml",
//								   "cfg/expirys.xml",	--- 20120901 - now encrypted
								   "cfg/factory.xml",
								   "cfg/hardware.xml",
								   "cfg/settings.xml",
								   "cfg/system.xml",
								   "cfg/timeouts.xml",
								   "cfg/units.xml",
//								   "log.current",
//								   "log.old",
								   0 };

static bool		ArchiveFile( const char * destDirName, const char * destFile, const char * sourceName, bool encrypt );
static int		GetArchiveSize();
static char *	GetArchiveDirName( char * serNum );
static bool		CreateArchiveDirectories( const char * destDirName );
static bool		CopyFiles( const char * destDirName );
static void		SendRecallMessage( RecallMsgId_t msgId, int data );

static unsigned long long	totalSize = 0;
static unsigned long long	archivedSize = 0;

void *
ServiceArchive( void * arg )
{
	struct statvfs		vfsBuf;
	char				destDirName[256];
	unsigned long long	freeSpace;
	char *				serNum = (char *)arg;

	// Make /Abaxis our current working directory so we can use relative paths.
	chdir( "/Abaxis" );

	if ( !MountThumbDrive() )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}

	// Get the free space on the thumb drive.
	if ( statvfs( THUMB_DRIVE_PATH, &vfsBuf ) == -1 )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}
	freeSpace = vfsBuf.f_bfree * vfsBuf.f_bsize;
	
	// Get the size of the files to back up, and clear the progress amount.
	totalSize = GetArchiveSize();
	archivedSize = 0;
	
	// Error if not enough space available.
	if ( totalSize > freeSpace )
	{
		SendRecallMessage( MSG_RECALL_ARCHIVE_INSUFF_SPACE, 0 );
		return 0;
	}
	
	sprintf( destDirName, THUMB_DRIVE_PATH"/%s", GetArchiveDirName( serNum ) );

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
ArchiveFile( const char * destDir, const char *destFile, const char * sourceFile, bool encrypt )
{
	int		source, dest;
	int		count;
	int		i;
	char	buff[ 32768 ];
	char	destPath[ 200 ];

	
	source = open( sourceFile, O_RDONLY );
	
	if ( source == -1 )
	{
		return false;
	}
	
	sprintf( destPath, "%s/%s", destDir, destFile );
	dest = open( destPath, O_WRONLY | O_CREAT, 0777 );
		
	if ( dest == -1 )
	{
		close( source );
		return false;
	}
		
	while ( (count = read( source, buff, 32768 )) > 0 )
	{
		if ( encrypt )
		{
			for ( i = 0; i < count; i++ )
			{
				buff[ i ] ^= 0xFF;
			}
		}
			
		if ( write( dest, buff, count ) == -1 )
		{
			return false;
		}

		// Update the progress bar.
		archivedSize += count;
		SendRecallMessage( MSG_RECALL_ARCHIVE_PROGRESS, archivedSize * 100 / totalSize );
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
	
	sprintf( dirName, "ServiceArchive_%s_%04d%02d%02d%02d%02d%02d",	serNum,
																	tm->tm_year + 1900,
																	tm->tm_mon + 1,
																	tm->tm_mday,
																	tm->tm_hour,
																	tm->tm_min,
																	tm->tm_sec );
	
	return dirName;
}

int
GetArchiveSize()
{
	DIR *			dirp;
	struct dirent *	direntp;
	struct stat		statBuf;
	char *			ext;
	char			buff[50];
	int				size = 0;
	int				i;

	// Get the size of the RAD files.
	dirp = opendir( "Rad" );
	if ( dirp != 0 )
	{
		for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
		{
			ext = strrchr( direntp->d_name, '.' );
			if ( (ext != 0) && ((strcmp( ext, ".rad" ) == 0) || (strcmp( ext, ".ndx" ) == 0)) )
			{
				sprintf( buff, "Rad/%s", direntp->d_name );
				if ( stat( buff, &statBuf ) == 0 )
				{
					size += statBuf.st_size;
				}
			}			
		}
		closedir( dirp );
	}
	
	// Get the size of the log files.
	dirp = opendir( "." );
	if ( dirp != 0 )
	{
		for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
		{
			if ( (strstr( direntp->d_name, "log.current" ) != 0) || (strstr( direntp->d_name, "log.old" ) != 0) )
			{
				if ( stat( direntp->d_name, &statBuf ) == 0 )
				{
					size += statBuf.st_size;
				}
			}			
		}
		closedir( dirp );
	}
	
	// Get the size of the files in fileList.
	for ( i = 0; fileList[ i ] != 0; i++ )
	{
		if ( stat( fileList[ i ], &statBuf ) == 0 )
		{
			size += statBuf.st_size;
		}
	}
	
	return size;
}

bool
CreateArchiveDirectories( const char * destDirName )
{
	char			destSubDirName[256];

	if ( mkdir( destDirName, 0777 ) != 0 )
	{
		return false;
	}
		
	sprintf( destSubDirName, "%s/%s", destDirName, "Rad" );
	if ( mkdir( destSubDirName, 0777 ) != 0 )
	{
		return false;
	}
		
	sprintf( destSubDirName, "%s/%s", destDirName, "Results" );
	if ( mkdir( destSubDirName, 0777 ) != 0 )
	{
		return false;
	}

	sprintf( destSubDirName, "%s/%s", destDirName, "RefRanges" );
	if ( mkdir( destSubDirName, 0777 ) != 0 )
	{
		return false;
	}

	sprintf( destSubDirName, "%s/%s", destDirName, "cfg" );
	if ( mkdir( destSubDirName, 0777 ) != 0 )
	{
		return false;
	}
	
	return true;
}

bool
CopyFiles( const char * destDirName )
{
	DIR *			dirp;
	struct dirent *	direntp;
	char 			buff[100];
	char *			ext;
	int				i;

	// Open the RAD file directory.
	dirp = opendir( "Rad" );
	if ( dirp == 0 )
	{
		return false;
	}

	// Archive all the files in the RAD file directory that end in .rad or .ndx.
	for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
	{
		ext = strrchr( direntp->d_name, '.' );
		if ( (ext != 0) && ((strcmp( ext, ".rad" ) == 0) || (strcmp( ext, ".ndx" ) == 0)) )
		{
			sprintf( buff, "Rad/%s", direntp->d_name );
			if ( ArchiveFile( destDirName, buff, buff, false ) == false )
			{
				closedir( dirp );
				return false;
			}
		}			
	}
	closedir( dirp );

	// Open the current directory.
	dirp = opendir( "." );
	if ( dirp == 0 )
	{
		return false;
	}

	// Archive all the log files.
	for ( direntp = readdir( dirp ); direntp != 0; direntp = readdir( dirp ) )
	{
		if ( (strstr( direntp->d_name, "log.current" ) != 0) || (strstr( direntp->d_name, "log.old" ) != 0) )
		{
			if ( ArchiveFile( destDirName, direntp->d_name, direntp->d_name, false ) == false )
			{
				closedir( dirp );
				return false;
			}
		}
	}
	closedir( dirp );



	for ( i = 0; fileList[ i ] != 0; i++ )
	{
// GLD
// Currently, ArchiveFile returns false if the source file doesn't exist, as well
// as if the thumb drive is disconnected. Therefore, we'll ignore the return
// value until such time as it's changed to give us a false only upon a disconnect.
// Currently, Recall treats both completion and disconnect messages the same
// anyway.
		if ( ArchiveFile( destDirName, fileList[ i ], fileList[ i ], false ) == false )
		{
//			return false;
			;
		}
	}
	
	// Encrypt the expirys file when archiving. It gets a different name since the archive file is in binary format.
	ArchiveFile( destDirName, "cfg/expirys.bin", "cfg/expirys.xml", true );
	
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
