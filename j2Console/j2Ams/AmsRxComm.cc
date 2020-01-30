/*
 * FILENAME
 *  AmsRxComm.c
 *
 * MODULE DESCRIPTION
 *  Process  AMS messages 
 *
 * AUTHOR
 */
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
#include "State.h"
#include "AmsComm.h"
#include "EngComm.h"
#include "LogErr.h"
#include "FtdiConfig.h"
#include "Eeprom.h"
#include "Global.h"


AmsBuf_t amsBuf;
char sysName [11];
char rxChar;
CRITICAL_SECTION    csReceivedChar;
HANDLE              hReceivedChar;
static              void SendAmsRxQueue (char *msg, DWORD txSize);
LogFileParameters_t logFileParameters;
static HANDLE       LogSemHandle                = NULL;    //log file semaphore

#define             UPLOAD_LOG_FILE_MAX_SIZE    1000000  // max upload log file size is 1MB
#define             LOG_RENAME_FIX

/* serial port message status string */ 
char  *MsgStatusStr [] = 
        {
            "no error\n",
            "command error\n",
            "format error\n",  
            "length error\n",
            "crc error\n",
            "tx error\n",
            "ams rx timeout\n",
            "ams rx buffer overflow\n",
            "ams serial port open fail\n",
            "ams command does not support\n",
            "fail to open file\n",
            "fail to open factory file\n",
            "fail to open calibration file\n",
            "fail to open factory xml file\n",
            "fail to create parser\n",
            "xml syntax error\n",
            "xml crc error\n",
            "xml read error\n",
        };

char *opticalFilterStr [] = 
                            {
                                "Set W850 Filter Parameter",
                                "Set W630 Filter Paramter",
                                "Set W600 Filter Paramter",
                                "Set W550 Filter Paramter",
                                "Set W515 Filter Paramter",
                                "Set W500 Filter Paramter",
                                "Set W467 Filter Paramter",
                                "Set W405 Filter Paramter",
                                "Set W340 Filter Paramter",
                            };

/*
 * this table is created for simulation purpose only
 */
unsigned short heaterSettingTable [] = 
		{
             0, 300, 35, 30, 300
        };




/* 
 * create special even handle for windows CE
 */
void
InitialEvent (void)
{
    //Create events
    hReceivedChar =  CreateEvent (NULL, FALSE, FALSE, L"ReceivedChar");
    //Initialize Critical Sections
    InitializeCriticalSection (&csReceivedChar);
    
}


/*
 * FUNCTION
 *  PrintMsgStatus 
 *
 * DESCRIPTION
 *  print serail port message process status on the console f
 *  for debug only
 *
 * PARAMETERS
 *  msgStatus  message status 
 * 
 * RETURNS
 *   void 
 */

void
PrintMsgStatus (AmsError_t msgStatus)
{
#ifdef AMS_DEBUG
    fprintf (stderr, "%s", MsgStatusStr[msgStatus]);
#endif
}

/*
 * FUNCTION
 *  CheckMsg 
 *
 * DESCRIPTION
 *  Check received package 
 *
 * PARAMETERS
 *  *msgPtr  --- received package message   
 *
 * RETURNS
 *   Package message code
 */

AmsError_t
MsgCheck (FT_HANDLE fdAms, char *msgPtr)
{
    char command;
    
    AmsError_t    msgStatus = NO_ERR;

    /* check STX */
    if (msgPtr[STX_OFFSET] != STX) {
        msgStatus = FORMAT_ERR;
    }

    /* get command */
    command   = msgPtr[CMD_OFFSET];


    /*
     * check CRC , 
     * no CRC and response  for ACK and NAK 
     */
    if (command != ACK && command != NAK)  {
        msgStatus = MsgCrcCheck (msgPtr);
        if (msgStatus != NO_ERR) {
            AmsTransmitMsg (fdAms, NAK , NULL, 0);
        } else {
           AmsTransmitMsg (fdAms, ACK , NULL, 0);
        }
    }
    /* for debug only */
    if (msgStatus != NO_ERR) {
        PrintMsgStatus (msgStatus);
    }

    return msgStatus;
}


/*
 * FUNCTION
 * ProcessChangeCal
 *
 * DESCRIPTION
 *  process CHANGE_CAL command 
 * 
 * PARAMETERS
 *  msgPtr  AMS message pointer
 *
 * RETURNS
 */

