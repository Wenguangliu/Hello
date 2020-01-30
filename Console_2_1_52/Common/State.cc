/*
 * FILENAME
 * 	State.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the State class.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <sys/neutrino.h>
#include <string.h>

#include "State.h"
#include "Shmem.h"
#include "SettingsData.h"
#include "LogErr.h"

mqd_t				State::analysisQueue = 0;
mqd_t				State::astmQueue = 0;
mqd_t				State::guiQueue = 0;
mqd_t				State::recallQueue = 0;
mqd_t				State::settingsQueue = 0;
mqd_t				State::engineCommQueue = 0;
mqd_t				State::calcResultsQueue = 0;
mqd_t				State::heaterControlQueue = 0;
mqd_t				State::serialCommQueue = 0;
mqd_t				State::startupQueue = 0;
SettingsData_t *	State::settings = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
StateStack			State::stateStack;
SystemError			State::systemError;
Analytes			State::analytes;
//AStrings			State::strings;
AStrings			strings;

/*
 * StateStack
 */

/*
 * FUNCTION
 * 	StateStack::Push
 * 
 * DESCRIPTION
 * 	Pushes a state onto the state stack.
 * 
 * PARAMETERS
 * 	state - State to push onto the stack.
 */

void
StateStack::
Push( state_t state )
{
	if ( top < (MAX_STATESTACK_SIZE - 1) )
	{
		stack[ ++top ] = state;
	}
	else
	{
		LogError( "StateStack overflow, state = %d", (int)state );
	}
}

/*
 * FUNCTION
 * 	StateStack::Pop
 * 
 * DESCRIPTION
 * 	Pops a state from the state stack.
 * 
 * RETURNS
 * 	The top state on the stack, or -1 if the stack is empty.
 */

state_t
StateStack::
Pop()
{
	if ( top >= 0 )
	{
		return stack[ top-- ];
	}
	else
	{
		LogError( "StateStack underflow" );
		return -1;
	}
}

/*
 * FUNCTION
 * 	StateStack::Pop
 * 
 * DESCRIPTION
 * 	Pops all the states off the state stack until the given state is reached.
 * 	That state is also popped and then returned.
 * 	NOTE: If the given state is not found, the stack is cleared, but the given
 * 		  state is still returned.
 * 
 * PARAMETERS
 * 	state - State to pop down to.
 * 
 * RETURNS
 * 	The given state.
 */

state_t
StateStack::
Pop( state_t state )
{
	state_t	pop;
	
	do
	{
		pop = Pop();
	} while ( pop != state && pop != -1 );
	
	return state;
}

/*
 * class State
 */

/*
 * FUNCTION
 * 	State::InitMessageQueue
 * 
 * DESCRIPTION
 * 	Initializes a message queue.
 * 
 * PARAMETERS
 * 	queueName - Name of the queue.
 */

mqd_t
State::
InitMessageQueue( const char * queueName )
{
	mqd_t queue;

	queue = mq_open( queueName, O_WRONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open %s queue.", queueName );
		queue = 0;
	}
	
	return queue;
}

/*
 * FUNCTION
 * 	State::InitGuiComm
 * 
 * DESCRIPTION
 * 	Initializes return communication with the GUI.  Sends it the name of the
 * 	current process' queue, so the GUI knows where to send button presses, etc.
 *
 * PARAMETERS
 * 	queueName - Name of the current process's queue.
 */

void
State::
InitGuiComm( const char * queueName )
{
	GuiReceiveMsg_t msg;
	
	msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
	strcpy( msg.data.inputReceiver.name, queueName );

	SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );		
}

/*
 * FUNCTION
 * 	InitEngineComm
 * 
 * DESCRIPTION
 * 	Initializes return communication with the EngineComm.  Sends it the name of
 *	the	current process' queue, so the EngineComm knows where to send
 *  messages received from the Engine, analysis status messages, etc.
 *
 * PARAMETERS
 * 	queueName - Name of the current process's queue.
 */

