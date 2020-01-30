/*
 * FILENAME
 * 	SoundStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the sound settings states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SOUNDSTATES_H_INCLUDED
#define __SOUNDSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateSoundSelect : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSoundClick : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		systemSetting;
};

class StateSoundAlert : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		systemSetting;
	int		clickSetting;
};

class StateSoundStatus : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		systemSetting;
};

#endif	// __SOUNDSTATES_H_INCLUDED
