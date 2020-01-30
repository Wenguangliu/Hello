// ********************************************************************
//
// FILENAME:  
// 
// 		Hl7Constants.h	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains constant definitions for the Hl7 protocol implementation
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//

#ifndef __HL7CONSTANTS_MSH_INCLUDED
#define __HL7CONSTANTS_MSH_INCLUDED

#include "HL7Configuration.h"
// HL7 transmit record ESC and receive record parameter method definitions       

// ASCII command characters

#define ACK										0x06 // Positive acknowledgement
#define CR										0x0d // Carriage return
#define NAK										0x15 // Negative acknowledgement
#define DEL										0x7f // DEL char

// Message status/error codes
#define APPLICATION_ACCEPT						"AA" // Used in MSA-1
#define APPLICATION_ERROR						"AE" // Used in MSA-1
#define APPLICATION_REJECT						"AR" // Used in MSA-1

//Response message types
#define MSH3_RESULT_QUERY_QRY					"QRY" // Used in MSH-9
#define MSH3_RESULT_QUERY_R02					"R02" // Used in MSH-9
#define MSH3_RESULT_RESPONSE_ORF				"ORF" // Used in MSH-9
#define MSH3_RESULT_RESPONSE_R04				"R04" // Used in MSH-9
#define MSH3_RESULT_UNSOLICITED_RESPONSE_ORU	"ORU" // Used in MSH-9
#define MSH3_RESULT_UNSOLICITED_RESPONSE_R01	"R01" // Used in MSH-9

#define SUPPORTED_HL7_VERSION					"2.3"
	
#define	MSH_RECORD								"MSH" // Message header record
#define	QRD_RECORD								"QRD" // Query record

//Default delimiters used in template definitions
#define TEMPLATE_FIELD_DELIMITER				'|'
#define TEMPLATE_COMPONENT_DELIMITER			'^'
#define TEMPLATE_REPEAT_DELIMITER				'~'
#define TEMPLATE_ESCAPE_CHARACTER				'\\'
#define TEMPLATE_SUB_COMPONENT_DELIMITER		'&'




// tx ESC method definitions       

typedef enum
{
	HL7_MSH_FIELD_DELIMITER				 = 1,	// MSH header record methods
	HL7_MSH_COMPONENT_DELIMITER,
	HL7_MSH_REPEAT_DELIMITER,
	HL7_MSH_ESCAPE_CHARACTER,
	HL7_MSH_SUB_COMPONENT_DELIMITER,
	HL7_MSH_ANALYZER_TYPE,
	HL7_MSH_SOFTWARE_VERSION,
	HL7_MSH_SERIAL_NUMBER,
	HL7_MSH_SENDING_APPLICATION,
	HL7_MSH_SENDING_FACILITY,
	HL7_MSH_RECEIVING_APPLICATION, 
	HL7_MSH_RECEIVING_FACILITY,
	HL7_MSH_DATE_TIME_OF_MESSAGE,
	HL7_MSH_SECURITY,
	HL7_MSH_MESSAGE_TYPE,
	HL7_MSH_MESSAGE_CONTROL_ID,
	HL7_MSH_PROCESSING_ID,
	HL7_MSH_VERSION_ID,		

	HL7_PID_SET_ID,								// PID patient record methods
	HL7_PID_EXTERNAL_ID,
	HL7_PID_INTERNAL_ID,
	HL7_PID_ALTERNATE_ID,
	HL7_PID_PATIENT_NAME,
	HL7_PID_BIRTHDATE,				
	HL7_PID_PATIENT_SEX,
	HL7_PID_PATIENT_RACE,
	HL7_PID_PHONE_NUMBER,

	HL7_PV1_SET_ID,
	HL7_PV1_PATIENT_CLASS,
	HL7_PV1_ASSIGNED_LOCATION,
	HL7_PV1_ADMITTING_DOCTOR,

	HL7_OBR_SET_ID,								// OBR order record methods       	
	HL7_OBR_ROTOR_IQC_NAME,				
	HL7_OBR_LOT_NUM,						
	HL7_OBR_REQUESTED_DATE_TIME,
	HL7_OBR_OBSERVATION_START_TIME,
	HL7_OBR_OBSERVATION_END_TIME,
	HL7_OBR_COLLECTOR_ID,			
	HL7_OBR_PHYSICIAN_ID,
	HL7_OBR_RESULT_STATUS,

	HL7_OBX_SET_ID,								// OBX result record methods       	
	HL7_OBX_ID,					
	HL7_OBX_VALUE,							
	HL7_OBX_UNITS,				
	HL7_OBX_REFERENCE_RANGE,				
	HL7_OBX_ABNORMAL_FLAG,
	HL7_OBX_RESULT_STATUS,
	HL7_OBX_OBSERVATION_METHOD,
	HL7_OBX_SETUP_NEXT,

	HL7_NTE_SET_ID,								// NTE comment record methods - result
	HL7_NTE_COMMENT_RESULT_RAW,		
	HL7_NTE_COMMENT_CODE,					
	HL7_NTE_COMMENT_TEXT,					
	HL7_NTE_COMMENT_TYPE,

	HL7_QRD_DATE_TIME,							// QRD record methods - handled as generic field strings       
	HL7_QRD_FORMAT_CODE,			
	HL7_QRD_PRIORITY,
	HL7_QRD_ID,
	HL7_QRD_DEFERRED_RESPONSE_TYPE,
	HL7_QRD_DEFERRED_RESPONSE_DATE,
	HL7_QRD_QUALITY_REQUEST,
	HL7_QRD_WHO_SUBJECT_FILTER,
	HL7_QRD_WHAT_SUBJECT_FILTER,
	HL7_QRD_WHAT_DEPT_DATA_CODE,
	HL7_QRD_WHAT_DEPT_VALUE_QUAL,

	HL7_ABORT_OBR_ROTOR_NAME_LOT,				// abort rotor record methods
	HL7_ABORT_OBR_SETUP,							       		
	HL7_ABORT_NTE_COMMENT_TEXT,	

	HL7_NO_QUERY_RESULT_PID_ID,					// no query results record methods       
	HL7_NO_QUERY_RESULT_NTE_TEXT,		

	HL7_NTE_CODE,								// hl7 comment record methods       
	HL7_NTE_TEXT,

	HL7_MSA_ACK_CODE,							// Message acknowledge record methods
	HL7_MSA_MSG_CONTROL_ID,
	HL7_MSA_TEXT_MSG,
	HL7_MSA_ERROR_CONDITION,
	HL7_OBR_SETUP_RESULTS,

	HL7_RX_FIELD_DELIMITER
} Hl7EscMethod_t;

