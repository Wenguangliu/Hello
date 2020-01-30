// ********************************************************************
//
// FILENAME:  
// 
// 		Temperature.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		System temperature calculations
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "SharedFactory.h"
#include "Temperature.h"
#include "AlgGlobals.h"


#define	PLATE_TEMPERATURE_ADC_CONVERT_MULT		0.0010557433	// 1.0557433 millidegree C per plate ADC count
#define	PLATE_TEMPERATURE_ADC_CONVERT_OFFSET	7.758012		// 7.758012 degree C plate temperature ADC convert offset
#define	PLATE_VOLTAGE_ADC_COVERT_MULT			0.000625		// 62.5 microvolts per plate ADC count	

#define ROTOR_TEMPERATURE_CONVERT_MULT			0.00625			// 6.25 millidegree C per rotor temperature sensor ADC count

#define AMBIENT_TEMPERATURE_CONVERT_MULT		0.0125			// 12.5 millidegree C per ambient temperature sensor ADC count
#define AMBIENT_TEMPERATURE_CONVERT_OFFSET		273.15			// 0.0 degrees C is 273.15 degrees Kelvin offset for ambient temperature sensor			


// ********************************************************************
// FUNCTION:
//
//		CalcPlateTemp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a plate temperature from average adc reading (avg of 16 in Engine)
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

float CalcPlateTemp(unsigned short avgAdc)
{ 
	return ((float)avgAdc * PLATE_TEMPERATURE_ADC_CONVERT_MULT) + PLATE_TEMPERATURE_ADC_CONVERT_OFFSET; 
}


// ********************************************************************
// FUNCTION:
//
//		CalcPlateVoltage() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a plate voltage from avg adc reading (avg of 16 in Engine)
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

float CalcPlateVoltage(unsigned short avgAdc)
{
			return (float)avgAdc * PLATE_VOLTAGE_ADC_COVERT_MULT;
}


// ********************************************************************
// FUNCTION:
//
//		CalcTemperature() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a temperature/voltage from raw adc avg reading (avg of 16 in Engine) & 
//		selected type of temperature/voltage using Engine temperature record
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

float	CalcTemperature(unsigned type, RawTemperatures_t *rawTemperaturesPtr)
{
	float	temperature;

	switch (type)
	{
		case ROTOR_TEMPERATURE:
			temperature = ((float)rawTemperaturesPtr->rotorTemperature * ROTOR_TEMPERATURE_CONVERT_MULT) + factoryData_g->airTemperatureOffset;
			break;
		case AMBIENT_TEMPERATURE:
			temperature = ((float)rawTemperaturesPtr->ambientTemperature * AMBIENT_TEMPERATURE_CONVERT_MULT) - AMBIENT_TEMPERATURE_CONVERT_OFFSET;
			break;
		case TOP_PLATE_TEMPERATURE:
			temperature = CalcPlateTemp(rawTemperaturesPtr->topPlateTemperature); 
			break;
		case BOTTOM_PLATE_TEMPERATURE:
			temperature = CalcPlateTemp(rawTemperaturesPtr->bottomPlateTemperature); 
			break;
		case TOP_PLATE_VOLTAGE:
			temperature = CalcPlateVoltage(rawTemperaturesPtr->topPlateCurrent);
			break;
		case BOTTOM_PLATE_VOLTAGE:
			temperature = CalcPlateVoltage(rawTemperaturesPtr->bottomPlateCurrent);
			break;
		default:
			temperature = NOVAL_NEG_FLOAT;
			break;
	}

	return temperature;
}

// GLD
// This is the old version - remove if all is well with the above.
#if 0
// ********************************************************************
//
// FILENAME:  
// 
// 		Temperature.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		System temperature calculations
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "SharedFactory.h"
#include "Temperature.h"


#define	AIR_TEMP_DIVISOR				160.82		// (was 72.82) ADC reading to temp conversion factor
#define	ADC_COUNTS_PER_VOLT				7281.78	
#define	PLATE_VOLT_OFFSET				0.44	
#define	PLATE_VOLTS_PER_DEGREE			0.059	


// ********************************************************************
// FUNCTION:
//
//		CalcPlateTemp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a plate temperature from average adc reading (avg of 16 in Engine)
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

float CalcPlateTemp(unsigned short avgAdc)
{ 
	return (((float)avgAdc / ADC_COUNTS_PER_VOLT) + PLATE_VOLT_OFFSET) / PLATE_VOLTS_PER_DEGREE; 
}


// ********************************************************************
// FUNCTION:
//
//		CalcPlateCurrent() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a plate current from avg adc reading (avg of 16 in Engine)
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

float CalcPlateVoltage(unsigned short avgAdc)
{
			return (float)avgAdc / ADC_COUNTS_PER_VOLT;
}


// ********************************************************************
// FUNCTION:
//
//		CalcTemperature() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate a temperature from raw adc avg reading (avg of 16 in Engine) & 
//		selected type of temperature using Engine temperature record
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

float	CalcTemperature(unsigned type, RawTemperatures_t *rawTemperaturesPtr)
{
	float	temperature;

	switch (type)
	{
		case ROTOR_TEMPERATURE:
			temperature = ((float)rawTemperaturesPtr->rotorTemperature / AIR_TEMP_DIVISOR) + factoryData_g->airTemperatureOffset;
			break;
		case AMBIENT_TEMPERATURE:
			temperature = (float)rawTemperaturesPtr->ambientTemperature / AIR_TEMP_DIVISOR;
			break;
		case TOP_PLATE_TEMPERATURE:
			temperature = CalcPlateTemp(rawTemperaturesPtr->topPlateTemperature); 
			break;
		case BOTTOM_PLATE_TEMPERATURE:
			temperature = CalcPlateTemp(rawTemperaturesPtr->bottomPlateTemperature); 
			break;
		case TOP_PLATE_VOLTAGE:
			temperature = CalcPlateVoltage(rawTemperaturesPtr->topPlateTemperature);
			break;
		case BOTTOM_PLATE_VOLTAGE:
			temperature = CalcPlateVoltage(rawTemperaturesPtr->bottomPlateTemperature);
			break;
		default:
			temperature = NOVAL_NEG_FLOAT;
			break;
	}

	return temperature;
}
#endif
