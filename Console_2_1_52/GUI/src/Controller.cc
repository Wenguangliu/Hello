/*
 * FILENAME
 * 	Controller.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the Controller class, which handles receiving messages
 * 	from other processes and passing them on to the correct objects.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/neutrino.h>
// GLD
#include <errno.h>

/* Local headers */
#include "ablibs.h"
#include "Global.h"
#include "abimport.h"
#include "Controller.h"
#include "TopSection.h"
#include "MiddleSection.h"
#include "BottomSection.h"
#include "GuiMessages.h"
#include "CommonDefines.h"
#include "Shmem.h"
#include "LogErr.h"
#include "StartupMessages.h"
#include "Sound.h"
#include "SettingsData.h"
#include "Shmem.h"

#define IS_TOP_MESSAGE( msgId )			( msgId > MSG_GUI_FIRST_TOP			&& msgId < MSG_GUI_LAST_TOP )
#define IS_MIDDLE_MESSAGE( msgId )		( msgId > MSG_GUI_FIRST_MIDDLE		&& msgId < MSG_GUI_LAST_MIDDLE )
#define IS_BOTTOM_MESSAGE( msgId )		( msgId > MSG_GUI_FIRST_BOTTOM		&& msgId < MSG_GUI_LAST_BOTTOM )
#define IS_WIN_MESSAGE( msgId )			( msgId > MSG_GUI_FIRST_WIN			&& msgId < MSG_GUI_LAST_WIN )
#define IS_UPDATE_MESSAGE( msgId )		( msgId > MSG_GUI_FIRST_UPDATE		&& msgId < MSG_GUI_LAST_UPDATE )
#define IS_SOUND_MESSAGE( msgId )		( msgId > MSG_GUI_FIRST_SOUND		&& msgId < MSG_GUI_LAST_SOUND )
#define IS_SCREENSAVE_MESSAGE( msgId)	( msgId > MSG_GUI_FIRST_SCREENSAVE	&& msgId < MSG_GUI_LAST_SCREENSAVE ) 

SettingsData_t * Controller::settingsData = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );


/*
 * FUNCTION
 * 	Constructor
 * 
 * DESCRIPTION
 * 	Creates the window subscreen objects and the thread that handles messages
 * 	received from other processes.
 */

