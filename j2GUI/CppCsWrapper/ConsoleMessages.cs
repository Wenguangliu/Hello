using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;

namespace CppCsWrapper
{
    /// <summary>
    /// This is used to maintain the same definition between j2Console and j2Gui
    /// </summary>
    public class ConsoleMessages
    {
        //These defines are used to set the starting value for the message ID for the
        //messages defined by the various processes.  They are set so that no two
        //messages will use the same message ID.

        public const int MSG_FIRST_GUI_ID = 0; //Message ID for the messages defined by GUI process
        public const int MSG_FIRST_STARTUP_ID = 8000; //Message ID for the messages defined by Startup process
        public const int MSG_FIRST_ERR_LOGGER_ID = 11000; //Message ID for the messages defined by Error Logger process

        public const int MSG_FIRST_AMS_ID = 6000;   //Message ID for the messages defined by AMS GUI process.
    }
}
