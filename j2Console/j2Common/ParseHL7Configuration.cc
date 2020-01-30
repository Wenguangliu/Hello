#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <expat.h>
#include <ctype.h>
#include <errno.h>



#include "Commondefines.h"
#include "Shmem.h"
#include "Crc32.h"  // CRC routines from the Common library
#include "ParseHL7Configuration.h"
#include "LogErr.h"
#include "CfgErr.h"

// Size of the data buffer into which the config file is read.  If the buffer
// is smaller than the size of the file, multiple reads will occur.
#define BUFF_SIZE       8196

static XML_Parser       parser_m;
static char             paramValue_m [BUFF_SIZE];
static char *           paramName_m;
static char *           attributes_m[2];
static unsigned *       version_m;
static unsigned         receivedCrc_m;
static unsigned         calculatedCrc_m;
static bool             inData_m;
static bool             inParam_m;
static bool             inCrc_m;
static bool             inVersion_m;
static unsigned         wCrc_m;

static Hl7Configuration_t * hl7Configuration_m;

#if 0
#define MAX_FILE_INDEX          5
// hl7 configuration file name table
char *hl7ConfigTable [] =
                         {
                            {HL7_CONFIG_FILE_NAME},         // hl7 xml config 1 default xml conifuration
                            {HL7_EPIC_CONFIG_FILE_NAME},    // hl7 xml config 2
                            {HL7_PRACTICE_CONFIG_FILE_NAME},// hl7 xml config 3
                            {HL7_CONFIG_FILE_NAME},         // hl7 xml config 4
                            {HL7_CONFIG_FILE_NAME},         // hl7 xml config 5
                            {HL7_CONFIG_FILE_NAME}          // hl7 xml config 6

                         };
#endif

// segment reading flag
typedef enum
{
    NO_SEGMENT,
    MSH_INPUT_SEG,
    QRD_INPUT_SEG,
    MSH_OUTPUT_SGE,
    PID_OUTPUT_SEG,
    PV1_OUTPUT_SEG,
    OBR_OUTPUT_SEG,
    OBX_OUTPUT_SEG,
    NTE_OUTPUT_SEG,
    BLK_OUTPUT_SEG,
} SegmentRead_t;

static SegmentRead_t        segmentRead = NO_SEGMENT;
Hl7ConfigurationFileList_t  *hl7FileListShareMem_m;


// MSH INPUT  Segment reading
void
SetMshInputSeg (void)
{
    if (_stricmp (paramName_m, "FieldDelimiter") == 0) {
        hl7Configuration_m->segmentMSHIn.mshFieldDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "RepeatDelimiter") == 0) {
        hl7Configuration_m->segmentMSHIn.mshRepeatDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "ComponentDelimiter") == 0) {
        hl7Configuration_m->segmentMSHIn.mshComponentDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "SubComponentDelimter") == 0) {
        hl7Configuration_m->segmentMSHIn.mshSubComponentDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "EscapeCharacter") == 0) {
        hl7Configuration_m->segmentMSHIn.mshEscapeCharacter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "SendingApplication") == 0) {
        hl7Configuration_m->segmentMSHIn.mshSendingApplication = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SendingFacility") == 0) {
        hl7Configuration_m->segmentMSHIn.mshSendingFacility = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReceivingApplication") == 0) {
        hl7Configuration_m->segmentMSHIn.mshReceivingApplication = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReceivingFacility") == 0) {
        hl7Configuration_m->segmentMSHIn.mshReceivingFacility = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MsgDateTime") == 0) {
        hl7Configuration_m->segmentMSHIn.mshTimeofMessage = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Security") == 0) {
        hl7Configuration_m->segmentMSHIn.mshSecurity = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MessageType") == 0) {
        hl7Configuration_m->segmentMSHIn.mshMessageType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MessageControlID") == 0) {
        hl7Configuration_m->segmentMSHIn.mshMessageControlID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ProcessingID") == 0) {
        hl7Configuration_m->segmentMSHIn.mshProcessingID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "VersionID") == 0) {
        hl7Configuration_m->segmentMSHIn.mshVersionID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SeqNumber") == 0) {
        hl7Configuration_m->segmentMSHIn.mshSequenceNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ContinuationPointer") == 0) {
        hl7Configuration_m->segmentMSHIn.mshContinuationPointer = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AcceptAckType") == 0) {
        hl7Configuration_m->segmentMSHIn.mshAcceptAcknowledgementType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ApplicationACKType") == 0) {
        hl7Configuration_m->segmentMSHIn.mshAcknowledgementType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CountryCode") == 0) {
        hl7Configuration_m->segmentMSHIn.mshCountryCode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CharacterSet") == 0) {
        hl7Configuration_m->segmentMSHIn.mshCountrySet = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MsgLanguage") == 0) {
        hl7Configuration_m->segmentMSHIn.mshPrincipalLanguage = atoi (paramValue_m);
    }
}

// QRD Segment reading
void
SetQrdInputSeg (void)
{
    if (_stricmp (paramName_m, "QueryDateTime") == 0) {
        hl7Configuration_m->segmentQRD.qrdQueryDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "QueryFormatCode") == 0) {
        hl7Configuration_m->segmentQRD.qrdQueryFormatCode = atoi (paramValue_m); 
    }
    if (_stricmp (paramName_m, "QueryPriority") == 0) {
        hl7Configuration_m->segmentQRD.qrdQueryPriority = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "QueryID") == 0) {
        hl7Configuration_m->segmentQRD.qrdQueryID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DeferredResponseType") == 0) {
        hl7Configuration_m->segmentQRD.qrdDeferredResponseType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DeferredResponseDateTime") == 0) {
        hl7Configuration_m->segmentQRD.qrdDeferredResponseDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "QuantityLimitedRequest") == 0) {
        hl7Configuration_m->segmentQRD.qrdQuantityLimitedRequest = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "WhoSubjectFilter") == 0) {
        hl7Configuration_m->segmentQRD.qrdWhoSubjectFilter = atoi (paramValue_m); 
    }
    if (_stricmp (paramName_m, "WhatSubjectFilter") == 0) {
        hl7Configuration_m->segmentQRD.qrdWhatSubjectFilter = atoi (paramValue_m); 
    }
    if (_stricmp (paramName_m, "WhatDepartmentDataCode") == 0) {
        hl7Configuration_m->segmentQRD.qrdWhatDepartmentDataCode = atoi (paramValue_m); 
    }
    if (_stricmp (paramName_m, "WhatDataCodeValueQual") == 0) {
        hl7Configuration_m->segmentQRD.qrdWhatDataCodeValueQual = atoi (paramValue_m); 
    }
    if (_stricmp (paramName_m, "QueryResultsLevel") == 0) {
        hl7Configuration_m->segmentQRD.qrdWhatQueryResultsLevel = atoi (paramValue_m); 
    }
}

