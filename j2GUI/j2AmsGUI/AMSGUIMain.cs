using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;
using Resco.Controls.OutlookControls;
using Resco.Controls.CommonControls;

namespace j2AMSGUI
{
    public partial class AMSGUIMain : UserControl
    {

        public static event j2AMSGUI.j2Startup.UpdateEventHandler UpdatingDisplay;

        private Utilities Util;

        //j2 GUI formatting.
        TopGuiDefines topGUIDefines = new TopGuiDefines();
        BottomGuiDefines bottomGUIDefines = new BottomGuiDefines();
        MiddleSectionMessage middleSectionMessage = new MiddleSectionMessage();
        MiddleSectionButton middleSectionButton = new MiddleSectionButton();

        private Messaging.SendQueue SendMessageToQueue;

        public AMSGUIMain()
        {
            InitializeComponent();

            SendMessageToQueue = new j2AMSGUI.Messaging.SendQueue();

            Util = new Utilities();

            //set default form properties.
            this.Height = GuiForm.FormHeight;
            this.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);

            //set info button image properties
            btnCalibration.ImageDefault = Properties.Resources._213x75_normal;
            btnCalibration.Font = middleSectionMessage.Font;
            btnCalibration.ForeColor = middleSectionMessage.FontColor;

            btnHeaterTest.ImageDefault = Properties.Resources._213x75_normal;
            btnHeaterTest.Font = middleSectionMessage.Font;
            btnHeaterTest.ForeColor = middleSectionMessage.FontColor;

            btnDrawerTest.ImageDefault = Properties.Resources._213x75_normal;
            btnDrawerTest.Font = middleSectionMessage.Font;
            btnDrawerTest.ForeColor = middleSectionMessage.FontColor;

            btnOpen.ImageDefault = Properties.Resources._213x75_normal;
            btnOpen.Font = middleSectionMessage.Font;
            btnOpen.ForeColor = middleSectionMessage.FontColor;

            btnClose.ImageDefault = Properties.Resources._213x75_normal;
            btnClose.Font = middleSectionMessage.Font;
            btnClose.ForeColor = middleSectionMessage.FontColor;

            btnCancel.ImageDefault = Properties.Resources._213x75_normal;
            btnCancel.Font = middleSectionMessage.Font;
            btnCancel.ForeColor = middleSectionMessage.FontColor;

            btnExit.BackColor = bottomGUIDefines.NormalStartButtonColor;
            btnExit.Font = bottomGUIDefines.Font;
            btnExit.ForeColor = bottomGUIDefines.NormalFontColor;
            btnExit.PressedForeColor = bottomGUIDefines.PressedFontColor;
            btnExit.BorderStyle = Resco.Controls.RescoBorderStyle.FixedSingle;
            btnExit.FocusedBorderColor = Color.Transparent;
            btnExit.BorderCornerSize = 0;
            btnExit.VistaButtonInflate = new Size(-1, -1);

            //Top title bar
            lblTopTitle.BackColor = topGUIDefines.BackColor;
            lblTopTitle.Font = topGUIDefines.Font;
            lblTopTitle.ForeColor = topGUIDefines.FontColor;
            lblTopTitle.Text = "AMS Test";

            //Initialize labels.
            lblErrorCode.Text = string.Empty; //"Error Code";
            lblMessage.Text = "Rotor Chamber is empty";
        }

        /// <summary>
        /// Loads AMS status and error codes received from AMS.
        /// </summary>
        /// <param name="msg">msg received from the queue</param>
        public void LoadAMSGUI(AmsGuiMsg_t msg)
        {
            try
            {
                switch (msg.msgId)
                {
                    case (int)AmsMessages.AmsMsgId_t.MSG_AMS_STATUS:

                        //status                        
                        lblMessage.Text = AmsMessages.amsProcessStatus[(int)msg.amsStatus];
                        break;
                    case (int)AmsMessages.AmsMsgId_t.MSG_AMS_ERROR_CODE:

                        //error                            
                        lblErrorCode.Text = "0x" + msg.errorCode.ToString("X2");

                        //debug
                        //System.Diagnostics.Debug.Write("msg size: " + System.Runtime.InteropServices.Marshal.SizeOf(msg).ToString() + Environment.NewLine);
                        //System.Diagnostics.Debug.Write("msg id size: " + System.Runtime.InteropServices.Marshal.SizeOf(msg.msgId).ToString() + Environment.NewLine);
                        //System.Diagnostics.Debug.Write("ams status size: " + System.Runtime.InteropServices.Marshal.SizeOf(msg.amsStatus).ToString() + Environment.NewLine);
                        //System.Diagnostics.Debug.Write("error code size: " + System.Runtime.InteropServices.Marshal.SizeOf(msg.errorCode).ToString() + Environment.NewLine);

                        break;
                }
            }
            catch //TODO (System.Exception e)
            {
                SendMessageToQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to Load Window Error Screen in WinMessageControls.cs");
            }
        }

        /// <summary>
        /// Exit AMS.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnExit_Click(object sender, EventArgs e)
        {
           // Application.Exit();
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_AMS_EXIT;
            msg.data.engineMsg.length = 1;

            SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        /// <summary>
        /// Open analyzer drawer
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOpen_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'D';
            msg.data.engineMsg.message[1] = 'O';
            msg.data.engineMsg.length = 3;

            SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        private void btnCalibration_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'C';
            msg.data.engineMsg.length = 2;

            SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'D';
            msg.data.engineMsg.message[1] = 'M';
            msg.data.engineMsg.length = 3;

            SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'A';
            msg.data.engineMsg.message[1] = 'C';
            msg.data.engineMsg.length = 3;

            SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        private void btnHeaterTest_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'H';
            msg.data.engineMsg.length = 2;

            //TODO SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }

        private void btnDrawerTest_Click(object sender, EventArgs e)
        {
            EngineCommReceiveMsg_t msg = new EngineCommReceiveMsg_t();
            msg.msgId = (int)CppCsWrapper.AmsMessages.AmsMsgId_t.MSG_CONSOLE_RX_READY;
            msg.data.engineMsg.message[0] = 'R';
            msg.data.engineMsg.length = 2;

            //TODO SendMessageToQueue.SendMessage(new string(CppCsWrapper.AmsMessages.AMS_QUEUE_NAME), msg);
        }
    }
}
