#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include "Serial.h"
#include "XmlData.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "CommonDefines.h"
#include "EngComm.h"
#include "Temperature.h"
#include "State.h"
#include "AmsComm.h"
#include "Util.h"
#include "RotorBarcode.h"
#include "LogErr.h"
#include "Global.h"

HANDLE                  AmsEngineCommQueue = NULL;


/*
 * FUNCTION
 *   InitEngineComm 
 *
 * DESCRIPTION
 *   Initial AmsEngineComm message queue and prcocess
 *
 * PARAMETERS
 *   nont
 *
 * RETURNS
 *   bool   --- false, initial fail
 *          --- true   initial sucessfule
 */

bool
CreateAmsEngineQueue (void)
{

     /* create engine comm message queue */
    AmsEngineCommQueue = CreateMessageQueue (AMS_ENGINE_COMM_QUEUE_NAME, false);
    if (AmsEngineCommQueue == NULL) {
        return false;
    }
    return true;
}

void
CloseAmsEngineQueue (void)
{
   if (AmsEngineCommQueue != NULL) {
        CloseMsgQueue (AmsEngineCommQueue);
   }   
}


/*
 * FUNCTION
 *   SendEngineCommMessage 
 *
 * DESCRIPTION
 *  Ams process sending a message queue to AmsEngineComm process. The
 *  will be foward to Engine board
 *
 * PARAMETERS
 *   EngineCommReceiveMsg_t * msg   --- Message pointer
 *   msgDataLen                     --- Message length
 *
 * RETURNS
 *  none
 */

void
SendEngineCommMessage (EngineCommReceiveMsg_t * msg, int msgDataLen)
{
    if (AmsEngineCommQueue != 0) {
        WriteMsgQueue (AmsEngineCommQueue, (char*)msg, msgDataLen + sizeof(int), INFINITE, NULL);
    }
}



/*
 * FUNCTION
 *   SendDrawerMessage 
 *
 * DESCRIPTION
 *  Send Analyzer drawer open/close command to AmsEngineComm
 *
 * PARAMETERS
 *  char command    --- drawer operation command 
 *
 * RETURNS
 *  none
 */

void
SendDrawerMessage(char command)
{
    EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "Sending 'D' '%c' message\n", command );
#endif
    /*  Create the message */
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'D';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.length = 2;
   
    /* Send the message */
    SendEngineCommMessage(&msg, sizeof( EngineCommMessageData_t ));

}


/*
 * FUNCTION
 *  SendHeaterCommand
 *
 * DESCRIPTION
 *  Sending Heater command to engine board
 *  
 * PARAMETERS
 *  command     --- Engine command 
 *
 * RETURNS
 *  none
 */

void
SendHeaterCommand (char command)
{
    EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'H' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'H';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 10;  // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;  // MSB of binary count
    /* add extra value for debug purpose */
    memcpy (&msg.data.engineMsg.message[5], &FactoryData.heaterDacSettings [0], 2);
    memcpy (&msg.data.engineMsg.message[7], &FactoryData.heaterDacSettings [2], 2);
    msg.data.engineMsg.message[9] = FactoryData.fanTemperatureSetting;      // set heater DAC, fanDac is not used
    memcpy (&msg.data.engineMsg.message[10],  &FactoryData.airTemperatureOffset, 4);
    msg.data.engineMsg.length = 15;
    /* Send the message */
    SendEngineCommMessage(&msg, sizeof( EngineCommMessageData_t ));
}

// Set Fan idle speed 
void
SendFanIdleSpeed (char idleSpeed)
{
    EngineCommReceiveMsg_t  msg;

    LogDebug ("Send Fan cmd=%c dac =%d", 'F', IDLE_FAN_HIGH_SPEED);
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0]  = 'H';
    msg.data.engineMsg.message[1]  = 'F';    // cmd = F, set fan idle speed
    msg.data.engineMsg.message[2]  = ESC;
    msg.data.engineMsg.message[3]  = 10;     // LSB of binary count.
    msg.data.engineMsg.message[4]  = 0;      // MSB of binary count.
    msg.data.engineMsg.message[5]  = 0;      // Top DAC, two bytes for 10 bit  DAC
    msg.data.engineMsg.message[6]  = 0;
    msg.data.engineMsg.message[7]  = 0;      // Bootom DAC, two bytes for 10 bit DAC
    msg.data.engineMsg.message[8]  = 0;
    msg.data.engineMsg.message[9]  = idleSpeed;    // Fan idle speed, low = 0, high = 1
    msg.data.engineMsg.message[10] = 0;      // Fan speed set
    msg.data.engineMsg.message[11] = 0;      // air temperature offset 4 bytes
    msg.data.engineMsg.message[12] = 0;
    msg.data.engineMsg.message[13] = 0;
    msg.data.engineMsg.message[14] = 0;
    msg.data.engineMsg.length = 15;
    SendEngineCommMessage(&msg, sizeof( EngineCommMessageData_t ));
}

