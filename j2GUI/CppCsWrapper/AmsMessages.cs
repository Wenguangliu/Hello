using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;


namespace CppCsWrapper
{
    public class AmsMessages
    {
        public static char[] AMS_QUEUE_NAME = "/AmsCom".ToCharArray();
        public static char[] AMS_GUI_QUEUE_NAME = "/AmsGui".ToCharArray();

        public static int RX_FRAME_SIZE = 4000;                         // RS232 receive buffer size.
        public static int ENGINE_COMM_MAX_MSG = (RX_FRAME_SIZE - 7);	// Overhead bytes.

       public static string[]  amsProcessStatus = new string[]{
            "Please close drawer",
            "Rotor chamber is empty",
            "Waiting for Ams Command",
            "Initial Serial number",
            "Downloading optical parameters",
            "Optical calculation done",
            "Downloading global delay ris file",
            "Analyzing global delay", 
            "Uploading global delay results",
            "Analyzing barcode",
            "Uploading barcode results",
            "Downloading NDXT ris file",
            "Analyzing NDXT",
            "Uploading NDXT results",
            "Downloading temperature ris file",
            "Analyzing temperature",
            "Uploading temperature results",
            "Downloading motor ris file",
            "Analyzing motor",
            "Uploading motor results",
            "Uploading instrument setting", 
            "Getting calibration parameters", 
            "Setting calibration parameters", 
            "Canceling test, please wait ...",
            "Uploading current log file ...",
            "Uploading old log file ...",
            "Barcode alignment test ...",
            "Barcode gain test ...",
            "Cuvette detection test ...",
            "Uploading Rad file ...",
            "Ethernet Port Testing ...",
            string.Empty
        };

        public enum AmsMsgId_t
        {
            MSG_AMS_RX_READY = ConsoleMessages.MSG_FIRST_AMS_ID,
            MSG_AMS_TX_READY,
            MSG_CONSOLE_RX_READY,
            MSG_AMS_EXIT,
            MSG_AMS_STATUS,
            MSG_AMS_ERROR_CODE,
        }

        public enum AmsStatus_t
        {
            AMS_IDLE,
            NO_ROTOR,
            WAITING_FOR_COMMAND,
            INIT_SERIAL_NUM,
            DOWNLOAD_OPTICAL_PARAMETER,
            OPTICAL_CAL_DONE,
            DOWNLOAD_GLOBAL_DELAY_RIS_FILE,
            ANALYSIS_GLOBAL_DELAY,
            UPLOAD_GLOBAL_DELAY_RESULTS,
            ANALYSIS_BARCODE_TEST,
            UPLOAD_BARCODE_RESULTS,
            DOWNLOAD_NDXT_RIS_FILE,
            ANALYSIS_NDXT,
            UPLOAD_NDXT_RESULTS,
            DOWNLOAD_TEMP_RIS_FILE,
            ANALYSIS_TEMP,
            UPLOAD_TEMP_RESULTS,
            DOWNLOAD_MOTOR_RIS_FILE,
            ANALYSIS_MOTOR_TEST,
            UPLOAD_MOTOR_RESULTS,
            UPLOAD_INSTRUMENT_SETTING,
            GET_CAL_PARAMETERS,
            CHG_CAL_PARAMETERS,
            ANALYSIS_CONCEL,
            UPLOAD_CURRENT_LOG_FILE,
            UPLOAD_OLD_LOG_FILE,
            BARCODE_ALIGN_TEST,
            BARCODE_GAIN_TEST,
            UPLOADING_RAD_FILE,
            ETHERNET_PORT_TEST,
            END_OF_AMS_STATUS,
        }

        public struct EngineCommInitAnalysisData_t
        {
           public byte filler;		// Adeneo - WinCE C compiler requires that a struct has at least one member. changed to byte nwr 7/5/2011
        }
    }

    ///* 
    // * message struct within AMS precess
    // */
    public class AmsGuiMsg_t
    {
        public int msgId;
        public AmsMessages.AmsStatus_t amsStatus;
        public ushort errorCode;
    }

    /*
        * Data field for the MSG_ENGINE_COMM_SEND_MESSAGE and
        * MSG_ENGINE_COMM_MESSAGE_RECEIVED messages.
        */
    public class EngineCommMessageData_t
    {
        public char[] message = new char[AmsMessages.ENGINE_COMM_MAX_MSG];
        public int length;
    }

    /*
     * Data field for the MSG_ENGINE_COMM_SET_INPUT_RECEIVER message.
     */
    public class EngineCommMessageReceiverData_t
    {
        public char[] name = new char[100];
    }

    /*
         * Data structure for all messages received by EngineComm.
         */
    public class EngineCommReceiveMsg_t
    {
        public int msgId;
        public EngineCommReceiveMsgData_t data = new EngineCommReceiveMsgData_t();
    }

    /*
     * Union of all possible data fields for messages received by EngineComm.
    */
    public class EngineCommReceiveMsgData_t
    {
        public AmsMessages.EngineCommInitAnalysisData_t initAnalysis = new AmsMessages.EngineCommInitAnalysisData_t();
        public EngineCommMessageData_t engineMsg = new EngineCommMessageData_t();
        public EngineCommMessageReceiverData_t messageReceiver = new EngineCommMessageReceiverData_t();
    }

}
