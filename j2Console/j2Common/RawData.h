#ifndef __RAWDATA_H
#define __RAWDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "CommonDefines.h"


#define	MAX_ANALYSIS_STATUS		42
#define MAX_TEMPERATURES		40
#define MAX_DELAY_TIMES         76
#define MAX_NDXT_SAMPLE_FLASHES 20

#undef  MOTOR_48_TEST
#ifdef MOTOR_48_TEST
#define MAX_MOTOR_TEST_DATA     39000
#define MOTOR_DUMP_DATA_LEN     39000
#else
#define MAX_MOTOR_TEST_DATA     9216
#define MOTOR_DUMP_DATA_LEN     8665
#endif



typedef struct
{
    unsigned short  position;
} RawAmsMotorTestData_t;


typedef struct
{
	unsigned long	engineTime;
	unsigned short	adcReferenceOffset;
} AdcBias_t;

typedef struct
{
	unsigned long	engineTime;
	unsigned char	analysisStatus;
	unsigned char	analysisParameter;
} AnalysisStatus_t;

typedef struct
{
	unsigned long	engineTime;
	unsigned short	rotorTemperature;
	unsigned short	ambientTemperature;
	unsigned short	topPlateTemperature;
	unsigned short	bottomPlateTemperature;
	unsigned short	topPlateCurrent;
	unsigned short	bottomPlateCurrent;
} RawTemperatures_t;

typedef struct
{
	unsigned long	engineTime;
	unsigned short	rawFlashReadings[NUM_WAVELENGTHS];
	unsigned char	sampleType;
	unsigned char	cuvetteNumber;
	unsigned char	flashNumber;
} RawFlashWavelengths_t;


typedef struct
{
	AdcBias_t				adcBias;
	AnalysisStatus_t		analysisStatus[MAX_ANALYSIS_STATUS];
	RawTemperatures_t		temperatures[MAX_TEMPERATURES];
	RawFlashWavelengths_t	beadCheck1[NUM_CUVETTES][NUM_BEAD_CHECK_FLASHES+1];
	RawFlashWavelengths_t	beadCheck2[NUM_CUVETTES][NUM_BEAD_CHECK_FLASHES+1];
	RawFlashWavelengths_t	distributionCheck[NUM_CUVETTES][NUM_DIST_CHECK_FLASHES+1];
	RawFlashWavelengths_t	opticalDacs[NUM_OPTICAL_DACS_FLASHES+1];
	RawFlashWavelengths_t	blackOffsets[NUM_BLACK_OFFSETS_FLASHES+1];
	RawFlashWavelengths_t	preSampleFullRotorScan[NUM_CUVETTES][NUM_FRS_SAMPLE_FLASHES+1];
	RawFlashWavelengths_t	rotorAnalyteSamples[MAX_SAMPLE_TYPES][MAX_PASSES][NUM_SAMPLE_FLASHES+1];
	RawFlashWavelengths_t	postSampleFullRotorScan[NUM_CUVETTES][NUM_FRS_SAMPLE_FLASHES+1];
    RawFlashWavelengths_t   ndxtSamples [NUM_CUVETTES][MAX_NDXT_SAMPLE_FLASHES +1];
    RawFlashWavelengths_t   cuvetteDelay [MAX_DELAY_TIMES][NUM_CUV_DELAY_FLASHES + 1];
    RawAmsMotorTestData_t   motorTestData [MAX_MOTOR_TEST_DATA];
} RotorRawReadings_t;


typedef struct
{
	unsigned char	statusCount;
	unsigned char	temperatureCount;
	unsigned char	passCount;
	unsigned char	analyteHasData[128];
	unsigned		flashCount;
} ReceivedDataCounter_t;

typedef struct
{
	RotorRawReadings_t		rotorRawReadings;
	ReceivedDataCounter_t	receivedDataCounter;
} RawData_t;


#if defined(__cplusplus)
}
#endif

#endif	// __RAWDATA_H
