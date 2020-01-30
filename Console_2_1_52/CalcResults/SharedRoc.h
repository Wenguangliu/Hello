#ifndef __SHAREDROC_H
#define __SHAREDROC_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "RocData.h"


extern	RotorConfig_t *	rotorConfig_g;


void LoadTestRoc(void);


#if defined(__cplusplus)
}
#endif

#endif	// __SHAREDROC_H
