// ********************************************************************
//
// FILENAME:  
// 
// 		Algorithms.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Top level Console process main() that implements rotor algorithm processing 
// 		per the ROC file and using raw data from the Engine	for the current rotor
//		(for DAC trims,	offsets, system bead checks, system distribution checks & analyte calculations) 
//		with output of rotor results & flags to a rotor results data record
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//

#ifdef UNDER_CE
#include <windows.h>
#include <msgqueue.h>
#include <stdio.h>
#include "CommonDefines.h"
#include "AnalysisMessages.h"
#include "CalcResultsMessages.h"
#include "LogErr.h"
#include "Shmem.h"
#else
#include "ParseRoc.h"
#endif

#include <string.h>
#include "AlgGlobals.h"
#include "SharedFactory.h"
#include "SharedCal.h"
#include "SharedBarcode.h"
#include "SharedRoc.h"
#include "SharedRaw.h"

#include "DacTrims.h"
#include "Offsets.h"
#include "BeadCheck1.h"
#include "BeadCheck2.h"
#include "DistributionCheck.h"
#include "TestCuvetteAbs.h"
#include "RotorResults.h"
#include "MfMessages.h"


#include "ResultsData.h"


CommonAlgGlobals_t		commonAlgGlobals;
CommonAlgGlobals_t		*commonAlgGlobals_g = &commonAlgGlobals;


#ifdef UNDER_CE 
RotorResults_t *	rotorResults_g = 0;

#else
static RotorResults_t	rotorResults;
RotorResults_t *		rotorResults_g = &rotorResults;

// Message definitions

#define	NO_THREAD					0
#define	DAC_TRIMS_THREAD			1
#define	OFFSETS_THREAD				2
#define	BEAD1_THREAD				3
#define	BEAD2_THREAD				4
#define	DISTRIBUTION_THREAD			5	
#define	ANALYTE_RESULTS_THREAD		6
#define	LAST_ALG_THREAD				ANALYTE_RESULTS_THREAD
#define	BAD_THREAD					0
#define	ABORT_THREAD_PROCESSING		0xffff
#endif


#ifndef UNDER_CE
// ******************************************************************************************

	// ADD code here to fix error in current RAS file for rotor 52 format 1
	// 10th flash for pass 1 optical blank sampleType = 0 is missing in RAS file
	// following code populates the 10th flash
	// REMOVE this code when RAS file is corrected
	//
	// for sampleType 0; pass number 1-1 = 0; flash number 10
	//
void PatchSharedRaw()
{	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_340]   = 0xa93c;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_405]   = 0xa9ed;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_467]   = 0xaa3f;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_500]   = 0x8187;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_515]   = 0xa9e7;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_550]   = 0xad56;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_600]   = 0xaa48;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_630]   = 0xad37;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_850]   = 0xad98;	
	rotorRawReadings_g->rotorAnalyteSamples[0][1-1][10].rawFlashReadings[WL_WHITE] = 0xa696;	
}

#endif


// ********************************************************************
// FUNCTION:
//
//		InitCommonAlgGlobals() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Init commonAlgGlobals structure for rotor calcs/testing 
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

void InitCommonAlgGlobals()
{
	// clear common algorithm globals prior to each rotor run

	memset(commonAlgGlobals_g, 0, sizeof( CommonAlgGlobals_t ));


	// initialize for unspecified patient type, age, gender & race for test

	commonAlgGlobals_g->patientType = PATIENT_TYPE_UNSPECIFIED;

//	commonAlgGlobals_g->age = 18;
	commonAlgGlobals_g->age = 40;
//	commonAlgGlobals_g->age = NO_AGE_ENTERED;

	commonAlgGlobals_g->gender = GENDER_FEMALE;
//	commonAlgGlobals_g->gender = GENDER_MALE;
//	commonAlgGlobals_g->gender = GENDER_UNKNOWN;

	commonAlgGlobals_g->race = RACE_BLACK;
//	commonAlgGlobals_g->race = RACE_WHITE;
//	commonAlgGlobals_g->race = RACE_UNKNOWN;


	// initialize for standard calculation error mask (not TRIG)

	commonAlgGlobals_g->calcErrorMask = CALC_ERROR_MASK;
}


