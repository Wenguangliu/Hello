#ifndef __COMMONDEFINES_H_INCLUDED
#define __COMMONDEFINES_H_INCLUDED

/*
 * This file is for #defines, etc., used by multiple modules.
 */

#ifndef	true
#define true	1
#endif

#ifndef false
#define false	0
#endif

//#define MAX_QUEUE_LENGTH			4096
#define MAX_QUEUE_LENGTH			4096 *2

#define ETHERNET_AAEON_ADAPTER			L"PCI\\E1Q51CE61"
#define ETHERNET_ADLINK_ADAPTER			L"PCI\\RTCENIC1"
#define ETHERNET_ADLINK_BK_ADAPTER      L"PCI\\E1R51CE61"
#define IP_AAEON_REG_PATH				L"Comm\\PCI\\E1Q51CE61\\Parms\\TcpIp"
#define IP_ADLINK_REG_PATH				L"Comm\\PCI\\RTCENIC1\\Parms\\TcpIp"
#define IP_ADLINK_BK_REG_PATH           L"Comm\\PCI\\E1R51CE61\\Parms\\TcpIp"

#define LOCKOUT_BACKDOOR			"911"

#define MAX_ROTOR_NUM               99   //Range of actural rotors is from 1 to 99

#define ESC							0x1b

#define ID_LEN						15
#define LOT_NUMBER_LEN				8
#define ROTOR_NAME_LEN				32
#define SERIAL_NUMBER_LEN			12

#define NO_AGE_ENTERED				-100000

#define BACKLIGHT_OFF				0
#define MAX_BFD_CALS				32
#define	NUM_CUVETTES				30
#define	NUM_HEATER_DAC_SETTINGS		4
#define	NUM_WAVELENGTHS				10
#define NUM_BARCODE_DACS			2
#define NUM_CUVETTE_MARK_DACS		2

// Parameters for 'A' messages from the Engine to the Console. Also used for
// sample type in the 'R' messages.
#define BEAD_CHK_1					128
#define BEAD_CHK_2					129
#define DISTRIBUTION_CHK			130
#define	PRE_SAMPLE					131
#define POST_SAMPLE					132
#define PASS_DONE					133
#define ANALYSIS_DONE				134
#define MIX_DONE					136
#define CANCEL_DONE					137
#define TEMPERATURE_TEST_DONE       138
#define CUV_DELAY_TEST_DONE         139
#define CUV_DELAY_SAMPLE            140
#define NDXT_SAMPLE                 141
#define NDXT_TEST_DONE              142
#define MOTOR_TEST_DONE             143
#define NO_FLASH                    252
#define OPTICAL_DACS				253
#define BLACK_OFFSETS				254

#define	NUM_DIST_CHECK_FLASHES		10
#define	NUM_BEAD_CHECK_FLASHES		2
#define	NUM_OPTICAL_DACS_FLASHES	10
#define	NUM_BLACK_OFFSETS_FLASHES	50
#define	NUM_FRS_SAMPLE_FLASHES		2
#define	NUM_SAMPLE_FLASHES			10
#define NUM_CUV_DELAY_FLASHES       20

#define	MAX_PASSES					32
#define	MAX_SAMPLE_TYPES			128
#define	MAX_IQC_LEVEL1_CHECKS		8
#define	MAX_IQC_LEVEL2_CHECKS		8

#define	ALG_VERSION_SIZE			6
#define	MAX_ANALYTE_TYPES		    128	
#define MAX_ROTOR_RESULTS			64		// Maximum number of results for a rotor (affects results DB)
#define	MAX_CORRECTION_FACTORS		8
#define MAX_BC_FACTORS				44

#define RQC_ACCEPTABLE_MINIMUM		50

#define	NUM_BARCODE_DIGITS			110
#define NUM_BARCODE_TRANSITIONS		600
#define NUM_BARCODE_PARAMETERS      8

#define VET_ROTOR					'1'
#define HUMAN_ROTOR					'2'

#define VETSCAN_PRODUCT_CODE		'1'
#define PICCOLO_PRODUCT_CODE		'2'

#define UPLOADED_ROTOR_NAME_LEN     20

