/*
 * FILENAME
 * 	DisplayStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set display
 * 	settings states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <process.h>

#include "DisplayStates.h"
#include "SettingsMessages.h"
#include "SettingsData.h"
#include "ParseSettings.h"
#include "Hardware.h"
#include "LogErr.h"

#define BACKLIGHT_INCREMENT		10
#define BACKLIGHT_MINIMUM		10
#define BACKLIGHT_MAXIMUM		100

// Used by both ScreenSaver and PowerSave.
#define TIME_MINIMUM			15
#define TIME_MAXIMUM			720
#define TIME_NEVER				0

typedef enum
{
	INCREMENT_TIME,
	DECREMENT_TIME
} AdjustTime_t;

// GLD
// Class method?  Used by screen saver and power save
static int
AdjustTime( AdjustTime_t incDec, int time )
{
	if ( incDec == INCREMENT_TIME )
	{
		if ( time != TIME_NEVER )
		{
			if ( time < 60 )
			{
				time += 15;
			}
			else if ( time < TIME_MAXIMUM )
			{
				time += 60;
			}
			else
			{
				time = TIME_NEVER;
			}
		}
	}
	
	else if ( incDec == DECREMENT_TIME )
	{
		if ( time == TIME_NEVER )
		{
			time = TIME_MAXIMUM;
		}
		else if ( time > TIME_MINIMUM )
		{
			if ( time > 60 )
			{
				time -= 60;
			}
			else
			{
				time -= 15;
			}
		}
	}

	return time;
}	
	
/*
 * class StateDisplaySelect
 */
 
void
StateDisplaySelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_DISPLAY_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_BACKLIGHT ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SCREEN_SAVER ] );
	wcscpy( data->buttonText[2], strings[ MSTR_POWER_SAVE ] );
	//strcpy( data->buttonText[3], strings[ MSTR_CALIBRATE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateDisplaySelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_DISPLAY_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Backlight
					retVal = STATE_DISPLAY_BACKLIGHT;
					stateStack.Push( STATE_DISPLAY_SELECT );
					break;
				case 1:		// Screen Saver
					retVal = STATE_DISPLAY_SCREEN_SAVER;
					stateStack.Push( STATE_DISPLAY_SELECT );
					break;
				case 2:		// Power Save
					retVal = STATE_DISPLAY_POWER;
					stateStack.Push( STATE_DISPLAY_SELECT );
					break;
				case 3:		// Recalibrate
					retVal = STATE_DISPLAY_TOUCH;
					stateStack.Push( STATE_DISPLAY_SELECT );
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
			infoText = strings[ ISTR_DISPLAY_SETTINGS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_DISPLAY_SELECT );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateDisplayBacklight
 */
 
void
StateDisplayBacklight::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_BACKLIGHT_BRIGHTNESS ] );

	setting = settingsData->backlightLevel;
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );
	wsprintf( data->currentValue, L"%d%%", setting );
	wsprintf( data->newValue,L"%d%%", setting );
	wcscpy( data->button0Text, strings[ MSTR_FULL ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateDisplayBacklight::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_DISPLAY_BACKLIGHT;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Full
					setting = BACKLIGHT_MAXIMUM;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.backlightLevel;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			if ( setting < BACKLIGHT_MAXIMUM )
			{
				setting += BACKLIGHT_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( setting > BACKLIGHT_MINIMUM )
			{
				setting -= BACKLIGHT_INCREMENT;
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
                    LcdBackLightSet (systemData->cpuBoard, settingsData->backlightLevel);
					retVal = stateStack.Pop();
					break;
				case 1:		// Home
                    LcdBackLightSet (systemData->cpuBoard, settingsData->backlightLevel);
					retVal = stateStack.Home();
					break;
				case 2:		// Save
					settingsData->backlightLevel = setting;
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
			break;
		default:
			break;
	}

	if ( doUpdate == true )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		wsprintf( msg.data.middleEnterValue.newValue, L"%d%%", setting );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

        LcdBackLightSet (systemData->cpuBoard, setting);
	}

	return retVal;
}

/*
 * class StateDisplayScreenSaver
 */
 
void
StateDisplayScreenSaver::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_SCREEN_SAVER_WAIT ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );

	setting = settingsData->screenSaverTimeout;
	
	if ( setting == 0 )
	{
		wcscpy( data->currentValue, strings[ MSTR_NEVER ] );
	}
	else if ( setting < 60 )
	{
		wsprintf( data->currentValue, L"%d %s", setting, strings[ MSTR_MIN ] );
	}
	else
	{
		wsprintf( data->currentValue, L"%d %s", setting / 60, strings[ MSTR_HR ] );
	}
	
	wcscpy( data->newValue, data->currentValue );
	wcscpy( data->button0Text, strings[ MSTR_NEVER ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateDisplayScreenSaver::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_DISPLAY_SCREEN_SAVER;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Never
					setting = 0;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.screenSaverTimeout;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			setting = AdjustTime( INCREMENT_TIME, setting );
			doUpdate = true;
			break;
		case MSG_GUI_OUT_DOWN:
			setting = AdjustTime( DECREMENT_TIME, setting );
			doUpdate = true;
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
				case 2:		// Save
					settingsData->screenSaverTimeout = setting;
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
			break;
		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		if ( setting == 0 )
		{
			wcscpy( msg.data.middleEnterValue.newValue, strings[ MSTR_NEVER ] );
		}
		else if ( setting < 60 )
		{
			wsprintf( msg.data.middleEnterValue.newValue, L"%d %s", setting, strings[ MSTR_MIN ] );
		}
		else
		{
			wsprintf( msg.data.middleEnterValue.newValue, L"%d %s", setting / 60, strings[ MSTR_HR ] );
		}
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );
	}
	return retVal;
}