Controller::
Controller()
{
	mqd_t			queue;
	StartupMsg_t	msg;
	
	/*
	 * Create the subscreen objects.
	 */
	topTitle 				= new TopTitle();

	middleAnalysisInProcess = new MiddleAnalysisInProcess();
	middleButtons 			= new MiddleButtons();
	middleButtonsInfo 		= new MiddleButtonsInfo();
	middleEnterDate 		= new MiddleEnterDate();
	middleEnterId 			= new MiddleEnterId();
	middleEnterIp			= new MiddleEnterIp();
	middleEnterRange 		= new MiddleEnterRange();
	middleEnterTime 		= new MiddleEnterTime();
	middleEnterValue		= new MiddleEnterValue();
	middleGraphicInfo1		= new MiddleGraphicInfo1();
	middleGraphicInfo2		= new MiddleGraphicInfo2();
	middleGraphicInfo3		= new MiddleGraphicInfo3();
	middleHomeReady 		= new MiddleHomeReady();
	middleHomeReadyMfg 		= new MiddleHomeReadyMfg();
	middleHomeWarming 		= new MiddleHomeWarming();
	middleHomeWarmingMfg 	= new MiddleHomeWarmingMfg();
	middleIconMatrix1 		= new MiddleIconMatrix1();
	middleIconMatrix2  		= new MiddleIconMatrix2();
	middleList 				= new MiddleList();
	middleListColored		= new MiddleListColored();
	middleListMulti			= new MiddleListMulti();
	middleListRefRanges  	= new MiddleListRefRanges();
	middleListResults 		= new MiddleListResults();
	middleListSettings		= new MiddleListSettings();
	middleLoadOrder			= new MiddleLoadOrder();
	middleMessage 			= new MiddleMessage();
	middleMessageInfo 		= new MiddleMessageInfo();
	middleMultiInfo			= new MiddleMultiInfo();
	middleMultiSelect		= new MiddleMultiSelect();
	middleOptionInfo		= new MiddleOptionInfo();
	middleProgBarMessage 	= new MiddleProgBarMessage();
	middleSelectOption		= new MiddleSelectOption();
	middleSelectType  		= new MiddleSelectType();
	middleViewResults 		= new MiddleViewResults();
	
	bottomButtons1 			= new BottomButtons1();
	bottomButtons2 			= new BottomButtons2();
	bottomButtons3 			= new BottomButtons3();
	bottomDateTime 			= new BottomDateTime();
	
	winWarning1				= new WinWarning1();
	winWarning2				= new WinWarning2();
	winError				= new WinError();
	winCriticalStop			= new WinCriticalStop();
	winCriticalStop2		= new WinCriticalStop2();

	/*
	 * Initialize the pointers to the current subscreen objects.
	 */	
	topSection				= 0;
	middleSection			= 0;
	bottomSection			= 0;
	winSection				= 0;

	/*
	 * Initialize the sound.
	 */
	InitSound();
	SetClickVolume( settingsData->touchClickVolume );
	SetAlertVolume( settingsData->alertVolume );
	SetStatusVolume( settingsData->statusVolume );

	/*
	 * Start the thread that receives command messages from external processes.
	 */
	ThreadCreate( 0, (void*(*)(void*))Controller::MsgReceiveThread, this, NULL );

	queue = mq_open( STARTUP_QUEUE_NAME, O_WRONLY );
	msg.msgId = MSG_STARTUP_GUI_READY;
	mq_send( queue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
}

/*
 * FUNCTION
 * 	RestoreCurrentWindow
 * 
 * DESCRIPTION
 * 	Restores the current window (the main window or one of the error windows)
 * 	to the front.  Called by the screen saver when it exits to make sure the
 * 	correct window is at the front and repainted.
 */

void
Controller::
RestoreCurrentWindow()
{
#if 0
	if ( winSection == 0 )
	{
		PtWindowToFront( ABW_winLcd );
	}
	else
	{
		PtWindowToFront( winSection->Window() );
	}
#endif
}

/*
 * FUNCTION
 * 	MsgReceiveThread
 * 
 * DESCRIPTION
 * 	Thread that receives messages from other processes and passes them to the
 * 	appropriate GUI object to be handled.
 * 
 * PARAMETERS
 * 	controller - The Controller object that calls this function.  Since this
 * 			must be a static method in order to be called as a thread function,
 * 			this is the only way for this function to access the controller.
 * 
 * RETURNS
 * 	NULL - Only returns if there is an error connecting to the incoming message
 * 			queue.
 */

void *
Controller::
MsgReceiveThread( Controller * controller )
{
	ssize_t			receivedSize = 0;
	GuiReceiveMsg_t	msg;
	struct mq_attr	mqstat;
	bool			firstTime = true;
		
	/*
	 * Open the queue and get its size.
	 */
	mqd_t queue = mq_open( GUI_QUEUE_NAME, O_RDONLY );
	
	if ( queue == -1 )
	{
		LogError( "failed to open message queue, errno: %d\n", errno );
		return NULL;
	}
	
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failed to get message queue attributes, errno: %d", errno );
		return NULL;
	}

	/*
	 * Continue forever receiving messages and processing them.
	 */
	while ( 1 )
	{
		receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );

		if ( receivedSize != -1 )
		{
			if ( receivedSize > 0 )
			{
				// Lock Photon for this thread.
				if ( PtEnter( 0 ) >= 0 )
				{
					if ( IS_TOP_MESSAGE( msg.msgId ) )
					{
						controller->SetTop( msg.msgId, &msg.data );
					}
					
					else if ( IS_MIDDLE_MESSAGE( msg.msgId ) )
					{
						controller->SetMiddle( msg.msgId, &msg.data );
					}
					
					else if ( IS_BOTTOM_MESSAGE( msg.msgId ) )
					{
						controller->SetBottom( msg.msgId, &msg.data );
					}

					else if ( IS_WIN_MESSAGE( msg.msgId ) )
					{
						controller->SetWin( msg.msgId, &msg.data );
					}
					
					else if ( IS_UPDATE_MESSAGE( msg.msgId ) )
					{
						controller->Update( &msg );
					}
					
					else if ( IS_SOUND_MESSAGE( msg.msgId ) )
					{
						controller->Sound( msg.msgId, &msg.data );
					}
					
					else if ( IS_SCREENSAVE_MESSAGE( msg.msgId ) )
					{
						controller->ScreenSaver( msg.msgId );
					}

					else if ( msg.msgId == MSG_GUI_SET_INPUT_RECEIVER )
					{
						Comm::SetCommReceiver( msg.data.inputReceiver.name );
					}
					
					else if ( msg.msgId == MSG_GUI_GET_ID_TEXT )
					{
						controller->middleEnterId->SendIdText();
					}
					
					else if ( msg.msgId == MSG_GUI_GET_RANGE )
					{
						controller->middleEnterRange->SendRange();
					}
					
					else if ( msg.msgId == MSG_GUI_SET_DATE_TIME_STRINGS )
					{
						UpdateDateTime::Init( &msg.data.bottomDateTime );
					}
					
					// Make sure display is up-to-date and release Photon.
					PtFlush();
					PtLeave( 0 );
				}
				else
				{
					LogError( "PtEnter() failed" );
				}
			}
		}
		else
		{
			LogError( "message receive error, errno: %d", errno );
			break;
		}
		
		// If this is the first time, make our window visible.
		if ( firstTime )
		{
			PhPoint_t pos = {240, 0};
			PtEnter( 0 );
			PtSetResource( ABW_winLcd, Pt_ARG_POS, &pos, 0 );
			PtSetResource( ABW_winLcd, Pt_ARG_CURSOR_TYPE, cursorType_g, 0 );
			PtBkgdHandlerProcess();
			PtLeave( 0 );
			
			firstTime = false;
		}
	}
	
	return NULL;
}

