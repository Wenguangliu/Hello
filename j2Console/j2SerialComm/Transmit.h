#ifndef __TRANSMIT_H_INCLUDED
#define __TRANSMIT_H_INCLUDED

//#include <termios.h>

#include "SettingsData.h"
#include "SerialCommMessages.h"
#include "RecallMessages.h"
#include "RefRanges.h"
#include "SystemData.h"

typedef struct
{
#ifdef QNX
	int		fdSer;
#endif
#ifdef WINCE
	FT_HANDLE	fdSer;
#endif
	FILETIME	startDate;
	FILETIME	endDate;
} SendArg_t;

extern bool cancelSend;

class Transmit
{
public:
#if defined(QNX)
					void Run( int _fdSer );
#elif defined(WINCE)
					void Run( FT_HANDLE _fdSer );
#else
					Must have QNX or WINCE defined
#endif
	
private:
	static void *	SendAsciiRange( void * arg );
	void			SendAsciiReport( SendReportData_t reportData );
	static void		SendRadFile( char * radFileName );
	static void	*	SendRadRange( void * arg);
	static void		SendRecallMessage( RecallMsg_t * msg, int msgDataLen );
	void			SendRefRanges( SampleType_t sampleType );
	static void *	SendXmlRange( void * arg );
	static void		SetPortForRad( /*termios * saveTerm*/ );
#if defined(QNX)
	void			SendXmlReport( int fdSer, int index );
	static int		fdSer;
#elif defined(WINCE)
	void			SendXmlReport( FT_HANDLE fdSer, int index );
	static FT_HANDLE	fdSer;
#else
					Must have QNX or WINCE defined
#endif

	//static pthread_t		sendThread;
	static HANDLE			sendThread;
	static int				sendThreadDone;
	SettingsData_t *		settingsData;
	RefRangeData_t *		refRanges;
	static FactoryData_t *	factoryData;
	static SystemData_t *	systemData;
	static SendArg_t		sendArg;
};

#ifdef QNX
#define J2_PTHREAD_INVALID -1
#define J2_FD_INVALID -1
#endif
#ifdef WINCE
#define J2_PTHREAD_INVALID(_pt) ((NULL, 0}
#define J2_FD_INVALID NULL
#endif

#endif // __TRANSMIT_H_INCLUDED
