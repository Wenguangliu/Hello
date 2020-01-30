/*
 * FILENAME
 * 	GuiMessages.h
 * 
 * MODULE DESCRIPTION
 * 	Defines and type definitions for messages defined by the GUI process.
 * 
 * AUTHOR
 * 	Greg De Hoogh
 */
 
#ifndef __GUIMESSAGES_H_INCLUDED
#define __GUIMESSAGES_H_INCLUDED

#include "ConsoleMessages.h"
#include "Units.h"

/*
 * Posix message queue used to receive messages by the GUI process.
 */
#define GUI_QUEUE_NAME	"/Gui"

/*
 * Message IDs for messages received by the GUI process, plus messages sent by
 * the GUI process to destinations that aren't known until run-time.
 */
typedef enum
{
	// Top section settings.
	MSG_GUI_FIRST_TOP = MSG_FIRST_GUI_ID,			// 0
	MSG_GUI_TOP_NONE,
	MSG_GUI_TOP_LABEL,
	MSG_GUI_LAST_TOP,

	// Middle section settings.	
	MSG_GUI_FIRST_MIDDLE = MSG_FIRST_GUI_ID + 100,	// 100
	MSG_GUI_MIDDLE_NONE,
	MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS,
	MSG_GUI_MIDDLE_BUTTONS,
	MSG_GUI_MIDDLE_BUTTONS_INFO,
	MSG_GUI_MIDDLE_ENTER_DATE,
	MSG_GUI_MIDDLE_ENTER_ID,
	MSG_GUI_MIDDLE_ENTER_IP,
	MSG_GUI_MIDDLE_ENTER_RANGE,
	MSG_GUI_MIDDLE_ENTER_TIME,
	MSG_GUI_MIDDLE_ENTER_VALUE,
	MSG_GUI_MIDDLE_HOME_READY,
	MSG_GUI_MIDDLE_HOME_READY_MFG,
	MSG_GUI_MIDDLE_HOME_WARMING,
	MSG_GUI_MIDDLE_HOME_WARMING_MFG,
	MSG_GUI_MIDDLE_ICON_MATRIX_1,
	MSG_GUI_MIDDLE_ICON_MATRIX_2,
	MSG_GUI_MIDDLE_LIST,
	MSG_GUI_MIDDLE_LIST_COLORED,
	MSG_GUI_MIDDLE_LIST_MULTI,
	MSG_GUI_MIDDLE_LIST_REF_RANGES,
	MSG_GUI_MIDDLE_LIST_RESULTS,
	MSG_GUI_MIDDLE_LIST_SETTINGS,
	MSG_GUI_MIDDLE_LOAD_ORDER,
	MSG_GUI_MIDDLE_MESSAGE,
	MSG_GUI_MIDDLE_MESSAGE_INFO,
	MSG_GUI_MIDDLE_MULTI_INFO,
	MSG_GUI_MIDDLE_MULTI_SELECT,
	MSG_GUI_MIDDLE_OPTION_INFO,
	MSG_GUI_MIDDLE_PROG_BAR_MESSAGE,
	MSG_GUI_MIDDLE_SELECT_OPTION,
	MSG_GUI_MIDDLE_SELECT_TYPE,
	MSG_GUI_MIDDLE_VIEW_RESULTS,
	
	MSG_GUI_MIDDLE_GRAPHIC_INFO1,
	MSG_GUI_MIDDLE_GRAPHIC_INFO2,
	MSG_GUI_MIDDLE_GRAPHIC_INFO3,
	
	MSG_GUI_LAST_MIDDLE,

	//* Bottom section settings.
	MSG_GUI_FIRST_BOTTOM = MSG_FIRST_GUI_ID + 200,	// 200
	MSG_GUI_BOTTOM_NONE,
	MSG_GUI_BOTTOM_ONE_BUTTON,
	MSG_GUI_BOTTOM_TWO_BUTTONS,
	MSG_GUI_BOTTOM_THREE_BUTTONS,
	MSG_GUI_BOTTOM_DATE_TIME,
	MSG_GUI_LAST_BOTTOM,

	// Whole-window settings (warnings, errors and critical stops).	
	MSG_GUI_FIRST_WIN = MSG_FIRST_GUI_ID + 300,		// 300
	MSG_GUI_WIN_NONE,
	MSG_GUI_WIN_WARNING1,
	MSG_GUI_WIN_WARNING2,
	MSG_GUI_WIN_ERROR,
	MSG_GUI_WIN_CRITICAL_STOP,
	MSG_GUI_WIN_CRITICAL_STOP2,
	MSG_GUI_LAST_WIN,
	
	// Updates to specific fields that don't change the screen layout.
	MSG_GUI_FIRST_UPDATE = MSG_FIRST_GUI_ID + 400,	// 400
	MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT,
	MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS,
	MSG_GUI_UPDATE_ENTER_DATE_LEFT,
	MSG_GUI_UPDATE_ENTER_DATE_MIDDLE,
	MSG_GUI_UPDATE_ENTER_DATE_RIGHT,
	MSG_GUI_UPDATE_ENTER_ID,
	MSG_GUI_UPDATE_ENTER_IP,
	MSG_GUI_UPDATE_ENTER_VALUE_NEW,
	MSG_GUI_UPDATE_HOME_WARMING_PROGRESS,
	MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS,
	MSG_GUI_UPDATE_LIST,
	MSG_GUI_UPDATE_LIST_MULTI,
	MSG_GUI_UPDATE_LIST_REF_RANGES,
	MSG_GUI_UPDATE_LIST_RESULTS,
	MSG_GUI_UPDATE_LIST_SETTINGS,
	MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT,
	MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS,
	MSG_GUI_LAST_UPDATE,
	
	// Set the receiver process queue for GUI button presses and retrieved
	// field information.
	MSG_GUI_SET_INPUT_RECEIVER = MSG_FIRST_GUI_ID + 500,	// 500

	// Request for the value of the Enter ID text field.
	MSG_GUI_GET_ID_TEXT,
	
	// Request the entered range from the Enter Range object.
	MSG_GUI_GET_RANGE,
	
	// Set the strings for the bottom date/time field - at start-up or when the
	// language is changed.
	MSG_GUI_SET_DATE_TIME_STRINGS,
	
	
	// Sounds
	MSG_GUI_FIRST_SOUND = MSG_FIRST_GUI_ID + 600,		// 600
	MSG_GUI_SOUND_CRITICAL_STOP,
	MSG_GUI_SOUND_ERROR,
	MSG_GUI_SOUND_WARNING,
	MSG_GUI_SOUND_PRINTING_DONE,
	MSG_GUI_SOUND_CLICK,
	MSG_GUI_SOUND_SET_ALERT_VOLUME,
	MSG_GUI_SOUND_SET_STATUS_VOLUME,
	MSG_GUI_SOUND_SET_CLICK_VOLUME,
	MSG_GUI_LAST_SOUND,

	// Screen/power saver.
	MSG_GUI_FIRST_SCREENSAVE = MSG_FIRST_GUI_ID + 700,	// 700
	MSG_GUI_SCREENSAVE_WAKEUP,
	MSG_GUI_SCREENSAVE_DISABLE,
	MSG_GUI_SCREENSAVE_ENABLE,
	MSG_GUI_SCREENSAVE_BUTTON_PRESS,
	MSG_GUI_LAST_SCREENSAVE,

	/*
	 * The following are all messages sent by the GUI to other processes.
	 */
	 
	MSG_GUI_FIRST_OUT = MSG_FIRST_GUI_ID + 900,			// 900
	
	// GUI button presses.
	MSG_GUI_OUT_ACTIVATE,
	MSG_GUI_OUT_BOTTOM_BUTTON,
	MSG_GUI_OUT_BUTTON,
	MSG_GUI_OUT_DOWN,
	MSG_GUI_OUT_ICON_MATRIX,
	MSG_GUI_OUT_ID_TEXT,
	MSG_GUI_OUT_INFO,
	MSG_GUI_OUT_LIST,
	MSG_GUI_OUT_MANUFACTURING,
	MSG_GUI_OUT_NEXT_SETTINGS,
	MSG_GUI_OUT_PREVIOUS_SETTINGS,
	MSG_GUI_OUT_RANGE,
	MSG_GUI_OUT_RECALL,
	MSG_GUI_OUT_SETTINGS,
	MSG_GUI_OUT_TOGGLE,
	MSG_GUI_OUT_UP,
	
	MSG_GUI_LAST_OUT,
} GuiMsgId_t;

