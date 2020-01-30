/*
 * FILENAME
 * 	States.cc
 * 
 * MODULE DESCIPTION
 * 	Member function definitions for the Analysis state machine state classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdio.h>
#include <sys/neutrino.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>
#include <math.h>
#include <stdlib.h>

#include "AnalysisMessages.h"
#include "AnalysisStates.h"
#include "CommonDefines.h"
#include "Demo.h"
#include "EngineCommMessages.h"
#include "HeaterControlMessages.h"
#include "RecallMessages.h"
#include "ResultsDb.h"
#include "ResultsPrint.h"
#include "SettingsMessages.h"
#include "StartupMessages.h"
#include "SystemError.h"
#include "WriteRad.h"
#include "LogErr.h"
#include "ParseCounters.h"
#include "ParseRoc.h"
#include "Species.h"
#include "GetThumbKeyMode.h"

/*
 * FUNCTION
 * 	StateAnalysis::AgeEnabled
 * 
 * DESCRIPTION
 * 	Determines whether to prompt for age. Can only be true if this is a patient, not a control. Then, if age is enabled
 * 	or this is a piccolo and eGfr is enabled, true will be returned.
 * 
 * RETURN
 * 	true/false
 */

bool
StateAnalysis::
AgeEnabled()
{
	if ( runInfo.patientControl == 'P' )
	{
		if ( settingsData->ageEnabled )
		{
			return true;
		}
		else if ( settingsData->egfrEnabled && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			return true;
		}
	}
	return false;
}

/*
 * FUNCTION
 * 	StateAnalysis::GenderEnabled
 * 
 * DESCRIPTION
 * 	Determines whether to prompt for gender. Can only be true if this is a patient, not a control. Then, if age is enabled
 * 	or this is a piccolo and eGfr is enabled, true will be returned.
 * 
 * RETURN
 * 	true/false
 */

bool
StateAnalysis::
GenderEnabled()
{
	if ( runInfo.patientControl == 'P' )
	{
		if ( settingsData->genderEnabled )
		{
			return true;
		}
		else if ( settingsData->egfrEnabled && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
		{
			return true;
		}
	}
	return false;
}

/*
 * FUNCTION
 * 	StateAnalysis::RaceEnabled
 * 
 * DESCRIPTION
 * 	Determines whether to prompt for race. Can only be true if this is a patient, not a control, and if this is a
 * 	piccolo. Then, if either age or eGfr is enabled, will return true.
 * 
 * RETURN
 * 	true/false
 */

bool
StateAnalysis::RaceEnabled()
{
	if ( (runInfo.patientControl == 'P') && (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) )
	{
		if ( settingsData->raceEnabled || settingsData->egfrEnabled )
		{
			return true;
		}
	}
	return false;
}


/*
 * class StateAnalysisComplete
 */
 
/*
 * class StateAborting
 */

/*
 * FUNCTION
 * 	StateAborting::Enter
 * 
 * DESCRIPTION
 * 	Sets up the GUI and condition variables upon entrance to this state.
 * 
 * NOTE
 * 	This header applies to all Enter() methods for the following subclasses of
 * 	class State.
 */

void
StateAborting::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CANCELING_ANALYSIS ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );

	SendAnalysisMessage( MSG_ANALYSIS_TRIGGER );
}

/*
 * FUNCTION
 * 	StateAborting::ProcessMessage
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
 * 	This header applies to all ProcessMessage() methods for the following
 *	subclasses of class State.
 */

int
StateAborting::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_ABORTING;
	time_t	startTime;
	int		elapsedTime;

	if ( msgId == MSG_ANALYSIS_TRIGGER )
	{
		startTime = time(0);

		ProcessResults();

		uploadedOrderInProcess = 0;

		// If need be, sleep so the "Canceling analysis..." message stays up
		// at least two seconds.
		elapsedTime = (int)difftime( time(0), startTime );
		if ( elapsedTime < 2 )
		{
			sleep( elapsedTime > 0 ? 2 - elapsedTime : 2 );
		}

		retVal = STATE_ANALYZE_ERROR;
	}

	return retVal;
}


void
StateAnalysisComplete::
Enter()
{
	GuiReceiveMsg_t	msg;
	char			topLabel[40];
	StringId_t		msgStr;

	// If rotor 1 and we haven't run all of the requested runs, or Rotor 100, send ourself
	// a bottom button message, which will trigger ProcessMessage to go on.
	if ( (isRotor1 && (rotor1Counter > 1) && (runInfo.errNum == SERR_NONE) ) || isRotor100 )
	{
		SendAnalysisMessage( (AnalysisMsgId_t)MSG_GUI_OUT_BOTTOM_BUTTON );
	}
	
	// If we're in research mode and we've had an error that wasn't displayed
	// when it came in, display it now.
	else if ( systemData->inResearchMode && (runInfo.errNum != SERR_NONE) )
	{
		// Get the message text.
		msgStr = systemError.GetScreenText( runInfo.errNum );
	
		msg.msgId = MSG_GUI_WIN_ERROR;
	
		strcpy( msg.data.winError.topLabel, strings[ TSTR_ERROR ] );
	
		// Add the error number to the beginning of the error message string.
		sprintf( msg.data.winError.message, "%x\n%s", runInfo.errNum, strings[ msgStr ] );
	
		strcpy( msg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
	
		SendGuiMessage( &msg, sizeof( GuiWinWarningData_t ) );
	}

	// Display the Analysis Complete screen.
	else
	{
		if ( runInfo.sampleType >= STYPE_CONTROL &&
			 runInfo.sampleType <= STYPE_LAST_CONTROL )
		{
			sprintf( topLabel, "%s %s", strings[ TSTR_CONTROL ], runInfo.patientControlId );
		}
		else
		{
			sprintf( topLabel, "%s %s", strings[ TSTR_PATIENT ], runInfo.patientControlId );
		}
	
		SetTopLabel( topLabel );
		
		msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
		strcpy( msg.data.middleMessage.message, strings[ MSTR_ANALYSIS_COMPLETE ] );
		SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
		
		SetBottomButtons( strings[ BSTR_OPEN ], true );
		
		// If this was a Rotor 1 and we had an error or if we've run the full sequence,
		// clear the isRotor1 flag so we don't do more.
		isRotor1 = false;
	}
		
	// Re-enable the screen saver in case it was been disabled awaiting user input.
	SendGuiMessage( MSG_GUI_SCREENSAVE_ENABLE );
}

int
StateAnalysisComplete::
ProcessMessage( int msgId, char * data )
{
	int retVal = STATE_ANALYSIS_COMPLETE;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
			{
				retVal = STATE_LOCKOUT_WITH_ROTOR;
			}
			else
			{
				retVal = STATE_OPEN_DRAWER_WITH_ROTOR;
			}
			break;
		
		case MSG_GUI_OUT_INFO:	// Only if we're displaying the error screen.
			infoText = strings[ systemError.GetInfoText( runInfo.errNum ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ANALYSIS_COMPLETE );
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateAnalysisInProcess
 */

void
StateAnalysisInProcess::
Enter()
{
	GuiReceiveMsg_t	msg;
	char			topLabel[40];
	
	if ( runInfo.sampleType >= STYPE_CONTROL &&
		 runInfo.sampleType <= STYPE_LAST_CONTROL )
	{
		sprintf( topLabel, "%s %s", strings[ TSTR_CONTROL ], runInfo.patientControlId );
	}
	else
	{
		sprintf( topLabel, "%s %s", strings[ TSTR_PATIENT ], runInfo.patientControlId );
	}

	SetTopLabel( topLabel );

	if ( receivedRotorName || rotorNameValid )
	{
		msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
		strcpy( msg.data.middleProgBarMessage.progBarText, strings[ MSTR_ANALYZING_SAMPLE ] );
	
		sprintf( msg.data.middleProgBarMessage.message, " %s \n %s ",
				 rotorConfig->rotorScreenNameLine1,
				 rotorConfig->rotorScreenNameLine2 );
		msg.data.middleProgBarMessage.fontColor = rotorConfig->rotorScreenNameFontColor;
		msg.data.middleProgBarMessage.backgroundColor = rotorConfig->rotorScreenNameBackgroundColor;
		receivedRotorName = false;	// So ProcessMessage doesn't act on this flag.
		rotorNameValid = true;	// So we display the rotor name if we come back from ConfirmCancel.
	}
	else
	{
		msg.msgId = MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
		strcpy( msg.data.middleProgBarMessage.progBarText, strings[ MSTR_ANALYZING_SAMPLE ] );
		strcpy( msg.data.middleProgBarMessage.message, strings[ MSTR_DETERMINING_ROTOR_TYPE ] );
		msg.data.middleProgBarMessage.fontColor = 0x4E4E5E;		// Standard text color
		msg.data.middleProgBarMessage.backgroundColor = 0xFFFFFFFF;	// Transparent
	}

	msg.data.middleProgBarMessage.progBarValue = progBarValue;
	SendGuiMessage( &msg, sizeof( GuiMiddleProgBarMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CANCEL ], true );
	
	// Indicate to StateAnalysis that all the user input has been received.
	allInputReceived = true;
}

int
StateAnalysisInProcess::
ProcessMessage( int msgId, char * data )
{
	int retVal = STATE_ANALYSIS_IN_PROCESS;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Cancel
			if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
			{
				retVal = STATE_LOCKOUT_CANCEL;
			}
			else
			{
				retVal = STATE_CONFIRM_CANCEL;
			}
			stateStack.Push( STATE_ANALYSIS_IN_PROCESS );
			break;

		default:
			int savedProgBarValue = progBarValue;
			ProcessOtherMessages( msgId, data );

			if ( isRotor1 && rotor1RunsComplete )
			{
					retVal = STATE_ANALYSIS_COMPLETE;
			}
			else if ( analyzeError != SERR_NONE )
			{
				retVal = STATE_ABORTING;
			}
			else if ( receivedRotorName )
			{
				GuiReceiveMsg_t msg;

				msg.msgId = MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
				strcpy( msg.data.middleProgBarMessage.progBarText, strings[ MSTR_ANALYZING_SAMPLE ] );
			
				sprintf( msg.data.middleProgBarMessage.message, " %s \n %s ",
						 rotorConfig->rotorScreenNameLine1,
						 rotorConfig->rotorScreenNameLine2 );
				msg.data.middleProgBarMessage.fontColor = rotorConfig->rotorScreenNameFontColor;
				msg.data.middleProgBarMessage.backgroundColor = rotorConfig->rotorScreenNameBackgroundColor;
				msg.data.middleProgBarMessage.progBarValue = progBarValue;
				SendGuiMessage( &msg, sizeof( GuiMiddleProgBarMessageData_t ) );

				receivedRotorName = false;	// So ProcessMessage doesn't act on this flag.
				rotorNameValid = true;	// So we display the rotor name if we come back from ConfirmCancel.
			}
			else if ( analysisDone )
			{
				retVal = STATE_SAVING_RESULTS;
			}
			else if ( progBarValue != savedProgBarValue )
			{
				GuiReceiveMsg_t msg;
				msg.msgId = MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
				msg.data.middleProgBarMessage.progBarValue = progBarValue;
				SendGuiMessage( &msg, sizeof( GuiMiddleProgBarMessageData_t ) );
			}
			break;
	}	
	return retVal;
}

/*
 * class StateCanceling
 */

void
StateCanceling::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CANCELING_ANALYSIS ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );
}

int
StateCanceling::
ProcessMessage( int msgId, char * data )
{
	int 						retVal = STATE_CANCELING;
	EngineCommMessageData_t *	engineMsg;
	
	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineMsg = (EngineCommMessageData_t *)data;
			if ( engineMsg->message[0] == 'A' && engineMsg->message[8] == CANCEL_DONE )
			{
				retVal = STATE_OPEN_DRAWER_WITH_ROTOR;
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}


/*
 * class StateCloseDrawer
 */

void
StateCloseDrawer::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CLOSING_DRAWER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );

	if ( isRotor100 == true )
	{
		SendDrawerMessage( 'M' );
	}
	else
	{
		SendDrawerMessage( 'C' );
	}
	
	/*
	 *  Clear the state stack here. Otherwise, putting in a new rotor at the end of a run will result in the
	 *	stack continuing to grow (since the only other place the stack is cleared is when a Home button is
	 *	pressed. Added 10/6/2010 due to state stack overflow reports.
	 */
	stateStack.Home();
}