#define IN_CUSTOMER_MODE            (!systemData->inDemoMode && !systemData->inResearchMode && !systemData->inManufacturingMode && !systemData->inServiceMode && !systemData->inRandDMode)

#define RIS_FILE_DIRECTORY					"/Hard Disk/Abaxis/Ris"
#define ROC_FILE_DIRECTORY					"/Hard Disk/Abaxis/Roc"

#define FACTORY_CONFIG_FILE_BASENAME		"factory.xml"
#define FACTORY_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/"FACTORY_CONFIG_FILE_BASENAME

#define CALIBRATION_CONFIG_FILE_BASENAME	"calibration.xml"
#define CALIBRATION_CONFIG_FILE_NAME		"/Hard Disk/Abaxis/cfg/"CALIBRATION_CONFIG_FILE_BASENAME

#define	COUNTERS_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/counters.xml"
#define SETTINGS_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/settings.xml"
#define NETWORK_CONFIG_FILE_NAME            "/Hard Disk/Abaxis/cfg/networks.xml"
#define SYSTEM_CONFIG_FILE_NAME				"/Hard Disk/Abaxis/cfg/system.xml"
#define UNITS_CONFIG_FILE_NAME				"/Hard Disk/Abaxis/cfg/units.xml"
#define EXPIRYS_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/expirys.xml"
#define TIMEOUTS_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/timeouts.xml"
#define HARDWARE_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/hardware.xml"
#define ROTOR_INFO_FILE_NAME                "/Hard Disk/Abaxis/cfg/rotorInfo.xml"
#define MANUFACTURING_TEST_SET_FILE_NAME    "/Hard Disk/Abaxis/cfg/manufacturingTestSet.xml"
#define HL7_CONFIG_FILE_NAME                "/Hard Disk/Abaxis/cfg/hl7/hl7Configuration.xml"
#define HL7_EPIC_CONFIG_FILE_NAME           "/Hard Disk/Abaxis/cfg/hl7/hl7EpicConfiguration.xml"
#define HL7_PRACTICE_CONFIG_FILE_NAME       "/Hard Disk/Abaxis/cfg/hl7/hl7PracticeConfiguration.xml"
#define HL7_CONFIG_FILE_NAMES               "/Hard Disk/Abaxis/cfg/hl7/hl7ConfigurationFileNames.xml"
#define IP_ADDRESS_FILE_NAME                "/Hard Disk/SetIpAddress"

#define REF_RANGES_DIRECTORY				"/Hard Disk/Abaxis/RefRanges/"
#define REF_RANGES_CURRENT_FILE				REF_RANGES_DIRECTORY"CurrentRefRanges.xml"

#define DATABASE_CHECK_FILE_NAME            "/Hard Disk/Abaxis/cfg/datbaseCheckFile.dat"

#define VERSION_CONFIG_FILE_BASENAME		"version.dat"
#define VERSION_CONFIG_FILE_NAME			"/Hard Disk/Abaxis/cfg/"VERSION_CONFIG_FILE_BASENAME
#define ENGINE_VERSION_FILE_NAME            "/Hard Disk/Abaxis/Engine/EngineVersion.dat"
#define BSP_VERSION_FILE_NAME               "/Hard Disk/Abaxis/cfg/bspVersion.dat"
#define BSP_BT_VERSION_FILE_NAME            "/Hard Disk/Abaxis/cfg/bspVersion.bt.dat"
#define HL7_CONFIG_FILE_PATH                "/Hard Disk/Abaxis/cfg/hl7"

#define MAX_VERSION_LEN						32  // double size for union code  
#define MAX_ENG_SOFTWARE_VERSION_LEN        32  //double size for union code 16 

#define KEY_CONFIG_FILE_NAME				"key.xml"

#define BC_DIGITS_FILE						"/Hard Disk/Abaxis/bcdigits.txt"
#define BC_TRANSITIONS_FILE					"/Hard Disk/Abaxis/bctransitions.txt"

#define TEST_RESULTS_DB_IDX_NAME            "results/TestResults.idx"
#define TEST_RESULTS_DB_NAME                "results/TestResults.db"

