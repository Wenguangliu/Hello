#ifndef __TEMPERATURE_H_INCLUDED
#define __TEMPERATURE_H_INCLUDED

#include "HeaterGlobals.h"


// temperature types

#define ROTOR_TEMPERATURE				0
#define AMBIENT_TEMPERATURE				1
#define TOP_PLATE_TEMPERATURE			2
#define BOTTOM_PLATE_TEMPERATURE		3
#define TOP_PLATE_VOLTAGE				4
#define BOTTOM_PLATE_VOLTAGE			5


float	CalcTemperature(unsigned, RawTemperatures_t*);

#endif	// __TEMPERATURE_H_INCLUDED