#define IS_GUI_OUT_MSG( msgID ) ( msgId >= MSG_GUI_FIRST_OUT && msgID <= MSG_GUI_LAST_OUT )

/*
 * Data values for a MSG_GUI_OUT_ICON_MATRIX message.
 */
typedef enum
{
	ICON_ADVANCED,
	ICON_ARCHIVE_REF_RANGES,
	ICON_COMM,
	ICON_DATE_TIME,
	ICON_FACTORY,
	ICON_LANGUAGE,
	ICON_PRINTER,
	ICON_REF_RANGES,
	ICON_SCREEN,
	ICON_SECURITY,
	ICON_SOUND,
	ICON_UNITS,
	ICON_UPDATE,
	ICON_WHO
} GuiIconMatrixData_t;

/*
 * The following data structures are for the data fields of the screen section
 * setting and field update messages.
 */
 
typedef struct
{
	char label[40];
} GuiTopLabelData_t;

typedef struct
{
	char buttonText[6][30];
	char buttonHighlighted[6];
} GuiMiddleButtonsData_t;

typedef struct
{
	char buttonText[4][30];
	char buttonHighlighted[4];
} GuiMiddleButtonsInfoData_t;

typedef struct
{
	char labelLeft[30];
	char labelMiddle[30];
	char labelRight[30];
	char textLeft[30];
	char textMiddle[30];
	char textRight[30];
	char buttonText[30];
} GuiMiddleEnterDateData_t;