int
StateCloseDrawer::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_CLOSE_DRAWER;
	EngineCommMessageData_t *	engineCommMsg;

	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_CLOSE_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x10 )	// Drawer closed
				{
					if ( status & 0x40 )	// Rotor present
					{
						counterData->camCycleCount++;

						// In research mode, and the drawer was closed automatically, so
						// we don't want to run the rotor.
						if ( drawerClosedAutomatically )
						{
							retVal = STATE_HOME_READY;
						}
						else
						{
							StartAnalysis();
	
							if ( isRotor1 && rotor1RunsComplete )
							{
								retVal = STATE_ANALYSIS_COMPLETE;
							}
							
							else if ( allInputReceived )
							{
								retVal = STATE_ANALYSIS_IN_PROCESS;
							}
	
							// Check operator ID if enabled and it wasn't entered in the lockout screen.
							else if ( settingsData->operatorIdEnabled && (lockoutOperatorId[0] == '\0') )
							{
								retVal = STATE_ENTER_OPERATOR_ID;
							}
	
							else if ( uploadedOrderSelected != 0 )
							{
// GLDGLDGLD
// Is this where we make the transition, or do we do so when the close button is pressed?
								retVal = STATE_ANALYSIS_IN_PROCESS;
							}
							
							else if ( settingsData->sampleTypeAdvanced )
							{
								retVal = STATE_SELECT_TYPE_ADVANCED;
							}
							else
							{
								retVal = STATE_SELECT_TYPE_EXPRESS;
							}
						}
					}
					else	// Rotor not present
					{
						// Clear our Rotor 1/100 flags.
						isRotor1 = false;
						isRotor100 = false;
						
						// Clear the runInfo for Rotor 100, but not for Rotor 1.
						memset( &runInfoRotor100, 0, sizeof( RunInfo_t ) );

						retVal = STATE_HOME_READY;
					}
				
					drawerClosedAutomatically = false;
					counterData->drawerCycleCount++;
					WriteCounters( counterData );
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
					retVal = STATE_HOME_READY;
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}


/*
 * class StateConfirmCancel
 */
 
void
StateConfirmCancel::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CANCEL_ANALYSIS ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_NO ], true );
}

int
StateConfirmCancel::
ProcessMessage( int msgId, char * data )
{
	int retVal = STATE_CONFIRM_CANCEL;
	int	idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		
			if ( idata == 0 )		// Cancel confirmed.
			{
				CancelAnalysis();
				retVal = STATE_CANCELING;
				stateStack.Pop();	// Discard return state.
			}
			
			else if ( idata == 1 )	// No cancel, so return to previous state.
			{
				retVal = stateStack.Pop();
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateConfirmCancelOrder
 */
 
void
StateConfirmCancelOrder::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_CONFIRM ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CANCEL_ORDER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_NO ], true );
}

