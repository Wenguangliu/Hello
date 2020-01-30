/*
 *  AmsTxComm.c
 *
 * MODULE DESCRIPTION
 *  Process  Iris tx  messages to AMS
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

#include "CommonDefines.h"
#include "Serial.h"
#include "AmsComm.h"
#include "XmlData.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "State.h"
#include "Temperature.h"
#include "EngComm.h"
#include "AmsData.h"
#include "RotorBarcode.h"
#include "LogErr.h"
#include "Rad.h"
#include "LogErr.h"
#include "Global.h"

#define UPLOAD_LOG_FILE_MAX_SIZE   1000000

// txQueue
HANDLE          AmsTxQueue = 0;
// tx message array
#define         MAX_TX_MSG_NUM          8
AmsTxMsg_t      AmsTxMsg [MAX_TX_MSG_NUM];
// to heater message array
HeaterMsg_t     HeaterMsg [MAX_TX_MSG_NUM];

#define MAX_PACK_SIZE   4096
#undef  CRC_DEBUG
#undef  LOG_CRC_CHK

DWORD   ACK_TIMEOUT =  10000L;  /* 10 second  timeout */


static unsigned short wavelengthAdcOffset [10];

static radFileList_t   *fileStart, *fileLast, *fileCurrent;


/* Dac trim wavelength index table */
unsigned char dacWaveLengthTable [] =
        {
           W850, W630, W515, W600,
           W850, W550, W467, W500,
           W850, W467, W405, W340
        };

/* Ndxt wavelength index table */
unsigned char ndxtWaveLengthTable [] =
        {
            W850, W630, W515, W600,
            W850, W550, W467, W500,
            W850, W405, W340, W340
        };

/* Dac value wavelength index table */
unsigned char
wavelengthDacTable [] =
        {
            W850, W630, W515, W600,
            W550, W500, W467, W405,
            W340, WHTE
        };


/*
 * FUNCTION
 *  InitBlackOffset
 *
 * DESCRIPTION
 *  clear black offset array
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  None
 */

void
InitBlackOffset (void)
{
    memset (wavelengthAdcOffset, 0, sizeof (wavelengthAdcOffset));
}

/*
 * FUNCTION
 *  InitAmsTxComm
 *
 * DESCRIPTION
 *  Create AmsTxqueue
 *
 * PARAMETERS
 *  None
 *
 * RETURNS
 *  true  ---  queue is created
 *  false ---  fail to created
 */

bool
CreateAmsTxQueue (void)
{
    /* create engine comm message queue */
    AmsTxQueue = CreateMessageQueue (AMS_TX_QUEUE_NAME, false);
    if (AmsTxQueue == NULL) {
        return false;
    }
    return true;
}

void
CloseAmsTxQueue (void)
{
    if (AmsTxQueue != NULL) {
        CloseMsgQueue (AmsTxQueue);
    }
}



/* send message to ams tx thread  */
void
SendMsgToAmsTxThread (AmsTxCommad_t  command, void *data, unsigned short length)
{

    // for windows OS, messaage queue is pointer to message. 
    // must use multiple message to prevent overwritten previous message
    static int          txMsgIndex = 0;
    AmsTxMsg_t          *amsTxMsg;

    amsTxMsg            = &AmsTxMsg [txMsgIndex++];

    if (txMsgIndex >= MAX_TX_MSG_NUM) {
        txMsgIndex = 0;
    }

    amsTxMsg->msgId      = MSG_AMS_TX_READY;
    amsTxMsg->txCommand  = command;

    if (length != 0) {
        memcpy (amsTxMsg->data, (unsigned char *)data, length);
    }
    if (AmsTxQueue != NULL) {
        WriteMsgQueue (AmsTxQueue,  amsTxMsg, sizeof (AmsTxMsg_t), INFINITE, NULL);
    } else {
        LogError ( "AmsTxComm: Error to get AmsTxQueue");
    }

}


#if 0

/*
 * send message to ams heater control thread
 */

void
SendMsgToHeaterThread (HeaterMsgId_t command, RawTemperature_t *rawTemp)
{
    static int          txMsgIndex = 0;
    HeaterMsg_t         *msg;
    static HANDLE       queue = NULL;

    if (queue == NULL) {
        queue = CreateMessageQueue (AMS_HEATER_CONTROL_QUEUE_NAME, false);
        if (queue == NULL) {
            LogError ("EngineTx: failure to open HeaterControl queue");
            return;
        }
    }
    msg = &HeaterMsg [txMsgIndex++]; 
    if (txMsgIndex >= MAX_TX_MSG_NUM) {
        txMsgIndex = 0;
    }
    msg->msgId = command;
    if (rawTemp != NULL) {
        memcpy (&msg->heaterData, rawTemp, sizeof (RawTemperature_t));
    }
    WriteMsgQueue (queue,  msg, sizeof (HeaterMsg_t), INFINITE, NULL);
}
#endif

/*
 * send temperatrue to AMS
 */
void
TxAmsTemp (FT_HANDLE fdAms, AmsTxProcessState_t txState)
{

    unsigned short  length = 10;
    char            msg[10];
    unsigned char   id = 0;
    float           temperature = 0;


    switch (txState) {
        case AMS_TX_READY_TX_1_TEMPERATURE:
            id = 0;
            temperature = rawTemperatures.rotorTemperature;
            amsState.amsTxState =  AMS_TX_READY_TX_2_TEMPERATURE;
        break;

        case AMS_TX_READY_TX_2_TEMPERATURE:
            id = 1;
            temperature = rawTemperatures.ambientTemperature;
            amsState.amsTxState =  AMS_TX_READY_TX_3_TEMPERATURE;
        break;

        case AMS_TX_READY_TX_3_TEMPERATURE:
            id = 2;
            temperature = rawTemperatures.topPlateTemperature;
            amsState.amsTxState =  AMS_TX_READY_TX_4_TEMPERATURE;
        break;

        case AMS_TX_READY_TX_4_TEMPERATURE:
            id = 3;
            temperature = rawTemperatures.bottomPlateTemperature;
            amsState.amsTxState =  AMS_TX_IDEL;
            amsState.amsRxState =  AMS_RX_IDEL;
        break;

        default:
        break;
    }

    ByteToAscii (id, msg);
    FloatToAscii (temperature, &msg[2], TRUE);
    AmsTransmitMsg (fdAms, TEMP_RESPONSE , msg, length);
}

/*
 * transimit Barcode Alignment test results to AMS PC
 */
void
TxBarcodeAlign (FT_HANDLE fdAms, char *barNumber)
{
    char msg [20], *ptr, i;

    ptr = &msg [0];

    /* convert to ASCII */
    for (i =0; i < 10; i++) {
        ptr = ByteToAscii (*barNumber++, ptr);
    }
    /* send results to AMS PC */
    AmsTransmitMsg (fdAms, BARCODE_ALIGN_RESPONSE, msg, 20);

}


/*
 * tramsimit barcode gain t o AMS PC
 */
void
TxBarcodeGain (FT_HANDLE fdAms, unsigned short *adcValue)
{
    char msg [52], *ptr, i;

    ptr = &msg [0];

    /* convert to ASCII */
    for (i = 0; i < 13; i++) {
        ptr = WordToAscii (*adcValue++, ptr, 0);
    }

    /* send results to AMS PC */
    AmsTransmitMsg (fdAms, BARCODE_GAIN_RESPONSE, msg, 52);
}

/*
 * transimit cuvette width to AMS PC
 */
void
TxCuvetteDetection (FT_HANDLE fdAms,  unsigned short *cuvInxWidth)
{
    char msg [64], *ptr, i;

    ptr = &msg [0];

    /* convert to ASCII */
    for (i = 0; i < 8; i++) {
        ptr = WordToAscii (*cuvInxWidth++, ptr, 0);
    }
    /* send results to AMS PC */
    AmsTransmitMsg (fdAms, CUVETTE_TEST_RESPONSE, msg, 32);
}


/*
 * upload log file to PC
 */
