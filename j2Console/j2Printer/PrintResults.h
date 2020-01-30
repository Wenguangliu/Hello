#ifndef J2_PRINTER_H
#define J2_PRINTER_H

#include "State.h"
#include "RefRanges.h"
#include "AStrings.h"
#include "Analytes.h"
#include "Units.h"
#include "Species.h"
#include "VersionData.h"

typedef enum
{
    INTERNAL_PRINTER, 
    EXTERNAL_PRINTER, 
} printerType_t;

typedef enum
{
    NO_PRINTER_TEST,
    INTERNAL_PRINTER_TEST,
    EXTERNAL_PRINTER_TEST,
} printerTest_t;

typedef struct
{
    unsigned char   cuvCurrentDac;
    unsigned char   cuvThresholdDac;
    unsigned short  totalCuvNum;
    unsigned short  indexWidth;
    unsigned short  maxCuvWidth;
    unsigned short  minCuvWidth;
    unsigned short  cuvWidth [30];
    unsigned short  cuvError;
} cuvetteTestParameters_t;


#define CUVETTE_TEST_RESULTS_FILE   "Hard Disk/Abaxis/cfg/cuvTest.dat"
#define EXTERNAL_PRINTER_PORT       L"LPT2:" 
#define EXTERNAL_PRINTER_BUFF_SIZE  3000

class ResultsPrint : public State
{
public:
    static void             Receiver (void);        // main thread 
    static int              PrinterInitial (printerType_t printerType);
    static SystemErrNum_t   PrintResult( int index );
    static SystemErrNum_t   PrintResult( ResultsDbEntry_t * entry );
    static SystemErrNum_t   PrintErrorReport( int index, bool abbreviated );
    static SystemErrNum_t   PrintErrorReport( ResultsDbEntry_t * entry, bool abbreviated );
    static SystemErrNum_t   PrintIqc( int index );
    static SystemErrNum_t   PrintIqc( ResultsDbEntry_t * entry );
    static SystemErrNum_t   PrintRefRanges( SampleType_t sampleType );
    static SystemErrNum_t   PrintCuvetteTestResults (void);
   
    static void             CreateResultDisplay( int index, char * buff );
    static void             CreateResultDisplay( ResultsDbEntry_t * entry, char * buff );
    static void             CreateErrorReportDisplay( int index, char * buff, bool abbreviated );
    static void             CreateErrorReportDisplay( ResultsDbEntry_t * entry, char * buff, bool abbreviated );
    static void             CreateAbbreviatedErrorReportDisplay( int index, char * buff );
    static void             CreateAbbreviatedErrorReportDisplay( ResultsDbEntry_t * entry, char * buff );
    static void             CreateIqcDisplay( int index, char * buff );
    static void             CreateCuvetteTestDisplay  (char * buff);
    static void             CreateIqcDisplay( ResultsDbEntry_t * entry, char * buff );
    static void             CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, char * buff );
   
    static void             FormatCounter( char * str,unsigned  value );

    static SystemErrNum_t   TestExternalPrinter();
    static SystemErrNum_t   TestInternalPrinter();

private:
    static char *           CenterText( const char * str, char * buff );
    static char *           CreateHeaderDisplay( int index, char * buff );
    static char *           CreateHeaderDisplay( ResultsDbEntry_t * entry, char * buff );
    static char *           CreateRefRangesHeaderDisplay( SampleType_t sampleType, Gender_t gender, char * buff );

    static bool             GetResult( ResultsDbEntry_t * entry, int index );
    static char *           GetDate( FILETIME * time );
    static char *           GetDate( Date_t * date );
    static char *           GetTime( FILETIME * time );


    static SystemErrNum_t   PrintReport( char * resultText );
    static void *           PrintThread (void *arg);
    static void             GetPrinterId (void);


    static const char *     GetEndogenousString(float value, const float limits[] );

    static void             PrinterTranslate( char * text );

    static int              WriteSer( const void * buf, size_t nbytes );
    static void             CreateInternalPrinterTestStr (char *buff);         
    static void             ClearPrinterBuffer (int repeatTime);
    static char             *CreatePrintBuff (int  bufSize);

    static void             UnicodeSend (wchar_t *buff, char *pBuff);

    static HANDLE           fdSer;
    static HANDLE           fdUsb;

    static ULONGLONG        lastReportTime;
    static CounterData_t *  counterData;
    static FactoryData_t *  factoryData;
    static SettingsData_t * settings;
    static SystemData_t *   systemData;
    static VersionData_t *  versionData;
    static RefRangeData_t * refRanges;
    static AStrings         strings;
    static Analytes         analytes;
    static Units            units;
    static Species          species;

    static HANDLE           printThread;
    static char             *printBuff [64];
    static char             externalPrinterBuff [EXTERNAL_PRINTER_BUFF_SIZE];
    static char             printInIndex;
    static char             printOutIndex;
    static HANDLE           hPrintState;
    static printerTest_t    printerTest;

};
#endif
