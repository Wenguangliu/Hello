#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "LogErr.h"
#include "SetExpireTime.h"


// calculate julian date
unsigned int
JulianDate (unsigned int month, unsigned int day, unsigned short year)
{
    unsigned jDate;
    unsigned temp;

    temp = 12 * ( year + 4800 ) + month - 3;
    jDate = ( 2 * ( temp - ( temp / 12 ) * 12 ) + 7 + 365 * temp ) / 12;
    jDate += day + temp / 48 - 32045 + temp / 4800 - temp / 1200;

    return jDate;
}



// write new ExpireTime to file
int
WriteExpireJulianDate (unsigned int expireDays, unsigned int expirejulianDate)
{
    FILE            *fp;
    char            wStr [512];


    fp = fopen (EXPIRE_DATE_FILE, "wb");

    if (fp != 0) {
        // first write expireDyas
        sprintf (wStr,"%u\n",expireDays);
        fwrite (wStr, strlen (wStr), 1, fp);
        // seconds write julianDate
        sprintf (wStr,"%u\n",expirejulianDate);
        fwrite (wStr, strlen (wStr), 1, fp);
        fclose (fp);
        return 0;
    } else {
        LogError ("Fail to open file %s\n", EXPIRE_DATE_FILE);
    }

    return -1;

}

//read expire month and day
int
ReadExpireJulianDate (unsigned int *exprieDays, unsigned int *expireJulianDate)
{
    FILE            *fp;
    char            rStr [512];

    fp = fopen (EXPIRE_DATE_FILE, "rb");
    if (fp != 0) {
        // first get expired days
        fgets (rStr, 512,  fp);
        *exprieDays = strtol (rStr, NULL, 10);
        fgets (rStr, 512,  fp);
        *expireJulianDate = strtol (rStr, NULL, 10);
        fclose (fp);
        return 0;
    } else {
        LogError ("fail to open %s file\n", EXPIRE_DATE_FILE);
    }
    return -1;
}


// get current julian date
unsigned int
GetCurrentJulianDate (SYSTEMTIME currentDate)
{

    return (JulianDate (currentDate.wMonth,  currentDate.wDay, currentDate.wYear));

}



// calcuate expired date left
int
CalculateExpireDateLeft (SYSTEMTIME     currentDate, unsigned int *expireDays)
{
    unsigned int    currentJulianDay, expireJulianDay;
    int             expireDateLeft;

    currentJulianDay = GetCurrentJulianDate (currentDate);

    ReadExpireJulianDate (expireDays, &expireJulianDay);

    expireDateLeft = expireJulianDay - currentJulianDay;
    
    return expireDateLeft;
}


// calcuate new expire time
int
CalculateExpireDays (SYSTEMTIME oldDate, SYSTEMTIME   newDate)
{

    unsigned int    currentJulianDay;
    unsigned int    expireDays;
    int             expireDateLeft;
    unsigned int    newJulianExpireDay;

    expireDateLeft = CalculateExpireDateLeft (oldDate, &expireDays);

    currentJulianDay = GetCurrentJulianDate (newDate);

    if (expireDateLeft > 0) {
        newJulianExpireDay = currentJulianDay + expireDateLeft; 
    } else {
        newJulianExpireDay = 0;
    }

    WriteExpireJulianDate (expireDays, newJulianExpireDay);
    
    return 0;

}

// set new expire Julian Days
int
SetExpireJulianDays (SYSTEMTIME date, unsigned int expireDays)
{
    unsigned int    currentJulianDate;
    unsigned int    expireJulianDate;

    currentJulianDate = GetCurrentJulianDate (date);

    expireJulianDate = currentJulianDate + expireDays;

    WriteExpireJulianDate (expireDays, expireJulianDate);
	return 0;

}

// detect the expire file exist or not
int
DetectExpireDateFile (void)
{
    DWORD           attribs;
    wchar_t         fileName [255];

    wsprintf (fileName, L"%S", EXPIRE_DATE_FILE);
    attribs = GetFileAttributes (fileName);

    if (attribs == INVALID_FILE_ATTRIBUTES) {
        return -1;
    } 
    return 1;

}
