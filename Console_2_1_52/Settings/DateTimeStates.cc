/*
 * FILENAME
 * 	DateTimeStates.cc
 * 
 * MODULE DESCRIPTION
 * 	Member function definitions for the Settings state machine date/time states
 *  classes.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#include <stdio.h>
#include <sys/neutrino.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>

#include "DateTimeStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "LogErr.h"
#include "CfgErr.h"

/*
 * Class StateDateTime
 */

/*
 * FUNCTION
 * 	StateDateTime::SetRtc
 * 
 * DESCRIPTION
 * 	Sets up the RTC from the OS system date/time.
 */

void
StateDateTime::
SetRtc()
{
    uintptr_t		handle;
    uint8_t			inVal;
    time_t			t;
    struct tm *		tm;

	if (ThreadCtl( _NTO_TCTL_IO, 0) == -1 )
	{
		LogError( "failed to get memory control privilege" );
		return;
	}
	
	handle = mmap_device_io( 16, 0x0240 );
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError("failed to map I/O");
		return;
	}
	
	// Inhibit update transfers.
	inVal = in8( handle + 0xF );
	inVal &= ~0x80;
	out8( handle + 0xF, inVal );
	
	// Get the system date/time.
	t = time( 0 );
	tm = gmtime( &t );

	// Set the RTC.

	out8( handle + 0, HexToBcd( tm->tm_sec ) );					// Seconds
	out8( handle + 1, HexToBcd( tm->tm_min ) );					// Minutes
	out8( handle + 2, HexToBcd( tm->tm_hour ) );				// Hours
	out8( handle + 4, HexToBcd( tm->tm_mday ) );				// Day
	out8( handle + 5, ( in8( handle + 5 ) & 0xE0 ) | HexToBcd( tm->tm_mon + 1 ) );	// Month
	out8( handle + 6, HexToBcd( tm->tm_year % 100 ) );			// Year
	out8( handle + 7, HexToBcd( (tm->tm_year + 1900) / 100 ) );	// Century

	// Reenable transfers.
	inVal = in8( handle + 0xF );
	inVal |= 0x80;
	out8( handle + 0xF, inVal );
}

unsigned char
StateDateTime::
HexToBcd( unsigned char hexVal )
{
	uint8_t	lowNibble = hexVal % 10;
	uint8_t highNibble = hexVal / 10;
	uint8_t bcdVal = lowNibble | ( highNibble << 4 );

	return bcdVal;
}

/*
 * Class StateDateTimeSetDate
 */

/*
 * FUNCTION
 * 	StateDateTimeSetDate::Enter
 * 
 * DESCRIPTION
 * 	Sets up the GUI and condition variables upon entrance to this state.
 * 
 * NOTE
 * 	This header applies to all Enter() methods for the following subclasses of
 * 	class State.
 */

void
StateDateTimeSetDate::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	data = &msg.data.middleEnterDate;
	
	SetTopLabel( strings[ TSTR_SET_DATE ] );

	msg.msgId = MSG_GUI_MIDDLE_ENTER_DATE;
	strcpy( data->labelLeft, strings[ MSTR_DAY ] );
	strcpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	strcpy( data->labelRight, strings[ MSTR_YEAR ] );

	sprintf( data->textLeft, "%d", tm.tm_mday );
	sprintf( data->textMiddle, "%s", strings.Month( tm.tm_mon ) );
	sprintf( data->textRight, "%d", tm.tm_year + 1900 );
	
	data->buttonText[0] = '\0';
	
	SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );

	SetBottomButtons( strings[ BSTR_DONE ], true );
}

/*
 * FUNCTION
 * 	StateDateTimeSetDate::ProcessMessage
 * 
 * DESCRIPTION
 * 	Handles a message received by this state.
 * 
 * PARAMETERS
 * 	msgId - ID for the received message.
 * 	data - Data field of the received message.
 * 
 * RETURNS
 * 	State to proceed to, depending on the result of the message processing.  If
 * 	a change in state isn't called for, the current state is returned so a
 * 	state change won't take place.
 *
 * NOTE
 * 	This header applies to all ProcessMessage() methods for the following subclasses of
 * 	class State.
 */