int
UploadLogFile (FT_HANDLE fdAms, char *fileName)
{

    size_t              count;
    char                *ptr, ch;
    unsigned long       totalLen ,txTotallLen, offset;
    FILE                *fd;
    int                 ret, reTxTime;
    static char         buff [4096], header [24];
    wchar_t             cFilename [255],  tFilename [255];


    // current log file
    if (strcmp (fileName, LOG_FILE_CURRENT) == 0) {
        fileName = LOG_FILE_TEMP;
        offset     = logFileParameters.cOffset;
        totalLen   = logFileParameters.cTotalLen;
    } else {
        // old log file
        offset     = logFileParameters.oOffset;
        totalLen   = logFileParameters.oTotalLen;
    }

    LogDebug ("upload file name=%s length =%d offset =%d",fileName, totalLen, offset);
    fd = fopen (fileName,  "rb");
    if (fd != NULL) {
       fseek (fd, offset, SEEK_SET); 
    }

    /* already send length */
    txTotallLen = 0;
    reTxTime = 0;
    count = 0;

    while (txTotallLen < totalLen) {
        /* not a re-send package, read new data into buff */
        if (reTxTime == 0) {
            memset (buff, 0, 4096);
            count = fread (buff, 1, MAX_PACK_SIZE, fd);
            if (count > 0) {
                txTotallLen += count;
            }
        }
        /* now send package length */
        if (count > 0) {
            ptr = &header [0];
            /* first line is package length */
            ptr = LongToAscii (count, header, 1);
            AppenEol (ptr);
            /* send out package header */
            WriteSerPort (fdAms, header, 10);
            WriteSerPort (fdAms, buff, count);
            /* set ACK state machine to wait ack response state */
            amsState.amsRxState = AMS_RX_UPLOAD_WAIT_ACK;
            /* wait for ACK signal */
            ret = WaitForSingleObject (hReceivedChar, ACK_TIMEOUT);
            EnterCriticalSection (&csReceivedChar);
            ch = rxChar;
            LeaveCriticalSection (&csReceivedChar);
            amsState.amsRxState = AMS_RX_IDEL;
            /* timeout, something wrong with PC, end of tx package */
            if ((ret == WAIT_TIMEOUT) || ((ch == NAK) && (reTxTime >= 3))) {
                if (reTxTime >= 3) {
                    LogError ("More than 3 times NAK, end of tx package");
                    SendErrorCommnd (fdAms, UPLOAD_LOG_NAK_ERROR);
                } else {
                    LogError ("PC timeout, end of tx package");
                    SendErrorCommnd (fdAms, UPLOAD_LOG_TIMEOUT_ERROR);
                }
                fclose (fd);
                return -2;
            } else if (ch == NAK) {
                reTxTime++;
                printf ("PC Send NAK %d times\n", reTxTime);
            } else {
               /* good package, reset reTxTime */
               reTxTime = 0;
            }
        } /* end of if count > 0 */
    } /* end of txLen < TotallLen */

    // close log file
    fclose (fd);
    // delay for pc to process package
    Sleep (2000);
    return 0;
}


/*
 * start tx log file
 */
void
TxAmsLogFile (FT_HANDLE  fdAms)
{
    static char endOfFile = EOFILE;

    SendErrorCommnd (fdAms, 0);
    /* update Gui Status display */
    SendGuiMessage (MSG_AMS_STATUS, UPLOAD_CURRENT_LOG_FILE, 0);

    /* changing baud rate to be upload speed before uploading the data */
    ChangeSerialPortBaudRate (fdAms, AMS_LOG_UUPLOAD_BAUD_RATE, AMS_PORT);
    Sleep (5000);

    /* clear error code before start upload log file */
    //SendErrorCommnd (fdAms, 0);

    if (UploadLogFile (fdAms, LOG_FILE_CURRENT) == 0) {
        /* upload old log file */
        SendGuiMessage (MSG_AMS_STATUS, UPLOAD_OLD_LOG_FILE, 0);
        UploadLogFile (fdAms, LOG_FILE_OLD);
    }

    /* changing baud rate back to 19200  */
    ChangeSerialPortBaudRate (fdAms, AMS_BAUD_RATE, AMS_PORT);
    SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
}

/*
 * upload Rad file to AMS
 */
void
UploadRadFile (FT_HANDLE fdAms, radFile_t radFile)
{

    char           msg [2048], *ptr, *tPtr;
    unsigned char           i;
    unsigned char           waveLength, flash ;
    unsigned short          length = 596;

    ptr = &msg [0];

    /* get time string */
    ptr = WordToAscii (radFile.fileTime.year,   ptr, 0);
    ptr = ByteToAscii (radFile.fileTime.month,  ptr);
    ptr = ByteToAscii (radFile.fileTime.day,    ptr);
    ptr = ByteToAscii (radFile.fileTime.hour,   ptr);
    ptr = ByteToAscii (radFile.fileTime.minute, ptr);

    /* get serial number */
    tPtr= &radFile.instrumentInfo.instrumentSerialNumber [0];

    for (i = 0; i < 12; i++) {
        ptr = ByteToAscii (*tPtr++, ptr);
    }

    /* now, get the Rad file raw data */
    for (waveLength = 0; waveLength < 10; waveLength++) {
        for (flash = 0; flash < 11; flash++) {
            ptr = WordToAscii (radFile.rawFlashWaveLength[flash].rawFlashReading [waveLength], ptr, 0);
            *ptr++ = ',';
        }
        ptr--;
        ptr = AppenEol (ptr);
    }
#ifdef UPLOAD_RAD_SIMU
    printf ("%s\n", msg);
#endif

    AmsTransmitMsg (fdAms, START_UPLOAD_RAD_FILE , msg, length);
}


/*
 * put all needs sending Rad file into link list
 */
void
SearchRadFile (timeFlag_t *timeFlag)
{

    wchar_t             wFilename [255];
    char                fileName [255], finalFileName [255];
    WIN32_FIND_DATA     fileData;
    HANDLE              hSearch;
    BOOL                bFinished = FALSE;
    timeFlag_t          fileTime;
    char                timeChar [5];
    char *              s;
    unsigned char       sendFlag;
    unsigned int        fileIndex = 0;


    fileStart = fileLast = NULL;

    
    //get file direcory name
    wsprintf(wFilename, L"%S/Rad/*.Rad", "/Hard Disk/Abaxis");

    //get first file
    hSearch = FindFirstFile(wFilename, &fileData);

    if (hSearch != INVALID_HANDLE_VALUE) {
        while (!bFinished) {
            // convert w32 to char file name
            sprintf (fileName, "%S", fileData.cFileName);
            strcpy (finalFileName, fileName);
            s = strtok (fileName,".");
            if (s == NULL) {
                goto NEXT;
            }
            s = strtok (0, ".");
            if (s == NULL) {
                goto NEXT;
            }
            s = strtok (0, ".");
            if (s == NULL) {
                goto NEXT;
            }
            /* get year */
            strncpy (timeChar, s, 4);
            s += 4;
            timeChar [4] = '\0';
            fileTime.year = atoi (timeChar);
            /* get month */
            strncpy (timeChar, s, 2);
            s += 2;
            timeChar [2] = '\0';
            fileTime.month = atoi (timeChar);
            /* get day */
            strncpy (timeChar, s, 2);
            s += 2;
            timeChar [2] = '\0';
            fileTime.day = atoi (timeChar);
            /* get hour */
            strncpy (timeChar, s, 2);
            s += 2;
            timeChar [2] = '\0';
            fileTime.hour = atoi (timeChar);
           /* get min */
            strncpy (timeChar, s, 2);
            timeChar [2] = '\0';
            fileTime.minute = atoi (timeChar);
#if 0
            LogDebug ("year = %d month = %d day =%d hour =%d min =%d",
                         fileTime.year, fileTime.month, fileTime.day,
                         fileTime.hour, fileTime.minute);
#endif
             sendFlag = FALSE;
              /* compare with time flag */
             if (timeFlag != NULL) {
                if (timeFlag->year < fileTime.year) {
                    sendFlag = TRUE;
                 } else if (timeFlag->year == fileTime.year) {
                    if (timeFlag->month < fileTime.month) {
                        sendFlag = TRUE;
                    }  else if (timeFlag->month == fileTime.month) {
                        if (timeFlag->day < fileTime.day) {
                            sendFlag = TRUE;
                        } else if (timeFlag->day == fileTime.day) {
                            if (timeFlag->hour < fileTime.hour) {
                                sendFlag = TRUE;
                            } else if (timeFlag->hour == fileTime.hour) {
                                if (timeFlag->minute <= fileTime.minute) {
                                    sendFlag = TRUE;
                                }
                            }
                        }
                    }
                }
            }
            if (sendFlag == TRUE) {
                    /* put the file into link list */
                    if (fileStart == NULL) {
                        if ((fileStart = (radFileList_t *)malloc (sizeof (radFileList_t))) != NULL) {
                            fileLast = fileStart;
                            fileLast->fileIndex = fileIndex;
                            strcpy (fileLast->fileName, finalFileName);
                            memcpy (&fileLast->fileTime, &fileTime, sizeof (timeFlag_t));
                            fileLast->next = NULL;
                        } else {
                            LogError ("Fail to get Rad link list memory");
                            return;
                        }

                    } else {
                        if ((fileLast->next = (radFileList_t *)malloc (sizeof (radFileList_t))) != NULL) {
                            fileLast = fileLast->next;
                            fileLast->fileIndex = fileIndex;
                            strcpy (fileLast->fileName, finalFileName);
                            memcpy (&fileLast->fileTime, &fileTime, sizeof (timeFlag_t));
                            fileLast->next = NULL;
                        } else {
                            LogError ("Fail to get Rad link list memory");
                            return;
                        }

                    }
                    fileIndex++;
           } /* if (sendFlag == TRUE) */
            // now find out next one 
NEXT:     if (!FindNextFile (hSearch, &fileData)) {
                if (GetLastError () == ERROR_NO_MORE_FILES) {
                    LogDebug("No more Rad files");
                    FindClose(hSearch);
                    bFinished = TRUE;
                }
           }
        }
        
    } else {
         LogDebug ("Rad file folder is empty");
    }

    fileCurrent = fileStart;
}

