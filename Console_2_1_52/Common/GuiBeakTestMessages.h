/*
 * FILENAME
 * 	GuiBeakTestMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the GUI process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __GUIBEAKTESTMESSAGES_H_INCLUDED
#define __GUIBEAKTESTMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "Units.h"

/*
 * Posix message queue used to receive messages by the GUI process.
 */
#define BGUI_QUEUE_NAME	"/BGui"

/*
 * Message IDs for messages received by the GUI process, plus messages sent by
 * the GUI process to destinations that aren't known until run-time.
 */
typedef enum
{
	// Top section settings.
	MSG_BGUI_FIRST_TOP = MSG_FIRST_BEAK_TEST_ID,			// 0
	MSG_BGUI_TOP_NONE,
	MSG_BGUI_TOP_LABEL,
	MSG_BGUI_LAST_TOP,

	// Middle section settings.	
	MSG_BGUI_FIRST_MIDDLE = MSG_FIRST_GUI_ID + 100,	// 100
	MSG_BGUI_MIDDLE_NONE,
	MSG_BGUI_MIDDLE_ANALYSIS_IN_PROCESS,
	MSG_BGUI_MIDDLE_BUTTONS,
	MSG_BGUI_MIDDLE_BUTTONS_INFO,
	MSG_BGUI_MIDDLE_ENTER_DATE,
	MSG_BGUI_MIDDLE_ENTER_ID,
	MSG_BGUI_MIDDLE_ENTER_RANGE,
	MSG_BGUI_MIDDLE_ENTER_TIME,
	MSG_BGUI_MIDDLE_ENTER_VALUE,
	MSG_BGUI_MIDDLE_HOME_READY,
	MSG_BGUI_MIDDLE_HOME_READY_MFG,
	MSG_BGUI_MIDDLE_HOME_WARMING,
	MSG_BGUI_MIDDLE_HOME_WARMING_MFG,
	MSG_BGUI_MIDDLE_ICON_MATRIX_1,
	MSG_BGUI_MIDDLE_ICON_MATRIX_2,
	MSG_BGUI_MIDDLE_LIST,
	MSG_BGUI_MIDDLE_LIST_MULTI,
	MSG_BGUI_MIDDLE_LIST_REF_RANGES,
	MSG_BGUI_MIDDLE_LIST_RESULTS,
	MSG_BGUI_MIDDLE_LIST_SETTINGS,
	MSG_BGUI_MIDDLE_MESSAGE,
	MSG_BGUI_MIDDLE_MESSAGE_INFO,
	MSG_BGUI_MIDDLE_MULTI_SELECT,
	MSG_BGUI_MIDDLE_OPTION_INFO,
	MSG_BGUI_MIDDLE_PROG_BAR_MESSAGE,
	MSG_BGUI_MIDDLE_SELECT_OPTION,
	MSG_BGUI_MIDDLE_SELECT_TYPE,
	MSG_BGUI_MIDDLE_VIEW_RESULTS,
    MSG_BGUI_MIDDLE_TEST_SET,
    MSG_BGUI_MIDDLE_TEST_BUTTONS,
    MSG_BGUI_MIDDLE_VENDOR_TEST_BUTTONS,
    MSG_BGUI_MIDDLE_SET_BUTTONS,
    MSG_BGUI_MIDDLE_VENDOR_SET_BUTTONS,
	
	MSG_BGUI_MIDDLE_GRAPHIC_INFO1,
	MSG_BGUI_MIDDLE_GRAPHIC_INFO2,
	MSG_BGUI_MIDDLE_GRAPHIC_INFO3,

    MSG_BGUI_MIDDLE_FLASH_IN_PROCESS,
    MSG_BGUI_MIDDLE_NEXT_RUN_TIME,
    MSG_BGUI_MIDDLE_VENDOR_FLASH_IN_PROCESS,
    MSG_BGUI_MIDDLE_REPORT_MESSAGE,

    MSG_BGUI_MIDDLE_TRIM_DAC_SET_SELECT,
    MSG_BGUI_MIDDLE_TRIM_DAC_AUTO_SET,
    MSG_BGUI_MIDDLE_TRIM_DAC_MANUAL_SET,
	
	MSG_BGUI_LAST_MIDDLE,

	//* Bottom section settings.
	MSG_BGUI_FIRST_BOTTOM = MSG_FIRST_GUI_ID + 200,	// 200
	MSG_BGUI_BOTTOM_NONE,
	MSG_BGUI_BOTTOM_ONE_BUTTON,
	MSG_BGUI_BOTTOM_TWO_BUTTONS,
	MSG_BGUI_BOTTOM_THREE_BUTTONS,
	MSG_BGUI_BOTTOM_DATE_TIME,
	MSG_BGUI_LAST_BOTTOM,

	// Whole-window settings (warnings, errors and critical stops).	
	MSG_BGUI_FIRST_WIN = MSG_FIRST_GUI_ID + 300,		// 300
	MSG_BGUI_WIN_NONE,
	MSG_BGUI_WIN_WARNING1,
	MSG_BGUI_WIN_WARNING2,
	MSG_BGUI_WIN_ERROR,
	MSG_BGUI_WIN_CRITICAL_STOP,
	MSG_BGUI_LAST_WIN,
	
	// Updates to specific fields that don't change the screen layout.
	MSG_BGUI_FIRST_UPDATE = MSG_FIRST_GUI_ID + 400,	// 400
	MSG_BGUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT,
	MSG_BGUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS,
	MSG_BGUI_UPDATE_ENTER_DATE_LEFT,
	MSG_BGUI_UPDATE_ENTER_DATE_MIDDLE,
	MSG_BGUI_UPDATE_ENTER_DATE_RIGHT,
	MSG_BGUI_UPDATE_ENTER_ID,
	MSG_BGUI_UPDATE_ENTER_VALUE_NEW,
	MSG_BGUI_UPDATE_HOME_WARMING_PROGRESS,
	MSG_BGUI_UPDATE_HOME_WARMING_MFG_PROGRESS,
	MSG_BGUI_UPDATE_LIST,
	MSG_BGUI_UPDATE_LIST_MULTI,
	MSG_BGUI_UPDATE_LIST_REF_RANGES,
	MSG_BGUI_UPDATE_LIST_RESULTS,
	MSG_BGUI_UPDATE_LIST_SETTINGS,
	MSG_BGUI_UPDATE_PROG_BAR_MESSAGE_TEXT,
	MSG_BGUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS,
    MSG_BGUI_UPDATE_FLASH_TEST_BUTTON,
    MSG_BGUI_UPDATE_FLASH_TEST_TEXT,
    MSG_BGUI_UPDATE_FLASH_CV_ERR_TEXT,
    MSG_BGUI_UPDATE_TRIM_DAC_VALUE,
    MSG_BGUI_UPDATE_FLASH_SD_ERR_TEXT,
	MSG_BGUI_UPDATE_ENTER_VALUE_LEFT,
	MSG_BGUI_UPDATE_ENTER_VALUE_MIDDLE,
	MSG_BGUI_UPDATE_ENTER_VALUE_RIGHT,
    MSG_BGUI_UPDATE_TEST_SET_VIEW,
    MSG_BGUI_UPDATE_NEXT_RUN_TIME_LEFT,
    MSG_BGUI_UPDATE_NEXT_RUN_TIME_RIGHT,
    MSG_BGUI_UPDATE_VENDOR_TEST_FLASH_NUMBER_TEXT,
    MSG_BGUI_UPDATE_VENDOR_TEST_TIME_LEFT_TEXT,
    MSG_BGUI_UPDATE_VENDOR_TEST_CV_ERR_TEXT,
    MSG_BGUI_UPDATE_VENDOR_TEST_ADC_ERR_TEXT,
	MSG_BGUI_UPDATE_VENDOR_FLASH_PROG_BAR_IN_PROGRESS,
	MSG_BGUI_UPDATE_VENDOR_FLASH_MESSAGE,
    MSG_BGUI_UPDATE_GET_ID_MESSAGE,
    MSG_BGUI_UPDATE_MANUAL_TRIM_DAC_VALUE,
    
    

	MSG_BGUI_LAST_UPDATE,
	
	// Set the receiver process queue for GUI button presses and retrieved
	// field information.
	MSG_BGUI_SET_INPUT_RECEIVER = MSG_FIRST_GUI_ID + 500,	// 500

	// Request for the value of the Enter ID text field.
	MSG_BGUI_GET_ID_TEXT,
	
	// Request the entered range from the Enter Range object.
	MSG_BGUI_GET_RANGE,
	
	// Set the strings for the bottom date/time field - at start-up or when the
	// language is changed.
	MSG_BGUI_SET_DATE_TIME_STRINGS,
	
	
	// Sounds
	MSG_BGUI_FIRST_SOUND = MSG_FIRST_GUI_ID + 600,		// 600
	MSG_BGUI_SOUND_CRITICAL_STOP,
	MSG_BGUI_SOUND_ERROR,
	MSG_BGUI_SOUND_WARNING,
	MSG_BGUI_SOUND_PRINTING_DONE,
	MSG_BGUI_SOUND_CLICK,
	MSG_BGUI_SOUND_SET_ALERT_VOLUME,
	MSG_BGUI_SOUND_SET_STATUS_VOLUME,
	MSG_BGUI_SOUND_SET_CLICK_VOLUME,
	MSG_BGUI_LAST_SOUND,

	// Screen/power saver.
	MSG_BGUI_FIRST_SCREENSAVE = MSG_FIRST_GUI_ID + 700,	// 700
	MSG_BGUI_SCREENSAVE_WAKEUP,
	MSG_BGUI_SCREENSAVE_DISABLE,
	MSG_BGUI_SCREENSAVE_ENABLE,
	MSG_BGUI_SCREENSAVE_BUTTON_PRESS,
	MSG_BGUI_LAST_SCREENSAVE,

	/*
	 * The following are all messages sent by the GUI to other processes.
	 */
	 
	MSG_BGUI_FIRST_OUT = MSG_FIRST_GUI_ID + 900,			// 900
	
	// GUI button presses.
	MSG_BGUI_OUT_ACTIVATE,
	MSG_BGUI_OUT_BOTTOM_BUTTON,
	MSG_BGUI_OUT_BUTTON,
	MSG_BGUI_OUT_DOWN,
	MSG_BGUI_OUT_ICON_MATRIX,
	MSG_BGUI_OUT_ID_TEXT,
	MSG_BGUI_OUT_INFO,
	MSG_BGUI_OUT_LIST,
	MSG_BGUI_OUT_MANUFACTURING,
	MSG_BGUI_OUT_NEXT_SETTINGS,
	MSG_BGUI_OUT_PREVIOUS_SETTINGS,
	MSG_BGUI_OUT_RANGE,
	MSG_BGUI_OUT_RECALL,
	MSG_BGUI_OUT_SETTINGS,
	MSG_BGUI_OUT_TOGGLE,
	MSG_BGUI_OUT_UP,
    MSG_BGUI_DIGITAL_BUTTON,
	
	MSG_BGUI_LAST_OUT,
} BGuiMsgId_t;

