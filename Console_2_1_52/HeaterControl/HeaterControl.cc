// ********************************************************************
//
// FILENAME:  
// 
// 		HeaterControl.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console process main() that implements Engine heater control processor
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include <stdio.h>	// for terminal window debugging - to be removed later
#include <stdlib.h>
#include <sys/neutrino.h>
#include <mqueue.h>
#include <string.h>
#include <sys/mman.h>

#include "HeaterGlobals.h"
#include "Temperature.h"
#include "HeaterControlMessages.h"
#include "EngineCommMessages.h"
#include "AnalysisMessages.h"
#include "CommonDefines.h"
#include "Shmem.h"
#include "SystemError.h"
#include "LogErr.h"


FactoryData_t * factoryData_g = 0;
int	ambientplateDacOffset_m;

// heater control defintions

#define HEATER_DAC_OFF						0
#define HEATER_DAC_MAX						0xff

#define FAN_OFF								0xff


#define	LOW_TOP_PLATE_CURRENT				0.0		// 0.68		// amps	
#define	HIGH_TOP_PLATE_CURRENT				10.0	// 2.351	// amps
#define	LOW_BOTTOM_PLATE_CURRENT			0.0		// 0.954	// amps	
#define	HIGH_BOTTOM_PLATE_CURRENT			10.0	// 3.319	// amps

#define INIT_WARMING_KICK					40



#define AMB_TEMP_MATCH_OFFSET_L_1            7.5    // match NGA to J2 Ambient temperature adjustment in low temperature 
#define AMB_TEMP_MATCH_OFFSET_L_2            10     // match NGA to J2 Ambient temperature adjustment in low temperature 
#define AMB_TEMP_MATCH_OFFSET_M_1            9.0    // match NGA to J2 Ambient temperature adjustment in middle temperature 
#define AMB_TEMP_MATCH_OFFSET_M_2            8.5    // match NGA to J2 Ambient temperature adjustment in middle temperature 
#define AMB_TEMP_MATCH_OFFSET_H              7.5    // match NGA to J2 Ambient tempetature adjustment in high   tempeature 

#define AMB_TEMP_MATCH_L_1_THRESHOLD         23
#define AMB_TEMP_MATCH_L_2_THRESHOLD         26.5
#define AMB_TEMP_MATCH_M_1_THRESHOLD         27.5
#define AMB_TEMP_MATCH_M_2_THRESHOLD         30 

#define MAX_OFFSET_STEPS                     16


#define	PLATE_TEMPERATURE_DAC_CONVERT_MULT		0.197952	// 197.952 millidegree C per plate DAC count
#define	PLATE_TEMPERATURE_DAC_CONVERT_OFFSET	7.758012	// 7.758012 degree C plate temperature DAC convert offset
#define PLATE_TEMPERATURE_THRESHOLD_FACTOR		0.90		// threshold at 90% of setpoint

#define PLATE_CURRENT_CONVERT_MULT				0.000625	// 625 microamps per plate current ADC count

#define	TOP_PLATE_TOO_HOT					38.5

#define	LOW_AMBIENT_TEMPERATURE_LIMIT		13.0	// operation temperature limits	
#define	ROTOR_AIR_SENSOR_THRESHOLD			33.5
#define	ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR	35.0
#define	DEAD_AIR_SENSOR_THRESHOLD			5.0


 
// NGA temperature DAC offset table based on J2 DAC offset table
 
typedef struct
{
    float       ambTemp;
    char        offset;
} ngaToJ2Offset_t;

ngaToJ2Offset_t ngaToJ2OffsetTable [] =
                            {
                                {23.5,      7},
                                {24.5,      6},
                                {25.5,      4},
                                {28.0,      2},
                                {29.0,      1},
                                {31.0,      0},
                                {32.0,      -1},
                                {33.0,      -2},
                                {34.0,      -3},
                                {35.0,      -4},
                                {36.0,      -5},
                                {37.0,      -6},
                                {37.6,      -7},
                                {45.0,      -9},
                                {45.0,      -9},        // extra 2 entry for identical max 16 steps with J2 offset table 
                                {45.0,      -9},

                            };


// heater states

typedef enum
{
	NO_HEATER_STATE,

	INIT_HEATER_STATE,

	ENGINE_WARMING_INIT_STATE,
	ENGINE_WARMING_STATE,

	HEATER_IDLING_STATE,

	ANALYSIS_INIT_STATE,
	ANALYSIS_PRE_WARMING_MIX_STATE,
	ANALYSIS_CALC_WARMING_TIME_STATE,
	ANALYSIS_WARMING_STATE,
	ANALYSIS_WAITING_MIX_DONE_STATE,
	ANALYSIS_WAITING_DONE_STATE
} HeaterState_t;


// Engine heater settings used to calc rotor temperature kick time for cold rotors

typedef struct
{
	unsigned	high_temperature_kick_time;
	unsigned	low_temperature_kick_time;
	unsigned	high_temperature_kick_limit;
	unsigned	low_temperature_kick_limit;
	unsigned	max_kick_time_after_mix;
} HeaterKickTimeSettings_t;

