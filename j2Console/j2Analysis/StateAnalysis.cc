#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>

#include "StateAnalysis.h"
#include "WriteRad.h"
#include "AnalysisMessages.h"
#include "HeaterControlMessages.h"
#include "CommonDefines.h"
#include "CreateDisplay.h"
#include "ParseRoc.h"
#include "ParseCounters.h"
#include "Shmem.h"
#include "SystemError.h"
#include "LogErr.h"
#include "FormatResults.h"
#include "ParseTimeouts.h"
#include "AlgGlobals.h"

AStrings    strings;

BarcodeData_t *			StateAnalysis::barcodeData		= (BarcodeData_t *)		MapSharedMem( BARCODE_SHMEM_NAME,		sizeof( BarcodeData_t ) );
CalibrationData_t *		StateAnalysis::calibrationData	= (CalibrationData_t *)	MapSharedMem( CAL_DATA_SHMEM_NAME,		sizeof( CalibrationData_t ) );
CounterData_t *			StateAnalysis::counterData		= (CounterData_t *)		MapSharedMem( COUNTERS_SHMEM_NAME,		sizeof( CounterData_t ) );
ResultsDbChanged_t *	StateAnalysis::dbChanged		= (ResultsDbChanged_t *)MapSharedMem( DATABASE_SHMEM_NAME,		sizeof( ResultsDbChanged_t ) );
FactoryData_t *			StateAnalysis::factoryData 		= (FactoryData_t *)		MapSharedMem( FACTORY_SHMEM_NAME,		sizeof( FactoryData_t ) );
RefRangeData_t * 		StateAnalysis::refRanges		= (RefRangeData_t *)	MapSharedMem( REF_RANGES_SHMEM_NAME,	sizeof( RefRangeData_t ) );
RawData_t *				StateAnalysis::rawData			= (RawData_t *)			MapSharedMem( RAW_SHMEM_NAME,			sizeof( RawData_t ) );
RotorConfig_t *			StateAnalysis::rotorConfig		= (RotorConfig_t *)		MapSharedMem( ROC_SHMEM_NAME,			sizeof( RotorConfig_t ) );
RotorResults_t *		StateAnalysis::rotorResults		= (RotorResults_t *)	MapSharedMem( RESULTS_SHMEM_NAME,		sizeof( RotorResults_t ) );
SettingsData_t *		StateAnalysis::settingsData		= (SettingsData_t *)	MapSharedMem( SETTINGS_SHMEM_NAME,		sizeof( SettingsData_t ) );
SystemData_t *			StateAnalysis::systemData		= (SystemData_t *)		MapSharedMem( SYSTEM_SHMEM_NAME,		sizeof( SystemData_t ) );
VersionData_t *			StateAnalysis::versionData		= (VersionData_t *)		MapSharedMem( VERSION_SHMEM_NAME,		sizeof( VersionData_t ) );
OrdersList_t *          OrderQueue::ordersList          = (OrdersList_t *)      MapSharedMem( ORDERS_SHMEM_NAME,        sizeof( OrdersList_t ) );
bool				StateAnalysis::analysisStarted = false;
bool				StateAnalysis::analysisDone = false;
bool				StateAnalysis::analysisInProcess = false;
SystemErrorNumber_t	StateAnalysis::analyzeError = SERR_NONE;
int					StateAnalysis::barcodeReadCount = 0;
bool				StateAnalysis::receivedRotorName = false;
bool				StateAnalysis::rotorNameValid = false;
bool				StateAnalysis::isOvertemp = false;
int					StateAnalysis::progBarValue = 0;
bool				StateAnalysis::resultsComplete = false;
bool				StateAnalysis::runCanceled = false;
RunInfo_t			StateAnalysis::runInfo = {};
RunInfo_t			StateAnalysis::runInfoRotor1 = {};
RunInfo_t			StateAnalysis::runInfoRotor100 = {};
DemoResult_t *		StateAnalysis::demoResult = GetRandomDemoResult();
const wchar_t *		StateAnalysis::infoText = L"";
HANDLE				StateAnalysis::timerThread = 0;
bool				StateAnalysis::warmupComplete = false;
WarningQueue		StateAnalysis::analysisWarningQueue;
Expirys				StateAnalysis::expirys;
TimeoutData_t		StateAnalysis::timeoutData;
bool				StateAnalysis::isRotor1 = false;
bool				StateAnalysis::isRotor100 = false;
bool                StateAnalysis::isVsRotor = false;
bool				StateAnalysis::allInputReceived = false;
bool				StateAnalysis::sampleTypeSelected = false;
bool                StateAnalysis::rotor1RunsComplete = false;
long long			StateAnalysis::rotor1Counter = -1;
long long			StateAnalysis::rotor100Counter = 1;
char				StateAnalysis::lockoutOperatorId[];
char                StateAnalysis::uploadedRotorName[];
bool                StateAnalysis::uploadedRotorReceived = false;
unsigned long       StateAnalysis::num4214Errors = 0;
bool                StateAnalysis::watchDogDisable = false;
OrderQueue          StateAnalysis::orderQueue;
int                 StateAnalysis::continueState = 0;
bool                StateAnalysis::drawerClosedAutomatically = false;

OrderId_t           uploadedOrderSelected = 0;
OrderId_t           uploadedOrderInProcess = 0;
OrderId_t           uploadedOrderIdBak = 0;
OrderDisplayList_t  orderListBak [5]; 


// for analysis test only
//#define             SKIP_OVER_TEMP


/*
 * FUNCTION
 * 	WarningQueue::Enqueue
 * 
 * DESCRIPTION
 * 	Queues up a warning so we can process more than one in succession.
 * 
 * PARAMETERS
 *	errNum - warning ID.
 */

void
WarningQueue::
Enqueue( SystemErrorNumber_t errNum )
{
	if ( last < 19 )
	{
		list[ last++ ] = errNum;
	}
}

/*
 * FUNCTION
 * 	WarningQueue::Dequeue
 * 
 * DESCRIPTION
 * 	Retrieves the next warning from the warnings queue, removing it as it does so.
 * 
 * RETURNS
 *	Warning ID.
 */

SystemErrorNumber_t
WarningQueue::
Dequeue()
{
	if ( index < last )
	{
	 	return list[ index++ ];
	}
	else
	{
		return SERR_NONE;
	}
}

/*
 * FUNCTION
 * 	StateAnalysis::CancelAnalysis
 * 
 * DESCRIPTION
 * 	Cancels the analysis in process.
 */
 
 void
 StateAnalysis::
 CancelAnalysis()
 {
 	// Cancel the timeout timer.
	SetTimer( -1 );

	// Put heater control back in idling.
	SendHeaterControlMessage( MSG_HEATER_CONTROL_ANALYSIS_DONE );
    
				
	// Tell the Engine to cancel.
	SendAMessage( 'C' );
				
	// Set the flag so nothing more done with the run.
	runCanceled = true;

	// Put an entry in the log.
	LogInfo( "Analysis canceled by user" );

	// Update the counter
	counterData->rotorAbortCount++;
	WriteCounters( counterData );
				
    // Make sure that, if this a Rotor 100, we start the sequence over again if the next rotor is also a Rotor 100.
	memset( &runInfoRotor100, 0, sizeof( RunInfo_t ) );

	runInfo.errNum = SERR_ROTOR_CANCELED;
 }
 
/*
 * FUNCTION
 * 	StateAnalysis::SetTimer
 * 
 * DESCRIPTION
 * 	Sets up the timer. Deletes an existing timer thread if there is one, then
 *  creates a new thread if the time given is greater than zero.
 * 
 * PARAMETERS
 *	time - Delay time in seconds.
 */

void
StateAnalysis::
SetTimer( int time )
{
	// We need a static because pthread_create is passed a pointer to the time
	// value, and this thread will continue and a local variable would go away
	// as soon as this function returned.
	static int	_time;
	
	_time = time;
	
	// Cancel the current timer, if there is one.
	if ( timerThread != NULL )
	{
		TerminateThread(timerThread, 0);
		CloseHandle(timerThread);
	}
	timerThread = NULL;

	// Create a new timer.
	if ( _time >= 0 )
	{
		timerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TimerThread, &_time, 0, NULL);
	}
}

/*
 * FUNCTION
 * 	StateAnalysis::TimerThread
 * 
 * DESCRIPTION
 * 	Sleeps the given length of time, then sends a message to signal the main thread.
 * 
 * PARAMETERS
 *	arg - Pointer to the delay time, in seconds.
 */
void *
StateAnalysis::
TimerThread( void * arg )
{
	const int		msg = MSG_ANALYSIS_TIMER;
	
	static HANDLE	queue = NULL;

	unsigned		time = *(unsigned *)arg;

	if (queue == NULL) {
		MSGQUEUEOPTIONS			options;
		options.dwSize = sizeof(MSGQUEUEOPTIONS);
		options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
		options.bReadAccess = FALSE;
		options.dwMaxMessages = 0;
		options.cbMaxMessage = MAX_QUEUE_LENGTH;

		queue = CreateMsgQueue(ANALYSIS_QUEUE_NAME, &options);
		
		if ( queue == NULL )
		{
			LogError( "failed to open queue %s", ANALYSIS_QUEUE_NAME );
			return 0;
		}
	}

	Sleep( time * 1000 );

	WriteMsgQueue(queue, (void *)&msg, sizeof( int ), INFINITE, NULL);

	return 0;
}

/*
 * FUNCTION
 * 	StateAnalysis::ReadCalibrationFile
 * 
 * DESCRIPTION
 * 	Reads and parses the calibration data file.
 */

void
StateAnalysis::
ReadCalibrationFile()
{
	CfgError_t cfgErr;
	
	cfgErr = ParseCalibration( calibrationData );
	if ( cfgErr != CFG_ERR_NONE && cfgErr != CFG_ERR_CRC )
	{
		LogError( "error parsing calibration file: %d", cfgErr );
	}
}

/*
 * FUNCTION
 * 	StateAnalysis::ReadTimeoutFile
 * 
 * DESCRIPTION
 * 	Reads and parses the analysis segments timeout data file.
 */

void
StateAnalysis::
ReadTimeoutFile()
{
	ParseTimeouts( &timeoutData );
}

/*
 * FUNCTION
 * 	StateAnalysis::StartAnalysis
 * 
 * DESCRIPTION
 * 	Processes messages that aren't handled by the child processes.
 */

