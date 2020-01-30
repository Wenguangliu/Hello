#ifndef __CREATEDISPLAY_H_INCLUDED
#define __CREATEDISPLAY_H_INCLUDED

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

#ifdef QNX
#define INTERNAL_PRINTER_PORT		"/dev/usbpar0"
#define EXTERNAL_PRINTER_PORT		"/dev/usbpar0"
#else
#define EXTERNAL_PRINTER_PORT		L"LPT2:"
#endif


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

class CreateDisplay 
{
public:
	static void				CreateResultDisplay( int index, wchar_t * buff, AStrings *strings );
	static void				CreateResultDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings );
	static void				CreateErrorReportDisplay( int index, wchar_t * buff, bool abbreviated, AStrings *strings );
	static void				CreateErrorReportDisplay( ResultsDbEntry_t * entry, wchar_t * buff, bool abbreviated,  AStrings *strings );
	static void				CreateIqcDisplay( int index, wchar_t * buff, AStrings *strings );
	static void				CreateIqcDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings );
	static void				CreateRefRangesDisplay( SampleType_t sampleType, Gender_t gender, wchar_t * buff, AStrings *strings );
	
	static void				FormatCounter( wchar_t * str,unsigned  value );


private:
	static wchar_t *		CenterText( const wchar_t * str, wchar_t * buff );
	static wchar_t *		CreateHeaderDisplay( int index, wchar_t * buff, AStrings *strings);
	static wchar_t *		CreateHeaderDisplay( ResultsDbEntry_t * entry, wchar_t * buff, AStrings *strings );
	static wchar_t *		CreateRefRangesHeaderDisplay( SampleType_t sampleType, Gender_t gender, wchar_t * buff,AStrings *strings );

	static bool				GetResult( ResultsDbEntry_t * entry, int index );
	static wchar_t *		GetDate( FILETIME * time, AStrings *strings );
	static wchar_t *		GetDate( Date_t * date, AStrings *strings );
	static wchar_t *		GetTime( FILETIME * time );
	static const wchar_t *	GetEndogenousString( float value, const float limits[] );
	
	static ULONGLONG		lastReportTime;
	static CounterData_t *	counterData;
	static FactoryData_t *	factoryData;
	static SettingsData_t *	settings;
	static SystemData_t *	systemData;
	static RefRangeData_t *	refRanges;
	//static AStrings			strings;
	static Analytes			analytes;
	static Units			units;
	static Species			species;
	static char *		    version;
};


#endif // __RESULTSPRINT_H_INCLUDED

