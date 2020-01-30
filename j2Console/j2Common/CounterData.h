#ifndef __COUNTERDATA_H_INCLUDED
#define __COUNTERDATA_H_INCLUDED

//#include <stdint.h>

typedef struct
{
	UINT32	rotorRunCount;
	UINT32	rotorAbortCount;
	UINT32	printCount;
	UINT32	flashCount;
	UINT32	missedFlashCount;
	UINT32	camCycleCount;
	UINT32	drawerCycleCount;
	UINT32	spindleRevolutionCount;
} CounterData_t;

#endif	// __COUNTERDATA_H_INCLUDED
