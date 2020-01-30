#include <windows.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <atlbase.h>
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



#define BILLION                         1000000000LL;
#define EXTERNAL_PRINTER_DELAY_TIME     500                     //delay time (ms) 

#undef  TEST_STRING

const char *        endogenousStrings[] = { "0", "1+", "2+", "3+" };
const float         hemLimits[] = { 20.0, 45.0, 181.0 };
const float         lipLimits[] = { 150.0, 221.0, 301.0 };
const float         ictLimits[] = { 2.8, 6.0, 12.1 };

const char *        wavelengthStrings[] = { "340", "405", "467", "500", "515", "550", "600", "630" };

static            unsigned int   setMemIndex = 0;
static            unsigned int   freeMemIndex = 0;

#define           HARDWARE_RESET


// function to detect printer initialization errors
void
DetectPrinterError (DWORD error)
{
    DWORD  initError = 0;
    DWORD  status;

    status = (error & 0xFFFFFF0F);

    initError = (error & 0xF0);

    LogDebug ("Printer Status = %x", status);

    switch (initError) {
        case PRN_CURRENT_ERR:
            LogError ("Priniter Initial Crrrent Fail");
        break;

        case PRN_SPEED_ERR:
            LogError ("Priniter Initial Speed Fail");
        break;

        case PRN_INTERSITY_ERR:
            LogError ("Priniter Initial Intensity Fail");
        break;

        case PRN_FONT_ERR:
            LogError ("Priniter Initial Font Fail");
        break;

        case PRN_HEAT_ERR:
            LogError ("Priniter Initial Heat Fail");
        break;


        case PRN_LINE_SPACE_ERR:
            LogError ("Priniter Initial Line Space  Fail");
        break;

        case PRN_LETTER_SPACE_ERR:
            LogError ("Priniter Initial Letter Space  Fail");
        break;

        default:
            LogError ("Priniter Initial unknow %x Fail", initError);
        break;

    }
}


// clear APS printer buffer after hardware reset 
void
ResultsPrint::
ClearPrinterBuffer (int repeatTimes)
{
    int     i;
    char    buf [128];
    
    memset (buf, 0, sizeof (buf));

    // special 'CAN' 48 timescommand to clear printer buff  
    memset (buf, 0x18, 48);
    for (i =0; i <repeatTimes; i++) {
         WriteSer (buf, strlen (buf));
    }
}


// get printer id and printer one line *
void
ResultsPrint::
GetPrinterId (void)
{
    char            buf [256];
#ifdef TEST_STRING
    char            *testStr = "test aps printer";
#endif

    // clear printer buffer
    ClearPrinterBuffer (2);
    memset (buf, 0, sizeof (buf));
    // get printer id
    GetPrinterFirmWareVersion ((PBYTE)buf);
    strcpy (versionData->apsVersion, &buf[17]);
    LogDebug ("APS printer Id=%s",buf);
    // clear printer buffer again
    ClearPrinterBuffer (1);
#ifdef TEST_STRING
    // print one line *
    memset (buf, 0, sizeof (buf));
//    memset (buf, '*', 30);
    strcpy (buf, testStr);
    WriteSer (buf, strlen (testStr));
    memset (buf, 0, sizeof (buf));
    // paper roll 3 line up
    memset (buf, '\n', 3);
    WriteSer (buf, 30);
#endif
}




// initial interanal printer during power on
int
ResultsPrint::
PrinterInitial (printerType_t printerType)
{
    DWORD   ret;
    char    reTry = 0;
    
    if (printerType == INTERNAL_PRINTER) {
        // close external printer port
        if (fdUsb != INVALID_HANDLE_VALUE) {
            PrinterClose (fdUsb);
            fdUsb = INVALID_HANDLE_VALUE;
        }
        if (fdSer != INVALID_HANDLE_VALUE) {
            PrinterClose (fdSer);
            fdSer = INVALID_HANDLE_VALUE;
        }
        /* intial internal printer */
        fdSer = ApsPrinterPortOpen ();

        if (fdSer == INVALID_HANDLE_VALUE) {
            /* needs to be changed as  logError */
            LogError ("Fail to open internal APS printer");
            return -1;
        }
        /* initial Aps printer parameters */
        if ((ret = InitialApsPrinter (fdSer)) != ERROR_SUCCESS) {
            /* re-try 4 times */
            while (reTry++ < 4) { 
                LogError ("Internal printer initial fail:%lx", GetApsPrinterStatus (fdSer));
                //DetectPrinterError (ret);
                LogError ("Reset printer");
                /* reset printer here */
                ResetPrinter (fdSer);
                /* wait for  printer re-boot */
                Sleep (5000);
                LogError ("Re-open printer port");
                fdSer = ApsPrinterPortOpen ();
                if (fdSer == INVALID_HANDLE_VALUE) {
                    LogError ("First time re-open printer port fail");
                    // wait for another 3 seconds to re-open again
                    Sleep (3000);
                    fdSer = ApsPrinterPortOpen ();
                    if (fdSer == INVALID_HANDLE_VALUE) {
                        LogError ("Second time re-open printer port fail");
                        LogError ("Printer Initial Fail");
                    } 
                } 
                LogError ("Re-Initial printer");
                /* wait re-initial printer */
                if ((ret = InitialApsPrinter (fdSer)) != ERROR_SUCCESS) {
                     LogError ("Re-initial port fail");
                } else {
                    /* APS printer initial success */
                    LogError ("Re initial printer done");
                    break;
                }
            }
            if (reTry == 4) {
                LogError ("Printer Initial Fail");
            }
        } else {
            /* APS printer initial success */
            LogError ("Internal printer initial done");
        }
        // re-initial printer success, needs to clear printer buffer
        if (reTry < 4) {
            // get printer Id and print one line *
            GetPrinterId ();
        } else {
            // fail to initial APS printer
            return -1;
        } 
    
    } else {
     //   LogDebug ("Initial External printer");
        // close internal printer port
        if (fdSer != INVALID_HANDLE_VALUE) {
            PrinterClose (fdSer);
            fdSer = INVALID_HANDLE_VALUE;
        }
        // Open the printer port.
        if (fdUsb != INVALID_HANDLE_VALUE) {
            // first close port
            PrinterClose (fdUsb);
            fdUsb = INVALID_HANDLE_VALUE;
        }
        fdUsb = PrinterOpen (EXTERNAL_PRINTER_PORT);
        if (fdUsb == INVALID_HANDLE_VALUE) {
            LogError("Fail to open external printer");
            return SERR_EXT_PTR_NOT_FOUND; 
        }

    }

    return 0;
}


/*
 * function  will be able to monitor internal printer port printer status
 * error code :
 * ERROR_SUCCESS           0x00
 * PRINTER_E_OFFLINE       0x80000001
 * PRINTER_E_NOPAPER       0x80000002
 * PRINTER_E_DEVICE_FAULT  0x80000003
 * PRINTER_E_TIMEOUT       0x80000004
 * PRINTER_E_PAPERJAM      0x80000005
 * ERROR_UNKNOWN_PORT      0x704
 */
int
ResultsPrint::
WriteSer (const void * buf, size_t nbytes)
{
    DWORD       status;

    if (fdSer != INVALID_HANDLE_VALUE) {
        status = PrinterSend (fdSer, buf, nbytes);
        if (status != ERROR_SUCCESS) {
            status = GetApsPrinterStatus (fdSer); 
            if (status != PRINTER_E_NOPAPER) {
                LogError ("Printer Send Error = %x", status);
            }
            return -1;
        }
    } else {
        return -1;
    }
    return 0;
}