/*
 * free the link list
 */
void
FreeAllList (void)
{
    radFileList_t  *freeList;

    while (fileCurrent) {
        freeList = fileCurrent;
        fileCurrent = fileCurrent->next;
        free (freeList);
    }
}

/*
 *  send out RAD file based on timeFlag
 */
unsigned char
SendRadFile (FT_HANDLE fdAms)
{
    radFile_t       radFile;
    unsigned long   radFileLength;
    unsigned long   offset;
    unsigned char   flash;
    static char     buff [MAX_RAD_FILE_LENGTH], *ptr;
    int             count;
    FILE            *fd;
    char            fileName [255];


    if (fileCurrent  != NULL) {
        sprintf (fileName, "%s%s","Hard Disk/Abaxis/Rad/",fileCurrent->fileName); 
        //fd = fopen ("Hard Disk/Abaxis/Rad/"fileCurrent->fileName,  "rb");
        fd = fopen (fileName,  "rb");
        if (fd == NULL) {
            LogError ("Fail to open Rad file %s", fileCurrent->fileName);
         } else {
            ptr = &buff [0];
            radFileLength = 0;

#if 0
            while (((count = fread (ptr, 1, INT_MAX, fd)) > 0) && (radFileLength < MAX_RAD_FILE_LENGTH)) {
                ptr += count;
                radFileLength += count;
            }
#else
            while (((count = fread (ptr, 1, 8192, fd)) > 0) && (radFileLength < MAX_RAD_FILE_LENGTH)) {
                ptr += count;
                radFileLength += count;
            }
#endif
            fclose (fd);
            //printf ("radFileLength  = %ld\n", radFileLength);

            ptr = &buff [0] + INSTRUMENTINFO_OFFSET ;
            /* get seial number */
            memcpy (&radFile.instrumentInfo, ptr, sizeof (instrumentInfo_t));
            //printf ("seria num =%s\n", radFile.instrumentInfo.instrumentSerialNumber);
            /* now get the counter number */
            ptr = &buff [0] + DATA_DESCRIPTOR_OFFSET;
            memcpy (&radFile.dataDescriptor, ptr, sizeof (dataDescriptor_t));
            count = 0;
            /* get analyResults Counter */
            for (flash = 0; flash < 64; flash++) {
                if (radFile.dataDescriptor.resultsAnalyteIndices [flash]  != 0xFF) {
                    count++;
                 }
            }

            /* calculate optical trim raw data offset */
            offset = ROTOR_ANALY_RESULT_OFFSET + (count * ANALYSIS_RESULTS_LENGTH) +
                        (radFile.dataDescriptor.absorbanceCount * ABSORBANCES_LENGTH) +
                        (radFile.dataDescriptor.statusCount * ANALYSIS_STATUS_LENGTH) +
                        (radFile.dataDescriptor.temperatureCount * RAW_TEMPERATURE_LENGTH) +
                         TRIM_DAC_RAW_DATA_OFFSET;
            /* now get the optical  raw data */
            ptr = &buff [0] + offset;
            for (flash = 0; flash < 11; flash++) {
                memcpy (&radFile.rawFlashWaveLength [flash], ptr, sizeof (rawFlashWaveLength_t));
                ptr += sizeof (rawFlashWaveLength_t);
             }
#if 0
            for (flash = 0; flash < 11; flash++) {
                for (waveLength = 0; waveLength < 10; waveLength++) {
                    printf ("numr = %d cuv =%d raw = %d ",  radFile.rawFlashWaveLength [flash].flashNum,
                                                            radFile.rawFlashWaveLength[flash].cuvetteNum,
                                                            radFile.rawFlashWaveLength[flash].rawFlashReading [waveLength]);
                 }
                 printf ("\n");
            }
#endif
            memcpy (&radFile.fileTime, &fileCurrent->fileTime, sizeof (timeFlag_t));
            /* send out message to AMS */
            UploadRadFile (fdAms, radFile);
        }
        return TRUE;
    }
    return FALSE;
}


/*
 * start to tx Rad file
 */
int
TxRadFile (FT_HANDLE fdAms, timeFlag_t *timeFlag)
{
    unsigned char   reTxTime = 0;
    char            ch;
    int             ret;
    radFileList_t   *freeList;


    SendGuiMessage (MSG_AMS_STATUS, UPLOADING_RAD_FILE, 0);

    LogDebug ("change baud rate to 57600");
     /* changing baud rate to be upload speed before uploading the data */
    ChangeSerialPortBaudRate (fdAms, AMS_LOG_UUPLOAD_BAUD_RATE, AMS_PORT);
    Sleep (5000);

    /* put file name into link list */
    SearchRadFile (timeFlag);


    while (SendRadFile (fdAms) != FALSE) {
        /* set ACK state machine to wait ack response state */
         amsState.amsRxState = AMS_RX_UPLOAD_RAD_WAIT_ACK;

        // LogDebug ("Wait for ACK");
         /* wait for ACK signal */
         ret = WaitForSingleObject (hReceivedChar, ACK_TIMEOUT);
         EnterCriticalSection(&csReceivedChar);   
         ch = rxChar;
         LeaveCriticalSection(&csReceivedChar);
         amsState.amsRxState = AMS_RX_IDEL;
         /* timeout, something wrong with PC, end of tx package */
         if (ret == WAIT_TIMEOUT) {
            LogError ("PC timeout, end of tx package");
            SendErrorCommnd (fdAms, UPLOAD_RAD_TIMEOUT_ERROR);
            FreeAllList ();
            AmsTransmitMsg (fdAms, TRANSMIT_COMPLETE , NULL, 0);
            /* changing baud rate back to 19200  */
            ChangeSerialPortBaudRate (fdAms, AMS_BAUD_RATE, AMS_PORT);
            return -1;
         } else if (ch == NAK) { 
            if (reTxTime < 3) {
                reTxTime++;
                LogError ("Get PC  NAK, resend package");
            } else {
                LogError ("More than 3 times NAK, end of tx package");
                SendErrorCommnd (fdAms, UPLOAD_RAD_NAK_ERROR);
                AmsTransmitMsg (fdAms, TRANSMIT_COMPLETE , NULL, 0);
                /* changing baud rate back to 19200  */
                ChangeSerialPortBaudRate (fdAms, AMS_BAUD_RATE, AMS_PORT);
                return -1;
            }
         } else {
                //LogDebug ("Get ACK");
               /* good package, free current list and reset reTxTime */
                freeList    = fileCurrent;
                fileCurrent = fileCurrent->next;
                free (freeList);
                reTxTime = 0;
        }
    }

    LogDebug ("Send t command to pc");
    /* wait for response from AMS before change back to 19200 */
    amsState.amsRxState = AMS_RX_UPLOAD_RAD_FINISH_WAIT_ACK;

    /* send t command to AMS pc */
    AmsTransmitMsg (fdAms, TRANSMIT_COMPLETE , NULL, 0);

    LogDebug ("Wait for final ACK");
    /* wait for ACK signal */
    ret = WaitForSingleObject (hReceivedChar, ACK_TIMEOUT);

    EnterCriticalSection (&csReceivedChar);
    ch = rxChar;
    LeaveCriticalSection(&csReceivedChar);
    amsState.amsRxState = AMS_RX_IDEL;
    if (ret == WAIT_TIMEOUT) {
        LogError ("PC responses timeout");
    } else if ((ch == ACK) || (ch == NAK)) {
        if (ch == ACK) {
            LogDebug ("Finish Rad file upload %s", "ACK");     
        } else {
            LogDebug ("Finish Rad file upload %s", "NAK");     
        }
    } else {
        LogError ("Unexpect resposne from pc %x", ch); 
    }
    LogDebug ("change back to 19200");
    /* changing baud rate back to 19200  */
    ChangeSerialPortBaudRate (fdAms, AMS_BAUD_RATE, AMS_PORT);
    return 0;
}

