/*
 * FILENAME
 * 	SecurityStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine security
 * 	states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mount.h>

#include "SettingsStates.h"
#include "SecurityStates.h"
#include "ParseSettings.h"
#include "GetThumbKeyMode.h"

/*
 * class StateSecuritySelect
 */
 
void
StateSecuritySelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_SECURITY_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_ADMINISTRATORS ] );
	strcpy( data->buttonText[1], strings[ MSTR_OPERATORS ] );
	strcpy( data->buttonText[2], strings[ MSTR_CONTROLS ] );

	if ( LockoutKeyInstalled() )
	{
		strcpy( data->buttonText[3], strings[ MSTR_LOCKOUT ] );
		useLockoutInfo = true;
	}
	else
	{
		useLockoutInfo = false;
	}
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecuritySelect::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_SELECT;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Administrators
					retVal = STATE_SECURITY_ADMIN_MANAGE;
					break;
				case 1:		// Operators
					retVal = STATE_SECURITY_OP_MANAGE;
					break;
				case 2:		// Controls
					retVal = STATE_SECURITY_CONTROL_SET_DATE;
					break;
				case 3:		// Lockout
					retVal = STATE_SECURITY_LOCKOUT_SETTINGS;
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_SELECT );
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
			if ( useLockoutInfo )
			{
				infoText = strings[ ISTR_SECURITY_SETTINGS_LOCKOUT ];
			}
			else
			{
				infoText = strings[ ISTR_SECURITY_SETTINGS ];
			}
			retVal = STATE_INFO;
			stateStack.Push( STATE_SECURITY_SELECT );
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityLockoutSettings
 */
 
void
StateSecurityLockoutSettings::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleMultiInfoData_t *	data = &msg.data.middleMultiInfo;

	SetTopLabel( strings[ TSTR_LOCKOUT_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_MULTI_INFO;

	memset( data, 0, sizeof( GuiMiddleMultiInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_ANALYZE ] );
	strcpy( data->buttonText[1], strings[ MSTR_RECALL_RESULTS ] );
	strcpy( data->buttonText[2], strings[ MSTR_SETTINGS ] );
	data->buttonChecked[0] = settingsData->analysisLockoutEnabled ? 1 : 0;
	data->buttonChecked[1] = settingsData->recallLockoutEnabled ? 1 : 0;
	data->buttonChecked[2] = settingsData->settingsLockoutEnabled ? 1 : 0;

	SendGuiMessage( &msg, sizeof( GuiMiddleMultiInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityLockoutSettings::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_SECURITY_LOCKOUT_SETTINGS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_TOGGLE:
			switch ( idata & 0x0F )
			{
				case 0:
					settingsData->analysisLockoutEnabled = idata & 0xF0 ? 1 : 0;
					break;
				case 1:
					settingsData->recallLockoutEnabled = idata & 0xF0 ? 1 : 0;
					break;
				case 2:
					settingsData->settingsLockoutEnabled = idata & 0xF0 ? 1 : 0;
					break;
			}
			WriteSettings( settingsData );
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_SECURITY_LOCKOUT ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_SECURITY_LOCKOUT_SETTINGS );
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
				default:
					break;
			}
			break;
		default:
			break;
	}

	return retVal;
}