// send print thread event to start printing
SystemErrNum_t
ResultsPrint::
PrintReport (char * resultText)
{
    SystemErrNum_t errNum = SERR_NONE;

    // send event to printThread
    if (SetEvent (hPrintState) == 0) {
        LogError ("Fail to SetEvent=%x, error=%d", hPrintState, GetLastError());
    }
    return errNum;
}


// convert Unicode to Ascii and send to printer
// Note: W2CA put converted string to stack, it must
//       be sub-fuction to free stack  
void
ResultsPrint::
UnicodeSend (wchar_t *buff, char *pBuff)
{
    LPCSTR          prt;

    USES_CONVERSION;

    prt = W2CA (buff);
    strncpy (pBuff, prt, strlen (prt));
}

// print thread, sending string to printer
void *
ResultsPrint::
PrintThread (void * arg)
{
    SYSTEMTIME          currentST;
    FILETIME            currentFT;
    ULONGLONG           elapsedTime = 0;
    char                *resultText = NULL;

    while (1) {
        // wait for event
        WaitForSingleObject (hPrintState, INFINITE);
            //until all buffer are printed
            while (printInIndex != printOutIndex) {
                resultText = printBuff [printOutIndex];
                printOutIndex++;
                printOutIndex &= 0x3f;
                if (resultText == NULL) {
                    LogDebug ("resultsText = NULL");
                }
                // Translate ISO-8859-1 characters to printer characters as necessary.
                PrinterTranslate (resultText);
                // internal printer
                if (((settings->defaultPrinter == INTERNAL_PTR)  || (printerTest == INTERNAL_PRINTER_TEST)) && (printerTest != EXTERNAL_PRINTER_TEST)) {
                   // LogDebug("ResultsPrint::PrintReport -> Internal Printer");
                    printerTest = NO_PRINTER_TEST;
                    //in the case of external printer port open, close it
                    if (fdUsb != INVALID_HANDLE_VALUE) {
                        PrinterClose (fdUsb);
                        fdUsb = INVALID_HANDLE_VALUE;
                    }
                    if (fdSer == INVALID_HANDLE_VALUE) {
                        LogDebug ("Reinitial internal printer");
                        PrinterInitial (INTERNAL_PRINTER);
                        if (fdSer == INVALID_HANDLE_VALUE) {
                            LogDebug("fdSer was INVALID_HANDLE_VALUE");
                            free (resultText);
                            freeMemIndex++;
                            continue;
                        }
                    }
                        
                    // Get the current time.  If it's been less than 5 seconds since we
                    // printed the last report, add a delay until we hit the 5 seconds.
                    // We can get a negative number if the date/time was changed between
                    // reports, so we also make sure the elapsed time is greater than 0.
                    GetLocalTime (&currentST);
                    SystemTimeToFileTime (&currentST, &currentFT);
                    elapsedTime = (((ULONGLONG) currentFT.dwHighDateTime) << 32) + currentFT.dwLowDateTime - lastReportTime;
                    if ((elapsedTime >= 0) && (elapsedTime < 5000)) {
                        Sleep( 5000 - elapsedTime );
                    }

                    lastReportTime = (((ULONGLONG) currentFT.dwHighDateTime) << 32) + currentFT.dwLowDateTime;
                    WriteSer (resultText, strlen( resultText));
                    counterData->printCount++;
                    WriteCounters (counterData);

            } else {
                wchar_t         buff[100];
                char *          s;
                char            *pBuff;  

                //LogDebug("ResultsPrint::PrintReport -> External Printer");
                printerTest = NO_PRINTER_TEST;
                if (fdUsb == INVALID_HANDLE_VALUE) {
                    Sleep (1000);
                    PrinterInitial (EXTERNAL_PRINTER);
                    if (fdUsb == INVALID_HANDLE_VALUE) {
                        LogDebug("fdUSB was INVALID_HANDLE_VALUE");
                        if (resultText != NULL) {
                            free (resultText);
                            freeMemIndex++;
                            continue;
                        }
                    }

                }
                
                memset (externalPrinterBuff, 0, sizeof (externalPrinterBuff));
                pBuff = externalPrinterBuff;

                if (fdUsb != INVALID_HANDLE_VALUE) {
                    // Break the text into individual lines and print them with leading blanks.
                    s = strtok (resultText, "\r\n");
                    while (s != 0) {
                        sprintf (pBuff,"%s", "                         ");
                        pBuff += strlen (pBuff);
                        //transfer from unicode to ascii
                        wsprintf (buff, L"%S", s, strlen (s));
                        UnicodeSend (buff, pBuff);
                        pBuff += strlen (pBuff);
                        sprintf (pBuff,"%s","\r\n");
                        pBuff += strlen (pBuff);
                        s = strtok (0, "\r\n");
                    }

                    // Finish with a formfeed.
                    sprintf (pBuff, "\f");
                    PrinterSend (fdUsb, (char *)externalPrinterBuff, strlen (externalPrinterBuff));
                    // waiting for finishing current printing
                    Sleep (EXTERNAL_PRINTER_DELAY_TIME);
                }
            }
            // finish printing, free memory 
            if (resultText != NULL) {
                free (resultText);
            }
            resultText = NULL;
            freeMemIndex++;
            //LogError ("set=%d free=%d index=%d outindx=%d", setMemIndex, freeMemIndex, printInIndex, printOutIndex);
        } // end of InIndex != OutIndex
        // close external printer port after print to prevent from port conflicting
   } // end of while (1)
}

void
ResultsPrint::
PrinterTranslate (char * text)
{
    unsigned int i;
    const unsigned char xlate[] = {
        0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xA8,
        0x20, 0x20, 0x20, 0x20, 0x8E, 0x8F, 0x92, 0x20, 0x20, 0x90, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0xA5, 0x20, 0x20, 0x20, 0x20, 0x99, 0x20, 0x20, 0x20, 0x20, 0x20, 0x9A, 0x20, 0x20, 0xE1,
        0x85, 0xA0, 0x83, 0x20, 0x84, 0x86, 0x91, 0x87, 0x8A, 0x82, 0x88, 0x89, 0x8D, 0xA1, 0x8C, 0x8B,
        0x20, 0xA4, 0x95, 0xA2, 0x93, 0x20, 0x94, 0x20, 0x20, 0x97, 0xA3, 0x96, 0x81, 0x20, 0x20, 0x98
    };

    for (i = 0; i < strlen( text ); i++) {
        if ((unsigned char)(text[ i ]) >= 0xB0) {
            text[i] = xlate [(unsigned char)text[ i ] - 0xB0];
        }
    }
    text[i++] = '\n';
    text[i++] = '\n';
    text[i++] = '\n';
    text [i]= '\0';
}


