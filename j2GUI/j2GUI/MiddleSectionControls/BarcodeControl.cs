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
    public partial class BarcodeControl : UserControl
    {
        private Messaging.SendQueue SendMessageToQueue;
        private MiddleSectionButton ButtonDefines;
        private MiddleSectionEnterValueBarcode BarcodeDefines;

        public BarcodeControl()
        {
            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            ButtonDefines = new MiddleSectionButton();
            BarcodeDefines = new MiddleSectionEnterValueBarcode();

            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            txtResult.Text = "";
            txtResult.Font = BarcodeDefines.TextBoxFontSmall;
            txtResult.ForeColor = BarcodeDefines.TextBoxFontColor;

            lblCurrent.Font = BarcodeDefines.LabelFont;
            lblCurrent.ForeColor = BarcodeDefines.LabelFontColor;

            txtCurrent.Text = "";
            txtCurrent.Font = BarcodeDefines.TextBoxFont;
            txtCurrent.ForeColor = BarcodeDefines.TextBoxFontColor;

            btnUp.Text = "";
            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;
            btnUp.Font = ButtonDefines.Font;
            btnUp.ForeColor = ButtonDefines.NormalFontColor;
            btnUp.PressedForeColor = ButtonDefines.PressedFontColor;

            btnDown.Text = "";
            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;
            btnDown.Font = ButtonDefines.Font;
            btnDown.ForeColor = ButtonDefines.NormalFontColor;
            btnDown.PressedForeColor = ButtonDefines.PressedFontColor;

            btnCalibration.Text = "";
            btnCalibration.Font = ButtonDefines.Font;
            btnCalibration.ForeColor = ButtonDefines.NormalFontColor;
            btnCalibration.PressedForeColor = ButtonDefines.PressedFontColor;
            btnCalibration.ImageDefault = Properties.Resources._213x75_normal;
            btnCalibration.ImagePressed = Properties.Resources._213x75_down;

            btnOpen.Text = "";
            btnOpen.Font = ButtonDefines.Font;
            btnOpen.ForeColor = ButtonDefines.NormalFontColor;
            btnOpen.PressedForeColor = ButtonDefines.PressedFontColor;
            btnOpen.ImageDefault = Properties.Resources._213x75_normal;
            btnOpen.ImagePressed = Properties.Resources._213x75_down;

            btnRead.Text = "";
            btnRead.Font = ButtonDefines.Font;
            btnRead.ForeColor = ButtonDefines.NormalFontColor;
            btnRead.PressedForeColor = ButtonDefines.PressedFontColor;
            btnRead.ImageDefault = Properties.Resources._213x75_normal;
            btnRead.ImagePressed = Properties.Resources._213x75_down;

        }

        /// <summary>
        /// Send the button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Button_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }

        /// <summary>
        /// Send the up button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UpButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendUpMessage(sender);
        }

        /// <summary>
        /// Send the down button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DownButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendDownMessage(sender);
        }

        /// <summary>
        /// Set the Barcode control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetBarcodeControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BARCODE_DATA):
                        {
                            txtResult.Text = new string(messageData.middleBarcode.resultsText);
                            txtCurrent.Text = new string(messageData.middleBarcode.valueText);
                            btnCalibration.Text = new string(messageData.middleBarcode.middleBotton0);
                            btnRead.Text = new string(messageData.middleBarcode.middleBotton1);
                            btnOpen.Text = new string(messageData.middleBarcode.middleBotton2);
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in MotorControl.cs");
            }
        }

        /// <summary>
        /// Update the Barcode control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void UpdateBarcodeControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_BARCODE_DATA):
                        {
                            if (messageData.middleBarcode.resultsText.Sum(total => total) > 0)
                            {
                                txtResult.Text = new string(messageData.middleBarcode.resultsText);
                            }

                            if (messageData.middleBarcode.valueText.Sum(total => total) > 0)
                            {
                                txtCurrent.Text = new string(messageData.middleBarcode.valueText);
                            }

                            if (messageData.middleBarcode.middleBotton0.Sum(total => total) > 0)
                            {
                                btnCalibration.Text = new string(messageData.middleBarcode.middleBotton0);
                            }

                            if (messageData.middleBarcode.middleBotton1.Sum(total => total) > 0)
                            {
                                btnRead.Text = new string(messageData.middleBarcode.middleBotton1);
                            }

                            if (messageData.middleBarcode.middleBotton2.Sum(total => total) > 0)
                            {
                                btnOpen.Text = new string(messageData.middleBarcode.middleBotton2);
                            }
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the controls in BarcodeControl.cs");
            }
        }
    }
}
