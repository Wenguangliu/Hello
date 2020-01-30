#ifndef _BARCODETESTSTATES_H
#define _BARCODETESTSTATES_H
#include "GuiMessages.h"
#include "ManufacturingStates.h"
#include "SystemErrorNums.h"


typedef struct
{
    SystemErrorNumber_t     errorNum;
    char                    resultStr [32]; 
} BarcodeResultStrings_t;

typedef struct
{
    unsigned char   guiUpdateFlag;
    unsigned char   drawerReadyFlag;
    unsigned char   drawerOpenFlag;
    unsigned char   barcodeStartTestFlag;
} BarcodeTestFlags_t;

class StateBarcodeTest : public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage (int msgId, char * data);
private:
    static unsigned char        barcodeCurrentDacValue;
    static BarcodeTestFlags_t   barcodeTestFlags;
    static char   *FindBarcodeTestResultString (SystemErrorNumber_t errorNum);
    static void   UpdateBarcodeCalibrationData (unsigned char * message,  GuiMiddleTestData_t *middleTestData);
};
#endif

