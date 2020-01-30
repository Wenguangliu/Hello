/*
 * FILENAME
 * 	RecallStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the states of the Recall state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __RECALLSTATES_H_INCLUDED
#define __RECALLSTATES_H_INCLUDED

#include <msgqueue.h>
//#include <stack>		// stack and deque cause std::_Bool definition errors
//#include <deque>
//#include <pthread.h>

#include "State.h"
#include "FactoryData.h"
#include "GuiMessages.h"
#include "ResultsDb.h"
#include "CreateDisplay.h"
#include "CounterData.h"
#include "SystemData.h"
#include "SystemErrorNums.h"
#include "Expirys.h"

extern AStrings     strings;
typedef enum
{
	STATE_BACKGROUND,
	STATE_CLINICAL_ARCHIVE_TRANSMITTING,
	STATE_CONFIRM_CANCEL_ARCHIVE,
	STATE_CONFIRM_CANCEL_TRANSMIT,
    STATE_CONFIRM_TRANSMIT_ALL,
	STATE_DATE_RANGE_END,
	STATE_DATE_RANGE_START,
	STATE_ENTER_DATE,
	STATE_ENTER_OPERATOR_ID,
	STATE_ENTER_PATIENT_CONTROL_ID,
	STATE_ERROR,
	STATE_INFO,
	STATE_NO_RESULTS,
	STATE_LIST,
	STATE_SEARCH_USING,
	STATE_SELECT_OUTPUT_TYPE,
	STATE_SELECT_RECALL_TYPE,
	STATE_SELECT_REPORT_TYPE,
	STATE_SENDING_REPORT,
	STATE_SERVICE_ARCHIVE,
	STATE_TRANSMIT_ALL,
	STATE_TRANSMIT_RANGE_QUEUED,
	STATE_TRANSMIT_RANGE_TRANSMITTING,
	STATE_VIEW_RESULT,
	STATE_WARNING,
	
	STATE_COUNT			// Number of states in this enumeration.
} RecallState_t;

typedef enum
{
	SORT_DATE,
	SORT_ID,
	SORT_PATIENT,
	SORT_CONTROL,
	SORT_NONE,
} RecallSortOrder_t;

typedef enum
{
	REPORT_NONE = 0,
	REPORT_ALL,
	REPORT_RESULTS,
	REPORT_IQC,
	REPORT_ERROR,
	
	REPORT_PRINT = 0x10,
	REPORT_TRANSMIT = 0x20,
} RecallReportType_t;

typedef struct
{
	FILETIME	start;
	FILETIME	end;
	char *	serNum;
	int		archiveSize;
} DateRange_t;

typedef enum
{
	DATE_RANGE_CLINICAL_ARCHIVE,
	DATE_RANGE_TRANSMIT,
} DateRangeMode_t;

class StateRecall : public State
{
protected:
	static void			Sort();
	static const char *	GetResultType( int index );
	static char *		GetResultDate( int index );
	void				ProcessOtherMessages( int msgId, char * data );
	static void			ReadIndexFile();
	static void			GetRadFileName( char * buff, int index );
	static void			ProcessDates( DateRange_t * dateRange );
	static void			InitDateRange();
	static bool			RadFilesFound( DateRange_t * dateRange );
	static bool			ResultsFound( DateRange_t * dateRange );
	static void *		TimerThread( void * arg );

	static ResultsDbIndex_t		resultsIndex;
	static int					listCount;		// Count of selected entries.
	static int					topEntry;
	static int					selectedItem;	// Index in db of selected list item.
	static bool					selectedItemIsError;
	static bool					selectedItemIsPatient;
	static int					lastRunIndex;	// Index in db of last-run result.
	static bool					lastRunIsError;
	static bool					lastRunIsPatient;
	static RecallReportType_t	reportType;
	static bool					printSelected;
	static bool					transmitSelected;
	static RecallSortOrder_t	sortOrder;
	static CounterData_t *		counterData;
	static FactoryData_t *		factoryData;
	static SystemData_t *		systemData;
	static SettingsData_t *		settingsData;
	static SystemErrNum_t		errNum;
	static const wchar_t *		infoText;
	static HANDLE				archiveThread;
	static bool					serviceArchiveDisabled;
	static DateRange_t			dateRange;
	static SYSTEMTIME			tm;
	static DateRangeMode_t		dateRangeMode;
	static HANDLE				timerThread;

private:
	static int				CompareDate( const void * a, const void * b );
	static int				CompareId( const void * a, const void * b );
	static int				ComparePatient( const void * a, const void * b );
	static int				CompareControl( const void * a, const void * b );
	
	static int				totalCount;		// Count of all entries.
	static int				patientCount;	// Count of patient entries.
	static int				controlCount;	// Count of control entries.
};

class StateBackground : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	HANDLE	analysisQueue;
};

class StateClinicalArchiveTransmitting : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};


class StateConfirmCancelArchive : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateConfirmCancelTransmit : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateConfirmTransmitAll : public StateRecall
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateDateRangeEnd : public StateRecall
{
public:
	void		Enter();
	state_t		ProcessMessage( int msgId, char * data );
};

class StateDateRangeStart : public StateRecall
{
public:
	void		Enter();
	state_t		ProcessMessage( int msgId, char * data );
};

class StateEnterDate : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		FindSelectedDate();
	
	int		month;
	int		year;
	int		day;
};

class StateEnterOperatorId : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	static Expirys	expirys;
	
};

class StateEnterPatientControlId : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	void	GetId();
	int		FindSelectedId( char * patientControlId );
};

class StateError : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateInfo : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateList : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	void	SetMiddleList();
};

class StateNoResults : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSearchUsing : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectOutputType : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectRecallType : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectReportType : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSendingReport : public StateRecall
{
public:
	void	Init();
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateServiceArchive : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateTransmitAll : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateTransmitRangeEnd : public StateRecall
{
public:
	void		Enter();
	state_t		ProcessMessage( int msgId, char * data );
	static void	SetDate();
};

class StateTransmitRangeQueued : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateTransmitRangeStart : public StateRecall
{
public:
	void		Enter();
	state_t		ProcessMessage( int msgId, char * data );
	static void	SetDate();
};

class StateTransmitRangeTransmitting : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	HANDLE	analysisQueue;
};

class StateViewResult : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateWarning : public StateRecall
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif // __RECALLSTATES_H_INCLUDED
