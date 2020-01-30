/*
 * FILENAME
 * 	SecurityOpStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine security
 * 	operator states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SecurityOpStates.h"

Expiry_t *	StateSecurityOp::expiry = 0;
char		StateSecurityOp::searchId[ ID_LEN ];
FILETIME		StateSecurityOp::searchDate;

wchar_t *
StateSecurityOp::
FormatDate( FILETIME date )
{
	static wchar_t buff[ 100 ];
	SYSTEMTIME		tp;
	
	if ( date.dwHighDateTime == 0 && date.dwLowDateTime == 0 )
	{
		wcscpy( buff, strings[ MSTR_NEVER ] );
	}
	else
	{
		FileTimeToSystemTime(&date, &tp);
	
		wsprintf( buff, L"%02d %s %d", tp.wDay, strings.Month( tp.wMonth ), tp.wYear );
	}
	
	return buff;
}

/*
 * class StateSecurityOpManage
 */
 
void
StateSecurityOpManage::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_MANAGE_OPERATORS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_ADD_OPERATOR ] );
	wcscpy( data->buttonText[1], strings[ MSTR_SEARCH ] );
	wcscpy( data->buttonText[2], strings[ MSTR_BROWSE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpManage::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_MANAGE;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Add Operator
                    if ( expirys.NumOps() < MAX_OPS )
                    {
                        retVal = STATE_SECURITY_OP_NEW_ID;
                    }
                    else
                    {
                        errNum = SERR_OPERATOR_ID_LIST_FULL;
                        retVal = STATE_ERROR;
                    }
					break;
				case 1:		// Search
					retVal = STATE_SECURITY_OP_SEARCH;
					break;
				case 2:		// Browse
					retVal = STATE_SECURITY_OP_LIST_ALL;
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_OP_MANAGE );
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
 * class StateSecurityOpSearch
 */

void
StateSecurityOpSearch::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;

	SetTopLabel( strings[ TSTR_SEARCH_OPERATORS ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_OPERATOR_ID ] );
	wcscpy( data->buttonText[1], strings[ MSTR_EXPIRATION_DATE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpSearch::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_SEARCH;
	int		idata = *(int *)data;
	//time_t	t;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Operator ID
					retVal = STATE_SECURITY_OP_ENTER_ID;
					break;
				case 1:		// Expiration Date
					retVal = STATE_SECURITY_OP_ENTER_DATE;
					
					// Start with the current date.
					//t = time( 0 );
					//memcpy( &tm, gmtime( &t ), sizeof( struct tm ) );
					GetLocalTime(&tm);
					
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_OP_SEARCH );
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
 * class StateSecurityOpNewId
 */

void
StateSecurityOpNewId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_NEW_OPERATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_ADD ], true );
}

state_t
StateSecurityOpNewId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_NEW_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	static Expiry_t	newExpiry;
	CfgError_t		cfgErr;

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
			if ( expirys.GetOp( sdata ) != 0 )		// Duplicate ID
			{
				retVal = STATE_ERROR;
				errNum = SERR_DUPLICATE_OP_ID;
				stateStack.Push( STATE_SECURITY_OP_NEW_ID );
			}
			else if (*sdata == 0) 
            {
                SendGuiMessage (MSG_GUI_SOUND_WARNING);
            } 
            else
			{
				expiry = &newExpiry;
				memset( expiry, 0, sizeof( Expiry_t ) );
				strncpy( expiry->id, sdata, ID_LEN );
				settingsData->operatorIdEnabled = true;

				// This Adds the new operator and updates the pointer to point
				// to the new operator in the operators list.
				cfgErr = expirys.AddOp( &expiry );
				if ( cfgErr != CFG_ERR_NONE )
				{
					errNum = SERR_FILE_WRITE;
					stateStack.Pop( STATE_SECURITY_OP_NEW_ID );	// So we go back to Manage Operators.
					retVal = STATE_ERROR;
				}
				else
				{
					retVal = STATE_SECURITY_OP_EDIT_SETTINGS;
                    // Only push this state if we have room to add another operator. Else, the Back button on the
                    // next screen should go back to the previous screen, not this one.
                    if ( expirys.NumOps() < MAX_OPS )
                    {
                        stateStack.Push( STATE_SECURITY_OP_NEW_ID );
                    }

				}
			}
			break;

		default:
			break;
	}
	
	return retVal;
}


