/*
 * FILENAME
 * 	AnalyzerInfoStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine set analyzer
 * 	info states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/neutrino.h>

#include "AnalyzerInfoStates.h"
#include "SettingsMessages.h"
#include "Shmem.h"
#include "CommonDefines.h"

InfoItem_t		StateAnalyzerInfo::info[ NUM_INFO_ITEMS ];
CounterData_t *	StateAnalyzerInfo::counterData = (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
VersionData_t *	StateAnalyzerInfo::versionData = (VersionData_t *)MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );

/*
 * class StateAnalyzerInfoView
 */
 
void
StateAnalyzerInfoView::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	topItem = 0;
	
	strcpy( info[0].label, strings[ MSTR_NAME ] );
	strcpy( info[0].value, strings[ STR_PRODUCT_NAME ] );
	strcpy( info[1].label, strings[ MSTR_SERIAL_NO ] );
	strcpy( info[1].value, factoryData->instrumentSerialNumber );
	strcpy( info[2].label, strings[ MSTR_VERSION ] );
	strcpy( info[2].value, versionData->version );
	numItems = 3;
		
	SetTopLabel( strings[ TSTR_ANALYZER_INFORMATION ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );
	for ( i = 0; i < 6 && i < numItems; i++ )
	{
		strcpy( data->items[i][0], info[i].label );
		strcpy( data->items[i][1], info[i].value );
	}
	data->isSelectable = 0;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateAnalyzerInfoView::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ANALYZERINFO_VIEW;
	int		idata = *(int *)data;
	bool	doUpdate = false;

	switch ( msgId )
	{
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
				case 1:		// Print
// GLD
//					retVal = STATE_ANALYZERINFO_PRINT;
//					stateStack.Push( STATE_ANALYZERINFO_VIEW );
//					break;
//				case 2:		// Home
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
			strcpy( data->items[i][0], info[j].label );
			strcpy( data->items[i][1], info[j].value );
		}
	
		data->isSelectable = 1;
		data->hasUpDown = 1;
	
		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}

	return retVal;
}

/*
 * class StateAnalyzerInfoPrint
 */
 
void
StateAnalyzerInfoPrint::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( 0 );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	
	strcpy( msg.data.middleMessage.message, strings[ MSTR_SENDING ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();

// GLD
// Need to do the actual print here.
	ThreadCreate( 0, TimerThread, NULL, NULL );

}

state_t
StateAnalyzerInfoPrint::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ANALYZERINFO_PRINT;
	
	if ( msgId == MSG_SETTINGS_TIMER )
	{
// GLD
// Should only get the timer for now.  Eventually we'll need to have a message
// come back from the thread that does the printing.  Either that or we
// do the printing in the Enter() method and just use the timer to get to here.
		retVal = stateStack.Pop();
	}
	
	return retVal;
}
