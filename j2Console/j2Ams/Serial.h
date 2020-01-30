#ifndef _SERIAL_H
#define _SERIAL_H

#include "AmsProtocol.h"


#define MAX_MSG_LENGTH          2000

/* serail port parameters */
#define  AMS_BAUD_RATE                  19200
#if 0
#define  AMS_LOG_UUPLOAD_BAUD_RATE      115200 
#else
#define  AMS_LOG_UUPLOAD_BAUD_RATE      57600 
#endif


#define LOG_FILE_CURRENT    "Hard Disk/Abaxis/log.current"
#define LOG_FILE_TEMP       "Hard Disk/Abaxis/log.tmp"
#define LOG_FILE_OLD        "Hard Disk/Abaxis/log.old"
//#define LOG_FILE_MAX_SIZE   1000000
#define LOG_FILE_MAX_SIZE   10000000



/* ams debug flag */
#undef   AMS_DEBUG
#undef   AMS_TIMER_DEBUG
#undef   AMS_WRITE_RAM_DEBUG 
#undef   AMS_ANA_DEBUG
#undef   AMS_TRAN_PROFILE_DEBUG
#undef   AMS_RUN_PROFILE_DEBUG
#undef   AMS_FAIL_DEBUG
#undef   AMS_GET_CALS_DEBUG
#undef   AMS_CHANGE_CAL_DEBUG
#undef   AMS_DUMP_DEBUG
#undef   AMS_SG_Q_DEBUG
#undef   AMS_TX_DEBUG
#undef   XML_DEBUG
#undef   AMS_ENG_COMM_DEBUG
#undef   AMS_MSG_Q_DEBUG
#undef   CONSOLE_RX_DEBUG
#undef   ENGINE_DOWNLOAD
#define  ENGINE_II

#undef   UPLOAD_LOG_DEBUG
/* ZZZ */
#undef    UPLOAD_LOG_SIMU
#undef    UPLOAD_RAD_SIMU
#undef    AMS_RX_DEBUG

#undef   AMS_SIMULATION      /* ams simulation flag */

/* Timeout for receiving characters */
#define RX_TIMEOUT  10000  /* 1 second */

#define  UPLOAD_LOG_TIMEOUT_ERROR       0x4400 
#define  UPLOAD_LOG_NAK_ERROR           0x4401
#define  UPLOAD_RAD_TIMEOUT_ERROR       0x4402
#define  UPLOAD_RAD_NAK_ERROR           0x4403
#define  AMS_RX_ERROR                   0x4404
#define  AMS_CUVETTE_TEST_ERROR         0x4405
#define  AMS_ETHERNET_TEST_ERROR        0x4500

/* serial port  status */  
typedef enum
{
    NO_ERR,
    CMD_ERR,
    FORMAT_ERR,
    LENGTH_ERR,
    CRC_ERR,
    TX_ERR,
    AMS_RX_TIMEOUT,
    AMS_RX_BUF_OVERFLOW,
    AMS_PORT_OPEN_FAIL,
    AMS_CMD_NOT_SUPPORT,
    OPEN_FILE_FAIL,
    OPEN_FACTORY_FILE_FAIL,
    OPEN_CALIBRATION_FILE_FAIL,
    FACTORY_XML_OPEN_FAIL,
    CREATE_PARSER_FAIL,
    XML_SYNTAX_ERR,
    XML_CRC_ERR,
    XML_READ_ERR,
    VARIABLE_IDX_OVERFLOW,
    DUMP_FILE_READ_ERR
} AmsError_t;

/* convert ascii to decimal  type */
typedef enum
{
    HEX_TYPE,
    DEC_TYPE
} ConvertType_t;


#define AMS_PORT        2

/* serial port rx states */
typedef enum {
    RX_IDLE,
    RX_DATA
} RxState_t;


/* AMS serial port buffer */
typedef struct {
    
    char            AmsRxMsg[MAX_MSG_LENGTH];
    unsigned short  rxLength;
    char            AmsTxMsg[MAX_MSG_LENGTH];
    unsigned short  txLength;
} AmsBuf_t;


extern AmsBuf_t   amsBuf; 

#endif

