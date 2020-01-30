/*
 * FILENAME
 * 	LanguageStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the language states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __LANGUAGESTATES_H_INCLUDED
#define __LANGUAGESTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateLanguageSetDisplay : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateLanguageSetKeyboard : public StateSettings
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif	// __LANGUAGESTATES_H_INCLUDED
