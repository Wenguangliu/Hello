#include "SystemError.h"
#include "CommonDefines.h"
#include "Shmem.h"

/*
 * Table associating error numbers to actions and strings.  The actual error
 * number is used, instead of the enum name, because it's easier to make sure all
 * errors are here that way.
 */

const SystemErrorEntry_t systemErrorTable[] =
{
	// ErrNum										Normal Action			Research Action			Demo Action				Screen Text						Info Text
	{ 0,											SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		STR_BLANK,						STR_BLANK },
	{ SERR_ENGINE_NAK					/* 4003 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_COMM,				EISTR_INTERNAL_COMM },
	{ SERR_INDEX_CUV_TOO_LONG			/* 400f */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_INDEX_CUV_TOO_SHORT			/* 4010 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_MISSED_CUV_WINDOW			/* 4011 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_CUV_TOO_LONG					/* 4012 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_CUV_TOO_SHORT				/* 4013 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_CUV_MISSING					/* 4014 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_MSG_TIMEOUT					/* 4018 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_ANALYZER_TIMEOUT,			EISTR_TIMEOUT },
	{ SERR_ILLEGAL_VETSCAN_ROTOR		/* 401c */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_ILLEGAL_ROTOR_TYPE,		EISTR_WRONG_ROTOR },
	{ SERR_ILLEGAL_PICCOLO_ROTOR		/* 401d */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_ILLEGAL_ROTOR_TYPE,		EISTR_WRONG_ROTOR },
	{ SERR_TOP_HEATER_LOW_CURRENT		/* 4026 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_HEATER,			EISTR_TOP_HEATER_LOW },
	{ SERR_TOP_HEATER_HIGH_CURRENT		/* 4027 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_HEATER,			EISTR_TOP_HEATER_HIGH },
	{ SERR_BOTTOM_HEATER_LOW_CURRENT	/* 4028 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_HEATER,			EISTR_BOTTOM_HEATER_LOW },
	{ SERR_BOTTOM_HEATER_HIGH_CURRENT	/* 4029 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_HEATER,			EISTR_BOTTOM_HEATER_HIGH },
	{ SERR_LOST_CUV_SYNC				/* 402a */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_RESULT_CORRUPTION			/* 402b */,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_RESULT_CORRUPTION,			EISTR_RESULT_CORRUPTION },
	{ SERR_AMBIENT_TEMPERATURE			/* 402c */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_AMBIENT_TEMP,				EISTR_AMBIENT_TEMP },
	{ SERR_DAC_SETTING					/* 402e */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_NEEDS_SERVICE },
	{ SERR_FLASH_SATURATION				/* 4031 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_SATURATION },
	{ SERR_MOTOR_FAULT					/* 4035 */, SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_SPINDLE_MOTOR,				EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_NEED_COOL_DOWN_TIME			/* 4036 */,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_ANALYZER_TOO_HOT,			EISTR_TOO_HOT },
	{ SERR_INSUFFICIENT_SAMPLE			/* 4037 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_INSUFF_SAMPLE,				EISTR_INSUFFICIENT_SAMPLE },
	{ SERR_SAMPLE_TO_CUV				/* 4038 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_SAMPLE_DISTRIBUTION,		EISTR_SAMPLE_DISTRIBUTION },
	{ SERR_DILUENT_ABS					/* 4039 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_DILUENT_ABSORBANCE,		EISTR_BACKGROUND_ABSORBANCE },
	{ SERR_BEAD_MISSING					/* 403a */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_BEADS_MISSING,				EISTR_BEAD_MISSING },
	{ SERR_DISTRIBUTION					/* 403b */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_DISTRIBUTION,				EISTR_REAGENT_DISTRIBUTION },
	{ SERR_ILLEGAL_LOG_PARAM			/* 403c */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_NEEDS_SERVICE },
	{ SERR_SAMPLE_DIL_MIX				/* 403d */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_SAMPLE_MIX,				EISTR_SAMPLE_MIX },
	{ SERR_INSUFFICIENT_DILUENT			/* 403e */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_DILUENT,					EISTR_INSUFFICIENT_DILUENT },
	{ SERR_DIVIDE_BY_0					/* 403f */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_NEEDS_SERVICE },
	{ SERR_TEMPERATURE_SWING			/* 4042 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_TEMPERATURE,				EISTR_TEMP_VARIATION },
	{ SERR_SB_BEAD_MIX					/* 4043 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_SAMPLE_BLANK_BEAD,			EISTR_SAMPLE_BLANK },
	{ SERR_QC_MIX						/* 4044 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_IQC_BEAD_MIX,				EISTR_BEAD_MIX },
	{ SERR_RQC_OUT_OF_RANGE				/* 4045 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_RQC,						EISTR_RQC },
	{ SERR_SB_START_ABS					/* 4046 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_SAMPLE_BLANK_BEAD,			EISTR_STARTING_ABSORBANCE },
	{ SERR_OFFSET_SD					/* 4047 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_DARK_NOISE },
	{ SERR_WAVE_CV						/* 4048 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_LIGHT_NOISE },
	{ SERR_OFFSET_LEVEL					/* 4049 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_BACKGROUND_NOISE },
	{ SERR_FLASH_RANGE					/* 404d */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_LAMP_NOISE,				EISTR_FLASH_LAMP_NOISE },
	{ SERR_ANALYSIS_TEMPERATURE			/* 404f */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_TEMPERATURE,				EISTR_TEMPERATURE },
	{ SERR_LAMP_INTENSITY				/* 4051 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_LAMP_LEVEL,				EISTR_LAMP },
	{ SERR_EXPIRED_ROTOR				/* 4054 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_ROTOR_EXPIRED,				EISTR_EXPIRED_ROTOR },
	{ SERR_BC_ILLEGAL_DATE				/* 4056 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_ROTOR_DATE_ILLEGAL,		EISTR_BARCODE },
	{ SERR_P100_RPM				        /* 4057 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_SPINDLE_MOTOR,	            EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_ENGINE_RESET					/* 405E */, SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_ENGINE_RESET,				EISTR_ENGINE_RESET },
	{ SERR_ANALYSIS_TIMEOUT				/* 4063 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_ANALYSIS_TIMEOUT,			EISTR_ANALYSIS_TIMEOUT },
	{ SERR_DEAD_AIR_SENSOR				/* 4068 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_HEATER,			EISTR_TEMP_SENSOR },
	{ SERR_SAMPLE_QUALITY				/* 4069 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_SAMPLE_QUALITY,			EISTR_SAMPLE_QUALITY },
	{ SERR_MISSING_FILTER_CURVE_FACTORS	/* 406b */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_NONE,		ESTR_CORRUPTED_CALIBRATION,		EISTR_MISSING_FILTER_CURV },
	{ SERR_IMPROPER_DATE				/* 406c */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_ROTOR_DATE_IMPROPER,		EISTR_ANALYZER_DATE },
	{ SERR_IQC_BEAD						/* 4073 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_IQC_BEAD,					EISTR_IQC },
	{ SERR_IQC_RATIO					/* 4074 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_IQC_RATIO,					EISTR_IQC },
	{ SERR_IQC_RATIO_PRECISION			/* 4075 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_IQC_PRECISION,				EISTR_IQC },
	{ SERR_SHORT_SAMPLE					/* 4076 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_DILUTION,					EISTR_DILUTION },
	{ SERR_N5500_RPM			        /* 407f */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_SPINDLE_MOTOR,	            EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_N1000_RPM			        /* 4080 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_SPINDLE_MOTOR,	            EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_P1000_RPM				    /* 4081 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_SPINDLE_MOTOR,	            EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_NO_SAMPLE_DILUTED			/* 4085 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_DILUTION,					EISTR_SAMPLE_MIX },
	{ SERR_ADC_REFERENCE_OFFSET			/* 4086 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_BIAS },
	{ SERR_BC_END_TRANSITION			/* 410d */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE },
	{ SERR_BC_TRANSITIONS				/* 410e */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE },
	{ SERR_BC_NO_START					/* 410f */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE_FORMAT },
	{ SERR_BC_NO_STOP					/* 4110 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE_FORMAT },
	{ SERR_BC_BAD_DECODE				/* 4111 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE_FORMAT },
//	{ SERR_BC_BAD_CHECK					/* 4112 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE_FORMAT },
//	{ SERR_BC_BAD_CRC					/* 4113 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE_FORMAT },
	{ SERR_RIS_NOT_THERE				/* 4114 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_NEW_ROTOR_TYPE,			EISTR_SOFTWARE_UPDATE },
	{ SERR_ROC_NOT_THERE				/* 4115 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_UPDATED_ROTOR_TYPE,		EISTR_SOFTWARE_UPDATE },
	{ SERR_BFD_ILLEGAL_LOG_PARAM		/* 4116 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_NONE,		ESTR_CORRUPTED_CALIBRATION,		EISTR_FILTER_CURV },
	{ SERR_BFD_DIVIDE_BY_ZERO			/* 4117 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_NONE,		ESTR_CORRUPTED_CALIBRATION,		EISTR_FILTER_CURV },
	{ SERR_ENGINE_DOWNLOAD				/* 4118 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_COMM,				EISTR_INTERNAL_COMM },
	{ SERR_ENGINE_INIT_TIMEOUT			/* 4119 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INTERNAL_COMM,				EISTR_INTERNAL_COMM },
	{ SERR_PROCESS_INIT_TIMEOUT			/* 411a */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_INITIALIZATION,			EISTR_PROCESS_INIT },
	{ SERR_FILE_READ					/* 411b */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_MEMORY_READ,				EISTR_FLASH_READ },
	{ SERR_FILE_WRITE					/* 411c */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_MEMORY_SAVE,				EISTR_FLASH_SAVE },
	{ SERR_CD_UPDATE					/* 411d */,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_CD_READ,					EISTR_CDROM },
	{ SERR_EXT_PTR_NOT_FOUND			/* 411e */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_PRINTER_NOT_FOUND,			EISTR_PRINTER_NOT_FOUND },
	{ SERR_PRODUCT_CODE					/* 411f */, SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_PRODUCT_CODE,				EISTR_PRODUCT_CODE },
 	{ SERR_DUPLICATE_ADMIN_ID			/* 4120 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_DUPLICATE_ADMIN_ID,		EISTR_DUPLICATE_ADMIN_ID },
 	{ SERR_DUPLICATE_OP_ID				/* 4121 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_DUPLICATE_OP_ID,			EISTR_DUPLICATE_OP_ID },
	{ SERR_ADMIN_NOT_FOUND				/* 4122 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_ADMIN_NOT_FOUND,			EISTR_ADMIN_NOT_FOUND },
 	{ SERR_CANNOT_REMOVE_OP_ID_SCREEN	/* 4123 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_CANNOT_REMOVE_OP_ID,		EISTR_CANNOT_REMOVE_OP_ID },
 	{ SERR_UNAUTHORIZED_OPERATOR		/* 4124 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_UNAUTHORIZED_OP,			EISTR_UNAUTHORIZED_OP },
 	{ SERR_OPERATOR_RENEWAL_REQUIRED	/* 4125 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_OP_RENEWAL_REQUIRED,		EISTR_OP_RENEWAL_REQUIRED },
 	{ SERR_UNAUTHORIZED_ADMINISTRATOR	/* 4126 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_UNAUTHORIZED_OP,			EISTR_UNAUTHORIZED_ADMIN },
 	{ SERR_CONTROL_EXPIRED				/* 4127 */, SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	SERR_ACTION_WARNING,	ESTR_CONTROL_EXPIRED,			EISTR_CONTROL_EXPIRED },
	{ SERR_ROTOR_CANCELED				/* 4128 */, SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_ANALYSIS_CANCELED,			EISTR_ANALYSIS_CANCELED },
	{ SERR_INSUFF_STORAGE				/* 4129 */,	SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_INSUFF_STORAGE,			EISTR_INSUFFICIENT_STORAGE },
    { SERR_OPERATOR_ID_LIST_FULL        /* 412a */, SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    ESTR_OP_ID_LIST_FULL,           EISTR_OP_ID_LIST_FULL },
    { SERR_ADMINISTRATOR_ID_LIST_FULL   /* 412a */, SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    ESTR_ADMIN_ID_LIST_FULL,        EISTR_ADMIN_ID_LIST_FULL },

	{ SERR_NO_FILES_TO_ARCHIVE			/* 4130 */, SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_NO_FILES_TO_ARCHIVE,		EISTR_NO_FILES_TO_ARCHIVE },
	{ SERR_NO_FILES_TO_TRANSMIT			/* 4131 */, SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_NO_FILES_TO_TRANSMIT,		EISTR_NO_FILES_TO_TRANSMIT },
	{ SERR_NO_RESULTS_TO_TRANSMIT		/* 4132 */, SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_NO_RESULTS_TO_TRANSMIT,	EISTR_NO_RESULTS_TO_TRANSMIT },
    { SERR_PATIENT_IDS_DONT_MATCH       /* 4133 */, SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    ESTR_PATIENT_IDS_DONT_MATCH,    EISTR_PATIENT_IDS_DONT_MATCH },
    { SERR_ORDER_WAS_CANCELED           /* 4134 */, SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    SERR_ACTION_WARNING,    ESTR_ORDER_WAS_CANCELED,        EISTR_ORDER_WAS_CANCELED },
    { SERR_DATABASE_FILE_TOO_LARGE      /* 4135 */, SERR_ACTION_RESTART,    SERR_ACTION_RESTART,    SERR_ACTION_RESTART,    ESTR_DATABASE_FILE_TOO_LARGE,   EISTR_DATABASE_FILE_TOO_LARGE },

	{ SERR_BC_QUIET_ZONE				/* 4200 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_FORMAT,			EISTR_BARCODE },
	{ SERR_BC_SET_THRESHOLD				/* 4201 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_SENSOR,			EISTR_BARCODE },
	{ SERR_BC_SET_CURRENT				/* 4202 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_SENSOR,			EISTR_BARCODE },
	{ SERR_BC_READ_TIMEOUT				/* 4203 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_TIMEOUT,			EISTR_BARCODE },
	{ SERR_BC_CALIBRATION_TIMEOUT		/* 4204 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_NONE,		ESTR_BARCODE_TIMEOUT,			EISTR_BARCODE },
	{ SERR_CUVETTE_CURRENT				/* 4210 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_CUVETTE_THRESHOLD			/* 4211 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_SAMPLE_TIMEOUT				/* 4212 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_CUVETTE_TIMEOUT,			EISTR_TIMEOUT },
	{ SERR_SAMPLE_STATE_MACHINE			/* 4213 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_PHOTOMETER,				EISTR_NEEDS_SERVICE },
	{ SERR_WAVE_CV_UNREF				/* 4214 */,	SERR_ACTION_ABORT,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		ESTR_PHOTOMETER,				EISTR_LIGHT_NOISE },
	{ SERR_DRAWER_OPEN_JAM				/* 4220 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_DRAWER_OPEN_JAM,			EISTR_DRAWER_OPEN },
	{ SERR_DRAWER_CLOSE_JAM				/* 4221 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_DRAWER_CLOSED_JAM,			EISTR_DRAWER_CLOSE },
	{ SERR_CAM_UP_JAM					/* 4222 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_CAM_UP_JAM,				EISTR_NEEDS_SERVICE },
	{ SERR_CAM_DOWN_JAM					/* 4223 */,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	SERR_ACTION_RESTART,	ESTR_CAM_DOWN_JAM,				EISTR_NEEDS_SERVICE },
//	{ SERR_P5V							/* 4300 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_PS,				EISTR_POWER_SUPPLY },
	{ SERR_P10V							/* 4301 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_PS,				EISTR_POWER_SUPPLY },
	{ SERR_N10V							/* 4302 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_PS,				EISTR_POWER_SUPPLY },
	{ SERR_CPLD							/* 4303 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_LOGIC,			EISTR_HARDWARE },
	{ SERR_CUVETTE_THRESHOLD_DAC		/* 4304 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_CUVETTE_SENSOR,			EISTR_CUVETTE_MARK },
	{ SERR_BARCODE_THRESHOLD_DAC		/* 4305 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_BARCODE_SENSOR,			EISTR_BARCODE },
	{ SERR_BARCODE_SIGNAL_DAC			/* 4306 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_BARCODE_SENSOR,			EISTR_BARCODE },
	{ SERR_MOTOR_DRIVER_CHIP			/* 4307 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_SPINDLE_MOTOR,				EISTR_MOTOR_CONTROLLER },
	{ SERR_RIS_FILE_DOWNLOAD			/* 4308 */,	SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		SERR_ACTION_ABORT,		ESTR_INTERNAL_COMM,				EISTR_INTERNAL_COMM },
	{ SERR_MOTOR_STILL_SPINNING			/* 4309 */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_SOFTWARE,			EISTR_INTERNAL_SOFTWARE },
	{ SERR_ILLEGAL_INTERRUPT			/* 430a */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_INTERNAL_SOFTWARE,			EISTR_INTERNAL_SOFTWARE },
	{ SERR_SPINDLE_MOTOR_STUCK			/* 430b */,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	SERR_ACTION_SHUTDOWN,	ESTR_SPINDLE_MOTOR_STUCK,		EISTR_SPINDLE_MOTOR_STUCK },
	{ SERR_SOFTWARE_EXPIRED			    /* 430c */,	SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    ESTR_SOFTWARE_EXPIRED,		    EISTR_SOFTWARE_EXPIRED },
	{ SERR_ENGINE_CODE_DOWNLOAD         /* 430e */,	SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    ESTR_INTERNAL_SOFTWARE,		    EISTR_ENGINE_CODE_DOWNLOAD },
	{ SERR_ENGINE_OUT_SYNC			    /* 430d */,	SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    SERR_ACTION_NONE,	    ESTR_INTERNAL_SOFTWARE,		    EISTR_ENGINE_CODE_OUT_SYNC },
    
    { SERR_CUVETTE_TEST_ERROR           /* 4406 */, SERR_ACTION_RESTART,    SERR_ACTION_RESTART,    SERR_ACTION_RESTART,    ESTR_CUVETTE_SENSOR,            EISTR_CUVETTE_MARK },
	{ 0xffff,										SERR_ACTION_NONE,		SERR_ACTION_NONE,		SERR_ACTION_NONE,		STR_BLANK,						STR_BLANK },
};

