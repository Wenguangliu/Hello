#ifndef _MISCELLANEOUSE_H
#define _MISCELLANEOUSE_H
// wavelength
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

typedef struct
{
    unsigned char       drawerTestFlag;
    unsigned char       barcodeTestFlag;
    unsigned char       motorTestflag;
    unsigned char       camFlag;
} ManufacturingFlags_t;


typedef struct
{
    unsigned int        openCounter;
    unsigned int        closeCounter;
    unsigned int        upCounter;
    unsigned int        downCounter;
} DrawerTestCounters_t;

typedef enum
{
    DRAWER_TEST_TYPE,
    MOTOR_TEST_TYPE,
    BARCODE_TEST_TYPE,
    TEMPERATURE_TEST_TYPE,
    TRIM_DAC_CALI_TYPE,
    CUVETTE_TEST_TYPE,
    NETWORK_IP_TYPE,
    NETWORK_DHCP_TYPE,
    NETWORK_SOCKET_TYPE,
    NETWORK_HOST_NAME_TYPE,
    AMS_MOTOR_TEST_TYPE,
    AMS_NDXT_TEST_TYPE,
    AMS_TEMP_TEST_TYPE,
    AMS_DELAY_TEST_TYPE,
    AMS_BARCODE_TEST_TYPE,
    AMS_ALIGNMENT_TEST_TYPE,
    AMS_CUV_TEST_TYPE,
    END_OF_INFO_TYTE,
} InfoType_t;

typedef struct
{
    InfoType_t      type;
    wchar_t         title [36];
    wchar_t         message [128];
    wchar_t         text [256];
} InfoText_t;

#endif
