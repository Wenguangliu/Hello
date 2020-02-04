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

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// MESSAGE + INFO CONTROL
    /// This middle sub section has a message control and a info button control. The info button control becomes visible or not 
    /// depending on the message Id received from the message queue.
    /// </summary>
    public partial class MessageInfoControl : UserControl
    {
        private MiddleSectionMessage MessageDefines = new MiddleSectionMessage();
        private Messaging.SendQueue SendMessageToQueue;

        public MessageInfoControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnInfo.Text = "";//clear the button image text
            btnInfo.ImageDefault = Properties.Resources.Info_Icon;

            lblMessage.Font = MessageDefines.Font;
            lblMessage.ForeColor = MessageDefines.FontColor;
        }

        /// <summary>
        /// Sets the middle sub section user control with the correct set of controls
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetMessageInfoControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE_INFO):
                        {                            
                            lblMessage.Text = new string(messageData.middleMessage.message);
                            btnInfo.Visible = true;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE):
                        {                           
                            lblMessage.Text = new string(messageData.middleMessage.message);
                            btnInfo.Visible = false;
                           
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in SetMessageInfoControl.cs");
            }
        }

        /// <summary>
        /// User has clicked on the Info button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnInfo_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendInfoMessage();            
        }       
    }
}
