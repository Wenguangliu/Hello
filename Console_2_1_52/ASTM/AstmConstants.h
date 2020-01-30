#ifndef __ASTMCONSTANTS_H_INCLUDED
#define __ASTMCONSTANTS_H_INCLUDED

#include "AstmInput.h"


// ASTM transmit record ESC and receive record parameter method definitions       

// ASCII command characters
#define STX		0x02	// Start of checksummed data
#define ETX		0x03	// End of checksummed data
#define EOT		0x04	// End of transmission
#define ENQ		0x05	// Enquire
#define ACK		0x06	// Positive acknowledgement
#define LF		0x0a	// Line feed
#define CR		0x0d	// Carriage return
#define NAK		0x15	// Negative acknowledgement
#define ESC		0x1b	// Escape
#define DEL		0x7f	// DEL char


// tx ESC method definitions       

typedef enum
{
	ASTM_TX_H_ANALYZER_TYPE,						// header record methods       
	ASTM_TX_H_SOFTWARE_VERSION,			
	ASTM_TX_H_SERIAL_NUMBER,				
	ASTM_TX_H_PROCESSING_ID,				
	ASTM_TX_H_DATE_TIME_OF_MESSAGE,		

	ASTM_TX_L_TERMINATION_CODE,						// terminator record methods       

	ASTM_TX_P_SETUP,								// patient record methods       	
	ASTM_TX_P_PATIENT_CONTROL_ID,
	ASTM_TX_P_SAMPLE_ID,
	ASTM_TX_P_ALTERNATE_ID,
	ASTM_TX_P_BIRTHDATE,				
	ASTM_TX_P_PATIENT_SEX,
	ASTM_TX_P_PATIENT_RACE,
	ASTM_TX_P_PHONE_NUMBER,					
	ASTM_TX_P_SPECIAL1,
	ASTM_TX_P_SPECIAL2,
	ASTM_TX_P_LOCATION,			

	ASTM_TX_O_SEQ_NUM,								// order record methods       	
	ASTM_TX_O_ROTOR_IQC_NAME,				
	ASTM_TX_O_LOT_NUM,						
	ASTM_TX_O_RUN_DATE_TIME,					
	ASTM_TX_O_OPERATOR_ID,					
	ASTM_TX_O_PHYSICIAN_ID,					
	ASTM_TX_O_REPORT_TYPE,					
	ASTM_TX_O_SETUP_RESULTS,						

	ASTM_TX_R_SEQ_NUM,								// result record methods       	
	ASTM_TX_R_LOINC_STRING,					
	ASTM_TX_R_VALUE,							
	ASTM_TX_R_UNITS,							
	ASTM_TX_R_REFERENCE_RANGE,				
	ASTM_TX_R_ABNORMAL_FLAG,
	ASTM_TX_R_RESULT_STATUS,					
	ASTM_TX_R_SETUP_NEXT,					

	ASTM_TX_C_SEQ_NUM,								// comment record methods - result       	
	ASTM_TX_C_COMMENT_RESULT_RAW,		
	ASTM_TX_C_COMMENT_CODE,					
	ASTM_TX_C_COMMENT_TEXT,					
	ASTM_TX_C_COMMENT_TYPE,				
	
	ASTM_TX_ABORT_O_ROTOR_NAME_LOT,					// abort rotor record methods
	ASTM_TX_ABORT_O_SETUP,							       		
	ASTM_TX_ABORT_C_COMMENT_TEXT,			

	ASTM_TX_NO_QUERY_RESULT_P_ID,					// no query results record methods       
	ASTM_TX_NO_QUERY_RESULT_C_TEXT,		

	ASTM_TX_1394_C_CODE,							// astm 1394 comment methods       
	ASTM_TX_1394_C_TEXT,					

	ASTM_TX_M1_FIELD3,								// manufacturer type 1 record methods       
	ASTM_TX_M1_FIELD4,								// (Q record loopback rx parameters)         
	ASTM_TX_M1_FIELD5,

	ASTM_TX_INVALID_P_PATIENT_CONTROL_ID,			// order data invalid POC record methods
	ASTM_TX_INVALID_P_SAMPLE_ID,
	ASTM_TX_INVALID_P_ALTERNATE_ID,
	ASTM_TX_INVALID_P_BIRTHDATE,
	ASTM_TX_INVALID_P_PATIENT_SEX,
	ASTM_TX_INVALID_P_PATIENT_RACE,
	ASTM_TX_INVALID_P_PHONE_NUMBER,
	ASTM_TX_INVALID_P_SPECIAL1,
	ASTM_TX_INVALID_P_SPECIAL2,
	ASTM_TX_INVALID_P_LOCATION,
	ASTM_TX_INVALID_O_ROTOR_NUMBER,
	ASTM_TX_INVALID_O_ORDER_DATE_TIME,
	ASTM_TX_INVALID_O_ACTION_CODE,
	ASTM_TX_INVALID_O_PHYSICIAN_ID,
	ASTM_TX_INVALID_O_REPORT_TYPE,
	ASTM_TX_INVALID_C_CODE,
	ASTM_TX_INVALID_C_TEXT,
	
	ASTM_TX_CANCEL_P_PATIENT_CONTROL_ID,			// order cancellation POC record methods
	ASTM_TX_CANCEL_O_ROTOR_NUMBER,
	ASTM_TX_CANCEL_O_ORDER_DATE_TIME,
	ASTM_TX_CANCEL_O_REPORT_TYPE,
	ASTM_TX_CANCEL_C_CODE,
	ASTM_TX_CANCEL_C_TEXT,
	
	ASTM_TX_M_SEQ_NUM,								// manufacturer record methods
	ASTM_TX_M_RECORD_IDENTIFIER,					// manufacturer record identifier method requested by LIS ("AS" or "PO")					
	ASTM_TX_M_ANALYZER_STATUS,						// manufacturer AS analyzer status method requested by LIS (0=not ready; 1=ready; 2=busy)					
	ASTM_TX_M_ORDER_DATE_TIME,						// manufacturer PO pending order date / time
	ASTM_TX_M_ORDER_PATIENT_CONTROL_ID,				// manufacturer PO pending order patient / control ID
	ASTM_TX_M_ORDER_ROTOR_NUMBER,					// manufacturer PO pending order rotor number
	ASTM_TX_M_ORDER_LABORATORY_FIELD_1,				// manufacturer PO pending order laboratory field 1 (order label)
} AstmTxEscMethod_t;

//       rx record parameter method definitions       

