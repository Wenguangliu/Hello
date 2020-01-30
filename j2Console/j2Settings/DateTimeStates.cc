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
 
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "DateTimeStates.h"
#include "SettingsMessages.h"
#include "ParseSettings.h"
#include "LogErr.h"
#include "CfgErr.h"
#include "SetExpireTime.h"

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
 	int	rc;
	
	rc = SetLocalTime(&tm);
	if ( rc == 0 )
	{
		LogError( "Failed to set system RTC" );
	}
}

unsigned char
StateDateTime::
HexToBcd( unsigned char hexVal )
{
	UINT8 lowNibble = hexVal % 10;
	UINT8 highNibble = hexVal / 10;
	UINT8 bcdVal = lowNibble | ( highNibble << 4 );

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
	wcscpy( data->labelLeft, strings[ MSTR_DAY ] );
	wcscpy( data->labelMiddle, strings[ MSTR_MONTH ] );
	wcscpy( data->labelRight, strings[ MSTR_YEAR ] );

	wsprintf( data->textLeft, L"%d", tm.wDay);
	wsprintf( data->textMiddle, L"%s", strings.Month( tm.wMonth ) );
	wsprintf( data->textRight, L"%d", tm.wYear);
	
	data->buttonText[0] = L'\0';
	
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
	//time_t						t;
	SYSTEMTIME					tp;
	//struct timespec				stime;
	const int					daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_UP:
			switch ( idata )
			{
				case 0:
					tm.wDay++;
					if ( tm.wDay > 31 )
					{
						tm.wDay = 1;
					}
					updateDay = true;
					break;
				case 1:
					tm.wMonth++;
					if ( tm.wMonth > 12 )
					{
						tm.wMonth = 1;
					}
					updateMonth = true;
					break;
				case 2:
					tm.wYear++;
					if ( tm.wYear < 0 )
					{
						tm.wYear = 0;
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
					tm.wDay--;
					if ( tm.wDay < 1 )
					{
						tm.wDay = 31;
					}
					updateDay = true;
					break;
				case 1:
					tm.wMonth--;
					if ( tm.wMonth < 1 )
					{
						tm.wMonth = 12;
					}
					updateMonth = true;
					break;
				case 2:
					tm.wYear--;
					if ( tm.wYear < 0 )
					{
						tm.wYear = 0;
					}
					updateYear = true;
					break;
				default:
					break;
			}
			break;
		case MSG_GUI_OUT_BOTTOM_BUTTON:
			if ( tm.wDay <= daysInMonth[ tm.wMonth-1 ]
			|| ( tm.wDay == 29 && tm.wMonth == 2 && tm.wYear % 4 == 0 && ( (tm.wYear) / 100 ) % 4 == 0 ) )
			{
				// Get the current time and put it in our tm struct.
				//t = time( 0 );
				//tp = gmtime( &t );
				GetLocalTime(&tp);
				tm.wSecond = tp.wSecond;
				tm.wMinute = tp.wMinute;
				tm.wHour = tp.wHour;
                //in the case of expire file exist, update expire date each time day and month changed 
                if (DetectExpireDateFile () == 1) {
                    // calculate and set system experiation date 
                    unsigned int expireDayLeft, expireDay;
                    expireDayLeft = CalculateExpireDateLeft (tp, &expireDay); 
                    //LogDebug ("old Expire Day left =%d", expireDayLeft);
                    // update new expire julian day
                    SetExpireJulianDays (tm, expireDayLeft);
                    //LogDebug ("new Expire Day left =%d", CalculateExpireDateLeft (tm, &expireDay));
                    
                }
                
				//t = mktime( &tm );

				// Update the OS clock.
				//stime.tv_sec = t;
				//stime.tv_nsec = 0;
				//clock_settime( CLOCK_REALTIME, &stime );
				SetLocalTime(&tm);

				// Update the RTC.
				//SetRtc();
				
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
		wsprintf( outData->textLeft, L"%d", tm.wDay );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateMonth )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
		wsprintf( outData->textMiddle, L"%s", strings.Month( tm.wMonth ) );
		SendGuiMessage( &msg, sizeof( GuiMiddleEnterDateData_t ) );
	}
	else if ( updateYear )
	{
		msg.msgId = MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
		wsprintf( outData->textRight, L"%d", tm.wYear );
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
	wcscpy( data->hoursLabel, strings[ MSTR_HOUR ] );
	wcscpy( data->minutesLabel, strings[ MSTR_MINUTE ] );
	wcscpy( data->formatButtonText, L"12/24" );
	wcscpy( data->zeroSecButtonText, strings[ MSTR_ZERO_SEC ] );

	SendGuiMessage( &msg, sizeof( GuiMiddleEnterTimeData_t ) );

	SetBottomButtons( strings[ BSTR_DATE ], true );
}

state_t
StateDateTimeSetTime::
ProcessMessage( int msgId, char * data )
{
	state_t		retVal = STATE_DATETIME_SET_TIME;
	//time_t		t;
	CfgError_t	cfgErr;
	
	switch ( msgId )
	{
		case MSG_GUI_OUT_BUTTON:
			settingsData->time24Format = !settingsData->time24Format;
			// This updates the 12/24 hour format.
			SetBottomDateTimeStrings(&strings);
			break;

		case MSG_GUI_OUT_BOTTOM_BUTTON:		// Date
			// Update the RTC.
			//SetRtc();
	
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
				//t = time( 0 );
				//memcpy( &tm, gmtime( &t ), sizeof ( struct tm ) );
				GetLocalTime(&tm);
		
				retVal = STATE_DATETIME_SET_DATE;
			}
			break;
		default:
			LogDebug( "got %d message", msgId );
			break;
	}
	
	return retVal;
}
