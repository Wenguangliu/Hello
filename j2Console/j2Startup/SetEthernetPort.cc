#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>
#include <ctype.h>
#include <winsock2.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <pnp.h>
#include <notify.h>

#include "StartupGlobal.h"


#define                     ADLINK_ETHERNET_PORT_DESP    "PCI\\RTCENIC1"        //Ethernet port description
#define                     AAEON_ETHERNET_PORT_DESP     "PCI\\E1Q51CE61"       //Ethernet port description

#undef                      NETWORK_DETECTION_DEBUG

static EthernetPortInfo_t   ethernetPortInfo;

// create SetIpAddress file
void
WriteSetIpAddress (SettingsData_t *settings)
{
    FILE *  fp;
    WCHAR   wFileName [128];
    int     error;

   // convert to wchar file
    wsprintf (wFileName, L"%S", IP_ADDRESS_FILE_NAME);

    //change file attribution
    if (SetFileAttributes (wFileName, FILE_ATTRIBUTE_NORMAL) == 0) {
      error = GetLastError ();
      LogError ("Fail to change file %s write permit , error = %u", wFileName, error);
      return ;
    }

    fp = fopen (IP_ADDRESS_FILE_NAME, "wb");
    if (fp != 0) {
        if (settings->comm.analyzerIp [0] == '\0') {
            fprintf (fp, "%s\n", "0.0.0.0");
        } else {
            fprintf (fp, "%s\n", settings->comm.analyzerIp );
        }
        if (settings->comm.ipMask[0] == '\0') {
            // save mask as zero */
            fprintf(fp,  "%s\n","0.0.0.0");
        } else {
            fprintf (fp, "%s\n",settings->comm.ipMask);
        }
        if (settings->comm.gateway[0] == '\0') {
            fprintf(fp,  "%s\n","0.0.0.0");
        } else {
            fprintf( fp, "%s\n", settings->comm.gateway);
        }
        fclose (fp);
        //update settings.xml
        WriteSettings (settings);
    }
}

// configure ethernet port
// enable/disable DHCP
// set board ip address in the case of DHCP disable
void
SetEthernet (SettingsData_t *settings, SystemData_t *systemData)
{
    if (settings->comm.dhcpEnabled) {
        // if dhcp is not enable, enable dhcp here
        if (ethernetPortInfo.dhcpEnabled == false) {
            LogDebug ("setting dhcp ...");
            SetDhcp (systemData->cpuBoard, true, settings->comm.hostName);
            //settings->comm.dhcpEnabled = true;
            //update xml file here
            // WriteSettings (settings);
        }
    } else {
        // disable dhcp
        if (ethernetPortInfo.dhcpEnabled == true) {
            SetDhcp (systemData->cpuBoard, false, settings->comm.hostName);
        }
        SetStaticIp (systemData->cpuBoard, settings);
    }
}

// check host name.
// if host name does not match settings name
// set new name to system
void
CheckHostName (SettingsData_t *settings)
{
    //char            hostName [128];
    int             len;

    // default host name, must be changed to Analyzer serial number
    if (strcmp (settings->comm.hostName, "Analyzer123") == 0) {
        memset (settings->comm.hostName, 0, sizeof (settings->comm.hostName));
        len = strlen (factoryData->instrumentSerialNumber);
        strncpy (settings->comm.hostName,  factoryData->instrumentSerialNumber + (len - 6), 6);
        // set default host name
        SetHostName (settings->comm.hostName);
        // update settings xml file  host name  
        WriteSettings (settings);
    }

}

