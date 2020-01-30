#ifndef __TRANSMIT_H_INCLUDED
#define __TRANSMIT_H_INCLUDED

#include <termios.h>

#include "SettingsData.h"
#include "SerialCommMessages.h"
#include "RecallMessages.h"
#include "RefRanges.h"
#include "SystemData.h"

typedef struct
{
	int		fdSer;
	time_t	startDate;
	time_t	endDate;
} SendArg_t;

extern bool cancelSend;

class Transmit
{
public:
	void			Run( int fdSer );
	
private:
	static void *	SendAsciiRange( void * arg );
	void			SendAsciiReport( SendReportData_t reportData );
	static void		SendRadFile( char * radFileName );
	static void	*	SendRadRange( void * arg);
	static void		SendRecallMessage( RecallMsg_t * msg, int msgDataLen );
	void			SendRefRanges( SampleType_t sampleType );
	static void *	SendXmlRange( void * arg );
	void			SendXmlReport( int fdSer, int index );
	static void		SetPortForRad( termios * saveTerm );

	static int				fdSer;
	static pthread_t		sendThread;
	SettingsData_t *		settingsData;
	RefRangeData_t *		refRanges;
	static FactoryData_t *	factoryData;
	static SystemData_t *	systemData;
	static SendArg_t		sendArg;
};

#endif // __TRANSMIT_H_INCLUDED