// external printer test
SystemErrNum_t
ResultsPrint::
TestExternalPrinter()
{

    LogDebug("TestExternalPrinter");
    
    char        *pBuff;
    char        tBuff [100];
    int         i, j;
    wchar_t     c;
    const char *header;

    if (fdUsb == INVALID_HANDLE_VALUE) {
        PrinterInitial (EXTERNAL_PRINTER);
        if (fdUsb == INVALID_HANDLE_VALUE) {
            LogDebug ("Fail to open external printer");
            return SERR_EXT_PTR_NOT_FOUND;
        }
    }

    memset (externalPrinterBuff, 0, sizeof (externalPrinterBuff));
    pBuff = externalPrinterBuff;
    
    // Write the header - centered in a 60-character-wide line.
    header = WstrToCstr (strings.PrintString (PSTR_PRINTER_TEST)); 
    i = strlen (header);
    sprintf(pBuff, "%*s\r\n", i + (60 - i) / 2, header);
    pBuff += strlen (pBuff);
    sprintf (pBuff, "%s", "............................................................\r\n");

    // Write the test lines.
    memset (tBuff, 0, sizeof (tBuff));
    for (i = 0, c = '!'; i < 35; i++, c++) {
        for (j = 0; j < 60; j++) {
            tBuff[ j ] = c + j;
        }
        tBuff[60] = '\r';
        tBuff [61] = '\n';
        strncpy (pBuff, tBuff, strlen (tBuff));
        pBuff += strlen (pBuff);
        memset (tBuff, 0, sizeof (tBuff));
    }
    // Move the paper up.
    sprintf(pBuff, "%s", "\f" );
    PrinterSend (fdUsb, (char *)externalPrinterBuff, strlen (externalPrinterBuff));

    PrinterClose(fdUsb);
    fdUsb = INVALID_HANDLE_VALUE;

    return SERR_NONE;
}

void
ResultsPrint::
CreateInternalPrinterTestStr (char *buff)
{
    char *pBuff; 
    unsigned char i, j, c = '0';

    pBuff = buff;


    CenterText (WstrToCstr (strings.PrintString (PSTR_PRINTER_TEST)), pBuff);
    pBuff += strlen (pBuff);
    strcpy (pBuff, (char *)"..............................\r\n");
    pBuff += strlen (pBuff);
    for (i =0 , c = '!'; i < 65; i++, c++) {
        for (j = 0; j < 30; j++) {
            *pBuff++ = char (c +j);
        }
        *pBuff++ = '\r';
        *pBuff++ = '\n';
    }
    strcpy (pBuff, (char *)"\r\n\r\n\r\n");
}

SystemErrNum_t
ResultsPrint::
TestInternalPrinter ()
{
    char *buff;

    if ((buff = CreatePrintBuff (4096)) != NULL) {
        CreateInternalPrinterTestStr (buff);
        printerTest = INTERNAL_PRINTER_TEST;
        return PrintReport (buff);
    } else {
        return SERR_NONE;
    }
}

/*
 * FUNCTION
 *  ResultsPrint::PrintResult
 *
 * DESCRIPTION
 *  Prints a result to the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 */

