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
    public partial class ListColoredControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private Messaging.SendQueue SendMessageToQueue;
        private Timer RepeatTimer = new Timer();
        private bool IsUpPressed = true;
        private int UserTag;

        public ListColoredControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            //initialize the controls
            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnUp.Text = "";
            btnDown.Text = "";

            btnUp.Visible = true;
            btnDown.Visible = true;
        }

        internal void SetListColoredControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;
                ClearButtons();

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_COLORED):
                        {
                            imageButton0.ForeColor = Color.FromArgb((int)messageData.middleListColored.fontColor[0]);
                            if ((int)messageData.middleListColored.backgroundColor[0] == 0x00FFFFFF)//White color background for Rotor 1 was showing as transparent color since the Console sends the Color as 0x00FFFFFF instead of 0xFFFFFFFF(ARGB - Alpha, red, green, blue)
                            {
                                imageButton0.BackColor = Color.White;
                            }
                            else
                            {
                                imageButton0.BackColor = Color.FromArgb((int)messageData.middleListColored.backgroundColor[0]);
                            }

                            imageButton1.ForeColor = Color.FromArgb((int)messageData.middleListColored.fontColor[1]);
                            if ((int)messageData.middleListColored.backgroundColor[1] == 0x00FFFFFF)
                            {
                                imageButton1.BackColor = Color.White;
                            }
                            else
                            {
                                imageButton1.BackColor = Color.FromArgb((int)messageData.middleListColored.backgroundColor[1]);
                            }

                            imageButton2.ForeColor = Color.FromArgb((int)messageData.middleListColored.fontColor[2]);
                            if ((int)messageData.middleListColored.backgroundColor[2] == 0x00FFFFFF)
                            {
                                imageButton2.BackColor = Color.White;
                            }
                            else
                            {
                                imageButton2.BackColor = Color.FromArgb((int)messageData.middleListColored.backgroundColor[2]);
                            }

                            imageButton3.ForeColor = Color.FromArgb((int)messageData.middleListColored.fontColor[3]);
                            if ((int)messageData.middleListColored.backgroundColor[3] == 0x00FFFFFF)
                            {
                                imageButton3.BackColor = Color.White;
                            }
                            else
                            {
                                imageButton3.BackColor = Color.FromArgb((int)messageData.middleListColored.backgroundColor[3]);
                            }

                            imageButton4.ForeColor = Color.FromArgb((int)messageData.middleListColored.fontColor[4]);
                            if ((int)messageData.middleListColored.backgroundColor[4] == 0x00FFFFFF)
                            {
                                imageButton4.BackColor = Color.White;
                            }
                            else
                            {
                                imageButton4.BackColor = Color.FromArgb((int)messageData.middleListColored.backgroundColor[4]);
                            }

                            string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleListColored.items);

                            imageButton0.Text = buttonText[0];
                            imageButton1.Text = buttonText[1];
                            imageButton2.Text = buttonText[2];
                            imageButton3.Text = buttonText[3];
                            imageButton4.Text = buttonText[4];

                            imageButton0.Font = ButtonDefines.Font;
                            imageButton1.Font = ButtonDefines.Font;
                            imageButton2.Font = ButtonDefines.Font;
                            imageButton3.Font = ButtonDefines.Font;
                            imageButton4.Font = ButtonDefines.Font;
                           
                            //make only those buttons visible                            
                            if (buttonText[0] != string.Empty)
                            {
                                imageButton0.Visible = true;
                            }

                            if (buttonText[1] != string.Empty)
                            {
                                imageButton1.Visible = true;
                            }

                            if (buttonText[2] != string.Empty)
                            {
                                imageButton2.Visible = true;
                            }

                            if (buttonText[3] != string.Empty)
                            {
                                imageButton3.Visible = true;
                            }

                            if (buttonText[4] != string.Empty)
                            {
                                imageButton4.Visible = true;
                            }
                            
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the list colored control in ListColoredControl.cs");
            }
        }

        /// <summary>
        /// Make the buttons invisible to begin with
        /// </summary>
        private void ClearButtons()
        {
            imageButton0.Visible = false;
            imageButton1.Visible = false;
            imageButton2.Visible = false;
            imageButton3.Visible = false;
            imageButton4.Visible = false;            
        }

        private void button_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }

        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = true;//an up button is clicked

            UserTag = SendMessageToQueue.GetTagData(sender);//get the tag of the button clicked. It can be either Day or Month or Year button
        }

        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            //this will be called if the click on the button is quicker than the timer tick to trigger ie when the user wants 
            //to just increment by one and not continuously
            SendMessageToQueue.SendUpMessage(sender);
        }

        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = false;

            UserTag = SendMessageToQueue.GetTagData(sender);
        }

        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;
            SendMessageToQueue.SendDownMessage(sender);
        }

        /// <summary>
        /// Timer event handler gets called when the user has pressed a up or a down button continuously
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RepeatTimer_Tick(object sender, EventArgs e)
        {
            if (IsUpPressed)
            {
                SendMessageToQueue.SendUpMessage(UserTag);
            }
            else
            {
                SendMessageToQueue.SendDownMessage(UserTag);
            }
        }
    }
}
