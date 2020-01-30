#ifndef _NETWORKTEST_H
#define _NETWORKTEST_H
#include "State.h"
#include "GuiMessages.h"
#include "NetworkSettings.h"
#include "ManufacturingStates.h"


#define     IP_CHANGED                  0x01
#define     HOST_NAME_CHANGED           0x02
#define     DHCP_ENABLE                 0x04
#define     DHCP_DISABLE                0x08
#define     SOCKET_CHANGED              0x10

class StateNetworkTestGlobal :  public StateRandD 
{
public:
    void foo ();
protected:
    bool                            ValidateIp (char * ip);
    bool                            ValidateSocket (char * socket);
    static EthernetPortInfo_t       ethernetPortInfo;
    static unsigned char            configChanged;       
    static StringId_t               errString;
    static StringId_t               errInfoString;
    
};



class StateNetworkTest : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateNetworkConfig : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    int     numItems;
    int     topItem;
    char    label[20][30];
    char    value[20][30];
};

class StateEnterAnalyzerIp : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateEnterAnalyzerMask : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateEnterGateway : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateEnterServerIp : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateEnterServerSocket : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateEnterHostname : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    char    value [30];
};

class StateNetworkConfigError : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateNetworkConfiguring : public StateNetworkTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    int     ConfigureNetwork (void);
};

class StateTelnetConfig : public StateNetworkTestGlobal
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

#endif
