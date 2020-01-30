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


#include <windows.h>
#include <stdio.h>	// for terminal window debugging - to be removed later
#include <stdlib.h>
#include <msgqueue.h>
#include <string.h>

#include "HeaterGlobals.h"
#include "Temperature.h"
#include "HeaterControlMessages.h"
#include "EngineCommMessages.h"
#include "AnalysisMessages.h"
#include "CommonDefines.h"
#include "Shmem.h"
#include "SystemError.h"
#include "StartupMessages.h"
#include "LogErr.h"
#include "MfMessages.h"

#undef   TURN_OFF_KICK_TIME                 /* no kick time and offset */
#undef   TURN_WARNING_OFF                   /* turn off too hot warning */

FactoryData_t   *factoryData_g = 0;
SettingsData_t  *settingsData  = 0;
SystemData_t    *systemData    = 0; 

// heater control defintions

#define HEATER_DAC_OFF						0
#define HEATER_DAC_MAX						0xff

#define FAN_OFF								0xff


#define	LOW_TOP_PLATE_CURRENT				0.0		// 0.68		// amps	
#define	HIGH_TOP_PLATE_CURRENT				10.0	// 2.351	// amps
#define	LOW_BOTTOM_PLATE_CURRENT			0.0		// 0.954	// amps	
#define	HIGH_BOTTOM_PLATE_CURRENT			10.0	// 3.319	// amps

#ifdef TURN_OFF_KICK_TIME

#define INIT_WARMING_KICK					0
#else
#define INIT_WARMING_KICK					40

#endif

#define LT_20_AMBIENT_KICK					8
#define LT_22_AMBIENT_KICK					6
#define LT_24_AMBIENT_KICK					4
#define LT_28_AMBIENT_KICK					2


#define	PLATE_TEMPERATURE_DAC_CONVERT_MULT		0.197952	// 197.952 millidegree C per plate DAC count
#define	PLATE_TEMPERATURE_DAC_CONVERT_OFFSET	7.758012	// 7.758012 degree C plate temperature DAC convert offset
#define PLATE_TEMPERATURE_THRESHOLD_FACTOR		0.90		// threshold at 90% of setpoint

#define PLATE_CURRENT_CONVERT_MULT				0.000625	// 625 microamps per plate current ADC count

#define	TOP_PLATE_TOO_HOT					38.5

#define	LOW_AMBIENT_TEMPERATURE_LIMIT		13.0	// operation temperature limits	
#define	ROTOR_AIR_SENSOR_THRESHOLD			33.5
#define	ROTOR_AIR_SENSOR_TEMPERATURE_FLOOR	35.0
#define	DEAD_AIR_SENSOR_THRESHOLD			5.0


//#define KICK_TIME_DAC_OFFSET               32
#define KICK_TIME_DAC_OFFSET                 6 

typedef struct
{
    int         ambTemp; 
    char        offset;
}   ambPlateDacOffset_t;

typedef struct
{
    float       ambTemp;
    float       tempDiff;
} lowToHighFanAmbTempTable_t;


#define MIN_AMB_TEMP            32.0
#define MAX_AMB_TEMP            45.0
#define MAX_OFFSET_STEP         16
#define MAX_P_OFFSET            7 
#define MAX_N_OFFSET            -9

#define LOW_TO_HIGH_FAN_START_MATCH_TEMP    38.8
#define LOW_TO_HIGH_FAN_END_MATCH_TEMP      50.0


// abminet offset table 
ambPlateDacOffset_t   ambPlateDacOffsetTable [] = 
                                        { 
                                            {30, 10},
                                            {31, 9},
                                            {32, 8},
                                            {33, 7},
                                            {34, 6},
                                            {35, 4},
                                            {36, 2},
                                            {37, 1},
                                            {38, 0},
                                            {39, -1},
                                            {40, -2},
                                            {41, -3},
                                            {42, -4},
                                            {43, -5},
                                            {44, -6},
                                            {45, -7},
    
                                        };


