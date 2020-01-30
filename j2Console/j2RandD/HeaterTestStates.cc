#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "HeaterTestStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "Shmem.h"
#include "LogErr.h"

#define PLATE_TEMPERATURE_DAC_CONVERT_MULT      0.197952    // 197.952 millidegree C per plate DAC count
#define PLATE_TEMPERATURE_DAC_CONVERT_OFFSET    7.758012    // 7.758012 degree C plate temperature DAC convert offset

HeaterValues_t      StateHeateTest::heaterValues;
HeaterValues_t      StateHeateTest::prevHeaterValues;
HeaterTestFlags_t   StateHeateTest::heaterTestFlags;

// waiting for gui
#define            GUI_DONE



// calculate target set temperature value
float 
StateHeateTest::
CalculateTargetTemperature (unsigned short setDac)
{
    float tempValue;

    tempValue = ((float)setDac * PLATE_TEMPERATURE_DAC_CONVERT_MULT) + PLATE_TEMPERATURE_DAC_CONVERT_OFFSET;

    return tempValue;
}

// convert temperature value to string
void
StateHeateTest::
SetGuiTemperatureValues (GuiMiddleTemperatureData_t *temperatureData)
{
    wsprintf (temperatureData->rotorTemperatureValue, L"%.3f", heaterValues.rotorTemperature);
    wsprintf (temperatureData->ambientTemperatureValue, L"%.3f", heaterValues.ambientTemperature);
    wsprintf (temperatureData->ambientDacOffsetValue, L"%u", heaterValues.ambientDacOffset);
    wsprintf (temperatureData->airTemperatureOffsetValue, L"%.3f", heaterValues.rotorTemperatureOffset);
    wsprintf (temperatureData->topHeaterTemperatureValue, L"%.3f", heaterValues.topPlateTemperature);
    wsprintf (temperatureData->bottomHeaterTemperatureValue, L"%.3f", heaterValues.bottomPlateTemperature);
    wsprintf (temperatureData->topHeaterCurrentValue, L"%.2f", heaterValues.topPlateVoltage);
    wsprintf (temperatureData->bottomHeaterCurrentValue, L"%.2f", heaterValues.bottomPlateVoltage);
}

// set all vaule to be zero string
void
StateHeateTest::
SetAllValuesZero (GuiMiddleTemperatureData_t *temperatureData)
{
    // enter test window, all data fied should be 0 
    temperatureData->rotorTemperatureValue [0]          = '0';
    temperatureData->ambientTemperatureValue [0]        = '0';
    temperatureData->ambientDacOffsetValue [0]          = '0';
    temperatureData->airTemperatureOffsetValue [0]      = '0';
    temperatureData->topHeaterTemperatureValue  [0]     = '0';
    temperatureData->bottomHeaterTemperatureValue [0]   = '0';
    temperatureData->topHeaterCurrentValue [0]          = '0';
    temperatureData->bottomHeaterCurrentValue [0]       = '0';
    temperatureData->topHeaterTemperatureSetValue [0]   = '0';
    temperatureData->bottomHeaterTemperatureSetValue[0] = '0';
    temperatureData->topDacSetValue [0]                 = '0';
    temperatureData->bottomDacSetValue [0]              = '0';
}

// convet top DAC value to string
void
StateHeateTest::
SetGuiTopDacValue (GuiMiddleTemperatureData_t *temperatureData)
{
    wsprintf (temperatureData->topHeaterTemperatureSetValue, L"%u", heaterValues.topHeaterDacValue);
    wsprintf (temperatureData->topDacSetValue, L"%.2f", CalculateTargetTemperature (heaterValues.topHeaterDacValue));
}

// convet bottom DAC value to string
void
StateHeateTest::
SetGuiBotDacValue (GuiMiddleTemperatureData_t *temperatureData)
{
    wsprintf (temperatureData->bottomHeaterTemperatureSetValue, L"%u", heaterValues.bottomHeaterDacValue);
    wsprintf (temperatureData->bottomDacSetValue, L"%.2f", CalculateTargetTemperature (heaterValues.bottomHeaterDacValue));
}