// configure ethernet and create SetIpAddress for new system
void
ActiveDefaultIpAddress (SettingsData_t *settings,  SystemData_t *systemData)
{
    DWORD attribs;
    wchar_t                     fileName[255];
//    PIP_ADAPTER_ADDRESSES       ipAdapterAddressPtr;
//    unsigned long               len;
    



    //Get ethernet port inforamtion
    GetEthernetPortInfo (&ethernetPortInfo);

    //copy cpu board type to systemData
    systemData->cpuBoard = ethernetPortInfo.cpuBoard;
    if (systemData->cpuBoard == ADLINK_CPU_BOARD) {
        LogDebug ("Adlink CPU board");
    } else if (systemData->cpuBoard == AAEON_CPU_BOARD) {
        LogDebug ("Aaeon CPU board");
    } else if (systemData->cpuBoard == ADLINK_BT_CPU_BOARD) {
        LogDebug ("Adlink BT CPU board");
    } else {
        LogDebug ("Unknow CPU board");
    }
    // copy mac address
    strcpy (settings->comm.macAddress, ethernetPortInfo.macAddress);

    //check and set host name
    CheckHostName (settings);

    //configuration Ethernet port
    SetEthernet (settings, systemData);

    //create default ip address file in the case of it does not exist
    if (settings->comm.dhcpEnabled == false) {
        wsprintf (fileName, L"%S", IP_ADDRESS_FILE_NAME);
        attribs = GetFileAttributes (fileName);
        //if SetIpAddress file does not exist , create one
        if (attribs == INVALID_FILE_ATTRIBUTES) {
            WriteSetIpAddress (settings);
        }
    }

    // get DNS server IP address
    GetDnsServerAddress (systemData->cpuBoard, settings->comm.dnsServer, settings->comm.dhcpEnabled);

    LogDebug ("Ip Address = %s", settings->comm.analyzerIp);
    LogDebug ("Ip Mask    = %s", settings->comm.ipMask);
    LogDebug ("DNS server = %s", settings->comm.dnsServer);

#if 0
    // for DNS server configuration test
    LogDebug ("Set DNS server ip = %s", settings->comm.dnsServer);
    SetDnsServerAddress (systemData->cpuBoard, settings->comm.dnsServer)
    SetDnsServerAddress (systemData->cpuBoard, "10.1.1.20");
#endif
}

#if 0
bool
SocketDetection ()
{
    int                 status = 0;
    struct sockaddr_in  serverSocket = { 0 };
    bool                socketError = false;
    int                 clientSocket = -1;

    KillAstmProcess ();

    clientSocket = socket (AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        LogError( "socket() failed %d", WSAGetLastError());
        socketError = true;
        return socketError;
    }

    serverSocket.sin_addr.S_un.S_addr = inet_addr(settings->comm.serverIp);
    if  (serverSocket.sin_addr.S_un.S_addr == 0 || serverSocket.sin_addr.S_un.S_addr == INADDR_NONE) {
        LogError( "inet_addr() failed" );
        socketError = true;
    } else {
        serverSocket.sin_family = AF_INET;
        serverSocket.sin_port = htons( settings->comm.serverSocket );
        status = connect (clientSocket, (struct sockaddr *)&serverSocket, sizeof(serverSocket));
        if (status == -1) {
            LogError( "connect() failed" );
            socketError = true;
        } else {
            closesocket (clientSocket);
            LogDebug ("Socket connect success!");
        }
    }
    return socketError;
}
#endif

// if the ASTM process is running, kill it and return 0
// otherwise, return 1
int
KillAstmProcess (void)
{
//    int result;
//    PROCESS_INFORMATION     proc_info;
//    wchar_t                 processWSTR[256];
    DWORD                   exitCode;

    // first kill ASTM process
    if ((settings->comm.commInterface == COMM_ASTM) && (systemData->commPid != -1)) {
        HANDLE process = OpenProcess(0,FALSE, systemData->commPid);
        if (process != NULL) {
            GetExitCodeProcess(process, &exitCode);
            TerminateProcess(process, exitCode);
            CloseHandle(process);
        }
        systemData->commPid = -1;
        return 0;
    }
    LogDebug ("ASTM process is not running, ignore it");
    return 1;
}

/*
 *  in the calbe is plug in, the ASTM process needs to be restarted
 *  to make sure connection between client and server is recovered
 */