#define IS_GUI_OUT_MSG( msgID ) ( msgId >= MSG_GUI_FIRST_OUT && msgID <= MSG_GUI_LAST_OUT )

/*
 * Data values for a MSG_GUI_OUT_ICON_MATRIX message.
 */
typedef enum
{
	BICON_ADVANCED,
	BICON_ARCHIVE_REF_RANGES,
	BICON_COMM,
	BICON_DATE_TIME,
	BICON_FACTORY,
	BICON_LANGUAGE,
	BICON_PRINTER,
	BICON_REF_RANGES,
	BICON_SCREEN,
	BICON_SECURITY,
	BICON_SOUND,
	BICON_UNITS,
	BICON_UPDATE,
	BICON_WHO
} BGuiIconMatrixData_t;

/*
 * The following data structures are for the data fields of the screen section
 * setting and field update messages.
 */
 
typedef struct
{
	char label[40];
} BGuiTopLabelData_t;

typedef struct
{
	char buttonText[6][30];
	char buttonHighlighted[6];
} BGuiMiddleButtonsData_t;

typedef struct
{
	char buttonText[4][30];
	char buttonHighlighted[4];
} BGuiMiddleButtonsInfoData_t;

typedef struct
{
	char labelLeft[30];
	char labelMiddle[30];
	char labelRight[30];
	char textLeft[30];
	char textMiddle[30];
	char textRight[30];
	char buttonText[30];
} BGuiMiddleEnterDateData_t;

