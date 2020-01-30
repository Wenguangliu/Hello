#ifndef __RUNINFO_H_INCLUDED
#define __RUNINFO_H_INCLUDED

#include <time.h>

#include "CommonDefines.h"
#include "SystemErrorNums.h"

typedef struct
{
	int					rotorId;
	char				patientControl;			// 'P' for patient, or 'C' for control
	time_t				runTime;
	char				patientControlId[ ID_LEN ];
	char				operatorId[ ID_LEN ];
	char				alternateId[ ID_LEN ];
	char				doctorId[ ID_LEN ];
	char				location[ ID_LEN ];
	char				phoneNumber[ ID_LEN ];
	char				admissionOwnerId[ ID_LEN ];
	Date_t				dateOfBirth;
	Date_t				lastVaccination;
	char				sampleId[ ID_LEN ];
	Gender_t			gender;
	int					sampleType;
	PatientType_t		patientType;			// Used by CalcResults
	int					age;
	AgeUnits_t			ageUnits;				// Days, weeks, months or years
	Race_t				race;
	char				rocFileName[32];
	char				risFileName[32];
	char				radFileName[ MAX_RAD_FILENAME_LEN ];
	char				rotorName[32];
	SystemErrorNumber_t	errNum;
	bool				barcodeRead;
	bool				operatorExpired;
	bool				operatorUnauthorized;
} RunInfo_t;

#endif // __RUNINFO_H_INCLUDED
