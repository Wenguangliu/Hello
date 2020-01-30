#ifndef _AMSTEMPTESTSTATES_H
#define _AMSTEMPTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "RawData.h"


#undef  TEMP_SIMULATION                 // simulation temperature test

#define PLATE_TEMPERATURE_ADC_CONVERT_MULT      0.0010557433    // 1.0557433 millidegree C per plate ADC count
#define PLATE_TEMPERATURE_ADC_CONVERT_OFFSET    7.758012        // 7.758012 degree C plate temperature ADC convert offset
#define PLATE_VOLTAGE_ADC_COVERT_MULT           0.000625       // 62.5 microvolts per plate ADC count

#define ROTOR_TEMPERATURE_CONVERT_MULT          0.00625         // 6.25 millidegree C per rotor temperature sensor ADC count

#define AMBIENT_TEMPERATURE_CONVERT_MULT        0.0125          // 12.5 millidegree C per ambient temperature sensor ADC count
#define AMBIENT_TEMPERATURE_CONVERT_OFFSET      273.15          // 0.0 degrees C is 273.15 degrees Kelvin offset for ambient temperature sensor

#define PLATE_TEMPERATURE_DAC_CONVERT_MULT      0.197952    // 197.952 millidegree C per plate DAC count
#define PLATE_TEMPERATURE_DAC_CONVERT_OFFSET    7.758012    // 7.758012 degree C plate temperature DAC convert offset

#define PLATE_CURRENT_CONVERT_MULT              0.000625    // 625 microamps per plate current ADC count

#define THERM_MULT                              5.0
#define THERM_OFFSET                            36.9
#define DEFAULT_AMS_TEMP_TEST_DAC               152
#define DEFAULT_AMS_BOTTOM_H_DAC                176

#define MAX_TEMP_REPORT_STR_LENGTH              4000

typedef enum 
{
    NO_TEMP_ERROR_REPORT,
    ROTOR_OHMS_OUT_RANGE,
    TEMP_MEM_ALLOC_ERROR,
    TEMP_TOO_LOW,
    TEMP_TOO_HIGH,
    TEMP_INIT_TOO_LOW,
    TEMP_INIT_TOO_HIGH,
    TEMP_AVG_TOO_LOW,
    TEMP_AVG_TOO_HIGH,
    TEMP_OFFSET_TOO_LOW,
    TEMP_OFFSET_TOO_HIGH,
    TEMP_SDT_TOO_LOW,
    TEMP_SDT_TOO_HIGH,
    END_TEMP_ERROR,
} TempError_t;


typedef struct
{
    char                    str [64];
    TempError_t             error;
    char                    final [18];
} TempFinalErrorReport_t;


typedef struct
{
    float                   tempAvg;
    float                   tempMax;
    float                   tempMin;
    float                   tempRange;
    float                   tempStddev;
    float                   rotorResistance;
    float                   rotorTemp;
    float                   tempOffset;
    float                   tempInit;
    float                   offset;
    unsigned char           topDac; 
    unsigned char           botDac; 
} TempCalResults_t;

typedef struct
{
    unsigned char           guiUpdateFlag;
    unsigned char           drawerReadyFlag;
    unsigned char           drawerOpenFlag;
    unsigned char           tempStartTestFlag;
} AmsTempTestFlags_t;

typedef struct 
{
    int     rotorId;
    float   offset;
    int     rotorOhms;
} TempRotor_t;

typedef struct
{
    unsigned short          initDac [2];
    float                   airInitTemp;
    float                   ambInitTemp;
    float                   topInitTemp;
    float                   botInitTemp;
    float                   offset;
} TempInitValues_t;

#define  MAX_TEMP_ROTOR     300
#define  MAX_TEMP_DATA      13


class StateTempTestGlobal :  public StateRandD 
{
public:
    void foo ();
protected:
    static TempRotor_t              tempRotor;
    static TempInitValues_t         tempInitValues;
    static unsigned short           tempDefaultDac [2];
    static float                    tempData [MAX_TEMP_DATA];
    static TempCalResults_t         tempCalResults;
    static TempFinalErrorReport_t   tempFinalReport [END_TEMP_ERROR];
    static char                     errorReport;           
};

class StateAmsTempTest : public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static  AmsTempTestFlags_t amsTempTestFlags;

};

class StateAmsTempTestInputRotorId : public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
    int     CheckValidRotor (void);
};

class StateAmsTempTestWrongRotorId : public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsTempTestInProgress : public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
     SystemErrNum_t                StartTempTest ();

};

class StateAmsTempTestInCanceling :  public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsTempTestInputOhms :  public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsTempTestResultsCal :  public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
	void    GetTemperatureData ();
    void    CalculateTestResults (); 
    float   OhmToDegree ();
    void    CalDacValues ();
    void    FormatTestReport ();
};


class StateAmsTempTestShowResults :  public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsTempTestShowReports :  public StateTempTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsAnalyWaitWarmup :  public StateTempTestGlobal
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

#endif
