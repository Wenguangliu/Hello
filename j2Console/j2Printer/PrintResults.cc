#include <windows.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include "PrinterComm.h"
#include "prnport.h"


#include "ResultsDb.h"
#include "Shmem.h"
#include "LogErr.h"
#include "Species.h"
#include "Analytes.h"
#include "Units.h"
#include "ParseCounters.h"
#include "SystemError.h"
#include "FormatResults.h"
#include "PrintResults.h"
#include "PrinterMessages.h"


#define strupr _strupr


#define BILLION             1000000000LL;


HANDLE              ResultsPrint::fdSer = INVALID_HANDLE_VALUE;
HANDLE              ResultsPrint::fdUsb = INVALID_HANDLE_VALUE;

ULONGLONG           ResultsPrint::lastReportTime = 0;
AStrings            ResultsPrint::strings;
Analytes            ResultsPrint::analytes;
Units               ResultsPrint::units;
Species             ResultsPrint::species;
HANDLE              ResultsPrint::printThread = 0;
char                ResultsPrint::printInIndex = 0;
char                ResultsPrint::printOutIndex = 0;
char          *     ResultsPrint::printBuff [64];
char                ResultsPrint::externalPrinterBuff [EXTERNAL_PRINTER_BUFF_SIZE];
HANDLE              ResultsPrint::hPrintState;
printerTest_t       ResultsPrint::printerTest = NO_PRINTER_TEST;


CounterData_t *     ResultsPrint::counterData   = (CounterData_t *)MapSharedMem( COUNTERS_SHMEM_NAME, sizeof( CounterData_t ) );
FactoryData_t *     ResultsPrint::factoryData   = (FactoryData_t *)MapSharedMem( FACTORY_SHMEM_NAME, sizeof( FactoryData_t ) );
SettingsData_t *    ResultsPrint::settings      = (SettingsData_t *)MapSharedMem( SETTINGS_SHMEM_NAME, sizeof( SettingsData_t ) );
SystemData_t *      ResultsPrint::systemData    = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
RefRangeData_t *    ResultsPrint::refRanges     = (RefRangeData_t *)MapSharedMem( REF_RANGES_SHMEM_NAME, sizeof( RefRangeData_t ) );
VersionData_t *     ResultsPrint::versionData   = (VersionData_t *)MapSharedMem( VERSION_SHMEM_NAME, sizeof (VersionData_t) );

void    Receiver(void);

int
main (void)
{

   ResultsPrint::Receiver ();

   return 0;
}


// receive main thread to handle message  
void
ResultsPrint::
Receiver (void)
{
    DWORD           receivedSize = 0;
    PrinterMsg_t    msg;
    MSGQUEUEOPTIONS options;
    HANDLE          queue;
    HANDLE          respQueue;
    BOOL            result;
    DWORD           flags;
    SystemErrNum_t  errNum;


    fdUsb   = INVALID_HANDLE_VALUE;
    fdSer   = INVALID_HANDLE_VALUE;
    
    if (PrinterInitial (INTERNAL_PRINTER) == -1) {
        exit (-1);
    }

    /*
     * Open the Analysis input queue and get its size.
     */
    options.dwSize = sizeof (MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = TRUE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;

    queue = CreateMsgQueue (PRINTER_QUEUE_NAME, &options);
    if (queue == NULL) {
        LogError( "failure to open printer message queue, errno: %d", GetLastError ());
        exit (-1);
    }
    /*
     * Open the Analysis input queue and get its size.
     */
    options.dwSize = sizeof (MSGQUEUEOPTIONS);
    options.dwFlags = MSGQUEUE_ALLOW_BROKEN;
    options.bReadAccess = FALSE;
    options.dwMaxMessages = 0;
    options.cbMaxMessage = MAX_QUEUE_LENGTH;

    respQueue = CreateMsgQueue (PRINTER_RESP_QUEUE_NAME, &options);
    if (respQueue == NULL) {
        LogError( "failure to open printer response message queue, errno: %d", GetLastError ());
        exit (-1);
    }

    // create event
    hPrintState = CreateEvent (NULL, FALSE, FALSE, L"PrintState");
    if (hPrintState == NULL) {
        LogError ("Fail to create print event %d", GetLastError());
        exit (-1);
    }
    // create print thread
    printThread =  CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)PrintThread, NULL, 0, NULL);
    if (printThread == NULL) {
        LogError ("Fail to create print thread %d", GetLastError());
        exit (-1);
    }

    /*
     * Receive messages and handle state changes.
     */

    while (1) {
     //   result = ReadMsgQueue(queue, &msg, sizeof(ConsoleMsg_t), &receivedSize, INFINITE, &flags);
        result = ReadMsgQueue(queue, &msg, sizeof(PrinterMsg_t), &receivedSize, INFINITE, &flags);
        if (result != FALSE)  {
            if (receivedSize > 0) {
                errNum = SERR_NONE;
                switch (msg.msgId) {
                    case MSG_PRINT_RESULTS_BY_INDEX:
                        LogDebug ("Print by index");
                        errNum = PrintResult (msg.selectedItem);
                    break;

                    case MSG_PRINT_IQC_BY_INDEX:
                        LogDebug ("Print Qc");
                        errNum = PrintIqc (msg.selectedItem);
                    break;

                    case MSG_PRINT_ERROR_REPORT_BY_INDEX:
                        LogDebug ("Print Error by Index");
                        errNum = PrintErrorReport (msg.selectedItem, msg.abbreviated);
                    break;

                    case MSG_PRINT_REFERANCE:
                        LogDebug ("Print reference");
                        errNum = PrintRefRanges (msg.sampleType);
                    break;

                    case MSG_PRINT_INTERNAL_PRINTER_TEST:
                        LogDebug ("Print internal test");
                        errNum = TestInternalPrinter ();
                    break;

                    case MSG_PRINT_EXTERNAL_PRINTER_TEST:
                        LogDebug ("Print External");
                        errNum = TestExternalPrinter ();
                    break;

                    case MSG_PRINT_INTERNAL_PRINTER_DEFAULT:
                        LogDebug ("Set internal printer default");
                        PrinterInitial (INTERNAL_PRINTER);
                    break;

                    case MSG_PRINT_EXTERNAL_PRINTER_DEFAULT:
                        LogDebug ("Set external printer default");
                        PrinterInitial (EXTERNAL_PRINTER);
                    break;

                    case MSG_PRINT_CUVETTE_TEST_RESULTS:
                        LogDebug ("Print Cuvette Test Results");
                        PrintCuvetteTestResults ();
                    break;

                    case MSG_PRINT_GET_PRINTER_ID:
                        GetPrinterId ();
                    break;

                    default:
                    break;
                }
                msg.msgId = MSG_PRINT_RESPONSE;
                msg.errNumber = errNum;
                if (respQueue != NULL) {
                    if (errNum != SERR_NONE) {
                        LogDebug ("Printer response fail number 0x%x", errNum);
                    }
                    WriteMsgQueue(respQueue, &msg, sizeof (PrinterMsg_t) + sizeof (int), INFINITE, NULL);
                }
            }
        }
    }
}