SystemErrNum_t
ResultsPrint::
PrintResult( int index )
{
    SystemErrNum_t      errNum = SERR_NONE;
    ResultsDbEntry_t    entry;

    if (GetResult (&entry, index) == true) {
        errNum = PrintResult (&entry);
    }
    return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintResult( ResultsDbEntry_t * entry )
{
    char *buff;

    if ((buff = CreatePrintBuff (2048)) != NULL) {
        CreateResultDisplay (entry, buff);
        return PrintReport (buff);
    } else {
        return SERR_NONE;
    }
}


/*
 * FUNCTION
 *  ResultsPrint::PrintErrorReport
 *
 * DESCRIPTION
 *  Prints an error report to the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 *  abbreviated - If true, omit the results.
 */

SystemErrNum_t
ResultsPrint::
PrintErrorReport( int index, bool abbreviated )
{
    SystemErrNum_t      errNum = SERR_NONE;
    ResultsDbEntry_t    entry;

    if (GetResult( &entry, index ) == true) {
        errNum = PrintErrorReport (&entry, abbreviated);
    }
    return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintErrorReport( ResultsDbEntry_t * entry, bool abbreviated )
{

    char *buff;

    if ((buff = CreatePrintBuff (2048)) != NULL) {
        CreateErrorReportDisplay (entry, buff, abbreviated);
        return PrintReport (buff);
    } else {
        return SERR_NONE;
    }
}

/*
 * FUNCTION
 *  ResultsPrint::PrintIqc
 *
 * DESCRIPTION
 *  Prints an IQC report to the printer.
 *
 * PARAMETERS
 *  index - Index of the desired sample in the results database.
  */

SystemErrNum_t
ResultsPrint::
PrintIqc( int index )
{
    SystemErrNum_t      errNum = SERR_NONE;
    ResultsDbEntry_t    entry;

    if ( GetResult( &entry, index ) == true )
    {
        errNum = PrintIqc( &entry );
    }
    return errNum;
}

SystemErrNum_t
ResultsPrint::
PrintIqc( ResultsDbEntry_t * entry )
{
    char *buff;

    if ((buff = CreatePrintBuff (2048)) != NULL) {
        CreateIqcDisplay(entry, buff);
        return PrintReport( buff );
    } else  {
         return SERR_NONE;
    }
}

SystemErrNum_t
ResultsPrint::
PrintCuvetteTestResults (void)
{
    char *buff;

    if ((buff = CreatePrintBuff (2048)) != NULL) {
        CreateCuvetteTestDisplay  (buff);
        return PrintReport( buff );
    } else  {
         return SERR_NONE;
    }
}


char *
ResultsPrint::
CreatePrintBuff (int  bufSize)
{
    char *buff;

    buff =  (char *)malloc (bufSize);
    if (buff != NULL) {
        setMemIndex++;
        printBuff [printInIndex] = buff;
        printInIndex++;
        printInIndex &= 0x3f;
    } else {
        LogError ("Fail to alloc printer memory");
    }
    return buff;
}

/*
 * FUNCTION
 *  ResultsPrint::PrintRefRanges
 *
 * DESCRIPTION
 *  Prints reference ranges to the printer.
 *
 * PARAMETERS
 *  sampleType - ID for the desired sample type, or STYPE_ALL to print all.
 */

SystemErrNum_t
ResultsPrint::
PrintRefRanges (SampleType_t sampleType)
{
    char            *buff;
    int             i, j;
    SystemErrNum_t  errNum = SERR_NONE;


   // LogDebug ("sampleType =%d", sampleType);

    if (sampleType == STYPE_ALL) {
        for (i = 0; i < STYPE_NUM_SPECIES && errNum == SERR_NONE; i++) {
            if (refRanges->isDefined[ i ]) {
                // All three genders for piccolo if not a control.
                if ((factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
                     (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL)) {
                        // print patient, unknow, man and women
                    for (j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++) {
                        if ((buff = CreatePrintBuff (2048)) != NULL) {
                            CreateRefRangesDisplay( (SampleType_t)i, (Gender_t)j, buff);
                            errNum = PrintReport (buff);
                        }
                    } 
                } else {
                    if ((buff = CreatePrintBuff (2048)) != NULL) {
                        CreateRefRangesDisplay ((SampleType_t)i, GENDER_UNKNOWN, buff);
                        errNum = PrintReport (buff);
                    }
                }
            }
        }
    } else {
        // All three genders for piccolo if not a control.
        if ((factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
             (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL)) {
            for (j = GENDER_UNKNOWN; j <= GENDER_FEMALE; j++) {
                if ((buff = CreatePrintBuff (2048)) != NULL) {
                    CreateRefRangesDisplay (sampleType, (Gender_t)j, buff);
                    errNum = PrintReport (buff);
                }
            }
        } else {
            // print control
            if ((buff = CreatePrintBuff (2048)) != NULL) {
                CreateRefRangesDisplay (sampleType, GENDER_UNKNOWN, buff);
                errNum = PrintReport (buff );
            }
        }
    }

    return errNum;
}

/*
 * FUNCTION
 *  ResultsPrint::CreateHeaderDisplay
 *
 * DESCRIPTION
 *  Creates the header for use by the other display routines.  Formats the
 *  data for display on the front panel.  Also used to format the display for
 *  the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 *  buff - Buffer large enough to hold the result string.
 *
 * RETURNS
 *  Pointer to the buffer.
 */

char *
ResultsPrint::
CreateHeaderDisplay (int index, char * buff)
{
    ResultsDbEntry_t entry;

    if ( GetResult( &entry, index ) == true )
    {
        return CreateHeaderDisplay( &entry, buff );
    }

    return 0;
}
char *
ResultsPrint::
CreateHeaderDisplay( ResultsDbEntry_t * entry, char * buff )
{
    char *          pBuff = buff;
    StringId_t      sid;
    const char *    str;
    int             startPad, endPad;
	char			buff1 [MAX_STRING_LENGTH], buff2 [MAX_STRING_LENGTH];
    const wchar_t * wStr;

    pBuff = CenterText( WstrToCstr (strings.PrintString (STR_PRODUCT_NAME)), pBuff );

    if ( systemData->inDemoMode )
    {
        pBuff = CenterText( WstrToCstr (strings.PrintString (RSTR_DEMONSTRATION_SOFTWARE)), pBuff );
    }
    else if ( systemData->inResearchMode )
    {
        sprintf( pBuff, "%-21.21s%9s\r\n", WstrToCstr (strings.PrintString (RSTR_RESEARCH_SOFTWARE)), versionData->version );
        pBuff += strlen( pBuff );
    }

    pBuff = CenterText( entry->rotorName, pBuff );

    sprintf( pBuff, "%s           %s\r\n", GetDate( &entry->dateTime ), GetTime( &entry->dateTime ) );
    pBuff += strlen( pBuff );

	wcstombs (buff1, strings.PrintString(RSTR_SAMPLE_TYPE), MAX_STRING_LENGTH);
	wcstombs (buff2, species.GetSampleTypePrintString ( (SampleType_t)entry->sampleType, &strings), MAX_STRING_LENGTH);

    //sprintf( pBuff, "%-13.13s%17.17s\r\n", WstrToCstr (strings.PrintString(RSTR_SAMPLE_TYPE)), species.GetSampleTypeString ( (SampleType_t)entry->sampleType, &strings) );
    sprintf( pBuff, "%-13.13s%17.17s\r\n", buff1, buff2);
    pBuff += strlen( pBuff );

    if ( entry->controlExpired == true )
    {
        str = WstrToCstr (strings.PrintString (RSTR_CONTROL_RENEWAL));
        startPad = (30 - strlen( str )) / 2;
        endPad = 30 - startPad - strlen( str );
        sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
        pBuff += strlen( pBuff );
    }

    if ( entry->sampleType >= STYPE_CONTROL && entry->sampleType <= STYPE_LAST_CONTROL )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_CONTROL_ID)), entry->patientControlId );
    }
    else
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_PATIENT_ID)), entry->patientControlId );
    }
    pBuff += strlen( pBuff );

    if ( entry->alternateId[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_ALTERNATE_ID)), entry->alternateId );
        pBuff += strlen( pBuff );
    }

    if ( entry->sampleId[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_SAMPLE_ID)), entry->sampleId );
        pBuff += strlen( pBuff );
    }

    if ( entry->doctorId[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_DOCTOR_ID)), entry->doctorId );
        pBuff += strlen( pBuff );
    }
    if ( entry->location[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_LOCATION)), entry->location );
        pBuff += strlen( pBuff );
    }

    if ( entry->phoneNumber[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_PHONE_NUM)), entry->phoneNumber );
        pBuff += strlen( pBuff );
    }

    if ( entry->admissionOwnerId[0] != '\0' )
    {
        const char * prompt;

        if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
            prompt = WstrToCstr (strings.PrintString (RSTR_ADMISSION_ID));
        else
            prompt = WstrToCstr (strings.PrintString (RSTR_OWNER_ID));
        sprintf( pBuff, "%-19.19s%11.11s\r\n", prompt, entry->admissionOwnerId );
        pBuff += strlen( pBuff );
    }

    if ( entry->dateOfBirth.year != 0 )
    {
        sprintf( pBuff, "%-19.19s%11.11s\r\n", WstrToCstr (strings.PrintString (RSTR_DOB)), GetDate( &entry->dateOfBirth ) );
        pBuff += strlen( pBuff );
    }

    if ( (factoryData->abaxisProductCode == VETSCAN_PRODUCT_CODE) && (entry->lastVaccination.year != 0) )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_DOV)), GetDate( &entry->lastVaccination ) );
        pBuff += strlen( pBuff );
    }

    if ( entry->gender != GENDER_NONE )
    {
        switch ( entry->gender )
        {
            case GENDER_FEMALE:     sid = GSTR_FEMALE;      break;
            case GENDER_MALE:       sid = GSTR_MALE;        break;
            case GENDER_SPAYED:     sid = GSTR_SPAYED;      break;
            case GENDER_NEUTERED:   sid = GSTR_NEUTERED;    break;
            case GENDER_UNKNOWN:    sid = GSTR_UNKNOWN;     break;
            default:                sid = GSTR_UNKNOWN;     break;
        }

        wcstombs (buff1,  strings.PrintString (RSTR_GENDER), MAX_STRING_LENGTH);
        wcstombs (buff2, strings.PrintString (sid), MAX_STRING_LENGTH);
        sprintf( pBuff, "%-16.16s%14.14s\r\n", buff1, buff2);
        //sprintf( pBuff, "%-16.16s%14.14s\r\n", strings.PrintString (RSTR_GENDER), strings.PrintString (sid));
        pBuff += strlen( pBuff );
    }
    if ( entry->age > 0 )
    {
        char ageBuff[20];

        if ( entry->age == 1 )
        {
            switch ( entry->ageUnits )
            {
                case AGE_UNITS_DAYS:    sid = RSTR_DAY;     break;
                case AGE_UNITS_WEEKS:   sid = RSTR_WEEK;    break;
                case AGE_UNITS_MONTHS:  sid = RSTR_MONTH;   break;
                case AGE_UNITS_YEARS:   sid = RSTR_YEAR;    break;
                default:                sid = RSTR_YEAR;    break;
            }
        }
        else
        {
            switch ( entry->ageUnits )
            {
                case AGE_UNITS_DAYS:    sid = RSTR_DAYS;    break;
                case AGE_UNITS_WEEKS:   sid = RSTR_WEEKS;   break;
                case AGE_UNITS_MONTHS:  sid = RSTR_MONTHS;  break;
                case AGE_UNITS_YEARS:   sid = RSTR_YEARS;   break;
                default:                sid = RSTR_YEARS;   break;
            }
        }

        sprintf( ageBuff, "%d %s", entry->age, WstrToCstr (strings.PrintString (sid)) );
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_AGE)), ageBuff );
        pBuff += strlen( pBuff );
    }

    if ( entry->race != RACE_NONE )
    {
        switch ( entry->race )
        {
 
            case RACE_UNKNOWN:                  wStr = strings.PrintString (RSTR_UNKNOWN);                  break;
            case RACE_WHITE:                    wStr = strings.PrintString (RSTR_WHITE);                    break;
            case RACE_BLACK:                    wStr = strings.PrintString (RSTR_BLACK);                    break;
            case RACE_ASIAN_PACIFIC_ISLANDER:   wStr = strings.PrintString (RSTR_ASIAN_PACIFIC_ISLANDER);   break;
            case RACE_NATIVE_AMERICAN:          wStr = strings.PrintString (RSTR_NATIVE_AMERICAN);          break;
            case RACE_HISPANIC:                 wStr = strings.PrintString (RSTR_HISPANIC);                 break;
            default:                            wStr = strings.PrintString (RSTR_UNKNOWN);                  break;
        }

        wcstombs (buff1, wStr , MAX_STRING_LENGTH);
        wcstombs (buff2, strings.PrintString (RSTR_RACE), MAX_STRING_LENGTH);
        sprintf( pBuff, "%-8.8s%22.22s\r\n", buff2, buff1);
        pBuff += strlen( pBuff );
    }

    if ( entry->operatorId[0] != '\0' )
    {
        sprintf( pBuff, "%-16.16s%14.14s\r\n", WstrToCstr (strings.PrintString (RSTR_OPERATOR_ID)), entry->operatorId );
        pBuff += strlen( pBuff );
    }

    if ( entry->operatorUnauthorized == true )
    {
        str = WstrToCstr (strings.PrintString (RSTR_UNAUTHORIZED_OPERATOR));
        startPad = (30 - strlen( str )) / 2;
        endPad = 30 - startPad - strlen( str );
        sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
        pBuff += strlen( pBuff );
    }
    if ( entry->operatorExpired == true )
    {
        str = WstrToCstr (strings.PrintString (RSTR_OPERATOR_RENEWAL));
        startPad = (30 - strlen( str )) / 2;
        endPad = 30 - startPad - strlen( str );
        sprintf( pBuff, "%*s%s%*s\r\n", startPad, " ", str, endPad, " " );
        pBuff += strlen( pBuff );
    }

    sprintf( pBuff, "%-23.23s%7.7s\r\n", WstrToCstr (strings.PrintString (RSTR_ROTOR_LOT_NUM)), entry->lotNumber );
    pBuff += strlen( pBuff );

    sprintf( pBuff, "%-20.20s%10.10s\r\n", WstrToCstr (strings.PrintString (RSTR_SERIAL_NUM)), factoryData->instrumentSerialNumber );
    pBuff += strlen( pBuff );

    sprintf( pBuff, "..............................\r\n" );
    pBuff += strlen( pBuff );

    return pBuff;
}
/*
 * FUNCTION
 *  ResultsPrint::CreateRefRangesHeaderDisplay
 *
 * DESCRIPTION
 *  Creates the header for a reference ranges listing.  Formats the
 *  data for display on the front panel.  Also used to format the display for
 *  the printer.
 *
 * PARAMETERS
 *  sampleType - ID to indicate the desired species.
 *  genter - Gender for piccolo xpress (male/female/unknown).
 *  buff - Buffer large enough to hold the result string.
 *
 * RETURNS
 *  Pointer to the  buffer.
 */

