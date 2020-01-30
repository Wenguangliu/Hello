#include <sys/mman.h>
#include <hw/inout.h>
#include <sys/neutrino.h>
#include <stdio.h>			// debugging

#include "Gpio.h"
#include "LogErr.h"

/*
 * FUNCTION
 *  PciRead 
 *
 * DESCRIPTION
 *  Read a value from a PCI register.
 *
 * PARAMETERS
 *	handle		- Memory handle (QNX IO memory mapping)
 *	device		- PCI device number
 *	regIndex	- PCI register number
 *
 * RETURNS
 *	Value read
 */

uint8_t
PciRead( int handle, uint32_t device, uint32_t regIndex )
{
	out32( handle + 0xcf8 - 0xcf8,
		   0x80000000 | (device << 11) | (regIndex & 0xfc) );
	return in8( handle + 0xcfc - 0xcf8 | (regIndex & 0x03) );
}

/*
 * FUNCTION
 *  PciWrite 
 *
 * DESCRIPTION
 *  Write a value to a PCI register.
 *
 * PARAMETERS
 *	handle		- Memory handle (QNX IO memory mapping)
 *	device		- PCI device number
 *	regIndex	- PCI register number
 * 	val			- Value to write
 *
 * RETURNS
 *	None
 */

void
PciWrite( int handle, uint32_t device, uint32_t regIndex, uint8_t val )
{
	out32( handle + 0xcf8 - 0xcf8,
		   0x80000000 | (device << 11) | (regIndex & 0xfc) );
	out8( handle + 0xcfc - 0xcf8 | (regIndex & 0x03), val );
}


/*
 * FUNCTION
 *  GpioInit 
 *
 * DESCRIPTION
 *  Initializes the GPIO, setting the default I/O bit directions and the
 * 	register image value.
 *
 * PARAMETERS
 *	gpioImage - Pointer to location for storing initialized GPIO image.
 *
 * RETURNS
 *	true if successful, or false if an error.
 */

bool
GpioInit( uint8_t * gpioImage )
{
	uintptr_t	handle;
	uint16_t	gpioAddr;
	

	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "failed to get memory control privilege" );
		return false;
	}

	handle = mmap_device_io (128, 0xcf8);
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError( "mmap_device_io PCI failed" );
		return false;
	}

	gpioAddr = (PciRead( handle, 0x0c, 0x46 ) | (PciRead( handle, 0x0c, 0x47 ) * 256)) & 0xfff8;
	if ( gpioAddr != GPIO_BASE_ADDR )
	{
		LogError( "get gpio base address error %x", gpioAddr );
		return false;
	}

	// Turn on the GPIO.
	PciWrite( handle, 0x0c, 0x40, PciRead ( handle, 0x0c, 0x40 ) | 0x02 );

	if ( ThreadCtl( _NTO_TCTL_IO, 0 ) == -1 )
	{
		LogError( "failed to get memory control privilege" );
		return false;
	}

	handle = mmap_device_io( 128, GPIO_BASE_ADDR );
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError( "mmap_device_io gpio failed" );
		return false;
	}

	// Set the GPIO port bit directions.
	out8( GPIO_PORT_DIR_REG, GPIO_PORT_CONFIG );

	// Set the GPIO default value.
	*gpioImage = 0xff;
	out8( GPIO_PORT_REG, *gpioImage );

	return true;
}