#define IMPORT_REF_RANGES_FILE              "Hard Disk2/RefRanges.xml"
#define FUSE_SETTINGS_FILE_NAME             "Hard Disk2/jen2fusesettings.xml"

#define DEMO_FILE_NAME                      "demo.ris"
#define FAST_DEMO_FILE_NAME                 "fastdemo.ris"

#define STARTUP_PROGRAM                     "/Hard Disk/Abaxis/bin/j2Startup.exe"
#define	ANALYSIS_PROGRAM					"/Hard Disk/Abaxis/bin/j2Analysis.exe"
#define	ASTM_PROGRAM						"/Hard Disk/Abaxis/bin/j2ASTM.exe"
#define	HL7_PROGRAM						    "/Hard Disk/Abaxis/bin/j2HL7.exe"
#define	CALCRESULTS_PROGRAM					"/Hard Disk/Abaxis/bin/j2CalcResults.exe"
#define	ENGINECOMM_PROGRAM					"/Hard Disk/Abaxis/bin/j2EngineComm.exe"
#define	ERRLOGGER_PROGRAM					"/Hard Disk/Abaxis/bin/j2ErrLogger.exe"
#define	GUI_PROGRAM							"/Hard Disk/Abaxis/bin/j2GUI.exe"
#define	HEATERCONTROL_PROGRAM				"/Hard Disk/Abaxis/bin/j2HeaterControl.exe"
#define	RECALL_PROGRAM						"/Hard Disk/Abaxis/bin/j2Recall.exe"
#define	PRINTER_PROGRAM						"/Hard Disk/Abaxis/bin/j2Printer.exe"
#define	SERIALCOMM_PROGRAM					"/Hard Disk/Abaxis/bin/j2SerialComm.exe"
#define	SETTINGS_PROGRAM					"/Hard Disk/Abaxis/bin/j2Settings.exe"
#define STARTUP_PROGRAM                     "/Hard Disk/Abaxis/bin/j2Startup.exe"

#define USB_KEY_DETECT_PROGRAM              "/Hard Disk/Abaxis/bin/j2UsbDetect.exe"
#define	AMS_PROGRAM							"/Hard Disk/Abaxis/bin/j2Ams.exe"
#define AMS_ENGINGCOMM_PROGRAM              "/Hard Disk/Abaxis/bin/j2AmsEngineComm.exe"
#define AMS_GUI_PROGRAM                     "/Hard Disk/Abaxis/bin/j2AmsGui.exe"
#define ENGING_TEST_PROGRAM                 "/Hard Disk/Aabxis/bin/j2EngineTest.exe"
#define DRAWRER_TEST_PROGRAM                "/Hard Disk/Abaxis/bin/j2DrawerTest.exe"          
#define REBOOT_PROGRAM                      "/Hard Disk/Abaxis/bin/j2Reboot.exe"
#define HEATER_TEST_PROGRAM                 "/Hard Disk/Abaxis/bin/j2HeaterTest.exe"
#define TOUCH_SCREEN_PROGRAM                "hcecal.exe"           
#define UPLOADED_ROTOR_ROC_NAME             "/Hard Disk/Abaxis/vsroc"
#define UPLOADED_ROTOR_RIS_NAME             "/Hard Disk/Abaxis/vsris"
#define UPLOADED_ROTOR_BARCODE_NAME         "/Hard Disk/Abaxis/vsbarcode"
#define MANUFACTURING_PROGRAM               "/Hard Disk/Abaxis/bin/j2Manufacturing.exe"
#define RANDD_PROGRAM                       "/Hard Disk/Abaxis/bin/j2RandD.exe"
#define ERROR_PROCESS_PROGRAM               "/Hard Disk/Abaxis/bin/j2ErrorProcess.exe"

#define DEMO_ROTOR_ID						0	// Rotor ID used for Demo mode
#define UPLOADED_ROTOR_ID                   -1  // Rotor ID used for uploaded rotor data



