#ifndef _LAMPTESTSTATES_H
#define _LAMPTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"

class StateStartLampTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static  void  GetRawDataAvg (void);
    static  void  DisplayAdcValues (void);
    static  char  testStartFlag;
    static  unsigned short avg [NUM_WAVELENGTHS];
    
};

class StateStartLampTrimDacCali : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateLampTrimDacCaliProgress : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

#endif