typedef struct
{
	char	isPassword;
	char	initialValue[30];
} BGuiMiddleEnterIdData_t;

typedef struct
{
	char			lowerLabel[20];
	char			upperLabel[20];
	float			lowerValue;
	float			upperValue;
	float			lowerDefault;
	float			upperDefault;
	NumericFormat_t	format;
	char			buttonText[2][20];
} BGuiMiddleEnterRangeData_t;

typedef struct
{
	char	twentyFourHourFormat;
	char	hoursLabel[30];
	char	minutesLabel[30];
	char	formatButtonText[30];
	char	zeroSecButtonText[30];
} BGuiMiddleEnterTimeData_t;

typedef struct
{
	char	currentLabel[20];
	char	newLabel[20];
	char	currentValue[20];
	char	newValue[20];
	char	button0Text[20];
	char	button1Text[20];
	char	checkButtonText[2][20];
	char	buttonChecked[2];
} BGuiMiddleEnterValueData_t;

typedef struct
{
	char	buttonText[30];
	char	buttonHighlighted;
} BGuiMiddleHomeReadyData_t;

typedef struct
{
	char	progBarText[30];
	int		progBarValue;
} BGuiMiddleHomeWarmingData_t;

typedef struct
{
} BGuiMiddleIconMatrix1Data_t;

