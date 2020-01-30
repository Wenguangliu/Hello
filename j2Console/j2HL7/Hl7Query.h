// ********************************************************************
//
// FILENAME:  
// 
// 		Hl7Query.h	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains query references for Hl7 protocol implementation
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//
#ifndef __HL7QUERY_MSH_INCLUDED
#define __HL7QUERY_MSH_INCLUDED

#if defined(__cplusplus)
extern "C" {
#endif

#include "Hl7Data.h"
#include "InstrumentData.h"
#include "HL7Configuration.h"

void				FormatDateTime( FILETIME t, char * buff, int size );
Hl7Record_t *		GetHl7ResultsRecord( int recordId );
void				clearResultList(void);
char *				GetCurrentDateTime( void ); 
InstrumentInfo_t *	GetInstrumentInfo( void );
Hl7Record_t *		GetNextHl7ResultsRecord( void );
Hl7Record_t *		GetNewestHl7ResultsRecord( void );
Hl7Record_t *		GetOldestHl7ResultsRecord( void );
Hl7Record_t *		SearchHl7ResultsRecord( const char * patientControlId, const char * startTime, const char * endTime );
void				MarkResultsRecordSent( int recordId );
void				MarkTransmitResultsRecordsDone( void );

extern Hl7Configuration_t * hl7Configuration_m;
extern char observationStartTime[16];
extern char observationEndTime[16];

#if defined(__cplusplus)
}
#endif

#endif	// __HL7QUERY_MSH_INCLUDED


