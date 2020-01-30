#include <windows.h>
#include <tlhelp32.h>

#include "LogErr.h"
#include "KillProcessByName.h"


// get pid by process name
DWORD
GetProcId (TCHAR *procname)
{
    HANDLE proctable;
    PROCESSENTRY32 pe;
    BOOL success;
    proctable = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
    if (proctable == INVALID_HANDLE_VALUE) {
        LogError ("CreateToolhelp32Snapshot fail");
        return -1;
    }

    pe.dwSize = sizeof (PROCESSENTRY32);
    success=Process32First (proctable, &pe);
    while (success){
        if (_tcsicmp(pe.szExeFile, procname) == 0) {
            LogDebug("Found %S: %d", pe.szExeFile,  pe.th32ProcessID);
            return(pe.th32ProcessID);
        }
        success= Process32Next(proctable, &pe);
    }
    return -1;
}

// kill process by pid
int
KillProcId(DWORD id)
{
    HANDLE process;

    process = OpenProcess (0,0,id);
    if (process == INVALID_HANDLE_VALUE) {
        LogError ("OpenProcess Failed");
        return -1;
    }

    if (!TerminateProcess (process, 999)) {
        LogError ("Kill Process Fail");
        return -1;
    }
    return 0;
}

// function to kill procss by name
int
KillProcessByName (char *processName)
{
    wchar_t ProcessName [128];
    DWORD p_pid = 0;

    wsprintf (ProcessName, L"%S", processName);
    p_pid = GetProcId (ProcessName);

    return (KillProcId (p_pid));
    
}