typedef enum
{
	ASTM_RX_EMPTY_FIELD = 1,						// generic: for empty or undefined method 			      
	ASTM_RX_FIELD_DELIMITER,						// generic: non header records as single char check       

	ASTM_RX_H_FIELD_DELIMITER,						// H record methods - single char       
	ASTM_RX_H_REPEAT_DELIMITER,			
	ASTM_RX_H_COMPONENT_DELIMITER,		
	ASTM_RX_H_ESCAPE_DELIMITER,			

	ASTM_RX_Q_SEQUENCE_NUMBER,						// Q record methods - handled as generic field strings       
	ASTM_RX_Q_STARTING_RANGE_ID,			
	ASTM_RX_Q_BEGIN_RESULTS_DATE_TIME,	
	ASTM_RX_Q_END_RESULTS_DATE_TIME,		

	ASTM_RX_L_SEQUENCE_NUMBER,						// L record method 	- verifies generic field length of 1 and value = '1'        

	ASTM_RX_H_MESSAGE_CONTROL_ID,					// H record method 	- Message Control ID = HMAC key index
	ASTM_RX_H_ACCESS_PASSWORD,						// 					- Access Password = HMAC computed value
	ASTM_RX_H_SENDER_NAME_OR_ID,					// 					- Sender Name or ID value
	ASTM_RX_H_RECEIVER_ID,							// 					- Receiver ID = Analyzer Serial Number to match
	ASTM_RX_H_MESSAGE_DATE_TIME,					// 					- Date and Time of Message value

	ASTM_RX_P_SEQUENCE_NUMBER,						// P record methods - verifies generic field length of 1 and value = '1'
	ASTM_RX_P_PATIENT_CONTROL_ID,					//					- Practice assigned patient ID
	ASTM_RX_P_SAMPLE_ID,							//                  - Laboratory assigned patient ID
	ASTM_RX_P_ALTERNATE_ID,							//					- Patient ID No. 3
	ASTM_RX_P_BIRTHDATE,							//					- Birthdate
	ASTM_RX_P_PATIENT_SEX,							//					- Patient sex, gender
	ASTM_RX_P_PATIENT_RACE,							//					- Patient race, ethnic origon
	ASTM_RX_P_PATIENT_PHONE_NUMBER,					//					- Patient telephone number
	ASTM_RX_P_PATIENT_SPECIAL1,						//					- Special Field 1 (Owner Admission ID^Age^Last Vaccination)
	ASTM_RX_P_PATIENT_SPECIAL2,						//					- Special Field 2 (Reference Range Label)
	ASTM_RX_P_PATIENT_LOCATION,						//					- Location (e.g. kennel number for VS2)

	ASTM_RX_O_SEQUENCE_NUMBER,						// O record methods - verifies generic field length of 1 and value = '1'
	ASTM_RX_O_ROTOR_NUMBER,							//					- Universal test ID (^^^ Manufacturer's Valid Test Battery Code)
	ASTM_RX_O_ORDER_DATE_TIME,						//					- Requested/Ordered Date and Time (unique Test Order identifer)
	ASTM_RX_O_ACTION_CODE,							//					- Action Code (C=cancel order; N=new order; Q=QC Test Sample)
	ASTM_RX_O_PHYSICIAN_ID,							//					- Ordering Physician (Physician ID assigned by practice)
	ASTM_RX_O_LABORATORY_FIELD_1,					//					- Laboratory Field No. 1
	ASTM_RX_O_REPORT_TYPE,							//					- Report Types (always 'O' for LIS order)
	
	ASTM_RX_M_SEQUENCE_NUMBER,						// M record methods - verifies generic field length of 1 and value = '1'
	ASTM_RX_M_RECORD_IDENTIFIER,					//					- Manufacturer Record ID (always 'AS' for request Analyzer Status)
} AstmRxEscMethod_t;

#define ASTM_RX_NUM_FIELD_METHODS			37
 
#define	MAX_ASTM_RX_FIELD					65		// maximum length of temp astm rx field string       	


//       ASTM tx record (frame) template indices & definitions       

typedef enum
{
	ASTM_TX_MESSAGE_DONE,							// index indicates tx message setup done, so no template       

	ASTM_TX_HEADER,									// index to tx header record template ptr       
	ASTM_TX_TERMINATOR,								// index to tx terminator record template ptr       

	ASTM_TX_MANUFACTURER_TYPE1,						// index to tx manufacturer type 1 record template ptr       

	ASTM_TX_PATIENT,								// index to tx patient record template ptr       
	ASTM_TX_ORDER,									// index to tx test order record template ptr       
	ASTM_TX_RESULT,									// index to tx result record template ptr       

	ASTM_TX_COMMENT,								// index to tx comment record template ptr       

	ASTM_TX_ABORT_ORDER,							// index to tx abort order record template ptr       
	ASTM_TX_ABORT_COMMENT,							// index to tx abort comment record template ptr       

	ASTM_TX_NO_QUERY_RESULTS_PATIENT,				// index to tx no query results patient record template ptr       
	ASTM_TX_NO_QUERY_RESULTS_ORDER,					// index to tx no query results test order record template ptr       
	ASTM_TX_NO_QUERY_RESULTS_COMMENT,				// index to tx no query results test comment record template ptr       

	ASTM_TX_1394_COMMENT,							// index to tx no query results test comment record template ptr
	
	ASTM_TX_ORDER_DATA_INVALID_PATIENT,				// index to tx order invalid data patient record template ptr
	ASTM_TX_ORDER_DATA_INVALID_ORDER,				// index to tx order invalid data order record template ptr
	ASTM_TX_ORDER_DATA_INVALID_COMMENT,				// index to tx order invalid data order comment template ptr

	ASTM_TX_ORDER_CANCELLATION_PATIENT,				// index to tx order cancellation patient record template ptr
	ASTM_TX_ORDER_CANCELLATION_ORDER,				// index to tx order cancellation order record template ptr
	ASTM_TX_ORDER_CANCELLATION_COMMENT,				// index to tx order cancellation order comment template ptr

	ASTM_TX_MANUFACTURER,							// index to tx manufacturer analyzer status template ptr
	ASTM_TX_MANUFACTURER_PO,						// index to tx manufacturer pending order template ptr

	ASTM_TX_MANUFACTURER_PO_REPEAT = 0x7e,			// flags repeat next manufacturer pending order, no template       
	ASTM_TX_RESULTS_REPEAT = 0x7f,					// flags repeat next results/comment, no template       
} AstmTxRecordType_t;

#define NUM_ASTM_TX_RECORDS				22			// number of astm tx records (frame types) defined       

#define LAST_TX_IQC_R_RECORD_XPRESS		17			// flags repeat next results/comment, no template xPress       
#define LAST_TX_IQC_R_RECORD_VS2		8			// flags repeat next results/comment, no template VS2       


//       ASTM rx record (frame) template indices & definitions       

#define NUM_RX_FRAME_TEMPLATES			9			// number of astm rx frame templates in ptr array       

#define NUM_ABAXIS_ASTM_RX_MSGS			3			// number of abaxis astm receive messages 				      		


//       ASTM Abaxis type message definitions       

//       abaxis transmit msg type definitions       

typedef enum
{
	ASTM_TX_NO_MSGS,								// astmTxMsgType_m: no message to send			      
	ASTM_TX_1394_COMMENT_MSG,						// astmTxMsgType_m: comment message HCL			      
	ASTM_TX_MANUFACTURER_TYPE1_MSG,					// astmTxMsgType_m: debug message HML			      
	ASTM_TX_RESULTS_MSG,							// astmTxMsgType_m: abaxis results msg  		      
	ASTM_TX_ABORT_MSG,								// astmTxMsgType_m: abaxis abort rotor error msg  	      
	ASTM_TX_NO_QUERY_RESULTS_MSG,					// astmTxMsgType_m: abaxis no query results msg
	ASTM_TX_ORDER_DATA_INVALID_MSG,					// astmTxMsgType_m: abaxis order from LIS data invalid msg HPOCL       
	ASTM_TX_ORDER_CANCELLATION_MSG,					// astmTxMsgType_m: abaxis order cancellation msg HPOCL       
	ASTM_TX_MANUFACTURER_AS_MSG,					// astmTxMsgType_m: abaxis AS analyzer status request HML			      
	ASTM_TX_MANUFACTURER_PO_MSG,					// astmTxMsgType_m: abaxis PO pending order request HM...ML (variable M)			      
} AstmTxMsgType_t;

#define NUM_ABAXIS_ASTM_TX_MSGS			10			// number of abaxis astm transmit message types (includes 0: no msg       


// ASTM comment strings defintions

#define NUM_ASTM_1394_STRINGS			12			// number of astm comment message text strings        
#define ASTM_1394_STRING_SIZE			(32+1)		// size   of astm comment message text strings       


// ASTM  *** E 1394-97 ***  Record (frame) transmit templates 

// Notes:

// 	1.	The only NULL ('\0') in a record is the last char to indicate end of record text 
//		and to signal insertion of CR ('\r') record terminator at end of frame   

// 	2.	ESC char ('\x1b') signals for insertion of ASCII text based on character value
//		immediately after the ESC (from 0 '\x00' to  255 '\xff') which 
//		incdicates ESC method for variable text insertion   

//	3.	ESC methods are separated as single lines from and concatenated to text strings
//		in the record definitions below   


	// (H) Message Header Record: Abaxis to Host   

