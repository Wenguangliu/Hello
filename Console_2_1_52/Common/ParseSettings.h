#ifndef __PARSESETTINGS_H_INCLUDED
#define __PARSESETTINGS_H_INCLUDED

#include "SettingsData.h"
#include "CfgErr.h"

CfgError_t ParseSettings( SettingsData_t * settingsData_t, unsigned * version = 0 );
CfgError_t WriteSettings( SettingsData_t * settingsData );

#define SETTINGS_FILE_VERSION	1

#endif	// __PARSESETTINGS_H_INCLUDED
