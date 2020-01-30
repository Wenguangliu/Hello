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
#include "FactoryData.h"
#include "Shmem.h"

#define MAX_ALLOW_DATABASE_FILE_SIZE   100000
#define DATABASE_SIZE_FILE_NAME         "/Hard Disk2/DataBaseSize.txt"
#define DATABASE_FILE_NAME              L"/Hard Disk/Documents and Settings/default.vol"
#define SHUTDOWN_PROCESS                L"shutdown.exe"

#undef DATABASE_DEBUG

CEOID DBqueue;
WORD  QueueRecs = 0;
CEOID DBevents;
WORD  EventRecs = 0;



/*
 * convert wchar_t string to char string
 */
char *
WstrToCstr (const wchar_t *wStr)
{
    //max convert length is 512 bytes
    static char cStr [512];

    wcstombs (cStr, wStr, 512);

    return cStr;
}



// Detect notification to ce database
// Debug only

void
OutputNotifications (void)
{
    HANDLE                      *hNotifications = NULL;
    DWORD                       NumNotifications;
#ifdef DATABASE_DEBUG
    DWORD                       Count;
    DWORD                       BytesNeeded;
    CE_NOTIFICATION_INFO_HEADER *NIHeader;
    CE_NOTIFICATION_TRIGGER*    Trigger;
    CE_USER_NOTIFICATION*       UserNotification;
    void                        *pBuffer;
#endif

    // First call CeGetUserNotifications to find out how many notifications are available
    // by passing in zero as the second argument
    CeGetUserNotificationHandles (hNotifications, 0, &NumNotifications);
    LogDebug("Num Notifications available %d", NumNotifications);
#ifdef DATABASE_DEBUG
    hNotifications = (HANDLE *)malloc (NumNotifications * sizeof( HANDLE *));
    // Now get the notifications
    CeGetUserNotificationHandles (hNotifications, NumNotifications, &NumNotifications);
    for (Count = 0; Count < NumNotifications; Count++) {
        // Now with a valid handle to a notification find out how many bytes are needed
        // to get the data
        CeGetUserNotification (hNotifications[Count], 0, &BytesNeeded, NULL);
        LogDebug("Notifiy %d,  Bytes needed %d", Count, BytesNeeded);
        pBuffer = malloc (BytesNeeded);
        // Error checking left for  you, but now we can get the data
        CeGetUserNotification (hNotifications[Count], BytesNeeded, &BytesNeeded, (BYTE *)pBuffer);
        NIHeader = (CE_NOTIFICATION_INFO_HEADER *)pBuffer;
        Trigger = NIHeader->pcent;
        UserNotification = NIHeader->pceun;
        LogDebug ("Status %d", NIHeader->dwStatus);
        LogDebug("Trigger Type %d", Trigger->dwType);
        switch (Trigger->dwType) {
            case CNT_EVENT:
                LogDebug ("Event");
            break;
            case CNT_TIME:
                LogDebug ("Time");
            break;
            case CNT_PERIOD:
                LogDebug ("Period");
            break;
            case CNT_CLASSICTIME:
                LogDebug ("Classic Time");
            break;
            default:
                LogDebug ("Unkonw type");
            break;
         }

         LogDebug ("Trigger Event %d", Trigger->dwEvent);
                 switch (Trigger->dwEvent) {
            case NOTIFICATION_EVENT_NONE:
                LogDebug("None");
            break;
            case NOTIFICATION_EVENT_TIME_CHANGE:
                LogDebug("Time Change");
            break;
            case NOTIFICATION_EVENT_SYNC_END:
                LogDebug("Sync End");
            break;
            case NOTIFICATION_EVENT_ON_AC_POWER:
                LogDebug("On AC Power");
            break;
            case NOTIFICATION_EVENT_OFF_AC_POWER:
                LogDebug ("Off AC Power");
            break;
            case NOTIFICATION_EVENT_NET_CONNECT:
                LogDebug ("Net Connect");
            break;
            case NOTIFICATION_EVENT_NET_DISCONNECT:
                LogDebug ("Net Disconnect");
            break;
            case NOTIFICATION_EVENT_DEVICE_CHANGE:
                LogDebug ("Device Change");
            break;
            case NOTIFICATION_EVENT_IR_DISCOVERED:
                LogDebug ("IR Discovered");
            break;
            case NOTIFICATION_EVENT_RS232_DETECTED:
                LogDebug ("RS232 Detected");
            break;
            case NOTIFICATION_EVENT_RESTORE_END:
                LogDebug ("Restore End");
            break;
            case NOTIFICATION_EVENT_WAKEUP:
                LogDebug ("Wakeup");
            break;
            case NOTIFICATION_EVENT_TZ_CHANGE:
                LogDebug ("Timezone Change");
            break;
            case NOTIFICATION_EVENT_MACHINE_NAME_CHANGE:
                LogDebug ("Machine Name Change");
            break;
            default:
                LogDebug ("Unkonw event");
            break;
         }
        LogDebug ("Trigger Application %s", WstrToCstr (Trigger->lpszApplication));
        LogDebug ("Trigger Arguments %s", Trigger->lpszArguments);
        LogDebug ("Trigger EndTime %d/%d/%d %d:%02d",   Trigger->stEndTime.wMonth,
                                                        Trigger->stEndTime.wDay,
                                                        Trigger->stEndTime.wYear,
                                                        Trigger->stEndTime.wHour,
                                                        Trigger->stEndTime.wMinute);
        LogDebug ("Trigger StartTime %d/%d/%d %d:%02d", Trigger->stStartTime.wMonth,
                                                        Trigger->stStartTime.wDay,
                                                        Trigger->stStartTime.wYear,
                                                        Trigger->stStartTime.wHour,
                                                        Trigger->stStartTime.wMinute);

        if (UserNotification != NULL) {
            LogDebug ("User Action %d", UserNotification->ActionFlags);
            if( UserNotification->ActionFlags & PUN_LED) {
                LogDebug("LED");
            }
            if (UserNotification->ActionFlags & PUN_VIBRATE) {
                LogDebug ("Vibrate");
            }
            if (UserNotification->ActionFlags & PUN_DIALOG) {
                LogDebug ("Dialog");
            }
            if (UserNotification->ActionFlags & PUN_SOUND) {
                LogDebug ("Sound");
            }
            if (UserNotification->ActionFlags & PUN_REPEAT) {
                LogDebug ("Repeat");
            }
            if (UserNotification->pwszDialogText != NULL) {
                LogDebug ("User Dialog Title %s", WstrToCstr (UserNotification->pwszDialogTitle));
                LogDebug ("User Dialog Text %s", WstrToCstr (UserNotification->pwszDialogText));
            }
            if( UserNotification->ActionFlags & PUN_SOUND) {
                LogDebug ("User Sound %s", WstrToCstr (UserNotification->pwszSound));
            }
        }
        free (pBuffer);
    } //end of for loop
    free (hNotifications);
#endif
}