HeaterKickTimeSettings_t heaterKickTimeSettings_m; 

// ambient offset to be added to top and bottom plate DAC values for heater control during rotor run 


// Current image of heater control Engine output DAC values
  
typedef struct
{
	unsigned char	topPlateDac;
	unsigned char	bottomPlateDac;
	unsigned char	fanControlDac;
} HeaterControl_t;

HeaterControl_t		heaterControl_m;		// current image of heater control setting DACs (Console	to Engine)


// Engine received raw temperature related readings

RawTemperatures_t	rawTemperatures_m;


typedef struct
{
	float	rotorTemperature;
	float	ambientTemperature;
	float	topPlateTemperature;
	float	bottomPlateTemperature;
	float	topPlateVoltage;
	float	bottomPlateVoltage;
} HeaterData_t;

HeaterData_t		heaterData_m;			// calculated heater values from raw readings

// used top & bottom heater plate DAC settings from factory.cfg

unsigned char	topPlateDacNormal_m;
unsigned char	bottomPlateDacNormal_m;
unsigned char	bottomPlateDacHigh_m;

// fan DAC setting from factory.cfg

unsigned char	fanDacOn_m;


// heater plate temperature threshold calculated compare values

float	topPlateTemperatureThreshold_m;
float	bottomPlateTemperatureThreshold_m;


// heater control delay timer variables

unsigned	heaterControlTimer_m;
unsigned	heaterControlDelayTimeSet_m;

// delay timer values

unsigned	analysisRotorHeatKickDelay_m;

#define	DELAY_10S				2
#define	DELAY_30S				6

#define SECONDS_5_DIVISOR		5



// *********************************************************************
// FUNCTION:
//
//     NgaToJ2Offset 
// _____________________________________________________________________
// DESCRIPTION:
//
//     Lookup DAC offset table 
// _____________________________________________________________________
// PARAMETERS:
//
//      none
// ____________________________________________________________________
// RETURNED:
//
//      none
//
// ********************************************************************/

char NgaToJ2Offset (float ambTemp)
{
    // max negative offset (ambTemp >= 45.0)
    char offset = -9;
    int  i;

    for (i = 0; i < MAX_OFFSET_STEPS; i++) 
    {
        if (ambTemp < ngaToJ2OffsetTable [i].ambTemp) 
        {
            offset = ngaToJ2OffsetTable [i].offset;
            break;
        }
    }
     
    return offset;
}

// *********************************************************************
// FUNCTION:
//
//     GetNgaMatchToJ2DacOffset 
// _____________________________________________________________________
// DESCRIPTION:
//
//      Get DAC offset based on ngaToJ2  ambient temperature-DAC table
// _____________________________________________________________________
// PARAMETERS:
//
//      none
// ____________________________________________________________________
// RETURNED:
//
//      none
//
// ********************************************************************/
int GetNgaMatchToJ2DacOffset (float ambTemp)
{
    char    offset = 0;
    float   tempMatchOffset = 0;


    // get DAC offset value
    offset = NgaToJ2Offset (ambTemp);

    // NGA match to Jen2  temperature, for debug purpose only
    if (ambTemp < AMB_TEMP_MATCH_L_1_THRESHOLD) 
    {
        tempMatchOffset = AMB_TEMP_MATCH_OFFSET_L_1;
    } 
    else if (ambTemp < AMB_TEMP_MATCH_L_2_THRESHOLD) 
    {
        tempMatchOffset = AMB_TEMP_MATCH_OFFSET_L_2;
    } 
    else if (ambTemp < AMB_TEMP_MATCH_M_1_THRESHOLD) 
    {
        tempMatchOffset = AMB_TEMP_MATCH_OFFSET_M_1;
    } 
    else if (ambTemp < AMB_TEMP_MATCH_M_2_THRESHOLD) 
    {
        tempMatchOffset = AMB_TEMP_MATCH_OFFSET_M_2;
    } 
    else  
    {
        tempMatchOffset = AMB_TEMP_MATCH_OFFSET_H;
    }

    ambTemp += tempMatchOffset;

    LogDebug ("ambTemp =%.3f offset = %d", ambTemp, offset);

    return offset;

}

// *********************************************************************
// FUNCTION:
//
//		TimerThread() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sends a message to this process every 5 seconds. 
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

void * TimerThread( void * arg )
{
	const int msg = MSG_HEATER_CONTROL_TIMER;

	static mqd_t queue = mq_open( HEATER_CONTROL_QUEUE_NAME, O_WRONLY );
	
	if ( queue == -1 )
	{
		LogError( "failure to open HeaterControl queue in TimerThread" );
		exit( -1 );
	}

	while ( 1 )
	{
		sleep( 5 );
	
		mq_send( queue, (const char *)&msg, sizeof( int ), MQ_PRIO_MAX-1 );
	}
	
	return 0;
}

// *********************************************************************
// FUNCTION:
//
//		SendEngineMessage() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sends a message to the Engine. 
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