typedef struct {
	Hl7EscMethod_t method;
	int size;
}rxIndex;

#define MAX_RX_SIZE_FIELDS			18	
static const rxIndex hl7RxFieldSizes [MAX_RX_SIZE_FIELDS] =	{
		{ HL7_MSH_SENDING_APPLICATION,		180 },
		{ HL7_MSH_SENDING_FACILITY,			180 },
		{ HL7_MSH_RECEIVING_APPLICATION,	180 },
		{ HL7_MSH_RECEIVING_FACILITY,		180 },
		{ HL7_MSH_DATE_TIME_OF_MESSAGE,		 26 },
		{ HL7_MSH_SECURITY,					 40 },
		{ HL7_MSH_MESSAGE_TYPE,				  8 },
		{ HL7_MSH_PROCESSING_ID,			  3 },
		{ HL7_MSH_MESSAGE_CONTROL_ID,		 20 },
		{ HL7_MSH_VERSION_ID,				  8 },
		{ HL7_QRD_DATE_TIME,				 26 },
		{ HL7_QRD_FORMAT_CODE,				  1 },
		{ HL7_QRD_PRIORITY,					  1 },
		{ HL7_QRD_ID,						 10 },
		{ HL7_QRD_WHO_SUBJECT_FILTER,		 60 },
		{ HL7_QRD_WHAT_SUBJECT_FILTER,		 60 },
		{ HL7_QRD_WHAT_DEPT_DATA_CODE,		 60 },
		{ HL7_QRD_WHAT_DEPT_VALUE_QUAL,		 20 }
};

//       rx record parameter method definitions       
#define	MAX_HL7_RX_FIELD					180		// maximum length of temp hl7 rx field string       	


//       HL7 tx record (frame) template indices & definitions       
typedef enum
{
	HL7_TX_MESSAGE_DONE,							// index indicates tx message setup done, so no template       
	HL7_TX_HEADER,									// index to tx header record template ptr       
	HL7_TX_MSG_ACK,									// index to tx acknowledge record
	HL7_TX_QUERY,									// index to tx query record

	HL7_TX_PATIENT,									// index to tx patient record template ptr       
	HL7_TX_PATIENT_VISIT,							// index to tx patient visit record template ptr
	HL7_TX_ORDER,									// index to tx test order record template ptr       
	HL7_TX_RESULT,									// index to tx result record template ptr       

	HL7_TX_COMMENT,									// index to tx comment record template ptr       

	HL7_TX_ABORT_ORDER,								// index to tx abort order record template ptr       
	HL7_TX_ABORT_COMMENT,							// index to tx abort comment record template ptr       

	HL7_TX_NO_QUERY_RESULTS_PATIENT,				// index to tx no query results patient record template ptr       
	HL7_TX_NO_QUERY_RESULTS_ORDER,					// index to tx no query results test order record template ptr       
	HL7_TX_NO_QUERY_RESULTS_COMMENT,				// index to tx no query results test comment record template ptr       

	HL7_TX_GENERIC_COMMENT,							// index to tx no query results test comment record template ptr       

	HL7_TX_RESULTS_REPEAT = 0x7f,					// flags repeat next results/comment, no template       
} Hl7TxRecordType_t;

#define NUM_HL7_TX_RECORDS				15			// number of hl7 tx records (frame types) defined       
#define LAST_TX_IQC_OBX_RECORD_XPRESS	17			// flags repeat next results/comment, no template xPress       
#define LAST_TX_IQC_OBX_RECORD_VS2		8			// flags repeat next results/comment, no template VS2       


//       HL7 rx record (frame) template indices & definitions       

#define NUM_RX_FRAME_TEMPLATES			2			// number of hl7 rx frame templates in ptr array       

#define NUM_ABAXIS_HL7_RX_MSGS			1			// number of abaxis hl7 receive messages 				      		


//       HL7 Abaxis type message definitions       

//       abaxis transmit msg type definitions       

typedef enum
{
	HL7_TX_NO_MSGS,							// hl7TxMsgType_m: no message to send			      
	HL7_TX_COMMENT_MSG,						// hl7TxMsgType_m: comment message HCL			      
	HL7_TX_RESULTS_MSG,						// hl7TxMsgType_m: abaxis query results msg  		      
	HL7_TX_UNSOLICITED_RESULTS_MSG,			// hl7TxMsgType_m: abaxis unsolicited results msg  		      
	HL7_TX_ABORT_MSG,						// hl7TxMsgType_m: abaxis abort rotor error msg  	      
	HL7_TX_NO_QUERY_RESULTS_MSG,			// hl7TxMsgType_m: abaxis no query results msg
	HL7_TX_ERROR_MSG
} Hl7TxMsgType_t;

#define NUM_ABAXIS_HL7_TX_MSGS			7			// number of abaxis hl7 transmit message types (includes 0: no msg       


// HL7 comment strings defintions

#define NUM_HL7_STRINGS			7			// number of hl7 comment message text strings        
#define HL7_STRING_SIZE			(32+1)		// size   of hl7 comment message text strings       


// HL7  ***  ***  Record (frame) transmit templates 

// Notes:

// 	1.	The only NULL ('\0') in a record is the last char to indicate end of record text 
//		and to signal insertion of CR ('\r') record terminator at end of frame   

// 	2.	ESC char ('\x1b') signals for insertion of ASCII text based on character value
//		immediately after the ESC (from 0 '\x00' to  255 '\xff') which 
//		incdicates ESC method for variable text insertion   

//	3.	ESC methods are separated as single lines from and concatenated to text strings
//		in the record definitions below   


	// (MSH) Message Header Record: Abaxis to Host   