int
StateConfirmCancelOrder::
ProcessMessage( int msgId, char * data )
{
	int retVal = STATE_CONFIRM_CANCEL_ORDER;
	int	idata = *(int *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		
			switch ( idata )
			{
				case 0:	// Cancel
					// If the order wasn't already deleted via ASTM, delete the order.
					if ( !orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
					{
						orderQueue.SendAstmCancelMessage( uploadedOrderSelected, 'X' );
						orderQueue.Dequeue( uploadedOrderSelected );
						uploadedOrderSelected = 0;
					}
					retVal = stateStack.Pop();
					break;
			
				case 1:	// No cancel, so return to previous state.
					// If the order was deleted while we were in this state, post a warning
					// message.
					if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
					{
						analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
						retVal = STATE_WARNING;
					}
	
					// Order not deleted, so just go back.
					else
					{
						retVal = stateStack.Pop();
					}
					break;
				default: break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateConfirmOrder
 */
 
void
StateConfirmOrder::
Enter()
{
	GuiReceiveMsg_t 	msg;
// GLDGLDGLD
// We need safety-net code just in case it's possible that this order has been deleted.
	UploadedOrder_t	*	orderInfo = orderQueue.GetEntryInfo( uploadedOrderSelected );
	
	SetTopLabel( strings[ TSTR_CONFIRM_ORDER ] );

	msg.msgId = MSG_GUI_MIDDLE_LOAD_ORDER;

	strcpy( msg.data.middleLoadOrder.orderLabel, orderInfo->orderLabel );
	sprintf( msg.data.middleLoadOrder.rotorName, " %s \n %s ",
			 orderInfo->rotorNameLine1,
			 orderInfo->rotorNameLine2 );
	msg.data.middleLoadOrder.fontColor = orderInfo->rotorNameFontColor;
	msg.data.middleLoadOrder.backgroundColor = orderInfo->rotorNameBackgroundColor;
	strcpy( msg.data.middleLoadOrder.buttonText, strings[ MSTR_CANCEL_ORDER ] );

	SendGuiMessage( &msg, sizeof( GuiMiddleLoadOrderData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CONFIRM ], true );
}

int
StateConfirmOrder::
ProcessMessage( int msgId, char * data )
{
	int	idata = *(int *)data;
	int	retVal = STATE_CONFIRM_ORDER;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:	// Cancel Order
			if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
			{
				analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
				retVal = STATE_WARNING;
			}
			else
			{
				retVal = STATE_CONFIRM_CANCEL_ORDER;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
			{
				analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
				retVal = STATE_WARNING;
			}
			else
			{
				switch ( idata )
				{
					case 0:		// Back
						retVal = stateStack.Pop();
						break;
							
					case 1:		// Confirm
						if ( settingsData->astmOrderMethod == ASTM_ORDERS_ENTER_ID )
						{
							retVal = STATE_PATIENT_ID_AUTHENTICATION;
						}
						else
						{
							// No canceling order from this point on.
							uploadedOrderInProcess = uploadedOrderSelected;
							retVal = STATE_OPEN_DRAWER_WITHOUT_ROTOR;
						}
						break;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateEnterAge
 */
 
void
StateEnterAge::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_AGE ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

	msg.data.middleEnterDate.labelLeft[0] = '\0';
	msg.data.middleEnterDate.labelMiddle[0] = '\0';
	msg.data.middleEnterDate.labelRight[0] = '\0';
	strcpy( msg.data.middleEnterDate.textLeft, "0" );
	strcpy( msg.data.middleEnterDate.textMiddle, "0" );
	strcpy( msg.data.middleEnterDate.textRight, strings[ MSTR_YRS ] );
	msg.data.middleEnterDate.buttonText[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
	
	ageUnits = AGE_UNITS_YEARS;
	ageValue = 0;
}

int
StateEnterAge::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_AGE;
	int				idata = *(int *)data;
	bool			updateTens = false;
	bool			updateOnes = false;
	bool			updateUnits = false;
	GuiReceiveMsg_t	msg;

	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0:
					if ( ageValue < 90 )
					{
						ageValue += 10;
						updateTens = true;
					}
					break;
				case 1:
					if ( (ageValue % 10) < 9 )
					{
						ageValue += 1;
						updateOnes = true;
					}
					break;
				case 2:
					if ( ageUnits < AGE_UNITS_YEARS )
					{
						ageUnits++;
						updateUnits = true;
					}
					break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0:
					if ( ageValue >= 10 )
					{
						ageValue -= 10;
						updateTens = true;
					}
					break;
				case 1:
					if ( (ageValue % 10) > 0)
					{
						ageValue -= 1;
						updateOnes = true;
					}
					break;
				case 2:
					if ( ageUnits > AGE_UNITS_DAYS )
					{
						ageUnits--;
						updateUnits = true;
					}
					break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done.
					runInfo.age = ageValue;
					runInfo.ageUnits = (AgeUnits_t)ageUnits;
					stateStack.Push( STATE_ENTER_AGE );
					if ( RaceEnabled() )
					{
						retVal = STATE_SELECT_RACE;
					}
					else
					{
						retVal = STATE_ENTER_PATIENT_CONTROL_ID;
					}
					LogDebug( "Age entered" );
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}

	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	else if ( updateTens )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		sprintf( msg.data.middleEnterDate.textLeft, "%d", ageValue / 10 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	else if ( updateOnes )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		sprintf( msg.data.middleEnterDate.textMiddle, "%d", ageValue % 10 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	else if ( updateUnits )
	{
		StringId_t		sid = STR_BLANK;
		
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;

		switch ( ageUnits )
		{
			case AGE_UNITS_DAYS:	sid = MSTR_DAYS;	break;
			case AGE_UNITS_WEEKS:	sid = MSTR_WKS;		break;
			case AGE_UNITS_MONTHS:	sid = MSTR_MOS;		break;
			case AGE_UNITS_YEARS:	sid = MSTR_YRS;		break;
		}
		strcpy( msg.data.middleEnterDate.textRight, strings[ sid ] );

		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterAlternateId
 */
 
void
StateEnterAlternateId::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_ALTERNATE_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterAlternateId::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_ALTERNATE_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.alternateId, sdata, sizeof( runInfo.alternateId ) );

			if ( settingsData->sampleIdEnabled )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_SAMPLE_ID;
			}
			else if ( settingsData->doctorIdEnabled )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_DOCTOR_ID;
			}
			else if ( settingsData->locationEnabled )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_LOCATION;
			}
			else if ( settingsData->phoneNumberEnabled )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_PHONE_NUMBER;
			}
			else if ( settingsData->admissionOwnerIdEnabled )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
			}
			else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
			{
				stateStack.Push( STATE_ENTER_ALTERNATE_ID );
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterDoctorId
 */
 
void
StateEnterDoctorId::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_DOCTOR_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterDoctorId::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_DOCTOR_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.doctorId, sdata, sizeof( runInfo.doctorId ) );

			if ( settingsData->locationEnabled )
			{
				stateStack.Push( STATE_ENTER_DOCTOR_ID );
				retVal = STATE_ENTER_LOCATION;
			}
			else if ( settingsData->phoneNumberEnabled )
			{
				stateStack.Push( STATE_ENTER_DOCTOR_ID );
				retVal = STATE_ENTER_PHONE_NUMBER;
			}
			else if ( settingsData->admissionOwnerIdEnabled )
			{
				stateStack.Push( STATE_ENTER_DOCTOR_ID );
				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
			}
			else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_DOCTOR_ID );
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
			{
				stateStack.Push( STATE_ENTER_DOCTOR_ID );
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterLocation
 */
 
void
StateEnterLocation::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_LOCATION ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterLocation::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_LOCATION;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.location, sdata, sizeof( runInfo.location ) );

			if ( settingsData->phoneNumberEnabled )
			{
				stateStack.Push( STATE_ENTER_LOCATION );
				retVal = STATE_ENTER_PHONE_NUMBER;
			}
			else if ( settingsData->admissionOwnerIdEnabled )
			{
				stateStack.Push( STATE_ENTER_LOCATION );
				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
			}
			else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_LOCATION );
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
			{
				stateStack.Push( STATE_ENTER_LOCATION );
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterPhoneNumber
 */
 
void
StateEnterPhoneNumber::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_PHONE_NUMBER ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterPhoneNumber::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_PHONE_NUMBER;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.phoneNumber, sdata, sizeof( runInfo.phoneNumber ) );

			if ( settingsData->admissionOwnerIdEnabled )
			{
				stateStack.Push( STATE_ENTER_PHONE_NUMBER );
				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
			}
			else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_PHONE_NUMBER );
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
			{
				stateStack.Push( STATE_ENTER_PHONE_NUMBER );
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterAdmissionOwnerId
 */
 
void
StateEnterAdmissionOwnerId::
Enter()
{
	GuiReceiveMsg_t msg;

	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		SetTopLabel( strings[ TSTR_ENTER_ADMISSION_ID ] );
	}
	else
	{
		SetTopLabel( strings[ TSTR_ENTER_OWNER_ID ] );
	}
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterAdmissionOwnerId::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.admissionOwnerId, sdata, sizeof( runInfo.admissionOwnerId ) );

			if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
			{
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterDob
 */
 
void
StateEnterDob::
Enter()
{
	GuiReceiveMsg_t msg;
	time_t			t;
	struct tm * 	currentTime;

	SetTopLabel( strings[ TSTR_ENTER_DATE_OF_BIRTH ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;

	strcpy( msg.data.middleEnterDate.labelLeft, strings[ MSTR_DAY ] );
	strcpy( msg.data.middleEnterDate.labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( msg.data.middleEnterDate.labelRight, strings[ MSTR_YEAR ] );
	
	// Get the current date.
	t = time( 0 );
	currentTime = gmtime( &t );
	
	// Save the current date values.
	day = currentTime->tm_mday;
	month = currentTime->tm_mon;
	year = currentTime->tm_year + 1900;
	
	sprintf( msg.data.middleEnterDate.textLeft, "%d", day );
	strcpy( msg.data.middleEnterDate.textMiddle, strings.Month( month ) );
	sprintf(msg.data.middleEnterDate.textRight, "%d", year );
	
	msg.data.middleEnterDate.buttonText[0] = '\0';

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterDob::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_DOB;
	bool			updateDay = false;
	bool			updateMonth = false;
	bool			updateYear = false;
	GuiReceiveMsg_t	updateMsg;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0: day++;		updateDay = true;	break;
				case 1:	month++;	updateMonth = true;	break;
				case 2: year++;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0: day--;		updateDay = true;	break;
				case 1:	month--;	updateMonth = true;	break;
				case 2: year--;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done.
					runInfo.dateOfBirth.year = year;
					runInfo.dateOfBirth.month = month;
					runInfo.dateOfBirth.day = day;
					
					if ( settingsData->dovEnabled && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
					{
						stateStack.Push( STATE_ENTER_DOV );
						retVal = STATE_ENTER_DOV;
					}
					else
					{
						if ( analysisDone )
						{
							retVal = STATE_SAVING_RESULTS;
						}
						else
						{
							retVal = STATE_ANALYSIS_IN_PROCESS;
						}
					}
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}

	if ( updateDay )
	{
		if ( day > 31 )
			day = 1;
		else if ( day < 1 )
			day = 31;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		sprintf( updateMsg.data.middleEnterDate.textLeft, "%d", day );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		if ( month > 11 )
			month = 0;
		else if ( month < 0 )
			month = 11;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		strcpy( updateMsg.data.middleEnterDate.textMiddle, strings.Month( month ) );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
		
	}
	else if ( updateYear )
	{
		if ( year > 9999 )
			year = 1;
		else if ( year < 1 )
			year = 9999;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		sprintf( updateMsg.data.middleEnterDate.textRight, "%04d", year );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}

	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterDov
 */
 
void
StateEnterDov::
Enter()
{
	GuiReceiveMsg_t msg;
	time_t			t;
	struct tm * 	currentTime;

	SetTopLabel( strings[ TSTR_ENTER_LAST_VACCINATION ] );

	strcpy( msg.data.middleEnterDate.labelLeft, strings[ MSTR_DAY ] );
	strcpy( msg.data.middleEnterDate.labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( msg.data.middleEnterDate.labelRight, strings[ MSTR_YEAR ] );
	
	// Get the current date.
	t = time( 0 );
	currentTime = gmtime( &t );
	
	// Save the current date values.
	day = currentTime->tm_mday;
	month = currentTime->tm_mon;
	year = currentTime->tm_year + 1900;
	
	sprintf( msg.data.middleEnterDate.textLeft, "%d", day );
	strcpy( msg.data.middleEnterDate.textMiddle, strings.Month( month ) );
	sprintf(msg.data.middleEnterDate.textRight, "%d", year );
	
	msg.data.middleEnterDate.buttonText[0] = '\0';

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterDov::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_ENTER_DOV;
	bool			updateDay = false;
	bool			updateMonth = false;
	bool			updateYear = false;
	GuiReceiveMsg_t	updateMsg;
	int				idata = ((GuiSendMsgData_t *)data)->idata;
	
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0: day++;		updateDay = true;	break;
				case 1:	month++;	updateMonth = true;	break;
				case 2: year++;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0: day--;		updateDay = true;	break;
				case 1:	month--;	updateMonth = true;	break;
				case 2: year--;		updateYear = true;	break;
				default: break;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done.
					runInfo.lastVaccination.year = year;
					runInfo.lastVaccination.month = month;
					runInfo.lastVaccination.day = day;

					if ( analysisDone )
					{
						retVal = STATE_SAVING_RESULTS;
					}
					else
					{
						retVal = STATE_ANALYSIS_IN_PROCESS;
					}
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( updateDay )
	{
		if ( day > 31 )
			day = 1;
		else if ( day < 1 )
			day = 31;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		sprintf( updateMsg.data.middleEnterDate.textLeft, "%d", day );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		if ( month > 11 )
			month = 0;
		else if ( month < 0 )
			month = 11;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		strcpy( updateMsg.data.middleEnterDate.textMiddle, strings.Month( month ) );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
		
	}
	else if ( updateYear )
	{
		if ( year > 9999 )
			year = 1;
		else if ( year < 1 )
			year = 9999;
		updateMsg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		sprintf( updateMsg.data.middleEnterDate.textRight, "%04d", year );
		SendGuiMessage( &updateMsg, sizeof( GuiMiddleEnterDateData_t ) );
	}

	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterSampleId
 */
 
void
StateEnterSampleId::
Enter()
{
	GuiReceiveMsg_t msg;

	SetTopLabel( strings[ TSTR_ENTER_SAMPLE_ID ] );
		
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
		
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StateEnterSampleId::
ProcessMessage( int msgId, char * data )
{
	int					retVal = STATE_ENTER_SAMPLE_ID;
	GuiReceiveMsg_t		msg;
	int					idata = *(int *)data;
	char *				sdata = (char *)data;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back.
					retVal = stateStack.Pop();
					break;
				case 1:		// Done, so get ID text from GUI.
					msg.msgId = MSG_GUI_GET_ID_TEXT;
					SendGuiMessage( &msg, 0 );
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			msg.msgId = MSG_GUI_GET_ID_TEXT;
			SendGuiMessage( &msg, 0 );
			break;

		case MSG_GUI_OUT_ID_TEXT:
			strncpy( runInfo.sampleId, sdata, sizeof( runInfo.sampleId ) );

			/*
			 * We assume that EnterSampleId is the last enter ID state which
			 * is not conditional on patient/control status.  Therefore, this is
			 * the only place we test for patient/control.
			 */

			if ( settingsData->doctorIdEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_DOCTOR_ID;
			}
			else if ( settingsData->locationEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_LOCATION;
			}
			else if ( settingsData->phoneNumberEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_PHONE_NUMBER;
			}
			else if ( settingsData->admissionOwnerIdEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_ADMISSION_OWNER_ID;
			}
			else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_DOB;
			}
			else if ( settingsData->dovEnabled  &&
					 (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) &&
					 (runInfo.patientControl == 'P') )
			{
				stateStack.Push( STATE_ENTER_SAMPLE_ID );
				retVal = STATE_ENTER_DOV;
			}
			else
			{
				if ( analysisDone )
				{
					retVal = STATE_SAVING_RESULTS;
				}
				else
				{
					retVal = STATE_ANALYSIS_IN_PROCESS;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
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
	
	SetBottomButtons( strings[ BSTR_CANCEL ], false, strings[ BSTR_DONE ], true );
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

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( idata == 0 )		// Cancel.
			{
				stateStack.Push( STATE_ENTER_OPERATOR_ID );
				if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
				{
					retVal = STATE_LOCKOUT_CANCEL;
				}
				else
				{
					retVal = STATE_CONFIRM_CANCEL;
				}
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
			strncpy( runInfo.operatorId, sdata, sizeof( runInfo.operatorId ) );

			if ( settingsData->sampleTypeAdvanced )
			{
				retVal = STATE_SELECT_TYPE_ADVANCED;
			}
			else
			{
				retVal = STATE_SELECT_TYPE_EXPRESS;
			}
			stateStack.Push( STATE_ENTER_OPERATOR_ID );

			if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) && (expirys.NumOps() > 0) )
			{
				expiry = expirys.GetOp( sdata );

				// Operator is in the system.
				if ( expiry != 0 )
				{
					// Normally, this is already false. However, if they enter an invalid ID, then back up and
					// try again and succeed, we need to clear the flag.
					runInfo.operatorUnauthorized = false;

					// Operator expired.
					if ( (expiry->expiration != 0) && (expiry->expiration < time( 0 )) )
					{
						runInfo.operatorExpired = true;
						analysisWarningQueue.Enqueue( SERR_OPERATOR_RENEWAL_REQUIRED );
						stateStack.Push( retVal );
						retVal = STATE_WARNING;
					}
				}

				// No operator with that ID in the system.
				else
				{
					runInfo.operatorUnauthorized = true;

					// Admin with that ID in the system.
					if ( expirys.GetAdmin( sdata ) )
					{
						analysisWarningQueue.Enqueue( SERR_UNAUTHORIZED_ADMINISTRATOR );
						retVal = STATE_WARNING;
					}
					// No admin with that ID in the system.
					else
					{
						analysisWarningQueue.Enqueue( SERR_UNAUTHORIZED_OPERATOR );
						stateStack.Push( retVal );
						retVal = STATE_WARNING;
					}
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}

	// All input we need from the operator has been done, so we're off to the AIP screen.
	else if ( allInputReceived || (uploadedOrderInProcess != 0) )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateEnterPatientControlId
 */
 
void
StateEnterPatientControlId::
Enter()
{
	GuiReceiveMsg_t msg;
	
	// If we've got an ID already, we skip this step.
	if ( (isRotor1 && (runInfoRotor1.rotorId == 1)) || isRotor100 )
	{
		SendAnalysisMessage( MSG_ANALYSIS_TRIGGER );
	}
	else
	{
		if ( runInfo.patientControl == 'P' )
		{
			SetTopLabel( strings[ TSTR_ENTER_PATIENT_ID ] );
		}
		else
		{
			SetTopLabel( strings[ TSTR_ENTER_CONTROL_ID ] );
		}
	
		msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
		msg.data.middleEnterId.isPassword = 0;
		strcpy( msg.data.middleEnterId.initialValue, "" );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
		
		SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
	}
}

int
StateEnterPatientControlId::
ProcessMessage( int msgId, char * data )
{
	int					retVal = STATE_ENTER_PATIENT_CONTROL_ID;
	int					idata = *(int *)data;
	char *				sdata = (char *)data;
	bool				gotoNextState = false;
	int					rotorId;
	char				rotorIdStr[3];
	char 				rocFileName[100];
	SystemErrorNumber_t	errNum;
	CfgError_t			cfgErr;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;
				case 1:		// Done
					// If we've already got an ID, then we just go on.
					gotoNextState = ((isRotor1 && (runInfoRotor1.rotorId == 1)) || isRotor100);
					if ( !gotoNextState )
					{
						SendGuiMessage( MSG_GUI_GET_ID_TEXT );
					}
					break;
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			gotoNextState = ((isRotor1 && (runInfoRotor1.rotorId == 1)) || isRotor100);
			if ( !gotoNextState )
			{
				SendGuiMessage( MSG_GUI_GET_ID_TEXT );
			}
			break;

		case MSG_GUI_OUT_ID_TEXT:
			// If we haven't already got an automatic ID, then store the sample ID entered,
			// or use "None" if no ID was entered.
			if ( !(isRotor1 && (runInfoRotor1.rotorId == 1)) && !isRotor100 )
			{
				if ( sdata[0] == '\0' )
				{
					strcpy( runInfo.patientControlId, strings[ MSTR_NONE ] );
				}
				else
				{
					strncpy( runInfo.patientControlId, sdata, sizeof( runInfo.patientControlId ) );
				}
				
				// If we've got a Rotor 1, this is the first time, so we need to set the
				// number of times to run it.
				if ( isRotor1 )
				{
					Rotor1GetCount();
				}

				stateStack.Push( STATE_ENTER_PATIENT_CONTROL_ID );

				// Handle the case where a rotor ID is selected in Demo mode.
				if ( systemData->inDemoMode && (strncmp( runInfo.patientControlId, "777", 3 ) == 0) )
				{
					strncpy( rotorIdStr, runInfo.patientControlId + 3, 2 );
					rotorIdStr[2] = '\0';
					rotorId = atoi( rotorIdStr );
					demoResult = GetDemoResultById( rotorId );
					
					// Rotor ID not found in the demo data, so we'll pick one at random from the demo data.
					if ( demoResult == 0 )
					{
						demoResult = GetRandomDemoResult();
					}
					
	 				// Read the ROC file.
					sprintf( rocFileName, "%s/%d.%d.roc", ROC_FILE_DIRECTORY, demoResult->rotorId, demoResult->rotorFormat );
					cfgErr = ParseRoc( rocFileName, rotorConfig );
						
					// ROC file not found, so go through all the demo results, in order, looking for one
					// that does have a ROC file.
					if ( cfgErr != CFG_ERR_NONE ) 
					{
						LogError( "ROC file not found: %s", rocFileName );
						demoResult = GetFirstDemoResult( 0 );
						do
						{
							sprintf( rocFileName, "%s/%d.%d.roc", ROC_FILE_DIRECTORY, demoResult->rotorId, demoResult->rotorFormat );
							cfgErr = ParseRoc( rocFileName, rotorConfig );
							if ( cfgErr != CFG_ERR_NONE )
							{
								LogError( "ROC file not found: %s", rocFileName );
								demoResult = GetNextDemoResult( 0 );
							}
						} while ( (demoResult != 0) && (cfgErr != CFG_ERR_NONE) );
					}
		
					// If we failed to find a ROC file, set the error flag.
					if ( cfgErr != CFG_ERR_NONE )
					{
						if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
						{
							errNum = SERR_ROC_NOT_THERE;
						}
						else
						{
							errNum = SERR_FILE_READ;
						}
						LogError( "ROC parse error # %d", cfgErr );
					}
						
					// ROC file parsed OK.
					else
					{
						// Get the barcode data from the demo dataset.
						memset( barcodeData, 0, sizeof( BarcodeData_t ) );
						barcodeData->rotorId = demoResult->rotorId;
						barcodeData->rotorFormat = demoResult->rotorFormat;
						barcodeData->rotorMfgLot.lsdOfYear = demoResult->lotNum.lsdOfYear;
						barcodeData->rotorMfgLot.weekOfYear = demoResult->lotNum.weekOfYear;
						barcodeData->rotorMfgLot.dayOfWeek = demoResult->lotNum.dayOfWeek;
						strcpy( barcodeData->rotorMfgLot.code, demoResult->lotNum.code );
						runInfo.barcodeRead = true;
						errNum = SERR_NONE;
			
						isRotor1 = false;
			
						sprintf( rocFileName, "%s/%d.%d.roc",
								 ROC_FILE_DIRECTORY,
								 barcodeData->rotorId,
								 barcodeData->rotorFormat );
			
			
						// We want the name from the demo file, not the one from the ROC file, on the
						// results printouts and displays.
						strcpy( rotorConfig->rotorPrinterName, demoResult->rotorName );
							
						// Send the RIS file to the Engine.
						errNum = SendRisFile( DEMO_ROTOR_ID );
					}

 					strcpy( runInfo.rotorName, rotorConfig->rotorPrinterName );
				}

			}
			gotoNextState = true;
			break;

		case MSG_ANALYSIS_TRIGGER:
			gotoNextState = true;
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	// Error, so abort.
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Rotor 1 or rotor 100 on the second or later go-around.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}
	
	// We're done here, so determine the next state.
	else if ( gotoNextState )
	{
		if ( settingsData->alternateIdEnabled )
		{
			retVal = STATE_ENTER_ALTERNATE_ID;
		}
		else if ( settingsData->sampleIdEnabled )
		{
			retVal = STATE_ENTER_SAMPLE_ID;
		}
		else if ( settingsData->doctorIdEnabled )
		{
			retVal = STATE_ENTER_DOCTOR_ID;
		}
		else if ( settingsData->locationEnabled )
		{
			retVal = STATE_ENTER_LOCATION;
		}
		else if ( settingsData->phoneNumberEnabled )
		{
			retVal = STATE_ENTER_PHONE_NUMBER;
		}
		else if ( settingsData->admissionOwnerIdEnabled )
		{
			retVal = STATE_ENTER_ADMISSION_OWNER_ID;
		}
		else if ( settingsData->dobEnabled && (runInfo.patientControl == 'P') )
		{
			retVal = STATE_ENTER_DOB;
		}
		else if ( settingsData->dovEnabled && (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) )
		{
			retVal = STATE_ENTER_DOV;
		}
		else if ( analysisDone )
		{
			retVal = STATE_SAVING_RESULTS;
		}
		else
		{
			retVal = STATE_ANALYSIS_IN_PROCESS;
		}
	}

	return retVal;
}

/*
 * class StateHomeReady
 */
 
void
StateHomeReady::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_HOME ] );
	
	memset( &msg, 0, sizeof( GuiReceiveMsg_t ) );
	
	// Display the appropriate window title.
	if ( systemData->inManufacturingMode || systemData->inServiceMode )
	{
		msg.msgId = MSG_GUI_MIDDLE_HOME_READY_MFG;
	}
	else
	{
		msg.msgId = MSG_GUI_MIDDLE_HOME_READY;
	}

	// Analyze button is always displayed.
	strcpy( msg.data.middleHomeReady.buttonText[0], strings[ MSTR_ANALYZE ] );
	msg.data.middleHomeReady.buttonHighlighted[0] = 1;

	// If the user changed the comm interface to something other than ASTM, or turned off ASTM orders,
	// and we've got any orders in the queue, we need to clear the queue.
	if ( ((settingsData->comm.interface != COMM_ASTM) && (orderQueue.NumOrders() > 0)) ||
	     (settingsData->astmOrderMethod == ASTM_ORDERS_DISABLED) )
	{
		orderQueue.Clear();
	}
	
	// If we've got orders in the order queue, display the Pending Order button.
	if ( orderQueue.NumOrders() > 0 )
	{
		strcpy( msg.data.middleHomeReady.buttonText[1], strings[ MSTR_PENDING_ORDER ] );
		pendingOrdersButtonDisplayed = true;
	}
	else
	{
		pendingOrdersButtonDisplayed = false;
	}

	// If in Research mode, we may have an uploaded rotor.
	if ( systemData->inResearchMode == true )
	{
		if ( uploadedRotorName[0] != '\0' )
		{
			strcpy( msg.data.middleHomeReady.buttonText[2], uploadedRotorName );
		}

		// We've displayed the uploaded rotor name, if there is one, so clear the flag to keep us from getting
		// triggered again in StateHomeReady::ProcessMessage.
		uploadedRotorReceived = false;
	}
	
	SendGuiMessage( &msg, sizeof( GuiMiddleHomeReadyData_t ) );
	
	SetBottomDateTime();

	// Make sure the stack is clear, then set the home state to this state.
	stateStack.Home();
	stateStack.Push( STATE_HOME_READY );
	
	// Reset the rotor 100 data.
	memset( &runInfoRotor100, 0, sizeof( RunInfo_t ) );
	
	// Clear the saved rotor ID entered in the Lockout ID screen.
	memset( lockoutOperatorId, 0, sizeof( lockoutOperatorId ) );

	// Clear the uploaded order ID flags.
	uploadedOrderSelected = 0;	// Set when the order is selected.
	uploadedOrderInProcess = 0;	// Set when analysis of the order begins.
	
	// Clear flags for rotor 100 and VS rotor.
	isRotor100 = isVsRotor = false;

	systemData->systemStatus = SYSTEM_STATUS_READY;
}

int
StateHomeReady::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_HOME_READY;
	int		idata = (int)*data;
	time_t	t;
	bool	controlExpired;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:

			// Determine our eventual destination - either immediate or after a warning screen
			// for over temp or expired control.
			switch ( idata )
			{
				case 0:	// Analyze
					continueState = STATE_OPEN_DRAWER_WITHOUT_ROTOR;
				break;

				case 1:	// Pending Order
					continueState = STATE_SELECT_PENDING_ORDER;
					break;
					
				case 2:	// Uploaded rotor.
					isVsRotor = true;
					continueState = STATE_OPEN_DRAWER_WITHOUT_ROTOR;
					break;
			}

			// Determine if the control is expired.
			t = expirys.GetControlExpiration();
			controlExpired = ((factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) && (t > 0) && (t < time( 0 )));

			// If lockout enabled, go to the lockout state.
			if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
			{
				stateStack.Push( STATE_HOME_READY );
				retVal = STATE_LOCKOUT_WITHOUT_ROTOR;
			}

			// If the control is expired or the instrument temperature is too high, display a warning,
			// then proceed to the next state.
			else if ( controlExpired || isOvertemp )
			{
				// Push the eventual destination determined above.
				stateStack.Push( continueState );
				retVal = STATE_WARNING;

				if ( isOvertemp )
				{
					analysisWarningQueue.Enqueue( SERR_NEED_COOL_DOWN_TIME );
				}
				if ( controlExpired )
				{
					analysisWarningQueue.Enqueue( SERR_CONTROL_EXPIRED );
				}
			}
			
			// All is OK, so proceed to the selected state.
			else
			{
				retVal = continueState;
			}
			break;

		case MSG_GUI_OUT_RECALL:
			SendRecallMessage( MSG_RECALL_GOTO_FOREGROUND );
			retVal = STATE_HOME_READY_BACKGROUND;
			break;

		case MSG_GUI_OUT_SETTINGS:
			SendSettingsMessage( MSG_SETTINGS_GOTO_FOREGROUND );
			retVal = STATE_HOME_READY_BACKGROUND;
			break;

		case MSG_GUI_OUT_MANUFACTURING:
			SendStartupMessage( MSG_STARTUP_GOTO_AMS );
			break;

		default:
			ProcessOtherMessages( msgId, data );
			
			// If we got an uploaded rotor, an order was received by ASTM, or an order was
			// cancelled by ASTM and it was the last order in the queue, reset the screen
			// to display the corresponding button.
			if ( (uploadedRotorReceived == true) ||
				 (!pendingOrdersButtonDisplayed && orderQueue.NumOrders() > 0) ||
				 (pendingOrdersButtonDisplayed && orderQueue.NumOrders() == 0) )
			{
				Enter();
			}
			break;
	}

	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}

	// If we're not staying in the Home screen, set the analyzer status to Busy.
	if ( retVal != STATE_HOME_READY )
	{
		systemData->systemStatus = SYSTEM_STATUS_BUSY;
	}
	
	return retVal;
}

/*
 * class StateHomeReadyBackground
 */
 
void
StateHomeReadyBackground::
Enter()
{
	stateStack.Home();	// Make sure the stack is clear.

	systemData->systemStatus = SYSTEM_STATUS_READY;
}

int
StateHomeReadyBackground::
ProcessMessage( int msgId, char * data )
{
	int 				retVal = STATE_HOME_READY_BACKGROUND;
	
	switch ( msgId )
	{
		case MSG_ANALYSIS_GOTO_FOREGROUND:
			GuiReceiveMsg_t msg;
	
			// Inform the GUI that we want input now.
			msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
			strcpy( msg.data.inputReceiver.name, ANALYSIS_QUEUE_NAME );
			SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );

			// If we've gotten a shutdown message, we need to go to the Critical
			// Error screen.
			if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
				 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			else
			{
				retVal = STATE_HOME_READY;
			}
			break;

		default:
			// If we've already gotten a shutdown message, we ignore everything
			// else except the message to go to the foreground.  
			if ( (systemError.GetAction( analyzeError ) != SERR_ACTION_SHUTDOWN) &&
				 (systemError.GetAction( analyzeError ) != SERR_ACTION_RESTART) )
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
	}

	
	return retVal;
}

/*
 * class StateHomeWarming
 */
 
void
StateHomeWarming::
Enter()
{
	GuiReceiveMsg_t msg;
	static bool		firstTime = true;

	// Initialize the bottom date/time strings.
	SetBottomDateTimeStrings();

	// If there's a rotor present at start-up, we need to move to the
	// RotorPresentWarning state.
	if ( systemData->rotorPresent )
	{
		SendAnalysisMessage( MSG_ANALYSIS_TRIGGER );
	}
	
	else
	{
		SetTopLabel( strings[ TSTR_HOME ] );
		
		if ( systemData->inManufacturingMode || systemData->inServiceMode )
		{
			msg.msgId = MSG_GUI_MIDDLE_HOME_WARMING_MFG;
		}
		else
		{
			msg.msgId = MSG_GUI_MIDDLE_HOME_WARMING;
		}
	
		strcpy( msg.data.middleHomeWarming.progBarText, strings[ MSTR_WARMING ] );
		if ( firstTime )
		{
			progBarValue = 0;
	
			// If in demo mode or the system config says don't wait for the heater,
			// send a message to trigger a change to HomeReady.
			if ( systemData->inDemoMode || !systemData->waitForHeater )
			{
				SendAnalysisMessage( MSG_ANALYSIS_TRIGGER );
			}
			firstTime = false;
		}
		msg.data.middleHomeWarming.progBarValue = progBarValue;
		SendGuiMessage( &msg, sizeof( GuiMiddleHomeWarmingData_t ) );
	
		SetBottomDateTime();
	}
	
	stateStack.Home();	// Make sure the stack is clear.
}

int
StateHomeWarming::
ProcessMessage( int msgId, char * data )
{
	AnalysisTemperatureData_t *	temperatureData;
	int							retVal = STATE_HOME_WARMING;
	int							currentTemp;
	
	switch ( msgId )
	{
		case MSG_ANALYSIS_TRIGGER:
			if ( systemData->rotorPresent )
			{
				retVal = STATE_ROTOR_PRESENT_WARNING;
				systemData->rotorPresent = false;
			}
			else
			{
				retVal = STATE_HOME_READY;
			}
			break;

		case MSG_ANALYSIS_TEMPERATURE:
			temperatureData = (AnalysisTemperatureData_t *)data;
			GuiReceiveMsg_t msg;
			if ( systemData->inManufacturingMode || systemData->inServiceMode )
			{
				msg.msgId = MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS;
			}
			else
			{
				msg.msgId = MSG_GUI_UPDATE_HOME_WARMING_PROGRESS;
			}
			currentTemp = (int)round( temperatureData->temperature );
			progBarValue = (currentTemp - 23) * 8;

			// Make sure we have some indication that things are going on.
			if ( progBarValue < 5 )
			{
				progBarValue = 5;
			}
			msg.data.middleHomeWarming.progBarValue = progBarValue;
			SendGuiMessage( &msg, sizeof( GuiMiddleHomeWarmingData_t ) );
			break;
			
		case MSG_ANALYSIS_WARMUP_DONE:
			LogDebug( "Warmup done received" );
			retVal = STATE_HOME_READY;
			break;

		case MSG_GUI_OUT_RECALL:
			SendRecallMessage( MSG_RECALL_GOTO_FOREGROUND );
			retVal = STATE_HOME_WARMING_BACKGROUND;
			break;

		case MSG_GUI_OUT_SETTINGS:
			SendSettingsMessage( MSG_SETTINGS_GOTO_FOREGROUND );
			retVal = STATE_HOME_WARMING_BACKGROUND;
			break;
		
		case MSG_GUI_OUT_MANUFACTURING:
			SendStartupMessage( MSG_STARTUP_GOTO_AMS );
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateHomeWarmingBackground
 */
 
void
StateHomeWarmingBackground::
Enter()
{
	stateStack.Home();	// Make sure the stack is clear.
}

int
StateHomeWarmingBackground::
ProcessMessage( int msgId, char * data )
{
	AnalysisTemperatureData_t *	temperatureData;
	GuiReceiveMsg_t				msg;
	int							currentTemp;

	int retVal = STATE_HOME_WARMING_BACKGROUND;

	switch ( msgId )
	{
		case MSG_ANALYSIS_GOTO_FOREGROUND:
	
			// Inform the GUI that we want input now.
			msg.msgId = MSG_GUI_SET_INPUT_RECEIVER;
			strcpy( msg.data.inputReceiver.name, ANALYSIS_QUEUE_NAME );
			SendGuiMessage( &msg, sizeof( GuiInputReceiverData_t ) );

			// If we've gotten a shutdown message, we need to go to the Critical
			// Error screen.
			if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
				 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			else
			{
				retVal = STATE_HOME_WARMING;
			}
			break;
		
		case MSG_ANALYSIS_TEMPERATURE:
			temperatureData = (AnalysisTemperatureData_t *)data;
			currentTemp = (int)round( temperatureData->temperature );
			progBarValue = (currentTemp - 23) * 8;
			break;
			
		case MSG_ANALYSIS_WARMUP_DONE:
			LogDebug( "Warmup done received" );
			retVal = STATE_HOME_READY_BACKGROUND;
			break;

		default:
			// If we've already gotten a critical stop error, we don't do anything
			// that might change the analyzeError flag.
			if ( (systemError.GetAction( analyzeError ) != SERR_ACTION_SHUTDOWN) &&
				 (systemError.GetAction( analyzeError ) != SERR_ACTION_RESTART) )
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
	}
	
	return retVal;
}

/*
 * class StateLoadRotor
 */

void
StateLoadRotor::
Enter()
{
	GuiReceiveMsg_t msg;
	
	if ( uploadedOrderSelected != 0 )
	{
		UploadedOrder_t	* orderInfo = orderQueue.GetEntryInfo( uploadedOrderSelected );
		
		SetTopLabel( strings[ TSTR_LOAD_ORDER ] );
	
		msg.msgId = MSG_GUI_MIDDLE_LOAD_ORDER;
		strcpy( msg.data.middleLoadOrder.orderLabel, orderInfo->orderLabel );
		sprintf( msg.data.middleLoadOrder.rotorName, " %s \n %s ",
				 orderInfo->rotorNameLine1,
				 orderInfo->rotorNameLine2 );
		msg.data.middleLoadOrder.fontColor = orderInfo->rotorNameFontColor;
		msg.data.middleLoadOrder.backgroundColor = orderInfo->rotorNameBackgroundColor;
		msg.data.middleLoadOrder.buttonText[0] = '\0';
		SendGuiMessage( &msg, sizeof( GuiMiddleLoadOrderData_t ) );
	}
	else
	{
		SetTopLabel( strings[ TSTR_LOAD_ROTOR ] );
	
		msg.msgId = MSG_GUI_MIDDLE_MESSAGE_INFO;
		strcpy( msg.data.middleMessage.message, strings[ MSTR_CLOSE_DRAWER_TO_ANALYZE ] );
		SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	}
	
	SetBottomButtons( strings[ BSTR_CLOSE ], true );

	timerStarted = false;
	if ( systemData->inResearchMode )
	{
		SetTimer( systemData->researchDrawerOpen );
		timerStarted = true;
	}
}

int
StateLoadRotor::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_LOAD_ROTOR;
	EngineCommMessageData_t *	engineCommMsg;

	switch ( msgId )
	{
		case MSG_GUI_OUT_INFO:			// Only if this is Load Rotor.
			retVal = STATE_GRAPHIC_INFO1;
			stateStack.Push( STATE_LOAD_ROTOR );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:	// Close.
			// Make sure the timer is off.
			SetTimer( -1 );
			retVal = STATE_CLOSE_DRAWER;
			break;
	
		case MSG_ANALYSIS_TIMER:
			SetTimer( -1 );
			retVal = STATE_CLOSE_DRAWER;
			break;
	
		// User manually closed the drawer.
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_CLOSE_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x10 )	// Drawer closed
				{
					if ( status & 0x40 )	// Rotor present
					{
						counterData->camCycleCount++;

						if ( settingsData->operatorIdEnabled )
						{
							retVal = STATE_ENTER_OPERATOR_ID;
						}
						else if ( uploadedOrderSelected != 0 )
						{
							retVal = STATE_ANALYSIS_IN_PROCESS;
						}
						else if ( settingsData->sampleTypeAdvanced )
						{
							retVal = STATE_SELECT_TYPE_ADVANCED;
						}
						else
						{
							retVal = STATE_SELECT_TYPE_EXPRESS;
						}
						StartAnalysis();
					}
					else
					{
						retVal = STATE_HOME_READY;
					}
				
					counterData->drawerCycleCount++;
					WriteCounters( counterData );
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	// If we just went into Research mode and the drawer timer hasn't been started,
	// start it.
	if ( systemData->inResearchMode && !timerStarted )
	{
		SetTimer( systemData->researchDrawerOpen );
	}

	return retVal;
}

/*
 * class StateLockoutCancel
 */
 
void
StateLockoutCancel::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_ENTER_ID_TO_CANCEL ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CANCEL ], true );
}

int
StateLockoutCancel::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_LOCKOUT_CANCEL;
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
			else if ( idata == 1 )	// Cancel, so get ID text from GUI.
			{
				msg.msgId = MSG_GUI_GET_ID_TEXT;
				SendGuiMessage( &msg, 0 );
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Cancel.
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
					analysisWarningQueue.Enqueue( SERR_UNAUTHORIZED_OPERATOR );
					stateStack.Push( STATE_LOCKOUT_CANCEL );
					retVal = STATE_WARNING;
				}
	
				// Operator ID expired.
				else if ( (expiry->expiration != 0) && (expiry->expiration < time( 0 )) )
				{
					analysisWarningQueue.Enqueue( SERR_OPERATOR_RENEWAL_REQUIRED );
					retVal = STATE_WARNING;
				}
	
				// Operator ID OK.
				else
				{
					idOk = true;
				}
			}
			if ( idOk )
			{
				CancelAnalysis();
				retVal = STATE_CANCELING;
				stateStack.Pop();	// Discard return state.
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateLockoutWithRotor
 */
 
void
StateLockoutWithRotor::
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
StateLockoutWithRotor::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_LOCKOUT_WITH_ROTOR;
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
					analysisWarningQueue.Enqueue( SERR_UNAUTHORIZED_OPERATOR );
					stateStack.Push( STATE_LOCKOUT_WITH_ROTOR );
					retVal = STATE_WARNING;
				}
	
				// Operator ID expired.
				else if ( (expiry->expiration != 0) && (expiry->expiration < time( 0 )) )
				{
					analysisWarningQueue.Enqueue( SERR_OPERATOR_RENEWAL_REQUIRED );
					retVal = STATE_WARNING;
				}
	
				// Operator ID OK.
				else
				{
					idOk = true;
				}
			}
			if ( idOk )
			{
				retVal = STATE_OPEN_DRAWER_WITH_ROTOR;
				strncpy( runInfo.operatorId, sdata, sizeof( runInfo.operatorId ) );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateLockoutWithoutRotor
 */
 
void
StateLockoutWithoutRotor::
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
StateLockoutWithoutRotor::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_LOCKOUT_WITHOUT_ROTOR;
	GuiReceiveMsg_t	msg;
	int				idata = *(int *)data;
	char *			sdata = (char *)data;
	Expiry_t *		expiry;
	time_t			t;
	bool			controlExpired;
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
					analysisWarningQueue.Enqueue( SERR_UNAUTHORIZED_OPERATOR );
					stateStack.Push( STATE_LOCKOUT_WITHOUT_ROTOR );
					retVal = STATE_WARNING;
				}
	
				// Operator ID expired.
				else if ( (expiry->expiration != 0) && (expiry->expiration < time( 0 )) )
				{
					analysisWarningQueue.Enqueue( SERR_OPERATOR_RENEWAL_REQUIRED );
					retVal = STATE_WARNING;
				}
	
				// Operator ID OK.
				else
				{
					idOk = true;
				}
			}
			
			if ( idOk )
			{
				// Save the ID so we don't ask for it again if enableOperatorId is true.
				strncpy( lockoutOperatorId, sdata, sizeof( lockoutOperatorId ) );
	
				// Determine whether the control has expired.
				t = expirys.GetControlExpiration();
				controlExpired = ((factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) && (t > 0) && (t < time( 0 )));
		
				// Warning for control expired or overtemperature. Display the warning and then
				// proceed to the state selected in HomeReady.
				if ( controlExpired || isOvertemp )
				{
					stateStack.Push( continueState );
					retVal = STATE_WARNING;
			
					if ( isOvertemp )
					{
						analysisWarningQueue.Enqueue( SERR_NEED_COOL_DOWN_TIME );
					}
					if ( controlExpired )
					{
						analysisWarningQueue.Enqueue( SERR_CONTROL_EXPIRED );
					}
				}
				
				// Proceed to the state selected in HomeReady.
				else
				{
					retVal = continueState;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}


/*
 * class StateOpenDrawerWithRotor
 */
 
void
StateOpenDrawerWithRotor::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_OPENING_DRAWER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();
	
	SendDrawerMessage( 'O' );
}

