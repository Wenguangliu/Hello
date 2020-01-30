/*
 * FILENAME
 * 	MGuiMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the MGUI process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __MGUIMESSAGES_H_INCLUDED
#define __MGUIMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "Units.h"

/*
 * Posix message queue used to receive messages by the GUI process.
 */
#define MGUI_QUEUE_NAME	"/MGui"

/*
 * Message IDs for messages received by the MGUI process, plus messages sent by
 * the MGUI process to destinations that aren't known until run-time.
 */
typedef enum
{
	// Top section settings.
	MSG_MGUI_FIRST_TOP = MSG_FIRST_MGUI_ID,					// 0
	MSG_MGUI_TOP_NONE,
	MSG_MGUI_TOP_LABEL,
	MSG_MGUI_LAST_TOP,

	// Middle section settings.	
	MSG_MGUI_FIRST_MIDDLE = MSG_FIRST_MGUI_ID + 100,		// 100
	MSG_MGUI_MIDDLE_NONE,
	MSG_MGUI_MIDDLE_ANALYSIS_IN_PROCESS,
	MSG_MGUI_MIDDLE_BUTTONS,
	MSG_MGUI_MIDDLE_COUNTERS,
	MSG_MGUI_MIDDLE_ENTER_VALUE,
	MSG_MGUI_MIDDLE_FLASH_BEAK,
	MSG_MGUI_MIDDLE_MESSAGE,
	MSG_MGUI_MIDDLE_READ_BARCODE,
	MSG_MGUI_MIDDLE_READ_TEMPERATURE,
	MSG_MGUI_MIDDLE_SET_DACS,
	MSG_MGUI_MIDDLE_TEST_MOTOR,
	MSG_MGUI_MIDDLE_VIEW_RESULTS,
	
	MSG_MGUI_LAST_MIDDLE,

	//* Bottom section settings.
	MSG_MGUI_FIRST_BOTTOM = MSG_FIRST_MGUI_ID + 200,		// 200
	MSG_MGUI_BOTTOM_NONE,
	MSG_MGUI_BOTTOM_ONE_BUTTON,
	MSG_MGUI_BOTTOM_TWO_BUTTONS,
	MSG_MGUI_BOTTOM_THREE_BUTTONS,
	MSG_MGUI_LAST_BOTTOM,

	// Whole-window settings (warnings, errors and critical stops).	
	MSG_MGUI_FIRST_WIN = MSG_FIRST_MGUI_ID + 300,			// 300
	MSG_MGUI_WIN_NONE,
	MSG_MGUI_WIN_RESET_ANALYZER,
	MSG_MGUI_WIN_WARNING1,
	MSG_MGUI_WIN_ERROR,
	MSG_MGUI_WIN_CRITICAL_STOP,
	MSG_MGUI_LAST_WIN,
	
	// Updates to specific fields that don't change the screen layout.
	MSG_MGUI_FIRST_UPDATE = MSG_FIRST_MGUI_ID + 400,		// 400
	MSG_MGUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT,
	MSG_MGUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS,
	MSG_MGUI_UPDATE_FLASH_BEAK,
	MSG_MGUI_UPDATE_READ_BARCODE_RESULT,
	MSG_MGUI_UPDATE_READ_TEMPERATURE_VALUES,
	MSG_MGUI_UPDATE_TEST_MOTOR_SPEED_VALUE,
	MSG_MGUI_LAST_UPDATE,
	
	// Set the receiver process queue for MGUI button presses and retrieved
	// field information.
	MSG_MGUI_SET_INPUT_RECEIVER = MSG_FIRST_MGUI_ID + 500,	// 500

	// Request the entered range from the Set DACs object.
	MSG_MGUI_GET_DAC_SETTINGS,
	
	// Request the entered value from the Enter Value object.
	MSG_MGUI_GET_VALUE,

	// Request the barcode settings from the Read Barcode object.
	MSG_MGUI_GET_BARCODE_SETTINGS,
	
	// Request the settings from the Test Motor object
	MSG_MGUI_GET_MOTOR_SETTINGS,
	
	// Sounds
	MSG_MGUI_FIRST_SOUND = MSG_FIRST_MGUI_ID + 600,			// 600
	MSG_MGUI_SOUND_CRITICAL_STOP,
	MSG_MGUI_SOUND_ERROR,
	MSG_MGUI_SOUND_WARNING,
	MSG_MGUI_SOUND_PRINTING_DONE,
	MSG_MGUI_SOUND_CLICK,
	MSG_MGUI_SOUND_SET_ALERT_VOLUME,
	MSG_MGUI_SOUND_SET_STATUS_VOLUME,
	MSG_MGUI_SOUND_SET_CLICK_VOLUME,
	MSG_MGUI_LAST_SOUND,

	// Screen/power saver.
	MSG_MGUI_FIRST_SCREENSAVE = MSG_FIRST_MGUI_ID + 700,	// 700
	MSG_MGUI_SCREENSAVE_WAKEUP,
	MSG_MGUI_SCREENSAVE_DISABLE,
	MSG_MGUI_SCREENSAVE_ENABLE,
	MSG_MGUI_SCREENSAVE_BUTTON_PRESS,
	MSG_MGUI_LAST_SCREENSAVE,

	/*
	 * The following are all messages sent by the MGUI to other processes.
	 */
	 
	MSG_MGUI_FIRST_OUT = MSG_FIRST_MGUI_ID + 900,			// 900
	
	// MGUI button presses.
	MSG_MGUI_OUT_ACTIVATE,
	MSG_MGUI_OUT_BARCODE_SETTINGS,
	MSG_MGUI_OUT_BOTTOM_BUTTON,
	MSG_MGUI_OUT_BUTTON,
	MSG_MGUI_OUT_DAC_SETTINGS,
	MSG_MGUI_OUT_DOWN,
	MSG_MGUI_OUT_ICON_MATRIX,
	MSG_MGUI_OUT_ID_TEXT,
	MSG_MGUI_OUT_INFO,
	MSG_MGUI_OUT_LIST,
	MSG_MGUI_OUT_MANUFACTURING,
	MSG_MGUI_OUT_MOTOR_SETTINGS,
	MSG_MGUI_OUT_NEXT_SETTINGS,
	MSG_MGUI_OUT_PREVIOUS_SETTINGS,
	MSG_MGUI_OUT_RECALL,
	MSG_MGUI_OUT_SETTINGS,
	MSG_MGUI_OUT_TOGGLE,
	MSG_MGUI_OUT_UP,
	MSG_MGUI_OUT_VALUE,
	
	MSG_MGUI_LAST_OUT,
} MguiMsgId_t;