const	char	astmTxHrecord[] = 	
{
	'H','|','\\','^','&','|','|','|','A','B','A','X','I','S',',',' ','I','N','C','.','^',			
										
	ESC,ASTM_TX_H_ANALYZER_TYPE,		// ESC Analyzer   
										
	'^', 

	ESC,ASTM_TX_H_SOFTWARE_VERSION,		// ESC Software Version   
										 
	'^',

	ESC,ASTM_TX_H_SERIAL_NUMBER,		// ESC Serial Number   

	'|','|','|','|','|','|','|',

	ESC,ASTM_TX_H_PROCESSING_ID,		// ESC Processing ID   
										
	'|','E',' ','1','3','9','4','-','9','7','|',
										
	ESC,ASTM_TX_H_DATE_TIME_OF_MESSAGE, // ESC Date and Time of Message   

	'\0'								// end of record template delimiter   
};




	// (L) Message Terminator Record: Abaxis to Host   

const	char	astmTxLrecord[] =	
{
	'L','|','1','|',
										
	ESC,ASTM_TX_L_TERMINATION_CODE,		// ESC Termination Code   

	'\0'								// end of record template delimiter   
};

										

	// (P) Patient Information Record: Abaxis to Host   

const	char	astmTxPrecord[] = 	
{
	ESC,ASTM_TX_P_SETUP,				// ESC setup to process P record & init order/results processing   

	'P','|','1','|',					// 8.1.1 & 8.1.2			
										
	ESC,ASTM_TX_P_PATIENT_CONTROL_ID,	// 8.1.3   ESC patient or control ID, practice    
										
	'|',

	ESC,ASTM_TX_P_SAMPLE_ID,			// 8.1.4   ESC sample ID, laboratory ID

	'|',
	
	ESC,ASTM_TX_P_ALTERNATE_ID,			// 8.1.5   ESC alternate ID, patient ID 3 

	'|','|','|',
	
	ESC,ASTM_TX_P_BIRTHDATE,			// 8.1.8   ESC date of birth (DOB) 

	'|',
	
	ESC,ASTM_TX_P_PATIENT_SEX,			// 8.1.9   ESC patient sex, gender   
										 
	'|',
	
	ESC,ASTM_TX_P_PATIENT_RACE,			// 8.1.10  ESC patient race-ethnic origin   
										 
	'|','|','|',

	ESC,ASTM_TX_P_PHONE_NUMBER,			// 8.1.13  ESC patient phone number
	
	'|','|', 

	ESC,ASTM_TX_P_SPECIAL1,				// 8.1.15  ESC "Owner/Admission ID^Age^Last Vaccination (DOV)" string   
										 
	'|',

	ESC,ASTM_TX_P_SPECIAL2,				// 8.1.16  ESC reference range label string   
										 
	'|','|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_P_LOCATION,				// 8.1.26  ESC location (e.g. kennel number for VS 2)

	'\0'								// end of record template delimiter   
};



	// (O) Test Order Record: Abaxis to Host   

const	char	astmTxOrecord[] = 	
{
	'O','|',							// 9.4.1			
										
	ESC,ASTM_TX_O_SEQ_NUM,				// 9.4.2   ESC order record sequence number    
										
	'|','|','|','^','^','^', 

	ESC,ASTM_TX_O_ROTOR_IQC_NAME,		// 9.4.5.4 ESC universal test ID rotor name or IQC header   
	
	':',' ', 
	
	ESC,ASTM_TX_O_LOT_NUM,				// 9.4.5.4 ESC universal test ID lot number   
										 
	'|','|',

	ESC,ASTM_TX_O_RUN_DATE_TIME,		// 9.4.7   ESC date and time sample was run   

	'|','|','|','|',

	ESC,ASTM_TX_O_OPERATOR_ID,			// 9.4.11  ESC operator ID   

	'|','|','|','|','|','|',

	ESC,ASTM_TX_O_PHYSICIAN_ID,			// 9.4.17  ESC physician ID entered by operator   

	'|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_O_REPORT_TYPE,			// 9.4.26  ESC report type   

	ESC,ASTM_TX_O_SETUP_RESULTS,		// ESC setup results for order   

	'\0'								// end of record template delimiter   
};



	// (R) Result Record: Abaxis to Host   

const	char	astmTxRrecord[] = 	
{
	'R','|',			
										
	ESC,ASTM_TX_R_SEQ_NUM,				// ESC result record sequence number    
										
	'|', 

	ESC,ASTM_TX_R_LOINC_STRING,			// ESC ASTM "LOINC" type STRING coding for chem results   
										 
	'|',

	ESC,ASTM_TX_R_VALUE,				// ESC result value, same resolution as print card   

	'|',

	ESC,ASTM_TX_R_UNITS,				// ESC using ASTM & ISO standard   

	'|',

	ESC,ASTM_TX_R_REFERENCE_RANGE,		// ESC result reference range, same resolution as print card   

	'|',

	ESC,ASTM_TX_R_ABNORMAL_FLAG,		// ESC result abnormal flags   

	'|','|',

	ESC,ASTM_TX_R_RESULT_STATUS,		// ESC result abnormal flags   

	ESC,ASTM_TX_R_SETUP_NEXT,			// ESC setup for potential comment & next result   

	'\0'								// end of record template delimiter   
};



	// (C) Comment Record: Abaxis to Host   

const	char	astmTxCrecord[] = 	
{
	'C','|',			
										
	ESC,ASTM_TX_C_SEQ_NUM,				// ESC comment record sequence number    
										
	'|','I','|', 

	// ESC,ASTM_TX_C_COMMENT_RESULT_RAW,	// ESC comment code (result, raw format) 04/10/2016 removed since suppressed result in RAD file  
										 
	'^',

	ESC,ASTM_TX_C_COMMENT_CODE,			// ESC comment code (result_flags, error code)   
										 
	'^',

	ESC,ASTM_TX_C_COMMENT_TEXT,			// ESC comment free text   

	'|',

	ESC,ASTM_TX_C_COMMENT_TYPE,			// ESC comment type: generic or instrument flag   

	'\0'								// end of record template delimiter   
};



	// (M) Message Manufacturer Type 1 Record: Abaxis to Host   
	//	   (used for loopback of debug parameters) 			    

const	char	astmTxM1record[] =	
{
	'M','|','1','|',
										
	ESC,ASTM_TX_M1_FIELD3, 				// ESC debug field   
										
	'|',

	ESC,ASTM_TX_M1_FIELD4,				// ESC debug field    
										 
	'|',

	ESC,ASTM_TX_M1_FIELD5,				// ESC debug field   

	'\0'								// end of record template delimiter   
};



	// (O) Abort Order Record: Abaxis to Host   

const	char	astmTxAbortOrecord[] = 	
{
	'O','|',							// 9.4.1			
										
	ESC,ASTM_TX_O_SEQ_NUM,				// 9.4.2   ESC order record sequence number    
										
	'|','|','|', 

	ESC,ASTM_TX_ABORT_O_ROTOR_NAME_LOT,	// 9.4.5   ESC universal test ID for rotor name & lot number, if known    
										 
	'|','|',

	ESC,ASTM_TX_O_RUN_DATE_TIME,		// 9.4.7   ESC date and time sample was run   

	'|','|','|','|',

	ESC,ASTM_TX_O_OPERATOR_ID,			// 9.4.11  ESC operator ID   

	'|','|','|','|','|','|',

	ESC,ASTM_TX_O_PHYSICIAN_ID,			// 9.4.17  ESC physician ID entered by operator   

	'|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_O_REPORT_TYPE,			// 9.4.26  ESC report type   

	ESC,ASTM_TX_ABORT_O_SETUP,			// ESC setup results for abort order   

	'\0'								// end of record template delimiter   
};



	// (C) Abort Comment Record: Abaxis to Host   

const	char	astmTxAbortCrecord[] = 	
{
	'C','|',			
										
	ESC,ASTM_TX_C_SEQ_NUM,				// ESC comment record sequence number    
										
	'|','I','|', 

	ESC,ASTM_TX_ABORT_C_COMMENT_TEXT,	// ESC comment free text   

	'|',

	'I',								// always instrument flag   

	'\0'								// end of record template delimiter   
};



	// (P) No Query Results Patient Information Record: Abaxis to Host   