void
StateAnalysis::
StartAnalysis()
{
	SYSTEMTIME		time;
    UploadedOrder_t *   uploadedOrder;

	analysisStarted = true;		// Signals that our data structs are initialized for a run.
	analysisDone = false;		// Signals whether the Engine has finished.
	resultsComplete = false;	// Signals whether ResultsCalc has finished.
	receivedRotorName = false;	// Tells AnalysisInProcess that we've received a rotor name.
	rotorNameValid = false;		// Tells AnalysisInProcess that we previously received a rotor name.
	runCanceled = false;		// Set true when cancel is confirmed.
	analysisInProcess = false;	// Set true when we actually start the analysis run.
	progBarValue = 0;

	// Initialize our run-time info.
	memset( &runInfo, 0, sizeof( RunInfo_t ) );
	runInfo.sampleType = STYPE_NONE;
	allInputReceived = false;
	sampleTypeSelected = false;

    // If we're running an uploaded order, copy the information into runInfo.
    if ( uploadedOrderInProcess )
    {
        uploadedOrder = orderQueue.GetEntryInfo( uploadedOrderInProcess );
        if ( uploadedOrder != 0 )
        {
            strcpy( runInfo.patientControlId, uploadedOrder->patientId );
            strcpy( runInfo.sampleId, uploadedOrder->sampleId );
            strcpy( runInfo.alternateId, uploadedOrder->alternateId );
            strcpy( runInfo.doctorId, uploadedOrder->doctorId );
            strcpy( runInfo.location, uploadedOrder->location );
            strcpy( runInfo.phoneNumber, uploadedOrder->phoneNumber );
            strcpy( runInfo.admissionOwnerId, uploadedOrder->admissionOwnerId );
            runInfo.age = uploadedOrder->age;
            runInfo.ageUnits = AGE_UNITS_YEARS;
            runInfo.dateOfBirth = uploadedOrder->dateOfBirth;
            runInfo.lastVaccination = uploadedOrder->lastVaccination;
            runInfo.gender = uploadedOrder->gender;
            runInfo.race = uploadedOrder->race;
            runInfo.sampleType = uploadedOrder->sampleType;
            runInfo.patientType = uploadedOrder->patientType;
            //runInfo.patientControl = (uploadedOrder->actionCode == 'Q') ? 'C' : 'P';
            runInfo.patientControl = (uploadedOrder->actionCode == ACTION_CODE_Q) ? 'C' : 'P';
        }
    }


	// Initialize EngineComm and CalcResults for a new analysis run.
	SendEngineCommMessage( MSG_ENGINE_COMM_INIT_ANALYSIS );

	// Initialize the results data.  If we get a barcode error, we want the
	// results data to be empty.
	memset( rotorResults, 0, sizeof( RotorResults_t ) );
	for ( int i = 0; i < MAX_ANALYTE_TYPES; i++ )
	{
		rotorResults->rotorAnalyteResult[i].analyteType = 0xff;
		rotorResults->rotorAnalyteResult[i].printOrder = 0xff;
	}

	// Send the calibration factors.
	SendCalibrationFactorsMessage();

	// If a Rotor 100, trigger the run start without a barcode read.
	if ( isRotor100 || isVsRotor || systemData->inDemoMode )
	{
		SendAnalysisMessage( MSG_ANALYSIS_SKIP_BARCODE );

		// If the last rotor was also a rotor 100, we don't need any input this time.
		if ( runInfoRotor100.rotorId == 100 )
		{
			allInputReceived = true;

			// Up the counter for the ID string here and put it in runInfo so it's
			// ready for the Analysis In Process screen..
			rotor100Counter++;
			if ( rotor100Counter > 9999999999999LL )
			{
				rotor100Counter = 1;
			}
			sprintf( runInfo.patientControlId, "%lld", rotor100Counter );
		}
	}

	// Not a Rotor 100, so read the barcode.
	else
	{
        SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
        watchDogDisable = true;
		SendBarcodeMessage( 'R' );
		barcodeReadCount = 1;
	}
	
	analyzeError = SERR_NONE;
	
	// Save the current time.
	GetLocalTime(&time);
	SystemTimeToFileTime(&time, &(runInfo.runTime));

	// Update the rotor run count.
	counterData->rotorRunCount += 1;
	WriteCounters( counterData );

    // If in Demo mode, get the demo result.
    if ( systemData->inDemoMode )
    {
        // If we're running an uploaded order, attempt to get a demo result for the
        // specified rotor ID. Otherwise, just get a random demo result.
        if ( uploadedOrder != 0 )
        {
            demoResult = GetDemoResultById( uploadedOrder->rotorId );
            if ( demoResult == 0 )
            {
                demoResult = GetRandomDemoResult();
            }
        }
        else
        {
            demoResult = GetRandomDemoResult();
        }
    }
}

// detect barcode errors
bool
StateAnalysis::
IsBarcodeError (SystemErrNum_t errNum)
{
    if ((errNum == SERR_BC_END_TRANSITION)  ||
        (errNum == SERR_BC_TRANSITIONS)     ||
        (errNum == SERR_BC_NO_START)        ||
        (errNum == SERR_BC_NO_STOP)         ||
        (errNum == SERR_BC_BAD_DECODE)      ||
        (errNum == SERR_BC_QUIET_ZONE)) {
        return true;
    }
    return false;

}


//process Engine error code
//in the case of error code is barcode errror
//send calibration command to Engine

SystemErrNum_t
StateAnalysis::
ProcessEngineError (SystemErrNum_t errNum)
{
    SystemErrNum_t err ;

    err = errNum;

    if (IsBarcodeError (errNum)) {
        if ((barcodeReadCount > 0) && (barcodeReadCount < 4)) {
            // calibration barcode
            barcodeReadCount++;
            if (watchDogDisable == false) {
                SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
                watchDogDisable = true;
            }
            SendBarcodeMessage ('C');
            err = SERR_NONE;
            
        } else {
            barcodeReadCount = 0;
            if (watchDogDisable == true) {
                watchDogDisable = false; 
                SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
            }
        }
    } else {
        // other error message, in case watchdog disable, re-enable it
        if (watchDogDisable == true) {
            watchDogDisable = false;
           SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
         }
    }

    return err;
}

/*
 * FUNCTION
 * 	StateAnalysis::ProcessOtherMessages
 * 
 * DESCRIPTION
 * 	Processes messages that aren't handled by the child processes.
 * 
 * PARAMETERS
 *	msgId - ID of the received message.
 *	data - Received message data.
 */

