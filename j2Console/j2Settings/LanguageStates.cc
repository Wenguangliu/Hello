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
 
#include <windows.h>
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


    numItems = 8; // include Chinese language
    topItem  = 0;

	SetTopLabel (strings [TSTR_SELECT_DISPLAY_LANG]);

	msg.msgId = MSG_GUI_MIDDLE_LIST;
    wcscpy (data->items[0][0], strings.Language (LANGUAGE_ENGLISH));
    wcscpy (data->items[1][0], strings.Language (LANGUAGE_FRENCH));
    wcscpy (data->items[2][0], strings.Language (LANGUAGE_GERMAN));
    wcscpy (data->items[3][0], strings.Language (LANGUAGE_ITALIAN));
    wcscpy (data->items[4][0], strings.Language (LANGUAGE_SPANISH));

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
    GuiReceiveMsg_t     msg;
    GuiMiddleListData_t *mdata = &msg.data.middleList;
    bool                doUpdate = false;
    

    

	switch (msgId) {
		case MSG_GUI_OUT_LIST:
            if (topItem < 5) {
			    switch (idata) {
				    case 0:	
                        settingsData->language = LANGUAGE_ENGLISH;	
                    break;
				    case 1:	
                        settingsData->language = LANGUAGE_FRENCH;	
                    break;
				    case 2:	
                        settingsData->language = LANGUAGE_GERMAN;	
                    break;
				    case 3:	
                        settingsData->language = LANGUAGE_ITALIAN;	
                    break;
				    case 4:	
                        settingsData->language = LANGUAGE_SPANISH;	
                    break;
				    default:	
                    break;
                }
			} else {
                switch (idata) {

                    case 0:
                        settingsData->language = LANGUAGE_PORTUGUESE;  
                    break;

                    case 1:
                        settingsData->language = LANGUAGE_PORTUGUESE_B; 
                    break;

                    case 2: 
                        if (settingsData->enableChinese) {
                            settingsData->language = LANGUAGE_CHINESE;
                            settingsData->keyboard = LANGUAGE_ENGLISH; // chinese default keyboard is English
                        }
                    break;

                    default:
                    break;
                }
            }

			// Save the updated settings.
			cfgErr = WriteSettings (settingsData);
			if (cfgErr != CFG_ERR_NONE) {
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			} else {
                if (settingsData->language == LANGUAGE_CHINESE) {
                    // directly goback languge setting page instead of goto keyboard setting page
                    retVal = stateStack.Pop();
                } else {
				    retVal = STATE_LANGUAGE_SET_KEYBOARD;
				    stateStack.Push (STATE_LANGUAGE_SET_DISPLAY);
                }
			}

			// Update the date/time strings for the bottom date/time window.
			SetBottomDateTimeStrings (&strings);

		break;

		case MSG_GUI_OUT_UP:
            if (topItem > 0) {
                topItem -= 5;
            }
            if (topItem < 0) {
                topItem = 0;
            }
            doUpdate = true;
		break;

		case MSG_GUI_OUT_DOWN:
			// Temp disable for future Chinese charcter used
            if (topItem + 5 < numItems) {
                topItem += 5;
                doUpdate = true;

            }
		break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch (idata) {
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

    if (doUpdate == true) {

        msg.msgId = MSG_GUI_UPDATE_LIST;
        memset (data, 0, sizeof (GuiMiddleListData_t));
        if (topItem < 5) {
            // page 0, 5 language
            wcscpy (mdata->items[0][0], strings.Language (LANGUAGE_ENGLISH));
            wcscpy (mdata->items[1][0], strings.Language (LANGUAGE_FRENCH));
            wcscpy (mdata->items[2][0], strings.Language (LANGUAGE_GERMAN));
            wcscpy (mdata->items[3][0], strings.Language (LANGUAGE_ITALIAN));
            wcscpy (mdata->items[4][0], strings.Language (LANGUAGE_SPANISH));
        } else {
            // page 1, more languages
            wcscpy (mdata->items[0][0], strings.Language (LANGUAGE_PORTUGUESE));
            wcscpy (mdata->items[1][0], strings.Language (LANGUAGE_PORTUGUESE_B));
            if (settingsData->enableChinese) {
			    wcscpy (mdata->items[2][0], strings.Language (LANGUAGE_CHINESE));
            } else {
                wcscpy (mdata->items[2][0], L"");
            }
            wcscpy (mdata->items[3][0], L"");
            wcscpy (mdata->items[4][0], L"");
        } 
        mdata->isSelectable = 1;
        mdata->hasUpDown = 1;
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

    numItems = 8; // include Chinese language
    topItem  = 0;
    
	SetTopLabel( strings[ TSTR_SELECT_KEYBOARD ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;
    wcscpy( data->items[0][0], strings.Language (LANGUAGE_ENGLISH) );
    wcscpy( data->items[1][0], strings.Language (LANGUAGE_FRENCH) );
    wcscpy( data->items[2][0], strings.Language (LANGUAGE_GERMAN) );
    wcscpy( data->items[3][0], strings.Language (LANGUAGE_ITALIAN) );
    wcscpy( data->items[4][0], strings.Language (LANGUAGE_SPANISH) );
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
    GuiReceiveMsg_t     msg;
    GuiMiddleListData_t *mdata = &msg.data.middleList;
    bool                doUpdate = false;

	msg.msgId = MSG_GUI_SET_LANGUAGE;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
            if (topItem < 5) {
			    switch ( idata )
			    {
				    case 0:	settingsData->keyboard = LANGUAGE_ENGLISH;	msg.data.languageData.kbd = 0x00000409;	break;
				    case 1:	settingsData->keyboard = LANGUAGE_FRENCH;	msg.data.languageData.kbd = 0x0000040c;	break;
				    case 2:	settingsData->keyboard = LANGUAGE_GERMAN;	msg.data.languageData.kbd = 0x00000407;	break;
				    case 3:	settingsData->keyboard = LANGUAGE_ITALIAN;	msg.data.languageData.kbd = 0x00000410;	break;
				    case 4:	settingsData->keyboard = LANGUAGE_SPANISH;	msg.data.languageData.kbd = 0x0000040a;	break;
				    default:	break;
			    }
            } else {
                switch ( idata ) {
                    case 0: settingsData->keyboard = LANGUAGE_PORTUGUESE;    msg.data.languageData.kbd = 0x00000816; break;
                    case 1: settingsData->keyboard = LANGUAGE_PORTUGUESE_B;  msg.data.languageData.kbd = 0x00000416; break;
                    derault: break;
                }

            }
			SendGuiMessage(&msg, sizeof(msg));

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
            // chinese enable, new bsp, support portuguese keyboard
            if (settingsData->enableChinese) {
                if (topItem > 0) {
                    topItem -= 5;
                }
                if (topItem < 0) {
                    topItem = 0;
                }
                doUpdate = true;
            }

			break;

		case MSG_GUI_OUT_DOWN:
            //only chinese enable, new bsp, support portuguese keyboard
            if (settingsData->enableChinese) {
                if (topItem + 5 < numItems) {
                    topItem += 5;
                    doUpdate = true;

                }
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

       if (doUpdate == true) {
        msg.msgId = MSG_GUI_UPDATE_LIST;
        memset (mdata, 0, sizeof (GuiMiddleListData_t));
        if (topItem < 5) {
            // page 0, 5 language
            wcscpy( mdata->items[0][0], strings.Language (LANGUAGE_ENGLISH) );
            wcscpy( mdata->items[1][0], strings.Language (LANGUAGE_FRENCH) );
            wcscpy( mdata->items[2][0], strings.Language (LANGUAGE_GERMAN) );
            wcscpy( mdata->items[3][0], strings.Language (LANGUAGE_ITALIAN) );
            wcscpy( mdata->items[4][0], strings.Language (LANGUAGE_SPANISH) );
        } else {
            // page 1, 
            //ZZZ future add to language string
            wcscpy (mdata->items[0][0], strings.Language (LANGUAGE_PORTUGUESE));
            wcscpy (mdata->items[1][0], strings.Language (LANGUAGE_PORTUGUESE_B));
            //wcscpy (mdata->items[2][0], L"");
            //wcscpy (mdata->items[3][0], L"");
        }
        mdata->isSelectable = 1;
        mdata->hasUpDown = 1;
        SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
    }

	return retVal;
}
