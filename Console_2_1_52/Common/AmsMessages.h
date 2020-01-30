/*
 * FILENAME
 *  AmsMessages.h
 *
 * MODULE DESCRIPTION
 *  Defines and type definitions for messages defined by the AMS process.
 *
 * AUTHOR
 */

#ifndef _AMSMESSAGE_H
#define _AMSMESSAGE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "ConsoleMessages.h"

/* 
 * message queue received by AMS process
 */
#define AMS_QUEUE_NAME                  "/AmsCom"
#define AMS_TX_QUEUE_NAME               "/AmsTx"
#define AMS_RX_QUEUE_NAME               "/AmsRx"
#define AMS_ENGINE_COMM_QUEUE_NAME      "/AmsEngineComm"
#define AMS_HEATER_CONTROL_QUEUE_NAME   "/AmsHeaterControl"
#define AMS_GUI_QUEUE_NAME              "/AmsGui"

#define AMS_STATUS_STR_LEN              60

typedef enum {
    MSG_AMS_RX_READY = MSG_FIRST_AMS_ID,
    MSG_AMS_TX_READY,
    MSG_CONSOLE_RX_READY,
    MSG_AMS_EXIT,
    MSG_AMS_STATUS,
    MSG_AMS_ERROR_CODE,
} AmsMsgId_t;

typedef enum {
    AMS_IDLE,
    NO_ROTOR,
    WAITING_FOR_COMMAND,
    INIT_SERIAL_NUM,
    DOWNLOAD_OPTICAL_PARAMETER,
    OPTICAL_CAL_DONE,
    DOWNLOAD_GLOBAL_DELAY_RIS_FILE,
    ANALYSIS_GLOBAL_DELAY,
    UPLOAD_GLOBAL_DELAY_RESULTS,
    ANALYSIS_BARCODE_TEST,
    UPLOAD_BARCODE_RESULTS,
    DOWNLOAD_NDXT_RIS_FILE,
    ANALYSIS_NDXT,
    UPLOAD_NDXT_RESULTS,
    DOWNLOAD_TEMP_RIS_FILE,
    ANALYSIS_TEMP,
    UPLOAD_TEMP_RESULTS,
    DOWNLOAD_MOTOR_RIS_FILE,
    ANALYSIS_MOTOR_TEST,
    UPLOAD_MOTOR_RESULTS,
    UPLOAD_INSTRUMENT_SETTING,
    GET_CAL_PARAMETERS,
    CHG_CAL_PARAMETERS,
    ANALYSIS_CONCEL,
    UPLOAD_CURRENT_LOG_FILE,
    UPLOAD_OLD_LOG_FILE,
    BARCODE_ALIGN_TEST,
    BARCODE_GAIN_TEST,
    UPLOADING_RAD_FILE,
    END_OF_AMS_STATUS,
} AmsStatus_t;
/* 
 * message struct within AMS precess
 */
typedef struct {
    int             msgId;
    AmsStatus_t     amsStatus;
    unsigned short  errorCode;
} AmsGuiMsg_t;

#if defined(__cplusplus)
}
#endif

#endif