/*
 * FUNCTION
 * 	SetTop
 * 
 * DESCRIPTION
 * 	Handles a message to set the top section of the screen.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the received message.
 * 	msgData - Message data for the received message.
 */

void
Controller::
SetTop( int msgId, GuiReceiveMsgData_t * msgData )
{
	TopSection * newTopSection = 0;
	
	// Make sure there is no warning/error/critical stop window.
	if ( winSection != 0 )
	{
		winSection->Hide();
		winSection = 0;
	}
	
	// Determine the new top section.
	switch ( msgId )
	{
		case MSG_GUI_TOP_NONE:
			newTopSection = 0;
			break;
			
		case MSG_GUI_TOP_LABEL:
			newTopSection = topTitle;
			break;
			
		default:
			newTopSection = 0;
			break;
	}

	// If we're currently showing a section, hide it.
	if ( topSection != 0 )
	{
		topSection->Hide();
	}
	
	// If there's a new section ...
	if ( newTopSection != 0 )
	{
		// Set its fields.
		newTopSection->SetData( msgData );
		
		// Show the new section.
		newTopSection->Show();
	}

	// Remember this section for next time.
	topSection = newTopSection;
}


/*
 * FUNCTION
 * 	SetMiddle
 * 
 * DESCRIPTION
 * 	Handles a message to set the middle section of the screen.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the received message.
 * 	msgData - Message data for the received message.
 */

