#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#define PLATE_TEMPERATURE_ADC_CONVERT_MULT      0.0010557433    // 1.0557433 millidegree C per plate ADC count
#define PLATE_TEMPERATURE_ADC_CONVERT_OFFSET    7.758012        // 7.758012 degree C plate temperature ADC convert offset
#define PLATE_VOLTAGE_ADC_COVERT_MULT           0.000625       // 62.5 microvolts per plate ADC count

#define ROTOR_TEMPERATURE_CONVERT_MULT          0.00625         // 6.25 millidegree C per rotor temperature sensor ADC count

#define AMBIENT_TEMPERATURE_CONVERT_MULT        0.0125          // 12.5 millidegree C per ambient temperature sensor ADC count
#define AMBIENT_TEMPERATURE_CONVERT_OFFSET      273.15          // 0.0 degrees C is 273.15 degrees Kelvin offset for ambient temperature sensor

#define PLATE_TEMPERATURE_DAC_CONVERT_MULT      0.197952    // 197.952 millidegree C per plate DAC count
#define PLATE_TEMPERATURE_DAC_CONVERT_OFFSET    7.758012    // 7.758012 degree C plate temperature DAC convert offset

#define PLATE_CURRENT_CONVERT_MULT              0.000625    // 625 microamps per plate current ADC count

#define TOP_PLATE_TOO_HOT                       38.5

#define LOW_AMBIENT_TEMPERATURE_LIMIT           13.0    // operation temperature limits
#define ROTOR_AIR_SENSOR_THRESHOLD              33.5
#define ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR      35.0
#define DEAD_AIR_SENSOR_THRESHOLD               5.0




// temperature types

#define ROTOR_TEMPERATURE               0
#define AMBIENT_TEMPERATURE             1
#define TOP_PLATE_TEMPERATURE           2
#define BOTTOM_PLATE_TEMPERATURE        3
#define TOP_PLATE_VOLTAGE               4
#define BOTTOM_PLATE_VOLTAGE            5
#define NOVAL_NEG_FLOAT                 -999999.0

#define DELAY_10S                       2
#define DELAY_15S                       3 
#define DELAY_20S                       4 
#define DELAY_30S                       6
#define SECONDS_5_DIVISOR               5

#define TOP_HEATER_DAC_DEFAULT          0
#define TOP_HEATER_DAC_HIGH             1
#define BOTTOM_HEATER_DAC_DEFAULT       2
#define BOTTOM_HEATER_DAC_HIGH          3

#define LT_20_AMBIENT_KICK              8
#define LT_22_AMBIENT_KICK              6
#define LT_24_AMBIENT_KICK              4
#define LT_28_AMBIENT_KICK              2

typedef struct
{
    float      rotorTemperature;
    float      ambientTemperature;
    float      topPlateTemperature;
    float      bottomPlateTemperature;
    float      topPlateCurrent;
    float      bottomPlateCurrent;
} RawTemperature_t;

typedef struct
{
    unsigned char  topPlateDacNormal_m;
    unsigned char  bottomPlateDacNormal_m;
    unsigned char  bottomPlateDacHigh_m;
    unsigned char  fanDacOn_m;
    unsigned char  ambientplateDacOffset_m;
    unsigned short heaterControlTimer_m;
    unsigned short heaterControlDelayTimeSet_m;
    unsigned short heatKickDelay_m;
    float          topPlateTemperatureThreshold_m;
    float          bottomPlateTemperatureThreshold_m;
} HeaterControllerFlags_t;

typedef struct
{
    unsigned    high_temperature_kick_time;
    unsigned    low_temperature_kick_time;
    unsigned    high_temperature_kick_limit;
    unsigned    low_temperature_kick_limit;
    unsigned    max_kick_time_after_mix;
} HeaterKickTimeSettings_t;

typedef struct
{
    int                 msgId;
    RawTemperature_t    heaterData; 
} HeaterMsg_t;

typedef enum {
    MSG_HEATER_TIMER,
    MSG_HEATER_WARM_START,
    MSG_HEATER_DATA,
} HeaterMsgId_t;

typedef struct 
{
    unsigned short   topHeaterDac;
    unsigned short   botHeaterDac;
} heaterDac_t;
extern RawTemperature_t rawTemperatures;
    
#endif