void
StateAnalysis::
ProcessOtherMessages( int msgId, char * data )
{
	EngineCommMessageData_t *	engineMsg;
	AnalysisOvertempData_t *	overtempData;
	SystemErrorNumber_t			errNum;
	CfgError_t					cfgErr;
	char 						rocFileName[100];
	int							count;
	SYSTEMTIME					time;

	errNum = SERR_NONE;
	
	switch ( msgId )
	{
		case MSG_ENGINE_COMM_OUT_MESSAGE_RECEIVED:
			engineMsg = (EngineCommMessageData_t *)data;
			switch ( engineMsg->message[0] )
			{
				case 'A':
					switch ( engineMsg->message[8] )
					{
						case OPTICAL_DACS:
							LogDebug( "Optical DACs" );
							// This results in the CalcResults process sending
							// us a MSG_ANALYSIS_DAC_TRIMS_COMPLETE message.
							SendCalcResultsMessage( MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE );
							progBarValue = 29;
							SetTimer( timeoutData.beadCheck1);
							break;
						case BEAD_CHK_1:
							LogDebug( "Bead Check 1" );
							SendCalcResultsMessage( MSG_CALC_RESULTS_BEAD_CHECK_1_COMPLETE );
							progBarValue = 30;
							SetTimer( timeoutData.beadCheck2);
							break;
						case BEAD_CHK_2:
							LogDebug( "Bead Check 2" );
							SendCalcResultsMessage( MSG_CALC_RESULTS_BEAD_CHECK_2_COMPLETE );
							SetTimer( timeoutData.distributionCheck );
							break;
						case DISTRIBUTION_CHK:
							LogDebug( "Dist Check" );
							SendCalcResultsMessage( MSG_CALC_RESULTS_DISTRIBUTION_CHECK_COMPLETE );
							progBarValue = 55;
							SetTimer( timeoutData.mixDone );
							break;
						case MIX_DONE:
							LogDebug( "Mix Done" );
							SendAMessage( 'S' );
							SendHeaterControlMessage( MSG_HEATER_CONTROL_MIX_DONE );
							progBarValue = 65;
							SetTimer( timeoutData.blackOffsets );
							break;
						case BLACK_OFFSETS:
							LogDebug( "Black Offsets" );
							SendCalcResultsMessage( MSG_CALC_RESULTS_BLACK_OFFSETS_COMPLETE );
							progBarValue = 66;
							SetTimer( timeoutData.pass );
							break;
						case PRE_SAMPLE:
							LogDebug( "Pre Sample" );
							progBarValue = 68;
							break;
						case PASS_DONE:
							LogDebug( "Pass Done: %d", engineMsg->message[9] );
							progBarValue += 2;
							SetTimer( timeoutData.pass );
							break;
						case POST_SAMPLE:
							LogDebug( "Post Sample" );
							progBarValue += 2;
							SetTimer( timeoutData.analysisDone );
							break;
						case ANALYSIS_DONE:
							LogDebug( "Analysis Done" );
							analysisDone = true;
							SendGuiMessage( MSG_GUI_SCREENSAVE_DISABLE );
							// Cancel the timeout timer.
							SetTimer( -1 );
							SendHeaterControlMessage( MSG_HEATER_CONTROL_ANALYSIS_DONE );
							break;
						case CANCEL_DONE:
							break;

						default:
							LogDebug( "'A' msg: %d", engineMsg->message[8] );
							break;
					}
					break;
				
				case 'B':
					LogDebug( "got barcode message" );

                    SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_ENABLE);
                    watchDogDisable = false;
					// Cancel the timeout timer.
					SetTimer( -1 );
					
					// Don't do anything if the user just canceled the run.
					if ( runCanceled )
					{
						break;
					}
					
					barcodeReadCount++;

					// Process the barcode data to get the stored info.  
					//errNum = ProcessRotorBarcode( (const unsigned short *)(engineMsg->message + 8), barcodeData );
					errNum = ProcessRotorBarcode( (const unsigned short *)(engineMsg->message + BARCODE_DATA_OFFSET), barcodeData );
					// Ignore date errors if we're running a rotor 1.
					if ( (barcodeData->rotorId == 1) &&
						 ( (errNum == SERR_BC_ILLEGAL_DATE) ||
						   (errNum == SERR_IMPROPER_DATE) ||
						   (errNum == SERR_EXPIRED_ROTOR) ) )
					{
						errNum = SERR_NONE;
					}

                    // If an uploaded order was selected, and this rotor has a different ID from
                    // what's specified in the order, clear the uploadedOrderId so we don't dequeue
                    // the order at the end of the run.
                    if (uploadedOrderInProcess != 0) {
                        if ( orderQueue.GetEntryInfo( uploadedOrderInProcess )->rotorId != barcodeData->rotorId )
                        {
                            uploadedOrderInProcess = 0;
                            uploadedOrderSelected = 0;
                        }
                    }

					if ( systemError.GetAction( errNum ) == SERR_ACTION_NONE )
					{
                        // success read barcode, reset count to be 0
                        barcodeReadCount = 0;
						// If we're in Research mode and we got a barcode error,
						// we need to save the error now because we overwrite
						// errNum later.
						if ( systemData->inResearchMode && (runInfo.errNum == SERR_NONE) )
						{
							runInfo.errNum = errNum;
						}

						// Update calibration.xml with new barcode DAC values.
						UpdateBarcodeCalibrationData( engineMsg->message );

						// Signal to our results processing routine that we've
						// read a valid barcode.
						runInfo.barcodeRead = true;

						// Rotor 1, so set the needed flags, etc. We don't have a ROC file,
						// so we have to hard-code some information.
						if ( barcodeData->rotorId == 1 )
						{
							isRotor1 = true;

							memset( rotorConfig, 0, sizeof( RotorConfig_t ) );
							strcpy( rocFileName, "None" );
							sprintf( rotorConfig->rotorPrinterName, "Rotor 1" );
							sprintf( rotorConfig->rotorScreenNameLine1, "Rotor 1" );
                            // set rotor number here for DistributionChecks in calculaResults process 
                            rotorConfig->rotorNumber = 1;
						    	
                            
							// Set font to standard font color and background to white.
							rotorConfig->rotorScreenNameFontColor = 0x004E4E5E;
							rotorConfig->rotorScreenNameBackgroundColor = 0x00FFFFFF;

							cfgErr = CFG_ERR_NONE;

                            //If we've run a Rotor 1 previously, we need to copy data over
                            // so we don't request user input.
							if ( runInfoRotor1.rotorId == 1 )
							{
								runInfoRotor1.errNum = SERR_NONE;
								memcpy( &runInfo, &runInfoRotor1, sizeof( RunInfo_t ) );

								// We want the current time, though.
								GetLocalTime(&time);
								SystemTimeToFileTime(&time, &(runInfo.runTime));
								
                                // Decrement the counter for the ID string.
                                rotor1Counter--;

                                // Rotor 1 has been run the previously-specified number of
                                // times, so we don't run another Rotor 1.
                                if ( rotor1Counter < 1 )
                                {
                                    // Set a flag so the foreground state machine can go to the
                                    // correct state.
                                    rotor1RunsComplete = true;
                                }

                               // Set the patient/control ID field to the rotor 1 count.
                                else
                                {
                                    sprintf( runInfo.patientControlId, "%lld", rotor1Counter );
                                }

                                // Signal to the foreground state machine that we've got all
                                // the input we need and it should go on to the Analysis in
                                // Process screen.

								allInputReceived = true;
								sampleTypeSelected = true;
							}
                            // No Rotor 1 run previously, so if the patient/control ID has
                            // been entered then we get the count for # of Rotor 1 runs.
                            // Otherwise, we have to wait for the ID to be entered.
                            else if ( runInfo.patientControlId[0] != '\0' )
                            {
                                Rotor1GetCount();
                            }
                        
                        }
						
						// Read the ROC file for this rotor.
						else
						{
							isRotor1 = false;

							sprintf( rocFileName, "%s/%d.%d.roc",
									 ROC_FILE_DIRECTORY,
									 barcodeData->rotorId,
									 barcodeData->rotorFormat );

							cfgErr = ParseRoc( rocFileName, rotorConfig );
							if ( cfgErr != CFG_ERR_NONE )
							{
								if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
								{
									errNum = SERR_ROC_NOT_THERE;
								}
								else
								{
									errNum = SERR_FILE_READ;
								}
								LogError( "ROC parse error # %d", cfgErr );
							}

						}
						
						// Save the rotor ID.
						runInfo.rotorId = barcodeData->rotorId;
                        
                        // ROC read was OK, so go on with the run unless we've got a Rotor 1 
                        // and we've hit the total run count.
						if ( ( cfgErr == CFG_ERR_NONE ) && !(isRotor1 && rotor1RunsComplete) )
						{
                            // Make sure the rotor is OK for this instrument
							if ( !systemData->inDemoMode &&
								 !systemData->inResearchMode &&
								 (runInfo.rotorId != 1) &&
								 (rotorConfig->rotorProductCode != factoryData->abaxisProductCode) )
							{
								if ( factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE )
								{
									errNum = SERR_ILLEGAL_VETSCAN_ROTOR;
								}
								else
								{
									errNum = SERR_ILLEGAL_PICCOLO_ROTOR;
								}
							}
							
							// Rotor is OK for this instrument, so send the RIS file
							// to the Engine and start the run.							
							else
							{
								char *		fileName;
								
								fileName = strrchr(rocFileName, '/');
								if (fileName != NULL) {
									fileName++;
									strncpy( runInfo.rocFileName, fileName, sizeof( runInfo.rocFileName ) );
								} else {
									strncpy( runInfo.rocFileName, rocFileName, sizeof( runInfo.rocFileName ) );
								}
								errNum = SendRisFile( barcodeData->rotorId );

								if ( errNum == SERR_NONE )
								{
									/*
									 * Here is where we start the actual analysis run.  CalcResults needs
									 * to initialize its results data, the Engine needs to know it must
									 * start running the RIS file profile, and HeaterControl needs to know
									 * it must check for a needed heater kick because of the introduction of
									 * a rotor that isn't up to temperature.
									 */
									SendCalcResultsMessage( MSG_CALC_RESULTS_INIT_CALCULATION );
									SendAMessage( 'M' );
									SendHeaterControlMessage( MSG_HEATER_CONTROL_START_ANALYSIS );
									strcpy( runInfo.rotorName, rotorConfig->rotorPrinterName );
									SetTimer( timeoutData.opticalDacs );
									receivedRotorName = true;
									analysisInProcess = true;
								}
							}
						}
					}

					// If we didn't get a good barcode read, and our count isn't too high,
					// re-read the barcode.
					else if ( errNum != SERR_BC_ILLEGAL_DATE &&
							  errNum != SERR_IMPROPER_DATE   &&
							  errNum != SERR_EXPIRED_ROTOR   &&
							  barcodeReadCount < 4 )
					{
                         
                        SendHeaterControlMessage (MSG_HEATER_CONTROL_WATCH_DOG_DISABLE);
                        watchDogDisable = true;
						SendBarcodeMessage( 'C' );
						errNum = SERR_NONE;
					}
					break;
				
				// Error number sent by the Engine.
				case 'E':
                    errNum = ProcessEngineError ((SystemErrorNumber_t)*(short *)&engineMsg->message[8]);
                    if (errNum != SERR_NONE) {
                        LogError( "got Engine error %x", errNum );
                    }
					break;

				// Engine debug message.
				case '?':
					count = *(unsigned short *)&engineMsg->message[2];
					LogInfo( "Engine ? msg: %*.*s", count, count, &engineMsg->message[4] );
					break;
				
				// Indicates the Engine has reset asynchronously.
				case 'Z':
					errNum = SERR_ENGINE_RESET;
					break;

				default:
					LogError( "unexpected Engine Comm msg: %c (%d)", engineMsg->message[0], engineMsg->message[0] );
					break;
			}
			break;

        /*
         * Skip Barcode read - used for Rotor 100 and Demo mode.
         */
		
		case MSG_ANALYSIS_SKIP_BARCODE:

			// Don't do anything if the user just canceled the run.
			if ( runCanceled )
			{
				break;
			}

            if ( isRotor100 )
            {
                LogDebug ("skip, rotor is 100");

			    // Hard-code info.
			    memset( barcodeData, 0, sizeof( BarcodeData_t ) );
			    barcodeData->rotorId = 100;		
			    memset( rotorConfig, 0, sizeof( RotorConfig_t ) );
			    rotorConfig->rotorNumber = 100;	
			    sprintf( rotorConfig->rotorPrinterName, "Rotor 100" );
			    sprintf( rotorConfig->rotorScreenNameLine1, "Rotor 100" );
			    strcpy( runInfo.rocFileName, "None" );
			    runInfo.rotorId = 100;
			    runInfo.barcodeRead = true;

			    // Set font to standard font color and background to white.
			    rotorConfig->rotorScreenNameFontColor = 0x004E4E5E;
			    rotorConfig->rotorScreenNameBackgroundColor = 0x00FFFFFF;

			    cfgErr = CFG_ERR_NONE;
							
			    // If the previous rotor was a rotor 100 also, we need
			    // to copy some data over so we don't request user input.
                // ZZZ wliu ( runInfoRotor100.rotorId ???)
			    if ( runInfoRotor100.rotorId == 100 )
			    {
				    runInfoRotor100.errNum = SERR_NONE;
				    memcpy( &runInfo, &runInfoRotor100, sizeof( RunInfo_t ) );

				    // We want the current time, though.
				    GetLocalTime(&time);
				    SystemTimeToFileTime(&time, &(runInfo.runTime));
								
				    // NOTE: We up the counter for the rotor ID up in StartAnalysis so
				    //		 it's there for the Analysis In Process screen.
				    sampleTypeSelected = true;
			    }
							
			    // If the last rotor wasn't also a Rotor 100, init the rotorId in runInfoRotor100,
			    // reset the counter, and put the ID in runInfo.
			    else
			    {
				    runInfoRotor100.rotorId = 100;
				    rotor100Counter = 1;
				    sprintf( runInfo.patientControlId, "%lld", rotor100Counter );
			    }
							
			    errNum = SendRisFile( 100 );
            }
            
            // Using uploaded rotor info, so use the uploaded barcode data and ROC and RIS files.
            else if ( isVsRotor )
            {
                errNum = ReadUploadedBarcodeFile();
                if ( errNum == SERR_NONE )
                {
                    runInfo.barcodeRead = true;
                    
                    cfgErr = ParseRoc( UPLOADED_ROTOR_ROC_NAME, rotorConfig );
                    if ( cfgErr != CFG_ERR_NONE )
                    {
                        if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
                        {
                            errNum = SERR_ROC_NOT_THERE;
                            LogError( "Uploaded ROC file not found" );
                        }
                        else
                        {
                            errNum = SERR_FILE_READ;
                            LogError( "Parse error in uploaded ROC file" );
                        }
                    }
                    else
                    {
                        errNum = SendRisFile( UPLOADED_ROTOR_ID );
                    }
                }
            }
            // Not Rotor 100 or uploaded rotor, so it must be Demo mode.
            else
            {

                // Read the ROC file. If not found, we start going through all the demo results, in order, looking for one
                // that does have a ROC file.
                sprintf( rocFileName, "%s/%d.%d.roc", ROC_FILE_DIRECTORY, demoResult->rotorId, demoResult->rotorFormat );
                cfgErr = ParseRoc( rocFileName, rotorConfig );
                if ( cfgErr != CFG_ERR_NONE )
                {
                    LogError( "ROC file not found: %s", rocFileName );
                    demoResult = GetFirstDemoResult(0);
                    do
                    {
                        sprintf( rocFileName, "%s/%d.%d.roc", ROC_FILE_DIRECTORY, demoResult->rotorId, demoResult->rotorFormat );
                        cfgErr = ParseRoc( rocFileName, rotorConfig );
                        if ( cfgErr != CFG_ERR_NONE )
                        {
                            LogError( "ROC file not found: %s", rocFileName );
                            demoResult = GetNextDemoResult(0);
                        }
                    } while ( (demoResult != 0) && (cfgErr != CFG_ERR_NONE) );
                }

                // If we failed to find a ROC file, set the error flag.
                if ( cfgErr != CFG_ERR_NONE )
                {
                    if ( cfgErr == CFG_ERR_FILE_DOES_NOT_EXIST )
                    {
                        errNum = SERR_ROC_NOT_THERE;
                    }
                    else
                    {
                        errNum = SERR_FILE_READ;
                    }
                    LogError( "ROC parse error # %d", cfgErr );
                }
                
                // ROC file parsed OK.
                else
                {

                    // Get the barcode data from the demo dataset.
                    memset( barcodeData, 0, sizeof( BarcodeData_t ) );
                    barcodeData->rotorId = demoResult->rotorId;
                    barcodeData->rotorFormat = demoResult->rotorFormat;
                    barcodeData->rotorMfgLot.lsdOfYear = demoResult->lotNum.lsdOfYear;
                    barcodeData->rotorMfgLot.weekOfYear = demoResult->lotNum.weekOfYear;
                    barcodeData->rotorMfgLot.dayOfWeek = demoResult->lotNum.dayOfWeek;
                    strcpy( barcodeData->rotorMfgLot.code, demoResult->lotNum.code );
                    runInfo.barcodeRead = true;
                    errNum = SERR_NONE;
    
                    memset( &runInfoRotor100, 0, sizeof( RunInfo_t ) );
                    isRotor100 = false;
    
                    sprintf( rocFileName, "%s/%d.%d.roc",
                             ROC_FILE_DIRECTORY,
                             barcodeData->rotorId,
                             barcodeData->rotorFormat );
    
    
                    // We want the name from the demo file, not the one from the ROC file, on the
                    // results printouts and displays.
                    strcpy( rotorConfig->rotorPrinterName, demoResult->rotorName );
                    
                    // Send the RIS file to the Engine.
                    errNum = SendRisFile( DEMO_ROTOR_ID );
                }
            }

			if ( errNum == SERR_NONE )
			{
				/*
				 * Here is where we start the actual analysis run.  CalcResults needs
				 * to initialize its results data, the Engine needs to know it must
				 * start running the RIS file profile, and HeaterControl needs to know
				 * it must check for a needed heater kick because of the introduction of
				 * a rotor that isn't up to temperature.
				 */
				SendCalcResultsMessage( MSG_CALC_RESULTS_INIT_CALCULATION );
				SendAMessage( 'M' );
				SendHeaterControlMessage( MSG_HEATER_CONTROL_START_ANALYSIS );
				strcpy( runInfo.rotorName, rotorConfig->rotorPrinterName );
				SetTimer( timeoutData.opticalDacs );
				receivedRotorName = true;
				analysisInProcess = true;
			}
			break;

		case MSG_ANALYSIS_RESULTS_CALC_COMPLETE:
			SetTimer( -1 );
			resultsComplete = true;
			LogDebug( "Results calc complete" );

			errNum = (SystemErrorNumber_t)*(short *)data;

			if ( !allInputReceived )
			{
				SendGuiMessage( MSG_GUI_SCREENSAVE_DISABLE );
			}
			break;

		/*
		 * Received after OPTICAL_DACS received from the Engine and CalcResults
		 * calculates new values.
		 */
		case MSG_ANALYSIS_DAC_TRIMS_COMPLETE:
			SendCalibrationFactorsMessage();
			cfgErr = WriteCalibration( calibrationData );
			if ( cfgErr != CFG_ERR_NONE )
			{
				LogError( "error writing calibration file: %d", cfgErr );
			}
			break;

		case MSG_ANALYSIS_ERROR:
			errNum = (SystemErrorNumber_t)*(short *)data;
			break;
		
		case MSG_ANALYSIS_OVERTEMP:
			overtempData = (AnalysisOvertempData_t *)data;
			isOvertemp = overtempData->overtempFlag;
			LogDebug( "overtemp = %s", isOvertemp ? "true" : "false" );
			LogDebug( "rotorTemperature = %0.2f", overtempData->rotorTemperature );
			LogDebug( "ambientTemperature = %0.2f", overtempData->ambientTemperature );
			LogDebug( "topPlateTemperature = %0.2f", overtempData->topPlateTemperature );
			LogDebug( "bottomPlateTemperature = %0.2f", overtempData->bottomPlateTemperature );
			LogDebug( "topPlateVoltage = %0.3f", overtempData->topPlateVoltage );
			LogDebug( "bottomPlateVoltage = %0.3f", overtempData->bottomPlateVoltage );
			break;
			
		case MSG_ANALYSIS_TEMPERATURE:
			// For now, we're ignoring these messages.
			break;

		case MSG_ANALYSIS_TIMER:
			errNum = SERR_ANALYSIS_TIMEOUT;
			break;

		case MSG_ANALYSIS_WARMUP_DONE:
			warmupComplete = true;

		// Just ignore - these get sent at start-up by Recall and Settings.
		case MSG_ANALYSIS_GOTO_FOREGROUND:
			break;
    
        case MSG_ANALYSIS_UPLOADED_ROTOR:
            strncpy( uploadedRotorName, data, UPLOADED_ROTOR_NAME_LEN );
            uploadedRotorReceived = true;
            break;
    
        case MSG_ANALYSIS_UPLOADED_ORDER:
            SendGuiMessage( MSG_GUI_SOUND_PRINTING_DONE );
            SendGuiMessage( MSG_GUI_SCREENSAVE_WAKEUP );
            orderQueue.ProcessAstmOrder( (UploadedOrder_t *)data );
            break;

		default:
			LogDebug( "received %d message", msgId );
			break;
	}

	// Handle any error.
	if ( errNum != SERR_NONE )
	{
		switch ( systemError.GetAction( errNum ) )
		{
			case SERR_ACTION_NONE:
				// If we're in research mode and there was no previous error,
				// save this error for reporting and printing at the end of the run.
				if ( systemData->inResearchMode && (runInfo.errNum == SERR_NONE) )
				{
					runInfo.errNum = errNum;
				}
				LogError( "error: %x", errNum );
				break;

			case SERR_ACTION_ABORT:
			case SERR_ACTION_SHUTDOWN:
            case SERR_ACTION_RESTART:
                // If this is a 4214 error, check to see if we've reached our limit. If not, just
                // log the error and go on.
                if ( errNum == SERR_WAVE_CV_UNREF )
                {
                    num4214Errors++;
                    if ( num4214Errors < systemData->max4214Errors )
                    {
                        LogError( "error: %x", errNum );
                        errNum = SERR_NONE;
                        break;
                    }
                }


				// Cancel the timer.
				SetTimer( -1 );

				LogError( "error: %x", errNum );
				
				runInfo.errNum = errNum;

				if ( analysisInProcess )
				{
					// Tell the Engine to cancel.
					SendAMessage( 'C' );

					// Put heater control back in idling.
					SendHeaterControlMessage( MSG_HEATER_CONTROL_ANALYSIS_DONE );
				}

				// Update the counter
				counterData->rotorAbortCount++;
				
				// Set the global error flag.
				analyzeError = errNum;

				break;

			case SERR_ACTION_WARNING:
				// Let the current state handle the error.
				analyzeError = errNum;
				break;
		}
	}
}

