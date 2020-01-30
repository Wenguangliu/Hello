#ifndef _AMSCUVETTETESTSTATES_H
#define _AMSCUVETTETESTSTATES_H
#include "GuiMessages.h"
#include "RandDStates.h"
#include "RawData.h"

#define DAC120_SET_POINT                760
#define MAX_CUV_REPORT_STR_LENGTH       4000
#define MAX_CUVETTE_ROTOR               300
#define MAX_CUVETTE_LOOP                8

typedef enum 
{
    NO_CUV_ERROR_REPORT,
    CUV_SLOPE_LESS_THAN_MIN,
    CUV_SLOPE_LARGE_THAN_MAX,
    CUV_OFFSET_LESS_THAN_MIN,
    CUV_OFFSET_LARGE_THAN_MAX,
    END_CUV_ERROR,
} CuvetteError_t;


typedef struct
{
    char                    str [64];
    CuvetteError_t          error;
} CuvetteFinalErrorReport_t;


typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   cuvetteStartTestFlag;
} AmsCuvetteTestFlags_t;

typedef struct 
{
    int     rotorId;
    float   slope;
    float   intercept;
} CuvetteRotor_t;


typedef struct
{
    int     offset;
    float   slope;  
    int     error [END_CUV_ERROR];
} CuvetteCalResults_t;

typedef enum
{
    DAC_50,
    DAC_60,
    DAC_70,
    DAC_80,
    DAC_90,
    DAC_100,
    DAC_110,
    DAC_120
} DacIndex_t;

typedef struct
{
    unsigned char       cuvetteTestStatus;
    unsigned char       cuvetteCurrent;
    unsigned char       cuvetteThreshold;
    unsigned char       cuvetteNum;
    unsigned short      cuvetteWidth [30];
    unsigned short      cuvetteDistance [30];
} CuvetteTestData_t;




class StateCuvetteTestGlobal :  public StateRandD 
{
public:
    void foo ();
protected:
    static  CuvetteRotor_t               cuvetteRotor;
    static  CuvetteCalResults_t          cuvetteCalResults;
    static  CuvetteFinalErrorReport_t    cuvetteFinalReport [END_CUV_ERROR];
    static  unsigned short               cuvetteData [MAX_CUVETTE_LOOP]; 
};

class StateAmsCuvetteTest : public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static  AmsCuvetteTestFlags_t amsCuvetteTestFlags;

};

class StateAmsCuvetteTestInputRotorId : public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
    int     CheckValidRotor (void);
};

class StateAmsCuvetteTestWrongRotorId : public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsCuvetteTestInProgress : public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
     SystemErrNum_t                 StartCuvetteTest ();
     unsigned char                  testLoopCount; 

};

class StateAmsCuvetteTestInCanceling :  public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsCuvetteTestResultsCal :  public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    void    CalculateTestResults (); 
    void    FormatTestReport ();
};


class StateAmsCuvetteTestShowResults :  public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsCuvetteTestShowReports :  public StateCuvetteTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
