#ifndef __RECEIVE_H_INCLUDED
#define __RECEIVE_H_INCLUDED

#include <mqueue.h>

#include "EngineCommMessages.h"

// States for the Run function.
typedef enum {
	RX_IDLE,
//	RX_WAIT_FOR_ENQ,
	RX_DATA,
	RX_BINARY_MSB,
	RX_BINARY_LSB,
	RX_BINARY_DATA
} RxState_t;

class Receive
{
public:
					Receive();
	void			Run( int fdSer );
	void			InitRawData();
	void			SetMessageReceiver( char * receiverName );

private:
	void			InitSharedMemory();
	RxState_t		WaitForEnq();
	void			HandleAdcReferenceOffset();
	RxState_t		HandleAsciiChar( char ch );
	RxState_t		HandleBinaryCountLsb( char ch );
	RxState_t		HandleBinaryCountMsb( char ch );
	RxState_t		HandleBinaryData( char ch );
	void			HandlePhotometricData();
	void			HandleTemperatureData();
	void			HandleAnalysisStatusMessage();
	void			InitFrame();
	char			VerifyFrame();
	void			ProcessFrame();
	unsigned char	AsciiToHex( char ascii );
	void			UpdateChecksum( char c ) { if ( calcChecksum ) calculatedChecksum += c; };
	void			SendNak();

	int				fdSer;
	bool	 		calcChecksum;				// Flag to enable/disable checksumming.
	unsigned char	calculatedChecksum;			// Checksum calculated for the current frame.
	unsigned		binaryCount;				// Binary data count.
	unsigned char	rxFrame[ RX_FRAME_SIZE ];	// Buffer to receive a frame.
	unsigned char *	rxFramePtr;					// Position to store a new character.
	int				rxCount;					// Received character count.
	mqd_t			mqueue;
	mqd_t			heaterControlQueue;			// Destination for all 'H' messages
	
	RotorRawReadings_t *	rotorRawReadings;
	ReceivedDataCounter_t *	receivedDataCounter;
};

#endif // __RECEIVE_H_INCLUDED

