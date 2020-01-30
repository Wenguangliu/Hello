#ifndef __INSTRUMENTDATA_H_INCLUDED
#define __INSTRUMENTDATA_H_INCLUDED


// data structure for an ASTM 'H' record
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

#endif	// __INSTRUMENTDATA_H_INCLUDED


