#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "AmsData.h"
#include "BarcodeData.h"
#include "Serial.h"
#include "Temperature.h"
#include "State.h"
#include "CalData.h"
#include "ftd2xx.h"
#include "AmsComm.h"
#include "XmlData.h"
#include "PingTest.h"


/* sw version */
#define ENG_SW_VER      "   1.01 "
#define CONSOL_SW_VER   "   01.001.1.01      "  /* temp put here */

#define FALSE       0
#define TRUE        1

#define false       0
#define true        1

#define          START_ANALYSIS_PROCESS

//typedef unsigned char bool;

typedef enum
{
    START_NONE,         /* nothing start */
    START_AMS,          /* start ams process */
    START_ANALYZER,     /* start analysis process */
    START_ENGINE_TEST, /* start engine board test process */
} StartProcess_t;

typedef enum
{
    AMS_PROCESS,
    DRAWER_PROCESS,
    ENGCOMM_PROCESS,
    HEATERTEST_PROCESS,
    ERRORLOG_PROCESS,
} ProcessName_t;

typedef struct 
{
    unsigned long   totalLen;
    unsigned long   cTotalLen;
    unsigned long   oTotalLen;
    unsigned long   cOffset;
    unsigned long   oOffset;
} LogFileParameters_t;

extern LogFileParameters_t logFileParameters;

extern HANDLE               AmsEngineCommQueue;

extern CRITICAL_SECTION     csReceivedChar;
extern HANDLE               hReceivedChar;

extern AmsData_t            *amsData;
extern BarcodeData_t        *barCodeData;

extern unsigned short
UpdateCrc16 (unsigned short crc, char c);

extern unsigned short
AsciiToWord (char *ascii, char swapByte);

extern char
HexToAscii(unsigned char hex);

extern unsigned char
AsciiToHex (char ascii);

extern AmsError_t
MsgCrcCheck (char *msg);

extern unsigned short
MsgCrcCalculate (char *msg, unsigned short length);

extern AmsError_t
ProcessAmsRxMsg (FT_HANDLE fdAms, char *msgPtr);

AmsError_t
AmsTransmitMsg (FT_HANDLE fdAms, char command , char *msgPtr, unsigned short length);

extern void
PrintMsgStatus (AmsError_t msgStatus);

extern AmsError_t
WriteData (char *fileName, char *data);

extern AmsError_t
WriteCalibration (CalibrationData_t *data);

extern AmsError_t
ParseFactoryXml (FactoryData_t * factoryData);

extern AmsError_t
ParseCalibration (CalibrationData_t  * calibrationData);

extern AmsError_t
DumpRotorDelayRawData (FT_HANDLE fdAms, char *fileName);

extern char *
ByteToAscii (char byte, char *asciiPtr);

extern char *
FloatToAscii (float genFloat, char *asciiPtr, char byteSwap);

extern unsigned char
AsciiToByte (char *ascii, ConvertType_t type);

extern float
AsciiToFloat (char *asciiPtr, char swapByte);

extern char *
WordToAscii (unsigned short word, char *asciiPtr, char swapByte);

extern char *
LongToAscii (unsigned int genInt, char *asciiPtr, char byteSwap);

extern AmsError_t
MsgCheck (FT_HANDLE fdAms, char *msgPtr);

extern DWORD 
StartProcess(const char * processName);

extern void
RegisterEngineCommQueue (HANDLE engineCommQueue, char *queuName);


extern HANDLE 
CreateMessageQueue(const WCHAR * queueName, BOOL read);

extern void
SendDrawerMessage(char command);

extern bool
CreateAmsEngineQueue (void);

extern void
CloseAmsEngineQueue (void);

extern int
InitSerialPort (unsigned short portNum, FT_HANDLE *fdAms);


extern void
ProcessEngRxMsg (FT_HANDLE fdAms, char *msgPtr, int length);

extern void
SendHeaterCommand (char command);

extern void
SendFanIdleSpeed (char idleSpeed);

extern bool
CreateAmsTxQueue (void);

extern void
CloseAmsTxQueue (void);

extern bool
CreatAmsQueue (void);

void
CloseAmsQueue (void);

extern bool
CreatLogQueue (void);

extern void
CloseLogQueue (void);

extern void
SendMsgToAmsTxThread(AmsTxCommad_t command,void *data, unsigned short length);

extern void *
AmsTxThread (void *arg);

extern void *
AmsRxThread (void *arg);

extern AmsError_t
DumpRawData (FT_HANDLE fdAms, char *fileName, unsigned char needDelay);

extern void
DumpId (FT_HANDLE fdAms, char *idPtr, unsigned char needDelay);

extern void *
CreateSharedMem(const char * sharedMemName, DWORD size);

extern unsigned short
AmsCrcCalculate (char *msg, unsigned short length, unsigned short crc);

extern unsigned short
Crc16 (char msg, unsigned short crc);

extern char *
AppenEol (char *ptr);

extern char *
Format16BitTxLine (unsigned short *dataPtr, char *txPtr, unsigned char length);

extern char *
FormatTimeTxLine (unsigned long time,  char *txPtr);

extern unsigned short
AppenCrc (char *txPtr);

extern void
DumpOpticalDacData (FT_HANDLE fdAms, unsigned char needDelay);

extern void
DumpTemperatureTestData (FT_HANDLE fdAms, unsigned char needDelay);

extern bool
SendRisFile (char *risFileName);

extern void
SendAnalysisCommand (char command);

