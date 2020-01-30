#ifndef __RESULTSDB_H_INCLUDED
#define __RESULTSDB_H_INCLUDED

#include <time.h>

#include "CommonDefines.h"
#include "ResultsData.h"	// Defines RotorInformationResults_t & RotorAnalyteResult_t
#include "SystemErrorNums.h"

//#define RESULTS_DB_VERSION	6		// 250 entries (RESULTS_MAX)
//#define RESULTS_DB_VERSION	7		// 5000 entries ::: S/W versions ??? - 2.1.7
//#define RESULTS_DB_VERSION	8		// added flags for control expired, operator expired and unauthorized operator
//#define RESULTS_DB_VERSION	9		// intermediate version - not released
#define RESULTS_DB_VERSION		10		// added support for multiple alternate IDs (previously - could only select one)

#define RESULTS_DIRECTORY		"/Abaxis/Results"
#define CUSTOMER_DB_INDEX		RESULTS_DIRECTORY"/Results.ndx"
#define CUSTOMER_DB_DATA		RESULTS_DIRECTORY"/Results.db"
#define RESULTS_LOCK			RESULTS_DIRECTORY"/Lock"
#define OTHER_DB_INDEX			RESULTS_DIRECTORY"/OtherResults.ndx"
#define OTHER_DB_DATA			RESULTS_DIRECTORY"/OtherResults.db"

#define RESULTS_MAX				5000	// Maximum number of results stored.

// This array is used in shared memory to flag when a database entry has
// been changed by Analysis, so that ASTM will know the data is no longer valid
// from its last query.
typedef bool				ResultsDbChanged_t[ RESULTS_MAX ];

typedef struct
{
	unsigned char				rotorId;
	char						rotorName[ ROTOR_NAME_LEN ];
	char						lotNumber[ LOT_NUMBER_LEN ];
	char						patientControlId[ ID_LEN ];
	char						operatorId[ ID_LEN ];
	char						alternateId[ ID_LEN ];
	char						doctorId[ ID_LEN ];
	char						location[ ID_LEN ];
	char						phoneNumber[ ID_LEN ];
	char						admissionOwnerId[ ID_LEN ];
	Date_t						dateOfBirth;
	char						dateOfBirthString[ ID_LEN ];		// Support for data entered into old DB versions
	Date_t						lastVaccination;
	char						lastVaccinationString[ ID_LEN ];	// Support for data entered into old DB versions
	char						sampleId[ ID_LEN ];
	Gender_t					gender;
// GLD
// Should this be SampleType_t?
	int							sampleType;
// GLD
// Change made to accommodate addition of the race and dataSent fields
//	int							age;
	short						age;
	char						race;
	char						dataSent;
	AgeUnits_t					ageUnits;				// Days, weeks, months or years
	time_t						dateTime;
	char						radFileName[ MAX_RAD_FILENAME_LEN ];
	SystemErrorNumber_t			errNum;
	bool						controlExpired;
	bool						operatorExpired;
	bool						operatorUnauthorized;

	RotorInformationResults_t	rotorInformationResults;
	RotorAnalyteResult_t		rotorAnalyteResults[ MAX_ROTOR_RESULTS ];
} ResultsDbEntry_t;

typedef struct
{
	char	patientControlId[ ID_LEN ];
	char	patientControl;		// 'P' = patient, 'C' = control
	bool	errorFlag;			// true = error, false = no error
	time_t	dateTime;
	int		index;				// index into the database
} ResultsDbIndexEntry_t;

typedef struct
{
	int						dbVersion;
	int						lastEntry;
	ResultsDbIndexEntry_t	entries[ RESULTS_MAX ];
} ResultsDbIndex_t;

#endif // __RESULTSDB_H_INCLUDED
