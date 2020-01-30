#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "AmsMotorTestStates.h"
#include "Shmem.h"
#include "LogErr.h"

// coefficent = 41.66667/20 = 2.0833
#define  POSITION_TO_SPEED_COEFFICIENT      2.0833


// error information
AmsMotorTestError_t   amsMotorTestError [MAX_AMS_MOTOR_TEST_ERROR];

// speed check range
AmsMotorCheckSpeedTable_t checkSpeedTable [] = 
                    {
                        {5000,   5500,  4500},
                        {-5000,  5500,  4500},
                        {7000,   7500,  6500}
                    };

// check index points
CheckPoints_t           checkPointTable [] = 
                    {
                        {P_5000_ENTRY, 300,     320},
                        {N_5000_ENTRY, 1000,    1020},
                        {N_7000_ENTRY, 6000,    8000},
                        {ACE_1_ENTRY,  320,     1000},
                        {ACE_2_ENTRY,  1020,    1800},
                    };




// error messages 
AmsMotorTestErrorStr_t    amsMotorTestErrorStr [] = 
                        {
                            {NO_AMS_MOTOR_ERROR,            "AMS motor test  pass"},
                            {AMS_MOTOR_TEST_FILE_FAIL,      "Fail to open ams motor data raw data file"},
                            {FIND_OUT_P_5000_ENTRY_FAIL,    "Fail to find out +5000 entry point"},
                            {FIND_OUT_N_5000_ENTRY_FAIL,    "Fail to find out -5000 entry point"},
                            {FIND_OUT_N_7000_ENTRY_FAIL,    "Fail to find out -7000 entry point"},
                            {FIND_OUT_ACE_1_ENTRY_FAIL,     "Fail to find out acc1 entry point"},
                            {FIND_OUT_ACE_2_ENTRY_FAIL,     "Fail to find out acc2 entry point"},
                            {SPEED_LOWER_THAN_P_5000,       "Velocity lower than 5000 RPM"},
                            {SPEED_HIGHER_THAN_P_5000,      "Velocity higher than 5000 RPM"},
                            {SPEED_LOWER_THAN_N_5000,       "Velocity lower  than -5000 RPM"},
                            {SPEED_HIGHER_THAN_N_5000,      "Velocity higher than -5000 RPM"},
                            {SPEED_LOWER_THAN_N_7000,       "Velocity lower than -7000RPM"},
                            {FAIL_TO_FIND_FIRST_ACCE,       "Fail to find out first acceleration"},
                            {FAIL_TO_FIND_SECOND_ACCE,      "Fail to find out second acceleration"},
                            {ACC_5000_MIN_FAIL,             "5000 RPM Acceleration fail"},
                            {ACC_4000_MIN_FAIL,             "4000 RPM Acceleration fail"},
                            {ACC_3000_MIN_FAIL,             "3000 RPM Acceleration fail"},
                            {ACC_2000_MIN_FAIL,             "2000 RPM Acceleration fail"},
                            {ACC_1000_MIN_FAIL,             "1000 RPM Acceleration fail"},
                            {DEC_4000_MIN_FAIL,             "4000 RPM decceleration fail"},
                            {DEC_3000_MIN_FAIL,             "3000 RPM decceleration fail"},
                            {DEC_2000_MIN_FAIL,             "2000 RPM decceleration fail"},
                            {DEC_1000_MIN_FAIL,             "1000 RPM decceleration fail"},
                            {DEC_0000_MIN_FAIL,             "0000 RPM decceleration fail"},
                            {UNKNOW_ERROR,                  "Unkown Error"}
                        };


// speed table
int velocityTable  [] = {
                            5000, 4000, 3000, 2000, 1000, 4000, 3000, 2000, 1000, 0
                        }; 




// find out check motor raw data start and end point
int
FindoutTestPoint (CheckEntry_t entry, int *startPoint, int *endPoint)
{
    int  i;
    AmsMotorTestError_t     testError;


    memset (&testError, 0, sizeof (AmsMotorTestError_t));


    *startPoint = 0;
    *endPoint = 0;

    for (i = 0; i < END_ENTRY; i++) {
        if (checkPointTable [i].entry == entry) {
            *startPoint = checkPointTable[i].startPoint;
            *endPoint   = checkPointTable[i].endPoint;
            return 0;
        } 
    }

    testError.errorStr.errorCode =  (AmsMotorTestErrorCode_t)(FIND_OUT_P_5000_ENTRY_FAIL + entry);
    strcpy (testError.errorStr.str, amsMotorTestErrorStr [testError.errorStr.errorCode].str);
    testError.flag = 1;
    memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
    return -1;
}

