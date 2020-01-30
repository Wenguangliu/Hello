/*
 * FILENAME
 *  AmsMessages.h
 *
 * MODULE DESCRIPTION
 *  Defines and type definitions for messages defined by the AMS process.
 *
 * AUTHOR
 */

#ifndef _ENGINETESTMESSAGE_H
#define _ENGINETESTMESSAGE_H 

#if defined(__cplusplus)
extern "C" {
#endif

#include "ConsoleMessages.h"

/* 
 * message queue received by Engine Test process
 */
#define ENGINE_TEST_GUI_RX_QUEUE_NAME       "/EngineTestGuiRx"  /* gui recived message from other process */
#define ENGINE_TEST_GUI_TX_QUEUE_NAME       "/EngineTestGuiTx"  /* gui send out message to process */
#define ENGINE_TEST_RX_COMM_QUEUE_NAME      "/EngineTestRx"     /* rx message from engine */
#define ENGINE_TEST_TX_COMM_QUEUE_NAME      "/EngineTesttx"     /* tx message to engine */
#define ENGINE_TEST_QUEUE_NAME              "/EngineTestQueue"  /* for engnie test process internal used */ 


#define ENGINE_ID_LEN       16

typedef enum {
    /* message send to gui */
    MSG_ENGINE_TEST_GUI_FIRST = 100,
    MSG_TEST_GUI_SET_INPUT_RECEIVER,
    MSG_TEST_NO_USB_KEY_WINDOW,
    MSG_TEST_MANUFAC_WINDOW,
    MSG_TEST_VENDOR_WINDOW,
    MSG_TEST_SHOW_RESULTS,
    MSG_TEST_SET_PROC_BAR,
    MSG_TEST_REPORT_WINDOW,

    /* gui send message to other process */
    MSG_ENGINE_TEST_TIMER_SET,
    MSG_ENGINE_TEST_ID_SET,
    MSG_POST_TEST,
    MSG_DRAWER_TEST,
    MSG_BARCODE_CALI_TEST,
    MSG_CUV_CALI_TEST,
    MSG_HEAER_TEST,
    MSG_FLASH_BEAK_TEST,
    MSG_MEMORY_TEST,
    MSG_USB_TEST,
    MSG_TEST_ALL_START,
    MSG_AMS_START,
    MSG_START_AMS,
    MSG_GOTO_RESULT,
    
    /* internal process message */
    MSG_FROM_ENGINE,
    MSG_FROM_GUI,
    MSG_TO_ENGINE,
    MSG_TO_GUI,
    MSG_ENGINE_TEST_LAST,

} EngineTestMsgId_t;

#pragma pack (push,1)           /* alignment data structure to be 1 byte */

typedef struct {
    unsigned char  testFlag; 
    unsigned short flashRawDataAvg;      
} BeakTestResults_t;

typedef struct {
    unsigned char  testFlag;
    unsigned short blackOffsetAvg;
} BlackOffsetResults_t;

typedef struct {
    unsigned char       postTest;
    unsigned char       drawerTest;
    unsigned char       barcodeCalibrationTest;
    unsigned char       cuvetteCalibrationTest;
    unsigned char       heaterTest;
} EngineSelfTestResults_t;

typedef struct {
    unsigned int        address;
    unsigned char       memoryTest;
} MemoryTest_t;

typedef struct {
    unsigned char           serialPort;
    unsigned char           usbPort;
    MemoryTest_t            memory; 
    EngineSelfTestResults_t selfTestResults;
    BeakTestResults_t       beakTestResults [10]; 
    BlackOffsetResults_t    blackOffsetResults [10];
} EngineTestResults_t;


/*
 * test time recode
 */
typedef struct
{
    int     sec;        /* 0 -59  */
    int     min;        /* 0 -59  */
    int     hour;       /* 0 - 23 */
    int     mday;       /* 1 - 31 */
    int     mon;        /* 1 -12  */
    int     year;       /* year */
} TestTime_t;

/*
 *  test results index daba base entry
 */
typedef struct
{
    char           instrumentSerialNumber [12];          /* test set serial number */
    char           engineBoardId [ENGINE_ID_LEN];        /* board id number */
    TestTime_t     testTime;                             /* end of test time */
    unsigned int   index;                                /* test results offset index*/
} TestResultsDbIndexEntry_t;

#pragma pack (pop)      /* set back default alignment */

typedef struct {
    unsigned char           passFlag;
    char                    reportMsg [256];
} EngineTestReport_t;

typedef struct {
    char text [20];
} EngineTestBoardId_t;

typedef struct {
    char    hoursLabel[30];
    char    minutesLabel[30];
    char    formatButtonText[30];
    char    zeroSecButtonText[30];
} EngineTestTimeData_t;

typedef struct {
    char label [40];
} EngineTestTopLabel_t;

typedef struct {
    char buttonText[3][30];
    char buttonHighlighted[3];
} EngineTestBottomLabel_t;


typedef struct {
    char name [100];
} ReceiverName_t;

typedef struct {
    EngineTestResults_t         testResults;
    TestResultsDbIndexEntry_t   testBoardInfo;
    ReceiverName_t              receiver;
    unsigned char               procBarValue;
    char                        testItem [64];
    EngineTestReport_t          testReport;
} EngineTestGuiRxMsgData_t;

typedef enum {
    postResults,
    drawerResults,
    barcodeResults,
    cuvetteResults,
    heaterResults, 
    memoryResults,
    usbResults,
    flashBeakResults,
    allResults
} EngineTestResultType_t;


/* 
 * Gui receive message from  other process
 *
 */
typedef struct {
    int                         msgId;
    EngineTestResultType_t      resultType;
    EngineTestGuiRxMsgData_t    data;
} EngineTestGuiRxMsg_t;

/*
 * Gui send message to other process
 */
typedef struct {
    int     msgId;
    char    data [128];     /* temp put data field here */      
} EngineTestGuiTxMsg_t;

/*
 * EngineTest process internal queue 
 */
typedef struct {
    int             msgId;
    unsigned char   command;
    int             len;
    char            data [128]; 
} EngineTestMsg_t;

/* summary of test */
#define FAIL_SUMMARY            0x01

/* power on  selftest */
#define P_10V                   0x02 
#define N_10V                   0x04 
#define SPINDLE_CHIP            0x08 
#define FAN_CONTROL             0x10 

/* memory test */
#define DATA_PATTERN_00         0x02
#define DATA_PATTERN_55         0x04
#define DATA_PATTERN_AA         0x08
#define DATA_PATTERN_FF         0x10
#define ADDRESS_FAIL            0x20
#define MEMORY_DOWNLOAD         0x40

/* drawer test */
#define DRAWER_OPEN_JAM         0x02 
#define DRAWER_CLOSE_JAM        0x04 
#define CAM_UP_JAM              0x06 
#define CAM_DOWN_JAM            0x08 

/* heater test */
#define TOP_HEATER              0x02 
#define BOTTOM_HEAER            0x04 

/* barcode calibration test */
#define BARCODE_SIG_CALI        0x02 
#define BARCODE_THRESHOLD_CALI  0x04 

/* cuvette calibration test */
#define CUV_SIGNAL_CALI         0x02 
#define CUV_THRESHOLD_CALI      0x04 

/* beak flash test */
#define DAC_WEAK_SIGNAL         0x02
#define DAC_NO_SIGNAL           0x04
#define DAC_TOO_MUCH_SIGNAL     0x08
#define DAC_SATURATION          0x10
#define MISS_FLASH_SIGNAL       0x20

/* USB port test */
#define USB_PORT0               0x02
#define USB_PORT1               0x04
#define USB_PORT2               0x08   
#define USB_PORT3               0x10

#define TEST_NOT_AVAILABLE      0x80


#if defined(__cplusplus)
}
#endif

#endif