/*
 * class StateDisplayPower
 */
 
void
StateDisplayPower::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterValueData_t *	data = &msg.data.middleEnterValue;

	SetTopLabel( strings[ TSTR_POWER_SAVE_WAIT ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_VALUE;
	memset( data, 0, sizeof( GuiMiddleEnterValueData_t ) );
	wcscpy( data->currentLabel, strings[ MSTR_CURRENT ] );
	wcscpy( data->newLabel, strings[ MSTR_NEW ] );

	setting = settingsData->powerSaveTimeout;
	
	if ( setting == 0 )
	{
		wcscpy( data->currentValue, strings[ MSTR_NEVER ] );
	}
	else if ( setting < 60 )
	{
		wsprintf( data->currentValue, L"%d %s", setting, strings[ MSTR_MIN ] );
	}
	else
	{
		wsprintf( data->currentValue, L"%d %s", setting / 60, strings[ MSTR_HR ] );
	}
	
	wcscpy( data->newValue, data->currentValue );
	wcscpy( data->button0Text, strings[ MSTR_NEVER ] );
	wcscpy( data->button1Text, strings[ MSTR_DEFAULT ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_SAVE ], true );
}

state_t
StateDisplayPower::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_DISPLAY_POWER;
	int			idata = *(int *)data;
	bool		doUpdate = false;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Never
					setting = 0;
					doUpdate = true;
					break;
				case 1:		// Default
					setting = defaultSettings.powerSaveTimeout;
					doUpdate = true;
					break;
			}
			break;
		case MSG_GUI_OUT_UP:
			setting = AdjustTime( INCREMENT_TIME, setting );
			doUpdate = true;
			break;
		case MSG_GUI_OUT_DOWN:
			setting = AdjustTime( DECREMENT_TIME, setting );
			doUpdate = true;
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
				case 2:		// Save
					settingsData->powerSaveTimeout = setting;
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
			break;
		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t	msg;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_VALUE_NEW;
		if ( setting == 0 )
		{
			wcscpy( msg.data.middleEnterValue.newValue, strings[ MSTR_NEVER ] );
		}
		else if ( setting < 60 )
		{
			wsprintf( msg.data.middleEnterValue.newValue, L"%d %s", setting, strings[ MSTR_MIN ] );
		}
		else
		{
			wsprintf( msg.data.middleEnterValue.newValue, L"%d %s", setting / 60, strings[ MSTR_HR ] );
		}
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterValueData_t ) );
	}
	return retVal;
}

/*
 * class StateDisplayTouch
 */
 
void
StateDisplayTouch::
Enter()
{
	GuiReceiveMsg_t				msg;

	SetTopLabel( strings[ TSTR_CALIBRATE_SCREEN ] );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_PRESS_YES_TO_START ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], false, strings[ BSTR_YES ], true );
}

state_t
StateDisplayTouch::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_DISPLAY_TOUCH;
	int		idata = *(int *)data;
	wchar_t	argBuff[10];
	int		pid;
	PROCESS_INFORMATION		proc_info;
	int		result;
	wchar_t			touchCalib[256];

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
					wsprintf( argBuff, L"-l %d", settingsData->language );
					wsprintf(touchCalib, L"%S", TOUCH_SCREEN_PROGRAM);

					//pid = spawnlp( P_WAIT, "Tcalib", "Tcalib",  "-l", argBuff, 0 );
					result = CreateProcess(touchCalib, argBuff, NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
					pid = proc_info.dwProcessId;
					if ( result == 0 )
					{
						LogError( "failed to start hcecal (Touch Calibration)" );
					}

					retVal = stateStack.Pop();
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_TOUCH_CALIBRATE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_DISPLAY_TOUCH );
			break;
		default:
			break;
	}
	return retVal;
}
