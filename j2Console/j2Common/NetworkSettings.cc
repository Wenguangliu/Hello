#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <winsock2.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <iptypes.h>
#include <iphlpapi.h>

#include "CommonDefines.h"
#include "LogErr.h"
#include "NetWorkSettings.h"

#define ETHERNET_AAEON_ADAPTER_NAME          "PCI\\E1Q51CE61"
#define ETHERNET_ADLINK_ADAPTER_NAME         "PCI\\RTCENIC1"
#define ETHERNET_ADLINK_BK_ADAPTER_NAME      "PCI\\E1R51CE61"

int
SetTelentFtp (DWORD enable)
{
    HKEY        hKey = NULL;
    LONG        hRes;

    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"COMM\\FTPD", 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        hRes = RegSetValueEx (hKey, L"IsEnabled", 0, REG_DWORD,
                        (LPBYTE)&enable, sizeof(DWORD));
        if (hRes != ERROR_SUCCESS) {
            LogError("Failed to modify registry for FTPD. error %u", GetLastError());
            return -1;
        }
        hRes = RegSetValueEx (hKey, L"AllowAnonymous", 0, REG_DWORD,
                     (LPBYTE)&enable, sizeof(DWORD));
       if (hRes != ERROR_SUCCESS) {
            LogError("Failed to modify registry for Anonymous. error %u", GetLastError());
            return -1;
       }
       RegFlushKey(hKey);
       RegCloseKey(hKey);
    } else {
        LogError("Failed to open registry for FTPD. error %u", GetLastError());
        return -1;
    }
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, L"COMM\\TELNETD", 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        hRes = RegSetValueEx (hKey, L"IsEnabled", 0, REG_DWORD,
                    (LPBYTE)&enable, sizeof(DWORD));
        if(hRes != ERROR_SUCCESS) {
                LogError("Failed to modify registry for TELNETD. error %u", GetLastError());
                return -1;
        }
        RegFlushKey(hKey);
        RegCloseKey(hKey);
    } else {
        LogError("Failed to open registry for TELNETD. error %u", GetLastError());
        return -1;
    }

    return 0;
}

// set static ip address based on settings structure
int
SetStaticIp (char boardId, SettingsData_t * settings)
{
    wchar_t     *adpPath;
    wchar_t     *regPath;
    HKEY        hKey = NULL;
    LONG        hRes;
    BSTR        adapterName;
    wchar_t     ipWSTR[256];

    //ethernet port adapter and register path
    if (boardId  == 'D') {
        adpPath = ETHERNET_ADLINK_ADAPTER;
        regPath = IP_ADLINK_REG_PATH;
    } else if (boardId == 'A') {
        adpPath = ETHERNET_AAEON_ADAPTER;
        regPath = IP_AAEON_REG_PATH;
    } else if (boardId == 'B') {
        adpPath = ETHERNET_ADLINK_BK_ADAPTER;
        regPath = IP_ADLINK_BK_REG_PATH;
    } else {
        LogError ("Invalid CPU board Id, Fail to Configure Ip Address");
        return -1;
    }
    LogDebug ("Set static Ip %s", settings->comm.analyzerIp);

     // Modify Network  Registry Settings
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, regPath, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S", settings->comm.analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                       (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        if (settings->comm.ipMask[0] != '\0') {
            wsprintf (ipWSTR, L"%S", settings->comm.ipMask);
            hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        if (settings->comm.gateway[0] != '\0') {
            wsprintf(ipWSTR, L"%S", settings->comm.gateway);
            hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                        (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        }
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
        if (hRes != ERROR_SUCCESS) {
            LogError ("failed to set Ip registry");
            return -1;
        } else {
            // Rebind Adapter
            // Open the NDIS driver
            HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING, 0, NULL);
             // Send the device command.
             adapterName = SysAllocString (adpPath);
             DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapterName,
                                    (wcslen(adapterName)+1)*sizeof(wchar_t),
                                    NULL, 0, NULL, NULL);
             SysFreeString (adapterName);
             CloseHandle (hNdis);
        }

    } else {
        LogDebug ("Fail to open the register path key %S", regPath);
        return -1;
    }

    return 0;
}

