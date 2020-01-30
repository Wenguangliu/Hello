#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

#include "Global.h"


/*
 * calibration touch screen
 */
void
TouchScreenCal (void)
{
    StartProcess (TOUCH_SCREEN_PROGRAM);
}