void
Controller::
SetMiddle( int msgId, GuiReceiveMsgData_t * msgData )
{
	MiddleSection * newMiddleSection = 0;
	
	// Make sure there is no warning/error/critical stop window.
	if ( winSection != 0 )
	{
		winSection->Hide();
		winSection = 0;
	}
	
	// Determine the new middle section.
	switch ( msgId )
	{
		case MSG_GUI_MIDDLE_NONE:
			newMiddleSection = 0;
			break;
			
		case MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS:
			newMiddleSection = middleAnalysisInProcess;
			break;

		case MSG_GUI_MIDDLE_BUTTONS:
			newMiddleSection = middleButtons;
			break;
			
		case MSG_GUI_MIDDLE_BUTTONS_INFO:
			newMiddleSection = middleButtonsInfo;
			break;
			
		case MSG_GUI_MIDDLE_ENTER_DATE:
			newMiddleSection = middleEnterDate;
			break;
			
		case MSG_GUI_MIDDLE_ENTER_ID:
			newMiddleSection = middleEnterId;
			break;

		case MSG_GUI_MIDDLE_ENTER_IP:
			newMiddleSection = middleEnterIp;
			break;

		case MSG_GUI_MIDDLE_ENTER_TIME:
			newMiddleSection = middleEnterTime;
			break;
			
		case MSG_GUI_MIDDLE_ENTER_RANGE:
			newMiddleSection = middleEnterRange;
			break;
			
		case MSG_GUI_MIDDLE_ENTER_VALUE:
			newMiddleSection = middleEnterValue;
			break;
			
		case MSG_GUI_MIDDLE_GRAPHIC_INFO1:
			newMiddleSection = middleGraphicInfo1;
			break;

		case MSG_GUI_MIDDLE_GRAPHIC_INFO2:
			newMiddleSection = middleGraphicInfo2;
			break;

		case MSG_GUI_MIDDLE_GRAPHIC_INFO3:
			newMiddleSection = middleGraphicInfo3;
			break;

		case MSG_GUI_MIDDLE_HOME_READY:
			newMiddleSection = middleHomeReady;
			break;
		
		case MSG_GUI_MIDDLE_HOME_READY_MFG:
			newMiddleSection = middleHomeReadyMfg;
			break;
			
		case MSG_GUI_MIDDLE_HOME_WARMING:
			newMiddleSection = middleHomeWarming;
			break;
			
		case MSG_GUI_MIDDLE_HOME_WARMING_MFG:
			newMiddleSection = middleHomeWarmingMfg;
			break;
			
		case MSG_GUI_MIDDLE_ICON_MATRIX_1:
			newMiddleSection = middleIconMatrix1;
			break;
			
		case MSG_GUI_MIDDLE_ICON_MATRIX_2:
			newMiddleSection = middleIconMatrix2;
			break;
			
		case MSG_GUI_MIDDLE_LIST:
			newMiddleSection = middleList;
			break;
		
		case MSG_GUI_MIDDLE_LIST_COLORED:
			newMiddleSection = middleListColored;
			break;
			
		case MSG_GUI_MIDDLE_LIST_MULTI:
			newMiddleSection = middleListMulti;
			break;
			
		case MSG_GUI_MIDDLE_LIST_REF_RANGES:
			newMiddleSection = middleListRefRanges;
			break;
			
		case MSG_GUI_MIDDLE_LIST_RESULTS:
			newMiddleSection = middleListResults;
			break;
			
		case MSG_GUI_MIDDLE_LIST_SETTINGS:
			newMiddleSection = middleListSettings;
			break;
			
		case MSG_GUI_MIDDLE_LOAD_ORDER:
			newMiddleSection = middleLoadOrder;
			break;
			
		case MSG_GUI_MIDDLE_MESSAGE:
			newMiddleSection = middleMessage;
			break;
			
		case MSG_GUI_MIDDLE_MESSAGE_INFO:
			newMiddleSection = middleMessageInfo;
			break;
		
		case MSG_GUI_MIDDLE_MULTI_INFO:
			newMiddleSection = middleMultiInfo;
			break;

		case MSG_GUI_MIDDLE_MULTI_SELECT:
			newMiddleSection = middleMultiSelect;
			break;

		case MSG_GUI_MIDDLE_OPTION_INFO:
			newMiddleSection = middleOptionInfo;
			break;

		case MSG_GUI_MIDDLE_PROG_BAR_MESSAGE:
			newMiddleSection = middleProgBarMessage;
			break;
			
		case MSG_GUI_MIDDLE_SELECT_OPTION:
			newMiddleSection = middleSelectOption;
			break;
			
		case MSG_GUI_MIDDLE_SELECT_TYPE:
			newMiddleSection = middleSelectType;
			break;
			
		case MSG_GUI_MIDDLE_VIEW_RESULTS:
			newMiddleSection = middleViewResults;
			break;

		default:
			newMiddleSection = 0;
			break;
	}
	
	// If we're currently showing a section, hide it.
	if ( middleSection != 0 )
	{
		middleSection->Hide();
	}
	
	// If there's a new section ...
	if ( newMiddleSection != 0 )
	{
		// Set its fields.
		newMiddleSection->SetData( msgData );
		
		// Show the new section.
		newMiddleSection->Show();
	}

	// Remember this section for next time.
	middleSection = newMiddleSection;
}

