#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <math.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "AmsTempTestStates.h"
#include "Shmem.h"
#include "LogErr.h"

TempFinalErrorReport_t  tempFinalErrorReportTable [] = 
                    {
                        {"Temperature Test Pass!!!",        NO_TEMP_ERROR_REPORT, " "},        
                        {"Rotor Ohms Out of Range",         ROTOR_OHMS_OUT_RANGE, " "},
                        {"Fail to alloc memory",            TEMP_MEM_ALLOC_ERROR, " "},
                        {"Temperature too low",             TEMP_TOO_LOW, " "},
                        {"Temperature too high",            TEMP_TOO_HIGH, " "},
                        {"Initial temperature too low",     TEMP_INIT_TOO_LOW, " "},
                        {"Initial temperature too high",    TEMP_INIT_TOO_HIGH, " "},
                        {"Avrage temperature too low",      TEMP_AVG_TOO_LOW, " "},
                        {"Avrage temperature too high",     TEMP_AVG_TOO_HIGH, " "},
                        {"Temperature offset too low",      TEMP_OFFSET_TOO_LOW, " "},
                        {"Temperature offset too high",     TEMP_OFFSET_TOO_HIGH, " "},
                        {"Standard Deviation too low",      TEMP_SDT_TOO_LOW, " "},
                        {"Standard Deviation too high",     TEMP_SDT_TOO_HIGH, " "}
                        
                    };

//Ams Temperature Test Results Calculation
void
StateAmsTempTestResultsCal::
Enter (void)
{
    GuiReceiveMsg_t         msg;
    wchar_t                 *topLable = L"Temperature Test Results Calculation";
    wchar_t                 *str = L"Results Calculating ...";

    SetTopLabel (topLable);

    msg.msgId = MSG_GUI_MIDDLE_MESSAGE;
    wcscpy (msg.data.middleMessage.message, str);
    SendGuiMessage(&msg, sizeof (GuiMiddleMessageData_t));
    SetBottomButtons ();

    // calculate test results
    CalculateTestResults ();

    // generate test report
    FormatTestReport ();

    // send message queue to itself
    SendManufactureMessage (MSG_MANUFACTURE_TEMP_TEST_CAL_COMPLETE);
}

