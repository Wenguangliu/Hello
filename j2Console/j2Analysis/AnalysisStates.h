/*
 * FILENAME
 * 	AnalysisState.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the states of the Analysis state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ANALYSISSTATES_H_INCLUDED
#define __ANALYSISSTATES_H_INCLUDED

#include <msgqueue.h>

#include "StateAnalysis.h"
#include "SystemError.h"
#include "RefRanges.h"
#include "Species.h"

#undef  SIMU_DRAWER_TEST

class StateAborting : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateAnalysisComplete : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateAnalysisInProcess : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
#ifdef SIMU_DRAWER_TEST
    static void *   DebugTimerThread (void * arg);
#endif
};

class StateAnalyzeError : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	SystemErrorAction_t	action;
};

class StateCanceling : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCancelledSelection : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCloseDrawer : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateConfirmCancel : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateConfirmCancelOrder : public StateAnalysis
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateConfirmOrder : public StateAnalysis
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateEnterAge : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		ageUnits;
	int		ageValue;
};

class StateEnterAdmissionOwnerId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterAlternateId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterDob : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		month;
	int		year;
	int		day;
};

class StateEnterDoctorId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterDov : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		month;
	int		year;
	int		day;
};

class StateEnterLocation : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterOperatorId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterPatientControlId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterPhoneNumber : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateEnterSampleId : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateGraphicInfo1 : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateGraphicInfo2 : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateGraphicInfo3 : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateHomeReady : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
    bool    pendingOrdersButtonDisplayed;
};

class StateHomeReadyBackground : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateHomeWarming : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateHomeWarmingBackground : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateInfo : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateLoadRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
private:
    char timerStarted;
};

class StateLockoutCancel : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateLockoutWithRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateLockoutWithoutRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateOpenDrawerWithRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateOpenDrawerWithoutRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StatePatientIdAuthentication : public StateAnalysis
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};


class StateRemoveRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
    bool    timerStarted;
};

class StateRotorPresentCloseDrawer : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
class StateRotorPresentOpenDrawer : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
class StateRotorPresentRemoveRotor : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
class StateRotorPresentWarning : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
class StateSavingResults : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectGender : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectPendingOrder : public StateAnalysis
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );

private:
    OrderDisplayList_t orderDisplayList [5];
    int         page;
};

class StateSelectRace : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSelectTypeAdvanced : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	ActiveRangesList_t *	activeRanges;
	Species					species;
	int						topItem;
};

class StateSelectTypeExpress : public StateAnalysis
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateWarning : public StateAnalysis
{
public:
			StateWarning() { analysisWarning = SERR_NONE; };
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	SystemErrorNumber_t		analysisWarning;
};


#endif	// __ANALYSISSTATES_H_INCLUDED
