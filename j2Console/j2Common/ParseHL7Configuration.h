#ifndef _PARSEHL7CONFIGURATION_H 
#define _PARSEHL7CONFIGURATION_H
#include "HL7Configuration.h"
#include "CfgErr.h"


extern CfgError_t 
ParseHL7Configuration (Hl7Configuration_t  *hl7Configuration, char *fileName);

extern CfgError_t ParseHL7ConfigurationByIndex (Hl7Configuration_t  *hl7Configuration, unsigned short index);

extern unsigned int
CalculateMemoryCrc (Hl7Configuration_t  *hl7Configuration);

extern CfgError_t
ParseHL7ConfigurationFileName (Hl7ConfigurationFileList_t  *hl7ConfigurationFileList);

#define HL7_FILE_VERSION        1

// define HL7 default values
const MSHSegment_t    MSHSegmentInputDefaultSettings = {
    '|',                // MSH-1 Field delimiter  '|',
    '^',                // MSH-2 Field delimiter  '^',
    '~',                // MSH-2 Field delimiter  '~',
    '&',                // MSH-2 Field delimiter  '&',
    '\\',               // MSH-2 Field delimiter  '\\',
    0,                  //mshSendingApplication;                 
    0,                  //mshSendingFacility;                     
    0,                  //mshReceivingApplication;                
    0,                  //mshReceivingFacility;                  
    13,                 //mshTimeofMessage;                      
    14,                 //mshSecurity;                            
    15,                 //mshMessageType;                        
    16,                 //mshMessageControlID;                    
    17,                 //mshProcessingID;                       
    18,                 //mshVersionID;                          
    0,                  //mshSequenceNumber;                      
    0,                  //mshContinuationPointer;                 
    0,                  //mshAcceptAcknowledgementType;           
    0,                  //mshAcknowledgementType;                 
    0,                  //mshCountryCode;                         
    0,                  //mshCountrySet;                          
    0,                  //mshPrincipalLanguage;                  
};

const QRDSegment_t     QRDSegmentDefaultSettings = {
    55,                 //qrdQueryDateTime;                      
    56,                 //qrdQueryFormatCode;                    
    57,                 //qrdQueryPriority;                      
    58,                 //qrdQueryID;                           
    59,                 //qrdDeferredResponseType;             
    60,                 //qrdDeferredResponseDateTime;        
    61,                 //qrdQuantityLimitedRequest;             
    62,                 //qrdWhoSubjectFilter;                   
    63,                 //qrdWhatSubjectFilter;                  
    64,                 //qrdWhatDepartmentDataCode;            
    65,                 //qrdWhatDataCodeValueQual;              
    0,                  //qrdWhatQueryResultsLevel;               
};

const MSHSegment_t    MSHSegmentOutputDefaultSettings = {
    '|',                // MSH-1 Field delimiter  '|',
    '^',                // MSH-2 Field delimiter  '^',
    '~',                // MSH-2 Field delimiter  '~',
    '&',                // MSH-2 Field delimiter  '&',
    '\\',               // MSH-2 Field delimiter  '\\',
    0,                  //mshSendingApplication;                 
    0,                  //mshSendingFacility;                     
    0,                  //mshReceivingApplication;                
    0,                  //mshReceivingFacility;                  
    13,                 //mshTimeofMessage;                      
    14,                 //mshSecurity;                            
    15,                 //mshMessageType;                        
    16,                 //mshMessageControlID;                    
    17,                 //mshProcessingID;                       
    18,                 //mshVersionID;                          
    0,                  //mshSequenceNumber;                      
    0,                  //mshContinuationPointer;                 
    0,                  //mshAcceptAcknowledgementType;           
    0,                  //mshAcknowledgementType;                 
    0,                  //mshCountryCode;                         
    0,                  //mshCountrySet;                          
    0,                  //mshPrincipalLanguage;                  
};

const PIDSegment_t    PIDSegmentDefaultSettings = {
    19,                 //pidSetID;                               
    20,                 //pidPatientIDExternal;                   
    21,                 //pidPatientIDInternal;                   
    22,                 //pidAlternatePatientID;                  
    23,                 //pidPatientName;                         
    0,                  //pidMothersMaidenName;                   
    24,                 //pidDateTimeofBirth;                    
    25,                 //pidSex;                                
    0,                  //pidPatientAlias;                       
    26,                 //pidRace;                               
    0,                  //pidPatientAddress;                      
    0,                  //pidCountryCode;                         
    27,                 //pidPhoneNumberHome;                     
    0,                  //pidPhoneNumberBusiness;                 
    0,                  //pidPrimaryLanguage;                     
    0,                  //pidMaritalStatus;                       
    0,                  //pidReligion;                            
    0,                  //pidPatientAccountNumber;               
    0,                  //pidSSNNumberPatient;                   
    0,                  //pidDriversLicenseNumberPatient;         
    0,                  //pidMothersIdentifier;                   
    0,                  //pidEthnicGroup;                         
    0,                  //pidBirthPlace;                         
    0,                  //pidMultipleBirthIndicator;             
    0,                  //pidBirthOrder;                         
    0,                  //pidCitizenship;                       
    0,                  //pidVeteransMilitaryStatus;              
    0,                  //pidNationality;                        
    0,                  //pidPatientDeathDateTime;                
    0,                  //pidPatientDeathIndicator;               
};