void
RestartAstmProcess (void)
{

    int result;
    PROCESS_INFORMATION     proc_info;
    wchar_t                 processWSTR[256];
//    DWORD                   exitCode;

    // active interface is ASTM and protocol is TCP/IP and not at networking setting status
    if ((settings->comm.commInterface == COMM_ASTM) && 
         (settings->comm.protocol == COMM_TCPIP) &&
        (systemData->networkSetting == false)) {
        // only current ASTM process is running, restart it
        // in the case of ASTM process is not running (in the networks setting mode),
        // don't restart this process to prevent confilct with j2Settings
        if (KillAstmProcess () == 0) {
            LogDebug ("restart ASTM process");
            //create a new ASTM process
            wsprintf(processWSTR, L"%S", ASTM_PROGRAM);
            result = CreateProcess (processWSTR, L"", NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proc_info);
            systemData->commPid = proc_info.dwProcessId;
            if (result == 0) {
                LogError( "failed to restart ASTM" );
                systemData->commPid = -1;
            }
        }
    }
    return;
}


/*
 * for debuging message only
 */
void
SendDebugMegToGui (wchar_t  *debugMsg)
{
    GuiReceiveMsg_t         msg;

    msg.msgId = MSG_GUI_MIDDLE_HOME_READY;
    wcscpy( msg.data.middleHomeReady.buttonText[0], debugMsg);
    SendGuiMessage( &msg, sizeof( GuiMiddleMessageData_t ) );
}

/*
 * thread for network status detection
 * it shall be trigged :
 * 1. Etherent cable is plugged in
 * 2. telnet or ssh is connected
 */
void *
NetworkDetectionThread (void * arg)
{
    HANDLE                  hEvent;
    static HANDLE           hNotify = 0;
    SYSTEMTIME              curSysTime;
    SYSTEMTIME              endSysTime;
    CE_NOTIFICATION_TRIGGER trigger;
    int                     i = 0;

    Sleep (20000);

    // set trgger parameters
    trigger.dwSize = sizeof(CE_NOTIFICATION_TRIGGER);

    // System event notification
    trigger.dwType = CNT_EVENT;

    // The trigger device event 
      trigger.dwEvent = NOTIFICATION_EVENT_NET_CONNECT ;
    //trigger.dwEvent = NOTIFICATION_EVENT_NET_DISCONNECT;
    //trigger.dwEvent = NOTIFICATION_EVENT_DEVICE_CHANGE;

    // this will be a named event
    trigger.lpszApplication = _T("\\\\.\\Notifications\\NamedEvents\\DeviceEvent\0");
    trigger.lpszArguments = NULL;

    // set start and end of notification period
    GetSystemTime (&curSysTime);
    endSysTime = curSysTime;
    endSysTime.wYear = endSysTime.wYear + 20;
    trigger.stStartTime = curSysTime;
    trigger.stEndTime = endSysTime;
    // create user notification
    hNotify = CeSetUserNotificationEx (hNotify, &trigger, NULL);
    // create notification event
    hEvent = CreateEvent (NULL, FALSE, FALSE, _T("DeviceEvent"));

     while (1) {
        // if named event was created
        if (hEvent) {
            // if user notification was created
            if(hNotify ) {
                // wait for my event
                if( WaitForSingleObject (hEvent, INFINITE) != WAIT_OBJECT_0 ) {
                    LogDebug ("Unknown Event signaled");
                } else {
                    // my event was triggered
                    LogDebug ("Network event signaled %d", ++i);
#ifdef NETWORK_DETECTION_DEBUG
                    wchar_t  buff [128];
                    memset (buff, 0, sizeof (buff));
                    wsprintf (buff,L"%s %d", L"Event", i);
                    SendDebugMegToGui (buff);
#endif
                    // Ethernet cable plugged, restart ASTM process
                    RestartAstmProcess ();
                }
            } else {
                LogDebug ("CeSetUserNotificationEx failed");
                break;
            }
        } else {
            LogDebug ("CreateEvent failed");
            break;
        }
        // debounce  
        Sleep (1000);
        ResetEvent (hEvent);
        
    }
    // clear use notifiy, should never be here
    if (hNotify) {
        CeClearUserNotification (hNotify);
    }
    return 0;
}
