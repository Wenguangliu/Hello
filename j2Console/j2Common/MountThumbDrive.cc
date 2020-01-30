#include <windows.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "MountThumbDrive.h"
#include "LogErr.h"


// make sure USB thumb driver is plugged in
bool
MountThumbDrive (void)
{
    ULARGE_INTEGER      availableFreeBytes;
    ULARGE_INTEGER      totalBytes;
    ULARGE_INTEGER      totalFreeBytes;

    // See if we've got a USB drive attached.
    if (GetDiskFreeSpaceEx(L"/Hard Disk2", &availableFreeBytes, &totalBytes, &totalFreeBytes) != 0 ) {
        return true;
    }  
    return false;
}
