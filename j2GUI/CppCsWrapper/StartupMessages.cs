using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace CppCsWrapper
{
    /// <summary>
    /// Defines and type definitions for messages defined by the Startup process.
    /// </summary>
    public class StartupMessages
    {
        public static char[] STARTUP_QUEUE_NAME = "/Startup".ToCharArray();

        /// <summary>
        /// Messages sent from other processes to start up
        /// </summary>
        public enum StartupMsgId_t
        {
            // Top section settings.
            MSG_STARTUP_GOTO_AMS = ConsoleMessages.MSG_FIRST_STARTUP_ID,
            MSG_STARTUP_GUI_READY,
            MSG_STARTUP_ERROR,
        }

        /// <summary>
        /// Startup message structure
        /// </summary>
        public class StartupMsg_t
        {
            public StartupMsgId_t msgId;
        }
    }
}
