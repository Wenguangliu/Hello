#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#include <winioctl.h>
#include <ntddndis.h>

#include "Commondefines.h"
#include "PingTest.h"
#include "Global.h"


// setting Ethernet port for AMS ethernet testing
void
SetEthernet (netSettings_t netSettings)
{
    HKEY hKey = NULL;
    LONG hRes;
    wchar_t     ipWSTR[256];
    BSTR        adapter_name;


    // Modify Network Aaeon Registry Settings
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_AAEON_REG_PATH, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S", netSettings.analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                       (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        if (netSettings.ipMask[0] != '\0') {
            wsprintf (ipWSTR, L"%S", netSettings.ipMask);
            hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        if (netSettings.gateway[0] != '\0') {
            wsprintf(ipWSTR, L"%S", netSettings.gateway);
            hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
    }
    if (hRes != ERROR_SUCCESS) {
        printf ( "failed to modify ip address" );
    } else {
        // Rebind Adapter
        // Open the NDIS driver
        HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING, 0, NULL);
        // Send the device command.
        adapter_name = SysAllocString (ETHERNET_AAEON_ADAPTER);
        if (!DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                        (wcslen(adapter_name)+1)*sizeof(wchar_t),
                        NULL, 0, NULL, NULL )) {
            printf ("Startup::Not Support Aaeon Ethernet Port");
        }
        SysFreeString(adapter_name);
        CloseHandle(hNdis);
    }

    // Modify Network Adlink Registry Settings
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_ADLINK_REG_PATH, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S", netSettings.analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        if (netSettings.ipMask[0] != '\0') {
            wsprintf(ipWSTR, L"%S", netSettings.ipMask);
            hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        if (netSettings.gateway[0] != '\0') {
            wsprintf(ipWSTR, L"%S", netSettings.gateway);
            hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
    }
   if (hRes != ERROR_SUCCESS) {
            printf ("failed to set ip address registry");
    } else {
        // Rebind Adapter
        // Open the NDIS driver
        HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING, 0, NULL);
         // Send the device command.
        adapter_name = SysAllocString (ETHERNET_ADLINK_ADAPTER);
        if (!DeviceIoControl(hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                        (wcslen(adapter_name)+1)*sizeof(wchar_t),
                        NULL, 0, NULL, NULL )) {
                printf ("Startup::Not Support Adlink Ethernet Port");
        }
        SysFreeString (adapter_name);
        CloseHandle (hNdis);
    }

    // Modify Network Adlink bay trail Registry Settings
     hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_ADLINK_BK_REG_PATH, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S", netSettings.analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        if (netSettings.ipMask[0] != '\0') {
            wsprintf(ipWSTR, L"%S", netSettings.ipMask);
            hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        if (netSettings.gateway[0] != '\0') {
            wsprintf(ipWSTR, L"%S", netSettings.gateway);
            hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
    }
    if (hRes != ERROR_SUCCESS) {
            printf ("failed to set ip address registry");
    } else {
        // Rebind Adapter
        // Open the NDIS driver
        HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING, 0, NULL);
         // Send the device command.
        adapter_name = SysAllocString (ETHERNET_ADLINK_BK_ADAPTER);
        if (!DeviceIoControl(hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                        (wcslen(adapter_name)+1)*sizeof(wchar_t),
                        NULL, 0, NULL, NULL )) {
                printf ("Startup::Not Support Adlink Ethernet Port");
        }
        SysFreeString (adapter_name);
        CloseHandle (hNdis);
    }

}