/*
 * FUNCTION
 *  SendAnalysisCommand 
 *
 * DESCRIPTION
 *  Send analysis command to Engine
 *
 * PARAMETERS
 *  command --- analysis command 
 *
 * RETURNS
 *  none
 */

void
SendAnalysisCommand (char command)
{
  EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'A' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'A';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.length = 2;
    /* Send the message */
    SendEngineCommMessage(&msg, sizeof( EngineCommMessageData_t ));
}


/*
 * FUNCTION
 *  SendCalibrationFactors 
 *
 * DESCRIPTION
 *  Send calibration factors to Engine borad
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  none
 */

void
SendCalibrationFactors (void)
{
    EngineCommReceiveMsg_t  msg;
    unsigned short          byteLength =  10;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'C' message\n");
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'C';
    msg.data.engineMsg.message[1] = ESC;
    msg.data.engineMsg.message[2] = byteLength & 0xFF; 
    msg.data.engineMsg.message[3] = (byteLength >> 8)& 0xFF; 
    memcpy (msg.data.engineMsg.message + 4,
            &CalibrationData.wavelengthDacTrims [0], byteLength);
    msg.data.engineMsg.length = 14;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}

/*
 * FUNCTION
 *  SendFactoryDatas 
 *
 * DESCRIPTION
 *  Send Default factory parameter to Engine board
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  none
 */

void
SendFactoryDatas (void)
{
    EngineCommReceiveMsg_t  msg;
    unsigned short          byteLength =  32;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'F' message\n");
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'F';
    msg.data.engineMsg.message[1] = ESC;
    msg.data.engineMsg.message[2] = byteLength & 0xFF;
    msg.data.engineMsg.message[3] = (byteLength >> 8)& 0xFF;
    memcpy (msg.data.engineMsg.message + 4,
            &FactoryData.globalCuvetteDelay, byteLength);
    msg.data.engineMsg.length = 36;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));

}

/*
 * FUNCTION
 *  SendRisFile 
 *
 * DESCRIPTION
 *  Download Ris file to Engine board
 *
 * PARAMETERS
 *  risFileName  --- ris file name 
 *
 * RETURNS
 *  none
 */

bool
SendRisFile (char *risFileName)
{
    unsigned char           fileBuff[3000];
    unsigned                fsize;
    FILE *                  fp;
    EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    LogDebug ( "EngComm: Sending 'P' %s file\n", risFileName);
#endif

    fp = fopen (risFileName, "rb");
    if (fp == 0) {
        LogError ("EngComm: Fail to open %s file\n", risFileName);
        return false;
    }

    fsize = fread (fileBuff, sizeof(char), 3000, fp);

    if (fsize > 0) {
        msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
        msg.data.engineMsg.message[0] = 'P';
        msg.data.engineMsg.message[1] = ESC;
        msg.data.engineMsg.message[2] = fsize & 0xFF;   // LSB of binary count.
        msg.data.engineMsg.message[3] = fsize >> 8;     // MSB of binary count.
        memcpy(msg.data.engineMsg.message + 4, fileBuff, fsize );
        msg.data.engineMsg.length = fsize + 4;
        SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
    } else {
        LogError ("EngComm: Read  %s file fail\n", risFileName);
        fclose (risFileName);
        return false;
    }
    fclose (risFileName);
    return true;
}


/*
 * FUNCTION
 *  SendBarCodeCommand 
 *
 * DESCRIPTION
 *  sending bardcode calibration or read command to engine board
 *
 * PARAMETERS
 *  command     --- barcode read command
 *
 * RETURNS
 *  none
 */

void
SendBarCodeCommand (char command)
{
      EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'B' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'B';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 4;      /* LSB of binary count */
    msg.data.engineMsg.message[4] = 0;      /* MSB of binary count */
    msg.data.engineMsg.message[5] = CalibrationData.barcodeDacs[0];
    msg.data.engineMsg.message[6] = CalibrationData.barcodeDacs[1];
    msg.data.engineMsg.message[7] = CalibrationData.cuvetteMarkDacs[0];
    msg.data.engineMsg.message[8] = CalibrationData.cuvetteMarkDacs[1];
    msg.data.engineMsg.length = 9;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}

