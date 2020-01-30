// ********************************************************************
//
// FILENAME:  
// 
// 		Hl7Data.h	
// _____________________________________________________________________
// 
// MODULE DESCRIPTION:
// 
//		Contains data definitions for the Hl7 protocol implementation
// _____________________________________________________________________
//
// HISTORY: (maintained by SourceSafe)
//
// *******************************************************************      
//
#ifndef __HL7DATA_MSH_INCLUDED
#define __HL7DATA_MSH_INCLUDED


#include "CommonDefines.h"
#include "ResultsData.h"

// HL7 results database query type

#define HL7_RECORD_RESULTS				0
#define HL7_RECORD_ABORT				1

#define	HL7_CHEM_STRING_SIZE			39					// 38 characters plus NULL */
#define MAX_RESULTS						16

#define CONFIG_RECORD_FIELD_SIZE		60

#define	IQC_LEVEL_NAME_NUM_STRINGS		17
#define	IQC_LEVEL_NAME_STRING_SIZE		19

#define	IQC_RESULTS_NUM_STRINGS			17
#define	IQC_RESULTS_STRING_SIZE			4

#define METHOD_TEXT_SIZE				200

// data structure for an HL7 record to be used to transmit patient/control/error results data from results database to LIS
// (HL7 paragraph reference in comments below, as applicable)

typedef struct
{
	// 'R' record specific fields
	char  rnUniversalTestId[ HL7_CHEM_STRING_SIZE ];		// 10.1.3  Universal Test ID (for each analyteType based on units defined for all analytes/units/combinations)
	char  rnResultString[ 8 ];								// 10.1.4  Data or Measurement Value 
															//         - rnResultString is set to NULL '\0' char for end of results list (no more results)    
	char  rnUnitsString[ 8 ];								// 10.1.5  Units
	char  rnLoRefRangeString[ 8 ];							// 10.1.6  Reference Ranges (1st Part) low  reference range
	char  rnHiRefRangeString[ 8 ];							// 10.1.6  Reference Ranges (2nd Part) high reference range

	char  rnAbnormalFlag;									// 10.1.7  Result Abnormal Flags
															//         '\0': legacy uses as init NULL value	(highest priority), for error result (e.g. '~~~', 'HEM', 'LIP', 'ICT' printed etc.) or reference range not checked/printed with no errors
															//         '<' : below dynamic or system range  (next    priority)
															//         '>' : above dynamic or system range	(next    priority)
															//         'L' : below reference range			(next    priority), checked only if reference range used/printed
															//         'H' : above reference range			(next    priority), checked only if reference range used/printed
															//         'N' : within normal range			(lowest  priority), set     only if reference range used/printed				

															// 10.1.9  Result Status (implemented by check of hl7ResultRecordSent global flag for each 'R' record for rotor 
															//		   'F' : final results 							('F' for hl7ResultRecordSent global flag = 0)	
															//		   'R' : this result was previously transmitted ('R' for hl7ResultRecordSent global flag = non-zero)
	// 'R' record, 'C' record specific fields
	float 				rnC1ResultRaw;						// from analyteResult in RotorAnalyteResult_t (non unit adjusted raw result) 
	unsigned long		rnC1AnalyteFlags;					// from analyteFlags in RotorAnalyteResult_t
	ResultPrintFlags_t	rnC1ResultPrintFlags;				// from resultPrintFlags in RotorAnalyteResult_t (need to update to set flags for both below reference range lo and above reference range hi)

} Hl7Result_t;
typedef struct
{
	char fieldValue[CONFIG_RECORD_FIELD_SIZE];
}Hl7Field;

