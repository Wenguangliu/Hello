/*
 * FILENAME
 * 	FactoryDefaultsStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set factory
 * 	defaults states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FactoryDefaultsStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "Hardware.h"
#include "RefRanges.h"
#include "Units.h"
#include "ParseRefRanges.h"
#include "CfgErr.h"
#include "LogErr.h"
#include "CommStates.h"
#include "GetThumbKeyMode.h"
#include "NetworkSettings.h"

static char *noChineseBspVersion = "3.1.2.9";


/*
 * class StateFactoryDefaults
 */
 
void
StateFactoryDefaults::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_FACTORY_DEFAULTS ] );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_PRESS_YES_DEFAULTS ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_YES ], true );
}

state_t
StateFactoryDefaults::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_FACTORY_DEFAULTS;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
				case 2:		// Yes
					cfgErr = SetDefaults();
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop();
					}
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_FACTORY_DEFAULTS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_FACTORY_DEFAULTS );
			break;
		default:
			break;
	}
	return retVal;
}


CfgError_t
StateFactoryDefaults::
SetDefaults()
{
	Units			        units;
	GuiReceiveMsg_t	        msg;
	CfgError_t		        cfgErr;
	int				        pid;
	CommInterface_t	        commInterface;
	int				        analysisLockoutEnabled;
	int				        recallLockoutEnabled;
	int				        settingsLockoutEnabled;
	PROCESS_INFORMATION		proc_info;
	int						result;
	wchar_t					processWSTR[256];
    int                     len;
    EthernetPortInfo_t      ethernetPortInfo;
    bool                    chineseDisplay;

	/*
	 *  Remember the current comm interface and lockout settings.
	 */

	commInterface = settingsData->comm.commInterface;
	analysisLockoutEnabled = settingsData->analysisLockoutEnabled;
	recallLockoutEnabled = settingsData->recallLockoutEnabled;
	settingsLockoutEnabled = settingsData->settingsLockoutEnabled;


	/*
	 *  Set the main settings data to the defaults.
	 */

    //remeber current Chinese display settings
    chineseDisplay = settingsData->enableChinese;
	// Set the settingsData structure to the default values.
	*settingsData = defaultSettings;
    //copy back the chinese display settings
    settingsData->enableChinese = chineseDisplay;
    

    //the default host name is analyzer last 6 serial numbers
    memset (settingsData->comm.hostName, 0, sizeof (settingsData->comm.hostName));
    len = strlen (factoryData->instrumentSerialNumber);
    strncpy (settingsData->comm.hostName,  factoryData->instrumentSerialNumber + (len - 6), 6);

	// If the lockout key isn't installed, restore the lockout settings.
	if ( !LockoutKeyInstalled() )
	{
		settingsData->analysisLockoutEnabled = analysisLockoutEnabled;
		settingsData->recallLockoutEnabled = recallLockoutEnabled;
		settingsData->settingsLockoutEnabled = settingsLockoutEnabled;
	}

	// Write the settingsData config file.
	cfgErr = WriteSettings( settingsData );
	if ( cfgErr != CFG_ERR_NONE )
	{
		return cfgErr;
	}

	/*
	 *  Set the reference ranges and units, which are stored in separate files,
	 *	to their defaults.
	 */

	SetRefRangesToDefaults( refRanges, factoryData->abaxisProductCode );
	cfgErr = WriteRefRanges( REF_RANGES_CURRENT_FILE, refRanges );
	if ( cfgErr != CFG_ERR_NONE )
	{
		return cfgErr;
	}

	cfgErr = units.SetToDefaults();
	if ( cfgErr != CFG_ERR_NONE )
	{
		return cfgErr;
	}

	/*
	 *  Delete all the administrators and operators, and set the control expiration to never.
	 */

	expirys.SetToDefaults();

	/*
	 *  Adjust the backlight and sound levels to the new settings.
	 */

    LcdBackLightSet (systemData->cpuBoard, settingsData->backlightLevel);

	msg.msgId = MSG_GUI_SOUND_SET_ALERT_VOLUME;
	msg.data.soundData.volume = settingsData->alertVolume;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );

	msg.msgId = MSG_GUI_SOUND_SET_STATUS_VOLUME;
	msg.data.soundData.volume = settingsData->statusVolume;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );

	msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
	msg.data.soundData.volume = settingsData->touchClickVolume;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );

	/*
	 * Set the keyboard to US.
	 */
	msg.msgId = MSG_GUI_SET_LANGUAGE;
	msg.data.languageData.kbd = 0x00000409;
	SendGuiMessage(&msg, sizeof(msg));

	
	/*
	 * Set the serial port comm program to SerialComm if we've been in ASTM.
	 */

	if (( commInterface == COMM_ASTM ) || (commInterface == COMM_HL7))
	{
		//pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", ASTM_PROGRAM,  0 );
		DWORD exitCode;
		HANDLE process = OpenProcess(0,FALSE, systemData->commPid);
		if (process != NULL) {
			GetExitCodeProcess(process, &exitCode);
			result = TerminateProcess(process, exitCode);
			if ( result == 0 )
			{
				LogError( "failed to run terminate ASTM process" );
			}
			CloseHandle(process);
			
			systemData->commPid = -1;
		
			//systemData->commPid = spawnlp( P_NOWAIT, SERIALCOMM_PROGRAM, SERIALCOMM_PROGRAM, 0 );
			wsprintf(processWSTR, L"%S", SERIALCOMM_PROGRAM);
			result = CreateProcess(processWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
			systemData->commPid = proc_info.dwProcessId;
			if ( result == 0 )
			{
				LogError( "failed to start SerialComm" );
				systemData->commPid = -1;
			}
		}
	}

	/*
	 *  Set the ethernet port to the default settings and rewrite SetIpAddress.
	 */

	StateComm::SetEthernet( true );

    // set default host name
    SetHostName (settingsData->comm.hostName);

    // get mac address
    GetEthernetPortInfo (&ethernetPortInfo);
    strcpy (settingsData->comm.macAddress, ethernetPortInfo.macAddress);
    
    /*
     *  set idle fan to be default speed
     */

    StateSettings::SendFanMessage ('F', settingsData->fanIdleSpeed);

	/*
	 *  Set the strings for the bottom date/time window.
	 */

	SetBottomDateTimeStrings (&strings);

	return CFG_ERR_NONE;
}
