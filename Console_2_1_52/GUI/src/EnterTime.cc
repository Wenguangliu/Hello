/*
 * FILENAME
 * 	EnterTime.cc
 * 
 * MODULE DESCRIPTION
 * 	Class method definitions for the EnterTime class, which manages the Enter
 * 	Time subscreen.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "Global.h"
#include "abimport.h"
#include "Sound.h"

const long int	TIMER_INTERVAL = 100;	// 100 milliseconds

char			EnterTime::twentyFourHourFormat = 0;

/*
 * FUNCTION
 * 	Start
 * 
 * DESCRIPTION
 * 	Sets the time displayed to the current time and starts the timer that
 * 	updates the display periodically.
 */

void
EnterTime::Start()
{
	Update( 0, 0, 0 );
	PtSetResource( ABW_ptTimerEnterTime, Pt_ARG_TIMER_INITIAL, TIMER_INTERVAL, 0 );
}

/*
 * FUNCTION
 * 	Stop
 * 
 * DESCRIPTION
 * 	Stops updates to the displayed time.
 */

void
EnterTime::Stop()
{
	PtSetResource( ABW_ptTimerEnterTime, Pt_ARG_TIMER_INITIAL, 0, 0 );
}
	
/*
 * FUNCTION
 * 	SetFormat
 * 
 * DESCRIPTION
 * 	Sets the time display format to either 12- or 24-hour mode.
 * 
 * PARAMETERS
 * 	format - flag to indicate 12- or 24-hour format.
 */

void
EnterTime::SetFormat( char format )
{
twentyFourHourFormat = format;
}

/*
 * FUNCTION
 * 	HourUp
 * 
 * DESCRIPTION
 * 	Callback handler for the Hour Up button.  Increments the current hour by
 * 	one.  Does not cause the day to increment when the hour is incremented past
 * 	midnight.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::HourUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct timespec stime;
	
	clock_gettime( CLOCK_REALTIME, &stime );
	int minSec = stime.tv_sec % (60*60);
	int hour = (stime.tv_sec / (60*60) ) % 24;
	hour += 1;
	if ( hour > 23 )
	{
		hour = 0;
	}
	stime.tv_sec =(stime.tv_sec / (60*60*24) ) * (60*60*24) + (hour * 60*60) + minSec;
	clock_settime( CLOCK_REALTIME, &stime );
	
	Update( 0, 0, 0 );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	HourDown
 * 
 * DESCRIPTION
 * 	Callback handler for the Hour Down button.  Decrements the current hour by
 * 	one.  Does not day to decrement when the hour is decremented past midnight.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */


int
EnterTime::HourDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct timespec stime;
	
	clock_gettime( CLOCK_REALTIME, &stime );
	int minSec = stime.tv_sec % (60*60);
	int hour = (stime.tv_sec / (60*60) ) % 24;
	hour -= 1;
	if ( hour < 0 )
	{
		hour = 23;
	}
	stime.tv_sec =(stime.tv_sec / (60*60*24) ) * (60*60*24) + (hour * 60*60) + minSec;
	clock_settime( CLOCK_REALTIME, &stime );
	
	Update( 0, 0, 0 );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	MinuteUp
 * 
 * DESCRIPTION
 * 	Callback handler for the Minute Up button.  Increments the current minute
 * 	by one.  Does not increment the current hour when the minute is incremented
 * 	past 59.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::MinuteUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct timespec stime;
	
	clock_gettime( CLOCK_REALTIME, &stime );
	int sec = stime.tv_sec % 60;
	int min = (stime.tv_sec / 60) % 60;
	min += 1;
	if ( min > 59 )
	{
		min = 0;
	}
	stime.tv_sec = (stime.tv_sec / (60*60) ) * (60*60) + (min * 60) + sec;
	clock_settime( CLOCK_REALTIME, &stime );

	Update( 0, 0, 0 );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	MinuteDown
 * 
 * DESCRIPTION
 * 	Callback handler for the Minute Down button.  Decrements the current minute
 * 	by one.  Does not decrement the current hour when the minute is decremented
 * 	past zero.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::MinuteDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct timespec stime;
	
	clock_gettime( CLOCK_REALTIME, &stime );
	int sec = stime.tv_sec % 60;
	int min = (stime.tv_sec / 60) % 60;
	min -= 1;
	if ( min < 0 )
	{
		min = 59;
	}
	stime.tv_sec =(stime.tv_sec / (60*60) ) * (60*60) + (min * 60) + sec;
	clock_settime( CLOCK_REALTIME, &stime );
	
	Update( 0, 0, 0 );

	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Format
 * 
 * DESCRIPTION
 * 	Callback handler for the Format button.  Toggles the data display between
 * 	12- and 24-hour formats.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::Format( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	twentyFourHourFormat = ! twentyFourHourFormat;
	Update( 0, 0, 0 );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	ZeroSec
 * 
 * DESCRIPTION
 * 	Callback handler for the Zero Seconds button.  Sets the current seconds
 * 	value to zero.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::ZeroSec( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	struct timespec stime;
	
	clock_gettime( CLOCK_REALTIME, &stime );
	stime.tv_sec = (stime.tv_sec / 60 ) * 60;
	stime.tv_nsec = 0;
	clock_settime( CLOCK_REALTIME, &stime );
	
	Click();
	
	return( Pt_CONTINUE );
}

/*
 * FUNCTION
 * 	Update
 * 
 * DESCRIPTION
 * 	Callback handler for the update timer button.  Also called at screen start-
 * 	up to set the display to the current time.
 * 
 * PARAMETERS
 * 	cbinfo - Callback info, which contains information on the newly-input text.
 * 	widget, apinfo - Standard Photon callback parameters - ignored.
 * 
 * RETURNS
 * 	Pt_CONTINUE - Standard Photon callback return value.
 */

int
EnterTime::Update( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	static int prevHour = 0;
	static int prevMin = 0;
	static int prevSec = 0;
	
	// Get the current time.
	time_t t = time( 0 );
	struct tm *currentTime = gmtime( &t );
	
	if ( prevHour != currentTime->tm_hour || prevMin != currentTime->tm_min ||
		 prevSec != currentTime->tm_sec )
	{
		// Save the time.
		prevHour = currentTime->tm_hour;
		prevMin = currentTime->tm_min;
		prevSec = currentTime->tm_sec;
	
		// Update the display.
		SetTimeDisplay( currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, twentyFourHourFormat );
	}

	return Pt_CONTINUE;
}

/*
 * FUNCTION
 * 	SetTimeDisplay
 * 
 * DESCRIPTION
 * 	Sets the displayed time to the value passed.
 * 
 * PARAMETERS
 * 	hour - Hour value to display.
 * 	minute - Minute value to display.
 *  second - Second value to diplay.
 *  twentyFourHourFormat - Boolean: true->24-hour format, false->12-hour format.
 */

void
EnterTime::SetTimeDisplay( int hour, int minute, int second, char twentyFourHourFormat )
{
	char			buff[ 12 ];
	const char *	suffix;
	
	if ( twentyFourHourFormat == 0 )
	{
		if ( hour >= 12 )
		{
			suffix = " PM";
			if ( hour > 12 )
			{
				hour -= 12;
			}
		}
		else
		{
			suffix = " AM";
			if ( hour == 0 )
			{
				hour = 12;
			}
		}
	}
	else
	{
		suffix = "";
	}
	
	sprintf( buff, "%2.2d:%2.2d:%2.2d%s", hour, minute, second, suffix );

	PtSetResource( ABW_ptTextEnterTime, Pt_ARG_TEXT_STRING, buff, 0 );
}
