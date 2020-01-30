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
 
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "State.h"
#include "Shmem.h"
//#include "SettingsData.h"
#include "LogErr.h"

HANDLE				State::analysisQueue = 0;
HANDLE				State::astmQueue = 0;
HANDLE				State::hl7Queue = 0;
HANDLE				State::guiQueue = 0;
HANDLE				State::recallQueue = 0;
HANDLE				State::settingsQueue = 0;
HANDLE				State::engineCommQueue = 0;
HANDLE				State::calcResultsQueue = 0;
HANDLE				State::heaterControlQueue = 0;
HANDLE				State::serialCommQueue = 0;
HANDLE				State::startupQueue = 0;
HANDLE              State::printerQueue = 0;
HANDLE              State::printerRespQueue = 0;
HANDLE              State::manufactureQueue = 0;
HANDLE              State::errorProcessQueue = 0;

SettingsData_t *	State::settings = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
StateStack			State::stateStack;
SystemError			State::systemError;
Analytes			State::analytes;
//AStrings			State::strings;
//AStrings            strings;

#undef GUI_MSG_DEBUG

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

HANDLE
State::
InitMessageQueue( const wchar_t * queueName , BOOL read )
{
	HANDLE queue;
	MSGQUEUEOPTIONS		options;

	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = read;
	options.dwMaxMessages = 0;
	//options.cbMaxMessage = (MAX_QUEUE_LENGTH * 2); // doulbe union code queue size 
	options.cbMaxMessage = MAX_QUEUE_LENGTH ; // max queue size  

	//queue = mq_open( queueName, O_WRONLY );
	queue = CreateMsgQueue(queueName, &options);
	
	if ( queue == NULL )
	{
		LogError( "failure to open %s queue.", queueName );
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
InitGuiComm( const wchar_t * queueName )
{
	GuiReceiveMsg_t msg;
	
	msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
	//strcpy( msg.data.inputReceiver.name, queueName );
	wsprintf(msg.data.inputReceiver.name, L"%s", queueName);

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
InitEngineComm( const wchar_t * queueName )
{
	EngineCommReceiveMsg_t msg;

	msg.msgId = MSG_ENGINE_COMM_SET_MESSAGE_RECEIVER;
	//strcpy( msg.data.messageReceiver.name, queueName );
	sprintf(msg.data.messageReceiver.name, "%S", queueName);
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
	if ( analysisQueue == NULL )
	{
		analysisQueue = InitMessageQueue( ANALYSIS_QUEUE_NAME, FALSE );
	}
	
	if ( analysisQueue != NULL )
	{
		//mq_send( analysisQueue, (const char *)msg, msgDataLen + sizeof( AnalysisMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(analysisQueue, msg, msgDataLen + sizeof( AnalysisMsgId_t ), INFINITE, NULL);
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
	if ( analysisQueue == NULL )
	{
		analysisQueue = InitMessageQueue( ANALYSIS_QUEUE_NAME, FALSE );
	}
	
	if ( analysisQueue != NULL )
	{
		//mq_send( analysisQueue, (const char *)&msgId, sizeof( AnalysisMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(analysisQueue, &msgId, sizeof( AnalysisMsgId_t ), INFINITE, NULL);
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
	if ( guiQueue == NULL )
	{
		guiQueue = InitMessageQueue( GUI_QUEUE_NAME, FALSE );
	}
	
	if ( guiQueue != NULL )
	{
		//mq_send( guiQueue, (const char *)msg, msgDataLen + sizeof( GuiMsgId_t ), MQ_PRIO_MAX-1 );
		BOOL result = WriteMsgQueue(guiQueue, msg, msgDataLen + sizeof( GuiMsgId_t ), INFINITE, NULL);
#ifdef GUI_MSG_DEBUG
		LogDebug("State::SendGuiMessage | sent msgid %u %s, error %u",msg->msgId, result?"passed":"failed", GetLastError());
#endif

	}
}

void
State::
SendGuiMessage( GuiMsgId_t msgId )
{
	if ( guiQueue == NULL )
	{
		guiQueue = InitMessageQueue( GUI_QUEUE_NAME, FALSE );
	}
	
	if ( guiQueue != NULL )
	{
		//mq_send( guiQueue, (const char *)&msgId, sizeof( GuiMsgId_t ), MQ_PRIO_MAX-1 );
		BOOL result = WriteMsgQueue(guiQueue, &msgId, sizeof( GuiMsgId_t ), INFINITE, NULL);
#ifdef GUI_MSG_DEBUG
		LogDebug("State::SendGuiMessage | sent msgid %u %s, error %u",msgId, result?"passed":"failed", GetLastError());
#endif
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
	if ( recallQueue == NULL )
	{
		recallQueue = InitMessageQueue( RECALL_QUEUE_NAME, FALSE );
	}
	
	if ( recallQueue != NULL )
	{
		//mq_send( recallQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(recallQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendRecallMessage( RecallMsgId_t msgId )
{
	if ( recallQueue == NULL )
	{
		recallQueue = InitMessageQueue( RECALL_QUEUE_NAME, FALSE );
	}
	
	if ( recallQueue != NULL )
	{
		//mq_send( recallQueue, (const char *)&msgId, sizeof( RecallMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(recallQueue, &msgId, sizeof( RecallMsgId_t ), INFINITE, NULL);
	}
}

SystemErrNum_t
State::
SendPrinterMessage (PrinterMsg_t *msg)
{

    BOOL            result;
    DWORD           flags;
    DWORD           receivedSize = 0;
    PrinterMsg_t    respMsg;
    SystemErrNum_t  errNumber = SERR_NONE;
    DWORD           waitTime = 3000; // 3 second timeout

    if (printerQueue == NULL) {
        printerQueue = InitMessageQueue (PRINTER_QUEUE_NAME, FALSE);
    }
    if (printerRespQueue == NULL) {
        printerRespQueue = InitMessageQueue (PRINTER_RESP_QUEUE_NAME, TRUE);
    }

    // send print command to print process
    if (printerQueue != NULL) {
        WriteMsgQueue(printerQueue, msg, sizeof (PrinterMsg_t) + sizeof (int), INFINITE, NULL);
    }

    // wait for response. should have timeout
    if (printerRespQueue != NULL) {
        result = ReadMsgQueue(printerRespQueue, &respMsg, sizeof(PrinterMsg_t), &receivedSize, waitTime, &flags);
        if (result != FALSE)  {
            if (receivedSize > 0) {
                if (respMsg.msgId == MSG_PRINT_RESPONSE) {
                    errNumber = respMsg.errNumber;
                    if (errNumber != SERR_NONE) {
                        LogDebug ("Receive printer response 0x%x", errNumber);
                    }
                }
            } else {
                LogError ("Printer does not response, error=%d", GetLastError ());
            }
        } else {
            LogError ("Printer does not response, error=%d", GetLastError ());
        }
    }
    return errNumber;
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
	if ( settingsQueue == NULL )
	{
		settingsQueue = InitMessageQueue( SETTINGS_QUEUE_NAME, FALSE );
	}
	
	if ( settingsQueue != NULL )
	{
		//mq_send( settingsQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(settingsQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendSettingsMessage( SettingsMsgId_t msgId )
{
	if ( settingsQueue == NULL )
	{
		settingsQueue = InitMessageQueue( SETTINGS_QUEUE_NAME, FALSE );
	}
	
	if ( settingsQueue != NULL )
	{
		//mq_send( settingsQueue, (const char *)&msgId, sizeof( SettingsMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(settingsQueue, &msgId, sizeof( SettingsMsgId_t ), INFINITE, NULL);
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
	if ( engineCommQueue == NULL )
	{
		engineCommQueue = InitMessageQueue( ENGINE_COMM_QUEUE_NAME, FALSE );
	}
	
	if ( engineCommQueue != NULL )
	{
		//mq_send( engineCommQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(engineCommQueue, msg, sizeof(EngineCommReceiveMsg_t)/*msgDataLen + sizeof( int )*/, INFINITE, NULL);
	}
}

void
State::
SendEngineCommMessage( EngineCommMsgId_t msgId )
{
	if ( engineCommQueue == NULL )
	{
		engineCommQueue = InitMessageQueue( ENGINE_COMM_QUEUE_NAME, FALSE );
	}
	
	if ( engineCommQueue != NULL )
	{
		//mq_send( engineCommQueue, (const char *)&msgId, sizeof( EngineCommMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(engineCommQueue, &msgId, sizeof( EngineCommMsgId_t ), INFINITE, NULL);
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
	if ( calcResultsQueue == NULL )
	{
		calcResultsQueue = InitMessageQueue( CALC_RESULTS_QUEUE_NAME, FALSE );
	}
	
	if ( calcResultsQueue != NULL )
	{
		//mq_send( calcResultsQueue, (const char *)&msgId, sizeof( CalcResultsMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(calcResultsQueue, &msgId, sizeof( CalcResultsMsgId_t ), INFINITE, NULL);
	}
}

void
State::
SendCalcResultsMessage( CalcResultsMsgId_t msgId, CalcResultsMsgData_t * data )
{
	CalcResultsMsg_t	msg;
	
	if ( calcResultsQueue == NULL )
	{
		calcResultsQueue = InitMessageQueue( CALC_RESULTS_QUEUE_NAME, FALSE );
	}
	
	if ( calcResultsQueue != NULL )
	{
		msg.msgId = msgId;
		msg.data = *data;
		//mq_send( calcResultsQueue, (const char *)&msg, sizeof( CalcResultsMsg_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(calcResultsQueue, &msg, sizeof( CalcResultsMsg_t ), INFINITE, NULL);
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
	if ( heaterControlQueue == NULL )
	{
		heaterControlQueue = InitMessageQueue( HEATER_CONTROL_QUEUE_NAME, FALSE );
	}
	
	if ( heaterControlQueue != NULL )
	{
		WriteMsgQueue(heaterControlQueue, &msgId, sizeof( HeaterControlMsgId_t ), INFINITE, NULL);
	}
} 

void
State::
SendHeaterControlMessage( HeaterControlMsg_t *msg, int msgDataLen )
{
	if ( heaterControlQueue == NULL )
	{
		heaterControlQueue = InitMessageQueue( HEATER_CONTROL_QUEUE_NAME, FALSE );
	}
	
	if ( heaterControlQueue != NULL )
	{
		WriteMsgQueue(heaterControlQueue, msg, msgDataLen + sizeof (int), INFINITE, NULL);
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
	if ( startupQueue == NULL )
	{
		startupQueue = InitMessageQueue( STARTUP_QUEUE_NAME, FALSE );
	}
	
	if ( startupQueue != NULL )
	{
		//mq_send( startupQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(startupQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendStartupMessage( StartupMsgId_t msgId )
{
	if ( startupQueue == NULL )
	{
		startupQueue = InitMessageQueue( STARTUP_QUEUE_NAME, FALSE );
	}
	
	if ( startupQueue != NULL )
	{
		//mq_send( startupQueue, (const char *)&msgId, sizeof( StartupMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(startupQueue, &msgId, sizeof( StartupMsgId_t ), INFINITE, NULL);
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
	if ( serialCommQueue == NULL )
	{
		serialCommQueue = InitMessageQueue( SERIAL_COMM_QUEUE_NAME, FALSE );
	}
	
	if ( serialCommQueue != NULL )
	{
		//mq_send( serialCommQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(serialCommQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendSerialCommMessage( SerialCommMsgId_t msgId )
{
	if ( serialCommQueue == NULL )
	{
		serialCommQueue = InitMessageQueue( SERIAL_COMM_QUEUE_NAME, FALSE );
	}
	
	if ( serialCommQueue != NULL )
	{
		//mq_send( serialCommQueue, (const char *)&msgId, sizeof( SerialCommMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(serialCommQueue, &msgId, sizeof( SerialCommMsgId_t ), INFINITE, NULL);
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
	if ( astmQueue == NULL )
	{
		astmQueue = InitMessageQueue( ASTM_QUEUE_NAME, FALSE );
	}
	
	if ( astmQueue != NULL )
	{
		//mq_send( astmQueue, (const char *)msg, msgDataLen + sizeof( int ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(astmQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendAstmMessage( AstmMsgId_t msgId )
{
	if ( astmQueue == NULL )
	{
		astmQueue = InitMessageQueue( ASTM_QUEUE_NAME, FALSE );
	}
	
	if ( astmQueue != NULL )
	{
		//mq_send( astmQueue, (const char *)&msgId, sizeof( SerialCommMsgId_t ), MQ_PRIO_MAX-1 );
		WriteMsgQueue(astmQueue, &msgId, sizeof( SerialCommMsgId_t ), INFINITE, NULL);
	}
}

/*
 * FUNCTION
 * 	State::SendHl7Message
 * 
 * DESCRIPTION
 * 	Sends a message to the Hl7 process.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Hl7 process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendHl7Message( Hl7Msg_t * msg, int msgDataLen )
{
	if ( hl7Queue == NULL )
	{
		hl7Queue = InitMessageQueue( HL7_QUEUE_NAME, FALSE );
	}
	
	if ( hl7Queue != NULL )
	{
		WriteMsgQueue (hl7Queue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendHl7Message( Hl7MsgId_t msgId )
{
	if ( hl7Queue == NULL )
	{
		hl7Queue = InitMessageQueue( HL7_QUEUE_NAME, FALSE );
	}
	
	if ( hl7Queue != NULL )
	{
		WriteMsgQueue (hl7Queue, &msgId, sizeof( Hl7MsgId_t ), INFINITE, NULL);
	}
}

/*
 * FUNCTION
 * 	State::SendErrorProcessMessage
 * 
 * DESCRIPTION
 * 	Sends a message to the ErrorProcess.
 * 
 * PARAMETERS
 * 	msg - Message to send to the Error process.
 * 	msgDataLen - Length of the data field of the message.
 */

void
State::
SendErrorProcessMessage( ErrorProcessMsg_t *msg, int msgDataLen )
{
	if ( errorProcessQueue == NULL )
	{
		errorProcessQueue = InitMessageQueue( ERROR_PROCESS_QUEUE_NAME, FALSE );
	}
	
	if ( errorProcessQueue != NULL )
	{
		WriteMsgQueue (errorProcessQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendErrorProcessMessage( ErrorProcessMsg_t msgId )
{
	if ( errorProcessQueue == NULL )
	{
		errorProcessQueue = InitMessageQueue( ERROR_PROCESS_QUEUE_NAME, FALSE );
	}
	
	if ( errorProcessQueue != NULL )
	{
		WriteMsgQueue (errorProcessQueue, &msgId, sizeof( ErrorProcessMsgId_t ), INFINITE, NULL);
	}
}




/*
 * FUNCTION
 *  State::SendManufactureMessage
 *
 * DESCRIPTION
 *  Sends a message to the manufacturing process.
 *
 * PARAMETERS
 *  msg - Message to send to the manufacturing process.
 *  msgDataLen - Length of the data field of the message.
 */

void
State::
SendManufactureMessage (ManufactureMsg_t* msg, int msgDataLen)
{
	if (manufactureQueue == NULL) {
		manufactureQueue = InitMessageQueue (MANUFACTURE_QUEUE_NAME, FALSE);
	}
	
	if (manufactureQueue != NULL) {
		WriteMsgQueue (manufactureQueue, msg, msgDataLen + sizeof( int ), INFINITE, NULL);
	}
}

void
State::
SendManufactureMessage (ManufactureId_t msgId)
{
	if (manufactureQueue == NULL) {
		manufactureQueue = InitMessageQueue (MANUFACTURE_QUEUE_NAME, FALSE);
	}
	
	if (manufactureQueue != NULL) {
		WriteMsgQueue(manufactureQueue, &msgId, sizeof(ManufactureId_t), INFINITE, NULL);
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
SetTopLabel( const wchar_t * label )
{
	GuiReceiveMsg_t msg;
	int				dataLen = 0;
	
	if ( label != 0 )
	{
		msg.msgId = MSG_GUI_TOP_LABEL;
		wcsncpy( msg.data.topLabel.label, label, sizeof( msg.data.topLabel.label ) );
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
SetBottomButtons( const wchar_t * label0, bool selected0,
				  const wchar_t * label1, bool selected1,
				  const wchar_t * label2, bool selected2 )
{
	GuiReceiveMsg_t msg;
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	if ( label0 != 0 )
	{
		wcsncpy( msg.data.bottomButtons.buttonText[0], label0, sizeof( msg.data.bottomButtons.buttonText[0] ) );
		msg.data.bottomButtons.buttonHighlighted[0] = selected0;
		msg.msgId = MSG_GUI_BOTTOM_ONE_BUTTON;

		if ( label1 != 0 )
		{
			wcsncpy( msg.data.bottomButtons.buttonText[1], label1, sizeof( msg.data.bottomButtons.buttonText[1] ) );
			msg.data.bottomButtons.buttonHighlighted[1] = selected1;
			msg.msgId = MSG_GUI_BOTTOM_TWO_BUTTONS;
			
			if ( label2 != 0 )
			{
				wcsncpy( msg.data.bottomButtons.buttonText[2], label2, sizeof( msg.data.bottomButtons.buttonText[2] ) );
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

// ZZZ temp disable Chinese time display for future Gui code changed
#if 0 
void
State::
SetBottomDateTimeStrings(AStrings *strings)
{

	GuiReceiveMsg_t 		msg;
	int						i;

	msg.msgId = MSG_GUI_SET_DATE_TIME_STRINGS;
	msg.data.bottomDateTime.twentyFourHourFormat = settings->time24Format;
	for ( i = 0; i < 7; i++ )
	{
		wcscpy( msg.data.bottomDateTime.daysOfWeek[ i ], (*strings).DayOfWeek( i ) );
	}
	for ( i = 0; i < 12; i++ )
	{
		wcscpy( msg.data.bottomDateTime.months[ i ], (*strings).Month( i+1 ) );
	}
				
	SendGuiMessage( &msg, sizeof( GuiBottomDateTimeData_t ) );
}

#else
void
State::
SetBottomDateTimeStrings(AStrings *strings)
{

	GuiReceiveMsg_t 		msg;
	int						i;

	msg.msgId = MSG_GUI_SET_DATE_TIME_STRINGS;
    memset (&msg.data.bottomDateTime, 0, sizeof (msg.data.bottomDateTime));
	msg.data.bottomDateTime.twentyFourHourFormat = settings->time24Format;

	for ( i = 0; i < 7; i++ )
	{
		wcscpy( msg.data.bottomDateTime.daysOfWeek[ i ], (*strings).PrintDayOfWeek( i ) );
	}
	for ( i = 0; i < 12; i++ )
	{
		wcscpy( msg.data.bottomDateTime.months[ i ], (*strings).PrintMonth( i+1 ) );
	}
				
	SendGuiMessage( &msg, sizeof( GuiBottomDateTimeData_t ) );
}
#endif



/* 
 * convert wchar_t string to char string
 */
char *
State::
WstrToCstr (const wchar_t *wStr)
{
    //max convert length is 512 bytes
    static char cStr [MAX_STRING_LENGTH];

    wcstombs (cStr, wStr, MAX_STRING_LENGTH);

    return cStr;
}


/*
 * convert char string to wchar_t string
 */
wchar_t *
State::
CstrToWstr (const char *cStr)
{
    static wchar_t wStr [MAX_STRING_LENGTH *2];

    mbstowcs (wStr, cStr, MAX_STRING_LENGTH);;

    return wStr;
}