void SendEngineMessage( char command )
{
	EngineCommReceiveMsg_t	msg;
	static mqd_t			queue = -1;
	
	if ( queue < 0 )
	{
		queue = mq_open( ENGINE_COMM_QUEUE_NAME, O_WRONLY );
	
		if ( queue == -1 )
		{
			LogError( " failure to open EngineComm queue" );
			exit( -1 );
		}
	}

	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'H';
	msg.data.engineMsg.message[1] = command;
	msg.data.engineMsg.message[2] = ESC;
	msg.data.engineMsg.message[3] = 3;	// LSB of binary count.
	msg.data.engineMsg.message[4] = 0;	// MSB of binary count.
	msg.data.engineMsg.message[5] = heaterControl_m.topPlateDac;
	msg.data.engineMsg.message[6] = heaterControl_m.bottomPlateDac;
	msg.data.engineMsg.message[7] = heaterControl_m.fanControlDac;
	msg.data.engineMsg.length = 8;
		
	mq_send( queue, (const char *)&msg, sizeof( EngineCommReceiveMsg_t ), MQ_PRIO_MAX-1 );
}

// *********************************************************************
// FUNCTION:
//
//		SendAnalysisMessage() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sends a message to the Analysis process. 
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

void SendAnalysisMessage( AnalysisMsg_t * msg )
{
	static mqd_t	queue = 0;
	
	if ( queue == 0 )
	{
		queue = mq_open( ANALYSIS_QUEUE_NAME, O_WRONLY );
	
		if ( queue == -1 )
		{
			LogError( "failure to open Analysis queue" );
			exit( -1 );
		}
	}
	
	mq_send( queue, (const char *)msg, sizeof( AnalysisMsg_t ), MQ_PRIO_MAX-1 );
}

void SendAnalysisMessage( AnalysisMsgId_t msgId )
{
	AnalysisMsg_t	msg;
	
	msg.msgId = msgId;
	
	SendAnalysisMessage( &msg );
}
	
void SendAnalysisIdataMessage( AnalysisMsgId_t msgId, int idata )
{
	AnalysisMsg_t	msg;

	msg.msgId = msgId;
	msg.idata = idata;
		
	SendAnalysisMessage( &msg );
}

void SendAnalysisOvertempMessage( bool overtempFlag )
{
	AnalysisMsg_t	msg;

	msg.msgId = MSG_ANALYSIS_OVERTEMP;
	msg.overtempData.overtempFlag = overtempFlag;
	msg.overtempData.rotorTemperature = heaterData_m.rotorTemperature;
	msg.overtempData.ambientTemperature = heaterData_m.ambientTemperature;
	msg.overtempData.topPlateTemperature = heaterData_m.topPlateTemperature;
	msg.overtempData.bottomPlateTemperature = heaterData_m.bottomPlateTemperature;
	msg.overtempData.topPlateVoltage = heaterData_m.topPlateVoltage;
	msg.overtempData.bottomPlateVoltage = heaterData_m.bottomPlateVoltage;
	
	SendAnalysisMessage( &msg );
}

// *********************************************************************
// FUNCTION:
//
//		SetDelayTime() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Save current time as set time for delay time comparison 
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

void SetDelayTime(void)
{
	heaterControlDelayTimeSet_m = heaterControlTimer_m; 
}

// *********************************************************************
// FUNCTION:
//
//		DelayTimeUp() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Check if delay time is up
//			Uses modulo unsigned subtract to compare difference in current time
//			versus set time against delay time passed 
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

unsigned DelayTimeUp(unsigned delayTime)
{
	return ((heaterControlTimer_m - heaterControlDelayTimeSet_m) >= delayTime);
}


// *********************************************************************
// FUNCTION:
//
//		SendTemperatureMsg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Send error message from heater control process to QNX process 
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

void SendTemperatureMsg(HeaterState_t heaterState, unsigned timer,  float setPoint, float temperature)
{
	AnalysisMsg_t	msg;

	msg.msgId = MSG_ANALYSIS_TEMPERATURE;
	msg.temperatureData.heaterState = heaterState;
	msg.temperatureData.timer = timer;
	msg.temperatureData.setPoint = setPoint;
	msg.temperatureData.temperature = temperature;
	
	SendAnalysisMessage( &msg );
}


// *********************************************************************
// FUNCTION:
//
//		SendHeaterErrorMsg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Send error message from heater control process to QNX process 
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

void SendHeaterErrorMsg(unsigned short errorNumber)
{
	SendAnalysisIdataMessage( MSG_ANALYSIS_ERROR, errorNumber );
}


// *********************************************************************
// FUNCTION:
//
//		CalcHeaterData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Initialize heater process variables and flags here 
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

