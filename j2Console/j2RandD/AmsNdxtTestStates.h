#ifndef _AMSNDXTTESTSTATES_H
#define _AMSNDXTTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "RawData.h"



#define CHANNUM         4   // each ndxt dump each line has 4 raw data
#define FLASHNUM        20  // each validate ndxt flash is 21 -1
#define MAX_PACKETS     100
#define NUM_DACS        10

#define WAVENUM         9   // NUMBER OF WAVELENGTHS 
#define CUVNUM          30  // NUMBER OF CUVETTES 
#define PASSNUM         3   // NUMBER OF PASSES 
#define ND05_CUVETTE    21
#define ND10_CUVETTE    22
#define ND20_CUVETTE    23
#define OPTICAL_CUVETTE 29
#define BLACK_CUVETTE   0
#define DAC_DATA_INDEX  0  // First 3 sets are DAC flashes
#define MAIN_DATA_INDEX 3  // Next 90 sets are normal sample sets




typedef enum
{
    NO_ERRORS                               =0,
    ERROR_MASK                              =0x2000,
    ERROR_BIT_MASK                          =0xF000,
    TX_LINE_LENGTH_ERROR                    =0x2001,
    CMD_SEQ_ERROR                           =0x2002,
    LINE_FORMAT_ERROR                       =0x2003,
    ILLEGAL_COMMAND                         =0x2004,
    CRC_ERROR                               =0x2005,
    MSG_TIMEOUT                             =0x2006,
    OK_TIMEOUT                              =0x2007,
    USER_INTERRUPT                          =0x2008,
    FAIL_MESSAGE                            =0x2009,
    NO_ACTION                               =0x200a,
    NAK_RECEIVED                            =0x200b,
    ERRORS_MSG_RECEIVED                     =0x200c,
    DATA_VALUE_ERROR                        =0x200d,
    INSTRUMENT_NOT_READY                    =0x200e,
    ILLEGAL_ENTRY                           =0x200f,
    FILE_OPEN_ERROR                         =0x2010,
    FILE_OVERWRITE_WARNING                  =0x2011,
    NO_CONFIG_FILE_ERROR                    =0x2012,
    CONFIG_FILE_READ_ERROR                  =0x2013,
    CONFIG_FILE_LINE_FORMAT_ERROR           =0x2014,
    FILE_ERASE_ERROR                        =0x2015,
    PERMISSION_DENIED                       =0x2016,
    NO_SUCH_FILE_ERROR                      =0x2017,
    MISSING_TMP_FILE                        =0x2018,
    TX_OVERRUN_ERROR                        =0x2019,
    RX_OVERRUN_ERROR                        =0x201a,
    USER_ABORT                              =0x201b,
    USER_EXIT                               =0x201c,
    ILLEGAL_ID_LINE_FORMAT                  =0x201d,
    ILLEGAL_DATA_LINE_FORMAT                =0x201e,
    DIVIDE_BY_ZERO_ERROR                    =0x201f,
    ILLEGAL_LOG_PARAM_ERROR                 =0x2020,
    DATA_FLOW_OVERRUN                       =0x2021,
    ILLEGAL_LINE_LENGTH                     =0x2022,
    EMPTY_RECEIVE_BUFFER_ERROR              =0x2023,
    ILLEGAL_DAC_LINE_FORMAT                 =0x2024,
    UNEXPECTED_EOF                          =0x2025,
    ILLEGAL_NUMBER_OF_ARGS                  =0x2026,
    ILLEGAL_COMMAND_LINE_ARGS               =0x2027,
    DIVIDE_BY_ZERO_1_ERROR                  =0x2028,
    DIVIDE_BY_ZERO_2_ERROR                  =0x2029,
    DIVIDE_BY_ZERO_3_ERROR                  =0x202a,
    DIVIDE_BY_ZERO_4_ERROR                  =0x202b,
    DIVIDE_BY_ZERO_5_ERROR                  =0x202c,
    DIVIDE_BY_ZERO_6_ERROR                  =0x202d,
    DIVIDE_BY_ZERO_7_ERROR                  =0x202e,
    DIVIDE_BY_ZERO_8_ERROR                  =0x202f,
    MEM_ALLOC_ERROR                         =0x2030,
    NDXT_REPORT_FILE_ERROR                  =0x2031,
} AmsNdxtTestErrorCode_t;

typedef enum
{
    NO_ERROR_REPORT,
    DAC_SETTING_MIN_LEVEL_ERR,
    DAC_SETTING_MAX_LEVEL_ERR,
    AVG_MIN_OFFSET_ERR,
    AVG_MAX_OFFSET_ERR,
    MAX_OFFSET_SD_ERR,
    MAX_CV_ERR,
    MIN_SCALED_COUNTS_ERR,
    ABS_VALUE_LARGE_ERR,
    MAX_NORM_CV_ERR,
    BAND_PASS_LIMIT_ERR,
    BAND_BLOCK_LIMIT_ERR,
    BAND_BLOCK_MIN_SIGNAL_ERR,
    MAX_REPORT_ERR,
} NdxtFinalReportError_t;

typedef struct 
{
    NdxtFinalReportError_t  error;
    char                str [64];
} NdxtFinalErrorReport_t;

typedef struct
{
    double  averages [CHANNUM];
    double  stds [CHANNUM];
    double  ranges [CHANNUM];
    double  cvs [CHANNUM];
} TrimmedData_t;


typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   ndxtStartTestFlag;
} AmsNdxtTestFlags_t;

class StateAmsNdxtTest : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    SystemErrNum_t StartNdxtTest ();
    static AmsNdxtTestFlags_t  amsNdxtTestFlags;

};

class StateAmsNdxtTestInProgress : public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);

};

class StateAmsNdxtTestInCanceling :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsNdxtTestResultsCal :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    AmsNdxtTestErrorCode_t   NdxtTestDataCheck (RawData_t *rawData);
    void    FormatTestReport (void);
    AmsNdxtTestErrorCode_t GenerateReport (void);
    AmsNdxtTestErrorCode_t  GenerateDisplayReport (void);
};

class StateAmsNdxtTestShowResults :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsNdxtTestShowReports :  public StateRandD 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
