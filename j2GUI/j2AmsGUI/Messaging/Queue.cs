using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Messaging;
using System.MessagingCE;
using CppCsWrapper;

namespace j2AMSGUI.Messaging
{
    public class Queue
    {
        public static string MessageReceiverName;//queue name used by Gui for sending Gui messages only. This is set whenever Gui receives MSG_GUI_SET_INPUT_RECEIVER message. Not used for logging error
        private const bool READONLY = false;//flag to write Gui messages to the messageReceiver process (may be Analysis or Recall)        
        //TODO private P2PMessageQueue MessageReceiverQueue;         

        /// <summary>
        /// This method will return the queue if existing and if not it will
        /// create a new one and will return the queue
        /// </summary>
        /// <param name="forReading">true if queue should be opened only for reading</param>
        /// <param name="queueName">name of the queue</param>
        /// <returns></returns>
        public P2PMessageQueue GetQueue(bool forReading, string queueName)
        {
            try
            {
                if (!string.IsNullOrEmpty(queueName))
                {
                    return (new P2PMessageQueue(forReading, queueName));
                }

                return null;
            }
            catch (System.Exception e)
            {
                throw new Exception("Failed to open process receive message queue in Queue.cs", e);
            }
        }

        /// <summary>
        /// Set the message queue name which needs to receive gui messages. One and only one process can receive a message from Gui
        /// </summary>
        /// <param name="queueName"></param>
        public void SetReceiverQueueName(char[] queueName)
        {
            char[] temp = new char[queueName.GetUpperBound(0) + 1];

            int j = 0;
            for (j = 0; j < (queueName.GetUpperBound(0) + 1); j++)
            {
                if (queueName[j] == '\0')//end of string
                {
                    break;
                }

                temp[j] = queueName[j];
            }

            //MessageReceiverName has the name of the process which will be receiving the gui messages
            MessageReceiverName = new string(temp, 0, j);
        }
    }
}
