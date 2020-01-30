#ifndef __SHAREDRAW_H
#define __SHAREDRAW_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "RawData.h"


extern	RotorRawReadings_t *	rotorRawReadings_g;


unsigned JulianDate( char month, char day, unsigned short year );

void LoadTestRaw( char * fileName );


#if defined(__cplusplus)
}
#endif

#endif	// __SHAREDRAW_H
