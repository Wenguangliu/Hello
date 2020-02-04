using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using CppCsWrapper;

//Copyright (c) 2012 Abaxis.


namespace j2GUI.j2Startup
{
    //delegate used to update the main Gui thread from the receive message gui queue thread
    public delegate void UpdateEventHandler(GuiReceiveMsg_t msg);    

    //delegate used to show screen saving screens on the main Gui thread from the screen saver thread
    public delegate void ShowScreenSaverHandler(string ImageFileName, bool Show);

    //delegate used to display the keyboard on the screen
    public delegate void KeyboardEventHandler(bool display);
   
    /// <summary>
    /// This is the main form which is always present on the analyzer screen. User controls get added or removed on this screen. The actual Gui
    /// gets started from this start up form. The screens required by startup process and gui process use this main screen as a container. All 
    /// the screens other than this is a user control and this MainScreen form acts like a Form container
    /// </summary>
    public partial class MainScreen : Form
    {
        private Messaging.Queue GuiQueue;
        private Messaging.ReceiveQueue ReceiveQueue;
        private Messaging.SendQueue SendQueue;
        private SplashScreen SplashScreen = null;
        private ErrorInformationScreen ErrorInformationScreen;
        private Thread GuiReceiveThread = null;
        private Thread ScreenSaverThread = null;
        private Sound PlaySound = null;
        private ScreenSaver ScreenSave;
        private j2GUI.j2MiddleSectionControls.AlphaNumericKeyboardControl AlphaNumericKeyPad;//we had to add this here since we need to do internal keyboard visibility handhling

        //sections
        private TopSectionControls TopSection;
        private BottomSectionControls BottomSection;
        private MiddleSectionControls MiddleSection;
        private WinMessageControls WinMessageSection;
               
        /// <summary>
        /// Check if the gui needs to be started with cursor enabled or not. Then start off the gui receive queue thread
        /// </summary>
        /// <param name="cursorEnabled"></param>
        public MainScreen(string cursorEnabled)
        {
            try
            {
                InitializeComponent();

                //enable or disable the cursor on the analyzer screen
                if (cursorEnabled == "-c")
                {
                    Cursor.Show();
                }
                else
                {
                    Cursor.Hide();
                }
                              
                GuiQueue = new j2GUI.Messaging.Queue();
                SendQueue = new j2GUI.Messaging.SendQueue();
                ReceiveQueue = new j2GUI.Messaging.ReceiveQueue();

                ScreenSave = new ScreenSaver();
                ScreenSave.StartScreenSaving += new ShowScreenSaverHandler(ShowScreenSaver);

                PlaySound = new Sound();

                //set the Resco Keyboard skin path
                keyboardPro1.Skin = "\\Hard Disk\\Abaxis\\Skins\\AnalyzerSkin.rpk";
                
                //handler for updating the main screen from a gui receive message thread
                ReceiveQueue.UpdatingDisplay += new UpdateEventHandler(UpdateScreen);                
            
                //start the gui receive message queue and then send Gui Ready message to 'Startup' process
                StartGuiReceiveQueue();         
            }
            catch (System.Exception e)
            {
                SendQueue.SendLogErrorMessage("Unable to start the Gui receive thread in MainScreen.cs");
            }
        }        

        /// <summary>
        /// Update the main screen. This is mainly called from the receive gui message queue thread
        /// </summary>
        /// <param name="receiveMessage"></param>
        public void UpdateScreen(GuiReceiveMsg_t receiveMessage)
        {
            try
            {
                if (InvokeRequired)
                {
                    Invoke(new UpdateEventHandler(UpdateScreen), new object[] { receiveMessage });
                    return;
                }

                ShowGuiScreen(receiveMessage);
            }
            catch (System.Exception)
            {
                SendQueue.SendLogErrorMessage("Unable to Update the Screen in MainScreen.cs");
            }
        }

        /// <summary>
        /// Show the screen saver screen on the main window now. This is mainly called from the screen saver thread
        /// </summary>
        /// <param name="receiveMessage"></param>
        public void ShowScreenSaver(string imageFileName, bool show)
        {
            try
            {
                if (InvokeRequired)
                {
                    Invoke(new ShowScreenSaverHandler(ShowScreenSaver), new object[] {imageFileName, show});
                    return;
                }

                StartScreenSaverMode(imageFileName, show);
            }
            catch (System.Exception)
            {
                SendQueue.SendLogErrorMessage("Unable to show the screen saver in MainScreen.cs");
            }
        }

