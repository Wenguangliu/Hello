#include <sys/mount.h>
#include <sys/stat.h>

#include "CommonDefines.h"

bool
MountThumbDrive()
{
	struct stat			statBuf;
	const char *		device = 0;

	// Start by unmounting the thumb drive. The mount() call will fail if the drive
	// is already mounted.
	umount( THUMB_DRIVE_PATH, _MOUNT_FORCE );

	// Try to locate a thumb drive.
	if ( stat( "/dev/hd1t6", &statBuf ) == 0 )
	{
		device = "/dev/hd1t6";
	}
	else if ( stat( "/dev/hd1t11", &statBuf ) == 0 )
	{
		device = "/dev/hd1t11";
	}
	else if ( stat( "/dev/hd1t12", &statBuf ) == 0 )
	{
		device = "/dev/hd1t12";
	}

	// Thumb drive found, so mount it.
	if ( device != 0 )
	{
		if ( mount( device, THUMB_DRIVE_PATH, 0, "dos", 0, 0 ) == 0 )
		{
			return true;	// Drive found and mounted.
		}
	}
	
	return false;	// Drive not found, or mount failed.
}