// ********************************************************************
// FUNCTION:
//
//		InitRotorResults() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Init rotor results structure for test 
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

void InitRotorResults()
{
	unsigned	i;

	// clear all results prior to each rotor run

	memset(rotorResults_g, 0, sizeof( RotorResults_t ));

// DLR - modify below to always set from barcodeData_g, Greg to read in from RAD file

	if ( ( barcodeData_g->rotorId > 1 ) && ( barcodeData_g->rotorId < 100 ) ) 
	{
		rotorResults_g->rotorNumber = rotorConfig_g->rotorNumber;
		rotorResults_g->rotorFormat = rotorConfig_g->rotorFormat;
		rotorResults_g->rotorProductCode = rotorConfig_g->rotorProductCode;
	}
	else
	{
		rotorResults_g->rotorNumber = barcodeData_g->rotorId;
		rotorResults_g->rotorFormat = barcodeData_g->rotorFormat;
		rotorResults_g->rotorProductCode = '3';
	}

	// init for min/max scanning of ADC reading values during rotor run

	rotorResults_g->rotorInformationResults.rotorFlashLampIntensityMin  = LAMP_SATURATION_THRESHOLD;
	rotorResults_g->rotorInformationResults.rotorFlashLampSaturationMax = MINIMUM_LAMP_INTENSITY;

// DLR
// Initialize temperature here in case of errors such that temperature calc would be bypassed

	rotorResults_g->rotorInformationResults.rotorAverageTemperature = 37.0;


// DLR
// initialize HEM, LIP, ICT, RQC values to ensure no result = 0 reported due to rotor error
// GLD - comment this out if we take out the issue #71 change yet again.

	rotorResults_g->rotorInformationResults.rotorHemolyzedIndex = NOVAL_NEG_FLOAT;
	rotorResults_g->rotorInformationResults.rotorIctericIndex   = NOVAL_NEG_FLOAT;
	rotorResults_g->rotorInformationResults.rotorLipemicIndex   = NOVAL_NEG_FLOAT;

	rotorResults_g->rotorAnalyteResult[RQC].analyteResult    = NOVAL_NEG_FLOAT;
	rotorResults_g->rotorInformationResults.rotorRqcLowLimit = NOVAL_NEG_FLOAT;


	// initialize all to no analyteType and no printOrder prior to rotor run

	for (i = 0; i < MAX_ANALYTE_TYPES; i++)
	{
		rotorResults_g->rotorAnalyteResult[i].analyteType = 0xff;
		rotorResults_g->rotorAnalyteResult[i].printOrder = 0xff;
	}


	// init rotor cuvette empty, bead & distribution check limits

	for ( i = 0; i < NUM_CUVETTES; i++ )
	{
		rotorResults_g->rotorCuvetteEmptyLimits[i] 		  = -0.7;
		rotorResults_g->rotorCuvetteBeadLimits[i] 		  = 1.50;
		rotorResults_g->rotorCuvetteDistributionLimits[i] = 0.0;
	}

	// hard code distribution limits for cuvettes 0,1,2 to 0.30 for all rotors

	rotorResults_g->rotorCuvetteDistributionLimits[0] = 0.30;
	rotorResults_g->rotorCuvetteDistributionLimits[1] = 0.30;
	rotorResults_g->rotorCuvetteDistributionLimits[2] = 0.30;
}


// ********************************************************************
// FUNCTION:
//
//		SetVetSpeciesSystemFlags() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set vet species system flags to set up for results calculation 
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

void SetVetSpeciesSystemFlags( unsigned patientType )
{
	const unsigned	vetPatientTypeSystemFlags[] = 
	{
		USED_DOG_FACTORS  ,	// DOG_FEMALE_PATIENT_TYPE
		USED_CAT_FACTORS  ,	// CAT_MALE_PATIENT_TYPE
		USED_HORSE_FACTORS,	// HORSE_PATIENT_TYPE
		USED_OTHER_FACTORS,	// PATIENT_TYPE_UNSPECIFIED
	};	

	if (( rotorResults_g->rotorProductCode == VETSCAN_PRODUCT_CODE) && ( patientType <= PATIENT_TYPE_UNSPECIFIED ))
	{
		rotorResults_g->rotorInformationResults.rotorSystemFlags |= vetPatientTypeSystemFlags[ patientType ];
	}
}


