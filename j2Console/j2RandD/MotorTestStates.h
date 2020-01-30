#ifndef _MOTORTESTSTATES_H
#define _MOTORTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"

typedef struct 
{
    char                direction;          // 0 forward, 1 reverse
    unsigned short      speedIndex;         // motor speed
} MotorSettings_t;

typedef struct
{
    char    guiUpdateFlag;
    char    motorStartFlag;
    char    drawerOpenFalg;
    char    drawerReadyFlag;
} MotorTestFlags_t;

class StateMotorTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static MotorSettings_t  motorSettings;
    static MotorTestFlags_t motorTestFlags;
    static unsigned int GetMotorSettingSpeed (void);
    static void  SendEngineMotorCmd (char command, unsigned short speed);  
};
#endif