/*
 * FUNCTION
 * 	SetBottom
 * 
 * DESCRIPTION
 * 	Handles a message to set the bottom section of the screen.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the received message.
 * 	msgData - Message data for the received message.
 */

void
Controller::
SetBottom( int msgId, GuiReceiveMsgData_t * msgData )
{
	BottomSection * newBottomSection = 0;

	// Make sure there is no warning/error/critical stop window.
	if ( winSection != 0 )
	{
		winSection->Hide();
		winSection = 0;
	}
	
	// Determine the new bottom section.
	switch ( msgId )
	{
		case MSG_GUI_BOTTOM_NONE:
			newBottomSection = 0;
			break;
			
		case MSG_GUI_BOTTOM_ONE_BUTTON:
			newBottomSection = bottomButtons1;
			break;
			
		case MSG_GUI_BOTTOM_TWO_BUTTONS:
			newBottomSection = bottomButtons2;
			break;
			
		case MSG_GUI_BOTTOM_THREE_BUTTONS:
			newBottomSection = bottomButtons3;
			break;
			
		case MSG_GUI_BOTTOM_DATE_TIME:
			newBottomSection = bottomDateTime;
			break;
			
		default:
			newBottomSection = 0;
			break;
	}

	// If we're currently showing a section,hide it.
	if ( bottomSection != 0 )
	{
		bottomSection->Hide();
	}
	
	// If there's a new section ...
	if ( newBottomSection != 0 )
	{
		// Set its fields.
		newBottomSection->SetData( msgData );
		
		// Show the new section.
		newBottomSection->Show();
	}

	// Remember this section for next time.
	bottomSection = newBottomSection;
}

/*
 * FUNCTION
 * 	SetWin
 * 
 * DESCRIPTION
 * 	Handles a message to set a full-screen window.
 * 
 * PARAMETERS
 * 	msgId - Message ID for the received message.
 * 	msgData - Message data for the received message.
 */

void
Controller::
SetWin( int msgId, GuiReceiveMsgData_t * msgData )
{
	WinSection *	newWinSection = 0;
	Sound_t			sound = SOUND_NONE;
	
	// Make sure the regular top, middle and bottom are cleared.
	if ( topSection != 0 )
	{
		topSection->Hide();
		topSection = 0;
	}
	if ( middleSection != 0 )
	{
		middleSection->Hide();
		middleSection = 0;
	}
	if ( bottomSection != 0 )
	{
		bottomSection->Hide();
		bottomSection = 0;
	}
		
	// Determine the new window.
	switch ( msgId )
	{
		case MSG_GUI_WIN_NONE:
			newWinSection = 0;
			break;
			
		case MSG_GUI_WIN_WARNING1:
			newWinSection = winWarning1;
			sound = SOUND_WARNING;
			break;
		
		case MSG_GUI_WIN_WARNING2:
			newWinSection = winWarning2;
			sound = SOUND_WARNING;
			break;

		case MSG_GUI_WIN_ERROR:
			newWinSection = winError;
			sound = SOUND_ERROR;
			break;
			
		case MSG_GUI_WIN_CRITICAL_STOP:
			newWinSection = winCriticalStop;
			sound = SOUND_CRITICAL_STOP;
			break;
			
		case MSG_GUI_WIN_CRITICAL_STOP2:
			newWinSection = winCriticalStop2;
			sound = SOUND_CRITICAL_STOP;
			break;

		default:
			newWinSection = 0;
			break;
	}

	// If we're currently showing a section, hide it.
	if ( winSection != 0 )
	{
		winSection->Hide();
	}
	
	// If there's a new section ...
	if ( newWinSection != 0 )
	{
		// Set its fields.
		newWinSection->SetData( msgData );
		
		// Show the new section.
		newWinSection->Show();
	}

	if ( sound != SOUND_NONE )
	{
		PlaySound( sound );
	}
	
	// Remember this section for next time.
	winSection = newWinSection;
}

