#ifndef __RECEIVE_H_INCLUDED
#define __RECEIVE_H_INCLUDED

#include "SerialCommMessages.h"
#include "FactoryData.h"
#include "SystemData.h"
#include "EngineCommMessages.h"

#define SR_RX_FRAME_SIZE	1000

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
	RX_NONE,
} RxFrameState_t;


class Receive
{
public:
					Receive();
#if defined(QNX)
					void Run( int fdSer );
#elif defined(WINCE)
					void Run( FT_HANDLE fdSer );
#else
					Must have QNX or WINCE defined
#endif

private:
	RxState_t		WaitForSoh();
	void			InformAnalysisRecall();
	void			InitFrame();
	void			AddChar( char ch );
	void			ProcessFrame();
	void			SendAck();
	void			SendNak();
	void			SendSerNum();
	void			SendVersion();
    void            SendEngineCommMessage (EngineCommReceiveMsg_t * msg, int msgDataLen);
    void            SendEngineFanHighSpeed ();

#if defined(QNX)
					int fdSer;
#elif defined(WINCE)
					FT_HANDLE fdSer;
#endif
	char			rxFrame[ SR_RX_FRAME_SIZE ];	// Buffer to receive a frame.
	char *			rxFramePtr;					// Position to store a new character.
	int				rxCount;					// Received character count.
	unsigned short	crc;
	FactoryData_t *	factoryData;
	SystemData_t *	systemData;
};

#endif // __RECEIVE_H_INCLUDED

