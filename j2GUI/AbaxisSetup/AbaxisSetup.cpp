// AbaxisSetup.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "SettingsData.h"
#include "FactoryData.h"
#include "SystemData.h"

#define ABAXIS_SETTINGS_SHARED_MEM	L"/SettingsMem"
#define ABAXIS_FACTORY_SHARED_MEM	L"/FactoryMem"
#define ABAXIS_SYSTEM_SHARED_MEM	L"/SystemMem"

HANDLE			g_hSettingsMappedFile = NULL;
HANDLE			g_hFactoryMappedFile = NULL;
HANDLE			g_hSystemMappedFile = NULL;

SettingsData_t*	g_pSettings = NULL;
FactoryData_t*  g_pFactory = NULL;
SystemData_t*   g_pSystemData = NULL;

/*
	void InitFileMapping()

	This function will be called by DllMain when the dll is attached to the process.
	It will create the new shared memory pointers
*/
static void InitFileMapping()
{
	//////////////////////////SETTINGS SHARED MEMORY///////////////////////////////
	g_hSettingsMappedFile = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
									  NULL,
									  PAGE_READWRITE,
									  0,
									  sizeof(SettingsData_t),	//	RGUNS: this might be changed depending on how the memory sharing is implemented
									  ABAXIS_SETTINGS_SHARED_MEM);

	g_pSettings = (SettingsData_t*) MapViewOfFile(g_hSettingsMappedFile,
												  FILE_MAP_ALL_ACCESS,
												  0,
												  0,
												  sizeof(SettingsData_t));	
	/////////////////////////FACTORY DATA SHARED MEMORY///////////////////////////////
	g_hFactoryMappedFile = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
									  NULL,
									  PAGE_READWRITE,
									  0,
									  sizeof(FactoryData_t),	//	RGUNS: this might be changed depending on how the memory sharing is implemented
									  ABAXIS_FACTORY_SHARED_MEM);
	g_pFactory = (FactoryData_t*) MapViewOfFile(g_hFactoryMappedFile,
												  FILE_MAP_ALL_ACCESS,
												  0,
												  0,
												  sizeof(FactoryData_t));
	
	/////////////////////////SYSTEM DATA SHARED MEMORY///////////////////////////////
	g_hSystemMappedFile = CreateFileMapping((HANDLE)INVALID_HANDLE_VALUE,
									  NULL,
									  PAGE_READWRITE,
									  0,
									  sizeof(SystemData_t),	//	RGUNS: this might be changed depending on how the memory sharing is implemented
									  ABAXIS_SYSTEM_SHARED_MEM);
	g_pSystemData = (SystemData_t*) MapViewOfFile(g_hSystemMappedFile,
												  FILE_MAP_ALL_ACCESS,
												  0,
												  0,
												  sizeof(SystemData_t));
}

/*
	void DeinitFileMapping()
	This function unmap the shared memory
*/
static void DeinitFileMapping()
{
	if(g_pSettings != NULL)
	{
		UnmapViewOfFile(g_pSettings);
	}

	if(g_pFactory != NULL)
	{
		UnmapViewOfFile(g_pFactory);
	}

    if (g_pSystemData != NULL) 
    {
        UnmapViewOfFile(g_pSystemData);
    }

	if(g_hSettingsMappedFile != NULL)
	{
		CloseHandle(g_hSettingsMappedFile);
	}

	if(g_hFactoryMappedFile != NULL)
	{
		CloseHandle(g_hFactoryMappedFile);
	}

	if(g_hSystemMappedFile != NULL)
	{
		CloseHandle(g_hSystemMappedFile);
	}
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			InitFileMapping();
			break;
		case DLL_PROCESS_DETACH:
			DeinitFileMapping();
			break;
		default:
			break;
	}
    return TRUE;
}

