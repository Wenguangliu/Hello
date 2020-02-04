using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using CppCsWrapper;
using System.MessagingCE;

namespace j2AMSGUI.Messaging
{
    /// <summary>
    /// This class handles the messages received by the Gui Message queue from other processes
    /// </summary>
    public class ReceiveQueue
    {
        private Queue AmsGuiQueue;
        public static event j2AMSGUI.j2Startup.UpdateEventHandler UpdatingDisplay;
        private const bool READONLY = true;//flag to read the messages in the GuiMessageQueue

        public ReceiveQueue()
        {
            AmsGuiQueue = new Queue();
        }
        /// <summary>
        /// This is the receive queue thread function which reads the Gui message queue and converts the message from byte array to GuiReceiveMsg_t
        /// structure and calls the MainScreen Update method to update the display/screen. Since P2PMessageQueue needs to send and receive messages only 
        /// as byte array we need to translate the corresponding byte arrays to correct Gui structures.
        /// </summary>
        public void ProcessReceiveQueue()
        {
            SendQueue SendMessageQueue = new SendQueue();

            try
            {
                P2PMessageQueue AmsGuiMessageQueue = null;
                Utilities util = new Utilities();
                Message msg = new Message();
                ReadWriteResult rwResult;
                AmsGuiMsg_t msgReceive;//structure which rest of the Gui code and other processes will understand for processing

                byte[] receiveBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];//buffer to read the data from the guimessage queue

                //open the Gui message queue
                try
                {
                    AmsGuiMessageQueue = AmsGuiQueue.GetQueue(READONLY, new string(CppCsWrapper.AmsMessages.AMS_GUI_QUEUE_NAME));
                }
                catch //TODO (System.Exception e)
                {
                    //throw new Exception("Failed to open Gui message queue in ReceiveQueue.cs", e);
                    SendMessageQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Failed to open Gui queue for receiving messages");
                }

                //start reading the gui message queue and process the messages
                //it waits here indefinitely until a message appears in the gui message queue to process
                while (true)
                {
                    //block here until a message is present in the queue
                    rwResult = AmsGuiMessageQueue.Receive(msg, P2PMessageQueue.TimeoutInfinite);

                    if (rwResult == ReadWriteResult.OK)
                    {
                        receiveBytes = msg.MessageBytes;
                        msgReceive = util.TranslateToAmsGuiReceiveMessage(receiveBytes);

                        //update the Gui accordingly
                        UpdatingDisplay(msgReceive);
                    }
                }
            }
            catch //TODO(System.Exception e)
            {
                //TraceWrite.TraceLog("Unable to read messages from the Gui Message Queue");
                SendMessageQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to read messages from the Gui Message Queue");
            }
        }

    }
}
