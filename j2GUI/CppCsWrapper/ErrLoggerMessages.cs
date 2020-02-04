using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace CppCsWrapper
{
    /// <summary>
    /// This is used to maintain the same definition between j2Console and j2Gui
    /// </summary>
    public class ErrLoggerMessages
    {
        public static char[] ERR_LOGGER_QUEUE_NAME = "/ErrLogger".ToCharArray();

        //Message IDs for messages received by the ErrLogger processs
        public enum ErrLoggerMsgId_t
        {
            MSG_ERR_LOGGER_MESSAGE = ConsoleMessages.MSG_FIRST_ERR_LOGGER_ID,
        }
    }

    /// <summary>
    /// This structure has the error message Id and the data
    /// </summary>
    public class ErrLoggerMsg_t
    {
        public ErrLoggerMessages.ErrLoggerMsgId_t msgId;
        public char[] message = new char[200];
    }
}