int
StateOpenDrawerWithRotor::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_OPEN_DRAWER_WITH_ROTOR;
	EngineCommMessageData_t *	engineCommMsg;
	
	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_OPEN_JAM;
					LogError( "Analysis: drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x20 )	// Door open
				{
					retVal = STATE_REMOVE_ROTOR;
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
					retVal = STATE_HOME_READY;
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateOpenDrawerWithoutRotor
 */

void
StateOpenDrawerWithoutRotor::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_OPENING_DRAWER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();
	
	SendDrawerMessage( 'O' );
}

int
StateOpenDrawerWithoutRotor::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_OPEN_DRAWER_WITHOUT_ROTOR;
	EngineCommMessageData_t *	engineCommMsg;
	
	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_OPEN_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x20 )	// Drawer open
				{
					retVal = STATE_LOAD_ROTOR;
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
					retVal = STATE_HOME_READY;
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
	
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * StatePatientIdAuthentication
 */

void
StatePatientIdAuthentication::
Enter()
{
	GuiReceiveMsg_t msg;
	
// GLD
// Ideally, we'd have it display TSTR_ENTER_CONTROL_ID in the case of a control.
// How do we know?
	SetTopLabel( strings[ TSTR_ENTER_PATIENT_ID ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_ID;
	msg.data.middleEnterId.isPassword = 0;
	strcpy( msg.data.middleEnterId.initialValue, "" );
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterIdData_t ) );
		
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_DONE ], true );
}

