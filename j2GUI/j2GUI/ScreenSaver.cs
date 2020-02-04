using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;
using CppCsWrapper;

namespace j2GUI
{
    /// <summary>
    /// This class handles the screen saver functionality for the analyzer. The screen saver image files depend on the
    /// analyzer product code.
    /// </summary>
    public partial class ScreenSaver : UserControl
    {
        public event j2GUI.j2Startup.ShowScreenSaverHandler StartScreenSaving;

        //location on the hard disk where screen saver image files can be found
        public const string PICCOLO_SCREEN_SAVER_DIR = "\\Hard Disk\\Abaxis\\ScreenSaver\\Piccolo\\";
        public const string VETSCAN_SCREEN_SAVER_DIR = "\\Hard Disk\\Abaxis\\ScreenSaver\\VetScan\\";

        private Messaging.SendQueue SendMessageToQueue;
       
        private List<string> FileNameList = new List<string>();
        private int Index;//the index of the screen saver image being shown currently
        private int CurrentImageTime;//the time elapsed showing the current screen saver screen

        public ScreenSaver()
        {
            InitializeComponent();

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            Index = 0;
        }

        /// <summary>
        /// Set up the screen saver functionality. Load the filename list depending on the product code.
        /// As of 07/21/2011, we have hardcoded the timeouts since AbaxisSetup.dll is not pulling correct values
        /// from the Settings shared memory. We have hardcoded ScreenSaverTimerout to 15 minutes. We need to correct this
        /// issue later
        /// </summary>
        public void InitScreenSaver()
        {
            double inactionTime = 0;
            bool screenSaveEnabled = false;
            ushort screenSaverTimerout = 15;

            lock (GuiDefines.IsScreenSaverThreadStartedLock)
            {
                GuiDefines.IsScreenSaverThreadStarted = true;
            }

            //get the list of screen saver image files
            FileNameList = GetScreenSaverFiles();
                                    
            if(FileNameList.Count == 0)
            {
                //log error that no screen saver files available
                SendMessageToQueue.SendLogErrorMessage("Screen saver did not find any image files in InitScreenSaver");

                return;
            }
          
            //pre load the first image           
            StartScreenSaving(FileNameList[0], false);//0th image and dont show the screen saver yet. Just pre load the first image

            while (true)
            {
                // Check every 0.5 seconds to see if it's time to display the screen saver.
                Thread.Sleep(500);

                // Increment the inactive time (in minutes) by 1/2 second and get the
                // value for use in this thread.
                lock (GuiDefines.ScreenSaveEnabledLock)
                {
                    lock (GuiDefines.ErrorScreenActiveLock)
                    {
                        lock (GuiDefines.InactionTimeLock)
                        {
                            screenSaveEnabled = GuiDefines.ScreenSaveEnabled & (!GuiDefines.ErrorScreenActive);
                            GuiDefines.InactionTime += 0.0083333;
                            inactionTime = GuiDefines.InactionTime;
                        }
                    }                    
                }
                                
                Native.GetScreenSaverTimeout(ref screenSaverTimerout);

                //Console.WriteLine(inactionTime.ToString());
                
                // If the screen saver timeout is not zero (Never) and the inaction time
                // has exceeded the timeout, activate.                
                if ((screenSaverTimerout > 0) && (inactionTime > screenSaverTimerout) && screenSaveEnabled)
                {
                    DoScreenSave(FileNameList);
                }
            }
        }

