#ifndef __SHMEM_H_INCLUDED
#define __SHMEM_H_INCLUDED

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define AMS_SHMEM_NAME                  "/Ams"
#define	BARCODE_SHMEM_NAME				"/Barcode"
#define CAL_DATA_SHMEM_NAME				"/Calibration"
#define COUNTERS_SHMEM_NAME				"/Counters"
#define DATABASE_SHMEM_NAME				"/Database"
#define EXPIRY_SHMEM_NAME				"/Expirys"
#define FACTORY_SHMEM_NAME				"/Factory"
#define GPIO_SHMEM_NAME					"/Gpio"
#define HARDWARE_SHMEM_NAME				"/Hardware"
#define ORDERS_SHMEM_NAME				"/Orders"
#define RAW_SHMEM_NAME					"/Raw"
#define REF_RANGES_SHMEM_NAME			"/RefRanges"
#define RESULTS_SHMEM_NAME				"/Results"
#define ROC_SHMEM_NAME					"/Roc"
#define SETTINGS_SHMEM_NAME				"/Settings"
#define SYSTEM_SHMEM_NAME				"/System"
#define UNITS_SHMEM_NAME				"/Units"
#define VERSION_SHMEM_NAME				"/Version"

void * MapSharedMem( const char * shmemName, size_t size );

#if defined(__cplusplus)
}
#endif

#endif	// __SHMEM_H_INCLUDED
