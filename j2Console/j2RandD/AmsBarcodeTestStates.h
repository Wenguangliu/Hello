#ifndef _AMSBARCODETESTSTATES_H
#define _AMSBARCODETESTSTATES_H
#include "GuiMessages.h"
#include "RandDStates.h"
#include "RawData.h"




typedef enum
{
    NO_BARCODE_ERROR_REPORT   = 0x00,
    BARCODE_END_TRANSITION_ERR,
    BARCODE_TRANSITIONS_ERR,
    BARCODE_NO_START_ERR,
    BARCODE_NO_STOP_ERR,
    BARCODE_BAD_DECODE_ERR,
    BARCODE_QUIET_ZONE_ERR,      
    BARCODE_SET_CURRENT_ERR,
    BARCODE_READ_TIMEOUT_ERR,
    BARCODE_CALIBRATION_ERR,
    BARCODE_UNKNOWN_ERR,
    END_BARCODE_ERROR,
} BarcodeError_t;


typedef struct
{
    char                    str [64];
    BarcodeError_t          error;
    SystemErrorNumber_t     sysError;
} BarcodeErrorReport_t;

typedef struct 
{
    SystemErrorNumber_t     sysError;
    BarcodeError_t          bcError;
} SystemErrMappingBarcodeErr_t;


typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   startTestFlag;
} AmsBarcodeTestFlags_t;


class StateAmsBarcodeTestGlobal :  public  StateRandD 
{
public:
    void foo ();
protected:
   static  BarcodeData_t           barcodeData;
   static  unsigned char           barcodeCurDacValue;
   static  BarcodeErrorReport_t    barcodeErrorReport;

};

class StateAmsBarcodeTest : public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    AmsBarcodeTestFlags_t  amsBarcodeTestFlags;   

};

class StateAmsBarcodeTestInProgress : public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    SystemErrNum_t              StartBarcodeTest ();
    BarcodeError_t              BarcodeErrorMappings (SystemErrorNumber_t   sysErr);     
    

};

class StateAmsBarcodeTestInCanceling :  public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsBarcodeTestResultsCal :  public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    void    FormatTestReport ();
};


class StateAmsBarcodeTestShowResults :  public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsBarcodeTestShowReports :  public StateAmsBarcodeTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
