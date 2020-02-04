using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    public partial class CuvetteControl : UserControl
    {
        private MiddleSectionCuvette CuvetteDefines;
        private Messaging.SendQueue SendMessageToQueue;

        public CuvetteControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;
            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            CuvetteDefines = new MiddleSectionCuvette();

            txtCuvetteTestStatus.Text = "";
            txtCuvetteTestStatus.Font = CuvetteDefines.TextBoxFontSmall;
            txtCuvetteTestStatus.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtCurrentDAC.Text = "";
            txtCurrentDAC.Font = CuvetteDefines.TextBoxFontSmall;
            txtCurrentDAC.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtThresholdDAC.Text = "";
            txtThresholdDAC.Font = CuvetteDefines.TextBoxFontSmall;
            txtThresholdDAC.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtTotalCuvNum.Text = "";
            txtTotalCuvNum.Font = CuvetteDefines.TextBoxFontSmall;
            txtTotalCuvNum.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtIndexCuvWidth.Text = "";
            txtIndexCuvWidth.Font = CuvetteDefines.TextBoxFontSmall;
            txtIndexCuvWidth.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtMaxCuvWidth.Text = "";
            txtMaxCuvWidth.Font = CuvetteDefines.TextBoxFontSmall;
            txtMaxCuvWidth.ForeColor = CuvetteDefines.TextBoxFontColor;

            txtMinCuvWidth.Text = "";
            txtMinCuvWidth.Font = CuvetteDefines.TextBoxFontSmall;
            txtMinCuvWidth.ForeColor = CuvetteDefines.TextBoxFontColor;

            lblCurrentDAC.Font = CuvetteDefines.LabelFontSmall;
            lblCurrentDAC.ForeColor = CuvetteDefines.LabelFontColor;

            lblThresholdDAC.Font = CuvetteDefines.LabelFontSmall;
            lblThresholdDAC.ForeColor = CuvetteDefines.LabelFontColor;

            lblTotalCuvNum.Font = CuvetteDefines.LabelFontSmall;
            lblTotalCuvNum.ForeColor = CuvetteDefines.LabelFontColor;

            lblIndexCuvWidth.Font = CuvetteDefines.LabelFontSmall;
            lblIndexCuvWidth.ForeColor = CuvetteDefines.LabelFontColor;

            lblMaxCuvWidth.Font = CuvetteDefines.LabelFontSmall;
            lblMaxCuvWidth.ForeColor = CuvetteDefines.LabelFontColor;

            lblMinCuvWidth.Font = CuvetteDefines.LabelFontSmall;
            lblMinCuvWidth.ForeColor = CuvetteDefines.LabelFontColor;
        }

        /// <summary>
        /// Set the Cuvette control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetCuvetteControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_CUVETTE_DATA):
                        {
                            txtCuvetteTestStatus.Text = new string(messageData.middleCuvette.resultsText);

                            txtCurrentDAC.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 0));
                            txtThresholdDAC.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 1));
                            txtTotalCuvNum.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 2));
                            txtIndexCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 3));
                            txtMaxCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 4));
                            txtMinCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 5));

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in CuvetteControl.cs");
            }
        }

        /// <summary>
        /// Returns a row in the valueText 2D array.
        /// </summary>
        /// <param name="valueText"></param>
        /// <param name="Idx"></param>
        /// <returns></returns>
        private char[] GetValueTextRow(char[,] valueText, int row)
        {
            char[] tempArray = new char[valueText.GetUpperBound(1) + 1];
            for (int i = 0; i < valueText.GetUpperBound(1) + 1; i++)
            {
                tempArray[i] = valueText[row, i];
            }

            return tempArray;
        }

        /// <summary>
        /// Update the Cuvette control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void UpdateCuvetteControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_CUVETTE_DATA):
                        {
                            if (messageData.middleCuvette.resultsText.Sum(total => total) > 0)
                            {
                                txtCuvetteTestStatus.Text = new string(messageData.middleCuvette.resultsText);
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 0).Sum(total => total) > 0)
                            {
                                txtCurrentDAC.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 0));
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 1).Sum(total => total) > 0)
                            {
                                txtThresholdDAC.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 1));
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 2).Sum(total => total) > 0)
                            {
                                txtTotalCuvNum.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 2));
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 3).Sum(total => total) > 0)
                            {
                                txtIndexCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 3));
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 4).Sum(total => total) > 0)
                            {
                                txtMaxCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 4));
                            }

                            if (GetValueTextRow(messageData.middleCuvette.valueText, 5).Sum(total => total) > 0)
                            {
                                txtMinCuvWidth.Text = new string(GetValueTextRow(messageData.middleCuvette.valueText, 5));
                            }

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the controls in CuvetteControl.cs");
            }
        }
    }
}