char *
ResultsPrint::
CreateRefRangesHeaderDisplay (SampleType_t sampleType, Gender_t gender, char * buff)
{
    char *  pBuff = buff;
    FILETIME        currentTime;
    SYSTEMTIME      time;
    char            buff1[MAX_STRING_LENGTH], buff2 [MAX_STRING_LENGTH];

    GetLocalTime(&time);
    SystemTimeToFileTime(&time, &currentTime);

    pBuff = CenterText( WstrToCstr (strings.PrintString (STR_PRODUCT_NAME)), pBuff );

    if ( systemData->inDemoMode )
    {
        pBuff = CenterText( WstrToCstr (strings.PrintString (RSTR_DEMONSTRATION_SOFTWARE)), pBuff );
    }
    else if ( systemData->inResearchMode )
    {
        pBuff = CenterText( WstrToCstr (strings.PrintString (RSTR_RESEARCH_SOFTWARE)), pBuff );
    }

    sprintf( pBuff, "%s           %s\r\n", GetDate( &currentTime ), GetTime( &currentTime ) );
    pBuff += strlen( pBuff );

    wcstombs (buff1,  strings.PrintString (RSTR_RANGE_LABEL), MAX_STRING_LENGTH);
    wcstombs (buff2,  species.GetSampleTypePrintString ( sampleType, &strings), MAX_STRING_LENGTH);
    sprintf( pBuff, "%-13.13s%17.17s\r\n", buff1, buff2);
    //sprintf( pBuff, "%-13.13s%17.17s\r\n", strings.PrintString (RSTR_RANGE_LABEL), species.GetSampleTypeString ( sampleType, &strings) );
    pBuff += strlen( pBuff );

    // Gender entry only if piccolo xpress and not a control.
    if ( (factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE) &&
         (sampleType < STYPE_CONTROL || sampleType > STYPE_LAST_CONTROL) )
    {
        wcstombs (buff1, strings.PrintString (RSTR_GENDER), MAX_STRING_LENGTH);
        wcstombs (buff2, strings.PrintGender (gender), MAX_STRING_LENGTH);
        sprintf( pBuff, "%-16.16s%14.14s\r\n", buff1, buff2 );
       // sprintf( pBuff, "%-16.16s%14.14s\r\n", strings.PrintString (RSTR_GENDER), strings.PrintGender (gender) );
        pBuff += strlen( pBuff );
    }

    sprintf( pBuff, "%-20.20s%10.10s\r\n", WstrToCstr (strings.PrintString (RSTR_SERIAL_NUM)), factoryData->instrumentSerialNumber );
    pBuff += strlen( pBuff );

    sprintf( pBuff, "..............................\r\n" );
    pBuff += strlen( pBuff );

    return pBuff;
}
/*
 * FUNCTION
 *  ResultsPrint::CenterText
 *
 * DESCRIPTION
 *  Centers a string in a field 30 characters wide, with a CR/LF appended to the
 *  end (CR/LF not included in the 30 characters).
 *
 * PARAMETERS
 *  str - String to center.
 *  buff - Buffer to accept the field.
 *
 * RETURNS
 *  Pointer to the next position in the buffer after the added text.
 */

char *
ResultsPrint::
CenterText( const char * str, char * buff )
{
    const char  blanks[] = "                              ";

    // If the string is too long, just copy it to the buffer.
    if ( strlen( str ) >= 30 )
    {
        strcpy( buff, str );
    }
    else
    {
        strcpy( buff, blanks );
        memcpy( buff + (30 - strlen( str )) / 2, str, strlen( str ) );
    }

    strcat( buff, "\r\n" );

    return buff + strlen( buff );
}