state_t
StateAmsTempTestResultsCal::
ProcessMessage (int msgId, char *data)
{
    int                         retVal = STATE_AMS_TEMP_TEST_RESULTS_CAL;

    switch (msgId) {
        case MSG_MANUFACTURE_TEMP_TEST_CAL_COMPLETE:
            if ((errorReport == 0) || (testCount >= 2)) {
                //test passed, show test results;
                retVal = STATE_AMS_TEMP_TEST_SHOW_RESULTS;
            } else {
                // test fail, try again
                testCount++;
                // pop up STATE_AMS_TEMP_TEST state 
                retVal = stateStack.Pop();
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
        break;
    }
    return retVal;
}


// read temperature raw data, calculate temperature and save the data on the file
void
StateAmsTempTestResultsCal::
GetTemperatureData (void)
{
    int     i;
    float   offset = 0;
    FILE    *fp;
    char    *pBuff;
    char tempStr [256];

    pBuff = tempStr;
    memset (tempStr, 0, sizeof (tempStr));

    for (i = 0; i < MAX_TEMP_DATA; i++) {
        // Note: for AMS temperature test, initial offset is 0 
        tempData [i] = (float)((float)rawData->rotorRawReadings.temperatures [i].rotorTemperature * ROTOR_TEMPERATURE_CONVERT_MULT) + offset;
//      LogDebug ("Temperature [%d] =%f",i, tempData [i]);
        sprintf (pBuff,"%f\n", tempData [i]);
        pBuff += strlen (pBuff);
    } 
    fp = fopen (AMS_TEMPERATURE_DATA_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to open file %s", AMS_TEMPERATURE_DATA_FILE);
    } else {
        fwrite (tempStr, strlen (tempStr), 1, fp);
        fclose (fp);
    }
}

float
StateAmsTempTestResultsCal::
OhmToDegree ()
{
   float K    = 273.15;  /* kelvin offset factor */
   float B    = 3929.023;
   float T0   = 35.0;
   float RT0  = 6531.0;
   float ohms;

   ohms = tempRotor.offset + tempRotor.rotorOhms; 

   return (( B / (log(ohms/RT0) + (B/(T0 + K)))) - K); 
    
}

void
StateAmsTempTestResultsCal::
CalDacValues (void)
{
    unsigned char dacOffset, dac, dacMax;
    
    dacOffset = (unsigned char)(THERM_MULT * (THERM_OFFSET - tempCalResults.rotorTemp));
    dacMax = (unsigned char)DEFAULT_AMS_BOTTOM_H_DAC;
    dac    = (unsigned char)DEFAULT_AMS_TEMP_TEST_DAC;
    dac += dacOffset;
    if (dac < dacMax) {
        tempCalResults.topDac = dac;
        tempCalResults.botDac = dac;
    } else {
        tempCalResults.topDac = dacMax;
        tempCalResults.botDac = dacMax;
    }
}

void
StateAmsTempTestResultsCal::
CalculateTestResults (void)
{
    float   sum = 0, avg, max, min, std, s, x;
    int i;

#ifdef TEMP_SIMULATION
    // temperature simulation data
    float simuTemp [] = {
        36.893749,
        36.662498,
        36.931252,
        36.868752,
        36.918751,
        36.950001,
        36.981251,
        37.006248,
        37.025002,
        37.043751,
        37.056252,
        37.068748,
        37.081249
    };

    memcpy (tempData, simuTemp, sizeof (tempData));
#else
    // read temperature from data base
    GetTemperatureData ();
#endif

    max = tempData [3];
    min = tempData [3];
    // calculate last 10 (from 4 to 13)temperature (final 10 loops) 
    for (i = 3; i < 13; i++) {
        sum += tempData [i];
        if (tempData [i] > max) {
            max = tempData [i];
        }
        if (tempData [i] < min) {
            min = tempData [i];
        }
    }

    // calculate std
    avg = sum / 10;
    for (s = 0, i = 3; i < 13; i++) {
        x = tempData [i] - avg;
        s += (x *x);
    }
    std = sqrt (s /10);

    tempCalResults.tempAvg = avg;
    tempCalResults.tempMax = max; 
    tempCalResults.tempMin = min;
    tempCalResults.tempRange = max - min;
    tempCalResults.tempStddev = std;
    tempCalResults.tempInit  = tempData [0];
    tempCalResults.rotorTemp = OhmToDegree ();
    tempCalResults.offset   = tempCalResults.rotorTemp - tempCalResults.tempAvg;
    CalDacValues ();
}


void
StateAmsTempTestResultsCal::
FormatTestReport ()
{
    char                *reportStr;
    char                *pBuff; 
	wchar_t				*dBuff;
    int                 i;
    FILE                *fp;
    SYSTEMTIME          time;

    dBuff = displayText;
    reportStr = (char *)malloc (MAX_TEMP_REPORT_STR_LENGTH);

    if (reportStr == NULL) {
        LogError ("Fail to allocate temperature report memory");
        wcscpy  (displayText, L"Fail to alloc temperature report memory");
        return ;
    }
    memset (reportStr, 0, MAX_TEMP_REPORT_STR_LENGTH);
    pBuff = reportStr;

    // get courrent time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    pBuff += strlen (pBuff);

    wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);
    LogDebug ("%s", WstrToCstr (dBuff));
    dBuff += wcslen (dBuff);



    sprintf (pBuff, "          --Intial Condition--\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Heater DAC Settings:\n");    
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Top_Dac   Bottom_Dac\n");     
    pBuff += strlen (pBuff);
    sprintf (pBuff, "%d         %d\n", tempDefaultDac [0], tempDefaultDac [1]);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Temperatures\n");     
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Air     Ambient    Top      Bottom\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "%2.3f  %2.3f    %2.3f    %2.3f\n", tempInitValues.airInitTemp,
                                                        tempInitValues.ambInitTemp,
                                                        tempInitValues.topInitTemp,
                                                        tempInitValues.botInitTemp);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Temperature Offset = %2.3f\n\n", tempInitValues.offset);
    pBuff += strlen (pBuff);

    sprintf (pBuff, "   --Temperature Test Measurements--\n");
    pBuff += strlen (pBuff);

    sprintf (pBuff, "Test numbers: %d\n", testCount + 1);
    pBuff += strlen (pBuff);
    
    sprintf (pBuff, "Heater DAC Settings:\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Top_Dac   Bottom_Dac\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "%d         %d\n",tempInitValues.initDac [0], tempInitValues.initDac [1]);
    pBuff += strlen (pBuff);
    // copy calibrated Dac value to be next calibration initial DAC value in case test fail
    LogDebug ("calculated DAC value =%d", tempCalResults.topDac);
    tempInitValues.initDac [0] = tempCalResults.topDac;
    tempInitValues.initDac [1] = tempCalResults.botDac;
    
    sprintf (pBuff, "Temperatuers:\n");
    pBuff += strlen (pBuff);
    for (i = 3; i < 13; i++) {
        sprintf (pBuff,"# %d:  %2.3f\n", i - 3, tempData [i]);
        pBuff += strlen (pBuff);
    }
    
    sprintf (pBuff, "Mean=%2.3f, Range=%2.3f, Std=%2.3f\n\n", tempCalResults.tempAvg, 
                                                             tempCalResults.tempRange, 
                                                             tempCalResults.tempStddev);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "   --Temperatuer Rotor Information--\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Rotor Id =%d, Rotor Offset =%2.3f\n",tempRotor.rotorId, tempRotor.offset);
    pBuff += strlen (pBuff);

    sprintf (pBuff, "Rotor resistance =%dOhms\n",tempRotor.rotorOhms);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Rotor Temperaure =%2.3f\n\n",tempCalResults.rotorTemp);
    pBuff += strlen (pBuff);


    // final test conclution
    memset (tempFinalReport, 0, (sizeof (TempFinalErrorReport_t) * END_TEMP_ERROR));

    // rotor temperature 
    if (tempCalResults.rotorTemp < manufacturingTestSet.rotorTempMin) {
        tempFinalReport[TEMP_TOO_LOW].error = TEMP_TOO_LOW;
        strcpy (tempFinalReport[TEMP_TOO_LOW].final, "*Too low");
    } else if (tempCalResults.rotorTemp > manufacturingTestSet.rotorTempMax) {
        tempFinalReport[TEMP_TOO_HIGH].error = TEMP_TOO_HIGH;
        strcpy (tempFinalReport[TEMP_TOO_LOW].final, "*Too High");
    }
    // average temperature
    if (tempCalResults.tempAvg < manufacturingTestSet.rotorTempAveLow) {
        tempFinalReport[TEMP_AVG_TOO_LOW].error = TEMP_AVG_TOO_LOW;
        strcpy (tempFinalReport[TEMP_AVG_TOO_LOW].final, "*Too low");
    } else if (tempCalResults.tempAvg > manufacturingTestSet.rotorTempAveHigh) {
        tempFinalReport[TEMP_AVG_TOO_HIGH].error = TEMP_AVG_TOO_HIGH;
        strcpy (tempFinalReport[TEMP_AVG_TOO_LOW].final, "*Too High");
    } 
    //initial air
    if (tempData [0] < manufacturingTestSet.rotorTempInitLow) {
        tempFinalReport[TEMP_INIT_TOO_LOW].error = TEMP_INIT_TOO_LOW;
        strcpy (tempFinalReport[TEMP_INIT_TOO_LOW].final, "*Too low");
    } else if (tempData [0] > manufacturingTestSet.rotorTempInitHigh) {
        tempFinalReport[TEMP_AVG_TOO_HIGH].error = TEMP_AVG_TOO_HIGH;
        strcpy (tempFinalReport[TEMP_AVG_TOO_LOW].final, "*Too High");
    }
    // temperature offset
    if (tempCalResults.offset < manufacturingTestSet.rotorTempOffsetMin) {
        tempFinalReport[TEMP_OFFSET_TOO_LOW].error = TEMP_OFFSET_TOO_LOW;
        strcpy (tempFinalReport[TEMP_OFFSET_TOO_LOW].final, "*Too low");
    } else if (tempCalResults.offset > manufacturingTestSet.rotorTempOffsetMax) {
        tempFinalReport[TEMP_OFFSET_TOO_HIGH].error = TEMP_OFFSET_TOO_HIGH;
        strcpy (tempFinalReport[TEMP_OFFSET_TOO_LOW].final, "*Too High");
    } 
    // std div
    if (tempCalResults.tempStddev < manufacturingTestSet.rotorTempstdDevMin) {
        tempFinalReport[TEMP_SDT_TOO_LOW].error = TEMP_SDT_TOO_LOW;
        strcpy (tempFinalReport[TEMP_SDT_TOO_LOW].final, "*Too low");
    } else if (tempCalResults.tempStddev > manufacturingTestSet.rotorTempstdDevMax) {
        tempFinalReport[TEMP_SDT_TOO_HIGH].error = TEMP_SDT_TOO_HIGH;
        strcpy (tempFinalReport[TEMP_SDT_TOO_LOW].final, "*Too High");
    } 

    sprintf (pBuff, "      --Final conditions--\n");
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Rotor       :%2.3f %s\n",tempCalResults.rotorTemp, tempFinalReport[TEMP_TOO_LOW].final);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Average Air :%2.3f %s\n",tempCalResults.tempAvg,tempFinalReport[TEMP_AVG_TOO_LOW].final);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Initial Air :%2.3f %s\n",tempData [0], tempFinalReport[TEMP_AVG_TOO_LOW].final);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Final Offset:%2.3f %s\n",tempCalResults.offset, tempFinalReport[TEMP_OFFSET_TOO_LOW].final);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "Std Dev     :%2.3f %s\n\n",tempCalResults.tempStddev, tempFinalReport[TEMP_SDT_TOO_LOW].final);
    pBuff += strlen (pBuff);

    sprintf (pBuff, "     --Temperature Test Results--\n");
    pBuff += strlen (pBuff);

    errorReport = 0;
    // now search string and ...
    for (i = 1; i < END_TEMP_ERROR; i++) {
        if (tempFinalReport [i].error != 0) {
            errorReport = 1;
            // for report
            sprintf (pBuff,"%s\n", tempFinalErrorReportTable[i].str);
            pBuff += strlen (pBuff);
            // for results display 
            wsprintf  (dBuff,L"%s\n", CstrToWstr (tempFinalErrorReportTable[i].str));
            LogDebug ("%s",WstrToCstr(dBuff));
            dBuff += wcslen (dBuff);
        }
    }  
    // no error
    if (errorReport == 0) {
        // for report
        sprintf (pBuff,"Temperature Test: Pass");
        pBuff += strlen (pBuff);
        // for results display
        wsprintf  (dBuff,L"Temperature Test: Pass");
        LogDebug ("%s",WstrToCstr(dBuff));
        dBuff += wcslen (dBuff);
    } else {
        // for report
        sprintf (pBuff,"Temperature Test: Fail");
        pBuff += strlen (pBuff);
        // for results display
        wsprintf  (dBuff, L"Temperature Test: Fail");
        LogDebug ("%s", WstrToCstr (dBuff));
        dBuff += wcslen (dBuff);
    }

    // save to file
    fp = fopen (AMS_TEMPERATURE_REPORT_FILE, "wb");
    if (fp != NULL) {
        fwrite (reportStr, 1 ,strlen (reportStr), fp);
        fclose (fp);
    }
    free (reportStr);
}
