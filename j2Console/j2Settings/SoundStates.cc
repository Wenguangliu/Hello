/*
 * FILENAME
 * 	SoundStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set sounds
 * 	settings states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SoundStates.h"
#include "SettingsMessages.h"
#include "SettingsData.h"
#include "ParseSettings.h"
#include "CfgErr.h"

#define SOUND_MAXIMUM	100
#define SOUND_MINIMUM	0
#define SOUND_INCREMENT	10

/*
 * class StateSoundSelect
 */
 
void
StateSoundSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_SOUND_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;
	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_SCREEN_CLICKS ] );
	wcscpy( data->buttonText[1], strings[ MSTR_ALERTS ] );
	wcscpy( data->buttonText[2], strings[ MSTR_STATUS ] );
	data->buttonHighlighted[0] = false;
	data->buttonHighlighted[1] = false;
	data->buttonHighlighted[2] = false;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSoundSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SOUND_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Click
					retVal = STATE_SOUND_CLICK;
					stateStack.Push( STATE_SOUND_SELECT );
					break;
				case 1:		// Alerts
					retVal = STATE_SOUND_ALERT;
					stateStack.Push( STATE_SOUND_SELECT );
					break;
				case 2:		// Status
					retVal = STATE_SOUND_STATUS;
					stateStack.Push( STATE_SOUND_SELECT );
					break;
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					retVal = stateStack.Home();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SOUND_SETTINGS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_SOUND_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSoundClick
 */
 
void
StateSoundClick::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_CLICK_VOLUME ] );

	systemSetting = settingsData->touchClickVolume;
	
	// Turn the automatic click off.  We don't want a click until *after* we've
	// adjusted the volume each time.
	msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
	msg.data.soundData.volume = 0;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d%%", systemSetting );
	wsprintf( data->newValue, L"%d%%", systemSetting );
	wcscpy( data->button0Text, strings[ MSTR_OFF ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateSoundClick::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_SOUND_CLICK;
	int				idata = *(int *)data;
	bool			doUpdate = false;
	GuiReceiveMsg_t	msg;
	CfgError_t		cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Off
					settingsData->touchClickVolume = SOUND_MINIMUM;
					break;
				case 1:		// Default
					settingsData->touchClickVolume = defaultSettings.touchClickVolume;
					break;
			}
			doUpdate = true;
			break;
		case MSG_GUI_OUT_UP:
			if ( settingsData->touchClickVolume < SOUND_MAXIMUM )
			{
				settingsData->touchClickVolume += SOUND_INCREMENT;
			}
			doUpdate = true;
			break;
		case MSG_GUI_OUT_DOWN:
			if ( settingsData->touchClickVolume > SOUND_MINIMUM )
			{
				settingsData->touchClickVolume -= SOUND_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					settingsData->touchClickVolume = systemSetting;
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					settingsData->touchClickVolume = systemSetting;
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					cfgErr = WriteSettings( settingsData );
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
			// Turn the click back on.
			msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
			msg.data.soundData.volume = settingsData->touchClickVolume;
			SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
			
			// Do a click.
			SendGuiMessage( MSG_GUI_SOUND_CLICK );
			break;
		default:
			break;
	}

	if ( doUpdate )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue, L"%d%%", settingsData->touchClickVolume );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

		// Do a "reference" click.
		msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
		msg.data.soundData.volume = settingsData->touchClickVolume;
		SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
		SendGuiMessage( MSG_GUI_SOUND_CLICK );

		// Turn the automatic click back off.
		msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
		msg.data.soundData.volume = 0;
		SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
	}
		
	return retVal;
}

/*
 * class StateSoundAlert
 */
 
void
StateSoundAlert::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_ALERT_VOLUME ] );

	systemSetting = settingsData->alertVolume;

	// Turn the click off.
	msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
	msg.data.soundData.volume = 0;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d%%", systemSetting );
	wsprintf( data->newValue, L"%d%%", systemSetting );
	wcscpy( data->button0Text, strings[ MSTR_OFF ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateSoundAlert::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_SOUND_ALERT;
	int				idata = *(int *)data;
	bool			doUpdate = false;
	GuiReceiveMsg_t	msg;
	CfgError_t		cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Off
					settingsData->alertVolume = SOUND_MINIMUM;
					break;
				case 1:		// Default
					settingsData->alertVolume = defaultSettings.alertVolume;
					break;
			}
			doUpdate = true;
			break;
		case MSG_GUI_OUT_UP:
			if ( settingsData->alertVolume < SOUND_MAXIMUM )
			{
				settingsData->alertVolume += SOUND_INCREMENT;
			}
			doUpdate = true;
			break;
		case MSG_GUI_OUT_DOWN:
			if ( settingsData->alertVolume > SOUND_MINIMUM )
			{
				settingsData->alertVolume -= SOUND_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					settingsData->alertVolume = systemSetting;
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					settingsData->alertVolume = systemSetting;
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					cfgErr = WriteSettings( settingsData );
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
			// Turn the click back on.
			msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
			msg.data.soundData.volume = settingsData->touchClickVolume;
			SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
			
			// Do a click.
			SendGuiMessage( MSG_GUI_SOUND_CLICK );
			break;
		default:
			break;
	}

	if ( doUpdate )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue,L"%d%%", settingsData->alertVolume );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

		msg.msgId = MSG_GUI_SOUND_SET_ALERT_VOLUME;
		msg.data.soundData.volume = settingsData->alertVolume;
		SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
		SendGuiMessage( MSG_GUI_SOUND_WARNING );
	}
		
	return retVal;
}

/*
 * class StateSoundStatus
 */
 
void
StateSoundStatus::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_STATUS_VOLUME ] );

	systemSetting = settingsData->statusVolume;
	
	// Turn the click off.
	msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
	msg.data.soundData.volume = 0;
	SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d%%", systemSetting );
	wsprintf( data->newValue, L"%d%%", systemSetting );
	wcscpy( data->button0Text, strings[ MSTR_OFF ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateSoundStatus::
ProcessMessage( int msgId, char * data )
{
	state_t 		retVal = STATE_SOUND_STATUS;
	int				idata = *(int *)data;
	bool			doUpdate = false;
	GuiReceiveMsg_t	msg;
	CfgError_t		cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Off
					settingsData->statusVolume = SOUND_MINIMUM;
					doUpdate = true;
					break;
				case 1:		// Default
					settingsData->statusVolume = defaultSettings.statusVolume;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			if ( settingsData->statusVolume < SOUND_MAXIMUM )
			{
				settingsData->statusVolume += SOUND_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( settingsData->statusVolume > SOUND_MINIMUM )
			{
				settingsData->statusVolume -= SOUND_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					settingsData->statusVolume = systemSetting;
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
					settingsData->statusVolume = systemSetting;
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					cfgErr = WriteSettings( settingsData );
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
			// Turn the click back on.
			msg.msgId = MSG_GUI_SOUND_SET_CLICK_VOLUME;
			msg.data.soundData.volume = settingsData->touchClickVolume;
			SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
			
			// Do a click.
			SendGuiMessage( MSG_GUI_SOUND_CLICK );
			break;
		default:
			break;
	}

	if ( doUpdate )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue, L"%d%%", settingsData->statusVolume );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

		msg.msgId = MSG_GUI_SOUND_SET_STATUS_VOLUME;
		msg.data.soundData.volume = settingsData->statusVolume;
		SendGuiMessage( &msg, sizeof( GuiSoundData_t ) );
		SendGuiMessage( MSG_GUI_SOUND_PRINTING_DONE );
	}
		
	return retVal;
}

