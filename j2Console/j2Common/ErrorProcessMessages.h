#ifndef _ERRORPROCESSMESSAGES_H
#define _ERRORPROCESSMESSAGES_H
#include "ConsoleMessages.h"

#define ERROR_PROCESS_QUEUE_NAME        L"/ErrorProcess"

typedef enum
{
   MSG_WARNING_REPORT = MSG_FIRST_ERROR_PROCESS_ID,
   MSG_ERROR_REPORT,
} ErrorProcessMsgId_t;

typedef struct
{
    char            processName [120];
    unsigned int    errorNumber;
    unsigned int    retState;
} ErrorProcessMsgData_t;

typedef struct
{
    ErrorProcessMsgId_t     msgId;
    ErrorProcessMsgData_t   data;

} ErrorProcessMsg_t;


#endif
