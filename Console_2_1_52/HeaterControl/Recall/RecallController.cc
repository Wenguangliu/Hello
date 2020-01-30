/*
 * FILENAME
 * 	RecallController.c
 * 
 * MODULE DESCRIPTION
 * 	Controller for the Recall process state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <sys/neutrino.h>
#include <errno.h>

#include "State.h"
#include "RecallMessages.h"
#include "GuiMessages.h"
#include "RecallStates.h"
#include "Shmem.h"
#include "LogErr.h"

void 	Receiver();
void *	Sender( void * arg );
void	InitStates();

State *	states[ STATE_COUNT ];
state_t	currentState;

/*
 * FUNCTION
 * 	main
 * 
 * DESCRIPTION
 * 	Initializes the states and interprocess communication.  Then, turns control
 * 	over to Receive().
 * 
 * PARAMETERS
 * 	Standard main params -- not used.
 * 
 * RETURNS
 * 	-1 if there was an error establishing communications.  Else, we never return.
 */

int
main( int argc, char ** argv )
{
	InitStates();								// Init the states.
	
	currentState = STATE_BACKGROUND;			// Go to the first state.
	states[ currentState ]->Enter();
	
	Receiver();									// Process messages and state changes.
	
	return -1;
}

/*
 * FUNCTION
 * 	InitStates
 * 
 * DESCRIPTION
 * 	Initializes all the states with the states they proceed to based on inputs.
 */

void InitStates()
{
	states[ STATE_BACKGROUND ]						= new StateBackground();
	states[ STATE_CLINICAL_ARCHIVE_TRANSMITTING ]	= new StateClinicalArchiveTransmitting();
	states[ STATE_CONFIRM_CANCEL_ARCHIVE ]			= new StateConfirmCancelArchive();
	states[ STATE_CONFIRM_CANCEL_TRANSMIT ]			= new StateConfirmCancelTransmit();
	states[ STATE_DATE_RANGE_END ]					= new StateDateRangeEnd();
	states[ STATE_DATE_RANGE_START ]				= new StateDateRangeStart();
	states[ STATE_ENTER_DATE ]						= new StateEnterDate();
	states[ STATE_ENTER_OPERATOR_ID ]				= new StateEnterOperatorId();
	states[ STATE_ENTER_PATIENT_CONTROL_ID ]		= new StateEnterPatientControlId();
	states[ STATE_ERROR ]							= new StateError();
	states[ STATE_INFO ]							= new StateInfo();
	states[ STATE_LIST ]							= new StateList();
	states[ STATE_NO_RESULTS ]						= new StateNoResults();
	states[ STATE_SEARCH_USING ]					= new StateSearchUsing();
	states[ STATE_SELECT_OUTPUT_TYPE ]				= new StateSelectOutputType();
	states[ STATE_SELECT_RECALL_TYPE ]				= new StateSelectRecallType();
	states[ STATE_SELECT_REPORT_TYPE ]				= new StateSelectReportType();
	states[ STATE_SENDING_REPORT ]					= new StateSendingReport();
	states[ STATE_SERVICE_ARCHIVE ]					= new StateServiceArchive();
	states[ STATE_TRANSMIT_ALL ]					= new StateTransmitAll();
	states[ STATE_TRANSMIT_RANGE_QUEUED ]			= new StateTransmitRangeQueued();
	states[ STATE_TRANSMIT_RANGE_TRANSMITTING ]		= new StateTransmitRangeTransmitting();
	states[ STATE_VIEW_RESULT ]						= new StateViewResult();
	states[ STATE_WARNING ]							= new StateWarning();
}

/*
 * FUNCTION
 * 	Receiver
 * 
 * DESCRIPTION
 * 	Receives messages, passes them on to the current state, and changes to a
 * 	new state if the current state dictates.
 */

void
Receiver()
{
	ssize_t			receivedSize = 0;
	ConsoleMsg_t	msg;
	struct mq_attr	mqstat;
	state_t			newState;
	
	/*
	 * Open the Analysis input queue and get its size.
	 */
	 
	mqd_t queue = mq_open( RECALL_QUEUE_NAME, O_RDONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open message queue, errno: %d", errno );
		return;
	}
	
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes, errno: %d", errno );
		return;
	}
	
	/*
	 * Receive messages and handle state changes.
	 */
	 
	while ( 1 )
	{
		receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );

		if ( receivedSize != -1 )
		{
			if ( receivedSize > 0 )
			{
				newState = states[ currentState ]->ProcessMessage( msg.msgId, msg.data );
				if ( newState >= 0
					 && newState != currentState
					 && newState < STATE_COUNT
					 && states[ newState ] != 0 )
				{
					states[ newState ]->Enter();
					currentState = newState;
				}
			}
		}
		else
		{
			LogError( "message receive error, errno: %d", errno );
			break;
		}
	}
}