// Bit masks for the result print flags.
#define MASK_RESULTS_SUPPRESSED		0x00000001
#define MASK_LOW_SYSTEM_LIMIT  		0x00000002
#define MASK_HIGH_SYSTEM_LIMIT		0x00000004
#define MASK_LOW_DYNAMIC_LIMIT		0x00000008
#define MASK_HIGH_DYNAMIC_LIMIT		0x00000010
#define MASK_HEMOLYZED				0x00000020
#define MASK_HEMOLYZED_LOW			0x00000040
#define MASK_HEMOLYZED_HIGH			0x00000080
#define MASK_HEMOLYZED_SUPPRESSED	0x00000100
#define MASK_LIPEMIC				0x00000200
#define MASK_LIPEMIC_LOW			0x00000400
#define MASK_LIPEMIC_HIGH			0x00000800
#define MASK_LIPEMIC_SUPPRESSED		0x00001000
#define MASK_ICTERIC				0x00002000
#define MASK_ICTERIC_LOW			0x00004000
#define MASK_ICTERIC_HIGH			0x00008000
#define MASK_ICTERIC_SUPPRESSED		0x00010000
#define MASK_DERIVED_RESULT			0x00020000
#define MASK_DILUTION_CHECK			0x00040000
#define MASK_REF_RANGE_LIMIT		0x00080000
#define MASK_POS_NEG_RESULT			0x00100000
#define MASK_NOT_COMPUTED			0x00200000		// GLD - added for eGFR
#define MASK_LOW_DYNAMIC_SPECIAL	0x00400000		// DLR - added for generic & specifically CRP_1, CRP_2, CRP
#define MASK_HIGH_DYNAMIC_SPECIAL	0x00800000		// DLR - added for generic & specifically eGFR
#define MASK_END_FLAG				0x00000000

// Specifies how numbers should be displayed.
typedef enum
{
	Xxxx,		// Three decimal places
	Xxx,		// Two decimal places
	Xx,			// One decimal place
	X,			// No decimal places
	X0,			// Always 0 in 10's place
	X00			// Always 0 in both 10's and 100's places
} NumericFormat_t;

typedef enum {
	LANGUAGE_ENGLISH,
	LANGUAGE_FRENCH,
	LANGUAGE_GERMAN,
	LANGUAGE_SPANISH,
	LANGUAGE_ITALIAN,
    LANGUAGE_PORTUGUESE,
    LANGUAGE_PORTUGUESE_B,
    LANGUAGE_CHINESE,
	NUM_LANGUAGES
} Language_t;

typedef enum
{
	SYSTEM_CHECK = 0,
	SAMPLE_BLANK = 1,
	RQC			 = 2,
	IQC			 = 3,
	FIRST_ANALYTE_RESULT_TYPE = 4,
	AG			 = 4,
	ALB_BCG		 = 5,
	ALB_BCP		 = 6,
	ALP			 = 7,
	ALT_1		 = 8,
	AMY			 = 9,
	AST_1		 = 10,
	BA_1		 = 11,
	BA_2		 = 12,
	BUN_1		 = 13,
	CA			 = 14,
	CHOL		 = 15,
	CK_1		 = 16,
	CL_1		 = 17,
	CL_2		 = 18,
	CRE_1		 = 19,
	DBIL		 = 20,
	GGT			 = 21,
	GLOB		 = 22,
	GLU			 = 23,
	HDL_1		 = 24,
	HDL_2		 = 25,
	IBIL		 = 26,
	K			 = 27,
	LDH			 = 28,
	LDL			 = 29,
	MG			 = 30,
	NA_1		 = 31,
	NA_2		 = 32,
	PHOS		 = 33,
	T4			 = 34,
	TBIL		 = 35,
	TCH			 = 36,
	TCO2		 = 37,
	TP			 = 38,
	TRIG		 = 39,
	UA			 = 40,
	VLDL		 = 41,
    CTBIL        = 42,
    BA           = 43,
    CL           = 44,
    HDL          = 45,
    NA           = 46,
    ALT_2		 = 47,
    ALT			 = 48,
	BUN_2		 = 49,
	BUN			 = 50,
	CRE_2		 = 51,
	CRE			 = 52,
	CHW			 = 53,
	ALB_BCP_A	 = 54,
	CRP_1		 = 55,
	CRP_2		 = 56,
	CRP			 = 57,
	EGFR		 = 58,
    NACL         = 59,
    LAC          = 60,
    HB           = 61,
    HCT          = 62,
    NHDLC        = 63,
    AST_2        = 64,
    AST          = 65,
    CK_2         = 66,
    CK           = 67,
    PHB          = 68,
} AnalyteResultType_t;

