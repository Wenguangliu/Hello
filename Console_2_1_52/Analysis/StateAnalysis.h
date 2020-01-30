#ifndef __STATEANALYSIS_H_INCLUDED
#define __STATEANALYSIS_H_INCLUDED

#include "AstmMessages.h"
#include "CommonDefines.h"
#include "State.h"
#include "BarcodeData.h"
#include "CounterData.h"
#include "Demo.h"
#include "Expirys.h"
#include "FactoryData.h"
#include "OrdersData.h"
#include "RawData.h"
#include "RefRanges.h"
#include "ResultsData.h"
#include "RocData.h"
#include "RotorBarcode.h"
#include "ParseCalibration.h"
#include "ResultsDb.h"
#include "RunInfo.h"
#include "SettingsData.h"
#include "SystemData.h"
#include "SystemError.h"
#include "TimeoutData.h"
#include "VersionData.h"

enum
{
	STATE_ABORTING = 0,
	STATE_ANALYSIS_COMPLETE,
	STATE_ANALYSIS_IN_PROCESS,
	STATE_ANALYZE_ERROR,
	STATE_CANCELING,
	STATE_CLOSE_DRAWER,
	STATE_CONFIRM_CANCEL,
	STATE_CONFIRM_CANCEL_ORDER,
	STATE_CONFIRM_ORDER,
	STATE_ENTER_AGE,
	STATE_ENTER_ADMISSION_OWNER_ID,
	STATE_ENTER_ALTERNATE_ID,
	STATE_ENTER_DOB,
	STATE_ENTER_DOCTOR_ID,
	STATE_ENTER_DOV,
	STATE_ENTER_LOCATION,
	STATE_ENTER_OPERATOR_ID,
	STATE_ENTER_PATIENT_CONTROL_ID,
	STATE_ENTER_PHONE_NUMBER,
	STATE_ENTER_SAMPLE_ID,
	STATE_GRAPHIC_INFO1,
	STATE_GRAPHIC_INFO2,
	STATE_GRAPHIC_INFO3,
	STATE_HOME_READY,
	STATE_HOME_READY_BACKGROUND,
	STATE_HOME_WARMING,
	STATE_HOME_WARMING_BACKGROUND,
	STATE_INFO,
	STATE_LOAD_ROTOR,
	STATE_LOCKOUT_CANCEL,
	STATE_LOCKOUT_WITH_ROTOR,
	STATE_LOCKOUT_WITHOUT_ROTOR,
	STATE_OPEN_DRAWER_WITH_ROTOR,
	STATE_OPEN_DRAWER_WITHOUT_ROTOR,
	STATE_PATIENT_ID_AUTHENTICATION,
	STATE_REMOVE_ROTOR,
	STATE_ROTOR_PRESENT_CLOSE_DRAWER,
	STATE_ROTOR_PRESENT_OPEN_DRAWER,
	STATE_ROTOR_PRESENT_REMOVE_ROTOR,
	STATE_ROTOR_PRESENT_WARNING,
	STATE_SAVING_RESULTS,
	STATE_SELECT_GENDER,
	STATE_SELECT_PENDING_ORDER,
	STATE_SELECT_RACE,
	STATE_SELECT_TYPE_ADVANCED,
	STATE_SELECT_TYPE_EXPRESS,
	STATE_WARNING,
		
	STATE_COUNT			// Number of states in this enumeration.
};

// Type for the ID of an uploaded order.
typedef time_t	OrderId_t;

typedef struct
{
	OrderId_t	orderId;
	char		orderLabel[20];
	unsigned	rotorNameFontColor;
	unsigned	rotorNameBackgroundColor;
} OrderDisplayList_t;

class OrderQueue
{
public:
	void				ProcessAstmOrder( UploadedOrder_t * order );
	void				Enqueue( UploadedOrder_t * order );
	void				Dequeue( OrderId_t orderId );
	OrderId_t			FindOrder( UploadedOrder_t * order );
	void				Clear( void );
	void				GetDisplayList( int page, OrderDisplayList_t * list );
	UploadedOrder_t	*	GetEntryInfo( OrderId_t orderId );
	int					NumOrders() { return ordersList->numOrders; };
	bool				OrderWasDeleted( OrderId_t orderId );
	void				SendAstmCancelMessage( OrderId_t orderId, char code );

private:
	void				SendAstmCancelMessage( UploadedOrder_t * order, char code );

