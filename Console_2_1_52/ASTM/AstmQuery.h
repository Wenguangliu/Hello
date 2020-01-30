#ifndef __ASTMQUERY_H_INCLUDED
#define __ASTMQUERY_H_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include "AstmData.h"
#include "InstrumentData.h"
#include "Species.h"

void				FormatDateTime( time_t t, char * buff );
AstmRecord_t *		GetAstmResultsRecord( int recordId );
char *				GetCurrentDateTime( void ); 
InstrumentInfo_t *	GetInstrumentInfo( void );
AstmRecord_t *		GetNextAstmResultsRecord( void );
AstmRecord_t *		SearchAstmResultsRecord( const char * patientControlId, const char * startTime, const char * endTime );
void				MarkResultsRecordSent( int recordId );
void				MarkTransmitResultsRecordsDone( void );

extern Species		species;

#if defined(__cplusplus)
}
#endif

#endif	// __ASTMQUERY_H_INCLUDED


