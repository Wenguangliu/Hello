using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace j2AMSGUI.j2Startup
{
    //public delegate void UpdateEventHandler();   
    
    public partial class SplashScreen : UserControl
    {
        //event to tell the main screen that initialization is done and now start the Gui message queue receive thread
        public event StartReceiveQueueHandler StartingReceiveQueue;
        private SplashScreenGuiDefines SplashDefines = new SplashScreenGuiDefines();

        public SplashScreen()
        {
            InitializeComponent();            

            //gradientFilledPanelTitle.Text = "Jen2 Abaxis Analyzer";
            pictureBoxSplash.Image = j2AMSGUI.Properties.Resources.J2splashRotor;   

            //top title color
            lblTopTitle.BackColor = SplashDefines.TitleBackColor;
            lblTopTitle.Font = SplashDefines.TitleFont;
            lblTopTitle.ForeColor = SplashDefines.TitleFontColor;
            lblTopTitle.Text = "Abaxis Jen2 AMS";

            lblMessage.Text = "Initializing.............";
            timerStartup.Enabled = true;            
            progressBar1.Value = 10;
        }

        /// <summary>
        /// TO BE DONE: STARTUP code should update the progress bar on the startup screen. But once the gui process is started, we need to start the rx message
        /// queue up and running. Need to still figure out with Adeneo the order in which the progress bar moves and the next screen need to be displayed
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void timerStartup_Tick(object sender, EventArgs e)
        {
            try
            {
                if (progressBar1.Value < 100)
                {
                    progressBar1.Value += 10;
                }
                else
                {
                    timerStartup.Enabled = false;
                }

                if (progressBar1.Value == 90)//this needs to change once we set the process right with Adeneo
                {
                    timerStartup.Enabled = false;
                    //TO BE DONE -- when GUI gets called, initialize the GUI and start the rxQueue
                    StartingReceiveQueue();//uncomment this once the message queues work//raise the event to tell the mainscreen that now we can start the GUI rx queue to begin processing the Gui messages
                  }
            }
            catch (System.Exception)
            {
                throw new Exception("Failed in Timer Startup in SplashScreen.cs");
            }
        }
    }
}
