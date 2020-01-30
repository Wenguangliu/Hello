#ifndef _AMSTESTREPORTDDISPLAY_H
#define _AMSTESTREPORTDDISPLAY_H
#include "GuiMessages.h"
#include "RandDStates.h"

#define DIS_PAGE_SIZE           4000

typedef enum 
{
    SMALL,
    MEDIUM,
    LARGE,
} DisplayFont_t;

typedef struct 
{
    wchar_t              topLabel [32];
    char                *fileName;
    DisplayFont_t       font;
} DisplayParameters_t;

class StateAmsTestReportGlobal :  public  StateRandD
{
public:
    void foo ();
protected:
    static DisplayParameters_t displayParameters;
    
};

class StateAmsTestReport :  public  StateAmsTestReportGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateAmsTestReport2 :  public  StateAmsTestReportGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
};

class StateDisplayAmsReport : public  StateAmsTestReportGlobal 
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
    void    SendReportMessage (void);
private:
    static int              pageNumber;
    static int              totalPages;
    static wchar_t          displayMsg [DIS_PAGE_SIZE];
    static int              totallReadLen;
    static int              fileSize;
    static int              lastPgLen;
    static char             mPage;
};
#endif