char	hl7TxMSHrecord[] = 	
{
	'M','S','H',
	ESC,HL7_MSH_FIELD_DELIMITER,			//MSH-1 Field delimiter  '|',
	ESC,HL7_MSH_COMPONENT_DELIMITER,		//MSH-2 Component delimiter  '^',
	ESC,HL7_MSH_REPEAT_DELIMITER,			//MSH-2 Field repeat delimiter  '~',
	ESC,HL7_MSH_ESCAPE_CHARACTER,			//MSH-2 escape chanracter  '\\',
	ESC,HL7_MSH_SUB_COMPONENT_DELIMITER,	//MSH-2 sub-component delimiter  '&',
	TEMPLATE_FIELD_DELIMITER,
	'A','B','A','X','I','S',',',' ','I','N','C','.', // MSH 3 - Sending application
	TEMPLATE_COMPONENT_DELIMITER,			
	ESC,HL7_MSH_ANALYZER_TYPE,				// MSH-3 Analyzer   
	TEMPLATE_COMPONENT_DELIMITER, 
	ESC,HL7_MSH_SOFTWARE_VERSION,			// MSH-3 Software Version   								 
	TEMPLATE_COMPONENT_DELIMITER, 
	ESC,HL7_MSH_SERIAL_NUMBER,				// MSH-3 Serial Number   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,				// MSH-4 Sending facility
	TEMPLATE_FIELD_DELIMITER,				// MSH-5 Receiving application
	TEMPLATE_FIELD_DELIMITER,				// MSH-6 Receiving facility
	ESC,HL7_MSH_DATE_TIME_OF_MESSAGE,		// MSH-7 ESC Date and Time of Message   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,				// MSH-8 Security
	ESC,HL7_MSH_MESSAGE_TYPE,				// MSH-9 ESC Type of message
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_MSH_MESSAGE_CONTROL_ID,			// MSH-10 ESC Unique identifier
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_MSH_PROCESSING_ID,				// MSH-11 ESC Processing ID   (debug, production)
	TEMPLATE_FIELD_DELIMITER,
	'2','.','3',							// HL7 Version ID (2.3)
	TEMPLATE_FIELD_DELIMITER,
	'\0'									// end of record template delimiter   
};



	// (PID) Patient Information Record: Abaxis to Host   

const char	hl7TxPIDrecord[] = 	
{
	'P','I','D',
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_SET_ID,				// PID-1 Transaction ID
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_EXTERNAL_ID,		// PID-2 External patient ID
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_INTERNAL_ID,		// PID-3 Internal patient ID
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_ALTERNATE_ID,		// PID-4 Alternate patient ID
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_PATIENT_NAME,		// PID-5 Patient name 
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// PID-6 Mothers's maiden name
	ESC,HL7_PID_BIRTHDATE,			// PID-7 Date/time birth date
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PID_PATIENT_SEX,		// PID-8 Sex
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// PID-9 Patient alias
	ESC,HL7_PID_PATIENT_RACE,		// PID-10 Race
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// PID-11 Patient address
	TEMPLATE_FIELD_DELIMITER,		// PID-12 Country code
	ESC,HL7_PID_PHONE_NUMBER,		// PID-13 Patient phone number
	TEMPLATE_FIELD_DELIMITER,
	'\0'							// end of record template delimiter   
};

const char	hl7TxPV1record[] = 	
{
	'P','V','1',
	TEMPLATE_FIELD_DELIMITER,
	'1',							// PV1-1 Set ID
	TEMPLATE_FIELD_DELIMITER,
	'O',							// PV1-2 Patient class
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_PV1_ASSIGNED_LOCATION,	// PV1-3 Assigned location
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// PV1-4 Admission type
	TEMPLATE_FIELD_DELIMITER,		// PV1-5 Preadmit Number
	TEMPLATE_FIELD_DELIMITER,		// PV1-6 Prior patient location
	TEMPLATE_FIELD_DELIMITER,		// PV1-7 Attending doctor
	TEMPLATE_FIELD_DELIMITER,		// PV1-8 Reffering doctor
	TEMPLATE_FIELD_DELIMITER,		// PV1-9 Consulting doctor
	TEMPLATE_FIELD_DELIMITER,		// PV1-10 Hospital service
	TEMPLATE_FIELD_DELIMITER,		// PV1-11 Temp location
	TEMPLATE_FIELD_DELIMITER,		// PV1-12 Preadmit test indicator
	TEMPLATE_FIELD_DELIMITER,		// PV1-13 Readmission indicator
	TEMPLATE_FIELD_DELIMITER,		// PV1-14 Admit source
	TEMPLATE_FIELD_DELIMITER,		// PV1-15 Ambulatory status
	TEMPLATE_FIELD_DELIMITER,		// PV1-16 VIP Indicator
	TEMPLATE_FIELD_DELIMITER,		// PV1-17 Admitting doctor
	TEMPLATE_FIELD_DELIMITER,		// PV1-18 Patient Type
	ESC,HL7_PV1_ADMITTING_DOCTOR,	// PV1-19 Visit number
	TEMPLATE_FIELD_DELIMITER,
	'\0'							// end of record template delimiter   
};



	// (OBR) Test Order Record: Abaxis to Host   

const char	hl7TxOBRrecord[] = 	
{
	'O','B','R',
	TEMPLATE_FIELD_DELIMITER,									
	ESC,HL7_OBR_SET_ID,					// OBR-1 Order record sequence number    							
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-2 Placer order number 
	TEMPLATE_FIELD_DELIMITER,			// OBR-3 Filler order number 
	TEMPLATE_COMPONENT_DELIMITER,		// OBR-4 Preamble, e.g. ^^^BASIC METABOLIC .... 
	TEMPLATE_COMPONENT_DELIMITER,
	TEMPLATE_COMPONENT_DELIMITER,
	ESC,HL7_OBR_ROTOR_IQC_NAME,			// OBR-4 Universal test ID rotor name or IQC header   
	':',' ',							// OBR-4 Separator before the lot number
	ESC,HL7_OBR_LOT_NUM,				// OBR-4 Lot number   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-5 Priority
	ESC,HL7_OBR_REQUESTED_DATE_TIME,	// OBR-6 Requested date time  								 
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBR_OBSERVATION_START_TIME,	// OBR-7 Observation Date/Time
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBR_OBSERVATION_END_TIME,	// OBR-8 Observation End Date/Time
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-9 Collection volume
	ESC,HL7_OBR_COLLECTOR_ID,			// OBR-10 Collector identifier
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-11 Specimen Action Code
	TEMPLATE_FIELD_DELIMITER,			// OBR-12 Danger Code
	TEMPLATE_FIELD_DELIMITER,			// OBR-13 Relevant Clinical Info
	TEMPLATE_FIELD_DELIMITER,			// OBR-14 Specimen Received Date/Time
	TEMPLATE_FIELD_DELIMITER,			// OBR-15 Specimen source
	ESC,HL7_OBR_PHYSICIAN_ID,			// OBR-16 Ordering provider   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-17 Specimen source
	TEMPLATE_FIELD_DELIMITER,			// OBR-18 Placer field 1
	TEMPLATE_FIELD_DELIMITER,			// OBR-19 Placer field 2
	TEMPLATE_FIELD_DELIMITER,			// OBR-20 Filler field 1
	TEMPLATE_FIELD_DELIMITER,			// OBR-21 Filler field 2
	TEMPLATE_FIELD_DELIMITER,			// OBR-22 Results Rpt/Status Chng - Date/Time
	TEMPLATE_FIELD_DELIMITER,			// OBR-23 Charge to Practice
	TEMPLATE_FIELD_DELIMITER,			// OBR-24 Diagnostic Serv Sect ID
	ESC,HL7_OBR_RESULT_STATUS,			// OBR-25 Result Status
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBR_SETUP_RESULTS,			// ESC setup results for order   
	'\0'								// end of record template delimiter   
};



	// (OBX) Result Record: Abaxis to Host   

