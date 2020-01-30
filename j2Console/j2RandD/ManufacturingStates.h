#ifndef _MANUFACTURINGSTATES_H
#define _MANUFACTURINGSTATES_H
#include "RandDStates.h"
 

#define AMS_MOTOR_RAW_DATA_FILE         "/Hard Disk/Abaxis/Ams/AmsMotorRawData.txt"
#define AMS_MOTOR_TEST_REPORT_FILE      "/Hard Disk/Abaxis/Ams/AmsMotorReport.txt"
#define AMS_MOTOR_SPEED_FILE            "/Hard Disk/Abaxis/Ams/AmsMotorSpeed.txt"
#define AMS_NDXT_REPORT_FILE            "/Hard Disk/Abaxis/Ams/AmsNdxtReport.txt"
#define AMS_NDXT_DISP_REPORT_FILE       "/Hard Disk/Abaxis/Ams/AmsNdxtDisplayReport.txt"
#define AMS_TEMPERATURE_DATA_FILE       "/Hard Disk/Abaxis/Ams/AmsTemperature.txt"
#define AMS_TEMPERATURE_REPORT_FILE     "/Hard Disk/Abaxis/Ams/AmsTemperatureReport.txt"
#define AMS_DELAY_DATA_FILE             "/Hard Disk/Abaxis/Ams/AmsDelayData.txt"
#define AMS_DELAY_REPORT_FILE           "/Hard Disk/Abaxis/Ams/AmsDelayReport.txt"
#define AMS_BARCODE_DATA_FILE           "/Hard Disk/Abaxis/Ams/AmsBarcodeData.txt"
#define AMS_BARCODE_REPORT_FILE         "/Hard Disk/Abaxis/Ams/AmsBarcodeReport.txt"
#define AMS_ALIGNMENT_DATA_FILE         "/Hard Disk/Abaxis/Ams/AmsAlignmentData.txt"
#define AMS_ALIGNMENT_REPORT_FILE       "/Hard Disk/Abaxis/Ams/AmsAlignmentReport.txt"
#define AMS_CUVETTE_DATA_FILE           "/Hard Disk/Abaxis/Ams/AmsCuvetteData.txt"
#define AMS_CUVETTE_REPORT_FILE         "/Hard Disk/Abaxis/Ams/AmsCuvetteReport.txt"
#define AMS_DRAWER_TEST_REPORT_FILE     "/Hard Disk/Abaxis/Ams/AmsDrawereport.txt"

#define AMS_DATA_DIRECTARY              "/Hard Disk/Abaxis/Ams"

#define AMS_DATA_USB_DIRECTORY          "/Hard Disk2/Ams"
#define AMS_NDXT_TEST_REPORT_FILE       "AmsNdxtTestReport"


class StateManufacturingSelection :  public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );

};


class StateManufacturingTest :  public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );

};

class StateManufacturingTest2 :  public StateRandD
{
public:
    void    Enter();
    state_t ProcessMessage( int msgId, char * data );

};
#endif
