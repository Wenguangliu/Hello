#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "MotorTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"



#if 0
#define                 MAX_MOTOR_SPEED_INDEX        12
                                  // 0   1     2    3     4     5     6     7      8     9    10   11     12
unsigned short motorSpeedTable [] = {0, 100, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000};
#else
#define                 MAX_MOTOR_SPEED_INDEX        36 
unsigned short motorSpeedTable [] = {0, 100, 1000, 1500, 2000, 2500, 
                                     3000, 3100, 3200, 3300, 3400, 3500, 3600, 3700, 3800, 3900,
                                     4000, 4100, 4200, 4300, 4400, 4500, 4600, 4700, 4800, 4900,
                                     5000, 5100, 5200, 5300, 5400, 5500, 5600, 5700, 5800, 5900,
                                     6000};
        
#endif

MotorSettings_t         StateMotorTest::motorSettings;    
MotorTestFlags_t        StateMotorTest::motorTestFlags;

// send command to Engine board
void
StateMotorTest::
SendEngineMotorCmd (char command, unsigned short speed)
{
    char Parameter [16];

    memset (Parameter, 0, 16);
    Parameter [0] = motorSettings.direction;
    *(unsigned short *)(&Parameter [2]) = speed;

    SendTestRequestMessage (command, Parameter, 16);
}


// Motor Test state  
void
StateMotorTest::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Motor Test";
    GuiMiddleMotorData_t    *motorData;

    
    SetTopLabel (topLable);

    // try to get current drawer status
    SendDrawerMessage ('S');

    memset (&motorSettings, 0, sizeof (MotorSettings_t));
    memset (&motorTestFlags, 0, sizeof (MotorTestFlags_t));
    motorData =  &msg.data.middleMotorData;
    // initial motor test data string
    wsprintf (motorData->motorSpeedReadValue,L"%s", L" ");
    wsprintf (motorData->motorSpeedSetValue, L"%u", 0);
    wsprintf (motorData->motorDirection, L"%s", L"Forward");
    msg.msgId = MSG_GUI_MIDDLE_MOTOR_DATA;
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));
    SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Open", false);
     
}