typedef struct
{
	char	isPassword;
	char	initialValue[30];
} GuiMiddleEnterIdData_t;

typedef struct
{
	char	text[30];
} GuiMiddleEnterIpData_t;

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
} GuiMiddleEnterRangeData_t;

typedef struct
{
	char	twentyFourHourFormat;
	char	hoursLabel[30];
	char	minutesLabel[30];
	char	formatButtonText[30];
	char	zeroSecButtonText[30];
} GuiMiddleEnterTimeData_t;

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
} GuiMiddleEnterValueData_t;

typedef struct
{
	char	buttonText[3][30];
	char	buttonHighlighted[3];
} GuiMiddleHomeReadyData_t;

typedef struct
{
	char	progBarText[30];
	int		progBarValue;
} GuiMiddleHomeWarmingData_t;

typedef struct
{
} GuiMiddleIconMatrix1Data_t;

typedef struct
{
} GuiMiddleIconMatrix2Data_t;

typedef struct
{
	char	items[5][3][50];
	char	isSelectable;
	char	hasUpDown;
} GuiMiddleListData_t;

typedef struct
{
	char		items[5][30];
	unsigned	fontColor[5];
	unsigned	backgroundColor[5];
} GuiMiddleListColoredData_t;

typedef struct
{
	char	items[5][50];
	char	isSelected[5];
	char	hasUpDown;
} GuiMiddleListMultiData_t;

typedef struct
{
	char		orderLabel[30];
	char		rotorName[300];
	char		buttonText[30];
	unsigned	backgroundColor;
	unsigned	fontColor;
} GuiMiddleLoadOrderData_t;

typedef struct
{
	char	message[300];
} GuiMiddleMessageData_t;

typedef struct
{
	char buttonText[4][30];
	char buttonChecked[4];
} GuiMiddleMultiInfoData_t;

typedef struct
{
	char buttonText[6][30];
	char buttonChecked[6];
} GuiMiddleMultiSelectData_t;

typedef struct
{
	char	buttonText[4][30];
	int		selectedButton;
} GuiMiddleOptionInfoData_t;


typedef struct
{
	char		progBarText[30];
	int			progBarValue;
	unsigned	backgroundColor;
	unsigned	fontColor;
	char		message[300];
} GuiMiddleProgBarMessageData_t;

typedef struct
{
	char	buttonText[6][30];
	int		selectedButton;
} GuiMiddleSelectOptionData_t;

