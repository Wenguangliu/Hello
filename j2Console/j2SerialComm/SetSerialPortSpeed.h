#ifndef SET_SERIAL_PORT_SPEED_H_
#define SET_SERIAL_PORT_SPEED_H_

#ifndef BAUD_RATE_NORMAL
#define BAUD_RATE_ASCII_DUMP    9600
#define BAUD_RATE_NORMAL        19200
#define BAUD_RATE_RAD_XFER      57600
#endif
#define  BAUD_RATE_INVALID      0xFFFF 

#ifdef __cplusplus
extern "C" {
#endif

#define SER_TX_TIMEOUT    10000  
#define SER_RX_TIMEOUT    1000  

extern int
InitialSerialPort (void);

extern int
SetSerialPortSpeed (unsigned int speed);

extern int
WriteSerPort (void *buf, int nbyte);

extern int 
ReadSerPort (void *buf, int nbyte, unsigned long *status);

extern void
RecoverPrevSpeed (void);

extern int
ReadSerPortCond (void *buf, int n, int min, int time, int timeout);

extern void
SetFlowControl(unsigned int flow);

extern void
RecoverPrevFlow (void);

extern int
SetTimeout (unsigned int rxTimeout, unsigned int txTimeout);

extern void
SerialPortClose (void);

#ifdef __cplusplus
}
#endif

#endif

