// ********************************************************************
//
// FILENAME:  
// 
// 		Phos.c	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		PHOS calculation algorithm
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// ********************************************************************/
//


#include "CommonAlgFuncs.h"
#include "Statistics.h"


// PHOS average calc indices & barcode indices

#define PHOS_AVG1_IDX					0
#define PHOS_AVG2_IDX					1

#define PHOS_SLOPE_IDX					0
#define PHOS_INTERCEPT_IDX				1

// PHOS standard endpoint alg pass indices

#define	PHOS_PASS7_IDX					0
#define	PHOS_PASS13_IDX					6

// PHOS slope rate abs correction calc pass indices & pass 1 limit / constants

#define	PHOS_PASS2_IDX					0
#define	PHOS_PASS2_FRACTION_DIVISOR		20.0

#define PHOS_RATE_START_PASS			2
#define PHOS_RATE_END_PASS				8
#define	PHOS_RATE_NUM_POINTS		(PHOS_RATE_END_PASS - PHOS_RATE_START_PASS) + 1

// PHOS average abs correction calc pass indices & pass 1 limit / constants

#define PHOS_CORRECTED_AVG_START_PASS	6
#define PHOS_CORRECTED_AVG_END_PASS		13

#define	CORRECTED_AVG_CALC				0x00000040	// Rate in endpoint Sample, OTHER patient type, used average 
#define	CORRECTED_RATE_CALC				0x00000020	// Rate in endpoint sample, OTHER patient type, used starting rate 


// ********************************************************************
// FUNCTION:
//
//		PhosRateInEndpointCorrectionCheck() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		Process correction for PHOS blanked average
//		if rate in endpoint from standard endpoint alg 
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

unsigned PhosRateInEndpointCorrectionCheck(EndpointAlgRecord_t *endpointAlgPtr, AvgAbs_t *avgAbs, unsigned long *phosError)
{
	unsigned char		sampleTypeAnalyte;		
	unsigned char		sampleTypeAnalyteBlank;		
	unsigned char		sampleTypeOpticalBlank;
	unsigned char		analyteCuvetteNumber;
	unsigned char		analyteBlankCuvetteNumber;			
	float				desiredPathLength;

	float				phosDelta7To13;
	float				phosPass2AbsFractionLimit;
	float				phosSlopeRate;
	float				phosIntercept;

	// check for RATE_IN_ENDPOINT from standard endpoint averaging/checks to allow for any corrections/checks

	if (*phosError & RATE_IN_ENDPOINT)
	{
		if ((commonAlgGlobals_g->patientType == PATIENT_TYPE_UNSPECIFIED) && (rotorConfig_g->analyteCalcRecord[PHOS].algorithmFlags[ROTOR_ROC_ENDPOINT_RATE_CORRECT_FLAG])) 
		{
			// got standard RATE_IN_ENDPOINT, so clear flag to check for correction and later possible set of RATE_IN_ENDPOINT flag

			*phosError &= ~RATE_IN_ENDPOINT;

			// rate in endpoint correction for unspecified patient type only or rotor that can be corrected

			sampleTypeAnalyte         = endpointAlgPtr->sampleTypeAnalyte;	
			sampleTypeAnalyteBlank    = rotorConfig_g->analyteCalcRecord[PHOS].sampleTypeAnalyteBlank;	
			sampleTypeOpticalBlank    = rotorConfig_g->analyteCalcRecord[PHOS].sampleTypeOpticalBlank;	
			analyteCuvetteNumber      = rotorConfig_g->analyteCalcRecord[PHOS].analyteCuvetteNumber;	
			analyteBlankCuvetteNumber = rotorConfig_g->analyteCalcRecord[PHOS].analyteBlankCuvetteNumber;	
			desiredPathLength		  =	rotorConfig_g->analyteCalcRecord[PHOS].analyteDesiredPathLength;

			// calculate linear abs slope rate from abs data

			phosDelta7To13 = (avgAbs[PHOS_AVG1_IDX].blankedAbs[PHOS_PASS13_IDX] - avgAbs[PHOS_AVG1_IDX].blankedAbs[PHOS_PASS7_IDX]) / (avgAbs[PHOS_AVG1_IDX].flashTime[PHOS_PASS13_IDX] - avgAbs[PHOS_AVG1_IDX].flashTime[PHOS_PASS7_IDX]);

			// get abs & time points for number of passes at rate/noise wavelength (does not blank or use average, max or min)
	 
			if (CalcAverageAbsorbance(&avgAbs[PHOS_AVG2_IDX], sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, PHOS_RATE_START_PASS, PHOS_RATE_END_PASS, endpointAlgPtr->endpointAvgRecord.avgParams[PHOS_AVG1_IDX].avgWavelengthPair, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, endpointAlgPtr->endpointAvgRecord.avgBlankCorrectionOrder, phosError))
			{
				return	TRUE;						// abort algorithm processing flag
			}

			// create limit check value as pass 2 blanked abs

			phosPass2AbsFractionLimit = avgAbs[PHOS_AVG2_IDX].blankedAbs[PHOS_PASS2_IDX] / PHOS_PASS2_FRACTION_DIVISOR;


			// IMV - store PHOS algorithm specific parameters

			StoreIntermediateValue( PHOS, IMV_PHOS_DELTA_7_TO_13,      IMV_DIRECT_SET_METHOD, phosDelta7To13 );
			StoreIntermediateValue( PHOS, IMV_PHOS_ABS_FRACTION_LIMIT, IMV_DIRECT_SET_METHOD, phosPass2AbsFractionLimit );

			// end IMV


			// check if last 7 passes linear slope is above the pass 1 abs threshold (5% of pass 1 abs)

			if (phosDelta7To13 > phosPass2AbsFractionLimit)
			{
				// set for PHOS uses rate correction for rate in endpoint

				*phosError |= CORRECTED_RATE_CALC;

				// get PHOS slope & intercept from 7 passes from blanked abs for correction

				if (LinearRegression(avgAbs[PHOS_AVG2_IDX].flashTime, avgAbs[PHOS_AVG2_IDX].blankedAbs, PHOS_RATE_NUM_POINTS, &phosSlopeRate, &phosIntercept))
				{
					*phosError |= DIVIDE_BY_ZERO;
					return TRUE;
				}


				// IMV - store PHOS algorithm specific parameters

				StoreIntermediateValue( PHOS, IMV_PHOS_RATE,      IMV_DIRECT_SET_METHOD, phosSlopeRate );
				StoreIntermediateValue( PHOS, IMV_PHOS_INTERCEPT, IMV_DIRECT_SET_METHOD, phosIntercept );

				StoreIntermediateValue( PHOS, IMV_PHOS_PASS_START_IN_MINUTES, IMV_DIRECT_SET_METHOD, ConvertEngineTimeToMinutes(rotorRawReadings_g->blackOffsets[0].engineTime) );

				// end IMV


				// do rate correction only if rate is positive for average to use in result

				if (phosSlopeRate > 0.0)
				{
					// set PHOS blanked average for result calc using rate correction

					avgAbs[PHOS_AVG1_IDX].avg = (phosSlopeRate * ConvertEngineTimeToMinutes(rotorRawReadings_g->blackOffsets[0].engineTime)) + phosIntercept;
				}
				else
				{
					// for negative rate set for RATE_IN_ENDPOINT

					*phosError |= RATE_IN_ENDPOINT;
				}
			}
			else
			{
				// set for PHOS uses average correction for rate in endpoint

				*phosError |= CORRECTED_AVG_CALC;

				// do correction using last 8 passes (6 through 13) points for average to use in result 

				if (CalcAverageAbsorbance(&avgAbs[PHOS_AVG1_IDX], sampleTypeAnalyte, sampleTypeAnalyteBlank, sampleTypeOpticalBlank, PHOS_CORRECTED_AVG_START_PASS, PHOS_CORRECTED_AVG_END_PASS, endpointAlgPtr->endpointAvgRecord.avgParams[0].avgWavelengthPair, desiredPathLength, analyteCuvetteNumber, analyteBlankCuvetteNumber, endpointAlgPtr->endpointAvgRecord.avgBlankCorrectionOrder, phosError))
				{
					return	TRUE;						// abort algorithm processing flag
				}
			}
		}
	}

	return FALSE;
}