void CalcHeaterData(RawTemperatures_t *rawTemperatures, HeaterData_t *heaterData)
{
	heaterData->rotorTemperature       = CalcTemperature(ROTOR_TEMPERATURE, rawTemperatures);
	heaterData->ambientTemperature     = CalcTemperature(AMBIENT_TEMPERATURE, rawTemperatures);
	heaterData->topPlateTemperature    = CalcTemperature(TOP_PLATE_TEMPERATURE, rawTemperatures);
	heaterData->bottomPlateTemperature = CalcTemperature(BOTTOM_PLATE_TEMPERATURE, rawTemperatures);
	heaterData->topPlateVoltage        = CalcTemperature(TOP_PLATE_VOLTAGE, rawTemperatures);
	heaterData->bottomPlateVoltage     = CalcTemperature(BOTTOM_PLATE_VOLTAGE, rawTemperatures);
}


// ********************************************************************
// FUNCTION:
//
//		SetEngineTemperatureControl() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Sets Engine top and bottom plate setpoint & fan DACs
//		by sending 'H message 'S' command to Engine
//		with current DAC parameters	set by heater control state machine
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

void SetEngineTemperatureControl(unsigned char heaterTopPlateDac, unsigned char heaterBottomPlateDac, unsigned char fanControlDac)
{
	// update current image in heaterControl_m

	heaterControl_m.topPlateDac = heaterTopPlateDac;
	heaterControl_m.bottomPlateDac = heaterBottomPlateDac;
	heaterControl_m.fanControlDac = fanControlDac;

	// send 'H' message 'S' command to Engine with current image DAC values   
	SendEngineMessage( 'S' );
}


// ********************************************************************
// FUNCTION:
//
//		RequestEngineHeaterData() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Gets Engine heater data raw readings 
//		by sending 'H message 'R' command to Engine	
//		with current image of DAC parameters set by heater control state machine
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

void RequestEngineHeaterData(void)
{
	SendEngineMessage( 'R' );
}


// ********************************************************************
// FUNCTION:
//
//		WaitForMsg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits for QNX message received for:
//			- 5 second timeout message 	
//			- analysis start message 		
//			- Engine 'H' message & stores rawTemperatures & exits with heaterState			    
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

HeaterControlMsgId_t WaitForMsg(RawTemperatures_t *rawTemperatures)
{
	HeaterControlMsg_t		msg;
	static mqd_t			queue = 0;
	static struct mq_attr	mqstat;
	ssize_t					receivedSize = 0;

	if ( queue == 0 )
	{
		queue = mq_open( HEATER_CONTROL_QUEUE_NAME, O_RDONLY );
	
		if ( queue == -1 )
		{
			LogError( "failure to open HeaterControl queue" );
			exit( -1 );
		}
		
		if ( mq_getattr( queue, & mqstat ) == -1 )
		{
			LogError( "failure to get HeaterControl queue attributes" );
			exit( -1 );
		}
	}
		
	receivedSize = mq_receive( queue, (char *)&msg, mqstat.mq_msgsize, NULL );

	if ( receivedSize != -1 )
	{
		if ( receivedSize > 0 )
		{
			switch ( msg.msgId )
			{
				case MSG_HEATER_CONTROL_TEMPERATURES:
// GLD
// Ought to do a message structure for this!!!!!!
					// copy the temperatures into rawTemperatures
					rawTemperatures->engineTime				= *(unsigned int *)(msg.data +  4);
					rawTemperatures->rotorTemperature		= *(unsigned short *)(msg.data +  8);
					rawTemperatures->ambientTemperature		= *(unsigned short *)(msg.data + 10);
					rawTemperatures->topPlateTemperature	= *(unsigned short *)(msg.data + 12);
					rawTemperatures->bottomPlateTemperature	= *(unsigned short *)(msg.data + 14);
					rawTemperatures->topPlateCurrent		= *(unsigned short *)(msg.data + 16);
					rawTemperatures->bottomPlateCurrent		= *(unsigned short *)(msg.data + 18);
					break;

				case MSG_HEATER_CONTROL_TIMER:
				case MSG_HEATER_CONTROL_START_ANALYSIS:
				case MSG_HEATER_CONTROL_MIX_DONE:
				case MSG_HEATER_CONTROL_ANALYSIS_DONE:
					break;

				default:
					LogWarning( "received unexpected message %d", msg.msgId );
					return (HeaterControlMsgId_t)-1;
					break;
			}
		}
	}
	else
	{
		LogError( "message receive error" );
		return (HeaterControlMsgId_t)-1;
	}

	return msg.msgId;
}	


// ********************************************************************
// FUNCTION:
//
//		WaitForNextHeaterMsg() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits until message received for heater control state processing
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

HeaterControlMsgId_t WaitForNextHeaterMsg(void)
{
	HeaterControlMsgId_t	msgId;

	msgId = WaitForMsg(&rawTemperatures_m);	

	switch (msgId)
	{
		case MSG_HEATER_CONTROL_TIMER:
			heaterControlTimer_m++;		// increment running delay timer
			break;

		case MSG_HEATER_CONTROL_TEMPERATURES:
			CalcHeaterData(&rawTemperatures_m, &heaterData_m);
			break;

		default:
			break;
	}

	return msgId;
}


