#ifndef __ASTMINPUT_H_INCLUDED
#define __ASTMINPUT_H_INCLUDED

// astm RX field lengths

#define ASTM_RX_H_MESSAGE_CONTROL_ID_LEN    2                       //
#define ASTM_RX_H_ACCESS_PASSWORD_LEN       64                      //
#define ASTM_RX_H_SENDER_NAME_OR_ID_LEN     32                      //
#define ASTM_RX_H_RECEIVER_ID_LEN			10						//  
#define ASTM_RX_H_MESSAGE_DATE_TIME_LEN     14                      //

#define ASTM_RX_P_SEQUENCE_NUMBER_LEN		1						// 
#define ASTM_RX_P_PATIENT_CONTROL_ID_LEN	14						// 
#define ASTM_RX_P_SAMPLE_ID_LEN				14						// 							
#define ASTM_RX_P_ALTERNATE_ID_LEN			14						// 							
#define ASTM_RX_P_BIRTHDATE_LEN				8						// 							
#define ASTM_RX_P_PATIENT_SEX_LEN			1						// 							
#define ASTM_RX_P_PATIENT_RACE_LEN			2						// 							
#define ASTM_RX_P_PATIENT_PHONE_NUMBER_LEN	14						// 					
#define ASTM_RX_P_PATIENT_SPECIAL1_LEN		38						// 						
#define ASTM_RX_P_PATIENT_SPECIAL2_LEN		31						// 						
#define ASTM_RX_P_PATIENT_LOCATION_LEN		14						// 						

#define ASTM_RX_O_SEQUENCE_NUMBER_LEN		1						// 						
#define ASTM_RX_O_ROTOR_NUMBER_LEN			5						// 						
#define ASTM_RX_ORDER_DATE_TIME_LEN			14						//
#define ASTM_RX_O_ACTION_CODE_LEN			1						// 							
#define ASTM_RX_O_PHYSICIAN_ID_LEN			14						// 							
#define ASTM_RX_O_LABORATORY_FIELD_1_LEN    17                      //
#define ASTM_RX_O_REPORT_TYPE_LEN			1						// 						
	
#define ASTM_RX_M_SEQUENCE_NUMBER_LEN		1						// 					
#define ASTM_RX_M_RECORD_IDENTIFIER_LEN		2						// 					


typedef struct
{
	// 'H' record
    char            hMessageControlId[ ASTM_RX_H_MESSAGE_CONTROL_ID_LEN + 1 ];      // 7.1.3
    char            hAccessPassword[ ASTM_RX_H_ACCESS_PASSWORD_LEN + 1 ];           // 7.1.4
    char            hSenderNameOrId[ ASTM_RX_H_SENDER_NAME_OR_ID_LEN + 1 ];         // 7.1.5
	char 			hReceiverId[ ASTM_RX_H_RECEIVER_ID_LEN + 1 ];					// 7.1.10  
    char            hMessageDateAndTime[ ASTM_RX_H_MESSAGE_DATE_TIME_LEN + 1 ];     // 7.1.14

	// 'P' record
	char 			pSequenceNumber[ ASTM_RX_P_SEQUENCE_NUMBER_LEN + 1 ];			// 8.1.2  
	char 			pPatientControlId[ ASTM_RX_P_PATIENT_CONTROL_ID_LEN + 1 ];		// 8.1.3  
	char 			pSampleId[ ASTM_RX_P_SAMPLE_ID_LEN + 1 ];						// 8.1.4  
	char 			pAlternateId[ ASTM_RX_P_ALTERNATE_ID_LEN + 1 ];					// 8.1.5  
	char 			pBirthdate[ ASTM_RX_P_BIRTHDATE_LEN + 1 ];						// 8.1.8  
	char 			pPatientSex[ ASTM_RX_P_PATIENT_SEX_LEN + 1 ];					// 8.1.9  
	char 			pPatientRace[ ASTM_RX_P_PATIENT_RACE_LEN + 1 ];					// 8.1.10  
	char 			pPatientPhoneNumber[ ASTM_RX_P_PATIENT_PHONE_NUMBER_LEN + 1 ];	// 8.1.13  
	char 			pPatientSpecial1[ ASTM_RX_P_PATIENT_SPECIAL1_LEN + 1 ];			// 8.1.15  
	char 			pPatientSpecial2[ ASTM_RX_P_PATIENT_SPECIAL2_LEN + 1 ];			// 8.1.16  
	char 			pPatientLocation[ ASTM_RX_P_PATIENT_LOCATION_LEN + 1 ];			// 8.1.26
	
	// 'O' record
	char 			oSequenceNumber[ ASTM_RX_O_SEQUENCE_NUMBER_LEN + 1 ];			// 9.4.2  
	char 			oRotorNumber[ ASTM_RX_O_ROTOR_NUMBER_LEN + 1 ];					// 9.4.5
	char			oOrderDateTime[ ASTM_RX_ORDER_DATE_TIME_LEN + 1 ];				// 9.4.7  
	char 			oActionCode[ ASTM_RX_O_ACTION_CODE_LEN + 1 ];					// 9.4.12  
	char 			oPhysicianId[ ASTM_RX_O_PHYSICIAN_ID_LEN + 1 ];					// 9.4.17  
    char            oLaboratoryField1[ ASTM_RX_O_LABORATORY_FIELD_1_LEN + 1 ];      // 9.4.21
	char 			oReportType[ ASTM_RX_O_REPORT_TYPE_LEN + 1 ];					// 9.4.26  
	  
	// 'M' record
	char 			mSequenceNumber[ ASTM_RX_M_SEQUENCE_NUMBER_LEN + 1 ];			// 15.1.1  
	char 			mRecordIdentifier[ ASTM_RX_M_RECORD_IDENTIFIER_LEN + 1 ];		// 15.1.2 - defined by Abaxis for analyzer status

} AstmInput_t;
typedef unsigned long long  AstmInputErrorCode_t;

