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

namespace j2AMSGUI.j2Startup
{
    //delegate used to update the main Gui thread from the receive message queue thread
    public delegate void UpdateEventHandler(AmsGuiMsg_t msg);

    //delegate used to display the keyboard on the screen
    //public delegate void KeyboardEventHandler();

    //delegate used to inform the mainscreen from the splash screen timer thread that it is now time to start a thread to receive the gui messages
    public delegate void StartReceiveQueueHandler();
    
    /// <summary>
    /// This is the main form which is always present on the analyzer screen. User controls get added or removed on this screen. The actual Gui
    /// gets started from this start up form
    /// </summary>
    public partial class MainScreen : Form
    {
        private Messaging.Queue GuiQueue;
        private Messaging.ReceiveQueue ReceiveQueue;
        private Messaging.SendQueue SendQueue;
        private SplashScreen SplashScreen;        
        private Thread ReceiveThread = null;

        //TODO public static event j2AMSGUI.j2Startup.UpdateEventHandler UpdatingDisplay;

        private AMSGUIMain AmsGUI;
                   
        public MainScreen()
        {
            try
            {
                InitializeComponent();

                //add the image to the picture box
                pictureBoxLogo.Image = Properties.Resources.abaxis_logo;

                GuiQueue = new j2AMSGUI.Messaging.Queue();

                SendQueue = new j2AMSGUI.Messaging.SendQueue();
                
                ReceiveQueue = new j2AMSGUI.Messaging.ReceiveQueue();
                Messaging.ReceiveQueue.UpdatingDisplay += new UpdateEventHandler(UpdateScreen);

                SplashScreen = new SplashScreen();
                SplashScreen.StartingReceiveQueue += new StartReceiveQueueHandler(StartReceiveQueue);

                ShowSplashScreen();                       
            }
            catch //TODO(System.Exception e)
            {
               // TODO TraceWrite.TraceLog("Unable to show the splash screen in MainScreen.cs");
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to show the splash screen in MainScreen.cs");
            }
        }        

        public void ShowSplashScreen()
        {
            //clear up the main screen
            this.Controls.Clear();//remove the logo

            //TO BE DONE: STARTUP CODE which tells the version and the product type to display on the splash screen
            //add the splash screen user control and show
                  
            this.Controls.Add(SplashScreen);            
        }             

        /// <summary>
        /// Update the main screen. This is mainly called from the receive gui message queue thread
        /// </summary>
        /// <param name="receiveMessage"></param>
        public void UpdateScreen(AmsGuiMsg_t receiveMessage)
        {
            try
            {
                if (InvokeRequired)
                {
                    Invoke(new UpdateEventHandler(UpdateScreen), new object[] { receiveMessage });
                    return;
                }

                ShowDisplay(receiveMessage);
            }
            catch (System.Exception)
            {
                TraceWrite.TraceLog("Unable to Update the Screen in MainScreen.cs");
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to Update the Screen in MainScreen.cs");
            }
        }
        
        /// <summary>
        /// Update AMS GUI with new error and status label text.
        /// </summary>
        /// <param name="message">The receive message data as read from the Gui message queue</param>
        internal void ShowDisplay(AmsGuiMsg_t message)
        {
            try
            {                              
                AmsGUI.Hide();
                AmsGUI.LoadAMSGUI(message);
                AmsGUI.Show();
            }
            catch //TODO(System.Exception e)
            {
                //throw new Exception("Unable to Show the display in MainScreen.cs");
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Unable to Show the display in MainScreen.cs");
            }
        }      
        
        /// <summary>
        /// Check if the message Id is a Screen Save message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        //private bool IsScreenSaveMessage(int msgId)
        //{
        //    if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SCREENSAVE) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SCREENSAVE))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

        /// <summary>
        /// Check if the message Id is a Sound message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        //private bool IsSoundMessage(int msgId)
        //{
        //    if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_SOUND) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_SOUND))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

        /// <summary>
        /// Check if the message Id is a update message
        /// </summary>
        /// <param name="msgId"></param>
        /// <returns></returns>
        //private bool IsUpdateMessage(int msgId)
        //{
        //    if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_UPDATE) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_UPDATE))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}
        
        /// <summary>
        /// Is the message a top section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        //internal bool IsTopMessage(int msgId)
        //{
        //    if ((msgId >= (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_TOP) && (msgId <= (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_TOP))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

        /// <summary>
        /// Is the Message Id a Middle Section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        //internal bool IsMiddleMessage(int msgId)
        //{
        //    if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_MIDDLE) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_MIDDLE))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

        /// <summary>
        /// Is the Message Id a Bottom section message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        //internal bool IsBottomMessage(int msgId)
        //{
        //    if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_BOTTOM) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_BOTTOM))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