//Get default.vol database file, save the file to thumb drive
int
GetDataBaseFileSize (FactoryData_t *fData, bool bootup, unsigned long maxAllowedSize)
{
    HANDLE              hSearch;
    WIN32_FIND_DATA     fileData;
    ULONGLONG           fileSize;
#ifdef USB_SAVE
    FILE *              fp;
    char                buff [256];
#endif

    hSearch = FindFirstFile (DATABASE_FILE_NAME, &fileData);

    if (hSearch != INVALID_HANDLE_VALUE) {
        fileSize = fileData.nFileSizeHigh;
        fileSize <<= 32;
        fileSize += fileData.nFileSizeLow;
        //send log and save the file size to thumb drive only during system bootup
        if (bootup == true) {
            LogDebug ("Database size = %lu", fileSize);
#ifdef USB_SAVE
            fp = fopen(DATABASE_SIZE_FILE_NAME, "ab");
            if (fp != NULL) {
                memset (buff, 0, sizeof (buff));
                if (fData != NULL) {
                    sprintf (buff, "Analyzer %5s Database Size = %llu\n", fData->instrumentSerialNumber, fileSize);
                } else {
                    sprintf (buff, "DataBase Size = %llu\n",fileSize);
                }
                fwrite (buff, 1, strlen (buff), fp);
                fclose (fp);
            }
#endif
        }
        FindClose (hSearch);
    } else {
        LogDebug ("Fail to find Database file %S", DATABASE_FILE_NAME);
        return -1;
    }
    if (fileSize > maxAllowedSize) {
        LogDebug ("maxAllowedSize = %lu", maxAllowedSize);
        LogDebug ("Databbase size %lu is larger than maxAllowedSize, need to optimized database", fileSize);
        return 1;
    }

    return 0;
}

