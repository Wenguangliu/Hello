#ifndef _PHOTO_TEST_H
#define _PHOTO_TEST_H

#include "GuiMessages.h"

#define PHOTO_TEST_QUEUE_NAME           "/PhotoTest"    /* Gui send message queue to photo process */
#define GUI_PHOTO_TEST_QUEUE_NAME       "/GuiPhotoTest" /* photo process send  queue to Gui process */
#define PHOTO_TEST_ENG_RCV_QUEUE_NAME   "/PhotoTestEng"   /* photo test received queue from engine comm */

/* send message to photo process for GUI initialization is ready */
#define MSG_PHOTO_GUI_READY     (MSG_GUI_LAST_OUT + 1)

typedef struct 
{
    int     msgId;
    char    sdata [100];
} TestStateSendMsg_t;

typedef enum 
{
    PHOTO_TEST_START_CMD        = MSG_PHOTO_GUI_READY,
    PHOTO_TEST_CD_READ_ERROR ,
    PHOTO_TEST_LAST_CMD,
    PHOTO_TEST_SET_TIMER,
    PHOTO_TEST_CD_UPDATING,
    PHOTO_TEST_NEXT_RUN_TIME,
    PHOTO_TEST_SAVE_DATA_CMD,
} PhotoTestCommand_t;

#endif
