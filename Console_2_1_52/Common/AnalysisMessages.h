/*
 * FILENAME
 * 	AnalysisMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the Analysis process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __ANALYSISMESSAGES_H_INCLUDED
#define __ANALYSISMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "CommonDefines.h"

#define ANALYSIS_QUEUE_NAME	"/Analysis"

/*
 * Message IDs for messages received by the Analysis process.
 */
typedef enum
{
	MSG_ANALYSIS_GOTO_FOREGROUND = MSG_FIRST_ANALYSIS_ID,
	MSG_ANALYSIS_RESULTS_CALC_COMPLETE,
	MSG_ANALYSIS_DAC_TRIMS_COMPLETE,
	MSG_ANALYSIS_TIMER,
	MSG_ANALYSIS_ERROR,
	MSG_ANALYSIS_TEMPERATURE,
	MSG_ANALYSIS_OVERTEMP,
	MSG_ANALYSIS_WARMUP_DONE,
	MSG_ANALYSIS_TRIGGER,				// Used to trigger a state change from an Enter method.
	MSG_ANALYSIS_SKIP_BARCODE,			// Skip barcode read for a Rotor 0.
	MSG_ANALYSIS_UPLOADED_ROTOR,		// Data for special rotor uploaded by PC.
	MSG_ANALYSIS_UPLOADED_ORDER,		// Data for order uploaded by ASTM
} AnalysisMsgId_t;

typedef struct
{
	int			heaterState;
	unsigned	timer;
	float		setPoint;
	float		temperature;
} AnalysisTemperatureData_t;

typedef struct
{
	int			overtempFlag;
	float		rotorTemperature;
	float		ambientTemperature;
	float		topPlateTemperature;
	float		bottomPlateTemperature;
	float		topPlateVoltage;
	float		bottomPlateVoltage;
} AnalysisOvertempData_t;

typedef struct
{
	AnalysisMsgId_t	msgId;
	union
	{
		AnalysisTemperatureData_t	temperatureData;
		AnalysisOvertempData_t		overtempData;
		UploadedOrder_t				uploadedOrder;
		char						rotorName[ UPLOADED_ROTOR_NAME_LEN ];
		int							idata;
	};
} AnalysisMsg_t;

#endif	/* __ANALYSISMESSAGES_H_INCLUDED */

