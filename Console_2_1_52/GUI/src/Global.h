/* Header "global.h" for Isis Application */

#ifndef __GLOBAL_H_INCLUDED
#define __GLOBAL_H_INCLUDED

#include "Controller.h"
#include "EnterId.h"
#include "EnterRange.h"
#include "EnterTime.h"
#include "ViewResults.h"
#include "UpdateDateTime.h"
#include "Comm.h"

extern Controller *		controller_g;
extern unsigned short	cursorType_g;
extern double			inactionTime_g;
extern bool				screenSaveEnabled_g;
extern bool				screenSaveActive_g;
extern bool				errorScreenActive_g;
extern SettingsData_t *	settingsData_g;

#endif