const char	hl7TxOBXrecord[] = 	
{
	'O','B','X',
	TEMPLATE_FIELD_DELIMITER,								
	ESC,HL7_OBX_SET_ID,				// OBX-1 Result record sequence number    								
	TEMPLATE_FIELD_DELIMITER, 
	TEMPLATE_FIELD_DELIMITER,		// OBX-2 Value type
	ESC,HL7_OBX_ID,					// OBX-3 Observation ID  
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// OBX-4 Observation sub ID
	ESC,HL7_OBX_VALUE,				// OBX-5 Result value, same resolution as print card   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBX_UNITS,				// OBX-6 Units using HL7 & ISO standard   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBX_REFERENCE_RANGE,	// OBX-7 Result reference range, same resolution as print card   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBX_ABNORMAL_FLAG,		// OBX-8 Result abnormal flags   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// OBX-9 Probability
	TEMPLATE_FIELD_DELIMITER,		// OBX-10 Nature of abnormal test
	ESC,HL7_OBX_RESULT_STATUS,		// OBX-11 Observation Result Status
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// OBX-12 Date Last Obs Normal Values
	TEMPLATE_FIELD_DELIMITER,		// OBX-13 User Defined Access Checks
	TEMPLATE_FIELD_DELIMITER,		// OBX-14 Date/Time of the Observation
	TEMPLATE_FIELD_DELIMITER,		// OBX-15 Producer's ID
	TEMPLATE_FIELD_DELIMITER,		// OBX-16 Responsible Observer
	ESC,HL7_OBX_OBSERVATION_METHOD,	// OBX-17 Observation Method
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBX_SETUP_NEXT,			// ESC setup for potential comment & next result   

	'\0'							// end of record template delimiter   
};



	// (NTE) Comment Record: Abaxis to Host   

const char	hl7TxNTErecord[] = 	
{
	'N','T','E',
	TEMPLATE_FIELD_DELIMITER,												
	ESC,HL7_NTE_SET_ID,				// NTE-1 Comment record sequence number    
	TEMPLATE_FIELD_DELIMITER,
	'I',							// NTE-2 Source of comment )only from analyser = I)
	TEMPLATE_FIELD_DELIMITER, 
	ESC,HL7_NTE_COMMENT_RESULT_RAW,	// NTE-3 Comment code (result, raw format)   
	TEMPLATE_COMPONENT_DELIMITER,
	ESC,HL7_NTE_COMMENT_CODE,		// NTE-3 Comment code (result_flags, error code)   
	TEMPLATE_COMPONENT_DELIMITER,
	ESC,HL7_NTE_COMMENT_TEXT,		// NTE-3 Comment free text   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_NTE_COMMENT_TYPE,		// NTE-4 comment type
	TEMPLATE_FIELD_DELIMITER,
	'\0'							// end of record template delimiter   
};

	// (MSA) Message Ackowledgment Record: Abaxis to Host   

const char	hl7TxMSArecord[] = 	
{
	'M','S','A',
	TEMPLATE_FIELD_DELIMITER,												
	ESC,HL7_MSA_ACK_CODE,			// MSA-1 Comment record sequence number    
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_MSA_MSG_CONTROL_ID,		// MSA-2 Comment code (result, raw format)   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_MSA_TEXT_MSG,			// MSA-3 Comment code (result_flags, error code)   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,		// MSA-4 Expected Sequence Number
	TEMPLATE_FIELD_DELIMITER,		// MSA-5 Delayed Acknowldge Type
	ESC,HL7_MSA_ERROR_CONDITION,	// MSA-6 Further specify if error		
	TEMPLATE_FIELD_DELIMITER,
	'\0'							// end of record template delimiter   
};



	// (OBR) Abort Order Record: Abaxis to Host   

const char	hl7TxAbortOBRrecord[] = 	
{
	'O','B','R',
	TEMPLATE_FIELD_DELIMITER,									
	ESC,HL7_OBR_SET_ID,					// OBR-1 Order record sequence number    							
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-2 Placer order number 
	TEMPLATE_FIELD_DELIMITER,			// OBR-3 Filler order number 
	ESC,HL7_ABORT_OBR_ROTOR_NAME_LOT,	// OBR-4 Universal test ID rotor name
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-5 Priority
	TEMPLATE_FIELD_DELIMITER,			// OBR-6 Requested date/time   								 
	ESC,HL7_OBR_OBSERVATION_START_TIME,	// OBR-7 Observation Date/Time
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBR_OBSERVATION_END_TIME,	// OBR-8 Observation End Date/Time
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-9 Collection volume
	ESC,HL7_OBR_COLLECTOR_ID,			// OBR-10 Collector identifier
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-11 Specimen Action Code
	TEMPLATE_FIELD_DELIMITER,			// OBR-12 Danger Code
	TEMPLATE_FIELD_DELIMITER,			// OBR-13 Relevant Clinical Info
	TEMPLATE_FIELD_DELIMITER,			// OBR-14 Specimen Received Date/Time
	TEMPLATE_FIELD_DELIMITER,			// OBR-15 Specimen source
	ESC,HL7_OBR_PHYSICIAN_ID,			// OBR-16 Ordering provider   
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// OBR-17 Specimen source
	TEMPLATE_FIELD_DELIMITER,			// OBR-18 Placer field 1
	TEMPLATE_FIELD_DELIMITER,			// OBR-19 Placer field 2
	TEMPLATE_FIELD_DELIMITER,			// OBR-20 Filler field 1
	TEMPLATE_FIELD_DELIMITER,			// OBR-21 Filler field 2
	TEMPLATE_FIELD_DELIMITER,			// OBR-22 Results Rpt/Status Chng - Date/Time
	TEMPLATE_FIELD_DELIMITER,			// OBR-23 Charge to Practice
	TEMPLATE_FIELD_DELIMITER,			// OBR-24 Diagnostic Serv Sect ID
	'Z',								// OBR-25 Result Status
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_OBR_SETUP_RESULTS,			// ESC setup results for order   
	'\0'								// end of record template delimiter   
};



	// (C) Abort Comment Record: Abaxis to Host   

