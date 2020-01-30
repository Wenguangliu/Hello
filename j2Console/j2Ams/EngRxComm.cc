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
#include "LogErr.h"
#include "RotorBarcode.h"
#include "Global.h"



DrawerRotorStatus_t     DrawerRotorStatus;
RawTemperature_t        rawTemperatures;


/*
 * FUNCTION
 *  ProcessDrawerCommand   
 * DESCRIPTION
 *  Get System drawer/rotor status
 *
 * PARAMETERS
 *  char * msgPtr   --- msssage queue pointer from AmsEngComm proecss
 *
 * RETURNS
 *  none
 */

void
ProcessDrawerCommand (char *msgPtr)
{
    unsigned char drawerStatus;

    drawerStatus = msgPtr [8];

#ifdef  AMS_ENG_COMM_DEBUG
    printf ("Rx Engine drawer status 0x%x\n", drawerStatus);
#endif


    if (drawerStatus & BIT7) {
        DrawerRotorStatus.DrawerStatus = MECHANISM_JAMMED;
    } else if (drawerStatus & BIT3) {
        DrawerRotorStatus.DrawerStatus = MECHANISM_BUSY;
    } else if (drawerStatus & BIT5) {
        if (drawerStatus & BIT0) {
            DrawerRotorStatus.DrawerStatus = CAM_DOWN_AND_DRAWER_OPEN;
            SendGuiMessage (MSG_AMS_STATUS, AMS_IDLE, 0);
        } else if (drawerStatus & BIT1){
            DrawerRotorStatus.DrawerStatus = CAM_UP_AND_DRAWER_OPEN;
            SendGuiMessage (MSG_AMS_STATUS, AMS_IDLE, 0);
        } else {
            DrawerRotorStatus.DrawerStatus = DRAWER_INVALID_STATUS;
        }
    } else if (drawerStatus & BIT4) {
        if (drawerStatus & BIT0) {
            DrawerRotorStatus.DrawerStatus = CAM_DOWN_AND_DRAWER_CLOSE;
            SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
            SendGuiMessage (MSG_AMS_ERROR_CODE, 0, 0);
        } else if (drawerStatus & BIT1) {
            DrawerRotorStatus.DrawerStatus = CAM_UP_AND_DRAWER_CLOSE;
            /* waiting for drawer to close */
            if (amsState.waitingDrawerClose == 1) {
                amsState.waitingDrawerClose = 0;
#if 0
                /* 
                 * in the case of drawer is open, wait until drawer closed, 
                 * then start to process AMS command 
                 */
                // disable this feature 
                SendRxDataMsgToMainThread (amsState.amsCommand);
#endif
            } else {
                SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
                SendGuiMessage (MSG_AMS_ERROR_CODE, 0, 0);
            }
        } else {
            DrawerRotorStatus.DrawerStatus = DRAWER_INVALID_STATUS;
        }
    }

    if (drawerStatus & BIT6) {
        DrawerRotorStatus.RotorStatus = ROTOR_PRESENT;
    } else {
        DrawerRotorStatus.RotorStatus = ROTOR_NOT_PRESENT;
        if ((DrawerRotorStatus.DrawerStatus == CAM_DOWN_AND_DRAWER_CLOSE) ||
            (DrawerRotorStatus.DrawerStatus == CAM_UP_AND_DRAWER_CLOSE)) { 
            SendGuiMessage (MSG_AMS_STATUS, NO_ROTOR, 0);
        }
    }
}


/*
 * FUNCTION
 *  CalcPlateTemp 
 *
 * DESCRIPTION
 *  Calculate system plate temperature
 *  
 * PARAMETERS
 *  avgAdc --- Average ADC values  
 *
 * RETURNS
 *  float   --- calcluate result
 */ 

float 
CalcPlateTemp(unsigned short avgAdc) 
{   
    return ((float)(((float)avgAdc * PLATE_TEMPERATURE_ADC_CONVERT_MULT) + PLATE_TEMPERATURE_ADC_CONVERT_OFFSET));
}

/*
 * FUNCTION
 *  CalcTemperature 
 *
 * DESCRIPTION
 *  Calculate system temperatures
 * 
 * PARAMETERS
 *  engTemp     --- engine tmperature ADC value
 *  rawTemp     --- system temperature calculation results 
 *
 * RETURNS
 *  none
 */

/* calculate temperatues */
void
CalcTemperature (EngineTemperature_t *engTemp,  RawTemperature_t *rawTemp)
{

    rawTemp->rotorTemperature           =  (float)((float)engTemp->rotorTemperature * ROTOR_TEMPERATURE_CONVERT_MULT) +
                                            FactoryData.airTemperatureOffset;
    rawTemp->ambientTemperature         =  (float)(((float)engTemp->ambientTemperature * AMBIENT_TEMPERATURE_CONVERT_MULT) - 
                                            AMBIENT_TEMPERATURE_CONVERT_OFFSET);
    rawTemp->topPlateTemperature        =  CalcPlateTemp (engTemp->topPlateTemperature); 
    rawTemp->bottomPlateTemperature     =  CalcPlateTemp (engTemp->bottomPlateTemperature);
    rawTemp->topPlateCurrent            =  (float)((float)engTemp->topPlateCurrent * PLATE_CURRENT_CONVERT_MULT);
    rawTemp->bottomPlateCurrent         =  (float)((float)engTemp->bottomPlateCurrent * PLATE_CURRENT_CONVERT_MULT); 
}