// get DNS server Ip address
int
GetDnsServerAddress (char boardId, char *DnsSeverIp, char DhcpEnable)
{
    wchar_t *adpPath;
    wchar_t *regPath;
    HKEY    hKey = NULL;
    LONG    hRes;
    wchar_t ipWSTR[256];
    DWORD   dwcbData;

    //ethernet port adapter and register path
    if (boardId == 'D') {
        adpPath = ETHERNET_ADLINK_ADAPTER;
        regPath = IP_ADLINK_REG_PATH;
    } else if (boardId == 'A') {
        adpPath = ETHERNET_AAEON_ADAPTER;
        regPath = IP_AAEON_REG_PATH;
    } else if (boardId == 'B') {
        adpPath = ETHERNET_ADLINK_BK_ADAPTER;
        regPath = IP_ADLINK_BK_REG_PATH;
    } else {
        LogError ("Invalid CPU board Id, Fail to get server DNS");
        return -1;
    }
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, regPath, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        dwcbData = sizeof (ipWSTR);
        memset ((char *)ipWSTR, 0, dwcbData);
        if (DhcpEnable) {
            hRes = RegQueryValueEx(hKey, L"DhcpDNS", NULL, NULL, (BYTE *)ipWSTR, &dwcbData);
        } else {
            hRes = RegQueryValueEx(hKey, L"DNS", NULL, NULL, (BYTE *)ipWSTR, &dwcbData);
        }  
        sprintf (DnsSeverIp, "%S", ipWSTR);
    } else {
        LogDebug ("Fail to open the register path key %S", regPath);
        return -1;
    }  
    return 0;
}

int
SetDnsServerAddress (char boardId, char * DnsSeverIp)
{
    wchar_t *adpPath;
    wchar_t *regPath;
    HKEY    hKey = NULL;
    LONG    hRes;
    BSTR    adapterName;
    wchar_t ipWSTR[256];

    //ethernet port adapter and register path
    if (boardId == 'D') {
        adpPath = ETHERNET_ADLINK_ADAPTER;
        regPath = IP_ADLINK_REG_PATH;
    } else if (boardId == 'A') {
        adpPath = ETHERNET_AAEON_ADAPTER;
        regPath = IP_AAEON_REG_PATH;
    } else if (boardId == 'B') {
        adpPath = ETHERNET_ADLINK_BK_ADAPTER;
        regPath = IP_ADLINK_BK_REG_PATH;
    } else {
        LogError ("Invalid CPU board Id, Fail to Set Server DNS");
        return -1;
    }
    LogDebug ("Starting setting DNS server %s ...", DnsSeverIp);

    //Modify Network  Registry Settings
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, regPath, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S", DnsSeverIp);
        //hRes = RegSetValueEx (hKey, L"DhcpDNS", 0, REG_MULTI_SZ,
        //               (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        hRes = RegSetValueEx (hKey, L"DNS", 0, REG_MULTI_SZ,
                       (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
        if (hRes != ERROR_SUCCESS) {
            LogError ("failed to set DNS registry");
            return -1;
        } else {
            // Rebind Adapter
            // Open the NDIS driver
            HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                             0, NULL, OPEN_EXISTING, 0, NULL);
             // Send the device command.
             adapterName = SysAllocString (adpPath);
             DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapterName,
                                    (wcslen(adapterName)+1)*sizeof(wchar_t),
                                    NULL, 0, NULL, NULL);
             SysFreeString (adapterName);
             CloseHandle (hNdis);
        }

    } else {
        LogDebug ("Fail to open the register path key %S", regPath);
        return -1;
    }
    return 0;
}

