//#include <fcntl.h>
//#include <sys/mman.h>
#include <windows.h>
#include <stdio.h>			// debugging

#include "LogErr.h"

void *
MapSharedMem( char * shmemName, DWORD size )
{
	HANDLE			hMappedFile = NULL;
	void *			pMemMap = NULL;
	wchar_t			localWSTR[255];

	wsprintf(localWSTR, L"%S", shmemName);

	hMappedFile = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
									  NULL,
									  PAGE_READWRITE,
									  0,
									  size,
									  localWSTR);

	if ( hMappedFile == NULL )
	{
		LogError( "failed to open shared memory file %s", shmemName );
		return 0;
	}
	
	pMemMap = MapViewOfFile(hMappedFile,
								FILE_MAP_ALL_ACCESS,
								0,
								0,
								size);

	if ( pMemMap == NULL )
	{
		LogError( "failed to map shared memory file %s", shmemName );
		return 0;
	}

	CloseHandle(hMappedFile);

	return pMemMap;
}
