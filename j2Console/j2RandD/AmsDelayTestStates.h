#ifndef _AMSDELAYTESTSTATES_H
#define _AMSDELAYTESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "RawData.h"



#define MAX_DELAY_NUM       76      // max delay times 
#define MAX_CHANNEL         4       // 4 wavelength to be calculated
#define MAS_FLASH_NUM       20      // each cuvette flash numbers

#define                     MAX_OTHER_ERR1      8
#define                     MAX_OTHER_ERR2      6 

#define AMS_PAGE_SIZE           4000 


typedef enum 
{
    ERR_AVG1                = 0x01,
    ERR_AVG2                = 0x02,
    ERR_N01                 = 0x04,
    ERR_N02                 = 0x08,
    ERR_SAMP1               = 0x10,
    ERR_SAMP2               = 0x20,
    ERR_SETAV1              = 0x40,
    ERR_SETAV2              = 0x80,     // other error1 total = 8

    ERR_MIN1                = 0x100,
    ERR_MIN2                = 0x200,
    ERR_MAX1                = 0x400,
    ERR_MAX2                = 0x400,
    ERR_MINSLOPE            = 0x800,
    ERR_MAXSLOPE            = 0x1000,   // other error2, total = 6 
    ERR_OTHER_END           = 0xFFFF
} DelayOtherError_t;

typedef enum
{
    NO_DELAY_ERROR_REPORT   = 0x00,
    ERR_ZERO_DENOM          = 0x01,
    ERR_DUMMY_DATA          = 0x02,
    ERR_NO_MAX              = 0x03,
    ERR_SETSL_DIFF_0        = 0x04,
    ERR_BAD_INTERPOLATION   = 0x05,
    ERR_DELAY_MAM_ALLCO     = 0x06,
    ERR_OTHERS              = 0x07,
    END_DELAY_ERROR,
} DelayError_t;


typedef struct
{
    char                    str [64];
    DelayError_t            error;
    char                    final [18];
} DelayErrorReport_t;

typedef struct
{
    char                    str [64];
    DelayOtherError_t       error;
    char                    final [18];
} DelayOtherErrorReport_t;

typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   startTestFlag;
} AmsDelayTestFlags_t;

typedef struct
{
} DelayCalResults_t;


class StateDelayTestGlobal :  public StateRandD 
{
public:
    void foo ();
protected:
    static unsigned short          avgData [MAX_DELAY_NUM][MAX_CHANNEL];
    static unsigned int            error1 [MAX_CHANNEL];
    static unsigned int            error2[MAX_DELAY_NUM][MAX_CHANNEL];
    static DelayErrorReport_t      delayErrorReport;
};

class StateAmsDelayTest : public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static AmsDelayTestFlags_t  amsDelayTestFlags;   

};

class StateAmsDelayTestInProgress : public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
     SystemErrNum_t                StartDelayTest ();

};

class StateAmsDelayTestInCanceling :  public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsDelayTestResultsCal :  public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    void    GetAverageData (void);
    int     ProcessData (void);
    int     CheckData (void);
    void    FormatTestReport (void);

    float   setav [MAX_DELAY_NUM][MAX_CHANNEL];
    float   setsl [MAX_DELAY_NUM][MAX_CHANNEL];
    float   n0 [MAX_CHANNEL];
    float   nx [MAX_CHANNEL];
    int     ni [MAX_CHANNEL];
    int     intc [MAX_CHANNEL];
    float   avg; 
    int     nz;
    float   globalDelay;
};


class StateAmsDelayTestShowResults :  public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsDelayTestShowReports :  public StateDelayTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    void                    SendDelayMessage (void);
    static int              pageNumber;
    static int              totalPages;
    //static wchar_t          delayMsg [AMS_PAGE_SIZE];
    static int              totallReadLen;
    static int              fileSize;
    static int              lastPgLen;
    static char             mPage;

};
#endif
