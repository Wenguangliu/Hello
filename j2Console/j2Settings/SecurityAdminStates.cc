/*
 * FILENAME
 * 	SecurityAdminStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine security
 * 	administrator states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SecurityAdminStates.h"

char		StateSecurityAdmin::password[ 100 ];	// Used for entering/comparing password.
Expiry_t *	StateSecurityAdmin::expiry = 0;
char		StateSecurityAdmin::searchId[ ID_LEN ];
//time_t		StateSecurityAdmin::searchDate;

/*
 * class StateSecurityAdminManage
 */
 
void
StateSecurityAdminManage::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_MANAGE_ADMINISTRATORS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ADD_ADMINISTRATOR ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SEARCH ] );
	wcscpy( data->buttonText[2], strings[ MSTR_BROWSE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityAdminManage::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_MANAGE;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Add Administrator
                    if ( expirys.NumAdmins() < MAX_ADMINS )
                    {
                        retVal = STATE_SECURITY_ADMIN_NEW_ID;
                    }
                    else
                    {
                        errNum = SERR_ADMINISTRATOR_ID_LIST_FULL;
                        retVal = STATE_ERROR;
                    }
					break;
				case 1:		// Search
// GLD
//					retVal = STATE_SECURITY_ADMIN_SEARCH;
					retVal = STATE_SECURITY_ADMIN_ENTER_ID;
					break;
				case 2:		// Browse
					retVal = STATE_SECURITY_ADMIN_LIST_ALL;
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_ADMIN_MANAGE );
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
 * class StateSecurityAdminSearch
 */
/*
void
StateSecurityAdminSearch::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_SEARCH_ADMINISTRATORS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wstrcpy( data->buttonText[0], strings[ MSTR_ADMINISTRATOR_ID ] );
	wstrcpy( data->buttonText[1], strings[ MSTR_EXPIRATION_DATE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityAdminSearch::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_SEARCH;
	int		idata = *(int *)data;
	time_t	t;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Administrator ID
					retVal = STATE_SECURITY_ADMIN_ENTER_ID;
					break;
				case 1:		// Expiration Date
					retVal = STATE_SECURITY_ADMIN_ENTER_DATE;
					
					// Start with the current date.
					t = time( 0 );
					memcpy( &tm, gmtime( &t ), sizeof( struct tm ) );
					
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_ADMIN_SEARCH );
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
*/
/*
 * class StateSecurityAdminNewId
 */

void
StateSecurityAdminNewId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_NEW_ADMINISTRATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_ADD ], true );
}

state_t
StateSecurityAdminNewId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_NEW_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	static Expiry_t	newExpiry;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Add, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Add.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( expirys.GetAdmin( sdata ) != 0 )		// Duplicate ID
			{
				retVal = STATE_ERROR;
				errNum = SERR_DUPLICATE_ADMIN_ID;
				stateStack.Push( STATE_SECURITY_ADMIN_NEW_ID );
			}
            else  if (*sdata == 0)  // no addmit id input
            {
                SendGuiMessage (MSG_GUI_SOUND_WARNING);
            }
			else
			{
				expiry = &newExpiry;
				memset( expiry, 0, sizeof( Expiry_t ) );
				strncpy( expiry->id, sdata, ID_LEN );
				retVal = STATE_SECURITY_ADMIN_NEW_ENTER_PASSWORD;
                // Only push this state if we have room to add an additional administrator after this one.
                // Else, the Back button on the edit settings screen should go back to the previous screen,
                // not this one.
                if ( expirys.NumAdmins() < MAX_ADMINS-1 )
                {
                    stateStack.Push( STATE_SECURITY_ADMIN_NEW_ID );
                }

			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminNewEnterPassword
 */
 
void
StateSecurityAdminNewEnterPassword::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_PASSWORD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 1;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_NEXT ], true );
	
	// Init the password to empty.
	password[0] = '\0';
}

