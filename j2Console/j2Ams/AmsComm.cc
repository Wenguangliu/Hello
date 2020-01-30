#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include "FtdiConfig.h"
#include "Ftd2xx.h"
#include "LogErr.h"
#include "Global.h"

/*
 * write serial port
 */
int
WriteSerPort (FT_HANDLE fdSer, void *buf, int nbyte)
{
    int  writenByte = 0;

    writenByte = SendMsgToSerialPort (fdSer, (unsigned char*)buf, nbyte);

    if (writenByte != nbyte) {
        LogError ("AmsComm: Error write errno=%d", GetLastError ());

    }
    return writenByte;
}

/*
 * read serial port
 */
int
ReadSerPort (FT_HANDLE fdSer, void *buf, int nbyte)
{
    int readByte = 0;

    do {
        if (fdSer != NULL) {
          readByte = ReceiveMsgFromSerialPort (fdSer, (unsigned char*)buf, nbyte, 0);
          if (readByte <1 ) {
            LogError ("Ams Comm Read Serial Port Fail"); 
            return readByte;
          }
        }
    } while (readByte != nbyte);

    if (readByte != nbyte) {
        LogError ("AmsComm: Error read Serial Port");
    }

    return readByte;
}


/*
 * read serial port with timeout. 
 * Timeout unit is  ms 
 * return  0 --- timeout
 * none zero --- how many bytes to be read
 */
int
ReadSerPortCond (FT_HANDLE fdSer, void *buf, int nbyte, int timeout)
{
  return (ReceiveMsgFromSerialPort (fdSer, (unsigned char*)buf, nbyte, timeout*100));
}

/* initial AMS serial port */
int
InitialAmsSerialPort (FT_HANDLE *fdAms) 
{
    return (InitSerialPort (AMS_PORT, fdAms));
}