void
ProcessChangeCal (char *msgPtr)
{
    unsigned short type, length, index, value, i;
    char *picStr  = "PICCOLO";
    char *vetStr  = "VETSCAN";
    
    type   =   AsciiToWord (&msgPtr[MSG_HEADER], FALSE);
    length =   AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    index  =   AsciiToWord (&msgPtr[INDEX_OFFSET], FALSE); 
    
    value  =   AsciiToWord (&msgPtr[INDEX_OFFSET + 4], TRUE);
    
#ifdef AMS_CHANGE_CAL_DEBUG     
    printf ("type = %d index = %d data value =%u\n", type, index, value);
#endif
    
    switch (type) 
    {

        case GLOBAL_CUV_DELAY:
            FactoryData.globalCuvetteDelay =
                             AsciiToWord (&msgPtr[INDEX_OFFSET + 4],TRUE);
            amsState.amsRxState = AMS_RX_GLOBAL_CUV_DELAY;
            LogDebug ("Save global delay");
#ifdef AMS_CHANGE_CAL_DEBUG
            printf ("AmsRx: GlobalDelay = %d\n",FactoryData.globalCuvette
#endif

        break;

        case W850_FILTER_CALIBRATION:
        case W630_FILTER_CALIBRATION:
        case W600_FILTER_CALIBRATION:
        case W550_FILTER_CALIBRATION:
        case W515_FILTER_CALIBRATION:
        case W500_FILTER_CALIBRATION:
        case W467_FILTER_CALIBRATION:
        case W405_FILTER_CALIBRATION:
        case W340_FILTER_CALIBRATION:
            if (amsState.opticalDownload == 0) {
                SendGuiMessage (MSG_AMS_STATUS, DOWNLOAD_OPTICAL_PARAMETER, 0);
                amsState.opticalDownload = 1;
            }

            index >>= 1;
            FactoryData.wavelengthFilterCurves[WAVE_FILTER_OFFSET - type][index] = 
                    AsciiToWord (&msgPtr[INDEX_OFFSET + 4], TRUE);
            amsState.amsRxState = AMS_RX_WAVE_LENGTH;
        break;

        case INSTRUMENT_SERIAL_NUMBER:
            SendGuiMessage (MSG_AMS_STATUS, INIT_SERIAL_NUM, 0);
            for (i = 0; i < 10; i++) {
                FactoryData.instrumentSerialNumber[i] =
                    AsciiToByte (&msgPtr[PAYLOAD_OFFSET + (i << 1)], HEX_TYPE);
            }
            /* set RCT NvRam and factory procudtCode */
            switch (FactoryData.instrumentSerialNumber [4]) {
                case 'p':
                case 'P':
                    /* piccloc */
                    WriteNvRamStr (0, picStr, 7); 
                    FactoryData.abaxisProductCode = '2';
                break;

                case 'v':
                case 'V':
                    /* vetscan */
                    WriteNvRamStr (0,vetStr, 7); 
                    FactoryData.abaxisProductCode = '1';
                break;

                default:
                    LogInfo ("Invalid Serial number");
                break;
            }
            amsState.amsRxState = AMS_RX_SERIAL_NUM;
        break;

        case AIR_TEMPERATURE_OFFSET:
            FactoryData.airTemperatureOffset = AsciiToFloat (&msgPtr[INDEX_OFFSET + 4], TRUE);
            amsState.amsRxState = AMS_RX_AIR_TEMP_OFFSET;
     //       Sleep (200);
        break;
        
        case HEATER_DAC_SETTING:
            FactoryData.heaterDacSettings[index] = AsciiToByte (&msgPtr[INDEX_OFFSET + 4], HEX_TYPE); 
#ifdef AMS_CHANGE_CAL_DEBUG
            printf ("set index = %d dac = %d\n", index, FactoryData.heaterDacSettings[index]);
#endif
            amsState.amsRxState = AMS_RX_HEATER_DEC_SETTING;
      //      Sleep (200);
        break;

        case CUVETTE_DELAY:
#ifdef AMS_CHANGE_CAL_DEBUG
            /* needs  to see what is going on */
            printf ("Set Cuvette Delay Setting\n"); 
#endif
        break;
        
        default:
#ifdef AMS_CHANGE_CAL_DEBUG
            printf ("not support calibration factor change type !!!");
#endif
        break;

    }
}


/*
 * FUNCTION
 *  ProcessSetTimer 
 *
 * DESCRIPTION
 *  process AMS set timer commands, set timers (future defined)
 *
 * PARAMETERS
 *  msgPtr  AMS message pointer, 
 *
 * RETURNS
 *  void 
 */

void 
ProcessSetTimer (char *msgPtr)
{
    unsigned char i = 0;
    Timer_t timer;
    

    timer.month  = AsciiToByte (&msgPtr[MSG_HEADER + i], DEC_TYPE);

    i += 2;
    timer.day    = AsciiToByte (&msgPtr[MSG_HEADER + i], DEC_TYPE);
    
    i += 2;
    timer.year   = AsciiToByte (&msgPtr[MSG_HEADER + i], DEC_TYPE);

    i +=2;
    timer.minute = AsciiToByte (&msgPtr[MSG_HEADER + i], DEC_TYPE);

    i += 2;
    timer.second = AsciiToByte (&msgPtr[MSG_HEADER + i], DEC_TYPE);

#ifdef AMS_TIMER_DEBUG
    printf ("month=%d day=%d year=%d minute=%d second=%d\n",
            timer.month, timer.day, timer.year, timer.minute,
            timer.second);
#endif
}

/*
 * FUNCTION
 *  ProcessWriteRam 
 *
 * DESCRIPTION
 *  process AMS write nvram commands, 
 *
 * PARAMETERS
 *  msgPtr  AMS message pointer,
 *
 * RETURNS
 *   process status
 * 
 * Note: this command may call other function 
 *       in future to reset the counters
 */

AmsError_t
ProcessWriteRam (char *msgPtr)
{
    unsigned short length, varIdx;
    unsigned char  varId,  fieldIdx;

    length    =  AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    varId     =  AsciiToByte (&msgPtr[LEN_OFFSET + 4], HEX_TYPE);
    varIdx    =  AsciiToWord (&msgPtr[LEN_OFFSET + 6], FALSE);
    fieldIdx  =  AsciiToByte (&msgPtr[LEN_OFFSET + 10], HEX_TYPE);

#ifdef AMS_WRITE_RAM_DEBUG
    printf ("length = %d varId = %d varIdx =%d fieldIdx = %d\n",
             length, varId, varIdx, fieldIdx);
#endif

    /* 
     * for lagcey system, initial and update nvram  
     * just copy rom data such reference range to
     * nvram
     * for new Iris system, this command should 
     * be ignored ???
     */
    if (varId == INIT_NON_VOL_MEM) {
#ifdef AMS_WRITE_RAM_DEBUG
        printf ("rx AMS INIT_NON_VOL_MEM\n");
#endif
    } else if (varId ==  UPDATE_NON_VOL_MEM) {
#ifdef AMS_WRITE_RAM_DEBUG
        printf ("rx AMS UDATE_NON_VOL_MEM\n");
#endif
    } else {
#ifdef AMS_WRITE_RAM_DEBUG
        printf ("reset the counter index %d\n", varId);
#endif
    }  

    return NO_ERR;
}

/*
 * function to process ethernet port test
 */
AmsError_t
ProcessEthernetPortTest (FT_HANDLE fdAms)
{
    char msg [256];
    netPortTestMsg_t netPortTestMsg;
    char            *ptr;
    unsigned char   i;


    // ping itself
    if (Ping (PING_TIMES, &netPortTestMsg) != 0) {
        SendErrorCommnd (fdAms, AMS_ETHERNET_TEST_ERROR);
    }
    memset (msg, 0, sizeof (msg));
    ptr = &msg [0];
    // convert IP address to ascii
    ptr = LongToAscii (netPortTestMsg.ipAddress, ptr, 0); 
    // convert test result, time and status to ascii 
    for (i = 0; i < PING_TIMES;  i++) {
        ptr = ByteToAscii (netPortTestMsg.pingReport [i].pingPass,   ptr);
        ptr = WordToAscii (netPortTestMsg.pingReport [i].pingTime,   ptr, 0);
        ptr = WordToAscii (netPortTestMsg.pingReport [i].pingStatus, ptr, 0);
    }


    Sleep (5000);

#if 0
    LogDebug ("size =%d", strlen (msg));
    LogDebug ("msg =%s", msg);
#endif
    AmsTransmitMsg (fdAms, NETPORT_TEST_RESPONSE , msg, 58);

    return NO_ERR;
}

/*
 * FUNCTION
 *  ProcessAnalysis 
 *
 * DESCRIPTION
 *  process AMS Analysis commands
 *
 * PARAMETERS
 *  fdAms   Serial port fd
 *  msgPtr  AMS message pointer
 *
 * RETURNS
 *   AmsError_t message process status
 *
 * Note: wliu
 *        1. It needs to be update in the future to
 *           send command to Engine by using Ris file
 *        2. open data file is for simulation only. For
 *           real system, it needs to pick up data from
 *           raw data memory  
 */

int
ProcessAnalysis (FT_HANDLE fdAms, char *msgPtr)
{
    unsigned short length,speed, flags;
    unsigned char  type;
    int            ret = 0;

    length    =  AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    type      =  AsciiToByte (&msgPtr[LEN_OFFSET + 4], HEX_TYPE);
    speed     =  AsciiToWord (&msgPtr[LEN_OFFSET + 6], FALSE);
    flags     =  AsciiToWord (&msgPtr[LEN_OFFSET + 10], FALSE);

    amsState.amsCommand.command = msgPtr[CMD_OFFSET];
    memcpy (&amsState.amsCommand.type , &msgPtr[LEN_OFFSET + 4], 2); 

    memcpy (sysName, &msgPtr[LEN_OFFSET + 14], 10);

    sysName [10] = '\0';

#ifdef AMS_ANA_DEBUG
    printf ("length = %d type = 0x%x speed = 0x%x flag = 0x%x name = %s\n",
             length, type, speed, flags, sysName);
#endif

    switch (type) {

        case MOTOR_TEST:
            /* only dawer closed and rotoro present, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_MOTOR_TEST,0);
                /*  download motor test ris file */
                SendRisFile ("/Hard Disk/Abaxis/Ris/motor.ris");
                /* ZZZ turn fan off */
                FanControl (false); 
                SendAnalysisCommand ('M');
                amsState.waitingDrawerClose = 0;
                LogInfo ("Ams Motor Test"); 
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;

       case ROTOR_DELAY:
            /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                /*  download global delay ris file */
                SendRisFile ("/Hard Disk/Abaxis/Ris/delay.ris");
                SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_GLOBAL_DELAY,0);
                SendAnalysisCommand ('M');
                amsState.waitingDrawerClose = 0;
                LogInfo ("Ams Glogal Delay Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }
            
       break;

        case ENG_ANALYSIS:
            /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                /*  download next  ris file */
                 SendRisFile ("/Hard Disk/Abaxis/Ris/ndxt.ris");
                SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_NDXT,0);
                SendAnalysisCommand ('M');
                amsState.waitingDrawerClose = 0;
                LogInfo ("Ams NDXT Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }

        break;

        case BARCODE:
            /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_BARCODE_TEST, 0);
                SendBarCodeCommand ('C');
                amsState.waitingDrawerClose = 0;
                amsState.barcodeTest = 1;
                LogInfo ("Ams Barcode Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;

        case BARCODE_ALIGN:
              /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                SendGuiMessage (MSG_AMS_STATUS, BARCODE_ALIGN_TEST, 0);
                SendBarCodeAlignCommand ('C');
                amsState.waitingDrawerClose = 0;
                LogInfo ("Ams Barcode Alignment Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;

        case BARCODE_GAIN:
              /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                SendGuiMessage (MSG_AMS_STATUS, BARCODE_GAIN_TEST, 0);
                SendBarCodeGainCommand (0);
                LogInfo ("Ams Barcode Gain Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;

        case DOWNLOAD:
            /* only dawer closed and rotoro prosent, begin to run ris file */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                /*  download temperature ris file */
                SendRisFile ("/Hard Disk/Abaxis/Ris/temp.ris");
                SendGuiMessage (MSG_AMS_STATUS, ANALYSIS_TEMP, 0);
                SendAnalysisCommand ('M');
                amsState.waitingDrawerClose = 0;
                LogInfo ("Ams Temperature Test");
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;

        case CUVETTE_TEST:
            LogDebug ("AMS Cuvette Detect Start");
            /* send command to engine board for this special test */
            if (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE) {
                SendGuiMessage (MSG_AMS_STATUS, CUVETTE_DETECTION_TEST, 0);
                SendCuvetteDetectionCommand ('K');
            } else {
                amsState.waitingDrawerClose = 1;
            }
        break;


        case NETPORT_TEST:
            SendGuiMessage (MSG_AMS_STATUS, ETHERNET_PORT_TEST, 0); 
            /* delay one seconds to response OK to PC */
            Sleep (1000);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
            ProcessEthernetPortTest (fdAms);
            //Sleep (5000);
            SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0); 
            /* no extra OK command send out */
            ret = 1;
        break;

        default:
           printf ("amalysis does not support type 0x%x\n", type);
        break;
    }

    return ret;
}

/*
 * FUNCTION
 *  ProcessTranserMixProfile 
 *
 * DESCRIPTION
 *  process the profile download command. For real system, this command
 *  just be ignored since all the profile will be saved in RIS file.
 *
 * PARAMETERS
 *  fdAms   Serial port fd
 *  msgPtr  AMS message pointer
 *
 * RETURNS
 *   AmsError_t message process status
 *
 * Note: wliu
 *       There is no need to save those
 *       profile in the memory. In the
 *       real system, those profile should be defined
 *       in the RIS file
 */

AmsError_t
ProcessTranserMixProfile (char *msgPtr)
{
    unsigned short length,rpm,time, dir, gain, acce, rept;
    MixProfile_t          mixProfile;

    length    =  AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    rpm       =  AsciiToWord (&msgPtr[LEN_OFFSET + 4],  FALSE);
    time      =  AsciiToWord (&msgPtr[LEN_OFFSET + 8],  FALSE);
    dir       =  AsciiToWord (&msgPtr[LEN_OFFSET + 12], FALSE);
    gain      =  AsciiToWord (&msgPtr[LEN_OFFSET + 16], FALSE);
    acce      =  AsciiToWord (&msgPtr[LEN_OFFSET + 20], FALSE);
    rept      =  AsciiToWord (&msgPtr[LEN_OFFSET + 24], FALSE);


    amsState.amsRxState = AMS_RX_MIX_PROFILE;


    /* get mixprofile entry */
    mixProfile.rpm  = rpm;
    mixProfile.time = time;
    mixProfile.dir  = dir;
    mixProfile.gain = gain;
    mixProfile.acce = acce;
    mixProfile.rept = rept;
    mixProfile.flag = 1;

#ifdef AMS_TRAN_PROFILE_DEBUG
    printf ("AMS mix profile length =%d rpm =%d time =%d dir =%d gain =%d acc =%d rept =%d \n",
              length, rpm, time, dir, gain, acce, rept);
#endif
    
    return NO_ERR;

}


/*
 * FUNCTION
 *  ProcessTranserSampleProfile
 *
 * DESCRIPTION
 *  process the profile download command. For real system, this command
 *  just be ignored since all the profile will be saved in RIS file.
 *
 * PARAMETERS
 *  msgPtr  AMS message pointer
 *
 * RETURNS
 *   AmsError_t message process status
 *
 * Note: wliu
 *       1. There is no need to save those
 *          profile in the memory. In the
 *          real system, those profile should be defined
 *          in the RIS file
 *       2. some of prarameters may need to remember for 
 *          raw data dump (which wavelength to dump)   
 */

AmsError_t
ProcessTransferSampleProfile (char *msgPtr)
{
    SampleProfile_t SampleProfile;
    unsigned char cuvNumber, numSample, wavLen0, wavLen1, wavLen2;
    unsigned char loopCount, sampleType, optType;
    unsigned short length;

    amsState.amsRxState = AMS_RX_SAMPLE_PROFILE;

    length     =  AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    cuvNumber  =  AsciiToByte (&msgPtr[LEN_OFFSET  + 4], HEX_TYPE);
    numSample  =  AsciiToByte (&msgPtr[LEN_OFFSET  + 6], HEX_TYPE);
    wavLen0    =  AsciiToByte (&msgPtr[LEN_OFFSET  + 8], HEX_TYPE);
    wavLen1    =  AsciiToByte (&msgPtr[LEN_OFFSET  + 10], HEX_TYPE);
    wavLen2    =  AsciiToByte (&msgPtr[LEN_OFFSET  + 12], HEX_TYPE);
    loopCount  =  AsciiToByte (&msgPtr[LEN_OFFSET  + 14], HEX_TYPE);
    sampleType =  AsciiToByte (&msgPtr[LEN_OFFSET + 16], HEX_TYPE);
    optType    =  AsciiToByte (&msgPtr[LEN_OFFSET  + 18], HEX_TYPE);
    
    SampleProfile.cuvNumber  = cuvNumber;
    SampleProfile.numSample  = numSample;
    SampleProfile.wavLen0    = wavLen0;
    SampleProfile.wavLen1    = wavLen1;
    SampleProfile.wavLen2    = wavLen2;
    SampleProfile.loopCount  = loopCount;
    SampleProfile.sampleType = sampleType;
    SampleProfile.optType    = optType;

#ifdef AMS_TRAN_PROFILE_DEBUG
    printf ("AMS sample profile length =%d cuvNumber =%d numSample =%d wavLen0 =%d wavLen1 =%d wavLen2 =%d loopCount =%d sampleType =%d optType =%d\n",
              length, cuvNumber, numSample, wavLen0, wavLen1, wavLen2, loopCount, sampleType, optType);
#endif
    return NO_ERR;
}


/*
 * FUNCTION
 *  ProcessTransmitComplete
 *
 * DESCRIPTION
 *  handle the complete command from AMS
 *
 * PARAMETERS
 *  void
 *
 * RETURNS
 *  void
 *
 */

void
ProcessTransmitComplete (void)
{

    switch (amsState.amsRxState) {

        case AMS_RX_WAVE_LENGTH:
            /* calculate BFD */
            CalcBfds ();
            WriteData ("Hard Disk/Abaxis/cfg/factory.xml", (char *)&FactoryData);
            amsState.amsRxState = AMS_RX_IDEL;
            amsState.opticalDownload = 0;
            SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
        break;

        case AMS_RX_GLOBAL_CUV_DELAY:
            /* should be enable in new AMS */ 
            WriteData ("Hard Disk/Abaxis/cfg/factory.xml", (char *)&FactoryData);
            amsState.amsRxState = AMS_RX_IDEL;
            /* send new global delay to Engine board */
            SendFactoryDatas ();
        break;

        case AMS_RX_SERIAL_NUM:
            LogDebug ("download Serial number");
            WriteData ("Hard Disk/Abaxis/cfg/factory.xml", (char *)&FactoryData);
            amsState.amsRxState = AMS_RX_IDEL;
            SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
        break; 

        case AMS_RX_AIR_TEMP_OFFSET:
            WriteData ("Hard Disk/Abaxis/cfg/factory.xml", (char *)&FactoryData);
            amsState.amsRxState = AMS_RX_IDEL;
#ifdef AMS_CHANGE_CAL_DEBUG
            printf ("Set air offset %f\n", FactoryData.airTemperatureOffset); 
#endif
        break;

        case AMS_RX_HEATER_DEC_SETTING:
            WriteData ("Hard Disk/Abaxis/cfg/factory.xml", (char *)&FactoryData);
            amsState.amsRxState = AMS_RX_IDEL;
            SendHeaterCommand ('S');
        break;
    
        case AMS_RX_MIX_PROFILE:
        case AMS_RX_SAMPLE_PROFILE:
            amsState.amsRxState = AMS_RX_IDEL;
        break;

        case AMS_RX_UPLOAD_CALIBRATION:
            amsState.amsRxState = AMS_RX_IDEL;
            SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
        break;

        default:
        break;
    }
        
}



/*
 * Get current date/time and send back to AMS 
 */
void
ProcessDateTime (FT_HANDLE fdAms)
{
    char            msg [100], *ptr;
    

    SYSTEMTIME  currentDate;

    GetLocalTime (&currentDate);

    memset (msg, 0, sizeof (msg));
 
    ptr = &msg [0];
    ptr = WordToAscii ((unsigned short)(currentDate.wYear), ptr, FALSE);
    ptr = ByteToAscii ((unsigned char)(currentDate.wMonth),  ptr);
    ptr = ByteToAscii ((unsigned char)currentDate.wDay, ptr);
    ptr = ByteToAscii ((unsigned char)currentDate.wHour,ptr);
    ptr = ByteToAscii ((unsigned char)currentDate.wMinute,ptr);

    AmsTransmitMsg (fdAms, SEND_DATE_TIME, msg, 12);

#if 0
   // for debug purpose
    currentDate.wYear    = AsciiToWord (&msg [0], 0);
    currentDate.wMonth   = AsciiToByte (&msg [4], HEX_TYPE);
    currentDate.wDay	 = AsciiToByte (&msg [6], HEX_TYPE);
    currentDate.wHour    = AsciiToByte (&msg [8], HEX_TYPE);
    currentDate.wMinute  = AsciiToByte (&msg [10], HEX_TYPE);
    LogDebug ("Send Ams Time: year =%d mon =%d day=%d hour =%d min =%d",
            currentDate.wYear, currentDate.wMonth , currentDate.wDay,
            currentDate.wHour, currentDate.wMinute);
#endif
}
    

void
ProcessUpLoadRadFile (FT_HANDLE fdAms, char *msgPtr)
{
    timeFlag_t  timeFlag;


    timeFlag.year   =   AsciiToWord (&msgPtr[MSG_HEADER], FALSE);
    timeFlag.month  =   AsciiToByte (&msgPtr[MSG_HEADER + 4], HEX_TYPE);
    timeFlag.day    =   AsciiToByte (&msgPtr[MSG_HEADER + 6], HEX_TYPE);
    timeFlag.hour   =   AsciiToByte (&msgPtr[MSG_HEADER + 8], HEX_TYPE);
    timeFlag.minute =   AsciiToByte (&msgPtr[MSG_HEADER + 10], HEX_TYPE);

    //SendMsgToAmsTxThread  (AMS_UPLOAD_RAD_FILE, (void *)&timeFlag, 12);
    SendMsgToAmsTxThread  (AMS_UPLOAD_RAD_FILE, (void *)&timeFlag, sizeof (timeFlag_t));
}

// log time for  debug only
void
GetLocalTimeDebug (void)
{
    SYSTEMTIME      sysTime;

    GetLocalTime (&sysTime);
 
    LogDebug ("local time: Year=%d month=%d Day=%d Hour=%d Min=%d Sec=%d",
               sysTime.wYear, sysTime.wMonth, sysTime.wDay,
                                              sysTime.wHour,
                                              sysTime.wMinute,
                                              sysTime.wSecond);
}

// Sync PC timer
void
ProcessSetDateTime (FT_HANDLE fdAms, char *msgPtr)
{
    unsigned int    length;
    SYSTEMTIME      tm;

//    GetLocalTimeDebug ();

    length = AsciiToWord (&msgPtr[LEN_OFFSET], FALSE);

    tm.wYear    = AsciiToWord (&msgPtr[MSG_HEADER], FALSE);
    tm.wMonth   = (unsigned short)AsciiToByte (&msgPtr[MSG_HEADER +4], HEX_TYPE);
    tm.wDay     = (unsigned short)AsciiToByte (&msgPtr[MSG_HEADER +6], HEX_TYPE);
    tm.wHour    = (unsigned short)AsciiToByte (&msgPtr[MSG_HEADER +8], HEX_TYPE);
    tm.wMinute  = (unsigned short)AsciiToByte (&msgPtr[MSG_HEADER +10], HEX_TYPE);
    tm.wSecond  = (unsigned short)AsciiToByte (&msgPtr[MSG_HEADER +12], HEX_TYPE);

    LogDebug ("Ams SetTimer: Year=%d month=%d Day=%d Hour=%d Min=%d Sec=%d",
               tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);

    SetLocalTime (&tm);

//   GetLocalTimeDebug ();
}


// get log file length, if length larger than UPLOAD_LOG_FILE_MAX_SIZE, 
// set file length to be UPLOAD_LOG_FILE_MAX_SIZE
int
GetFileLength (char *fileName, unsigned long *fLength, unsigned long *fOffset)
{
    unsigned long       totalLen, offset;
    FILE                *fp;
    char                ch;

    offset      = 0;
    totalLen    = 0;

    // now, try to find out valid package size and offset
    fp = fopen (fileName,  "rb");
    if (fp != NULL) {
        // goto end of file
        fseek(fp, 0, SEEK_END);
        // get total to be send
        totalLen = ftell(fp);
        // reset to fle begining
        fseek(fp, 0, SEEK_SET);
        //LogDebug ("totalLen = %u", totalLen);
        // max upload log file is 1M bytes
        if (totalLen >= UPLOAD_LOG_FILE_MAX_SIZE) {
            offset = totalLen - UPLOAD_LOG_FILE_MAX_SIZE;
            //LogDebug ("offset = %d\n", offset);
            totalLen = UPLOAD_LOG_FILE_MAX_SIZE;
            fseek (fp, offset, SEEK_SET);
            //try to sync to begin of log line
            ch = 0;
            while (ch != '\n') {
                fread (&ch, 1, 1, fp);
                totalLen--;
                offset++;
            }
        }
//      LogDebug ("filename %s, totalLen = %d, offset =%d\n", fileName, totalLen, offset);
        *fLength    = totalLen;
        *fOffset    = offset; 
        fclose (fp);
    } else {
        LogError ("fail to open %s", fileName);
        return -1;
    }
}

/*
 * FUNCTION
 *  ChangeLogCurrentFileToLogTmpFile 
 *
 * DESCRIPTION
 *  Change log.curent file name to log.tmp  name
 *  before upload log file and rename log file 
 *
 * PARAMETERS
 *
 * RETURNS
 */
void
ChangeLogCurrentFileToLogTmpFile (void)
{
    wchar_t             cFilename [255],  tFilename [255];
    FILE                *fp;

#ifdef LOG_RENAME_FIX
      // create log semaphore
    if (LogSemHandle == NULL) {
        //Create Log file semaphore with initial value 0
        //the semaphore should be released by LogError process
        LogSemHandle = CreateSemaphore (NULL, 0, 1, LogSem);
        if (LogSemHandle == NULL) {
            LogError ("Fail to crate AMS log semaphore");
            return;
        }
    }
#endif

    // rename log files after require date and time by PC
    wsprintf(cFilename, L"%S", LOG_FILE_CURRENT);
    wsprintf(tFilename, L"%S", LOG_FILE_TEMP);
    // waiting for semaphore
    LogDebug ("Waiting for log semaphore");
    WaitForSingleObject (LogSemHandle, INFINITE);
    // delete tmp file first
    DeleteFile (tFilename);
    //rename current log file to tmp log file
    MoveFile (cFilename, tFilename);
   // create a new log.current file
   fp = fopen (LOG_FILE_CURRENT, "ab");
   fclose (fp);
   // release semaphore
   ReleaseSemaphore (LogSemHandle, 1, NULL);
   LogDebug ("Release log semaphore");

}



// get upload log file length and offset parameters
int
GetLogFileParameters (void)
{
    unsigned long       totalLen, offset;
    FILE                *fp;
    char                ch;
    char                *fileName;


    // clearn file parameters
    memset (&logFileParameters, 0, sizeof (LogFileParameters_t));

#ifndef LOG_RENAME_FIX
     // create log semaphore
    if (LogSemHandle == NULL) {
        //Create Log file semaphore with initial value 0
        //the semaphore should be released by LogError process
        LogSemHandle = CreateSemaphore (NULL, 0, 1, LogSem);
        if (LogSemHandle == NULL) {
            LogError ("Fail to crate AMS log semaphore");
            return -1;
        }
    }
#endif

    // before upload log file, the log,current file name changed 
    // to log.tmp
    ChangeLogCurrentFileToLogTmpFile ();

    // get tmp file size and offset 
    fileName = LOG_FILE_TEMP;
    if (GetFileLength (fileName, &totalLen, &offset) == -1) {
        return -1;
    }
    logFileParameters.totalLen  = totalLen;
    logFileParameters.cTotalLen = totalLen;
    logFileParameters.cOffset   = offset;

    // tmp (currnet) file size is 1MB, no need to upload old log file
    if (totalLen >= UPLOAD_LOG_FILE_MAX_SIZE - 128) {
        logFileParameters.oTotalLen = 0;
        logFileParameters.oOffset  = 0;

    } else {
        // get old log file length
        fileName = LOG_FILE_OLD;
        // get old log file size and offset
        if (GetFileLength (fileName, &totalLen, &offset) == -1) {
            return -1;
        }
        logFileParameters.totalLen += totalLen;
        logFileParameters.oTotalLen = totalLen;
        logFileParameters.oOffset  = offset;
    }

    return 0;
}

/*
 * process upload log file command
 */
AmsError_t
ProcessUploadLogFile (FT_HANDLE fdAms, char *msgPtr)
{

    char   type,    msg[100] ;
    unsigned long   pktLength;
    FILE *          fp;
    wchar_t         wFileName [128];


    type   =   AsciiToByte (&msgPtr[MSG_HEADER], HEX_TYPE);

    ByteToAscii (type, &msg[0]);

    switch (type) {

        case 0:
            // get current and old log file length and offset
            GetLogFileParameters ();
             LongToAscii (logFileParameters.totalLen, &msg[2], 1);
            /* wait for ACK for allowing to tx log file */
            amsState.amsRxState = AMS_RX_UPLOAD_CURRENT_LOG; 
            /* send 'l' back to PC AMS station for totally tx length  */
            AmsTransmitMsg (fdAms, START_TX_LOG_FILE, msg, 10);
        break;
        
        case 1:
            // this command no longer supported
        break;

        case 2:
            // new procedure, rename log file instead of delete log files
        break;

        default:
        break;

    }

    return NO_ERR;
}

/*
 * FUNCTION
 *  ProcessRunProfile
 *
 * DESCRIPTION
 *   AMS will send command 'M' to run a mix profile 
 *   for hardware mark signal adjustment.  
 *
 * PARAMETERS
 *   msgPtr    pointer to rx message  
 *
 * RETURNS
 *   process status
 *
 */

AmsError_t
ProcessRunProfile (char *msgPtr)
{

    amsState.amsRxState = AMS_RUN_MIX_PROFILE;
#ifdef AMS_RUN_PROFILE_DEBUG 
    printf ("Turn on mix profile running\n");
#endif
    /* 
     *  need sending RIS file to engine to run mix profile in the future 
     *  wliu
     */ 
    return NO_ERR;
}

/*
 * FUNCTION
 *  ProcessFail 
 *
 * DESCRIPTION
 *  There are two cases AMS send FAIL, 
 *      1. AMS received a wrong message from console card
 *      2. AMS want to stop engine card to run downloaded mix-profile 
 *
 * PARAMETERS
 *   msgPtr    pointer to rx message
 *
 * RETURNS
 *   process status
 *
 */

AmsError_t
ProcessFail (char *msgPtr)
{
    /*  
     * in the run AMS_RUN_MIX_PROFILE status, 
     * AMS will send FAIL command to stop running 
     * in the future, need send 'A" command to engine
     * for stopping running mix protile
     * wliu
     */
    if (amsState.amsRxState == AMS_RUN_MIX_PROFILE) {
        amsState.amsRxState = AMS_RX_IDEL;
    }
#ifdef AMS_FAIL_DEBUG 
    printf ("received AMS FAIL command\n");
#endif
    
     return NO_ERR;
}

/*
 * FUNCTION
 *  ProcessGetCals 
 *
 * DESCRIPTION
 *  Process AMS get calibration factor command
 *  For new system (Iris), some of command farctors
 *  no longer used, just put a fix data here to make
 *  AMS happy
 *
 * PARAMETERS
 *   fdAms     AMS serial port fd
 *   msgPtr    pointer to rx message
 *
 * RETURNS
 *   process status
 *
 */

AmsError_t
ProcessGetCals (FT_HANDLE fdAms, char *msgPtr)
{
    
    unsigned short type, length, index, value;
    char   msg[100], sending = TRUE, i, *ptr;
    char   *eepromVerPtr = "AE471406";
   
    type   =   AsciiToWord (&msgPtr[MSG_HEADER], FALSE);
    length =   AsciiToWord (&msgPtr[LEN_OFFSET],FALSE);
    index  =   AsciiToWord (&msgPtr[INDEX_OFFSET], FALSE);
   
    value  =   AsciiToWord (&msgPtr[INDEX_OFFSET + 4], TRUE);

    WordToAscii (type, &msg[0],FALSE);
    WordToAscii (index, &msg[4],FALSE);

    switch (type) {

        case GLOBAL_CUV_DELAY:
           // first paramet to get (type = 0), set display 
           SendGuiMessage (MSG_AMS_STATUS, GET_CAL_PARAMETERS, 0);
           amsState.amsRxState = AMS_RX_UPLOAD_CALIBRATION;
           length = 12;
           WordToAscii (FactoryData.globalCuvetteDelay, &msg[8], TRUE);
        break; 

        case AIR_TEMPERATURE_OFFSET:
           length = 16;
           FloatToAscii (FactoryData.airTemperatureOffset, &msg[8], TRUE);
        break;

        case HEATER_DAC_SETTING:
           length = 10;
           ByteToAscii (FactoryData.heaterDacSettings[index], &msg[8]); 
        break; 


        case BFD_CALIBRATION_FACTOR:
            length = 16; 
            index >>= 2;
            FloatToAscii (FactoryData.bfdCalibrationFactors [index],  &msg[8], TRUE);
        break;

        case INSTRUMENT_SERIAL_NUMBER:
            length = 28;
            ptr = FactoryData.instrumentSerialNumber;
            for (i = 0; i < 10; i++) {
                ByteToAscii (*ptr++, &msg[8 + (i << 1)]);
            }
        break;

        case EEPROM_VERSION:
            /* 
             * new system dose not have eeprom, send the number
             * just for cheating the AMS. wliu 
             */
            length = 16;
            memcpy (&msg[8], eepromVerPtr, 8);
        break;
        
        case HEATER_TIME_SETTING:
            /* 
             * Those data should be fixed in new system 
             * Just put the fix number here for AMS happy
             * wliu
             */
            length = 12;
            WordToAscii (heaterSettingTable [index >> 1], &msg[8], TRUE);
        break;

        // Take out fan DAC settings
        case FAN_TEMPERATURE_SETTING:
            /*
             * fan Dac setting, 
             */
            length = 10;
            ByteToAscii (FactoryData.fanTemperatureSetting,  &msg[8]);
        break;

        case W850_FILTER_CALIBRATION:
        case W630_FILTER_CALIBRATION:
        case W600_FILTER_CALIBRATION:
        case W550_FILTER_CALIBRATION:
        case W515_FILTER_CALIBRATION:
        case W500_FILTER_CALIBRATION:
        case W467_FILTER_CALIBRATION:
        case W405_FILTER_CALIBRATION:
        case W340_FILTER_CALIBRATION:
            length = 12;
            index >>= 1;
            WordToAscii (FactoryData.wavelengthFilterCurves [WAVE_FILTER_OFFSET - type][index],  
                        &msg[8], TRUE);
        break;


        case CUVETTE_DELAY:
            /* needs  to see what is going on */
            //printf ("Get Cuvette Delay Setting\n"); 
        break;

        default:
          sending = FALSE;
        break;
    }

    if (sending) {
        /* slow down for catch up the secreen */
        AmsTransmitMsg (fdAms, SENDING_CALS , msg, length);
    }

    return NO_ERR;
}


/*
 * FUNCTION
 *  ProcessAck
 *
 * DESCRIPTION
 *  process AMS ACK command
 *  If AMS sends get tempterature, system needs to response 
 *  4 temperatures one by one after receive ACK from AMS
 *
 * PARAMETERS
 *  fdAms   AMS serial port id
 *
 * RETURNS
            amsState.amsRxState = AMS_RX_IDEL;
 *   AmsError_t message process status
 * 
 */

AmsError_t
ProcessAck (FT_HANDLE fdAms)
{
#ifdef AMS_RX_DEBUG
   LogDebug ("rx ACK\n");
#endif
    switch (amsState.amsRxState) {

        case AMS_RX_TEMPERATURE:
            SendMsgToAmsTxThread (AMS_TX_TEMP, NULL, 0);
        break;

        case AMS_RX_UPLOAD_CURRENT_LOG:
        case AMS_RX_UPLOAD_OLD_LOG:
            SendMsgToAmsTxThread (AMS_UPLOAD_LOG_FILE, NULL, 0);
#ifdef AMS_RX_DEBUG
            printf ("Get allowed upload ACK from PC \n");
#endif
        break;

        case AMS_RX_UPLOAD_WAIT_ACK:
        case AMS_RX_UPLOAD_RAD_WAIT_ACK:
        case AMS_RX_UPLOAD_RAD_FINISH_WAIT_ACK:
#ifdef AMS_RX_DEBUG
            LogDebug ("Get ACK for next package\n"); 
#endif
            EnterCriticalSection(&csReceivedChar);
            rxChar = ACK;
            LeaveCriticalSection(&csReceivedChar);
            SetEvent (hReceivedChar);
            amsState.amsRxState = AMS_RX_IDEL;
        break;

        case AMS_RX_WAIT_UPLOAD_LOG_FINISH:
#ifdef AMS_RX_DEBUG
            LogDebug ("Rx Ack to finish Log Upload");
#endif
            amsState.amsRxState = AMS_RX_IDEL;
        break;

        default:
        break;
    }
    return NO_ERR;
}

/*
 * process NAK command 
 */

AmsError_t
ProcessNak (FT_HANDLE fdAms)
{
#ifdef AMS_RX_DEBUG
    LogDebug ("rx NAK\n");
#endif
    
    switch (amsState.amsRxState)  {
        case AMS_RX_UPLOAD_WAIT_ACK:
        case AMS_RX_UPLOAD_RAD_WAIT_ACK:
        case AMS_RX_UPLOAD_RAD_FINISH_WAIT_ACK:
#ifdef AMS_RX_DEBUG
            LogDebug ("Get NAK for resend package\n");
#endif
            LogError ("Get NAK for resend package");
            EnterCriticalSection(&csReceivedChar);
            rxChar = NAK;
            LeaveCriticalSection(&csReceivedChar);
            SetEvent(hReceivedChar);
            amsState.amsRxState = AMS_RX_IDEL;
        break;

        default:
            amsState.amsRxState = AMS_RX_IDEL;
        break;
    }
    return NO_ERR;
}

/*
 * FUNCTION
 *  ProcessGetTemp
 *
 * DESCRIPTION
 *  process AMS get temperature command 
 *  It needs to response AMS four temperautres (id 0 - 3)
 *
 * PARAMETERS
 *  msgPtr  AMS message pointer
 *  fdAms   AMS serial port id
 *
 * RETURNS
 *   AmsError_t message process status
 *  
 * Note: for real system, it needs
 *       send  command 'T' to engine
 *       to get the temperatures. wliu
 */

AmsError_t
ProcessGetTemp (FT_HANDLE fdAms, char *msgPtr)
{

    /* send request to Engine */

    /* set state machine */
    amsState.amsRxState = AMS_RX_TEMPERATURE;

    /* send message to Engine */
    SendHeaterCommand ('R');

    return NO_ERR;
}




/*
 * FUNCTION
 *  ProcessAmsRxMsg 
 *
 * DESCRIPTION
 *  process AMS commands
 * 
 * PARAMETERS
 *  msgPtr  AMS message pointer
 *  fdAms   AMS serial port id
 *
 * RETURNS
 *   AmsError_t message process status 
 */

AmsError_t
ProcessAmsRxMsg (FT_HANDLE fdAms, char *msgPtr)
{

    char            command;
    

    /* get command */
    command   = msgPtr[CMD_OFFSET];
#ifdef AMS_RX_DEBUG
    LogDebug ("AmsRxComm: AMS command %c\n",command);
#endif


    /* process command */
    switch (command) {

        case START_ANALYSIS:
           if (ProcessAnalysis (fdAms, msgPtr)  == 0) {
                AmsTransmitMsg (fdAms, OK, NULL, 0);
            }
        break;

        case CHANGE_CALS:
            ProcessChangeCal (msgPtr);
            Sleep (200);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
        break;


        case SET_DATE_TIME:
            // wait for 100ms for sync with AMS pc
            ProcessSetDateTime (fdAms, msgPtr);
            Sleep (150);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
        break;


        case  WRITE_DATA_RAM:
            // wait for 100ms for sync with AMS pc
            Sleep (150);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
        break;

        case TRANSFER_MIX_PROFILE:
            ProcessTranserMixProfile (msgPtr);
            Sleep (50);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
    
        break;

        case TRANSFER_SAMPLE_PROFILE:
            ProcessTransferSampleProfile (msgPtr);
            Sleep (50);
            AmsTransmitMsg (fdAms, OK, NULL, 0);
        break;

        case RUN_MIX_PROFILE:
            Sleep (150);
            ProcessRunProfile (msgPtr);
        break;


        case GET_CALS:
            Sleep (100);
            ProcessGetCals (fdAms, msgPtr);
        break;

        case GET_TEMPERATURE:
            Sleep (100);
            ProcessGetTemp (fdAms, msgPtr);
        break;

        case FAIL:
            ProcessFail (msgPtr);
        break;

        case ACK:
            ProcessAck (fdAms);
        break;


        case NAK:
            ProcessNak (fdAms);
        break;

        case TRANSMIT_COMPLETE:
            ProcessTransmitComplete (); 
        break;

        case UPLOAD_LOG_FILE:
            ProcessUploadLogFile (fdAms, msgPtr);
        break;


        case REQUEST_DATE_TIME:
            ProcessDateTime (fdAms);
            ChangeLogCurrentFileToLogTmpFile ();
            LogRenameFiles ();
        break;

        case UPLOAD_RAD_FILE:
            //AmsTransmitMsg (fdAms, ACK, NULL, 0);
            Sleep (150);
            ProcessUpLoadRadFile (fdAms, msgPtr);
        break;

        default:
            PrintMsgStatus (AMS_CMD_NOT_SUPPORT);
        break;
    }
    return NO_ERR;
}

// set serial port  timout for debug purpse
BOOL
SetSerialPortTimeout (FT_HANDLE ftH, int timeout)
{
    FTTIMEOUTS  ftTs;
    
    memset (&ftTs, 0, sizeof (FTTIMEOUTS));
    
    if (timeout !=0) {
        ftTs.ReadTotalTimeoutConstant = timeout;
    }

    if (FT_W32_SetCommTimeouts (ftH, &ftTs)) {
    
        return TRUE;
    }  else {
    
        LogError ("Fail to set serialport timeout");
        return FALSE;
    }
}

// get serial port timeout for debug purpose
int
GetSerialPortTimeoutValue (FT_HANDLE ftH)
{
    
    FTTIMEOUTS  ftTs;

    if (FT_W32_GetCommTimeouts (ftH, &ftTs)) {
            return ftTs.ReadTotalTimeoutConstant;
    } else {
        LogError ("Fail to get timeout value");
        return -1;
    }

}


/*
 * FUNCTION
 *  AmsRxThread
 *
 * DESCRIPTION
 *  The ams rx thread is created to process all recieved messages
 *  from external computer (AMS)
 *
 * PARAMETERS
 *  arg     serial port id
 *
 * RETURNS
 *  exit status: EXIT_FAILURE
 *               EXIT_SUCCESS
 */


void *
AmsRxThread (void *arg)
{

    unsigned int             rxCount;
    int                      nByte;
    unsigned char            ch;
    RxState_t                RxState = RX_IDLE;
    FT_HANDLE                *fdAms;
    AmsError_t               msgStatus;
    EngineCommSendMsg_t      amsMsg;
    HANDLE                   hEvent;
    DWORD                    rxBytes;
    DWORD                    txBytes;
    DWORD                    eventDword;
    
    fdAms = (FT_HANDLE *)arg;

    // initial event handle
    InitialEvent ();

    // create event for block reading 
    hEvent = CreateEvent (NULL, false, false, L"RxEvent");
    FT_SetEventNotification (*fdAms, FT_EVENT_RXCHAR, hEvent);

    // set infinit timeout
    //SetSerialPortTimeout (*fdAms, 0);

    memset (&amsMsg, 0, sizeof (EngineCommSendMsg_t));

    while (1) {		 
        //block here for waiting  rx data
        WaitForSingleObject (hEvent, INFINITE);
        FT_GetStatus (*fdAms, &rxBytes, &txBytes, &eventDword);
        if (rxBytes > 0) {
          //  LogDebug ("rxBytes = %d txByte = %d event =0x%x", rxBytes, txBytes, eventDword);
            /* wait the first rx char */
            nByte = ReceiveMsgFromSerialPort (*fdAms, &ch, 1, 10000);
            if (nByte < 1) {
                LogError ("nbyt < 1");
                continue;
            }
            /* Ignor all char until SOH */
            if (ch != SOH) {
                rxBytes = 0;
                continue;
            } else {
                rxCount = 0;
                amsBuf.AmsRxMsg[rxCount++] = ch;
                RxState = RX_DATA;
            }
            while (RxState == RX_DATA) {
                nByte = ReceiveMsgFromSerialPort (*fdAms, &ch, 1, 10000);
                if (nByte < 1) {
                    /* something wrong with rx */
                    LogError ("Rx wrong, nbyt = %d", nByte);
                    RxState = RX_IDLE;
                    break;
                } else {
                    amsBuf.AmsRxMsg[rxCount++] = ch;
                }
                if (rxCount == MAX_MSG_LENGTH) {
                    /* something wrong */
                    LogError ("AMS_RX_BUF_OVERFLOW");
                    RxState = RX_IDLE;
                }
                if (ch == ETX) {
                    RxState = RX_IDLE;
                    amsBuf.rxLength = rxCount ;
                    msgStatus = MsgCheck (*fdAms, amsBuf.AmsRxMsg);
                    if (msgStatus != NO_ERR) {
                        LogError ("MsgCheck error status = %d", msgStatus);
                    } else {
                        // process rx message   
                        ProcessAmsRxMsg (*fdAms, (char *)&amsBuf);
                    }
                } /* end of ch = ETX */
            } /* end of while (RxState ==RX_DATA) */
        }
    } /* end of while */
}

/* 
 * send command to main thread for re-initial analysis
 */
void
SendRxDataMsgToMainThread (AmsAnalysisCmd_t amsCommand)
{
    EngineCommSendMsg_t         amsMsg;
    static HANDLE               AmsRxQueue = 0;
    char                        AmsRxMsg [MAX_MSG_LENGTH];

    if (AmsRxQueue == 0) {
        AmsRxQueue = CreateMessageQueue (AMS_QUEUE_NAME, false);
        if (AmsRxQueue == NULL) {
            printf ("error to open message %s\n", AMS_QUEUE_NAME);
            return;
        }
    }

    AmsRxMsg [1] = amsCommand.command;
    memcpy (&AmsRxMsg [LEN_OFFSET + 4], &amsCommand.type, 2);
    amsMsg.msgId  = MSG_AMS_RX_READY;
    amsMsg.amsData = (char *)AmsRxMsg;
    SendAmsRxQueue ((char *)&amsMsg, sizeof (int) + sizeof (EngineCommSendMsg_t)); 
} 

/*
 * send out rxQueue
 */
void
SendAmsRxQueue (char *msg, DWORD txSize)
{
    static HANDLE               AmsRxQueue = 0;
    if (AmsRxQueue == 0) {
        AmsRxQueue = CreateMessageQueue (AMS_QUEUE_NAME, false);
        if (AmsRxQueue == NULL) {
            printf ("error to open message %s\n", AMS_QUEUE_NAME);
            return;
        }
    }

   WriteMsgQueue (AmsRxQueue, msg, txSize, INFINITE, NULL);
}
