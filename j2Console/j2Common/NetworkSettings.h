#ifndef _NETWORKSETTINGS_H_
#define _NETWORKSETTINGS_H_ 
#include "SettingsData.h"



#define                 AAEON_CPU_BOARD         'A'
#define                 ADLINK_CPU_BOARD        'D'
#define                 ADLINK_BT_CPU_BOARD     'B'
#define                 UNKNOWN_CPU_BOARD       'N'

typedef struct 
{
    char                analyzerIp          [30];
    char                gateway             [30];
    char                ipMask              [30];
    char                serverIp            [30];
    char                macAddress          [30];   
    char                hostName            [128];
    char                adapterName         [128];
    unsigned char       dhcpEnabled;
    char                cpuBoard;
} EthernetPortInfo_t;

extern int
SetTelentFtp (DWORD enable);

extern int 
SetDhcp (char boardId, unsigned char DhcpEnable, char *hostName);

extern  int 
SetStaticIp (char boardId, SettingsData_t * settings);

extern int 
SetHostName (char *hostName);

extern void
GetEthernetPortInfo (EthernetPortInfo_t *ethernetPortInfo);

extern int
GetDnsServerAddress (char boardId, char *DnsSeverIp, char dhcpEnalbe);

extern int
SetDnsServerAddress (char boardId, char *DnsSeverIp);
#endif
