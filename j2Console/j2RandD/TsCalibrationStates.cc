#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "TsCalibrationStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"


// Motor Test state  
void
StateTsCalibration::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Touch Screen Calibration";
    
    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, L"Press Cali To Start Calibration");
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
    SetBottomButtons (strings[BSTR_BACK], false, L"Cali", true);
     
}

//touch screen calibration 
state_t
StateTsCalibration::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_TOUCH_SCREEN_CALI;
    int                         idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: //Back
                    retVal = stateStack.Pop();
                break;

                case 1: // Cali to start calibration
                    StartProcess (TOUCH_SCREEN_PROGRAM);
                break;

                default:
                break;
            }
        break;


        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        infoText.type = MOTOR_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