// List DBs and chech access to "DBevents"
void
CheckForDBnotify()
{
    HANDLE      FoundDB;
    CEOID       CurrentOID;
    CEOID       ceoidRet=0;
    CEOIDINFO   OIDInfo;

    CString sQueue  =_T ("DB_notify_queue");
    CString sEvents =_T ("DB_notify_events");

    // search all database and get valid database handle
    FoundDB         = CeFindFirstDatabaseEx (NULL, 0);

    if(FoundDB == INVALID_HANDLE_VALUE) {
        LogDebug ("No Databases found");
    } else {
      //  LogDebug ("DataBase found");
        while (CurrentOID = CeFindNextDatabaseEx (FoundDB, 0)) {
            // get dababase information
            if (CeOidGetInfoEx (NULL, CurrentOID, &OIDInfo)) {
                if (OIDInfo.wObjType == OBJTYPE_DATABASE) {
#ifdef  DATABASE_DEBUG
                    LogDebug ("Database Name: %S", OIDInfo.infDatabase.szDbaseName);
                    LogDebug ("Database Size: %d",  OIDInfo.infDatabase.dwSize);
                    LogDebug ("Databse Record numbers: %d",  OIDInfo.infDatabase.wNumRecords);
#endif
                    if (sQueue.CompareNoCase (OIDInfo.infDatabase.szDbaseName) == 0) {
                        DBqueue = CurrentOID;
                        QueueRecs = OIDInfo.infDatabase.wNumRecords;
                        //ZZZ
                        //LogDebug ("QueueRecs =%d", QueueRecs);
                    } else if (sEvents.CompareNoCase (OIDInfo.infDatabase.szDbaseName) == 0) {
                        DBevents = CurrentOID;
                        EventRecs = OIDInfo.infDatabase.wNumRecords;
                        //ZZZ
                        //LogDebug ("EventRecs =%d", EventRecs);
                    }
                } else {
                    LogDebug ("Failed to get info for this database");
                }
            }
        }
        CloseHandle( FoundDB );
    }
}