extern AmsError_t
ProcessConsoleRxMsg (char *msgPtr, int length);

extern void
DumpAirTemperature (FT_HANDLE fdAms, unsigned char needDelay);

extern void
DumpEndFile (FT_HANDLE fdAms, unsigned char needDelay);

extern void
DumpGlobalCuvDelayData (FT_HANDLE fdAms, unsigned char needDelay);

extern void
CalcBfds (void);

extern void
SendCalibrationFactors (void);

extern void
SetDacTrims (void);

extern void
SendFactoryDatas (void);

extern void
DumpNdxtData (FT_HANDLE fdAms, unsigned char needDelay);

extern void
SetWavelengthOffsets (void);

int
ProcessRotorBarcode (char *msgPtr,unsigned short *barcodeFactor,  BarcodeData_t * barcodeData);


extern void
DumpBarCode (FT_HANDLE fdAms, char *barcodeDigits, unsigned short *barcodeFactor, int errNum, char dumpBarcode, unsigned char needDelay);

extern void
SendBarCodeCommand (char command);

extern void
SendBarCodeAlignCommand (char command);

extern void
SendBarCodeGainCommand (char command);

extern void
DumpMotorData (FT_HANDLE  fdAms, unsigned char needDelay);

extern void
TxBarcodeGain (FT_HANDLE  fdAms, unsigned short *adcValue);

#if 0
extern void
SendMsgToHeaterThread (HeaterMsgId_t command, RawTemperature_t *rawTemp);

extern void *
HeaterControllerThread (void *arg);

extern void *
TimerThread (void * arg);

extern bool
InitHeaterControler (void);
#endif

extern void
FanControl (bool fanOn);

extern void
StartupAllProcess (void);

extern void
KillAllProcess (void);

extern void
SendGuiMessage (int msgId, int amsStatus, unsigned short errorCode);

extern bool
CreateAmsGuiQueue (void);

extern void
CloseAmsGuiQueue (void);

#if 0
extern bool
CreateHeateQueue (void);

extern void
CloseHeateQueue (void);
#endif


extern void
SendRxDataMsgToMainThread (AmsAnalysisCmd_t command);

extern void *
TouchScreenThread (void *arg);

extern void
SlayProcess (ProcessName_t processName);

extern void
ReStartProcess (ProcessName_t processName);

extern void
TouchScreenCal (void);

extern void *
UsbToEthernetThread (void *arg);

extern void
SoundChipTest (void);

extern void
KeyTone(void);

extern void
SetSoundChipActive (void);

extern unsigned char
SetSoundChipStandby (void);

extern void
InitSoundChip (void);

extern void *
interrupt_thread (void *data);

extern int
OpenSoundFile (char *fileName);

extern void
WriteSoundChipReg (unsigned char  reg, unsigned char  value);

extern unsigned char
ReadSoundChipReg (unsigned char reg);

extern int
ReadSoundFile (char *buf, unsigned int len);


extern void
StartPlayADPCM (void);

extern void
StartPlayADPCM (void);

extern void
StopPlayADPCM (void);

extern void
SartPlayMIDI (void);

extern void
StopPlayMIDI (void);

extern void
LoadADPCMSoundData (void);

extern void
LoadMIDISoundData (void);

extern void *
SoundControlThread (void *arg);

extern void
InitKeyTone (void);

extern void
SetADPCMVolume (unsigned char volume);

extern void
CalcCrc16( unsigned char c, unsigned short * runningCrc );

extern unsigned char
ReadNvRamStr (char *str);

extern unsigned char
WriteNvRamStr (unsigned char startAddress ,char *str, unsigned char  len);

extern unsigned char
InitNvRamValue (unsigned char startAddress);

extern void
HeaterTest (void);

extern void
CamDrawerTest (void);

extern AmsError_t
ProcessUploadLogFile (FT_HANDLE fdAms, char *msgPtr);

extern AmsError_t
ProcessAck (FT_HANDLE fdAms);

extern  AmsError_t
ProcessNak (FT_HANDLE  fdAms);

extern int 
ChangeSerialPortBaudRate (FT_HANDLE fdAms, int BAUD_RATE, unsigned short port);

extern void
TxBarcodeAlign (FT_HANDLE fdAms, char *barNumber);

void
SendErrorCommnd (FT_HANDLE  fdAms, unsigned short  errorNumber);

extern void
ProcessDateTime (FT_HANDLE  fdAms);

extern int
TxRadFile (FT_HANDLE  fdAms, timeFlag_t *timeFlag);

extern void
ProcessUpLoadRadFile (FT_HANDLE  fdAms, char *msgPtr);

extern int
InitialEngineBoardInterface (void);

extern void
StartupLogErrorProcess (void);

extern int
WriteSerPort (FT_HANDLE fdSer, void *buf, int nbyte);

extern int
ReadSerPort (FT_HANDLE fdSer, void *buf, int nbyte);

extern int
ReadSerPortCond (FT_HANDLE fdSer, void *buf, int nbyte, int timeout);

extern int
InitialAmsSerialPort (FT_HANDLE *fdAms);

extern void
TxCuvetteDetection (FT_HANDLE fdAms,  unsigned short *cuvInxWidth);

extern void
SendCuvetteDetectionCommand (char command);
 
extern void
DeleteAllShareMemory (void);

extern void
ExitAms (StartProcess_t startProcess);

extern AmsError_t
ProcessEthernetPortTest (FT_HANDLE fdAms);

extern void
SetEthernet (netSettings_t netSettings);
#endif