/*
 * compare function designed for qsort to use
 */
int
Intcmp(const void *v1, const void *v2)
{
     return (*(int *)v1 - *(int *)v2);
}


/*
 * find the meidan value, sort the five data
 * first and return middle value
 */
int
MedianIntData (int *sData)
{
    int sBuf [24];

    memset (sBuf, 0, sizeof (sBuf));

    memcpy (sBuf, sData, 3 * sizeof (int));

    qsort (sBuf, 3, sizeof (int), Intcmp);

    return sBuf [1];

}

/*
 * Filter function, designed to filter hardware
 * speed glitch
 *
 */

void
FilterMotorSpeed (int *velBuf, int length)
{
    int i, *ptr;;

    ptr = velBuf;

    for (i = 0; i < (length - 3); i++) {
       *ptr++ = MedianIntData (ptr);
    }
}


/*
 * find out two continue array data larger than chcek data index
 */
int
FindTwoLargerDataIndex (int *data, int checkData, int length)
{
    int i = 1;

    while (i < length) {
        if (data [i] > checkData) {
            if (data [i - 1] > checkData) {
                return i;
            }
            if (data [i +1]  > checkData) {
                return (i +1);
            }
            i += 3;
        } else {
            i += 2;
        }
    } 
    return -1;
} 

/*
 * find out two continue array data Litter than check data index
 */
int
FindTwoLitterDataIndex (int *data, int checkData, int length)
{
    int i = 1;
	 

    while (i < length) {
        if (data [i] < checkData) {
            if (data [i - 1] < checkData) {
                return i;
            }
            if (data [i +1]  < checkData) {
                return (i +1);
            }
            i += 3;
        } else {
            i += 2;
        }
    } 
    return -1;
} 

/*
 * get accelation data
 */
int
GetFirstAccelerateData (int *data, int checkLength, int *acData, int *index)
{
    int                     i;
    int                     *ptr;
    int                     checkSpeed [10]; 
    int                     idx = 0;
    AmsMotorTestError_t     testError;

    memset (&testError, 0, sizeof (AmsMotorTestError_t));
             

    ptr = data;

    // create check speed table
    for (i = 0; i < 10; i++) {
        checkSpeed [i] = (4 - i)* 1000; 
    }
    for (i = 0; i < 10; i++) {
        idx = FindTwoLitterDataIndex (ptr, checkSpeed [i], checkLength - idx); 
        if (idx != -1) {
            ptr += idx;
            acData [i] = *ptr - *(ptr-3);
        } else {
            // error code adding here
            testError.errorStr.errorCode =  FAIL_TO_FIND_FIRST_ACCE;
            strcpy (testError.errorStr.str, amsMotorTestErrorStr [testError.errorStr.errorCode].str);
            testError.index = *index;
            testError.checkSpeed =  checkSpeed [i];
            testError.flag = 1;
            memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
            LogDebug ("%s, checkSpeed =%d  ",testError.errorStr.str, testError.checkSpeed);
            return -1;
        }
        *index += idx;
    }


    return 0;
}

int
GetSecondAccelerateData (int *data, int checkLength, int *acData, int *index)
{
    int                   i;
    int                   *p;
    int                   checkSpeed [10]; 
    int                   idx = 0;
    AmsMotorTestError_t     testError;

    memset (&testError, 0, sizeof (AmsMotorTestError_t));
    p = data;

    // create check speed table
    for (i = 0; i < 10; i++) {
        checkSpeed [i] = (i - 4)* 1000; 
    }
    
    for (i = 0; i < 10; i++) {
        idx = FindTwoLargerDataIndex (p, checkSpeed [i], checkLength - idx); 
        if (idx != -1) {
            p += idx;
            acData [i] = *p - *(p-3);
        } else {
             // error code adding here
            testError.errorStr.errorCode =  FAIL_TO_FIND_SECOND_ACCE;
            strcpy (testError.errorStr.str, amsMotorTestErrorStr [testError.errorStr.errorCode].str);
            testError.index = *index;
            testError.checkSpeed =  checkSpeed [i];
            testError.flag = 1;
            memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
            LogDebug ("%s, checkSpeed =%d  ",testError.errorStr.str, testError.checkSpeed);
            return -1;
        }
        *index += idx;
    }


    return 0;
}