// ********************************************************************
// FUNCTION:
//
//		WaitForTemperatures() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits until raw temperatures message from Engine received or timeout  
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

unsigned WaitForTemperatures(unsigned short *errorNumber)
{
	HeaterControlMsgId_t	msgId;

	SetDelayTime();

	while (POPE_IS_CATHOLIC)
	{
		msgId = WaitForNextHeaterMsg();

		if (msgId == MSG_HEATER_CONTROL_TEMPERATURES)
		{
			return false;
		}
		else if ( msgId == MSG_HEATER_CONTROL_TIMER )
		{
			if (DelayTimeUp(DELAY_30S))
			{
				// got timeout with no heater temperature data so attempt to shut off heater DACs for safety and exit

				SetEngineTemperatureControl(HEATER_DAC_OFF, HEATER_DAC_OFF, fanDacOn_m);

				// set for Engine communications timeout

				*errorNumber = SERR_MSG_TIMEOUT;

				return true;
			} 
		}
		else
		{
			LogDebug( "WaitForTemperatures received %d message", msgId );
		}
	}
}


// ********************************************************************
// FUNCTION:
//
//		WaitFor5SecTimeout() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Waits until 5 sec timeout message received from QNX process
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

void WaitFor5SecTimeout(void)
{
	int msgId;
	
	while (( msgId = WaitForNextHeaterMsg() ) != MSG_HEATER_CONTROL_TIMER)
	{
		LogDebug( "WaitFor5SecTimeout received %d message", msgId );
	}
}


// ********************************************************************
// FUNCTION:
//
//		HeaterPlatesCurrentTest() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Engine Heater top & bottom plates current testing  
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

unsigned HeaterPlatesCurrentTest(unsigned short *errorNumber)
{
	float	plateCurrent;

	// synch to 5 second timer 

	WaitFor5SecTimeout();

	// test for top heater plate current within limits

	SetEngineTemperatureControl(HEATER_DAC_MAX, HEATER_DAC_OFF, FAN_OFF);

	// wait for top plate filter time constants to decay before reading current

	WaitFor5SecTimeout();

	RequestEngineHeaterData();

	if (WaitForTemperatures(errorNumber))
	{
		return true;
	}

	plateCurrent = rawTemperatures_m.topPlateCurrent * PLATE_CURRENT_CONVERT_MULT;

	if (plateCurrent < LOW_TOP_PLATE_CURRENT) 
	{
		*errorNumber = SERR_TOP_HEATER_LOW_CURRENT;
		return true;
	}
	else if (plateCurrent > HIGH_TOP_PLATE_CURRENT)
	{
		*errorNumber = SERR_TOP_HEATER_HIGH_CURRENT;
		return true;
	}


	// test for bottom heater plate current within limits

	SetEngineTemperatureControl(HEATER_DAC_OFF, HEATER_DAC_MAX, FAN_OFF);

	// wait for bottom plate filter time constants to decay before reading current

	WaitFor5SecTimeout();

	RequestEngineHeaterData();

	if (WaitForTemperatures(errorNumber))
	{
		return true;
	}

	plateCurrent = rawTemperatures_m.bottomPlateCurrent * PLATE_CURRENT_CONVERT_MULT;

	if (plateCurrent < LOW_BOTTOM_PLATE_CURRENT) 
	{
		*errorNumber = SERR_BOTTOM_HEATER_LOW_CURRENT;
		return true;
	}
	else if (plateCurrent > HIGH_BOTTOM_PLATE_CURRENT)
	{
		*errorNumber = SERR_BOTTOM_HEATER_HIGH_CURRENT;
		return true;
	}

	return false;
}


// ********************************************************************
// FUNCTION:
//
//		CalculateHeaterKickTimeRequired() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Calculate time required to kick the heater based on current rotor temperature &
//		high & low time and temperature settings for a standard rotor		 
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

unsigned CalculateHeaterKickTimeRequired(float rotorTemperature)
{
	float	heaterKickTime;

	float	high_temperature_kick_time;
	float	low_temperature_kick_time;
	float	high_temperature_kick_limit;
	float	low_temperature_kick_limit;	 

	high_temperature_kick_time  = (float) heaterKickTimeSettings_m.high_temperature_kick_time;
	low_temperature_kick_time   = (float) heaterKickTimeSettings_m.low_temperature_kick_time;
	high_temperature_kick_limit = (float) heaterKickTimeSettings_m.high_temperature_kick_limit;
	low_temperature_kick_limit  = (float) heaterKickTimeSettings_m.low_temperature_kick_limit;
		 
	// clamp input rotor temperature to low temperature limit & high temperature limit

	if (low_temperature_kick_limit > rotorTemperature)
	{
		rotorTemperature = low_temperature_kick_limit;
	}

	if (high_temperature_kick_limit < rotorTemperature)
	{
		rotorTemperature = high_temperature_kick_limit;
	}

	// calculate linear kick time based on temperature in range of low & high time limits

	heaterKickTime = (((high_temperature_kick_time - low_temperature_kick_time) / (high_temperature_kick_limit - low_temperature_kick_limit))
				      *(rotorTemperature - low_temperature_kick_limit)) + low_temperature_kick_time; 

	if (heaterKickTime < 0.0)
	{
		heaterKickTime = 0.0;
	}

	// exit with total kick time seconds divided by state time divisor

	return ((unsigned)heaterKickTime / SECONDS_5_DIVISOR);
}


