#ifndef _R_AND_D_STATES_H
#define _R_AND_D_STATES_H
#include "State.h"
#include "GuiMessages.h"
#include "RawData.h"
#include "SettingsData.h"
#include "SystemData.h"
#include "CalData.h"
#include "BarcodeData.h"
#include "SystemErrorNums.h"
#include "FactoryData.h"
#include "ManufacturingTestSet.h"
#include "RandDTestStates.h"
#include "Miscellaneous.h"


#undef   NDXT_DEBUG

extern AStrings    strings;

// RandD state common function and variable 
class StateRandD : public State
{
public:
    // global functions
    static void                         SetTimer (int time);        // set timer
    static void                         CloseTimer (void);          // close timer thread
    static void *                       TimerThread (void * arg);   // timeThread
    static HANDLE                       timerThread;
    static void                         ProcessOtherMessages (int msgId, char * data);
    static void                         ReadManufacturingTestTimeSet (void);
    static void                         SendDrawerMessage (char command);
    static void                         SendHeaterCommand (char command, unsigned short tDac, unsigned short bDac, float airOffset);
    static void                         SendAMessage (char command);
    static void                         SendBarcodeMessage (char command);
    static void                         SendTestRequestMessage (char command, char *parmeters, unsigned char parameLen);
    static void                         SendCalibrationFactorsMessage (void);
    static void                         SendCuvetteDetectionCommand (char command);
    static DWORD                        StartProcess (const char * processName);
    static SystemErrNum_t               SendRisFile  (char *risFileName);
    static void                         SendBarcodeTestMessage (char command, unsigned char curDac);
    static void                         SendAlignmentTestMessage (char command, unsigned char curDac);
    static void                         CreateTestDirectory (char *dir);
    static int                          GetFileSize (char *fileName);
    static int                          SaveTestResultsToUsb (char *fileName, char *fileText);                         
    // global vairables
    static SystemErrorNumber_t          analyzeError;
    static ManufacturingFlags_t         manufacturingFlags;
    static ManufacturingTestSet_t       manufacturingTestSet;
    static InfoText_t                   infoText;
    static const wchar_t *              infoMsg;
    static RawData_t                    *rawData;
    static BarcodeData_t                *barcodeData;
    static CalibrationData_t            *calibrationData;
    static FactoryData_t                *factoryData;
    static SettingsData_t               *settings;
    static SystemData_t                 *systemData;
    static int                          progBarValue;
    static int                          warmupDoneFlag;                   
    static wchar_t                      displayText [4000];     // global string for display information on screen
    static char                         testCount;              // test number

};

//background state
class StateBackground : public StateRandD
{
public:
    void    Enter (void);
    state_t ProcessMessage( int msgId, char * data );
private:
    HANDLE  analysisQueue;
};

class StateMainSelection :  public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );

};


class StateInfo : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};


class StateRandDError : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
private:    
    static void GetInfoString (void);
};

class StateRandDInfo : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );
};
#endif
