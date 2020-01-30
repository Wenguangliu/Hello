/*
 * FILENAME
 * 	SettingsStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SETTINGSSTATES_H_INCLUDED
#define __SETTINGSSTATES_H_INCLUDED

#include <mqueue.h>
#include <stack>
#include <deque>
#include <time.h>

#include "State.h"
#include "GuiMessages.h"
#include "SettingsData.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "RefRanges.h"
#include "Expirys.h"

typedef enum
{
	STATE_BACKGROUND,
	STATE_ENTER_ID,
	STATE_ENTER_OPERATOR_ID,
	STATE_ENTER_PASSWORD,
	STATE_ERROR,
	STATE_ICON_MATRIX_1,
	STATE_ICON_MATRIX_2,
	STATE_INFO,
	STATE_INVALID_DATE,
	
	STATE_ADVANCED_SELECT,
	STATE_ADVANCED_ALTERNATE_ID,
	STATE_ADVANCED_ANALYTE_OPTIONS,
	STATE_ADVANCED_ASTM_ORDER_METHOD,
	STATE_ADVANCED_DATA_ENTRY_OPTIONS,
	STATE_ANALYZERINFO_VIEW,
	STATE_ANALYZERINFO_PRINT,
	STATE_ARCHIVE_SELECT,
	STATE_ARCHIVE_SEND,
	STATE_ARCHIVE_RETRIEVE,
	STATE_ARCHIVE_CONFIRM_RETRIEVE,
	STATE_ARCHIVE_SELECT_REPORT_EXPRESS,
	STATE_ARCHIVE_SELECT_REPORT_ADVANCED,
	STATE_ARCHIVE_SENDING,
	STATE_CDUPDATE_SELECT,
	STATE_CDUPDATE_LOAD_SOFTWARE_CD,
	STATE_CDUPDATE_LOAD_CONTROLS_CD,
	STATE_CDUPDATE_READING_SOFTWARE_CD,
	STATE_CDUPDATE_UPDATE_SOFTWARE,
	STATE_CDUPDATE_UPDATE_CONTROLS,
	STATE_CDUPDATE_ERROR,
	STATE_CDUPDATE_WARNING,
	STATE_COMM_ASTM_CHECK,
	STATE_COMM_ASTM_ENTER_ANALYZER_IP,
	STATE_COMM_ASTM_ENTER_ANALYZER_SOCKET,
	STATE_COMM_ASTM_ENTER_GATEWAY,
	STATE_COMM_ASTM_ENTER_IP_MASK,
	STATE_COMM_ASTM_ENTER_SERVER_IP,
	STATE_COMM_ASTM_ENTER_SERVER_SOCKET,
	STATE_COMM_ASTM_LAYER,
	STATE_COMM_ASTM_SELECT,
	STATE_COMM_ASTM_SETTINGS,
	STATE_COMM_ERROR,
	STATE_COMM_SET_PROTOCOL,
	STATE_DATETIME_SET_DATE,
	STATE_DATETIME_SET_TIME,
	STATE_DISPLAY_SELECT,
	STATE_DISPLAY_BACKLIGHT,
	STATE_DISPLAY_SCREEN_SAVER,
	STATE_DISPLAY_POWER,
	STATE_DISPLAY_TOUCH,
	STATE_FACTORY_DEFAULTS,
	STATE_LANGUAGE_SET_DISPLAY,
	STATE_LANGUAGE_SET_KEYBOARD,
	STATE_PRINTER_SELECT,
	STATE_PRINTER_CONFIGURE,
	STATE_PRINTER_TEST,
	STATE_PRINTER_SET_DEFAULT,
	STATE_PRINTER_SELECT_REPORTS,
	STATE_PRINTER_SET_RESULTS_COPIES,
	STATE_PRINTER_SET_IQC_COPIES,
	STATE_PRINTER_SET_ERROR_COPIES,
	STATE_REFRANGES_SELECT,
	STATE_REFRANGES_ORGANIZE,
	STATE_REFRANGES_LIST_ANALYTES,
	STATE_REFRANGES_LIST_SPECIES,
	STATE_REFRANGES_ADD_SPECIES,
	STATE_REFRANGES_PICK_NAME,
//	STATE_REFRANGES_SET_AGE_RANGE,
	STATE_REFRANGES_LIST_ANALYTE_RANGES,
	STATE_REFRANGES_LIST_SPECIES_RANGES,
	STATE_REFRANGES_MODIFY_RANGE,
	STATE_REFRANGES_REMOVE_SPECIES,
	STATE_REFRANGES_CONFIRM_REMOVAL,
	STATE_REFRANGES_SELECT_SPECIES_LABEL,
	STATE_REFRANGES_DEFAULT_SPECIES,
	STATE_REFRANGES_SELECT_ANALYTE_GENDER,
	STATE_REFRANGES_SELECT_DEMOGRAPHIC_GENDER,
	STATE_SECURITY_ADMIN_MANAGE,
//	STATE_SECURITY_ADMIN_SEARCH,
	STATE_SECURITY_ADMIN_NEW_ID,
	STATE_SECURITY_ADMIN_NEW_ENTER_PASSWORD,
	STATE_SECURITY_ADMIN_NEW_CONFIRM_PASSWORD,
	STATE_SECURITY_ADMIN_ENTER_ID,
//	STATE_SECURITY_ADMIN_ENTER_DATE,
	STATE_SECURITY_ADMIN_LIST_ID,
//	STATE_SECURITY_ADMIN_LIST_EXPIRATION,
	STATE_SECURITY_ADMIN_LIST_ALL,
	STATE_SECURITY_ADMIN_EDIT_SETTINGS,
//	STATE_SECURITY_ADMIN_SET_DATE,
	STATE_SECURITY_ADMIN_ENTER_PASSWORD,
	STATE_SECURITY_ADMIN_CONFIRM_PASSWORD,
	STATE_SECURITY_ADMIN_DELETE_WARNING,
	STATE_SECURITY_CONTROL_SET_DATE,
	STATE_SECURITY_CONTROL_DATE_WARNING,
	STATE_SECURITY_LOCKOUT_SETTINGS,
	STATE_SECURITY_OP_MANAGE,
	STATE_SECURITY_OP_SEARCH,
	STATE_SECURITY_OP_NEW_ID,
	STATE_SECURITY_OP_ENTER_ID,
	STATE_SECURITY_OP_ENTER_DATE,
	STATE_SECURITY_OP_LIST_ID,
	STATE_SECURITY_OP_LIST_EXPIRATION,
	STATE_SECURITY_OP_LIST_ALL,
	STATE_SECURITY_OP_EDIT_SETTINGS,
	STATE_SECURITY_OP_SET_DATE,
	STATE_SECURITY_OP_DELETE_WARNING,
	STATE_SECURITY_SELECT,
	STATE_SOUND_SELECT,
	STATE_SOUND_CLICK,
	STATE_SOUND_ALERT,
	STATE_SOUND_STATUS,
	STATE_UNITS_SELECT,
	STATE_UNITS_GLOBAL,
	STATE_UNITS_GROUP_SELECT,
	STATE_UNITS_ELECTROLYTES,
	STATE_UNITS_ENZYMES,
	STATE_UNITS_LIPIDS,
	STATE_UNITS_MINERALS,
	STATE_UNITS_PROTEINS,
	STATE_UNITS_LIST,
	STATE_UNITS_SET,
	STATE_WARNING,

	STATE_COUNT			// Number of states in this enumeration.
} SettingsState_t;



class StateSettings : public State
{
protected:
	static void *	TimerThread( void * arg );
	
	static mqd_t				settingsQueue;
	static RefRangeData_t *		refRanges;
	static SettingsData_t *		settingsData;
	static FactoryData_t *		factoryData;
	static SystemData_t *		systemData;
	static const char *			infoText;
	static SystemErrorNumber_t	errNum;
	static struct tm			tm;
	static Expirys				expirys;
};

class StateBackground : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	mqd_t	analysisQueue;
};

class StateLogin : public StateSettings
{
protected:
	static Expiry_t *	expiry;
};

class StateEnterId : public StateLogin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterOperatorId : public StateLogin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterPassword : public StateLogin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateIconMatrix : public StateSettings
{
protected:
	state_t	SelectIcon( int index, state_t currentState );
};

class StateIconMatrix1 : public StateIconMatrix
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateIconMatrix2 : public StateIconMatrix
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateInfo : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateError : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateInvalidDate : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
#endif // __SETTINGSSTATES_H_INCLUDED