/*
 * FUNCTION
 * 	StateAnalysis::ProcessResults
 * 
 * DESCRIPTION
 * 	Processes the results at the end of a run or upon an aborted run. Handles
 * 	the special case of a 655321 demo ID, saves the results to the database,
 * 	writes the RAD file and updates the flash count in the counters file.
 * 
 * RETURNS
 *	System error number, of SERR_NONE if there is no error.
 */

SystemErrNum_t
StateAnalysis::
ProcessResults()
{
	const char *	radFileName;
	SystemErrNum_t	errNum = SERR_NONE;

	if ( !analysisStarted )
	{
		return SERR_NONE;
	}

	if ( systemData->inDemoMode )
	{
		// If we've got the "A Clockwork Orange" sample ID, populate the
		// database with all the other demo results first, before storing the
		// current one.
		if ( strcmp( runInfo.patientControlId, "655321" ) == 0 )
		{
			DemoResult_t *	result;
			MfgLot_t		saveMfgLot;
			char			saveRotorName[100];

			runInfo.radFileName[0] = '\0';
			
			// Save the lot number and rotor name for the current rotor.
			memcpy( &saveMfgLot, &barcodeData->rotorMfgLot, sizeof( MfgLot_t ) );
			strcpy( saveRotorName, rotorConfig->rotorPrinterName );

			result = GetFirstDemoResult( demoResult );
            while ( result != 0 )
			{
				PopulateDemoResults( result, rotorResults );

				// Install the lot number and rotor name for this demo result.
				memcpy( &barcodeData->rotorMfgLot, &result->lotNum, sizeof( MfgLot_t ) );
				strcpy( rotorConfig->rotorPrinterName, result->rotorName );

				SaveResults( rotorResults, true );		// Flag no print or ASTM.
				result = GetNextDemoResult( demoResult );
			}

			// Restore the lot number and rotor name for the current rotor.
			memcpy( &barcodeData->rotorMfgLot, &saveMfgLot, sizeof( MfgLot_t ) );
			strcpy( rotorConfig->rotorPrinterName, saveRotorName );
			
		}

		// Put the demo results into the rotorResults data struct, in preparation
		// for writing the RAD file and saving and printing the results.
		PopulateDemoResults( demoResult, rotorResults );
	}

    // set low/high fan rotorSystemFlags before saved to Rad file 
    if (settingsData->fanIdleSpeed == IDLE_FAN_LOW_SPEED) {
        rotorResults->rotorInformationResults.rotorSystemFlags |= LOW_IDLE_FAN_SET;
    } else {
        rotorResults->rotorInformationResults.rotorSystemFlags &= (~LOW_IDLE_FAN_SET);
    }

	// Write the RAD file to disk.
	radFileName = WriteRadFile( &runInfo, factoryData, systemData, versionData, calibrationData, barcodeData, rotorResults, rawData );

	// Save the rad file name. (Needed by SaveResults.)
	strcpy( runInfo.radFileName, radFileName );

	// Save the results in the results database.
	errNum = SaveResults( rotorResults, false );

	// Update the flash count.
	counterData->flashCount += rawData->receivedDataCounter.flashCount;
	WriteCounters( counterData );
	
	analysisStarted = false;

	return errNum;
}