        /// <summary>
        /// Start showing screen saver screens. Currently due to some issue with AbaxisSetup.dll not able to pull
        /// correct Setting shared memory values, we are hard coding ScreenSaverTimeout and PowerSaveTimeout 07/21/11
        /// </summary>
        /// <param name="fileNameList">The image file names for showing the screen saver files</param>
        private void DoScreenSave(List<string> fileNameList)
        {
            double inActionTime = 0;
            bool powerSaveStart = true;
            ushort screenSaverTimeout = 15;
            ushort powerSaveTimeout = 30;
            byte backLightLevel = 0;

            //This isn't really the save on time, it's the delay for save time plus the save on time.
            Native.GetScreenSaverTimeout(ref screenSaverTimeout);

            double saverOnTime = (double)screenSaverTimeout;

            //Show the first image
            StartScreenSaving(fileNameList[Index], true);

            lock (GuiDefines.ScreenSaveActiveLock)
            {
                GuiDefines.ScreenSaveActive = true;
            }

            //loop until some action triggers the screen saver to go away
            while (true)
            {
                // Delay 1/2 second between checks for activity.
                Thread.Sleep(500);

                // Increment saver on time by 1/2 second.
                saverOnTime += 0.0083333;

                // Get our elapsed time.
                lock (GuiDefines.InactionTimeLock)
                {
                    inActionTime = GuiDefines.InactionTime;
                }

                Native.GetScreenSaverTimeout(ref screenSaverTimeout);
                Native.GetPowerSaveTimeout(ref powerSaveTimeout);
                Native.GetBacklightLevel(ref backLightLevel);
                
                // Action detected.
                if (inActionTime == 0)
                {
                    if (++Index >= FileNameList.Count)
                    {
                        Index = 0;//loop thro' the images
                    }

                    //restore the currently active window to the front
                    StartScreenSaving(fileNameList[Index], false);

                    Native.SetDeviceBacklight((int)backLightLevel);
                    Native.SetDeviceLedSteadyOn();

                    lock (GuiDefines.ScreenSaveActiveLock)
                    {
                        GuiDefines.ScreenSaveActive = false;
                    }

                    return;
                }
                else
                {
                    // Still in screen saver if power save timeout is off or if the time
                    // is less than the power save timeout time.
                    if ((powerSaveTimeout == 0) || (saverOnTime < powerSaveTimeout))
                    {
                        if (++CurrentImageTime >= 120)
                        {
                            if (++Index >= FileNameList.Count)
                            {
                                Index = 0;//loog thro' images
                            }

                            //Load the next image
                            StartScreenSaving(fileNameList[Index], true);

                            CurrentImageTime = 0;
                        }
                    }
                    else
                    {
                        //Power save mode.
                        if (powerSaveStart)
                        {
                            //do not display any image now
                            StartScreenSaving(string.Empty, true);
                        }

                        //Set the backlight to its lowest setting
                        Native.SetDeviceBacklight(0);//enable this later
                        Native.SetDeviceLedBlink();//enable this later
                        powerSaveStart = false;

                    }
                }
            }
        }

        /// <summary>
        /// Get all the screen saver image file names for the correct product code
        /// </summary>
        /// <returns>List of all the image file names</returns>
        private List<string> GetScreenSaverFiles()
        {
            List<string> screenSavers = new List<string>();
            string screenSaverDir;
            string ext;
            byte productCode = 1;//default

            Native.GetAbaxisProductCode(ref productCode);

            if ( productCode == CommonDefines.PICCOLO_PRODUCT_CODE)
            {
                screenSaverDir = PICCOLO_SCREEN_SAVER_DIR;
            }
            else
            {
                screenSaverDir = VETSCAN_SCREEN_SAVER_DIR;
            }

            if (Directory.Exists(screenSaverDir))
            {
                foreach (string fileName in Directory.GetFiles(screenSaverDir))
                {                    
                    ext = Path.GetExtension(fileName);
                    ext = ext.ToUpper();
                    if (ext == ".JPG" || ext == ".JPEG" || ext == ".BMP" || ext == ".GIF" || ext == ".PCX")
                    {
                        screenSavers.Add(fileName);//add only the image file names whose extensions are only jpg, jpeg, bmp, gif and pcx
                    }
                }
            }
            else
            {
                //log error that screen saver directory does not exist
                //SendMessageToQueue.SendLogErrorMessage("Screen saver directory does not exist in ScreenSaver.cs");
                MessageBox.Show("Screen saver directory does not exist on the Compact Flash card. Do not proceed...", "Analyzer Software",MessageBoxButtons.OK,MessageBoxIcon.Exclamation,MessageBoxDefaultButton.Button1);
            }

            return (screenSavers);
        }

        /// <summary>
        /// Add the correct screen saver image to the picture box
        /// If the image file name is empty for example when the power save has started or when the user
        /// activity has just started, then dont show any image on the picture box
        /// </summary>
        /// <param name="imageFileName"></param>
        internal void AddImage(string imageFileName)
        {
            if (imageFileName != string.Empty)
            {
                pictureBoxScreenSaver.Image = new Bitmap(imageFileName);                
            }
            else
            {
                pictureBoxScreenSaver.Image = null;
            }

            pictureBoxScreenSaver.Refresh();
        }

        /// <summary>
        /// Here the main gui thread is posting a event to its own thread saying that we need to end the screen saver
        /// since there was some activity on the screen
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void pictureBoxScreenSaver_Click(object sender, EventArgs e)
        {
            StartScreenSaving(string.Empty, false);
        }
    }
}
