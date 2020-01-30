#ifndef __STANDARDDEVIATION_H
#define __STANDARDDEVIATION_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "RawData.h"


unsigned LinearRegression(float*, float*, unsigned, float*, float*);

unsigned StandardError(float*, float*, unsigned, float, float, float*);

float StandardDeviation(float *points, unsigned numPoints);

float PercentCv(float *points, unsigned numPoints);


#if defined(__cplusplus)
}
#endif

#endif	// __STANDARDDEVIATION_H