// ********************************************************************
// FUNCTION:
//
//		InitializeHeaterProcess() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Initialize heater process variables and flags here 
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

void InitializeHeaterProcess()
{
	// initialize heater control timer

	heaterControlTimer_m = 0;

	// turn OFF heater top & bottom plates and fan

	SetEngineTemperatureControl(HEATER_DAC_OFF, HEATER_DAC_OFF, FAN_OFF);
		
	// set up heater kick time algorithm settings

	heaterKickTimeSettings_m.high_temperature_kick_time  = 0;		// 0 seconds if already at high temperature
	heaterKickTimeSettings_m.low_temperature_kick_time   = 300;		// 300 seconds if at or below low temperature
	heaterKickTimeSettings_m.high_temperature_kick_limit = 35;		// 35 degrees high kick limit
	heaterKickTimeSettings_m.low_temperature_kick_limit  = 30;		// 30 degrees low kick limit
	heaterKickTimeSettings_m.max_kick_time_after_mix     = 300;		// maximum time for kick after mix
		
	// set up used heater DAC plates settings & fan setting from factory.cfg

	topPlateDacNormal_m		= factoryData_g->heaterDacSettings[TOP_HEATER_DAC_DEFAULT];
	bottomPlateDacNormal_m	= factoryData_g->heaterDacSettings[BOTTOM_HEATER_DAC_DEFAULT];
	bottomPlateDacHigh_m	= bottomPlateDacNormal_m + 32;

	fanDacOn_m = factoryData_g->fanTemperatureSetting;
}


// ********************************************************************
// FUNCTION:
//
//		ProcessHeaterControlState() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Engine Heater Controller State Machine Processor 
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