/*
 * FUNCTION
 *  ProcessHeaterCommand 
 *
 * DESCRIPTION
 *  Process engine temperature ADC data,  make tempreature
 *  calculation and send results back to AMS 
 *
 * PARAMETERS
 *  fdAms       --- AMS serial port fd
 *  msgPtr      --- message pointer for engine ADC data
 *
 * RETURNS
 *  none
 */

void
ProcessHeaterCommand (FT_HANDLE fdAms, char *msgPtr)
{
    EngineTemperature_t engTemperatures;

    memcpy (&engTemperatures, &msgPtr[4], 16);  

    /* calculate the real temperature */
    CalcTemperature (&engTemperatures, &rawTemperatures);
    /* now, should send this information to AMS */
    amsState.amsTxState = AMS_TX_READY_TX_1_TEMPERATURE;
    SendMsgToAmsTxThread (AMS_TX_TEMP, NULL, 0);
}


/*
 * FUNCTION
 *  ProcessTemperatureCommand 
 *
 * DESCRIPTION
 * this function do nothing now. Just put here 
 *
 * PARAMETERS
 *  fdAms       --- AMS serial port fd
 *  msgPtr      --- message pointer for engine ADC data
 *
 * RETURNS
 *  none
 */

void
ProcessTemperatureCommand (FT_HANDLE fdAms, char *msgPtr)
{
#if 0 
    EngineTemperature_t engTemperatures;

    memcpy (&engTemperatures, &msgPtr[4], sizeof (EngineTemperature_t));  

    /* calculate the raw temperature */
    CalcTemperature (&engTemperatures, &rawTemperatures);
    /*
     * SendMsgToHeterThread ()...
     */
    SendMsgToHeaterThread (MSG_HEATER_DATA, &rawTemperatures);
#endif
}

/*
 * to be done in the future
 */

void
ProcessErrorCommand (FT_HANDLE fdAms, char *msgPtr)
{
    unsigned short errorNumber;

    memcpy (&errorNumber, &msgPtr [8], 2);
    /* send error code message to GUI process */
    SendGuiMessage (MSG_AMS_ERROR_CODE, 0, errorNumber);
//  printf ("Engine Error code number 0x%x\n",errorNumber);
    // in the case of barcode test, get error message, open the drawer
    if (amsState.barcodeTest == 1) {
        amsState.barcodeTest = 0;
        // dump error code to AMS station, simulat no start barcode
        DumpBarCode (fdAms, NULL, NULL, ERR_BC_NO_START, true, false);
        SendDrawerMessage ('O');
    }

}

void
SendErrorCommnd (FT_HANDLE fdAms, unsigned short  errorNumber)
{
    SendGuiMessage (MSG_AMS_ERROR_CODE, 0, errorNumber);
}

/*
 * FUNCTION
 *  ProcessAnalysisDoneCommand 
 *
 * DESCRIPTION
 *  received analysis done command, get the analysis data
 *  from share memory and dump the results to AMS  
 *
 * PARAMETERS
 *  char * msgPtr   --- msssage queue pointer from AmsEngComm proecss
 *  int fdAms       --- Ams serial port fd
 *
 * RETURNS
 *  none
 */

void
ProcessACommand (FT_HANDLE fdAms, char *msgPtr)
{
    
    unsigned char  analysisStatus; 

    /* get analysis Status */
    analysisStatus = msgPtr [8];

    switch (analysisStatus) {
        case TEMPERATURE_TEST_DONE:
            /* need to dump all temperature test results to AMS */
            /* dump temp raw data ... */
            SendGuiMessage (MSG_AMS_STATUS, UPLOAD_TEMP_RESULTS, 0);
            DumpTemperatureTestData (fdAms, true);
            SendDrawerMessage ('O');
        break;

        case ANALYSIS_DONE:
        break;

        case CUV_DELAY_TEST_DONE:
            /* dump global delay test data */
            /* ZZZ */
            printf ("Global Delay done\n");
            SendGuiMessage (MSG_AMS_STATUS, UPLOAD_GLOBAL_DELAY_RESULTS , 0);
            Sleep (10);
            DumpGlobalCuvDelayData (fdAms, false); 
            /* 
                ZZZ 
                may add send command 'X' to get Bias value here 
             */
            SendDrawerMessage ('O');
        break;

        case OPTICAL_DACS:
            /*  
             * calculate trim DAC for all 9 wavelength, 
             * send DAC value back to Engine 
             */
#if 0
            SendGuiMessage (MSG_AMS_STATUS, OPTICAL_CAL_DONE, 0);
#endif
            SetDacTrims ();
        break;

        case NDXT_TEST_DONE:
            SendGuiMessage (MSG_AMS_STATUS,UPLOAD_NDXT_RESULTS, 0);
            DumpNdxtData (fdAms, false);
            SendDrawerMessage ('O');
        break;

        case BLACK_OFFSETS:
            /* calculate black wavelength */
            SetWavelengthOffsets ();
        break;

        case  MOTOR_TEST_DONE:
            SendGuiMessage (MSG_AMS_STATUS,UPLOAD_MOTOR_RESULTS, 0);
            DumpMotorData (fdAms, false);
            /* waiting for AMS to finish calculation */
            Sleep (20000);
            FanControl (true);
            SendDrawerMessage ('O');
        break;

        case CANCEL_DONE:
            FanControl (true);
            SendDrawerMessage ('O');
        break;

        default:
        break;
    }
}