/*
 * process tx message
 */
void
ProcessAmsTxMsg (FT_HANDLE fdAms, AmsTxMsg_t *msg)
{

#ifdef AMS_TX_DEBUG
    printf ("AmsTxComm: process command %d\n", msg->txCommand);
#endif
    switch (msg->txCommand) {

        case AMS_TX_TEMP:
            if (amsState.amsRxState == AMS_RX_TEMPERATURE) {
                TxAmsTemp (fdAms, amsState.amsTxState);
            }
        break;

        case AMS_UPLOAD_LOG_FILE:
            TxAmsLogFile (fdAms);
            //whenever AMS require upload file, log file needs to
            //renamed and backup
            LogRenameFiles ();
        break;

        case AMS_UPLOAD_RAD_FILE:
            //printf ("start upload rad file\n");
            if (TxRadFile (fdAms, (timeFlag_t *)msg->data) != 0) {
                LogError ("Upload Rad file error \n");
            }
           SendGuiMessage (MSG_AMS_STATUS, WAITING_FOR_COMMAND, 0);
        break;

        default:
        break;
    }
}

/*
 * thread to handle tx AMS package
 */
void *
AmsTxThread (void *arg)
{
    FT_HANDLE               *fdAms;
    HANDLE                  rxQueue;
    AmsTxMsg_t              msg;
    DWORD                   rxSize;
    DWORD                   flags;
    DWORD                   error;


    /* clear offset array */
    InitBlackOffset ();

    fdAms = (FT_HANDLE *)arg;


    // message queue to receive AMX tx messages
    rxQueue = CreateMessageQueue (AMS_TX_QUEUE_NAME, true);
    if (rxQueue == NULL) {
        return false;
    }
    while (1) {
        
        ReadMsgQueue (rxQueue, &msg, MAX_QUEUE_LENGTH, &rxSize, INFINITE, &flags);
        error = GetLastError ();

        if (rxSize > 0) {
#ifdef AMS_MSG_Q_DEBUG
          LogDebug ("AmsTxComm: rx msg queue = %d", msg.msgId);
#endif
            ProcessAmsTxMsg (*fdAms, &msg);
        } else {
            LogError ("AmsTxThread: rxSize = 0 Error =%d", error);
        }

    }

}


/*
 * FUNCTION
 *  AmsTransmitRawData
 *
 * DESCRIPTION
 *  Tramsmit raw data to  AMS
 *
 * PARAMETERS
 *  fdAms   --- Serial port fd
 *  msgPtr  --- pointer to dump data
 *  length  --- length to be transimtted
 *
 * RETURNS
 *  AmsError_t message status
 */

static AmsError_t
AmsTransmitRawData (FT_HANDLE fdAms, char *msgPtr, unsigned short length, unsigned char needDelay)
{
    int nByte, wByte;

    nByte = length;

    while (nByte--) {
        wByte = WriteSerPort (fdAms, msgPtr, 1);
        if (wByte != 1) {
            fprintf (stderr, "fdAms wByte = %d tx error\n",wByte);
            return TX_ERR;
        }
        if (needDelay) {
           Sleep (1);
        }
#if  0
        /* ZZZ */
        printf ("%c", *msgPtr);
#endif
        msgPtr++;

    }

    return NO_ERR;
}

/*
 * FUNCTION
 *  AmsTransmitMsg
 *
 * DESCRIPTION
 *  Tramsmit message to AMS
 *
 * PARAMETERS
 *  command --- Protocol command
 *  msgPtr  --- pointer to ASCII string payload.
 *  length  --- payload length
 *
 * RETURNS
 *  AmsError_t message status
 */

AmsError_t
AmsTransmitMsg (FT_HANDLE fdAms, char command , char *msgPtr, unsigned short length)
{
    unsigned int    txCount = 0;
    unsigned short  crc;
    int             nbyte;


#ifdef AMS_TX_DEBUG
    printf ("Ams = %d Tx AMS command 0x%x\n", fdAms, command);
#endif
    amsBuf.AmsTxMsg[txCount++] = SOH;
    amsBuf.AmsTxMsg[txCount++] = command;
    amsBuf.AmsTxMsg[txCount++] = STX;

    /* add length to tx buffer */
    if (length) {
        WordToAscii (length, &amsBuf.AmsTxMsg[txCount], FALSE);
        txCount += 4;
    }

    /* copy payload to tx buffer */
    if (msgPtr != NULL) {
        while (length--) {
            amsBuf.AmsTxMsg[txCount++] = *msgPtr++;
        }
    }
    /* add crc to tx buffer */
    if (command != ACK && command != NAK) {
        crc = MsgCrcCalculate (&amsBuf.AmsTxMsg[0], txCount);
        amsBuf.AmsTxMsg[txCount++] = HexToAscii ((char)((crc >> 4)  & 0x000f));
        amsBuf.AmsTxMsg[txCount++] = HexToAscii ((char)((crc >> 0)  & 0x000f));
        amsBuf.AmsTxMsg[txCount++] = HexToAscii ((char)((crc >> 12) & 0x000f));
        amsBuf.AmsTxMsg[txCount++] = HexToAscii ((char)((crc >> 8)  & 0x000f));
    }
    /* add end of text to tx buffer */
    amsBuf.AmsTxMsg[txCount++] = ETX;

    nbyte = WriteSerPort (fdAms, &amsBuf.AmsTxMsg[0], txCount);

    if (nbyte != txCount) {
        LogError ("need tx=%d  realtx= %d",txCount, nbyte);
        return TX_ERR;

    }

    return NO_ERR;
}

/*
 * FUNCTION
 *  DumpRawData
 *
 * DESCRIPTION
 *  Dump test results data to AMS (for simulation only)
 *
 * PARAMETERS
 *   fdAms      Serial port id
 *   fileName   dump file name
 *
 * RETURNS
 *   process status
 *
 */


AmsError_t
DumpRawData (FT_HANDLE fdAms, char *fileName, unsigned char needDelay)
{
    FILE  *fp;
    char  rBuff[200], i, *ptr;
    unsigned short len;

    fp = fopen (fileName, "rb");

    if (fp == NULL) {
        PrintMsgStatus (OPEN_FILE_FAIL);
        return OPEN_FILE_FAIL;
    }

    i = 0;
    ptr = &rBuff[0];
    while (1) {
        len = fread (ptr, 1, 1, fp);
        if (*ptr == LF) {
             *(ptr+1) ='\0';
             /* delay 5 ms */
             Sleep (1);
             ptr = &rBuff[0];
             AmsTransmitRawData (fdAms, rBuff, i+1, needDelay);
            i = 0;
        } else {
            ptr++;
            i++;
        }
        if (len ==0) {
            break;
        }
    }
    fclose (fp);

    /*
     * in the future, this function will be called after rx
     * complete data from engine card. Wliu
     */
    rBuff [0] = EOFILE;
    AmsTransmitRawData (fdAms, rBuff, 1, needDelay);

    return NO_ERR;

}


/*
 * FUNCTION
 *  DumpId
 *
 * DESCRIPTION
 *  Send out anslysis ID string and instrument serial number to AMS
 *
 * PARAMETERS
 *  fdAms   Serial port fd
 *  idIndex Id string table index
 *
 * RETURNS
 *   AmsError_t message process status
 *
 * Note: wliu
 *   This dump id need to be call before start dump the data
 */

