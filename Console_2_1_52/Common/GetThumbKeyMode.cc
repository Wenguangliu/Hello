#include <errno.h>
#include <fcntl.h>
#include <gulliver.h>
#include <hw/usb_devices.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/usbdi.h>
#include <unistd.h>

#include "GetThumbKeyMode.h"
#include "MountThumbDrive.h"
#include "ParseKey.h"
#include "LogErr.h"

#define  MAX_USB_BUS		9 
#define  MAX_DEVICE_NUMBER	64

void
GetThumbKeyMode( SystemData_t * systemData )
{
	KeyData_t			keyData;
	UsbSerNumList_t	*	serNumList;
	int					i;
	CfgError_t			cfgErr;

	if ( MountThumbDrive() )
	{
		cfgErr = ParseKey( &keyData );
		if ( cfgErr != CFG_ERR_NONE )
		{
			if ( cfgErr = CFG_ERR_CRC )
			{
				LogError( "key.xml CRC error" );
			}
			else if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
			{
				LogError( "key.xml error #%d", cfgErr );
			}
			else
			{
				LogError( "key.xml parse error" );
			}
			return;
		}
	}
	else
	{
		LogError( "mount of USB drive failed" );
		return;
	}
	
	// We don't get here unless we've found a key file.  Go on if we've got a
	// valid mode and serial number.
	if ( (keyData.mode != KEY_MODE_NONE) && (keyData.serialNumber[0] != '\0') )
	{
		// Get the serial numbers for all the USB mass storage devices.
		serNumList = GetUsbSerNums();
		
		// Find out if we've got a match with the serial number in the key file.
		for ( i = 0; i < serNumList->count; i++ )
		{
			// If we've got a serial number match, set the mode in systemData and we're done.
			if ( stricmp( keyData.serialNumber, serNumList->serNum[ i ] ) == 0 )
			{
				switch ( keyData.mode )
				{
					case KEY_MODE_DEMO:
						systemData->inDemoMode = true;
						LogInfo( "USB key mode: demo" );
						break;
					case KEY_MODE_RESEARCH:
						systemData->inResearchMode = true;
						LogInfo( "USB key mode: research" );
						break;
					case KEY_MODE_MANUFACTURING:
						systemData->inManufacturingMode = true;
						LogInfo( "USB key mode: manufacturing" );
						break;
					case KEY_MODE_ADMINISTRATOR:
						systemData->inAdministratorMode = true;
						LogInfo( "USB key mode: administrator" );
						break;
					case KEY_MODE_SERVICE:
						systemData->inServiceMode = true;
						LogInfo( "USB key mode: service" );
						break;
					default:
						break;
				}
				
				systemData->demoFromKey = keyData.demoFromKey;
				systemData->useFastDemo = keyData.useFastDemo;
				systemData->enableFtpTelnet |= keyData.enableFtpTelnet;
				break;
			}
		}

		if ( i == serNumList->count )
		{
			LogError( "serial number mismatch between USB drive and key.xml" );
		}
	}
}


int		// GLD - This should return a bool, but the bloody compiler was screwing up so I just gave in and made it an int.
LockoutKeyInstalled()
{
	KeyData_t			keyData;
	UsbSerNumList_t	*	serNumList;
	int					i;
	CfgError_t			cfgErr;

	if ( MountThumbDrive() )
	{
		cfgErr = ParseKey( &keyData );
		if ( cfgErr != CFG_ERR_NONE )
		{
			if ( cfgErr = CFG_ERR_CRC )
			{
				LogError( "key.xml CRC error" );
			}
			else if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
			{
				LogError( "key.xml error #%d", cfgErr );
			}
			else
			{
				LogError( "key.xml parse error" );
			}
			return false;
		}
	}
	else
	{
		LogError( "mount of USB drive failed" );
		return false;
	}

	// We don't get here unless we've found a key file.  Go on if we've got a
	// valid mode and serial number.
	if ( keyData.mode == KEY_MODE_LOCKOUT )
	{
		// Get the serial numbers for all the USB mass storage devices.
		serNumList = GetUsbSerNums();
		
		// Find out if we've got a match with the serial number in the key file.
		for ( i = 0; i < serNumList->count; i++ )
		{
			// If we've got a serial number match, we're done.
			if ( stricmp( keyData.serialNumber, serNumList->serNum[ i ] ) == 0 )
			{
				return true;
				break;
			}
		}

		if ( i == serNumList->count )
		{
			LogError( "serial number mismatch between USB drive and key.xml" );
		}
	}
	
	return false;
}

