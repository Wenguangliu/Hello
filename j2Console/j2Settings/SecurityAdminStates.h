/*
 * FILENAME
 * 	SecurityAdminStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the administrator security states of the Settings state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __SECURITYADMINSTATES_H_INCLUDED
#define __SECURITYADMINSTATES_H_INCLUDED

#include <time.h>

#include "State.h"
#include "SettingsStates.h"


class StateSecurityAdmin : public StateSettings
{
protected:
// GLD
//	char *	FormatDate( time_t date );
// GLD
// #define for the size?
	static char			password[ 100 ];
	static Expiry_t *	expiry;
	static char			searchId[ ID_LEN ];
//	static time_t		searchDate;
};

class StateSecurityAdminManage : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

/*
class StateSecurityAdminSearch : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
*/

class StateSecurityAdminNewId : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityAdminNewEnterPassword : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityAdminNewConfirmPassword : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityAdminEnterId : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

/*
 * class StateSecurityAdminEnterDate : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
*/

class StateSecurityAdminListId : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};

/*
 * class StateSecurityAdminListExpiration : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};
*/

class StateSecurityAdminListAll : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	Expiry_t *	expiryList;
	int			topItem;
	int			numItems;
};

class StateSecurityAdminEditSettings : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

/*
class StateSecurityAdminSetDate : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};
*/

class StateSecurityAdminEnterPassword : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityAdminConfirmPassword : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateSecurityAdminDeleteWarning : public StateSecurityAdmin
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};


#endif	// __SECURITYADMINSTATES_H_INCLUDED