typedef struct
{
	// 'MSH' record
	char 			hl7RecordType;							// 0: Results record data  (valid rotor & analyte data for patient or control)
															// 1: Error   record data  (aborted rotor with 40xx error)
	char			hFieldDelimiter;						// MSH-1 Field delimiter  '|',
	char			hComponentDelimiter;					// MSH-2 Field delimiter  '^',
	char			hRepeatDelimiter;						// MSH-2 Field delimiter  '~',
	char			hEscapeCharacter;						// MSH-2 Field delimiter  '\\',
	char			hSubComponentDelimiter;					// MSH-2 Field delimiter  '&',
	char			hMessageType[9];						// MSH-9 The type of message and trigger event
	char			hControlId[21];							// MSH-10 Message identifier
	char			hProcessingId[4];						// MSH-11 Production or debug
	int				recordId;								// database record ID

	char  			hl7ResultRecordSent;					// global rotor flag set for send of "10.1.9  Result Status" for each result
															// 0: not successfully sent out HL7 interface		
	// 'PID' record
	char 			pPatientControlId[ ID_LEN ];			// 8.1.3  Practice Assigned Patient ID
	char 			pSampleId[ ID_LEN ];					// 8.1.4  Laboratory Assigned Patient ID
	char			pAlternateId[ ID_LEN ];					// 8.1.5  Patient ID No. 3 (e.g. SSN, defined by user)
	char			pDateOfBirth[ 10 ];						// 8.1.8  Birthdate (DOB) in YYYYMMDD format
	char 			pGender;								// 8.1.9  Patient Sex - M: male / neutered; F: female / spayed; U: unspecified (gender)
	char			pRace;								// 8.1.10 Patient Race-Ethnic Origin - W: white; B: black; O: asian/pacific islander; NA: native american/alaskan native; H: Hispanic; NULL string: no race or unknown
	char			pPhoneNumber[ ID_LEN ];					// 8.1.13 Patient Telephone Number
	char			pOwnerAdmissionId[ID_LEN ];				// 8.1.15 Special Field 1: 1st Component - VS2: Owner ID; xpress: Admission ID						
	char 			pAge[ 16 ];								// 8.1.15 Special Field 1: 2nd Component - string including patient age followed by age units 
	char			pLastVaccination[ 10 ];					// 8.1.15 Special Field 1: 3rd Component - DOV in YYYYMMDD format
	char 			pReferenceRangeLabel[ 33 ];				// 8.1.16 Special Field 2: Reference Range Label (VS2 or xpress) 
	char			pLocation[ ID_LEN ];					// 8.1.26 Location - VS2: Kennel Number

	// 'OBR' record
	char			oRotorName[ 65 ];						// 9.4.5  Universal Test ID: 4th Component (1st part) - Manufacturer's or Local Code (e.g. "BASIC METABOLIC PANEL") for rotor
	char			oRotorLotNumber[ 9 ];					// 9.4.5  Universal Test ID: 4th Component (2nd part) - Manufacturer's or Local Code (e.g. "4073AA6") for rotor	
	char			oRunDateTime[ 16 ];						// 9.4.7  Requested/Ordered Date and Time  (in YYYYMMDDHHMMSS format) 
	char			oOperatorId[ ID_LEN ];					// 9.4.11 Collector ID
	char			oDoctorId [ ID_LEN ];					// 9.4.17 Ordering Physician



	// 'OBR' record, sequence '1' "Rotor Results" , 'C' record, sequence '1' - '5', specific fields
	char 			o1C2HemString[ 6 ];						// up to 4 whole number digits, no fraction from rotorHemolyzedIndex in RotorInformationResults_t
	char 			o1C2LipString[ 6 ];						// up to 4 whole number digits, no fraction from rotorLipemicIndex in RotorInformationResults_t
	char 			o1C2IctString[ 6 ];						// up to 4 whole number digits, no fraction from rotorIctericIndex in RotorInformationResults_t
	unsigned		o1C3RotorDilutionCheckError;			// rotor dilution verification error (short/long sample), from dilutionCheckResults.rotorDilutionCheckError in RotorInformationResults_t 
	char			o1C4OperatorSecurityStatus[ 33 ];		// operatorExpired or operatorUnauthorized or NULL string for comment
	char			o1C5ControlExpired[ 33 ];				// controlExpired or NULL string for comment

	// 'OBR' record, sequence '2' "Rotor IQC"     , 'C' record, sequence '1' - '2', specific fields
	char o2C1RqcResultString[ 5 ];							// up to 3 whole number digits, no fraction from rotorResults_g->rotorAnalyteResult[RQC].analyteResult
	char o2C2RqcLowLimitString[ 4 ];						// up to 2 whole number digits, no fraction from rotorRqcLowLimit in RotorInformationResults_t

	// 'OBR' record, sequence '1' "Rotor Error"   , 'C' record, sequence '1' - '7', specific fields
	char  		   o1C1RunCount[11];						// 2's complement & backwards
	char  		   o1C1AbortCount[11];						// 2's complement & backwards 
	char  		   o1C2PrintCount[11];						// 2's complement & backwards 
	char  		   o1C2FlashCount[11];						// 2's complement & backwards 
	unsigned short o1C3DacTrimFlags;						// from rotorDacTrimFlags in RotorInformationResults_t
	unsigned short o1C3OffsetErrorFlags;					// from rotorOffsetErrorFlags in RotorInformationResults_t
	unsigned long  o1C3SystemFlags;							// from rotorSystemFlags in RotorInformationResults_t
	unsigned long  o1C4BeadCheck1Flags;						// from rotorBeadCheck1Flags in RotorInformationResults_t
	unsigned long  o1C4BeadCheck2Flags;						// from rotorBeadCheck2Flags in RotorInformationResults_t
	unsigned long  o1C5EmptyCuvFlags;						// from RotorInformationResults_t 
	unsigned long  o1C5DistCheckFlags;						// from rotorDistributionCheckFlags in RotorInformationResults_t
	unsigned short o1C6QcFlags;								// from rotorQcFlags in RotorInformationResults_t
	unsigned short o1C6OffsetSdErrorFlags;					// from rotorOffsetSdErrorFlags in RotorInformationResults_t
	unsigned short o1C6DacWavelengthCvFlags;				// from rotorWavelengthCvFlags in RotorInformationResults_t
	unsigned short o1C7SystemErrorNumber;					// from rotorErrorNumber[ 0 ] in RotorInformationResults_t
	char		   o1C7SystemErrorString[ 65 ];				// English system error number text (short text string displayed with 4xxx error number)


	// 'OBX' record
	Hl7Result_t   rAnalyteResults[ MAX_RESULTS + 1 ];		// up to 16 results stored in print order (matches number of maximum legacy results allowed)
															//    rnResultString is set to NULL '\0' char for end of results list (no more results)    
	char		   rIqcResultsStrings[ IQC_RESULTS_NUM_STRINGS ][ IQC_RESULTS_STRING_SIZE ];			// 17 IQC results strings based on float values in RotorInformationResults_t
															//    indices 0 to 7:  based on rotorIqcLevel1Results 0 to 7 floats
															//    index   8     :  based on rotorIqcRatioPrecision float
															//    indices 9 to 16: based on rotorIqcLevel2Results 0 to 7 floats
	// IQC Results - OBX record text strings for IQC results for 0BR2OBX1 to 0BR2OBX17

	char	iqcHl7LevelNameStrings[IQC_LEVEL_NAME_NUM_STRINGS][IQC_LEVEL_NAME_STRING_SIZE];
} Hl7Record_t;

#endif	// __HL7DATA_MSH_INCLUDED

