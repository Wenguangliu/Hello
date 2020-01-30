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

typedef enum
{
    NONE_SOCKET,
    SERVER_SOCKET,
    CLIENT_SOCKET,
} socketType_t;

class StateComm : public StateSettings
{
public:
	static void	            SetEthernet( bool forceUpdate = false );

protected:
	state_t		            SaveConfiguration( state_t returnState, bool doStartComm = true );
	bool		            ValidateIp( char * ip );
	bool		            ValidateSocket( char * socket );
    void                    LoadFuseSettings();
	
	static CommInterface_t	currentCommInterface;
	static CommProtocol_t	currentCommProtocol;
	static char				currentAnalyzerIp[30];
	static char				currentGateway[30];
	static char				currentIpMask[30];
	static char				currentServerIp[30];
	static int				currentServerSocket;
	static int				currentClientSocket;
    static bool             displayFuseOption;
	static StringId_t		errString;
	static StringId_t		errInfoString;
    static char             hl7SetFlag;
    static socketType_t     socketSetType;         
    static wchar_t             displayText[128];

private:
	static void *	        ConfigurationThread (void * arg);
	bool			        ConfigurationChanged();
	static void		        WriteSetIpAddress();

	static bool	            analyzerIpChanged;
	static bool	            gatewayChanged;
	static bool	            ipMaskChanged;
};

class StateCommNetworkCheck : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommNetworkEnterAnalyzerIp : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	wchar_t	value[30];
};

class StateCommNetworkEnterGateway : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	wchar_t	value[30];
};

class StateCommNetworkEnterIpMask : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	wchar_t	value[30];
};

class StateCommNetworkEnterServerIp : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	wchar_t	value[30];
};

class StateCommNetworkEnterSocket : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	wchar_t	value[30];
};


class StateCommAstmLayer : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommHl7Layer : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommHl7ConfigSelect : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

protected:
    static Hl7ConfigurationFileList_t  *hl7FileList;

private:
    int			topItem;
    int			numItems;
    int			page;
    wchar_t    label[20][30];
    wchar_t    value[20][30];
};

class StateCommTcpIpSelect : public StateComm
{
public:
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );
};

class StateCommIpAddressModeSelect : public StateComm
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateCommNetworkSetting : public StateComm
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
private:
    int     ConfigureNetwork (void);
};

class StateCommTcpIpSettings : public StateComm
{
	void	Enter();
	state_t	ProcessMessage( int msgId, char * data );

private:
	int		topItem;
	int		numItems;
	wchar_t	label[20][30];
	wchar_t	value[20][30];
};

class StateCommEnterHostname : public StateComm 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    wchar_t    value [30];
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
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateCommSelectProtocol : public StateComm
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};


#endif	// __COMMSTATES_H_INCLUDED
