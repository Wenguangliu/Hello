/*
 * FILENAME
 * 	DateTimeStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the date/time states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __DATETIMESTATES_H_INCLUDED
#define __DATETIMESTATES_H_INCLUDED

#include <time.h>

#include "State.h"
#include "SettingsStates.h"

class StateDateTime : public StateSettings
{
protected:
	void			SetRtc();
	unsigned char	HexToBcd( unsigned char hexVal );
};

class StateDateTimeSetDate : public StateDateTime
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

};

class StateDateTimeSetTime : public StateDateTime
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif // __DATETIMESTATES_H_INCLUDED
