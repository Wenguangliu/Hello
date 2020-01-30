/*
 * FILENAME
 * 	HeaterControlMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the HeaterControl process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __HEATERCONTROLMESSAGES_H_INCLUDED
#define __HEATERCONTROLMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"

#define HEATER_CONTROL_QUEUE_NAME	L"/HeaterControl"

/*
 * Message IDs for messages received by the HeaterControl process.
 */
typedef enum
{
	MSG_HEATER_CONTROL_TIMER = MSG_FIRST_HEATER_CONTROL_ID,
	MSG_HEATER_CONTROL_TEMPERATURES,
	MSG_HEATER_CONTROL_START_ANALYSIS,
	MSG_HEATER_CONTROL_MIX_DONE,
	MSG_HEATER_CONTROL_ANALYSIS_DONE,
	MSG_HEATER_CONTROL_WATCH_DOG_ENABLE,
	MSG_HEATER_CONTROL_WATCH_DOG_DISABLE,
	
//	MSG_HEATER_CONTROL_OUT_TEMPERATURE,
//	MSG_HEATER_CONTROL_OUT_ERROR,
//	MSG_HEATER_CONTROL_OUT_WARMUP_DONE,
//	MSG_HEATER_CONTROL_OUT_OVERTEMP,

    MSG_HEATER_GET_TEMPERATURE_VALUES,
    MSG_HEATER_SET_TEMPERATURE_VALUES,
    MSG_HEATER_SEND_TEMPERATURE_VALUES,
    MSG_HEATER_TEST_ENABLE,
    MSG_HEATER_TEST_DISABLE,
    MSG_HEATER_TURN_OFF_HEATER,
    MSG_HEATER_LAST_COMMAND,
} HeaterControlMsgId_t;

typedef struct
{
    float                   rotorTemperature;
    float                   ambientTemperature;
    float                   topPlateTemperature;
    float                   bottomPlateTemperature;
    float                   topPlateVoltage;
    float                   bottomPlateVoltage;
    float                   rotorTemperatureOffset;
    int                     ambientDacOffset;
    unsigned short          topHeaterDacValue;
    unsigned short          bottomHeaterDacValue;
} HeaterValues_t;

typedef struct
{
    unsigned char           topDacValue;
    unsigned char           bottomDacValue;
} HeaterDacSetValues_t;

typedef struct
{
	HeaterControlMsgId_t	msgId;
	char				    data[4096 - sizeof( HeaterControlMsgId_t )];	
} HeaterControlMsg_t;

#endif	/* __HEATERCONTROLMESSAGES_H_INCLUDED */

