using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using CppCsWrapper;

namespace j2GUI.j2Startup
{
    /// <summary>
    /// Splash screen to show the user that all the processes are starting up. Here the progress bar is a 
    /// non-intelligent one progressing at a uniform interval and waiting for the timeout to occur or wait
    /// for the timer thread to be killed
    /// </summary>
    public partial class SplashScreen : UserControl
    {
        //event to tell the main screen that timeout has occurred
        //public event StartupErrorHandler StartupError;
        private TopGuiDefines topDefines = new TopGuiDefines();
        private Messaging.SendQueue SendMessageToQueue;
        private MiddleSectionProgressBar ProgressBarDefines = new MiddleSectionProgressBar();
        private const int MAX_STARTUP_TIME = 50; //time to wait till all the process are up and running and end of splash screen display
        private string ProgressBarMessage;
        private int ProgressBarIncrement;
        private int ElapsedTime = 0;
        Utilities Util = new Utilities();
        private bool TimerKilled = false;

        public bool IsTimerKilled
        {
            get
            {
                return TimerKilled;
            }           
        }

        /// <summary>
        /// Show the splash screen after updating the message on the progress bar and the title of the splash screen
        /// </summary>
        /// <param name="msg">the recieved data message</param>
        public SplashScreen(GuiReceiveMsg_t msg)
        {
            string productCode = string.Empty;
            string version = string.Empty;

            try
            {
                InitializeComponent();

                SendMessageToQueue = new j2GUI.Messaging.SendQueue();

                GuiReceiveMsgData_t msgData = msg.data;
                pictureBoxSplash.Image = j2GUI.Properties.Resources.SplashRotor;

                //top title properties
                lblTopTitle.BackColor = topDefines.BackColor;
                lblTopTitle.Font = topDefines.Font;
                lblTopTitle.ForeColor = topDefines.FontColor;

                //set the properties of the progress bar
                progressBar1.Font = ProgressBarDefines.Font;
                progressBar1.ForeColor = ProgressBarDefines.FontColor;

                //progress colors
                progressBar1.BarGradientColor.StartColor = ProgressBarDefines.StartBarColor;
                progressBar1.BarGradientColor.EndColor = ProgressBarDefines.EndBarColor;

                //progress bar solid background color
                progressBar1.BackgroundColor = ProgressBarDefines.BarBackColor;

                //get the data from the received message and update the splash screen
                ProgressBarMessage = Util.OneCharArrayToString(msgData.winSplashScreen.progBarText);
                productCode = Util.OneCharArrayToString(msgData.winSplashScreen.productCode);
                version = Util.OneCharArrayToString(msgData.winSplashScreen.version);
                lblTopTitle.Text = productCode + " " + version;                              

                //the progress bar increments automatically. It is not linked to any event.
                ProgressBarIncrement = 100 / MAX_STARTUP_TIME;
                TimerStartup.Enabled = true;
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to show splash screen");                
            }
        }

        /// <summary>
        /// Keep updating the progress bar on the splash screen until you reach the max startup time. Once Max startup time is reached, then
        /// disable the timer automatically. This max startup time should be larger than the time required by the startup to start all the 
        /// process, messages queues, and shared memory
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timerStartup_Tick(object sender, EventArgs e)
        {
            ElapsedTime++;
            progressBar1.TextOutputFormat = ProgressBarMessage;

            if (ElapsedTime < MAX_STARTUP_TIME)
            {
                progressBar1.Value += ProgressBarIncrement;
            }
            else
            {
                //Time out occurred
                KillTimer();
            }
        }

        /// <summary>
        /// Stop the Timer
        /// </summary>
        public void KillTimer()
        {
            TimerKilled = true;
            TimerStartup.Enabled = false;
        }
    }
}