// MSH OUT Segment reading
void
SetMshOutputSeg (void)
{
    if (_stricmp (paramName_m, "FieldDelimiter") == 0) {
        hl7Configuration_m->segmentMSHOut.mshFieldDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "RepeatDelimiter") == 0) {
        hl7Configuration_m->segmentMSHOut.mshRepeatDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "ComponentDelimiter") == 0) {
        hl7Configuration_m->segmentMSHOut.mshComponentDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "SubComponentDelimter") == 0) {
        hl7Configuration_m->segmentMSHOut.mshSubComponentDelimiter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "EscapeCharacter") == 0) {
        hl7Configuration_m->segmentMSHOut.mshEscapeCharacter = paramValue_m [0];
    }
    if (_stricmp (paramName_m, "SendingApplication") == 0) {
        hl7Configuration_m->segmentMSHOut.mshSendingApplication = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SendingFacility") == 0) {
        hl7Configuration_m->segmentMSHOut.mshSendingFacility = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReceivingApplication") == 0) {
        hl7Configuration_m->segmentMSHOut.mshReceivingApplication = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReceivingFacility") == 0) {
        hl7Configuration_m->segmentMSHOut.mshReceivingFacility = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MsgDateTime") == 0) {
        hl7Configuration_m->segmentMSHOut.mshTimeofMessage = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Security") == 0) {
        hl7Configuration_m->segmentMSHOut.mshSecurity = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MessageType") == 0) {
        hl7Configuration_m->segmentMSHOut.mshMessageType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MessageControlID") == 0) {
        hl7Configuration_m->segmentMSHOut.mshMessageControlID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ProcessingID") == 0) {
        hl7Configuration_m->segmentMSHOut.mshProcessingID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "VersionID") == 0) {
        hl7Configuration_m->segmentMSHOut.mshVersionID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SeqNumber") == 0) {
        hl7Configuration_m->segmentMSHOut.mshSequenceNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ContinuationPointer") == 0) {
        hl7Configuration_m->segmentMSHOut.mshContinuationPointer = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AcceptAckType") == 0) {
        hl7Configuration_m->segmentMSHOut.mshAcceptAcknowledgementType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ApplicationACKType") == 0) {
        hl7Configuration_m->segmentMSHOut.mshAcknowledgementType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CountryCode") == 0) {
        hl7Configuration_m->segmentMSHOut.mshCountryCode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CharacterSet") == 0) {
        hl7Configuration_m->segmentMSHOut.mshCountrySet = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MsgLanguage") == 0) {
        hl7Configuration_m->segmentMSHOut.mshPrincipalLanguage = atoi (paramValue_m);
    }
}

// PID Segment reading
void
SetPidOutputSeg (void)
{
    if (_stricmp (paramName_m, "PatientIDSetID") == 0) {
        hl7Configuration_m->segmentPID.pidSetID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientIDExternalID") == 0) {
        hl7Configuration_m->segmentPID.pidPatientIDExternal = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientIDInternalID") == 0) {
        hl7Configuration_m->segmentPID.pidPatientIDInternal = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientIDAlternateID") == 0) {
        hl7Configuration_m->segmentPID.pidAlternatePatientID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientName") == 0) {
        hl7Configuration_m->segmentPID.pidPatientName = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MothersMaidenName") == 0) {
        hl7Configuration_m->segmentPID.pidMothersMaidenName = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DateTimeOfBirth") == 0) {
        hl7Configuration_m->segmentPID.pidDateTimeofBirth = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Sex") == 0) {
        hl7Configuration_m->segmentPID.pidSex = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientAlias") == 0) {
        hl7Configuration_m->segmentPID.pidPatientAlias = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Race") == 0) {
        hl7Configuration_m->segmentPID.pidRace = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientAddress") == 0) {
        hl7Configuration_m->segmentPID.pidPatientAddress = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CountryCode") == 0) {
        hl7Configuration_m->segmentPID.pidCountryCode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PhoneNumberHome") == 0) {
        hl7Configuration_m->segmentPID.pidPhoneNumberHome = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PhoneNumberBusiness") == 0) {
        hl7Configuration_m->segmentPID.pidPhoneNumberBusiness = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PrimaryLanguage") == 0) {
        hl7Configuration_m->segmentPID.pidPrimaryLanguage = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MaritalStatus") == 0) {
        hl7Configuration_m->segmentPID.pidMaritalStatus = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Religion") == 0) {
        hl7Configuration_m->segmentPID.pidReligion = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientAccountNumber") == 0) {
        hl7Configuration_m->segmentPID.pidPatientAccountNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SSNPatinet") == 0) {
        hl7Configuration_m->segmentPID.pidSSNNumberPatient = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientDriversLicense") == 0) {
        hl7Configuration_m->segmentPID.pidDriversLicenseNumberPatient = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MothersIdentifier") == 0) {
        hl7Configuration_m->segmentPID.pidMothersIdentifier = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "EthnicGroup") == 0) {
        hl7Configuration_m->segmentPID.pidEthnicGroup = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "BirthPlace") == 0) {
        hl7Configuration_m->segmentPID.pidBirthPlace = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "MultipleBirthIndicators") == 0) {
        hl7Configuration_m->segmentPID.pidMultipleBirthIndicator = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "BirthOrder") == 0) {
        hl7Configuration_m->segmentPID.pidBirthOrder = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Citizenship") == 0) {
        hl7Configuration_m->segmentPID.pidCitizenship = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "VeteransMilitaryStatus") == 0) {
        hl7Configuration_m->segmentPID.pidVeteransMilitaryStatus = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Nationality") == 0) {
        hl7Configuration_m->segmentPID.pidNationality = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientDeathDateTime") == 0) {
        hl7Configuration_m->segmentPID.pidPatientDeathDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientDeathIndicator") == 0) {
        hl7Configuration_m->segmentPID.pidPatientDeathIndicator = atoi (paramValue_m);
    }
}