const	char	astmTxNoQueryResultsPrecord[] = 	
{
	'P','|','1','|',			
										
	ESC,ASTM_TX_NO_QUERY_RESULT_P_ID,	// ESC no query result patient or control ID    
										 
	'\0'								// end of record template delimiter   
};



	// (O) No Query Results Order Record: Abaxis to Host   

const	char	astmTxNoQueryResultsOrecord[] = 	
{
	'O','|','1','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|',

	'Z',								// report type (no record for query)   

	'\0'								// end of record template delimiter   
};



	// (C) No Query Results Comment Record: Abaxis to Host   

const	char	astmTxNoQueryResultsCrecord[] = 	
{
	'C','|','1','|','I','|','^','^',

	ESC,ASTM_TX_NO_QUERY_RESULT_C_TEXT,	// ESC no query result comment free text   

	'|',

	'G',								// always instrument flag   

	'\0'								// end of record template delimiter   
};



	// (C) ASTM 1394 Comment Record: Abaxis to Host (generic top level Comment message)  

const	char	astmTx1394Crecord[] = 	
{
	'C','|','1','|','I','|','^', 

	ESC,ASTM_TX_1394_C_CODE,			// ESC generic comment record type text char inserted if applicable   

	'^',								

	ESC,ASTM_TX_1394_C_TEXT,			// ESC generic comment free text   

	'|',

	'G',								// always instrument flag   

	'\0'								// end of record template delimiter   
};



	// (P) Order Data Invalid Patient Information Record: Abaxis to Host 

const	char	astmTxOrderDataInvalidPrecord[] = 	
{
	'P','|','1','|',							// 8.1.1 & 8.1.2			
										
	ESC,ASTM_TX_INVALID_P_PATIENT_CONTROL_ID,	// 8.1.3   ESC patient or control ID, practice    
										
	'|',

	ESC,ASTM_TX_INVALID_P_SAMPLE_ID,			// 8.1.4   ESC sample ID, laboratory ID

	'|',
	
	ESC,ASTM_TX_INVALID_P_ALTERNATE_ID,			// 8.1.5   ESC alternate ID, patient ID 3 

	'|','|','|',
	
	ESC,ASTM_TX_INVALID_P_BIRTHDATE,			// 8.1.8   ESC date of birth (DOB) 

	'|',
	
	ESC,ASTM_TX_INVALID_P_PATIENT_SEX,			// 8.1.9   ESC patient sex, gender   
										 
	'|',
	
	ESC,ASTM_TX_INVALID_P_PATIENT_RACE,			// 8.1.10  ESC patient race-ethnic origin   
										 
	'|','|','|',

	ESC,ASTM_TX_INVALID_P_PHONE_NUMBER,			// 8.1.13  ESC patient phone number
	
	'|','|', 

	ESC,ASTM_TX_INVALID_P_SPECIAL1,				// 8.1.15  ESC "Owner/Admission ID^Age^Last Vaccination (DOV)" string   
										 
	'|',

	ESC,ASTM_TX_INVALID_P_SPECIAL2,				// 8.1.16  ESC reference range label string   
										 
	'|','|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_INVALID_P_LOCATION,				// 8.1.26  ESC location (e.g. kennel number for VS 2)

	'\0'										// end of record template delimiter   
};



	// (O) Order Data Invalid Test Order Record: Abaxis to Host 

const	char	astmTxOrderDataInvalidOrecord[] = 	
{
	'O','|',									// 9.4.1 
	
	'1',										// 9.4.2			
										
	'|','|','|', 

	ESC,ASTM_TX_INVALID_O_ROTOR_NUMBER,			// 9.4.5   ESC universal test ID for rotor name & lot number, if known    
										 
	'|','|',

	ESC,ASTM_TX_INVALID_O_ORDER_DATE_TIME,		// 9.4.7   ESC requested/ordered date and time   

	'|','|','|','|','|',

	ESC,ASTM_TX_INVALID_O_ACTION_CODE,			// 9.4.12  ESC action code   

	'|','|','|','|','|',

	ESC,ASTM_TX_INVALID_O_PHYSICIAN_ID,			// 9.4.17  ESC physician ID entered by operator   

	'|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_INVALID_O_REPORT_TYPE,			// 9.4.26  ESC report type   

	'\0'										// end of record template delimiter   
};



	// (C) Order Data Invalid Comment Record: Abaxis to Host   

const	char	astmTxOrderDataInvalidCrecord[] = 	
{
	'C','|','1','|','I','|','^',				// 11.1.1, 11.1.2, 11.1.3, 11.1.4 component1 - not used 

	ESC,ASTM_TX_INVALID_C_CODE,					// 11.1.4 component2 ESC order data invalid error code - 32 bit hexadecimal   

	'^',								

	ESC,ASTM_TX_INVALID_C_TEXT,					// 11.1.4 component3 ESC order data invalid free text   

	'|',

	'G',										// 11.1.5 generic free text comment type   

	'\0'										// end of record template delimiter   
};



	// (P) Order Cancellation Patient Information Record: Abaxis to Host 

const	char	astmTxOrderCancellationPrecord[] = 	
{
	'P','|','1','|',							// 8.1.1 & 8.1.2			
										
	ESC,ASTM_TX_CANCEL_P_PATIENT_CONTROL_ID,	// 8.1.3   ESC patient or control ID, practice    
										
	'\0'										// end of record template delimiter   
};



	// (O) Order Data Invalid Test Order Record: Abaxis to Host 

const	char	astmTxOrderCancellationOrecord[] = 	
{
	'O','|',									// 9.4.1 
	
	'1',										// 9.4.2			
										
	'|','|','|','^','^','^',
	
	ESC,ASTM_TX_CANCEL_O_ROTOR_NUMBER,			// 9.4.5   ESC rotor number - battery code 
	
	'|','|',

	ESC,ASTM_TX_CANCEL_O_ORDER_DATE_TIME,		// 9.4.7   ESC requested/ordered date and time   

	'|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|','|',

	ESC,ASTM_TX_CANCEL_O_REPORT_TYPE,			// 9.4.26  ESC report type

	'\0'										// end of record template delimiter   
};



	// (C) Order Data Invalid Comment Record: Abaxis to Host   

const	char	astmTxOrderCancellationCrecord[] = 	
{
	'C','|','1','|','I','|','^',				// 11.1.1, 11.1.2, 11.1.3, 11.1.4 component1 

	ESC,ASTM_TX_CANCEL_C_CODE,					// 11.1.4 component2 ESC order cancellation code - character   

	'^',								

	ESC,ASTM_TX_CANCEL_C_TEXT,					// 11.1.4 component3 ESC order cancellation free text   

	'|',

	'G',										// 11.1.5 generic free text comment type   

	'\0'										// end of record template delimiter   
};



	// (M) Message Manufacturer Type Record: Abaxis to Host - M 1 AS analyzer status record   

const	char	astmTxMrecord[] =	
{
	'M','|','1','|',
										
	ESC,ASTM_TX_M_RECORD_IDENTIFIER, 			// 15.1.3 ESC Abaxis record identifier   
										
	'|',

	ESC,ASTM_TX_M_ANALYZER_STATUS,				// 15.1.4 ESC Abaxis analyzer status    
										 
	'\0'										// end of record template delimiter   
};



	// (M) Message Manufacturer Type Record: Abaxis to Host - M n PO pending orders record   

const	char	astmTxMPOrecord[] =	
{
	'M','|',
	
	ESC,ASTM_TX_M_SEQ_NUM,						// ESC manufacturer record sequence number
	
	'|',
										
	ESC,ASTM_TX_M_RECORD_IDENTIFIER, 			// 15.1.3 ESC Abaxis record identifier   
										
	'|',

	ESC,ASTM_TX_M_ORDER_DATE_TIME,				// 15.1.4 ESC Abaxis order record date time    
										 
	'|',

	ESC,ASTM_TX_M_ORDER_PATIENT_CONTROL_ID,		// 15.1.5 ESC Abaxis patient record patient or control id    
										 
	'|',

	ESC,ASTM_TX_M_ORDER_ROTOR_NUMBER,			// 15.1.6 ESC Abaxis order record rotor number    
										 
	'|',

	ESC,ASTM_TX_M_ORDER_LABORATORY_FIELD_1,		// 15.1.7 ESC Abaxis order record laboratory field 1    
										 
	'\0'										// end of record template delimiter   
};



	// table of pointers to astm transmit record (frame) templates   

