using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Drawing;
using System.Diagnostics;
using CppCsWrapper;

namespace j2GUI
{
    /// <summary>
    /// This class holds all the utility methods required for the project
    /// </summary>
    public class Utilities
    {
        private Messaging.SendQueue SendMessageToQueue = new j2GUI.Messaging.SendQueue();

        /// <summary>
        /// Convert Two Dimensional Char Array to String Array
        /// </summary>
        /// <param name="text">Two Dimensional Char array</param>
        /// <returns>string array</returns>
        public string[] TwoCharArrayToStringArray(char[,] text)
        {
            string[] ctrlText = new string[text.GetUpperBound(0) + 1];
            int i, j = 0;
            for (i = 0; i < text.GetUpperBound(0) + 1; i++)
            {
                char[] temp = new char[text.GetUpperBound(1) + 1];

                for (j = 0; j < text.GetUpperBound(1) + 1; j++)
                {
                    if (text[i, j] == '\0')//end of string
                    {
                        break;
                    }
                    temp[j] = text[i, j];
                }

                ctrlText[i] = new string(temp, 0, j);
            }

            return (ctrlText);
        }

        /// <summary>
        /// Convert One Dimensional Char Array To String
        /// </summary>
        /// <param name="text">one dimensional char array</param>
        /// <returns>string</returns>
        public string OneCharArrayToString(char[] text)
        {
            char[] temp = new char[text.GetUpperBound(0) + 1];

            int j = 0;
            for (j = 0; j < text.GetUpperBound(0) + 1; j++)
            {
                if (text[j] == '\0')//end of string
                {
                    break;
                }
                temp[j] = text[j];
            }

            return (new string(temp, 0, j));
        }

        /// <summary>
        /// Convert One dimensional char array to Two dimensional char array
        /// </summary>
        /// <param name="text">one dimensional char array</param>
        /// <returns>two dimensional char array</returns>
        public char[,] OneCharArrayToTwoCharArray(char[] text)
        {
            char[,] temp = new char[3, 30];
            int ind = 0;

            foreach (char c in text)
            {
                temp[0, ind++] = c;
            }

            return (temp);
        }

        /// <summary>
        /// Convert stream to two dimensional char array
        /// </summary>
        /// <param name="reader">binary reader to read the stream</param>
        /// <param name="arraySize1">size of the first dimension array</param>
        /// <param name="arraySize2">size of the second dimension array</param>
        /// <returns>two dimensional array</returns>
        public char[,] StreamToTwoCharArray(BinaryReader reader, int arraySize1, int arraySize2)
        {
            char[,] text = new char[arraySize1, arraySize2];

            for (int i = 0; i < arraySize1; i++)
            {
                for (int j = 0; j < arraySize2; j++)
                {
                    text[i, j] = reader.ReadChar();
                }

            }

            return (text);
        }

        /// <summary>
        /// Convert stream to three dimensional char array 
        /// </summary>
        /// <param name="reader">binary reader to read the stream</param>
        /// <param name="arraySize1">size of the first dimension array</param>
        /// <param name="arraySize2">size of the second dimension array</param>
        /// <param name="arraySize3">size of the third dimension array</param>
        /// <returns>three dimensional char array</returns>
        private char[, ,] StreamToThreeCharArray(BinaryReader reader, int arraySize1, int arraySize2, int arraySize3)
        {
            char[, ,] itemsText = new char[arraySize1, arraySize2, arraySize3];
            char c;

            for (int g = 0; g < arraySize1; g++)
            {
                for (int i = 0; i < arraySize2; i++)
                {
                    for (int j = 0; j < arraySize3; j++)
                    {
                        itemsText[g, i, j] = reader.ReadChar();
                    }
                }
            }

            return (itemsText);
        }

        /// <summary>
        /// Convert string to char array
        /// </summary>
        /// <param name="arraySize">size of the char array</param>
        /// <param name="text">string to be converted</param>
        /// <returns>char array of string</returns>
        private char[] StringToCharArray(int arraySize, string text)
        {
            char[] charArray = new char[arraySize];
            int i = 0;

            foreach (char c in text.ToCharArray())
            {
                charArray[i++] = c;
            }

            return (charArray);
        }

        /// <summary>
        /// debug only
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        private char[] FillNull(int index)
        {
            char[] temp = new char[index];
            for (int i = 0; i < index; i++)
            {
                temp[i] = (char)0;
            }

            return temp;
        }

