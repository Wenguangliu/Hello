#include <windows.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GetThumbKeyMode.h"
#include "MountThumbDrive.h"
#include "ParseKey.h"
#include "LogErr.h"

#define  MAX_USB_BUS		9
#define  MAX_DEVICE_NUMBER	64

// Registry key used for usb serial number documentation
#define REGISTRY_KEY_PATH	"Drivers\\USBMSC"

// Registry key value names for usb serial number documentation
#define REGISTRY_REFCOUNT_VALUE	"RefCount"
#define REGISTRY_SERIAL_VALUE "Serial"


void
GetThumbKeyMode( SystemData_t * systemData )
{
	KeyData_t			keyData;
	UsbSerNumList_t	*	serNumList;
	int					i;
	CfgError_t			cfgErr;
	const char *		path = 0;

//	ULARGE_INTEGER		availableFreeBytes;
//	ULARGE_INTEGER		totalBytes;
//	ULARGE_INTEGER		totalFreeBytes;

	// See if we've got a USB drive attached.
	if (MountThumbDrive ())					// Development system booting to hard disk
	{
		path = "/Hard Disk2";
	}

	if ( path != 0 )
	{
		cfgErr = ParseKey( path, &keyData );
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
		LogDebug( "no USB drive found" );
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
			if ( _stricmp( keyData.serialNumber, serNumList->serNum[ i ] ) == 0 )
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

					case KEY_MODE_R_AND_D:
						systemData->inRandDMode = true;
						LogInfo( "USB key mode: RandD" );
						break;
                    
					default:
						break;
				}

                systemData->demoFromKey      = keyData.demoFromKey;
                systemData->useFastDemo      = keyData.useFastDemo;
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
	const char *		path = 0;

//	ULARGE_INTEGER		availableFreeBytes;
//	ULARGE_INTEGER		totalBytes;
//	ULARGE_INTEGER		totalFreeBytes;

	// See if we've got a USB drive attached.
	if (MountThumbDrive ())					// Development system booting to hard disk
	{
		path = "/Hard Disk2";
	}

	if ( path != 0 )
	{
		cfgErr = ParseKey( path, &keyData );
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
		LogDebug( "no USB drive found" );
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
			if ( _stricmp( keyData.serialNumber, serNumList->serNum[ i ] ) == 0 )
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
	static UsbSerNumList_t		serNumList;
	HKEY						hUSBKey;
	HKEY						hCurrentUSB;
	LONG						hRes;
	DWORD						dwType;
	DWORD						dwRefCount;
	DWORD						dwcbData;
	wchar_t						usbDevice[255];
	wchar_t						serialNum[255];
	unsigned int				i;

	memset( &serNumList, 0, sizeof( UsbSerNumList_t ) );

	// Open Registry
	hRes = RegOpenKeyEx(HKEY_LOCAL_MACHINE , TEXT(REGISTRY_KEY_PATH), 0, 0, &hUSBKey);
	if (ERROR_SUCCESS == hRes && hUSBKey != NULL) 
	{
		dwcbData = sizeof(dwRefCount);

		hRes = RegQueryValueEx(hUSBKey, TEXT(REGISTRY_REFCOUNT_VALUE), NULL, &dwType, (LPBYTE)&dwRefCount, &dwcbData);
		if (hRes != ERROR_SUCCESS || dwRefCount == 0)
		{
			LogDebug("GetUsbSerNums: failed query refcount");
			goto EXIT;
		}

		for (i=0; i < dwRefCount; i++)
		{	
			dwcbData = sizeof(usbDevice);
			hRes = RegEnumKeyEx(hUSBKey, i, usbDevice, &dwcbData, NULL, NULL, NULL, NULL);
			if (hRes != ERROR_SUCCESS)
			{
				LogDebug("GetUsbSerNums: Failed Enum RegKey %u", i);
				goto EXIT;
			}

			//Get the serial number string.
			hRes = RegOpenKeyEx(hUSBKey, usbDevice, 0, 0, &hCurrentUSB);
			if (ERROR_SUCCESS == hRes && hCurrentUSB != NULL) 
			{
				dwcbData = sizeof(serialNum);
				hRes = RegQueryValueEx(hCurrentUSB, TEXT(REGISTRY_SERIAL_VALUE), NULL, NULL, (LPBYTE)serialNum, &dwcbData);
				if (hRes != ERROR_SUCCESS)
				{
					LogDebug("GetUsbSerNums: failed query regSerialNum, hRes=%u", hRes);
					RegCloseKey(hCurrentUSB);
					goto EXIT;
				}

				// Add the serial number to our list.
				if ( serialNum != 0 && (serNumList.count < 4) )
				{
					sprintf(serNumList.serNum[ serNumList.count ], "%S", serialNum);
					serNumList.serNum[ serNumList.count ][ 255 ] = '\0';
					serNumList.count++;
				}
				RegCloseKey(hCurrentUSB);
			}
		}
	}

EXIT:
	RegCloseKey(hUSBKey);
	LogDebug("GetUsbSerNums: Exit");
	return &serNumList;

}
