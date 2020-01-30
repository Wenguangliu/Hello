/*
 * FILENAME
 * 	UpdateDateTime.cc
 * 
 * MODULE DESCRIPTION
 * 	Updates the display of the date and time in the bottom section of the screen
 * 	when the date-time display is visible.
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
#include "TranslateUtf.h"

const long int TIMER_INTERVAL = 1000;	// 1000 milliseconds

static char daysOfWeek[7][20];
static char months[12][10];
static char twentyFourHourFormat;
static int	previousHour = -1;
static int	previousMinute = -1;

/*
 * FUNCTION
 * 	UpdateDateTime::Init
 * 
 * DESCRIPTION
 * 	Initializes the date/time display by saving the parameters passed in.
 * 
 * PARAMETERS
 * 	msgData - Message data containing the list of strings for the days of the
 * 			week, the list of strings for months of the year, and the time
 * 			format - 12- or 24-hour.
 */

void
UpdateDateTime::
Init( GuiBottomDateTimeData_t *msgData )
{
	int i;
	
	for ( i = 0; i < 7; i++ )
	{
		TranslateToUtf( msgData->daysOfWeek[ i ], daysOfWeek[ i ] );
	}
	
	for ( i = 0; i < 12; i++ )
	{
		TranslateToUtf( msgData->months[ i ], months[ i ] );
	}
	
	twentyFourHourFormat = msgData->twentyFourHourFormat;
}

/*
 * FUNCTION
 * 	UpdateDateTime::Start
 * 
 * DESCRIPTION
 * 	Starts the date/time display updates by starting the timer that triggers
 * 	the updates.
 */

void
UpdateDateTime::
Start()
{
	// Force the update of the displayed time and date.
	previousHour = -1;
	Update( 0, 0, 0 );

	PtSetResource( ABW_ptTimerBottomDateTime, Pt_ARG_TIMER_INITIAL, TIMER_INTERVAL, 0 );
}

/*
 * FUNCTION
 * 	UpdateDateTime::Stop
 * 
 * DESCRIPTION
 * 	Stop the date/time display updates by stoppng the timer that triggers
 * 	the updates.
 */

void
UpdateDateTime::
Stop()
{
	PtSetResource( ABW_ptTimerBottomDateTime, Pt_ARG_TIMER_INITIAL, 0, 0 );
}

/*
 * FUNCTION
 * 	UpdateDateTime::Update
 * 
 * DESCRIPTION
 * 	Callback function triggered by the timer for the date/time display updates.
 * 	Gets the current date/time and displays it on the screen.
 * 
 * PARAMETERS
 * 	Standard QNX callback parameters - ignored.
 */

int
UpdateDateTime::
Update( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	time_t			t;
	struct tm * 	currentTime;
	int				hour, minute;
	char			buff[40];
	const char *	suffix;

	// Get the current time.
	t = time( 0 );
	currentTime = gmtime( &t );
	hour = currentTime->tm_hour;
	minute = currentTime->tm_min;
	
	// Update the display if we've changed to the next minute.  We compare the
	// hour, too, in case someone has just changed the system time by exactly one
	// hour.
	if ( (previousHour != hour) || (previousMinute != minute) )
	{
		// Adjust the hour and set the time suffix, depending on the time format.
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
	
		// Create the date/time string.	
		sprintf( buff, "%s %d %s %d %2.2d:%2.2d %s", daysOfWeek[currentTime->tm_wday],
				 currentTime->tm_mday, months[currentTime->tm_mon], currentTime->tm_year+1900,
				 hour, minute, suffix );
	
		// Update the screen display.
// GLDGLDGLD
//		GuiSection::SetString( ABW_ptLabelBottomDateTime, buff, false );
		PtSetResource( ABW_ptLabelBottomDateTime, Pt_ARG_TEXT_STRING, buff, 0 );

		previousHour = currentTime->tm_hour;
		previousMinute = currentTime->tm_min;
	}
	
	return( Pt_CONTINUE );
}
