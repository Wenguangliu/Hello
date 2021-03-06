#ifndef _MANUFACTUREMESSAGES_H
#define _MANUFACTUREMESSAGES_H
#include "ConsoleMessages.h"

#define MANUFACTURE_QUEUE_NAME      L"/Manufacture"

typedef enum
{
    MSG_MANUFACTURE_GOTO_FOREGROUND = MSG_FIRST_MANUFACTURE_ID,
    MSG_MANUFACTURE_TIMER,
    MSG_MANUFACTURE_DAC_TRIMS_COMPLETE,
    MSG_MANUFACTURE_MOTOR_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_NDXT_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_TEMP_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_DELAY_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_BARCODE_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_ALIGNMENT_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_CUVETTE_TEST_CAL_COMPLETE,
    MSG_MANUFACTURE_NEXTWORK_CONFIG_COMPLETE,
    MSG_MANUFACTURE_WARMUP_DONE,
} ManufactureId_t;

typedef struct 
{
    ManufactureId_t msgId;
    int             idata;
} ManufactureMsg_t;

#endif
