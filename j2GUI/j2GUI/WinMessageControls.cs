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
using Resco.Controls.CommonControls;

namespace j2GUI
{
    public partial class WinMessageControls : UserControl
    {
        private Utilities Util;
        WinCriticalStopGuiDefines CriticalStopWinDefines = new WinCriticalStopGuiDefines();
        WinWarningGuiDefines WarningWinDefines = new WinWarningGuiDefines();
        WinErrorGuiDefines ErrorWinDefines = new WinErrorGuiDefines();
        private Messaging.SendQueue SendMessageToQueue;
        private Sound PlaySound = new Sound();

        public WinMessageControls()
        {
            InitializeComponent();

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            Util = new Utilities();

            //set default form properties
            this.Height = GuiForm.FormHeight;
            this.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
            
            //set info button image properties
            btnInfo.ImageDefault = Properties.Resources.Info_Icon;       
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);            
        }       

        /// <summary>
        /// 
        /// </summary>
        /// <param name="msg"></param>
        internal void SetWinMessageControl(GuiReceiveMsg_t msg)
        {
            try
            {
                bool showErrorScreen = true;

                ClearBottomControls();

                btnInfo.Text = "";//clear the default text for the image before displaying                

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_NONE):
                        {
                            this.Hide();
                            showErrorScreen = false;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP):
                        {                            
                            LoadWinCriticalError(msg.data);
                            this.Show();

                            PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_CRITICAL_STOP);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1):                        
                        {
                            LoadWinWarning1(msg);
                            this.Show();

                            PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_WARNING);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2):
                        {
                            LoadWinWarning2(msg);
                            this.Show();

                            PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_WARNING);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2)://added by nj on 04/14/2016
                        {
                            LoadWinCriticalError2(msg);
                            this.Show();

                            PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_CRITICAL_STOP);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR):
                        {
                            LoadWinError(msg);
                            this.Show();

                            PlaySound.Play(CppCsWrapper.Sound.Sound_t.SOUND_ERROR);
                            break;
                        }
                    default:
                        {
                            this.Hide();
                            showErrorScreen = false;

                            break;
                        }
                }

                lock (GuiDefines.ErrorScreenActiveLock)
                {
                    lock (GuiDefines.InactionTimeLock)
                    {
                        GuiDefines.ErrorScreenActive = showErrorScreen;
                        GuiDefines.InactionTime = 0;
                    }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Set Win Message controls in WinMessageControls.cs");
            }
        }

        /// <summary>
        /// Load the Window Critical Error2 screen with custom colors and fonts
        /// </summary>
        /// <param name="msg">data related to the screen</param>
        private void LoadWinCriticalError2(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t msgdata = msg.data;

                //back color                
                this.BackColor = CriticalStopWinDefines.BackColor;

                //top title color
                lblTopTitle.BackColor = CriticalStopWinDefines.TitleBackColor;
                lblTopTitle.Font = CriticalStopWinDefines.TitleFont;
                lblTopTitle.ForeColor = CriticalStopWinDefines.TitleFontColor;
                lblTopTitle.Text = new string(msgdata.winError.topLabel);

                //message
                lblMessage.Font = CriticalStopWinDefines.MessageFont;
                lblMessage.ForeColor = CriticalStopWinDefines.MessageFontColor;
                this.lblMessage.Text = new string(msgdata.winError.message);

                //bottom panel
                //show the bottom panel since for showing win error window, 
                //no bottom controls are needed
                panelBottom.Visible = true;

                //for the bottom panel, we need to add the buttons. Here we are using the BottomButtonsData
                //since the win error message has the bottom controls similiar to that of the bottom section
                //So, create a new GuiBottomButtonsData and populate the structure before calling the LoadBottomButtons
                //method
                GuiBottomButtonsData_t data = new GuiBottomButtonsData_t();
                data.buttonText = Util.OneCharArrayToTwoCharArray(msgdata.winError.buttonText);
                data.buttonHighlighted[0] = GuiDefines.HIGHLIGHT;//we are forcing it to highlight
                LoadBottomButtons(GuiDefines.ONEBUTTON, data, msg.msgId);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Load Window Critical Error 2 Screen in WinMessageControls.cs");
            }
        }

        /// <summary>
        /// Load the Window Critical Error screen with custom colors and fonts
        /// </summary>
        /// <param name="msgdata">data related to the screen</param>
        private void LoadWinCriticalError(GuiReceiveMsgData_t msgdata)
        {
            try
            {
                //back color
                this.BackColor = CriticalStopWinDefines.BackColor;

                //top title color
                lblTopTitle.BackColor = CriticalStopWinDefines.TitleBackColor;
                lblTopTitle.Font = CriticalStopWinDefines.TitleFont;
                lblTopTitle.ForeColor = CriticalStopWinDefines.TitleFontColor;
                lblTopTitle.Text = new string(msgdata.winCriticalStop.topLabel);

                //message
                lblMessage.Font = CriticalStopWinDefines.MessageFont;
                lblMessage.ForeColor = CriticalStopWinDefines.MessageFontColor;
                this.lblMessage.Text = new string(msgdata.winCriticalStop.message);

                //bottom panel
                //hide the bottom panel since for showing critical error window, 
                //no bottom controls are needed
                panelBottom.Visible = false;
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Load Window Critical Error Screen in WinMessageControls.cs");
            }
        }

        /// <summary>
        /// Load the Window Error screen with custom colors and fonts
        /// </summary>
        /// <param name="msg">data related to the screen</param>
        private void LoadWinError(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t msgdata = msg.data;

                //back color                
                this.BackColor = ErrorWinDefines.BackColor;

                //top title color
                lblTopTitle.BackColor = ErrorWinDefines.TitleBackColor;
                lblTopTitle.Font = ErrorWinDefines.TitleFont;
                lblTopTitle.ForeColor = ErrorWinDefines.TitleFontColor;
                lblTopTitle.Text = new string(msgdata.winError.topLabel);

                //message
                lblMessage.Font = ErrorWinDefines.MessageFont;
                lblMessage.ForeColor = ErrorWinDefines.MessageFontColor;
                this.lblMessage.Text = new string(msgdata.winError.message);
                
                //bottom panel
                //show the bottom panel since for showing win error window, 
                //no bottom controls are needed
                panelBottom.Visible = true;

                //for the bottom panel, we need to add the buttons. Here we are using the BottomButtonsData
                //since the win error message has the bottom controls similiar to that of the bottom section
                //So, create a new GuiBottomButtonsData and populate the structure before calling the LoadBottomButtons
                //method
                GuiBottomButtonsData_t data = new GuiBottomButtonsData_t();  
                data.buttonText = Util.OneCharArrayToTwoCharArray(msgdata.winError.buttonText);
                data.buttonHighlighted[0] = GuiDefines.HIGHLIGHT;//we are forcing it to highlight
                LoadBottomButtons(GuiDefines.ONEBUTTON, data, msg.msgId);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Load Window Error Screen in WinMessageControls.cs");
            }
        }        

        /// <summary>
        /// Load the Window Warning 1 screen with custom colors and fonts
        /// </summary>
        /// <param name="msg">data related to the screen</param>
        private void LoadWinWarning1(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t msgdata = msg.data;
                //back color                
                this.BackColor = WarningWinDefines.BackColor;

                //top title color
                lblTopTitle.BackColor = WarningWinDefines.TitleBackColor;
                lblTopTitle.Font = WarningWinDefines.TitleFont;
                lblTopTitle.ForeColor = WarningWinDefines.TitleFontColor;
                lblTopTitle.Text = new string(msgdata.winWarning.topLabel);

                //message
                lblMessage.Font = WarningWinDefines.MessageFont;
                this.lblMessage.Text = new string(msgdata.winWarning.message);

                //bottom panel
                //show the bottom panel since for showing win error window, 
                //no bottom controls are needed
                panelBottom.Visible = true;

                //for the bottom panel, we need to add the buttons. Here we are using the BottomButtonsData
                //since the win error message has the bottom controls similiar to that of the bottom section
                //So, create a new GuiBottomButtonsData and populate the structure before calling the LoadBottomButtons
                //method
                GuiBottomButtonsData_t data = new GuiBottomButtonsData_t();
                data.buttonText = msgdata.winWarning.buttonText;
                data.buttonHighlighted[0] = GuiDefines.HIGHLIGHT;
                LoadBottomButtons(GuiDefines.ONEBUTTON, data, msg.msgId);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Load Window Warning1 Screen in WinMessageControls.cs");
            }
        }

        /// <summary>
        /// Load the Window Warning 2 screen with custom colors and fonts
        /// </summary>
        /// <param name="msg">data related to the screen</param>
        private void LoadWinWarning2(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t msgdata = msg.data;

                //back color                
                this.BackColor = WarningWinDefines.BackColor;

                //top title color
                lblTopTitle.BackColor = WarningWinDefines.TitleBackColor;
                lblTopTitle.Font = WarningWinDefines.TitleFont;
                lblTopTitle.ForeColor = WarningWinDefines.TitleFontColor;
                lblTopTitle.Text = new string(msgdata.winWarning.topLabel);

                //message
                lblMessage.Font = WarningWinDefines.MessageFont;
                this.lblMessage.Text = new string(msgdata.winWarning.message);

                //bottom panel
                //show the bottom panel since for showing win error window, 
                //no bottom controls are needed
                panelBottom.Visible = true;

                //for the bottom panel, we need to add the buttons. Here we are using the BottomButtonsData
                //since the win error message has the bottom controls similiar to that of the bottom section
                //So, create a new GuiBottomButtonsData and populate the structure before calling the LoadBottomButtons
                //method
                GuiBottomButtonsData_t data = new GuiBottomButtonsData_t();
                data.buttonText = msgdata.winWarning.buttonText;
                data.buttonHighlighted[1] = GuiDefines.HIGHLIGHT;
                LoadBottomButtons(GuiDefines.TWOBUTTONS, data, msg.msgId);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Load Window Warning2 Screen in WinMessageControls.cs");
            }
        }

        internal void ClearBottomControls()
        {
            panelBottom.Controls.Clear();
        }

        /// <summary>
        /// Add the buttons to the user control depending on the button count. Also set the colors of the button
        /// depending on the highlight flag
        /// </summary>
        /// <param name="count">number of buttons to be placed in the bottom section user control</param>
        /// <param name="msgdata">data related to the bottom section controls</param>
        private void LoadBottomButtons(int count, GuiBottomButtonsData_t msgdata, int msgId)
        {
            try
            {
                string[] buttonText = Util.TwoCharArrayToStringArray(msgdata.buttonText);

                //dynamically start creating the buttons and place them at the correct location
                for (int i = 0; i < count; i++)
                {
                    ImageButton button = new ImageButton();
                    button.Click += new System.EventHandler(BottomButton_Click);
                    button.ButtonStyle = ImageButton.ButtonType.GradientImageButton;

                    button.Location = new Point(i * panelBottom.ClientRectangle.Width / count, 0);
                    if (count == 1)//only one button on the bottom section
                    {
                        button.Dock = DockStyle.Fill;
                    }
                    else
                    {
                        button.Size = new Size(panelBottom.ClientRectangle.Width / count, panelBottom.ClientRectangle.Height);
                    }

                    //set default properties for the buttons
                    if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR))
                    {
                        button.Font = ErrorWinDefines.ButtonFont;
                        button.ForeColor = ErrorWinDefines.NormalButtonFontColor;
                        button.PressedForeColor = ErrorWinDefines.PressedNormalButtonFontColor;
                    }
                    else if ((msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1)) || (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2)))
                    {
                        button.Font = WarningWinDefines.ButtonFont;
                        button.ForeColor = WarningWinDefines.NormalButtonFontColor;
                        button.PressedForeColor = WarningWinDefines.PressedNormalButtonFontColor;
                    }
                    else if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2))
                    {
                        button.Font = CriticalStopWinDefines.ButtonFont;
                        button.ForeColor = CriticalStopWinDefines.NormalButtonFontColor;
                        button.PressedForeColor = CriticalStopWinDefines.PressedNormalButtonFontColor;
                    }

                    button.BorderStyle = Resco.Controls.RescoBorderStyle.FixedSingle;
                    button.FocusedBorderColor = Color.Transparent;
                    button.BorderCornerSize = 0;
                    button.VistaButtonInflate = new Size(-1, -1);
                    button.Tag = i;

                    button.Text = buttonText[i];

                    string fillDirection = "Vertical";//default
                    if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR))
                    {
                        fillDirection = ErrorWinDefines.FillDirection;
                    }
                    else if ((msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1)) || (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2)))
                    {
                        fillDirection = WarningWinDefines.FillDirection;
                    }
                    else if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2))
                    {
                        fillDirection = CriticalStopWinDefines.FillDirection;
                    }

                    if (fillDirection == "Vertical")
                    {
                        button.GradientColors.FillDirection = Resco.Controls.OutlookControls.FillDirection.Vertical;
                        button.GradientColorsPressed.FillDirection = Resco.Controls.OutlookControls.FillDirection.Vertical;
                    }
                    else
                    {
                        button.GradientColors.FillDirection = Resco.Controls.OutlookControls.FillDirection.Horizontal;
                        button.GradientColorsPressed.FillDirection = Resco.Controls.OutlookControls.FillDirection.Horizontal;
                    }

                    if (msgdata.buttonHighlighted[i] != GuiDefines.NORMAL)
                    {
                        //HIGHLIGHT BUTTON
                        if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR))
                        {
                            button.GradientColors.StartColor = ErrorWinDefines.HighlightStartButtonColor;
                            button.GradientColors.EndColor = ErrorWinDefines.HighlightEndButtonColor;
                            button.GradientColorsPressed.StartColor = ErrorWinDefines.PressedHighlightStartButtonColor;
                            button.GradientColorsPressed.EndColor = ErrorWinDefines.PressedHighlightEndButtonColor;

                            button.BorderColor = ErrorWinDefines.HighlightButtonBorderColor;
                            button.PressedBorderColor = ErrorWinDefines.HighlightButtonBorderColor;
                        }
                        else if ((msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1)) || (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2)))
                        {
                            button.GradientColors.StartColor = WarningWinDefines.HighlightStartButtonColor;
                            button.GradientColors.EndColor = WarningWinDefines.HighlightEndButtonColor;
                            button.GradientColorsPressed.StartColor = WarningWinDefines.PressedHighlightStartButtonColor;
                            button.GradientColorsPressed.EndColor = WarningWinDefines.PressedHighlightEndButtonColor;

                            button.PressedBorderColor = WarningWinDefines.PressedHighlightButtonBorderColor;
                            button.BorderColor = WarningWinDefines.HighlightButtonBorderColor;                            
                        }
                        else if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2))
                        {
                            button.GradientColors.StartColor = CriticalStopWinDefines.HighlightStartButtonColor;
                            button.GradientColors.EndColor = CriticalStopWinDefines.HighlightEndButtonColor;
                            button.GradientColorsPressed.StartColor = CriticalStopWinDefines.PressedHighlightStartButtonColor;
                            button.GradientColorsPressed.EndColor = CriticalStopWinDefines.PressedHighlightEndButtonColor;

                            button.PressedBorderColor = CriticalStopWinDefines.PressedHighlightButtonBorderColor;
                            button.BorderColor = CriticalStopWinDefines.HighlightButtonBorderColor;                
                        }
                    }
                    else
                    {
                        //NORMAL BUTTON
                        if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_ERROR))
                        {
                            button.GradientColors.StartColor = ErrorWinDefines.NormalStartButtonColor;
                            button.GradientColors.EndColor = ErrorWinDefines.NormalEndButtonColor;
                            button.GradientColorsPressed.StartColor = ErrorWinDefines.PressedNormalStartButtonColor;
                            button.GradientColorsPressed.EndColor = ErrorWinDefines.PressedNormalEndButtonColor;

                            button.BorderColor = ErrorWinDefines.NormalButtonBorderColor;
                            button.PressedBorderColor = ErrorWinDefines.NormalButtonBorderColor;
                        }
                        else if ((msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING1)) || (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_WARNING2)))
                        {
                            button.GradientColors.StartColor = WarningWinDefines.NormalStartButtonColor;
                            button.GradientColors.EndColor = WarningWinDefines.NormalEndButtonColor;
                            button.GradientColorsPressed.StartColor = WarningWinDefines.PressedNormalStartButtonColor;
                            button.GradientColorsPressed.EndColor = WarningWinDefines.PressedNormalEndButtonColor;

                            button.PressedBorderColor = WarningWinDefines.PressedNormalButtonBorderColor;
                            button.BorderColor = WarningWinDefines.NormalButtonBorderColor;
                        }
                        else if (msgId == ((int)GuiMessages.GuiMsgId_t.MSG_GUI_WIN_CRITICAL_STOP2))
                        {
                            button.GradientColors.StartColor = CriticalStopWinDefines.NormalStartButtonColor;
                            button.GradientColors.EndColor = CriticalStopWinDefines.NormalEndButtonColor;
                            button.GradientColorsPressed.StartColor = CriticalStopWinDefines.PressedNormalStartButtonColor;
                            button.GradientColorsPressed.EndColor = CriticalStopWinDefines.PressedNormalEndButtonColor;

                            button.BorderColor = CriticalStopWinDefines.NormalButtonBorderColor;
                            button.PressedBorderColor = CriticalStopWinDefines.NormalButtonBorderColor;
                        }
                    }

                    panelBottom.Controls.Add(button);
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to load the bottom buttons in WinMessageControls.cs");
            }
        }   

        private void btnInfo_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendInfoMessage();
        }

        private void BottomButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendBottomButtonMessage(sender);
        }
    }
}
