#ifndef _XML_DATA_H
#define _XML_DATA_H

#include "CommonDefines.h"
#include "CalData.h"
#include "FactoryData.h"
#include "SettingsData.h"

/* 
 * factory xml file format 
 */

#define SERIAL_NUMBER_LEN   12  

/* 
 * mix profile  format
 */
typedef struct 
{
    unsigned short  rpm;
    unsigned short  time;
    unsigned short  dir;
    unsigned short  gain;
    unsigned short  acce;
    unsigned short  rept;
    unsigned char   flag;
} MixProfile_t;

typedef struct 
{
    MixProfile_t    mixProfile [100];
    unsigned short  profileIndex;
    
} DownloadMixProfile_t;

/* 
 * sample profile format 
 */
typedef struct
{
    unsigned char cuvNumber;
    unsigned char numSample;
    unsigned char wavLen0;
    unsigned char wavLen1;
    unsigned char wavLen2;
    unsigned char loopCount;
    unsigned char sampleType;
    unsigned char optType;
} SampleProfile_t;


/*
 * Timer Setting structure 
 */
typedef struct 
{
    unsigned  char  month;
    unsigned  char  day;
    unsigned  char  year;
    unsigned  char  minute;
    unsigned  char  second;
} Timer_t;


/* Global structure for saving calibration data */
extern FactoryData_t            FactoryData;
extern CalibrationData_t        CalibrationData;
extern SettingsData_t           Settings;
extern DownloadMixProfile_t     DownloadMixProfile;

#endif
