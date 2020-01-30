#include "Global.h"

int				transmitReceiveState = NEITHER_ACTIVE;
unsigned char	receivedChar;

Receive *		receive;

HANDLE			hTransmitReceiveState;
HANDLE			hReceivedChar;

CRITICAL_SECTION	csTransmitReceiveState;
CRITICAL_SECTION	csReceivedChar;

void InitGlobal() {
	//Create events
	hTransmitReceiveState = CreateEvent(NULL, FALSE, FALSE, L"TransmitReceiveState");
	hReceivedChar = CreateEvent(NULL, FALSE, FALSE, L"ReceivedChar");

	//Initialize Critical Sections
	InitializeCriticalSection(&csTransmitReceiveState);
	InitializeCriticalSection(&csReceivedChar);
}

