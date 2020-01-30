#ifndef __HL7CONFIGURATION_INCLUDED
#define __HL7CONFIGURATION_INCLUDED

#pragma pack(push, 1) // exact fit - no padding

//HL7 message segment definitions

typedef struct
{
	char				mshFieldDelimiter;					// MSH-1 Field delimiter  '|',
	char				mshComponentDelimiter;				// MSH-2 Field delimiter  '^',
	char				mshRepeatDelimiter;					// MSH-2 Field delimiter  '~',
	char				mshSubComponentDelimiter;			// MSH-2 Field delimiter  '&',
	char				mshEscapeCharacter;					// MSH-2 Field delimiter  '\\',
	unsigned short		mshSendingApplication;					// MSH-3
	unsigned short		mshSendingFacility;						// MSH-4
	unsigned short		mshReceivingApplication;				// MSH-5
	unsigned short		mshReceivingFacility;					// MSH-6
	unsigned short		mshTimeofMessage;						// MSH-7
	unsigned short		mshSecurity;							// MSH-8
	unsigned short		mshMessageType;							// MSH-9
	unsigned short		mshMessageControlID;					// MSH-10
	unsigned short		mshProcessingID;						// MSH-11
	unsigned short		mshVersionID;							// MSH-12
	unsigned short		mshSequenceNumber;						// MSH-13
	unsigned short		mshContinuationPointer;					// MSH-14
	unsigned short		mshAcceptAcknowledgementType;			// MSH-15
	unsigned short		mshAcknowledgementType;					// MSH-16
	unsigned short		mshCountryCode;							// MSH-17
	unsigned short		mshCountrySet;							// MSH-18
	unsigned short		mshPrincipalLanguage;					// MSH-19
} MSHSegment_t;

typedef struct
{
	unsigned short		qrdQueryDateTime;						// QRD-1
	unsigned short		qrdQueryFormatCode;						// QRD-2
	unsigned short		qrdQueryPriority;						// QRD-3
	unsigned short		qrdQueryID;								// QRD-4
	unsigned short		qrdDeferredResponseType;				// QRD-5
	unsigned short		qrdDeferredResponseDateTime;			// QRD-6
	unsigned short		qrdQuantityLimitedRequest;				// QRD-7
	unsigned short		qrdWhoSubjectFilter;					// QRD-8
	unsigned short		qrdWhatSubjectFilter;					// QRD-9
	unsigned short		qrdWhatDepartmentDataCode;				// QRD-10
	unsigned short		qrdWhatDataCodeValueQual;				// QRD-11
	unsigned short		qrdWhatQueryResultsLevel;				// QRD-12 ??? added based on xml file
} QRDSegment_t;


typedef struct
{
	unsigned short		pidSetID;								// PID-1
	unsigned short		pidPatientIDExternal;					// PID-2
	unsigned short		pidPatientIDInternal;					// PID-3
	unsigned short		pidAlternatePatientID;					// PID-4
	unsigned short		pidPatientName;							// PID-5
	unsigned short		pidMothersMaidenName;					// PID-6
	unsigned short		pidDateTimeofBirth;						// PID-7
	unsigned short		pidSex;									// PID-8
	unsigned short		pidPatientAlias;						// PID-9
	unsigned short		pidRace;								// PID-10
	unsigned short		pidPatientAddress;						// PID-11
	unsigned short		pidCountryCode;							// PID-12
	unsigned short		pidPhoneNumberHome;						// PID-13
	unsigned short		pidPhoneNumberBusiness;					// PID-14
	unsigned short		pidPrimaryLanguage;						// PID-15
	unsigned short		pidMaritalStatus;						// PID-16
	unsigned short		pidReligion;							// PID-17
	unsigned short		pidPatientAccountNumber;				// PID-18
	unsigned short		pidSSNNumberPatient;					// PID-19
	unsigned short		pidDriversLicenseNumberPatient;			// PID-20
	unsigned short		pidMothersIdentifier;					// PID-21
	unsigned short		pidEthnicGroup;							// PID-22
	unsigned short		pidBirthPlace;							// PID-23
	unsigned short		pidMultipleBirthIndicator;				// PID-24
	unsigned short		pidBirthOrder;							// PID-25
	unsigned short		pidCitizenship;							// PID-26
	unsigned short		pidVeteransMilitaryStatus;				// PID-27
	unsigned short		pidNationality;							// PID-28
	unsigned short		pidPatientDeathDateTime;				// PID-29
	unsigned short		pidPatientDeathIndicator;				// PID-30
} PIDSegment_t;

typedef struct
{
	unsigned short		pv1SetID;								// PV1-1
	unsigned short		pv1PatientClass;						// PV1-2
	unsigned short		pv1AssignedPatientLocation;				// PV1-3
	unsigned short		pv1AdmissionType;						// PV1-4
	unsigned short		pv1PreadmitNumber;						// PV1-5
	unsigned short		pv1PriorPatientLocation;				// PV1-6
	unsigned short		pv1AttendingDoctor;						// PV1-7
	unsigned short		pv1ReferringDoctor;						// PV1-8
	unsigned short		pv1ConsultingDoctor;					// PV1-9
	unsigned short		pv1HospitalService;						// PV1-10
	unsigned short		pv1TemporaryLocation;					// PV1-11
	unsigned short		pv1PreadmitTestIndicator;				// PV1-12
	unsigned short		pv1ReadmissionIndicator;				// PV1-13
	unsigned short		pv1AdmitSource;							// PV1-14
	unsigned short		pv1AmbulatoryStatus;					// PV1-15
	unsigned short		pv1VIPIndicator;						// PV1-16
	unsigned short		pv1AdmittingDoctor;						// PV1-17
	unsigned short		pv1PatientType;							// PV1-18
	unsigned short		pv1VisitNumber;							// PV1-19
} PV1Segment_t;

