/*
 * FILENAME
 * 	DateTimeStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine CD ROM update
 * 	states classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <sys/neutrino.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <process.h>

#include "CdUpdateStates.h"
#include "SettingsMessages.h"
#include "LogErr.h"
#include "ResultsDb.h"

#define SOFTWARE_UPDATER	"/cd0/SoftwareUpdater"
#define CONTROLS_UPDATER	"/cd0/ControlsUpdater"
#define DBASE_OK_FILE		"/cd0/dbase.dat"
#define RUNNER				"/tmp/Updater"

typedef enum
{
	ERR_UPDATE_OPEN_SOURCE = 1,
	ERR_UPDATE_OPEN_DESTINATION,
	ERR_UPDATE_SET_PERMS,
	ERR_UPDATE_EXECUTE,
	ERR_UPDATE_WAIT
} UpdateErr_t;

void *
StateCdUpdate::
UpdateSoftwareThread( void * arg )
{
	int 			retVal;
	FILE *			ifp;
	FILE *			ofp;
	char			buff[ 4096 ];
	int				count;
	SettingsMsg_t	msg;
	
	// Mount the CD.
//	mount( "/dev/cd0", "/cd0", 0, "cd", 0, 0 );
	
	ifp = fopen( SOFTWARE_UPDATER, "r" );
	if ( ifp == 0 )
	{
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_SOURCE;
		SendSettingsMessage( &msg, sizeof( int ) );
		
		return 0;
	}

	// Create updater on QNX file system.
	ofp = fopen( RUNNER, "w" );
	if ( ofp == 0 )
	{
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_OPEN_DESTINATION;
		SendSettingsMessage( &msg, sizeof( int ) );

		return 0;
	}
		
	// Copy the updater to the QNX file systerm.	
	while ( (count = fread( buff, 1, sizeof( buff ), ifp )) > 0 )
	{
		fwrite( buff, 1, count, ofp );
	}
	fclose( ifp );
	fclose( ofp );
		
	// Set the updater to read/write/execute by owner.
	retVal = chmod( RUNNER, S_IRWXU );
	if ( retVal == -1 )
	{
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = 	ERR_UPDATE_SET_PERMS;
		SendSettingsMessage( &msg, sizeof( int ) );

		return 0;
	}

	// Run the updater.
	retVal = spawnlp( P_WAIT, RUNNER, RUNNER,  0 );
	if ( retVal == -1 )
	{
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_EXECUTE;
		SendSettingsMessage( &msg, sizeof( int ) );

		return 0;
	}

	if ( (WIFEXITED( retVal ) == 0) || (WEXITSTATUS( retVal ) == 99) )
	{
		msg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		msg.data = ERR_UPDATE_WAIT;
		SendSettingsMessage( &msg, sizeof( int ) );

		return 0;
	}

	SendSettingsMessage( MSG_SETTINGS_CDUPDATE_DONE );
	
	return 0;
}


// GLD
// This state is skipped until we get to the point where we want to support
// loading controls this way.
/*
 * class StateCdUpdateSelect
 */
 
void
StateCdUpdateSelect::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleButtonsInfoData_t *	data = &msg.data.middleButtonsInfo;

	SetTopLabel( strings[ TSTR_CDROM_UPDATE ] );

	msg.msgId = MSG_GUI_MIDDLE_BUTTONS_INFO;

	memset( data, 0, sizeof( GuiMiddleButtonsInfoData_t ) );
	strcpy( data->buttonText[0], strings[ MSTR_UPDATE_SOFTWARE ] );
	strcpy( data->buttonText[1], strings[ MSTR_ADD_CONTROL_VALUES ] );
	data->buttonHighlighted[0] = true;
	
	SendGuiMessage( &msg, sizeof( GuiMiddleButtonsInfoData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_HOME ], true );
}

state_t
StateCdUpdateSelect::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_SELECT;
	int		idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata == 0 )	// Update Software
			{
				retVal = STATE_CDUPDATE_LOAD_SOFTWARE_CD;
			}
			else				// Add Control Values
			{
				retVal = STATE_CDUPDATE_LOAD_CONTROLS_CD;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )	// Back
			{
				retVal = stateStack.Pop();
			}
			else				// Home
			{
				retVal = stateStack.Home();
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = "CD Update Select";
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_SELECT );
			break;		
		default:
			break;
	}	
	return retVal;
}

/*
 * class StateCdUpdateLoadSoftwareCd
 */
 
void
StateCdUpdateLoadSoftwareCd::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleMessageData_t *	data = &msg.data.middleMessage;

	SetTopLabel( strings[ TSTR_LOAD_CD ] );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
	strcpy( data->message, strings[ MSTR_INSERT_CD ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CONTINUE ], true );
}