void
StateSecurityControlSetDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	time_t						t = expirys.GetControlExpiration();

	if ( t == 0 )
	{
		tm.tm_year = 0;
	}
	else
	{
		memcpy( &tm, gmtime( &t ), sizeof( struct tm ) );
	}

	SetTopLabel( strings[ TSTR_CONTROL_EXPIRATION_DATE ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
	strcpy( data->labelLeft, strings[ MSTR_DAY ] );
	strcpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( data->labelRight, strings[ MSTR_YEAR ] );

	if ( tm.tm_year != 0 )
	{
		sprintf( data->textLeft, "%d", tm.tm_mday );
		sprintf( data->textMiddle, "%s", strings.Month( tm.tm_mon ) );
		sprintf( data->textRight, "%d", tm.tm_year + 1900 );
	}
	else
	{
		strcpy( data->textLeft, "**" );
		strcpy( data->textMiddle, "***" );
		strcpy( data->textRight, "**" );
	} 
	
	strcpy( data->buttonText, strings[ MSTR_NEVER ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_DONE ], false );
}

state_t
StateSecurityControlSetDate::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_SECURITY_CONTROL_SET_DATE;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	bool						updateAll = false;
	const int					daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	CfgError_t					cfgErr;
	time_t						t;

	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			if ( tm.tm_year == 0 )
			{
				t = time( 0 );	// Default to today.
				memcpy( &tm, gmtime( &t ), sizeof( struct tm ) );
				updateAll = true;
			}
			else
			{
				switch ( idata )
				{
					case 0:
						tm.tm_mday++;
						if ( tm.tm_mday > 31 )
						{
							tm.tm_mday = 1;
						}
						updateDay = true;
						break;
					case 1:
						tm.tm_mon++;
						if ( tm.tm_mon > 11 )
						{
							tm.tm_mon = 0;
						}
						updateMonth = true;
						break;
					case 2:
						tm.tm_year++;
						if ( tm.tm_year < 0 )
						{
							tm.tm_year = 0;
						}
						updateYear = true;
						break;
					default:
						break;
				}
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( tm.tm_year == 0 )
			{
				t = time( 0 );	// Default to today + 90 days.
				memcpy( &tm, gmtime( &t ), sizeof( struct tm ) );
				updateAll = true;
			}
			else
			{
				switch ( idata )
				{
					case 0:
						tm.tm_mday--;
						if ( tm.tm_mday < 1 )
						{
							tm.tm_mday = 31;
						}
						updateDay = true;
						break;
					case 1:
						tm.tm_mon--;
						if ( tm.tm_mon < 0 )
						{
							tm.tm_mon = 11;
						}
						updateMonth = true;
						break;
					case 2:
						tm.tm_year--;
						if ( tm.tm_year < 0 )
						{
							tm.tm_year = 0;
						}
						updateYear = true;
						break;
					default:
						break;
				}
			}
			break;
		case MSG_GUI_OUT_BUTTON:
			tm.tm_year = 0;
			updateAll = true;
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( tm.tm_year == 0 )
			{
				cfgErr = expirys.SetControlExpiration( 0 );
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					retVal = STATE_ERROR;
				}
				else
				{
					retVal = stateStack.Pop();
				}
			}
			else if ( tm.tm_mday <= daysInMonth[ tm.tm_mon ]
			  || ( tm.tm_mday == 29 && tm.tm_mon == 1 && tm.tm_year % 4 == 0 && ( (tm.tm_year + 1900) / 100 ) % 4 == 0 ) )
			{
				cfgErr = expirys.SetControlExpiration( mktime( &tm ) );
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					retVal = STATE_ERROR;
				}
				else
				{
					retVal = stateStack.Pop();
				}
			}
			else
			{
				stateStack.Push( STATE_SECURITY_CONTROL_SET_DATE );
				retVal = STATE_INVALID_DATE;
			}
			break;

		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		sprintf( outData->textLeft, "%d", tm.tm_mday );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		sprintf( outData->textMiddle, "%s", strings.Month( tm.tm_mon ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		sprintf( outData->textRight, "%d", tm.tm_year + 1900 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateAll )
	{
		if ( tm.tm_year != 0 )
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			sprintf( outData->textLeft, "%d", tm.tm_mday );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			sprintf( outData->textMiddle, "%s", strings.Month( tm.tm_mon ) );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			sprintf( outData->textRight, "%d", tm.tm_year + 1900 );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
		else
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			strcpy( outData->textLeft, "**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			strcpy( outData->textMiddle, "***" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			strcpy( outData->textRight, "**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
	}
	
	return retVal;
}

