/*
 * FILENAME
 * 	SettingsStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine state classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <sys/neutrino.h>
#include <string.h>
#include <stdlib.h>

#include "SettingsStates.h"
#include "SettingsMessages.h"
#include "AnalysisMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "GetThumbKeyMode.h"

#define BACKDOOR_ID	"458-25-243"

static mqd_t		CreateQueue( const char * queueName );

mqd_t 				StateSettings::settingsQueue	= CreateQueue( SETTINGS_QUEUE_NAME );
FactoryData_t *		StateSettings::factoryData		= (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SettingsData_t *	StateSettings::settingsData		= (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
RefRangeData_t *	StateSettings::refRanges		= (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );
SystemData_t *		StateSettings::systemData		= (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );

const char *		StateSettings::infoText = "";
SystemErrorNumber_t	StateSettings::errNum = SERR_NONE;
struct tm			StateSettings::tm;
Expirys				StateSettings::expirys;
Expiry_t *			StateLogin::expiry;

void *
StateSettings::
TimerThread( void * arg )
{
	const int msg = MSG_SETTINGS_TIMER;

	static mqd_t queue = mq_open( SETTINGS_QUEUE_NAME, O_WRONLY );
	
	if ( queue == -1 )
	{
		LogError( "failed to open queue %s", SETTINGS_QUEUE_NAME );
		return NULL;
	}

	sleep( 1 );
	
	mq_send( queue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
	
	return 0;
}

static mqd_t
CreateQueue( const char * queueName )
{
	mqd_t queue = mq_open( queueName, O_WRONLY );
	
	if ( queue == -1 )
	{
		LogError( "failed to open queue %s", queueName );
		return 0;
	}

	return queue;
}

/*
 * Class StateBackground
 */

/*
 * FUNCTION
 * 	StateBackground::Enter
 * 
 * DESCRIPTION
 * 	Sets up the GUI and condition variables upon entrance to this state.
 * 
 * NOTE
 * 	This header applies to all Enter() methods for the following subclasses of
 * 	class State.
 */

void
StateBackground::
Enter()
{
	SendAnalysisMessage( MSG_ANALYSIS_GOTO_FOREGROUND );
}

/*
 * FUNCTION
 * 	StateBackground::ProcessMessage
 * 
 * DESCRIPTION
 * 	Handles a message received by this state.
 * 
 * PARAMETERS
 * 	msgId - ID for the received message.
 * 	data - Data field of the received message.
 * 
 * RETURNS
 * 	State to proceed to, depending on the result of the message processing.  If
 * 	a change in state isn't called for, the current state is returned so a
 * 	state change won't take place.
 *
 * NOTE
 * 	This header applies to all ProcessMessage() methods for the following subclasses of
 * 	class State.
 */

state_t
StateBackground::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_BACKGROUND;
	
	switch ( msgId )
	{
		case MSG_SETTINGS_GOTO_FOREGROUND:
		{
			GuiReceiveMsg_t msg;
	
			// Inform the GUI that we want input now.
			msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
			strcpy( msg.data.inputReceiver.name, SETTINGS_QUEUE_NAME );
			SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );

			// Change to the next state.
			if ( (settingsData->settingsLockoutEnabled == true) && !LockoutKeyInstalled() )
			{
				retVal = STATE_ENTER_OPERATOR_ID;
			}
			else
			{
				retVal = STATE_ICON_MATRIX_1;
			}
			stateStack.Push( STATE_BACKGROUND );
			break;
		}
		default: break;
	}
	
	return retVal;
}

/*
 * Class StateEnterId
 */

void
StateEnterId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_ENTER ], true );
}