void
DumpId (FT_HANDLE fdAms, char *idPtr, unsigned char needDelay)
{
    char idBuf[40], *ptr;
    ptr = &idBuf[0];
    *ptr++ =  '"';
    memcpy (ptr, idPtr, 10);
    ptr += 10;
    *ptr++ = '"';
    *ptr++ = ',';
    *ptr++ = '"';
    memcpy (ptr, FactoryData.instrumentSerialNumber,10);
    ptr += 10;
    *ptr++ = '"';
    *ptr++ = CR;
    *ptr++ = LF;
    *ptr++ = CR;
    *ptr++ = LF;
    AmsTransmitRawData (fdAms, idBuf, ID_LINE_LENGTH, needDelay);
}


/*
 * add CR and LF into tx buffer
 */
char *
AppenEol (char *ptr)
{
    *ptr++ = CR;
    *ptr++ = LF;
    return ptr;
}

/*
 * convert float to ascii, format the ascii to tx buffer
 */
char *
FormatFloatTxLine (float *dataPtr, char *txPtr, unsigned char length)
{
    unsigned char i;

    for (i = 0; i < length; i++) {
        /* convert float to ascii */
        /* ZZZ */
        txPtr = FloatToAscii (*dataPtr, txPtr, false);
        *txPtr++ = ',';
        dataPtr++;
    }
    /* take out last ',' */
    txPtr--;
    return (AppenEol (txPtr));
}

/*
 * fromat data into tx buffer
 * length --- how many data per line
 */
char *
Format16BitTxLine (unsigned short *dataPtr, char *txPtr, unsigned char length)
{
    unsigned char i;

    for (i = 0; i < length; i++) {
        /* convert data to ascii and put into tx buffer */
        txPtr = WordToAscii (*dataPtr, txPtr, false);
        *txPtr++ = ',';
        dataPtr++;
    }

    /* take out last ',' */
    txPtr--;
    return (AppenEol (txPtr));

}

/*
 * format 8bit data into tx buffer and add CR LF at the end of line
 * return tx next available buffer pointer
 */
char *
Format8BitTxLine (unsigned char *dataPtr, char *txPtr, unsigned char length)
{
    unsigned char i;

    for (i = 0; i < length; i++) {
         /* convert data to ascii and put into tx buffer */
        txPtr = ByteToAscii (*dataPtr, txPtr);
        *txPtr++ = ',';
        dataPtr++;

    }
    /* take out last ',' */
    txPtr--;
    return (AppenEol (txPtr));
}

/*
 *  format time (32bit) into tx buffer as ams dump format
 */
char *
FormatTimeTxLine (unsigned long time,  char *txPtr)
{
    txPtr = LongToAscii (time, txPtr,false);
    return  (AppenEol (txPtr));

}


/*
 * calculate crc check sum and add to to
 * the tx formated line
 */
unsigned short
AppenCrc (char *txPtr)
{
    unsigned short crc = 0;
    unsigned short length = 0;

    while (*txPtr != '\0') {
        crc = Crc16 (*txPtr++, crc);
        length++;
    }

    *txPtr++ = '/';
    *txPtr++ = '/';
    txPtr = WordToAscii (crc, txPtr, true);
    txPtr= AppenEol (txPtr);
    txPtr= AppenEol (txPtr);
    length += 10;

    return length;
}

/*
 * Dump sample photometric data. The format is
 * xxxxxxxx\r\n   --- Time
 * xxxx,xxxx,xxxx,xxxx\r\n
 * //CCCC
 *
 */
void
DumpPhotometricData (FT_HANDLE fdAms, unsigned char sampleType,
                     unsigned char passCount, unsigned char flashNumber,
                     unsigned char w1, unsigned char w2, unsigned char w3,
                     unsigned char needDelay)
{
    char                    txBuff [2048];
    unsigned short          dumpData [4];
    unsigned short          length;
    RawFlashWavelengths_t   *rawData;
    unsigned char           w0 = W850;
    char                    *txPtr;

    rawData = &amsData->rotorAnalyteSamples[sampleType][passCount][flashNumber];

    txPtr = txBuff;

    txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);
     /* copy required  the data */
    if (rawData->rawFlashReadings [w0] != 0) {
        dumpData [0] = rawData->rawFlashReadings [w0] - wavelengthAdcOffset [w0];
        dumpData [1] = rawData->rawFlashReadings [w1] - wavelengthAdcOffset [w1];
        dumpData [2] = rawData->rawFlashReadings [w2] - wavelengthAdcOffset [w2];
        dumpData [3] = rawData->rawFlashReadings [w3] - wavelengthAdcOffset [w3];
    } else {
        memset (dumpData, 0, sizeof (dumpData));
    }
    txPtr        = Format16BitTxLine (dumpData, txPtr, 4);
    *txPtr = '\0';
    /* add crc at the end of dump */
    txPtr = txBuff;
    length = AppenCrc (txPtr);
    /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/* dump cuvette number 29 data */
void
DumpTemperatureTestPhotometricData (FT_HANDLE fdAms, unsigned char needDelay)
{
    unsigned char flashNumber;

    for (flashNumber = 1; flashNumber < 11; flashNumber++) {
        DumpPhotometricData (fdAms, 29, 0, flashNumber, W405, W600, W405, needDelay);
    }

}



/*
 * dump Dac trim data
 */
void
DumpOpticalDacData (FT_HANDLE fdAms, unsigned char needDelay)
{
    RawFlashWavelengths_t   *rawData;
    unsigned char           flashNumber;
    char                    txBuff [2048];
    char                    *txPtr;
    bool                    engineTime;
    unsigned short          dumpData [4];
    unsigned short          length;
    unsigned char           wIndex, group;

    txPtr = txBuff;
    engineTime  = false;

    wIndex      = 0;

    for (group = 0; group < 3; group++) {
        /* skip first one */
        for (flashNumber = 1; flashNumber < NUM_OPTICAL_DACS_FLASHES + 1; flashNumber++) {
            rawData     = &amsData->opticalDacs [flashNumber];
            /* only need get first engine time */
            if (engineTime == false) {
                txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);
                engineTime = true;
            }
            /* Optical DAC dump, no offset substracted */
            if (rawData->rawFlashReadings [dacWaveLengthTable [wIndex]] != 0) {
                /* copy required  the data */
                dumpData [0] = rawData->rawFlashReadings [dacWaveLengthTable [wIndex]];
                if (group == 2) {
                    dumpData [1] = FLASH;
                } else {
                    dumpData [1] = rawData->rawFlashReadings [dacWaveLengthTable [wIndex+1]];
                }
                dumpData [2] = rawData->rawFlashReadings [dacWaveLengthTable [wIndex+2]];
                dumpData [3] = rawData->rawFlashReadings [dacWaveLengthTable [wIndex+3]];
            }else {
                memset (dumpData, 0, sizeof (dumpData));
            }
            txPtr = Format16BitTxLine (dumpData, txPtr, 4);
        }
        /* end of string */
        *txPtr = '\0';
        /* add crc at the end of dump */
        txPtr = txBuff;
        length = AppenCrc (txPtr);
        /* now ready to dump */
        AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
        wIndex += 4;
        txPtr = txBuff;
        engineTime  = false;
    }
}


/*
 * dump ndxt raw data
 * formate as
 * xxxxxxx\r\n   --- time
 * xxxx,xxxx,xxxx,xxxx\r\n  --  raw data
 *  ...                     --  20 lines
 * //CCCC\r\n                   -- crc
 * ...                  repeate  90 times
 *
 */

void
DumpNdxtRawData (FT_HANDLE fdAms, unsigned char needDelay)
{
    RawFlashWavelengths_t   *rawData;
    unsigned char           flashNumber;
    char                    txBuff [4096];
    char                    *txPtr;
    bool                    engineTime;
    unsigned short          dumpData [4];
    unsigned short          length;
    unsigned char           wIndex, cuvNum, group;


    txPtr = txBuff;
    engineTime  = false;
    wIndex = 0;

    for (group = 0; group < 3; group++) {
        for (cuvNum = 0; cuvNum < 30; cuvNum++) {
            for (flashNumber = 1; flashNumber < MAX_NDXT_SAMPLE_FLASHES + 1; flashNumber++) {
                rawData =  &amsData->ndxtSamples [cuvNum][flashNumber];
                /* only need get first engine time */
                if (engineTime == false) {
                    txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);
                    engineTime = true;
                }
                if (rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex]] != 0) {
#if 0
                    /* copy required  the data */
                    dumpData [0] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex]]    - wavelengthAdcOffset [wIndex];
                    dumpData [1] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+1]]  - wavelengthAdcOffset [wIndex+1];
                    dumpData [2] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+2]]  - wavelengthAdcOffset [wIndex+2] ;
                    dumpData [3] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+3]]  - wavelengthAdcOffset [wIndex+3];
