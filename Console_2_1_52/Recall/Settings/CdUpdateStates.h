/*
 * FILENAME
 * 	CdUpdateStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the CD ROM update states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __CDUPDATESTATES_H_INCLUDED
#define __CDUPDATESTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateCdUpdate : public StateSettings
{
protected:
	static void *	UpdateSoftwareThread( void * arg );
};

class StateCdUpdateSelect : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCdUpdateLoadSoftwareCd : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

/*
class StateCdUpdateLoadControlsCd : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
*/

class StateCdUpdateReadingSoftwareCd : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCdUpdateUpdateSoftware : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		progBarValue;
	bool	done;
};

/*
class StateCdUpdateUpdateControls : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		progBarValue;
	bool	done;
};
*/

class StateCdUpdateError : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCdUpdateWarning : public StateCdUpdate
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif // __CDUPDATESTATES_H_INCLUDED
