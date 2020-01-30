/*
 * FILENAME
 * 	FactoryDefaultsStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the set factory defaults states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __FACTORYDEFAULTSSTATES_H_INCLUDED
#define __FACTORYDEFAULTSSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"
#include "CfgErr.h"

class StateFactoryDefaults : public StateSettings
{
public:
	void		Enter();
	state_t		ProcessMessage( int msgId, char * data );

private:
	CfgError_t	SetDefaults();
};

#endif	// __FACTORYDEFAULTSSTATES_H_INCLUDED