// PV1 Segment reading
void
SetPv1OutputSeg (void)
{
    if (_stricmp (paramName_m, "SetID") == 0) {
        hl7Configuration_m->segmentPV1.pv1SetID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientClass") == 0) {
        hl7Configuration_m->segmentPV1.pv1PatientClass = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AssignedLocation") == 0) {
        hl7Configuration_m->segmentPV1.pv1AssignedPatientLocation = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AdmissionType") == 0) {
        hl7Configuration_m->segmentPV1.pv1AdmissionType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PreadmitNumber") == 0) {
        hl7Configuration_m->segmentPV1.pv1PreadmitNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PriorPatientLocation") == 0) {
        hl7Configuration_m->segmentPV1.pv1PriorPatientLocation = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AttendingDoctor") == 0) {
        hl7Configuration_m->segmentPV1.pv1AttendingDoctor = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "RefferingDoctor") == 0) {
        hl7Configuration_m->segmentPV1.pv1ReferringDoctor = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ConsultingDoctor") == 0) {
        hl7Configuration_m->segmentPV1.pv1ConsultingDoctor = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "HospitalService") == 0) {
        hl7Configuration_m->segmentPV1.pv1HospitalService = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "TempLocation") == 0) {
        hl7Configuration_m->segmentPV1.pv1TemporaryLocation = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PreadmitTestIndicator") == 0) {
        hl7Configuration_m->segmentPV1.pv1PreadmitTestIndicator = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReadmissionIndicator") == 0) {
        hl7Configuration_m->segmentPV1.pv1ReadmissionIndicator = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AdmitSource") == 0) {
        hl7Configuration_m->segmentPV1.pv1AdmitSource = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AmbulatoryStatus") == 0) {
        hl7Configuration_m->segmentPV1.pv1AmbulatoryStatus = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "VIPIndicator") == 0) {
        hl7Configuration_m->segmentPV1.pv1VIPIndicator = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AdmittingDoctor") == 0) {
        hl7Configuration_m->segmentPV1.pv1AdmittingDoctor = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PatientType") == 0) {
        hl7Configuration_m->segmentPV1.pv1PatientType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "VisitNumber") == 0) {
        hl7Configuration_m->segmentPV1.pv1VisitNumber = atoi (paramValue_m);
    }
}

// OBR Segment reading
void
SetObrOutputSeg (void)
{
    if (_stricmp (paramName_m, "SetID") == 0) { 
        hl7Configuration_m->segmentOBR.obrSetID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PlacerOrderNumber") == 0) { 
        hl7Configuration_m->segmentOBR.obrPlacerOrderNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "FillerOrderNumber") == 0) { 
        hl7Configuration_m->segmentOBR.obrFillerOrderNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "UniversalServiceID") == 0) { 
        hl7Configuration_m->segmentOBR.obrUniversalServiceID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Priority") == 0) { 
        hl7Configuration_m->segmentOBR.obrPriority = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "RequestedDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrRequestedDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrObservationDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationEndDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrObservationEndDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CollectionVolume") == 0) { 
        hl7Configuration_m->segmentOBR.obrCollectionVolume = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CollectorIdentifier") == 0) { 
        hl7Configuration_m->segmentOBR.obrCollectorIdentifier = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SpecimenActionCode") == 0) { 
        hl7Configuration_m->segmentOBR.obrSpecimenActionCode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DangerCode") == 0) { 
        hl7Configuration_m->segmentOBR.obrDangerCode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "RelevantClicinalInfo") == 0) { 
        hl7Configuration_m->segmentOBR.obrRelevantClinicalInfo = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SpecimenReceivedDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrSpecimenReceivedDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SpecimenSource") == 0) { 
        hl7Configuration_m->segmentOBR.obrSpecimenSource = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "OrderingProvider") == 0) { 
        hl7Configuration_m->segmentOBR.obrOrderingProvider = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "OrderCallbackPhoneNumber") == 0) { 
        hl7Configuration_m->segmentOBR.obrOrderCallbackPhoneNumber = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PlacerField1") == 0) { 
        hl7Configuration_m->segmentOBR.obrPlacerField1 = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PlacerField2") == 0) { 
        hl7Configuration_m->segmentOBR.obrPlacerField2 = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "FillerField1") == 0) { 
        hl7Configuration_m->segmentOBR.obrFillerField1 = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "FillerField2") == 0) { 
        hl7Configuration_m->segmentOBR.obrFillerField2 = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ResultsRptChangeDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrResultsStatusChngDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ChargeToPractice") == 0) { 
        hl7Configuration_m->segmentOBR.obrChargetoPractice = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DiagnosticServSectID") == 0) { 
        hl7Configuration_m->segmentOBR.obrDiagnosticServSectID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ResultsStatus") == 0) { 
        hl7Configuration_m->segmentOBR.obrResultStatus = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "QuantityTiming") == 0) { 
        hl7Configuration_m->segmentOBR.obrQuantityTiming = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ResultsCopiesTo") == 0) { 
        hl7Configuration_m->segmentOBR.obrResultCopiesTo = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Parent") == 0) { 
        hl7Configuration_m->segmentOBR.obrParent = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "TransportationMode") == 0) { 
        hl7Configuration_m->segmentOBR.obrTransportationMode = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReasonForStudy") == 0) { 
        hl7Configuration_m->segmentOBR.obrReasonForStudy = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PrincipalResultInterpreter") == 0) { 
        hl7Configuration_m->segmentOBR.obrPrincipalResultInterpreter = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AssistantResultInterpreter") == 0) { 
        hl7Configuration_m->segmentOBR.obrAssistantResultInterpreter = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Technician") == 0) { 
        hl7Configuration_m->segmentOBR.obrTechnician = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Transcriptionist") == 0) { 
        hl7Configuration_m->segmentOBR.obrTranscriptionist = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ScheduledDateTime") == 0) { 
        hl7Configuration_m->segmentOBR.obrScheduledDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "NumberOfSampleContainers") == 0) { 
        hl7Configuration_m->segmentOBR.obrNumberofSampleContainers = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "TransportLogistics") == 0) { 
        hl7Configuration_m->segmentOBR.obrTransportLogisticsofCollectedSample = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CollectorsComment") == 0) { 
        hl7Configuration_m->segmentOBR.obrCollectorsComment = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "TransportArrangementResponsibility") == 0) { 
        hl7Configuration_m->segmentOBR.obrTransportArrangementResponsibility = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "TransportArranged") == 0) { 
        hl7Configuration_m->segmentOBR.obrTransportArranged = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "EscortRequired") == 0) { 
        hl7Configuration_m->segmentOBR.obrEscortRequired = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "PlannedPatientTransportComment") == 0) { 
        hl7Configuration_m->segmentOBR.obrPlannedPatientTransportComment = atoi (paramValue_m);
    }
}

// OBX Segment reading
void
SetObxOutputSeg (void)
{
    if (_stricmp (paramName_m, "SetID") == 0) { 
        hl7Configuration_m->segmentOBX.obxSetID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ValueType") == 0) { 
        hl7Configuration_m->segmentOBX.obxValueType = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationIdentifier") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservationIdentifier = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationSubID") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservationSubId = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationValue") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservationValue = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Units") == 0) { 
        hl7Configuration_m->segmentOBX.obxUnits = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ReferenceRange") == 0) { 
        hl7Configuration_m->segmentOBX.obxReferenceRange = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "AbnormalFlags") == 0) { 
        hl7Configuration_m->segmentOBX.obxAbnormalFlags = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "Probability") == 0) { 
        hl7Configuration_m->segmentOBX.obxProbability = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "NatureOfAbnormalTest") == 0) { 
        hl7Configuration_m->segmentOBX.obxNatureofAbnormalTest = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservResultStatus") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservResultStatus = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "UserDefinedAccessChecks") == 0) { 
        hl7Configuration_m->segmentOBX.obxUserDefinedAccessChecks = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "DateTimeOfObservation") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservationDateTime = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ProducersID") == 0) { 
        hl7Configuration_m->segmentOBX.obxProducerId = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ResponsibleObserver") == 0) { 
        hl7Configuration_m->segmentOBX.obxResponsibleObserver = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "ObservationMethod") == 0) { 
        hl7Configuration_m->segmentOBX.obxObservationMethod = atoi (paramValue_m);
    }
}