const char*  astmTxRecordTemplatePtrs_m[NUM_ASTM_TX_RECORDS] =
{
	NULL,								// 0:  message done, no template
	 					  
	&astmTxHrecord[0],					// 1:  header       record template ptr 					   
	&astmTxLrecord[0],					// 2:  terminator   record template ptr 				  
	&astmTxM1record[0],					// 3:  manufacturer record template ptr - debug only 			  
	&astmTxPrecord[0],					// 4:  patient      record template ptr 					  
	&astmTxOrecord[0],					// 5:  order        record template ptr 					  
	&astmTxRrecord[0],					// 6:  results      record template ptr 					  
	&astmTxCrecord[0],					// 7:  comment      record template ptr 					  

	&astmTxAbortOrecord[0],				// 8:  abort order   record template ptr 				  
	&astmTxAbortCrecord[0],				// 9:  abort comment record template ptr			  

	&astmTxNoQueryResultsPrecord[0],	// 10: no query results patient record template ptr	  
	&astmTxNoQueryResultsOrecord[0],	// 11: no query results order   record template ptr	  
	&astmTxNoQueryResultsCrecord[0],	// 12: no query results comment record template ptr	  

	&astmTx1394Crecord[0],				// 13: astm 1394 comment record						  

	&astmTxOrderDataInvalidPrecord[0],	// 14: no query results patient record template ptr	  
	&astmTxOrderDataInvalidOrecord[0],	// 15: no query results order   record template ptr	  
	&astmTxOrderDataInvalidCrecord[0],	// 16: no query results comment record template ptr	  

	&astmTxOrderCancellationPrecord[0],	// 17: no query results patient record template ptr	  
	&astmTxOrderCancellationOrecord[0],	// 18: no query results order   record template ptr	  
	&astmTxOrderCancellationCrecord[0],	// 19: no query results comment record template ptr	  

	&astmTxMrecord[0],					// 20: manufacturer AS analyzer status record template ptr 			  
	&astmTxMPOrecord[0],				// 21: manufacturer PO pending order record template ptr 			  
};	



// Abaxis transmit messages definitions based on  ASTM  *** E 1394-97 ***  Records (frames)   

// Notes:

// 	1.	Each Abaxis transmit message is defined as a list of indices referenced to
//		astm_tx_record_template_ptrs[] which point to templates 
//		for record (frame) types used to format & construct astm transmit messages   

// 	2.	All Abaxis astm transmit messages are referenced using abaxis_astm_tx_data_to_send as index to
//		abaxis_astm_tx_msg_ptrs[] each of which point to the list of indices to
//		record (frame) pointers for that message   


	// table of indices used to lookup pointers to astm record (frame) templates   


	// abaxis tx test message definition   
	
const	char	astmTx1394CommentMsgDef[] =			
{
	ASTM_TX_HEADER,

	ASTM_TX_1394_COMMENT,

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx manufacturer type 1 message definition (used to loop back query parameters for test)   

const	char	astmTxManufacturerType1MsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_MANUFACTURER_TYPE1,

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx patient or control results / qc message definition   

const	char	astmTxResultsMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_PATIENT,			// P 1	  

	ASTM_TX_ORDER,				// O 1				(where 0 1  is rotor results order record 		  
	ASTM_TX_COMMENT,			// O 1: C 1			(where C 1  is comment for "INST QC: OK    CHEM: OK")   
	ASTM_TX_COMMENT,			// O 1: C 2			(where C 2  is comment for "HEM X ,  LIP Y ,  ICT Z")   
	ASTM_TX_COMMENT,			// O 1: C 3 		(where C 3  is comment for rotor if dilution verfication message required, else NULL comment)   		
	ASTM_TX_COMMENT,			// O 1: C 4			(where C 4  is comment for rotor if operator security status issue message, else NULL comment)   
	ASTM_TX_COMMENT,			// O 1: C 5			(where C 5  is comment for rotor if control expired condition, else NULL comment)   

	ASTM_TX_RESULT,				// O 1: R 1 to R N 	(where N    is number of analytes for rotor) 	   		
	ASTM_TX_COMMENT,			// O 1: R 1: C 1	(where C 1  is possible comment for a result)   

	ASTM_TX_RESULTS_REPEAT,		// analyte result: variable number R records with possible comment following   


	ASTM_TX_ORDER,				// O 2				(where 0 2  is rotor IQC record) 		  
	ASTM_TX_COMMENT,			// O 2: C 1   
	ASTM_TX_COMMENT,			// O 2: C 2   

	ASTM_TX_RESULT,				// O 2: R 1 to R 17 (R1 to R8 for VS 2)   
			
	ASTM_TX_RESULTS_REPEAT,		// QC result:  repeat until all 17 (all 8 for VS 2) IQC R records done   

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx patient or control abort rotor message definition   

const	char	astmTxAbortMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_PATIENT,			// P 1	  

	ASTM_TX_ABORT_ORDER,		// O 1 		  
	ASTM_TX_ABORT_COMMENT,		// O 1: C 1   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 2   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 3   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 4   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 5   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 6   
	ASTM_TX_ABORT_COMMENT,		// O 1: C 7   

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx no results for query message definition   

const	char	astmTxNoQueryResultsMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_NO_QUERY_RESULTS_PATIENT,	// P 1	  

	ASTM_TX_NO_QUERY_RESULTS_ORDER,		// O 1		  
	ASTM_TX_NO_QUERY_RESULTS_COMMENT,	// 0 1: C 1   

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx order data invalid message definition   

const	char	astmTxOrderDataInvalidMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_ORDER_DATA_INVALID_PATIENT,	// P 1

	ASTM_TX_ORDER_DATA_INVALID_ORDER,	// O 1		  
	ASTM_TX_ORDER_DATA_INVALID_COMMENT,	// 0 1: C 1   

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx order cancellation message definition   

const	char	astmTxOrderCancellationMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_ORDER_CANCELLATION_PATIENT,	// P 1

	ASTM_TX_ORDER_CANCELLATION_ORDER,	// O 1		  
	ASTM_TX_ORDER_CANCELLATION_COMMENT,	// 0 1: C 1   

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx manufacturer AS analyzer status message definition HML   

const	char	astmTxManufacturerMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_MANUFACTURER,				// M 1 - AS analyzer status

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};


	// abaxis tx manufacturer PO pending orders message definition HM...ML (up to 100 M)   

const	char	astmTxManufacturerPoMsgDef[] =	
{
	ASTM_TX_HEADER,

	ASTM_TX_MANUFACTURER_PO,			// M n - PO pending orders for n M n records
	ASTM_TX_MANUFACTURER_PO_REPEAT,

	ASTM_TX_TERMINATOR,
	ASTM_TX_MESSAGE_DONE
};



	// table of pointers to Abaxis astm transmit message record definition indices   

const char*  abaxisAstmTxMsgPtrs_m[NUM_ABAXIS_ASTM_TX_MSGS] =	// astmTxMsgType_m is idx   
{
	NULL,									// 0:  no msg to send							   
	&astmTx1394CommentMsgDef[0],			// 1:  tx astm 1394 generic comment msg					  
	&astmTxManufacturerType1MsgDef[0],		// 2:  tx manufacturer msg (for loop back test)						  
	&astmTxResultsMsgDef[0],				// 3:  tx patient or control results / iqc msg	  
	&astmTxAbortMsgDef[0],					// 4:  tx patient or control rotor abort msg	  
	&astmTxNoQueryResultsMsgDef[0],			// 5:  tx no query results available msg		  
	&astmTxOrderDataInvalidMsgDef[0],		// 6:  tx order data invalid msg		  
	&astmTxOrderCancellationMsgDef[0],		// 7:  tx order cancellation msg		  
	&astmTxManufacturerMsgDef[0],			// 8:  tx manufacturer msg (for AS analyzer status request)		  
	&astmTxManufacturerPoMsgDef[0],			// 9:  tx manufacturer msg (for PO pending order request)		  
};	