// Send TemperatureMsgToGui
void
StateHeateTest::
SendTemperatureMsgToGui (SendGuiItems_t sendItem)
{
    GuiReceiveMsg_t              msg;
    GuiMiddleTemperatureData_t   *temperatureData ;

    temperatureData = &msg.data.middleTemperature;
    // zero buffers, only updated values could be copied to string
    memset (temperatureData, 0, sizeof GuiMiddleTemperatureData_t);
    msg.msgId = MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES;

    switch (sendItem) {
        case SEND_ZERO:
            msg.msgId = MSG_GUI_MIDDLE_TEMPERATURE_INFO;
            SetAllValuesZero (temperatureData);
        break;

        case SEND_ALL:
            msg.msgId = MSG_GUI_MIDDLE_TEMPERATURE_INFO;
            SetGuiTemperatureValues (temperatureData);
            SetGuiTopDacValue (temperatureData);
            SetGuiBotDacValue (temperatureData);
        break;

        case SEND_UPDATA_TEMPERATURE:
            SetGuiTemperatureValues (temperatureData);
        break;

        case SEND_TOP_DAC_VAULE:
            SetGuiTopDacValue (temperatureData);  
        break;

        case SEND_BOTTOM_DAC_VALUE:
            SetGuiBotDacValue (temperatureData);
        break; 

        default:
        break;

        
    }
#ifdef GUI_DONE
    SendGuiMessage (&msg, sizeof (GuiReceiveMsg_t));
#endif
}

// wait until analyzer warmed up, then allow to start heater test
void
StateWaitWarmup::
Enter (void)
{
    GuiReceiveMsg_t     msg;
    wchar_t				*topLable =  L"Analyzer Warming Up";
    wchar_t             *str = L"Analyzer warming up, please wait ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiReceiveMsg_t));

    SetBottomButtons (strings[BSTR_BACK], true);
}

state_t
StateWaitWarmup::
ProcessMessage (int msgId, char *data)
{
    int retVal = STATE_WAIT_ANALY_WARM_UP;
    
    switch (msgId) {
            case MSG_MANUFACTURE_WARMUP_DONE:
                warmupDoneFlag = 1;
                LogDebug ("RandD Received Warmup Done Status");
                retVal = STATE_HEATER_TEST; 
            break;

            case MSG_GUI_OUT_BOTTOM_BUTTON:
                retVal = stateStack.Pop();
            break;

            default:
                ProcessOtherMessages (msgId, data);
            break;
    }
    return retVal;
}

