#ifndef _RAD_H
#define _RAD_H

#include "CommonDefines.h"
#include "SystemErrorNums.h"

#define MAX_ROTOR_ERROR_NUMBERS         20
#define MAX_ABS_RECORDS                 4000
#if 0
#define MAX_RAD_FILE_LENGTH             614400L
#else
#define MAX_RAD_FILE_LENGTH             307200L
#endif

#pragma pack (push,1)
typedef struct
{
    char            factoryCfgFileName [32]; 
    char            versionCfgFileName [32];
    char            calibrationCfgFileName [32];
    char            rocFileName [32];
    char            risFileName [32];

    char            abaxisProductCode;
    char            instrumentSerialNumber [12];
    char            softwareVersionNumber [16];

    float           airTemperatureOffset;
    float           cuvetteTransmitLimit;
    float           bfdCalibrationFactors [32];
    unsigned short  vavelengthFilterVurvers[9][32];
    unsigned short  globalCuvetteDelay;
    unsigned short  cuvetteDelays [30];
    unsigned short  flashLampThreshold [10];
    unsigned char   heaterDacSettings [4];
    unsigned char   fanTEmperatureSetting;

    unsigned char   barcodeDacs [2];
    unsigned char   cuvetteMarkDacs [2];
    unsigned char   wavelengthDacTrims [10];
    unsigned short  wavelengthAdcOffsets [10];

} instrumentInfo_t;

typedef struct
{
    unsigned char   rotorNumber;
    unsigned char   rotorFormat;
    char            rotorProductCode;
    char            rotorLotNumber [8];
    char            rotorExpiryDate [9];
    
    char            rotorResultType;
    char            rotorName [32];
    char            rotorRunDateTime [13];

    char            patientControlldentifier [15];
    char            operatorIdentifier [15];
    char            alternateSampleIdentifier [15];

    unsigned long   rotorSystemFlags;
    unsigned short  rotorDacTrimFlags;
    unsigned short  rotorOffsetFlags;
    unsigned long   rotorBeadCheck1Flags;
    unsigned long   rotorBeadCheck2Flags;
    unsigned long   rotorDistributinCheckFlags;
    unsigned short  rotorQcFlags;
    unsigned short  rotorOffsetSdErrorFlags;
    unsigned short  rotorWavelengthCvFlags;

    float           rotorHemolysisIndex;
    float           rotorLipemicsIndex;
    float           rotorLctericIndex;
    float           rotorRqcLowLimit;
    float           rotorIqcRatioPrecision;
    float           rotorIqcLevel1Results [8];
    float           rotorIqcLevel2Results [8];

    unsigned long   referenceRangeType;

    float           rotorBarcodeFactors [44];
    
    unsigned short  rotorErrorNum;

} rotorInfomationResults_t;


typedef struct
{
    unsigned char   resultsAnalyteIndices [64];
    unsigned char   statusCount;
    unsigned char   temperatureCount;
    unsigned char   passCount;
    unsigned long   absorbanceCount;
    unsigned char   rawDataAnalyteIndices [128];
} dataDescriptor_t;


typedef struct
{
    unsigned char   analyteType;
    unsigned char   analyteAlgorithmVersion [6];
    
    float           analyteResult;
    unsigned long   analyteFlags;

    float           lowReferenceRangeLimit;
    float           highReferenceRangeLimit;

    float           lowDynamicRangeLimit;
    float           highDynamicRangeLimit;

    float           lowSystemRangeLimit;
    float           highSystemRangeLimit;
} rotorAnalyteResults_t;


typedef struct 
{
    float           engineMinutesTime;
    unsigned char   sampleType;
    unsigned char   sampleTypeOpticalBlank;
    unsigned char   samplePass;
    unsigned char   wavelengthPair;
    float           desirePathLength;
    unsigned char   cuvetteNumber;
    unsigned long   analyteError;
    float           sampleAbsorbance;
} rotorAbsorbances_t;

typedef struct
{
    unsigned long   engineTime;
    unsigned char   analysisStatus;
    unsigned char   analysisParameter;
} analysisStatus_t;

typedef struct
{
    unsigned long   engineTime;

    unsigned short  rotorTemperature;
    unsigned short  ambientTemperature;
    unsigned short  topPlateTemperature;
    unsigned short  bottomPlateTemperature;
    unsigned short  topPlateCurrent;
    unsigned short  bottomPlateCurrent;
} rawTemperatures_t;


typedef struct
{
    unsigned long   engineTime;
    unsigned short  rawFlashReading [10];
    unsigned char   sampleType;
    unsigned char   cuvetteNum;
    unsigned char   flashNum;
}  rawFlashWaveLength_t;

typedef struct
{
    instrumentInfo_t            instrumentInfo;
    rotorInfomationResults_t    rotorInfomationResults;
    dataDescriptor_t            dataDescriptor;
    rotorAnalyteResults_t       rotorAnalyteResults;
    rotorAbsorbances_t          rotorAbsorbances;
    timeFlag_t                  fileTime;
    rawFlashWaveLength_t        rawFlashWaveLength [11];
} radFile_t;



#pragma pack (pop)

/* 
 * Rad file link list which needs to be send
 */

typedef struct  _radFileList_t
{
    unsigned int            fileIndex;
    timeFlag_t              fileTime;
    char                    fileName [128];
    struct _radFileList_t   *next;
} radFileList_t;


/* data structure offset */
#define INSTRUMENTINFO_OFFSET           sizeof (unsigned long)
#define ROTOR_INFO_RESULTS_OFFSET       INSTRUMENTINFO_OFFSET + sizeof (instrumentInfo_t) 
#define DATA_DESCRIPTOR_OFFSET          ROTOR_INFO_RESULTS_OFFSET + sizeof (rotorInfomationResults_t) 
#define ROTOR_ANALY_RESULT_OFFSET       DATA_DESCRIPTOR_OFFSET + sizeof (dataDescriptor_t)

#define ANALYSIS_RESULTS_LENGTH         sizeof (rotorAnalyteResults_t)
#define ABSORBANCES_LENGTH              sizeof (rotorAbsorbances_t)
#define ANALYSIS_STATUS_LENGTH          sizeof (analysisStatus_t)
#define RAW_TEMPERATURE_LENGTH          sizeof (rawTemperatures_t)
#define TRIM_DAC_RAW_DATA_OFFSET        13770 //14790    

#endif