typedef struct
{
	unsigned short		obrSetID;								// OBR-1
	unsigned short		obrPlacerOrderNumber;					// OBR-2
	unsigned short		obrFillerOrderNumber;					// OBR-3
	unsigned short		obrUniversalServiceID;					// OBR-4
	unsigned short		obrPriority;							// OBR-5
	unsigned short		obrRequestedDateTime;					// OBR-6
	unsigned short		obrObservationDateTime;					// OBR-7
	unsigned short		obrObservationEndDateTime;				// OBR-8
	unsigned short		obrCollectionVolume;					// OBR-9
	unsigned short		obrCollectorIdentifier;					// OBR-10
	unsigned short		obrSpecimenActionCode;					// OBR-11
	unsigned short		obrDangerCode;							// OBR-12
	unsigned short		obrRelevantClinicalInfo;				// OBR-13
	unsigned short		obrSpecimenReceivedDateTime;			// OBR-14
	unsigned short		obrSpecimenSource;						// OBR-15
	unsigned short		obrOrderingProvider;					// OBR-16
	unsigned short		obrOrderCallbackPhoneNumber;			// OBR-17
	unsigned short		obrPlacerField1;						// OBR-18
	unsigned short		obrPlacerField2;						// OBR-19
	unsigned short		obrFillerField1;						// OBR-20
	unsigned short		obrFillerField2;						// OBR-21
	unsigned short		obrResultsStatusChngDateTime;			// OBR-22
	unsigned short		obrChargetoPractice;					// OBR-23
	unsigned short		obrDiagnosticServSectID;				// OBR-24
	unsigned short		obrResultStatus;						// OBR-25
	unsigned short		obrParentResult;						// OBR-26
	unsigned short		obrQuantityTiming;						// OBR-27
	unsigned short		obrResultCopiesTo;						// OBR-28
	unsigned short		obrParent;								// OBR-29
	unsigned short		obrTransportationMode;					// OBR-30
	unsigned short		obrReasonForStudy;						// OBR-31
	unsigned short		obrPrincipalResultInterpreter;			// OBR-32
	unsigned short		obrAssistantResultInterpreter;			// OBR-33
	unsigned short		obrTechnician;							// OBR-34
	unsigned short		obrTranscriptionist;					// OBR-35
	unsigned short		obrScheduledDateTime;					// OBR-36
	unsigned short		obrNumberofSampleContainers;			// OBR-37
	unsigned short		obrTransportLogisticsofCollectedSample;	// OBR-38
	unsigned short		obrCollectorsComment;					// OBR-39
	unsigned short		obrTransportArrangementResponsibility;	// OBR-40
	unsigned short		obrTransportArranged;					// OBR-41
	unsigned short		obrEscortRequired;						// OBR-42
	unsigned short		obrPlannedPatientTransportComment;		// OBR-43
} OBRSegment_t;

typedef struct
{
	unsigned short		obxSetID;								// OBX-1
	unsigned short		obxValueType;							// OBX-2
	unsigned short		obxObservationIdentifier;				// OBX-3
	unsigned short		obxObservationSubId;					// OBX-4
	unsigned short		obxObservationValue;					// OBX-5
	unsigned short		obxUnits;								// OBX-6
	unsigned short		obxReferenceRange;						// OBX-7
	unsigned short		obxAbnormalFlags;						// OBX-8
	unsigned short		obxProbability;							// OBX-9
	unsigned short		obxNatureofAbnormalTest;				// OBX-10
	unsigned short		obxObservResultStatus;					// OBX-11
	unsigned short		obxDataLastObsNormalValues;				// OBX-12
	unsigned short		obxUserDefinedAccessChecks;				// OBX-13
	unsigned short		obxObservationDateTime;					// OBX-14
	unsigned short		obxProducerId;							// OBX-15
	unsigned short		obxResponsibleObserver;					// OBX-16
	unsigned short		obxObservationMethod;					// OBX-17
} OBXSegment_t;

typedef struct
{
	unsigned short		nteSetID;								// NTE-1
	unsigned short		nteCommentSource;						// NTE-2
	unsigned short		nteCommentText;							// NTE-3
	unsigned short		nteCommentType;							// NTE-4
} NTESegment_t;

typedef struct
{
    unsigned short      blockStart;                             // SB
    unsigned short      blockEnd;                               // EB
} BLKSegment_t;

typedef struct
{
	// 'MSH' Input segment
	MSHSegment_t segmentMSHIn;
	// 'QRD' Input segment
	QRDSegment_t segmentQRD;
	// 'MSH' Output segment	
	MSHSegment_t segmentMSHOut;
	// 'PID' Output segment
	PIDSegment_t segmentPID;
	// 'PV1' Output segment
	PV1Segment_t segmentPV1;
	// 'OBR' Output segment
	OBRSegment_t segmentOBR;
	// 'OBX' Output segment
	OBXSegment_t segmentOBX;
	// 'NTE' Output segment
	NTESegment_t segmentNTE;
    //Block SB/EB 
    BLKSegment_t  segmentBLK;
    // memory checking 
    unsigned int crcCheckSum;
} Hl7Configuration_t;


#pragma pack(pop) //back to whatever the previous packing mode was

#define         HL7_MAX_CONFIGURATION_INDEX     50

typedef struct
{
    unsigned short  index;
    char            fileName [128];
    char            displayText [128];
} Hl7ConfigurationFile_t;

typedef struct
{
    unsigned short              tIndex;     // totally configuration file number
    Hl7ConfigurationFile_t      hl7FileName [HL7_MAX_CONFIGURATION_INDEX];
} Hl7ConfigurationFileList_t; 

#endif	// __HL7CONFIGURATION_INCLUDED
