#ifndef __RECEIVE_H_INCLUDED
#define __RECEIVE_H_INCLUDED

#include <windows.h>
#include <msgqueue.h>

#include <FTD2XX.h>
#include "EngineCommMessages.h"
#include "FactoryData.h"
//#include "AmsData.h"

// States for the Run function.
typedef enum {
	RX_IDLE,
//	RX_WAIT_FOR_ENQ,
	RX_DATA,
	RX_BINARY_MSB,
	RX_BINARY_LSB,
	RX_BINARY_DATA
} RxState_t;

typedef struct
{
    int     motorCount;
    int     temperatureCount;
    int     cuvetteDelayCount;
    int     ndxtCount;
} AmsTestCounter_t;

class Receive
{
public:
					Receive();
	void			Run( FT_HANDLE fdSer );
	void			InitRawData();
	void			SetMessageReceiver( char * receiverName );
    void            GetDateTime ();
    void            InitialCount ();

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
    int             HandleTemperatureDumpData ();
    void            HandleMotorTestMessage ();
	void			InitFrame();
	char			VerifyFrame();
	void			ProcessFrame();
	unsigned char	AsciiToHex( char ascii );
	void			UpdateChecksum( char c ) { if ( calcChecksum ) calculatedChecksum += c; };
	void			SendNak();
    void            SendDataMessage ();

	FT_HANDLE		fdSer;
	bool	 		calcChecksum;				// Flag to enable/disable checksumming.
	unsigned char	calculatedChecksum;			// Checksum calculated for the current frame.
	unsigned		binaryCount;				// Binary data count.
	unsigned char	rxFrame[ RX_FRAME_SIZE ];	// Buffer to receive a frame.
	unsigned char *	rxFramePtr;					// Position to store a new character.
	int				rxCount;					// Received character count.
    int             cuvetteDelayCount;          // Received cuvette delay count
	HANDLE			mqueue;
	HANDLE			heaterControlQueue;			// Destination for all 'H' messages
    HANDLE          watchdogQueue;              // Destination for 'W' messages
    char            dateTimeStr [24];           // buff save local time string
    AmsTestCounter_t amsTestCounter;            // ams test counter
	
	RotorRawReadings_t *	    rotorRawReadings;
	ReceivedDataCounter_t *	    receivedDataCounter;
    static FactoryData_t *      factoryData;
};

#endif // __RECEIVE_H_INCLUDED