// sum smelen array and put in sumBuff
int 
SumArray (int *data, int length, int sumLen, int *sumBuf)
{
    int i;

    if (length < sumLen) {
        return -1;
    }

    sumBuf [0] = 0;

    // first sumLen sum data 
    for (i = 0; i < sumLen; i++) {
        sumBuf [0] += data [i];
    }

    for (i = 1; i < length - sumLen +1; i++) {
        sumBuf [i]= sumBuf [i -1] - data [i -1] + data [i + sumLen - 1];
    }

    return i;
}

// find out max speed -7000RPM average 5 speed mininal speed  
int
FindMaxAvgSpeedMinValue (int *data, int length, int *index)
{
    int   minSpeed;
    int   idx;
    int   *sumBuff; 
   // static int   sumBuff [2048];
    int   i;

    sumBuff = (int *)malloc (2048 * sizeof (int));
    if (sumBuff != NULL) {
        memset (sumBuff, 0 , sizeof (sumBuff));
    } else {
        LogError ("Fail to allocate sumBuff of FindMaxAvgSpeedMinValue");
        return 0;
    }
    idx = SumArray (data, length, 3, sumBuff);

    minSpeed = sumBuff [0];

    // find min value
    if (idx != -1) {
        for (i =0; i < idx; i++) {
            if (abs (minSpeed) < abs (sumBuff [i])) {
                minSpeed = sumBuff [i];
            }
        }
        *index = idx;
    } else {
        minSpeed = 0;
        *index = 0;
    }
    free (sumBuff);
    return (int)(minSpeed/3) ;
}


// get error code by speed checking

AmsMotorTestErrorCode_t
GetErrorCode (int checkSpeed, char higher)
{
    AmsMotorTestErrorCode_t errorCode = NO_AMS_MOTOR_ERROR;

    if (higher) {
        if (checkSpeed == 5000) {
           errorCode =  SPEED_HIGHER_THAN_P_5000;
        } else if (checkSpeed == -5000) {
           errorCode =  SPEED_HIGHER_THAN_N_5000;
        } else {
            errorCode = UNKNOW_ERROR;
        }
    } else {
        if (checkSpeed == 5000) {
           errorCode =  SPEED_LOWER_THAN_P_5000;
        } else if (checkSpeed == -5000) {
           errorCode =  SPEED_LOWER_THAN_N_5000;
        } else if (checkSpeed == -7000) {
            errorCode = SPEED_LOWER_THAN_N_7000;
        } else {
            errorCode = UNKNOW_ERROR;
        }
    }
    return errorCode;
}

// convert postion to speed
void
ConvertPostionToSpeed (unsigned short *position, int *speed, unsigned int length)
{
    unsigned int i;

    for (i = 0; i < length -1; i++) {
        speed [i] = (int)((position [i+1] - position [i]) * POSITION_TO_SPEED_COEFFICIENT);
        // take care wrraped over 
        if (abs (speed [i]) > 10000) {
             if (position [i] > 60000) {
                speed [i] = (abs (position [i+1]) + (0xffff - abs (position [i])));
            } else {
                speed [i] = ((0xffff - abs (position [i+1])) + abs (position [i])) * -1;
            }
            speed [i] *= POSITION_TO_SPEED_COEFFICIENT;
        } 
        // speed and potion is inverted
        speed [i] *= -1;
    }
}