state_t
StateCdUpdateLoadSoftwareCd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_LOAD_SOFTWARE_CD;
	int		idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Continue
					retVal = STATE_CDUPDATE_READING_SOFTWARE_CD;
					break;
			}
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_CD_UPDATE ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_LOAD_SOFTWARE_CD );
			break;
		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateCdUpdateLoadControlsCd
 */
/* 
void
StateCdUpdateLoadControlsCd::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleMessageData_t *	data = &msg.data.middleMessage;

	SetTopLabel( strings[ TSTR_LOAD_CONTROLS_CD ] );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
	strcpy( data->message, strings[ MSTR_INSERT_CD ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons( strings[ BSTR_CONTINUE ], true );
}

state_t
StateCdUpdateLoadControlsCd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_LOAD_CONTROLS_CD;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = STATE_CDUPDATE_UPDATE_CONTROLS;
			break;
		case MSG_GUI_OUT_INFO:
			infoText = "Controls Update";
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_LOAD_CONTROLS_CD );
			break;
		default:
			break;
	}
	
	return retVal;
}
*/

/*
 * class StateCdUpdateReadingSoftwareCd
 */
 
void
StateCdUpdateReadingSoftwareCd::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	FILE *			fp;
	SettingsMsg_t	settingsMsg;
	char			buff[1024];
	char *			s;
	
	SetTopLabel( 0 );

	guiMsg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( guiMsg.data.middleMessage.message, strings[ MSTR_READING_CD ] );
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleMessageData_t ) );
	
	guiMsg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &guiMsg, 0 );

	// Mount the CD.
	mount( "/dev/cd0", "/cd0", 0, "cd", 0, 0 );
	
	// See if the updater file is on the CD.  If not, trigger an error message.
	fp = fopen( SOFTWARE_UPDATER, "r" );
	if ( fp == 0 )
	{
		settingsMsg.msgId = MSG_SETTINGS_CDUPDATE_ERROR;
		settingsMsg.data = ERR_UPDATE_OPEN_SOURCE;
		SendSettingsMessage( &settingsMsg, sizeof( int ) );
		
		return;
	}
	fclose( fp );

	// See if the database version conversion info file is on the CD.  If not,
	// trigger a warning message.
	fp = fopen( DBASE_OK_FILE, "r" );
	if ( fp == 0 )
	{
		SendSettingsMessage( MSG_SETTINGS_CDUPDATE_WARNING );
		
		return;
	}


	// Read the file and check to see if the current version of the database is
	// in there.  If it is, trigger a move on to the update state.
	fread( buff, sizeof( char ), 1024, fp );
	s = strtok( buff, "," );
	while ( s != 0 )
	{
		int version = atoi( s );
		if ( version == RESULTS_DB_VERSION )
		{
			SendSettingsMessage( MSG_SETTINGS_CDUPDATE_OK );
			fclose( fp );
			return;
		}
		
		s = strtok( 0, "," );
	}

	// No converter for the current database version, so trigger a warning
	// message.
	SendSettingsMessage( MSG_SETTINGS_CDUPDATE_WARNING );
	fclose( fp );
}

state_t
StateCdUpdateReadingSoftwareCd::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_READING_SOFTWARE_CD;

	switch ( msgId )
	{
		case MSG_SETTINGS_CDUPDATE_ERROR:
			LogError( "CdUpdate error #%d", *(int *)data );
			retVal = STATE_CDUPDATE_ERROR;
			break;
		
		case MSG_SETTINGS_CDUPDATE_WARNING:
			retVal = STATE_CDUPDATE_WARNING;
			break;
		
		case MSG_SETTINGS_CDUPDATE_OK:
			retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
			break;
	}
	
	return retVal;
}

/*
 * class StateCdUpdateUpdateSoftware
 */
 