const char	hl7TxAbortNTErecord[] = 	
{
	'N','T','E',
	TEMPLATE_FIELD_DELIMITER,												
	ESC,HL7_NTE_SET_ID,					// NTE-1 Comment record sequence number    
	TEMPLATE_FIELD_DELIMITER,
	'I',								// NTE-2 Source of comment )only from analyser = I)
	TEMPLATE_FIELD_DELIMITER, 
	ESC,HL7_ABORT_NTE_COMMENT_TEXT,		// NTE-3 Comment code (result, raw format)   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_NTE_COMMENT_TYPE,			// NTE-4 comment type
	TEMPLATE_FIELD_DELIMITER,
	'\0'								// end of record template delimiter   
};



	// (P) No Query Results Patient Information Record: Abaxis to Host   
const char	hl7TxNoQueryResultsPIDrecord[] = 	
{
	'P','I','D',
	TEMPLATE_FIELD_DELIMITER,
	'1',								// PID-1 Transaction ID
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// PID-2 External patient ID
	ESC,HL7_NO_QUERY_RESULT_PID_ID,		// PID-3 no query result patient or control ID
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_FIELD_DELIMITER,			// PID-4 Alternate patient ID
	TEMPLATE_FIELD_DELIMITER,			// PID-5 Patient name 
	TEMPLATE_FIELD_DELIMITER,			// PID-6 Mothers's maiden name
	TEMPLATE_FIELD_DELIMITER,			// PID-7 Date/time birth date
	TEMPLATE_FIELD_DELIMITER,			// PID-8 Sex
	TEMPLATE_FIELD_DELIMITER,			// PID-9 Patient alias
	TEMPLATE_FIELD_DELIMITER,			// PID-10 Race
	TEMPLATE_FIELD_DELIMITER,			// PID-11 Patient address
	TEMPLATE_FIELD_DELIMITER,			// PID-12 Country code
	TEMPLATE_FIELD_DELIMITER,			// PID-13 Patient phone number
	'\0'								// end of record template delimiter   
};



	// (O) No Query Results Order Record: Abaxis to Host   
const char hl7TxNoQueryResultsOBXrecord[] = 	
{
	'O','B','X',
	TEMPLATE_FIELD_DELIMITER,								
	'1',								// OBX-1 Result record sequence number    								
	TEMPLATE_FIELD_DELIMITER,			// OBX-2 Value type
	TEMPLATE_FIELD_DELIMITER,			// OBX-3 Observation ID  
	TEMPLATE_FIELD_DELIMITER,			// OBX-4 Observation sub ID
	TEMPLATE_FIELD_DELIMITER,			// OBX-5 Result value, same resolution as print card   
	TEMPLATE_FIELD_DELIMITER,			// OBX-6 Units using HL7 & ISO standard   
	TEMPLATE_FIELD_DELIMITER,			// OBX-7 Result reference range, same resolution as print card   
	TEMPLATE_FIELD_DELIMITER,			// OBX-8 Result abnormal flags   
	TEMPLATE_FIELD_DELIMITER,			// OBX-9 Probability
	TEMPLATE_FIELD_DELIMITER,			// OBX-10 Nature of abnormal test
	TEMPLATE_FIELD_DELIMITER,			// OBX-11 Observation Result Status
	TEMPLATE_FIELD_DELIMITER,			// ESC setup for potential comment & next result   

	'\0'								// end of record template delimiter   
};


	// (C) No Query Results Comment Record: Abaxis to Host   
const char	hl7TxNoQueryResultsNTErecord[] = 	
{
	'N','T','E',
	TEMPLATE_FIELD_DELIMITER,												
	'1',								// NTE-1 Comment record sequence number    
	TEMPLATE_FIELD_DELIMITER,
	'I',								// NTE-2 Source of comment )only from analyser = I)
	TEMPLATE_FIELD_DELIMITER, 
	ESC,HL7_NO_QUERY_RESULT_NTE_TEXT,	// NTE-3 Comment code (result, raw format)   
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_NTE_COMMENT_TYPE,			// NTE-4 comment type
	TEMPLATE_FIELD_DELIMITER,
	'\0'								// end of record template delimiter   
};


	// (C) HL7 Comment Record: Abaxis to Host (generic top level Comment message)  
const char	hl7TxGenericNTErecord[] = 	
{
	'N','T','E',
	TEMPLATE_FIELD_DELIMITER,
	'1',								// NTE-1 Comment record sequence number
	TEMPLATE_FIELD_DELIMITER,
	'I',								// NTE-2 Source of comment )only from analyser = I)
	TEMPLATE_FIELD_DELIMITER,
	TEMPLATE_COMPONENT_DELIMITER, 
	ESC,HL7_NTE_CODE,					// NTE-3 Generic comment record type text char inserted if applicable   
	TEMPLATE_COMPONENT_DELIMITER,								
	ESC,HL7_NTE_TEXT,					// NTE-3 Generic comment free text   
	TEMPLATE_FIELD_DELIMITER,
	'G',
	TEMPLATE_FIELD_DELIMITER,
	'\0'								// end of record template delimiter   
};


	// (C) HL7 QRD Record: resend back to Host
