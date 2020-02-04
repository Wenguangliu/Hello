using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace CppCsWrapper
{
    /// <summary>
    /// This is used to maintain the same definition between j2Console and j2Gui
    /// This has the structures for all the Gui messages received from other processes. This should be updated whenever we make an update to GuiMessages.h
    /// in J2Console in Common Library
    /// </summary>
    public class GuiMessages
    {

        /// <summary>
        /// Message queues used to send and receive messages by the GUI process
        /// </summary>         
        public static char[] GUI_QUEUE_NAME = "/Gui".ToCharArray();
        public static char[] ANALYSIS_QUEUE_NAME = "/Analysis".ToCharArray();
        public static char[] RECALL_QUEUE_NAME = "/Recall".ToCharArray();

        /// <summary>
        /// Message IDs for messages received by the GUI process, plus messages sent by
        ///the GUI process to destinations that aren't known until run-time
        /// </summary>
        public enum GuiMsgId_t
        {
            // Top section settings.
            MSG_GUI_FIRST_TOP = ConsoleMessages.MSG_FIRST_GUI_ID,			// 0
            MSG_GUI_TOP_NONE,
            MSG_GUI_TOP_LABEL,
            MSG_GUI_LAST_TOP,

            // Middle section settings.	
            MSG_GUI_FIRST_MIDDLE = ConsoleMessages.MSG_FIRST_GUI_ID + 100,	// 100
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
            MSG_GUI_MIDDLE_LIST_COLORED,
            MSG_GUI_MIDDLE_LIST,
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

            MSG_GUI_MIDDLE_TEMPERATURE_INFO,    //new for jen2 manufacturing 
            MSG_GUI_MIDDLE_MOTOR_DATA,          //new for jen2 manufacturing
            MSG_GUI_MIDDLE_BARCODE_DATA,        //new for jen2 manufacturing
            MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT,  //new for jen2 manufacturing 
            MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT,  //new for jen2 manufacturing 
            MSG_GUI_MIDDLE_HOME_READY_RD,       //new for jen2 manufacturing 
            MSG_GUI_MIDDLE_HOME_WARMING_RD,     //new for jen2 manufacturing
            MSG_GUI_MIDDLE_CUVETTE_DATA,        //new for jen2 manufacturing

            MSG_GUI_LAST_MIDDLE,

            //* Bottom section settings.
            MSG_GUI_FIRST_BOTTOM = ConsoleMessages.MSG_FIRST_GUI_ID + 200,	// 200
            MSG_GUI_BOTTOM_NONE,
            MSG_GUI_BOTTOM_ONE_BUTTON,
            MSG_GUI_BOTTOM_TWO_BUTTONS,
            MSG_GUI_BOTTOM_THREE_BUTTONS,
            MSG_GUI_BOTTOM_DATE_TIME,
            MSG_GUI_LAST_BOTTOM,

            // Whole-window settings (warnings, errors and critical stops).	
            MSG_GUI_FIRST_WIN = ConsoleMessages.MSG_FIRST_GUI_ID + 300,		// 300
            MSG_GUI_WIN_NONE,
            MSG_GUI_WIN_WARNING1,
            MSG_GUI_WIN_WARNING2,
            MSG_GUI_WIN_ERROR,
            MSG_GUI_WIN_CRITICAL_STOP,
            MSG_GUI_WIN_CRITICAL_STOP2,
            MSG_GUI_LAST_WIN,

            // Updates to specific fields that don't change the screen layout.
            MSG_GUI_FIRST_UPDATE = ConsoleMessages.MSG_FIRST_GUI_ID + 400,	// 400
            MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT,//401
            MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS,//402
            MSG_GUI_UPDATE_ENTER_DATE_LEFT,//403
            MSG_GUI_UPDATE_ENTER_DATE_MIDDLE,//404
            MSG_GUI_UPDATE_ENTER_DATE_RIGHT,//405
            MSG_GUI_UPDATE_ENTER_ID,//406
            MSG_GUI_UPDATE_ENTER_IP,//407
            MSG_GUI_UPDATE_ENTER_VALUE_NEW,//408
            MSG_GUI_UPDATE_HOME_WARMING_PROGRESS,//409
            MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS,//410
            MSG_GUI_UPDATE_LIST,//411
            MSG_GUI_UPDATE_LIST_MULTI,//412
            MSG_GUI_UPDATE_LIST_REF_RANGES,//413
            MSG_GUI_UPDATE_LIST_RESULTS,//414
            MSG_GUI_UPDATE_LIST_SETTINGS,//415
            MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT,//416
            MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS,//417
            MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES,//418 added nwr 4/24/2013
            MSG_GUI_UPDATE_READ_MOTOR_DATA,//419 added nwr 5/6/2013   
            MSG_GUI_UPDATE_BARCODE_DATA,//420 added nwr 5/7/2013
            MSG_GUI_UPDATE_HOME_WARMING_RD_PROGRESS,//421 nwr 5/30/2013
            MSG_GUI_UPDATE_CUVETTE_DATA,//422 nwr 5/30/2013

            MSG_GUI_LAST_UPDATE,//421

            // Set the receiver process queue for GUI button presses and retrieved
            // field information.
            MSG_GUI_SET_INPUT_RECEIVER = ConsoleMessages.MSG_FIRST_GUI_ID + 500,	// 500

            // Request for the value of the Enter ID text field.
            MSG_GUI_GET_ID_TEXT,

            // Request the entered range from the Enter Range object.
            MSG_GUI_GET_RANGE,

            // Set the strings for the bottom date/time field - at start-up or when the
            // language is changed.
            MSG_GUI_SET_DATE_TIME_STRINGS,

            // Startup gui specific messages
            MSG_GUI_STARTUP_SPLASH, //new in jen 2
            MSG_GUI_STARTUP_ERRORINFO, //new in jen 2

            //set language
            MSG_GUI_SET_LANGUAGE,//new in jen 2

            //set mouse
            MSG_GUI_ENABLE_MOUSE,                   // new for jen2
            MSG_GUI_DISABLE_MOUSE,                  // new for jen2


            // Sounds
            MSG_GUI_FIRST_SOUND = ConsoleMessages.MSG_FIRST_GUI_ID + 600,		// 600
            MSG_GUI_SOUND_CRITICAL_STOP,//601
            MSG_GUI_SOUND_ERROR,//602
            MSG_GUI_SOUND_WARNING,//603
            MSG_GUI_SOUND_PRINTING_DONE,//604
            MSG_GUI_SOUND_CLICK,//605
            MSG_GUI_SOUND_SET_ALERT_VOLUME,//606
            MSG_GUI_SOUND_SET_STATUS_VOLUME,//607
            MSG_GUI_SOUND_SET_CLICK_VOLUME,//608
            MSG_GUI_LAST_SOUND,//609

            // Screen/power saver.
            MSG_GUI_FIRST_SCREENSAVE = ConsoleMessages.MSG_FIRST_GUI_ID + 700,	// 700
            MSG_GUI_SCREENSAVE_WAKEUP,//701
            MSG_GUI_SCREENSAVE_DISABLE,//702
            MSG_GUI_SCREENSAVE_ENABLE,//703
            MSG_GUI_SCREENSAVE_BUTTON_PRESS,//704
            MSG_GUI_LAST_SCREENSAVE,//705


            //The following are all messages sent by the GUI to other processes.

            // GUI button presses.
            MSG_GUI_FIRST_OUT = ConsoleMessages.MSG_FIRST_GUI_ID + 900,			// 900           
            MSG_GUI_OUT_ACTIVATE,//901
            MSG_GUI_OUT_BOTTOM_BUTTON,//902
            MSG_GUI_OUT_BUTTON,//903
            MSG_GUI_OUT_DOWN,//904
            MSG_GUI_OUT_ICON_MATRIX,//905
            MSG_GUI_OUT_ID_TEXT,//906
            MSG_GUI_OUT_INFO,//907
            MSG_GUI_OUT_LIST,//908
            MSG_GUI_OUT_MANUFACTURING,//909
            MSG_GUI_OUT_NEXT_SETTINGS,//910
            MSG_GUI_OUT_PREVIOUS_SETTINGS,//911
            MSG_GUI_OUT_RANGE,//912
            MSG_GUI_OUT_RECALL,//913
            MSG_GUI_OUT_SETTINGS,//914
            MSG_GUI_OUT_TOGGLE,//915
            MSG_GUI_OUT_UP,//916
            MSG_GUI_LAST_OUT,//917
        }


        //cannot use this in C# file. It has been moved out of this library
        //#define IS_GUI_OUT_MSG( msgID ) ( msgId >= MSG_GUI_FIRST_OUT && msgID <= MSG_GUI_LAST_OUT )

        //Data values for a MSG_GUI_OUT_ICON_MATRIX message
        public enum GuiIconMatrixData_t
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
        }
    }

    /// <summary>
    /// The following data structures are for the data fields of the screen section
    /// setting and field update messages.
    /// </summary>     
    public class GuiTopLabelData_t
    {
        public char[] label = new char[40];
    }

    public class GuiMiddleButtonsData_t
    {
        public char[,] buttonText = new char[6, 30];
        public char[] buttonHighlighted = new char[6];
    }

    public class GuiMiddleButtonsInfoData_t
    {
        public char[,] buttonText = new char[4, 30];
        public char[] buttonHighlighted = new char[4];
    }

    public class GuiMiddleEnterDateData_t
    {
        public char[] labelLeft = new char[30];
        public char[] labelMiddle = new char[30];
        public char[] labelRight = new char[30];
        public char[] textLeft = new char[30];
        public char[] textMiddle = new char[30];
        public char[] textRight = new char[30];
        public char[] buttonText = new char[30];
    }

    public class GuiMiddleEnterIdData_t
    {
        public char isPassword;
        public char[] initialValue = new char[30];
    }

    public class GuiMiddleEnterIpData_t
    {
        public char[] text = new char[30];
    }

    public class GuiMiddleEnterRangeData_t
    {
        public char[] lowerLabel = new char[20];
        public char[] upperLabel = new char[20];
        public float lowerValue;
        public float upperValue;
        public float lowerDefault;
        public float upperDefault;
        public CommonDefines.NumericFormat_t format;
        public char[,] buttonText = new char[2, 20];
    }

    public class GuiMiddleEnterTimeData_t
    {
        public char twentyFourHourFormat;
        public char[] hoursLabel = new char[30];
        public char[] minutesLabel = new char[30];
        public char[] formatButtonText = new char[30];
        public char[] zeroSecButtonText = new char[30];
    }

    public class GuiMiddleEnterValueData_t
    {
        public char[] currentLabel = new char[20];
        public char[] newLabel = new char[20];
        public char[] currentValue = new char[20];
        public char[] newValue = new char[20];
        public char[] button0Text = new char[20];
        public char[] button1Text = new char[20];
        public char[,] checkButtonText = new char[2, 20];
        public char[] buttonChecked = new char[2];
    }

    public class GuiMiddleHomeReadyData_t
    {
        public char[,] buttonText = new char[3, 30];
        public char[] buttonHighlighted = new char[3];
    }

    public class GuiMiddleHomeWarmingData_t
    {
        public char[] progBarText = new char[30];
        public int progBarValue;
    }

    public class GuiMiddleIconMatrix1Data_t
    {
    }

    public class GuiMiddleIconMatrix2Data_t
    {
    }

    public class GuiMiddleListData_t
    {
        public char[, ,] items = new char[5, 3, 50];
        public char isSelectable;
        public char hasUpDown;
    }

    public class GuiMiddleListMultiData_t
    {
        public char[,] items = new char[5, 50];
        public char[] isSelected = new char[5];
        public char hasUpDown;
    }

    public class GuiMiddleMessageData_t
    {
        public char[] message = new char[300];
    }

    public class GuiMiddleMultiInfoData_t
    {
        public char[,] buttonText = new char[4, 30];
        public char[] buttonChecked = new char[4];
    }

    public class GuiMiddleMultiSelectData_t
    {
        public char[,] buttonText = new char[6, 30];
        public char[] buttonChecked = new char[6];
    }

    public class GuiMiddleOptionInfoData_t
    {
        public char[,] buttonText = new char[5, 30];
        public int selectedButton;
    }

    public class GuiMiddleLoadOrderData_t
    {
        public char[] orderLabel = new char[30];
        public char[] rotorName = new char[300];
        public char[] buttonText = new char[30];
        public uint backgroundColor;
        public uint fontColor;
    }

    public class GuiMiddleListColoredData_t
    {
        public char[,] items = new char[5, 30];
        public uint[] fontColor = new uint[5];
        public uint[] backgroundColor = new uint[5];
    }

    public class GuiMiddleProgBarMessageData_t
    {

        //////    char		progBarText[30];
        //////int			progBarValue;
        //////unsigned	backgroundColor;
        //////unsigned	fontColor;
        //////char		message[300];


        public char[] progBarText = new char[30];
        public int progBarValue;
        public uint backgroundColor;
        public uint fontColor;
        public char[] message = new char[300];
    }

    public class GuiMiddleSelectOptionData_t
    {
        public char[,] buttonText = new char[6, 30];
        public int selectedButton;
    }

    public class GuiMiddleSelectTypeData_t
    {
        public char[,] buttonText = new char[6, 30];
        public char[] buttonChecked = new char[2];
    }

    public class GuiMiddleTemperatureData_t
    {
        public char[] rotorTemperatureValue = new char[16];
        public char[] ambientTemperatureValue = new char[16];
        public char[] ambientDacOffsetValue = new char[16];
        public char[] airTemperatureOffsetValue = new char[16];
        public char[] topHeaterTemperatureValue = new char[16];
        public char[] bottomHeaterTemperatureValue = new char[16];
        public char[] topHeaterCurrentValue = new char[16];
        public char[] bottomHeaterCurrentValue = new char[16];
        public char[] topHeaterTemperatureSetValue = new char[16];
        public char[] bottomHeaterTemperatureSetValue = new char[16];
        public char[] topDacSetValue = new char[16];
        public char[] bottomDacSetValue = new char[16];
    }

    public class GuiMiddleMotorData_t //motor test
    {
        public char[] motorSpeedReadValue = new char[16];
        public char[] motorSpeedSetValue = new char[16];
        public char[] motorDirection = new char[16];
    }

    public class GuiMiddleCuvetteTestData_t  //new for jen2 manufacturing {
    {
        public char[] resultsText = new char[64];              // test results text       
        public char[,] valueText = new char[6,8];              // test values
    }

    public class GuiMiddleTestData_t  //barcode test
    {
        public char[] resultsText = new char[64];             // test results text
        public char[] valueText = new char[8];                // set value text
        public char[] middleBotton0 = new char[12];            // calibration button
        public char[] middleBotton1 = new char[8];            // read  button
        public char[] middleBotton2 = new char[8];            // open/close button button
    }

    public class GuiMiddleViewResultsData_t
    {
       public char[] message = new char[4000];// Current message queue size max is 4096, including the msg ID, etc.
    }

    public class GuiMiddleGraphicInfoData_t
    {
        public char[] message = new char[300];
    }

    public class GuiBottomButtonsData_t
    {

        public char[,] buttonText = new char[3, 30];
        public char[] buttonHighlighted = new char[3];
    }

    public class GuiBottomDateTimeData_t
    {
        public char twentyFourHourFormat;
        public char[,] daysOfWeek = new char[7, 20];
        public char[,] months = new char[12, 20];
    }

    public class GuiWinWarningData_t
    {
        public char[] topLabel = new char[40];
        public char[] message = new char[300];
        public char[,] buttonText = new char[2, 30];
    }

    public class GuiWinErrorData_t
    {
        public char[] topLabel = new char[40];
        public char[] message = new char[300];
        public char[] buttonText = new char[30];
    }

    public class GuiWinCriticalStopData_t
    {
        public char[] topLabel = new char[40];
        public char[] message = new char[300];
    }

    /// <summary>
    /// Added new for Jen II since the start up gui is separated from the startup process
    /// Used for displaying splash screen at startup
    /// </summary>
    public class GuiSplashScreenData_t
    {
        public char[] productCode = new char[40];
        public char[] version = new char[30];
        public char[] progBarText = new char[30];
    }

    /// <summary>
    /// Language data that Gui needs to set
    /// </summary>
    public class GuiLanguageData_t
    {
        public int language;
    }

    /// <summary>
    /// Data field for the MSG_GUI_SET_INPUT_RECEIVER message
    /// </summary>
    public class GuiInputReceiverData_t
    {
        public char[] name = new char[100];
    }

    /// <summary>
    /// Data field for the MSG_GUI_SOUND_SET_xxx_VOLUME messages
    /// </summary>
    public class GuiSoundData_t
    {
        public byte volume;
    }

    /// <summary>
    /// Union of all possible data fields for messages received by the GUI.
    /// </summary> 
    public class GuiReceiveMsgData_t
    {
        public GuiTopLabelData_t topLabel;

        public GuiMiddleButtonsData_t middleButtons;
        public GuiMiddleButtonsInfoData_t middleButtonsInfo;
        public GuiMiddleEnterDateData_t middleEnterDate;
        public GuiMiddleEnterIdData_t middleEnterId;
        public GuiMiddleEnterIpData_t middleEnterIp;
        public GuiMiddleEnterTimeData_t middleEnterTime;
        public GuiMiddleEnterRangeData_t middleEnterRange;
        public GuiMiddleEnterValueData_t middleEnterValue;
        public GuiMiddleGraphicInfoData_t middleGraphicInfo;
        public GuiMiddleLoadOrderData_t middleLoadOrder; //nj added on 12/16/2015 for Order project
        public GuiMiddleListColoredData_t middleListColored; //nj added on 12/16/2015 for Order project
        public GuiMiddleHomeReadyData_t middleHomeReady;
        public GuiMiddleHomeWarmingData_t middleHomeWarming;
        public GuiMiddleListData_t middleList;
        public GuiMiddleListMultiData_t middleListMulti;
        public GuiMiddleMessageData_t middleMessage;
        public GuiMiddleMultiInfoData_t middleMultiInfo;
        public GuiMiddleMultiSelectData_t middleMultiSelect;
        public GuiMiddleOptionInfoData_t middleOptionInfo;
        public GuiMiddleProgBarMessageData_t middleProgBarMessage;
        public GuiMiddleSelectOptionData_t middleSelectOption;
        public GuiMiddleSelectTypeData_t middleSelectType;
        public GuiMiddleViewResultsData_t middleViewResults;
        public GuiMiddleTemperatureData_t middleTemperature;   //added nwr 4/24/2013
        public GuiMiddleMotorData_t middleMotor;   //added nwr 5/6/2013
        public GuiMiddleTestData_t middleBarcode;  //added nwr 5/6/2013
        public GuiMiddleCuvetteTestData_t middleCuvette; //added nwr 5/30/2013
      
        public GuiBottomButtonsData_t bottomButtons;
        public GuiBottomDateTimeData_t bottomDateTime;

        public GuiWinWarningData_t winWarning;
        public GuiWinErrorData_t winError;
        public GuiWinCriticalStopData_t winCriticalStop;
        public GuiSplashScreenData_t winSplashScreen;

        public GuiInputReceiverData_t inputReceiver;

        public GuiSoundData_t soundData;

        public GuiLanguageData_t languageData;
    }

    /// <summary>
    /// Data structure for all messages received by the GUI.
    /// </summary>
    public class GuiReceiveMsg_t
    {
        public int msgId;
        public GuiReceiveMsgData_t data;
    }

    /// <summary>
    /// Range data returned by GUI_OUT_RANGE
    /// </summary>
    public class GuiSendRangeData_t
    {
        public float lower;
        public float upper;
    }

    /// <summary>
    /// Union of data fields for messages sent by the GUI - that are defined by the
    /// GUI.  i.e. these are messages for which the GUI does not know the receiver
    /// until run-time.
    /// </summary>
    public class GuiSendMsgData_t
    {
        public int idata;
        public char[] sdata = new char[100];
        public GuiSendRangeData_t range;
    }

    /// <summary>
    /// Data structure for GUI-defined messages sent by the GUI. 
    /// </summary>    
    public class GuiSendMsg_t
    {
        public int msgId;
        public GuiSendMsgData_t data;

    }
}
