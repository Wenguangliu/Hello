#ifndef _CUVETTETESTSTATES_H
#define _CUVETTETESTSTATES_H 
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "SystemErrorNums.h"


typedef struct
{
    SystemErrorNumber_t     errorNum;
    char                    resultStr [32]; 
} CuvetteResultStrings_t;

typedef struct
{
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   guiUpdateFlag;
    unsigned char   testStartedFlag;
    unsigned char   resetDisplayFlag;
} CuvetteTestFlags_t;

typedef struct
{
    unsigned char   cuvCurrentDac;  
    unsigned char   cuvThresholdDac;
    unsigned short  totalCuvNum;
    unsigned short  indexWidth;
    unsigned short  maxCuvWidth;
    unsigned short  minCuvWidth;
    unsigned short  cuvWidth [30];
    unsigned short  cuvError;
} cuvetteTestParameters_t;

class StateCuvetteTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static unsigned char        cuvCurrentDacValue;
    static unsigned char        cuvThresholdDacValue;
    static CuvetteTestFlags_t   cuvetteTestFlags;
    static char                 *FindCuvetteTestResultString (SystemErrorNumber_t errorNum);
    static void                 SendCuvetteTestMessage (char command);
    static void                 SaveTestResults (cuvetteTestParameters_t *cuvetteTestParameters);
    static void                 ResetDisplayParameter (GuiMiddleCuvetteTestData_t *middleCuvetteTestData);
};
#endif