// Motor test
state_t
StateMotorTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_MOTOR_TEST;
    int                         idata = *(int *)data;
    EngineCommMessageData_t *   engineCommMsg;
    GuiReceiveMsg_t             msg;
    GuiMiddleMotorData_t        *motorData;
    unsigned short              aspeed;
    short                       speed;
    unsigned char               status;

    motorData = &msg.data.middleMotorData; 
    memset (motorData, 0, sizeof (GuiMiddleMotorData_t));
           
    
    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: //Back, always close drawer
                    if (motorTestFlags.motorStartFlag == 0) {
                        SendDrawerMessage ('C');
                        retVal = stateStack.Pop();
                    }
                break;

                case 1://Start/Stop Test
                    // check drawer status, only rotor exist, allow run motor
                    if (motorTestFlags.drawerReadyFlag) {
                        if (motorTestFlags.motorStartFlag == 0) { 
                            motorTestFlags.motorStartFlag = 1;
                            aspeed = GetMotorSettingSpeed ();
                            // send Engine command to start motor
                            SendEngineMotorCmd ('M', aspeed);
                            SetBottomButtons (strings[BSTR_BACK], false, L"Stop", false, L"Open", false);
                        } else if (motorTestFlags.motorStartFlag == 1) {
                            motorTestFlags.motorStartFlag = 0;
                            // send commad to Engine to stop test
                            SendEngineMotorCmd ('m', 0);
                            SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Open", false);
                        }
                    }
                break;


                case 2://drawer open/close
                    // allow operation only motor test is not started.
                    if (motorTestFlags.motorStartFlag == 0) {
                        if (motorTestFlags.drawerOpenFalg == 0) {
                            SendDrawerMessage ('O');
                        } else {
                            SendDrawerMessage ('C');
                        }
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_UP:       
            switch (idata) {
                case 0:             //forward direction
                    if (motorSettings.direction == 1) {
                        motorSettings.direction = 0;
                        // format Gui message
                        wsprintf (motorData->motorDirection, L"%s", L"Forward");
                        motorTestFlags.guiUpdateFlag = 1;
                    }
                break;

                case 1:             // increase speed
                    if (motorSettings.speedIndex < MAX_MOTOR_SPEED_INDEX) {
                        motorSettings.speedIndex++;
                        // format Gui message
                        wsprintf (motorData->motorSpeedSetValue, L"%u",
                                 motorSpeedTable [motorSettings.speedIndex]);
                        motorTestFlags.guiUpdateFlag = 1;
                    }
                break;

                default:
                break;
            }

        break;

        case MSG_GUI_OUT_DOWN:      
            switch (idata) {
                case 0:             //reverse direction
                    if (motorSettings.direction == 0) {
                        motorSettings.direction = 1;
                        // format Gui message
                        wsprintf (motorData->motorDirection, L"%s", L"Reverse");
                        motorTestFlags.guiUpdateFlag = 1;
                        // send command to Engine
                    }
                break;

                case 1:             // decrease speed
                    if (motorSettings.speedIndex > 0) {
                        motorSettings.speedIndex--;
                        // format Gui message
                        wsprintf (motorData->motorSpeedSetValue, L"%u",
                                 motorSpeedTable [motorSettings.speedIndex]);
                        motorTestFlags.guiUpdateFlag = 1;
                    }
                break;

                default:
                break;
            }

            
        break;

        case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:      //messages from Engine board
            engineCommMsg = (EngineCommMessageData_t *)data;
            switch (engineCommMsg->message[0]) {
                case 'D':  // get Drawer status command 
                    status = engineCommMsg->message[8];
                    if (status & 0x80) {//drawer jammed
                        analyzeError = SERR_DRAWER_CLOSE_JAM;
                        wsprintf (motorData->motorSpeedReadValue,L"%s", L"Drawer jammed");    
                        LogError("drawer jammed");
                    } else if (status & 0x10) {//drawer closed
                        motorTestFlags.drawerOpenFalg = 0;
                        if (status & 0x40) {//rotor present
                            motorTestFlags.drawerReadyFlag =1;
                            wsprintf (motorData->motorSpeedReadValue, L"%s", L"Ready");
                        } else {
                            wsprintf (motorData->motorSpeedReadValue,L"%s", L"Load rotor");
                        }
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Open", false);
                    } else if (status & 0x20) { // drawer open
                        motorTestFlags.drawerOpenFalg = 1;
                        motorTestFlags.drawerReadyFlag = 0;
                        wsprintf (motorData->motorSpeedReadValue,L"%s", L"Load rotor");
                        SetBottomButtons (strings[BSTR_BACK], false, L"Start", false, L"Close", false);
                    }
                    motorTestFlags.guiUpdateFlag = 1;
                break;

                case 'G':  // get motor speed
                    if  (engineCommMsg->message [1] == 'M') {
                        speed = *(int *)(&engineCommMsg->message[6]);
                        //update speed display
                        wsprintf (motorData->motorSpeedReadValue,L"%d", speed);
                        motorTestFlags.guiUpdateFlag = 1;
                    }
                break;

                default:
                    ProcessOtherMessages (msgId, data);
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }

    // send message to Gui 
    if (motorTestFlags.guiUpdateFlag == 1) {
        motorTestFlags.guiUpdateFlag = 0;
        msg.msgId = MSG_GUI_UPDATE_READ_MOTOR_DATA;
        SendGuiMessage (&msg, sizeof (GuiReceiveMsg_t));
        // update Engine speed only test started
        if (motorTestFlags.motorStartFlag == 1) {
            aspeed = GetMotorSettingSpeed ();
            SendEngineMotorCmd ('M', aspeed);
        }
    }
    if (analyzeError != SERR_NONE) {
        infoText.type = MOTOR_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}

// get motor setting speed by index
unsigned int
StateMotorTest::
GetMotorSettingSpeed (void)
{
    int speed;

    if (motorSettings.speedIndex <= MAX_MOTOR_SPEED_INDEX) {
        speed = motorSpeedTable [motorSettings.speedIndex];
    }
    return speed;
}