// ASTM  *** E 1394-97 ***  Record (frame) receive templates   

// Notes:

// 	1.	The NULL ('\0') in a template is the last char to indicate end of string,
//		and to signal DONT'T CARE what is text from this point to end of frame (<CR>,ETX) chars   

// 	2.	MAX fields in record is defined per ASTM 1394-97 standard   


	// (H) Message Header Record: Host to Abaxis   

const	char	astmRxHtemplate[] =
{
	14,								// max fields in record						  

	'H',							// 7.1.1 	record type								  
				
	ASTM_RX_H_FIELD_DELIMITER,		// 7.1.2 	extract "field delimiter" 	default '|'   
										
	ASTM_RX_H_REPEAT_DELIMITER,		// 7.1.2 	extract "repeat delimiter" 	default '\'   
										 
	ASTM_RX_H_COMPONENT_DELIMITER,	// 7.1.2 	extract "component delim."   default '^'   

	ASTM_RX_H_ESCAPE_DELIMITER, 	// 7.1.2 	extract "escape delimiter" 	default '&'   

	ASTM_RX_EMPTY_FIELD,			// 7.1.2	ignore field data -  as part of 7.1.2 logic					  

	ASTM_RX_H_MESSAGE_CONTROL_ID,	// 7.1.3	Message Control Id - HMAC key index					  

	ASTM_RX_H_ACCESS_PASSWORD, 		// 7.1.4	Access Password = HMAC computed value 					  

	ASTM_RX_H_SENDER_NAME_OR_ID, 	// 7.1.5	Sender Name or ID value
	 					  
	ASTM_RX_EMPTY_FIELD,			// 7.1.6	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 			// 7.1.7	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 			// 7.1.8	ignore field data
	 					  
	ASTM_RX_EMPTY_FIELD, 			// 7.1.9	ignore field data

	ASTM_RX_H_RECEIVER_ID,			// 7.1.10	analyzer serial number to verify (if NULL then no verification)
	 					  
	ASTM_RX_EMPTY_FIELD, 			// 7.1.11	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 			// 7.1.12	ignore field data
	 					  
	ASTM_RX_EMPTY_FIELD, 			// 7.1.13	ignore field data

	ASTM_RX_H_MESSAGE_DATE_TIME,	// 7.1.14	Date and Time of Message value 					  

	'\0'							// end of record template string 			  
};
										


	// (Q) Request Information Record: Host to Abaxis   

const	char	astmRxQtemplate[] =									
{
	13,									// max fields in record					  

	'Q',								// 12.1.1	record type						  
				
	ASTM_RX_FIELD_DELIMITER,			// 			match to current field delimiter		  

	ASTM_RX_Q_SEQUENCE_NUMBER,			// 12.1.2	extract Q record "sequence number" 	  
										
	ASTM_RX_Q_STARTING_RANGE_ID,		// 12.1.3	extract "starting range ID number" 	  
										 
	ASTM_RX_EMPTY_FIELD,				// 12.1.4	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 12.1.5	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 12.1.6	ignore field data 					  

	ASTM_RX_Q_BEGIN_RESULTS_DATE_TIME,	// 12.1.7	extract "begin result date and time"	  
										
	ASTM_RX_Q_END_RESULTS_DATE_TIME,	// 12.1.8	extract "end result date and time" 	  
										 
	'\0'								// end of record template string 		  
};
										

	// (L) Message Terminator Record: Host to Abaxis   

const	char	astmRxLtemplate[] =	
{
	4,								// max fields in record									  

	'L',							// 13.1.1	record type											  
				
	ASTM_RX_FIELD_DELIMITER,		// 			match to current field delimiter						  

	ASTM_RX_L_SEQUENCE_NUMBER,		// 13.1.2	extract L record "sequence number" - must match '1' 	  
										
	'\0'							// end of record template string 						  
};

									

	// (M) Manufacturer Information Record: Host to Abaxis   

const	char	astmRxMtemplate[] =	
{
	3,								// max fields in record									  

	'M',							// 15.1.1	record type											  
				
	ASTM_RX_FIELD_DELIMITER,		// 			match to current field delimiter		  

	ASTM_RX_M_SEQUENCE_NUMBER,		// 15.1.2	extract M record "sequence number"
	
	ASTM_RX_M_RECORD_IDENTIFIER,	// 15.1.3	extract record identifier = "AS" for analyzer status or "PO" for pending order- Abaxis defined 	  
										
	'\0'							// end of record template string 						  
};

										

	// (P) Patient Information Record: Host to Abaxis   

const	char	astmRxPtemplate[] =	
{
	35,									// max fields in record									  

	'P',								// 8.1.1	record type											  
				
	ASTM_RX_FIELD_DELIMITER,			// 			match to current field delimiter		  

	ASTM_RX_P_SEQUENCE_NUMBER,			// 8.1.2	extract P record "sequence number"
	
	ASTM_RX_P_PATIENT_CONTROL_ID,		// 8.1.3	Practice assigned patient ID

	ASTM_RX_P_SAMPLE_ID,				// 8.1.4    Laboratory assigned patient ID

	ASTM_RX_P_ALTERNATE_ID,				// 8.1.5	Patient ID No. 3

	ASTM_RX_EMPTY_FIELD,				// 8.1.6	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.7	ignore field data 					  

	ASTM_RX_P_BIRTHDATE,				// 8.1.8	Birthdate

	ASTM_RX_P_PATIENT_SEX,				// 8.1.9	Patient sex, gender

	ASTM_RX_P_PATIENT_RACE,				// 8.1.10	Patient race, ethnic origon

	ASTM_RX_EMPTY_FIELD,				// 8.1.11	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.12	ignore field data 					  

	ASTM_RX_P_PATIENT_PHONE_NUMBER,		// 8.1.13	Patient telephone number

	ASTM_RX_EMPTY_FIELD,				// 8.1.14	ignore field data 					  

	ASTM_RX_P_PATIENT_SPECIAL1,			// 8.1.15	Special Field 1 (Owner Admission ID^Age^Last Vaccination)

	ASTM_RX_P_PATIENT_SPECIAL2,			// 8.1.16	Special Field 2 (Reference Range Label)

	ASTM_RX_EMPTY_FIELD,				// 8.1.17	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.18	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 8.1.19	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.20	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 8.1.21	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.22	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 8.1.23	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.24	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 8.1.25	ignore field data 					  

	ASTM_RX_P_PATIENT_LOCATION,			// 8.1.26	Location (e.g. kennel number for VS2)

	'\0'								// end of record template string 						  
};

										

	// (O) Test Order Record: Host to Abaxis   

const	char	astmRxOtemplate[] =	
{
	31,									// max fields in record									  

	'O',								// 9.4.1	record type											  
				
	ASTM_RX_FIELD_DELIMITER,			// 			match to current field delimiter		  

	ASTM_RX_O_SEQUENCE_NUMBER,			// 9.4.2	extract O record "sequence number"
	
	ASTM_RX_EMPTY_FIELD,				// 9.4.3	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.4	ignore field data 					  

	ASTM_RX_O_ROTOR_NUMBER,				// 9.4.5	Universal test ID (^^^ Manufacturer's Valid Test Battery Code)

	ASTM_RX_EMPTY_FIELD,				// 9.4.6	ignore field data 					  

	ASTM_RX_O_ORDER_DATE_TIME, 			// 9.4.7	Requested/Ordered Date and Time (unique Test Order ID) 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.8	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.9	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.10	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.11	ignore field data 					  

	ASTM_RX_O_ACTION_CODE,				// 9.4.12	Action Code (C=cancel order; N=new order; Q=QC Test Sample)

	ASTM_RX_EMPTY_FIELD,				// 9.4.13	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.14	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.15	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.16	ignore field data 					  

	ASTM_RX_O_PHYSICIAN_ID,				// 9.4.17	Ordering Physician (Physician ID assigned by practice)

	ASTM_RX_EMPTY_FIELD,				// 9.4.18	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.19	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.20	ignore field data 					  

	ASTM_RX_O_LABORATORY_FIELD_1, 		// 9.4.21	Laboratory Field No. 1 (work list on analyzer display) 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.22	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.23	ignore field data 					  

	ASTM_RX_EMPTY_FIELD,				// 9.4.24	ignore field data 					  

	ASTM_RX_EMPTY_FIELD, 				// 9.4.25	ignore field data 					  

	ASTM_RX_O_REPORT_TYPE,				// 9.4.26	Report Types (always 'O' for LIS order)

	'\0'								// end of record template string 						  
};



	// (R) Result Record: Host to Abaxis   

