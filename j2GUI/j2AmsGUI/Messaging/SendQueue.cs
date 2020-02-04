﻿using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Messaging;
using System.IO;
using System.Collections;
using System.MessagingCE;
using CppCsWrapper;
using Resco.Controls.OutlookControls;
using Resco.Controls.CommonControls;
using Resco.Controls.AdvancedList;
using System.Runtime.InteropServices;

namespace j2AMSGUI.Messaging
{
    /// <summary>
    /// This class deals with sending the messages from the GUI to the receiving process. All the messages sent from the GUI go through
    /// this class
    /// </summary>
    public class SendQueue
    {
        private const bool READONLY = false;//flag to write Gui messages to the messageReceiver process (may be Analysis or Recall)
        private byte[] MessageBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];//buffer size for sending messages
        private MemoryStream MessageStream = null;
        private BinaryWriter MessageWriter = null;
        private System.MessagingCE.Message MessageData = null;
        private Queue ReceiverQueue;//the queue which needs to receive the Gui message

        public SendQueue()
        {
            ReceiverQueue = new Queue();
        }

        /// <summary>
        /// Send the message(int) to the message queue if the MessageReceiverQueue is not null. The MessageReceiverQueue has the name of the receving process
        ///queue
        /// </summary>
        /// <param name="queueName"></param>
        /// <param name="msgId"></param>
        /// <param name="data"></param>
        public void SendMessage(string queueName, int msgId, int data)
        {
            try
            {
                P2PMessageQueue queue;
                //queuename
                queue = ReceiverQueue.GetQueue(false, queueName);

                if (queue != null)
                {
                    using (MessageStream = new MemoryStream(MessageBytes, 0, GuiDefines.RECEIVEBUFFERSIZE, true, true))
                    using (MessageWriter = new BinaryWriter(MessageStream))
                    {
                        //since Message object can take in only byte array, we need to convert the data to byte array
                        MessageStream.Seek(0, SeekOrigin.Begin);
                        MessageWriter.Write(msgId);
                        MessageWriter.Write(data);

                        //copy only those bytes which are relevant to this message Id
                        byte[] tempBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];
                        Buffer.BlockCopy(MessageStream.GetBuffer(), 0, tempBytes, 0, (int)MessageStream.Position);

                        MessageData = new System.MessagingCE.Message(tempBytes);
                        queue.Send(MessageData);

                        //lush the buffer and close the writer and stream to get ready for the next message
                        MessageWriter.Flush();
                        MessageStream.Close();
                        MessageWriter.Close();
                    }
                }
            }
            catch //TODO(System.Exception ex)
            {
                SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to send int messages to the queue " + queueName);
            }
        }

        /// <summary>
        /// Send the message(int) to the message queue if the MessageReceiverQueue is not null. The MessageReceiverQueue has the name of the receving process
        ///queue
        /// </summary>
        /// <param name="queueName"></param>
        /// <param name="msgId"></param>
        /// <param name="data"></param>
        public void SendMessage(string queueName, int msgId)
        {
            try
            {
                P2PMessageQueue queue;
                //queuename
                queue = ReceiverQueue.GetQueue(false, queueName);

                if (queue != null)
                {
                    using (MessageStream = new MemoryStream(MessageBytes, 0, GuiDefines.RECEIVEBUFFERSIZE, true, true))
                    using (MessageWriter = new BinaryWriter(MessageStream))
                    {
                        //since Message object can take in only byte array, we need to convert the data to byte array
                        MessageStream.Seek(0, SeekOrigin.Begin);
                        MessageWriter.Write(msgId);

                        //copy only those bytes which are relevant to this message Id
                        byte[] tempBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];
                        Buffer.BlockCopy(MessageStream.GetBuffer(), 0, tempBytes, 0, (int)MessageStream.Position);

                        MessageData = new System.MessagingCE.Message(tempBytes);
                        queue.Send(MessageData);

                        //lush the buffer and close the writer and stream to get ready for the next message
                        MessageWriter.Flush();
                        MessageStream.Close();
                        MessageWriter.Close();
                    }
                }
            }
            catch //TODO(System.Exception ex)
            {
                SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to send Id Message to the receiving process");
            }
        }

        /// <summary>
        /// Send the message(string) to the message queue if the MessageReceiverQueue is not null. The MessageReceiverQueue has the name of the receving process
        /// queue
        /// </summary>
        /// <param name="queueName"></param>
        /// <param name="msgId"></param>
        /// <param name="text"></param>
        public void SendMessage(string queueName, int msgId, string text)
        {
            try
            {
                P2PMessageQueue queue;

                //If it is a error log message
                if (queueName == new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME))
                {
                    text = "j2AMSGUI.exe, " + text;
                }

                queue = ReceiverQueue.GetQueue(false, queueName);

                if (queue != null)
                {
                    using (MessageStream = new MemoryStream(MessageBytes, 0, 4096, true, true))
                    using (MessageWriter = new BinaryWriter(MessageStream))
                    {
                        MessageStream.Seek(0, SeekOrigin.Begin);
                        MessageWriter.Write(msgId);
                        MessageWriter.Write(text.ToCharArray());

                        //copy only those bytes which are relevant to this message Id
                        byte[] tempBytes = new byte[4096];
                        Buffer.BlockCopy(MessageStream.GetBuffer(), 0, tempBytes, 0, (int)MessageStream.Position);
                        MessageData = new System.MessagingCE.Message(tempBytes);
                        queue.Send(MessageData);

                        MessageWriter.Flush();
                        MessageStream.Close();
                        MessageWriter.Close();
                    }
                }
            }
            catch //TODO (System.Exception ex)
            {
                TraceWrite.TraceLog("Unable to send text messages to the queue" + queueName);
            }
        }

        /// <summary>
        /// Send the message(string) to the message queue if the MessageReceiverQueue is not null. The MessageReceiverQueue has the name of the receving process
        /// queue
        /// </summary>
        /// <param name="queueName"></param>
        /// <param name="msgId"></param>
        /// <param name="text"></param>
        public void SendMessage(string queueName, int msgId,  GuiSendRangeData_t rangeData)
        {
            try
            {
                P2PMessageQueue queue;

                queue = ReceiverQueue.GetQueue(false, queueName);

                if (queue != null)
                {
                    using (MessageStream = new MemoryStream(MessageBytes, 0, GuiDefines.RECEIVEBUFFERSIZE, true, true))
                    using (MessageWriter = new BinaryWriter(MessageStream))
                    {
                        //since Message object can take in only byte array, we need to convert the data to byte array
                        MessageStream.Seek(0, SeekOrigin.Begin);
                        MessageWriter.Write(msgId);
                        MessageWriter.Write(rangeData.lower);
                        MessageWriter.Write(rangeData.upper);

                        //copy only those bytes which are relevant to this message Id
                        byte[] tempBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];
                        Buffer.BlockCopy(MessageStream.GetBuffer(), 0, tempBytes, 0, (int)MessageStream.Position);

                        MessageData = new System.MessagingCE.Message(tempBytes);
                        queue.Send(MessageData);

                        //flush the buffer and close the writer and stream to get ready for the next message
                        MessageWriter.Flush();
                        MessageStream.Close();
                        MessageWriter.Close();
                    }
                }
            }
            catch //TODO (System.Exception ex)
            {
                SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to send range message to the receiving process");
            }
        }

        /// <summary>
        /// Send the message(string) to the message queue if the MessageReceiverQueue is not null. The MessageReceiverQueue has the name of the receving process
        /// queue
        /// </summary>
        /// <param name="queueName"></param>
        /// <param name="msgId"></param>
        /// <param name="text"></param>
        public void SendMessage(string queueName, EngineCommReceiveMsg_t msg)
        {
            try
            {
                P2PMessageQueue queue;

                queue = ReceiverQueue.GetQueue(false, queueName);

                if (queue != null)
                {
                    using (MessageStream = new MemoryStream(MessageBytes, 0, GuiDefines.RECEIVEBUFFERSIZE, true, true))
                    using (MessageWriter = new BinaryWriter(MessageStream))
                    {
                        //since Message object can take in only byte array, we need to convert the data to byte array
                        MessageStream.Seek(0, SeekOrigin.Begin);
                                    
                        //start here nathan!
                        MessageWriter.Write(msg.msgId);
                        MessageWriter.Write(msg.data.engineMsg.message);
                        MessageWriter.Write(msg.data.engineMsg.length);
                        //MessageWriter.Write(msg.data.initAnalysis.filler);
                        //MessageWriter.Write(msg.data.messageReceiver.name);
                        
                                              
                        //copy only those bytes which are relevant to this message Id
                        byte[] tempBytes = new byte[GuiDefines.RECEIVEBUFFERSIZE];
                        Buffer.BlockCopy(MessageStream.GetBuffer(), 0, tempBytes, 0, (int)MessageStream.Position);

                        MessageData = new System.MessagingCE.Message(tempBytes);
                        queue.Send(MessageData);

                        //flush the buffer and close the writer and stream to get ready for the next message
                        MessageWriter.Flush();
                        MessageStream.Close();
                        MessageWriter.Close();
                    }
                }
            }
            catch //TODO (System.Exception ex)
            {
                SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to send range message to the receiving process");
            }
        }  

        /// <summary>
        /// All the button click messages except bottom section buttons are sent from the Gui to the
        /// receiving message queue
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendButtonMessage(object sender)
        //{
        //    int buttonIdentity;

        //    ImageButton btn = (ImageButton)sender;

        //    if (btn.Tag != null)
        //    {
        //        buttonIdentity = Int32.Parse(btn.Tag.ToString());
        //    }
        //    else
        //    {
        //        buttonIdentity = -1;
        //    }

        //   SendMessage(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME, GuiMessages.GuiMsgId_t.MSG_GUI_AMS, buttonIdentity);
        //}

        /// <summary>
        /// All the bottom button click messages are sent from the Gui to the
        /// receiving message queue
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendBottomButtonMessage(object sender)
        //{
        //    int buttonIdentity;

        //    ImageButton btn = (ImageButton)sender;
        //    if (btn.Tag != null)
        //    {
        //        buttonIdentity = (int)btn.Tag;
        //    }
        //    else
        //    {
        //        buttonIdentity = -1;
        //    }

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_BOTTOM_BUTTON, buttonIdentity);
        //}

        //internal void SendInfoMessage()
        //{
        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_INFO);
        //}        

        /// <summary>
        /// All the icon click messages on the Icon Matrix 1 and 2 screens are sent from the Gui via this method
        /// </summary>
        /// <param name="iconMessageId"></param>
        //internal void SendIconMessage(int iconMessageId)
        //{
        //    SendMessage(Queue.MessageReceiverName, iconMessageId, 0);
        //}

        /// <summary>
        /// All the check box messages are sent to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendToggleMessage(object sender)
        //{
        //    int data;

        //    TouchCheckBox btn = (TouchCheckBox)sender;

        //    if (btn.Tag != null)
        //    {
        //        data = Int32.Parse(btn.Tag.ToString());
        //    }
        //    else
        //    {
        //        data = -1;
        //    }

        //    if (btn.Checked)
        //    {
        //        data |= 0xF0;
        //    }

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_TOGGLE, data);
        //}

        /// <summary>
        /// Up button clicks messages are sent to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendUpMessage(object sender)
        //{
        //    int data = 0;

        //    //if the input arg is not of type int, then this message is sent from the timer 
        //    //when the user has pressed a arrow key for a longer duration
        //    if (sender.GetType() != data.GetType())               
        //    {  
        //        ImageButton btn = (ImageButton)sender;

        //        if (btn.Tag != null)
        //        {
        //            data = Int32.Parse(btn.Tag.ToString());
        //        }
        //        else
        //        {
        //            data = -1;
        //        }
        //    }
        //    else
        //    {
        //        //tag information
        //        data = (int)sender;
        //    }

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_UP, data);
        //}

        /// <summary>
        /// Down button clicks messages are sent to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendDownMessage(object sender)
        //{
        //    int data = 0;

        //     //if the input arg is not of type int, then this message is sent from the timer 
        //    //when the user has pressed a arrow key for a longer duration
        //    if (sender.GetType() != data.GetType())
        //    {
        //        ImageButton btn = (ImageButton)sender;

        //        if (btn.Tag != null)
        //        {
        //            data = Int32.Parse(btn.Tag.ToString());
        //        }
        //        else
        //        {
        //            data = -1;
        //        }
        //    }
        //    else
        //    {
        //        //tag information
        //        data = (int)sender;
        //    }

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_DOWN, data);
        //}

        /// <summary>
        /// List item click messages for the list Box item with single select property are sent from here.
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendListSelectedMessage(object sender)
        //{
        //    AdvancedList list = (AdvancedList)sender;
        //    Row rowSelected = list.SelectedRow;

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_LIST, rowSelected.Index);
        //}

        /// <summary>
        /// Handles all the messages to be sent from the gui for multiselect list box items
        /// selection
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendMultiListSelectedMessage(object sender)
        //{
        //    int selected = 0;

        //    AdvancedList list = (AdvancedList)sender;

        //    if (list.MultiSelect)
        //    {
        //        foreach (Row r in list.SelectedRows)
        //        {
        //            selected |= 1 << r.Index;
        //        }

        //        SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_LIST, selected);
        //    }
        //}

        /// <summary>
        /// Send keypad text message out
        /// </summary>
        /// <param name="text">text to be sent out to the receiving process</param>
        internal void SendTextMessage(string text)
        {
            SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_ID_TEXT, text);
        }

        /// <summary>
        /// Handles Icon Matrix user click messages to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        //internal void SendIconMessage(object sender)
        //{
        //    int data = -1;

        //    string buttonIdentity = string.Empty;

        //    ImageButton btn = (ImageButton)sender;

        //    if (btn.Tag != null)
        //    {
        //        buttonIdentity = btn.Tag.ToString();
        //    }

        //    Hashtable translate = new Hashtable();

        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_WHO, "Who");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_DATE_TIME, "DateTime");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_LANGUAGE, "Language");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_PRINTER, "Printer");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_SCREEN, "Screen");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_SOUND, "Sound");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_UNITS, "Units");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_REF_RANGES, "RefRanges");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_ARCHIVE_REF_RANGES, "ArchiveRefRanges");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_COMM, "Comm");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_SECURITY, "Security");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_ADVANCED, "Advanced");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_UPDATE, "Update");
        //    translate.Add(GuiMessages.GuiIconMatrixData_t.ICON_FACTORY, "Factory");

        //    foreach (DictionaryEntry de in translate)
        //    {
        //        //get the entry from the HashTable which matches the icon tag value clicked by the user
        //        if (de.Value.ToString() == buttonIdentity)
        //        {
        //            data = (int)de.Key;

        //            break;
        //        }
        //    }

        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_ICON_MATRIX, data);
        //}

        //internal void SendRangeMessage(GuiSendRangeData_t rangeData)
        //{
        //    SendMessage(Queue.MessageReceiverName, (int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_RANGE, rangeData);
        //}

        /// <summary>
        /// Get the tag data of the control clicked
        /// </summary>
        /// <param name="sender"></param>
        /// <returns></returns>
        //internal int GetTagData(object sender)
        //{
        //    int data;

        //    ImageButton btn = (ImageButton)sender;

        //    if (btn.Tag != null)
        //    {
        //        data = Int32.Parse(btn.Tag.ToString());
        //    }
        //    else
        //    {
        //        data = -1;
        //    }

        //    return data;
        //}
    }
}
