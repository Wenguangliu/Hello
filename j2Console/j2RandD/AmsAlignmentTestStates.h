#ifndef _AMSALIGNMENTTESTSTATES_H
#define _AMSALIGNMENTTESTSTATES_H

#include "GuiMessages.h"
#include "RandDStates.h"
#include "RawData.h"




typedef enum
{
    NO_ALIGNMENT_ERROR_REPORT   = 0x00,
    END_ALIGNMENT_ERROR,
} AlignmentError_t;


typedef struct
{
    char                    str [64];
    unsigned short          error;
} AlignmentErrorReport_t;


typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   startTestFlag;
} AmsAlignmentTestFlags_t;


#define MAX_BAR_ACOUNT            10           
class StateAmsAlignmentTestGlobal :  public StateRandD 
{
public:
    void foo ();
protected:
    static  AlignmentErrorReport_t      alignmentErrorReport;
    static  unsigned char               alignmentDac; 
    static  unsigned char               barCount [MAX_BAR_ACOUNT];

};

class StateAmsAlignmentTest : public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    AmsAlignmentTestFlags_t  amsAlignmentTestFlags;   

};

class StateAmsAlignmentTestInProgress : public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    SystemErrNum_t              StartAlignmentTest ();
    

};

class StateAmsAlignmentTestInCanceling :  public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};


class StateAmsAlignmentTestResultsCal :  public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    void    FormatTestReport ();
};


class StateAmsAlignmentTestShowResults :  public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsAlignmentTestShowReports :  public StateAmsAlignmentTestGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};
#endif