const char	hl7TxQRDrecord[] = 	
{
	'Q','R','D',							// QRD record methods - handled as generic field strings
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_DATE_TIME,					// QRD-1 Query Date/Time
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_FORMAT_CODE,				// QRD-2 Query Format Code
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_PRIORITY,					// QRD-3 Query Priority
	TEMPLATE_FIELD_DELIMITER, 
	ESC,HL7_QRD_ID,							// QRD-4 Query ID
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_DEFERRED_RESPONSE_TYPE,		// QRD-5 Deferred Response Type
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_DEFERRED_RESPONSE_DATE,		// QRD-6 Deferred Response Date/Time
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_QUALITY_REQUEST,			// QRD-7 Quantity Limited Request
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_WHO_SUBJECT_FILTER,			// QRD-8 Who Subject Filter
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_WHAT_SUBJECT_FILTER,		// QRD-9 What Subject Filter
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_WHAT_DEPT_DATA_CODE,		// QRD-10 What Department Data Code
	TEMPLATE_FIELD_DELIMITER,
	ESC,HL7_QRD_WHAT_DEPT_VALUE_QUAL,		// QRD-11 What Data Code Value Qual.
	TEMPLATE_FIELD_DELIMITER,
	'\0'					// end of record template delimiter   
};


	// table of pointers to hl7 transmit record (frame) templates   

const char*  hl7TxRecordTemplatePtrs_m[NUM_HL7_TX_RECORDS] =
{
	NULL,
	&hl7TxMSHrecord[0],					// 1:  header				record template ptr 					   
	&hl7TxMSArecord[0],					// 2:  acknowledge			record template ptr 					  
	&hl7TxQRDrecord[0],					// 3:  query definition     record template ptr 					  
	&hl7TxPIDrecord[0],					// 4:  patient ID			record template ptr 					  
	&hl7TxPV1record[0],					// 5:  patient visit info   record template ptr 					  
	&hl7TxOBRrecord[0],					// 6:  request				record template ptr 					  
	&hl7TxOBXrecord[0],					// 7:  results				record template ptr 					  
	&hl7TxNTErecord[0],					// 8:  comment				record template ptr 					  

	&hl7TxAbortOBRrecord[0],			// 9:  abort order   record template ptr 				  
	&hl7TxAbortNTErecord[0],			// 10:  abort comment record template ptr			  

	&hl7TxNoQueryResultsPIDrecord[0],	// 11: no query results patient record template ptr	  
	&hl7TxNoQueryResultsOBXrecord[0],	// 12: no query results order   record template ptr	  
	&hl7TxNoQueryResultsNTErecord[0],	// 13: no query results comment record template ptr	  

	&hl7TxGenericNTErecord[0],			// 14: hl7 comment record						  
};	



// Abaxis transmit messages definitions based on  HL7  ***  ***  Records (frames)   

// Notes:

// 	1.	Each Abaxis transmit message is defined as a list of indices referenced to
//		hl7_tx_record_template_ptrs[] which point to templates 
//		for record (frame) types used to format & construct hl7 transmit messages   

// 	2.	All Abaxis hl7 transmit messages are referenced using abaxis_hl7_tx_data_to_send as index to
//		abaxis_hl7_tx_msg_ptrs[] each of which point to the list of indices to
//		record (frame) pointers for that message   


	// table of indices used to lookup pointers to hl7 record (frame) templates   

	// abaxis tx test message definition   
char	hl7TxCommentMsgDef[] =			
{
	HL7_TX_HEADER,			// MSH

	HL7_TX_GENERIC_COMMENT,	// NTE

	HL7_TX_MESSAGE_DONE
};


	// abaxis tx patient or control results / qc message definition   

const	char	hl7TxUnsolicitedResultsMsgDef[] =	
{
	HL7_TX_HEADER,			// MSH

	HL7_TX_PATIENT,			// PID 1			
	HL7_TX_PATIENT_VISIT,	// PV1 1	  

	HL7_TX_ORDER,			// OBR 1				(where 0 1  is rotor results order record 		  
	HL7_TX_COMMENT,			// OBR 1: NTE 1			(where NTE 1  is comment for "INST QC: OK    CHEM: OK")   
	HL7_TX_COMMENT,			// OBR 1: NTE 2			(where NTE 2  is comment for "HEM X ,  LIP Y ,  ICT Z")   
	HL7_TX_COMMENT,			// OBR 1: NTE 3 		(where NTE 3  is comment for rotor if dilution verfication message required, else NULL comment)   		
	HL7_TX_COMMENT,			// OBR 1: NTE 4			(where NTE 4  is comment for rotor if operator security status issue message, else NULL comment)   
	HL7_TX_COMMENT,			// OBR 1: NTE 5			(where NTE 5  is comment for rotor if control expired condition, else NULL comment)   

	HL7_TX_RESULT,			// OBR 1: OBX 1 to OBX N (where N    is number of analytes for rotor) 	   		
	HL7_TX_COMMENT,			// OBR 1: OBX 1: NTE 1	(where NTE 1  is possible comment for a result)   

	HL7_TX_RESULTS_REPEAT,	// analyte result: variable number R records with possible comment following   


	HL7_TX_ORDER,			// OBR 2				(where 0 2  is rotor IQC record) 		  
	HL7_TX_COMMENT,			// OBR 2: NTE 1   
	HL7_TX_COMMENT,			// OBR 2: NTE 2   

	HL7_TX_RESULT,			// OBR 2: OBX 1 to OBX 17 (OBX1 to OBX8 for VS 2)   
			
	HL7_TX_RESULTS_REPEAT,	// QC result:  repeat until all 17 (all 8 for VS 2) IQC R records done   

	HL7_TX_MESSAGE_DONE
};

	// abaxis tx patient or control results / qc message definition   

