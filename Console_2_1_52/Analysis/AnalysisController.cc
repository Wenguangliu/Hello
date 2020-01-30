/*
 * FILENAME
 * 	AnalysisController.c
 * 
 * MODULE DESCRIPTION
 * 	Controller for the Analysis process state machine.
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

#include "AnalysisMessages.h"
#include "GuiMessages.h"
#include "StateAnalysis.h"
#include "AnalysisStates.h"
#include "RecallMessages.h"
#include "SettingsMessages.h"
#include "EngineCommMessages.h"
#include "Shmem.h"
#include "LogErr.h"

void 	Receiver();
void *	Sender( void * arg );
void	InitStates();

static State *	states[ STATE_COUNT ];
static state_t	currentState;

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
	InitStates();						// Init the states.
	
	State::InitGuiComm( ANALYSIS_QUEUE_NAME );		// Tell the GUI about us.
	State::InitEngineComm( ANALYSIS_QUEUE_NAME );	// Tell the EngineComm about us.
		 
	// Read the calibration file into memory.
	StateAnalysis::ReadCalibrationFile();
	
	// Get the timeout values for the analysis run stages.
	StateAnalysis::ReadTimeoutFile();
	
	currentState = STATE_HOME_WARMING;	// Go to the first state.
	states[ currentState ]->Enter();
	
	Receiver();							// Process messages and state changes.
	
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
	states[ STATE_ABORTING ]					= new StateAborting();
	states[ STATE_ANALYSIS_COMPLETE ]			= new StateAnalysisComplete();
	states[ STATE_ANALYSIS_IN_PROCESS ]			= new StateAnalysisInProcess();
	states[ STATE_ANALYZE_ERROR ]				= new StateAnalyzeError();
	states[ STATE_CANCELING ]					= new StateCanceling();
	states[ STATE_CLOSE_DRAWER ]				= new StateCloseDrawer();
	states[ STATE_CONFIRM_CANCEL ]				= new StateConfirmCancel();
	states[ STATE_CONFIRM_CANCEL_ORDER ]		= new StateConfirmCancelOrder();
	states[ STATE_CONFIRM_ORDER ]				= new StateConfirmOrder();
	states[ STATE_ENTER_ADMISSION_OWNER_ID ]	= new StateEnterAdmissionOwnerId();
	states[ STATE_ENTER_AGE ]					= new StateEnterAge();
	states[ STATE_ENTER_ALTERNATE_ID ]			= new StateEnterAlternateId();
	states[ STATE_ENTER_DOB ]					= new StateEnterDob();
	states[ STATE_ENTER_DOCTOR_ID ]				= new StateEnterDoctorId();
	states[ STATE_ENTER_DOV ]					= new StateEnterDov();
	states[ STATE_ENTER_LOCATION ]				= new StateEnterLocation();
	states[ STATE_ENTER_OPERATOR_ID ]			= new StateEnterOperatorId();
	states[ STATE_ENTER_PATIENT_CONTROL_ID ]	= new StateEnterPatientControlId();
	states[ STATE_ENTER_PHONE_NUMBER ]			= new StateEnterPhoneNumber();
	states[ STATE_ENTER_SAMPLE_ID ]				= new StateEnterSampleId();
	states[ STATE_GRAPHIC_INFO1 ]				= new StateGraphicInfo1();
	states[ STATE_GRAPHIC_INFO2 ]				= new StateGraphicInfo2();
	states[ STATE_GRAPHIC_INFO3 ]				= new StateGraphicInfo3();
	states[ STATE_HOME_READY ]					= new StateHomeReady();
	states[ STATE_HOME_READY_BACKGROUND ]		= new StateHomeReadyBackground();
	states[ STATE_HOME_WARMING ]				= new StateHomeWarming();
	states[ STATE_HOME_WARMING_BACKGROUND ]		= new StateHomeWarmingBackground();
	states[ STATE_INFO ]						= new StateInfo();
	states[ STATE_LOAD_ROTOR ]					= new StateLoadRotor();
	states[ STATE_LOCKOUT_CANCEL ]				= new StateLockoutCancel();
	states[ STATE_LOCKOUT_WITH_ROTOR ]			= new StateLockoutWithRotor();
	states[ STATE_LOCKOUT_WITHOUT_ROTOR ]		= new StateLockoutWithoutRotor();
	states[ STATE_OPEN_DRAWER_WITH_ROTOR ]		= new StateOpenDrawerWithRotor();
	states[ STATE_OPEN_DRAWER_WITHOUT_ROTOR ]	= new StateOpenDrawerWithoutRotor();
	states[ STATE_PATIENT_ID_AUTHENTICATION	]	= new StatePatientIdAuthentication();
	states[ STATE_REMOVE_ROTOR ]				= new StateRemoveRotor();
	states[ STATE_ROTOR_PRESENT_CLOSE_DRAWER ]	= new StateRotorPresentCloseDrawer();
	states[ STATE_ROTOR_PRESENT_OPEN_DRAWER ]	= new StateRotorPresentOpenDrawer();
	states[ STATE_ROTOR_PRESENT_REMOVE_ROTOR ]	= new StateRotorPresentRemoveRotor();
	states[ STATE_ROTOR_PRESENT_WARNING ]		= new StateRotorPresentWarning();
	states[ STATE_SAVING_RESULTS ]				= new StateSavingResults();
	states[ STATE_SELECT_GENDER ]				= new StateSelectGender();
	states[ STATE_SELECT_PENDING_ORDER ]		= new StateSelectPendingOrder();
	states[ STATE_SELECT_RACE ]					= new StateSelectRace();
	states[ STATE_SELECT_TYPE_ADVANCED ]		= new StateSelectTypeAdvanced();
	states[ STATE_SELECT_TYPE_EXPRESS ]			= new StateSelectTypeExpress();
	states[ STATE_WARNING ]						= new StateWarning();
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
	 
	mqd_t queue = mq_open( ANALYSIS_QUEUE_NAME, O_RDONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open message queue, errno: %d", errno );
		exit( -1 );
	}
	
	if ( mq_getattr( queue, & mqstat ) == -1 )
	{
		LogError( "failure to get message queue attributes, errno: %d", errno );
		exit( -1 );
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
				if ( newState >= 0 && newState != currentState && newState < STATE_COUNT )
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