typedef struct
{
	char	buttonText[6][30];
	char	buttonChecked[2];
} GuiMiddleSelectTypeData_t;

typedef struct
{
	char text[4000];		// Current message queue size max is 4096, including the msg ID, etc.
} GuiMiddleViewResultsData_t;

typedef struct
{
	char	message[300];
} GuiMiddleGraphicInfoData_t;

typedef struct
{
	char buttonText[3][30];
	char buttonHighlighted[3];
} GuiBottomButtonsData_t;

typedef struct
{
	char twentyFourHourFormat;
	char daysOfWeek[7][20];
	char months[12][20];
} GuiBottomDateTimeData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[2][30];
} GuiWinWarningData_t;

typedef struct
{
	char topLabel[40];
	char message[300];
	char buttonText[30];
} GuiWinErrorData_t;

/*
 * Data field for the MSG_GUI_SET_INPUT_RECEIVER message.
 */
 typedef struct
{
	char name[100];
} GuiInputReceiverData_t;

/*
 * Data field for the MSG_GUI_SOUND_SET_xxx_VOLUME messages.
 */
typedef struct
{
	unsigned char volume;
} GuiSoundData_t;

/*
 * Union of all possible data fields for messages received by the GUI.
 */
typedef union
{
	GuiTopLabelData_t				topLabel;

	GuiMiddleButtonsData_t			middleButtons;
	GuiMiddleButtonsInfoData_t		middleButtonsInfo;
	GuiMiddleEnterDateData_t		middleEnterDate;
	GuiMiddleEnterIdData_t			middleEnterId;
	GuiMiddleEnterIpData_t			middleEnterIp;
	GuiMiddleEnterTimeData_t		middleEnterTime;
	GuiMiddleEnterRangeData_t		middleEnterRange;
	GuiMiddleEnterValueData_t		middleEnterValue;
	GuiMiddleGraphicInfoData_t		middleGraphicInfo;
	GuiMiddleHomeReadyData_t		middleHomeReady;
	GuiMiddleHomeWarmingData_t		middleHomeWarming;
	GuiMiddleListData_t				middleList;
	GuiMiddleListColoredData_t		middleListColored;
	GuiMiddleListMultiData_t		middleListMulti;
	GuiMiddleLoadOrderData_t		middleLoadOrder;
	GuiMiddleMessageData_t			middleMessage;
	GuiMiddleMultiInfoData_t		middleMultiInfo;
	GuiMiddleMultiSelectData_t		middleMultiSelect;
	GuiMiddleOptionInfoData_t		middleOptionInfo;
	GuiMiddleProgBarMessageData_t	middleProgBarMessage;
	GuiMiddleSelectOptionData_t		middleSelectOption;
	GuiMiddleSelectTypeData_t		middleSelectType;
	GuiMiddleViewResultsData_t		middleViewResults;
			
	GuiBottomButtonsData_t			bottomButtons;
	GuiBottomDateTimeData_t			bottomDateTime;
	
	GuiWinWarningData_t				winWarning;
	GuiWinErrorData_t				winError;
	
	GuiInputReceiverData_t			inputReceiver;
	
	GuiSoundData_t					soundData;
} GuiReceiveMsgData_t;

/*
 * Data structure for all messages received by the GUI.
 */
typedef struct
{
	int					msgId;
	GuiReceiveMsgData_t	data;
} GuiReceiveMsg_t;

/*
 * Range data returned by GUI_OUT_RANGE
 */
typedef struct
{
	float lower;
	float upper;
} GuiSendRangeData_t;

/*
 * Union of data fields for messages sent by the GUI - that are defined by the
 * GUI.  i.e. these are messages for which the GUI does not know the receiver
 * until run-time.
 */
typedef union
{
	int					idata;
	char				sdata[100];
	GuiSendRangeData_t	range;
} GuiSendMsgData_t;

/*
 * Data structure for GUI-defined messages sent by the GUI.
 */
typedef struct
{
	int					msgId;
	GuiSendMsgData_t	data;
} GuiSendMsg_t;

#endif /* __GUIMESSAGES_H_INCLUDED */
