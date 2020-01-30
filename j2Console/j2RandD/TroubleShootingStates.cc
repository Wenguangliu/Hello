#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "TroubleShootingStates.h"
#include "DrawerTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "ParseSystem.h"
#include "LogErr.h"

// analyzer troulbe shooting first menu 
void
StateTroubleShooting::
Enter ()
{
    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Trouble Shooting";

    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Heater Test");
    wcscpy (data->buttonText[1], L"Flash Lamp Test");
    wcscpy (data->buttonText[2], L"Spindle Motor Test");
    wcscpy (data->buttonText[3], L"Barcode Test");
    wcscpy (data->buttonText[4], L"Drawer Test");
    wcscpy (data->buttonText[5], L"Cuvette Detector Test");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true, strings [BSTR_NEXT]);

}

state_t
StateTroubleShooting::
ProcessMessage (int msgId, char * data)
{
    state_t retVal = STATE_TROUBLE_SHOOTING;
    int     idata = *(int *)data;

    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {
                case 0: //Temperature
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    if (warmupDoneFlag  == 1) {
                        retVal = STATE_HEATER_TEST;
                    } else {
                        retVal = STATE_WAIT_ANALY_WARM_UP;
                    }
                break;

                case 1: //beak
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    retVal = STATE_LAMP_TEST;

                break;

                case 2: //spindle motor
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    retVal = STATE_MOTOR_TEST;
                break;

                case 3: //barcode
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    retVal = STATE_BARCODE_READ_TEST;
                break;

                case 4: // drawer test
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    retVal = STATE_DT_DRAWER_IS_OPENING;
                break;

                case 5: //cuvette detect test 
                    stateStack.Push (STATE_TROUBLE_SHOOTING);
                    retVal = STATE_CUVETTE_TEST;
                break;

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
                   stateStack.Push (STATE_TROUBLE_SHOOTING);
                   retVal = STATE_TROUBLE_SHOOTING_2;
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    return retVal;
}

// analyzer troulbe shooting second menu
void
StateTroubleShooting2::
Enter ()
{
    GuiReceiveMsg_t         msg;
    GuiMiddleButtonsData_t  *data = &msg.data.middleButtons;
    wchar_t                 *topLable = L"Trouble Shooting";

    SetTopLabel (topLable);
    memset (data, 0, sizeof (GuiMiddleButtonsData_t));
    wcscpy (data->buttonText[0], L"Telnet Set");
    wcscpy (data->buttonText[1], L"Engine Debug Port Set");
    wcscpy (data->buttonText[2], L"Touch Screen Cali");
    wcscpy (data->buttonText[3], L"Log File View");
    msg.msgId = MSG_GUI_MIDDLE_BUTTONS;
    SendGuiMessage (&msg, sizeof(GuiMiddleButtonsData_t));
    SetBottomButtons (strings [BSTR_BACK], true);
}

state_t
StateTroubleShooting2::
ProcessMessage (int msgId, char * data)
{
    state_t retVal = STATE_TROUBLE_SHOOTING_2;
    int     idata = *(int *)data;


    switch (msgId) {
        case MSG_GUI_OUT_BUTTON:
            switch (idata) {
                case 0: // enable/disable telnet port 
                    stateStack.Push (STATE_TROUBLE_SHOOTING_2);
                    retVal = STATE_TELNET_CONFIG;
                break;

                case 1: // enable/disable engine serial debug port
                    stateStack.Push (STATE_TROUBLE_SHOOTING_2);
                    retVal = STATE_ENGINE_SERIAL_DEBUG_PORT_SET;
                break;

                case 2: // calibrate touch screen
                    stateStack.Push (STATE_TROUBLE_SHOOTING_2);
                    retVal = STATE_TOUCH_SCREEN_CALI;
                break;

                case 3: // View log file 
                    stateStack.Push (STATE_TROUBLE_SHOOTING_2);
                    retVal = STAET_VIEW_LOG_FILE;
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

//Engine debug port configuration
void
StateSetEngineSerialDebugPort::
Enter (void)
{
    GuiReceiveMsg_t             msg;
    wchar_t                     *topLable = L"Engine Debug Port Set";
    GuiMiddleOptionInfoData_t   *data = &msg.data.middleOptionInfo;

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_OPTION_INFO;

    memset (data, 0, sizeof (GuiMiddleOptionInfoData_t));
    wcscpy (data->buttonText[0], L"Enable");
    wcscpy (data->buttonText[1], L"Disable");
    if (systemData->engDebugPortEanble) {
        data->selectedButton = 0;
    } else {
        data->selectedButton = 1;
    }
    SendGuiMessage (&msg, sizeof( GuiMiddleOptionInfoData_t));

    SetBottomButtons (strings[BSTR_BACK], true);
}

//Engine debug port configuration
state_t
StateSetEngineSerialDebugPort::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_ENGINE_SERIAL_DEBUG_PORT_SET;
    int                         idata = *(int *)data;
    DWORD                       enable;

    switch (msgId) {
        case MSG_GUI_OUT_TOGGLE:
            switch (idata) {
                case 0xF0:      //enable 
                case 0:
                    if (systemData->engDebugPortEanble == 0) {
                        systemData->engDebugPortEanble = 1;
                        // send setting command to Engine
                        SendTestRequestMessage ('D', NULL, 0);
                        WriteSystem (systemData);
                    }
                break;

                case 0xF1:      //disalbe
                case 1:
                    if (systemData->engDebugPortEanble == 1) {
                        systemData->engDebugPortEanble = 0;
                        // send setting command to Engine
                        SendTestRequestMessage ('d', NULL, 0);
                        WriteSystem (systemData);
                    }
                break;

                default:
                break;
            }
            retVal = stateStack.Pop();
        break;

        case MSG_GUI_OUT_BOTTOM_BUTTON:
            retVal = stateStack.Pop();
        break;

        default:
        break;
    }
    return retVal;
}
