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
    /// <summary>  
    /// TIME CONTROLS
    /// This middle sub section has controls to set time on the system. The time is set here and also in J2Console so that the text box on the time
    /// sub section can show real time time.The actual date and time is set in J2Console code. We set the time here also in order to show real time 
    /// when the user is viewing this screen
    /// </summary>
    public partial class TimeControl : UserControl
    {
        MiddleSectionTime TimeDefines = new MiddleSectionTime();
        private Messaging.SendQueue SendMessageToQueue;
        private System.Windows.Forms.Timer TimerTime;
        private int PreviousHour;
        private int PreviousMinute;
        private int PreviousSecond;
        private char TwentyFourHourFormat;
        private Native.SystemTime CurrentSystemTime;
        private Sound PlaySound;

        public TimeControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            PlaySound = new Sound();
            CurrentSystemTime = new Native.SystemTime();
            TimerTime = new System.Windows.Forms.Timer();            

            //update the time displayed to the user every one sec
            TimerTime.Interval = 1000;// 1000 milliseconds
            TimerTime.Tick += new System.EventHandler(TimerTime_Tick);

            btnHourUp.Text = "";
            btnHourDown.Text = "";
            btnMinuteUp.Text = "";
            btnMinuteDown.Text = "";

            btnHourUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnHourUp.ImagePressed = Properties.Resources._213x75_scrollup_down;
            btnHourDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnHourDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnMinuteUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnMinuteUp.ImagePressed = Properties.Resources._213x75_scrollup_down;
            btnMinuteDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnMinuteDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnHourFormat.Text = "";
            btnZeroSecond.Text = "";

            btnHourFormat.ImageDefault = Properties.Resources._213x75_normal;
            btnHourFormat.ImagePressed = Properties.Resources._213x75_down;

            btnZeroSecond.ImageDefault = Properties.Resources._213x75_normal;
            btnZeroSecond.ImagePressed = Properties.Resources._213x75_down;

            lblHour.Font = TimeDefines.Font;
            lblHour.ForeColor = TimeDefines.FontColor;

            lblMinute.Font = TimeDefines.Font;
            lblMinute.ForeColor = TimeDefines.FontColor;

            btnHourFormat.Font = TimeDefines.ButtonFont;
            btnHourFormat.ForeColor = TimeDefines.ButtonFontColor;

            btnZeroSecond.Font = TimeDefines.ButtonFont;
            btnZeroSecond.ForeColor = TimeDefines.ButtonFontColor;

            txtCurrentTime.Font = TimeDefines.TextBoxFont;
            txtCurrentTime.ForeColor = TimeDefines.TextBoxFontColor;
        }

        /// <summary>
        /// Set the middle sub section with the correct controls depending on the message Id
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetTimeControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_TIME):
                        {
                            lblHour.Text = new string(messageData.middleEnterTime.hoursLabel);
                            lblMinute.Text = new string(messageData.middleEnterTime.minutesLabel);
                            btnHourFormat.Text = new string(messageData.middleEnterTime.formatButtonText);
                            btnZeroSecond.Text = new string(messageData.middleEnterTime.zeroSecButtonText);
                           
                            TwentyFourHourFormat = messageData.middleEnterTime.twentyFourHourFormat;

                            //start the timer to update the time
                            StartTimer();

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the time in TimeControl.cs");
            }
        }       

        /// <summary>
        /// start the time ONLY if the message Id was MSG_GUI_BOTTOM_DATE_TIME
        /// </summary>
        private void StartTimer()
        {
            TimerTime.Enabled = true;
        }

        /// <summary>
        /// Stop the timer if the bottom section is other than date time control
        /// </summary>
        public void StopTimer()
        {
            TimerTime.Enabled = false;
        }

        /// <summary>
        /// Update the date time label
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TimerTime_Tick(object sender, EventArgs e)
        {
            UpdateTime();
        }

        /// <summary>
        /// Update the time on the screen
        /// </summary>
        private void UpdateTime()
        {
            try
            {
                DateTime currentDateTime = DateTime.Now;
                int hour = currentDateTime.Hour;
                int minute = currentDateTime.Minute;
                int second = currentDateTime.Second;
                string meridian;

                // Update the display if we've changed to the next second.  We compare the
                // hour, too, in case someone has just changed the system time by exactly one
                // hour and the same holds good for the minute
                if ((PreviousHour != hour) || (PreviousMinute != minute) || (PreviousSecond != second))
                {
                    // Adjust the hour and set the time suffix, depending on the time format.
                    if (TwentyFourHourFormat == GuiDefines.FALSE)
                    {
                        if (currentDateTime.Hour >= 12)
                        {
                            meridian = " PM";
                            if (currentDateTime.Hour > 12)
                            {
                                hour -= 12;
                            }
                        }
                        else
                        {
                            meridian = " AM";
                            if (hour == 0)
                            {
                                hour = 12;
                            }
                        }
                    }
                    else
                    {
                        meridian = "";
                    }

                    //build the date and time string to be displayed
                    StringBuilder time = new StringBuilder();
                    time.Append(hour.ToString().PadLeft(2, '0'));
                    time.Append(":");
                    time.Append(minute.ToString().PadLeft(2, '0'));
                    time.Append(":");
                    time.Append(second.ToString().PadLeft(2, '0'));
                    time.Append(" ");
                    time.Append(meridian);

                    //udpate the text box text
                    txtCurrentTime.Text = time.ToString();

                    PreviousHour = currentDateTime.Hour;
                    PreviousMinute = currentDateTime.Minute;
                    PreviousSecond = currentDateTime.Second;
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the time in TimeControl.cs");
            }
        }

        /// <summary>
        /// Increase the current hour by one and set the system time
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHourUp_Click(object sender, EventArgs e)
        {
            //get the current time and then set it by adding one extra hour
            DateTime dt = DateTime.UtcNow;
            SetSystemTimeParameters(dt);//convert the DateTime variable to SystemTime structure and make a call to native function           

            CurrentSystemTime.wHour = (ushort)((dt.AddHours(1)).Hour);
            bool result = Native.WinCESetSystemTime(ref CurrentSystemTime);

            UpdateTime();
            PlaySound.Click();
        }        

        /// <summary>
        /// Decrease the current hour by one and set the system time
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHourDown_Click(object sender, EventArgs e)
        {
            //get the current time and then set it by subtracting one extra hour
            DateTime dt = DateTime.UtcNow;
            SetSystemTimeParameters(dt);
          
            CurrentSystemTime.wHour = (ushort)((dt.AddHours(-1)).Hour);
            bool result = Native.WinCESetSystemTime(ref CurrentSystemTime);

            UpdateTime();
            PlaySound.Click();
        }

        /// <summary>
        /// Set the current seconds to zero seconds and set the system time and update the same to show it
        /// to the user
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnZeroSecond_Click(object sender, EventArgs e)
        {
            DateTime dt = DateTime.UtcNow;
            SetSystemTimeParameters(dt);
            CurrentSystemTime.wSecond = 0;

            bool result = Native.WinCESetSystemTime(ref CurrentSystemTime);

            UpdateTime();
            PlaySound.Click();
        }
        /// <summary>
        /// Increase the current minute by one and set the system time
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnMinuteUp_Click(object sender, EventArgs e)
        {
            DateTime dt = DateTime.UtcNow;
            SetSystemTimeParameters(dt);            

            CurrentSystemTime.wMinute = (ushort)((dt.AddMinutes(1)).Minute);

            if (CurrentSystemTime.wMinute > 59)
            {
                CurrentSystemTime.wMinute = 0;
            }

            bool result = Native.WinCESetSystemTime(ref CurrentSystemTime);

            UpdateTime();
            PlaySound.Click();
        }

        /// <summary>
        /// Decrease the current minute by one and set the system time
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnMinuteDown_Click(object sender, EventArgs e)
        {
            DateTime dt = DateTime.UtcNow;
            SetSystemTimeParameters(dt);
           
            CurrentSystemTime.wMinute = (ushort)((dt.AddMinutes(-1)).Minute);

            //since we have to type cast it to ushort, the maximum we can go for ushort is 65535
            if (CurrentSystemTime.wMinute == 65535)
            {
                CurrentSystemTime.wMinute = 59;
            }

            bool result = Native.WinCESetSystemTime(ref CurrentSystemTime);

            UpdateTime();
            PlaySound.Click();
        }

        /// <summary>
        /// This method populates the .net DateTime structure into the native SystemTime structure in order to make a
        /// call to a native method
        /// </summary>
        /// <param name="dt">the current time in .net DateTime structure</param>
        private void SetSystemTimeParameters(DateTime dt)
        {
            CurrentSystemTime.wYear = (ushort)dt.Year;
            CurrentSystemTime.wMonth = (ushort)dt.Month;
            CurrentSystemTime.wDay = (ushort)dt.Day;
            CurrentSystemTime.wDayOfWeek = (ushort)dt.DayOfWeek;
            CurrentSystemTime.wHour = (ushort)dt.Hour;
            CurrentSystemTime.wMinute = (ushort)dt.Minute;
            CurrentSystemTime.wSecond = (ushort)dt.Second;
            CurrentSystemTime.wMillisecond = (ushort)dt.Millisecond;
        }  

        /// <summary>
        /// Set the time format to 24 hour format or 12 hour format and send the same to the receiving process via the
        /// Gui message queue
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnHourFormat_Click(object sender, EventArgs e)
        {
            if (TwentyFourHourFormat != GuiDefines.FALSE)
            {
                //change it to 12 hour format
                TwentyFourHourFormat = Convert.ToChar(0);
            }
            else
            {
                //change it to 24 hour format
                TwentyFourHourFormat = Convert.ToChar(1);
            }

           // PlaySound.Click();

            //send messaging to the receiving process that the hour format got changed
            SendMessageToQueue.SendButtonMessage(sender);
        }

        private void repeatButton1_Click(object sender, EventArgs e)
        {
            btnHourUp_Click(null, null);
        }
    }
}