const	char	astmRxRtemplate[] =	
{
	14,								// max fields in record									  

	'R',							// record type											  
				
	'\0'							// end of record template string 						  
};

										

	// (C) Comment Record: Host to Abaxis   

const	char	astmRxCtemplate[] =	
{
	5,								// max fields in record									  

	'C',							// record type											  
				
	'\0'							// end of record template string 						  
};

										

	// (S) Scientific Record: Host to Abaxis   

const	char	astmRxStemplate[] =	
{
	21,								// max fields in record									  

	'S',							// record type											  
				
	'\0'							// end of record template string 						  
};

										

	// table of pointers to astm receive record (frame) templates 	  
	//	 - scan thru to match second string char for record type being received 	  

const char*	astmRxRecordTemplatePtrs_m[NUM_RX_FRAME_TEMPLATES] =
{
	&astmRxHtemplate[0],			// header record template ptr 			   
	&astmRxQtemplate[0],			// query record template ptr 			  
	&astmRxLtemplate[0],			// terminator record template ptr 		  
	&astmRxMtemplate[0],			// manufacturer record template ptr 	  
	&astmRxPtemplate[0],			// patient record template ptr 			  
	&astmRxOtemplate[0],			// order record template ptr 			  
	&astmRxRtemplate[0],			// result record template ptr 			  
	&astmRxCtemplate[0],			// comment record template ptr 			  
	&astmRxStemplate[0],			// scientifc record template ptr 		  
};	


const char	maxAstmRxFieldSize_m[ASTM_RX_NUM_FIELD_METHODS] =	
{
	0,												// rx template delim. '/0' (not used)	  

	MAX_ASTM_RX_FIELD - 1,							// ASTM_RX_EMPTY_FIELD (generic ignore)	  
	0,												// ASTM_RX_FIELD_DELIMITER (generic)	  

	0,												// ASTM_RX_H_FIELD_DELIMITER 			  
	0,												// ASTM_RX_H_REPEAT_DELIMITER			  
	0,												// ASTM_RX_H_COMPONENT_DELIMITER		  
	0,												// ASTM_RX_H_ESCAPE_DELIMITER 			  

	1,												// ASTM_RX_Q_SEQUENCE_NUMBER   		 	  
	32,												// ASTM_RX_Q_STARTING_RANGE_ID 		 	  
	14,												// ASTM_RX_Q_BEGIN_RESULTS_DATE_TIME	  
	14,												// ASTM_RX_Q_END_RESULTS_DATE_TIME 	 	  

	1,												// ASTM_RX_L_SEQUENCE_NUMBER

	ASTM_RX_H_MESSAGE_CONTROL_ID_LEN,				// ASTM_RX_H_MESSAGE_CONTROL_ID
	ASTM_RX_H_ACCESS_PASSWORD_LEN,					// ASTM_RX_H_ACCESS_PASSWORD
	ASTM_RX_H_SENDER_NAME_OR_ID_LEN,				// ASTM_RX_H_SENDER_NAME_OR_ID
	ASTM_RX_H_RECEIVER_ID_LEN,						// ASTM_RX_H_RECEIVER_ID
	ASTM_RX_H_MESSAGE_DATE_TIME_LEN,				// ASTM_RX_H_MESSAGE_DATE_TIME

	ASTM_RX_P_SEQUENCE_NUMBER_LEN,					// ASTM_RX_P_SEQUENCE_NUMBER
	ASTM_RX_P_PATIENT_CONTROL_ID_LEN,				// ASTM_RX_P_PATIENT_CONTROL_ID
	ASTM_RX_P_SAMPLE_ID_LEN,						// ASTM_RX_P_SAMPLE_ID							
	ASTM_RX_P_ALTERNATE_ID_LEN,						// ASTM_RX_P_ALTERNATE_ID							
	ASTM_RX_P_BIRTHDATE_LEN,						// ASTM_RX_P_BIRTHDATE							
	ASTM_RX_P_PATIENT_SEX_LEN,						// ASTM_RX_P_PATIENT_SEX							
	ASTM_RX_P_PATIENT_RACE_LEN,						// ASTM_RX_P_PATIENT_RACE							
	ASTM_RX_P_PATIENT_PHONE_NUMBER_LEN,				// ASTM_RX_P_PATIENT_PHONE_NUMBER					
	ASTM_RX_P_PATIENT_SPECIAL1_LEN,					// ASTM_RX_P_PATIENT_SPECIAL1						
	ASTM_RX_P_PATIENT_SPECIAL2_LEN,					// ASTM_RX_P_PATIENT_SPECIAL2						
	ASTM_RX_P_PATIENT_LOCATION_LEN,					// ASTM_RX_P_PATIENT_LOCATION						

	ASTM_RX_O_SEQUENCE_NUMBER_LEN,					// ASTM_RX_O_SEQUENCE_NUMBER						
	ASTM_RX_O_ROTOR_NUMBER_LEN,						// ASTM_RX_O_ROTOR_NUMBER						
	ASTM_RX_ORDER_DATE_TIME_LEN,					// ASTM_RX_O_ORDER_DATE_TIME
	ASTM_RX_O_ACTION_CODE_LEN,						// ASTM_RX_O_ACTION_CODE							
	ASTM_RX_O_PHYSICIAN_ID_LEN,						// ASTM_RX_O_PHYSICIAN_ID							
	ASTM_RX_O_LABORATORY_FIELD_1_LEN,				// ASTM_RX_O_LABORATORY_FIELD_1							
	ASTM_RX_O_REPORT_TYPE_LEN,						// ASTM_RX_O_REPORT_TYPE						
	
	ASTM_RX_M_SEQUENCE_NUMBER_LEN,					// ASTM_RX_M_SEQUENCE_NUMBER					
	ASTM_RX_M_RECORD_IDENTIFIER_LEN,				// ASTM_RX_M_RECORD_IDENTIFIER					
};



// Abaxis allowed receive messages definitions based on  ASTM  *** E 1394-97 ***  Records (frames)   

// Notes:

// 	1.	Each Abaxis allowed receive message is defined as a list of record 1st characters 
//		for record (frame) types used to format & construct astm receive messages   

// 	2.	After all records up to terminator are received for message then the abaxis message
//		received is compared against the list(s)   

// 	3.	If no match then returned index is 0, else returns index to ptr of matched record chars
//		found in list   

	// tables of record first characters for allowed abaxis receive messages   


const	char	astmRxQMsgDef[] =			// rx abaxis Q (Query) message definition 					  

								"HQL" ;		// requires header, request information, terminator records   


const	char	astmRxMMsgDef[] =			// rx abaxis M (Manufacturer Informaton-Analyzer Status) message definition 					  

								"HML" ;		// requires header, request manufacturer information, terminator records   


const	char	astmRxPOMsgDef[] =			// rx abaxis PO (Patient Order-downloaded) message definition 					  

								"HPOL" ;	// requires header, patient record, order record, terminator records   


	// table of pointers to Abaxis allowed astm receive messages   

const char*	abaxisAstmRxMsgPtrs_m[NUM_ABAXIS_ASTM_RX_MSGS] =	
{
	&astmRxQMsgDef[0],						// 1: Query Abaxis message		  
	&astmRxMMsgDef[0],						// 2: Manufacturer Status Abaxis message		  
	&astmRxPOMsgDef[0],						// 3: Patient Order downloaded Abaxis message		  
};	