int
StatePatientIdAuthentication::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_PATIENT_ID_AUTHENTICATION;
	int							idata = *(int *)data;
	char *						sdata = (char *)data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
			{
				analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
				retVal = STATE_WARNING;
			}
			else
			{
				switch ( idata )
				{
					case 0:		// Back
						retVal = stateStack.Pop();
						break;
					case 1:		// Done
						SendGuiMessage( MSG_GUI_GET_ID_TEXT );
						break;
				}
			}
			break;

		case MSG_GUI_OUT_ACTIVATE:	// Same as Done.
			if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
			{
				analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
				retVal = STATE_WARNING;
			}
			else
			{
				SendGuiMessage( MSG_GUI_GET_ID_TEXT );
			}
			break;

		case MSG_GUI_OUT_ID_TEXT:
			if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
			{
				analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
				retVal = STATE_WARNING;
			}
			else
			{
				if ( strcmp( (orderQueue.GetEntryInfo( uploadedOrderSelected ))->patientId, sdata ) == 0 )
				{
					// This signals that the order is in process, and so can no
					// longer be deleted.
					uploadedOrderInProcess = uploadedOrderSelected;
						
					retVal = STATE_OPEN_DRAWER_WITHOUT_ROTOR;
				}
				else
				{
					analysisWarningQueue.Enqueue( SERR_PATIENT_IDS_DONT_MATCH );
					retVal = STATE_WARNING;
				}
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	return retVal;
}

