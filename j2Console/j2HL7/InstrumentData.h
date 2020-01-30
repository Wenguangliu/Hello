// ********************************************************************
//
// FILENAME:  
// 
// 		InstrumentData.h	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains data structure for the HL7 'MSH' record instrument specific constant info
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//
#ifndef __INSTRUMENTDATA_MSH_INCLUDED
#define __INSTRUMENTDATA_MSH_INCLUDED


// data structure for an HL7 'H' record
// instrument specific constant info 
// (to be used to transmit both information/comment type messages (not database result record dependent)  &  database result type records to LIS)

typedef struct
{
	// 'H' record

	char hAnalyzerName[ 32 ];								// gld ???, discuss size
	char hSoftwareVersion[ 32 ];							// gld ???, discuss size
	char hSerialNumber[ 32 ];								// gld ???, discuss size
	char hProcessingId;										// P: Production mode; D: Demo mode
	
	char productCode;										// Piccolo/VetScan product code
} InstrumentInfo_t;

#endif	// __INSTRUMENTDATA_MSH_INCLUDED


