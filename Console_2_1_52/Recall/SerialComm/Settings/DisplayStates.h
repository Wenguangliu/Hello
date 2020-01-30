/*
 * FILENAME
 * 	DisplayStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the display settings states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __DISPLAYSTATES_H_INCLUDED
#define __DISPLAYSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateDisplaySelect : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateDisplayBacklight : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
};

class StateDisplayScreenSaver : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
};

class StateDisplayPower : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		setting;
};

class StateDisplayTouch : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif	// __DISPLAYSTATES_H_INCLUDED