// ********************************************************************
// FUNCTION:
//
//		SetOffsets2AdcBias() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Set offsets to adcBias reading for rotor 1 & rotor 100 testing 
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

void SetOffsets2AdcBias( void )
{
	unsigned int	i;

	for ( i = 0; i <= WL_WHITE; i++ )
	{ 
		rotorResults_g->rotorInformationResults.wavelengthAdcOffsets[i] = rotorRawReadings_g->adcBias.adcReferenceOffset;
	}
}


// ********************************************************************
// FUNCTION:
//
//		WaitForMessage() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Debug used to simulate wait for QNX message in windows test environment 
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

unsigned WaitForMessage(unsigned message)
{
	return	(message + 1);
}


// ********************************************************************
// FUNCTION:
//
//		main() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Console Algorithms process top level main function 
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
#ifdef UNDER_CE
    unsigned short      errorNumber;
    DWORD               receivedSize = 0;
    CalcResultsMsg_t    receivedMsg;
    AnalysisMsg_t       analysisMsg;
    ManufactureMsg_t    mfpMsg;
    HANDLE              inputQueue;
    HANDLE              analysisQueue;
    HANDLE              mfpQueue;
    MSGQUEUEOPTIONS     inputOptions;
    MSGQUEUEOPTIONS     analysisOptions;
    MSGQUEUEOPTIONS     mfpOptions;
    DWORD               flags;
    RawData_t *         rawData;
    BOOL                result;

	
	/*
	 * Open the input queue and get its size.
	 */
    inputOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
    inputOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    inputOptions.bReadAccess = TRUE;
    inputOptions.dwMaxMessages = 0;
    inputOptions.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(CalcResultsMsg_t)*/;

    inputQueue = CreateMsgQueue(CALC_RESULTS_QUEUE_NAME, &inputOptions);

    if ( inputQueue == NULL )
    {
        LogError( "failure to open input queue" );
        return 0;
    }


	/*
	 * Open the Analysis queue.
	 */
    analysisOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
    analysisOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    analysisOptions.bReadAccess = FALSE;
    analysisOptions.dwMaxMessages = 0;
    analysisOptions.cbMaxMessage = MAX_QUEUE_LENGTH/*sizeof(AnalysisMsg_t)*/;

    analysisQueue = CreateMsgQueue(ANALYSIS_QUEUE_NAME, &analysisOptions);

    if ( analysisQueue == NULL )
    {
        LogError( "failure to open Analysis queue" );
        return 0;
    }

	/*
	 * Open the manufacturing queue.
	 */
    mfpOptions.dwSize = sizeof(MSGQUEUEOPTIONS);
    mfpOptions.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    mfpOptions.bReadAccess = FALSE;
    mfpOptions.dwMaxMessages = 0;
    mfpOptions.cbMaxMessage = MAX_QUEUE_LENGTH;

    mfpQueue = CreateMsgQueue(MANUFACTURE_QUEUE_NAME, &mfpOptions);

    if (mfpQueue == NULL )
    {
        LogError( "failure to open mfp queue" );
        return 0;
    }



	// set up shared data structures for rotor analyte algorithm testing 
	factoryData_g = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
	calibrationData_g = (CalibrationData_t *)MapSharedMem( CAL_DATA_SHMEM_NAME, sizeof( CalibrationData_t ) );
	barcodeData_g = (BarcodeData_t *)MapSharedMem( BARCODE_SHMEM_NAME, sizeof( BarcodeData_t ) );

	rotorConfig_g = (RotorConfig_t *)MapSharedMem( ROC_SHMEM_NAME, sizeof( RotorConfig_t ) );
	
	rawData = (RawData_t *)MapSharedMem( RAW_SHMEM_NAME, sizeof( RawData_t ) );
	rotorRawReadings_g = &rawData->rotorRawReadings;
	rotorResults_g = (RotorResults_t *)MapSharedMem( RESULTS_SHMEM_NAME, sizeof( RotorResults_t ) );

	while ( 1 )
	{

        result = ReadMsgQueue(inputQueue, &receivedMsg, sizeof(CalcResultsMsg_t), &receivedSize, INFINITE, &flags);

        if ( result )
		{
			if ( receivedSize > 0 )
			{
				switch (receivedMsg.msgId )
				{
					case MSG_CALC_RESULTS_INIT_CALCULATION:
						InitCommonAlgGlobals();
						InitRotorResults();
						errorNumber = 0;
						break;
					case MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE:
						errorNumber = SetWavelengthDacTrims();
						analysisMsg.msgId = MSG_ANALYSIS_DAC_TRIMS_COMPLETE;
                        WriteMsgQueue(analysisQueue, &analysisMsg, sizeof( int ), INFINITE, NULL);
						break;
                    case MSG_CALC_RESULTS_DAC_TRIMS_COMPLETE_MFP:
                        // send trim DAC value to manufacturing process
                        errorNumber = SetWavelengthDacTrims();
						mfpMsg.msgId = MSG_MANUFACTURE_DAC_TRIMS_COMPLETE;
                        WriteMsgQueue(mfpQueue, &mfpMsg, sizeof( int ), INFINITE, NULL);
                        break;
					case MSG_CALC_RESULTS_BLACK_OFFSETS_COMPLETE:
						errorNumber = SetWavelengthOffsets();
						break;
					case MSG_CALC_RESULTS_BEAD_CHECK_1_COMPLETE:
						errorNumber = SystemBeadChecks1();
						break;
					case MSG_CALC_RESULTS_BEAD_CHECK_2_COMPLETE:
						errorNumber = SystemBeadChecks2();
						break;
					case MSG_CALC_RESULTS_DISTRIBUTION_CHECK_COMPLETE:
						errorNumber = SystemDistributionChecks();
						break;
					case MSG_CALC_RESULTS_ANALYSIS_DONE:
						if ( ( barcodeData_g->rotorId <= 1 ) || ( barcodeData_g->rotorId >= 100 ) ) 
						{
							switch ( barcodeData_g->rotorId )
							{
								case 100:
									SetOffsets2AdcBias();		// for rotor 100 only
								case 1:
								default:
									errorNumber = CalcTestCuvetteAbs();		// for rotor 1 / default only
									break;
							}
						}
						else
						{
							commonAlgGlobals_g->patientType = receivedMsg.data.patientType;
							commonAlgGlobals_g->age = receivedMsg.data.age;
							commonAlgGlobals_g->race = receivedMsg.data.race;
							commonAlgGlobals_g->gender = receivedMsg.data.gender;


							// check if control and set for controlType and patientType as unspecified if so

							if ( commonAlgGlobals_g->patientType == PATIENT_TYPE_CONTROL )
							{
								commonAlgGlobals_g->controlType = TRUE;
								commonAlgGlobals_g->patientType = PATIENT_TYPE_UNSPECIFIED;
							}
							else
							{
								commonAlgGlobals_g->controlType = FALSE;
							}

							// Set species flag based on patient type for Vet rotors only

							SetVetSpeciesSystemFlags( commonAlgGlobals_g->patientType );

							// Calc results for all analytes on rotor

							errorNumber = CalcRotorAnalyteResults();
						}

						// Inform Analysis that the results are complete.  Include
						// the error number.
						analysisMsg.msgId = MSG_ANALYSIS_RESULTS_CALC_COMPLETE;
						analysisMsg.idata = errorNumber;
                        WriteMsgQueue(analysisQueue, &analysisMsg, sizeof( AnalysisMsg_t ), INFINITE, NULL);
						
						// Clear the error number, since we've already informed Analysis.
						errorNumber = 0;
						break;
					default:
						errorNumber = 0;
						break;
				}

				// Inform Analysis if there's an error.
				if ( errorNumber != 0 ) 
				{
					analysisMsg.msgId = MSG_ANALYSIS_ERROR;
					analysisMsg.idata = errorNumber;
                    WriteMsgQueue(analysisQueue, &analysisMsg, 2 * sizeof( int ), INFINITE, NULL);
					
					errorNumber = 0;
				}
			}
		}
		else
		{
			LogError( "message receive error" );
			break;
		}
	}
	
	return 0;


	// Windows Visual Studio