// NTE Segment reading
void
SetNteOutputSeg (void)
{
    if (_stricmp (paramName_m, "SetID") == 0) {
        hl7Configuration_m->segmentNTE.nteSetID = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SourceOfComment") == 0) {
        hl7Configuration_m->segmentNTE.nteCommentSource = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CommentText") == 0) {
        hl7Configuration_m->segmentNTE.nteCommentText = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "CommentType") == 0) {
        hl7Configuration_m->segmentNTE.nteCommentType = atoi (paramValue_m);
    }

}

// BLK Segment reading
void
SetBlkOutputSeg (void)
{
    if (_stricmp (paramName_m, "SetSB") == 0) {
        hl7Configuration_m->segmentBLK.blockStart = atoi (paramValue_m);
    }
    if (_stricmp (paramName_m, "SetEB") == 0) {
        hl7Configuration_m->segmentBLK.blockEnd = atoi (paramValue_m);
    }
}

// call each segment reading function based on segment flag
static void
SetParam()
{

    if (segmentRead == MSH_INPUT_SEG) {
        SetMshInputSeg ();
    } else if (segmentRead == QRD_INPUT_SEG) {
        SetQrdInputSeg ();
    } else if (segmentRead == MSH_OUTPUT_SGE) {
        SetMshOutputSeg ();
    } else if (segmentRead == PID_OUTPUT_SEG) {
        SetPidOutputSeg ();
    } else if (segmentRead == PV1_OUTPUT_SEG) {
        SetPv1OutputSeg ();
    } else if (segmentRead == OBR_OUTPUT_SEG) {
        SetObrOutputSeg ();
    } else if (segmentRead == OBX_OUTPUT_SEG) {
        SetObxOutputSeg ();
    } else if (segmentRead == NTE_OUTPUT_SEG) {
        SetNteOutputSeg ();
    } else if (segmentRead == BLK_OUTPUT_SEG) {
        SetBlkOutputSeg ();
    }
}

static void
CharacterHandler (void * userData, const XML_Char * s, int len)
{
    strncat( paramValue_m, s, len );
}

// set segment flag for segment reading
void
SetSegmentFlag (const char *el)
{
       if (_stricmp (el, "segmentMSHIn") == 0) {
            segmentRead = MSH_INPUT_SEG;
            //printf ("msh input segment\n\r");
        } else if (_stricmp (el, "segmentQRD") == 0) {
            segmentRead = QRD_INPUT_SEG;
            //printf ("qrd input segment\n\r");
        } else if (_stricmp (el, "segmentMSHOut") == 0) {
            segmentRead = MSH_OUTPUT_SGE;
            //printf ("hsh output segment\n\r");
        } else if (_stricmp (el, "segmentPID") == 0) {
            segmentRead = PID_OUTPUT_SEG;
            //printf ("pid output segment\n\r");
        } else if (_stricmp (el, "segmentPV1") == 0) {
            segmentRead = PV1_OUTPUT_SEG;
            //printf ("pv1 output segment\n\r");
        } else if (_stricmp (el, "segmentOBR") == 0) {
            segmentRead = OBR_OUTPUT_SEG;
            //printf ("obr output segment\n\r");
        } else if (_stricmp (el, "segmentOBX") == 0) {
            segmentRead = OBX_OUTPUT_SEG;
            //printf ("obr output segment\n\r");
        } else if (_stricmp (el, "segmentNTE") == 0) {
            segmentRead = NTE_OUTPUT_SEG;
            //printf ("nte output segment\n\r");
        } else if (_stricmp (el, "segmentBLK") == 0) {
            segmentRead = BLK_OUTPUT_SEG;
            //printf ("nte output segment\n\r");
        }  
}

// Callback function for the expat parser.  This is called when an start tag is detected

static void
StartTag (void *data, const char *el, const char **attr)
{
    /*
     * If we're in the data section (between the "Record" tags), this is a
     * parameter in the file.  We save the name and activate the character data
     * handler to receive the tag data.
     */
    if (inData_m) {
        /* set segment flag */
        SetSegmentFlag (el);
        paramName_m = _strdup (el);
        if (attr[0] != 0) {
            attributes_m[0] = _strdup (attr[0]);

            if (attr[1] != 0) {
                attributes_m[1] = _strdup( attr[1] );
            }
        }

        XML_SetCharacterDataHandler (parser_m, CharacterHandler);
        paramValue_m[0] = '\0';
        inParam_m = true;
    }

    /*
     * A "Records" tag indicates we've started the section with the parameter
     * data.
     */
    else if (_stricmp( el, "Records" ) == 0) {
        inData_m = true;
    }

    /*
     * A "Version" tag indicates this is the file version.  We activate the
     * character data handler to receive the version number.
     */
    else if (_stricmp( el, "Version" ) == 0) {
        XML_SetCharacterDataHandler (parser_m, CharacterHandler);
        paramValue_m[0] = '\0';
        inVersion_m = true;
    }

    /*
     * A "Signature" tag indicates this is the file signature (CRC).  We
     * activate the character data handler to receive the CRC.
     */
    else if (_stricmp( el, "Signature" ) == 0) {
        XML_SetCharacterDataHandler( parser_m, CharacterHandler );
        paramValue_m[0] = '\0';
        inCrc_m = true;
    }
}

// Callback function for the expat parser.  This is called when an end tag is detected

