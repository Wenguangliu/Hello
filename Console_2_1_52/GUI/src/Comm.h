/*
 * FILENAME
 * 	Comm.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the Comm class, which handles the sending of messages
 * 	to other processes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __COMM_H_INCLUDED
#define __COMM_H_INCLUDED

#include <mqueue.h>

#include "GuiMessages.h"

class Comm
{
public:
	static int	Activate( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	BottomButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Button( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Down( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	DownRepeat( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	IconMatrix( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Info( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	List( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	ListMulti( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Manufacturing( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	NextSettings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	PreviousSettings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Recall( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Settings( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Toggle( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	Up( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int	UpRepeat( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );

	static void	SetCommReceiver( const char * queueName );
	static void SendMessage( int msgId, const int data );
	static void SendMessage( int msgId, const char * data );
	static void SendMessage( int msgId, GuiSendRangeData_t range );

private:	
	static mqd_t mqueue;
};
#endif