// check speed stability 
int
CheckSpeedBounds (int *speed, CheckEntry_t checkEntry,  unsigned int length, int *index)
{
    unsigned int    i;
    int   maxSpeed, minSpeed, checkSpeed;
    AmsMotorTestError_t testError;

    memset (&testError, 0, sizeof (AmsMotorTestError_t));

    maxSpeed = checkSpeedTable [checkEntry].maxSpeed;
    minSpeed = checkSpeedTable [checkEntry].minSpeed;
    checkSpeed = checkSpeedTable [checkEntry].checkSpeed;

    for (i = 0; i < length; i++) {

    if (abs (speed [i]) > maxSpeed) {
            testError.errorStr.errorCode  =  GetErrorCode (checkSpeed, 1);
            if (testError.errorStr.errorCode <= UNKNOW_ERROR) {
                strcpy (testError.errorStr.str, amsMotorTestErrorStr [testError.errorStr.errorCode].str);
            }
            testError.index = *index;
            testError.checkSpeed = checkSpeed;
            testError.realSpeed  = speed [i];
            testError.flag = 1;
            memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
            LogError ("%s,checkSpeed=%d failSpeed=%d", testError.errorStr.str, testError.checkSpeed, testError.realSpeed);
            return -1;
        } else if (abs (speed [i]) < minSpeed) {
            testError.errorStr.errorCode  =  GetErrorCode (checkSpeed, 0);
            if (testError.errorStr.errorCode <= UNKNOW_ERROR) {
                strcpy (testError.errorStr.str, amsMotorTestErrorStr [testError.errorStr.errorCode].str);
            }
            testError.index = *index;
            testError.checkSpeed = checkSpeed;
            testError.realSpeed  = speed [i];
            testError.flag = 1;
            memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
            LogError ("%s,checkSpeed=%d failSpeed=%d", testError.errorStr.str, testError.checkSpeed, testError.realSpeed);
            return -1;
        }
        *index ++;
    }

    return 0;
}

//#define ACCE_PARAMETER      34.722   // acceleration convert parameter 
#define ACCE_PARAMETER        16.6668   // acceleration convert parameter 
// calcluate 
void
CalculateAcceleration (int rawData [][10], float *acc, float *dacc, float *dec , float *ddec)
{
    int i;
    int fData, sData; 

    for (i =0; i < 5; i++) {
        fData = rawData [0][9 - i];
        sData = rawData [1][9 - i];
        acc [i]     =  (float)(abs (fData) + abs (sData)) /2;
        acc [i]     *= ACCE_PARAMETER;
        dacc [i]    =  (float)(abs (fData) - abs (sData)) /2;
        dacc [i]    *= ACCE_PARAMETER;
    }

    for (i =0; i < 5; i++) {
        fData = rawData [0][i];
        sData = rawData [1][i];
        dec [i]     =  (float)(abs (fData) + abs (sData)) /2;
        dec [i]     *= ACCE_PARAMETER;
        ddec [i]    =  (float)(abs (fData) - abs (sData)) /2;
        ddec [i]    *= ACCE_PARAMETER;
    }
}


// final motor data checking 
void    
StateAmsMotorTestResultsCal::
MotorTestErrorCheck (int min7000Speed, float *acc, float *dec)
{
    int i ;
    unsigned short *chkPtr;
    AmsMotorTestError_t testError;
    memset (&testError, 0, sizeof (AmsMotorTestError_t));
    int errorCode;

    errorCode = (int)ACC_5000_MIN_FAIL;

    chkPtr = &manufacturingTestSet.acc5000Min;

    // first check acceleration
    for ( i = 0; i < 5; i++) {
        if (abs (acc[i]) < *chkPtr) {
            testError.errorStr.errorCode = (AmsMotorTestErrorCode_t)errorCode;
            testError.checkAcc = *chkPtr;
            testError.acc = acc[i];
            if (errorCode <= UNKNOW_ERROR) {
                strcpy (testError.errorStr.str, amsMotorTestErrorStr [errorCode].str);
                testError.flag = 1;
                memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
                LogError ("%s,checkAcce =%d failAcce=%f", testError.errorStr.str , testError.checkAcc, testError.acc);
                memset (&testError, 0, sizeof (AmsMotorTestError_t));
            }
        }
        errorCode++;
        chkPtr++;
    }
    // second check decceleration
    for ( i = 0; i < 5; i++) {
        if (abs (dec[i]) < *chkPtr) {
            testError.errorStr.errorCode = (AmsMotorTestErrorCode_t)errorCode;
            testError.checkDec = *chkPtr;
            testError.dec = dec[i];
            if (errorCode <= UNKNOW_ERROR) {
                strcpy (testError.errorStr.str, amsMotorTestErrorStr [errorCode].str);
                testError.flag = 1;
                memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
                LogError ("%s,checkDece =%d failDece=%f", testError.errorStr.str, testError.checkDec, testError.dec);
                memset (&testError, 0, sizeof (AmsMotorTestError_t));
            }
        }
        errorCode++;
        chkPtr++;
    }
    // check -7000 RPM 
    if (abs (min7000Speed) < manufacturingTestSet.motor7000MinSpeed) {
        if (errorCode <= UNKNOW_ERROR) {
                strcpy (testError.errorStr.str, amsMotorTestErrorStr [errorCode].str);
        }
        testError.flag = 1;
        testError.checkSpeed = manufacturingTestSet.motor7000MinSpeed;
        testError.realSpeed = min7000Speed;
        testError.errorStr.errorCode = SPEED_LOWER_THAN_N_7000;
        memcpy (&amsMotorTestError [testError.errorStr.errorCode], &testError, sizeof (AmsMotorTestError_t));
        LogError ("%s,checkSpeed=%d failSpeed=%d", testError.errorStr.str, testError.checkSpeed, testError.realSpeed);
    }

}