HeaterState_t ProcessHeaterControlState(HeaterState_t heaterState, unsigned short *errorNumber)
{
	HeaterControlMsgId_t	msgId;
	float					ambientTemperature;
	float					rotorTemperature;
	static bool				overTempMsgSent = false;
	bool					overTempFlag;

	switch (heaterState)
	{
		case INIT_HEATER_STATE:
			// do plate current tests & init for normal DACs if pass
			if (HeaterPlatesCurrentTest(errorNumber))
			{
				return NO_HEATER_STATE;
			}

			// set up normal init plate & fan settings & thresholds for engine heater warming checks
			// turn fan off for warm-up
//			SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
			SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, FAN_OFF);

			topPlateTemperatureThreshold_m    = (((float) heaterControl_m.topPlateDac    * PLATE_TEMPERATURE_DAC_CONVERT_MULT) + PLATE_TEMPERATURE_DAC_CONVERT_OFFSET) * PLATE_TEMPERATURE_THRESHOLD_FACTOR; 
			bottomPlateTemperatureThreshold_m = (((float) heaterControl_m.bottomPlateDac * PLATE_TEMPERATURE_DAC_CONVERT_MULT) + PLATE_TEMPERATURE_DAC_CONVERT_OFFSET) * PLATE_TEMPERATURE_THRESHOLD_FACTOR;
			 
			// synch to 5 second timer & go to check for engine warming

			WaitFor5SecTimeout();
			heaterState = ENGINE_WARMING_INIT_STATE;
			LogDebug( "ENGINE_WARMING_INIT_STATE" );
			break;


		case ENGINE_WARMING_INIT_STATE:
			// set up to init or check for Engine rotor temperature within range to set for warming 

			WaitFor5SecTimeout();

			RequestEngineHeaterData();

			if (WaitForTemperatures(errorNumber))
			{
				return NO_HEATER_STATE;
			}

			// check if top & bottom plate temperatures are both enough to check for engine warming done or init

			if ((heaterData_m.topPlateTemperature > topPlateTemperatureThreshold_m) && (heaterData_m.bottomPlateTemperature > bottomPlateTemperatureThreshold_m))
			{
				rotorTemperature = heaterData_m.rotorTemperature;
			
				SendTemperatureMsg(ENGINE_WARMING_INIT_STATE, heaterControlTimer_m, ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR, rotorTemperature);
				if (rotorTemperature < ROTOR_AIR_SENSOR_THRESHOLD)
				{
					// got to warm up some more so check for dead rotor error sensor

					if (rotorTemperature < DEAD_AIR_SENSOR_THRESHOLD)
					{
						*errorNumber = SERR_DEAD_AIR_SENSOR;

						return NO_HEATER_STATE;
					}

					// turn up top & bottom plate DACs to accelerate warming
					// turn fan off for warm-up
//					SetEngineTemperatureControl(topPlateDacNormal_m + INIT_WARMING_KICK, bottomPlateDacNormal_m + INIT_WARMING_KICK, fanDacOn_m);
					SetEngineTemperatureControl(topPlateDacNormal_m + INIT_WARMING_KICK, bottomPlateDacNormal_m + INIT_WARMING_KICK, FAN_OFF);

					heaterState = ENGINE_WARMING_STATE;
					LogDebug( "ENGINE_WARMING_STATE" );
				}
				else
				{
					// rotor temperature > init threshold so idle the heater now without more warming

					SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

					heaterState = HEATER_IDLING_STATE;

					SendAnalysisMessage( MSG_ANALYSIS_WARMUP_DONE );
				
					LogDebug( "HEATER_IDLING_STATE" );
				} 
			}
			break;


		case ENGINE_WARMING_STATE:
			// engine warming so check for Engine rotor temperature within analysis range 

			WaitFor5SecTimeout();

			RequestEngineHeaterData();

			if (WaitForTemperatures(errorNumber))
			{
				return NO_HEATER_STATE;
			}

			rotorTemperature = heaterData_m.rotorTemperature;
			
			SendTemperatureMsg(ENGINE_WARMING_STATE, heaterControlTimer_m, ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR, rotorTemperature);

			if (rotorTemperature > ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR)
			{
				// rotor temperature > warming threshold so idle the heater	at normal DAC settings since in analysis range

				SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

				heaterState = HEATER_IDLING_STATE; 

				SendAnalysisMessage( MSG_ANALYSIS_WARMUP_DONE );

				LogDebug( "HEATER_IDLING_STATE" );
			}
			break;


		case HEATER_IDLING_STATE:
			// poll every 5 seconds to check if plates too hot & waits for start of rotor analysis  

			msgId = WaitForNextHeaterMsg();

			switch (msgId)
			{
				case MSG_HEATER_CONTROL_TIMER:
					RequestEngineHeaterData();
					break;

				case MSG_HEATER_CONTROL_TEMPERATURES:
					rotorTemperature = heaterData_m.rotorTemperature;

					if (rotorTemperature > TOP_PLATE_TOO_HOT)
					{
						// If we haven't already done so, let Analysis know we're over the temperature limit.
						if ( !overTempMsgSent )
						{
							overTempFlag = true;
							SendAnalysisOvertempMessage( overTempFlag );
							overTempMsgSent = true;
						}
					}
					else
					{
						// If we previously told Analysis we're over the temperature limit, let it know
						// that we're OK now.
						if ( overTempMsgSent )
						{
							overTempFlag = false;
							SendAnalysisOvertempMessage( overTempFlag );
							overTempMsgSent = false;
						}
					}
					break;

				case MSG_HEATER_CONTROL_START_ANALYSIS:
					// set delay time for first part of rotor analysis pre heat kick mix done
					 
					SetDelayTime();

					// turn fan off since going to mix (with normal plate DACS)

					SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
			
					heaterState = ANALYSIS_INIT_STATE;
					LogDebug( "ANALYSIS_INIT_STATE" );
					break;

				default:
					LogDebug( "HEATER_IDLING_STATE received %d message", msgId );
					break;
			}
			break;


		case ANALYSIS_INIT_STATE:

			// init for warming required if cold rotor inserted

			msgId = WaitForNextHeaterMsg();

			switch (msgId)
			{
				case MSG_HEATER_CONTROL_TIMER:
					RequestEngineHeaterData();
					break;

				case MSG_HEATER_CONTROL_TEMPERATURES:
					// use ambient temperature to calculate ambient DAC offset kick

					ambientTemperature = heaterData_m.ambientTemperature;

					SendTemperatureMsg(ANALYSIS_INIT_STATE, heaterControlTimer_m, LOW_AMBIENT_TEMPERATURE_LIMIT, ambientTemperature);

					if (ambientTemperature < LOW_AMBIENT_TEMPERATURE_LIMIT)
					{
						*errorNumber = SERR_AMBIENT_TEMPERATURE;

						return NO_HEATER_STATE;
					}
                    ambientplateDacOffset_m = GetNgaMatchToJ2DacOffset (ambientTemperature);
					heaterState = ANALYSIS_PRE_WARMING_MIX_STATE;
					LogDebug( "ANALYSIS_PRE_WARMING_MIX_STATE" );
					break;

				default:
					LogDebug( "ANALYSIS_INIT_STATE received %d message", msgId );
					break;
			}
			break;


		case ANALYSIS_PRE_WARMING_MIX_STATE:
			// check if rotor analysis motor mix pre rotor kick warming time up

			WaitFor5SecTimeout();

			if (DelayTimeUp(DELAY_30S))
			{
				// set up to read & compute rotor temperature kick time calc

				RequestEngineHeaterData();	
			
				heaterState = ANALYSIS_CALC_WARMING_TIME_STATE;
				LogDebug( "ANALYSIS_CALC_WARMING_TIME_STATE" );
			}
			break;


		case ANALYSIS_CALC_WARMING_TIME_STATE:
			// calc heater kick time 

			if (WaitForTemperatures(errorNumber))
			{
				return NO_HEATER_STATE;
			}

			// calculate heater kick time for rotor temperature & kick time settings
					
			rotorTemperature = heaterData_m.rotorTemperature;

			SendTemperatureMsg(ANALYSIS_CALC_WARMING_TIME_STATE, heaterControlTimer_m, ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR, rotorTemperature);

			analysisRotorHeatKickDelay_m = CalculateHeaterKickTimeRequired(rotorTemperature);
			LogDebug( "Calculated kick time = %u", analysisRotorHeatKickDelay_m );

			SetDelayTime();

			// add ambient offset calculated to plate DACs for top normal & bottom high	& set fan to on	if non-zero kick time

// GLD
// Originally was == 0, which was a bug.  However, that's how the VetScan was released at version 2.0.5.
			if (analysisRotorHeatKickDelay_m != 0)
			{
				SetEngineTemperatureControl(topPlateDacNormal_m + ambientplateDacOffset_m, bottomPlateDacHigh_m + ambientplateDacOffset_m, fanDacOn_m);
			}

			heaterState = ANALYSIS_WARMING_STATE;
			LogDebug( "ANALYSIS_WARMING_STATE" );
			break;


		case ANALYSIS_WARMING_STATE:
			// wait till heater analysis warming kick time is up

			if (DelayTimeUp(analysisRotorHeatKickDelay_m))
			{
				// keep ambient offset added to plate DACs for top normal & back to bottom normal	at end of mixing time
			
				SetEngineTemperatureControl(topPlateDacNormal_m + ambientplateDacOffset_m, bottomPlateDacNormal_m + ambientplateDacOffset_m, fanDacOn_m);

				heaterState = ANALYSIS_WAITING_MIX_DONE_STATE;
				LogDebug( "ANALYSIS_WAITING_MIX_DONE_STATE" );
			}

			// get next timeout for next rotor heat kick delay done check or engine A msg

			msgId = WaitForNextHeaterMsg();

			// make sure robust exit if engine messages received prematurely

			if ((msgId == MSG_HEATER_CONTROL_MIX_DONE) || (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE))
			{
				// set plate DACs for top normal & bottom normal & set fan off 

				SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

				heaterState = HEATER_IDLING_STATE;
			}
			break;


		case ANALYSIS_WAITING_MIX_DONE_STATE:
			// wait for Engine 'A' message with MIX_DONE parameter

			msgId = WaitForNextHeaterMsg();

			if (msgId == MSG_HEATER_CONTROL_MIX_DONE)
			{
				// keep ambient offset added to plate DACs for top normal & bottom normal & set fan off for cuvette sampling time

				SetEngineTemperatureControl(topPlateDacNormal_m + ambientplateDacOffset_m, bottomPlateDacNormal_m + ambientplateDacOffset_m, fanDacOn_m);

				heaterState = ANALYSIS_WAITING_DONE_STATE;
				LogDebug( "ANALYSIS_WAITING_DONE_STATE" );
			}

			// make sure robust exit if engine messages received prematurely

			else if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE)
			{
				// set plate DACs for top normal & bottom normal & set fan off 

				SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

				heaterState = HEATER_IDLING_STATE;
			}

			else if ( msgId != MSG_HEATER_CONTROL_TIMER )
			{
				LogDebug( "ANALYSIS_WAITING_MIX_DONE_STATE received %d message", msgId );
			}
			break;


		case ANALYSIS_WAITING_DONE_STATE:
			// wait for Engine 'A' message with ANALYSIS_DONE parameter to set DACS back to normal with fan on for heater idling to wait for next rotor run

			msgId = WaitForNextHeaterMsg();

			if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE)
			{

				// keep ambient offset added to plate DACs for top normal & bottom normal & set fan off for cuvette sampling time

				SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

				heaterState = HEATER_IDLING_STATE;
				LogDebug( "HEATER_IDLING_STATE" );
			}
			else if ( msgId != MSG_HEATER_CONTROL_TIMER )
			{
				LogDebug( "ANALYSIS_WAITING_DONE_STATE received %d message", msgId );
			}

			break;


		default:
			break;
	}

	return heaterState;
}


// ********************************************************************
// FUNCTION:
//
//		main() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Heater Control process top level main function 
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

int main( int argc, char **argv )
{
	unsigned short	errorNumber;
	HeaterState_t	heaterState;

	factoryData_g = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	if ( factoryData_g == 0 )
	{
		return -1;
	}

	// Start heater control. 
	InitializeHeaterProcess();

	heaterState = INIT_HEATER_STATE;
	LogDebug( "INIT_HEATER_STATE" );

	errorNumber = SERR_NONE;

	// Start the timer thread.
	ThreadCreate( 0, TimerThread, NULL, NULL );
	
	while ( errorNumber == SERR_NONE )
	{
		heaterState = ProcessHeaterControlState(heaterState, &errorNumber);
	}

	SendHeaterErrorMsg( errorNumber );

	LogError( "HeaterControl: exiting with error %X", errorNumber );

	return 0;
}

