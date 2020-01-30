#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <time.h>
#include <errno.h>
#include <winsock2.h>
#include <winioctl.h>
#include <ntddndis.h>
#include <iptypes.h>
#include <iphlpapi.h>
#include <pnp.h>
#include <notify.h>
#include <commctrl.h>
#include <afx.h>

#include "LogErr.h"
#include "StartupGlobal.h"

#if 0
#define  MAX_ALLOW_DATABASE_FILE_SIZE   100000
//#define  MAX_ALLOW_DATABASE_FILE_SIZE   10000

#define DATABASE_SIZE_FILE_NAME         "/Hard Disk2/DataBaseSize.txt"
#define DATABASE_FILE_NAME              L"/Hard Disk/Documents and Settings/default.vol"
#define SHUTDOWN_PROCESS                L"shutdown.exe"

#undef  DATABASE_DEBUG

CEOID DBqueue;
WORD  QueueRecs = 0;
CEOID DBevents;
WORD  EventRecs = 0;
#endif


// device info structures
typedef union {
  DEVDETAIL d;
  char pad[sizeof(DEVDETAIL)+MAX_DEVCLASS_NAMELEN];
} MyDevice_t;

typedef enum {
    NO_FILTER,
    SHOW_HARDDISK_FTDI,
    SHOW_TSU_FTDI,
    SHOW_FTDI,
} FilterFlag_t;


// enumerate devices
void
EnumerateDevices (HANDLE h, DWORD timeout, char filterFlag)
{
    MyDevice_t detail;
    DWORD flags;
    DWORD size;

    //if (WaitForSingleObject (h, 5000) == WAIT_OBJECT_0) {
    if (WaitForSingleObject (h, INFINITE) == WAIT_OBJECT_0) {
        while (ReadMsgQueue(h, &detail, sizeof(detail), &size, timeout, &flags) == TRUE) {
            // filter special char drivers to prevent destory log file format
            // first char should not be '/'and start from 'A'(0x41), otherwise, the log format shall not meet the requirement
            if (((char)detail.d.szName [0] != 0x5C)  && ((char)detail.d.szName [0] != 0x24) && ((char)detail.d.szName [0] > 0x40)){
                if (filterFlag == NO_FILTER) {
                    LogDebug ("Device notification: %S %s", detail.d.szName, detail.d.fAttached ? "appeared" : "removed");
                } else if (filterFlag == SHOW_HARDDISK_FTDI) {
                    if ((wcsncmp (detail.d.szName, L"FTD", 3) == 0) || (wcsncmp (detail.d.szName, L"DSK", 3) == 0)) {
                        LogDebug ("Device notification: %S %s", detail.d.szName, detail.d.fAttached ? "appeared" : "removed");
                    }
                } else if (filterFlag == SHOW_TSU_FTDI) {
                    if ((wcsncmp (detail.d.szName, L"FTD", 3) == 0) || (wcsncmp (detail.d.szName, L"TSU", 3) == 0)) {
                        LogDebug ("Device notification: %S %s", detail.d.szName, detail.d.fAttached ? "appeared" : "removed");
                    }

                } else if (filterFlag == SHOW_FTDI) {
                    // only display FTDI drivers
                    if (wcsncmp (detail.d.szName, L"FTD", 3) == 0)  {
                        LogDebug ("Device notification: %S %s", detail.d.szName, detail.d.fAttached ? "appeared" : "removed");
                    }
                } else {
                    LogDebug ("Device notification: illegal filter flag");
                }
            }
        }
    }
}

// Device Noticfication thread
void *
DeviceNotificationHandler (void *arg)
{
    GUID guid = {0};        // or any known and relevant device interface GUID
    HANDLE                  hq, hn;
    MSGQUEUEOPTIONS         msgopts;

    msgopts.dwSize          = sizeof(MSGQUEUEOPTIONS);
    msgopts.dwMaxMessages   = 0;
    msgopts.dwFlags         = 0;
    msgopts.bReadAccess     = TRUE;
    msgopts.cbMaxMessage    = sizeof(MyDevice_t);

    hq = CreateMsgQueue (NULL, &msgopts);

    if (hq == 0) {
        LogError ("Fail to create DeviceNotification queue");
        return 0;
    }

    // TRUE, show all current device, FALSE only show current
    // appear or remove driver
    //hn = RequestDeviceNotifications (&guid, hq, TRUE);
    hn = RequestDeviceNotifications (&guid, hq, FALSE);

    while (1) {
        EnumerateDevices (hq, 1, SHOW_TSU_FTDI);
        //EnumerateDevices (hq, 1, NO_FILTER);
    }
    // should never get here
    StopDeviceNotifications (hn);
    CloseMsgQueue (hq);
    return 0;
}


// Show all current appear devices 
// When something wrong, such as Engine board unsync reset, 
// try to display FTDI and harddisk attached device drivers  
void
StreamDeviceNotification (void)
{
    GUID guid = DEVCLASS_STREAM_GUID;   //stream device interface GUID
    HANDLE hq, hn;
    MSGQUEUEOPTIONS msgopts;

    msgopts.dwSize          = sizeof(MSGQUEUEOPTIONS);
    msgopts.dwMaxMessages   = 0;
    msgopts.dwFlags         = 0;
    msgopts.bReadAccess     = TRUE;
    msgopts.cbMaxMessage    = sizeof(MyDevice_t);

    hq = CreateMsgQueue (NULL, &msgopts);

    if (hq == 0) {
        LogError ("Fail to create DeviceNotification queue");
        return;
    }

    LogDebug ("Stream Device Notification");
    // TRUE, show all current device, FALSE only show current
    // appear or remove driver
    hn = RequestDeviceNotifications(&guid, hq, TRUE);

    // during analyzer, this function is call to only show how many FTDI drivers are attached
    EnumerateDevices (hq, 1, SHOW_HARDDISK_FTDI);
    // should never get here
    StopDeviceNotifications (hn);
    CloseMsgQueue (hq);
}