lowToHighFanAmbTempTable_t  lowToHighFanAmbTempTable [] =
                                        {
                                            {38.5,   1.5},
                                            {39.5,   2.0},
                                            {40.0,   2.4},
                                            {40.1,   2.5},
                                            {40.4,   2},
                                            {41.0,   1.0},
                                            {41.4,   0.8},
                                            {42.0,   0.3},
                                            {42.5,   0.2},
                                            {255.0,  0},   // impossible value

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
unsigned char	ambientplateDacOffset_m;


// Current image of heater control Engine output DAC values
  
typedef struct
{
	unsigned short	topPlateDac;
	unsigned short	bottomPlateDac;
	unsigned char	fanControlDac;
    float           airOffset;
} HeaterControl_t;

HeaterControl_t		heaterControl_m;		// current image of heater control setting DACs (Console	to Engine)


// Engine received raw temperature related readings

RawTemperatures_t	rawTemperatures_m;

// Engine received calculated heater data values

typedef struct
{
	float	rotorTemperature;
	float	ambientTemperature;
	float	topPlateTemperature;
	float	bottomPlateTemperature;
	float	topPlateVoltage;
	float	bottomPlateVoltage;
} HeaterData_t;

typedef struct
{
    unsigned char   pollingFlag;
    unsigned char   lockDacFlag;
    unsigned short  topDac;
    unsigned short  botDac;
} HeaterTestSet_t;

HeaterData_t		heaterData_m;			// calculated heater values from raw readings

// used top & bottom heater plate DAC settings from factory.cfg

unsigned short	topPlateDacNormal_m;
unsigned short	bottomPlateDacNormal_m;
unsigned short	bottomPlateDacHigh_m;

// fan DAC setting from factory.cfg

unsigned char	fanDacOn_m;


// heater plate temperature threshold calculated compare values

float	topPlateTemperatureThreshold_m;
float	bottomPlateTemperatureThreshold_m;


// heater control delay timer variables

unsigned	heaterControlTimer_m;
unsigned	heaterControlDelayTimeSet_m;
unsigned    char    WatchDogFlag = 0;
unsigned    char    StopWatchDog = 0;

// debug only
static int         startTime, endTime;

// delay timer values

unsigned	analysisRotorHeatKickDelay_m;
static      HeaterTestSet_t   heaterTestSet;

#define DELAY_5S                1
#define	DELAY_10S				2
#define DELAY_20S               4
#define	DELAY_30S				6

#define SECONDS_5_DIVISOR		5

// for analysis test only
//#define                         ANALYSIS_TEST

// get low vs high fan ambient temperature difference
float
FindFanLowToHighTempDiff (float ambTemp)
{
    float tempDiff = 0;
    int i = 0;

    if ((ambTemp >= LOW_TO_HIGH_FAN_START_MATCH_TEMP) &&  (ambTemp <= LOW_TO_HIGH_FAN_END_MATCH_TEMP)) {
        while  (lowToHighFanAmbTempTable [i].ambTemp != 255.0) {
            if (ambTemp <= lowToHighFanAmbTempTable [i].ambTemp) {
                tempDiff = lowToHighFanAmbTempTable [i].tempDiff;
                break;
            }
            i++;
        }
    } 
    LogDebug ("ambTemp =%.3f  diff = %.3f", ambTemp, tempDiff);
    return tempDiff;
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

DWORD WINAPI TimerThread( void * arg )
{
	MSGQUEUEOPTIONS		options;
	const int msg = MSG_HEATER_CONTROL_TIMER;

	options.dwSize = sizeof(MSGQUEUEOPTIONS);
	options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
	options.bReadAccess = FALSE;
	options.dwMaxMessages = 0;
	options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(HeaterControlMsg_t)*/;

	static HANDLE queue = CreateMsgQueue(HEATER_CONTROL_QUEUE_NAME, &options);
	
	if ( queue == NULL )
	{
		LogError( "failure to open HeaterControl queue in TimerThread" );
		exit( -1 );
	}

	while ( 1 )
	{
		Sleep( 5000 );

		WriteMsgQueue(queue, (void *)&msg, sizeof( int ), INFINITE, NULL);
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

void SendEngineMessage (char command)
{
	EngineCommReceiveMsg_t	msg;
	static HANDLE			queue = NULL;
	MSGQUEUEOPTIONS			options;
	
	if ( queue == NULL )
	{
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(EngineCommReceiveMsg_t)*/;
		queue = CreateMsgQueue(ENGINE_COMM_QUEUE_NAME, &options);
	
		if ( queue == NULL )
		{
			LogError( " failure to open EngineComm queue" );
			exit( -1 );
		}
	}

    memset (&msg.data.engineMsg.message[0], 0, sizeof (msg.data.engineMsg.message));
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'H';
	msg.data.engineMsg.message[1] = command;
	msg.data.engineMsg.message[2] = ESC;
	msg.data.engineMsg.message[3] = 10;	// LSB of binary count.
	msg.data.engineMsg.message[4] = 0;	// MSB of binary count.
    if (heaterTestSet.lockDacFlag == 1) {
        //heater test on, DAC value settings are locked by 
        //heater test process 
        memcpy (&msg.data.engineMsg.message[5], &heaterTestSet.topDac, 2);
        memcpy (&msg.data.engineMsg.message[7], &heaterTestSet.botDac, 2);
        heaterControl_m.topPlateDac    = heaterTestSet.topDac;
        heaterControl_m.bottomPlateDac = heaterTestSet.botDac;
    } else {
        memcpy (&msg.data.engineMsg.message[5], &heaterControl_m.topPlateDac, 2);
        memcpy (&msg.data.engineMsg.message[7], &heaterControl_m.bottomPlateDac, 2);
    }
	msg.data.engineMsg.message[9] = heaterControl_m.fanControlDac;       // idle fan speed
	msg.data.engineMsg.message[10] = heaterControl_m.fanControlDac;      // fan speed set 
    memcpy (&msg.data.engineMsg.message[11],  &heaterControl_m.airOffset, 4); // air offset
	msg.data.engineMsg.length = 15;
	WriteMsgQueue(queue, &msg, sizeof( EngineCommReceiveMsg_t ), INFINITE, NULL);
}

// send timeout message to watchdog thread
void
SendWatchDogTimeoutMessage (void)
{
	StartupMsg_t	        msg;
	static HANDLE			queue = NULL;
	MSGQUEUEOPTIONS			options;
    static  char            sentFlag = 0;
	
	if (queue == NULL) {
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH ;
		queue = CreateMsgQueue (STARTUP_QUEUE_NAME, &options);
	
		if (queue == NULL) {
			LogError (" failure to open WatchDog queue");
			exit(-1);
		}
	}
    // only allow to send timeout message once
    if (sentFlag == 0) {
	    msg.msgId = MSG_STARTUP_WATCH_DOG;
	    WriteMsgQueue(queue, &msg,  sizeof(StartupMsg_t) + sizeof (int), INFINITE, NULL);
        LogError ("Heater Timeout, send watchdog timeout message");
        sentFlag = 1;
    }
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
	static HANDLE	queue = NULL;
	MSGQUEUEOPTIONS		options;
	
	if ( queue == NULL )
	{
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(AnalysisMsg_t)*/;
		queue = CreateMsgQueue(ANALYSIS_QUEUE_NAME, &options);
	
		if ( queue == NULL )
		{
			LogError( "failure to open Analysis queue" );
			exit( -1 );
		}
	}
	
	WriteMsgQueue(queue, msg, sizeof( AnalysisMsg_t ), INFINITE, NULL);
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

// send message to manufacturing
void
SendManufacturingTestMsg (HeaterData_t *heaterData)
{
    HeaterControlMsg_t          msg;
    static HANDLE               queue = NULL;
    MSGQUEUEOPTIONS             options;
    HeaterValues_t              *heaterValues;

    if (queue == NULL) {
        options.dwSize = sizeof (MSGQUEUEOPTIONS);
        options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess = FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage = MAX_QUEUE_LENGTH;
        queue = CreateMsgQueue (MANUFACTURE_QUEUE_NAME, &options);
        if (queue == NULL) {
            LogError( "failure to open manufacturing queue, error: %d",  GetLastError());
            return;
        }
    }

    heaterValues = (HeaterValues_t *)msg.data;

    heaterValues->rotorTemperature        = heaterData->rotorTemperature;
    heaterValues->ambientTemperature      = heaterData->ambientTemperature;
    heaterValues->topPlateTemperature     = heaterData->topPlateTemperature;
    heaterValues->bottomPlateTemperature  = heaterData->bottomPlateTemperature;
    heaterValues->topPlateVoltage         = heaterData->topPlateVoltage;
    heaterValues->bottomPlateVoltage      = heaterData->bottomPlateVoltage;
    heaterValues->ambientDacOffset        = ambientplateDacOffset_m;
    heaterValues->rotorTemperatureOffset  = factoryData_g->airTemperatureOffset;
    heaterValues->topHeaterDacValue       = heaterControl_m.topPlateDac;
    heaterValues->bottomHeaterDacValue    = heaterControl_m.bottomPlateDac;
    msg.msgId                             = MSG_HEATER_SEND_TEMPERATURE_VALUES;

    WriteMsgQueue(queue, &msg, sizeof(HeaterControlMsg_t), INFINITE, NULL);
}

void 
SendManufactureMessage  (ManufactureId_t msgId)
{
	ManufactureMsg_t	        msg;

    static HANDLE               queue = NULL;
    MSGQUEUEOPTIONS             options;

    if (queue == NULL) {
        options.dwSize = sizeof (MSGQUEUEOPTIONS);
        options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess = FALSE;
        options.dwMaxMessages = 0;
        options.cbMaxMessage = MAX_QUEUE_LENGTH;
        queue = CreateMsgQueue (MANUFACTURE_QUEUE_NAME, &options);
        if (queue == NULL) {
            LogError( "failure to open manufacturing queue, error: %d",  GetLastError());
            return;
        }
    }
	msg.msgId = msgId;
    WriteMsgQueue (queue, &msg, sizeof (ManufactureMsg_t), INFINITE, NULL);
	
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

void SetEngineTemperatureControl(unsigned short heaterTopPlateDac, unsigned short heaterBottomPlateDac, unsigned char fanControlDac)
{
	// update current image in heaterControl_m

	heaterControl_m.topPlateDac     = heaterTopPlateDac;
	heaterControl_m.bottomPlateDac  = heaterBottomPlateDac;
	heaterControl_m.fanControlDac   = fanControlDac;
    heaterControl_m.airOffset       = factoryData_g->airTemperatureOffset;
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

HeaterControlMsgId_t WaitForMsg (RawTemperatures_t *rawTemperatures)
{
	HeaterControlMsg_t		msg;
	static HANDLE			queue = NULL;
	MSGQUEUEOPTIONS			options;
	DWORD					receivedSize = 0;
	DWORD					flags;
	BOOL					result;

	if ( queue == NULL )
	{
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = TRUE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(HeaterControlMsg_t)*/;
		queue = CreateMsgQueue (HEATER_CONTROL_QUEUE_NAME, &options);
	
		if ( queue == NULL )
		{
			LogError( "failure to open HeaterControl queue" );
			exit( -1 );
		}
	}
		
	result = ReadMsgQueue(queue, &msg, sizeof(HeaterControlMsg_t), &receivedSize, INFINITE, &flags);

	if ( result )
	{
		if ( receivedSize > 0 )
		{
			switch ( msg.msgId )
			{
				case MSG_HEATER_CONTROL_TEMPERATURES:
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
                case MSG_HEATER_CONTROL_WATCH_DOG_DISABLE:
                case MSG_HEATER_CONTROL_WATCH_DOG_ENABLE:
                break;

                case MSG_HEATER_GET_TEMPERATURE_VALUES:
                     SendManufacturingTestMsg (&heaterData_m);
                break;

                case MSG_HEATER_SET_TEMPERATURE_VALUES:
                    // note: after HeaterTestSet.lockDacFlag = 1 
                    // HeaterControl DAC shall be locked 
                    // until exit temperature test 
                    heaterTestSet.lockDacFlag = 1;
                    // get heater test dac value settings 
                    heaterTestSet.topDac = *(unsigned short *)&msg.data [0];
                    heaterTestSet.botDac = *(unsigned short *)&msg.data [2];
                    // change the default DAC values
                    topPlateDacNormal_m     = heaterTestSet.topDac;
                    bottomPlateDacNormal_m  = heaterTestSet.botDac;
                    printf ("heater set topDac =%d\n\r", topPlateDacNormal_m);
                    printf ("heater set botDac =%d\n\r", bottomPlateDacNormal_m);
                    // send new DAC setting to Engine 
                    SendEngineMessage ('S');
                break;

                case MSG_HEATER_TEST_ENABLE:
                    heaterTestSet.pollingFlag = 1;
                break;

                case MSG_HEATER_TEST_DISABLE:
                    //exit test, dislable polling 
                    //recover heaterControl DAC settings 
                    //Note: the default DAC values
                    //topPlateDacNormal_m  and bottomPlateDacNormal_m 
                    //shall keep the temperature test setting values
                    heaterTestSet.lockDacFlag =  0;
                    heaterTestSet.pollingFlag = 0;
                break;

                case MSG_HEATER_TURN_OFF_HEATER:
                    //heater is turn off, keep seting top and botDac to be 0
                    heaterTestSet.topDac = 0;
                    heaterTestSet.botDac = 0;
                    heaterTestSet.lockDacFlag = 1;
                    //turn off Heater to prevent overheater during software updating
                    SetEngineTemperatureControl (HEATER_DAC_OFF, HEATER_DAC_OFF, fanDacOn_m);
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
    

	msgId = WaitForMsg (&rawTemperatures_m);	

	switch (msgId)
	{
		case MSG_HEATER_CONTROL_TIMER:
			heaterControlTimer_m++;		// increment running delay timer
			break;

		case MSG_HEATER_CONTROL_TEMPERATURES:
			CalcHeaterData (&rawTemperatures_m, &heaterData_m);
            if (heaterTestSet.pollingFlag == 1) {
                // send temperature data to manufacturing process
                SendManufacturingTestMsg (&heaterData_m);
            }
			break;

        case MSG_HEATER_CONTROL_WATCH_DOG_DISABLE:
             // turn off WatchDog
              LogDebug ("Disable WatchDog");
              StopWatchDog = 1;
              WatchDogFlag = 0;
            break;

        case MSG_HEATER_CONTROL_WATCH_DOG_ENABLE:
            StopWatchDog = 0;
            LogDebug ("Enable WatchDog");
            break;

        case MSG_HEATER_CONTROL_ANALYSIS_DONE:
            StopWatchDog = 0;
            WatchDogFlag = 0;
            LogDebug ("Enable Watchdog");
        break;
        

		default:
			break;
	}

	return msgId;
}

// wait for new message and check timeout
// in the case of timeout, send error message to Statup process
HeaterControlMsgId_t WaitForNewHeaterMsg (unsigned int delayTime)
{
    HeaterControlMsgId_t    msgId;
    unsigned int            duration;

    msgId = WaitForNextHeaterMsg();

    if (msgId == MSG_HEATER_CONTROL_TEMPERATURES) {
        // stop watchdog timer until next contorol timer received
        if ((WatchDogFlag == 1) && (StopWatchDog == 0))  {
             endTime = GetTickCount ();
             duration = endTime - startTime;
             if (duration > 2000) {
                LogDebug ("2 second timeout =%d", duration);
              }
        }
        WatchDogFlag = 0;
    } else if (msgId == MSG_HEATER_CONTROL_TIMER) {
            // for Engine communications timeout
            if ((WatchDogFlag == 1) && (StopWatchDog == 0)) {
                endTime = GetTickCount ();
                LogDebug ("5 seconds timeout=%d", (endTime - startTime));
                if (DelayTimeUp (delayTime)) {
                    SendWatchDogTimeoutMessage ();
                   // WatchDogFlag = 0;
                }
            }
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

HeaterControlMsgId_t
WaitForTemperatures (unsigned short *errorNumber, unsigned int delayTime)
{
	HeaterControlMsgId_t	msgId;

	SetDelayTime();

	while (POPE_IS_CATHOLIC) {
		msgId = WaitForNextHeaterMsg();
		if (msgId == MSG_HEATER_CONTROL_TEMPERATURES) {
			return msgId;
		} else if (msgId == MSG_HEATER_CONTROL_TIMER) {
			if (DelayTimeUp (delayTime)) {
				// got timeout with no heater temperature data so attempt to shut off heater DACs for safety and exit
				SetEngineTemperatureControl(HEATER_DAC_OFF, HEATER_DAC_OFF, fanDacOn_m);
				// set for Engine communications timeout
				*errorNumber = SERR_MSG_TIMEOUT;
				return msgId;
			} 
		} else if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE) {
            return msgId;
        } else {
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

int WaitFor5SecTimeout(void)
{
	int msgId;
	
	while (( msgId = WaitForNextHeaterMsg() ) != MSG_HEATER_CONTROL_TIMER)
	{
        if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE) {
            break;
        }
        if ((msgId < MSG_HEATER_GET_TEMPERATURE_VALUES) || 
            (msgId > MSG_HEATER_LAST_COMMAND)) {
		    LogDebug( "WaitFor5SecTimeout received %d message", msgId );
        }
	}
    return msgId;
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

	if (WaitForTemperatures(errorNumber, DELAY_30S) == MSG_HEATER_CONTROL_TIMER)
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

	if (WaitForTemperatures(errorNumber, DELAY_30S) == MSG_HEATER_CONTROL_TIMER)
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
    //ZZZ
    //LogDebug ("topPlateDacNormal_m =%d bottomPlateDacNormal_m =%d\n\r", topPlateDacNormal_m, bottomPlateDacNormal_m);

#ifdef TURN_OFF_KICK_TIME
	bottomPlateDacHigh_m	= bottomPlateDacNormal_m;
#else
	bottomPlateDacHigh_m	= bottomPlateDacNormal_m + KICK_TIME_DAC_OFFSET;
#endif

	fanDacOn_m = factoryData_g->fanTemperatureSetting;
}

// get top/bottom dac offset based on ambient temperature
char
GetAmbPlateOffsetDac (float ambTemp)
{
    char offset = 0;
    int temp, i;

    // Get ambient extra offset, two conditions:
    // 1. Fan speed set to be low speed
    // 2. In customer mode
    if ((settingsData->fanIdleSpeed == IDLE_FAN_LOW_SPEED) &&(IN_CUSTOMER_MODE)) {
        ambTemp -=  FindFanLowToHighTempDiff (ambTemp);
    }

    if (ambTemp < MIN_AMB_TEMP) {
        offset = MAX_P_OFFSET;
    } else if (ambTemp > MAX_AMB_TEMP) {
        offset = MAX_N_OFFSET;
    } else {
        temp = (int)(ambTemp + 0.5);
        for (i =0; i < MAX_OFFSET_STEP; i++) {
            if (ambPlateDacOffsetTable[i].ambTemp == temp) { 
                offset = ambPlateDacOffsetTable [i].offset; 
                break;
            }
        }
    }
    LogDebug ("ambTemp =%.3f offset=%d", ambTemp, offset);

    return offset;
    
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

HeaterState_t ProcessHeaterControlState (HeaterState_t heaterState, unsigned short *errorNumber)
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

			if (WaitForTemperatures(errorNumber, DELAY_30S) == MSG_HEATER_CONTROL_TIMER)
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
					SetEngineTemperatureControl(topPlateDacNormal_m + INIT_WARMING_KICK, bottomPlateDacNormal_m + INIT_WARMING_KICK, FAN_OFF);

					heaterState = ENGINE_WARMING_STATE;
					LogDebug( "ENGINE_WARMING_STATE" );
				}
				else
				{
					// rotor temperature > init threshold so idle the heater now without more warming

					SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);

					heaterState = HEATER_IDLING_STATE;


                    // tell analysis process warmup is done
                    SendAnalysisMessage( MSG_ANALYSIS_WARMUP_DONE );
                    // tell RandD process warmup is done
                    SendManufactureMessage (MSG_MANUFACTURE_WARMUP_DONE);

					LogDebug( "HEATER_IDLING_STATE" );
				} 
			}
			break;


		case ENGINE_WARMING_STATE:
			// engine warming so check for Engine rotor temperature within analysis range 

			WaitFor5SecTimeout();

			RequestEngineHeaterData();

			if (WaitForTemperatures(errorNumber, DELAY_30S) == MSG_HEATER_CONTROL_TIMER)
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

                // tell analysis process warmup is done
				SendAnalysisMessage( MSG_ANALYSIS_WARMUP_DONE );
                // tell RandD process warmup is done
                SendManufactureMessage (MSG_MANUFACTURE_WARMUP_DONE);

				LogDebug( "HEATER_IDLING_STATE" );
			}
			break;

		case HEATER_IDLING_STATE:
			// poll every 10 seconds to check if plates too hot & waits for start of rotor analysis  
            msgId = WaitForNewHeaterMsg (DELAY_20S);
			switch (msgId)
			{
				case MSG_HEATER_CONTROL_TIMER:
                    // request engine board timing info 
                    if (StopWatchDog == 0) {
					    RequestEngineHeaterData();
                        // Start Watchdog timing  
                        if (WatchDogFlag == 0) {
                            WatchDogFlag = 1;
                            startTime = GetTickCount ();
                            // reset time
                            SetDelayTime ();
                        }
                    }
					break;

				case MSG_HEATER_CONTROL_TEMPERATURES:
                    // in the case of watchdog stopped, skip the temperature reading value
                    // to work around hardware ADC cross over barcode led on problem
#ifndef TURN_WARNING_OFF  
                    if (StopWatchDog == 0) {
					    rotorTemperature = heaterData_m.rotorTemperature;
					    if (rotorTemperature > TOP_PLATE_TOO_HOT) {
						    // If we haven't already done so, let Analysis know we're over the temperature limit.
						    if (!overTempMsgSent) {
							    overTempFlag = true;
							    SendAnalysisOvertempMessage( overTempFlag );
							    overTempMsgSent = true;
						    }
					    } else {
						    // If we previously told Analysis we're over the temperature limit, let it know
						    // that we're OK now.
						    if (overTempMsgSent) {
							    overTempFlag = false;
							    SendAnalysisOvertempMessage( overTempFlag );
							    overTempMsgSent = false;
						    }
					    }
                    }
#endif
					break;

				case MSG_HEATER_CONTROL_START_ANALYSIS:

                    // turn off WatchDogFlag
                    LogError ("Disable WatchDog");
                    WatchDogFlag = 0;
                    StopWatchDog = 1;

					// set delay time for first part of rotor analysis pre heat kick mix done
					 
					SetDelayTime();

					// turn fan off since going to mix (with normal plate DACS)

                    // ZZZ take out for conflict console - engine traffic
#if 0
					SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
#endif
			
					heaterState = ANALYSIS_INIT_STATE;
					LogDebug( "ANALYSIS_INIT_STATE" );
					break;

                case MSG_HEATER_CONTROL_WATCH_DOG_DISABLE:
                case MSG_HEATER_CONTROL_WATCH_DOG_ENABLE:
                    break;

				default:
                    if ((msgId < MSG_HEATER_GET_TEMPERATURE_VALUES) ||
                        (msgId > MSG_HEATER_LAST_COMMAND)) {
					    LogDebug( "HEATER_IDLING_STATE received %d message", msgId );
                    }
					break;
			}
			break;


		case ANALYSIS_INIT_STATE:

			// init for warming required if cold rotor inserted

			msgId = WaitForNextHeaterMsg();

			switch (msgId)
			{
				case MSG_HEATER_CONTROL_TIMER:
					RequestEngineHeaterData ();
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
#ifdef TURN_OFF_KICK_TIME
					ambientplateDacOffset_m = 0;	// no offset for ambient > 28.0 degrees
#else
                    ambientplateDacOffset_m = GetAmbPlateOffsetDac (ambientTemperature);

#endif

					heaterState = ANALYSIS_PRE_WARMING_MIX_STATE;
					LogDebug( "ANALYSIS_PRE_WARMING_MIX_STATE" );
					break;

                case MSG_HEATER_CONTROL_ANALYSIS_DONE:
                    heaterState =  HEATER_IDLING_STATE;
                    SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
                    LogDebug ("HEATER_IDLING_STATE");
                    break;

				default:
					LogDebug( "ANALYSIS_INIT_STATE received %d message", msgId );
					break;
			}
			break;


		case ANALYSIS_PRE_WARMING_MIX_STATE:
			// check if rotor analysis motor mix pre rotor kick warming time up
			if (WaitFor5SecTimeout() == MSG_HEATER_CONTROL_ANALYSIS_DONE) {
                SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
                heaterState =  HEATER_IDLING_STATE;
                LogDebug ("HEATER_IDLING_STATE");
            } else {
			    if (DelayTimeUp(DELAY_30S)) {
				    // set up to read & compute rotor temperature kick time calc
				    RequestEngineHeaterData();	
				    heaterState = ANALYSIS_CALC_WARMING_TIME_STATE;
				    LogDebug( "ANALYSIS_CALC_WARMING_TIME_STATE" );
                }
			}
			break;


		case ANALYSIS_CALC_WARMING_TIME_STATE:
			// calc heater kick time 

			msgId = WaitForTemperatures(errorNumber, DELAY_30S);
            if (msgId == MSG_HEATER_CONTROL_TIMER) {
                LogDebug ("NO_HEATER_STATE");
                return NO_HEATER_STATE;
            } else if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE) {
                SetEngineTemperatureControl(topPlateDacNormal_m, bottomPlateDacNormal_m, fanDacOn_m);
                LogDebug ("HEATER_IDLING_STATE");
				return HEATER_IDLING_STATE;
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
				SetEngineTemperatureControl (topPlateDacNormal_m + ambientplateDacOffset_m, bottomPlateDacHigh_m + ambientplateDacOffset_m, fanDacOn_m);
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
                LogDebug ("HEATER_IDLING_STATE");
				heaterState = HEATER_IDLING_STATE;
			} else if (msgId == MSG_HEATER_CONTROL_ANALYSIS_DONE) {
                heaterState = HEATER_IDLING_STATE;
                LogDebug ("HEATER_IDLING_STATE");
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
                LogDebug ("HEATER_IDLING_STATE");
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

    memset (&heaterTestSet, 0, sizeof (HeaterTestSet_t));

	factoryData_g = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );

	if ( factoryData_g == 0 )
	{
        LogError ("Fail to mmemory share  %s", FACTORY_SHMEM_NAME);
		return -1;
	}

    settingsData  = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
    if (settingsData == 0) {
       LogError ("Fail to memory share  %s", SETTINGS_SHMEM_NAME);
        return -1;
    }

    // create share memory pointer for 10bits DAC flag detection
    // systemData->dac10BitsFlag
    systemData = (SystemData_t *)MapSharedMem (SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
    if (systemData == 0) {
        LogError ("Fail to memory share  %s", SYSTEM_SHMEM_NAME);
        return -1;

    }

	// Start heater control. 
	InitializeHeaterProcess();

#ifdef  ANALYSIS_TEST
	heaterState = HEATER_IDLING_STATE;
    LogDebug ("Set Heater at Idle state");
#else

	heaterState = INIT_HEATER_STATE;
	LogDebug( "INIT_HEATER_STATE" );
#endif

	errorNumber = SERR_NONE;

	// Start the timer thread.
	CreateThread(NULL, 0, TimerThread, NULL, 0, NULL);
	
	while ( errorNumber == SERR_NONE )
	{
		heaterState = ProcessHeaterControlState (heaterState, &errorNumber);
	}

	SendHeaterErrorMsg( errorNumber );

	LogError( "HeaterControl: exiting with error %X", errorNumber );

	return 0;
}