#define IS_MGUI_OUT_MSG( msgID ) ( msgId >= MSG_MGUI_FIRST_OUT && msgID <= MSG_MGUI_LAST_OUT )

/*
 * The following data structures are for the data fields of the screen section
 * setting and field update messages.
 */
 
typedef struct
{
	char label[40];
} MguiTopLabelData_t;

typedef struct
{
	char buttonText[6][30];
	char buttonHighlighted[6];
} MguiMiddleButtonsData_t;

typedef struct
{
	int	runs;
	int	aborts;
	int	prints;
	int	flashes;
} MguiMiddleCountersData_t;

typedef struct
{
	float	value;
	float	deflt;
	float	min;
	float	max;
	int		decPlaces;
	char	suffix[10];
} MguiMiddleEnterValueData_t;

typedef struct
{
	int		value340;
	int		value405;
	int		value467;
	int		value500;
	int		value515;
	int		value550;
	int		value600;
	int		value630;
	int		value850;
	int		valueWhite;
} MguiMiddleFlashBeakData_t;

typedef struct
{
	char	message[300];
} MguiMiddleMessageData_t;

// GLD - used by AIP
typedef struct
{
	char		progBarText[30];
	int			progBarValue;
	unsigned	backgroundColor;
	unsigned	fontColor;
	char		message[300];
} MguiMiddleProgBarMessageData_t;

typedef struct
{
	int		brightnessValue;
	int		thresholdValue;
	char	result[30];
} MguiMiddleReadBarcodeData_t;