state_t
StateEnterId::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ENTER_ID;
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
			else if ( idata == 1 )	// Enter, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Enter.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			expiry = expirys.GetAdmin( sdata );
			
			// Admin in the database, or the back-door ID.
			if ( (expiry != 0) || (strcmp( sdata, BACKDOOR_ID ) == 0) )
			{
				retVal = STATE_ENTER_PASSWORD;
				stateStack.Push( STATE_ENTER_ID );
			}

			// Admin not found.
			else
			{
				retVal = STATE_ERROR;
				errNum = SERR_ADMIN_NOT_FOUND;
				stateStack.Push( STATE_ENTER_ID );
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * class StateEnterOperatorId
 */
 
void
StateEnterOperatorId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_OPERATOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterOperatorId::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_OPERATOR_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	Expiry_t *		expiry;
	bool			idOk = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Back.
			{
				retVal = stateStack.Pop();
			}
			else if ( idata == 1 )	// Done, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( strcmp( sdata, LOCKOUT_BACKDOOR ) == 0 )
			{
				idOk = true;
			}
			else
			{
				expiry = expirys.GetOp( sdata );
	
				// Invalid operator ID.
				if ( expiry == 0 )
				{
					errNum = SERR_UNAUTHORIZED_OPERATOR;
					stateStack.Push( STATE_ENTER_OPERATOR_ID );
					retVal = STATE_ERROR;
				}
	
				// Operator ID expired.
				else if ( (expiry->expiration != 0) && (expiry->expiration < time( 0 )) )
				{
					errNum =  SERR_OPERATOR_RENEWAL_REQUIRED;
					retVal = STATE_ERROR;
				}
	
				// Operator ID OK.
				else
				{
					idOk = true;
				}
			}
			
			if ( idOk )
			{
				retVal = STATE_ICON_MATRIX_1;
			}
			break;
	}
	
	return retVal;
}

/*
 * Class StateEnterPassword
 */

void
StateEnterPassword::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_PASSWORD ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 1;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_ENTER ], true );
}

state_t
StateEnterPassword::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ENTER_PASSWORD;
	GuiReceiveMsg_t	msg;
	char *			sdata = (char *)data;
	int				idata = *(int *)data;
	const char *	password;
	const char *	backdoorPassword;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:	// Enter
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;				
				case 1:		// Enter
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Enter.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			// Use backdoor password (last four characters of instrument S/N), if that was the user ID,
			// otherwise the administrator's password.
			backdoorPassword = factoryData->instrumentSerialNumber + strlen( factoryData->instrumentSerialNumber ) - 4;
			password = (expiry == 0) ? backdoorPassword : expiry->password;

			// Correct password.
			if ( stricmp( sdata, password ) == 0 )
			{
				retVal = STATE_ICON_MATRIX_2;
				stateStack.Pop( STATE_ICON_MATRIX_1 );	// Next pop will bring us back to home.
			}

			// Incorrect password.
			else
			{
				GuiReceiveMsg_t msg;
				
				msg.msgId = MSG_GUI_UPDATE_ENTER_ID;
				msg.data.middleEnterId.isPassword = 1;
				strcpy( msg.data.middleEnterId.initialValue, "" );
				SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
				
				SendGuiMessage( MSG_GUI_SOUND_ERROR );
			}
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * Class StateIconMatrix
 */

state_t
StateIconMatrix::
SelectIcon( int index, state_t currentState )
{
	state_t retVal = currentState;
	
	stateStack.Push( currentState );
	switch ( index )
	{
		case ICON_ADVANCED:				retVal = STATE_ADVANCED_SELECT;				break;
		case ICON_ARCHIVE_REF_RANGES:	retVal = STATE_ARCHIVE_SELECT;				break;
		case ICON_COMM:					retVal = STATE_COMM_SET_PROTOCOL;			break;
		case ICON_DATE_TIME:			retVal = STATE_DATETIME_SET_TIME;			break;
		case ICON_FACTORY:				retVal = STATE_FACTORY_DEFAULTS;			break;
		case ICON_LANGUAGE:				retVal = STATE_LANGUAGE_SET_DISPLAY;		break;
		case ICON_PRINTER:				retVal = STATE_PRINTER_SELECT;				break;
		case ICON_REF_RANGES:			retVal = STATE_REFRANGES_SELECT;			break;
		case ICON_SCREEN:				retVal = STATE_DISPLAY_SELECT;				break;
		case ICON_SECURITY:				retVal = STATE_SECURITY_SELECT;				break;
		case ICON_SOUND:				retVal = STATE_SOUND_SELECT;				break;
		case ICON_UPDATE:				retVal = STATE_CDUPDATE_LOAD_SOFTWARE_CD;	break;
		case ICON_UNITS:				retVal = STATE_UNITS_SELECT;				break;
		case ICON_WHO:					retVal = STATE_ANALYZERINFO_VIEW;			break;
		default:
			LogWarning( "unknown icon %d", index );
			stateStack.Pop();
			break;
	}
	
	return retVal;
}