// check motor test data
int
StateAmsMotorTestResultsCal::
MotorDataCheck (RawData_t  *rawData)
{
    int                     checkDataLength = MAX_MOTOR_TEST_DATA;
    int                     speed [MAX_MOTOR_TEST_DATA];
    char                    buf [64];
    int                     i;
    FILE                    *fp;
    int                     startPoint;
    int                     endPoint;
    int                     checkLength;
    int                     index;
    int                     acceData [2][10];
    int                     min7000Speed;
    float                   acc [5], dacc [5], dec [5], ddec [5];          

    memset (amsMotorTestError, 0, sizeof (AmsMotorTestError_t));
    

    // ZZZ debug, disalbe 
#ifndef MOTOR_DEBUG 
    // open write file 
    fp = fopen (AMS_MOTOR_RAW_DATA_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to open file %s error =%d", AMS_MOTOR_RAW_DATA_FILE, GetLastError ());
        amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].errorStr.errorCode = AMS_MOTOR_TEST_FILE_FAIL;
        strcpy (amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].errorStr.str, amsMotorTestErrorStr [AMS_MOTOR_TEST_FILE_FAIL].str);
        amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].flag = 1;
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    for (i = 0; i < checkDataLength; i++) {
        // save data to file
        fwrite (&rawData->rotorRawReadings.motorTestData [i].position, sizeof (unsigned short) , 1, fp);
    }
    fclose (fp);

    // counvert position to speed
    ConvertPostionToSpeed (&rawData->rotorRawReadings.motorTestData [0].position,  speed, checkDataLength);

    //smooth speed (filtering speed glitch)
    FilterMotorSpeed (speed, checkDataLength - 1);
    
    // after filter, speed length = orignal raw data - 4
    checkDataLength -= 4;
    LogDebug ("checkDataLength =%d", checkDataLength);
    // generate speed text fromat file for future debug use
    fp = fopen (AMS_MOTOR_SPEED_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to pen file %s error =%d", AMS_MOTOR_RAW_DATA_FILE, GetLastError ());
        amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].errorStr.errorCode = AMS_MOTOR_TEST_FILE_FAIL;
        strcpy (amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].errorStr.str, amsMotorTestErrorStr [AMS_MOTOR_TEST_FILE_FAIL].str);
        amsMotorTestError [AMS_MOTOR_TEST_FILE_FAIL].flag = 1;
    } else {
        for (i = 0; i < checkDataLength ; i++) {
            sprintf (buf, "%d\n", speed [i]);
            fwrite (buf, strlen (buf), 1 ,fp); 
        }
        fclose (fp);
    }
#else
    fp = fopen (AMS_MOTOR_RAW_DATA_FILE, "rb");
    if (fp == NULL) {
        LogError ("Fail to open file %s error =%d", AMS_MOTOR_RAW_DATA_FILE, GetLastError ());
    }

    for (i = 0; i < checkDataLength; i++) {
        // save data to file
        fread (&rawData->rotorRawReadings.motorTestData [i].position, sizeof (unsigned short) , 1, fp);
    }
    fclose (fp);

    // counvert position to speed
    ConvertPostionToSpeed (&rawData->rotorRawReadings.motorTestData [0].position,  speed, checkDataLength);
    //smooth speed (filtering speed glitch)
    FilterMotorSpeed (speed, checkDataLength - 1);
    //ZZZ save speed as string for debug
    // after filter, speed length = orignal raw data - 4
    checkDataLength -= 4;
    