        /// <summary>
        /// The P2P message queue gets the gui messages in a byte array. Convert this to GuiReceiveMsg_t format which is
        /// understood by other sections of the screens
        /// </summary>
        /// <param name="messageData">byte array of gui messages</param>
        /// <returns>GuiReceiveMsg_t format of the received byte array depending on the message Id</returns>
        internal GuiReceiveMsg_t TranslateToGuiReceiveMessage(byte[] messageData)
        {
            GuiReceiveMsg_t msg = new GuiReceiveMsg_t();
            GuiReceiveMsgData_t msgData = new GuiReceiveMsgData_t();

            int msgId = 0;

            try
            {

                using (MemoryStream receiveQueueStream = new MemoryStream(messageData))
                {
                    //BinaryReader reader = new BinaryReader(receiveQueueStream, Encoding.Default);
                    BinaryReader reader = new BinaryReader(receiveQueueStream, Encoding.Unicode);//for chinese

                    msgId = reader.ReadInt32();

                    //log the message Id received by the Gui
                    //SendMessageToQueue.SendLogErrorMessage("Gui Received MsgId = " + msgId.ToString()); debug only

                    switch (msgId)
                    {
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_TOP):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_TOP;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_NONE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_NONE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_LABEL):
                            {
                                GuiTopLabelData_t topLabelData = new GuiTopLabelData_t();
                                topLabelData.label = reader.ReadChars(topLabelData.label.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_LABEL;
                                msgData.topLabel = topLabelData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_TOP):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_TOP;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_BOTTOM):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_BOTTOM;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_NONE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_NONE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_ONE_BUTTON):
                            {
                                GuiBottomButtonsData_t bottomButtonsData = new GuiBottomButtonsData_t();

                                bottomButtonsData.buttonText = StreamToTwoCharArray(reader, bottomButtonsData.buttonText.GetUpperBound(0) + 1, bottomButtonsData.buttonText.GetUpperBound(1) + 1);
                                bottomButtonsData.buttonHighlighted = reader.ReadChars(bottomButtonsData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_ONE_BUTTON;
                                msgData.bottomButtons = bottomButtonsData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_TWO_BUTTONS):
                            {
                                GuiBottomButtonsData_t bottomButtonsData = new GuiBottomButtonsData_t();

                                bottomButtonsData.buttonText = StreamToTwoCharArray(reader, bottomButtonsData.buttonText.GetUpperBound(0) + 1, bottomButtonsData.buttonText.GetUpperBound(1) + 1);
                                bottomButtonsData.buttonHighlighted = reader.ReadChars(bottomButtonsData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_TWO_BUTTONS;
                                msgData.bottomButtons = bottomButtonsData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_THREE_BUTTONS):
                            {
                                GuiBottomButtonsData_t bottomButtonsData = new GuiBottomButtonsData_t();

                                bottomButtonsData.buttonText = StreamToTwoCharArray(reader, bottomButtonsData.buttonText.GetUpperBound(0) + 1, bottomButtonsData.buttonText.GetUpperBound(1) + 1);
                                bottomButtonsData.buttonHighlighted = reader.ReadChars(bottomButtonsData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_THREE_BUTTONS;
                                msgData.bottomButtons = bottomButtonsData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_DATE_TIME):
                            {
                                GuiBottomDateTimeData_t bottomDateTimeData = new GuiBottomDateTimeData_t();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_DATE_TIME;
                                msgData.bottomDateTime = bottomDateTimeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_BOTTOM):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_BOTTOM;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_WIN):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_WIN;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_NONE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_NONE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP):
                            {
                                GuiWinCriticalStopData_t criticalStopData = new GuiWinCriticalStopData_t();

                                criticalStopData.topLabel = reader.ReadChars(criticalStopData.topLabel.GetUpperBound(0) + 1);
                                criticalStopData.message = reader.ReadChars(criticalStopData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP;
                                msgData.winCriticalStop = criticalStopData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2):
                            {
                                GuiWinErrorData_t winErrorData = new GuiWinErrorData_t();

                                winErrorData.topLabel = reader.ReadChars(winErrorData.topLabel.GetUpperBound(0) + 1);
                                winErrorData.message = reader.ReadChars(winErrorData.message.GetUpperBound(0) + 1);
                                winErrorData.buttonText = reader.ReadChars(winErrorData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2;
                                msgData.winError = winErrorData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR):
                            {
                                GuiWinErrorData_t winErrorData = new GuiWinErrorData_t();

                                winErrorData.topLabel = reader.ReadChars(winErrorData.topLabel.GetUpperBound(0) + 1);
                                winErrorData.message = reader.ReadChars(winErrorData.message.GetUpperBound(0) + 1);
                                winErrorData.buttonText = reader.ReadChars(winErrorData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR;
                                msgData.winError = winErrorData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1):
                            {
                                GuiWinWarningData_t winWarningData = new GuiWinWarningData_t();

                                winWarningData.topLabel = reader.ReadChars(winWarningData.topLabel.GetUpperBound(0) + 1);
                                winWarningData.message = reader.ReadChars(winWarningData.message.GetUpperBound(0) + 1);
                                winWarningData.buttonText = StreamToTwoCharArray(reader, winWarningData.buttonText.GetUpperBound(0) + 1, winWarningData.buttonText.GetUpperBound(1) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1;
                                msgData.winWarning = winWarningData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2):
                            {
                                GuiWinWarningData_t winWarningData = new GuiWinWarningData_t();

                                winWarningData.topLabel = reader.ReadChars(winWarningData.topLabel.GetUpperBound(0) + 1);
                                winWarningData.message = reader.ReadChars(winWarningData.message.GetUpperBound(0) + 1);
                                winWarningData.buttonText = StreamToTwoCharArray(reader, winWarningData.buttonText.GetUpperBound(0) + 1, winWarningData.buttonText.GetUpperBound(1) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2;
                                msgData.winWarning = winWarningData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_WIN):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_WIN;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_MIDDLE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_MIDDLE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_NONE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_NONE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE):
                            {
                                GuiMiddleMessageData_t middleMessageData = new GuiMiddleMessageData_t();

                                middleMessageData.message = reader.ReadChars(middleMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE;
                                msgData.middleMessage = middleMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE_INFO):
                            {
                                GuiMiddleMessageData_t middleMessageData = new GuiMiddleMessageData_t();

                                middleMessageData.message = reader.ReadChars(middleMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE_INFO;
                                msgData.middleMessage = middleMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS):
                            {
                                GuiMiddleButtonsData_t middleButtonsData = new GuiMiddleButtonsData_t();

                                middleButtonsData.buttonText = StreamToTwoCharArray(reader, middleButtonsData.buttonText.GetUpperBound(0) + 1, middleButtonsData.buttonText.GetUpperBound(1) + 1);
                                middleButtonsData.buttonHighlighted = reader.ReadChars(middleButtonsData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS;
                                msgData.middleButtons = middleButtonsData;
                                msg.data = msgData;
                                break;

                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS_INFO):
                            {
                                GuiMiddleButtonsInfoData_t middleButtonsInfoData = new GuiMiddleButtonsInfoData_t();

                                middleButtonsInfoData.buttonText = StreamToTwoCharArray(reader, middleButtonsInfoData.buttonText.GetUpperBound(0) + 1, middleButtonsInfoData.buttonText.GetUpperBound(1) + 1);
                                middleButtonsInfoData.buttonHighlighted = reader.ReadChars(middleButtonsInfoData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS_INFO;
                                msgData.middleButtonsInfo = middleButtonsInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_INFO):
                            {
                                GuiMiddleMultiInfoData_t middleMultiInfoData = new GuiMiddleMultiInfoData_t();

                                middleMultiInfoData.buttonText = StreamToTwoCharArray(reader, middleMultiInfoData.buttonText.GetUpperBound(0) + 1, middleMultiInfoData.buttonText.GetUpperBound(1) + 1);
                                middleMultiInfoData.buttonChecked = reader.ReadChars(middleMultiInfoData.buttonChecked.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_INFO;
                                msgData.middleMultiInfo = middleMultiInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_SELECT):
                            {
                                GuiMiddleMultiSelectData_t middleMultiSelectData = new GuiMiddleMultiSelectData_t();

                                middleMultiSelectData.buttonText = StreamToTwoCharArray(reader, middleMultiSelectData.buttonText.GetUpperBound(0) + 1, middleMultiSelectData.buttonText.GetUpperBound(1) + 1);
                                middleMultiSelectData.buttonChecked = reader.ReadChars(middleMultiSelectData.buttonChecked.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_SELECT;
                                msgData.middleMultiSelect = middleMultiSelectData;
                                msg.data = msgData;
                                break;
                            }

                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_OPTION_INFO):
                            {
                                GuiMiddleOptionInfoData_t middleOptionInfoData = new GuiMiddleOptionInfoData_t();

                                middleOptionInfoData.buttonText = StreamToTwoCharArray(reader, middleOptionInfoData.buttonText.GetUpperBound(0) + 1, middleOptionInfoData.buttonText.GetUpperBound(1) + 1);
                                middleOptionInfoData.selectedButton = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_OPTION_INFO;
                                msgData.middleOptionInfo = middleOptionInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_OPTION):
                            {
                                GuiMiddleSelectOptionData_t middleSelectOptionData = new GuiMiddleSelectOptionData_t();

                                middleSelectOptionData.buttonText = StreamToTwoCharArray(reader, middleSelectOptionData.buttonText.GetUpperBound(0) + 1, middleSelectOptionData.buttonText.GetUpperBound(1) + 1);
                                middleSelectOptionData.selectedButton = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_OPTION;
                                msgData.middleSelectOption = middleSelectOptionData;
                                msg.data = msgData;
                                break;
                            }

                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY):
                            {
                                GuiMiddleHomeReadyData_t middleHomeReadyData = new GuiMiddleHomeReadyData_t();

                                middleHomeReadyData.buttonText = StreamToTwoCharArray(reader, middleHomeReadyData.buttonText.GetUpperBound(0) + 1, middleHomeReadyData.buttonText.GetUpperBound(1) + 1);
                                middleHomeReadyData.buttonHighlighted = reader.ReadChars(middleHomeReadyData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY;
                                msgData.middleHomeReady = middleHomeReadyData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_MFG):
                            {
                                GuiMiddleHomeReadyData_t middleHomeReadyData = new GuiMiddleHomeReadyData_t();

                                middleHomeReadyData.buttonText = StreamToTwoCharArray(reader, middleHomeReadyData.buttonText.GetUpperBound(0) + 1, middleHomeReadyData.buttonText.GetUpperBound(1) + 1);
                                middleHomeReadyData.buttonHighlighted = reader.ReadChars(middleHomeReadyData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_MFG;
                                msgData.middleHomeReady = middleHomeReadyData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_RD):
                            {
                                GuiMiddleHomeReadyData_t middleHomeReadyData = new GuiMiddleHomeReadyData_t();

                                middleHomeReadyData.buttonText = StreamToTwoCharArray(reader, middleHomeReadyData.buttonText.GetUpperBound(0) + 1, middleHomeReadyData.buttonText.GetUpperBound(1) + 1);
                                middleHomeReadyData.buttonHighlighted = reader.ReadChars(middleHomeReadyData.buttonHighlighted.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_RD;
                                msgData.middleHomeReady = middleHomeReadyData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING):
                            {
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_MFG):
                            {
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_MFG;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT):
                            {
                                //added nwr 5/14/2013
                                GuiMiddleViewResultsData_t middleViewResults = new GuiMiddleViewResultsData_t();
                                middleViewResults.message = reader.ReadChars(middleViewResults.message.GetUpperBound(0) + 1);
                                //middleViewResults.message = AddCRLF(middleViewResults.message); //CR and LF already present in the message from the console
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT;
                                msgData.middleViewResults = middleViewResults;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT):
                            {
                                //added nwr 5/14/2013
                                GuiMiddleViewResultsData_t middleViewResults = new GuiMiddleViewResultsData_t();
                                middleViewResults.message = reader.ReadChars(middleViewResults.message.GetUpperBound(0) + 1);
                                //middleViewResults.message = AddCRLF(middleViewResults.message);
                                char[] temp = AddCRLF(middleViewResults.message); //LF not present in the message from the console. 
                                Array.Resize(ref middleViewResults.message, temp.Length);
                                middleViewResults.message = temp;
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT;
                                msgData.middleViewResults = middleViewResults;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_RD):
                            {
                                //added nwr 5/14/2013
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_RD;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_PROGRESS):
                            {
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_PROGRESS;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_CUVETTE_DATA):
                            {
                                GuiMiddleCuvetteTestData_t middleCuvetteTestData = new GuiMiddleCuvetteTestData_t();
                                middleCuvetteTestData.resultsText = reader.ReadChars(middleCuvetteTestData.resultsText.GetUpperBound(0) + 1);

                                for (int x = 0; x < middleCuvetteTestData.valueText.GetUpperBound(0) + 1; x++)
                                {
                                    for (int y = 0; y < middleCuvetteTestData.valueText.GetUpperBound(1) + 1; y++)
                                    {
                                        middleCuvetteTestData.valueText[x, y] = reader.ReadChar();
                                    }
                                }

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_CUVETTE_DATA;
                                msgData.middleCuvette = middleCuvetteTestData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_CUVETTE_DATA):
                            {
                                GuiMiddleCuvetteTestData_t middleCuvetteTestData = new GuiMiddleCuvetteTestData_t();
                                middleCuvetteTestData.resultsText = reader.ReadChars(middleCuvetteTestData.resultsText.GetUpperBound(0) + 1);

                                for (int x = 0; x < middleCuvetteTestData.valueText.GetUpperBound(0) + 1; x++)
                                {
                                    for (int y = 0; y < middleCuvetteTestData.valueText.GetUpperBound(1) + 1; y++)
                                    {
                                        middleCuvetteTestData.valueText[x, y] = reader.ReadChar();
                                    }
                                }

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_CUVETTE_DATA;
                                msgData.middleCuvette = middleCuvetteTestData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS):
                            {
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_PROG_BAR_MESSAGE):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_PROG_BAR_MESSAGE;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_COLORED):
                            {
                                GuiMiddleListColoredData_t middleListColoredData = new GuiMiddleListColoredData_t();

                                middleListColoredData.items = StreamToTwoCharArray(reader, middleListColoredData.items.GetUpperBound(0) + 1, middleListColoredData.items.GetUpperBound(1) + 1);

                                middleListColoredData.fontColor[0] = reader.ReadUInt32();
                                middleListColoredData.fontColor[1] = reader.ReadUInt32();
                                middleListColoredData.fontColor[2] = reader.ReadUInt32();
                                middleListColoredData.fontColor[3] = reader.ReadUInt32();
                                middleListColoredData.fontColor[4] = reader.ReadUInt32();

                                middleListColoredData.backgroundColor[0] = reader.ReadUInt32();
                                middleListColoredData.backgroundColor[1] = reader.ReadUInt32();
                                middleListColoredData.backgroundColor[2] = reader.ReadUInt32();
                                middleListColoredData.backgroundColor[3] = reader.ReadUInt32();
                                middleListColoredData.backgroundColor[4] = reader.ReadUInt32();
                                    
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_COLORED;
                                msgData.middleListColored = middleListColoredData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LOAD_ORDER):
                            {
                                GuiMiddleLoadOrderData_t middleLoadOrderData = new GuiMiddleLoadOrderData_t();

                                middleLoadOrderData.orderLabel = reader.ReadChars(middleLoadOrderData.orderLabel.GetUpperBound(0) + 1);
                                middleLoadOrderData.rotorName = reader.ReadChars(middleLoadOrderData.rotorName.GetUpperBound(0) + 1);
                                middleLoadOrderData.buttonText = reader.ReadChars(middleLoadOrderData.buttonText.GetUpperBound(0) + 1);
                                middleLoadOrderData.backgroundColor = reader.ReadUInt32();
                                middleLoadOrderData.fontColor = reader.ReadUInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LOAD_ORDER;
                                msgData.middleLoadOrder = middleLoadOrderData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_MIDDLE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_MIDDLE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_UPDATE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_UPDATE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT):
                            {
                                GuiMiddleProgBarMessageData_t middleProgBarMessageData = new GuiMiddleProgBarMessageData_t();

                                middleProgBarMessageData.progBarText = reader.ReadChars(middleProgBarMessageData.progBarText.GetUpperBound(0) + 1);
                                middleProgBarMessageData.progBarValue = reader.ReadInt32();
                                middleProgBarMessageData.backgroundColor = reader.ReadUInt32();
                                middleProgBarMessageData.fontColor = reader.ReadUInt32();
                                middleProgBarMessageData.message = reader.ReadChars(middleProgBarMessageData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT;
                                msgData.middleProgBarMessage = middleProgBarMessageData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES):
                            {
                                //added nwr 4/24/2013
                                GuiMiddleTemperatureData_t middleTemperatureData = new GuiMiddleTemperatureData_t();
                                middleTemperatureData.rotorTemperatureValue = reader.ReadChars(middleTemperatureData.rotorTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.ambientTemperatureValue = reader.ReadChars(middleTemperatureData.ambientTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.ambientDacOffsetValue = reader.ReadChars(middleTemperatureData.ambientDacOffsetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.airTemperatureOffsetValue = reader.ReadChars(middleTemperatureData.airTemperatureOffsetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterTemperatureValue = reader.ReadChars(middleTemperatureData.topHeaterTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterTemperatureValue = reader.ReadChars(middleTemperatureData.bottomHeaterTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterCurrentValue = reader.ReadChars(middleTemperatureData.topHeaterCurrentValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterCurrentValue = reader.ReadChars(middleTemperatureData.bottomHeaterCurrentValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterTemperatureSetValue = reader.ReadChars(middleTemperatureData.topHeaterTemperatureSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterTemperatureSetValue = reader.ReadChars(middleTemperatureData.bottomHeaterTemperatureSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topDacSetValue = reader.ReadChars(middleTemperatureData.topDacSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomDacSetValue = reader.ReadChars(middleTemperatureData.bottomDacSetValue.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES;
                                msgData.middleTemperature = middleTemperatureData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_BARCODE_DATA):
                            {
                                GuiMiddleTestData_t middleBarcodeData = new GuiMiddleTestData_t();
                                middleBarcodeData.resultsText = reader.ReadChars(middleBarcodeData.resultsText.GetUpperBound(0) + 1);
                                middleBarcodeData.valueText = reader.ReadChars(middleBarcodeData.valueText.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton0 = reader.ReadChars(middleBarcodeData.middleBotton0.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton1 = reader.ReadChars(middleBarcodeData.middleBotton1.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton2 = reader.ReadChars(middleBarcodeData.middleBotton2.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_BARCODE_DATA;
                                msgData.middleBarcode = middleBarcodeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_RD_PROGRESS):
                            {
                                //added nwr 8/22/2013
                                GuiMiddleHomeWarmingData_t middleHomeWarmingData = new GuiMiddleHomeWarmingData_t();

                                middleHomeWarmingData.progBarText = reader.ReadChars(middleHomeWarmingData.progBarText.GetUpperBound(0) + 1);
                                middleHomeWarmingData.progBarValue = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_RD_PROGRESS;
                                msgData.middleHomeWarming = middleHomeWarmingData;
                                msg.data = msgData;
                                break;

                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_MOTOR_DATA):
                            {
                                //added nwr 5/6/2013
                                GuiMiddleMotorData_t middleMotorData = new GuiMiddleMotorData_t();
                                middleMotorData.motorSpeedReadValue = reader.ReadChars(middleMotorData.motorSpeedReadValue.GetUpperBound(0) + 1);
                                middleMotorData.motorSpeedSetValue = reader.ReadChars(middleMotorData.motorSpeedSetValue.GetUpperBound(0) + 1);
                                middleMotorData.motorDirection = reader.ReadChars(middleMotorData.motorDirection.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_MOTOR_DATA;
                                msgData.middleMotor = middleMotorData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_MULTI):
                            {
                                GuiMiddleListMultiData_t middleListMultiData = new GuiMiddleListMultiData_t();

                                middleListMultiData.items = StreamToTwoCharArray(reader, middleListMultiData.items.GetUpperBound(0) + 1, middleListMultiData.items.GetUpperBound(1) + 1);
                                middleListMultiData.isSelected = reader.ReadChars(middleListMultiData.isSelected.GetUpperBound(0) + 1);
                                middleListMultiData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_MULTI;
                                msgData.middleListMulti = middleListMultiData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_MULTI):
                            {
                                GuiMiddleListMultiData_t middleListMultiData = new GuiMiddleListMultiData_t();

                                middleListMultiData.items = StreamToTwoCharArray(reader, middleListMultiData.items.GetUpperBound(0) + 1, middleListMultiData.items.GetUpperBound(1) + 1);
                                middleListMultiData.isSelected = reader.ReadChars(middleListMultiData.isSelected.GetUpperBound(0) + 1);
                                middleListMultiData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_MULTI;
                                msgData.middleListMulti = middleListMultiData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_REF_RANGES):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_REF_RANGES;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_REF_RANGES):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_REF_RANGES;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_SETTINGS):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_SETTINGS;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_SETTINGS):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_SETTINGS;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_RESULTS):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_RESULTS;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_RESULTS):
                            {
                                GuiMiddleListData_t middleListData = new GuiMiddleListData_t();

                                middleListData.items = StreamToThreeCharArray(reader, middleListData.items.GetUpperBound(0) + 1, middleListData.items.GetUpperBound(1) + 1, middleListData.items.GetUpperBound(2) + 1);
                                middleListData.isSelectable = reader.ReadChar();
                                middleListData.hasUpDown = reader.ReadChar();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_RESULTS;
                                msgData.middleList = middleListData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP):
                            {
                                GuiMiddleEnterIpData_t middleEnterIpData = new GuiMiddleEnterIpData_t();
                                middleEnterIpData.text = reader.ReadChars(middleEnterIpData.text.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP;
                                msgData.middleEnterIp = middleEnterIpData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP):
                            {
                                GuiMiddleEnterIpData_t middleEnterIpData = new GuiMiddleEnterIpData_t();
                                middleEnterIpData.text = reader.ReadChars(middleEnterIpData.text.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP;
                                msgData.middleEnterIp = middleEnterIpData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID):
                            {
                                GuiMiddleEnterIdData_t middleEnterIdData = new GuiMiddleEnterIdData_t();
                                middleEnterIdData.isPassword = reader.ReadChar();
                                middleEnterIdData.initialValue = reader.ReadChars(middleEnterIdData.initialValue.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID;
                                msgData.middleEnterId = middleEnterIdData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_ID):
                            {
                                GuiMiddleEnterIdData_t middleEnterIdData = new GuiMiddleEnterIdData_t();
                                middleEnterIdData.isPassword = reader.ReadChar();
                                middleEnterIdData.initialValue = reader.ReadChars(middleEnterIdData.initialValue.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_ID;
                                msgData.middleEnterId = middleEnterIdData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_INPUT_RECEIVER):
                            {
                                GuiInputReceiverData_t inputReceiverData = new GuiInputReceiverData_t();
                                inputReceiverData.name = reader.ReadChars(inputReceiverData.name.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_INPUT_RECEIVER;
                                msgData.inputReceiver = inputReceiverData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_ID_TEXT):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_ID_TEXT;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_RANGE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_RANGE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_DATE_TIME_STRINGS):
                            {
                                GuiBottomDateTimeData_t bottomDateTimeData = new GuiBottomDateTimeData_t();
                                bottomDateTimeData.twentyFourHourFormat = reader.ReadChar();
                                bottomDateTimeData.daysOfWeek = StreamToTwoCharArray(reader, bottomDateTimeData.daysOfWeek.GetUpperBound(0) + 1, bottomDateTimeData.daysOfWeek.GetUpperBound(1) + 1);
                                bottomDateTimeData.months = StreamToTwoCharArray(reader, bottomDateTimeData.months.GetUpperBound(0) + 1, bottomDateTimeData.months.GetUpperBound(1) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_DATE_TIME_STRINGS;
                                msgData.bottomDateTime = bottomDateTimeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_DATE):
                            {
                                GuiMiddleEnterDateData_t middleEnterDateData = new GuiMiddleEnterDateData_t();
                                middleEnterDateData.labelLeft = reader.ReadChars(middleEnterDateData.labelLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.labelMiddle = reader.ReadChars(middleEnterDateData.labelMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.labelRight = reader.ReadChars(middleEnterDateData.labelRight.GetUpperBound(0) + 1);
                                middleEnterDateData.textLeft = reader.ReadChars(middleEnterDateData.textLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.textMiddle = reader.ReadChars(middleEnterDateData.textMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.textRight = reader.ReadChars(middleEnterDateData.textRight.GetUpperBound(0) + 1);
                                middleEnterDateData.buttonText = reader.ReadChars(middleEnterDateData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_DATE;
                                msgData.middleEnterDate = middleEnterDateData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_LEFT):
                            {
                                GuiMiddleEnterDateData_t middleEnterDateData = new GuiMiddleEnterDateData_t();
                                middleEnterDateData.labelLeft = reader.ReadChars(middleEnterDateData.labelLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.labelMiddle = reader.ReadChars(middleEnterDateData.labelMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.labelRight = reader.ReadChars(middleEnterDateData.labelRight.GetUpperBound(0) + 1);
                                middleEnterDateData.textLeft = reader.ReadChars(middleEnterDateData.textLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.textMiddle = reader.ReadChars(middleEnterDateData.textMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.textRight = reader.ReadChars(middleEnterDateData.textRight.GetUpperBound(0) + 1);
                                middleEnterDateData.buttonText = reader.ReadChars(middleEnterDateData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_LEFT;
                                msgData.middleEnterDate = middleEnterDateData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_MIDDLE):
                            {
                                GuiMiddleEnterDateData_t middleEnterDateData = new GuiMiddleEnterDateData_t();
                                middleEnterDateData.labelLeft = reader.ReadChars(middleEnterDateData.labelLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.labelMiddle = reader.ReadChars(middleEnterDateData.labelMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.labelRight = reader.ReadChars(middleEnterDateData.labelRight.GetUpperBound(0) + 1);
                                middleEnterDateData.textLeft = reader.ReadChars(middleEnterDateData.textLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.textMiddle = reader.ReadChars(middleEnterDateData.textMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.textRight = reader.ReadChars(middleEnterDateData.textRight.GetUpperBound(0) + 1);
                                middleEnterDateData.buttonText = reader.ReadChars(middleEnterDateData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_MIDDLE;
                                msgData.middleEnterDate = middleEnterDateData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_RIGHT):
                            {
                                GuiMiddleEnterDateData_t middleEnterDateData = new GuiMiddleEnterDateData_t();
                                middleEnterDateData.labelLeft = reader.ReadChars(middleEnterDateData.labelLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.labelMiddle = reader.ReadChars(middleEnterDateData.labelMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.labelRight = reader.ReadChars(middleEnterDateData.labelRight.GetUpperBound(0) + 1);
                                middleEnterDateData.textLeft = reader.ReadChars(middleEnterDateData.textLeft.GetUpperBound(0) + 1);
                                middleEnterDateData.textMiddle = reader.ReadChars(middleEnterDateData.textMiddle.GetUpperBound(0) + 1);
                                middleEnterDateData.textRight = reader.ReadChars(middleEnterDateData.textRight.GetUpperBound(0) + 1);
                                middleEnterDateData.buttonText = reader.ReadChars(middleEnterDateData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_RIGHT;
                                msgData.middleEnterDate = middleEnterDateData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_TIME):
                            {
                                GuiMiddleEnterTimeData_t middleEnterTimeData = new GuiMiddleEnterTimeData_t();
                                middleEnterTimeData.twentyFourHourFormat = reader.ReadChar();
                                middleEnterTimeData.hoursLabel = reader.ReadChars(middleEnterTimeData.hoursLabel.GetUpperBound(0) + 1);
                                middleEnterTimeData.minutesLabel = reader.ReadChars(middleEnterTimeData.minutesLabel.GetUpperBound(0) + 1);
                                middleEnterTimeData.formatButtonText = reader.ReadChars(middleEnterTimeData.formatButtonText.GetUpperBound(0) + 1);
                                middleEnterTimeData.zeroSecButtonText = reader.ReadChars(middleEnterTimeData.zeroSecButtonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_TIME;
                                msgData.middleEnterTime = middleEnterTimeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS):
                            {
                                GuiMiddleViewResultsData_t middleViewResults = new GuiMiddleViewResultsData_t(); 
                                middleViewResults.message = reader.ReadChars(middleViewResults.message.GetUpperBound(0) + 1);
                                //middleViewResults.message = AddCRLF(middleViewResults.message);  //commented on 08/16/2016
                                char[] temp = AddCRLF(middleViewResults.message); //LF not present in the message from the console. Added by nj on 08/16/2016
                                Array.Resize(ref middleViewResults.message, temp.Length);
                                middleViewResults.message = temp;
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS;
                                msgData.middleViewResults = middleViewResults;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_TEMPERATURE_INFO):
                            {   //added nwr 4/24/2013
                                GuiMiddleTemperatureData_t middleTemperatureData = new GuiMiddleTemperatureData_t();
                                middleTemperatureData.rotorTemperatureValue = reader.ReadChars(middleTemperatureData.rotorTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.ambientTemperatureValue = reader.ReadChars(middleTemperatureData.ambientTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.ambientDacOffsetValue = reader.ReadChars(middleTemperatureData.ambientDacOffsetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.airTemperatureOffsetValue = reader.ReadChars(middleTemperatureData.airTemperatureOffsetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterTemperatureValue = reader.ReadChars(middleTemperatureData.topHeaterTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterTemperatureValue = reader.ReadChars(middleTemperatureData.bottomHeaterTemperatureValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterCurrentValue = reader.ReadChars(middleTemperatureData.topHeaterCurrentValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterCurrentValue = reader.ReadChars(middleTemperatureData.bottomHeaterCurrentValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topHeaterTemperatureSetValue = reader.ReadChars(middleTemperatureData.topHeaterTemperatureSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomHeaterTemperatureSetValue = reader.ReadChars(middleTemperatureData.bottomHeaterTemperatureSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.topDacSetValue = reader.ReadChars(middleTemperatureData.topDacSetValue.GetUpperBound(0) + 1);
                                middleTemperatureData.bottomDacSetValue = reader.ReadChars(middleTemperatureData.bottomDacSetValue.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_TEMPERATURE_INFO;
                                msgData.middleTemperature = middleTemperatureData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MOTOR_DATA):
                            {
                                //added nwr 5/6/2013
                                GuiMiddleMotorData_t middleMotorData = new GuiMiddleMotorData_t();
                                middleMotorData.motorSpeedReadValue = reader.ReadChars(middleMotorData.motorSpeedReadValue.GetUpperBound(0) + 1);
                                middleMotorData.motorSpeedSetValue = reader.ReadChars(middleMotorData.motorSpeedSetValue.GetUpperBound(0) + 1);
                                middleMotorData.motorDirection = reader.ReadChars(middleMotorData.motorDirection.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MOTOR_DATA;
                                msgData.middleMotor = middleMotorData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BARCODE_DATA):
                            {
                                //added nwr 5/7/2013
                                GuiMiddleTestData_t middleBarcodeData = new GuiMiddleTestData_t();
                                middleBarcodeData.resultsText = reader.ReadChars(middleBarcodeData.resultsText.GetUpperBound(0) + 1);
                                middleBarcodeData.valueText = reader.ReadChars(middleBarcodeData.valueText.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton0 = reader.ReadChars(middleBarcodeData.middleBotton0.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton1 = reader.ReadChars(middleBarcodeData.middleBotton1.GetUpperBound(0) + 1);
                                middleBarcodeData.middleBotton2 = reader.ReadChars(middleBarcodeData.middleBotton2.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BARCODE_DATA;
                                msgData.middleBarcode = middleBarcodeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_TYPE):
                            {
                                GuiMiddleSelectTypeData_t middleSelectTypeData = new GuiMiddleSelectTypeData_t();

                                middleSelectTypeData.buttonText = StreamToTwoCharArray(reader, middleSelectTypeData.buttonText.GetUpperBound(0) + 1, middleSelectTypeData.buttonText.GetUpperBound(1) + 1);
                                middleSelectTypeData.buttonChecked = reader.ReadChars(middleSelectTypeData.buttonChecked.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_TYPE;
                                msgData.middleSelectType = middleSelectTypeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO1):
                            {
                                GuiMiddleGraphicInfoData_t middleGraphicInfoData = new GuiMiddleGraphicInfoData_t();

                                middleGraphicInfoData.message = reader.ReadChars(middleGraphicInfoData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO1;
                                msgData.middleGraphicInfo = middleGraphicInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO2):
                            {
                                GuiMiddleGraphicInfoData_t middleGraphicInfoData = new GuiMiddleGraphicInfoData_t();

                                middleGraphicInfoData.message = reader.ReadChars(middleGraphicInfoData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO2;
                                msgData.middleGraphicInfo = middleGraphicInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO3):
                            {
                                GuiMiddleGraphicInfoData_t middleGraphicInfoData = new GuiMiddleGraphicInfoData_t();

                                middleGraphicInfoData.message = reader.ReadChars(middleGraphicInfoData.message.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO3;
                                msgData.middleGraphicInfo = middleGraphicInfoData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_1):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_1;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_2):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_2;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_VALUE):
                            {
                                GuiMiddleEnterValueData_t middleEnterValueData = new GuiMiddleEnterValueData_t();
                                middleEnterValueData.currentLabel = reader.ReadChars(middleEnterValueData.currentLabel.GetUpperBound(0) + 1);
                                middleEnterValueData.newLabel = reader.ReadChars(middleEnterValueData.newLabel.GetUpperBound(0) + 1);
                                middleEnterValueData.currentValue = reader.ReadChars(middleEnterValueData.currentValue.GetUpperBound(0) + 1);
                                middleEnterValueData.newValue = reader.ReadChars(middleEnterValueData.newValue.GetUpperBound(0) + 1);
                                middleEnterValueData.button0Text = reader.ReadChars(middleEnterValueData.button0Text.GetUpperBound(0) + 1);
                                middleEnterValueData.button1Text = reader.ReadChars(middleEnterValueData.button1Text.GetUpperBound(0) + 1);
                                middleEnterValueData.checkButtonText = StreamToTwoCharArray(reader, middleEnterValueData.checkButtonText.GetUpperBound(0) + 1, middleEnterValueData.checkButtonText.GetUpperBound(1) + 1);
                                middleEnterValueData.buttonChecked = reader.ReadChars(middleEnterValueData.buttonChecked.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_VALUE;
                                msgData.middleEnterValue = middleEnterValueData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_VALUE_NEW):
                            {
                                GuiMiddleEnterValueData_t middleEnterValueData = new GuiMiddleEnterValueData_t();
                                middleEnterValueData.currentLabel = reader.ReadChars(middleEnterValueData.currentLabel.GetUpperBound(0) + 1);
                                middleEnterValueData.newLabel = reader.ReadChars(middleEnterValueData.newLabel.GetUpperBound(0) + 1);
                                middleEnterValueData.currentValue = reader.ReadChars(middleEnterValueData.currentValue.GetUpperBound(0) + 1);
                                middleEnterValueData.newValue = reader.ReadChars(middleEnterValueData.newValue.GetUpperBound(0) + 1);
                                middleEnterValueData.button0Text = reader.ReadChars(middleEnterValueData.button0Text.GetUpperBound(0) + 1);
                                middleEnterValueData.button1Text = reader.ReadChars(middleEnterValueData.button1Text.GetUpperBound(0) + 1);
                                middleEnterValueData.checkButtonText = StreamToTwoCharArray(reader, middleEnterValueData.checkButtonText.GetUpperBound(0) + 1, middleEnterValueData.checkButtonText.GetUpperBound(1) + 1);
                                middleEnterValueData.buttonChecked = reader.ReadChars(middleEnterValueData.buttonChecked.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_VALUE_NEW;
                                msgData.middleEnterValue = middleEnterValueData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_UPDATE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_UPDATE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_RANGE):
                            {
                                GuiMiddleEnterRangeData_t middleEnterRangeData = new GuiMiddleEnterRangeData_t();
                                middleEnterRangeData.lowerLabel = reader.ReadChars(middleEnterRangeData.lowerLabel.GetUpperBound(0) + 1);
                                middleEnterRangeData.upperLabel = reader.ReadChars(middleEnterRangeData.upperLabel.GetUpperBound(0) + 1);
                                middleEnterRangeData.lowerValue = reader.ReadSingle();
                                middleEnterRangeData.upperValue = reader.ReadSingle();
                                middleEnterRangeData.lowerDefault = reader.ReadSingle();
                                middleEnterRangeData.upperDefault = reader.ReadSingle();

                                int format = reader.ReadInt32();
                                switch (format)
                                {
                                    case (int)CommonDefines.NumericFormat_t.Xxxx:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.Xxxx;
                                            break;
                                        }
                                    case (int)CommonDefines.NumericFormat_t.Xxx:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.Xxx;
                                            break;
                                        }
                                    case (int)CommonDefines.NumericFormat_t.Xx:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.Xx;
                                            break;
                                        }
                                    case (int)CommonDefines.NumericFormat_t.X:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.X;
                                            break;
                                        }
                                    case (int)CommonDefines.NumericFormat_t.X0:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.X0;
                                            break;
                                        }
                                    case (int)CommonDefines.NumericFormat_t.X00:
                                        {
                                            middleEnterRangeData.format = CommonDefines.NumericFormat_t.X00;
                                            break;
                                        }
                                }

                                middleEnterRangeData.buttonText = StreamToTwoCharArray(reader, middleEnterRangeData.buttonText.GetUpperBound(0) + 1, middleEnterRangeData.buttonText.GetUpperBound(1) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_RANGE;
                                msgData.middleEnterRange = middleEnterRangeData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_SPLASH):
                            {
                                //startup gui process
                                GuiSplashScreenData_t splashScreenData = new GuiSplashScreenData_t();
                                splashScreenData.productCode = reader.ReadChars(splashScreenData.productCode.GetUpperBound(0) + 1);
                                splashScreenData.version = reader.ReadChars(splashScreenData.version.GetUpperBound(0) + 1);
                                splashScreenData.progBarText = reader.ReadChars(splashScreenData.progBarText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_SPLASH;
                                msgData.winSplashScreen = splashScreenData;
                                msg.data = msgData;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_ERRORINFO):
                            {
                                //startup gui process
                                GuiWinErrorData_t winErrorData = new GuiWinErrorData_t();

                                winErrorData.topLabel = reader.ReadChars(winErrorData.topLabel.GetUpperBound(0) + 1);
                                winErrorData.message = reader.ReadChars(winErrorData.message.GetUpperBound(0) + 1);
                                winErrorData.message = AddCRLF(winErrorData.message);
                                winErrorData.buttonText = reader.ReadChars(winErrorData.buttonText.GetUpperBound(0) + 1);

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_ERRORINFO;
                                msgData.winError = winErrorData;
                                msg.data = msgData;
                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SOUND):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SOUND;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CRITICAL_STOP):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CRITICAL_STOP;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_ERROR):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_ERROR;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_WARNING):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_WARNING;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_PRINTING_DONE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_PRINTING_DONE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CLICK):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CLICK;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_ALERT_VOLUME):
                            {
                                GuiSoundData_t soundData = new GuiSoundData_t();
                                soundData.volume = reader.ReadByte();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_ALERT_VOLUME;
                                msgData.soundData = soundData;
                                msg.data = msgData;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_STATUS_VOLUME):
                            {
                                GuiSoundData_t soundData = new GuiSoundData_t();
                                soundData.volume = reader.ReadByte();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_STATUS_VOLUME;
                                msgData.soundData = soundData;
                                msg.data = msgData;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_CLICK_VOLUME):
                            {
                                GuiSoundData_t soundData = new GuiSoundData_t();
                                soundData.volume = reader.ReadByte();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_CLICK_VOLUME;
                                msgData.soundData = soundData;
                                msg.data = msgData;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SOUND):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SOUND;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SCREENSAVE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SCREENSAVE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_BUTTON_PRESS):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_BUTTON_PRESS;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_DISABLE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_DISABLE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_ENABLE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_ENABLE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_WAKEUP):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_WAKEUP;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SCREENSAVE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SCREENSAVE;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_LANGUAGE):
                            {
                                GuiLanguageData_t langData = new GuiLanguageData_t();
                                langData.language = reader.ReadInt32();

                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_LANGUAGE;
                                msgData.languageData = langData;
                                msg.data = msgData;

                                break;
                            }
                        case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_ENABLE_MOUSE):
                            {
                                msg.msgId = (int)GuiMessages.GuiMsgId_t.MSG_GUI_ENABLE_MOUSE;
                                break;
                            }
                        default:
                            {
                                SendMessageToQueue.SendLogErrorMessage("Unknown Message Id Received in Utilities.cs. MsgId  " + msgId.ToString());
                                return null;
                                break;
                            }
                    }

                    //log the message Id received by the Gui
                    //SendMessageToQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Received MsgId = " + msgId.ToString());

                }//end of using memory stream
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Message ID  " + msgId.ToString() + " Unable to Translate the received message byte array to Gui specific structure in Utilities.cs");
            }

            return (msg);
        }

        /// <summary>
        /// Adds \r before every \n in the character array in order to display a carriage return and line feed
        /// </summary>
        /// <param name="message">original character array from j2Console</param>
        /// <returns>a character array with \r\n inserted</returns>
        private char[] AddCRLF(char[] message)
        {
            List<char> temp = new List<char>();

            for (int c = 0; c < message.GetUpperBound(0) + 1; c++)
            {
                //for every \n found, replace it with \r\n since in QNX \n could do the job but
                //in windows, \n alone without \r prints a non printable character.
                if (message[c] == '\n')
                {
                    temp.Add('\r');
                }

                temp.Add(message[c]);
            }

            return (temp.ToArray());
        }
    }
}