/*
 * class StateRemoveRotor
 */
 
void
StateRemoveRotor::
Enter()
{
	GuiReceiveMsg_t msg;
	
	// Initialize flags used for automatically closing the drawer in Research mode.
	timerStarted = false;
	drawerClosedAutomatically = false;
	
	SetTopLabel( strings[ TSTR_REMOVE_ROTOR ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_LOAD_ROTOR_TO_ANALYZE ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CLOSE ], true );

	// Start a timer thread if this is a rotor 1 or 100.  The timer will trigger an
	// automatic closing of the drawer after the set time.
	if ( isRotor1 )
	{
		SetTimer( systemData->burnInDrawerOpen );
		timerStarted = true;
		
		memcpy( &runInfoRotor1, &runInfo, sizeof( RunInfo_t ) );
	}
	
	else if ( isRotor100)
	{
		SetTimer( systemData->burnInDrawerOpen );
		timerStarted = true;
		
		memcpy( &runInfoRotor100, &runInfo, sizeof( RunInfo_t ) );
	}
	
	else
	{
		if ( systemData->inResearchMode )
		{
			SetTimer( systemData->researchDrawerOpen );
			timerStarted = true;
		}
		memset( &runInfoRotor100, 0, sizeof( RunInfo_t ) );
	}
}

int
StateRemoveRotor::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_REMOVE_ROTOR;
	EngineCommMessageData_t *	engineCommMsg;
	unsigned char				status;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			SetTimer( -1 );
			retVal = STATE_CLOSE_DRAWER;
			break;

		case MSG_ANALYSIS_TIMER:
			SetTimer( -1 );
			if ( systemData->inResearchMode && !isRotor1 && !isRotor100 )
			{
				drawerClosedAutomatically = true;
			}
			retVal = STATE_CLOSE_DRAWER;
			break;
	
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				SetTimer( -1 );

				status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_CLOSE_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x10 )	// Drawer closed
				{
					if ( status & 0x40 )	// Rotor present
					{
						counterData->camCycleCount++;
						if ( settingsData->operatorIdEnabled )
						{
							retVal = STATE_ENTER_OPERATOR_ID;
						}
						else if ( settingsData->sampleTypeAdvanced )
						{
							retVal = STATE_SELECT_TYPE_ADVANCED;
						}
						else
						{
							retVal = STATE_SELECT_TYPE_EXPRESS;
						}
						StartAnalysis();
					}
					else
					{
						retVal = STATE_HOME_READY;
					}
				
					counterData->drawerCycleCount++;
					WriteCounters( counterData );
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			
			// If we just went into Research mode and the drawer timer hasn't been started,
			// start it.
			if ( systemData->inResearchMode && !timerStarted && !isRotor1 && !isRotor100 )
			{
				SetTimer( systemData->researchDrawerOpen );
			}
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}


/*
 * class StateRotorPresentCloseDrawer
 */

void
StateRotorPresentCloseDrawer::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_CLOSING_DRAWER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );

	SendDrawerMessage( 'C' );
}

int
StateRotorPresentCloseDrawer::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_ROTOR_PRESENT_CLOSE_DRAWER;
	EngineCommMessageData_t *	engineCommMsg;

	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_CLOSE_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x10 )	// Drawer closed
				{
					if ( status & 0x40 )	// Rotor present
					{
						retVal = STATE_ROTOR_PRESENT_WARNING;
					}
					else
					{
						retVal = warmupComplete ? STATE_HOME_READY : STATE_HOME_WARMING;
					}
				
					counterData->drawerCycleCount++;
					WriteCounters( counterData );
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
//					retVal = STATE_HOME_READY;
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

/*
 * class StateRotorPresentOpenDrawer
 */
 
void
StateRotorPresentOpenDrawer::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( 0 );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_OPENING_DRAWER ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons();
	
	SendDrawerMessage( 'O' );
}

int
StateRotorPresentOpenDrawer::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_ROTOR_PRESENT_OPEN_DRAWER;
	EngineCommMessageData_t *	engineCommMsg;
	
	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				unsigned char status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_OPEN_JAM;
					LogError( "Analysis: drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x20 )	// Door open
				{
					retVal = STATE_ROTOR_PRESENT_REMOVE_ROTOR;
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
//					retVal = STATE_HOME_READY;
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}


/*
 * class StateRotorPresentRemoveRotor
 */
 
void
StateRotorPresentRemoveRotor::
Enter()
{
	GuiReceiveMsg_t msg;
	
	SetTopLabel( strings[ TSTR_REMOVE_ROTOR ] );
	
	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_PLEASE_REMOVE_ROTOR ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	SetBottomButtons( strings[ BSTR_CLOSE ], true );
}

int
StateRotorPresentRemoveRotor::
ProcessMessage( int msgId, char * data )
{
	int							retVal = STATE_ROTOR_PRESENT_REMOVE_ROTOR;
	EngineCommMessageData_t *	engineCommMsg;
	unsigned char				status;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = STATE_ROTOR_PRESENT_CLOSE_DRAWER;
			break;
	
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineCommMsg = (EngineCommMessageData_t *)data;
			if ( engineCommMsg->message[0] == 'D' )
			{
				status = engineCommMsg->message[8];
				if ( status & 0x80 )		// Drawer jammed
				{
					analyzeError = SERR_DRAWER_CLOSE_JAM;
					LogError( "drawer jammed" );
					retVal = STATE_ANALYZE_ERROR;
				}
				else if ( status & 0x10 )	// Drawer closed
				{
					if ( status & 0x40 )	// Rotor present
					{
						retVal = STATE_ROTOR_PRESENT_WARNING;
					}
					else
					{
						retVal = warmupComplete ? STATE_HOME_READY : STATE_HOME_WARMING;
					}
				
					counterData->drawerCycleCount++;
					WriteCounters( counterData );
				}
				else						// Unexpected
				{
					LogError( "unexpected drawer status: %x", status );
				}
			}
			else
			{
				ProcessOtherMessages( msgId, data );
			}
			break;
			
		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}


/*
 * class StateRotorPresentWarning
 */
 
void
StateRotorPresentWarning::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	
	guiMsg.msgId = MSG_GUI_WIN_WARNING1;

	strcpy( guiMsg.data.winError.topLabel, strings[ TSTR_WARNING ] );
	strcpy( guiMsg.data.winError.message, strings[ ESTR_PLEASE_REMOVE_ROTOR ] );
	strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );

	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateRotorPresentWarning::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_ROTOR_PRESENT_WARNING;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			retVal = STATE_ROTOR_PRESENT_OPEN_DRAWER;
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ EISTR_ROTOR_DETECTED ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ROTOR_PRESENT_WARNING );
			break;
				
		default:
			ProcessOtherMessages( msgId, data );

			// If a critical stop, we need to handle it immediately.
			if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
				 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			break;
	}
	
	return retVal;
}


/*
 * class StateSavingResults
 */

void
StateSavingResults::
Enter()
{
	CalcResultsMsgData_t	calcResultsMsgData;
	GuiReceiveMsg_t			msg;
	
	SetTopLabel( 0 );

	msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
	strcpy( msg.data.middleMessage.message, strings[ MSTR_SAVING_RESULTS ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
	
	msg.msgId = MSG_GUI_BOTTOM_NONE;
	SendGuiMessage( &msg, 0 );

	// We wait until here to make sure we've got all the necessary input before we tell
	// CalcResults to do the results calculations.
	calcResultsMsgData.patientType = runInfo.patientType;
	calcResultsMsgData.age = CalculateAge();
	calcResultsMsgData.race = runInfo.race;
	calcResultsMsgData.gender = runInfo.gender;
	SendCalcResultsMessage( MSG_CALC_RESULTS_ANALYSIS_DONE, &calcResultsMsgData );

	// Set a timer in case CalcResults fails to return a results message.
	SetTimer( timeoutData.resultsCalcComplete );
}

int
StateSavingResults::
ProcessMessage( int msgId, char * data )
{
	int					retVal = STATE_SAVING_RESULTS;
	time_t				startTime;
	int					elapsedTime;
	SystemErrorNumber_t	processResultsError;
	
	// Let ProcessOtherMessages handle any input first. This may include a return message from CalcResults.
	ProcessOtherMessages( msgId, data );

	// If an error indicates system shutdown, do so.
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	else if ( resultsComplete )
	{
		startTime = time(0);
		LogDebug( "Starting to save results" );
		processResultsError = ProcessResults();

		// Top priority for error reporting is a fatal error in ProcessResults.
		// Second priority is errors in CalcResults.
		// Lowest priority is non-fatal errors in ProcessResults.
		if ( (systemError.GetAction( processResultsError ) == SERR_ACTION_SHUTDOWN) ||
			 (systemError.GetAction( processResultsError ) == SERR_ACTION_RESTART) ||
			 (analyzeError == SERR_NONE) )
		{
			analyzeError = processResultsError;
		}
		
		if ( analyzeError != SERR_NONE )
		{
			retVal = STATE_ANALYZE_ERROR;
			// Push our eventual destination, in case it's just a Warning.
			stateStack.Push( STATE_ANALYSIS_COMPLETE );
		}
		else
		{
			retVal = STATE_ANALYSIS_COMPLETE;
			
			// If we were running an uploaded order, remove it from the order queue.
			if ( uploadedOrderInProcess != 0 )
			{
				orderQueue.Dequeue( uploadedOrderInProcess );
				uploadedOrderInProcess = 0;
				uploadedOrderSelected = 0;
			}

			// If need be, sleep so the "Saving Results..." message stays up
			// at least two seconds.
			elapsedTime = (int)difftime( time(0), startTime );
			if ( elapsedTime < 2 )
			{
				sleep( elapsedTime > 0 ? 2 - elapsedTime : 2 );
			}
		}
	}
	
	return retVal;
}

/*
 * class StateSelectGender
 */
 
void
StateSelectGender::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	
	SetTopLabel( strings[ TSTR_SELECT_GENDER ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	memset( &guiMsg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
	strcpy( guiMsg.data.middleButtons.buttonText[0], strings[ MSTR_UNKNOWN ] );
	strcpy( guiMsg.data.middleButtons.buttonText[1], strings[ MSTR_MALE ] );
	strcpy( guiMsg.data.middleButtons.buttonText[2], strings[ MSTR_FEMALE ] );
	if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
	{
		strcpy( guiMsg.data.middleButtons.buttonText[3], strings[ MSTR_NEUTERED ] );
		strcpy( guiMsg.data.middleButtons.buttonText[4], strings[ MSTR_SPAYED ] );
	}
	guiMsg.data.middleButtons.buttonHighlighted[0] = 1;
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleButtonsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CANCEL ], true );
}

int
StateSelectGender::
ProcessMessage( int msgId, char * data )
{
	int	retVal = STATE_SELECT_GENDER;
	int	idata = (int)*data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:	runInfo.gender = GENDER_UNKNOWN;	break;
				case 1:	runInfo.gender = GENDER_MALE;		break;
				case 2:	runInfo.gender = GENDER_FEMALE;		break;
				case 3:	runInfo.gender = GENDER_NEUTERED;	break;
				case 4:	runInfo.gender = GENDER_SPAYED;		break;
				default:									break;
			}
			
			if ( AgeEnabled() )
			{
				retVal = STATE_ENTER_AGE;
			}
			else if ( RaceEnabled() )
			{
				retVal = STATE_SELECT_RACE;
			}
			else
			{
				retVal = STATE_ENTER_PATIENT_CONTROL_ID;
			}
			stateStack.Push( STATE_SELECT_GENDER );
			LogDebug( "Gender selected" );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;

				case 1:		// Cancel
					if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
					{
						retVal = STATE_LOCKOUT_CANCEL;
					}
					else
					{
						retVal = STATE_CONFIRM_CANCEL;
					}
					stateStack.Push( STATE_SELECT_GENDER );
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}
	
	return retVal;
}