#endif

    index = 0;
    //  check first speed bound
    if (FindoutTestPoint (P_5000_ENTRY, &startPoint, &endPoint) == -1) {
        LogError ("Fail to find out check entry %d", P_5000_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    checkLength = endPoint - startPoint + 1;
    if (CheckSpeedBounds (&speed [startPoint] , P_5000_ENTRY,  checkLength, &index) == -1) {
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    // get first acclerate data
    //  check first speed bound
    if (FindoutTestPoint (ACE_1_ENTRY, &startPoint, &endPoint) == -1) {
        LogError ("Failt to find out check entry %d",ACE_1_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    checkLength = endPoint - startPoint + 1;
    if (GetFirstAccelerateData (&speed [startPoint], checkLength, &acceData [0][0], &index) == -1) {
        LogError ("Fail to get first acceleration data",ACE_1_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    //  check second speed bound
    if (FindoutTestPoint (N_5000_ENTRY, &startPoint, &endPoint) == -1) {
        LogError ("Fail to find out check entry %d", N_5000_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    checkLength = endPoint - startPoint + 1;
    if (CheckSpeedBounds (&speed [startPoint] , N_5000_ENTRY,  checkLength, &index) == -1) {
        // speed bound checking fail
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    // get second acclerate data
    if (FindoutTestPoint (ACE_2_ENTRY, &startPoint, &endPoint) == -1) {
        LogError ("Failt to find out check entry %d",ACE_2_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    checkLength = endPoint - startPoint + 1;
    if (GetSecondAccelerateData (&speed [startPoint], checkLength, &acceData [1][0], &index) == -1) {
        LogError ("Fail to get second acceleration data",ACE_1_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    // check -7000 RPM min speed
    if (FindoutTestPoint (N_7000_ENTRY, &startPoint, &endPoint) == -1) {
        LogError ("Failt to find out check entry %d",N_7000_ENTRY);
        FormatTestResults ( 0, 0, 0, 0, 0);
        return -1;
    }
    checkLength = endPoint - startPoint + 1;
    min7000Speed = FindMaxAvgSpeedMinValue (&speed [startPoint], checkLength, &index);
    //LogDebug ("Min 7000 RPM =%d", min7000Speed);
    // calculate acce dce values
    CalculateAcceleration (acceData, acc, dacc, dec , ddec);
    // need checking ...
    MotorTestErrorCheck (min7000Speed, acc, dec);

    // format test resutls display 
    FormatTestResults (min7000Speed, acc, dacc, dec, ddec);

#if 0
    // debug only
    for ( i = 0 ; i< 5; i++) {
        LogDebug ("acc = %f dacc =%f  dec =%f ddec =%f", acc [i], dacc [i], dec [i], ddec [i]);
    }
#endif
    return 0;
}



// format test results for display 
void
StateAmsMotorTestResultsCal::
FormatTestResults (int min7000Speed, float *acc, float *dacc, float *dec, float *ddec)
{
    int                 i;
    //char                formatStr [4000];
    char                *formatStr;
    char                *pBuff; 
	wchar_t				*dBuff;
    char                *header1 = "Speed   Accel  Diff     MinOK\n";
    char                *header2 = "Speed   Decel  Diff     MinOK\n";
    int                 errorCode;
    unsigned short      *minSpeed;
    FILE                *fp;
    SYSTEMTIME          time;
    char                firstCheckError = 0;

    formatStr = (char *)malloc (4000);
    if (formatStr == NULL) {
        LogError ("Fail to allocate formatStr of FormatTestResults");
        return;
    } 
    memset (displayText, 0 , sizeof (displayText));
    memset (formatStr, 0, sizeof (formatStr));
    pBuff = formatStr;
    dBuff = displayText;
    
    // get current time
    GetLocalTime(&time);
    sprintf (pBuff, "Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    pBuff += strlen (pBuff);

    wsprintf (dBuff, L"Test Time: %u/%u/%u %u:%u:%u\n", time.wMonth, time.wDay,  time.wYear,
                                                      time.wHour, time.wMinute,time.wSecond);

    LogDebug ("%s", WstrToCstr (dBuff));
    dBuff += wcslen (dBuff);

    //first check error, if fail , only display error string on the screen
    for (i = AMS_MOTOR_TEST_FILE_FAIL; i <= FAIL_TO_FIND_SECOND_ACCE; i++) {
        if (amsMotorTestError [i].flag == 1) {
            wsprintf(dBuff, L"%s", CstrToWstr (amsMotorTestError [i].errorStr.str));
            LogDebug ("%s",WstrToCstr (dBuff));
            dBuff += wcslen (dBuff);
            sprintf(pBuff, "%s", amsMotorTestError [i].errorStr.str);
            pBuff += strlen (pBuff);
            firstCheckError = 1;
        }
    }

    if (firstCheckError == 0) {
        errorCode = (int)ACC_5000_MIN_FAIL;
        sprintf (pBuff, "%s", header1);
        pBuff += strlen (pBuff);
        minSpeed = &manufacturingTestSet.acc5000Min;
        for ( i = 0; i < 5; i++) {
            if (amsMotorTestError [errorCode].flag) { 
                sprintf(pBuff,"%04u    *%4u   +/-%4u  %4u\n", velocityTable [i], abs ((int)acc [i]), abs ((int)dacc [i]), *minSpeed); 
            } else {
                sprintf(pBuff,"%04u     %4u   +/-%4u  %4u\n", velocityTable [i], abs ((int)acc [i]), abs ((int)dacc [i]), *minSpeed); 
            }
            pBuff += strlen (pBuff);
            errorCode++;
            minSpeed++;
        }
        sprintf (pBuff, "\n");
        pBuff += strlen (pBuff);

        sprintf (pBuff, "%s", header2);
        pBuff += strlen (pBuff);
        errorCode = (int)DEC_4000_MIN_FAIL;
        minSpeed = &manufacturingTestSet.dec4000Min;
        for ( i = 0; i < 5; i++) {
            if (amsMotorTestError [errorCode].flag) { 
                sprintf(pBuff,"%04u    *%4u   +/-%4u  %4u\n", velocityTable [i +5], abs ((int)dec [i]), abs ((int)ddec [i]), *minSpeed); 
            } else {
                sprintf(pBuff,"%04u     %4u   +/-%4u  %4u\n", velocityTable [i +5], abs ((int)dec [i]), abs ((int)ddec [i]), *minSpeed); 
            }
            pBuff += strlen (pBuff);
            errorCode++;
            minSpeed++;
        }

        sprintf (pBuff, "\n");
        pBuff += strlen (pBuff);

        if (amsMotorTestError [SPEED_LOWER_THAN_N_7000].flag) {
            sprintf (pBuff, "7000     *%4u  OkSpeed =%4u\n", abs (min7000Speed), manufacturingTestSet.motor7000MinSpeed); 
        } else {
            sprintf (pBuff, "7000     %4u   OkSpeed =%4u\n", abs (min7000Speed), manufacturingTestSet.motor7000MinSpeed); 
        }
        pBuff += strlen (pBuff);

        sprintf (pBuff, "\n");
        pBuff += strlen (pBuff);

        sprintf(pBuff, "%s\n", "Final Motor Test Report:");
        pBuff += strlen (pBuff);

        errorCode = 0;
        //  errors messages
        for (i = 2; i < MAX_AMS_MOTOR_TEST_ERROR; i++) {
            if (amsMotorTestError [i].flag ==1) {
                sprintf(pBuff, "%s\n", amsMotorTestError [i].errorStr.str);
                pBuff += strlen (pBuff);
                LogDebug ("%s", amsMotorTestError [i].errorStr.str);
                // for display only
                wsprintf (dBuff, L"%s\n", CstrToWstr (amsMotorTestError [i].errorStr.str));
                LogDebug ("%s",WstrToCstr (dBuff));
                dBuff +=  wcslen (dBuff);
            }
        }
        if (errorCode == 0) {
            sprintf(pBuff, "%s\n", "Motor Test: Pass");
            pBuff += strlen (pBuff);
            wsprintf (dBuff, L"%s\n", L"Motor Test: Pass");
            LogDebug ("%s", WstrToCstr (dBuff));
            dBuff +=  wcslen (dBuff);
        }  else {
            sprintf(pBuff, "%s\n", "Motor Test: Fail");
            pBuff += strlen (pBuff);
            wsprintf (dBuff, L"%s\n", L"Motor Test: Fail");
            LogDebug ("%s",WstrToCstr (dBuff));
            dBuff +=  wcslen (dBuff);
        }
    }
    // end of display messages 
    *pBuff = '\0';
    *dBuff = L'\0';

    fp = fopen (AMS_MOTOR_TEST_REPORT_FILE, "wb");
    if (fp == NULL) {
        LogError ("Fail to pen file %s error =%d", AMS_MOTOR_TEST_REPORT_FILE, GetLastError ());
    } else {
        fwrite (formatStr, strlen (formatStr), 1, fp);
        fclose (fp);
        
    }
    free (formatStr);
}