static void
EndTag (void *data, const char *el)
{
    // Turn off the character data handler.
    XML_SetCharacterDataHandler (parser_m, 0);

    /*
     * We're in the data section (between the "Record" and "/Record" tags), so
     * we've either got the end of data for a parameter or the end of the data
     * section.
     */
    if (inData_m) {
        // End of data section.
        if (_stricmp( el, "Records" ) == 0) {
            inData_m = false;
        }

        // End of data for a parameter.
        else if (inParam_m) {
            // Update the calculated CRC value.  (NOTE: Do this before calling
            // SetParam(), which may modify paramValue_m. )
            calculatedCrc_m = Crc32Update (calculatedCrc_m, (unsigned char *)paramValue_m, strlen (paramValue_m));

            // Save the value if it's a parameter we're looking for.
            SetParam ();

            // Free the memory we allocated for storing the parameter name and
            // attribute, and signal we're no longer in a parameter.
            free (paramName_m);
            paramName_m = 0;
            if (attributes_m[0] != 0) {
                free (attributes_m[0]);
                attributes_m[0] = 0;
            }
            if (attributes_m[1] != 0) {
                free(attributes_m[1]);
                attributes_m[1] = 0;
            }
            inParam_m = false;
        }
    }
    /*
     * If we're in the Version, convert the string to a version number and save
     * it.  Update the CRC.
     */
    else if (inVersion_m) {
        sscanf (paramValue_m, "%u", version_m);
        calculatedCrc_m = Crc32Update (calculatedCrc_m, (unsigned char *)paramValue_m, strlen (paramValue_m));
        inVersion_m = false;
    }

    /*
     * If we're in the CRC, convert the string to a CRC number and save it.
     */
    else if (inCrc_m) {
        receivedCrc_m = strtoul (paramValue_m, 0, 0);
        inCrc_m = false;
    }
}

// calculate data HL7 structure crc for share memory checking
unsigned int
CalculateMemoryCrc (Hl7Configuration_t  *hl7Configuration)
{
    unsigned int crc = 0;
    unsigned int length, i;
    unsigned char     *ptr;

    ptr = (unsigned char *)hl7Configuration;
    // length should not include chceksum itself
    length =  sizeof (Hl7Configuration_t) - sizeof (unsigned int);

    crc =  Crc32Init();
    for (i =0; i < length; i++) {
        crc = Crc32Update (crc, (unsigned char *)ptr, 1);
        ptr++;
    }
    return crc;
}

