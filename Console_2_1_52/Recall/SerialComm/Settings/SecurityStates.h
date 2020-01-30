/*
 * FILENAME
 * 	SecurityStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the security states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SECURITYSTATES_H_INCLUDED
#define __SECURITYSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateSecuritySelect : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
private:
	bool	useLockoutInfo;
};

class StateSecurityLockoutSettings : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityControlSetDate : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif	// __SECURITYSTATES_H_INCLUDED
