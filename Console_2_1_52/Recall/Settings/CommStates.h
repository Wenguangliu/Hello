/*
 * FILENAME
 * 	CommStates.h
 * 
 * MODULE DESCRIPTION
 * 	Class definitions for the communication parameters states of the Settings
 *	state machine.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __COMMSTATES_H_INCLUDED
#define __COMMSTATES_H_INCLUDED

#include "State.h"
#include "SettingsStates.h"

class StateComm : public StateSettings
{
public:
	static void	SetEthernet( bool forceUpdate = false );

protected:
	state_t		SaveConfiguration( state_t returnState, bool doStartComm = true );
	bool		ValidateIp( char * ip );
	bool		ValidateSocket( char * socket );
	void		LoadFuseSettings();
	
	static CommInterface_t	currentCommInterface;
	static CommProtocol_t	currentCommProtocol;
	static char				currentAnalyzerIp[30];
	static char				currentGateway[30];
	static char				currentIpMask[30];
	static char				currentServerIp[30];
	static int				currentServerSocket;
	static StringId_t		errString;
	static StringId_t		errInfoString;

private:
	static void *	ConfigurationThread( void * arg );
	bool			ConfigurationChanged();
	static void		WriteSetIpAddress();

	static bool	analyzerIpChanged;
	static bool	gatewayChanged;
	static bool	ipMaskChanged;
};

class StateCommAstmCheck : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommAstmEnterAnalyzerIp : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	char	value[30];
};

class StateCommAstmEnterGateway : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	char	value[30];
};

class StateCommAstmEnterIpMask : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	char	value[30];
};

class StateCommAstmEnterServerIp : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	char	value[30];
};

class StateCommAstmEnterServerSocket : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	char	value[30];
};

class StateCommAstmLayer : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommAstmSelect : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommAstmSettings : public StateComm
{
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		topItem;
	int		numItems;
	char	label[20][30];
	char	value[20][30];
};
	
class StateCommError : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommSetProtocol : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommSelectProtocol : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

#endif	// __COMMSTATES_H_INCLUDED