#else
	unsigned short	errorNumber;
	unsigned		message;
	CfgError_t		rocRet;

	
	if ( argc < 2 )
	{
		printf( "Need RAD file name\n" );
		return -1;
	}

		if ( argc < 3 )
	{
		printf( "Need ROC file name\n" );
		return -1;
	}

	// set up shared data structures for rotor analyte algorithm testing 

	LoadTestFactory();
	LoadTestCal();
	LoadTestBarcode();

// DLR - no longer required if read ROC XML file
//	LoadTestRoc();

	rocRet = ParseRoc( argv[2], rotorConfig_g );

	if (rocRet  != CFG_ERR_NONE)
	{
		printf("Roc File Error Number: %c\n\n",	rocRet);
		return -1;
	}


// DLR- init commonAlgGlobals variables prior to LoadTestRaw for proper initialization of CommonAlgGlobals data when load from RAD version 7
	InitCommonAlgGlobals();


	LoadTestRaw( argv[1] );

// DLR - patching no longer required for good RAD files
//	PatchSharedRaw();	// GLD - I needed to split this part out


	InitRotorResults();


	printf("Rotor: %d Format: %d Code: %c\n\n",
				rotorConfig_g->rotorNumber,
				rotorConfig_g->rotorFormat,
				rotorConfig_g->rotorProductCode);



	// wait for message to invoke top level algorithm threads to populate a rotor results record 

	message = NO_THREAD;

	while (message <= LAST_ALG_THREAD)
	{
		message = WaitForMessage(message);

		switch (message)
		{
			case DAC_TRIMS_THREAD:
				errorNumber = SetWavelengthDacTrims();
				break;
			case OFFSETS_THREAD:
				errorNumber = SetWavelengthOffsets();
				break;
			case BEAD1_THREAD:
				errorNumber = SystemBeadChecks1();
				break;
			case BEAD2_THREAD:
				errorNumber = SystemBeadChecks2();
				break;
			case DISTRIBUTION_THREAD:
				errorNumber = SystemDistributionChecks();
				break;
			case ANALYTE_RESULTS_THREAD:
				if ( ( barcodeData_g->rotorId <= 1 ) || ( barcodeData_g->rotorId >= 100 ) ) 
				{
					switch ( barcodeData_g->rotorId )
					{
						case 100:
							SetOffsets2AdcBias();		// for rotor 100 only
						case 1:
						default:
							errorNumber = CalcTestCuvetteAbs();		// for rotor 1 / default only
							break;
					}
				}
				else
				{
					// check if control and set for controlType and patientType as unspecified if so

					if ( commonAlgGlobals_g->patientType == PATIENT_TYPE_CONTROL )
					{
						commonAlgGlobals_g->controlType = TRUE;
						commonAlgGlobals_g->patientType = PATIENT_TYPE_UNSPECIFIED;
					}
					else
					{
						commonAlgGlobals_g->controlType = FALSE;
					}

					// Set species flag based on patient type for Vet rotors only

					SetVetSpeciesSystemFlags( commonAlgGlobals_g->patientType );

					// Calc results for all analytes on rotor

					errorNumber = CalcRotorAnalyteResults();
				}
				break;
			default:
				errorNumber = BAD_THREAD;
				break;
		}

		// print errorNumber if abort type error found during calculations

		if (errorNumber)
		{
//			message = ABORT_THREAD_PROCESSING;
			printf("Rotor Error Number: %x\n\n", errorNumber);
		}
	}

		
	// signal algorithm processing done to monitor

	printf("\nRotor Algorithm Processing Completed\n\n");

	return 0;
