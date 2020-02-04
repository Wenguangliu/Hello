using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>  
    /// VIEW CONTROLS
    /// This middle sub section has controls which provides information to the users to view
    /// </summary>
    public partial class ViewControl : UserControl
    {
        private MiddleMultiTextRD MultiTextRDDefines;
        private Timer RepeatTimer = new Timer();//timer thread used for repeated scrolling
        private bool IsUpPressed = true;
        private Messaging.SendQueue SendMessageToQueue;
        private Sound PlaySound;

        public ViewControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            MultiTextRDDefines = new MiddleMultiTextRD();
            PlaySound = new Sound();

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    

            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnUp.Text = "";
            btnDown.Text = "";

            txtViewResults.Font = MultiTextRDDefines.Font;
            txtViewResults.ForeColor = MultiTextRDDefines.FontColor;
        }

        /// <summary>
        /// Sets the middle sub section to display the view control
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetViewControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS):
                        {
                            txtViewResults.Font = MultiTextRDDefines.Font;
                            txtViewResults.Text = new string(messagedata.middleViewResults.message);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT):
                        {
                            //added nwr 5/14/2013
                            txtViewResults.Font = MultiTextRDDefines.FontSmall;
                            txtViewResults.Text = new string(messagedata.middleViewResults.message);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT):
                        {
                            //added nwr 5/14/2013
                            txtViewResults.Font = MultiTextRDDefines.FontMiddle;
                            txtViewResults.Text = new string(messagedata.middleViewResults.message);

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the view in ViewControl.cs");
            }
        }

        /// <summary>
        /// When the user clicks on the text box or when the text box automatically gets focus, shift the focus
        /// to the up button instead of the text box control
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void touchTextBox1_GotFocus(object sender, EventArgs e)
        {
            btnUp.Focus();
        }

        /// <summary>
        /// Call the win32 api to scroll up the text box control
        /// </summary>
        private void ScrollUp()
        {
            Native.ScrollUp(txtViewResults);
        }

        /// <summary>
        /// Call the win32 api to scroll down the text box control
        /// </summary>
        private void ScrollDown()
        {
            Native.ScrollDown(txtViewResults);
        }

        /// <summary>
        /// Handles mouse up event for Up button. Disable the timer and call Scroll Up
        /// method
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            ScrollUp();
        }

        /// <summary>
        /// Handles mouse Down event for Up button. Enable the timer and set the IsUpPressed
        /// flag indicating that the timer tick event is being called due to user action on button
        /// Up
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = true;//an up button is clicked               
            PlaySound.Click();
        }

        /// <summary>
        /// Handles mouse up event for Down button. Disable the timer and call Scroll Down
        /// method
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            ScrollDown();
        }

        /// <summary>
        /// Handles mouse Down event for Down button. Enable the timer and reset the IsUpPressed
        /// flag indicating that the timer tick event is being called due to user action on button
        /// Down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = false;//an up button is clicked   
            PlaySound.Click();
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
                ScrollUp();
            }
            else
            {
                ScrollDown();
            }
        }
    }
}