const PV1Segment_t    PV1SegmentDefaultSettings = {
    28,                 //pv1SetID;                               
    29,                 //pv1PatientClass;                        
    30,                 //pv1AssignedPatientLocation;             
    0,                  //pv1AdmissionType;                       
    0,                  //pv1PreadmitNumber;                      
    0,                  //pv1PriorPatientLocation;                
    0,                  //pv1AttendingDoctor;                     
    0,                  //pv1ReferringDoctor;                     
    0,                  //pv1ConsultingDoctor;                    
    0,                  //pv1HospitalService;                     
    0,                  //pv1TemporaryLocation;                   
    0,                  //pv1PreadmitTestIndicator;               
    0,                  //pv1ReadmissionIndicator;                
    0,                  //pv1AdmitSource;                         
    0,                  //pv1AmbulatoryStatus;                    
    0,                  //pv1VIPIndicator;                        
    0,                  //pv1AdmittingDoctor;                     
    0,                  //pv1PatientType;                         
    31,                 //pv1VisitNumber;                         
};

const OBRSegment_t    OBRSegmentDefaultSettings = {
    32,                  //obrSetID;                               
    0,                  //obrPlacerOrderNumber;                   
    0,                  //obrFillerOrderNumber;                   
    0,                  //obrUniversalServiceID;                  
    0,                  //obrPriority;                            
    35,                 //obrRequestedDateTime;                   
    36,                 //obrObservationDateTime;                 
    37,                 //obrObservationEndDateTime;              
    0,                  //obrCollectionVolume;                    
    38,                 //obrCollectorIdentifier;                 
    0,                  //obrSpecimenActionCode;                  
    0,                  //obrDangerCode;                          
    0,                  //obrRelevantClinicalInfo;                
    0,                  //obrSpecimenReceivedDateTime;            
    0,                  //obrSpecimenSource;                      
    39,                 //obrOrderingProvider;                    
    0,                  //obrOrderCallbackPhoneNumber;            
    0,                  //obrPlacerField1;                        
    0,                  //obrPlacerField2;                        
    0,                  //obrFillerField1;                        
    0,                  //obrFillerField2;                        
    0,                  //obrResultsStatusChngDateTime;           
    0,                  //obrChargetoPractice;                    
    0,                  //obrDiagnosticServSectID;                
    40,                 //obrResultStatus;                        
    0,                  //obrParentResult;                        
    0,                  //obrQuantityTiming;                      
    0,                  //obrResultCopiesTo;                      
    0,                  //obrParent;                              
    0,                  //obrTransportationMode;                  
    0,                  //obrReasonForStudy;                      
    0,                  //obrPrincipalResultInterpreter;          
    0,                  //obrAssistantResultInterpreter;          
    0,                  //obrTechnician;                          
    0,                  //obrTranscriptionist;                    
    0,                  //obrScheduledDateTime;                   
    0,                  //obrNumberofSampleContainers;            
    0,                  //obrTransportLogisticsofCollectedSample; 
    0,                  //obrCollectorsComment;                   
    0,                  //obrTransportArrangementResponsibility;  
    0,                  //obrTransportArranged;                   
    0,                  //obrEscortRequired;                      
    0,                  //obrPlannedPatientTransportComment;      
};

const OBXSegment_t    OBXSegmentDefaultSettings = {
    41,                 //obxSetID;                               
    0,                  //obxValueType;                           
    42,                 //obxObservationIdentifier;               
    0,                  //obxObservationSubId;                    
    43,                 //obxObservationValue;                    
    44,                 //obxUnits;                               
    45,                 //obxReferenceRange;                      
    46,                 //obxAbnormalFlags;                       
    0,                  //obxProbability;                         
    0,                  //obxNatureofAbnormalTest;                
    47,                 //obxObservResultStatus;                  
    0,                  //obxDataLastObsNormalValues;             
    0,                  //obxUserDefinedAccessChecks;             
    0,                  //obxObservationDateTime;                 
    0,                  //obxProducerId;                          
    0,                  //obxResponsibleObserver;                 
    48,                 //obxObservationMethod;                   
};

const NTESegment_t    NTESegmentDefaultSettings = {
    50,                 //nteSetID;                              
    51,                 //nteCommentSource;                     
    53,                 //nteCommentText;                      
    54,                 //nteCommentType;                     
};

const BLKSegment_t    BLKSegmentDefaultSettings = {
    0,                  // SB
    0,                  // EB
}; 
#endif