typedef struct
{
} BGuiMiddleIconMatrix2Data_t;

typedef struct
{
	char	items[5][3][50];
	char	isSelectable;
} BGuiMiddleListData_t;

typedef struct
{
	char	items[5][50];
	char	isSelected[5];
} BGuiMiddleListMultiData_t;

typedef struct
{
	char	        message[300];
    unsigned long   bkColor;  
} BGuiMiddleMessageData_t;

typedef struct
{
	char buttonText[6][30];
	char buttonChecked[6];
} BGuiMiddleMultiSelectData_t;

typedef struct
{
	char	buttonText[4][30];
	int		selectedButton;
} BGuiMiddleOptionInfoData_t;


typedef struct
{
	char		progBarText[30];
	int			progBarValue;
	unsigned	backgroundColor;
	unsigned	fontColor;
	char		message[300];
} BGuiMiddleProgBarMessageData_t;

typedef struct
{
	char	buttonText[6][30];
	int		selectedButton;
} BGuiMiddleSelectOptionData_t;

typedef struct
{
	char	buttonText[6][30];
	char	buttonChecked[2];
} BGuiMiddleSelectTypeData_t;

typedef struct
{
	char text[4000];		// Current message queue size max is 4096, including the msg ID, etc.
} BGuiMiddleViewResultsData_t;

typedef struct
{
    char    timeText [30];
    char    LabelText [30];
    char    cvErrText [30];
    char    sdErrText [30];
	char    text[3200];		// Current message queue size max is 4096, including the msg ID, etc.
} BGuiMiddleViewFlashResultsData_t;

typedef struct
{
	char	message[300];
} BGuiMiddleGraphicInfoData_t;

typedef struct
{
	char buttonText[3][30];
	char buttonHighlighted[3];
} BGuiBottomButtonsData_t;

typedef struct
{
	char twentyFourHourFormat;
	char daysOfWeek[7][20];
	char months[12][20];
} BGuiBottomDateTimeData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[2][30];
} BGuiWinWarningData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[30];
} BGuiWinErrorData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
} BGuiWinCriticalStopData_t;

