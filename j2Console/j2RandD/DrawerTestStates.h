#ifndef _DRAWERTEST_H
#define _DRAWERTEST_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"

#define NUM_INFO_ITEMS  20

typedef struct
{
    char label[30];
    char value[30];
} InfoItem_t;

typedef struct
{
    int     errorCode;
    char    errStr [64];
} DrawerErrorStr_t;

class StateDrawerTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
    void    FormatTestReport (void);
    void    GetDrawerTestReport (void);
    void    DisplayDrawerTestCounter (void);
    static  DrawerTestCounters_t drawerTestCounters;
};

class StateDrawerIsOpening : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
};

class StateDrawerIsOpened : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
};


class StateDrawerIsClosing : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
};

class StateDrawerWatingForNextCycle : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateDrawerTestCompleted : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateDrawerTestCounterDisplay : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateDrawerTestCancelConfirm : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateDrawerTestCancelling : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateReadBarcode : public StateDrawerTest 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
