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

static int			topItem;
static char	*		languageStr[] = {"English", "Français", "Deutsch", "Italiano", "Español",
									 "Português", "Português Brasil", 0};
static	Language_t	language[] = {LANG_ENGLISH, LANG_FRENCH, LANG_GERMAN, LANG_ITALIAN, LANG_SPANISH,
								  LANG_PORTUGUESE, LANG_PORTUGUESE_B};
static char *		keyboardDvr[] = {"en_US_101.kbd", "fr_FR_102.kbd", "de_DE_102.kbd",
									 "it_IT_102.kbd", "es_ES_102.kbd", "pt_PT_102.kbd",
									 "pt_BR_102.kbd"};

/*
 * class StateLanguageSetDisplay
 */
 
void
StateLanguageSetDisplay::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;
	
	SetTopLabel( strings[ TSTR_SELECT_DISPLAY_LANG ] );

	memset( &msg, 0, sizeof( msg ) );
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	for ( i = 0; i < 5; i++ )
	{
		strcpy( data->items[i][0], languageStr[i] );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;
	
	topItem = 0;

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
	bool				doUpdate = false;
	int					i;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			settingsData->language = language[ idata + topItem ];

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
			if ( topItem == 5 )
			{
				topItem = 0;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			if ( topItem == 0)
			{
				topItem = 5;
				doUpdate = true;
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

		default:
			break;
	}
	
	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; (i < 5) && (languageStr[ i + topItem ] != 0); i++ )
		{
			strcpy( data->items[i][0], languageStr[ i + topItem ]);
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
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
	int						i;

	SetTopLabel( strings[ TSTR_SELECT_KEYBOARD ] );

	memset( &msg, 0, sizeof( msg ) );
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	for ( i = 0; (i < 5) && (languageStr[ i + topItem ] != 0); i++ )
	{
		strcpy( data->items[i][0], languageStr[i + topItem] );
	}
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
	const char *		kbd;
	bool				doUpdate = false;
	int					i;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			settingsData->keyboard = language[ idata + topItem ];
			kbd = keyboardDvr[ idata + topItem ];

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
			if ( topItem == 5 )
			{
				topItem = 0;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			if ( topItem == 0)
			{
				topItem = 5;
				doUpdate = true;
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
		default:
			break;
	}

	if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; (i < 5) && (languageStr[ i + topItem ] != 0); i++ )
		{
			strcpy( data->items[i][0], languageStr[ i + topItem ]);
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