// enable/disable DHCP
int
SetDhcp (char boardId, unsigned char DhcpEnable, char *hostName)
{
    wchar_t *adpPath;
    wchar_t *regPath;
    HKEY    hKey = NULL;
    LONG    hRes;
    BSTR    adapterName;
    DWORD   setValue;

    //ethernet port adapter and register path
    if (boardId == 'D') {
        adpPath = ETHERNET_ADLINK_ADAPTER;
        regPath = IP_ADLINK_REG_PATH;
    } else if (boardId == 'A') {
        adpPath = ETHERNET_AAEON_ADAPTER;
        regPath = IP_AAEON_REG_PATH;
    } else if (boardId == 'B') {
        adpPath = ETHERNET_ADLINK_BK_ADAPTER;
        regPath = IP_ADLINK_BK_REG_PATH;
    } else {
        LogError ("Invalid CPU board Id, Fail to Configure DHCP");
        return -1;
    }

    if (DhcpEnable) {
        setValue = 1;
    } else {
        setValue = 0;
    }
    if (DhcpEnable) {
        LogDebug ("Enable DHCP");
    } else {
        LogDebug ("Disable DHCP");
    }
    //Modify Network  Registry Settings
    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, regPath, 0, 0, &hKey);
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        hRes = RegSetValueEx (hKey, L"EnableDHCP", 0, REG_DWORD,
                                    (BYTE *)&setValue, sizeof (DWORD));
        if (hRes != ERROR_SUCCESS) {
            LogError ("Registry write error %d", hRes);
            return -1;
        } else {
            setValue = 132;
            hRes = RegSetValueEx (hKey, L"DhcpMaxRetry", 0, REG_DWORD,
                                    (BYTE *)&setValue, sizeof(DWORD));
            if (hRes != ERROR_SUCCESS) {
                LogError ("Registry write error %d", hRes);

            } else {
                hRes &= RegFlushKey(hKey);
                hRes &= RegCloseKey(hKey);
                if (hRes != ERROR_SUCCESS) {
                    LogError ("failed to set DHCP  registry");
                } else {
                    //LogDebug ("binding ndis drive ...");
                    // Rebind Adapter
                    // Open the NDIS driver
                    HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                                0, NULL, OPEN_EXISTING, 0, NULL);
                    // Send the device command.
                    adapterName = SysAllocString (adpPath);
                    DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapterName,
                                    (wcslen(adapterName)+1)*sizeof(wchar_t),
                                    NULL, 0, NULL, NULL);
                    SysFreeString (adapterName);
                    CloseHandle (hNdis);
                }
            }
        }
    } else {
        LogDebug ("Fail to open the register path key %S", regPath);
        return -1;
    }
    // Always set host name whenever DHCP enable 
   return 0;
}

//set device host name for DNS using
int
SetHostName (char *hostName)
{
    int retError = 0;

    

    if (sethostname (hostName, strlen (hostName) +1) != 0) {
        retError =  WSAGetLastError ();
        LogDebug ("set hostname error %d", retError);
    }

    return retError;
}

