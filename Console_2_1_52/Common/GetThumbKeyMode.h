#ifndef _GETTHUMBKEYMODE_H_
#define _GETTHUMBKEYMODE_H_

#include "SystemData.h"

typedef struct
{
	int		count;
	char	serNum[4][256];
} UsbSerNumList_t;

void				GetThumbKeyMode( SystemData_t * systemData );
int					LockoutKeyInstalled();
UsbSerNumList_t *	GetUsbSerNums();

#endif	// _GETTHUMBKEYMODE_H_