#define     AIE_NO_ERROR                             0x00000000
#define     AIE_SERIAL_NUMBER_MISMATCH               0x00000001 // s/n input does not match instrument's s/n
#define     AIE_PATIENT_CONTROL_ID_MISSING           0x00000002 // required field empty
#define     AIE_PATIENT_CONTROL_ID_BAD_FORMAT        0x00000004 // invalid character(s) found in field
#define     AIE_SAMPLE_ID_BAD_FORMAT                 0x00000008 // invalid character(s) found in field
#define     AIE_ALTERNATE_ID_BAD_FORMAT              0x00000010 // invalid character(s) found in field
#define     AIE_BIRTHDATE_MISSING                    0x00000020 // required field empty
#define     AIE_BIRTHDATE_BAD_FORMAT                 0x00000040 // incorrect date format
#define     AIE_GENDER_MISSING                       0x00000080 // required field empty
#define     AIE_GENDER_BAD_FORMAT                    0x00000400 // invalid value
#define     AIE_RACE_MISSING                         0x00000800 // required field empty
#define     AIE_RACE_BAD_FORMAT                      0x00001000 // invalid value
#define     AIE_PATIENT_PHONE_NUMBER_BAD_FORMAT      0x00002000 // invalid character(s) found in field
#define     AIE_OWNER_ADMISSION_ID_BAD_FORMAT        0x00004000 // invalid character(s) found in field
#define     AIE_OWNER_ADMISSION_ID_TOO_LONG          0x00008000 // too many characters in field
#define     AIE_AGE_BAD_FORMAT                       0x00010000 // incorrect age format
#define     AIE_DATE_OF_VACCINATION_BAD_FORMAT       0x00020000 // incorrect date format
#define     AIE_SAMPLE_TYPE_MISSING                  0x00040000 // required field empty
#define     AIE_SAMPLE_TYPE_INVALID                  0x00080000 // value given is invalid
#define     AIE_LOCATION_BAD_FORMAT                  0x00100000 // invalid character(s) found in field
#define     AIE_ROTOR_NUMBER_MISSING                 0x00200000 // required field empty
#define     AIE_ROTOR_NUMBER_INVALID                 0x00400000 // value given is not valid
#define     AIE_ACTION_CODE_MISSING                  0x00800000 // required field empty
#define     AIE_ACTION_CODE_INVALID                  0x01000000 // invalid value
#define     AIE_PHYSICIAN_ID_BAD_FORMAT              0x02000000 // invalid character(s) found in field
#define     AIE_REPORT_TYPE_INVALID                  0x04000000 // invalid value
#define     AIE_RECORD_IDENTIFIER_MISSING            0x08000000 // required field empty
#define     AIE_RECORD_IDENTIFIER_INVALID            0x10000000 // invalid value
#define     AIE_ORDER_DATE_TIME_MISSING              0x20000000 // required field empty
#define     AIE_MESSAGE_CONTROL_ID_MISSING           0x40000000 // required field empty
#define     AIE_MESSAGE_CONTROL_ID_INVALID           0x80000000 // invalid value
#define     AIE_ACCESS_PASSWORD_INVALID              0x000100000000ULL    // required field empty or otherwise invalid
#define     AIE_ACCESS_PASSWORD_INCORRECT            0x000200000000ULL    // calculated value does not match value received
#define     AIE_ORDER_LABEL_BAD_FORMAT               0x000400000000ULL    // invalid character(s) found in field 
#define     AIE_MESSAGE_DATE_TIME_INVALID            0x000800000000ULL    // invalid format for date/time
#define     AIE_MESSAGE_DATE_TIME_INCORRECT          0x001000000000ULL    // message date/time differs from analyzer date/time

void	SetHostPatientOrder( AstmInput_t * astmInput );

void	SetMfgStatusRequest( AstmInput_t * astmInput );

#endif	// __ASTM_INPUT_H_INCLUDED