/*
 * FUNCTION
 *  ResultsPrint::CreateResultDisplay
 *
 * DESCRIPTION
 *  Creates a display string for the indicated sample result.  Formats the
 *  data for display on the front panel.  Also used to format the display for
 *  the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 *  buff - Buffer large enough to hold the result string.
 *
 * RETURNS
 *  Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateResultDisplay( int index, char * buff )
{
    ResultsDbEntry_t entry;

    if ( GetResult( &entry, index ) == true )
    {
        CreateResultDisplay( &entry, buff );
    }
}

void
ResultsPrint::
CreateResultDisplay( ResultsDbEntry_t * entry, char * buff )
{
    char *  pBuff = buff;
    static SystemError  systemError;
    AnalyteResultType_t analyteIndex;
    char   temp [64];

    // Header text.
    pBuff = CreateHeaderDisplay( entry, pBuff );

    // Results text.
    for ( int i = 1; i <= MAX_ROTOR_RESULTS; i++ )
    {
        for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
        {
            if ( entry->rotorAnalyteResults[ j ].printOrder == i )
            {
                analyteIndex = (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType;
                AnalyteId_t analyteId = analytes.GetAnalyteId( analyteIndex );


                memset (temp, 0, sizeof (temp));
                wcstombs (temp, units.String(analyteId), 64);
                sprintf( pBuff, "%-5.5s %s %s %6.6s\r\n",
                    WstrToCstr (analytes.GetAnalysisString ( analyteId, &strings)),
                    FormatResult( entry, j, FMT_NORMAL, &strings ),
                    FormatRefRange( entry, j ),
                    temp);
                pBuff += strlen( pBuff );
                break;
            }
        }
    }

    // Line of blanks after the results.
    sprintf( pBuff, "%30s\r\n", " " );
    pBuff += strlen( pBuff );

    // Footer
    sprintf( pBuff, "%-30.30s\r\n", WstrToCstr (strings.PrintString (RSTR_QC_OK)));
    pBuff += strlen( pBuff );

    sprintf( pBuff, "HEM %-4.4s  LIP %-4.4s  ICT %-4.4s  \r\n",
        GetEndogenousString( entry->rotorInformationResults.rotorHemolyzedIndex, hemLimits ),
        GetEndogenousString( entry->rotorInformationResults.rotorLipemicIndex, lipLimits ),
        GetEndogenousString( entry->rotorInformationResults.rotorIctericIndex, ictLimits ) );
    pBuff += strlen( pBuff );

    // Warning line, only if Piccolo.
    if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
    {
        if ( (entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0 )
             && (systemError.GetAction( entry->errNum ) != SERR_ACTION_ABORT) )
        {
            sprintf( pBuff, "%-30.30s\r\n", WstrToCstr (strings.PrintString (RSTR_CONFIRM_LOW_RECOVERIES)));
            pBuff += strlen( pBuff );
        }
        else
        {
            sprintf( pBuff, "%30s\r\n", " " );
            pBuff += strlen( pBuff );
        }
    }

    // CR/LF pair to signal the end of the report.
    strcpy( pBuff, "\r\n" );
    pBuff += strlen( pBuff );
}


/*
 * FUNCTION
 *  ResultsPrint::CreateErrorReportDisplay
 *
 * DESCRIPTION
 *  Creates an error report display string for the indicated test.  Formats
 *  the data for display on the front panel.  Also used to format the display
 *  for the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 *  buff - Buffer large enough to hold the result string.
 *  abbreviated - If true, omit the results.
 *
 * RETURNS
 *  Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateErrorReportDisplay( int index, char * buff, bool abbreviated )
{
    ResultsDbEntry_t entry;

    if ( GetResult( &entry, index ) == true )
    {
        CreateErrorReportDisplay( &entry, buff, abbreviated );
    }
}

void
ResultsPrint::
FormatCounter( char * str, UINT32 value )
{
    char    buff[20];
    int     i;

    sprintf( buff, "%08X", ~value + 1 );
    for ( i = 0; i < 4; i++ )
    {
        str[ i ] = buff[ 7 - i ];
    }
    str[ 4 ] = ' ';
    str[ 5 ] = ' ';
    for ( i = 4; i < 8; i++ )
    {
        str[ i + 2 ] = buff[ 7 - i ];
    }
    str[ 10 ] = '\0';
}
void
ResultsPrint::
CreateErrorReportDisplay( ResultsDbEntry_t * entry, char * buff, bool abbreviated )
{
    char *              pBuff = buff;
    char                cBuffA[20];
    char                cBuffB[20];
    char                buff1 [MAX_STRING_LENGTH];
    unsigned            flags;
    AnalyteId_t         analyteId;
    int                 endogenousIndex;
    ResultFormatType_t  formatType;
    static SystemError  systemError;
    AnalyteResultType_t analyteIndex;

    // Header.
    pBuff = CreateHeaderDisplay( entry, pBuff );

    // Flags.
    FormatCounter( cBuffA, counterData->rotorRunCount );
    FormatCounter( cBuffB, counterData->rotorAbortCount );
    sprintf( pBuff, "01 %s  %s     \r\n", cBuffA, cBuffB );
    pBuff += strlen( pBuff );

    FormatCounter( cBuffA, counterData->printCount );
    FormatCounter( cBuffB, counterData->flashCount );
    sprintf( pBuff, "02 %s  %s     \r\n", cBuffA, cBuffB );
    pBuff += strlen( pBuff );

    sprintf( pBuff, "03 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
        entry->rotorInformationResults.rotorDacTrimFlags,
        entry->rotorInformationResults.rotorOffsetErrorFlags,
        entry->rotorInformationResults.rotorOffsetSdErrorFlags,
        entry->rotorInformationResults.rotorWavelengthCvFlags );
    pBuff += strlen( pBuff );

    UINT32 bc1 = entry->rotorInformationResults.rotorBeadCheck1Flags;
    UINT32 bc2 = entry->rotorInformationResults.rotorBeadCheck2Flags;
    sprintf( pBuff, "04 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
        bc1 >> 16, bc1 & 0xFFFF, bc2 >> 16, bc2 & 0xFFFF );
    pBuff += strlen( pBuff );

    UINT32 dc = entry->rotorInformationResults.rotorDistributionCheckFlags;
    UINT32 sf = entry->rotorInformationResults.rotorSystemFlags;
    sprintf( pBuff, "05 %4.4X  %4.4X  %4.4X  %4.4X     \r\n",
        dc >> 16, dc & 0xFFFF, sf >> 16, sf & 0xFFFF );
    pBuff += strlen( pBuff );

    sprintf( pBuff, "06 %4.4X%23s\r\n",
        entry->rotorInformationResults.rotorQcFlags,
        " " );
    pBuff += strlen( pBuff );
    // Results.
    if ( !abbreviated )
    {
        for ( int i = 1; i <= MAX_ROTOR_RESULTS; i++ )
        {
            for ( int j = 0; j < MAX_ROTOR_RESULTS; j++ )
            {
                if ( entry->rotorAnalyteResults[ j ].printOrder == i )
                {
                    if ( systemData->inDemoMode )
                        flags = entry->rotorAnalyteResults[ j ].resultPrintFlags;
                    else
                        flags = entry->rotorAnalyteResults[ j ].analyteFlags;

                    analyteIndex = (AnalyteResultType_t)entry->rotorAnalyteResults[ j ].analyteType;

                    analyteId = analytes.GetAnalyteId( analyteIndex );

                    // Result format depends on whether we're in research mode or not.
                    formatType = systemData->inResearchMode ? FMT_ERROR_RESEARCH : FMT_ERROR;
                    wcstombs (buff1, analytes.GetAnalysisString (analyteId, &strings), MAX_STRING_LENGTH);

#if 0
                    sprintf( pBuff, "%-5.5s %s %4.4X %4.4X %6.6s\r\n",
                        analytes.GetAnalysisString (analyteId, &strings),
                        FormatResult( entry, j, formatType, &strings ),
                        flags >> 16, flags & 0xFFFF,
                        units.String( analyteId ) );
#else
                    sprintf( pBuff, "%-5.5s %s %4.4X %4.4X %6.6s\r\n",
                        buff1,
                        FormatResult( entry, j, formatType, &strings ),
                        flags >> 16, flags & 0xFFFF,
                        WstrToCstr (units.String( analyteId )) );

#endif
                    pBuff += strlen( pBuff );
                }
            }
        }
    }

    // Line of blanks after the results.
    sprintf( pBuff, "%30s\r\n", " " );
    pBuff += strlen( pBuff );

    /*
     * Footer.
     */
    // RQC result.
    if ( entry->rotorAnalyteResults[ RQC ].analyteResult < 0 )
    {
        sprintf( pBuff, "RQC: ~~~%22s\r\n", " " );
    }
    else
    {
        int rqcResult = (int)( entry->rotorAnalyteResults[ RQC ].analyteResult + 0.5);
        sprintf( pBuff, "RQC: %3d%22s\r\n", rqcResult, " " );
    }
    pBuff += strlen( pBuff );


    // Endogenous values.

    // HEM
    if ( entry->rotorInformationResults.rotorHemolyzedIndex < -999.0 )
    {
        sprintf( pBuff, "HEM ~~~   " );
    }
    else
    {
        if ( entry->rotorInformationResults.rotorHemolyzedIndex >= 0 )
        {
            endogenousIndex = (int)( entry->rotorInformationResults.rotorHemolyzedIndex +0.5);
        }
        else
        {
            endogenousIndex = (int)(entry->rotorInformationResults.rotorHemolyzedIndex +0.5);
        }
        sprintf( pBuff, "HEM %-4d  ", endogenousIndex );
    }
    pBuff += strlen( pBuff );

    // LIP
    if ( entry->rotorInformationResults.rotorLipemicIndex < -999.0 )
    {
        sprintf( pBuff, "LIP ~~~   " );
    }
    else
    {
        endogenousIndex = (int)( entry->rotorInformationResults.rotorLipemicIndex +0.5);
        sprintf( pBuff, "LIP %-4d  ", endogenousIndex );
    }
    pBuff += strlen( pBuff );

    // ICT
    if ( entry->rotorInformationResults.rotorIctericIndex < -999.0 )
    {
        sprintf( pBuff, "ICT ~~~   \r\n" );
    }
    else
    {
        endogenousIndex = (int)( entry->rotorInformationResults.rotorIctericIndex +0.5);
        sprintf( pBuff, "ICT %-4d  \r\n", endogenousIndex );
    }
    pBuff += strlen( pBuff );

    // Warning line, only if Piccolo.
    if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
    {
        if ( (entry->rotorInformationResults.dilutionCheckResults.rotorDilutionCheckError != 0)
             && (systemError.GetAction( entry->errNum ) != SERR_ACTION_ABORT) )
        {
            sprintf( pBuff, "%-30.30s\r\n", WstrToCstr (strings.PrintString (RSTR_CONFIRM_LOW_RECOVERIES)));
            pBuff += strlen( pBuff );
        }
        else
        {
            sprintf( pBuff, "%30s\r\n", " " );
            pBuff += strlen( pBuff );
        }
    }


    // Software version.
    sprintf( pBuff, "%-21.21s%9s\r\n", WstrToCstr (strings.PrintString (MSTR_VERSION)), versionData->version );
    pBuff += strlen( pBuff );

    // Error text.
    if ( entry->errNum != SERR_NONE )
    {
        SystemError systemError;

        sprintf( pBuff, "%04X %25s\r\n", entry->errNum, WstrToCstr (strings.PrintString (systemError.GetScreenText(entry->errNum))));
        pBuff += strlen( pBuff );
    }

    // CR/LF pair to signal the end of the report.
    strcpy( pBuff, "\r\n" );
    pBuff += strlen( pBuff );

}

