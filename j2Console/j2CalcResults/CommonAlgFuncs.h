#ifndef __COMMONALGFUNCS_H
#define __COMMONALGFUNCS_H

#if defined(__cplusplus)
extern "C" {
#endif


#include "Algorithms.h"
#include "SharedRoc.h"
#include "SharedRaw.h"
#include "SharedBarcode.h"
 
#include "SystemCheckData.h"
#include "SampleBlankData.h"
#include "RqcData.h"
#include "IqcData.h"
#include "EndpointData.h"
#include "RateData.h"
#include "PosNegData.h"
#include "T4Data.h"
#include "DerivedData.h"
#include "Calc2CuvData.h"


// IMV - external function calls

#include "Imv.h"

// end IMV


extern	SystemCheckConfig_t		systemCheckConfig_g;
extern	SampleBlankConfig_t		sampleBlankConfig_g;
extern	RqcConfig_t				rqcConfig_g;
extern	IqcConfig_t				iqcConfig_g;
extern	EndpointConfig_t		endpointConfig_g;
extern	RateConfig_t			rateConfig_g;
extern	PosNegConfig_t			posNegConfig_g;
extern	T4Config_t				t4Config_g;
extern	DerivedConfig_t			derivedConfig_g;
extern	Calc2CuvConfig_t		calc2CuvConfig_g;


extern	const unsigned short wavelengthErrorFlags_g[NUM_WAVELENGTHS];

extern	const float	cuvettePathLengths_g[NUM_CUVETTES];


float ConvertEngineTimeToMinutes(unsigned long	engineTime);

unsigned long RqcAbusedCheck(void);

unsigned short SetAlgorithmErrorNumber(unsigned);

void ReportSystemError(unsigned long, unsigned short);

unsigned ProcessSystemCalcErrors(unsigned long);

unsigned short GetRawFlashReading(RawFlashWavelengths_t*, unsigned char, unsigned long*);

float CalcSampleTransmission(unsigned char, unsigned char, unsigned char, float*, unsigned long*); 

float CalcSampleAbsorbance(unsigned char, unsigned char, unsigned char, unsigned char, float, unsigned char, unsigned long*);	

unsigned long BeadMixCheck(unsigned char, unsigned char, unsigned char, float, unsigned long, unsigned long*);

float CalcCuvetteCheckTransmission(unsigned, unsigned char, unsigned char, unsigned char, unsigned, unsigned long*);

float CalcCuvetteCheckAbsorbance(unsigned, unsigned char, unsigned char, unsigned, unsigned long*);

unsigned long BeadMissingCheck(unsigned char, float, unsigned long*);

unsigned long BeadDistributionCheck(unsigned char, float, unsigned long*);

void SystemBeadMissingDistCheck(unsigned, const BeadDistCheck_t*, unsigned long, unsigned short);  

unsigned long StartingAbsorbanceCheck(unsigned char, unsigned char, unsigned char, unsigned char, float, unsigned long, unsigned long*);

unsigned long CalcAverageAbsorbance(AvgAbs_t*, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, float, unsigned char, unsigned char, unsigned char, unsigned long*);

unsigned long EndpointRateErrorCheck(AvgAbs_t*, float, float, float, unsigned long, unsigned long*);

float GetBarcodeParameter(unsigned char, unsigned);

float BfdCalAdjust(float, unsigned);

float CalcBfdStdBarcodeCorrectedResult(float, unsigned, unsigned char, unsigned, unsigned);

void EndogenousLimitsCheck(unsigned char, CommonFactorsLimits_t*, unsigned long*);

void ResultLimitsCheck(unsigned char, float, float, float, float, float, unsigned long*);

void LoadAnalyteAlgData(unsigned char);

CommonFactorsLimits_t *SetupAnalyteAlgInfo(unsigned char, unsigned);

void StoreAnalyteResultsRecord(unsigned char, float, unsigned long, CommonFactorsLimits_t*);

unsigned ProcessStdEndpointAvgAbs(unsigned char, unsigned, EndpointAlgRecord_t*, CommonFactorsLimits_t*, AvgAbs_t*, unsigned long*);

float CalcExponentialRotorAgeDecay(float); 

float CalcLinearTimeCorrection( unsigned char, float );

void ProcessStdEndpointAlgorithm(unsigned char, unsigned);

unsigned long GetSlopeTimeAbsData(SlopeParams_t*, unsigned char, unsigned char, RateCalcRecord_t*, float, unsigned char, SlopeAbsData_t*, unsigned long*);

unsigned ProcessStdRateChecks(unsigned char, unsigned char, unsigned char, unsigned char, RateAlgRecord_t*, CommonFactorsLimits_t*, unsigned long*);

float ProcessStdRateSlope(unsigned char, RateAlgRecord_t*, CommonFactorsLimits_t*, float*, unsigned char*, unsigned long*);

float CalcTemperatureCorrection(float);

void ProcessStdRateAlgorithm(unsigned char);

void ProcessStdPosNegAlgorithm(unsigned char);

void Process2CuvAlgorithm(unsigned char, unsigned char, unsigned char);


#if defined(__cplusplus)
}
#endif

#endif	// __COMMONALGFUNCS_H