void
StateAnalysis::
CreateResultsDatabase()
{
	FILE *				fdIndex;
	FILE *				fdData;
	static ResultsDbIndex_t	resultsIndex;
	const char *		dbIndex;
	const char *		dbData;
	int					status = 0;
	HANDLE				hFdData;
	wchar_t				fdDataWSTR[255];
	
	if ( IN_CUSTOMER_MODE )
	{
		dbIndex = CUSTOMER_DB_INDEX;
		dbData = CUSTOMER_DB_DATA;
	}
	else
	{
		dbIndex = OTHER_DB_INDEX;
		dbData = OTHER_DB_DATA;
	}

	wsprintf(fdDataWSTR, L"%S", dbData);

	LogDebug( "creating new %s database", IN_CUSTOMER_MODE ? "customer" : "research/manufacturing" );

	/*
	 * Index file.
	 */

	// First, delete the file if it exists.
	fdIndex = fopen( dbIndex, "wb" );
	if ( fdIndex == 0 )
	{
		LogError( "error creating results index file" );
		return;
	}
	memset( &resultsIndex, 0xFF, sizeof(ResultsDbIndex_t ) );
	resultsIndex.dbVersion = RESULTS_DB_VERSION;
	resultsIndex.lastEntry = RESULTS_MAX - 1;
	fwrite( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, fdIndex );
	fclose( fdIndex );

	/*
	 * Data file.
	 */

	// First, we assume the file is already there and just truncate it to the proper size.
	hFdData = CreateFile(fdDataWSTR,GENERIC_WRITE,
                     0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFdData != INVALID_HANDLE_VALUE )
	{
		SetFilePointer( hFdData, RESULTS_MAX * sizeof( ResultsDbEntry_t ), NULL, FILE_BEGIN );
		SetEndOfFile( hFdData );
		CloseHandle( hFdData );
	}

	// If that fails, either the file doesn't exist or there's something wrong with it.
	//if ( status != -1 )
	else
	{
		// Start by deleting the file if it exists.
		// Now, create it and set its length.
		fdData = fopen( dbData, "wb" );
		if ( fdData == 0 )
		{
			LogError( "error creating results database file" );
			return;
		}
		fclose( fdData );

		hFdData = CreateFile(fdDataWSTR,GENERIC_WRITE,
                     0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFdData != INVALID_HANDLE_VALUE )
		{
			SetFilePointer( hFdData, RESULTS_MAX * sizeof( ResultsDbEntry_t ), NULL, FILE_BEGIN );
			SetEndOfFile( hFdData );
			CloseHandle( hFdData );
		}
	}
}

/*
 * FUNCTION
 * 	StateAnalysis::SaveResults
 * 
 * DESCRIPTION
 * 	Called by ProcessResults to save the current results to the results database.
 * 
 * PARAMETERS
 *	rotorResults - Pointer to the results.
 *	is655321 - Flag to indicate 655321, so don't print, send the results out the
 * 	serial port, or notify ASTM.
 */

SystemErrNum_t
StateAnalysis::
SaveResults( RotorResults_t * rotorResults, bool is655321 )
{
	FILE *				indexFile = 0;
	FILE *				dataFile = 0;
	static ResultsDbIndex_t	resultsIndex;
	ResultsDbEntry_t	resultsDbEntry;
	bool				resultSuppressed = false;
	SystemErrNum_t		dbErrNum = SERR_NONE;
	SystemErrNum_t		printErrNum = SERR_NONE;
	HANDLE				lock;
	const char *		dbIndex;
	const char *		dbData;
	bool				createDb = false;
	DWORD				error;
	DWORD				dwWaitResult;
	
	if ( IN_CUSTOMER_MODE )
	{
		dbIndex = CUSTOMER_DB_INDEX;
		dbData = CUSTOMER_DB_DATA;
	}
	else
	{
		dbIndex = OTHER_DB_INDEX;
		dbData = OTHER_DB_DATA;
	}

	memset( &resultsDbEntry, 0, sizeof( ResultsDbEntry_t ) );

	// If we aborted before the user entered the sample ID, put the error code
	// in there.  Update runInfo.patientControlId because it's used twice - once in the
	// index and once in the results db.
	if ( runInfo.patientControlId[0] == '\0' )
	{
#if 0
        if (settingsData->language == LANGUAGE_CHINESE) {
            //should not save any chinese char to data base
		    sprintf( runInfo.patientControlId, "%s %X", WstrToCstr (strings.English (RSTR_ERROR)), runInfo.errNum );
        } else {
		    sprintf( runInfo.patientControlId, "%s %X", WstrToCstr (strings[ RSTR_ERROR ]), runInfo.errNum );
        }
#else
        sprintf ( runInfo.patientControlId, "%s %X", WstrToCstr (strings.PrintString (RSTR_ERROR)), runInfo.errNum);
#endif
	}

	// Lock the database.
	lock = CreateMutex( 
		NULL,           	// default security attributes
		FALSE,       		// initially not owned
		RESULTS_LOCK);		// unnamed mutex
	if ( lock == NULL ) {
		LogError( "unable to open handle to lock mutex" );
		return SERR_NONE;
	}
	dwWaitResult = WaitForSingleObject( lock, INFINITE );
	if ( dwWaitResult != WAIT_OBJECT_0 )
	{
		LogError( "unable to lock database" );
		CloseHandle(lock);
		return SERR_NONE;
	}

	/*
	 * Open and read the index file.
	 */
	indexFile = fopen( dbIndex, "r+b" );
	error = GetLastError();
	if ( indexFile == NULL )
	{
		createDb = true;
		if ( error != ERROR_FILE_NOT_FOUND )		// ENOENT = file does not exist.
		{
			LogError( "results index file corrupt" );
			dbErrNum = SERR_RESULT_CORRUPTION;
		}
	}
	else
	{
		fread( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, indexFile );
		if ( resultsIndex.dbVersion != RESULTS_DB_VERSION )
		{
			LogError( "results DB wrong version (is %d, should be %d)", resultsIndex.dbVersion, RESULTS_DB_VERSION );
			createDb = true;
			fclose( indexFile );
		}
	}

	/*
	 * If we opened the index file OK and we've got the right version, open the data file.
	 */

	if ( !createDb )
	{
		dataFile = fopen( dbData, "r+b" );
		if ( dataFile == 0 )
		{
			LogError( "error opening results database file" );
			createDb = true;
			dbErrNum = SERR_RESULT_CORRUPTION;
			fclose( indexFile );
		}
	}

	/*
	 * If we had a problem with either of the two files, create a new database and open the two
	 * files again.
	 */

	if ( createDb )
	{
		CreateResultsDatabase();

		indexFile = fopen( dbIndex, "r+b" );
		if ( indexFile == 0 )
		{
			LogError( "unable to open index file after new DB created" );
			ReleaseMutex(lock);
			CloseHandle(lock);
			return SERR_FILE_WRITE;		// Shutdown.
		}
		else
		{
			fread( &resultsIndex, sizeof( ResultsDbIndex_t ), 1, indexFile );
		}

		dataFile = fopen( dbData, "r+b" );
		if ( dataFile == 0 )
		{
			LogError( "unable to open data file after new DB created" );
			fclose( indexFile );
			ReleaseMutex(lock);
			CloseHandle(lock);
			return SERR_FILE_WRITE;		// Shutdown.
		}
	}

	/*
	 * Write the index info.
	 */

	// Point to the entry where we should store our result.
	if ( ++resultsIndex.lastEntry >= RESULTS_MAX )
	{
		resultsIndex.lastEntry = 0;
	}

	strcpy( resultsIndex.entries[ resultsIndex.lastEntry ].patientControlId, runInfo.patientControlId );

	resultsIndex.entries[ resultsIndex.lastEntry ].patientControl = runInfo.patientControl;

	// Set the error flag if we encountered a critical error or abort error during the run.
	resultsIndex.entries[ resultsIndex.lastEntry ].errorFlag = runInfo.errNum != SERR_NONE;

	resultsIndex.entries[ resultsIndex.lastEntry ].dateTime = runInfo.runTime;
	resultsIndex.entries[ resultsIndex.lastEntry ].index = resultsIndex.lastEntry;

	// Flag that we've changed this entry, so ASTM will know about it.
	(*dbChanged)[ resultsIndex.lastEntry ] = true;

	fseek(indexFile, 0L, SEEK_SET);
	fwrite( &resultsIndex, sizeof ( ResultsDbIndex_t ), 1, indexFile );
	fclose( indexFile );

	/*
	 * Write the results data.
	 */

	// Fill in the results record.	
	CreateResultsRecord( &resultsDbEntry, rotorResults, &resultSuppressed );
	
	// Write the current results and close the data file.
	fseek( dataFile, resultsIndex.lastEntry * sizeof( ResultsDbEntry_t ), SEEK_SET );
	fwrite( &resultsDbEntry, sizeof ( ResultsDbEntry_t ), 1, dataFile );
	fclose( dataFile );

	// Unlock the database.
	ReleaseMutex(lock);
	CloseHandle(lock);

	/*
	 *  If not a 655321, then print and transmit the results.
	 */
	if ( !is655321 )
	{
		printErrNum = PrintAndTransmitResults( resultsIndex.lastEntry, resultSuppressed );
	}

	return ( dbErrNum != SERR_NONE ? dbErrNum : printErrNum );
}

/*
 * FUNCTION
 * 	StateAnalysis::CreateResultsRecord
 * 
 * DESCRIPTION
 * 	Called by SaveResults to fill in the results DB record..
 * 
 * PARAMETERS
 *	resultsDbEntry - Results record to be filled in.
 *	rotorResults - Results.
 * 	resultSuppressed - Flag to indicate that there are suppressed results.
 */

