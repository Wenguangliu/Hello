#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <ctype.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "DrawerTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"
#include "ParseManufacturingTestSet.h"

void
StateManufacturingSelection::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Manufacturing Test Selection";
    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"AMS Test");
    wcscpy (data->buttonText[1], L"AMS Test Report");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);
}

// process button
state_t
StateManufacturingSelection::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_MANUFACTURING_SELECTION;
    int     idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {
                case 0: // manufacturing test 
                    stateStack.Push (STATE_MANUFACTURING_SELECTION);
                    retVal = STATE_MANUFACTURING;
                break;

                case 1: // Manufacturing report
                    stateStack.Push (STATE_MANUFACTURING_SELECTION);
                    retVal = STATE_AMS_TEST_REPORT;
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal =  stateStack.Pop();
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    return retVal;
}

// manufacturing test first selection menu
void
StateManufacturingTest::
Enter (void)
{

    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Manufacturing Tests";
    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Motor Test");
    wcscpy (data->buttonText[1], L"NDXT Test");
    wcscpy (data->buttonText[2], L"Temperature Calibration");
    wcscpy (data->buttonText[3], L"Global Delay Calibration");
    wcscpy (data->buttonText[4], L"Cuvette Test");
    wcscpy (data->buttonText[5], L"Barcode Test");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true, strings [BSTR_NEXT]);

}

// process button
state_t
StateManufacturingTest ::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_MANUFACTURING;
    int     idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {

                case 0: // motor test
                    stateStack.Push (STATE_MANUFACTURING);
                    retVal = STATE_AMS_MOTOR_TEST;
                break;

                case 1: // ndxt test
                    stateStack.Push (STATE_MANUFACTURING);
                    retVal = STATE_AMS_NDXT_TEST;
                break;

                case 2: // temperature test
                    stateStack.Push (STATE_MANUFACTURING);
                    // reset temperature test number to be zero
                    testCount = 0;
#if 1 
                    if (warmupDoneFlag  == 1) {
                        retVal = STATE_AMS_TEMP_TEST;
                    } else {
                        retVal = STATE_AMS_WAIT_ANALY_WARM_UP;
                    }
#else
                // ZZZ disable warming up wait time for debug purpose
                        retVal = STATE_AMS_TEMP_TEST;
#endif
                break;

                case 3: // cuvette global delay test
                    stateStack.Push (STATE_MANUFACTURING);
                    retVal = STATE_AMS_DELAY_TEST;
                break;

                case 4: // cuvette test
                    stateStack.Push (STATE_MANUFACTURING);
                    retVal = STATE_AMS_CUV_TEST;
                break;

                case 5:
                    stateStack.Push (STATE_MANUFACTURING);
                    retVal = STATE_AMS_BARCODE_TEST;
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:
                    retVal =  stateStack.Pop();
                break;

                case 1:
                   stateStack.Push (STATE_MANUFACTURING);
                   retVal = STATE_MANUFACTURING_2;
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    return retVal;
}

// manufacturing test second selection menu
void
StateManufacturingTest2::
Enter (void)
{   

    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Manufacturing Tests";
    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Barcode Alignment Test");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);

}


// process button
state_t
StateManufacturingTest2 ::
ProcessMessage (int msgId, char *data)
{
    state_t retVal = STATE_MANUFACTURING_2;
    int     idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {
                case 0: // barcode alignment file
                    stateStack.Push (STATE_MANUFACTURING_2);
                    retVal = STATE_AMS_ALIGNMENT_TEST;
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal =  stateStack.Pop();
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    return retVal;
}