/*
 * FUNCTION
 *  ResultsPrint::CreateIqcDisplay
 *
 * DESCRIPTION
 *  Creates an IQC display string for the indicated test.  Formats the
 *  data for display on the front panel.  Also used to format the display for
 *  the printer.
 *
 * PARAMETERS
 *  index - Index of the desired result in the results database.
 *  buff - Buffer large enough to hold the result string.
 *
 * RETURNS
 *  Pointer to the result display buffer.
 */

void
ResultsPrint::
CreateIqcDisplay( int index, char * buff )
{
    ResultsDbEntry_t entry;

    if ( GetResult( &entry, index ) == true )
    {
        CreateIqcDisplay( &entry, buff );
    }
}
void
ResultsPrint::
CreateIqcDisplay( ResultsDbEntry_t * entry, char * buff )
{
    char *          pBuff = buff;
    const char *    wavelength[] = { "340", "405", "467", "500", "515", "550", "600", "630" };

    // Header text.
    pBuff = CreateHeaderDisplay( entry, pBuff );

    // Body text.
    if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
    {
        sprintf( pBuff, "%-27s%3d\r\n",
            WstrToCstr (strings.PrintString (RSTR_INSTRUMENT_QC)),
            (int)( (entry->rotorInformationResults.rotorIqcRatioPrecision * 100) +0.5) );
        pBuff += strlen( pBuff );

        for ( int i = 0; i < 8; i++ )
        {
            sprintf( pBuff, "iQC %d:        %3d  %s nm: %3d\r\n",
                i+1,
                (int)( (entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100) +0.5 ),
                wavelength[ i ],
                (int)( (entry->rotorInformationResults.rotorIqcLevel2Results[ i ] * 100) +0.5 ) );
            pBuff += strlen( pBuff );
        }

        // Range.
        sprintf( pBuff, "%-11s90-110       95-105\r\n", WstrToCstr (strings.PrintString (RSTR_RANGE)) );
        pBuff += strlen( pBuff );
    }

    else
    {
        for ( int i = 0; i < 8; i++ )
        {
            sprintf( pBuff, "iQC %d:        %3d%13s\r\n",
                i+1,
                (int)( (entry->rotorInformationResults.rotorIqcLevel1Results[ i ] * 100) +0.5 ),
                " " );
            pBuff += strlen( pBuff );
        }

        // Range.
        sprintf( pBuff, "%-11s90-110             \r\n", WstrToCstr (strings.PrintString (RSTR_RANGE)));
        pBuff += strlen( pBuff );

    }

    // Space row.
    strcpy( pBuff, "\r\n" );
    pBuff += strlen( pBuff );

    // Footer text.
    sprintf( pBuff, "%-27s%3d\r\n",
        WstrToCstr (strings.PrintString (RSTR_CHEMISTRY_QC)),
        (int)( entry->rotorAnalyteResults[ RQC ].analyteResult +0.5) );
    pBuff += strlen( pBuff );
// GLD
    sprintf( pBuff, "%-27s%3d\r\n", WstrToCstr (strings.PrintString (RSTR_ACCEPTABLE_MINIMUM)), RQC_ACCEPTABLE_MINIMUM );
//  sprintf( pBuff, "%-27s%3d\r\n", wcstombs (strings.PrintString (RSTR_ACCEPTABLE_MINIMUM)), entry->otorInformationResults.rotorRqcLowLimit );
    pBuff += strlen( pBuff );

    // CR/LF pair to signal the end of the report.
    strcpy( pBuff, "\r\n" );
    pBuff += strlen( pBuff );
}

