#ifndef __PARSESYSTEM_H_INCLUDED
#define __PARSESYSTEM_H_INCLUDED

#include "CommonDefines.h"
#include "SystemData.h"
#include "CfgErr.h"

CfgError_t ParseSystem (SystemData_t *systemData);
CfgError_t WriteSystem (SystemData_t *systemData);


#endif	// __PARSESYSTEM_H_INCLUDED
