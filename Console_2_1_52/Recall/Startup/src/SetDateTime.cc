#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/neutrino.h>
#include <sys/mman.h>
#include <hw/inout.h>
#include <ctype.h>

#include "SetDateTime.h"
#include "LogErr.h"

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t century;
} RtcSettings_t;

void	SetClock( RtcSettings_t rtc );
uint8_t	BcdToHex( uint8_t bcdVal );
int		ParseCommandLine( char * arg, RtcSettings_t * rtc );

void SetDateTime()
{
    uintptr_t		handle;
    uint8_t			inVal;
    RtcSettings_t	rtc;

	if ( ThreadCtl (_NTO_TCTL_IO, 0) == -1 )
	{
		LogError( "failed to get memory control privilege" );
		return;
	}
	
	handle = mmap_device_io( 16, 0x0240 );
	if ( handle == MAP_DEVICE_FAILED )
	{
		LogError( "failed to map I/O" );
		return;
	}
	
	// Inhibit update transfers.
	inVal = in8( handle + 0xF );
	inVal &= ~0x80;
	out8( handle + 0xF, inVal );
	
	// Get the RTC time.
	rtc.seconds	= in8( handle + 0 );
	rtc.minutes	= in8( handle + 1 );
	rtc.hours	= in8( handle + 2 );
	rtc.date	= in8( handle + 4 );
	rtc.month	= in8( handle + 5 ) & 0x1F;
	rtc.year	= in8( handle + 6 );
	rtc.century	= in8( handle + 7 );
	
	// Reenable transfers.
	inVal = in8( handle + 0xF );
	inVal |= 0x80;
	out8( handle + 0xF, inVal );
	
	// Set the OS clock.
	SetClock( rtc );
		
	return;
}

void
SetClock( RtcSettings_t rtc )
{
	struct tm		tm;
	time_t			t;
	struct timespec	stime;
	uint32_t		lYear = (uint32_t)BcdToHex( rtc.century ) * 100 + BcdToHex( rtc.year );
	
	memset( &tm, 0, sizeof( struct tm ) );

	tm.tm_sec = BcdToHex( rtc.seconds );
	tm.tm_min = BcdToHex( rtc.minutes );
	tm.tm_hour = BcdToHex( rtc.hours );
	tm.tm_mday = BcdToHex( rtc.date );
	tm.tm_mon = BcdToHex( rtc.month ) - 1;
	tm.tm_year = lYear - 1900;
	
	t = mktime( &tm );
	
	stime.tv_sec = t;
	stime.tv_nsec = 0;
	
	clock_settime( CLOCK_REALTIME, &stime );
}

uint8_t
BcdToHex( uint8_t bcdVal )
{
	uint8_t	lowNibble = bcdVal & 0x0F;
	uint8_t highNibble = bcdVal >> 4;
	uint8_t hexValue = lowNibble + highNibble * 10;
	return hexValue;
}
