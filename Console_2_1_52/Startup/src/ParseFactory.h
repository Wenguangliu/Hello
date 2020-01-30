#ifndef __PARSEFACTORY_H_INCLUDED
#define __PARSEFACTORY_H_INCLUDED

#include "CommonDefines.h"
#include "FactoryData.h"
#include "CfgErr.h"

CfgError_t ParseFactory( FactoryData_t * factoryData );
CfgError_t ParseFactory( FactoryData_t * factoryData, unsigned * verson );


#endif	// __PARSEFACTORY_H_INCLUDED