typedef enum
{
	PATIENT_TYPE_DOG_FEMALE		= 0,
	PATIENT_TYPE_CAT_MALE		= 1,
	PATIENT_TYPE_HORSE			= 2,
	PATIENT_TYPE_UNSPECIFIED	= 3,
	PATIENT_TYPE_CONTROL		= 4,
} PatientType_t;

typedef enum
{
	GENDER_NONE = 0,	// This must be 0 so a data struct init to all 0's will have GENDER_NONE
	GENDER_UNKNOWN = 1,	// Unknown, Male and Female must be 1, 2 and 3 so that we can use the gender-1 for an index in ref ranges
	GENDER_MALE = 2,
	GENDER_FEMALE = 3,
	GENDER_SPAYED,
	GENDER_NEUTERED,
} Gender_t;

typedef enum				// GLD - added for eGFR
{
	RACE_NONE = 0,		// This must be 0 so we can tell when race wasn't entered.
	RACE_UNKNOWN,
	RACE_WHITE,
	RACE_BLACK,
	RACE_ASIAN_PACIFIC_ISLANDER,
	RACE_NATIVE_AMERICAN,
	RACE_HISPANIC,
} Race_t;

typedef struct
{
	unsigned short	year;
	unsigned char	month;
	unsigned char	day;
} Date_t;

typedef enum
{
	AGE_UNITS_DAYS,
	AGE_UNITS_WEEKS,
	AGE_UNITS_MONTHS,
	AGE_UNITS_YEARS,
} AgeUnits_t;

// GLD
// This is for the first pass at DLD+ support.  We may want this stuff back in
// WriteRad.h for the final version.
#define RAD_FILE_DIRECTORY		"/Hard Disk/Abaxis/Rad"
#define RAD_FILE_INDEX			RAD_FILE_DIRECTORY"/Rad.ndx"
#define MAX_RAD_FILENAME_LEN	100	// This gives us a bit of a cushion.
#define MAX_RAD_FILES			500
#define IDLE_FAN_HIGH_SPEED     1       //default high fan speed
#define IDLE_FAN_LOW_SPEED      0       //low fan speed



typedef struct
{
	int		lastEntry;
	char	fileName[MAX_RAD_FILES][ MAX_RAD_FILENAME_LEN ];
} RadDbIndex_t;

typedef enum
{
    ACTION_CODE_C,
    ACTION_CODE_N,
    ACTION_CODE_Q,
    ACTION_CODE_NONE,
} ActionCode_t;

typedef struct
{
    unsigned int    orderId;
    ActionCode_t    actionCode;
    unsigned char   rotorId;
    char            orderDateTime[15];
    char            rotorNameLine1[26];
    char            rotorNameLine2[26];
    unsigned        rotorNameFontColor;
    unsigned        rotorNameBackgroundColor;
    char            patientId[ ID_LEN ];
    char            sampleId[ ID_LEN ];
    char            alternateId[ ID_LEN ];
    char            doctorId[ ID_LEN ];
    char            location[ ID_LEN ];
    char            phoneNumber[ ID_LEN ];
    char            admissionOwnerId[ ID_LEN ];
    short           age;
    AgeUnits_t      ageUnits;       
    Date_t          dateOfBirth;
    Date_t          lastVaccination;
    Gender_t        gender;
    Race_t          race;
    int             sampleType;     // Species - uses int here since SampleType_t is in Species.h,
                                    // and including that would give us a circular inclusion
    PatientType_t   patientType;    // Dog/Female, Cat/Male, Horse, Unspecified, or Control
    char            orderLabel[20]; // Label used in the pick list instead of orderId

} UploadedOrder_t;
#endif	// __COMMONDEFINES_H_INCLUDED