SystemError::
SystemError()
{
	systemData = (SystemData_t *)MapSharedMem( SYSTEM_SHMEM_NAME, sizeof( SystemData_t ) );
}

const SystemErrorEntry_t *
SystemError::
GetEntry( unsigned errNum )
{
	int i;
	
	for ( i = 0; systemErrorTable[i].errNum != 0xffff; i++ )
	{
		if ( systemErrorTable[i].errNum == errNum )
		{
			break;
		}
	}

	return &systemErrorTable[ i ];
}

const SystemErrorAction_t
SystemError::
GetAction( unsigned errNum )
{
	const SystemErrorEntry_t *	errorEntry = GetEntry( errNum );
	
	if ( systemData->inDemoMode )
	{
		return errorEntry->demoAction;
	}
	else if ( systemData->inResearchMode )
	{
		return errorEntry->researchAction;
	}
	else
	{
		return errorEntry->normalAction;
	}
}

const StringId_t
SystemError::
GetScreenText( unsigned errNum )
{
	const SystemErrorEntry_t *	errorEntry = GetEntry( errNum );
	
	return errorEntry->screenText;
}

const StringId_t
SystemError::
GetInfoText( unsigned errNum )
{
	const SystemErrorEntry_t *	errorEntry = GetEntry( errNum );
	
	return errorEntry->infoText;
}