void
State::
InitEngineComm( const char * queueName )
{
	EngineCommReceiveMsg_t msg;

	msg.msgId = MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER;
	strcpy( msg.data.messageReceiver.name, queueName );
	
	SendEngineCommMessage( &msg, sizeof( EngineCommMessageReceiverData_t ) );
}

/*
 * FUNCTION
 * 	State::SendAnalysisMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the Analysis process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Analysis.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendAnalysisMessage( AnalysisMsg_t * msg, int msgDataLen )
{
	if ( analysisQueue == 0 )
	{
		analysisQueue = InitMessageQueue( ANALYSIS_QUEUE_NAME );
	}
	
	if ( analysisQueue != 0 )
	{
		mq_send( analysisQueue, (const char *)msg, msgDataLen + sizeof( AnalysisMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendAnalysisMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the Analysis process.
 * 
 * PARAMETERS
 * 	msgId - Message ID to send to Analysis.
 */

void
State::
SendAnalysisMessage( AnalysisMsgId_t msgId )
{
	if ( analysisQueue == 0 )
	{
		analysisQueue = InitMessageQueue( ANALYSIS_QUEUE_NAME );
	}
	
	if ( analysisQueue != 0 )
	{
		mq_send( analysisQueue, (const char *)&msgId, sizeof( AnalysisMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendGuiMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the GUI process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the GUI.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendGuiMessage( GuiReceiveMsg_t * msg, int msgDataLen )
{
	if ( guiQueue == 0 )
	{
		guiQueue = InitMessageQueue( GUI_QUEUE_NAME );
	}
	
	if ( guiQueue != 0 )
	{
		mq_send( guiQueue, (const char *)msg, msgDataLen + sizeof( GuiMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendGuiMessage( GuiMsgId_t msgId )
{
	if ( guiQueue == 0 )
	{
		guiQueue = InitMessageQueue( GUI_QUEUE_NAME );
	}
	
	if ( guiQueue != 0 )
	{
		mq_send( guiQueue, (const char *)&msgId, sizeof( GuiMsgId_t ), MQ_PRIO_MAX-1 );
	}
}
/*
 * FUNCTION
 * 	State::SendRecallMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the Recall process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Recall process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendRecallMessage( RecallMsg_t * msg, int msgDataLen )
{
	if ( recallQueue == 0 )
	{
		recallQueue = InitMessageQueue( RECALL_QUEUE_NAME );
	}
	
	if ( recallQueue != 0 )
	{
		mq_send( recallQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendRecallMessage( RecallMsgId_t msgId )
{
	if ( recallQueue == 0 )
	{
		recallQueue = InitMessageQueue( RECALL_QUEUE_NAME );
	}
	
	if ( recallQueue != 0 )
	{
		mq_send( recallQueue, (const char *)&msgId, sizeof( RecallMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendSettingsMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the Settings process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Settings process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendSettingsMessage( SettingsMsg_t * msg, int msgDataLen )
{
	if ( settingsQueue == 0 )
	{
		settingsQueue = InitMessageQueue( SETTINGS_QUEUE_NAME );
	}
	
	if ( settingsQueue != 0 )
	{
		mq_send( settingsQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendSettingsMessage( SettingsMsgId_t msgId )
{
	if ( settingsQueue == 0 )
	{
		settingsQueue = InitMessageQueue( SETTINGS_QUEUE_NAME );
	}
	
	if ( settingsQueue != 0 )
	{
		mq_send( settingsQueue, (const char *)&msgId, sizeof( SettingsMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendEngineCommMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the EngineComm process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the EngineComm process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendEngineCommMessage( EngineCommReceiveMsg_t * msg, int msgDataLen )
{
	if ( engineCommQueue == 0 )
	{
		engineCommQueue = InitMessageQueue( ENGINE_COMM_QUEUE_NAME );
	}
	
	if ( engineCommQueue != 0 )
	{
		mq_send( engineCommQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendEngineCommMessage( EngineCommMsgId_t msgId )
{
	if ( engineCommQueue == 0 )
	{
		engineCommQueue = InitMessageQueue( ENGINE_COMM_QUEUE_NAME );
	}
	
	if ( engineCommQueue != 0 )
	{
		mq_send( engineCommQueue, (const char *)&msgId, sizeof( EngineCommMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendCalcResultsMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the CalcResults process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the CalcResults process.
 */

void
State::
SendCalcResultsMessage( CalcResultsMsgId_t msgId )
{
	if ( calcResultsQueue == 0 )
	{
		calcResultsQueue = InitMessageQueue( CALC_RESULTS_QUEUE_NAME );
	}
	
	if ( calcResultsQueue != 0 )
	{
		mq_send( calcResultsQueue, (const char *)&msgId, sizeof( CalcResultsMsgId_t ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendCalcResultsMessage( CalcResultsMsgId_t msgId, CalcResultsMsgData_t * data )
{
	CalcResultsMsg_t	msg;
	
	if ( calcResultsQueue == 0 )
	{
		calcResultsQueue = InitMessageQueue( CALC_RESULTS_QUEUE_NAME );
	}
	
	if ( calcResultsQueue != 0 )
	{
		msg.msgId = msgId;
		msg.data = *data;
		mq_send( calcResultsQueue, (const char *)&msg, sizeof( CalcResultsMsg_t ), MQ_PRIO_MAX-1 );
	}
}

/*
 * FUNCTION
 * 	State::SendHeaterControlMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the HeaterControl process.
 * 
 * PARAMETERS
 * 	msgId - Message ID to send to the HeaterControl process.
 */

void
State::
SendHeaterControlMessage( HeaterControlMsgId_t msgId )
{
	if ( heaterControlQueue == 0 )
	{
		heaterControlQueue = InitMessageQueue( HEATER_CONTROL_QUEUE_NAME );
	}
	
	if ( heaterControlQueue != 0 )
	{
		mq_send( heaterControlQueue, (const char *)&msgId, sizeof( HeaterControlMsgId_t ), MQ_PRIO_MAX-1 );
	}
} 

/*
 * FUNCTION
 * 	State::SendStartupMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the Startup process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Startup process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendStartupMessage( StartupMsg_t * msg, int msgDataLen )
{
	if ( startupQueue == 0 )
	{
		startupQueue = InitMessageQueue( STARTUP_QUEUE_NAME );
	}
	
	if ( startupQueue != 0 )
	{
		mq_send( startupQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendStartupMessage( StartupMsgId_t msgId )
{
	if ( startupQueue == 0 )
	{
		startupQueue = InitMessageQueue( STARTUP_QUEUE_NAME );
	}
	
	if ( startupQueue != 0 )
	{
		mq_send( startupQueue, (const char *)&msgId, sizeof( StartupMsgId_t ), MQ_PRIO_MAX-1 );
	}
} 

/*
 * FUNCTION
 * 	State::SendSerialCommMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the SerialComm process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the SerialComm process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendSerialCommMessage( SerialCommMsg_t * msg, int msgDataLen )
{
	if ( serialCommQueue == 0 )
	{
		serialCommQueue = InitMessageQueue( SERIAL_COMM_QUEUE_NAME );
	}
	
	if ( serialCommQueue != 0 )
	{
		mq_send( serialCommQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendSerialCommMessage( SerialCommMsgId_t msgId )
{
	if ( serialCommQueue == 0 )
	{
		serialCommQueue = InitMessageQueue( SERIAL_COMM_QUEUE_NAME );
	}
	
	if ( serialCommQueue != 0 )
	{
		mq_send( serialCommQueue, (const char *)&msgId, sizeof( SerialCommMsgId_t ), MQ_PRIO_MAX-1 );
	}
}



/*
 * FUNCTION
 * 	State::SendAstmMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the ASTM process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the ASTM process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendAstmMessage( AstmMsg_t * msg, int msgDataLen )
{
	if ( astmQueue == 0 )
	{
		astmQueue = InitMessageQueue( ASTM_QUEUE_NAME );
	}
	
	if ( astmQueue != 0 )
	{
		mq_send( astmQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
	}
}

void
State::
SendAstmMessage( AstmMsgId_t msgId )
{
	if ( astmQueue == 0 )
	{
		astmQueue = InitMessageQueue( ASTM_QUEUE_NAME );
	}
	
	if ( astmQueue != 0 )
	{
		mq_send( astmQueue, (const char *)&msgId, sizeof( SerialCommMsgId_t ), MQ_PRIO_MAX-1 );
	}
}


/*
 * FUNCTION
 * 	State::SetTopLabel
 * 
 * DESCRIPTION
 * 	Sets the top label in the GUI to the specified text.
 * 
 * PARAMETERS
 * 	label - Label text.
 */

void
State::
SetTopLabel( const char * label )
{
	GuiReceiveMsg_t msg;
	int				dataLen = 0;
	
	if ( label != 0 )
	{
		msg.msgId = MSG_GUI_TOP_LABEL;
		strncpy( msg.data.topLabel.label, label, sizeof( msg.data.topLabel.label ) );
		dataLen = sizeof( GuiTopLabelData_t );
	}
	else
	{
		msg.msgId = MSG_GUI_TOP_NONE;
		dataLen = 0;
	}
				
	SendGuiMessage( &msg, dataLen );
}

/*
 * FUNCTION
 * 	State::SetBottomButtons
 * 
 * DESCRIPTION
 * 	Sets the bottom button window to the correct number of buttons, with the
 * 	labels and selected status given.
 * 
 * PARAMETERS
 * 	label0 - Button 0 label text.
 * 	selected0 - Button 0 selected state
 * 	label1 - Button 1 label text.
 * 	selected1 - Button 1 selected state
 * 	label2 - Button 2 label text.
 * 	selected2 - Button 2 selected state
 */

void
State::
SetBottomButtons( const char * label0, bool selected0,
				  const char * label1, bool selected1,
				  const char * label2, bool selected2 )
{
	GuiReceiveMsg_t msg;
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	if ( label0 != 0 )
	{
		strncpy( msg.data.bottomButtons.buttonText[0], label0, sizeof( msg.data.bottomButtons.buttonText[0] ) );
		msg.data.bottomButtons.buttonHighlighted[0] = selected0;
		msg.msgId = MSG_GUI_BOTTOM_ONE_BUTTON;

		if ( label1 != 0 )
		{
			strncpy( msg.data.bottomButtons.buttonText[1], label1, sizeof( msg.data.bottomButtons.buttonText[1] ) );
			msg.data.bottomButtons.buttonHighlighted[1] = selected1;
			msg.msgId = MSG_GUI_BOTTOM_TWO_BUTTONS;
			
			if ( label2 != 0 )
			{
				strncpy( msg.data.bottomButtons.buttonText[2], label2, sizeof( msg.data.bottomButtons.buttonText[2] ) );
				msg.data.bottomButtons.buttonHighlighted[2] = selected2;
				msg.msgId = MSG_GUI_BOTTOM_THREE_BUTTONS;
			}
		}
	}
	
	SendGuiMessage( &msg, sizeof( GuiBottomButtonsData_t ) );
	
}

/*
 * FUNCTION
 * 	State::SetBottomDateTime
 * 
 * DESCRIPTION
 * 	Sets the bottom section of the GUI to Date/Time.
 */

void
State::
SetBottomDateTime()
{
	SendGuiMessage( MSG_GUI_BOTTOM_DATE_TIME );
}

void
State::
SetBottomDateTimeStrings()
{
	GuiReceiveMsg_t 		msg;
	int						i;

	msg.msgId = MSG_GUI_SET_DATE_TIME_STRINGS;
	msg.data.bottomDateTime.twentyFourHourFormat = settings->time24Format;
	for ( i = 0; i < 7; i++ )
	{
		strcpy( msg.data.bottomDateTime.daysOfWeek[ i ], strings.DayOfWeek( i ) );
	}
	for ( i = 0; i < 12; i++ )
	{
		strcpy( msg.data.bottomDateTime.months[ i ], strings.Month( i ) );
	}
				
	SendGuiMessage( &msg, sizeof( GuiBottomDateTimeData_t ) );
}