/*
 * FUNCTION
 *  ResultsPrint::CreateRefRangesDisplay
 *
 * DESCRIPTION
 *  Creates a reference ranges display for the indicated sample type.  Formats
 *  the data for display on the printer.
 *
 *  NOTE: This does *not* do ALL sample types.  It must be called iteratively
 *        for each sample type if the user selected print all.
 *
 * PARAMETERS
 *  sampleType - Sample ID for the desired species.
 *  buff - Buffer large enough to hold the result string.
 *
 * RETURNS
 *  Pointer to the buffer.
 */

void
ResultsPrint::
CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, char * buff )
{
    char *                  pBuff = buff;
    int                     i;
    AnalyteId_t             analyteId;
    float                   low, high;
    const AnalyteList_t *   analyteList;
    char                    buff1 [MAX_STRING_LENGTH];

    if ( sampleType != STYPE_ALL )
    {
        // Header text.
        pBuff = CreateRefRangesHeaderDisplay( sampleType, gender, pBuff );

        if ( factoryData->abaxisProductCode == PICCOLO_PRODUCT_CODE )
        {
            analyteList = analytes.PiccoloAnalytesWithRefRanges();
        }
        else
        {
            analyteList = analytes.VetScanAnalytesWithRefRanges();
        }

        for ( i = 0; i < analyteList->len; i++ )
        {
            analyteId = analyteList->list[ i ];
            low = refRanges->ranges[ sampleType ][ analyteId ][ gender-1 ].low;
            high = refRanges->ranges[ sampleType ][ analyteId ][ gender-1 ].high;

            wcstombs (buff1, analytes.GetAnalysisString (analyteId, &strings), MAX_STRING_LENGTH);
            sprintf( pBuff, "%-5.5s %s %6.6s        \r\n",
                        buff1,
                        FormatRefRange( analyteId, low, high ),
                        WstrToCstr (units.String( analyteId )));
            pBuff += strlen( pBuff );
        }

        // CR/LF pair to signal the end of the report.
        strcpy( pBuff, "\r\n" );
        pBuff += strlen( pBuff );
    }
}

void
ResultsPrint::
CreateCuvetteTestDisplay (char *buff)
{
    char *                  pBuff = buff;
    cuvetteTestParameters_t cuvetteTestParameters;
    FILE                    *fp;
    int                     i;

    fp = fopen (CUVETTE_TEST_RESULTS_FILE, "rb");
    if (fp != NULL) {
        fread (&cuvetteTestParameters, sizeof (cuvetteTestParameters_t), 1, fp);
    } else {
        LogError ("Fail to open %s", CUVETTE_TEST_RESULTS_FILE);
        return;
    }

    // format print
    sprintf (pBuff, "      CUVETTE MARK TEST\n");
    pBuff += strlen (pBuff);
    if (cuvetteTestParameters.cuvError == 0) {
        sprintf (pBuff,"Test Pass\n");
        pBuff += strlen (pBuff);
    } else {
        sprintf (pBuff,"Test      Error:  %x\n",cuvetteTestParameters.cuvError);
        pBuff += strlen (pBuff);
    }
    sprintf (pBuff,"Current     DAC: %4u\n",cuvetteTestParameters.cuvCurrentDac);
    pBuff += strlen (pBuff);
    sprintf (pBuff,"Threshold   DAC: %4u\n",cuvetteTestParameters.cuvThresholdDac);
    pBuff += strlen (pBuff);
    sprintf (pBuff,"Index Cuv Width: %4u\n",cuvetteTestParameters.indexWidth);
    pBuff += strlen (pBuff);
    sprintf (pBuff,"Max   Cuv Width: %4u\n",cuvetteTestParameters.maxCuvWidth);
    pBuff += strlen (pBuff);
    sprintf (pBuff,"Min   Cuv Width: %4u\n",cuvetteTestParameters.minCuvWidth);
    pBuff += strlen (pBuff);

    sprintf (pBuff,"Index 00: %4u\n", cuvetteTestParameters.cuvWidth [0]);
    pBuff += strlen (pBuff);
    for (i = 1; i < 30; i++) {
        sprintf (pBuff,"Mark  %02u: %4u\n", i, cuvetteTestParameters.cuvWidth[i]);
        pBuff += strlen (pBuff);
    }
     /* finally feet the paper */
    for (i =0 ; i < 4; i++) {
        sprintf (pBuff, "\n");
        pBuff += strlen (pBuff);
    }
    *pBuff = '\0';
}

bool
ResultsPrint::
GetResult( ResultsDbEntry_t * entry, int index )
{
    bool            retVal = false;
    const char *    dbData;
    FILE *          fp;

    if ( IN_CUSTOMER_MODE )
        dbData = CUSTOMER_DB_DATA;
    else
        dbData = OTHER_DB_DATA;

    fp = fopen( dbData, "rb" );
    if ( fp != NULL )
    {
        if ( fseek( fp, index * sizeof( ResultsDbEntry_t ), SEEK_SET ) == 0 )
        {
            int bytesRead = fread( (char *)entry, 1, sizeof( ResultsDbEntry_t ), fp );
            if ( bytesRead == sizeof( ResultsDbEntry_t ) )
            {
                retVal = true;
            }
        }
        fclose( fp );
    }
    else
    {
        LogError( "failed to open %s", dbData );
    }

    return retVal;
}

char *
ResultsPrint::
GetDate( FILETIME * time )
{
    static char dateBuff[15];
    //struct tm * tm = gmtime( time );
    SYSTEMTIME  tm;
    FileTimeToSystemTime(time, &tm);

    sprintf( dateBuff, "%02d %s %04d", tm.wDay, WstrToCstr (strings.PrintMonth (tm.wMonth)), tm.wYear );

    return dateBuff;
}

#if 0
char *
ResultsPrint::
GetDate( Date_t * date )
{
    static char dateBuff[15];

    sprintf( dateBuff, "%02d %s %04d", date->day, strings.PrintMonth (date->month), date->year );

    return dateBuff;
}
#else
char *
ResultsPrint::
GetDate( Date_t * date )
{
    static char dateBuff[15];

    sprintf( dateBuff, "%02d %S %04d", date->day, strings.PrintMonth (date->month), date->year );

    return dateBuff;
}
#endif

char *
ResultsPrint::
GetTime( FILETIME * time )
{
    static char     timeBuff[10];
    //struct tm *       tm = gmtime( time );
    SYSTEMTIME  tm;
    FileTimeToSystemTime(time, &tm);
    int             hour = tm.wHour;
    const char *    amPm;

    if ( settings->time24Format )
    {
        sprintf( timeBuff, "   %02d:%02d", tm.wHour, tm.wMinute );
    }
    else
    {
        amPm = hour < 12 ? "AM" : "PM";

        if ( hour == 0 )
        {
            hour = 12;
        }
        else if ( hour > 12 )
        {
            hour -= 12;
        }

        sprintf( timeBuff, "%02d:%02d %s", hour, tm.wMinute, amPm );
    }

    return timeBuff;
}
const char *
ResultsPrint::
GetEndogenousString( float value, const float limits[] )
{
    if ( value < limits[0] )
    {
        return endogenousStrings[0];
    }
    else if ( value < limits[1] )
    {
        return endogenousStrings[1];
    }
    else if ( value < limits[2] )
    {
        return endogenousStrings[2];
    }
    else
    {
        return endogenousStrings[3];
    }
}

