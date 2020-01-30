#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>			// debugging

#include "LogErr.h"

void *
MapSharedMem( char * shmemName, size_t size )
{
	int		fdShm;
	void *	shmem;

	fdShm = shm_open( shmemName, O_RDWR, 0777 );

	if ( fdShm == -1 )
	{
		LogError( "failed to open shared memory file %s", shmemName );
		return 0;
	}
	
	shmem = mmap( 0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fdShm, 0 );
	if ( shmem == MAP_FAILED )
	{
		LogError( "failed to map shared memory file %s", shmemName );
		return 0;
	}
	
	close( fdShm );

	return shmem;
}
