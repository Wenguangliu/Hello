/*
 *
 * MODULE DESCRIPTION
 *  Iris system AMS process 
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
#include <pm.h>

#include "AmsComm.h"
#include "AmsData.h"
#include "CommonDefines.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "ErrLoggerMessages.h"
#include "BarcodeData.h"
#include "State.h"
#include "Shmem.h"
#include "FtdiConfig.h"
#include "ftdi.h"
#include "Epcs.h"
#include "LogErr.h"
#include "XmlData.h"
#include "ParseSettings.h"
#include "Global.h"


static HANDLE       AmsQueue = 0;
static HANDLE       logQueue = 0;
FT_HANDLE			fdAms;

FactoryData_t       FactoryData;
CalibrationData_t   CalibrationData;
AmsState_t          amsState;
SettingsData_t      Settings;

/* share memory pointer*/
AmsData_t        *amsData;
BarcodeData_t    *barCodeData;

#undef         PING_DEUBG



/* delete all share memory */
void
DeleteAllShareMemory (void)
{

    UnmapViewOfFile (amsData);
    UnmapViewOfFile (barCodeData);
}

/* create message queue */
bool
CreatAmsQueue (void)
{

    /* create ams main thread msg queue */
    AmsQueue =  CreateMessageQueue (AMS_QUEUE_NAME, TRUE); 
    if (AmsQueue == NULL) {
        return FALSE;
    }
    return TRUE;
}

void
CloseAmsQueue (void)
{
    if (AmsQueue != NULL) {
        CloseMsgQueue (AmsQueue);
    }
}

/* create message queue */
bool
CreatLogQueue (void)
{
    
    /* create ams main thread msg queue */
    logQueue = CreateMessageQueue (ERR_LOGGER_QUEUE_NAME , true);
    if (logQueue == NULL) {
        return false;
    }
    return true;
}

void
CloseLogQueue (void)
{
    if (logQueue != NULL) {
        CloseMsgQueue (logQueue);
    }
}

void *
TestThread (void *arg)
{
     
    FT_HANDLE   *fdAms;
    HANDLE      hEvent;
    char        ch;
    DWORD       rxBytes;
    DWORD       txBytes;
    DWORD       eventDword;

    fdAms = (FT_HANDLE *)arg;


    hEvent = CreateEvent (NULL, false, false, NULL);
    FT_SetEventNotification (*fdAms, FT_EVENT_RXCHAR, hEvent);

    while (1) {
       
       WaitForSingleObject (hEvent, INFINITE);
       FT_GetStatus (*fdAms, &rxBytes, &txBytes, &eventDword); 
       if (rxBytes > 0) {
            ReadSerPort (*fdAms, (unsigned char *)&ch, rxBytes); 
            WriteSerPort (*fdAms, (unsigned char *)&ch, rxBytes);
       }
       printf ("%c", ch);
            
    }
}

/*
 * FUNCTION
 *  main 
 *
 * DESCRIPTION
 *  main AMS process 
 *
 * PARAMETERS
 *  none
 *
 * RETURNS
 *  exit status: EXIT_FAILURE
 *               EXIT_SUCCESS
 */

