#ifndef __COUNTERDATA_H_INCLUDED
#define __COUNTERDATA_H_INCLUDED

#include <stdint.h>

typedef struct
{
	uint32_t	rotorRunCount;
	uint32_t	rotorAbortCount;
	uint32_t	printCount;
	uint32_t	flashCount;
	uint32_t	missedFlashCount;
	uint32_t	camCycleCount;
	uint32_t	drawerCycleCount;
	uint32_t	spindleRevolutionCount;
} CounterData_t;

#endif	// __COUNTERDATA_H_INCLUDED
