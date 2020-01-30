#ifndef STATE_H_
#define STATE_H_

/* Ams Rx Process States */
typedef enum
{
    AMS_RX_IDEL,
    AMS_RX_WAVE_LENGTH,
    AMS_RX_CUV_DELAY,
    AMS_RX_GLOBAL_CUV_DELAY,
    AMS_RX_SERIAL_NUM,
    AMS_RX_MIX_PROFILE,
    AMS_RX_SAMPLE_PROFILE,
    AMS_RUN_MIX_PROFILE,
    AMS_RUN_ENG_ANALYSIS,
    AMS_RX_AIR_TEMP_OFFSET,
    AMS_RX_HEATER_DEC_SETTING,
    AMS_RX_TEMPERATURE,
    AMS_RX_UPLOAD_CURRENT_LOG,
    AMS_RX_UPLOAD_OLD_LOG,
    AMS_RX_UPLOAD_WAIT_ACK,
    AMS_RX_WAIT_UPLOAD_LOG_FINISH,
    AMS_RX_UPLOAD_RAD_WAIT_ACK,
    AMS_RX_UPLOAD_RAD_FINISH_WAIT_ACK,
    AMS_RX_UPLOAD_CALIBRATION,
} AmsRxProcessState_t;


typedef enum 
{
    AMS_TX_IDEL,
    AMS_TX_WAIT_FOR_TEMPERATURE,
    AMS_TX_READY_TX_1_TEMPERATURE,
    AMS_TX_READY_TX_2_TEMPERATURE,
    AMS_TX_READY_TX_3_TEMPERATURE,
    AMS_TX_READY_TX_4_TEMPERATURE,
    AMS_TX_WAIT_FOR_TEMPERATURE_ANALY_DONE,
    AMS_TX_WAUT_FRO_MOTOR_ANALY_DONE,
} AmsTxProcessState_t;

typedef enum {
    HEATER_IDLE_STATE,  
    HEATER_WAIT_DATA_STATE,
    HEATER_CAL_DATA_STATE,
    HEATER_WARM_UP_STATE,
} HeaterState_t;

typedef struct  {
    char                command; 
    unsigned short      type; 
} AmsAnalysisCmd_t;


typedef struct 
{
    AmsRxProcessState_t     amsRxState;
    AmsTxProcessState_t     amsTxState;
    HeaterState_t           heaterState;
    AmsAnalysisCmd_t        amsCommand;
    unsigned    char        waitingDrawerClose;
    unsigned    char        opticalDownload;
    unsigned    char        barcodeTest;
} AmsState_t;

extern AmsState_t           amsState;

#endif