int 
main (void)
{
    DWORD								    receivedSize = 0;
    char								    *msgPtr;
    EngineCommSendMsg_t                     amsMsg;
    DWORD									flags;
    DWORD									error;
    BOOL                                    result;


    /* initial amsState machine */
    amsState.amsRxState = AMS_RX_IDEL;
    amsState.amsTxState = AMS_TX_IDEL;
    


    // create all message queues
    if (CreatAmsQueue () == false) {
        printf( "failure to creat ams message queue\n");
        return -1;
    }

    if (CreateAmsTxQueue () == false) {
        printf( "failure to creat ams Tx message queue\n");
        return -1;
    }
    if (CreateAmsEngineQueue () == false) {
        printf( "failure to creat ams engine message queue\n");
        return -1;
    }

    if (CreateAmsGuiQueue () == false) {
        printf( "failure to create Gui queue\n");
        return -1;
    }
    if (CreatLogQueue () == false) {
        printf( "failure to create log queue\n");
        return -1;
    }

    // start log process first 
     StartupLogErrorProcess ();

    /* create  share memory */
    amsData = (AmsData_t *)CreateSharedMem (AMS_SHMEM_NAME, sizeof (AmsData_t));
    barCodeData = (BarcodeData_t *)CreateSharedMem (BARCODE_SHMEM_NAME, sizeof (BarcodeData_t));


    /*initial calibration data structure*/
    memset (&FactoryData, 0, sizeof (FactoryData_t));
    memset (&CalibrationData, 0, sizeof (CalibrationData_t));
    
     /* read factory xml into global data structure */
    if (ParseFactoryXml (&FactoryData) != NO_ERR) {
        LogError ("Fail parse factory xml"); 
    }

    /* read factory xml into global data structure */
    if (ParseCalibration (&CalibrationData) != NO_ERR) {
        LogError ("Fail to parse calibration.xlm");
    }

    if (ParseSettings (&Settings) != NO_ERR) {
        LogError ("Fail to parse settings.xlm");
    }

    /* initial FTDI chip */
    InitialEngineBoardInterface ();
	if (InitialAmsSerialPort (&fdAms) == -1) {
        return -1;
    } 


 
    /* create Asm tx Thread */
    CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)AmsTxThread, (FT_HANDLE *)&fdAms, 0, NULL);


    /* create Asm rx Thread */
    CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)AmsRxThread, (FT_HANDLE *)&fdAms, 0, NULL);
#if 0

    /* create heater control thread */
    CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)HeaterControllerThread, NULL, 0,  NULL);

    /* create heater timer thread */
    CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, NULL, 0, NULL);
#endif



	/* start all child processes */
    StartupAllProcess ();
    // wait for 3 seconds for Engine board boot up 
    Sleep (3000);
    

    /* send default factory data to engine board */
    SendFactoryDatas ();

    /* send default calibration data to engine board*/
    SendCalibrationFactors ();

    /* set heater Dac and fan dac to engine board */ 
    SendHeaterCommand ('S');

    /* get engine drawer status */
    SendDrawerMessage ('S');

    /* set Engine Fan idle speed to be high */
    SendFanIdleSpeed (IDLE_FAN_HIGH_SPEED);

#ifdef UPLOAD_RAD_SIMU 
     ProcessUpLoadRadFile (fdAms, NULL);
#endif

#ifdef PING_DEUBG
    while (1) {
        Sleep (5000);
        SendGuiMessage (MSG_AMS_STATUS,ETHERNET_PORT_TEST, 0); 
        ProcessEthernetPortTest (fdAms);
        
    } 
#endif
  

    while (1) {
        
        /* waiting for message */
        result = ReadMsgQueue (AmsQueue, &amsMsg, MAX_QUEUE_LENGTH, &receivedSize, INFINITE, &flags);

        if (result != FALSE) {
            if (receivedSize > 0) {
                switch (amsMsg.msgId) {
                    case MSG_AMS_RX_READY:
                        /* msg from AMS station (PC) */
                        msgPtr = (char *)amsMsg.data.message;
                        /* process AMS rx message */
                        ProcessAmsRxMsg (fdAms, msgPtr);
                        amsMsg.msgId = 0;
                    break;

                    case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
                        /* msg from engine */
                        /* process engine forward message */
                        ProcessEngRxMsg (fdAms, (char *)amsMsg.data.message, amsMsg.data.length);
                    break;

                    case MSG_CONSOLE_RX_READY:
                        /* msg from Gui */
                        ProcessConsoleRxMsg ((char *)amsMsg.data.message, amsMsg.data.length);
                    break;

                    case MSG_AMS_EXIT:
#if  0 
                        ExitAms (START_ANALYZER);
#else
                        /*shutdown system and repower */
                        SetSystemPowerState (NULL, POWER_STATE_RESET, 0);
#endif
                    break;


                    default:
                    break;
                }
            } else {
                error = GetLastError ();
                LogError ("AmsEngineComm: AMS main queue error size =  -1 error=%d", error);
            }
        } else {
            error = GetLastError ();
            LogError ("AmsEngineComm: AMS main queue error size =  -1 error=%d", error);
        }
    }

    return 0;
}