/*
 * FUNCTION
 * 	Update
 * 
 * DESCRIPTION
 * 	Handles a message to update a parameter currently displayed on the screen.
 * 
 * PARAMETERS
 * 	msg - Message received, which contains the update parameters.
 */

void
Controller::
Update( GuiReceiveMsg_t * msg )
{
	/*
	 * Determine which subscreen object contains the specified parameter, and
	 * pass the message on to the Update method for that object.
	 */
	switch ( msg->msgId )
	{
		case MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT:
		case MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS:
			middleAnalysisInProcess->Update( msg );
			break;
		
		case MSG_GUI_UPDATE_ENTER_DATE_LEFT:
		case MSG_GUI_UPDATE_ENTER_DATE_MIDDLE:
		case MSG_GUI_UPDATE_ENTER_DATE_RIGHT:
			middleEnterDate->Update( msg );
			break;

		case MSG_GUI_UPDATE_ENTER_ID:
			middleEnterId->Update( msg );
			break;

		case MSG_GUI_UPDATE_ENTER_IP:
			middleEnterIp->Update( msg );
			break;

		case MSG_GUI_UPDATE_ENTER_VALUE_NEW:
			middleEnterValue->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_HOME_WARMING_PROGRESS:
			middleHomeWarming->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS:
			middleHomeWarmingMfg->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_LIST:
			middleList->Update( msg );
			break;

		case MSG_GUI_UPDATE_LIST_MULTI:
			middleListMulti->Update( msg );
			break;

		case MSG_GUI_UPDATE_LIST_REF_RANGES:
			middleListRefRanges->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_LIST_RESULTS:
			middleListResults->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_LIST_SETTINGS:
			middleListSettings->Update( msg );
			break;
			
		case MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS:
		case MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT:
			middleProgBarMessage->Update( msg );
			break;
			
		default:
			break;
	}
}
			
/*
 * FUNCTION
 * 	Sound
 * 
 * DESCRIPTION
 * 	Handles a sound message.
 * 
 * PARAMETERS
 * 	msgId - Message ID indicating which sound is desired.
 */

void
Controller::
Sound( int msgId, GuiReceiveMsgData_t * msgData )
{
	switch ( msgId )
	{
		case MSG_GUI_SOUND_CRITICAL_STOP:
			PlaySound( SOUND_CRITICAL_STOP );
			break;
		case MSG_GUI_SOUND_ERROR:
			PlaySound( SOUND_ERROR );
			break;
		case MSG_GUI_SOUND_WARNING:
			PlaySound( SOUND_WARNING );
			break;
		case MSG_GUI_SOUND_PRINTING_DONE:
			SetStatusVolume( settingsData->statusVolume );
			PlaySound( SOUND_PRINTING_DONE );
			break;
		case MSG_GUI_SOUND_CLICK:
			SetClickVolume( settingsData->touchClickVolume );
			Click();
			break;
		case MSG_GUI_SOUND_SET_ALERT_VOLUME:
			SetAlertVolume( msgData->soundData.volume );
			break;
		case MSG_GUI_SOUND_SET_STATUS_VOLUME:
			SetStatusVolume( msgData->soundData.volume );
			break;
		case MSG_GUI_SOUND_SET_CLICK_VOLUME:
			SetClickVolume( msgData->soundData.volume );
			break;
	}
}

void
Controller::
ScreenSaver( int msgId )
{
	pthread_sleepon_lock();
	switch ( msgId )
	{
		case MSG_GUI_SCREENSAVE_WAKEUP:
			inactionTime_g = 0;
			break;
		case MSG_GUI_SCREENSAVE_DISABLE:
			screenSaveEnabled_g = false;
			inactionTime_g = 0;
			break;
		case MSG_GUI_SCREENSAVE_ENABLE:
			if ( !screenSaveEnabled_g )
			{
				screenSaveEnabled_g = true;
				inactionTime_g = 0;
			}
			break;
		case MSG_GUI_SCREENSAVE_BUTTON_PRESS:
			if ( screenSaveEnabled_g )
			{
				if ( screenSaveActive_g )
				{
					inactionTime_g = 0;
				}
				else
				{
					inactionTime_g = settingsData->screenSaverTimeout + 0.001;
				}
			}
			break;
	}
	pthread_sleepon_unlock();
}
		
