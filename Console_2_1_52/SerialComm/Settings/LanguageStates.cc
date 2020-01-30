/*
 * FILENAME
 * 	LanguageStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set language
 * 	states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LanguageStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "LogErr.h"


/*
 * class StateLanguageSetDisplay
 */
 
void
StateLanguageSetDisplay::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;

	SetTopLabel( strings[ TSTR_SELECT_DISPLAY_LANG ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
	strcpy( data->items[0][0], "English" );
	strcpy( data->items[1][0], "Français" );
	strcpy( data->items[2][0], "Deutsch" );
	strcpy( data->items[3][0], "Italiano" );
	strcpy( data->items[4][0], "Español" );
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateLanguageSetDisplay::
ProcessMessage( int msgId, char * data )
{
	state_t 			retVal = STATE_LANGUAGE_SET_DISPLAY;
	int					idata = *(int *)data;
	CfgError_t			cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			switch ( idata )
			{
				case 0:	settingsData->language = LANG_ENGLISH;	break;
				case 1:	settingsData->language = LANG_FRENCH;	break;
				case 2:	settingsData->language = LANG_GERMAN;	break;
				case 3:	settingsData->language = LANG_ITALIAN;	break;
				case 4:	settingsData->language = LANG_SPANISH;	break;
				default:	break;
			}

			// Save the updated settings.
			cfgErr = WriteSettings( settingsData );
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			}
			else
			{
				retVal = STATE_LANGUAGE_SET_KEYBOARD;
				stateStack.Push( STATE_LANGUAGE_SET_DISPLAY );
			}

			// Update the date/time strings for the bottom date/time window.
			SetBottomDateTimeStrings();

			break;
		case MSG_GUI_OUT_UP:
			break;
		case MSG_GUI_OUT_DOWN:
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
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateLanguageSetKeyboard
 */
 
void
StateLanguageSetKeyboard::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;

	SetTopLabel( strings[ TSTR_SELECT_KEYBOARD ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
	strcpy( data->items[0][0], "English" );
	strcpy( data->items[1][0], "Français" );
	strcpy( data->items[2][0], "Deutsch" );
	strcpy( data->items[3][0], "Italiano" );
	strcpy( data->items[4][0], "Español" );
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateLanguageSetKeyboard::
ProcessMessage( int msgId, char * data )
{
	state_t 			retVal = STATE_LANGUAGE_SET_KEYBOARD;
	int					idata = *(int *)data;
	CfgError_t			cfgErr;
	int					pid;
	static const char *	kbd = "en_US_101.kbd";

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			switch ( idata )
			{
				case 0:	settingsData->keyboard = LANG_ENGLISH;	kbd = "en_US_101.kbd";	break;
				case 1:	settingsData->keyboard = LANG_FRENCH;	kbd = "fr_FR_102.kbd";	break;
				case 2:	settingsData->keyboard = LANG_GERMAN;	kbd = "de_DE_102.kbd";	break;
				case 3:	settingsData->keyboard = LANG_ITALIAN;	kbd = "it_IT_102.kbd";	break;
				case 4:	settingsData->keyboard = LANG_SPANISH;	kbd = "es_ES_102.kbd";	break;
				default:	break;
			}

			// Slay the driver.
			pid = spawnlp( P_WAIT, "slay", "slay", "-f", "-Q", "devi-hid",  0 );
			if ( pid == -1 )
			{
				LogError( "failed to run 'slay devi-hid'" );
			}
			else
			{	
				// Restart the driver.
				pid = spawnlp( P_WAIT, "devi-hid", "devi-hid", "mouse", "kbd", "keyboard", "-k", kbd, 0 );
				if ( pid == -1 )
				{
					LogError( "failed to start devi-hid" );
				}
			}

			// Save the updated settings.
			cfgErr = WriteSettings( settingsData );
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			}
			else
			{
				stateStack.Pop();	// Get rid of the Display state.
				retVal = stateStack.Pop();
			}
			break;
		case MSG_GUI_OUT_UP:
			break;
		case MSG_GUI_OUT_DOWN:
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
		default:
			break;
	}
	
	return retVal;
}

