#ifndef __RAWDATA_H
#define __RAWDATA_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "CommonDefines.h"


#define	MAX_ANALYSIS_STATUS		42
#define MAX_TEMPERATURES		40


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
