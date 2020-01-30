/*
 * FILENAME
 * 	EnterTime.h
 * 
 * MODULE DESCRIPTION
 * 	Class definition for the EnterTime class, which manages the Enter Time
 * 	subscreen.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ENTERTIME_H_INCLUDED
#define __ENTERTIME_H_INCLUDED

class EnterTime
{
public:
	static int HourUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int HourDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int MinuteUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int MinuteDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int Format( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int ZeroSec( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static int Update( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo );
	static void Start();
	static void Stop();
	static void SetFormat( char format );
	
private:
	static void SetTimeDisplay( int hour, int minute, int second, char format );
	
	static char	twentyFourHourFormat;
};

#endif