void
StateAnalysis::
CreateResultsRecord( ResultsDbEntry_t * resultsDbEntry, RotorResults_t * rotorResults, bool * resultSuppressed )
{
	int		i;
	float	low;
	float	high;
	float	suppressRefRange;
	FILETIME	currentFT;
	SYSTEMTIME	time;

	// Copy the RotorInformationResults and RotorAnalyteResults fields directly from the results to the result record.
	memcpy( &resultsDbEntry->rotorInformationResults,
		&rotorResults->rotorInformationResults, sizeof( RotorInformationResults_t ) );

	memcpy( &resultsDbEntry->rotorAnalyteResults[ SYSTEM_CHECK ], &rotorResults->rotorAnalyteResult[ SYSTEM_CHECK ], sizeof( RotorAnalyteResult_t ) );
	memcpy( &resultsDbEntry->rotorAnalyteResults[ SAMPLE_BLANK ], &rotorResults->rotorAnalyteResult[ SAMPLE_BLANK ], sizeof( RotorAnalyteResult_t ) );
	memcpy( &resultsDbEntry->rotorAnalyteResults[ RQC ], &rotorResults->rotorAnalyteResult[ RQC ], sizeof( RotorAnalyteResult_t ) );
	memcpy( &resultsDbEntry->rotorAnalyteResults[ IQC ], &rotorResults->rotorAnalyteResult[ IQC ], sizeof( RotorAnalyteResult_t ) );
	for ( i = FIRST_ANALYTE_RESULT_TYPE; i < MAX_ANALYTE_TYPES; i++ )
	{
		if ( (rotorResults->rotorAnalyteResult[ i ].printOrder > 0) && (rotorResults->rotorAnalyteResult[ i ].printOrder < 0xff) )
		{
			// Do not copy if this is a piccolo, eGFR is not enabled or this is a Control, and this is an eGFR result.
			if ( (rotorResults->rotorAnalyteResult[i].analyteType != EGFR)
				 || (settingsData->egfrEnabled && (runInfo.patientType != PATIENT_TYPE_CONTROL))
				 || (factoryData->abaxisProductCode != PICCOLO_PRODUCT_CODE) )
			{
                memcpy( &resultsDbEntry->rotorAnalyteResults[ FIRST_ANALYTE_RESULT_TYPE + rotorResults->rotorAnalyteResult[ i ].printOrder - 1 ], &rotorResults->rotorAnalyteResult[ i ], sizeof( RotorAnalyteResult_t ) );
//				memcpy( &resultsDbEntry->rotorAnalyteResults[ i ], &rotorResults->rotorAnalyteResult[ i ], sizeof( RotorAnalyteResult_t ) );
			}
		}
	}


	resultsDbEntry->rotorId = barcodeData->rotorId;

	// Fill in the barcode field.
	if ( runInfo.barcodeRead )
	{
		strcpy( resultsDbEntry->rotorName, rotorConfig->rotorPrinterName );
		sprintf( resultsDbEntry->lotNumber, "%d%02d%d%s",
				 barcodeData->rotorMfgLot.lsdOfYear,
				 barcodeData->rotorMfgLot.weekOfYear,
				 barcodeData->rotorMfgLot.dayOfWeek,
				 barcodeData->rotorMfgLot.code );
	}
	else
	{

#if 0
        if (settingsData->language == LANGUAGE_CHINESE) {
            //should not save any chinese char to data base
            strcpy (resultsDbEntry->rotorName, WstrToCstr (strings.English (RSTR_UNKNOWN)));
        } else {
		    strcpy( resultsDbEntry->rotorName, WstrToCstr (strings[ RSTR_UNKNOWN ]) );
        }
#else
        strcpy ( resultsDbEntry->rotorName, WstrToCstr (strings.PrintString (RSTR_UNKNOWN)));
#endif
		resultsDbEntry->lotNumber[0] = '\0';
	}

	// Pull a bunch of fields from the RunInfo data struct.
	strcpy( resultsDbEntry->operatorId, runInfo.operatorId );
	strcpy( resultsDbEntry->patientControlId, runInfo.patientControlId );
	strcpy( resultsDbEntry->alternateId, runInfo.alternateId );
	strcpy( resultsDbEntry->doctorId, runInfo.doctorId );
	strcpy( resultsDbEntry->location, runInfo.location );
	strcpy( resultsDbEntry->phoneNumber, runInfo.phoneNumber );
	strcpy( resultsDbEntry->admissionOwnerId, runInfo.admissionOwnerId );
	resultsDbEntry->dateOfBirth = runInfo.dateOfBirth;
	resultsDbEntry->lastVaccination = runInfo.lastVaccination;
	strcpy( resultsDbEntry->sampleId, runInfo.sampleId );
	resultsDbEntry->gender = runInfo.gender;
	resultsDbEntry->sampleType = runInfo.sampleType;
	resultsDbEntry->age = runInfo.age;
	resultsDbEntry->race = runInfo.race;
	resultsDbEntry->ageUnits = runInfo.ageUnits;
	resultsDbEntry->dateTime = runInfo.runTime;
	strcpy( resultsDbEntry->radFileName, runInfo.radFileName );
	resultsDbEntry->errNum = runInfo.errNum;

	// Set the expiration fields.
	FILETIME t = expirys.GetControlExpiration();
	GetLocalTime(&time);
	SystemTimeToFileTime(&time, &currentFT);

	if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) && (t.dwLowDateTime > 0 || t.dwHighDateTime > 0) && (CompareFileTime(&t, &currentFT) == -1) )
	{
		resultsDbEntry->controlExpired = true;
	}

	resultsDbEntry->operatorExpired = runInfo.operatorExpired;
	resultsDbEntry->operatorUnauthorized = runInfo.operatorUnauthorized;
	
	// Add reference ranges to the results, and set the resultsPrintFlags.  At the
	// same time, we determine whether any of the results are suppressed, which
	// will trigger an error report.
	for ( i = 0, *resultSuppressed = false; i < MAX_ROTOR_RESULTS; i++ )
	{
		if ( (resultsDbEntry->rotorAnalyteResults[ i ].printOrder > 0) &&
			 (resultsDbEntry->rotorAnalyteResults[ i ].printOrder <= MAX_ROTOR_RESULTS) )
		{
			// Gender defaults to Unknown, unless we're a piccolo and there was
			// a gender entered that was Male or Female.
			Gender_t gender = GENDER_UNKNOWN;
			if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
				 (resultsDbEntry->gender != GENDER_NONE) )
			{
				gender = resultsDbEntry->gender;
			}
			AnalyteId_t	analyteId = analytes.GetAnalyteId( (AnalyteResultType_t)resultsDbEntry->rotorAnalyteResults[ i ].analyteType );

			// Get the ref range and determine if it should be suppressed.
			low = refRanges->ranges[ runInfo.sampleType ][ analyteId ][ gender-1 ].low;
			high = refRanges->ranges[ runInfo.sampleType ][ analyteId ][ gender-1 ].high;
			suppressRefRange = SuppressRefRange( low, high, runInfo.rotorId, analyteId, (SampleType_t)runInfo.sampleType );

			// If we're not suppressing the reference range and the result is
			// out of range, set the appropriate flag in the resultsPrintFlag.
			if ( (!suppressRefRange) &&
				 ( (resultsDbEntry->rotorAnalyteResults[ i ].analyteResult < low) ||
				   (resultsDbEntry->rotorAnalyteResults[ i ].analyteResult > high) ) )
			{
				resultsDbEntry->rotorAnalyteResults[ i ].resultPrintFlags |= MASK_REF_RANGE_LIMIT;
			}

			// Save the reference range.
			resultsDbEntry->rotorAnalyteResults[ i ].lowReferenceRangeLimit = low;
			resultsDbEntry->rotorAnalyteResults[ i ].highReferenceRangeLimit = high;
			
			// Set flag if result is suppressed.
			if ( resultsDbEntry->rotorAnalyteResults[ i ].resultPrintFlags & MASK_RESULTS_SUPPRESSED )
			{
				// Special case for eGFR: we only flag if eGFR is enabled in Settings.
				if ( resultsDbEntry->rotorAnalyteResults[ i ].analyteType == EGFR )
				{
					 if ( settingsData->egfrEnabled )
					 {
                        LogDebug( "eGFR result suppressed" );
					 	*resultSuppressed = true;
					 }
				}
				else
				{
                    LogDebug( "analyte %d result suppressed",  i );
					*resultSuppressed = true;
				}
			}
		}
	}
}


/*
 * FUNCTION
 * 	StateAnalysis::PrintAndTransmitResults
 * 
 * DESCRIPTION
 * 	Called by SaveResults to print and transmit (SerialComm or ASTM) the results.
 * 
 * PARAMETERS
 *	ndx - Database index number for the current record.
 *	resultSuppressed - flag to indicate that there is at least one suppressed result.
 * 
 * RETURN
 * 	Returns SERR_NONE if there was no print error, or the print error number.
 */

SystemErrNum_t
StateAnalysis::
PrintAndTransmitResults( int ndx, bool resultSuppressed )
{
	bool				abbreviated = false;
	AstmMsg_t			astmMsg;
	Hl7Msg_t			hl7Msg;
	SerialCommMsg_t		serialCommMsg;
	SystemErrNum_t		errNum = SERR_NONE;
    PrinterMsg_t        printerMsg;
	int					i;

	// Set up the SerialComm message.
	serialCommMsg.msgId = MSG_SERIAL_COMM_SEND_REPORT;
	serialCommMsg.reportData.resultIndex = ndx;
	strcpy( serialCommMsg.reportData.radFileName, runInfo.radFileName );
	
	// Start with all flags false for transmitting the reports.
	serialCommMsg.reportData.sendResults = false;
	serialCommMsg.reportData.sendIqc = false;
	serialCommMsg.reportData.sendErrorReport = false;
	
	/*
	 * Print the reports.  These routines read the results from the results
	 * database, so they should be called AFTER updating the database.  Also,
	 * set the flags for transmitting the results to true if we also print any
	 * copies of the reports.
	 */
	
	// Print the selected number of results and iQC reports if there is no
	// error or if we're in research mode.
	if ( (runInfo.errNum == SERR_NONE) || systemData->inResearchMode )
	{
		for ( i = 0; i < settingsData->printNumResults && errNum == SERR_NONE; i++ )
		{
           printerMsg.selectedItem = ndx;
           printerMsg.abbreviated  = abbreviated;
           printerMsg.msgId= MSG_PRINT_RESULTS_BY_INDEX;
           errNum = SendPrinterMessage (&printerMsg);
		   serialCommMsg.reportData.sendResults = true;
		}
			
		for ( i = 0; i < settingsData->printNumIqc && errNum == SERR_NONE; i++ )
		{
            printerMsg.selectedItem = ndx;
            printerMsg.abbreviated  = abbreviated;
            printerMsg.msgId = MSG_PRINT_IQC_BY_INDEX;
            errNum = SendPrinterMessage (&printerMsg);
			serialCommMsg.reportData.sendIqc = true;
		}
	}
	
	// Print the selected number of error reports if there is an error, there
	// is a suppressed result or if error reports is set to non-auto.
    // If in Demo mode, only print error reports if the setting for error reports is Always.
    if ( (!systemData->inDemoMode && ((runInfo.errNum != SERR_NONE) || resultSuppressed)) ||
         (settingsData->errorPrintAuto == false) )
    {
        LogDebug( "Printing error report(s):" );
        if ( !systemData->inDemoMode && (runInfo.errNum != SERR_NONE) )
        {
            LogDebug( "  system error %x", runInfo.errNum );
        }
        else if ( !systemData->inDemoMode && resultSuppressed )
        {
            LogDebug( "  result suppressed" );
        }
        else if ( settingsData->errorPrintAuto == false )
        {
            LogDebug( "  Settings value is not Auto" );
        }
		// If there's a system error and we're not in research mode, we print
		// the abbreviated error report.
		if ( (runInfo.errNum != SERR_NONE) && !systemData->inResearchMode )
		{
			abbreviated = true;
		}
		else
		{
			abbreviated = false;
		}
		
		for ( i = 0; i < settingsData->printNumError && errNum == SERR_NONE; i++ )
		{
             printerMsg.selectedItem = ndx;
             printerMsg.abbreviated  = abbreviated;
             printerMsg.msgId = MSG_PRINT_ERROR_REPORT_BY_INDEX;
             errNum =SendPrinterMessage (&printerMsg);
			 serialCommMsg.reportData.sendErrorReport = true;
			 serialCommMsg.reportData.abbreviatedErrorReport = abbreviated;
		}
	}
		
	// Transmit the reports.
	if (( settings->comm.commInterface != COMM_ASTM ) && ( settings->comm.commInterface != COMM_HL7)) 
	{
		SendSerialCommMessage( &serialCommMsg, sizeof( SendReportData_t ) );
	}
	
	// Let ASTM know there are new results.
	else if (settings->comm.commInterface == COMM_ASTM)
	{
		astmMsg.msgId = MSG_ASTM_LAST_ROTOR_AVAILABLE;
		astmMsg.data = ndx;
		SendAstmMessage( &astmMsg, sizeof( astmMsg.data ) );
	} 
    // Let HL7 know a new resutls available 
	else if (settings->comm.commInterface == COMM_HL7)
	{
		hl7Msg.msgId = MSG_HL7_LAST_ROTOR_AVAILABLE;
		hl7Msg.data = ndx;
		SendHl7Message( &hl7Msg, sizeof( hl7Msg.data ) );
	} 
		
	/*
	 * Let the user know we're done printing the results.  Actually, the results
	 * aren't done printing at this point.  The serial port has a 2K buffer, plus
	 * there's a buffer in the printer, so at this point all we're sure of is
	 * that the results have gone to the serial port's buffer.
	 */
	SendGuiMessage( MSG_GUI_SOUND_PRINTING_DONE );
	
	return errNum;
}


