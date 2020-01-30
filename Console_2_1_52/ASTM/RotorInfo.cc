/*
 * FILENAME
 * 	RotorInfo.c
 * 
 * MODULE DESCRIPTION
 * 	This file contains code for the class RotorInfo, which provides rotor information used in the
 * 	order process. Specifically, it provides the rotor product code, plus information for displaying
 *  rotor names and the colors to display those names. This information is retrieved from a
 * 	configuration file at startup.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CommonDefines.h"
#include "RotorInfo.h"
#include "ParseRotorInfo.h"
#include "CfgErr.h"
#include "LogErr.h"

RotorInfo_t	RotorInfo::rotorInfo[100];

RotorInfo::
RotorInfo()
{
	CfgError_t	cfgErr;

	cfgErr = ParseRotorInfo( rotorInfo );

	if ( cfgErr != CFG_ERR_NONE )
	{
		LogError( "error %d parsing rotor info file", cfgErr );
	}
}