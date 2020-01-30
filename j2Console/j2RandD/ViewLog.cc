#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msgqueue.h>
#include <errno.h>


#include "ManufacturingStates.h"
#include "GuiMessages.h"
#include "MfMessages.h"
#include "ViewLog.h"
#include "LogErr.h"

int             StateViewLog::pageNumber = 0; 
int             StateViewLog::totalPages = 0;
int             StateViewLog::totallReadLen = 0; 
HANDLE          StateViewLog::logSemHandle = NULL;
char	        StateViewLog::logMsg [PAGE_SIZE];
int             StateViewLog::fileSize;
int             StateViewLog::lastPgLen;

// send log file to Gui 
void
StateViewLog::
SendLogMessage (void)
{
    GuiReceiveMsg_t     msg;
    FILE                *fp;
    int                 len;
    wchar_t             wBuff [PAGE_SIZE];

    // clear buffer
    memset (logMsg, 0, sizeof (logMsg));

    // wait for semaphore to access log file
    WaitForSingleObject (logSemHandle, INFINITE);

    fp = fopen (VIEW_LOG_FILE_NAME, "rb");
    if (fp == NULL) {
        LogError ("Fail to open %s", VIEW_LOG_FILE_NAME);
        wcscpy (wBuff, L"Could not be able to open Log.current file");
        len     = sizeof (logMsg);
    } else {
        //set file read point
        if (fseek (fp, totallReadLen, SEEK_SET) == 0) {
            len = fread (logMsg, 1, PAGE_SIZE, fp);
            totallReadLen += len;
            if (totallReadLen >= fileSize) {
                // records last page length
                lastPgLen = len;
            }
        } else {
            LogError ("Fail to set %s postion %d", VIEW_LOG_FILE_NAME, totallReadLen);
        }
        fclose (fp);
    }
    ReleaseSemaphore (logSemHandle, 1, NULL);
    memset (msg.data.middleViewResults.text, 0, sizeof (msg.data.middleViewResults.text));
    mbstowcs (wBuff, logMsg, len);
    wcsncpy (msg.data.middleViewResults.text, wBuff, len);
    //wcsncpy (msg.data.middleViewResults.text, logMsg, len);
    /* send message to Gui */
    msg.msgId = MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
    SendGuiMessage(&msg, sizeof(GuiMiddleViewResultsData_t));
}

// display log file enter
void
StateViewLog::
Enter (void)
{
    wchar_t              topLabel [128];
    
    // create log file semaphore
    if (logSemHandle == NULL) {
        logSemHandle = CreateSemaphore (NULL, 0, 1, LogSem);
        if (logSemHandle == NULL) {
            LogError ("Fail to crate view log semaphore");
            return;

        }
    }

    totallReadLen = 0;

    // wait for semaphore to access log file
    WaitForSingleObject (logSemHandle, INFINITE);

    // get log file size and calcuating page numbers
    fileSize = GetFileSize (VIEW_LOG_FILE_NAME);
    if (fileSize != 0) {
        lastPgLen = fileSize % PAGE_SIZE;
        totalPages = (fileSize / PAGE_SIZE) + 1;
        pageNumber = totalPages;
        wsprintf (topLabel,L"%s      %d/%d", L"View Log File", pageNumber, totalPages);
        totallReadLen = (fileSize - lastPgLen);
    } else {
        wsprintf (topLabel,L"%s      %d/%d", L"View Log File", 1, 1);
        LogError ("Fail to find %s file", VIEW_LOG_FILE_NAME);
    }
    // release semaphore
    ReleaseSemaphore (logSemHandle, 1, NULL);

    SetTopLabel (topLabel);
    SendLogMessage ();
    
    SetBottomButtons (strings [BSTR_BACK], false, L"PgUp", true, L"PgDn", true); 
}

state_t
StateViewLog::
ProcessMessage (int msgId, char *data)
{
    int                 retVal = STAET_VIEW_LOG_FILE;
    int                 idata = ((GuiSendMsgData_t *)data)->idata;
    wchar_t             topLabel [128];

    switch (msgId) {
        case MSG_GUI_OUT_BOTTOM_BUTTON:
            switch (idata) {
                case 0:     // Back
                    retVal =  stateStack.Pop();
                break;

                case 1:     //page up 
                    if (totallReadLen >= 0) {
                        // set current page first line
                        if (lastPgLen > 0) {
                            if (totallReadLen > lastPgLen) {
                                totallReadLen -= lastPgLen;
                            } else {
                                totallReadLen = 0;
                            }
                            lastPgLen = 0;
                        } else {
                            if (totallReadLen > PAGE_SIZE) {
                                totallReadLen -= PAGE_SIZE;
                            } else {
                                totallReadLen = 0;
                            }
                        }
                        /* move back one page */
                        if (totallReadLen > PAGE_SIZE) {
                            totallReadLen -= PAGE_SIZE;
                        } else {
                            totallReadLen = 0;
                        }
                        if (pageNumber > 1) {
                            pageNumber--;
                        }
                        wsprintf (topLabel,L"%s      %d/%d", L"View Log File", pageNumber, totalPages);
                        SetTopLabel (topLabel);
                        SendLogMessage (); 
                    }
                                        
                break;

                case 2:     //page down 
                    if (totallReadLen < fileSize) {
                        if (pageNumber < totalPages) {
                            pageNumber++;
                        }
                        wsprintf (topLabel,L"%s      %d/%d", L"View Log File", pageNumber, totalPages);
                        SetTopLabel (topLabel);
                        SendLogMessage ();
                    }
                break;

                default: 
                break;
            }
        break;

        default:
            ProcessOtherMessages (msgId, data);
       break;
    }
    return retVal;
}
