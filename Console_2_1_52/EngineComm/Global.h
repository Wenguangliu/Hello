#ifndef __GLOBAL_H_INCLUDED
#define __GLOBAL_H_INCLUDED

#include "RawData.h"
#include "Receive.h"

// ASCII command characters
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

extern int				transmitReceiveState;
extern unsigned char	receivedChar;
extern Receive *		receive;

#endif // __GLOBAL_H_INCLUDED