#else
                    /* copy required  the data */
                    dumpData [0] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex]]  ;
                    dumpData [1] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+1]];
                    dumpData [2] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+2]];
                    dumpData [3] = rawData->rawFlashReadings [ndxtWaveLengthTable [wIndex+3]];
#endif
                } else {
                    memset (dumpData, 0, sizeof (dumpData));
                }
                txPtr = Format16BitTxLine (dumpData, txPtr, 4);
            }
             /* end of string */
            *txPtr = '\0';
            /* add crc at the end of dump */
            txPtr = txBuff;
            length = AppenCrc (txPtr);
            /* now ready to dump */
            AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
            txPtr = txBuff;
            engineTime  = false;
        }
        /* change wavelength index */
        wIndex += 4;
    }
}

/*
 * dump beadcheck sample data
 * bardcheck always sampe at W850, W405, W467 and W340
 * format as :
 * xxxxxxx\r\n   --- time
 * xxxx,xxxx,xxxx,xxxx\r\n  -- bead check data
 *  ...                     -- 60 lines
 * //CCCC\r\n                   -- crc
 */
void
DumpBeadCheckData (FT_HANDLE fdAms, BeadCheckType_t beadcheck,
                   unsigned char needDelay)
{
    RawFlashWavelengths_t   *rawData = NULL;
    unsigned char           flashNumber;
    unsigned char           cuvNumber;
    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          dumpData [4];
    unsigned short          length;


    txPtr       = txBuff;

    rawData = &amsData->beadCheck1[0][1];
    /* only need get first engine time */
    txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);

    for (flashNumber = 1; flashNumber < 3; flashNumber++) {
        for (cuvNumber = 0; cuvNumber < 30; cuvNumber++) {
            if (beadcheck == AMS_BEAD_CHK_1) {
                rawData = &amsData->beadCheck1[cuvNumber][flashNumber];
            } else {
                rawData = &amsData->beadCheck2[cuvNumber][flashNumber];
            }

#if 0
            if (rawData->rawFlashReadings [W850] != 0) {
                /* copy required  the data */
                dumpData [0] = rawData->rawFlashReadings [W850] - wavelengthAdcOffset [W850];
                dumpData [1] = rawData->rawFlashReadings [W405] - wavelengthAdcOffset [W405];
                dumpData [2] = rawData->rawFlashReadings [W467] - wavelengthAdcOffset [W467];
                dumpData [3] = rawData->rawFlashReadings [W340] - wavelengthAdcOffset [W340];
            } else {
                memset (dumpData, 0, sizeof (dumpData));
            }
#else
            /*
                no offset substracted from offset
                wavelength seleted as
                W850, W405, W467, W340
             */
            dumpData [0] = rawData->rawFlashReadings [W850];
            dumpData [1] = rawData->rawFlashReadings [W405];
            dumpData [2] = rawData->rawFlashReadings [W467];
            dumpData [3] = rawData->rawFlashReadings [W340];
#endif
            txPtr = Format16BitTxLine (dumpData, txPtr, 4);
        }
    }
    /* end of string */
     *txPtr = '\0';
   /* add crc at the end of dump */
     txPtr = txBuff;
     length = AppenCrc (txPtr);
     /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dump distribution sample data
 * bardcheck always sampe at W850, W405, W467 and W340
 * format as :
 * xxxxxxx\r\n   --- time
 * xxxx,xxxx,xxxx,xxxx\r\n  -- bead check data
 *  ...                     -- 300 lines
 * //CCCC\r\n                   -- crc
 */
void
DumpDistributionData (FT_HANDLE fdAms, unsigned char needDelay)
{
    RawFlashWavelengths_t   *rawData = NULL;
    unsigned char           flashNumber;
    unsigned char           cuvNumber;
    char                    txBuff [8192];
    char                    *txPtr;
    unsigned short          dumpData [4];
    unsigned short          length;

    txPtr       = txBuff;

    rawData = &amsData->distributionCheck[0][1];
    /* only need get first engine time */
    txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);


    for (flashNumber = 1; flashNumber < 11; flashNumber++) {
        for (cuvNumber = 0; cuvNumber < 30; cuvNumber++) {
            rawData = &amsData->distributionCheck[cuvNumber][flashNumber];
#if 0
            if (rawData->rawFlashReadings [W850] != 0) {
            /* copy required  the data */
                dumpData [0] = rawData->rawFlashReadings [W850] - wavelengthAdcOffset [W850];
                dumpData [1] = rawData->rawFlashReadings [W405] - wavelengthAdcOffset [W405];
                dumpData [2] = rawData->rawFlashReadings [W467] - wavelengthAdcOffset [W467];
                dumpData [3] = rawData->rawFlashReadings [W340] - wavelengthAdcOffset [W340];
            } else {
                memset (dumpData, 0, sizeof (dumpData));
            }
#else
            /*
             *  no offset substracted from offset
             *  dump wavelength
             *  W860, W405, W467, W340
             */
                dumpData [0] = rawData->rawFlashReadings [W850];
                dumpData [1] = rawData->rawFlashReadings [W405];
                dumpData [2] = rawData->rawFlashReadings [W467];
                dumpData [3] = rawData->rawFlashReadings [W340];
#endif
            txPtr = Format16BitTxLine (dumpData, txPtr, 4);

        }
    }
    /* end of string */
     *txPtr = '\0';
   /* add crc at the end of dump */
     txPtr = txBuff;
     length = AppenCrc (txPtr);
     /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dump signal cuvette dealy. format as :
 * xxxx,xxxx,...xxxx\r\n  every line has 15 datas
 * xxxx,xxxx,...xxxx\r\n
 */

void
DumpCuvDelay (FT_HANDLE fdAms, unsigned char needDelay)
{
    char                txBuff [4096];
    char                *txPtr;
    unsigned short      length;

    txPtr       = txBuff;

    /* dump global cuvette delay */
    txPtr = Format16BitTxLine (&FactoryData.globalCuvetteDelay, txPtr, 1);

    /* dump signal cuvette delay */
    txPtr = Format16BitTxLine (&FactoryData.cuvetteDelays [0], txPtr, 15);
    txPtr = Format16BitTxLine (&FactoryData.cuvetteDelays [15],txPtr, 15);
     /* end of string */
     *txPtr = '\0';
   /* add crc at the end of dump */
     txPtr = txBuff;
     length = AppenCrc (txPtr);
     /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * calculate black offset values
 */
void
SetWavelengthOffsets (void)
{
    unsigned char   waveLength;
    unsigned        flashNumber;
    unsigned long   offsetSum;

    RawFlashWavelengths_t   *rawData;

    // compute average of black offset readings for each wavelength

    for (waveLength = 0; waveLength < NUM_WAVELENGTHS; waveLength++) {
        offsetSum = 0;
        // sum raw readings for number of flashes (skip flash 0)
        for (flashNumber = 1; flashNumber <= NUM_BLACK_OFFSETS_FLASHES; flashNumber++) {
            rawData = &amsData->blackOffsets[flashNumber];
            offsetSum += rawData->rawFlashReadings [waveLength];
        }
        wavelengthAdcOffset [waveLength]  = (unsigned short)(offsetSum / NUM_BLACK_OFFSETS_FLASHES);
    }
}

/*
 * calculate and dump black offset
 * format as :
 * xxxx,xxxx, ..... xxxx \r\n  8 elements
 * ...                  totally 4 lines
 * //CCCC
 */

void
DumpBlackOffset (FT_HANDLE fdAms, unsigned char needDelay)
{
    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          length;
    unsigned short          dumpData [10];
    unsigned char           i;

    txPtr       = txBuff;

    /* first line is W850 */
    for (i = 0; i < 8; i++) {
        dumpData [i] =  wavelengthAdcOffset [W850];
    }

    txPtr = Format16BitTxLine (dumpData, txPtr, 8);

    /* for channed A, W467 is FLASH */
    dumpData [0] = wavelengthAdcOffset [W630];
    dumpData [1] = wavelengthAdcOffset [W515];
    dumpData [2] = wavelengthAdcOffset [W600];
    dumpData [3] = wavelengthAdcOffset [W550];
    dumpData [4] = wavelengthAdcOffset [W500];
    dumpData [5] = FLASH;
    dumpData [6] = wavelengthAdcOffset [W405];
    dumpData [7] = wavelengthAdcOffset [W340];

    /* channel A */
    txPtr = Format16BitTxLine (dumpData, txPtr, 8);

    /* copy W467 back */
    dumpData [5] = wavelengthAdcOffset [W467];

    /* channel B */
    txPtr = Format16BitTxLine (dumpData, txPtr, 8);

    /* channel C, W467 is WHITE */
    dumpData [5] = wavelengthAdcOffset [WHTE];

    txPtr = Format16BitTxLine (dumpData, txPtr, 8);


    /* end of string */
     *txPtr = '\0';
   /* add crc at the end of dump */
     txPtr = txBuff;
     length = AppenCrc (txPtr);
     /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);

}

