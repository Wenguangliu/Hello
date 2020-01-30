#include "StartupGlobal.h"

// critical error, stop system
void
ErrorStop (SystemErrorNumber_t errNum, SystemData_t *systemData)
{
    ConsoleMsg_t    msg;
    DWORD           receivedSize = 0;
    DWORD           flags;
    static char        infoFlag = 0;

    if (guiInitFlag == 0) {
        InitialGui ();
    }

    // register for Startup talking to Gui
    RegisterGuiComm (STARTUP_QUEUE_NAME);

    SendCriticalErrorMessage (errNum);

    // in the case of product code error and ams key is plugged in, run ams
    if ((errNum == SERR_PRODUCT_CODE) || (errNum == SERR_PRODUCT_CODE)) {
        if (systemData->inManufacturingMode == true) {
            //wait for 5 seconds to remind product code wrong
            Sleep (5000);
            GotoManufacturing ();
        }
    }

    // Wait until reset or power-off.
    while (1) {
        ReadMsgQueue(startupQueue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);
        LogDebug ("gui id=%d", msg.msgId);
        if (receivedSize > 0) {
            switch (msg.msgId) {

                case MSG_GUI_OUT_INFO:
                    ErrorInfo (errNum);
                    infoFlag = 1;
                    break;

                case  MSG_GUI_OUT_BOTTOM_BUTTON:
                    if (infoFlag == 1) {
                        // from info screen back to critial screen
                        SendCriticalErrorMessage (errNum);
                        infoFlag =0;
                    } else {
                        // restart button 
                        GotoRestart (1);
                    }
                    break;

                default:
                    break;
            }
        }
    }
}

// Display error information
int
ErrorInfo (SystemErrorNumber_t errNum)
{
    GuiReceiveMsg_t msg;
   
    //set top lable
    msg.msgId = MSG_GUI_TOP_LABEL;
    wcscpy (msg.data.topLabel.label, (*strings)[TSTR_INFORMATION]);
    SendGuiMessage (&msg, sizeof(msg));

    //set information
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS;
    wcscpy (msg.data.middleViewResults.text,(*strings)[systemError->GetInfoText(errNum)]);
    SendGuiMessage (&msg, sizeof(msg));

    msg.msgId = MSG_GUI_BOTTOM_ONE_BUTTON;
    wcscpy (msg.data.bottomButtons.buttonText[0], (*strings)[BSTR_CONTINUE]);
    msg.data.bottomButtons.buttonHighlighted[0] = true;
    SendGuiMessage (&msg, sizeof(msg));

    return EXIT_SUCCESS;
}

// internal software error state machine
void
SoftwareErrorState (SystemErrorNumber_t errNum)
{
    DWORD           receivedSize = 0;
    DWORD           flags;
    ConsoleMsg_t     msg;
    engineCodeSyncState_t   syncState ;

    // start setting process first
    CreateProcess (SETTINGS_PROGRAM);
    // set default state machine
    syncState = MESSAGE_STATE;
   
    SendErrorMessage (errNum);

    while (1) {
        ReadMsgQueue  (startupQueue, &msg, sizeof(msg), &receivedSize, INFINITE, &flags);
        if (receivedSize > 0) {
            switch (syncState) {
                case MESSAGE_STATE:
                    switch (msg.msgId) {
                        case MSG_GUI_OUT_BOTTOM_BUTTON:     // Continue button
                            //goto j2Settings process
                            SendSettingsMessage (MSG_SETTINGS_SYNC_ENGINE_CODE);;
                        break;
    
                        case MSG_GUI_OUT_INFO:
                            ErrorInfo (errNum);
                            syncState = INFOMATION_STATE;
                        break;
    
                        default:
                        break;
                    }
                break;

                case INFOMATION_STATE:
                    switch (msg.msgId) {
                        case MSG_GUI_OUT_BOTTOM_BUTTON: // Back Button
                            SendErrorMessage (errNum);
                            syncState = MESSAGE_STATE;
                        break;

                        default:
                        break;
                    }
                break;
            }
        }
    }
}