state_t
StateSecurityAdminNewEnterPassword::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_NEW_ENTER_PASSWORD;
	GuiReceiveMsg_t	msg;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Next.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( strlen( sdata ) != 0 )
			{
// GLD
// #define
				strncpy( password, sdata, 99 );
				retVal = STATE_SECURITY_ADMIN_NEW_CONFIRM_PASSWORD;
				stateStack.Push( STATE_SECURITY_ADMIN_NEW_ENTER_PASSWORD );
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminNewConfirmPassword
 */
 
void
StateSecurityAdminNewConfirmPassword::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM_PASSWORD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 1;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_NEXT ], true );
}

state_t
StateSecurityAdminNewConfirmPassword::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_NEW_CONFIRM_PASSWORD;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	CfgError_t		cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Save, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Save.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			// Passwords entered match, so go on.
			if ( _stricmp( sdata, password ) == 0 )
			{
				strcpy( expiry->password, password );
				cfgErr = expirys.AddAdmin( expiry );
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					stateStack.Pop( STATE_SECURITY_ADMIN_NEW_ID );	// So we go back to Manage Administrators.
					retVal = STATE_ERROR;
				}
				else
				{
					stateStack.Pop();	// Get rid of the first entry screen.
					retVal = STATE_SECURITY_ADMIN_EDIT_SETTINGS;
				}
			}
			// Mismatch, so return to the password entry screen.
			else
			{
				retVal = stateStack.Pop();
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminEnterId
 */
 
void
StateSecurityAdminEnterId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ADMINISTRATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t
StateSecurityAdminEnterId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_ENTER_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Search, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Search.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( searchId, sdata, ID_LEN );
			retVal = STATE_SECURITY_ADMIN_LIST_ID;
			stateStack.Push( STATE_SECURITY_ADMIN_ENTER_ID );
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminEnterDate
 */
/*
void
StateSecurityAdminEnterDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	
	SetTopLabel( strings[ TSTR_ADMINISTRATOR_EXPIRATION ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

#if 0
	strcpy( data->labelLeft, strings[ MSTR_DAY ] );
	strcpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( data->labelRight, strings[ MSTR_YEAR ] );

	sprintf( data->textLeft, "%d", tm.tm_mday );
	sprintf( data->textMiddle, "%s", strings.Month( tm.tm_mon ) );
	sprintf( data->textRight, "%d", tm.tm_year + 1900 );
	
	data->buttonText[0] = '\0';
#else
    wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
    wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
    wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

    wsprintf( data->textLeft, L"%d", tm.wDay);
    wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth ) );
    wsprintf( data->textRight, L"%d", tm.wYear);

    data->buttonText[0] = L'\0';

#endif
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t
StateSecurityAdminEnterDate::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_SECURITY_ADMIN_ENTER_DATE;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
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
			break;

		case MSG_GUI_OUT_DOWN:
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
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
					
				case 1:		// Search
					searchDate = mktime( &tm );
					stateStack.Push( STATE_SECURITY_ADMIN_ENTER_DATE );
					retVal = STATE_SECURITY_ADMIN_LIST_EXPIRATION;
					break;
			}
			break;

		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.tm_mday );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.tm_mon ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.tm_year + 1900 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	return retVal;
}
*/
/*
 * class StateSecurityAdminListId
 */
 
void
StateSecurityAdminListId::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i, j;

	expiryList = expirys.GetAdminsById();
	numItems = expirys.NumAdmins();

	// Set the top item to the specified ID, or the first ID that comes after it.
	for ( topItem = 0; topItem < numItems; topItem++ )
	{
		if ( _stricmp(  searchId, expiryList[ topItem ].id ) <= 0 )
		{
			break;
		}
	}
	// If the specified ID is greater than all the IDs present, just show the
	// end of the list.
	if ( topItem > numItems )
	{
		topItem = numItems - 5;
		if ( topItem < 0 )
		{
			topItem = 0;
		}
	}
		
	SetTopLabel( strings[ TSTR_ADMINISTRATOR_ID ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );	
	for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
	{
		wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityAdminListId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_LIST_ID;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			expiry = &expiryList[ topItem + idata ];
			retVal = STATE_SECURITY_ADMIN_EDIT_SETTINGS;
			stateStack.Push( STATE_SECURITY_ADMIN_LIST_ID );
			break;
		case MSG_GUI_OUT_UP:
			if ( topItem > 0 )
			{
				topItem -= 5;
				if ( topItem < 0 )
				{
					topItem = 0;
				}
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( topItem + 6 < numItems )
			{
				topItem += 5;
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
				default:
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
		int						i, j;

		msg.msgId = MSG_GUI_UPDATE_LIST;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
		{
			wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}


/*
 * class StateSecurityAdminListAll
 */
 
void
StateSecurityAdminListAll::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	expiryList = expirys.GetAdminsByDate();
	numItems = expirys.NumAdmins();
	topItem = 0;
		
	SetTopLabel( strings[ TSTR_ADMINISTRATOR_ID ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );	
	for ( i = 0; i < 5 && i < numItems; i++ )
	{
		wcscpy( data->items[i][0], CstrToWstr (expiryList[ i ].id) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityAdminListAll::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_LIST_ALL;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			expiry = &expiryList[ topItem + idata ];
			retVal = STATE_SECURITY_ADMIN_EDIT_SETTINGS;
			stateStack.Push( STATE_SECURITY_ADMIN_LIST_ALL );
			break;
		case MSG_GUI_OUT_UP:
			if ( topItem > 0 )
			{
				topItem -= 5;
				if ( topItem < 0 )
				{
					topItem = 0;
				}
				doUpdate = true;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( topItem + 5 < numItems )
			{
				topItem += 5;
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
				default:
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
		int						i, j;

		msg.msgId = MSG_GUI_UPDATE_LIST;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
		{
			wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminEditSettings
 */
 
void
StateSecurityAdminEditSettings::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;
	wchar_t						buff[100];

	wsprintf( buff, L"%s %S", strings[ TSTR_ADMINISTRATOR ], expiry->id );
	SetTopLabel( buff );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
//	wcscpy( data->buttonText[0], strings[ MSTR_CHANGE_EXPIRATION ] );
	wcscpy( data->buttonText[0], strings[ MSTR_CHANGE_PASSWORD ] );
	wcscpy( data->buttonText[1], strings[ MSTR_DELETE_ADMINISTRATOR ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityAdminEditSettings::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_EDIT_SETTINGS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
// GLD
/*
				case 0:		// Change Expiration
					retVal = STATE_SECURITY_ADMIN_SET_DATE;
					if ( expiry->expiration != 0 )
					{
						memcpy( &tm, gmtime( &expiry->expiration ), sizeof( struct tm ) );
					}
					else
					{
						memset( &tm, 0, sizeof( struct tm ) );
					}
					break;
*/
				case 0:		// Change Password
					retVal = STATE_SECURITY_ADMIN_ENTER_PASSWORD;
					break;
				case 1:		// Delete Administrator
					retVal = STATE_SECURITY_ADMIN_DELETE_WARNING;
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_ADMIN_EDIT_SETTINGS );
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
 * class StateSecurityAdminSetDate
 */
/*
void
StateSecurityAdminSetDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	
	SetTopLabel( strings[ TSTR_SET_DATE ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
#if 0
	strcpy( data->labelLeft, strings[ MSTR_DAY ] );
	strcpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( data->labelRight, strings[ MSTR_YEAR ] );
#else
    wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
    wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
    wcscpy( data->labelRight, strings[ MSTR_YEAR ] );
#endif

	if ( tm.tm_year != 0 )
	{
		wsprintf( data->textLeft, L"%d", tm.tm_mday );
		wsprintf( data->textMiddle, L"%s", strings.Month( tm.tm_mon ) );
		wprintf( data->textRight, L"%d", tm.tm_year + 1900 );
	}
	else
	{
		wstrcpy( data->textLeft, L"**" );
		wstrcpy( data->textMiddle, L"***" );
		wstrcpy( data->textRight, L"**" );
	} 

	wstrcpy( data->buttonText, strings[ MSTR_NEVER ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_DONE ], false );
}

state_t
StateSecurityAdminSetDate::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_SECURITY_ADMIN_SET_DATE;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	bool						updateAll = false;
	time_t						t;
	const int					daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	CfgError_t					cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			if ( tm.tm_year == 0 )
			{
				t = time( 0 ) + 7776000;	// Default to today + 90 days.
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
				t = time( 0 ) + 7776000;	// Default to today + 90 days.
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
				expiry->expiration = 0;
				cfgErr = expirys.WriteExpirysFile();
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
// GLD
// By changing the value directly, our list is also updated.
				expiry->expiration = mktime( &tm );
				cfgErr = expirys.WriteExpirysFile();
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
				stateStack.Push( STATE_SECURITY_ADMIN_SET_DATE );
				retVal = STATE_INVALID_DATE;
			}
			break;
		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.tm_mday );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.tm_mon ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.tm_year + 1900 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateAll )
	{
		if ( tm.tm_year != 0 )
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			wsprintf( outData->textLeft, L"%d", tm.tm_mday );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			wsprintf( outData->textMiddle, L"%s", strings.Month( tm.tm_mon ) );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			wsprintf( outData->textRight, L"%d", tm.tm_year + 1900 );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
		else
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			wstrcpy( outData->textLeft, L"**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			wstrcpy( outData->textMiddle, L"***" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			wstrcpy( outData->textRight, L"**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
	}
	
	return retVal;
}
*/

/*
 * class StateSecurityAdminEnterPassword
 */
 
void
StateSecurityAdminEnterPassword::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_PASSWORD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 1;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_NEXT ], true );
	
	// Init the password to empty.
	password[0] = '\0';
}

state_t
StateSecurityAdminEnterPassword::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_ENTER_PASSWORD;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Next, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Next.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( strlen( sdata ) != 0 )
			{
// GLD
// #define
				strncpy( password, sdata, 99 );
				retVal = STATE_SECURITY_ADMIN_CONFIRM_PASSWORD;
				stateStack.Push( STATE_SECURITY_ADMIN_ENTER_PASSWORD );
            }
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminConfirmPassword
 */
 
void
StateSecurityAdminConfirmPassword::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM_PASSWORD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 1;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_NEXT ], true );
}

state_t
StateSecurityAdminConfirmPassword::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_ADMIN_CONFIRM_PASSWORD;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	CfgError_t		cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Save, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Save.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( _stricmp( sdata, password ) == 0 )
			{
				strcpy( expiry->password, password );
				cfgErr = expirys.WriteExpirysFile();
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					retVal = STATE_ERROR;
				}
				else
				{
					stateStack.Pop();	// Get rid of the first entry screen.
					retVal = stateStack.Pop();
				}
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityAdminDeleteWarning
 */
 
void
StateSecurityAdminDeleteWarning::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiWinWarningData_t *	data = &msg.data.winWarning;

	msg.msgId = MSG_GUI_WIN_WARNING2;
	
	wcscpy( data->topLabel, strings[ TSTR_WARNING ] );
	wsprintf( data->message, L"%s %s", CstrToWstr (expiry->id), strings[ ESTR_REMOVE_FROM_ADMIN_LIST ] );
	wcscpy( data->buttonText[0], strings[ BSTR_CANCEL ] );
	wcscpy( data->buttonText[1], strings[ BSTR_CONTINUE ] );

	SendGuiMessage( &msg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateSecurityAdminDeleteWarning::
ProcessMessage( int msgId, char * data )
{
	state_t		retVal = STATE_SECURITY_ADMIN_DELETE_WARNING;
	int			idata = *(int *)data;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Cancel
					retVal = stateStack.Pop();
					break;
				case 1:		// Continue
					cfgErr = expirys.DeleteAdmin( expiry->id );
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop( STATE_SECURITY_ADMIN_MANAGE );
					}
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_DELETE_ADMINISTRATOR ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_SECURITY_ADMIN_DELETE_WARNING );
			break;
		default:
			break;
	}
	
	return retVal;
}