/*
 * class StateSecurityOpEnterId
 */
 
void
StateSecurityOpEnterId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_OPERATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	wcscpy( msg.data.middleEnterId.initialValue, L"" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t
StateSecurityOpEnterId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_ENTER_ID;
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
			retVal = STATE_SECURITY_OP_LIST_ID;
			stateStack.Push( STATE_SECURITY_OP_ENTER_ID );
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateSecurityOpEnterDate
 */
 
void
StateSecurityOpEnterDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	
	SetTopLabel( strings[ TSTR_OPERATOR_EXPIRATION ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

	wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
	wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

	wsprintf( data->textLeft, L"%d", tm.wDay );
	wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth ) );
	wsprintf( data->textRight, L"%d", tm.wYear );
	
	data->buttonText[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_SEARCH ], true );
}

state_t
StateSecurityOpEnterDate::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_SECURITY_OP_ENTER_DATE;
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
					tm.wDay++;
					if ( tm.wDay > 31 )
					{
						tm.wDay = 1;
					}
					updateDay = true;
					break;
				case 1:
					tm.wMonth++;
					if ( tm.wMonth > 12 )
					{
						tm.wMonth = 1;
					}
					updateMonth = true;
					break;
				case 2:
					tm.wYear++;
					if ( tm.wYear < 0 )
					{
						tm.wYear = 0;
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
					tm.wDay--;
					if ( tm.wDay < 1 )
					{
						tm.wDay = 31;
					}
					updateDay = true;
					break;
				case 1:
					tm.wMonth--;
					if ( tm.wMonth < 1 )
					{
						tm.wMonth = 12;
					}
					updateMonth = true;
					break;
				case 2:
					tm.wYear--;
					if ( tm.wYear < 0 )
					{
						tm.wYear = 0;
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
					SystemTimeToFileTime(&tm, &searchDate);
					stateStack.Push( STATE_SECURITY_OP_ENTER_DATE );
					retVal = STATE_SECURITY_OP_LIST_EXPIRATION;
					break;
			}
			break;

		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.wDay );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.wYear );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	return retVal;
}

/*
 * class StateSecurityOpListId
 */
 