/*
 * FUNCTION
 *  SendBarCodeAlignCommand
 *
 * DESCRIPTION
 *  sending bardcode alignment  calibration or read command to engine board
 *
 * PARAMETERS
 *  command     --- barcode read command
 *
 * RETURNS
 *  none
 */

void
SendBarCodeAlignCommand (char command)
{
      EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'I' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'I';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 4;      /* LSB of binary count */
    msg.data.engineMsg.message[4] = 0;      /* MSB of binary count */
    msg.data.engineMsg.message[5] = CalibrationData.barcodeDacs[0];
    msg.data.engineMsg.message[6] = CalibrationData.barcodeDacs[1];
    msg.data.engineMsg.message[7] = CalibrationData.cuvetteMarkDacs[0];
    msg.data.engineMsg.message[8] = CalibrationData.cuvetteMarkDacs[1];
    msg.data.engineMsg.length = 9;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}

/*
 * FUNCTION
 *  SendBarCodeGainCommand
 *
 * DESCRIPTION
 *  sending bardcode alignment  calibration or read command to engine board
 *
 * PARAMETERS
 *  command     --- barcode read command
 *
 * RETURNS
 *  none
 */

void
SendBarCodeGainCommand (char command)
{
      EngineCommReceiveMsg_t  msg;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'K' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'K';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.length = 2;
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}

/*
 * FUNCTION
 *  SendCuvetteDetectionCommand
 *
 * DESCRIPTION
 *  sending Cuvette detection command to Engine board
 *
 * PARAMETERS
 *  command     --- auto test by increasing current DAC values
 *
 * RETURNS
 *  none
 */

void
SendCuvetteDetectionCommand (char command)
{
    EngineCommReceiveMsg_t  msg;
    CuvetteTestParameter_t cuvetteTestParameter;

#ifdef AMS_ENG_COMM_DEBUG
    printf( "EngComm: Sending 'Q' '%c' message\n", command );
#endif
    msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
    msg.data.engineMsg.message[0] = 'Q';
    msg.data.engineMsg.message[1] = command;
    msg.data.engineMsg.message[2] = ESC;
    msg.data.engineMsg.message[3] = 14;     // LSB of binary count.
    msg.data.engineMsg.message[4] = 0;      // MSB of binary count.
    msg.data.engineMsg.length = 19;
    /* Cuvette Test Start at threshold DAC = 0x20
     * current DAC = 50
     */
    cuvetteTestParameter.currentDac = 50;
    cuvetteTestParameter.thresholdDac = 0x20;
    memcpy (&msg.data.engineMsg.message[5], &cuvetteTestParameter, sizeof (CuvetteTestParameter_t));
    SendEngineCommMessage(&msg, sizeof(EngineCommReceiveMsg_t));
}

/*
 * turn on fan and set heater dac value to be normal 
 * turn off fan and set heater dac value to be 0 
 */

void
FanControl (bool fanOn)
{
    static heaterDac_t  heaterDacSet;
    static char         dacInit = 0;

    if (fanOn) {
        LogDebug ("Ams turn fan on");
        FactoryData.fanTemperatureSetting = 0;
        /* only dac set is copied */
        if (dacInit == 1) {
            /* recover courrent heater DAC settings */
            FactoryData.heaterDacSettings [TOP_HEATER_DAC_DEFAULT] =
                    heaterDacSet.topHeaterDac;
            FactoryData.heaterDacSettings [BOTTOM_HEATER_DAC_DEFAULT] =
                    heaterDacSet.botHeaterDac;
        }
    } else {
        LogDebug ("Ams turn fan off");
        /* save current DAC settings */
        heaterDacSet.topHeaterDac =  FactoryData.heaterDacSettings [TOP_HEATER_DAC_DEFAULT];
        heaterDacSet.botHeaterDac =  FactoryData.heaterDacSettings [BOTTOM_HEATER_DAC_DEFAULT];
        dacInit = 1;
        /* turn fan and heater off */
        FactoryData.fanTemperatureSetting = 255;
        FactoryData.heaterDacSettings[TOP_HEATER_DAC_DEFAULT] = 0;
        FactoryData.heaterDacSettings [BOTTOM_HEATER_DAC_DEFAULT] = 0;
    }
    SendHeaterCommand ('S');
}
