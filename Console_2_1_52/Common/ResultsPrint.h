#ifndef __RESULTSPRINT_H_INCLUDED
#define __RESULTSPRINT_H_INCLUDED

#include "ResultsDb.h"
#include "FactoryData.h"
#include "AStrings.h"
#include "SettingsData.h"
#include "CounterData.h"
#include "SystemData.h"
#include "Analytes.h"
#include "Units.h"
#include "Species.h"
#include "RefRanges.h"
#include "SystemErrorNums.h"

#define INTERNAL_PRINTER_PORT		"/dev/ser1"
#define INTERNAL_PRINTER_BAUD_RATE	38400
#define EXTERNAL_PRINTER_PORT		"/dev/usbpar0"

// What to print in the value field of a formatted result.
typedef enum
{
	PV_RESULT,
	PV_TILDES,
	PV_LOW_SYSTEM_LIMIT,
	PV_HIGH_SYSTEM_LIMIT,
	PV_LOW_DYNAMIC_LIMIT,
	PV_HIGH_DYNAMIC_LIMIT,
	PV_HEM,
	PV_LIP,
	PV_ICT,
} PrintedValue_t;

// All the elements of a formatted result.
typedef struct
{
	char			prefix;
	PrintedValue_t	printedValue;
	char			flag;
} PrintLine_t;

class ResultsPrint
{
public:
	static SystemErrNum_t	PrintResult( int index );
	static SystemErrNum_t	PrintResult( ResultsDbEntry_t * entry );
	static SystemErrNum_t	PrintErrorReport( int index, bool abbreviated );
	static SystemErrNum_t	PrintErrorReport( ResultsDbEntry_t * entry, bool abbreviated );
	static SystemErrNum_t	PrintIqc( int index );
	static SystemErrNum_t	PrintIqc( ResultsDbEntry_t * entry );
	static SystemErrNum_t	PrintRefRanges( SampleType_t sampleType );
	
	static void				CreateResultDisplay( int index, char * buff );
	static void				CreateResultDisplay( ResultsDbEntry_t * entry, char * buff );
	static void				CreateErrorReportDisplay( int index, char * buff, bool abbreviated );
	static void				CreateErrorReportDisplay( ResultsDbEntry_t * entry, char * buff, bool abbreviated );
	static void				CreateAbbreviatedErrorReportDisplay( int index, char * buff );
	static void				CreateAbbreviatedErrorReportDisplay( ResultsDbEntry_t * entry, char * buff );
	static void				CreateIqcDisplay( int index, char * buff );
	static void				CreateIqcDisplay( ResultsDbEntry_t * entry, char * buff );
	static void				CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, char * buff );
	
	static void				FormatCounter( char * str,unsigned  value );

	static SystemErrNum_t	TestExternalPrinter();
	static SystemErrNum_t	TestInternalPrinter();

private:
	static char *			CenterText( const char * str, char * buff );
	static char *			CreateHeaderDisplay( int index, char * buff );
	static char *			CreateHeaderDisplay( ResultsDbEntry_t * entry, char * buff );
	static char *			CreateRefRangesHeaderDisplay( SampleType_t sampleType, Gender_t gender, char * buff );

	static bool				GetResult( ResultsDbEntry_t * entry, int index );
	static char *			GetDate( time_t * time );
	static char *			GetDate( Date_t * date );
	static char *			GetTime( time_t * time );
	
	static SystemErrNum_t	PrintReport( char * resultText );
	
	static void				InitExternalPrinterPort();
	static void				InitInternalPrinterPort();

	static const char *		GetEndogenousString( float value, const float limits[] );

	static void				PrinterTranslate( char * text );

	static int				WriteSer( const void * buf, size_t nbytes );
	
	static int				fdSer;
	static int				fdUsb;
	static time_t			lastReportTime;
	static CounterData_t *	counterData;
	static FactoryData_t *	factoryData;
	static SettingsData_t *	settings;
	static SystemData_t *	systemData;
	static RefRangeData_t *	refRanges;
	static AStrings			strings;
	static Analytes			analytes;
	static Units			units;
	static Species			species;
	static char *			version;
};

#endif // __RESULTSPRINT_H_INCLUDED

