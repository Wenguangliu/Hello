#ifndef __PARSECOUNTERS_H_INCLUDED
#define __PARSECOUNTERS_H_INCLUDED

#include "CounterData.h"
#include "CfgErr.h"

CfgError_t ParseCounters( CounterData_t * counterData );
CfgError_t WriteCounters( CounterData_t * counterData );

#endif	// __PARSECOUNTERS_H_INCLUDED
