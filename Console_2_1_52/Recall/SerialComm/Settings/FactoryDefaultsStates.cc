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
	strcpy( msg.data.middleMessage.message, strings[ MSTR_PRESS_YES_DEFAULTS ] );
	
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
	Units			units;
	GuiReceiveMsg_t	msg;
	CfgError_t		cfgErr;
	int				pid;
	CommInterface_t	commInterface;
	int				analysisLockoutEnabled;
	int				recallLockoutEnabled;
	int				settingsLockoutEnabled;

	/*
	 *  Remember the current comm interface and lockout settings.
	 */

	commInterface = settingsData->comm.interface;
	analysisLockoutEnabled = settingsData->analysisLockoutEnabled;
	recallLockoutEnabled = settingsData->recallLockoutEnabled;
	settingsLockoutEnabled = settingsData->settingsLockoutEnabled;

	/*
	 *  Set the main settings data to the defaults.
	 */

	// Set the settingsData structure to the default values.
	*settingsData = defaultSettings;

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

	SetBacklight( settingsData->backlightLevel );

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
	 
	// Slay the driver.
	pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "devi-hid",  0 );
	if ( pid == -1 )
	{
		LogError( "failed to run 'slay devi-hid'" );
	}
	else
	{	
		// Restart the driver.
		pid = spawnlp( P_WAIT, "devi-hid", "devi-hid", "mouse", "kbd", "keyboard", "-k", "en_US_101.kbd", 0 );
		if ( pid == -1 )
		{
			LogError( "failed to start devi-hid" );
		}
	}
	
	/*
	 * Set the serial port comm program to SerialComm if we've been in ASTM.
	 */

	if ( commInterface == COMM_ASTM )
	{
		pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", ASTM_PROGRAM,  0 );
		if ( pid == -1 )
		{
			LogError( "failed to run 'slay ASTM'" );
		}
		else
		{	
			systemData->commPid = spawnlp( P_NOWAIT, SERIALCOMM_PROGRAM, SERIALCOMM_PROGRAM, 0 );
			if ( systemData->commPid == -1 )
			{
				LogError( "failed to start SerialComm" );
			}
		}
	}

	/*
	 *  Set the ethernet port to the default settings and rewrite SetIpAddress.
	 */

	StateComm::SetEthernet( true );

	/*
	 *  Set the strings for the bottom date/time window.
	 */

	SetBottomDateTimeStrings();

	return CFG_ERR_NONE;
}
