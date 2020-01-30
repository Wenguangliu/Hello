#ifndef __RECEIVE_H_INCLUDED
#define __RECEIVE_H_INCLUDED

#include <mqueue.h>

#include "SerialCommMessages.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "BarcodeData.h"

#define RX_FRAME_SIZE	1000

// Receive states.
typedef enum
{
	RX_IDLE,
	RX_DATA,
} RxState_t;

typedef enum
{
	RX_SERNUM,
	RX_VERSION,
	RX_ROC,
	RX_RIS,
	RX_NONE,
} RxFrameState_t;


class Receive
{
public:
					Receive();
	void			Run( int fdSer );

private:
	RxState_t		WaitForSoh();
	void			InformAnalysisRecall();
	void			InitFrame();
	void			AddChar( char ch );
	void			ProcessFrame();
	int				ReceiveBarcodeData( unsigned checksum );
	int				ReceiveFile( unsigned fileSize, char * fileName );
	void			SendAnalysisMessage( char * fileName );
	void			SendAck();
	void			SendNak();
	void			SendSerNum();
	void			SendVersion();
	void			WriteBarcodeData( BarcodeData_t * barcodeData );

	int				fdSer;
	char			rxFrame[ RX_FRAME_SIZE ];	// Buffer to receive a frame.
	char *			rxFramePtr;					// Position to store a new character.
	int				rxCount;					// Received character count.
	unsigned short	crc;
	FactoryData_t *	factoryData;
	SystemData_t *	systemData;
};

#endif // __RECEIVE_H_INCLUDED