const	char	hl7TxResultsMsgDef[] =	
{
	HL7_TX_HEADER,			// MSH
	HL7_TX_MSG_ACK,			// MSA 1
	HL7_TX_QUERY,			// QRD 1
	HL7_TX_PATIENT,			// PID 1	  
	HL7_TX_PATIENT_VISIT,	// PV1 1	  

	HL7_TX_ORDER,			// OBR 1				(where 0 1  is rotor results order record 		  
	HL7_TX_COMMENT,			// OBR 1: NTE 1			(where NTE 1  is comment for "INST QC: OK    CHEM: OK")   
	HL7_TX_COMMENT,			// OBR 1: NTE 2			(where NTE 2  is comment for "HEM X ,  LIP Y ,  ICT Z")   
	HL7_TX_COMMENT,			// OBR 1: NTE 3 		(where NTE 3  is comment for rotor if dilution verfication message required, else NULL comment)   		
	HL7_TX_COMMENT,			// OBR 1: NTE 4			(where NTE 4  is comment for rotor if operator security status issue message, else NULL comment)   
	HL7_TX_COMMENT,			// OBR 1: NTE 5			(where NTE 5  is comment for rotor if control expired condition, else NULL comment)   

	HL7_TX_RESULT,			// OBR 1: OBX 1 to OBX N (where N    is number of analytes for rotor) 	   		
	HL7_TX_COMMENT,			// OBR 1: OBX 1: NTE 1	(where NTE 1  is possible comment for a result)   

	HL7_TX_RESULTS_REPEAT,	// analyte result: variable number R records with possible comment following   


	HL7_TX_ORDER,			// OBR 2				(where 0 2  is rotor IQC record) 		  
	HL7_TX_COMMENT,			// OBR 2: NTE 1   
	HL7_TX_COMMENT,			// OBR 2: NTE 2   

	HL7_TX_RESULT,			// OBR 2: OBX 1 to OBX 17 (OBX1 to OBX8 for VS 2)   
			
	HL7_TX_RESULTS_REPEAT,	// QC result:  repeat until all 17 (all 8 for VS 2) IQC R records done   

	HL7_TX_MESSAGE_DONE
};


	// abaxis tx patient or control abort rotor message definition   

const	char	hl7TxAbortMsgDef[] =	
{
	HL7_TX_HEADER,				// MSH

	HL7_TX_PATIENT,				// PID 1	  
	HL7_TX_PATIENT_VISIT,		// PV1 1	  

	HL7_TX_ABORT_ORDER,			// OBR 1 		  
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 1   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 2   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 3   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 4   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 5   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 6   
	HL7_TX_ABORT_COMMENT,		// OBR 1: NTE 7   

	HL7_TX_MESSAGE_DONE
};



	// abaxis tx no results for query message definition   

const	char	hl7TxNoQueryResultsMsgDef[] =	
{
	HL7_TX_HEADER,						// MSH
	HL7_TX_MSG_ACK,						// MSA 1
	HL7_TX_QUERY,						// QRD 1
	HL7_TX_NO_QUERY_RESULTS_PATIENT,	// PID 1	  

	HL7_TX_NO_QUERY_RESULTS_ORDER,		// OBR 1		  
	HL7_TX_NO_QUERY_RESULTS_COMMENT,	// 0BR 1: NTE 1   

	HL7_TX_MESSAGE_DONE
};

const	char	hl7TxErrorMsgDef[] =
{
	HL7_TX_HEADER,						// MSH
	HL7_TX_MSG_ACK,						// MSA
	HL7_TX_QUERY,						// QRD
	HL7_TX_MESSAGE_DONE
};
	// table of pointers to Abaxis hl7 transmit message record definition indices   

const char*  abaxisHl7TxMsgPtrs_m[NUM_ABAXIS_HL7_TX_MSGS] =	// hl7TxMsgType_m is idx   
{
	NULL,									// 0:  no msg to send							   
	&hl7TxCommentMsgDef[0],					// 1:  tx hl7 generic comment msg					  
	&hl7TxResultsMsgDef[0],					// 3:  tx patient or control results / iqc msg	  
	&hl7TxUnsolicitedResultsMsgDef[0],
	&hl7TxAbortMsgDef[0],					// 4:  tx patient or control rotor abort msg	  
	&hl7TxNoQueryResultsMsgDef[0],			// 5:  tx no query results available msg		  
	&hl7TxErrorMsgDef[0]					// 6:  tx no query results available msg		  
};	



// HL7  ***  ***  Record (frame) receive templates   

// Notes:

// 	1.	The NULL ('\0') in a template is the last char to indicate end of string,
//		and to signal DONT'T CARE what is text from this point to end of frame (<CR>) chars   

	// (H) Message Header Record: Host to Abaxis   

const	char	hl7RxMSHtemplate[] =
{
	17,									// max fields in record						  

	'H',								// MSH record type								  
				
	HL7_MSH_FIELD_DELIMITER,			// MSH-2 extract "field delimiter" 	default '|'   
										
	HL7_MSH_COMPONENT_DELIMITER,		// MSH-2 extract "component delim."   default '^'   

	HL7_MSH_REPEAT_DELIMITER,			// MSH-2 extract "repeat delimiter" 	default '~'   										 

	HL7_MSH_ESCAPE_CHARACTER, 			// MSH-2 extract "escape delimiter" 	default '\\'   

	HL7_MSH_SUB_COMPONENT_DELIMITER,	// MSH-2 extract "sub component delim."   default '&'  

	HL7_MSH_FIELD_DELIMITER,

	HL7_MSH_SENDING_APPLICATION,		// MSH-3

	HL7_MSH_SENDING_FACILITY,			// MSH-4

	HL7_MSH_RECEIVING_APPLICATION,		// MSH-5

	HL7_MSH_RECEIVING_FACILITY,			// MSH-6

	HL7_MSH_DATE_TIME_OF_MESSAGE,		// MSH-7 extract "date/time of message"

	HL7_MSH_SECURITY,					// MSH-8 extract "security"

	HL7_MSH_MESSAGE_TYPE,				// MSH-9 extract "message type"

	HL7_MSH_MESSAGE_CONTROL_ID,			// MSH-10 extract "message control id"

	HL7_MSH_PROCESSING_ID,				// MSH-11 extract "message processing id"

	HL7_MSH_VERSION_ID,					// MSH-12 extract "supported HL7 version"

	'\0'								// end of record template string 			  
};
								


	// (Q) Request Information Record: Host to Abaxis   

