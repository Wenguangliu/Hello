// PAK: this is now a platform specific module
#ifdef WINCE
#
#include <windows.h>
#include <commctrl.h>
//#include <sys/unistd.h>
#include <FtdiConfig.h>
#include <errno.h>

#include "LogErr.h"
#include "SetSerialPortSpeed.h"

static const char 	*port = "FTDI Port 2";
static int			portnum = 2; 
static FT_HANDLE    fdSer = NULL;
static unsigned int PREV_SPEED, CURR_SPEED;
static unsigned int	PREV_FLOW, CURR_FLOW;


/*
 * initial serial port, start process  and open the port
 * start at 19200 baud rate
 */
int
InitialSerialPort (void)
{
    FT_STATUS   status;
	fdSer = OpenSerialPort (portnum);

	if (fdSer == INVALID_HANDLE_VALUE) {
		LogError( "failed to open %s", port );
		return -1;
	}
    status = FT_ResetDevice (fdSer);
    if (status != FT_OK) {
        LogError ("Fail to reset FTDI device");
    }


    PREV_SPEED = BAUD_RATE_NORMAL;  
    CURR_SPEED = BAUD_RATE_NORMAL;
	PREV_FLOW = FT_FLOW_NONE;
	CURR_FLOW = FT_FLOW_NONE;

    SetSerialPortSpeed (BAUD_RATE_NORMAL);
    
    SetFlowControl (CURR_FLOW);

    return 0;
}

/*
 * close serial port
 */
void
SerialPortClose (void)
{

    if (fdSer != INVALID_HANDLE_VALUE) {
        LogDebug ("Close serial port");
        CloseSerialPort (fdSer);
    }
}

/*
 * set serial port timeout
 */
int
SetTimeout (unsigned int rxTimeout, unsigned int txTimeout)
{
     if (FT_SetTimeouts (fdSer, rxTimeout, txTimeout) != FT_OK) {
        LogError ("Fail to set timeout");
        return -1;
    }
    
    return 0;

}

/*
 * changing serial port speed setting
 */

int
SetSerialPortSpeed (unsigned int speed)
{
	int rc;
	// PAK: should fail here
	if (fdSer == NULL) {
		LogError("%s not open", port);
		return(-1);
	}
	
    switch (speed) {
        case BAUD_RATE_NORMAL:
			rc = SetSerialPortBaudRate (fdSer, FT_BAUD_19200, portnum);
            LogInfo ("Start 19200 Baud Rate");
            if (rc == -1) {
                return -1;
            }
        break;

        case BAUD_RATE_ASCII_DUMP:
			rc = SetSerialPortBaudRate (fdSer, FT_BAUD_9600, portnum);
            LogInfo ("Start 9600 Baud Rate");
            if (rc == -1) {
                return -1;
            }
        break;

        case BAUD_RATE_RAD_XFER:
			rc = SetSerialPortBaudRate (fdSer, FT_BAUD_57600, portnum);
            LogInfo ("Start 57600 Baud Rate");
            if (rc == -1) {
                return -1;
            }
        break;

        default:
             /*wrong baud rate, change to be default 19200 baud rate */
			rc = SetSerialPortBaudRate (fdSer, FT_BAUD_19200, portnum);
            LogInfo ("Start default 19200 Baud Rate");
            if (rc == -1) {
                return -1;
            }
            LogError ("Wrong Baud Rate Setting %d", speed);
        break;
    }

    PREV_SPEED = CURR_SPEED; 
    CURR_SPEED = speed;

	return 0;
}

/*
 * write serial port
 */
int
WriteSerPort (void *buf, int nbyte)
{
    unsigned long   writenByte = 0;
    static int      setTimeout = 0;
    FT_STATUS       status;

    if (fdSer != INVALID_HANDLE_VALUE) {
        if (setTimeout ==0) {
            SetTimeout (SER_RX_TIMEOUT, SER_TX_TIMEOUT);
            setTimeout =1;
        }
        status = FT_Write (fdSer, (unsigned char*)buf, nbyte, &writenByte);
        if (status != FT_OK) {
            LogError ("Error write FtStatus = %x", status);
        }
    }
    return writenByte;
}

/*
 * read serial port
 */
int
ReadSerPort (void *buf, int nbyte, unsigned long  *status)
{
    unsigned long   readByte = 0;
    static int      setTimeout = 0;
  

    if (fdSer != INVALID_HANDLE_VALUE) {
        if (setTimeout ==0 ) {
            SetTimeout (SER_RX_TIMEOUT, SER_TX_TIMEOUT);
            setTimeout = 1; 
        }
        *status =  FT_Read (fdSer, (unsigned char*)buf, nbyte, &readByte);
        if (*status != FT_OK) {
            LogError ("Error read FtStatus = %x", *status);
        }
    } else {
        *status = FT_INVALID_HANDLE;
        LogError ("Invalid FTDI serial port handle");
    }

    return readByte;
}

int
ReadSerPortCond (void *buf, int n, int min, int time, int timeout)
{
    
    return (ReceiveMsgFromSerialPort (fdSer, (unsigned char*)buf, n, timeout*100));
}

/*
 * recover previous speed
 */
void
RecoverPrevSpeed (void)
{
    /* wait 4 seconds for finishing sending buffer */
    Sleep (4000/*4*/);
    SetSerialPortSpeed (PREV_SPEED);
}

/*
 * enable/disable FTDI RTS/CTS flow control
 */
void
SetFlowControl (unsigned int flow)
{
	FT_STATUS ret;
	PREV_FLOW = CURR_FLOW; 
	CURR_FLOW = flow;

	ret = FT_SetFlowControl (fdSer, CURR_FLOW, 0, 0);
	if (ret != FT_OK) {
		LogError("ASTM: Failed to set flow control %d", ret);
		return;
	} 
#if 0
    else {
        if (CURR_FLOW == FT_FLOW_RTS_CTS) {
            LogDebug ("Turn RTS/CTS on");
        } else {
            LogDebug ("Turn RTS/CTS off");
        }
    }
#endif

}

void
RecoverPrevFlow (void)
{
	SetFlowControl (PREV_FLOW);
}

#endif // WINCE