void
StateCdUpdateUpdateSoftware::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel( strings[ TSTR_SOFTWARE_UPDATE ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
	strcpy( data->progBarText, strings[ MSTR_UPDATING_SOFTWARE ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;
	data->fontColor = 0x004E4E5E;
	
	strcpy( data->message, strings[ MSTR_PLEASE_WAIT_CDUPDATE ] );
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons();

	progBarValue = 0;
	done = false;

	// Timer thread for progress bar updates.
	ThreadCreate( 0, TimerThread, 0, 0 );

	// Update thread to do the actual work.
	ThreadCreate( 0, UpdateSoftwareThread, 0, 0 );
}

state_t
StateCdUpdateUpdateSoftware::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
	
	switch ( msgId )
	{
		case MSG_SETTINGS_TIMER:
			if ( !done )
			{
				progBarValue += 10;
				if ( progBarValue > 100 )
				{
					progBarValue = 0;
				}
				GuiReceiveMsg_t msg;
				msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
				msg.data.middleProgBarMessage.progBarValue = progBarValue;
				SendGuiMessage( &msg, sizeof( GuiMiddleProgBarMessageData_t ) );
				ThreadCreate( 0, TimerThread, 0, 0 );
			}
			else
			{
				stateStack.Home();
			}
			break;
		case MSG_SETTINGS_CDUPDATE_ERROR:
			LogError( "CdUpdate error #%d", *(int *)data );
			retVal = STATE_CDUPDATE_ERROR;
			break;
		case MSG_SETTINGS_CDUPDATE_DONE:
			// We ignore this message because we just want to stay in the
			// moving prog bar mode until the update resets the system.
			break;
	}

	return retVal;
}

/*
 * class StateCdUpdateUpdateControls
 */
/* 
void
StateCdUpdateUpdateControls::
Enter()
{
	GuiReceiveMsg_t					msg;
	GuiMiddleProgBarMessageData_t *	data = &msg.data.middleProgBarMessage;

	SetTopLabel( strings[ TSTR_CONTROL_UPDATE ] );

	msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
	strcpy( data->progBarText, strings[ MSTR_UPDATING_CONTROLS ] );
	data->progBarValue = 0;
// GLD
// Need #defines
	data->backgroundColor = 0xFFFFFFFF;
	data->fontColor = 0x004E4E5E;
	data->message[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );

	SetBottomButtons();

	progBarValue = 0;
	done = false;

	// Timer for the progress bar.
	ThreadCreate( 0, TimerThread, NULL, NULL );
}

state_t
StateCdUpdateUpdateControls::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_CDUPDATE_UPDATE_CONTROLS;

// GLD
// Except for the above line, this code is identical to that for UpdateSoftware.
	switch ( msgId )
	{
		case MSG_SETTINGS_TIMER:
			if ( !done )
			{
				progBarValue += 10;
				if ( progBarValue > 100 )
				{
					progBarValue = 0;
				}
				GuiReceiveMsg_t msg;
				msg.msgId = MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
				msg.data.middleProgBarMessage.progBarValue = progBarValue;
				SendGuiMessage( &msg, sizeof( GuiMiddleProgBarMessageData_t ) );
				ThreadCreate( 0, TimerThread, NULL, NULL );
			}
			else
			{
				stateStack.Home();
			}
			break;
		case MSG_SETTINGS_CDUPDATE_ERROR:
			LogError( "CdUpdate error #%d", *(int *)data );
			retVal = STATE_CDUPDATE_ERROR;
			break;
		case MSG_SETTINGS_CDUPDATE_DONE:
			retVal = stateStack.Home();
			break;
	}
	
	return retVal;
}
*/

/*
 * StateCdUpdateError
 */

void
StateCdUpdateError::
Enter()
{
	GuiReceiveMsg_t	guiMsg;

	guiMsg.msgId = MSG_GUI_WIN_ERROR;
	strcpy( guiMsg.data.winError.topLabel, strings[ TSTR_ERROR ] );

	// Add the error number to the beginning of the error message.
	sprintf( guiMsg.data.winError.message, "%x\n%s", SERR_CD_UPDATE, strings[ MSTR_ERROR_READING_CD ] );

	strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateCdUpdateError::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_CDUPDATE_ERROR;
	SystemError	systemError;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( SERR_CD_UPDATE ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_ERROR );
			break;
			
		default:
			break;
	}
	
	return retVal;
}

/*
 * StateCdUpdateWarning
 */

void
StateCdUpdateWarning::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	
	guiMsg.msgId = MSG_GUI_WIN_WARNING2;
	strcpy( guiMsg.data.winWarning.topLabel, strings[ TSTR_WARNING ] );
	strcpy( guiMsg.data.winWarning.message, strings[ ESTR_DATA_LOSS ] );
	strcpy( guiMsg.data.winWarning.buttonText[0], strings[ BSTR_CONTINUE ] );
	strcpy( guiMsg.data.winWarning.buttonText[1], strings[ BSTR_CANCEL ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateCdUpdateWarning::
ProcessMessage( int msgId, char * data )
{
	state_t 	retVal = STATE_CDUPDATE_WARNING;
	int			idata = *(int *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_INFO:
			infoText = strings[ EISTR_DATA_LOSS ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_CDUPDATE_WARNING );
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Continue
					retVal = STATE_CDUPDATE_UPDATE_SOFTWARE;
					break;
				case 1:		// Cancel
					retVal = stateStack.Pop();
					break;
			}
			break;

		default:
			break;
	}

	return retVal;
}

