#ifndef _MANUFACTURE_TEST_SET_H
#define _MANUFACTURE_TEST_SET_H

// manufacture test configuration and threshold 
typedef struct
{
    unsigned short      nextDrawerTestWaitTime;     //drawer test parameters
    unsigned short      countDisplayTime;
    unsigned short      savedWaitTime;
    unsigned short      motor7000MinSpeed;          // ams motor test config
    unsigned short      acc5000Min;
    unsigned short      acc4000Min;
    unsigned short      acc3000Min;
    unsigned short      acc2000Min;
    unsigned short      acc1000Min;
    unsigned short      dec4000Min;
    unsigned short      dec3000Min;
    unsigned short      dec2000Min;
    unsigned short      dec1000Min;
    unsigned short      dec0000Min;
    unsigned short      maxBlackOffset;
    double              nd05ExpectedAbs [9];
    double              nd10ExpectedAbs [9];
    double              nd20ExpectedAbs [9];
    double              bandBlockLimits [9];
    double              nd05DiffLimit;
    double              nd10DiffLimit;
    double              nd20DiffLimit;
    float               rotorTempMin;
    float               rotorTempMax;
    float               rotorTempInitLow;
    float               rotorTempInitHigh;
    float               rotorTempAveLow;
    float               rotorTempAveHigh;
    float               rotorTempOffsetMin;
    float               rotorTempOffsetMax;
    float               rotorTempstdDevMin;
    float               rotorTempstdDevMax;
    float               delayOffset;
    float               delayMinSamp;
    float               delayMaxSamp;
    float               delayMulti;
    float               delaySmall;
    float               delayMinSlop;
    float               delayMaxSlop;
    float               okHalfWitdh;
    float               delayMinDerive;
    float               delayMaxDerive;
    unsigned short      alignmentF1Min;
    unsigned short      alignmentF2Min;
    unsigned short      alignmentF3Min;
    unsigned short      alignmentF4Min;
    unsigned short      alignmentF5Min;
    int                 cuvOffsetMin;
    int                 cuvOffsetMax;
    float               cuvSlopeMin;
    float               cuvSlopeMax;
} ManufacturingTestSet_t;
#endif
