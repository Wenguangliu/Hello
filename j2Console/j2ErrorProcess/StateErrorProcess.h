#ifndef _STATEERRORPROCESS_H_
#define _STATEERRORPROCESS_H_

#include "CommonDefines.h"
#include "State.h"
#include "SystemError.h"
#include "ErrorProcessMessages.h"
#include "SystemData.h"

extern AStrings  strings;

typedef struct 
{
    char    processName [120];
    int     foreGroundMsgId; 
} ProcessForegroundMsgIdTable_t;

typedef enum
{
    STATE_ERROR_NONE  = 0,
    STATE_BACKGROUND,
    STATE_ACTION_WARNING,
    STATE_ACTION_ABORT,
    STATE_ACTION_SHUTDOWN,
    STATE_INFO,    
    STATE_VIEW_LOG,
    STATE_COUNT             // toally state numbers
} ErrorProcessState_t;

class StateErrorProcess : public State
{
public:
    static  unsigned int    errorNumber;
    static  char            processName [120];
    static  unsigned int    foreGroundMsgId;
    static  SystemData_t    *systemData;
    static  char            processKilledFlag;
};

class StateBackground : public StateErrorProcess
{
public: 
    void            Enter();
    state_t         ProcessMessage (int msgId, char * data);
    unsigned int    GetForegroundMsgId (char *processName); 
    int             SendForegroundMsg (int msgId);
};

class StateActionWarning : public StateErrorProcess
{
public: 
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};

class StateActionAbort : public StateErrorProcess
{
public: 
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};

class StateActionShutdown : public StateErrorProcess
{
public: 
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    DWORD   GetProcId (TCHAR *procname);
    int     KillProcId (DWORD id);
    void    KillCurentProcesses (void);
};

class StateInfo : public StateErrorProcess
{
    public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};

#if 0
class StateViewLog : public StateErrorProcess
{
    public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};
#endif


#endif
