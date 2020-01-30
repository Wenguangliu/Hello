#ifndef _SET_EXPIRE_TIME_H
#define _SET_EXPIRE_TIME_H

#include <time.h>

#define  EXPIRE_DATE_FILE           "/Hard Disk/ExpireDate"

extern  int
CalculateExpireDateLeft (SYSTEMTIME  currentDate, unsigned int *expireDays);

extern int
CalculateExpireDays (SYSTEMTIME oldDate, SYSTEMTIME   newDate);

extern int
DetectExpireDateFile (void);


extern int
SetExpireJulianDays (SYSTEMTIME date, unsigned int expireDays);

// expire time data structure
typedef struct
{
    int     expireMonth;
    int     expireDay;
} expireTime_t;



#define MAX_MONTH_LEFT      9
#endif