/*
 * class StateSelectPendingOrder
 */
 
void
StateSelectPendingOrder::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	int				i;
	
	SetTopLabel( strings[ TSTR_PENDING_ORDERS ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_LIST_COLORED;
	
	memset( &guiMsg.data.middleListColored, 0, sizeof( GuiMiddleButtonsData_t ) );

	orderQueue.GetDisplayList( 0, orderDisplayList );
	
	for ( i = 0; i < orderQueue.NumOrders() && i < 5; i++ )
	{
		strcpy( guiMsg.data.middleListColored.items[i], orderDisplayList[ i ].orderLabel );
		guiMsg.data.middleListColored.fontColor[ i ] = orderDisplayList[ i ].rotorNameFontColor;
		guiMsg.data.middleListColored.backgroundColor[ i ] = orderDisplayList[ i ].rotorNameBackgroundColor;
	}

	SendGuiMessage( &guiMsg, sizeof( GuiMiddleListColoredData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false );
	
	page = 0;
}

int
StateSelectPendingOrder::
ProcessMessage( int msgId, char * data )
{
	int				retVal = STATE_SELECT_PENDING_ORDER;
	int				idata = (int)*data;
	bool			doUpdate = false;
	int				i;
	GuiReceiveMsg_t guiMsg;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			if ( idata >= 0 && idata <= 4 )
			{
				uploadedOrderSelected = orderDisplayList[ idata ].orderId;
				
				// If the order was deleted while we were in this state, post a warning
				// message.
				if ( orderQueue.OrderWasDeleted( uploadedOrderSelected ) )
				{
					analysisWarningQueue.Enqueue( SERR_ORDER_WAS_CANCELED );
					retVal = STATE_WARNING;
				}

				// Order not deleted, so confirm the order.
				else
				{
					retVal = STATE_CONFIRM_ORDER;
				}

				// In either case, push the current state, so the Back button returns
				// us here.
				stateStack.Push( STATE_SELECT_PENDING_ORDER );
			}
			break;
	
		case MSG_GUI_OUT_UP:
			if ( page > 0 )
			{
				page--;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_DOWN:
			if ( orderQueue.NumOrders() > 5 * (page+1) )
			{
				page++;
				doUpdate = true;
			}
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			retVal = stateStack.Home();
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}

	if ( doUpdate )
	{
		orderQueue.GetDisplayList( page, orderDisplayList );
	
		guiMsg.msgId = MSG_GUI_MIDDLE_LIST_COLORED;

		memset( &guiMsg.data.middleListColored, 0, sizeof( GuiMiddleButtonsData_t ) );
		
		for ( i = 0; (page + i) < orderQueue.NumOrders() && i < 5; i++ )
		{
			strcpy( guiMsg.data.middleListColored.items[i], orderDisplayList[ i ].orderLabel );
			guiMsg.data.middleListColored.fontColor[ i ] = orderDisplayList[ i ].rotorNameFontColor;
			guiMsg.data.middleListColored.backgroundColor[ i ] = orderDisplayList[ i ].rotorNameBackgroundColor;
		}

		SendGuiMessage( &guiMsg, sizeof( GuiMiddleListColoredData_t ) );
	}

	return retVal;
}

/*
 * class StateSelectRace
 */
 
void
StateSelectRace::
Enter()
{
	GuiReceiveMsg_t guiMsg;
	
	SetTopLabel( strings[ TSTR_SELECT_RACE ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	memset( &guiMsg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
	strcpy( guiMsg.data.middleButtons.buttonText[0], strings[ MSTR_UNKNOWN ] );
	strcpy( guiMsg.data.middleButtons.buttonText[1], strings[ MSTR_WHITE ] );
	strcpy( guiMsg.data.middleButtons.buttonText[2], strings[ MSTR_BLACK ] );
	strcpy( guiMsg.data.middleButtons.buttonText[3], strings[ MSTR_ASIAN_PACIFIC_ISLANDER ] );
	strcpy( guiMsg.data.middleButtons.buttonText[4], strings[ MSTR_NATIVE_AMERICAN ] );
	strcpy( guiMsg.data.middleButtons.buttonText[5], strings[ MSTR_HISPANIC ] );

	guiMsg.data.middleButtons.buttonHighlighted[0] = 1;
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleButtonsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CANCEL ], true );
}

int
StateSelectRace::
ProcessMessage( int msgId, char * data )
{
	int	retVal = STATE_SELECT_RACE;
	int	idata = (int)*data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			switch ( idata )
			{
				case 0:	runInfo.race = RACE_UNKNOWN;				break;
				case 1:	runInfo.race = RACE_WHITE;					break;
				case 2:	runInfo.race = RACE_BLACK;					break;
				case 3:	runInfo.race = RACE_ASIAN_PACIFIC_ISLANDER;	break;
				case 4:	runInfo.race = RACE_NATIVE_AMERICAN;		break;
				case 5:	runInfo.race = RACE_HISPANIC;				break;
				default:											break;
			}
			
			retVal = STATE_ENTER_PATIENT_CONTROL_ID;
			stateStack.Push( STATE_SELECT_RACE );
			LogDebug( "Race selected" );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = stateStack.Pop();
					break;

				case 1:		// Cancel
					if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
					{
						retVal = STATE_LOCKOUT_CANCEL;
					}
					else
					{
						retVal = STATE_CONFIRM_CANCEL;
					}
					stateStack.Push( STATE_SELECT_RACE );
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}
	
	return retVal;
}

/*
 * class StateSelectTypeAdvanced
 */
 
void
StateSelectTypeAdvanced::
Enter()
{
	GuiReceiveMsg_t			msg;
	GuiMiddleListData_t *	data = &msg.data.middleList;
	int						i;

	activeRanges = GetActiveRefRanges( refRanges );

	SetTopLabel( strings[ TSTR_SELECT_TYPE ] );

	memset( &msg, 0, sizeof( GuiReceiveMsg_t ) );
	
	msg.msgId = MSG_GUI_MIDDLE_LIST;

	for ( i = 0; (i < 5) && (i < activeRanges->len); i++ )
	{
		strcpy( data->items[ i ][0], species[ activeRanges->list[ i ] ] );
	}
	data->isSelectable = 1;
	data->hasUpDown = 1;

	SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );

	if ( settingsData->operatorIdEnabled == true )
	{
		SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CANCEL ], false );
	}
	else
	{
		SetBottomButtons( strings[ BSTR_CANCEL ], false );
	}
	
	topItem = 0;
}

int
StateSelectTypeAdvanced::
ProcessMessage( int msgId, char * data )
{
	state_t 				retVal = STATE_SELECT_TYPE_ADVANCED;
	int						idata = *(int *)data;
	bool					doUpdate = false;

	switch ( msgId )
	{
		case MSG_GUI_OUT_LIST:
			runInfo.patientControl = 'P';
			runInfo.sampleType = activeRanges->list[ topItem + idata ];

			if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
			{
				runInfo.patientType = PATIENT_TYPE_UNSPECIFIED;
			}
			else
			{
				runInfo.patientType = species.GetPatientType( (SampleType_t)runInfo.sampleType );
			}
						
			if ( runInfo.sampleType >= STYPE_CONTROL &&
				 runInfo.sampleType <= STYPE_LAST_CONTROL )
			{
				runInfo.patientControl = 'C';
				runInfo.patientType = PATIENT_TYPE_CONTROL;
			}
			else
			{
				runInfo.patientControl = 'P';
			}
						 
			if ( GenderEnabled() )
			{
				retVal = STATE_SELECT_GENDER;
			}
			else if ( AgeEnabled() )
			{
				retVal = STATE_ENTER_AGE;
			}
			else if ( RaceEnabled() )
			{
				retVal = STATE_SELECT_RACE;
			}
			else
			{
				retVal = STATE_ENTER_PATIENT_CONTROL_ID;
			}
			stateStack.Push( STATE_SELECT_TYPE_ADVANCED );
			break;

		case MSG_GUI_OUT_UP:
			if ( topItem > 0 )
			{
				topItem -= 4;
				if ( topItem < 0 )
				{
					topItem = 0;
				}
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_DOWN:
			if ( activeRanges->len > topItem + 5 )
			{
				topItem += 4;
			}
			doUpdate = true;
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( (settingsData->operatorIdEnabled == true) && (idata == 0) )	// Back
			{
				retVal = stateStack.Pop();
			}
			else		// Cancel
			{
				retVal = STATE_CONFIRM_CANCEL;
				if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
				{
					retVal = STATE_LOCKOUT_CANCEL;
				}
				else
				{
					retVal = STATE_CONFIRM_CANCEL;
				}
				stateStack.Push( STATE_SELECT_TYPE_ADVANCED );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}

	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}

	else if ( doUpdate )
	{
		GuiReceiveMsg_t			msg;
		GuiMiddleListData_t *	data = &msg.data.middleList;
		int						i;

		memset( &msg, 0, sizeof( msg ) );
		msg.msgId = MSG_GUI_UPDATE_LIST;

		for ( i = 0; i < 5 && i < activeRanges->len - topItem; i++ )
		{
			strcpy( data->items[ i ][0], species[ activeRanges->list[ topItem + i ] ] );
		}
		data->isSelectable = 1;
		data->hasUpDown = 1;

		SendGuiMessage( &msg, sizeof( GuiMiddleListData_t ) );
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}

	return retVal;
}

/*
 * class StateSelectTypeExpress
 */
 
void
StateSelectTypeExpress::
Enter()
{
	GuiReceiveMsg_t			guiMsg;
	
	SetTopLabel( strings[ TSTR_SELECT_TYPE ] );

	guiMsg.msgId = MSG_GUI_MIDDLE_BUTTONS;
	
	memset( &guiMsg.data.middleButtons, 0, sizeof( GuiMiddleButtonsData_t ) );
	if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
	{
		strcpy( guiMsg.data.middleButtons.buttonText[0], strings[ SSTR_PATIENT ] );
		strcpy( guiMsg.data.middleButtons.buttonText[1], strings[ SSTR_CONTROL ] );
	}
	else
	{
		strcpy( guiMsg.data.middleButtons.buttonText[0], strings[ SSTR_DOG ] );
		strcpy( guiMsg.data.middleButtons.buttonText[1], strings[ SSTR_CAT ] );
		strcpy( guiMsg.data.middleButtons.buttonText[2], strings[ SSTR_HORSE ] );
		strcpy( guiMsg.data.middleButtons.buttonText[3], strings[ SSTR_OTHER ] );
	}
	guiMsg.data.middleButtons.buttonHighlighted[0] = 1;
	SendGuiMessage( &guiMsg, sizeof( GuiMiddleButtonsData_t ) );
	
	if ( settingsData->operatorIdEnabled == true )
	{
		SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_CANCEL ], false );
	}
	else
	{
		SetBottomButtons( strings[ BSTR_CANCEL ], false );
	}
}

