#ifndef __SYSTEMERRORNUMS_H
#define __SYSTEMERRORNUMS_H

/*
 * System error numbers
 */
typedef enum
{
 	SERR_NONE				   			= 0,
	SERR_ENGINE_NAK						= 0x4003,		// Engine received three NAKs from Console
	SERR_INDEX_CUV_TOO_LONG				= 0x400f,
	SERR_INDEX_CUV_TOO_SHORT			= 0x4010,
	SERR_MISSED_CUV_WINDOW				= 0x4011,
	SERR_CUV_TOO_LONG					= 0x4012,
	SERR_CUV_TOO_SHORT					= 0x4013,
	SERR_CUV_MISSING					= 0x4014,
 	SERR_MSG_TIMEOUT					= 0x4018,		// expected msg not received in time
 	SERR_ILLEGAL_VETSCAN_ROTOR			= 0x401c,		// human rotor in Vetscan
 	SERR_ILLEGAL_PICCOLO_ROTOR			= 0x401d,		// animal rotor in Piccolo
	SERR_TOP_HEATER_LOW_CURRENT			= 0x4026,
	SERR_TOP_HEATER_HIGH_CURRENT		= 0x4027,		 
	SERR_BOTTOM_HEATER_LOW_CURRENT		= 0x4028,		 
	SERR_BOTTOM_HEATER_HIGH_CURRENT		= 0x4029,
	SERR_LOST_CUV_SYNC					= 0x402a,		
	SERR_RESULT_CORRUPTION				= 0x402b,		// database corrupt 
	SERR_AMBIENT_TEMPERATURE			= 0x402c,		// ambient temperature out of range	
	SERR_DAC_SETTING			   		= 0x402e,		// bad dac settings	
	SERR_FLASH_SATURATION			   	= 0x4031,		// wavelength saturation detected
	SERR_MOTOR_FAULT					= 0x4035,		// spindle motor stuck
	SERR_NEED_COOL_DOWN_TIME			= 0x4036,		// analyzer getting too hot	
	SERR_INSUFFICIENT_SAMPLE 	   		= 0x4037,		// not enough sample in	rotor 
	SERR_SAMPLE_TO_CUV			   		= 0x4038,		// not enough sample to	cuv	28 
	SERR_DILUENT_ABS			   		= 0x4039,		// diluent absorbance too high 
	SERR_BEAD_MISSING			   		= 0x403a,		// bead missing from rotor 
	SERR_DISTRIBUTION			   		= 0x403b,		// distribution	failed 
	SERR_ILLEGAL_LOG_PARAM		   		= 0x403c,		// math error in system	calcs 
	SERR_SAMPLE_DIL_MIX		   			= 0x403d,		// sample and diluent didn't mix 
	SERR_INSUFFICIENT_DILUENT 	   		= 0x403e,		// not enough dil in rotor 
	SERR_DIVIDE_BY_0		   			= 0x403f,		// system check divide by 0 error 
	SERR_TEMPERATURE_SWING			   	= 0x4042,		// too great of temp swing in run 
	SERR_SB_BEAD_MIX			   		= 0x4043,		// sample blank didn't mix 
	SERR_QC_MIX				   			= 0x4044,		// QC beads	didn't mix 
	SERR_RQC_OUT_OF_RANGE			   	= 0x4045,		// low QC bead out of range	
	SERR_SB_START_ABS			   		= 0x4046,		// sample blank starting abs 
	SERR_OFFSET_SD				   		= 0x4047,		// offset s.d. out of range	
	SERR_WAVE_CV			   			= 0x4048,		// wavelength c.v. out of range	
	SERR_OFFSET_LEVEL			   		= 0x4049,		// offset levels out of	range 
	SERR_FLASH_RANGE	   				= 0x404d,		// flash range error in	sys	
	SERR_ANALYSIS_TEMPERATURE	   		= 0x404f,		// temperature error during	run	
	SERR_LAMP_INTENSITY		   			= 0x4051,		// lamp no longer flashing 
	SERR_EXPIRED_ROTOR					= 0x4054,		// 
	SERR_BC_ILLEGAL_DATE				= 0x4056,		// invalid date information in rotor barcode
	SERR_ENGINE_RESET					= 0x405E,		// unexpected Engine reset
	SERR_ANALYSIS_TIMEOUT				= 0x4063,		// timeout during a phase of the analysis run
	SERR_DEAD_AIR_SENSOR				= 0x4068,		// rotor air sensor illegal value 
	SERR_SAMPLE_QUALITY				   	= 0x4069,		// Sample Blank 340, 405 or 467 Starting Absorbance Error 
	SERR_MISSING_FILTER_CURVE_FACTORS   = 0x406b,		
	SERR_IMPROPER_DATE					= 0x406c,		// rotor barcode contains date out of range
	SERR_IQC_BEAD				   		= 0x4073,
	SERR_IQC_RATIO				   		= 0x4074,
	SERR_IQC_RATIO_PRECISION	   		= 0x4075,
	SERR_SHORT_SAMPLE			   		= 0x4076,
	SERR_NO_SAMPLE_DILUTED		   		= 0x4085,
	SERR_ADC_REFERENCE_OFFSET			= 0x4086,		// bias reference over limit
	SERR_BC_END_TRANSITION				= 0x410d,		// end transition (big white space) < 3000 wide
	SERR_BC_TRANSITIONS					= 0x410e,		// transition before end > 3000 wide
	SERR_BC_NO_START					= 0x410f,
	SERR_BC_NO_STOP						= 0x4110,
 	SERR_BC_BAD_DECODE					= 0x4111,
//	SERR_BC_BAD_CHECK					= 0x4112,
//	SERR_BC_BAD_CRC						= 0x4113,
 	SERR_RIS_NOT_THERE					= 0x4114,		// RIS file not there
 	SERR_ROC_NOT_THERE					= 0x4115,		// ROC file not there
 	SERR_BFD_ILLEGAL_LOG_PARAM			= 0x4116,
 	SERR_BFD_DIVIDE_BY_ZERO				= 0x4117,
 	SERR_ENGINE_DOWNLOAD				= 0x4118,		// Engine code download error
 	SERR_ENGINE_INIT_TIMEOUT			= 0x4119,		// Engine failed to respond at start-up
 	SERR_PROCESS_INIT_TIMEOUT			= 0x411a,		// process failed to respond at start-up (GUI, Analysis, etc.)
 	SERR_FILE_READ						= 0x411b,		// error reading file
 	SERR_FILE_WRITE						= 0x411c,		// error writing file
 	SERR_CD_UPDATE						= 0x411d,		// error reading CD for software update
 	SERR_EXT_PTR_NOT_FOUND				= 0x411e,		// external printer not found
 	SERR_PRODUCT_CODE					= 0x411f,		// NVRAM and factory.xml product codes don't match
 	SERR_DUPLICATE_ADMIN_ID				= 0x4120,		// duplicate Administrator ID entered
 	SERR_DUPLICATE_OP_ID				= 0x4121,		// duplicate Operator ID entered
 	SERR_ADMIN_NOT_FOUND				= 0x4122,		// admininstrator not found going to second settings screen
 	SERR_CANNOT_REMOVE_OP_ID_SCREEN		= 0x4123,		// try to turn off op ID screen and there is at least one operator
 	SERR_UNAUTHORIZED_OPERATOR			= 0x4124,		// run a disc and no op or admin with the given ID in system
 	SERR_OPERATOR_RENEWAL_REQUIRED		= 0x4125,		// operator has expired
 	SERR_UNAUTHORIZED_ADMINISTRATOR		= 0x4126,		// admin does not have operator privilege
 	SERR_CONTROL_EXPIRED				= 0x4127,		// control expiration date passed
 	SERR_ROTOR_CANCELED					= 0x4128,		// rotor was cancelled by the user
 	SERR_INSUFF_STORAGE					= 0x4129,		// insufficient space for service archive
	SERR_OPERATOR_ID_LIST_FULL			= 0x412a,		// no room to add another operator ID
	SERR_ADMINISTRATOR_ID_LIST_FULL		= 0x412b,		// no room to add another administrator ID
	SERR_NO_FILES_TO_ARCHIVE			= 0x4130,		// no files to archive in specified date range
	SERR_NO_FILES_TO_TRANSMIT			= 0x4131,		// no files to transmit in specified date range
	SERR_NO_RESULTS_TO_TRANSMIT			= 0x4132,		// no results to transmit in specified date range
	SERR_PATIENT_IDS_DONT_MATCH			= 0x4133,		// patient ID entered does match uploaded ID selected
	SERR_ORDER_WAS_CANCELED				= 0x4134,		// ASTM order was canceled external command
	
	SERR_BC_QUIET_ZONE					= 0x4200,		// failed to find the quiet zone
	SERR_BC_SET_THRESHOLD				= 0x4201,		// barcode threshold calibration failed
	SERR_BC_SET_CURRENT					= 0x4202,		// barcode current calibration failed
	SERR_BC_READ_TIMEOUT				= 0x4203,		// barcode read timeout
	SERR_BC_CALIBRATION_TIMEOUT			= 0x4204,		// barcode calibration timeout
	SERR_CUVETTE_CURRENT				= 0x4210,		// cuvette current calibration failed
	SERR_CUVETTE_THRESHOLD				= 0x4211,		// cuvette threshold calibration failed
	SERR_SAMPLE_TIMEOUT					= 0x4212,		// sample timeout
	SERR_SAMPLE_STATE_MACHINE			= 0x4213,		// sample state machine error
	SERR_WAVE_CV_UNREF					= 0x4214,		// light noise
	SERR_DRAWER_OPEN_JAM				= 0x4220,		// open drawer jammed
	SERR_DRAWER_CLOSE_JAM				= 0x4221,		// close drawer jammed
	SERR_CAM_UP_JAM						= 0x4222,		// cam up jammed
	SERR_CAM_DOWN_JAM					= 0x4223,		// cam down jammed
//	SERR_P5V							= 0x4300,		// +5 v error
	SERR_P10V							= 0x4301,		// +10 v error
	SERR_N10V							= 0x4302,		// -10 v error 
	SERR_CPLD							= 0x4303,		// cpld version error
	SERR_CUVETTE_THRESHOLD_DAC			= 0x4304,		// cuvette threshold DAC error
	SERR_BARCODE_THRESHOLD_DAC			= 0x4305,		// barcode threshold DAC error
	SERR_BARCODE_SIGNAL_DAC				= 0x4306,		// barcode signal DAC error    
	SERR_MOTOR_DRIVER_CHIP				= 0x4307,		// spindle motor driver test error 
	SERR_RIS_FILE_DOWNLOAD				= 0x4308,		// download RIS file error
	SERR_MOTOR_STILL_SPINNING			= 0x4309,		// motor still spinning when open-drawer message received
	SERR_ILLEGAL_INTERRUPT				= 0x430a,		// illegal interrupt in Engine
	SERR_SPINDLE_MOTOR_STUCK			= 0x430b,		// spindle motor not turning or not moving properly
} SystemErrorNumber_t;

typedef SystemErrorNumber_t	SystemErrNum_t;

#endif	// __SYSTEMERRORNUMS_H
