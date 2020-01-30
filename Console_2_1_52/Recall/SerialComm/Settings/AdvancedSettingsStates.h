/*
 * FILENAME
 * 	AdvancedSettingsStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the advanced settings states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ADVANCEDSETTINGSSTATES_H_INCLUDED
#define __ADVANCEDSETTINGSSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateAdvancedSelect : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
	
	void	fred();
};

class StateAdvancedAnalyteOptions : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateAdvancedDataEntryOptions : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateAdvancedAlternateId : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		page;
};

class StateAdvancedAstmOrderMethod : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
	
#endif	// __ADVANCEDSETTINGSSTATES_H_INCLUDED
