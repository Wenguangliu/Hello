#include <stdint.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <hw/inout.h>
#include <stdio.h>

#include "Hardware.h"
#include "LogErr.h"

void
SetBacklight( int setting )
{
    static int			threadCtl = -1;
    static uintptr_t	handle;
    uint8_t				backlightValue;
    

	if ( threadCtl == -1 )
	{
		threadCtl = ThreadCtl( _NTO_TCTL_IO, 0 );
		if ( threadCtl == -1 )
		{
			LogError( "failed to get memory control privilege" );
			return;
		}
	
		handle = mmap_device_io( 1, 0x0221 );
		if ( handle == MAP_DEVICE_FAILED )
		{
			LogError( "failed to map I/O" );
			threadCtl = -1;
			return;
		}
	}

	// "Dim is bright, and bright is dim" - Audie Hickey, 10/20/05
	// In other words, 0 is full and 255 is off.	
	backlightValue = (uint8_t)(255 - setting * 2.55);

	out8( handle, backlightValue );
}

