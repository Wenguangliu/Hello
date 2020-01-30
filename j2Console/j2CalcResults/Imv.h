#ifndef __IMV_H
#define __IMV_H

#if defined(__cplusplus)
extern "C" {
#endif


void StoreIntermediateValue( unsigned char, ImvType_t, ImvMethod_t, float );
void InitNextIdxImv( void );

void InitRotorImv( void );

void InitAnalyteImv( void );

void StoreAlgParagraphNumImv( unsigned char, float );

void StoreAlgVersionNumImv( unsigned char );

void StoreCommonAnalyteImv( unsigned char, CommonFactorsLimits_t* );

void SetImvBeadMixParams( float, float, float );
void StoreBeadMixImv( unsigned char );

void SetImvStartAbsParams( float, float );
void StoreStartAbsImv( unsigned char );
void StorePosNegStartAbsImv( unsigned char );

void SetImvBfdCorrectedParams( float );
void StoreBfdCorrectedImv( unsigned char );
void SetImvWl1BfdCorrected( AvgAbs_t*, EndpointAlgRecord_t* );
void SetImvBarcodeParams( float );
void StoreBarcodeParamsImv( unsigned char );
void SetImvPatientTypeBarcodeParams( unsigned char );

void SetImvEndpointFlatnessParams( float, float );
void SetImvEndpointOverSystemRangeParams( float, float, float, float );
void StoreEndpointAvgAbsImv( unsigned char, unsigned char, EndpointAlgRecord_t*, AvgAbs_t* );
void SetImvEndpointDualWavelengthResults( float, float );
void SetImvEndpointDualWavelengthResultCompare( float );
void StoreEndpointResultCalcParamsImv( unsigned char );

void SetImvRateCompareWindowAbs( unsigned char, float );
void StoreRateSlopeWindowLimitsParamsImv( unsigned char, RateAlgRecord_t*, SlopeParams_t* );
void SetImvRateTemperatureCorrectionParams( float, float, float, float );
void StoreRateTemperatureCorrectionParamsImv( unsigned char );
void StoreRateResultCalcParamsImv( unsigned char );

void Store2CuvAlgParamsImv( unsigned char, unsigned char, unsigned long, float, float, float, float, float, Calc2CuvRecord_t* );

void SetImvSystemCheckTransmissionParams( unsigned, float );
void StoreSystemCheckTransmissionParamsImv( void );
void StoreSystemCheckAbsorbanceParamsImv( float*, float, float );

void StoreSampleBlankStartAbsImv( void );
void SetImvSampleBlankAvgAbsParams( unsigned, AvgAbs_t* );
void StoreSampleBlankAvgAbsImv( void );

void StoreIqcAbsRatioParamsImv( float*, float*, float*, float );

void StoreHdlStartAbsFlatnessParmsImv( unsigned char, EndpointAlgRecord_t* );

#if defined(__cplusplus)
}
#endif

#endif	// __IMV_H
