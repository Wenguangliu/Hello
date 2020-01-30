/*
 * FILENAME
 * 	State.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the State parent class used by the state subclasses
 * 	in the Analysis, Recall and Settings state machines.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __STATE_H_INCLUDED
#define __STATE_H_INCLUDED

#include <mqueue.h>

#include "AnalysisMessages.h"
#include "AstmMessages.h"
#include "GuiMessages.h"
#include "RecallMessages.h"
#include "SettingsMessages.h"
#include "EngineCommMessages.h"
#include "CalcResultsMessages.h"
#include "HeaterControlMessages.h"
#include "SerialCommMessages.h"
#include "SettingsData.h"
#include "StartupMessages.h"
#include "AStrings.h"
#include "SystemError.h"
#include "Analytes.h"

typedef int state_t;

#define MAX_STATESTACK_SIZE		100

extern AStrings	strings;

class StateStack
{
public:
	StateStack() { top = -1; }
	void	Push( state_t state );
	state_t	Pop();
	state_t	Pop( state_t state );
	state_t	Home() { top = -1; return stack[0]; }

private:
	state_t	stack[ MAX_STATESTACK_SIZE ];
	int		top;
};

class State
{
public:
	virtual void	Enter() {};
	virtual state_t	ProcessMessage( int msgId, char * data ) { return -1; };
	static void		InitEngineComm( const char * queueName );
	static void		InitGuiComm( const char * queueName );
	static void		SetDemoMode( bool flag );
	static void		SetResearchMode( bool flag );

protected:
	static mqd_t	InitMessageQueue( const char * queueName );
	
	static void		SendAnalysisMessage( AnalysisMsg_t * msg, int msgDataLen );
	static void		SendAnalysisMessage( AnalysisMsgId_t msgId );
	static void		SendGuiMessage( GuiReceiveMsg_t * msg, int msgDataLen );
	static void		SendGuiMessage( GuiMsgId_t msgId );
	static void		SendRecallMessage( RecallMsg_t * msg, int msgDataLen );
	static void		SendRecallMessage( RecallMsgId_t msgId );
	static void		SendSettingsMessage( SettingsMsg_t * msg, int msgDataLen );
	static void		SendSettingsMessage( SettingsMsgId_t msgId );
	static void		SendEngineCommMessage( EngineCommReceiveMsg_t * msg, int msgDataLen );
	static void		SendEngineCommMessage( EngineCommMsgId_t msgId );
	static void		SendCalcResultsMessage( CalcResultsMsgId_t msgId );
	static void		SendCalcResultsMessage( CalcResultsMsgId_t msgId, CalcResultsMsgData_t * data );
	static void		SendHeaterControlMessage( HeaterControlMsgId_t msgId );
	static void		SendSerialCommMessage( SerialCommMsg_t * msg, int msgDataLen );
	static void		SendSerialCommMessage( SerialCommMsgId_t msgId );
	static void		SendStartupMessage( StartupMsg_t * msg, int msgDataLen );
	static void		SendStartupMessage( StartupMsgId_t msgId );
	static void		SendAstmMessage( AstmMsg_t * msg, int msgDataLen );
	static void		SendAstmMessage( AstmMsgId_t msgId );

	void			SetTopLabel( const char * label );
	void			SetBottomButtons( const char * label0 = 0, bool selected0 = false,
									  const char * label1 = 0, bool selected1 = false,
									  const char * label2 = 0, bool selected2 = false );
	void			SetBottomDateTime();
	void			SetBottomDateTimeStrings();

	static mqd_t			analysisQueue;
	static mqd_t			astmQueue;
	static mqd_t			calcResultsQueue;
	static mqd_t 			guiQueue;
	static mqd_t			engineCommQueue;
	static mqd_t			heaterControlQueue;
	static mqd_t 			recallQueue;
	static mqd_t			serialCommQueue;
	static mqd_t 			settingsQueue;
	static mqd_t			startupQueue;
	
	static SettingsData_t *	settings;
	static StateStack		stateStack;
//	static AStrings			strings;
	static Analytes			analytes;
	static SystemError		systemError;
};

#endif	// __STATE_H_INCLUDED
