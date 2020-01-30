#ifndef __FORMATRESULTS_H_INCLUDED
#define __FORMATRESULTS_H_INCLUDED

#include "Analytes.h"
#include "ResultsData.h"
#include "ResultsDb.h"
#include "Species.h"

typedef enum
{
	FMT_NORMAL,
	FMT_ERROR,
	FMT_ERROR_RESEARCH
} ResultFormatType_t;

char *	FormatResult( ResultsDbEntry_t * entry, int resultIndex, ResultFormatType_t formatType, AStrings *strings );
char *	FormatRefRange( ResultsDbEntry_t * entry, int resultIndex );
char *	FormatRefRange( AnalyteId_t analyteId, float low, float high );
void    FormatValue( float value, char * valueBuff, NumericFormat_t format );
bool	SuppressRefRange( float low, float high, int rotorId, AnalyteId_t analyteId, SampleType_t sampleType );

#endif	// __FORMATRESULTS_H_INCLUDED