// read net adapter information
DWORD
ReadNetAdapterInfo (EthernetPortInfo_t *ethernetPortInfo)
{
    DWORD ret;
    IP_ADAPTER_INFO *info = NULL, *pos;
    DWORD           size = 0;
    char            *ptr, macAddr [128];
	unsigned int	i;

    // get adapter structure required buffer size
    ret = GetAdaptersInfo(info, &size);
    // allocate mac address
    info = (IP_ADAPTER_INFO *)malloc(size);
    // get adapter info
    ret = GetAdaptersInfo(info, &size);

    if (ret == NO_ERROR) {
        for (pos = info; pos != NULL; pos = pos->Next) {
            if (strcmp (pos->AdapterName, ETHERNET_ADLINK_ADAPTER_NAME) == 0) {
                ethernetPortInfo->cpuBoard  = ADLINK_CPU_BOARD;
                LogDebug ("Adlink CPU board");
            } else if (strcmp (pos->AdapterName, ETHERNET_AAEON_ADAPTER_NAME) == 0) {
                ethernetPortInfo->cpuBoard  = AAEON_CPU_BOARD;
                LogDebug ("Aaeon CPU board");
            } else if (strcmp (pos->AdapterName, ETHERNET_ADLINK_BK_ADAPTER_NAME) == 0) {
                ethernetPortInfo->cpuBoard  = ADLINK_BT_CPU_BOARD;
                LogDebug ("Adlink BT CPU board");
            } else {
                //LogDebug ("Unknow CPU board %s", pos->AdapterName);
                ethernetPortInfo->cpuBoard = UNKNOWN_CPU_BOARD;
            }
            ethernetPortInfo->dhcpEnabled =  pos->DhcpEnabled;
            // get mac address
            ptr = macAddr;
            sprintf (ptr, "%2.2x",pos->Address[0]);
            ptr += strlen (ptr);
            for (i = 1; i < pos->AddressLength; i++) {
                sprintf (ptr, ":%2.2x", pos->Address[i]);
                ptr += strlen (ptr);
            }
            strcpy (ethernetPortInfo->macAddress, macAddr);

            //get gatway
            strcpy (ethernetPortInfo->gateway, pos->GatewayList.IpAddress.String);
            //get Ip Address 
            strcpy (ethernetPortInfo->analyzerIp, pos->CurrentIpAddress->IpAddress.String);
            //get  ip Mask 
            strcpy (ethernetPortInfo->ipMask, pos->CurrentIpAddress->IpMask.String);
            // get server ip
            strcpy (ethernetPortInfo->serverIp, pos->DhcpServer.IpAddress.String);
            // get description 
            strcpy (ethernetPortInfo->adapterName, pos->AdapterName);
#if 0
            LogDebug ("haveWin =%d",            pos->HaveWins);
            LogDebug ("dhcp server =%s",        pos->DhcpServer.IpAddress.String);
            LogDebug ("Win server =%s",         pos->PrimaryWinsServer.IpAddress.String);
            LogDebug ("Win secd server =%s",    pos->SecondaryWinsServer.IpAddress.String);
            LogDebug ("Ip Address =%s", pos->CurrentIpAddress->IpAddress.String);
#endif
        }
        free(info);

        //get device name
        if (gethostname (ethernetPortInfo->hostName, 128) != 0) {
            LogDebug ("gethostname Err %d", WSAGetLastError ());
        }
    }  

    return ret;
}