// Write Nte Segment values to file
void
WriteNteSegment (FILE *  fp, NTESegment_t  *segmentNTE)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentNTE>\n");


    sprintf (dataBuff, "%d",  segmentNTE->nteSetID);
    fprintf (fp, "      <SetID>%s</SetID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentNTE->nteCommentSource);
    fprintf (fp, "      <SourceOfComment>%s</SourceOfComment>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentNTE->nteCommentText);
    fprintf (fp, "      <CommentText>%s</CommentText>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentNTE->nteCommentType);
    fprintf (fp, "      <CommentType>%s</CommentType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentNTE>\n");
}

// Write Obx Segment values to file
void
WriteObxSegment (FILE *  fp, OBXSegment_t  *segmentOBX)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentOBX>\n");

    sprintf (dataBuff, "%d",  segmentOBX->obxSetID);
    fprintf (fp, "      <SetID>%s</SetID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxValueType);
    fprintf (fp, "      <ValueType>%s</ValueType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservationIdentifier);
    fprintf (fp, "      <ObservationIdentifier>%s</ObservationIdentifier>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservationSubId);
    fprintf (fp, "      <ObservationSubID>%s</ObservationSubID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservationValue);
    fprintf (fp, "      <ObservationValue>%s</ObservationValue>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxUnits);
    fprintf (fp, "      <Units>%s</Units>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxReferenceRange);
    fprintf (fp, "      <ReferenceRange>%s</ReferenceRange>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxAbnormalFlags);
    fprintf (fp, "      <AbnormalFlags>%s</AbnormalFlags>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxProbability);
    fprintf (fp, "      <Probability>%s</Probability>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxNatureofAbnormalTest);
    fprintf (fp, "      <NatureOfAbnormalTest>%s</NatureOfAbnormalTest>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservResultStatus);
    fprintf (fp, "      <ObservResultStatus>%s</ObservResultStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxDataLastObsNormalValues);
    fprintf (fp, "      <DataLastObsNormalValues>%s</DataLastObsNormalValues>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxUserDefinedAccessChecks);
    fprintf (fp, "      <UserDefinedAccessChecks>%s</UserDefinedAccessChecks>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservationDateTime);
    fprintf (fp, "      <DateTimeOfObservation>%s</DateTimeOfObservation>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxProducerId);
    fprintf (fp, "      <ProducersID>%s</ProducersID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxResponsibleObserver);
    fprintf (fp, "      <ResponsibleObserver>%s</ResponsibleObserver>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBX->obxObservationMethod);
    fprintf (fp, "      <ObservationMethod>%s</ObservationMethod>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentOBX>\n");
}

// Write Obr Segment values to file
void
WriteObrSegment (FILE *  fp, OBRSegment_t  *segmentOBR)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentOBR>\n");

    sprintf (dataBuff, "%d",  segmentOBR->obrSetID);
    fprintf (fp, "      <SetID>%s</SetID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPlacerOrderNumber);
    fprintf (fp, "      <PlacerOrderNumber>%s</PlacerOrderNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrFillerOrderNumber);
    fprintf (fp, "      <FillerOrderNumber>%s</FillerOrderNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrUniversalServiceID);
    fprintf (fp, "      <UniversalServiceID>%s</UniversalServiceID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPriority);
    fprintf (fp, "      <Priority>%s</Priority>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrRequestedDateTime);
    fprintf (fp, "      <RequestedDateTime>%s</RequestedDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrObservationDateTime);
    fprintf (fp, "      <ObservationDateTime>%s</ObservationDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrObservationEndDateTime);
    fprintf (fp, "      <ObservationEndDateTime>%s</ObservationEndDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrCollectionVolume);
    fprintf (fp, "      <CollectionVolume>%s</CollectionVolume>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrCollectorIdentifier);
    fprintf (fp, "      <CollectorIdentifier>%s</CollectorIdentifier>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrSpecimenActionCode);
    fprintf (fp, "      <SpecimenActionCode>%s</SpecimenActionCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrDangerCode);
    fprintf (fp, "      <DangerCode>%s</DangerCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrRelevantClinicalInfo);
    fprintf (fp, "      <RelevantClicinalInfo>%s</RelevantClicinalInfo>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrSpecimenReceivedDateTime);
    fprintf (fp, "      <SpecimenReceivedDateTime>%s</SpecimenReceivedDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrSpecimenSource);
    fprintf (fp, "      <SpecimenSource>%s</SpecimenSource>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrOrderingProvider);
    fprintf (fp, "      <OrderingProvider>%s</OrderingProvider>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrOrderCallbackPhoneNumber);
    fprintf (fp, "      <OrderCallbackPhoneNumber>%s</OrderCallbackPhoneNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPlacerField1);
    fprintf (fp, "      <PlacerField1>%s</PlacerField1>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPlacerField2);
    fprintf (fp, "      <PlacerField2>%s</PlacerField2>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrFillerField1);
    fprintf (fp, "      <FillerField1>%s</FillerField1>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrFillerField2);
    fprintf (fp, "      <FillerField2>%s</FillerField2>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrResultsStatusChngDateTime);
    fprintf (fp, "      <ResultsRptChangeDateTime>%s</ResultsRptChangeDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrChargetoPractice);
    fprintf (fp, "      <ChargeToPractice>%s</ChargeToPractice>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrDiagnosticServSectID);
    fprintf (fp, "      <DiagnosticServSectID>%s</DiagnosticServSectID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrResultStatus);
    fprintf (fp, "      <ResultsStatus>%s</ResultsStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrParentResult);
    fprintf (fp, "      <ParentStatus>%s</ParentStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrQuantityTiming);
    fprintf (fp, "      <QuantityTiming>%s</QuantityTiming>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrResultCopiesTo);
    fprintf (fp, "      <ResultsCopiesTo>%s</ResultsCopiesTo>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrParent);
    fprintf (fp, "      <Parent>%s</Parent>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTransportationMode);
    fprintf (fp, "      <TransportationMode>%s</TransportationMode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrReasonForStudy);
    fprintf (fp, "      <ReasonForStudy>%s</ReasonForStudy>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPrincipalResultInterpreter);
    fprintf (fp, "      <PrincipalResultInterpreter>%s</PrincipalResultInterpreter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrAssistantResultInterpreter);
    fprintf (fp, "      <AssistantResultInterpreter>%s</AssistantResultInterpreter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTechnician);
    fprintf (fp, "      <Technician>%s</Technician>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTranscriptionist);
    fprintf (fp, "      <Transcriptionist>%s</Transcriptionist>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrScheduledDateTime);
    fprintf (fp, "      <ScheduledDateTime>%s</ScheduledDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrNumberofSampleContainers);
    fprintf (fp, "      <NumberOfSampleContainers>%s</NumberOfSampleContainers>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTransportLogisticsofCollectedSample);
    fprintf (fp, "      <TransportLogistics>%s</TransportLogistics>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrCollectorsComment);
    fprintf (fp, "      <CollectorsComment>%s</CollectorsComment>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTransportArrangementResponsibility);
    fprintf (fp, "      <TransportArrangementResponsibility>%s</TransportArrangementResponsibility>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrTransportArranged);
    fprintf (fp, "      <TransportArranged>%s</TransportArranged>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrEscortRequired);
    fprintf (fp, "      <EscortRequired>%s</EscortRequired>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentOBR->obrPlannedPatientTransportComment);
    fprintf (fp, "      <PlannedPatientTransportComment>%s</PlannedPatientTransportComment>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentOBR>\n");
}

// Write Pv1 Segment values to file
void
WritePv1Segment (FILE *  fp, PV1Segment_t  *segmentPV1)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentPV1>\n");

    sprintf (dataBuff, "%d",  segmentPV1->pv1SetID);
    fprintf (fp, "      <SetID>%s</SetID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1PatientClass);
    fprintf (fp, "      <PatientClass>%s</PatientClass>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AssignedPatientLocation);
    fprintf (fp, "      <AssignedLocation>%s</AssignedLocation>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AdmissionType);
    fprintf (fp, "      <AdmissionType>%s</AdmissionType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1PreadmitNumber);
    fprintf (fp, "      <PreadmitNumber>%s</PreadmitNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1PriorPatientLocation);
    fprintf (fp, "      <PriorPatientLocation>%s</PriorPatientLocation>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AttendingDoctor);
    fprintf (fp, "      <AttendingDoctor>%s</AttendingDoctor>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1ReferringDoctor);
    fprintf (fp, "      <RefferingDoctor>%s</RefferingDoctor>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1ConsultingDoctor);
    fprintf (fp, "      <ConsultingDoctor>%s</ConsultingDoctor>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1HospitalService);
    fprintf (fp, "      <HospitalService>%s</HospitalService>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1TemporaryLocation);
    fprintf (fp, "      <TempLocation>%s</TempLocation>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1PreadmitTestIndicator);
    fprintf (fp, "      <PreadmitTestIndicator>%s</PreadmitTestIndicator>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1ReadmissionIndicator);
    fprintf (fp, "      <ReadmissionIndicator>%s</ReadmissionIndicator>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AdmitSource);
    fprintf (fp, "      <AdmitSource>%s</AdmitSource>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AmbulatoryStatus);
    fprintf (fp, "      <AmbulatoryStatus>%s</AmbulatoryStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1VIPIndicator);
    fprintf (fp, "      <VIPIndicator>%s</VIPIndicator>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1AdmittingDoctor);
    fprintf (fp, "      <AdmittingDoctor>%s</AdmittingDoctor>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1PatientType);
    fprintf (fp, "      <PatientType>%s</PatientType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPV1->pv1VisitNumber);
    fprintf (fp, "      <VisitNumber>%s</VisitNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentPV1>\n" );
}

// Write PID Segment values to file
void
WritePidSegment (FILE *  fp, PIDSegment_t  *segmentPID)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentPID>\n" );

    sprintf (dataBuff, "%d",  segmentPID->pidSetID);
    fprintf (fp, "      <PatientIDSetID>%s</PatientIDSetID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientIDExternal);
    fprintf (fp, "      <PatientIDExternalID>%s</PatientIDExternalID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientIDInternal);
    fprintf (fp, "      <PatientIDInternalID>%s</PatientIDInternalID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidAlternatePatientID);
    fprintf (fp, "      <PatientIDAlternateID>%s</PatientIDAlternateID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientName);
    fprintf (fp, "      <PatientName>%s</PatientName>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidMothersMaidenName);
    fprintf (fp, "      <MothersMaidenName>%s</MothersMaidenName>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidDateTimeofBirth);
    fprintf (fp, "      <DateTimeOfBirth>%s</DateTimeOfBirth>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidSex);
    fprintf (fp, "      <Sex>%s</Sex>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientAlias);
    fprintf (fp, "      <PatientAlias>%s</PatientAlias>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidRace);
    fprintf (fp, "      <Race>%s</Race>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientAddress);
    fprintf (fp, "      <PatientAddress>%s</PatientAddress>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidCountryCode);
    fprintf (fp, "      <CountryCode>%s</CountryCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPhoneNumberHome);
    fprintf (fp, "      <PhoneNumberHome>%s</PhoneNumberHome>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPhoneNumberBusiness);
    fprintf (fp, "      <PhoneNumberBusiness>%s</PhoneNumberBusiness>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPrimaryLanguage);
    fprintf (fp, "      <PrimaryLanguage>%s</PrimaryLanguage>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidMaritalStatus);
    fprintf (fp, "      <MaritalStatus>%s</MaritalStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidReligion);
    fprintf (fp, "      <Religion>%s</Religion>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientAccountNumber);
    fprintf (fp, "      <PatientAccountNumber>%s</PatientAccountNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidSSNNumberPatient);
    fprintf (fp, "      <SSNPatinet>%s</SSNPatinet>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidDriversLicenseNumberPatient);
    fprintf (fp, "      <PatientDriversLicense>%s</PatientDriversLicense>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidMothersIdentifier);
    fprintf (fp, "      <MothersIdentifier>%s</MothersIdentifier>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidEthnicGroup);
    fprintf (fp, "      <EthnicGroup>%s</EthnicGroup>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidBirthPlace);
    fprintf (fp, "      <BirthPlace>%s</BirthPlace>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidMultipleBirthIndicator);
    fprintf (fp, "      <MultipleBirthIndicators>%s</MultipleBirthIndicators>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidBirthOrder);
    fprintf (fp, "      <BirthOrder>%s</BirthOrder>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidCitizenship);
    fprintf (fp, "      <Citizenship>%s</Citizenship>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidVeteransMilitaryStatus);
    fprintf (fp, "      <VeteransMilitaryStatus>%s</VeteransMilitaryStatus>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidNationality);
    fprintf (fp, "      <Nationality>%s</Nationality>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientDeathDateTime);
    fprintf (fp, "      <PatientDeathDateTime>%s</PatientDeathDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentPID->pidPatientDeathIndicator);
    fprintf (fp, "      <PatientDeathIndicator>%s</PatientDeathIndicator>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentPID>\n" );
}

// Write Qrd Segment values to file
void
WriteQrdSegment (FILE *  fp, QRDSegment_t *segmentQRD)
{
    char        dataBuff [2000];

    fprintf (fp, "    <segmentQRD>\n" );
    sprintf (dataBuff, "%d",  segmentQRD->qrdQueryDateTime);
    fprintf (fp, "      <QueryDateTime>%s</QueryDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdQueryFormatCode);
    fprintf (fp, "      <QueryFormatCode>%s</QueryFormatCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdQueryPriority);
    fprintf (fp, "      <QueryPriority>%s</QueryPriority>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdQueryID);
    fprintf (fp, "      <QueryID>%s</QueryID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdDeferredResponseType);
    fprintf (fp, "      <DeferredResponseType>%s</DeferredResponseType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdDeferredResponseDateTime);
    fprintf (fp, "      <DeferredResponseDateTime>%s</DeferredResponseDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdQuantityLimitedRequest);
    fprintf (fp, "      <QuantityLimitedRequest>%s</QuantityLimitedRequest>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdWhoSubjectFilter);
    fprintf (fp, "      <WhoSubjectFilter>%s</WhoSubjectFilter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdWhatSubjectFilter);
    fprintf (fp, "      <WhatSubjectFilter>%s</WhatSubjectFilter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdWhatDepartmentDataCode);
    fprintf (fp, "      <WhatDepartmentDataCode>%s</WhatDepartmentDataCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdWhatDataCodeValueQual);
    fprintf (fp, "      <WhatDataCodeValueQual>%s</WhatDataCodeValueQual>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentQRD->qrdWhatQueryResultsLevel);
    fprintf (fp, "      <QueryResultsLevel>%s</QueryResultsLevel>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    fprintf (fp, "    </segmentQRD>\n" );
}

// write MSH segment values to file
void
WriteMshSegment (FILE *  fp, MSHSegment_t *segmentMSH, char input)
{
    char        dataBuff [2000];

    if (input == 1) {
        fprintf (fp, "    <segmentMSHIn>\n" );
    } else {
        fprintf (fp, "    <segmentMSHOut>\n" );
    }

    sprintf (dataBuff, "%c",  segmentMSH->mshFieldDelimiter);
    fprintf (fp, "      <FieldDelimiter>%s</FieldDelimiter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%c",  segmentMSH->mshComponentDelimiter);
    fprintf (fp, "      <ComponentDelimiter>%s</ComponentDelimiter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%c",  segmentMSH->mshRepeatDelimiter);
    fprintf (fp, "      <RepeatDelimiter>%s</RepeatDelimiter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%c",  segmentMSH->mshSubComponentDelimiter);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));
    // special '&', must put "&amp;" string to xml file instead of '&' 
    if (segmentMSH->mshSubComponentDelimiter == '&'){
       strcpy (dataBuff, "&amp;");
    }
    fprintf (fp, "      <SubComponentDelimter>%s</SubComponentDelimter>\n", dataBuff);

    sprintf (dataBuff, "%c",  segmentMSH->mshEscapeCharacter);
    fprintf (fp, "      <EscapeCharacter>%s</EscapeCharacter>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshSendingApplication);
    fprintf (fp, "      <SendingApplication>%s</SendingApplication>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshSendingFacility);
    fprintf (fp, "      <SendingFacility>%s</SendingFacility>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshReceivingApplication);
    fprintf (fp, "      <ReceivingApplication>%s</ReceivingApplication>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshReceivingFacility);
    fprintf (fp, "      <ReceivingFacility>%s</ReceivingFacility>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshTimeofMessage);
    fprintf (fp, "      <MsgDateTime>%s</MsgDateTime>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshSecurity);
    fprintf (fp, "      <Security>%s</Security>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshMessageType);
    fprintf (fp, "      <MessageType>%s</MessageType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshMessageControlID);
    fprintf (fp, "      <MessageControlID>%s</MessageControlID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshProcessingID);
    fprintf (fp, "      <ProcessingID>%s</ProcessingID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshVersionID);
    fprintf (fp, "      <VersionID>%s</VersionID>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshSequenceNumber);
    fprintf (fp, "      <SeqNumber>%s</SeqNumber>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshContinuationPointer);
    fprintf (fp, "      <ContinuationPointer>%s</ContinuationPointer>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshAcceptAcknowledgementType);
    fprintf (fp, "      <AcceptAckType>%s</AcceptAckType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshAcknowledgementType);
    fprintf (fp, "      <ApplicationACKType>%s</ApplicationACKType>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshCountryCode);
    fprintf (fp, "      <CountryCode>%s</CountryCode>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshCountrySet);
    fprintf (fp, "      <CharacterSet>%s</CharacterSet>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    sprintf (dataBuff, "%d",  segmentMSH->mshPrincipalLanguage);
    fprintf (fp, "      <MsgLanguage>%s</MsgLanguage>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen( dataBuff));

    if (input == 1) {
        fprintf (fp, "    </segmentMSHIn>\n" );
    } else {
        fprintf (fp, "    </segmentMSHOut>\n" );
    }
}

// write default HL7 configruation values to xml file 
CfgError_t
WriteHL7Configuration (Hl7Configuration_t  *hl7Configuration)
{
    FILE *              fp;
    char                dataBuff [2000];
    wchar_t             wDirectroyName [128];
    HANDLE              hSearch;
    WIN32_FIND_DATA     fileData;

    // create directory
    wsprintf (wDirectroyName, L"%S",  HL7_CONFIG_FILE_PATH);
    hSearch = FindFirstFile (wDirectroyName, &fileData);
    if (hSearch == INVALID_HANDLE_VALUE) {
        if (CreateDirectory (wDirectroyName, NULL) == 0) {
            LogError ("Fail to create %S directory");
        }
    } else {
        FindClose (hSearch);
    }

    
    fp = fopen (HL7_CONFIG_FILE_NAME, "wb");
    if (fp == 0) {
        return CFG_ERR_FILE_OPEN;
    }

    // Initialize the CRC.
    wCrc_m = Crc32Init ();

    // Write the file header.
    fprintf (fp, "<?xml version='1.0' encoding='UTF-8'?>\n");
    fprintf (fp, "<!-- HL7 configuration -->\n");
    fprintf (fp, "<Configuration>\n");
    sprintf (dataBuff, "%u", HL7_FILE_VERSION);
    fprintf (fp, "  <Version>%s</Version>\n", dataBuff);
    wCrc_m = Crc32Update (wCrc_m, (unsigned char *)dataBuff, strlen(dataBuff));
    // Write the parameters.
    fprintf (fp, "  <Records>\n" );
    WriteMshSegment (fp, &hl7Configuration->segmentMSHIn, 1);
    WriteQrdSegment (fp, &hl7Configuration->segmentQRD);
    WriteMshSegment (fp, &hl7Configuration->segmentMSHOut, 0);
    WritePidSegment (fp, &hl7Configuration->segmentPID);
    WritePv1Segment (fp, &hl7Configuration->segmentPV1);
    WriteObrSegment (fp, &hl7Configuration->segmentOBR);
    WriteObxSegment (fp, &hl7Configuration->segmentOBX);
    WriteNteSegment (fp, &hl7Configuration->segmentNTE);
    fprintf( fp, "  </Records>\n" );
    // Write the file signature.
    fprintf (fp, "  <Signature>%lu</Signature>\n", Crc32Finalize (wCrc_m));
    fprintf (fp, "</Configuration>\n");
    fclose (fp);
	return CFG_ERR_NONE;
}

// Set HL7 data structure to be default value
// Create HL7 configuration xml file
void
SetHL7DefaultConfigurationValues (Hl7Configuration_t *hl7Configuration)
{
    // get default values
    memcpy (&hl7Configuration->segmentMSHIn, &MSHSegmentInputDefaultSettings, sizeof (MSHSegment_t));
    memcpy (&hl7Configuration->segmentQRD,   &QRDSegmentDefaultSettings, sizeof (QRDSegment_t));
    memcpy (&hl7Configuration->segmentMSHOut,&MSHSegmentOutputDefaultSettings, sizeof (MSHSegment_t));
    memcpy (&hl7Configuration->segmentPID,   &PIDSegmentDefaultSettings, sizeof (PIDSegment_t));
    memcpy (&hl7Configuration->segmentPV1,   &PV1SegmentDefaultSettings, sizeof (PV1Segment_t));
    memcpy (&hl7Configuration->segmentOBR,   &OBRSegmentDefaultSettings, sizeof (OBRSegment_t));
    memcpy (&hl7Configuration->segmentOBX,   &OBXSegmentDefaultSettings, sizeof (OBXSegment_t));
    memcpy (&hl7Configuration->segmentNTE,   &NTESegmentDefaultSettings, sizeof (NTESegment_t));
    memcpy (&hl7Configuration->segmentBLK,   &BLKSegmentDefaultSettings, sizeof (BLKSegment_t));
    // save default value to HL7 xml file
    WriteHL7Configuration (hl7Configuration);
}

// Parse HL7 configuation file based on index number
CfgError_t
ParseHL7ConfigurationByIndex (Hl7Configuration_t  *hl7Configuration, unsigned short index)
{
    char    *fileName;
    static  char shareMemFlag = 0;

    // only allowed to mapp share memory one time for each process
    if (shareMemFlag ==0 ) {
        hl7FileListShareMem_m   = (Hl7ConfigurationFileList_t *)MapSharedMem (HL7_FILE_SHEME_NAME, sizeof (Hl7ConfigurationFileList_t));
        shareMemFlag = 1;
    }

    if (hl7FileListShareMem_m != NULL) {
        // in the case of index beyond max value, set default configuration file
        if (index >= hl7FileListShareMem_m->tIndex ) {
            LogDebug ("HL7 configuration file index=%d beyond max value, set default configuration", index);
            index = 0;
        }
        LogDebug ("hl7 file index =%d", index);
        // get required HL7 configuration file name
        fileName = hl7FileListShareMem_m->hl7FileName [index].fileName;
        return  (ParseHL7Configuration (hl7Configuration, fileName));
	} else {
		LogDebug ("Fail to mapp HL7 Configuration File List share memory");
		return CFG_ERR_UNSPECIFIED;
	}
}

// Read HL7 configuration file to memory
CfgError_t
ParseHL7Configuration (Hl7Configuration_t  *hl7Configuration, char *fileName)
{
    FILE *      fp;
    int         done;
    char        buff [BUFF_SIZE];
    unsigned    _version;

    // get version address 
    version_m =  &_version;

    // initial pointer
    hl7Configuration_m = hl7Configuration;

    memset (hl7Configuration_m, 0, sizeof (Hl7Configuration_t));


    LogDebug ("Parse HL7 Configuration file =%s", fileName);
    
    // Open the config file.
    fp = fopen (fileName, "rb");
    if (fp == 0) {
        // Configuration does not exist, create one 
        LogDebug ("HL7 Configuration file does not exist, create default one");
        SetHL7DefaultConfigurationValues (hl7Configuration_m);
    } else {
         //Create the parser and install the start and end tag callbacks.
        parser_m = XML_ParserCreate (0);
        if (parser_m == 0) {
            fclose (fp);
            return CFG_ERR_PARSER_CREATE;
        }
        XML_SetElementHandler (parser_m, StartTag, EndTag);
        //Clear flags and init the CRC.
        inData_m = false;
        inParam_m = false;
        inCrc_m = false;
        calculatedCrc_m = Crc32Init();
        for (done = 0; !done; ) {
            int len;
            // Read the file into the buffer.
            len = fread (buff, 1, BUFF_SIZE, fp);
            if (ferror (fp)) {
                fclose (fp);
                return CFG_ERR_FILE_READ;
            }
            // EOF, so done.
            done = feof (fp);

            // Parse the buffer.
            if (XML_Parse (parser_m, buff, len, done) == 0) {
                fclose (fp);
                return CFG_ERR_SYNTAX;
            }
        }
        fclose(fp);
        // Finalize the CRC calculation and compare it to the CRC read from the file.
        calculatedCrc_m = Crc32Finalize (calculatedCrc_m);
        if (calculatedCrc_m != receivedCrc_m) {
            return CFG_ERR_CRC;
        }
    }

    // calculated and saved RAM HL7 structure checksum for share memory checking
    hl7Configuration_m->crcCheckSum = CalculateMemoryCrc (hl7Configuration);
    //printf ("check = %lu\n", hl7Configuration_m->crcCheckSum);

    return CFG_ERR_NONE;
}