/*
 * Class StateIconMatrix1
 */

void
StateIconMatrix1::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_ANALYZER_SETTINGS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ICON_MATRIX_1;
	
	SendGuiMessage( &msg, 0 );

	SetBottomButtons( strings[ BSTR_HOME ], true );
}

state_t
StateIconMatrix1::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ICON_MATRIX_1;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_ICON_MATRIX:
			retVal = SelectIcon( idata, STATE_ICON_MATRIX_1 );
			break;
		
		case MSG_GUI_OUT_NEXT_SETTINGS:
			if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
				 (expirys.NumAdmins() > 0) &&
				 !systemData->inAdministratorMode )
			{
				retVal = STATE_ENTER_ID;
				stateStack.Push( STATE_ICON_MATRIX_1 );
			}
			else
			{
				retVal = STATE_ICON_MATRIX_2;
			}
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = stateStack.Home();
			break;
	}
	
	return retVal;
}

/*
 * Class StateIconMatrix2
 */
 
void
StateIconMatrix2::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_ANALYZER_SETTINGS ] );

	msg.msgId = MSG_GUI_MIDDLE_ICON_MATRIX_2;

	SetBottomButtons( strings[ BSTR_HOME ], true );
	
	SendGuiMessage( &msg, 0 );
}

state_t
StateIconMatrix2::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_ICON_MATRIX_2;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_ICON_MATRIX:
			retVal = SelectIcon( idata, STATE_ICON_MATRIX_2 );
			break;
		
		case MSG_GUI_OUT_PREVIOUS_SETTINGS:
			retVal = STATE_ICON_MATRIX_1;
			break;
			
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = stateStack.Home();
			break;
	}
	
	return retVal;
}


/*
 * Class StateInfo
 */

void
StateInfo::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_INFORMATION ] );
	
	msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
	strcpy( msg.data.middleViewResults.text, infoText );
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], true );
}

state_t
StateInfo::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_INFO;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = stateStack.Pop();
			break;

		default:
			break;
	}
	
	return retVal;
}

/*
 * Class StateError
 */

void
StateError::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	StringId_t			msgStr;
	const char *		topText;
	
	// Get the message text.
	msgStr = systemError.GetScreenText( errNum );

	if ( systemError.GetAction( errNum ) == SERR_ACTION_WARNING )
	{
		guiMsg.msgId = MSG_GUI_WIN_WARNING1;
		topText = strings[ TSTR_WARNING ];
	}
	else
	{
		guiMsg.msgId = MSG_GUI_WIN_ERROR;
		topText = strings[ TSTR_ERROR ];
	}

	strcpy( guiMsg.data.winError.topLabel, topText );

	// Add the error number to the beginning of the error message.
	sprintf( guiMsg.data.winError.message, "%x\n%s", errNum, strings[ msgStr ] );

	strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateError::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_ERROR;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = stateStack.Pop();
			errNum = SERR_NONE;
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( errNum ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ERROR );
			break;
				
		default:
			break;
	}
	
	return retVal;
}

/*
 * StateInvalidDate
 */

void
StateInvalidDate::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiWinWarningData_t *	data = &msg.data.winWarning;

	msg.msgId = MSG_GUI_WIN_WARNING1;
	
	strcpy( data->topLabel, strings[ TSTR_WARNING ] );
	sprintf( data->message, " %d %s %d %s ", tm.tm_mday,
							strings.Month( tm.tm_mon ), tm.tm_year + 1900,
							strings[ MSTR_IS_NOT_VALID ] );
	strcpy( data->buttonText[0], strings[ BSTR_CONTINUE ] );

	SendGuiMessage( &msg, sizeof( GuiWinWarningData_t ) );
}

state_t
StateInvalidDate::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_INVALID_DATE;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = stateStack.Pop();
			break;
		case MSG_GUI_OUT_INFO:
			infoText = strings[ ISTR_DATE_INVALID ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_INVALID_DATE );
			break;
		default:
			break;
	}

	return retVal;
}
