/*
 * FILENAME
 * 	SecurityOpStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the operator security states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SECURITYOPSTATES_H_INCLUDED
#define __SECURITYOPSTATES_H_INCLUDED

#include <time.h>

#include "State.h"
#include "SettingsStates.h"


class StateSecurityOp : public StateSettings
{
protected:
	wchar_t *	FormatDate( FILETIME date );

	static Expiry_t *	expiry;
	static char			searchId[ ID_LEN ];
	static FILETIME		searchDate;
};

class StateSecurityOpManage : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpSearch : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpNewId : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpEnterId : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpEnterDate : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpListId : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};

class StateSecurityOpListExpiration : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};

class StateSecurityOpListAll : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};

class StateSecurityOpEditSettings : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpSetDate : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityOpDeleteWarning : public StateSecurityOp
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};


#endif	// __SECURITYOPSTATES_H_INCLUDED