/*
 * dump fbd calibration factor. Format as:
 * xxxxxxxx,xxxxxxxxx,xxxxxxxxx,xxxxxxxx,\r\n
 * ...      repeat 8 lines
 * //CCCC
 */

void
DumpBfdCalFactor (FT_HANDLE fdAms, unsigned char needDelay)
{

    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          length;
    unsigned char           i;

    txPtr       = txBuff;


    for (i = 0; i < 8; i++) {
        txPtr = FormatFloatTxLine (&FactoryData.bfdCalibrationFactors[i * 4], txPtr, 4);
    }
     /* end of string */
     *txPtr = '\0';
   /* add crc at the end of dump */
     txPtr = txBuff;
     length = AppenCrc (txPtr);
     /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dump air temperature , format as:
 * xxxxxxxx\r\n
 * ...   totally 32 lines
 * //CCCC
 *
 */

void
DumpAirTemperature (FT_HANDLE fdAms, unsigned char needDelay)
{
    RawTemperatures_t       *rawData = NULL;
    unsigned char i;
    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          length;
    float                   rotorTemperature;

    txPtr       = txBuff;


    /* Get Initial and BeakChk1 temperature */
    for (i = 0; i < 2; i++) {
       rawData = &amsData->temperatures[i];
       /* calculate temperature */
		  rotorTemperature = (float)((float)rawData->rotorTemperature *
                            ROTOR_TEMPERATURE_CONVERT_MULT) +
                            FactoryData.airTemperatureOffset;
        txPtr = FormatFloatTxLine (&rotorTemperature, txPtr, 1);
    }
    /* skip BeakChk2 temperature */
    rotorTemperature = 0;
    txPtr = FormatFloatTxLine (&rotorTemperature, txPtr, 1);

    /* dump rest of 28 temperatures */
    for (i = 2; i < 31; i++) {
       rawData = &amsData->temperatures[i];
       /* calculate temperature */
       rotorTemperature = (float)((float)rawData->rotorTemperature *
                            ROTOR_TEMPERATURE_CONVERT_MULT) +
                            FactoryData.airTemperatureOffset;
        txPtr = FormatFloatTxLine (&rotorTemperature, txPtr, 1);
    }

   /* end of string */
    *txPtr = '\0';
   /* add crc at the end of dump */
    txPtr = txBuff;
    length = AppenCrc (txPtr);
   /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dum cuvette delay value
 * format as
 * xxxxxxxx             --Engine time
 * xxxx,xxxx,xxxx,xxxx  --dump data
 * ...                  totally 20 lines
 * //CCCC
 */

void
DumpGlobalCuvDelay (FT_HANDLE fdAms, unsigned char needDelay)
{
    RawFlashWavelengths_t   *rawData = NULL;
    unsigned char           flashNumber;
    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          length;
    unsigned short          dumpData [4];
    unsigned char           delay;
    bool                    engineTime;

    /* dump global cuvette delay data */
    for (delay = 0; delay < 75; delay++) {
        txPtr       = txBuff;
        engineTime  = false;
        for (flashNumber = 1; flashNumber < 21; flashNumber++) {
            rawData = &amsData->cuvetteDealy [delay][flashNumber];
            if (engineTime == false) {
                txPtr = FormatTimeTxLine (rawData->engineTime, txPtr);
                engineTime = true;
            }
            /* copy required  the data */
            if (rawData->rawFlashReadings [W850] != 0) {
                dumpData [0] = rawData->rawFlashReadings [W850] - wavelengthAdcOffset [850];
                dumpData [1] = rawData->rawFlashReadings [W340] - wavelengthAdcOffset [340];
                dumpData [2] = rawData->rawFlashReadings [W340] - wavelengthAdcOffset [340];
                dumpData [3] = rawData->rawFlashReadings [W340] - wavelengthAdcOffset [340];
            } else {
                memset (dumpData, 0, sizeof (dumpData));
            }
            txPtr = Format16BitTxLine (dumpData, txPtr, 4);
        }
        /* end of string */
        *txPtr = '\0';
        /* add crc at the end of dump */
        txPtr = txBuff;
        length = AppenCrc (txPtr);
        /* now ready to dump */
        AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
    }
}

/*
 * dump calibration trim DAC value
 * format as :
 * xx,xx,xx,xx,xx,xx,xx,xx,xx,xx\r\n
 */

void
DumpTrimDacValue (FT_HANDLE fdAms, unsigned char needDelay)
{

    char                    txBuff [2048];
    char                    *txPtr;
    unsigned short          length;
    unsigned char           dumpData [10];
    unsigned char           i;

    txPtr       = txBuff;

    for (i =0; i < 10; i++) {
        dumpData [i] = CalibrationData.wavelengthDacTrims [wavelengthDacTable [i]];
    }

    txPtr = Format8BitTxLine (&dumpData [0], txPtr, 10);
    *txPtr = '\0';
    length = strlen (txBuff);

    /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * just format the requirement
 */
void
DumpErrorFlag (FT_HANDLE fdAms, unsigned char needDelay)
{
    unsigned short      sFlag = 0;
    unsigned long       lFlag = 0;
    unsigned short      length;
    char                txBuff [2048];
    char                *txPtr, i;

    txPtr       = txBuff;

    for (i = 0; i < 2; i++) {
        txPtr = WordToAscii (sFlag, txPtr, false);
        *txPtr++ = ',';
    }
    for (i = 0; i < 5; i++) {
        txPtr = LongToAscii (lFlag, txPtr, false);
        *txPtr++ = ',';
    }
    for (i =0; i < 3; i++) {
        txPtr = WordToAscii (sFlag, txPtr, false);
        *txPtr++ = ',';
    }

    txPtr--;
    txPtr = AppenEol (txPtr);

    /* end of string */
    *txPtr = '\0';
    /* add crc at the end of dump */
    txPtr = txBuff;
    length = AppenCrc (txPtr);
    /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/* temp simulate the barcode dump here */
void
DumpBarcode (FT_HANDLE fdAms, char dumpBarcode, unsigned char needDelay)
{
    char                txBuff [4096];
    char                *txPtr;
    unsigned char       i;
    unsigned short      length;
    unsigned short      barcodeData = 0;
    char                *barStr = "The Barcode was not read during this run                                                                      ";
    char                *ptr;

    txPtr       = txBuff;

    /* dump barcode here in the future */
    if (dumpBarcode) {
    }

    /* dump barcode factors */
    for (i =0; i < 10; i++) {
        txPtr = WordToAscii (barcodeData, txPtr, false);
        *txPtr++ = ',';
    }
    txPtr--;
    txPtr = AppenEol (txPtr);
    /* copy barcode string */
    length = strlen (barStr);
    ptr    = barStr;
    while (length--) {
       *txPtr++ = *ptr++;
    }
    txPtr = AppenEol (txPtr);
    /* end of string */
    *txPtr = '\0';
     /* add crc at the end of dump */
    txPtr = txBuff;
    length = AppenCrc (txPtr);
    /* now ready to dump */
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dump software version number
 */

void
DumpVersionNum (FT_HANDLE fdAms, unsigned char needDelay)
{
    char                txBuff [2048];
    char                *txPtr;
    unsigned char       i;
    unsigned short      length;

    txPtr       = txBuff;

    *txPtr++ = '"';
    /* dump engine sw version  */
    for (i =0; i < 8; i++) {
        *txPtr++ =  ENG_SW_VER [i];
    }
    *txPtr++ = '"';
    txPtr = AppenEol (txPtr);

    /* dump sonsole sw version */
    *txPtr++ = '"';

    for (i = 0; i < 17; i++) {
        *txPtr++ = CONSOL_SW_VER [i];
    }
    *txPtr++ = '"';
    txPtr = AppenEol (txPtr);
    txPtr = AppenEol (txPtr);


    *txPtr = '\0';
    length = strlen (txBuff);

    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/* send out end of dump file to AMS */
void
DumpEndFile (FT_HANDLE fdAms, unsigned char needDelay)
{
    char       txBuff [10];

    txBuff [0] = EOFILE;
    AmsTransmitRawData (fdAms, txBuff, 1, needDelay);
}

/*
 * dump temperature test results to Ams
 */

void
DumpTemperatureTestData (FT_HANDLE fdAms, unsigned char needDelay)
{

    /* dump system Id*/
//   printf ("Dump Id\n");
    DumpId (fdAms, sysName, false);
   /* dump trim dac */
//   printf ("Dump trim dac \n");
    DumpOpticalDacData (fdAms, false);
    /* dump sample data */
//    printf ("Dump Sample data \n");
    DumpTemperatureTestPhotometricData (fdAms, false);
    /* dump bead check 1 data */
//    printf ("Dump bead chk 1 data \n");
    DumpBeadCheckData (fdAms, AMS_BEAD_CHK_1, true);
    /* dump bead check 2 data */
//    printf ("Dump bead chk 2 data \n");
    DumpBeadCheckData (fdAms, AMS_BEAD_CHK_2, true);
    /* dump distribution data */
//    printf ("Dump distribution data \n");
    DumpDistributionData (fdAms, true);
    /* dump calibration trim dacs */
//    printf ("Dump calibration trim dac data \n");
    DumpTrimDacValue (fdAms, false);
    /* dump black offset */
//    printf ("dump black offset \n");
    DumpBlackOffset (fdAms, true);
    /* dump cuvette delay */
//    printf ("dump cuvette delay \n");
    DumpCuvDelay (fdAms, false);
    /* dump bfd */
//    printf ("dump bfd \n");
    DumpBfdCalFactor (fdAms, false);
    /* dump temperature */
//    printf ("dump air temperature\n");
    DumpAirTemperature (fdAms, true);
    /* dump error flag */
//    printf ("dump air flag\n");
    DumpErrorFlag (fdAms, false);
//    printf ("dump barcode\n");
    DumpBarCode (fdAms, NULL, NULL, 0, false, true);
//    printf ("dump engine sw version\n");
    DumpVersionNum (fdAms, false);
    /* need insert dump barcode here ZZZ */
//    printf ("dump end of file\n");
    DumpEndFile (fdAms, false);
    /* wait 35 second before open the drawer */
    Sleep (4500);
}

/*
 * dump cuvette delay test results to AMS
 */
void
DumpGlobalCuvDelayData (FT_HANDLE fdAms, unsigned char needDelay)
{
    /* dump id */
    DumpId (fdAms, sysName, false);
    /* dump raw data */
    DumpGlobalCuvDelay (fdAms, false);
    DumpEndFile (fdAms, false);
}

/*
 * dump  ndxt test results data to AMS
 */
void
DumpNdxtData (FT_HANDLE fdAms, unsigned char needDelay)
{
//    printf ("Dump Id\n");
    DumpId (fdAms, sysName, false);
    /* dump trim dac */
//    printf ("Dump trim dac \n");
    DumpOpticalDacData (fdAms, false);
    /* dump ndxt sampe data */
//    printf ("dump ndxt raw data\n");
    DumpNdxtRawData (fdAms, false);
    /* dump bead check 1 data */
//    printf ("Dump bead chk 1 data \n");
    DumpBeadCheckData (fdAms, AMS_BEAD_CHK_1, false);
    /* dump bead check 2 data */
//    printf ("Dump bead chk 2 data \n");
    DumpBeadCheckData (fdAms, AMS_BEAD_CHK_2, false);
    /* dump distribution data */
//    printf ("Dump distribution data \n");
    DumpDistributionData (fdAms, false);
    /* dump calibration trim dacs */
//    printf ("Dump calibration trim dac data \n");
    DumpTrimDacValue (fdAms, false);
    /* dump black offset */
//    printf ("dump black offset \n");
    DumpBlackOffset (fdAms, false);
    /* dump cuvette delay */
//    printf ("dump cuvette delay \n");
    DumpCuvDelay (fdAms, false);
    /* dump bfd */
//    printf ("dump bfd \n");
    DumpBfdCalFactor (fdAms, false);
    /* dump temperature */
//    printf ("dump air temperature\n");
    DumpAirTemperature (fdAms, false);
    /* dump error flag */
//    printf ("dump air flag\n");
    DumpErrorFlag (fdAms, false);
//    printf ("dump barcode\n");
    DumpBarCode (fdAms, NULL, NULL, 0, false, false);
//    printf ("dump engine sw version\n");
    DumpVersionNum (fdAms, false);
    /* need insert dump barcode here ZZZ */
//    printf ("dump end of file\n");
    DumpEndFile (fdAms, false);
}

char  *barCodeStr []  = {
                         "The barcode on this rotor is unreadable                                                                       ",
                         "The Barcode was not read during this run                                                                      ",

                        };


#if 0
unsigned short barcodeFactor [] =
               {
                    0xE67B,0x9FCF,0xD0D4,0x054C,0x0A99,
                    0xCB88,0x000F,0x0000,0x0000,0x0000
                };
// ZZZ for debug only
unsigned short barcodeFactor [] =
               {
                    0xE67B,0x9FCF,0x003A,0x054C,0x0A99,
                    0xCB88,0x000F,0x0000,0x0000,0x0000
                };
#endif

/*
 * dump barcode string to AMS
 */
void
DumpBarCode (FT_HANDLE fdAms, char *barcodeDigits, unsigned short *barcodeFactor,int errNum, char dumpBarcode,
             unsigned char needDelay)
{
    char                    txBuff [2048];
    char                    *txPtr;
    unsigned short          length;
    unsigned short          dumpData [20];
    unsigned char           i;

    txPtr       = txBuff;

    memset (dumpData, 0, 20);

    // copy barcode  parameter 
    if ((dumpBarcode) && (barcodeFactor != NULL)) {
        /* do barcode simulation */
        for (i =0; i < 8; i++) {
            dumpData [i] = barcodeFactor [i];
        }
    }

    txPtr = Format16BitTxLine (&dumpData [0], txPtr, 10);

    if (dumpBarcode) {
        if ((errNum == ERR_BC_NO_START) || (errNum == ERR_BC_NO_START)   ||
            (errNum == ERR_BC_NO_STOP)  || (errNum == ERR_BC_BAD_DECODE) ||
            (errNum == ERR_BC_BAD_CHECK)|| (errNum == ERR_BC_BAD_CRC)) {
            strcpy (txPtr , barCodeStr[0]);
            txPtr += strlen (barCodeStr [0]);
        } else {
            for (i = 0; i < NUM_BARCODE_DIGITS; i++) {
                *txPtr++ = HexToAscii (*barcodeDigits++);
            }
        }
    } else {
        strcpy (txPtr, barCodeStr [1]);
        txPtr += strlen (barCodeStr [1]);
    }

    txPtr = AppenEol (txPtr);
    *txPtr = '\0';
    /* add crc at the end of dump */
    txPtr = txBuff;
    length = AppenCrc (txPtr);
    AmsTransmitRawData (fdAms, &txBuff[0], length, needDelay);
}

/*
 * dump motor test position data to AMS
 */

void
DumpMotorData (FT_HANDLE fdAms, unsigned char needDelay)
{

    char                    txBuff [4096];
    char                    *txPtr;
    unsigned short          i;

    txPtr   = txBuff;

//    printf ("Dump Motor position data\n");

#if 0
#ifdef MOTOR_48_TEST

    for (i = 0; i < 39000; i++) {
        Format16BitTxLine (&amsData->motorTestData[i].position, txPtr, 1);
        AmsTransmitRawData (fdAms, &txBuff[0], 6, needDelay);
    }
#else
    for (i = 0; i < 8665; i++) {
        Format16BitTxLine (&amsData->motorTestData[i].position, txPtr, 1);
        AmsTransmitRawData (fdAms, &txBuff[0], 6, needDelay);
    }
#endif
#else

   for (i = 0; i < MOTOR_DUMP_DATA_LEN; i++) {
        Format16BitTxLine (&amsData->motorTestData[i].position, txPtr, 1);
        AmsTransmitRawData (fdAms, &txBuff[0], 6, needDelay);
    }

#endif
//    printf ("Dump End of fie\n");

    DumpEndFile (fdAms, false);

}