const	char	hl7RxQRDtemplate[] =									
{
	13,									// max fields in record					  

	'Q',								// QRD record type

	HL7_RX_FIELD_DELIMITER,

	HL7_QRD_DATE_TIME,					// QRD-1 Date time when query was generated	  

	HL7_QRD_FORMAT_CODE,				// QRD-2 Query response format code 	  
										
	HL7_QRD_PRIORITY,					// QRD-3 Priority  
										 
	HL7_QRD_ID,							// QRD-4 Unique identifier 					  

	HL7_QRD_DEFERRED_RESPONSE_TYPE, 	// QRD-5 					  

	HL7_QRD_DEFERRED_RESPONSE_DATE, 	// QRD-6 					  

	HL7_QRD_QUALITY_REQUEST, 			// QRD-7 					  

	HL7_QRD_WHO_SUBJECT_FILTER,			// QRD-8 Identifies subject or who the query is about
										
	HL7_QRD_WHAT_SUBJECT_FILTER,		// QRD-9 ORD or RES	  
										 
	HL7_QRD_WHAT_DEPT_DATA_CODE,		// QRD-10 Depratment code	  
										 
	HL7_QRD_WHAT_DEPT_VALUE_QUAL,		// QRD-11 - start end result date and time" 	  
										 

	'\0'								// end of record template string 		  
};
																				

	// table of pointers to hl7 receive record (frame) templates 	  
	//	 - scan thru to match second string char for record type being received 	  

const char*	hl7RxRecordTemplatePtrs_m[NUM_RX_FRAME_TEMPLATES] =
{
	&hl7RxMSHtemplate[0],			// header record template ptr 			   
	&hl7RxQRDtemplate[0]			// query record template ptr 			  
};	


// Abaxis allowed receive messages definitions based on  HL7  ***  ***  Records (frames)   

// Notes:

// 	1.	Each Abaxis allowed receive message is defined as a list of record 1st characters 
//		for record (frame) types used to format & construct hl7 receive messages   

// 	2.	After all records up to terminator are received for message then the abaxis message
//		received is compared against the list(s)   

// 	3.	If no match then returned index is 0, else returns index to ptr of matched record chars
//		found in list   

	// tables of record first characters for allowed abaxis receive messages   


const	char	hl7RxQMsgDef[] =			// rx abaxis Q (Query) message definition 					  

								"HQ" ;		// requires header, request information, terminator records   


	// table of pointers to Abaxis allowed hl7 receive messages   

const char*	abaxisHl7RxMsgPtrs_m[NUM_ABAXIS_HL7_RX_MSGS] =	
{
	&hl7RxQMsgDef[0]						// 1: Query Abaxis message		  
};	


// HL7 constant strings

// Patient results - OBR1NTEn record text strings - rotor analyte results

const char	qcOkTextString[] = 			     { "INST QC: OK    CHEM QC: OK" };	// 01NTE1

const char	hemTextString[] =			     { "HEM:" };						// 01NTE2
const char	lipTextString[] =			     { "LIP:" };						// 01NTE2
const char	ictTextString[] =			     { "ICT:" };						// 01NTE2

const char	rotorDiluteCheckFailedString[] = { "@ CONFIRM LOW RECOVERIES @" };	// 01NTE3


// IQC report header - 0BR2 record name

const char	iqcReportHeaderTextString[]	=    { "* QUALITY CONTROL REPORT *" };	// 0BR2

// IQC results - 0BR2NTEn record text strings

const char	chemQcTextString[] =		     { "CHEMISTRY QC:       " };		// 0BR2NTE1
const char	acceptableMinTextString[] =		 { "ACCEPTABLE MINIMUM: " };		// 0BR2NTE2


											
const char	iqcLimitStringLevel1[] =	{ "90 to 110" };	// for 02R1 to O2R8
const char	iqcLimitStringLevel2[] =	{ "95 to 105" };	// for O2R9 to O2R17


// text strings defined for HL7 RnC1 records if errors in Rn record   

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


// strings defined for HL7 abort rotor OBR record NTE1, NTE2, NTE3, NTE4, NTE5 & NTE6   

// OBR1 NTE1   
const char	hl7RunCountString[] =			{ "Run Count" };
const char	hl7AbortCountString[] =			{ "Abort Count" };

// OBR1 NTE2   
const char	hl7PrintCountString[] =			{ "Print Count" };
const char	hl7FlashCountString[] =			{ "Flash Count" };

// OBR1 NTE3   
const char	hl7DacTrimFlagsString[] =		{ "DAC Trim Flags" };
const char	hl7OffsetErrFlagsString[] =		{ "Offset Error Flags" };
const char	hl7SystemFlagsString[] =		{ "System Flags" };

// OBR1 NTE4   
const char	hl7BeadCheck1String[] =			{ "Bead Check 1 Flags" };
const char	hl7BeadCheck2String[] =			{ "Bead Check 2 Flags" };

// OBR1 NTE5   
const char	hl7EmptyCuvFlagsString[] =		{ "Empty Cuvette Flags" };
const char	hl7DistCheckFlagsString[] =		{ "Distribution Check Flags" };

// OBR1 NTE6   
const char	hl7QcFlagsString[] =			{ "Quality Control Flags" };
const char	hl7OffsetSdErrString[] =		{ "Offset SD Error Flags" };
const char	hl7WavelengthCvString[] =		{ "Wavelength CV Flags" };



// string defined for HL7 no query results for OBR1 record NTE1   


// OBR1 NTE1   
const char	hl7NoQueryResultString[] =		{ "No Results for this Query" };



// strings defined for HL7 generic comment message text strings   


const char	hl7CommentStrings[NUM_HL7_STRINGS][HL7_STRING_SIZE] =
{
	"Power Up Initialization Sign On ",		// 0: HL7_POWER_UP_SIGN_ON message   !!!!Note: Not used in this release

	"Received Delimiter Mismatch     ",		// 1: HL7_RX_DELIMITER_MISMATCH message   
	
	"Received Field Size Too Big     ",		// 2: HL7_RX_FIELD_SIZE_TOO_BIG message   

	"Received Invalid Record Type    ",		// 3: HL7_RX_INVALID_RECORD_TYPE message   

	"Received > Max Fields in Record ",		// 4: HL7_RX_TOO_MANY_FIELDS_IN_RECORD message   

	"Received Record Out of Sequence ",		// 5: HL7_RX_RECORD_OUT_OF_SEQUENCE message

	"Received Unsupported HL7 Version"		// 6: HL7_RX_UNSUPPORTED_HL7_VERSION message   

};




#endif	// __HL7CONSTANTS_MSH_INCLUDED






