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

#include <msgqueue.h>

#include "AnalysisMessages.h"
#include "AstmMessages.h"
#include "HL7Messages.h"
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
//#include "Analytes.h"
#include "PrinterMessages.h"
#include "MfMessages.h"
#include "ErrorProcessMessages.h"

typedef int state_t;

#define MAX_STATESTACK_SIZE		100
#define MAX_STRING_LENGTH       512

//extern AStrings strings;

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
	static void		InitEngineComm( const wchar_t * queueName );
	static void		InitGuiComm( const wchar_t * queueName );
	static void		SetDemoMode( bool flag );
	static void		SetResearchMode( bool flag );
	
protected:
	static HANDLE	InitMessageQueue( const wchar_t * queueName , BOOL read);
	
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
	static void		SendHeaterControlMessage( HeaterControlMsg_t *msg, int msgDataLen );
	static void		SendSerialCommMessage( SerialCommMsg_t * msg, int msgDataLen );
	static void		SendSerialCommMessage( SerialCommMsgId_t msgId );
	static void		SendStartupMessage( StartupMsg_t * msg, int msgDataLen );
	static void		SendStartupMessage( StartupMsgId_t msgId );
	static void		SendAstmMessage( AstmMsg_t * msg, int msgDataLen );
	static void		SendAstmMessage( AstmMsgId_t msgId );
	static void		SendHl7Message( Hl7Msg_t * msg, int msgDataLen );
	static void		SendHl7Message( Hl7MsgId_t msgId );
	static void		SendErrorProcessMessage( ErrorProcessMsg_t * msg, int msgDataLen );
	static void		SendErrorProcessMessage( ErrorProcessMsg_t msgId );
    static void     SendManufactureMessage (ManufactureMsg_t *msg, int msgDataLen );
    static void     SendManufactureMessage (ManufactureId_t msgId);

    static SystemErrNum_t     SendPrinterMessage (PrinterMsg_t *msg);

	void			SetTopLabel( const wchar_t * label );
	void			SetBottomButtons( const wchar_t * label0 = 0, bool selected0 = false,
									  const wchar_t * label1 = 0, bool selected1 = false,
									  const wchar_t * label2 = 0, bool selected2 = false );
	void			SetBottomDateTime();
	void			SetBottomDateTimeStrings(AStrings *strings);
    static char*    WstrToCstr (const wchar_t *wStr);
    static wchar_t* CstrToWstr (const char *cStr);

	static HANDLE			analysisQueue;
	static HANDLE			astmQueue;
    static HANDLE           hl7Queue;
    static HANDLE           errorProcessQueue;
	static HANDLE			calcResultsQueue;
	static HANDLE 			guiQueue;
	static HANDLE			engineCommQueue;
	static HANDLE			heaterControlQueue;
	static HANDLE 			recallQueue;
	static HANDLE			serialCommQueue;
	static HANDLE 			settingsQueue;
	static HANDLE			startupQueue;
    static HANDLE           printerQueue;
    static HANDLE           printerRespQueue;
    static HANDLE           manufactureQueue;
	
	static SettingsData_t *	settings;
	static StateStack		stateStack;
	//static AStrings			strings;
	static Analytes			analytes;
	static SystemError		systemError;
};

#endif	// __STATE_H_INCLUDED
