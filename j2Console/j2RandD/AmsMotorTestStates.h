#ifndef _AMSMOTORTESTSTATES_H
#define _AMSMOTORTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "RawData.h"


#undef  MOTOR_DEBUG

typedef struct 
{
} AmsMotorTestCfg_t;

typedef enum
{
    NO_AMS_MOTOR_ERROR,
    AMS_MOTOR_TEST_FILE_FAIL,
    FIND_OUT_P_5000_ENTRY_FAIL,
    FIND_OUT_N_5000_ENTRY_FAIL,
    FIND_OUT_N_7000_ENTRY_FAIL,
    FIND_OUT_ACE_1_ENTRY_FAIL,
    FIND_OUT_ACE_2_ENTRY_FAIL,
    SPEED_LOWER_THAN_P_5000,
    SPEED_HIGHER_THAN_P_5000,
    SPEED_LOWER_THAN_N_5000,
    SPEED_HIGHER_THAN_N_5000,
    SPEED_LOWER_THAN_N_7000,
    FAIL_TO_FIND_FIRST_ACCE,
    FAIL_TO_FIND_SECOND_ACCE,
    ACC_5000_MIN_FAIL,
    ACC_4000_MIN_FAIL,
    ACC_3000_MIN_FAIL,
    ACC_2000_MIN_FAIL,
    ACC_1000_MIN_FAIL,
    DEC_4000_MIN_FAIL,
    DEC_3000_MIN_FAIL,
    DEC_2000_MIN_FAIL,
    DEC_1000_MIN_FAIL,
    DEC_0000_MIN_FAIL,
    UNKNOW_ERROR,
    MAX_AMS_MOTOR_TEST_ERROR
} AmsMotorTestErrorCode_t;

typedef struct 
{
    AmsMotorTestErrorCode_t errorCode;
    char                    str [64];
} AmsMotorTestErrorStr_t;

typedef struct 
{
    char                      flag;
    unsigned int              index;
    int                       checkSpeed;
    int                       realSpeed;
    int                       checkAcc;
    float                     acc;
    int                       checkDec;
    float                     dec;
    AmsMotorTestErrorStr_t    errorStr;
} AmsMotorTestError_t;


typedef enum
{
    P_5000_ENTRY,
    N_5000_ENTRY,
    N_7000_ENTRY,
    ACE_1_ENTRY,
    ACE_2_ENTRY,
    END_ENTRY,
} CheckEntry_t;

typedef struct 
{
    int                   checkSpeed;
    int                   maxSpeed;
    int                   minSpeed;
} AmsMotorCheckSpeedTable_t;

typedef struct 
{
    CheckEntry_t    entry;
    int             startPoint;
    int             endPoint;
} CheckPoints_t;


typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   motorStartTestFlag;
} AmsMotorTestFlags_t;

class StateAmsMotorTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    SystemErrNum_t StartMotorTest ();
    static AmsMotorTestFlags_t  amsMotorTestFlags;

};

class StateAmsMotorTestInProgress : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};

class StateAmsMotorTestInCanceling :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsMotorTestResultsCal :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    int     MotorDataCheck (RawData_t *rawData);
    void    MotorTestErrorCheck (int min7000Speed, float *acc, float *dec);
    void    FormatTestResults (int min7000Speed, float *acc, float *dacc, float *dec, float *ddec);
};

class StateAmsMotorTestShowResults :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsMotorTestShowReports :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