void
StateAnalysis::
SendDrawerMessage( char command )
{
	EngineCommReceiveMsg_t	msg;

	// Create the message.
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'D';
	msg.data.engineMsg.message[1] = command;
	msg.data.engineMsg.length = 2;
		
	// Send the message.
	SendEngineCommMessage( &msg, sizeof( EngineCommMessageData_t ) );
}

void
StateAnalysis::
SendAMessage( char command )
{
	EngineCommReceiveMsg_t	msg;

	// Create the message.
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'A';
	msg.data.engineMsg.message[1] = command;
	msg.data.engineMsg.length = 2;
		
	// Send the message.
	SendEngineCommMessage( &msg, sizeof( EngineCommMessageData_t ) );
}
	
void
StateAnalysis::
SendCalibrationFactorsMessage()
{
	EngineCommReceiveMsg_t	msg;
	int						i;

	// Create the message.
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'C';
	msg.data.engineMsg.message[1] = ESC;
	msg.data.engineMsg.message[2] = 10;		// LSB of binary count.
	msg.data.engineMsg.message[3] = 0;		// MSB of binary count.
	for ( i = 0; i < NUM_WAVELENGTHS; i++ )
	{
		msg.data.engineMsg.message[4+i] = calibrationData->wavelengthDacTrims[i];
	}
	msg.data.engineMsg.length = 14;
		
	// Send the message.
	SendEngineCommMessage( &msg, sizeof( EngineCommMessageData_t ) );
}

void
StateAnalysis::
SendBarcodeMessage( char command )
{
	EngineCommReceiveMsg_t	msg;

	// Create the message.
	msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
	msg.data.engineMsg.message[0] = 'B';
	msg.data.engineMsg.message[1] = command;
	msg.data.engineMsg.message[2] = ESC;
	msg.data.engineMsg.message[3] = 4;		// LSB of binary count.
	msg.data.engineMsg.message[4] = 0;		// MSB of binary count.
	msg.data.engineMsg.message[5] = calibrationData->barcodeDacs[0];
	msg.data.engineMsg.message[6] = calibrationData->barcodeDacs[1];
	msg.data.engineMsg.message[7] = calibrationData->cuvetteMarkDacs[0];
	msg.data.engineMsg.message[8] = calibrationData->cuvetteMarkDacs[1];
	msg.data.engineMsg.length = 9;
		
	// Send the message.
	SendEngineCommMessage( &msg, sizeof( EngineCommReceiveMsg_t ) );
}

void
StateAnalysis::
UpdateBarcodeCalibrationData( unsigned char * message )
{
	calibrationData->barcodeDacs[0] = message[4];
	calibrationData->barcodeDacs[1] = message[5];
	
	WriteCalibration( calibrationData );
}


SystemErrNum_t
StateAnalysis::
SendRisFile( int rotorId )
{
	FILE *					fp;
	char 					risFileName[100];
	unsigned char			fileBuff[3000];
	unsigned				fsize;
	EngineCommReceiveMsg_t	msg;
	DWORD					error;

	if ( rotorId == DEMO_ROTOR_ID )
	{
        const char * demoFileName = systemData->useFastDemo ? FAST_DEMO_FILE_NAME : DEMO_FILE_NAME;
        sprintf( risFileName, "%s/%s", RIS_FILE_DIRECTORY, demoFileName );
	}
    else if ( rotorId == UPLOADED_ROTOR_ID )
    {
        strcpy( risFileName, UPLOADED_ROTOR_RIS_NAME );
    }
	else
	{
		sprintf( risFileName, "%s/%d.ris", RIS_FILE_DIRECTORY, rotorId );
	}

	fp = fopen( risFileName, "rb" );
	error = GetLastError();
	if ( fp == NULL )
	{
		LogError( "error opening RIS file %s", risFileName );
		if ( error == ERROR_FILE_NOT_FOUND )
		{
			return SERR_RIS_NOT_THERE;
		}
		else
		{
			return SERR_FILE_READ;
		}
	}
	
	fsize = fread( fileBuff, sizeof( char ), 3000, fp );
	fclose( fp );

	if ( fsize > 0 )
	{
		msg.msgId = MSG_ENGINE_COMM_SEND_MESSAGE;
		msg.data.engineMsg.message[0] = 'P';
		msg.data.engineMsg.message[1] = ESC;
		msg.data.engineMsg.message[2] = fsize & 0xFF;	// LSB of binary count.
		msg.data.engineMsg.message[3] = fsize >> 8;		// MSB of binary count.
		memcpy( msg.data.engineMsg.message + 4, fileBuff, fsize );
		msg.data.engineMsg.length = fsize + 4;
		
		SendEngineCommMessage( &msg, sizeof( EngineCommReceiveMsg_t ) );
	}
	else
	{
		LogError( "error reading RIS file %s", risFileName );
		return SERR_FILE_READ;
	}

	// Wait to do this until here - it modifies the source string.
	char *		fileName;

	fileName = strrchr(risFileName, '/');
	if (fileName != NULL) {
		fileName++;
		strncpy( runInfo.risFileName, fileName, sizeof( runInfo.risFileName ) );
	} else {
		strncpy( runInfo.risFileName, risFileName, sizeof( runInfo.risFileName ) );
	}

	return SERR_NONE;
}

/*
 * FUNCTION
 * 	StateAnalysis::CalculateAge
 * 
 * DESCRIPTION
 * 	Calculates the patient's age based on either the age entered or the DOB entered. If the latter, then
 * 	the age is based on today's date.
 * 
 * PARAMETERS
 *	runInfo - Global structure containing age and DOB fields.
 * 
 * RETURN
 * 	Age in years, rounded down.
 */

int
StateAnalysis::
CalculateAge()
{
	int			age = NO_AGE_ENTERED;
	SYSTEMTIME		currentTime;
	
	// If an age was entered, we use it. The maximum value that can be entered is 99,
	// so we only care if the units are years, months and weeks.
	if ( runInfo.age > 0 )
	{
		switch ( runInfo.ageUnits )
		{
			case AGE_UNITS_YEARS:	age = runInfo.age;		break;
			case AGE_UNITS_MONTHS:	age = runInfo.age / 12;	break;
			case AGE_UNITS_WEEKS:	age = runInfo.age / 52;	break;
			default:				age = 0;				break;
		}
	}
	
	// Otherwise, if a DOB was entered, we use it.
	else if ( runInfo.dateOfBirth.year > 0 )
	{	
		// Get the current date.
		GetLocalTime(&currentTime);
	
		// Set the age equal to the difference between the current year and the birth year.
		age = currentTime.wYear - runInfo.dateOfBirth.year;

		// If it's currently an earlier month than the birth month, or an earlier day in the same month, then decrement the age.
		if ( (currentTime.wMonth < runInfo.dateOfBirth.month) ||
			((currentTime.wMonth == runInfo.dateOfBirth.month) && (currentTime.wDay < runInfo.dateOfBirth.day)) )
		{
			age--;
		}
	}

	return age;	
}

SystemErrNum_t
StateAnalysis::
ReadUploadedBarcodeFile()
{
    FILE *  fp;

    fp = fopen( UPLOADED_ROTOR_BARCODE_NAME, "r" );
    if ( fp == 0 )
    {
        return SERR_BC_BAD_DECODE;
    }
    
    if ( fread( barcodeData, 1, sizeof( BarcodeData_t ), fp ) != sizeof( BarcodeData_t ) )
    {
        return SERR_BC_BAD_DECODE;
    }

    return SERR_NONE;
}
void
StateAnalysis::
Rotor1GetCount()
{
    unsigned    i;

    // Check the ID for any non-numeric characters. If there are any, then run the rotor
    // like a regular rotor.
    for ( i = 0; i < strlen( runInfo.patientControlId ); i++ )
    {
        if ( !isdigit( runInfo.patientControlId[ i ] ) )
        {
            // ZZZ should reset rotor counter to be -1 here ???
            isRotor1 = false;
            break;
        }
    }

    // No non-numeric characters found, so set the counter based on
    // the run count entered. If it's zero, run it once like a
    // regular rotor.
    if ( isRotor1 )
    {
        rotor1Counter = strtoul( runInfo.patientControlId, 0, 10);
        if ( rotor1Counter == 0 )
        {
            isRotor1 = false;
            rotor1Counter = -1;
        }
    }
}

/*
 * FUNCTION
 *  OrderQueue::ProcessAstmOrder
 *
 * DESCRIPTION
 *  Handles an order message received from ASTM. Either enqueues a new order or (attempts to)
 *  cancel a previously-received order.
 *
 * PARAMETERS
 *  order - Structure containing the order information.
 */

