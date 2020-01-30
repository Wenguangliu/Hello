/*
 * FILENAME
 * 	PrinterMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the print process.
 * 
 */
 
#ifndef __PRINTERMESSAGES_H_INCLUDED
#define __PRINTERMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "SystemErrorNums.h"


#define PRINTER_QUEUE_NAME	        L"/Printer"
#define PRINTER_RESP_QUEUE_NAME     L"/PrinterResp"

/*
 * Message IDs for messages received by the Analysis process.
 */
typedef enum
{
	MSG_PRINT_RESULTS_BY_INDEX = MSG_FIRST_PRINTER_ID,
	MSG_PRINT_RESULTS_BY_ENTRY,
	MSG_PRINT_ERROR_REPORT_BY_INDEX,
	MSG_PRINT_ERROR_REPORT_BY_ENTRY,
	MSG_PRINT_IQC_BY_INDEX,
	MSG_PRINT_IQC_BY_ENTRY,
	MSG_PRINT_REFERANCE,
	MSG_PRINT_INTERNAL_PRINTER_TEST,
	MSG_PRINT_EXTERNAL_PRINTER_TEST,
    MSG_PRINT_INTERNAL_PRINTER_DEFAULT,
    MSG_PRINT_EXTERNAL_PRINTER_DEFAULT,
    MSG_PRINT_RESPONSE,
    MSG_PRINT_CUVETTE_TEST_RESULTS,
    MSG_PRINT_GET_PRINTER_ID,
} PrinterMsgId_t;

typedef struct
{
	PrinterMsgId_t	msgId;
    int             selectedItem;
    BOOL            abbreviated;
    SampleType_t    sampleType;
    SystemErrNum_t  errNumber; 
} PrinterMsg_t;

#endif	/* __PRINTERMESSAGES_H_INCLUDED */