/* 
 *
 */
void
ProcessBarcodeCommand (FT_HANDLE fdAms, char *msgPtr)
{
    int                 errNum;
    char                barcodeDigits [sizeof (BarcodeData_t) + 1];
    unsigned short      barcodeFactor [8];


    if (amsState.barcodeTest ==1) {
        errNum = ProcessRotorBarcode (msgPtr, barcodeFactor,(BarcodeData_t *)barcodeDigits);
        DumpBarCode (fdAms, barcodeDigits, barcodeFactor, errNum, true, false);
        amsState.barcodeTest = 0;
        SendDrawerMessage ('O');
    }
    
}

/*
 * send barcode alignment test result to AMS PC
 */
void
ProcessBarcodeAlignCommand (FT_HANDLE fdAms, char *msgPtr)
{
    TxBarcodeAlign (fdAms, (msgPtr + 8));
    /* finishing barcode alignment testing, open the Drawer */
    SendDrawerMessage ('O');
    
}

/*
 * Send barcode gain resulte to AMS PC
 */
void
ProcessBarcodeGainCommand (FT_HANDLE fdAms, char *msgPtr)
{
    TxBarcodeGain (fdAms, (unsigned short *)(msgPtr + 4));
    /* finishing barcode alignment testing, open the Drawer */
    SendDrawerMessage ('O');
}


/*
 * send back cuvette index width value to AMS
 */
void
ProcessCuvetteDetecionCommand (FT_HANDLE fdAms, char *msgPtr)
{
    static unsigned short idxWidth [8];
    unsigned char loop;

    loop = msgPtr[6];

    if (loop < 8) {
        idxWidth [loop] = (*((unsigned short *)(msgPtr + 10)));
        LogInfo ("loop=%d idxw=%d", loop, idxWidth [loop]);
    }  else {
        LogError ("Cuv wrong loop %d", loop);
    }
    /* end of testing, send to AMS PC */
    if (loop == 7) {
        Sleep (2000);
        /* wait for 2 seconds for stop spindle motor */
        TxCuvetteDetection (fdAms, &idxWidth [0]);
        /* open the drawer */
        SendDrawerMessage ('O');
    } else if (loop == 255) {
        // in the case of loop = 255, something with test
        // send gui error code
        SendErrorCommnd (fdAms, AMS_CUVETTE_TEST_ERROR);
        // open drawer
        SendDrawerMessage ('O');
    }
}

/*
 * FUNCTION
 *   ProcessEngRxMsg 
 *
 * DESCRIPTION
 *  Process Engine command which is fowared by AmsEngineComm process
 *
 * PARAMETERS
 *   int  fdAms     --- Ams serial port fd
 *   char *msgPtr   --- Engine message queue pointer
 *   int  length    --- message length
 *
 * RETURNS
 *  none
 */

void
ProcessEngRxMsg (FT_HANDLE fdAms, char *msgPtr, int length)
{
    char   command;
    int    count;

    command = msgPtr [0];

#ifdef  AMS_ENG_COMM_DEBUG
    printf ("EngComm: Rx Engine cmmand %c\n", command);
#endif

    switch (command) {
        case 'D':
            ProcessDrawerCommand (msgPtr);
        break;

        case 'A':
            ProcessACommand (fdAms, msgPtr);
        break;

        case 'H':
            ProcessHeaterCommand (fdAms, msgPtr);
        break;
        
        case 'B':
            ProcessBarcodeCommand (fdAms, msgPtr);
        break;

        case 'T':
            ProcessTemperatureCommand (fdAms, msgPtr);
        break;

        case 'E':
            ProcessErrorCommand (fdAms, msgPtr);
        break;

        case 'I':
            LogDebug ("Get barcode alignment code");
            ProcessBarcodeAlignCommand (fdAms, msgPtr);
        break;

        case 'K':
            ProcessBarcodeGainCommand (fdAms, msgPtr);
        break;

        case 'Q':
            ProcessCuvetteDetecionCommand (fdAms, msgPtr);
        break;

        case '?':
            // for engine debug messages
            count = *(unsigned short *)&msgPtr[2];
            LogInfo ("Engine ? msg: %*.*s", count, count, &msgPtr[4]);
        break;

        default:
        
        break;
    }

}

