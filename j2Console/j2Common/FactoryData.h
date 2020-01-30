#ifndef __FACTORYDATA_H_INCLUDED
#define __FACTORYDATA_H_INCLUDED

#include "CommonDefines.h"

#pragma pack(push, 1) // exact fit - no padding

typedef struct
{
	float			airTemperatureOffset;
	float			cuvetteTransmitLimit;
	float			bfdCalibrationFactors[ MAX_BFD_CALS ];
	float			bfdAvgCurveFactors;
	unsigned short	wavelengthFilterCurves[ NUM_WAVELENGTHS - 1 ][ MAX_BFD_CALS ];
	unsigned short	globalCuvetteDelay;
	unsigned short	cuvetteDelays[ NUM_CUVETTES ];
	unsigned short	flashLampThresholds[ NUM_WAVELENGTHS ];
	unsigned short	heaterDacSettings[ NUM_HEATER_DAC_SETTINGS ];
	unsigned char	fanTemperatureSetting;
	char			instrumentSerialNumber[ SERIAL_NUMBER_LEN ];
	char			abaxisProductCode;
} FactoryData_t;

#pragma pack(pop) //back to whatever the previous packing mode was

#endif	// __FACTORYDATA_H_INCLUDED
