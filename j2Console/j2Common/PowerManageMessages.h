#ifndef __POWERMANAGEMESSAGES_H_INCLUDED
#define __POWERMANAGEMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define POWER_MANAGE_QUEUE_NAME L"/PowerManage"

/*
 * Message IDs for messages 
 */
typedef enum 
{
    MSG_SET_POWER_IDLE = MSG_FIRST_POWER_MANAGE_ID,
    MSG_SET_POWER_ON,
} PowerManageId_t;

typedef struct
{
    PowerManageId_t         msgId;
    unsigned int            data;       // for the future use
} PowerManageMsg_t;


#endif