        /// <summary>
        /// Show/Not show the keyboard to the user. The keyboard is shown only on screens which display the ID text box 
        /// </summary>
        /// <param name="show">if true, then show the keyboard</param>
        public void ShowKeyboardToUser(bool show)
        {
            try
            {
                CommonDefines.Language_t keyboardLang = CommonDefines.Language_t.LANGUAGE_ENGLISH;

                if (InvokeRequired)
                {
                    Invoke(new KeyboardEventHandler(ShowKeyboardToUser), new object[] { show });
                    return;
                }

                keyboardPro1.Visible = show;

                //The KeyboardPro library compose the *.wav sound file path like: ProjectPath + “\\Sounds\\” + TapNotification + “.wav”
                //To use a different location for the sound file, use '..' command to jump to the parent folder
                keyboardPro1.TapNotification = @"..\..\..\Abaxis\Sound\Click";
                
                lock (GuiDefines.ShowAlphaNumericKeyboardLock)
                {
                    GuiDefines.ShowAlphaNumericKeyboard = show;
                }

                //set the keyboard language as per user selection before showing
                if (show)
                {                    
                    Native.GetKeyboard(ref keyboardLang);

                    switch (keyboardLang)
                    {
                        case CommonDefines.Language_t.LANGUAGE_ENGLISH:
                            {
                                keyboardPro1.Language = "English";
                                break;
                            }
                        case CommonDefines.Language_t.LANGUAGE_FRENCH:
                            {
                                keyboardPro1.Language = "French";
                                break;
                            }
                        case CommonDefines.Language_t.LANGUAGE_GERMAN:
                            {
                                keyboardPro1.Language = "German";
                                break;
                            }
                        case CommonDefines.Language_t.LANGUAGE_ITALIAN:
                            {
                                keyboardPro1.Language = "Italian";
                                break;
                            }
                        case CommonDefines.Language_t.LANGUAGE_SPANISH:
                            {
                                keyboardPro1.Language = "Spanish";
                                break;
                            }

                        case CommonDefines.Language_t.LANGUAGE_PORTUGUESE:
                            {
                                keyboardPro1.Language = "Portuguese";
                                break;
                            }
                        case CommonDefines.Language_t.LANGUAGE_BRAZIL:
                            {
                                keyboardPro1.Language = "Brazil";
                                break;
                            }
                        default:
                            {
                                keyboardPro1.Language = "English";
                                break;
                            }
                    }
                }
            }
            catch (System.Exception)
            {
                SendQueue.SendLogErrorMessage("Unable to show or not show the keyboard in MainScreen.cs");
            }
        }

        /// <summary>
        /// This is called to either show the screen saver images or the screen before screen saver started
        /// </summary>
        /// <param name="imageFileName"></param>
        /// <param name="show"></param>
        private void StartScreenSaverMode(string imageFileName, bool show)
        {
            ScreenSave.AddImage(imageFileName);

            if (show)
            {
                //screen saver ON/enabled   
                ScreenSave.Show();
                ScreenSave.BringToFront();
               
                keyboardPro1.Show(false);//hide it once screen saver begins               
            }
            else
            {
                //come out of screen saver mode
                lock (GuiDefines.InactionTimeLock)
                {
                    GuiDefines.InactionTime = 0;
                }

                //screen saver OFF/disabled
                ScreenSave.Hide();
                ScreenSave.SendToBack();

                lock (GuiDefines.ShowAlphaNumericKeyboardLock)
                {
                    if (GuiDefines.ShowAlphaNumericKeyboard)
                    {
                        keyboardPro1.Visible = true;
                    }
                    else
                    {
                        keyboardPro1.Visible = false;
                    }
                }
            }
        }
        
