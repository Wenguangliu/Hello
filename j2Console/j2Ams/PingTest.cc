#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include <Icmpapi.h>
#include "LogErr.h"
#include "CommonDefines.h"
#include "PingTest.h"
#include "Global.h"


/* predefined ping Ip Address */
char    *analyzerIp  = "10.1.7.201";
char    *gateway  = "10.1.1.1";
char    *ipMask = "255.55.248.0";

// Read Ip Address From IpSetAddress file
int
ReadIpAddress (char *ipStr)
{
    FILE    *fp;
    char    *ptr;

    fp = fopen (IP_ADDRESS_FILE_NAME, "rb");

    if (fp != 0) {
        memset (ipStr,0, sizeof (ipStr));
        fgets (ipStr, MAX_IP_STRING_LENGTH, fp);  
        fclose (fp);
        // clear '\n'
        ptr = ipStr;
        while (*ptr != '\0') {
            if (*ptr == '\n') {
                *ptr = '\0';
                break;
            }
            ptr++;
        }

    } else {
        LogError ("AMS Fail to find Ip Address");
        return -1;
    }

    return 0;
    
}

// set AMS port test message
void
SetNetPortMessage (unsigned char pingTimes, netPortTestMsg_t *netPortTestMsgPrt, unsigned char passFail, unsigned long time,  unsigned short status)
{
    
    netPortTestMsgPrt->pingReport [pingTimes].pingPass     = passFail; 
    netPortTestMsgPrt->pingReport [pingTimes].pingStatus   = status; 
    netPortTestMsgPrt->pingReport [pingTimes].pingTime     = (unsigned short)time; 
}

//copy predefined AMS net settings to structures
void
GetPredefSettings (netSettings_t *netSettings)
{
    memset (netSettings, 0, sizeof (netSettings_t));
    strcpy (netSettings->analyzerIp, analyzerIp);
    strcpy (netSettings->gateway, gateway);
    strcpy (netSettings->ipMask, ipMask);
}

//copy settings.xml net settings to structures
void
GetNetSettings (netSettings_t *netSettings)
{
    memset (netSettings, 0, sizeof (netSettings_t));
    strcpy (netSettings->analyzerIp, Settings.comm.analyzerIp);
    strcpy (netSettings->gateway, Settings.comm.gateway);
    strcpy (netSettings->ipMask, Settings.comm.ipMask);
}

// ping itself and fill up AMS test message structure
int
Ping (unsigned char pingTimes, netPortTestMsg_t *netPortTestMsgPrt)
{

    HANDLE                  hIcmpFile;
    unsigned long           ipaddr;
    DWORD                   dwRetVal;
    struct in_addr          replyAddr;
    PICMP_ECHO_REPLY        pEchoReply;
    char SendData [32]      = "Abaxis Analyzer";
    char                    replyBuf [sizeof (ICMP_ECHO_REPLY) + sizeof (SendData)];
    char                    iPstr [MAX_IP_STRING_LENGTH];
    unsigned char           i;
    netSettings_t           netSettings;

    memset (netPortTestMsgPrt, 0, sizeof (netPortTestMsg_t));

    /* get predefined Ip address */
    GetPredefSettings (&netSettings);
    /* Set Ethernet Port to have predefined Ip address for ping test */
    SetEthernet (netSettings);
    /* wait 5 second to let ethernet port up */
    Sleep (5000);

    /* convert to network ipaddress */
    ipaddr = inet_addr (netSettings.analyzerIp);

    if (ipaddr == INADDR_NONE) {
        for (i = 0; i < pingTimes;  i++) {
            SetNetPortMessage (i, netPortTestMsgPrt, 0x02, 0, PING_INVALID_IP_ADDRESS);
        }
        LogError ("AMS Invalid Ip Address");
        return -2;
    }
    netPortTestMsgPrt->ipAddress = ipaddr;

    hIcmpFile = IcmpCreateFile ();

    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        for (i = 0; i < pingTimes;  i++) {
            SetNetPortMessage (i, netPortTestMsgPrt, 0x02, 0, PING_FAIL_OPEN_HANDLE);
        }

        LogError ("AMS fail to open net port handle error: %ld", GetLastError()); 
        return -3;
    }

    for (i = 0; i < pingTimes; i++) {
        dwRetVal = IcmpSendEcho (hIcmpFile, ipaddr, SendData, sizeof(SendData),
                                 NULL, replyBuf, sizeof (replyBuf), 1000);
        if (dwRetVal != 0) {
            pEchoReply = (PICMP_ECHO_REPLY)replyBuf;
            replyAddr.S_un.S_addr = pEchoReply->Address;
            if (pEchoReply->RoundTripTime >= 1000) {
                SetNetPortMessage (i, netPortTestMsgPrt, 0x02, pEchoReply->RoundTripTime, PING_TIME_OUT);
                LogDebug ("AMS ping timeout %d",  pEchoReply->RoundTripTime);
            } else if (strcmp ((char *)pEchoReply->Data, SendData) != 0) {
                SetNetPortMessage (i, netPortTestMsgPrt, 0x02, pEchoReply->RoundTripTime, PING_MSG_ERROR);
                LogDebug ("AMS ping message error %s",  pEchoReply->Data);
            } else {
                SetNetPortMessage (i, netPortTestMsgPrt, 0x01, pEchoReply->RoundTripTime, PING_SUCCESS);
                LogDebug ("Ping received time=%ld,status=%lx, %s",   
                       pEchoReply->RoundTripTime, pEchoReply->Status, pEchoReply->Data);
            }
        } else {
            SetNetPortMessage (i, netPortTestMsgPrt, 0x02, 0, PING_SEND_ECHO_FAIL);
            LogError ("IcmpSendEcho returned error: %ld", GetLastError());
        }
    }
    LogDebug ("total received %d pings", i);
    IcmpCloseHandle (hIcmpFile);

    /* after testing, recover the settings.xml net settings */
    GetNetSettings (&netSettings);
    SetEthernet (netSettings);

    return 0;
}
