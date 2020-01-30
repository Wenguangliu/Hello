#ifndef __HEATER_GLOBALS_H_INCLUDED
#define __HEATER_GLOBALS_H_INCLUDED

#include "FactoryData.h"

#define	NOVAL_NEG_FLOAT		-999999;
#define	NOVAL_POS_FLOAT		999999;


#define TOP_HEATER_DAC_DEFAULT		0
#define TOP_HEATER_DAC_HIGH			1
#define BOTTOM_HEATER_DAC_DEFAULT	2
#define BOTTOM_HEATER_DAC_HIGH		3


typedef struct
{
	unsigned long	engineTime;
	unsigned short	rotorTemperature;
	unsigned short	ambientTemperature;
	unsigned short	topPlateTemperature;
	unsigned short	bottomPlateTemperature;
	unsigned short	topPlateCurrent;
	unsigned short	bottomPlateCurrent;
} RawTemperatures_t;


extern	FactoryData_t	* factoryData_g;


// while forever loop define

#define POPE_IS_CATHOLIC		true

#endif	// __HEATER_GLOBALS_H_INCLUDED