UsbSerNumList_t *
GetUsbSerNums()
{
	struct usbd_connection *	connection;
	struct usbd_device *		device;
	usbd_hcd_info_t				version;
	usbd_device_instance_t		instance;
	struct usbd_desc_node *		dev;
	struct usbd_desc_node *		cfg;
	usbd_descriptors_t *		deviceDesc;
	usbd_descriptors_t *		stringDesc;
	usbd_descriptors_t *		interfaceDesc;
	int							busNo;
	int							devNo;
	int							langId;
	char *						string;
	usbd_connect_parm_t			parm = { 0, USB_VERSION, USBD_VERSION, 0, 0, 0, 0, 0, 0, 0 };
	static UsbSerNumList_t		serNumList;

	memset( &serNumList, 0, sizeof( UsbSerNumList_t ) );

	// Connect to the USB stack.
	if ( usbd_connect( &parm, &connection ) != EOK )
	{
		return &serNumList;
	}
	
	// Search all USB buses.
	for ( busNo = 0; busNo < MAX_USB_BUS;  busNo++ )
	{
		if ( usbd_hcd_ext_info( connection, busNo, &version ) != EOK )
		{
			continue;
		}
		 
		// Search all devices on the bus.
		for ( devNo = 1; devNo < MAX_DEVICE_NUMBER; devNo++ )
		{

			// Attach to the USB device.
			memset( &instance, USBD_CONNECT_WILDCARD, sizeof( usbd_device_instance_t ) );
			instance.path = busNo; 
			instance.devno = devNo;
			if ( usbd_attach( connection, &instance, 0, &device ) == EOK )
			{
				// Get the device descriptor.
				deviceDesc = usbd_parse_descriptors( device, 0, USB_DESC_DEVICE, 0, &dev );
				if ( deviceDesc != 0 )
				{
					// Get the configuration descriptor.
					if ( usbd_parse_descriptors( device, dev, USB_DESC_CONFIGURATION, 0, &cfg ) != 0 )
					{
						// Get the interface descriptor.
						interfaceDesc = usbd_parse_descriptors( device, cfg, USB_DESC_INTERFACE, 0, 0 );
						if ( interfaceDesc != 0 )
						{
							// Ignore all but mass storage devices.
							// desc->interface.bInterfaceClass == 0x08 for Mass Storage
							// desc->interface.bInterfaceSubClass == 0x06 for SCSI
							if ( interfaceDesc->interface.bInterfaceClass == 0x08 )
							{
								// Get the langId.
								stringDesc = usbd_parse_descriptors( device, 0, USB_DESC_STRING, 0, 0 );
								langId = stringDesc != 0 ? stringDesc->string.bString[0] : 0;
			
								// Get the serial number string.
								string = usbd_string( device, deviceDesc->device.iSerialNumber, langId );
								
								// Add the serial number to our list.
								if ( string != 0 && (serNumList.count < 4) )
								{
									strncpy( serNumList.serNum[ serNumList.count ], string, 255 );
									serNumList.serNum[ serNumList.count ][ 255 ] = '\0';
									serNumList.count++;
								}
							}
						}
					}
				}
		
				// Done, so detach the device.
				usbd_detach( device );
			}
		}
	}

	// Disconnect from the USB stack.
	usbd_disconnect( connection );

	return &serNumList;

}


