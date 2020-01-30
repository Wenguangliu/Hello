#ifndef __RECEIVE_H
#define __RECEIVE_H

#include <windows.h>
#include <msgqueue.h>

#include <FTD2XX.h>
#include "EngineCommMessages.h"
#include "FactoryData.h"

#include "AmsData.h"


// States for the Run function.
typedef enum {
	RX_IDLE,
	RX_DATA,
	RX_BINARY_MSB,
	RX_BINARY_LSB,
	RX_BINARY_DATA
} RxState_t;

class Receive
{
public:
					Receive();
	void			Run( FT_HANDLE  fdSer );
	void			SetMessageReceiver( char * receiverName );
    void            InitAmsData ();
    void            GetDateTime ();

private:
	void			InitSharedMemory();
	RxState_t		WaitForEnq();
	RxState_t		HandleAsciiChar( char ch );
	RxState_t		HandleBinaryCountLsb( char ch );
	RxState_t		HandleBinaryCountMsb( char ch );
	RxState_t		HandleBinaryData( char ch );
	void			HandlePhotometricData();
	void			HandleTemperatureData();
	void			HandleAnalysisStatusMessage();
    void            HandleMotorTestMessage();
    int             HandleTemperatureDumpData ();
	void			InitFrame();
	void			AddChar( char ch );
	char			VerifyFrame();
	void			ProcessFrame();
	unsigned char	AsciiToHex( char ascii );
	void			UpdateChecksum( char c ) { if ( calcChecksum ) calculatedChecksum += c; }
    void            SendNak();

	FT_HANDLE	    fdSer;
	bool	 		calcChecksum;				// Flag to enable/disable checksumming.
	unsigned char	calculatedChecksum;			// Checksum calculated for the current frame.
	unsigned		binaryCount;				// Binary data count.
	unsigned char	rxFrame[ RX_FRAME_SIZE ];	// Buffer to receive a frame.
	unsigned char *	rxFramePtr;					// Position to store a new character.
	int				rxCount;					// Received character count.
	FT_HANDLE		mqueue;
	
	AmsData_t *		amsData;
	int				passCount;					// Received pass count.
	int				temperatureCount;			// Received temperature message count.
    int             cuvetteDelayCount;          // Received cuvette delay count
	int				statusCount;				// Received status message count.
    int             motorCount;                 // Received motor postion count
    char            dateTimeStr [24];           // buff save local time string
    static FactoryData_t *      factoryData;
};

#endif // __RECEIVE_H

