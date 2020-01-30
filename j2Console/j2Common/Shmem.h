#ifndef __SHMEM_H_INCLUDED
#define __SHMEM_H_INCLUDED

#include <windows.h>
#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define AMS_SHMEM_NAME                  "/AmsMem"
#define	BARCODE_SHMEM_NAME				"/BarcodeMem"
#define CAL_DATA_SHMEM_NAME				"/CalibrationMem"
#define COUNTERS_SHMEM_NAME				"/CountersMem"
#define DATABASE_SHMEM_NAME				"/DatabaseMem"
#define EXPIRY_SHMEM_NAME				"/ExpirysMem"
#define FACTORY_SHMEM_NAME				"/FactoryMem"
#define GPIO_SHMEM_NAME					"/GpioMem"
#define HARDWARE_SHMEM_NAME				"/HardwareMem"
#define ORDERS_SHMEM_NAME               "/OrdersMem"
#define RAW_SHMEM_NAME					"/RawMem"
#define REF_RANGES_SHMEM_NAME			"/RefRangesMem"
#define RESULTS_SHMEM_NAME				"/ResultsMem"
#define ROC_SHMEM_NAME					"/RocMem"
#define SETTINGS_SHMEM_NAME				"/SettingsMem"
#define SYSTEM_SHMEM_NAME				"/SystemMem"
#define UNITS_SHMEM_NAME				"/UnitsMem"
#define VERSION_SHMEM_NAME				"/VersionMem"
#define HL7_SHEME_NAME                  "/Hl7Mem"
#define HL7_FILE_SHEME_NAME             "/Hl7FileMem"

void * MapSharedMem( const char * shmemName, DWORD size );

#if defined(__cplusplus)
}
#endif

#endif	// __SHMEM_H_INCLUDED
