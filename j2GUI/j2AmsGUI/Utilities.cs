using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using CppCsWrapper;

namespace j2AMSGUI
{
    /// <summary>
    /// This class holds all the utility methods required for the project
    /// </summary>
    public class Utilities
    {
        private Messaging.SendQueue SendMessageToQueue = new j2AMSGUI.Messaging.SendQueue();       

//        /// <summary>
//        /// The P2P message queue gets the gui messages in a byte array. Convert this to GuiReceiveMsg_t format which is
//        /// understood by other sections of the screens
//        /// </summary>
//        /// <param name="messageData">byte array of gui messages</param>
//        /// <returns>GuiReceiveMsg_t format of the received byte array depending on the message Id</returns>
        internal AmsGuiMsg_t TranslateToAmsGuiReceiveMessage(byte[] messageData)
        {
            AmsGuiMsg_t msg = new AmsGuiMsg_t();
            
            try
            {
                using (MemoryStream receiveQueueStream = new MemoryStream(messageData))
                {
                    BinaryReader reader = new BinaryReader(receiveQueueStream);

                    switch (reader.ReadInt32())
                    {
                        case ((int)AmsMessages.AmsMsgId_t.MSG_AMS_ERROR_CODE):
                            {
                                msg.amsStatus = (AmsMessages.AmsStatus_t)reader.ReadInt32();
                                msg.errorCode = reader.ReadUInt16();
                                msg.msgId = (int)AmsMessages.AmsMsgId_t.MSG_AMS_ERROR_CODE;
                                break;
                            }
                        case ((int)AmsMessages.AmsMsgId_t.MSG_AMS_STATUS):
                            {
                                msg.amsStatus = (AmsMessages.AmsStatus_t)reader.ReadInt32();
                                msg.errorCode = reader.ReadUInt16();
                                msg.msgId = (int)AmsMessages.AmsMsgId_t.MSG_AMS_STATUS;
                                break;
                            }
                    }
                }
            }
            catch //TODO (System.Exception e)
            {
                // throw new Exception("Unable to Translate the received message byte array to Gui specific structure in Utilities.cs");
                SendMessageToQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to Translate the received message byte array to Gui specific structure in Utilities.cs");
            }

            return (msg);
        }

    }
}
