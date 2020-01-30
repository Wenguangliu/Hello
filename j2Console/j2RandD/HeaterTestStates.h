#ifndef _HEATERTESTSTATES_H
#define _HEATERTESTSTATES_H
#include "GuiMessages.h"
#include "HeaterControlMessages.h"


typedef enum
{
    SEND_ZERO,
    SEND_ALL,
    SEND_UPDATA_TEMPERATURE,
    SEND_TOP_DAC_VAULE,
    SEND_BOTTOM_DAC_VALUE,
} SendGuiItems_t;

typedef struct
{
    char    pollingFlag;
    char    startFlag;
} HeaterTestFlags_t;

class StateHeateTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
private:
    static  float CalculateTargetTemperature (unsigned short setDac);
    static  void  SendTemperatureMsgToGui (SendGuiItems_t sendItem);
    static  void  SetGuiTemperatureValues (GuiMiddleTemperatureData_t *temperatureData);  
    static  void  SetGuiTopDacValue (GuiMiddleTemperatureData_t *temperatureData);
    static  void  SetGuiBotDacValue (GuiMiddleTemperatureData_t *temperatureData);
    static  void  SetAllValuesZero (GuiMiddleTemperatureData_t *temperatureData);
    static  HeaterValues_t      heaterValues;
    static  HeaterValues_t      prevHeaterValues;
    static HeaterTestFlags_t    heaterTestFlags;
};

class StateWaitWarmup: public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage(int msgId, char * data);
};
#endif