typedef struct
{
	float	top, bottom;
	float	rotorTemp, rotorOffset;
	float	ambientTemp, ambientOffset;
	float	topTemp, topCurrent;
	float	bottomTemp, bottomCurrent;
} MguiMiddleReadTemperatureData_t;

typedef struct
{
	char	label[2][50];
	float	value[2];
	float	min[2];
	float	max[2];
	int		decPlaces[2];
	char	suffix[2][10];
} MguiMiddleSetDacsData_t;

typedef struct
{
	int		speedValue;
	int		speedSetting;
	bool	forward;
} MguiMiddleTestMotorData_t;

typedef struct
{
	char text[4000];		// Current message queue size max is 4096, including the msg ID, etc.
} MguiMiddleViewResultsData_t;

typedef struct
{
	char buttonText[3][30];
	char buttonHighlighted[3];
} MguiBottomButtonsData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[2][30];
} MguiWinWarningData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[30];
} MguiWinErrorData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
} MguiWinCriticalStopData_t;

/*
 * Data field for the MSG_MGUI_SET_INPUT_RECEIVER message.
 */
 typedef struct
{
	char name[100];
} MguiInputReceiverData_t;

/*
 * Data field for the MSG_MGUI_SOUND_SET_xxx_VOLUME messages.
 */
typedef struct
{
	unsigned char volume;
} MguiSoundData_t;

/*
 * Union of all possible data fields for messages received by the MGUI.
 */
typedef union
{
	MguiTopLabelData_t				topLabel;

	MguiMiddleButtonsData_t			middleButtons;
	MguiMiddleCountersData_t		middleCounters;
	MguiMiddleEnterValueData_t		middleEnterValue;
	MguiMiddleFlashBeakData_t		middleFlashBeak;
	MguiMiddleMessageData_t			middleMessage;
	MguiMiddleProgBarMessageData_t	middleProgBarMessage;	// GLD - used by AIP
	MguiMiddleReadBarcodeData_t		middleReadBarcode;
	MguiMiddleReadTemperatureData_t	middleReadTemperature;
	MguiMiddleSetDacsData_t			middleSetDacs;
	MguiMiddleTestMotorData_t		middleTestMotor;
	MguiMiddleViewResultsData_t		middleViewResults;
			
	MguiBottomButtonsData_t			bottomButtons;
	
	MguiWinWarningData_t			winWarning;
	MguiWinErrorData_t				winError;
	MguiWinCriticalStopData_t		winCriticalStop;
	
	MguiInputReceiverData_t			inputReceiver;
	
	MguiSoundData_t					soundData;
} MguiReceiveMsgData_t;

// Data structure for all messages received by the MGUI.
typedef struct
{
	int						msgId;
	MguiReceiveMsgData_t	data;
} MguiReceiveMsg_t;

// Settings data returned by MGUI_OUT_GET_BARCODE_SETTINGS
typedef struct
{
	int	brightness;
	int	threshold;
} MguiSendBarcodeData_t;

// Settings data returned by the MGUI_OUT_GET_DAC_SETTINGS
typedef struct
{
	float	value[2];
} MguiSendDacsData_t;

/*
 * Settings data returned by MGUI_OUT_GET_MOTOR_SETTINGS
 */ 
typedef struct
{
	int		speed;
	bool	forward;
} MguiSendMotorData_t;

/*
 * Union of data fields for messages sent by the MGUI - that are defined by the
 * MGUI.  i.e. these are messages for which the MGUI does not know the receiver
 * until run-time.
 */
typedef union
{
	int						idata;
	float					fdata;
	char					sdata[100];
	MguiSendBarcodeData_t	barcodeData;
	MguiSendDacsData_t		dacsData;
	MguiSendMotorData_t		motorData;
} MguiSendMsgData_t;

/*
 * Data structure for MGUI-defined messages sent by the MGUI.
 */
typedef struct
{
	int					msgId;
	MguiSendMsgData_t	data;
} MguiSendMsg_t;

#endif /* __MGUIMESSAGES_H_INCLUDED */
