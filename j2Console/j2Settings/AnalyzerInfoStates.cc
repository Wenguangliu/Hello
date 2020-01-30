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
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/neutrino.h>

#include "AnalyzerInfoStates.h"
#include "SettingsMessages.h"
#include "Shmem.h"
#include "CommonDefines.h"

InfoItem_t		StateAnalyzerInfo::info[ NUM_INFO_ITEMS ];
CounterData_t *	StateAnalyzerInfo::counterData = (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
VersionData_t *	StateAnalyzerInfo::versionData = (VersionData_t *)MapSharedMem( VERSION_SHMEM_NAME, sizeof( VersionData_t ) );
SystemData_t  * StateAnalyzerInfo::systemData  = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME,   sizeof( SystemData_t ) );


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
    char                    displayItem;

	topItem = 0;
	
	wcscpy( info[0].label, strings[ MSTR_NAME ] );
	wcscpy( info[0].value, strings[ STR_PRODUCT_NAME ] );
	wcscpy( info[1].label, strings[ MSTR_SERIAL_NO ] );
	wcscpy( info[1].value, CstrToWstr (factoryData->instrumentSerialNumber) );
	wcscpy( info[2].label, strings[ MSTR_VERSION ] );
	wcscpy( info[2].value, CstrToWstr (versionData->version) );
    if ((systemData->inManufacturingMode == TRUE) || (systemData->inRandDMode == TRUE)){
        wcscpy( info[3].label, L"Engine  Version:");
	    wcscpy( info[3].value, CstrToWstr (versionData->engSoftwareVersion));
        wcscpy( info[4].label, L"FPGA Version:");
	    wcscpy( info[4].value, CstrToWstr (versionData->engFpgaVersion));
        wcscpy( info[5].label, L"BSP Version:");
	    wcscpy( info[5].value, CstrToWstr (versionData->bspVersion));
        wcscpy( info[6].label, L"Printer Version:");
	    wcscpy( info[6].value, CstrToWstr (versionData->apsVersion));
        wcscpy( info[7].label, L"CPU Board:");
		wcscpy( info[7].value, (systemData->cpuBoard == 'D') ? L"Adlink": (systemData->cpuBoard == 'A') ? L"Aaeon": L"Adlink Bt");
        if (systemData->inRandDMode == TRUE) {
            wcscpy (info [8].label, L"Rotor Run:");
            wsprintf (info[8].value,L"%d",counterData->rotorRunCount);
            wcscpy (info [9].label, L"Rotor Abort:");
            wsprintf (info[9].value,L"%d",counterData->rotorAbortCount);
            wcscpy (info [10].label, L"Flash Count:");
            wsprintf (info[10].value,L"%d",counterData->flashCount);
            wcscpy (info [11].label, L"Miss Flash:");
            wsprintf (info[11].value,L"%d",counterData->missedFlashCount);
            wcscpy (info [12].label, L"Drawer Cycles:");
            wsprintf (info[12].value,L"%d",counterData->drawerCycleCount);
            wcscpy (info [13].label, L"Cam Cycles:");
            wsprintf (info[13].value,L"%d",counterData->camCycleCount);
            numItems = 14;
        } else {
            numItems = 8;
        }
    } else {
	    numItems = 3;
    }
    // max item each page is 5
    if (numItems > 5) {
        displayItem = 5;
    } else {
        displayItem = numItems;
    }
		
	SetTopLabel( strings[ TSTR_ANALYZER_INFORMATION ] );

	msg.msgId = MSG_GUI_MIDDLE_LIST_SETTINGS;

	memset( data, 0, sizeof( GuiMiddleListData_t ) );

    
	for ( i = 0; i < displayItem ; i++ )
	{
		wcscpy( data->items[i][0], info[i].label );
		wcscpy( data->items[i][1], info[i].value );
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
			wcscpy( data->items[i][0], info[j].label );
			wcscpy( data->items[i][1], info[j].value );
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
	
	wcscpy( msg.data.middleMessage.message, strings[ MSTR_SENDING ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();

// GLD
// Need to do the actual print here.
	//ThreadCreate( 0, TimerThread, NULL, NULL );
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);

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