void
StateSecurityOpListId::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i, j;

	expiryList = expirys.GetOpsById();
	numItems = expirys.NumOps();

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
		
	SetTopLabel( strings[ TSTR_OPERATOR_ID ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );	
	for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
	{
		wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
		wcscpy( data->items[i][1], FormatDate( expiryList[ j ].expiration ) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpListId::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_LIST_ID;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			expiry = &expiryList[ topItem + idata ];
			retVal = STATE_SECURITY_OP_EDIT_SETTINGS;
			stateStack.Push( STATE_SECURITY_OP_LIST_ID );
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

		msg.msgId = MSG_GUI_UPDATE_LIST_SETTINGS;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
		{
			wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id ));
			wcscpy( data->items[i][1], FormatDate( expiryList[ j ].expiration ) );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

/*
 * class StateSecurityOpListExpiration
 */
 
void
StateSecurityOpListExpiration::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i, j;

	expiryList = expirys.GetOpsByDate();
	numItems = expirys.NumOps();

	// Set the top item to the first entry with the date specified, or the first
	// later date if none are found on that date.
	for ( topItem = 0; topItem < numItems; topItem++ )
	{
		//if (searchDate <= expiryList[ topItem ].expiration )
		if ( CompareFileTime(&searchDate, &expiryList[ topItem ].expiration) <=0 )
		{
			break;
		}
	}

	SetTopLabel( strings[ TSTR_OPERATOR_EXPIRATION ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );	
	for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
	{
		wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
		wcscpy( data->items[i][1], FormatDate( expiryList[ j ].expiration ) );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpListExpiration::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_LIST_EXPIRATION;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			expiry = &expiryList[ topItem + idata ];
			retVal = STATE_SECURITY_OP_EDIT_SETTINGS;
			stateStack.Push( STATE_SECURITY_OP_LIST_EXPIRATION );
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

		msg.msgId = MSG_GUI_UPDATE_LIST_SETTINGS;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
		{
			wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
			wcscpy( data->items[i][1], FormatDate( expiryList[ j ].expiration ) );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

/*
 * class StateSecurityOpListAll
 */
 
void
StateSecurityOpListAll::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	expiryList = expirys.GetOpsByDate();
	numItems = expirys.NumOps();
	topItem = 0;
		
	SetTopLabel( strings[ TSTR_OPERATOR_EXPIRATION ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );	
#if 0
	for ( i = 0; i < 6 && i < numItems; i++ )
	{
		strcpy( data->items[i][0], CstrToWstr (expiryList[ i ].id) );
		strcpy( data->items[i][1], FormatDate( expiryList[ i ].expiration ) );
	}
#else
	for ( i = 0; i < 5 && i < numItems; i++ )
	{
		wcscpy( data->items[i][0], CstrToWstr (expiryList[ i ].id) );
		wcscpy( data->items[i][1], FormatDate( expiryList[ i ].expiration ) );
	}
#endif
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpListAll::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_LIST_ALL;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			expiry = &expiryList[ topItem + idata ];
			retVal = STATE_SECURITY_OP_EDIT_SETTINGS;
			stateStack.Push( STATE_SECURITY_OP_LIST_ALL );
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

		msg.msgId = MSG_GUI_UPDATE_LIST_SETTINGS;
		memset( data, 0, sizeof( GuiMiddleListData_t ) );
		
		for ( i = 0, j = topItem; i < 5 && j < numItems; i++, j++ )
		{
			wcscpy( data->items[i][0], CstrToWstr (expiryList[ j ].id) );
			wcscpy( data->items[i][1], FormatDate( expiryList[ j ].expiration ) );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	return retVal;
}

/*
 * class StateSecurityOpEditSettings
 */
 
void
StateSecurityOpEditSettings::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleButtonsData_t *	data = &msg.data.middleButtons;
	wchar_t						buff[100];

	wsprintf( buff, L"%s %S", strings[ TSTR_OPERATOR ], expiry->id );
	SetTopLabel( buff );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS;

	memset( data, 0, sizeof( GuiMiddleButtonsData_t ) );
	wcscpy( data->buttonText[0], strings[ MSTR_CHANGE_EXPIRATION ] );
	wcscpy( data->buttonText[1], strings[ MSTR_DELETE_OPERATOR ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsData_t ) );

	SetBottomButtons( strings[ BSTR_BACK], false, strings[ BSTR_HOME ], true );
}

state_t
StateSecurityOpEditSettings::
ProcessMessage( int msgId, char * data )
{
	state_t	retVal = STATE_SECURITY_OP_EDIT_SETTINGS;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:		// Change Expiration
					retVal = STATE_SECURITY_OP_SET_DATE;
					//if ( expiry->expiration != 0 )
					if ( expiry->expiration.dwHighDateTime != 0 && expiry->expiration.dwLowDateTime != 0 )
					{
						//memcpy( &tm, gmtime( &expiry->expiration ), sizeof( struct tm ) );
						FileTimeToSystemTime(&expiry->expiration, &tm);
					}
					else
					{
						memset( &tm, 0, sizeof( tm ) );
					}
					break;
				case 1:		// Delete Operator
					retVal = STATE_SECURITY_OP_DELETE_WARNING;
					break;
				default:
					return retVal;
					break;
			}
			stateStack.Push( STATE_SECURITY_OP_EDIT_SETTINGS );
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
 * class StateSecurityOpSetDate
 */
 
void
StateSecurityOpSetDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	
	SetTopLabel( strings[ TSTR_SET_DATE ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
	wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
	wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

	if ( tm.wYear != 0 )
	{
		wsprintf( data->textLeft, L"%d", tm.wDay );
		wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth ) );
		wsprintf( data->textRight, L"%d", tm.wYear );
	}
	else
	{
		wcscpy( data->textLeft, L"**" );
		wcscpy( data->textMiddle, L"***" );
		wcscpy( data->textRight, L"**" );
	} 

	wcscpy( data->buttonText, strings[ MSTR_NEVER ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_DONE ], false );
}

state_t
StateSecurityOpSetDate::
ProcessMessage( int msgId, char * data )
{
	state_t						retVal = STATE_SECURITY_OP_SET_DATE;
	int							idata = ((GuiSendMsgData_t *)data)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	bool						updateAll = false;
	const int					daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	CfgError_t					cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			if ( tm.wYear == 0 )
			{
				GetLocalTime(&tm);
				updateAll = true;
			}
			else
			{
				switch ( idata )
				{
					case 0:
						tm.wDay++;
						if ( tm.wDay > 31 )
						{
							tm.wDay = 1;
						}
						updateDay = true;
						break;
					case 1:
						tm.wMonth++;
						if ( tm.wMonth > 12 )
						{
							tm.wMonth = 1;
						}
						updateMonth = true;
						break;
					case 2:
						tm.wYear++;
						if ( tm.wYear < 0 )
						{
							tm.wYear = 0;
						}
						updateYear = true;
						break;
					default:
						break;
				}
			}
			break;
		case MSG_GUI_OUT_DOWN:
			if ( tm.wYear == 0 )
			{
				// Default to today.
				GetLocalTime(&tm);
				updateAll = true;
			}
			else
			{
				switch ( idata )
				{
					case 0:
						tm.wDay--;
						if ( tm.wDay < 1 )
						{
							tm.wDay = 31;
						}
						updateDay = true;
						break;
					case 1:
						tm.wMonth--;
						if ( tm.wMonth < 1 )
						{
							tm.wMonth = 12;
						}
						updateMonth = true;
						break;
					case 2:
						tm.wYear--;
						if ( tm.wYear < 0 )
						{
							tm.wYear = 0;
						}
						updateYear = true;
						break;
					default:
						break;
				}
			}
			break;
		case MSG_GUI_OUT_BUTTON:
			tm.wYear = 0;
			updateAll = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( tm.wYear == 0 )
			{
				expiry->expiration.dwHighDateTime = 0;
				expiry->expiration.dwLowDateTime = 0;
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
			else if ( tm.wDay <= daysInMonth[ tm.wMonth-1 ]
			|| ( tm.wDay == 29 && tm.wMonth == 2 && tm.wYear % 4 == 0 && ( (tm.wYear) / 100 ) % 4 == 0 ) )
			{
				tm.wHour = 23;
				tm.wMinute = 59;
				tm.wSecond = 59;
// GLD
// By changing the value directly, our list is also updated.
				SystemTimeToFileTime(&tm, &expiry->expiration);
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
				stateStack.Push( STATE_SECURITY_OP_SET_DATE );
				retVal = STATE_INVALID_DATE;
			}
			break;
		default:
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		wsprintf( outData->textLeft, L"%d", tm.wDay );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.wYear );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
// GLD
// Ideally, we'll have a call that updates everything at once.
	else if ( updateAll )
	{
		if ( tm.wYear != 0 )
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			wsprintf( outData->textLeft, L"%d", tm.wDay );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth ) );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			wsprintf( outData->textRight, L"%d", tm.wYear );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
		else
		{
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
			wcscpy( outData->textLeft, L"**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
			wcscpy( outData->textMiddle, L"***" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
			msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
			wcscpy( outData->textRight, L"**" );
			SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
		}
	}
	
	return retVal;
}

/*
 * class StateSecurityOpDeleteWarning
 */
 
void
StateSecurityOpDeleteWarning::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiWinWarningData_t *	data = &msg.data.winWarning;

	msg.msgId = MSG_GUI_WIN_WARNING2;
	
	wcscpy( data->topLabel, strings[ TSTR_WARNING ] );
	wsprintf( data->message, L"%s %s", CstrToWstr (expiry->id), strings[ ESTR_REMOVE_FROM_OP_LIST ] );
	wcscpy( data->buttonText[0], strings[ BSTR_CANCEL ] );
	wcscpy( data->buttonText[1], strings[ BSTR_CONTINUE ] );

	SendGuiMessage( &msg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateSecurityOpDeleteWarning::
ProcessMessage( int msgId, char * data )
{
	state_t		retVal = STATE_SECURITY_OP_DELETE_WARNING;
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
					cfgErr = expirys.DeleteOp( expiry->id );
					if ( cfgErr != CFG_ERR_NONE )
					{
						errNum = SERR_FILE_WRITE;
						retVal = STATE_ERROR;
					}
					else
					{
						retVal = stateStack.Pop( STATE_SECURITY_OP_MANAGE );
					}
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_DELETE_OPERATOR ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_SECURITY_OP_DELETE_WARNING );
			break;
		default:
			break;
	}
	
	return retVal;
}

