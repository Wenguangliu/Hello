// ********************************************************************
//
// FILENAME:  
// 
// 		SharedFactory.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Shared factory file data definition and test load code for algorithm testing
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#include "FactoryData.h"

static FactoryData_t	factoryData;
FactoryData_t *			factoryData_g = &factoryData;

#ifndef UNDER_CE
// ********************************************************************
// FUNCTION:
//
//		LoadTestFactory() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load test data to structure for algorithm testing 
// _____________________________________________________________________
// PARAMETERS:
//
//		none
// ____________________________________________________________________
// RETURNED:
//
//		none
// 
// ********************************************************************/


void LoadTestFactory(void)
{
	unsigned	i;

// DLR original rotor 52 format 1
/*
	float	bfdCal[MAX_BFD_CALS] =
	{
		1.0064669, 1.0068263, 1.0265981, 1.0229095,
		1.0135733, 1.0131121, 1.0058143, 1.0031097,
		1.0092496, 1.0020233, 1.0046364, 1.0013826,
		1.0028387, 1.0097843, 1.0082685, 1.0      ,
		1.0      , 0.9869692, 1.0      , 1.0      ,
		1.0      , 1.0070403, 1.0026025, 1.0132256,
		1.004109 , 1.0030534, 1.0196593, 1.0100914,
		1.0009066, 1.0009981, 1.0      , 1.0
	};
*/

// DLR put current bfd values from crunch template here

	float	bfdCal[MAX_BFD_CALS] =
	{
		1.0068549,	1.0022122,	1.018398 ,	1.0634419,
		0.9964783,	1.0142565,	1.0188084,	0.9988199, 
		1.0041472,	1.0089592,	1.0008119,	0.9984739, 
		1.0014216,	1.004427 ,	1.0029708,	1.0      ,
		1.0      ,	0.9770536,	1.0      ,	1.035    ,
		1.036    ,	0.9987574,	1.0073203,	0.9993134,
		1.0112163,	0.9982357,	0.9772885,	1.0082026,
		0.9993619,	1.0059903,	1.0      ,	1.0
	};




	unsigned short flashLampThresholds[NUM_WAVELENGTHS] = 
	{
		20000,
		20000,
		20000,
		20000,
		20000,
		20000,
		20000,
		20000,
		20000,
		20000
	};


	for (i = 0; i < MAX_BFD_CALS; i++)
	{
		factoryData_g->bfdCalibrationFactors[i] = bfdCal[i];
	}

	for (i = 0; i < NUM_WAVELENGTHS; i++)
	{
		factoryData_g->flashLampThresholds[i] = flashLampThresholds[i];
	}

	factoryData_g->airTemperatureOffset = 0.500;
//	factoryData_g->airTemperatureOffset = 0.000;

	factoryData_g->bfdAvgCurveFactors = (float) 0x8000;
}
#endif
