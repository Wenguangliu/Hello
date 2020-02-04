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
    public partial class LoadOrderControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private MiddleSectionProgressBar ProgressBarDefines = new MiddleSectionProgressBar();
        private Messaging.SendQueue SendMessageToQueue;

        public LoadOrderControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            lblRotorName.Text = "";
            lblOrderLabel.Text = "";

            lblRotorName.Font = ProgressBarDefines.Font;

            //set the default properties for ALL the buttons. Even the buttons which are invisible to keep the coding neat
            btnLoadOrder.Text = "";//remove the default text
            btnLoadOrder.ImageDefault = Properties.Resources._442x75_normal;
            btnLoadOrder.ImagePressed = Properties.Resources._442x75_down;
            btnLoadOrder.Font = ButtonDefines.Font;
            btnLoadOrder.ForeColor = ButtonDefines.NormalFontColor;
            btnLoadOrder.PressedForeColor = ButtonDefines.PressedFontColor;
            btnLoadOrder.Visible = false;//make all the buttons invisible to begin with            
        }

        internal void SetLoadOrderControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;
                btnLoadOrder.Visible = false;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LOAD_ORDER):
                        {
                            lblOrderLabel.Text = new string(messageData.middleLoadOrder.orderLabel);

                            lblRotorName.Text = new string(messageData.middleLoadOrder.rotorName);
                            lblRotorName.ForeColor = Color.FromArgb((int)messageData.middleLoadOrder.fontColor);
                            if ((int)messageData.middleLoadOrder.backgroundColor == 0x00FFFFFF)//White color background for Rotor 1 was showing as transparent color since the Console sends the Color as 0x00FFFFFF instead of 0xFFFFFFFF(ARGB - Alpha, red, green, blue)
                            {
                                lblRotorName.BackColor = Color.White;
                            }
                            else
                            {
                                lblRotorName.BackColor = Color.FromArgb((int)messageData.middleLoadOrder.backgroundColor);
                            }

                            lblRotorName.Visible = true;
                           
                            string buttonText = Util.OneCharArrayToString(messageData.middleLoadOrder.buttonText);

                            btnLoadOrder.Text = buttonText;
                            if (buttonText != string.Empty)
                            {
                                btnLoadOrder.Visible = true;

                            }

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the load order controls in LoadOrderControl.cs");
            }
        }

        private void btnLoadOrder_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }
    }
}