// Check "DBevents" for multiple entries and delete them
void
CheckAndDeleteDBnotifymultiple (void)
{
    CEGUID      guid;
    HANDLE      hDatabase;
    WORD        wPropId = 0;
    CEPROPVAL*  Buffer = NULL;
    DWORD       dwSizeOfBuffer = 0;
    CEOID       ceoidRecord;
    DWORD       checked=0;
    DWORD       deleted = 0;
    CEPROPVAL*  BufferCmp;
    DWORD       dwSizeOfBufferCmp ;
    WORD        wPropIdCmp;
    WORD        i;
    CEOID       ceoidRecordCmp;

    CREATE_SYSTEMGUID (&guid);
    hDatabase = CeOpenDatabaseEx2 (&guid, &DBevents, NULL, NULL, CEDB_AUTOINCREMENT, NULL);

    if(hDatabase != INVALID_HANDLE_VALUE) {
        while (ceoidRecord = CeReadRecordPropsEx (hDatabase, CEDB_ALLOWREALLOC, &wPropId, /*Number of properties*/NULL,
                                                       (LPBYTE*)  &Buffer, &dwSizeOfBuffer, NULL/*hHeap*/)) {
            BufferCmp = NULL;
            dwSizeOfBufferCmp = 0;
            wPropIdCmp = 0;
            for (i = 0 ; i < (EventRecs-1-checked-deleted); i++) {
                ceoidRecordCmp = CeReadRecordPropsEx (hDatabase, CEDB_ALLOWREALLOC,
                                                     &wPropIdCmp, /*Number of properties*/NULL, (LPBYTE*)&BufferCmp, &dwSizeOfBufferCmp, /*hHeap*/NULL);
                if( ceoidRecordCmp && wcscmp(Buffer->val.lpwstr,BufferCmp->val.lpwstr) == 0 /*&& TODO: do additionalcomparison checks*/) {
#ifdef DATABASE_DEBUG
                    LogDebug ("Found duplicate: %S-%S", Buffer->val.lpwstr,BufferCmp->val.lpwstr);
#endif
                    if (CeDeleteRecord (hDatabase,  ceoidRecordCmp)) {
#ifdef DATABASE_DEBUG
                        LogDebug ("Delete duplicate entry: %S" ,BufferCmp->val.lpwstr);
#endif
                        deleted++;
                        checked--;
                        break;
                    } else {
                        //LogDebug ("Error while delete duplicate entry, error:%d",GetLastError());
                    }
                }
            }
            checked++;
            LocalFree (BufferCmp);
#if 0
            if (CeSeekDatabaseEx (hDatabase, CEDB_SEEK_BEGINNING, checked, 0, NULL)==0) {
                //LogDebug ("Error while seek db, error: %d", GetLastError());
            }
#endif
        }

        LogDebug ("%d Records deleted from DB_notify_events", deleted);
        LocalFree(Buffer);
        CloseHandle(hDatabase);
    }
}

// clear default.vol database events
// in the case of dababase too larger, clean database and reboot
void
CleanDatabaseEvents (FactoryData_t *fData, bool bootup)
{
    int                     ret;
    PROCESS_INFORMATION     proce_info;

    ret = GetDataBaseFileSize (fData, bootup, NULL);

    CheckForDBnotify ();

    if (DBevents != 0) {
        CheckAndDeleteDBnotifymultiple();
    }

    Sleep (2000);

    // only warming reboot, the event notification  could be safely removed from database.vol file
    if (ret == 1) {
        CreateProcess (SHUTDOWN_PROCESS, L"", NULL, NULL,FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proce_info);
    }
}

// force clean database and reboot
void
CleanDatabaseAndReboot ()
{
    PROCESS_INFORMATION     proce_info;

    CheckForDBnotify ();

    if (DBevents != 0) {
        CheckAndDeleteDBnotifymultiple();
    }

    Sleep (2000);
    CreateProcess (SHUTDOWN_PROCESS, L"", NULL, NULL,FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &proce_info);
}

// disable all possible event notification to prevent database.vol size increase
void
DisableEventNotifications ()
{
    if (!CeRunAppAtEvent(TEXT("repllog.exe"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the RS232 notification");
    }
    if (!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\DSTTimeChange"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the DSTTimeChange notification");
    }

    if(!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\DSTTzChange"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the DSTTzChange notification");
    }

    if(!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\DeviceEvent"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the DeviceEvent notification");
    }

    if(!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\ShellDSTEvent"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the ShellDSTEvent notification");
    }

    if(!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\DCOMSS\\RefreshSettings"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the RefreshSettings notification");
    }
    if(!CeRunAppAtEvent(_T("\\\\.\\Notifications\\NamedEvents\\SMB_NameNotifyEvent"), NOTIFICATION_EVENT_NONE)) {
        // Failed to remove the notification, let the user know
        LogDebug ("Fail to remove the SMB_NameNotifyEvent notification");
    }


}