        /// <summary>
        /// Show the correct section depending on the message Id
        /// </summary>
        /// <param name="message">The receive message data as read from the Gui message queue</param>
        internal void ShowGuiScreen(GuiReceiveMsg_t message)
        {
            try
            {
                int msgId = message.msgId;               

                //Hide the full screen windows to begin with
                if (SplashScreen != null)
                {
                    SplashScreen.Hide();
                }

                //this is used if there is an error during startup
                if (ErrorInformationScreen != null)
                {
                    ErrorInformationScreen.Hide();
                }
                    
                //kill the splash screen timer thread         
                if (SplashScreen != null)
                {
                    if (!SplashScreen.IsTimerKilled)
                    {
                        SplashScreen.KillTimer();
                    }
                }

                if (IsTopMessage(message.msgId))
                {
                    WinMessageSection.Hide();

                    lock (GuiDefines.ErrorScreenActiveLock)
                    {
                        GuiDefines.ErrorScreenActive = false;
                    }
                    TopSection.SetTopControl(message);
                }
                else if (IsBottomMessage(message.msgId))
                {
                    WinMessageSection.Hide();

                    lock (GuiDefines.ErrorScreenActiveLock)
                    {
                        GuiDefines.ErrorScreenActive = false;
                    }
                    BottomSection.SetBottomControl(message);
                }
                else if (IsMiddleMessage(message.msgId))
                {
                    WinMessageSection.Hide();

                    //the middle home warming message comes only once in the lifetime of analyzer ON. So once you get this,
                    //start the screen saver thread. We had to put this code here since if we do it in the beginning when
                    //splash screen needs to show up, the splash screen is not displayed correctly

                    lock (GuiDefines.IsScreenSaverThreadStartedLock)
                    {
                        if (!GuiDefines.IsScreenSaverThreadStarted)
                        {
                            ThreadStart screenSaver = new ThreadStart(ScreenSave.InitScreenSaver);
                            ScreenSaverThread = new Thread(screenSaver);
                            ScreenSaverThread.Start();
                        }
                    }

                    lock (GuiDefines.ErrorScreenActiveLock)
                    {
                        GuiDefines.ErrorScreenActive = false;
                    }

                    ShowKeyboardToUser(false);//make the alphanumeric keyboard invisible
                    MiddleSection.SetMiddleControl(message);                    
                }
                else if (IsWinMessage(message.msgId))
                {
                    //since window messages are full screen display, we hide all the other
                    //sections and show only the window
                    TopSection.Hide();
                    MiddleSection.Hide();
                    BottomSection.Hide();
                    ShowKeyboardToUser(false);//make the alphanumeric keyboard invisible

                    WinMessageSection.SetWinMessageControl(message);   
                }                
                else if (IsUpdateMessage(message.msgId))
                {
                    WinMessageSection.Hide();
                    MiddleSection.UpdateMiddleControl(message);
                }                
                else if (IsSoundMessage(message.msgId))
                {
                    Sound(message);
                }
                else if (IsScreenSaveMessage(message.msgId))
                {
                    ScreenSaver(message);
                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_LANGUAGE))
                {
                    //get the input locale identifier that needs to be activated
                    //LANGUAGE_ENGLISH = 0x00000409
                    //LANGUAGE_FRENCH = 0x0000040c
                    //LANGUAGE_GERMAN = 0x00000407
                    //LANGUAGE_ITALIAN = 0x00000410
                    //LANGUAGE_SPANISH = 0x0000040a
                    System.IntPtr lang = (System.IntPtr)message.data.languageData.language;

                    //we have set the flag to 0x00000100 which activates the specified locale identifier for the entire process
                    //KLF_SETFORPROCESS
                    Native.ActivateKeyboardLayout(lang, 0x00000100);
                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_SPLASH))
                {
                    WinMessageSection.Hide();

                    //show the splash screen
                    SplashScreen = new SplashScreen(message);

                    touchPanel1.Controls.Add(SplashScreen);
                    SplashScreen.Show();
                    this.Show();
                } 
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_ENABLE_MOUSE)) 
                {
                    Cursor.Show();
                } else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_STARTUP_ERRORINFO))
                {
                    WinMessageSection.Hide();

                    //show the error information screen
                    ErrorInformationScreen = new ErrorInformationScreen();
                    ErrorInformationScreen.SetErrorInformation(message);

                    touchPanel1.Controls.Add(ErrorInformationScreen);
                    ErrorInformationScreen.Show();
                    this.Show();

                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_INPUT_RECEIVER))
                {
                    GuiQueue.SetReceiverQueueName(message.data.inputReceiver.name);
                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_ID_TEXT))
                {
                    MiddleSection.GetIdText();
                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_GET_RANGE))
                {
                    MiddleSection.GetRange();
                }
                else if (message.msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_DATE_TIME_STRINGS))
                {
                    WinMessageSection.Hide();

                    BottomSection.SetBottomControl(message);
                }
            }
            catch (System.Exception e)
            {
                SendQueue.SendLogErrorMessage("Unable to Show the display in MainScreen.cs");
            }
        }

        /// <summary>
        /// Handles screen saver messages from the console.
        /// </summary>
        /// <param name="message"></param>
        private void ScreenSaver(GuiReceiveMsg_t message)
        {
            lock (GuiDefines.ScreenSaveEnabledLock)
            {
                lock (GuiDefines.ScreenSaveActiveLock)
                {
                    lock (GuiDefines.InactionTimeLock)
                    {
                        switch (message.msgId)
                        {
                            case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_WAKEUP):
                                {
                                    GuiDefines.InactionTime = 0;

                                    break;
                                }
                            case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_DISABLE):
                                {
                                    GuiDefines.ScreenSaveEnabled = false;
                                    GuiDefines.InactionTime = 0;

                                    break;
                                }
                            case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_ENABLE):
                                {
                                    if (!GuiDefines.ScreenSaveEnabled)
                                    {
                                        GuiDefines.ScreenSaveEnabled = true;
                                        GuiDefines.InactionTime = 0;
                                    }

                                    break;
                                }
                            case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SCREENSAVE_BUTTON_PRESS):
                                {
                                    ushort screenSaverTimeout = 15;
                                    Native.GetScreenSaverTimeout(ref screenSaverTimeout);

                                    if (GuiDefines.ScreenSaveEnabled)
                                    {
                                        if (GuiDefines.ScreenSaveActive)
                                        {
                                            GuiDefines.InactionTime = 0;
                                        }
                                        else
                                        {
                                            GuiDefines.InactionTime = (double)screenSaverTimeout + 0.001;
                                        }
                                    }

                                    break;
                                }
                            default:
                                {
                                    break;
                                }
                        }
                    }
                }
            }
        }

        /// <summary>
        /// handles sound messages sent from the console to the gui
        /// </summary>
        /// <param name="message"></param>
        private void Sound(GuiReceiveMsg_t message)
        {
            //initialize sound
            byte volume = 0;
            
            switch (message.msgId)
            {
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CRITICAL_STOP):
                    {
                        PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_CRITICAL_STOP);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_ERROR):
                    {                        
                        PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_ERROR);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_WARNING):
                    {                       
                        PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_WARNING);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_PRINTING_DONE):
                    {
                        //get the status volume first from the SettingsData shared memory
                        Native.GetStatusVolume(ref volume);
                        PlaySound.SetStatusVolume(volume);                        
                        
                        PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_PRINTING_DONE);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_CLICK):
                    {
                        //get the click volume first from the SettingsData shared memory
                        Native.GetTouchClickVolume(ref volume);
                        PlaySound.SetClickVolume(volume);

                        PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_CLICK);
                        
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_ALERT_VOLUME):
                    {
                        PlaySound.SetAlertVolume(message.data.soundData.volume);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_STATUS_VOLUME):
                    {
                        PlaySound.SetStatusVolume(message.data.soundData.volume);
                        break;
                    }
                case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SOUND_SET_CLICK_VOLUME):
                    {  
                        PlaySound.SetClickVolume(message.data.soundData.volume);
                        //PlaySound.SetWaveOutVolume(message.data.soundData.volume); commented out on 02/22/12 since the click volume is already set. This is repetitive
                        break;
                    }
                default:
                    {
                        break;
                    }
            }
        }      
        
        /// <summary>
        /// Check if the message Id is a Screen Save message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        private bool IsScreenSaveMessage(int msgId)
        {
            if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SCREENSAVE) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SCREENSAVE))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Check if the message Id is a Sound message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        private bool IsSoundMessage(int msgId)
        {
            if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SOUND) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SOUND))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Check if the message Id is a update message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        private bool IsUpdateMessage(int msgId)
        {
            if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_UPDATE) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_UPDATE))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        
        /// <summary>
        /// Is the message a top section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        internal bool IsTopMessage(int msgId)
        {
            if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_TOP) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_TOP))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Is the Message Id a Middle Section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        internal bool IsMiddleMessage(int msgId)
        {
            if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_MIDDLE) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_MIDDLE))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Is the Message Id a Bottom section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        internal bool IsBottomMessage(int msgId)
        {
            if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_BOTTOM) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_BOTTOM))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /// <summary>
        /// Is the message a Window message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        internal bool IsWinMessage(int msgId)
        {
            if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_WIN) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_WIN))
            {
                return true;
            }
            else
            {
                return false;
            }
        }       

        /// <summary>
        /// As soon as the GUI process is started ie, after start up GUI, we initialize all the user controls and the sub middle user controls
        /// Then we add the four main sections of the user controls to the mainscreen form control. We even add the sub middle user controls that gets
        /// contained in the middle section user control. Then hide all the four sections. So we are here creating all the controls that is ever shown 
        /// on the GUI and then hiding them. Then depending on the message Ids in the message queue, we show the corresponding one 
        /// </summary>
        private void InitializeGUIFormSections()
        {
            try
            {
               // this.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                touchPanel1.BackgroundImage = Properties.Resources.Background;

                TopSection = new TopSectionControls();
                BottomSection = new BottomSectionControls();

                MiddleSection = new MiddleSectionControls();
                AlphaNumericKeyPad = new j2GUI.j2MiddleSectionControls.AlphaNumericKeyboardControl();                
                AlphaNumericKeyPad.ShowKeyboard += new KeyboardEventHandler(ShowKeyboardToUser);
                MiddleSection.SetAlphaNumericKeyPad(AlphaNumericKeyPad);

                WinMessageSection = new WinMessageControls();

                touchPanel1.Controls.Add(TopSection);
                touchPanel1.Controls.Add(BottomSection);
                touchPanel1.Controls.Add(MiddleSection);
                touchPanel1.Controls.Add(WinMessageSection);
                touchPanel1.Controls.Add(ScreenSave);

                //to beginwith, hide all the user control section of the main screen
                TopSection.Hide();
                BottomSection.Hide();
                MiddleSection.Hide();
                WinMessageSection.Hide();
                ScreenSave.Hide();
                ShowKeyboardToUser(false);//make the alphanumeric keyboard invisible
            }
            catch (System.Exception)
            {
                SendQueue.SendLogErrorMessage("Failed to initialize the Gui Form sections in MainScreen.cs");
            }
        }  

        /// <summary>
        /// Start the gui receive thread to start processing gui messages
        /// </summary>        /// 
        public void StartGuiReceiveQueue()
        {
            try
            {
                //clear up the main screen
                touchPanel1.Controls.Clear();
                
                //initialize all the variables if any
                InitializeVariables();

                //create new instances of top, middle, bottom and win sections
                InitializeGUIFormSections();                

                //start the receive gui messages thread to start displaying the screens
                ThreadStart queueStarter = new ThreadStart(ReceiveQueue.GuiProcessReceiveQueue);

                GuiReceiveThread = new Thread(queueStarter);
                GuiReceiveThread.Start();
                
                //send gui ready message to startup message queue                        
                SendQueue.SendMessage(new string(StartupMessages.STARTUP_QUEUE_NAME), (int)StartupMessages.StartupMsgId_t.MSG_STARTUP_GUI_READY);

                //log the gui ready message
                SendQueue.SendLogErrorMessage("Gui ready message sent");

                PlaySound.InitializeSound(); 
            }
            catch (System.Exception)
            {
                SendQueue.SendLogErrorMessage("Failed to start the receive queue in MainScreen.cs");
            }
        }

        /// <summary>
        /// Initialize the variables to its default state before starting off the Gui
        /// </summary>
        private void InitializeVariables()
        {
            GuiDefines.InactionTime = 0;
            GuiDefines.ScreenSaveEnabled = true;
            GuiDefines.ScreenSaveActive = false;
            GuiDefines.ErrorScreenActive = false;
        }

        /// <summary>
        /// House keeping. Not required in actual real time scenario since the application is shutdown only when the analyzer is off and so
        /// anyway the resources are cleaned up automatically
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainScreen_Closing(object sender, CancelEventArgs e)
        {
            BottomSection.TimerDateTime.Enabled = false;

            if (GuiReceiveThread != null)
            {
                GuiReceiveThread.Abort();
            }
        }      
    }
}