void
OrderQueue::
ProcessAstmOrder( UploadedOrder_t  * order )
{
    OrderId_t   orderId;

    switch ( order->actionCode )
    {
        case ACTION_CODE_N:     // New patient order
        case ACTION_CODE_Q:     // New QC (control) order
            Enqueue( order );
            break;

        case ACTION_CODE_C:     // Cancel order
            if ( order->orderDateTime[0] != 0 )
            {
                orderId = FindOrder( order );

                if ( orderId == 0 ) // Order not found (possibly already run).
                {
                    SendAstmCancelMessage( order, 'Y' );
                }
                else                // Order found.
                {
                    if ( orderId == uploadedOrderInProcess )   // Order is currently being run.
                    {
                        SendAstmCancelMessage( order, 'I' );
                    }
                    else
                    {
                        Dequeue( orderId );
                    }
                }
            }
            else
            {
                SendAstmCancelMessage( order, 'X' );
            }
            break;

        default:
            break;
    }
}

/*
 * FUNCTION
 *  OrderQueue::Enqueue
 *
 * DESCRIPTION
 *  Enqueues an order received via ASTM. Sends a message to ASTM if there is no room in the queue
 *  and the order must be cancelled.
 *
 * PARAMETERS
 *  order - Structure containing the order information.
 */

void
OrderQueue::
Enqueue( UploadedOrder_t * order )
{
    int                 i, j;
    int                 insertIndex = -1;
    int                 insertData = -1;
    SYSTEMTIME          time;
    FILETIME            fTime;
    unsigned long long  longTime; 

    // get local time
    GetLocalTime(&time);
    // convert to file time
    SystemTimeToFileTime(&time, &fTime);
    // get totall 100 nanoseconds
    longTime = fTime.dwHighDateTime;
    longTime = (longTime << 32) | fTime.dwLowDateTime;
    // convert 100 nanosecond to second
    longTime /= 10000000UL;
    // orderId only has 32 bit, mask low 32 bits
    order->orderId = (unsigned long)(longTime & 0xFFFFFFFF);
    
    
    //LogDebug ("lowTime =%u higTime =%u", fTime.dwLowDateTime,  fTime.dwHighDateTime);
    
    if (  ordersList->numOrders < MAX_QUEUED_ORDERS )
    {
        // Find out where in the list our new order should go.
        for ( i = 0; i < ordersList->numOrders; i++ )
        {
            // Find the first order where the patient ID is greater than the new one.
            if ( strncmp( order->orderLabel, ordersList->orders[ ordersList->index[ i ] ].orderLabel, ID_LEN ) < 0 )
            {

                insertIndex = i;

                break;
            }
        }

        // If we didn't find an entry that's less than the new one, or if the list was empty,
        // insert the new entry at the end of the list.
        if (insertIndex == -1 )
        {
            insertIndex = ordersList->numOrders;
        }
        
        // Now, find a spot in the data list where we can store the new data. This is just
        // the first available slot.
        for ( i = 0; i < MAX_QUEUED_ORDERS; i++ )
        {
            // Scan the index list for the entry number.
            for ( j = 0; j < ordersList->numOrders; j++ )
            {
                // Entry number found, so go on to the next one.
                if ( ordersList->index[ j ] == i )
                {
                    break;
                }
            }

            // We didn't find this data index in the index list, so we've found a slot.
            if ( j == ordersList->numOrders )
            {
                insertData = i;
                break;
            }
        }
         // Error if we didn't find an open slot. This should never happen.
        if ( insertData == -1 )
        {
            LogError( "couldn't find slot to store new order" );
        }

        // If necessary, adjust the index array to make room to insert our new entry.
        if ( insertIndex < ordersList->numOrders )
        {
            for ( j = ordersList->numOrders; j >= insertIndex; j-- )
            {
                ordersList->index[ j+1 ] = ordersList->index[ j ];
            }
        }

        // Insert the new order.
        ordersList->index[ insertIndex ] = insertData;
        memcpy( &ordersList->orders[ insertData ], order, sizeof( UploadedOrder_t ) );

        ordersList->numOrders++;

        WriteOrdersFile( ordersList );

    }
    else
    {
        SendAstmCancelMessage( order, 'X' );
    }

}

/*
 * FUNCTION
 *  OrderQueue::Dequeue
 *
 * DESCRIPTION
 *  Removes the specified order from the queue.
 *
 * PARAMETERS
 *  orderId - The ID for the order we're deleting.
 */

void
OrderQueue::
Dequeue( OrderId_t orderId )
{
    int i;


    for ( i = 0; i < ordersList->numOrders; i++ )
    {
        // Find the order we're looking for.
        if ( orderId == ordersList->orders[ ordersList->index[ i ] ].orderId )
        {
            ordersList->numOrders--;

            // Move all the entries after it up one, which overwrites the one we're deleting.
            for ( ; i <= ordersList->numOrders; i++ )
            {
                ordersList->index[ i ] = ordersList->index[ i+1 ];
            }
             // Update the disk files.
            WriteOrdersFile( ordersList );

            break;
        }
    }
}

/*
 * FUNCTION
 *  OrderQueue::FindOrder
 *
 * DESCRIPTION
 *  Finds an order in the queue, based on matching of particular order values.
 *
 * PARAMETERS
 *  order - pointer to data struct holding data to match
 *
 * RETURN
 *  Order ID for the matching order in the queue, or zero if not found.
 */

OrderId_t
OrderQueue::
FindOrder( UploadedOrder_t * match )
{
    int i;

    for ( i = 0; i < ordersList->numOrders; i++ )
    {
        if ( strcmp( ordersList->orders[ ordersList->index[ i ] ].patientId, match->patientId ) == 0 )
        {
            if ( ordersList->orders[ ordersList->index[ i ] ].rotorId == match->rotorId )
            {
                if ( strcmp( ordersList->orders[ ordersList->index[ i ] ].orderDateTime, match->orderDateTime ) == 0 )
                {
                    return ordersList->orders[ ordersList->index[ i ] ].orderId;
                }
            }
        }
    }


    return 0;   // Order not found.
}

/*
 * FUNCTION
 *  OrderQueue::Clear
 *
 * DESCRIPTION
 *  Clears all entries from the order queue.
 */

void
OrderQueue::
Clear()
{
    memset( ordersList, 0, sizeof( OrdersList_t ) );
    memset( ordersList->index, -1, sizeof( ordersList->index ) );
    DeleteOrdersFile();

}


/*
 * FUNCTION
 *  OrderQueue::GetDisplayList
 *
 * DESCRIPTION
 *  Returns one page of orders to display - 5 orders maximum.
 *
 * PARAMETERS
 *  page - The page of orders to return (5 orders per page, with no overlap).
 *  list - Pointer to an array that can hold the data for 5 orders.
 */

void
OrderQueue::
GetDisplayList( int page, OrderDisplayList_t * list )
{
    int i, j;
   
    // Clear the array, so any we don't populate will be empty.
    memset( list, 0, 5 * sizeof( OrderDisplayList_t ) );
    // backup order list for overcoming orderList crash
    memset( orderListBak, 0, 5 * sizeof( OrderDisplayList_t ) );
   
    for ( i = page * 5, j = 0; i < ordersList->numOrders && i < (page * 5 + 5); i++, j++ )
    {
        list[ j ].orderId = ordersList->orders[ ordersList->index[ i ] ].orderId;
        orderListBak [j].orderId =  ordersList->orders[ ordersList->index[ i ] ].orderId;

        strncpy( list[ j ].orderLabel,  ordersList->orders[ ordersList->index[ i ] ].orderLabel, 20 );
        strncpy( orderListBak [ j ].orderLabel,  ordersList->orders[ ordersList->index[ i ] ].orderLabel, 20 );

        list[ j ].rotorNameFontColor =  ordersList->orders[ ordersList->index[ i ] ].rotorNameFontColor;;
        orderListBak [j].rotorNameFontColor =  ordersList->orders[ ordersList->index[ i ] ].rotorNameFontColor;;

        list[ j ].rotorNameBackgroundColor =  ordersList->orders[ ordersList->index[ i ] ].rotorNameBackgroundColor;
        orderListBak [ j ].rotorNameBackgroundColor =  ordersList->orders[ ordersList->index[ i ] ].rotorNameBackgroundColor;

    }
#if 0
    // ZZZ debug
    for ( i = page * 5, j = 0; i < numOrders && i < (page * 5 + 5); i++, j++ ) {
        if (list[ j ].orderId == 0) {
            LogDebug ("GetDisplay:numOrders =%d", numOrders);
            LogDebug ("list.orderId=%lu orders.orderId=%lu orderListBak.orderId=%u", list[ j ].orderId, orders[ i ].orderId, orderListBak[j].orderId);
        }
    }
#endif
}

UploadedOrder_t *
OrderQueue::
GetEntryInfo( OrderId_t orderId )
{
    int i;

    for ( i = 0; i < ordersList->numOrders; i++ )
    {
        if ( orderId == ordersList->orders[ ordersList->index[ i ] ].orderId )
        {
            return &ordersList->orders[ ordersList->index[ i ] ];
        }
    }

    // Error - entry not found. This could only happen if the order was canceled
    // externally.
    LogDebug( "ASTM order not found" );

    return 0;
}

bool
OrderQueue::
OrderWasDeleted( OrderId_t orderId)
{
    int i;

    for ( i = 0; i < ordersList->numOrders; i++ )
    {
        if ( orderId == ordersList->orders[ ordersList->index[ i ] ].orderId )
        {
            return false;
        }
    }

    return true;
}

void
OrderQueue::
SendAstmCancelMessage( UploadedOrder_t * order, char code )
{
    static HANDLE           queue = NULL;
    AstmMsg_t               msg;
    MSGQUEUEOPTIONS         options;

     if (queue == NULL) {
        options.dwSize          = sizeof(MSGQUEUEOPTIONS);
        options.dwFlags         = MSGQUEUE_ALLOW_BROKEN;
        options.bReadAccess     = FALSE;
        options.dwMaxMessages   = 0;
        options.cbMaxMessage    = MAX_QUEUE_LENGTH;

        queue = CreateMsgQueue (ASTM_QUEUE_NAME, &options);

        if (queue == NULL)
        {
            LogError( "failed to open queue %s", ASTM_QUEUE_NAME);
            return ;
        }
    }
    if ( queue != NULL )
    {
        msg.msgId = MSG_ASTM_CANCEL_ORDER;
        msg.cancelData.rotorId = order->rotorId;
        strcpy( msg.cancelData.patientControlId, order->patientId );
        strcpy( msg.cancelData.orderDateTime, order->orderDateTime );
        msg.cancelData.cancelType = code;
        WriteMsgQueue(queue, (void *)&msg, sizeof(AstmMsg_t ), INFINITE, NULL);
    }
}

void
OrderQueue::
SendAstmCancelMessage( OrderId_t orderId, char code )
{

    SendAstmCancelMessage( GetEntryInfo( orderId ), code );
}