// ASTM constant strings

// Patient results - O1Cn record text strings - rotor analyte results

const char	qcOkTextString[] = 			     { "INST QC: OK    CHEM QC: OK" };	// 01C1

const char	hemTextString[] =			     { "HEM:" };						// 01C2
const char	lipTextString[] =			     { "LIP:" };						// 01C2
const char	ictTextString[] =			     { "ICT:" };						// 01C2

const char	rotorDiluteCheckFailedString[] = { "@ CONFIRM LOW RECOVERIES @" };	// 01C3


// IQC report header - O2 record name

const char	iqcReportHeaderTextString[]	=    { "* QUALITY CONTROL REPORT *" };	// O2

// IQC results - O2Cn record text strings

const char	chemQcTextString[] =		     { "CHEMISTRY QC:       " };		// O2C1
const char	acceptableMinTextString[] =		 { "ACCEPTABLE MINIMUM: " };		// O2C2


// IQC Results - R record text strings for IQC results for 02R1 to 02R17

const char	iqcAstmLevelNameStrings[17][19] =
{
	"^^^LEVEL 1: IQC 1 ",
	"^^^LEVEL 1: IQC 2 ",
	"^^^LEVEL 1: IQC 3 ",
	"^^^LEVEL 1: IQC 4 ",
	"^^^LEVEL 1: IQC 5 ",
	"^^^LEVEL 1: IQC 6 ",
	"^^^LEVEL 1: IQC 7 ",
	"^^^LEVEL 1: IQC 8 ",
	"^^^LEVEL 2: PRE   ",
	"^^^LEVEL 2: 340 nm",
	"^^^LEVEL 2: 405 nm",
	"^^^LEVEL 2: 467 nm",
	"^^^LEVEL 2: 500 nm",
	"^^^LEVEL 2: 515 nm",
	"^^^LEVEL 2: 550 nm",
	"^^^LEVEL 2: 600 nm",
	"^^^LEVEL 2: 630 nm",
};
											
const char	iqcLimitStringLevel1[] =	{ "90 to 110" };	// for 02R1 to O2R8
const char	iqcLimitStringLevel2[] =	{ "95 to 105" };	// for O2R9 to O2R17


// text strings defined for ASTM RnC1 records if errors in Rn record   

const char	flashRangeErrorString31[] =			{ "Flash Range Error" };
const char	divideByZeroString30[] =			{ "Div/0 In Calc (Math Error/Opaque)" };
const char	testBeadMixErrorString29[] =		{ "Test Bead Mix Error" };
const char	blankBeadMixErrorString28[] =		{ "Blank Bead Mix Error" };

const char	hemolyzedLimitErrorString27[] =		{ "Hemolysis Limit Error" };
const char	lipemicLimitErrorString26[] =		{ "Lipemic Limit Error" };
const char	ictericLimitErrorString25[] =		{ "Icteric Limit Error" };
const char	startingAbsBadString24[] =			{ "Starting Abs Out Of Range" };


const char	endingAbsBadString23[] =			{ "Ending Abs Out Of Range" };
const char	rateInEndpointString22[] =			{ "Rate In Endpoint, Bead or sample problem" };
const char	falseRateString21[] =				{ "False Rate (Unexpected Reaction Occurred)" };
const char	rateNotLinearString20[] =			{ "Rate Not Linear" };

const char	aboveSystemRangeString19[] =		{ "Concentration > High System Limit" };
const char	belowSystemRangeString18[] =		{ "Concentration < Low System Limit" };
const char	highDynamicRangeString17[] =		{ "Concentration > High Dynamic Limit" };
const char	lowDynamicRangeString16[] =			{ "Concentration < Low Dynamic Limit" };

const char	illegalLogParamString15[] =			{ "Illegal Log10 Parameter (Math Error/Opaque)" };
const char	inconsistentResultString14[] =		{ "Inconsistent Result (Cuvette values do not match)" };
const char	rqcErrorString13[] =				{ "RQC low (thermal stress on unstable reagent)" };
const char	rateTooLowString12[] =				{ "Rate too low" };

const char	chemBeadMissingString11[] =			{ "Bead missing" };
const char	chemDistErrorString10[] =			{ "Distribution check failed" };
const char	chemDiluteCheckFailedString09[] = 	{ "@ CONFIRM LOW RECOVERIES @" };
const char	chemUnusedErrorString08[] =			{ "Chemistry Unused Error" };

const char	chemSpecificErrorString07To00[] =	{ "Chemistry Specific Error" };


// strings defined for ASTM abort rotor O record C1, C2, C3, C4, C5 & C6   

// O1 C1   
const char	astmRunCountString[] =				{ "Run Count" };
const char	astmAbortCountString[] =			{ "Abort Count" };

// O1 C2   
const char	astmPrintCountString[] =			{ "Print Count" };
const char	astmFlashCountString[] =			{ "Flash Count" };

// O1 C3   
const char	astmDacTrimFlagsString[] =			{ "DAC Trim Flags" };
const char	astmOffsetErrFlagsString[] =		{ "Offset Error Flags" };
const char	astmSystemFlagsString[] =			{ "System Flags" };

// O1 C4   
const char	astmBeadCheck1String[] =			{ "Bead Check 1 Flags" };
const char	astmBeadCheck2String[] =			{ "Bead Check 2 Flags" };

// O1 C5   
const char	astmEmptyCuvFlagsString[] =			{ "Empty Cuvette Flags" };
const char	astmDistCheckFlagsString[] =		{ "Distribution Check Flags" };

// O1 C6   
const char	astmQcFlagsString[] =				{ "Quality Control Flags" };
const char	astmOffsetSdErrString[] =			{ "Offset SD Error Flags" };
const char	astmWavelengthCvString[] =			{ "Wavelength CV Flags" };



// string defined for ASTM no query results for O1 record C1   


// O1 C1   
const char	astmNoQueryResultString[] =			{ "No Results for this Query" };



// strings defined for ASTM 1394 generic comment message text strings   


const char	astm1394CommentStrings[NUM_ASTM_1394_STRINGS][ASTM_1394_STRING_SIZE] =
{
	"Power Up Initialization Sign On ",		// 0: ASTM_POWER_UP_SIGN_ON message   

	"Received Delimiter Mismatch     ",		// 1: ASTM_1394_RX_DELIMITER_MISMATCH message   

	"Received Field Size Too Big     ",		// 2: ASTM_1394_RX_FIELD_SIZE_TOO_BIG message   

	"Received BAD Q Record Sequence #",		// 3: ASTM_1394_RX_Q_SEQUENCE_NUM_BAD message   

	"Received BAD L Record Sequence #",		// 4: ASTM_1394_RX_L_SEQUENCE_NUM_BAD message   

	"Received Invalid Record Type   ",		// 5: ASTM_1394_RX_INVALID_RECORD_TYPE message   

	"Received > Max Fields in Record ",		// 6: ASTM_1394_RX_TOO_MANY_FIELDS_IN_RECORD message   

	"Received Record Out of Sequence ",		// 7: ASTM_1394_RX_RECORD_OUT_OF_SEQUENCE message   

	"Received Message NOT Supported  ",		// 8: ASTM_1394_RX_MESSAGE_NOT_SUPPORTED message   

	"Received BAD P Record Sequence #",		// 9: ASTM_1394_RX_P_SEQUENCE_NUM_BAD message   

	"Received BAD O Record Sequence #",		// 10: ASTM_1394_RX_O_SEQUENCE_NUM_BAD message   

	"Received BAD M Record Sequence #",		// 11: ASTM_1394_RX_M_SEQUENCE_NUM_BAD message   
};


// strings defined for order data invalid comment text for O1 record C1 

const char	astmOrderDataInvalidString[]   = { "Order data invalid" };


// strings defined for order cancellation comment text types for O1 record C1

const char	astmOrderCancellationXString[] = { "Order cannot be done, order cancelled" };
const char	astmOrderCancellationIString[] = { "In instrument, analysis started" };
const char	astmOrderCancellationYString[] = { "No order on record" };


#endif	// __ASTMCONSTANTS_H_INCLUDED






