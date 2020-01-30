#ifndef _STARTUP_H_
#define _STARTUP_H_

//kill all flag
typedef enum {
    GOTO_MANUFACTURE,
    WATCHDOG_STOP,
    GOTO_REBOOT,
} killAllFlag_t;

//warning and error state machine
typedef enum {
   MESSAGE_STATE = 1,
   INFOMATION_STATE,
} engineCodeSyncState_t;


#define MAX_STARTUP_TIME    30 // # seconds to wait for Engine, GUI,
#define BILLION             1000000000LL;

#endif
