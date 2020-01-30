#ifndef __SERIALCOMM_H_INCLUDED
#define __SERIALCOMM_H_INCLUDED

#include "Receive.h"

// ASCII command characters
#define SOH		0x01	// Start of Header
#define STX		0x02	// Start of checksummed data
#define ETX		0x03	// End of checksummed data
#define EOT		0x04	// End of transmission
#define ENQ		0x05	// Enquire
#define ACK		0x06	// Positive acknowledgement
#define LF		0x0a	// Line feed
#define CR		0x0d	// Carriage return
#define NAK		0x15	// Negative acknowledgement
#define ESC		0x1b	// Escape

#define NEITHER_ACTIVE		0
#define TRANSMIT_ACTIVE		1
#define RECEIVE_ACTIVE		2

#define BAUD_RATE_ASCII_DUMP	9600
#define BAUD_RATE_NORMAL		19200
#define BAUD_RATE_RAD_XFER		57600

extern int				transmitReceiveState;
extern unsigned char	receivedChar;
extern Receive *		receive;
extern AStrings         strings;

#endif // __SERIALCOMM_H_INCLUDED
