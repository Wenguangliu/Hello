/*
 * FILENAME
 * 	SettingsController.c
 * 
 * MODULE DESCRIPTION
 * 	Controller for the Settings process state machine.
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
#include "SettingsMessages.h"
#include "GuiMessages.h"
#include "Shmem.h"
#include "LogErr.h"

#include "SettingsStates.h"
#include "AdvancedSettingsStates.h"
#include "AnalyzerInfoStates.h"
#include "ArchiveStates.h"
#include "CdUpdateStates.h"
#include "CommStates.h"
#include "DateTimeStates.h"
#include "DisplayStates.h"
#include "FactoryDefaultsStates.h"
#include "LanguageStates.h"
#include "PrinterStates.h"
#include "RefRangesStates.h"
#include "SecurityStates.h"
#include "SecurityAdminStates.h"
#include "SecurityOpStates.h"
#include "SoundStates.h"
#include "UnitsStates.h"

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
	InitStates();							// Init the state.
	
	currentState = STATE_BACKGROUND;		// Go to the first state.
	states[ currentState ]->Enter();
	
	Receiver();								// Process messages and state changes.
	
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
	states[ STATE_BACKGROUND ]							= new StateBackground();
	states[ STATE_ENTER_ID ]							= new StateEnterId();
	states[ STATE_ENTER_OPERATOR_ID ]					= new StateEnterOperatorId();
	states[ STATE_ENTER_PASSWORD ]						= new StateEnterPassword();
	states[ STATE_ERROR ]								= new StateError();
	states[ STATE_ICON_MATRIX_1 ]						= new StateIconMatrix1();
	states[ STATE_ICON_MATRIX_2 ]						= new StateIconMatrix2();
	states[ STATE_INFO ]								= new StateInfo();
	states[ STATE_INVALID_DATE ]						= new StateInvalidDate();

	states[ STATE_ADVANCED_SELECT ]						= new StateAdvancedSelect();
	states[ STATE_ADVANCED_ALTERNATE_ID ]				= new StateAdvancedAlternateId();
	states[ STATE_ADVANCED_ANALYTE_OPTIONS ]			= new StateAdvancedAnalyteOptions();
	states[ STATE_ADVANCED_ASTM_ORDER_METHOD ]			= new StateAdvancedAstmOrderMethod();
	states[ STATE_ADVANCED_DATA_ENTRY_OPTIONS ]			= new StateAdvancedDataEntryOptions();
	states[ STATE_ANALYZERINFO_VIEW ]					= new StateAnalyzerInfoView();
	states[ STATE_ANALYZERINFO_PRINT ]					= new StateAnalyzerInfoPrint();
	states[ STATE_ARCHIVE_SELECT ]						= new StateArchiveSelect();
	states[ STATE_ARCHIVE_SEND ]						= new StateArchiveSend();
	states[ STATE_ARCHIVE_RETRIEVE ]					= new StateArchiveRetrieve();
	states[ STATE_ARCHIVE_CONFIRM_RETRIEVE ]			= new StateArchiveConfirmRetrieve();
	states[ STATE_ARCHIVE_SELECT_REPORT_EXPRESS ]		= new StateArchiveSelectReportExpress();
	states[ STATE_ARCHIVE_SELECT_REPORT_ADVANCED ]		= new StateArchiveSelectReportAdvanced();
	states[ STATE_ARCHIVE_SENDING ]						= new StateArchiveSending();
	states[ STATE_CDUPDATE_SELECT ]						= new StateCdUpdateSelect();
	states[ STATE_CDUPDATE_LOAD_SOFTWARE_CD ]			= new StateCdUpdateLoadSoftwareCd();
	states[ STATE_CDUPDATE_READING_SOFTWARE_CD ]		= new StateCdUpdateReadingSoftwareCd();
	states[ STATE_CDUPDATE_UPDATE_SOFTWARE ]			= new StateCdUpdateUpdateSoftware();
	states[ STATE_CDUPDATE_ERROR ]						= new StateCdUpdateError();
	states[ STATE_CDUPDATE_WARNING ]					= new StateCdUpdateWarning();
	states[ STATE_COMM_ASTM_CHECK ]						= new StateCommAstmCheck();
	states[ STATE_COMM_ASTM_ENTER_ANALYZER_IP ]			= new StateCommAstmEnterAnalyzerIp();
	states[ STATE_COMM_ASTM_ENTER_GATEWAY ]				= new StateCommAstmEnterGateway();
	states[ STATE_COMM_ASTM_ENTER_IP_MASK ]				= new StateCommAstmEnterIpMask();
	states[ STATE_COMM_ASTM_ENTER_SERVER_IP ]			= new StateCommAstmEnterServerIp();
	states[ STATE_COMM_ASTM_ENTER_SERVER_SOCKET ]		= new StateCommAstmEnterServerSocket();
	states[ STATE_COMM_ASTM_LAYER ]						= new StateCommAstmLayer();
	states[ STATE_COMM_ASTM_SELECT ]					= new StateCommAstmSelect();
	states[ STATE_COMM_ASTM_SETTINGS ]					= new StateCommAstmSettings();
	states[ STATE_COMM_ERROR ]							= new StateCommError();
	states[ STATE_COMM_SET_PROTOCOL ]					= new StateCommSetProtocol();
	states[ STATE_COMM_SELECT_PROTOCOL ]				= new StateCommSelectProtocol();
	states[ STATE_DATETIME_SET_DATE ]					= new StateDateTimeSetDate();
	states[ STATE_DATETIME_SET_TIME ]					= new StateDateTimeSetTime();
	states[ STATE_DISPLAY_SELECT ]						= new StateDisplaySelect();
	states[ STATE_DISPLAY_BACKLIGHT ]					= new StateDisplayBacklight();
	states[ STATE_DISPLAY_SCREEN_SAVER]					= new StateDisplayScreenSaver();
	states[ STATE_DISPLAY_POWER]						= new StateDisplayPower();
	states[ STATE_DISPLAY_TOUCH]						= new StateDisplayTouch();
	states[ STATE_FACTORY_DEFAULTS ]					= new StateFactoryDefaults();
	states[ STATE_LANGUAGE_SET_DISPLAY ]				= new StateLanguageSetDisplay();
	states[ STATE_LANGUAGE_SET_KEYBOARD ]				= new StateLanguageSetKeyboard();
	states[ STATE_PRINTER_SELECT ]						= new StatePrinterSelect();
	states[ STATE_PRINTER_CONFIGURE ]					= new StatePrinterConfigure();
	states[ STATE_PRINTER_TEST ]						= new StatePrinterTest();
	states[ STATE_PRINTER_SET_DEFAULT ]					= new StatePrinterSetDefault();
	states[ STATE_PRINTER_SELECT_REPORTS ]				= new StatePrinterSelectReports();
	states[ STATE_PRINTER_SET_RESULTS_COPIES ]			= new StatePrinterSetResultsCopies();
	states[ STATE_PRINTER_SET_IQC_COPIES ]				= new StatePrinterSetIqcCopies();
	states[ STATE_PRINTER_SET_ERROR_COPIES ]			= new StatePrinterSetErrorCopies();
	states[ STATE_REFRANGES_SELECT ]					= new StateRefRangesSelect();
	states[ STATE_REFRANGES_ORGANIZE ]					= new StateRefRangesOrganize();
	states[ STATE_REFRANGES_LIST_ANALYTES ]				= new StateRefRangesListAnalytes();
	states[ STATE_REFRANGES_LIST_SPECIES ]				= new StateRefRangesListSpecies();
	states[ STATE_REFRANGES_ADD_SPECIES ]				= new StateRefRangesAddSpecies();
	states[ STATE_REFRANGES_PICK_NAME ]					= new StateRefRangesPickName();
	states[ STATE_REFRANGES_LIST_ANALYTE_RANGES ]		= new StateRefRangesListAnalyteRanges();
	states[ STATE_REFRANGES_LIST_SPECIES_RANGES ]		= new StateRefRangesListSpeciesRanges();
	states[ STATE_REFRANGES_MODIFY_RANGE ]				= new StateRefRangesModifyRange();
	states[ STATE_REFRANGES_REMOVE_SPECIES ]			= new StateRefRangesRemoveSpecies();
	states[ STATE_REFRANGES_CONFIRM_REMOVAL ]			= new StateRefRangesConfirmRemoval();
	states[ STATE_REFRANGES_SELECT_SPECIES_LABEL ]		= new StateRefRangesSelectSpeciesLabel();
	states[ STATE_REFRANGES_DEFAULT_SPECIES ]			= new StateRefRangesDefaultSpecies();
	states[ STATE_REFRANGES_SELECT_ANALYTE_GENDER ]		= new StateRefRangesSelectAnalyteGender();
	states[ STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER ]	= new StateRefRangesSelectDemographicGender();
	states[ STATE_SECURITY_ADMIN_MANAGE ]				= new StateSecurityAdminManage();
	states[ STATE_SECURITY_ADMIN_NEW_ID ]				= new StateSecurityAdminNewId();
	states[ STATE_SECURITY_ADMIN_NEW_ENTER_PASSWORD ]	= new StateSecurityAdminNewEnterPassword();
	states[ STATE_SECURITY_ADMIN_NEW_CONFIRM_PASSWORD ]	= new StateSecurityAdminNewConfirmPassword();
	states[ STATE_SECURITY_ADMIN_ENTER_ID ]				= new StateSecurityAdminEnterId();
	states[ STATE_SECURITY_ADMIN_LIST_ID ]				= new StateSecurityAdminListId();
	states[ STATE_SECURITY_ADMIN_LIST_ALL ]				= new StateSecurityAdminListAll();
	states[ STATE_SECURITY_ADMIN_EDIT_SETTINGS ]		= new StateSecurityAdminEditSettings();
	states[ STATE_SECURITY_ADMIN_ENTER_PASSWORD ]		= new StateSecurityAdminEnterPassword();
	states[ STATE_SECURITY_ADMIN_CONFIRM_PASSWORD ]		= new StateSecurityAdminConfirmPassword();
	states[ STATE_SECURITY_ADMIN_DELETE_WARNING ]		= new StateSecurityAdminDeleteWarning();
	states[ STATE_SECURITY_CONTROL_SET_DATE ]			= new StateSecurityControlSetDate();
	states[ STATE_SECURITY_LOCKOUT_SETTINGS ]			= new StateSecurityLockoutSettings();
	states[ STATE_SECURITY_OP_MANAGE ]					= new StateSecurityOpManage();
	states[ STATE_SECURITY_OP_SEARCH ]					= new StateSecurityOpSearch();
	states[ STATE_SECURITY_OP_NEW_ID ]					= new StateSecurityOpNewId();
	states[ STATE_SECURITY_OP_ENTER_ID ]				= new StateSecurityOpEnterId();
	states[ STATE_SECURITY_OP_ENTER_DATE ]				= new StateSecurityOpEnterDate();
	states[ STATE_SECURITY_OP_LIST_ID ]					= new StateSecurityOpListId();
	states[ STATE_SECURITY_OP_LIST_EXPIRATION ]			= new StateSecurityOpListExpiration();
	states[ STATE_SECURITY_OP_LIST_ALL ]				= new StateSecurityOpListAll();
	states[ STATE_SECURITY_OP_EDIT_SETTINGS ]			= new StateSecurityOpEditSettings();
	states[ STATE_SECURITY_OP_SET_DATE ]				= new StateSecurityOpSetDate();
	states[ STATE_SECURITY_OP_DELETE_WARNING ]			= new StateSecurityOpDeleteWarning();
	states[ STATE_SECURITY_SELECT ]						= new StateSecuritySelect();
	states[ STATE_SOUND_SELECT ]						= new StateSoundSelect();
	states[ STATE_SOUND_CLICK ]							= new StateSoundClick();
	states[ STATE_SOUND_ALERT ]							= new StateSoundAlert();
	states[ STATE_SOUND_STATUS ]						= new StateSoundStatus();
	states[ STATE_UNITS_SELECT ]						= new StateUnitsSelect();
	states[ STATE_UNITS_GLOBAL ]						= new StateUnitsGlobal();
	states[ STATE_UNITS_GROUP_SELECT ]					= new StateUnitsGroupSelect();
	states[ STATE_UNITS_ELECTROLYTES ]					= new StateUnitsElectrolytes();
	states[ STATE_UNITS_ENZYMES ]						= new StateUnitsEnzymes();
	states[ STATE_UNITS_LIPIDS ]						= new StateUnitsLipids();
	states[ STATE_UNITS_MINERALS ]						= new StateUnitsMinerals();
	states[ STATE_UNITS_PROTEINS ]						= new StateUnitsProteins();
	states[ STATE_UNITS_LIST ]							= new StateUnitsList();
	states[ STATE_UNITS_SET ]							= new StateUnitsSet();
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
	 
	mqd_t queue = mq_open( SETTINGS_QUEUE_NAME, O_RDONLY );
	
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