// initial screen set all values to be 0
void
StateHeateTest::
Enter (void)
{
    wchar_t                         *topLable =  L"Temperature Test";

    // send message to heaterProcess to get required parameters
    SendHeaterControlMessage (MSG_HEATER_GET_TEMPERATURE_VALUES);

    memset (&heaterTestFlags, 0, sizeof (HeaterTestFlags_t));

    SetTopLabel (topLable);

    // send all zero values at beginning
    SendTemperatureMsgToGui (SEND_ZERO);

    SetBottomButtons (strings[BSTR_BACK], false, L"Apply", false, L"Start", true);

    heaterTestFlags.pollingFlag = 0;
    
}
state_t
StateHeateTest::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_HEATER_TEST;
    int                         idata = *(int *)data;
    HeaterControlMsg_t          hMsg;

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0: //back need to be trouble shooting
                    // disable polling messages
                    SendHeaterControlMessage (MSG_HEATER_TEST_DISABLE);
                    retVal = stateStack.Pop();
                break;

                case 1://apply setting values
                    hMsg.msgId = MSG_HEATER_SET_TEMPERATURE_VALUES;
                    memcpy (&hMsg.data [0], &heaterValues.topHeaterDacValue, 2);
                    memcpy (&hMsg.data [2], &heaterValues.bottomHeaterDacValue, 2);
                    // send setting message to heaterControl process
                    SendHeaterControlMessage (&hMsg, 64);
                break;

                case 2://start polling temperature
                    // send gui current heater value
                    // send command to Heater process to enable polling
                    if (heaterTestFlags.startFlag == 0) {
                        SendHeaterControlMessage (MSG_HEATER_TEST_ENABLE);
                        // record current values
                        memcpy (&prevHeaterValues, &heaterValues, sizeof (HeaterValues_t));
                        SendTemperatureMsgToGui (SEND_ALL);
                        SetBottomButtons (strings[BSTR_BACK], false, L"Apply", false, L"Stop", true);
                        heaterTestFlags.pollingFlag = 1;
                        heaterTestFlags.startFlag = 1;
                    } else {
                        SendHeaterControlMessage (MSG_HEATER_TEST_DISABLE);
                        SetBottomButtons (strings[BSTR_BACK], false, L"Apply", false, L"Start", true);
                        heaterTestFlags.pollingFlag = 0;
                        heaterTestFlags.startFlag = 0;
                    }
                break;

                default:
                break;
            }
        break;


        case MSG_GUI_OUT_UP:  // increase dac value
            switch (idata) {
                case 0:

                    if (!heaterTestFlags.startFlag && (heaterValues.topHeaterDacValue < 255)) {
                        heaterValues.topHeaterDacValue++;
                        SendTemperatureMsgToGui (SEND_TOP_DAC_VAULE);
                    }
                break;

                case 1:
                    if (! heaterTestFlags.startFlag && (heaterValues.bottomHeaterDacValue < 255)) {
                        heaterValues.bottomHeaterDacValue++;
                        SendTemperatureMsgToGui (SEND_BOTTOM_DAC_VALUE);
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_GUI_OUT_DOWN: // descreaes dac value
             switch (idata) {
                case 0:
                    if (!heaterTestFlags.startFlag && (heaterValues.topHeaterDacValue > 0)) {
                        heaterValues.topHeaterDacValue--;
                        SendTemperatureMsgToGui (SEND_TOP_DAC_VAULE);
                    }
                break;

                case 1:
                    if (!heaterTestFlags.startFlag && (heaterValues.bottomHeaterDacValue > 0)) {
                        heaterValues.bottomHeaterDacValue--;
                        SendTemperatureMsgToGui (SEND_BOTTOM_DAC_VALUE);
                    }
                break;

                default:
                break;
            }
        break;

        case MSG_HEATER_SEND_TEMPERATURE_VALUES:
            // get heater temperature values
            memcpy ((char *)&heaterValues, data, sizeof (HeaterValues_t));
#if 0
            // debug messages
            printf ("rotortemp =%.1f ambTemp=%.1f topTemp=%.1f bomtemp=%.1f\n\r",
                     heaterValues.rotorTemperature, 
                     heaterValues.ambientTemperature,
                     heaterValues.topPlateTemperature,
                     heaterValues.bottomPlateTemperature);
            printf ("topVol=%.1f, bomVol=%.1f, rotorOffset=%.1f ambOffset=%u\n\r",
                     heaterValues.topPlateVoltage, 
                     heaterValues.bottomPlateVoltage,
                     heaterValues.rotorTemperatureOffset,
                     heaterValues.ambientDacOffset);
            printf ("topDac =%u bomDac =%u topSetTemp=%.1f botSetTemp=%.1f\n\r", 
                     heaterValues.topHeaterDacValue,
                     heaterValues.bottomHeaterDacValue,
                     CalculateTargetTemperature (heaterValues.topHeaterDacValue), 
                     CalculateTargetTemperature (heaterValues.bottomHeaterDacValue));
#endif
            if (heaterTestFlags.pollingFlag == 1) {
                if (prevHeaterValues.topHeaterDacValue != heaterValues.topHeaterDacValue) {
                    SendTemperatureMsgToGui (SEND_TOP_DAC_VAULE);
                }
                if (prevHeaterValues.bottomHeaterDacValue != heaterValues.bottomHeaterDacValue) {
                    SendTemperatureMsgToGui (SEND_BOTTOM_DAC_VALUE);
                }
                // always update temperature
                SendTemperatureMsgToGui (SEND_UPDATA_TEMPERATURE);
                // record current values
                memcpy (&prevHeaterValues, &heaterValues, sizeof (HeaterValues_t));
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    if (analyzeError != SERR_NONE) {
        infoText.type = TEMPERATURE_TEST_TYPE;
        retVal = STATE_MANUFACTURING_ERROR;
    }
    return retVal;
}
