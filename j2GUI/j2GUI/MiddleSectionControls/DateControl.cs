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
    /// DATE CONTROL
    /// This middle sub section has controls to enter the day, month and the year. The text boxes get the data from the other process. Once the
    /// data is set here, the receiving process will set the system date.
    /// </summary>
    public partial class DateControl : UserControl
    {
        MiddleSectionDate DateDefines = new MiddleSectionDate();
        private Messaging.SendQueue SendMessageToQueue;
        private Utilities Util = new Utilities();
        private MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private Timer RepeatTimer = new Timer();
        private bool IsUpPressed = true;       
        private int UserTag;

        public DateControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnDayUp.Text = "";
            btnDayDown.Text = "";
            btnMonthUp.Text = "";
            btnMonthDown.Text = "";
            btnYearUp.Text = "";
            btnYearDown.Text = "";

            //set up the aesthetics of the user controls
            //Day
            btnDayUp.ImageDefault = Properties.Resources._120x75_scroll_up_normal;
            btnDayUp.ImagePressed = Properties.Resources._120x75_scroll_up_down;
            btnDayDown.ImageDefault = Properties.Resources._120x75_scroll_down_normal;
            btnDayDown.ImagePressed = Properties.Resources._120x75_scroll_down_down;

            //Month
            btnMonthUp.ImageDefault = Properties.Resources._120x75_scroll_up_normal;
            btnMonthUp.ImagePressed = Properties.Resources._120x75_scroll_up_down;
            btnMonthDown.ImageDefault = Properties.Resources._120x75_scroll_down_normal;
            btnMonthDown.ImagePressed = Properties.Resources._120x75_scroll_down_down;

            //Year
            btnYearUp.ImageDefault = Properties.Resources._120x75_scroll_up_normal;
            btnYearUp.ImagePressed = Properties.Resources._120x75_scroll_up_down;
            btnYearDown.ImageDefault = Properties.Resources._120x75_scroll_down_normal;
            btnYearDown.ImagePressed = Properties.Resources._120x75_scroll_down_down;

            lblDay.Font = DateDefines.Font;
            lblDay.ForeColor = DateDefines.FontColor;

            txtDay.Font = DateDefines.TextBoxFont;
            txtDay.ForeColor = DateDefines.FontColor;

            lblMonth.Font = DateDefines.Font;
            lblMonth.ForeColor = DateDefines.FontColor;

            txtMonth.Font = DateDefines.TextBoxFont;
            txtMonth.ForeColor = DateDefines.FontColor;

            lblYear.Font = DateDefines.Font;
            lblYear.ForeColor = DateDefines.FontColor;

            txtYear.Font = DateDefines.TextBoxFont;
            txtYear.ForeColor = DateDefines.FontColor;

            //button at the bottom of the middle section user control
            imageButton0.Text = "";
            imageButton0.ImageDefault = Properties.Resources._442x75_normal;
            imageButton0.ImagePressed = Properties.Resources._442x75_down;
            imageButton0.Font = ButtonDefines.Font;
            imageButton0.ForeColor = ButtonDefines.NormalFontColor;
            imageButton0.PressedForeColor = ButtonDefines.PressedFontColor;
        }

        /// <summary>
        /// Sets the user controls from the data received by the Gui message
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetDateControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_DATE):
                        {
                            lblDay.Text = new string(messageData.middleEnterDate.labelLeft);
                            lblMonth.Text = new string(messageData.middleEnterDate.labelMiddle);
                            lblYear.Text = new string(messageData.middleEnterDate.labelRight);

                            txtDay.Text = new string(messageData.middleEnterDate.textLeft);
                            txtMonth.Text = new string(messageData.middleEnterDate.textMiddle);
                            txtYear.Text = new string(messageData.middleEnterDate.textRight);

                            string buttonText = Util.OneCharArrayToString(messageData.middleEnterDate.buttonText);

                            imageButton0.Visible = false;//added on 10/2/2012 bug fix

                            //make this button visible only if the text content is not empty
                            if ( buttonText != string.Empty)
                            {
                                imageButton0.Visible = true;
                                imageButton0.Text = new string(messageData.middleEnterDate.buttonText);
                            }

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the date in DateControl.cs");
            }
        }      

        /// <summary>
        /// User clicked on the button at the bottom of the middle sub section
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton0_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }

        /// <summary>
        /// Handles all the update messages to the Gui queue
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void UpdateDateControl(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_LEFT):
                        {
                            txtDay.Text = new string(messageData.middleEnterDate.textLeft);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_MIDDLE):
                        {
                            txtMonth.Text = new string(messageData.middleEnterDate.textMiddle);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_RIGHT):
                        {
                            txtYear.Text = new string(messageData.middleEnterDate.textRight);
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the date in DateControl.cs");
            }
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

        /// <summary>
        /// Called when the Day Up or Month Up or Year Up button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;//disable the timer

            //this will be called if the click on the button is quicker than the timer tick to trigger ie when the user wants 
            //to just increment by one and not continuously            
               SendMessageToQueue.SendUpMessage(sender);
            
        }
        /// <summary>
        /// Called when the Day Up or Month Up or Year Up button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        {            
            RepeatTimer.Enabled = true;
            IsUpPressed = true;//an up button is clicked

            UserTag = SendMessageToQueue.GetTagData(sender);//get the tag of the button clicked. It can be either Day or Month or Year button
        }
        /// <summary>
        /// Called when the Day Down or Month Down or Year Down button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            SendMessageToQueue.SendDownMessage(sender);
        }

        /// <summary>
        /// Called when the Day Down or Month Down or Year Down button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = false;                                                                                     
            UserTag = SendMessageToQueue.GetTagData(sender);     
        }    
    }
}
