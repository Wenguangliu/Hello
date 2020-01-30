#ifndef __SHAREDFACTORY_H
#define __SHAREDFACTORY_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "FactoryData.h"


extern	FactoryData_t *	factoryData_g;


void LoadTestFactory(void);


#if defined(__cplusplus)
}
#endif

#endif	// __SHAREDFACTORY_H
