#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include "Serial.h"
#include "AmsMessages.h"
#include "EngineCommMessages.h"
#include "Shmem.h"
#include "Global.h"



void
HeaterTest (void)
{
    SlayProcess (DRAWER_PROCESS);
    SlayProcess (ENGCOMM_PROCESS);
    SlayProcess (ERRORLOG_PROCESS);


    StartProcess (HEATER_TEST_PROGRAM);

    /* kill Ams_g process */
    ExitProcess (0);
    
}