#endif	
}


// SQUEEZER II - only

// ********************************************************************
// FUNCTION:
//
//		LoadFromSqueezer() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Load ROC and RAD data for Squeezer II processing and set up as DLL
//		(DLL for Windows entry) 
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

#ifndef UNDER_CE
int LoadFromSqueezer(char * radfilename, char * rocfilename)
{
	unsigned short    errorNumber;
	unsigned          message;
	CfgError_t        rocRet;

	if ( radfilename  == "" )
	{
		return -1;
	}

	if ( rocfilename  == "" )
	{
		return -1;
	}

	// set up shared data structures for rotor analyte algorithm testing 

	LoadTestFactory();
	LoadTestCal();
	LoadTestBarcode();

	rocRet = ParseRoc(rocfilename, rotorConfig_g );

	if (rocRet  != CFG_ERR_NONE)
	{
		return -1;
	}


// DLR- init commonAlgGlobals variables prior to LoadTestRaw for proper initialization of CommonAlgGlobals data when load from RAD version 7
	InitCommonAlgGlobals();


	LoadTestRaw( radfilename );


	InitRotorResults();


	// wait for message to invoke top level algorithm threads to populate a rotor results record 

	message = NO_THREAD;

	while (message <= LAST_ALG_THREAD)
	{
		message = WaitForMessage(message);

		switch (message)
		{
			case DAC_TRIMS_THREAD:
				errorNumber = SetWavelengthDacTrims();
				break;
			case OFFSETS_THREAD:
				errorNumber = SetWavelengthOffsets();
				break;
			case BEAD1_THREAD:
				errorNumber = SystemBeadChecks1();
				break;
			case BEAD2_THREAD:
				errorNumber = SystemBeadChecks2();
				break;
			case DISTRIBUTION_THREAD:
				errorNumber = SystemDistributionChecks();
				break;
			case ANALYTE_RESULTS_THREAD:
				if ( ( barcodeData_g->rotorId <= 1 ) || ( barcodeData_g->rotorId >= 100 ) ) 
				{
					switch ( barcodeData_g->rotorId )
					{
						case 100:
							SetOffsets2AdcBias();		// for rotor 100 only
						case 1:
						default:
							errorNumber = CalcTestCuvetteAbs();		// for rotor 1 / default only
							break;
					}
				}
				else
				{
					// check if control and set for controlType and patientType as unspecified if so

					if ( commonAlgGlobals_g->patientType == PATIENT_TYPE_CONTROL )
					{
						commonAlgGlobals_g->controlType = TRUE;
						commonAlgGlobals_g->patientType = PATIENT_TYPE_UNSPECIFIED;
					}
					else
					{
						commonAlgGlobals_g->controlType = FALSE;
					}

					// Set species flag based on patient type for Vet rotors only

					SetVetSpeciesSystemFlags( commonAlgGlobals_g->patientType );

					// Calc results for all analytes on rotor

					errorNumber = CalcRotorAnalyteResults();
				}
				break;
			default:
				errorNumber = BAD_THREAD;
				break;
		}
	}

	return 0;
}
#endif


// ********************************************************************
// FUNCTION:
//
//		GetEmptyBeadDistributionLimits() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		get the imv from the rotorIntermediateValues table based on imv index  
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

float GetEmptyBeadDistributionLimits( unsigned limitType, unsigned cuvetteNumber )
{
	float	limit;

	if ( cuvetteNumber < NUM_CUVETTES )
	{
		switch ( limitType )
		{
			case BEAD1_READINGS:
				limit = rotorResults_g->rotorCuvetteEmptyLimits[ cuvetteNumber ];
				break;
			case BEAD2_READINGS:
				limit = rotorResults_g->rotorCuvetteBeadLimits[ cuvetteNumber ];
				break;
			case DISTRIBUTION_READINGS:
				limit = rotorResults_g->rotorCuvetteDistributionLimits[ cuvetteNumber ];
				break;
			default:
				limit = NOVAL_NEG_FLOAT;;
				break;
		} 
	}
	else
	{
		limit = NOVAL_NEG_FLOAT;
	}

	return ( limit );
}


// end SQUEEZER II

