#ifndef _TROUBSHOOTING_STATES_H
#define _TROUBSHOOTING_STATES_H
#include "RandDStates.h"


class StateTroubleShooting : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateTroubleShooting2 : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};

class StateSetEngineSerialDebugPort : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};


#endif