int
StateSelectTypeExpress::
ProcessMessage( int msgId, char * data )
{
	int						retVal = STATE_SELECT_TYPE_EXPRESS;
	int						idata = (int)*data;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			runInfo.patientType = PATIENT_TYPE_UNSPECIFIED;
			if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
			{
				switch ( idata )
				{
					case 0:		// Patient
						runInfo.patientControl = 'P';
						runInfo.sampleType = STYPE_PATIENT;
						runInfo.patientType = PATIENT_TYPE_UNSPECIFIED;
						break;
					case 1:		// Control
						runInfo.patientControl = 'C';
						runInfo.sampleType = STYPE_CONTROL;
						runInfo.patientType = PATIENT_TYPE_CONTROL;
						break;
					default:
						break;
				}
			}
			else
			{
				switch ( idata )
				{
					case 0:
						runInfo.patientControl = 'P';
						runInfo.sampleType = STYPE_DOG;
						runInfo.patientType = PATIENT_TYPE_DOG_FEMALE;
						break;
					case 1:
						runInfo.patientControl = 'P';
						runInfo.sampleType = STYPE_CAT;
						runInfo.patientType = PATIENT_TYPE_CAT_MALE;
						break;
					case 2:
						runInfo.patientControl = 'P';
						runInfo.sampleType = STYPE_HORSE;
						runInfo.patientType = PATIENT_TYPE_HORSE;
						break;
					case 3:
						runInfo.patientControl = 'P';
						runInfo.sampleType = STYPE_OTHER;
						runInfo.patientType = PATIENT_TYPE_UNSPECIFIED;
						break;
					default:
						break;
				}
			}

			if ( GenderEnabled() )
			{
				retVal = STATE_SELECT_GENDER;
			}
			else if ( AgeEnabled() )
			{
				retVal = STATE_ENTER_AGE;
			}
			else if ( RaceEnabled() )
			{
				retVal = STATE_SELECT_RACE;
			}
			else
			{
				retVal = STATE_ENTER_PATIENT_CONTROL_ID;
			}
			stateStack.Push( STATE_SELECT_TYPE_EXPRESS );
			break;
	
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( (settingsData->operatorIdEnabled == true) && (idata == 0) )	// Back
			{
				retVal = stateStack.Pop();
			}
			else		// Cancel
			{
				if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
				{
					retVal = STATE_LOCKOUT_CANCEL;
				}
				else
				{
					retVal = STATE_CONFIRM_CANCEL;
				}
				stateStack.Push( STATE_SELECT_TYPE_EXPRESS );
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	if ( analyzeError != SERR_NONE )
	{
		retVal = STATE_ABORTING;
	}
	
	// Used for Rotor 1 and 100.
	if ( isRotor1 && rotor1RunsComplete )
	{
		retVal = STATE_ANALYSIS_COMPLETE;
	}
	else if ( allInputReceived )
	{
		retVal = STATE_ANALYSIS_IN_PROCESS;
	}
	
	return retVal;
}

/*
 * class StateAnalyzeError
 */
 
void
StateAnalyzeError::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	StringId_t			msgStr;
	const char *		topText;
	
	// Flush all file buffers before displaying the error message.
	fflush( 0 );

	// Get the message text.
	msgStr = systemError.GetScreenText( analyzeError );

	// Get the error action.
	action = systemError.GetAction( analyzeError );
	
	// Set the window type and top text.
	switch ( action )
	{
		case SERR_ACTION_WARNING:
			guiMsg.msgId = MSG_GUI_WIN_WARNING1;
			topText = strings[ TSTR_WARNING ];
			strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
			break;
		case SERR_ACTION_ABORT:
			guiMsg.msgId = MSG_GUI_WIN_ERROR;
			topText = strings[ TSTR_ERROR ];
			strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_CONTINUE ] );
			break;
		case SERR_ACTION_SHUTDOWN:
			guiMsg.msgId = MSG_GUI_WIN_CRITICAL_STOP;
			topText = strings[ TSTR_CRITICAL_STOP ];
			systemData->systemStatus = SYSTEM_STATUS_NOT_READY;
			break;
		case SERR_ACTION_RESTART:
			guiMsg.msgId = MSG_GUI_WIN_CRITICAL_STOP2;
			topText = strings[ TSTR_CRITICAL_STOP ];
			systemData->systemStatus = SYSTEM_STATUS_NOT_READY;
			strcpy( guiMsg.data.winError.buttonText, strings[ BSTR_RESTART ] );
		case SERR_ACTION_NONE:
		default:
			break;
	}

	strcpy( guiMsg.data.winError.topLabel, topText );

	// Add the error number to the beginning of the error message string.
	sprintf( guiMsg.data.winError.message, "%x\n%s", analyzeError, strings[ msgStr ] );

	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
}

int
StateAnalyzeError::
ProcessMessage( int msgId, char * data )
{
	int		retVal = STATE_ANALYZE_ERROR;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue/Restart
			if ( systemError.GetAction( analyzeError ) == SERR_ACTION_WARNING )
			{
				retVal = stateStack.Pop();
			}

			// Restart
			else if ( systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART )
			{
				// Restart is handled by Startup because it knows how to kill all
				// the programs (it has their process IDs).
				SendStartupMessage( MSG_STARTUP_RESTART );
			}

			// Abort
			else
			{
				if ( (settingsData->analysisLockoutEnabled) && !LockoutKeyInstalled() )
				{
					retVal = STATE_LOCKOUT_WITH_ROTOR;
					stateStack.Push( STATE_ANALYZE_ERROR );
				}
				else
				{
					retVal = STATE_OPEN_DRAWER_WITH_ROTOR;
				}
			}
			analyzeError = SERR_NONE;
			break;
	
		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( analyzeError ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_ANALYZE_ERROR );
			break;
				
		default:
			// Ignore all other input if we've reached a critical stop.
			if ( action != SERR_ACTION_SHUTDOWN )
			{
				ProcessOtherMessages( msgId, data );	// This may overwrite analyzeError.

				// If a critical stop, we need to handle it immediately.
				if ( systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN )
				{
					Enter();
				}
			}
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
//	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
//			switch ( idata )
//			{
//				case 0:
					retVal = stateStack.Pop();
//					break;

		default:
			// If we're not here from a critical stop error screen, process
			// other messages.
			if ( systemError.GetAction( analyzeError ) != SERR_ACTION_SHUTDOWN )
			{
				ProcessOtherMessages( msgId, data );

				// If a critical stop error came in, we handle it immediately.
				if ( systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN )
				{
					retVal = STATE_ANALYZE_ERROR;
				}
				break;
			}
	}
	
	return retVal;
}

/*
 * Class StateGraphicInfo1
 */

void
StateGraphicInfo1::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_INSERT_SAMPLE ] );
	
	msg.msgId = MSG_GUI_MIDDLE_GRAPHIC_INFO1;
	strcpy( msg.data.middleViewResults.text, strings[ ISTR_LOAD_ROTOR_1 ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_EXIT ], false, strings[ BSTR_NEXT ], true );
}

state_t
StateGraphicInfo1::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_GRAPHIC_INFO1;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Exit
					retVal = stateStack.Pop();
					break;
				case 1:		// Next
					retVal = STATE_GRAPHIC_INFO2;
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );

			if ( analyzeError != SERR_NONE )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			break;
	}
	
	return retVal;
}

/*
 * Class StateGraphicInfo2
 */

void
StateGraphicInfo2::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_INSERT_ROTOR ] );
	
	msg.msgId = MSG_GUI_MIDDLE_GRAPHIC_INFO2;
	strcpy( msg.data.middleViewResults.text, strings[ ISTR_LOAD_ROTOR_2 ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_EXIT ], false, strings[ BSTR_NEXT ], true );
}

state_t
StateGraphicInfo2::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_GRAPHIC_INFO2;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = STATE_GRAPHIC_INFO1;
					break;
				case 1:		// Exit
					retVal = stateStack.Pop();
					break;
				case 2:		// Next
					retVal = STATE_GRAPHIC_INFO3;
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );

			if ( analyzeError != SERR_NONE )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			break;
	}
	
	return retVal;
}

/*
 * Class StateGraphicInfo3
 */

void
StateGraphicInfo3::
Enter()
{
	GuiReceiveMsg_t	msg;

	SetTopLabel( strings[ TSTR_READ_RESULTS ] );
	
	msg.msgId = MSG_GUI_MIDDLE_GRAPHIC_INFO3;
	strcpy( msg.data.middleViewResults.text, strings[ ISTR_LOAD_ROTOR_3 ] );
	SendGuiMessage( &msg, sizeof( GuiMiddleViewResultsData_t ) );
	
	SetBottomButtons( strings[ BSTR_BACK ], false, strings[ BSTR_EXIT ], true );
}

state_t
StateGraphicInfo3::
ProcessMessage( int msgId, char * data )
{
	state_t retVal = STATE_GRAPHIC_INFO3;
	int		idata = ((GuiSendMsgData_t *)data)->idata;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			switch ( idata )
			{
				case 0:		// Back
					retVal = STATE_GRAPHIC_INFO2;
					break;
				case 1:		// Exit
					retVal = stateStack.Pop();
					break;
			}
			break;

		default:
			ProcessOtherMessages( msgId, data );

			if ( analyzeError != SERR_NONE )
			{
				retVal = STATE_ANALYZE_ERROR;
			}
			break;
	}
	
	return retVal;
}

/*
 * class StateWarning
 */

void
StateWarning::
Enter()
{
	GuiReceiveMsg_t		guiMsg;
	
	// We only get a new warning here if there isn't a current warning in-process.
	// This covers the case where we're returning from StateInfo.
	if ( analysisWarning == SERR_NONE )
	{
		analysisWarning = analysisWarningQueue.Dequeue();
	}

	guiMsg.msgId = MSG_GUI_WIN_WARNING1;
	strcpy( guiMsg.data.winWarning.topLabel, strings[ TSTR_WARNING ] );
	strcpy( guiMsg.data.winWarning.message, strings[ systemError.GetScreenText( analysisWarning ) ] );
	strcpy( guiMsg.data.winWarning.buttonText[0], strings[ BSTR_CONTINUE ] );
	
	SendGuiMessage( &guiMsg, sizeof( GuiWinWarningData_t ) );
	
	LogDebug( "Warning screen: %s", strings.English( systemError.GetScreenText( analysisWarning ) ) );
}

int
StateWarning::
ProcessMessage( int msgId, char * data )
{
	int	retVal = STATE_WARNING;

	switch ( msgId )
	{
		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Continue
			// No more warnings to display at this time.
			if ( analysisWarningQueue.IsEmpty() )
			{
				retVal = stateStack.Pop();

				// Reset the warning queue and the local warning variable.
				analysisWarningQueue.Reset();
				analysisWarning = SERR_NONE;
			}
			// Get the next warning and call Enter() to display it. (If we just
			// return, the Controller won't call Enter() since the state doesn't
			// change.
			else
			{
				analysisWarning = analysisWarningQueue.Dequeue();
				Enter();
			}
			LogDebug( "Continue pressed" );
			break;

		case MSG_GUI_OUT_INFO:
			infoText = strings[ systemError.GetInfoText( analysisWarning ) ];
			retVal = STATE_INFO;
			stateStack.Push( STATE_WARNING );
			break;

		default:
			ProcessOtherMessages( msgId, data );
			break;
	}
	
	// Handle a shutdown error that's just come in.
	if ( (systemError.GetAction( analyzeError ) == SERR_ACTION_SHUTDOWN) ||
		 (systemError.GetAction( analyzeError ) == SERR_ACTION_RESTART) )
	{
		retVal = STATE_ANALYZE_ERROR;
	}
	
	return retVal;
}