state_t
StateDateTimeSetDate::
ProcessMessage( int msgId, char * inData )
{
	state_t 					retVal = STATE_DATETIME_SET_DATE;
	int							idata = ((GuiSendMsgData_t *)inData)->idata;
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterDateData_t *	outData = &msg.data.middleEnterDate;
	bool						updateDay = false;
	bool						updateMonth = false;
	bool						updateYear = false;
	time_t						t;
	struct tm *					tp;
	struct timespec				stime;
	const int					daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0:
					tm.tm_mday++;
					if ( tm.tm_mday > 31 )
					{
						tm.tm_mday = 1;
					}
					updateDay = true;
					break;
				case 1:
					tm.tm_mon++;
					if ( tm.tm_mon > 11 )
					{
						tm.tm_mon = 0;
					}
					updateMonth = true;
					break;
				case 2:
					tm.tm_year++;
					if ( tm.tm_year < 0 )
					{
						tm.tm_year = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_DOWN:
			switch ( idata )
			{
				case 0:
					tm.tm_mday--;
					if ( tm.tm_mday < 1 )
					{
						tm.tm_mday = 31;
					}
					updateDay = true;
					break;
				case 1:
					tm.tm_mon--;
					if ( tm.tm_mon < 0 )
					{
						tm.tm_mon = 11;
					}
					updateMonth = true;
					break;
				case 2:
					tm.tm_year--;
					if ( tm.tm_year < 0 )
					{
						tm.tm_year = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( tm.tm_mday <= daysInMonth[ tm.tm_mon ]
			  || ( tm.tm_mday == 29 && tm.tm_mon == 1 && tm.tm_year % 4 == 0 && ( (tm.tm_year + 1900) / 100 ) % 4 == 0 ) )
			{
				// Get the current time and put it in our tm struct.
				t = time( 0 );
				tp = gmtime( &t );
				tm.tm_sec = tp->tm_sec;
				tm.tm_min = tp->tm_min;
				tm.tm_hour = tp->tm_hour;
				t = mktime( &tm );

				// Update the OS clock.
				stime.tv_sec = t;
				stime.tv_nsec = 0;
				clock_settime( CLOCK_REALTIME, &stime );

				// Update the RTC.
				SetRtc();
				
				retVal = stateStack.Pop();
			}
			else
			{
				stateStack.Push( STATE_DATETIME_SET_DATE );
				retVal = STATE_INVALID_DATE;
			}
			break;
		default:
			LogDebug( "got %d message", msgId );
			break;
	}

	if ( updateDay )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_LEFT;
		sprintf( outData->textLeft, "%d", tm.tm_mday );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		sprintf( outData->textMiddle, "%s", strings.Month( tm.tm_mon ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		sprintf( outData->textRight, "%d", tm.tm_year + 1900 );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	
	return retVal;
}

/*
 * Class StateDateTimeSetTime
 */


void
StateDateTimeSetTime::
Enter()
{
	GuiReceiveMsg_t				msg;
	GuiMiddleEnterTimeData_t *	data = &msg.data.middleEnterTime;
	
	SetTopLabel( strings[ TSTR_SET_TIME ] );
	
	msg.msgId = MSG_GUI_MIDDLE_ENTER_TIME;
	data->twentyFourHourFormat = settingsData->time24Format;
	strcpy( data->hoursLabel, strings[ MSTR_HOUR ] );
	strcpy( data->minutesLabel, strings[ MSTR_MINUTE ] );
	strcpy( data->formatButtonText, "12/24" );
	strcpy( data->zeroSecButtonText, strings[ MSTR_ZERO_SEC ] );

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterTimeData_t ) );

	SetBottomButtons( strings[ BSTR_DATE ], true );
}

state_t
StateDateTimeSetTime::
ProcessMessage( int msgId, char * data )
{
	state_t		retVal = STATE_DATETIME_SET_TIME;
	time_t		t;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			settingsData->time24Format = !settingsData->time24Format;

			// This updates the 12/24 hour format.
			SetBottomDateTimeStrings();
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Date
			// Update the RTC.
			SetRtc();
	
			// Update the settings config file, in case 12/24 format changed.
			cfgErr = WriteSettings( settingsData );
			if ( cfgErr != CFG_ERR_NONE )
			{
				errNum = SERR_FILE_WRITE;
				retVal = STATE_ERROR;
			}
			else
			{
				// Get the current date/time.
				// Do this from here instead of when we enter SetDate, so that when we return
				// to SetDate from InvalidDate we've got the date that was left there.
				t = time( 0 );
				memcpy( &tm, gmtime( &t ), sizeof ( struct tm ) );
		
				retVal = STATE_DATETIME_SET_DATE;
			}
			break;
		default:
			LogDebug( "got %d message", msgId );
			break;
	}
	
	return retVal;
}