        /// <summary>
        /// Is the message a Window message
        /// </summary>
        /// <param name="msgId">Message Id of the message</param>
        /// <returns></returns>
        //internal bool IsWinMessage(int msgId)
        //{
        //    if ((msgId > (int)GuiMessages.GuiMsgId_t.MSG_GUI_FIRST_WIN) && (msgId < (int)GuiMessages.GuiMsgId_t.MSG_GUI_LAST_WIN))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}

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
                this.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                                
                AmsGUI = new AMSGUIMain();
                               
                this.Controls.Add(AmsGUI);
                               
                AmsGUI.Show();    
            }
            catch (System.Exception)
            {
                //throw new Exception("Failed to initialize the Gui Form sections in MainScreen.cs");
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Failed to initialize the Gui Form sections in MainScreen.cs");
            }
        }

        /// <summary>
        /// Method to show screens instead of from message queue
        /// For debug purpose only or for testing purpose only
        /// </summary>
        public void StartReceiveQueue()
        {
            try
            {
                //clear up the main screen
                this.Controls.Clear();

                //create new instance of AMS GUI.
                InitializeGUIFormSections();                               
                   
                AMSGUIMain.UpdatingDisplay += new UpdateEventHandler(UpdateScreen);

                //start the receive gui messages thread to start displaying the screens
                ThreadStart queueStarter = new ThreadStart(ReceiveQueue.ProcessReceiveQueue);
                ReceiveThread = new Thread(queueStarter);
                ReceiveThread.Start();

                //only for debugging till recall test is done. This is the only gui message which is written by Gui other than error messages
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Gui receiving thread ready to start receiving messages");
             
                //SelectScreen.UpdatingDisplay += new UpdateEventHandler(UpdateScreen);
            }
            catch (System.Exception)
            {
                //throw new Exception("Failed to start the ReceiveQueue in MainScreen.cs");
                SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Failed to open the select screen in MainScreen.cs");
            }
        }

        /// <summary>
        /// Start the gui receive thread to start processing gui messages
        /// </summary>
        //public void StartReceiveQueue123()
        //{
        //    try
        //    {
        //        //clear up the main screen
        //        this.Controls.Clear();

        //        //create new instances of top, middle, bottom and win sections
        //        InitializeGUIFormSections();

        //        //start the receive gui messages thread to start displaying the screens
        //        ThreadStart queueStarter = new ThreadStart(ReceiveQueue.ProcessReceiveQueue);
        //        ReceiveThread = new Thread(queueStarter);
        //        ReceiveThread.Start();

        //        //only for debugging till recall test is done. This is the only gui message which is written by Gui other than error messages
        //        SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Gui receiving thread ready to start receiving messages");
             
        //        ////////////////////////////////////Uncomment this if GUI manual test process///////////////////////
        //        //screen.Show();
        //        ////////////////////////////////////Uncomment this if GUI manual process///////////////////////

        //        ////////////////////////////////////Uncomment this if Recall test process///////////////////////
        //        //ShowRecallScreen();
        //        ////////////////////////////////////Uncomment this if Recall test process///////////////////////
        //    }
        //    catch (System.Exception)
        //    {
        //        //throw new Exception("Failed to start the ReceiveQueue in MainScreen.cs");
        //        SendQueue.SendMessage(new string(ErrLoggerMessages.ERR_LOGGER_QUEUE_NAME), (int)ErrLoggerMessages.ErrLoggerMsgId_t.MSG_ERR_LOGGER_MESSAGE, "Failed to start the receive queue in MainScreen.cs");
        //    }
        //}       

        /// <summary>
        /// Used only for Recall process testing
        /// </summary>
        //private void ShowRecallScreen()
        //{
        //    Utilities util = new Utilities();
        //    GuiReceiveMsg_t msgReceive;

        //    msgReceive = new GuiReceiveMsg_t();

        //    //after splash screen, show blank screen
        //    msgReceive.msgId = 99;
        //    UpdateScreen(msgReceive);
           
        //    //send gui ready message to startup
        //    SendQueue.SendMessage(new string(StartupMessages.STARTUP_QUEUE_NAME), (int)StartupMessages.StartupMsgId_t.MSG_STARTUP_GUI_READY);
        //}

        /// <summary>
        /// House keeping. Not required in actual real time scenario since the application is shutdown only when the analyzer is off and so
        /// anyway the resources are cleaned up automatically
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MainScreen_Closing(object sender, CancelEventArgs e)
        {           
            if (ReceiveThread != null)
            {
                ReceiveThread.Abort();
            }
        }        
    }
}