// Set default network configuration in the case of 
// brand new CF card or nk.bin bsp update
void
SetDefaultNet (EthernetPortInfo_t *ethernetPortInfo)
{

    HKEY            hKey = NULL;
    LONG            hRes;
    wchar_t         ipWSTR[256];
    BSTR            adapter_name;

    // set default network configuration 
    strcpy (ethernetPortInfo->gateway, "0.0.0.0");
    strcpy (ethernetPortInfo->analyzerIp, "192.1.10.30");
    strcpy (ethernetPortInfo->ipMask, "255.255.255.0");
    strcpy (ethernetPortInfo->serverIp, "192.1.4.67");
    strcpy (ethernetPortInfo->adapterName, "unknow");
    strcpy (ethernetPortInfo->macAddress, "0:0:0:0");
    ethernetPortInfo->cpuBoard = UNKNOWN_CPU_BOARD;

    LogDebug ("Set default net configuration");

    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_AAEON_REG_PATH, 0, 0, &hKey);
    // now set adapter name, configure Ethernet board to default 
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S",ethernetPortInfo->analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                   (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf (ipWSTR, L"%S", ethernetPortInfo->ipMask);
        hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf(ipWSTR, L"%S", ethernetPortInfo->gateway);
        hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
        if (hRes != ERROR_SUCCESS) {
            LogError( "failed to set ip address");
        } else {
            // Rebind Adapter
            // Open the NDIS driver
            HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                            0, NULL, OPEN_EXISTING, 0, NULL);
            // Send the device command.
            adapter_name = SysAllocString (ETHERNET_AAEON_ADAPTER);
            if (DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                    (wcslen(adapter_name)+1)*sizeof(wchar_t),
                    NULL, 0, NULL, NULL ) != 0) {
                    ethernetPortInfo->cpuBoard  = AAEON_CPU_BOARD;
                //LogDebug ("Aaeon CPU board");
            }
            SysFreeString (adapter_name);
            CloseHandle (hNdis);
        }
    }  

    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_ADLINK_REG_PATH, 0, 0, &hKey);
    // now set adapter name, configure Ethernet board to default
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S",ethernetPortInfo->analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                   (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf (ipWSTR, L"%S", ethernetPortInfo->ipMask);
        hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf(ipWSTR, L"%S", ethernetPortInfo->gateway);
        hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
        if (hRes != ERROR_SUCCESS) {
            LogError( "failed to set ip address");
        } else {
            // Rebind Adapter
            // Open the NDIS driver
            HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_EXISTING, 0, NULL);
            // Send the device command.
            adapter_name = SysAllocString (ETHERNET_ADLINK_ADAPTER);
            if (DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                    (wcslen(adapter_name)+1)*sizeof(wchar_t),
                    NULL, 0, NULL, NULL ) != 0) {
                ethernetPortInfo->cpuBoard  = ADLINK_CPU_BOARD;
                //LogDebug ("Adlink CPU board");
            }
            SysFreeString (adapter_name);
            CloseHandle (hNdis);
        }
    }

    hRes = RegOpenKeyEx (HKEY_LOCAL_MACHINE, IP_ADLINK_BK_REG_PATH, 0, 0, &hKey);
    // now set adapter name, configure Ethernet board to default
    if (ERROR_SUCCESS == hRes && hKey != NULL) {
        wsprintf(ipWSTR, L"%S",ethernetPortInfo->analyzerIp);
        hRes = RegSetValueEx (hKey, L"IpAddress", 0, REG_MULTI_SZ,
                   (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf (ipWSTR, L"%S", ethernetPortInfo->ipMask);
        hRes &= RegSetValueEx (hKey, L"Subnetmask", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        wsprintf(ipWSTR, L"%S", ethernetPortInfo->gateway);
        hRes &= RegSetValueEx (hKey, L"DefaultGateway", 0, REG_MULTI_SZ,
                    (BYTE *)ipWSTR, (wcslen(ipWSTR)+1)*sizeof(wchar_t));
        hRes &= RegFlushKey(hKey);
        hRes &= RegCloseKey(hKey);
        if (hRes != ERROR_SUCCESS) {
            LogError( "failed to set ip address");
        } else {
            // Rebind Adapter
            // Open the NDIS driver
            HANDLE hNdis = CreateFile (DD_NDIS_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
                          0, NULL, OPEN_EXISTING, 0, NULL);
            // Send the device command.
            adapter_name = SysAllocString (ETHERNET_ADLINK_BK_ADAPTER);
            if (DeviceIoControl (hNdis, IOCTL_NDIS_REBIND_ADAPTER, adapter_name,
                    (wcslen(adapter_name)+1)*sizeof(wchar_t),
                    NULL, 0, NULL, NULL ) != 0) {
                ethernetPortInfo->cpuBoard  = ADLINK_CPU_BOARD;
                // LogDebug ("Adlink BK CPU board");
            }
            SysFreeString (adapter_name);
            CloseHandle (hNdis);
        }
    }
    // read adapter info again to get valid mac address
    ReadNetAdapterInfo (ethernetPortInfo);
}

// get Ethernet device information
void
GetEthernetPortInfo (EthernetPortInfo_t *ethernetPortInfo)
{
    if (ReadNetAdapterInfo (ethernetPortInfo) != NO_ERROR) {
        SetDefaultNet (ethernetPortInfo);
    }
}