	static OrdersList_t *	ordersList;
};

extern OrderId_t	uploadedOrderSelected;
extern OrderId_t	uploadedOrderInProcess;

class WarningQueue
{
public:
						WarningQueue() { Reset(); }
	void				Enqueue( SystemErrorNumber_t errNum );
	SystemErrorNumber_t	Dequeue();
	bool				IsEmpty() { return (index == last); };
	void				Reset() { last = 0; index = 0; };

private:
	SystemErrorNumber_t	list[20];
	int					index;
	int					last;
};

class StateAnalysis : public State
{
public:
	static void		ReadCalibrationFile();
	static void		ReadTimeoutFile();

protected:
	bool			AgeEnabled();
	int				CalculateAge();
	void			CancelAnalysis();
	void			CreateResultsDatabase();
	bool			GenderEnabled();
	void			ProcessOtherMessages( int msgId, char * data );
	SystemErrNum_t	ProcessResults();
	bool			RaceEnabled();
	SystemErrNum_t	ReadUploadedBarcodeFile();
	void			Rotor1GetCount();
	SystemErrNum_t	SaveResults( RotorResults_t * rotorResults, bool is655321 );
	void			SetTimer( int time );
	void			StartAnalysis();
	void			SendAMessage( char command );
	void			SendCalibrationFactorsMessage();
	void			SendDrawerMessage( char command );
	void			SendBarcodeMessage( char command );
	SystemErrNum_t	SendRisFile( int rotorId );
	void			SendTrigger( char arg );
	static void *	TimerThread( void * arg );
	void			UpdateBarcodeCalibrationData( unsigned char * message );

	static bool					analysisStarted;
	static bool					analysisDone;
	static SystemErrorNumber_t	analyzeError;
	static bool					analysisInProcess;		// Indicates that the Engine has started a run.
	static int					barcodeReadCount;
	static DemoResult_t *		demoResult;
	static bool					receivedRotorName;		// Flag used by AnalysisInProcess state.
	static bool					rotorNameValid;			// "
	static bool					isOvertemp;
	static int					progBarValue;
	static bool					resultsComplete;
	static bool					runCanceled;
	static RunInfo_t			runInfo;
	static RunInfo_t			runInfoRotor1;
	static RunInfo_t			runInfoRotor100;
	static const char *			infoText;
	static pthread_t			timerThread;
	static bool					warmupComplete;
	static WarningQueue			analysisWarningQueue;
	static Expirys				expirys;
	static TimeoutData_t		timeoutData;
	static bool					isRotor1;
	static bool					isRotor100;
	static bool					isVsRotor;
	static bool					allInputReceived;		// Indicates user has completed all input/selections.
	static bool					sampleTypeSelected;		// Indicates user has selected the sample type.
	static bool					rotor1RunsComplete;
	static long long			rotor1Counter;			// Counts down for the Rotor 1 ID.
	static long long			rotor100Counter;		// Counts up for the Rotor 100 ID.
	static char					lockoutOperatorId[ ID_LEN ];
	static char					uploadedRotorName[ UPLOADED_ROTOR_NAME_LEN ];
	static bool					uploadedRotorReceived;
	static unsigned long		num4214Errors;
	static OrderQueue			orderQueue;
	static int					continueState;
	static bool					drawerClosedAutomatically;

	static BarcodeData_t *		barcodeData;
	static CalibrationData_t *	calibrationData;
	static CounterData_t *		counterData;
	static ResultsDbChanged_t *	dbChanged;
	static FactoryData_t *		factoryData;
	static RawData_t *			rawData;
	static RefRangeData_t *		refRanges;
	static RotorConfig_t *		rotorConfig;
	static RotorResults_t *		rotorResults;
	static SettingsData_t *		settingsData;
	static SystemData_t *		systemData;
	static VersionData_t *		versionData;

private:
	void 			CreateResultsRecord( ResultsDbEntry_t * resultsDbEntry, RotorResults_t * rotorResults, bool * resultSuppressed );
	SystemErrNum_t	PrintAndTransmitResults( int ndx, bool resultSuppressed );
};

#endif	// __STATEANALYSIS_H_INCLUDED