// ********************************************************************
// FUNCTION:
//
//		CalcPhos() 
// _____________________________________________________________________
// DESCRIPTION:
//
//		PHOS calculation algorithm 
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

void	CalcPhos(void)
{
	float				phosResult;
	unsigned long		phosError;

	unsigned char		analyteType;
	unsigned			checkAnalyteBlank;

	EndpointAlgRecord_t		*endpointAlgPtr;
	CommonFactorsLimits_t	*factorsLimitsPtr;

	AvgAbs_t 	avgAbs[MAX_TO_AVG];

	// init endpoint result & flags

	analyteType =  PHOS;
	checkAnalyteBlank = TRUE;

	phosResult =  NOVAL_NEG_FLOAT;
	phosError =  NO_ANALYTE_ERROR;

	// get analyte alg setup data & limits for rotor from ROC or from rate algorithm default

	factorsLimitsPtr = SetupAnalyteAlgInfo(analyteType, ENDPOINT);
	
	endpointAlgPtr = &endpointConfig_g.endpointAlgRecord;		

	// process endpoint standard average absorbance algorithm for up to 2 wavelength pairs if enabled

	if (!ProcessStdEndpointAvgAbs(analyteType, checkAnalyteBlank, endpointAlgPtr, factorsLimitsPtr, avgAbs, &phosError))
	{
		// check for PHOS rate in endpoint custom correction, if not continues with standard endpoint calculation
		
		if (PhosRateInEndpointCorrectionCheck(endpointAlgPtr, avgAbs, &phosError) == FALSE)
		{


			// IMV - endpoint paragraph 6

			StoreAlgParagraphNumImv( analyteType, 6.0 );

			// end IMV


			// calc PHOS result using bfd & rotor bar code factors

			phosResult = CalcBfdStdBarcodeCorrectedResult(avgAbs[PHOS_AVG1_IDX].avg, endpointAlgPtr->endpointCalcRecord.bfdCalibrationFactorIndex[PHOS_AVG1_IDX], analyteType, PHOS_SLOPE_IDX, PHOS_INTERCEPT_IDX);


			// IMV - endpoint paragraph 7 / endpoint starting abs / bfd corrected / barcode factors store

			StoreAlgParagraphNumImv( analyteType, 7.0 );

			StoreEndpointResultCalcParamsImv( analyteType );

			// end IMV


			// Check PHOS  within system & dynamic range limits

			ResultLimitsCheck(analyteType, phosResult, factorsLimitsPtr->lowSystemLimit, factorsLimitsPtr->lowDynamicRange, factorsLimitsPtr->highDynamicRange, factorsLimitsPtr->highSystemLimit, &phosError);
		}
	}


	// IMV - endpoint paragraph 8

	StoreAlgParagraphNumImv( analyteType, 8.0 );

	// end IMV


	// Store results in results data record

	StoreAnalyteResultsRecord(analyteType, phosResult, phosError, factorsLimitsPtr);

#ifndef __QNX__
	printf("PHOS invoked\n");
#endif
}