/*
 * Data field for the MSG_GUI_SET_INPUT_RECEIVER message.
 */
 typedef struct
{
	char name[100];
} BGuiInputReceiverData_t;

/*
 * Data field for the MSG_GUI_SOUND_SET_xxx_VOLUME messages.
 */
typedef struct
{
	unsigned char volume;
} BGuiSoundData_t;

/*
 * define for Vendor Flash Test 
 */
typedef struct
{
	char labelUpLeft[30];
	char labelUpRight[30];
	char labelDownRight[30];
	char labelDownLeft[30];
	char textUpLeft[30];
	char textUpRight[30];
	char textDownLeft[30];
	char textDownRight[30];
	char messageLabel[30];
    int  progBarValue;
} BGuiMiddleEnterMutiData_t;

typedef struct
{
    unsigned char   update;
    unsigned char   key;
    unsigned char   value;
    unsigned char   trimDac [10];
} BGuiMiddleSetTrimDacData_t;

/*
 * Union of all possible data fields for messages received by the GUI.
 */
typedef union
{
	BGuiTopLabelData_t				topLabel;

	BGuiMiddleButtonsData_t			middleButtons;
	BGuiMiddleButtonsInfoData_t		middleButtonsInfo;
	BGuiMiddleEnterDateData_t		middleEnterDate;
	BGuiMiddleEnterIdData_t			middleEnterId;
	BGuiMiddleEnterTimeData_t		middleEnterTime;
	BGuiMiddleEnterRangeData_t		middleEnterRange;
	BGuiMiddleEnterValueData_t		middleEnterValue;
	BGuiMiddleGraphicInfoData_t		middleGraphicInfo;
	BGuiMiddleHomeReadyData_t		middleHomeReady;
	BGuiMiddleHomeWarmingData_t		middleHomeWarming;
	BGuiMiddleListData_t			middleList;
	BGuiMiddleListMultiData_t		middleListMulti;
	BGuiMiddleMessageData_t			middleMessage;
	BGuiMiddleMultiSelectData_t		middleMultiSelect;
	BGuiMiddleOptionInfoData_t		middleOptionInfo;
	BGuiMiddleProgBarMessageData_t	middleProgBarMessage;
	BGuiMiddleSelectOptionData_t	middleSelectOption;
	BGuiMiddleSelectTypeData_t		middleSelectType;
	BGuiMiddleViewResultsData_t		middleViewResults;
    BGuiMiddleViewFlashResultsData_t middleViewFlashResults;
    BGuiMiddleEnterMutiData_t       middleEnterMutiData;
    BGuiMiddleSetTrimDacData_t      middleEnterTrimDacData;
    
    
			
	BGuiBottomButtonsData_t			bottomButtons;
	BGuiBottomDateTimeData_t	    bottomDateTime;
	
	BGuiWinWarningData_t		    winWarning;
	BGuiWinErrorData_t				winError;
	BGuiWinCriticalStopData_t		winCriticalStop;
	
	BGuiInputReceiverData_t			inputReceiver;
	
	BGuiSoundData_t					soundData;
} BGuiReceiveMsgData_t;

/*
 * Data structure for all messages received by the GUI.
 */
typedef struct
{
	int					msgId;
	BGuiReceiveMsgData_t	data;
} BGuiReceiveMsg_t;

/*
 * Range data returned by GUI_OUT_RANGE
 */
typedef struct
{
	float lower;
	float upper;
} BGuiSendRangeData_t;

/*
 * Union of data fields for messages sent by the GUI - that are defined by the
 * GUI.  i.e. these are messages for which the GUI does not know the receiver
 * until run-time.
 */
typedef union
{
	int					idata;
	char				sdata[100];
	BGuiSendRangeData_t	range;
} BGuiSendMsgData_t;

/*
 * Data structure for GUI-defined messages sent by the GUI.
 */
typedef struct
{
	int					msgId;
	BGuiSendMsgData_t	data;
} BGuiSendMsg_t;

#endif /* __GUIBEAKTESTMESSAGES_H_INCLUDED */
