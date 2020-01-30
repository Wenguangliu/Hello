#ifndef __SHAREDCAL_H
#define __SHAREDCAL_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "CalData.h"


extern	CalibrationData_t *	calibrationData_g;


void LoadTestCal(void);


#if defined(__cplusplus)
}
#endif

#endif	// __SHAREDCAL_H
