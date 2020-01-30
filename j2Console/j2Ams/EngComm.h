#ifndef _ENGCOMM_H
#define _ENGCOMM_H



typedef enum {
    W340,
    W405,
    W467,
    W500,
    W515,
    W550,
    W600,
    W630,
    W850,
    WHTE,
} Wavelength_t;

#define FLASH       0

typedef enum {
    AMS_BEAD_CHK_1,
    AMS_BEAD_CHK_2,
} BeadCheckType_t;

/* new system ADC convert to legacy system  ADC */
#define     ADC_CONVERT_PARAMETER   0.455105

/* system drawer status */
typedef enum 
{
    
    DRAWER_INVALID_STATUS,
    CAM_DOWN_AND_DRAWER_OPEN,
    CAM_DOWN_AND_DRAWER_CLOSE,
    CAM_UP_AND_DRAWER_OPEN,
    CAM_UP_AND_DRAWER_CLOSE,
    MECHANISM_BUSY,
    MECHANISM_JAMMED,
} DrawerStatus_t;

typedef enum
{
    ROTOR_NOT_PRESENT,
    ROTOR_PRESENT,
} RotorStatus_t;

typedef struct
{
    DrawerStatus_t  DrawerStatus;    
    RotorStatus_t   RotorStatus;

} DrawerRotorStatus_t;

typedef struct
{
    unsigned int        engineTime;
    unsigned short      rotorTemperature; 
    unsigned short      ambientTemperature;
    unsigned short      topPlateTemperature;
    unsigned short      bottomPlateTemperature;
    unsigned short      topPlateCurrent;
    unsigned short      bottomPlateCurrent;
} EngineTemperature_t;

/*
 * talk to Eninge cuvette  protocol
 */
typedef struct
{
    unsigned char   currentDac;
    unsigned char   thresholdDac;
    unsigned short  maxIndexWidth;
    unsigned short  minIndexWidth;
    unsigned short  maxCuvetteWidth;
    unsigned short  minCuvetteWidth;
    unsigned short  maxCuvetteDistance;
    unsigned short  minCuvetteDistance;
} CuvetteTestParameter_t;

extern DrawerRotorStatus_t  DrawerRotorStatus;

#endif
