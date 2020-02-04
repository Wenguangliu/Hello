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
    /// PROGRESS BAR CONTROL
    /// This middle sub section has controls to show the progress of a task. Also depending on the message Id, few of the controls are shown
    /// or not shown
    /// </summary>
    public partial class ProgessControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionProgressBar ProgressBarDefines = new MiddleSectionProgressBar();
        private Messaging.SendQueue SendMessageToQueue;

        public ProgessControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnRecall.Text = "";//clear the button image text
            btnRecall.ImageDefault = Properties.Resources.Recall_Icon;

            btnSettings.Text = "";
            btnSettings.ImageDefault = Properties.Resources.Settings_Icon;

            btnFactory.Text = "";
            btnFactory.ImageDefault = Properties.Resources.FactorySettings_Icon;

            //set the properties of the progress bar
            progressBar1.Font = ProgressBarDefines.Font;
            progressBar1.ForeColor = ProgressBarDefines.FontColor;

            //progress colors
            progressBar1.BarGradientColor.StartColor = ProgressBarDefines.StartBarColor;
            progressBar1.BarGradientColor.EndColor = ProgressBarDefines.EndBarColor;

            //progress bar solid background color
            progressBar1.BackgroundColor = ProgressBarDefines.BarBackColor;           

            lblAnalysisMessage.Font = ProgressBarDefines.Font;
            lblAnalysisMessage.ForeColor = ProgressBarDefines.FontColor;
        }

        /// <summary>
        /// Sets the controls on the middle sub section depending on the message Id
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetProgressControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;
                lblAnalysisMessage.BackColor = Color.Transparent;// for few of the message IDs, there should not be a color block
                lblAnalysisMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.MiddleCenter;//set it to default
                lblAnalysisMessage.ForeColor = ProgressBarDefines.FontColor;//set it to default

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_MFG):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_RD): //added nwr 5/14/2013
                        {
                            //here for some reason Resco progress bar does not take a string constructor with char array with null termination as a null terminated string. So
                            //we are using a label control to first assign the text and then we are assigning the label text to the progress bar text as a work around.
                            lblMessage.Text = new string(messageData.middleHomeWarming.progBarText);
                            progressBar1.TextOutputFormat = lblMessage.Text;
                            progressBar1.Value = messageData.middleHomeWarming.progBarValue;

                            btnRecall.Visible = true;
                            btnSettings.Visible = true;

                            lblAnalysisMessage.Visible = false;

                            if (msg.msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_MFG)
                            {
                                btnFactory.Visible = true;
                            }
                            else if (msg.msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_RD)
                            {
                                //added nwr 5/14/2013
                                btnFactory.ImageDefault = Properties.Resources.FactorySettingsRD_Icon;
                                btnFactory.Visible = true;
                            }
                            else
                            {
                                btnFactory.Visible = false;
                            }
                            
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS):
                        {
                            lblAnalysisMessage.Size = new Size(450, 150);                            
                            lblAnalysisMessage.ForeColor = Color.FromArgb((int)messageData.middleProgBarMessage.fontColor);
                            if ((int)messageData.middleProgBarMessage.backgroundColor == 0x00FFFFFF)//bug fix 01/11/12. White color background for Rotor 1 was showing as transparent color since the Console sends the Color as 0x00FFFFFF instead of 0xFFFFFFFF(ARGB - Alpha, red, green, blue)
                            {
                                lblAnalysisMessage.BackColor = Color.White;
                            }
                            else
                            {
                                lblAnalysisMessage.BackColor = Color.FromArgb((int)messageData.middleProgBarMessage.backgroundColor);
                            }

                            lblAnalysisMessage.Text = new string(messageData.middleProgBarMessage.message);

                            //here for some reason Resco progress bar does not take a string constructor with char array with null termination as a null terminated string. So
                            //we are using a label control to first assign the text and then we are assigning the label text to the progress bar text as a work around.
                            lblMessage.Text = new string(messageData.middleProgBarMessage.progBarText);
                            progressBar1.TextOutputFormat = lblMessage.Text;
                            progressBar1.Value = messageData.middleProgBarMessage.progBarValue;

                            btnRecall.Visible = false;
                            btnSettings.Visible = false;
                            btnFactory.Visible = false;

                            lblAnalysisMessage.Visible = true;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_PROG_BAR_MESSAGE):
                        {
                            //used for CD update, Service Archive, Transmit All and Transmit Date Range too
                            lblAnalysisMessage.Size = new Size(450, 360);
                            lblAnalysisMessage.Text = new string(messageData.middleProgBarMessage.message);
                            
                            //here for some reason Resco progress bar does not take a string constructor with char array with null termination as a null terminated string. So
                            //we are using a label control to first assign the text and then we are assigning the label text to the progress bar text as a work around.
                            lblMessage.Text = new string(messageData.middleProgBarMessage.progBarText);
                            progressBar1.TextOutputFormat = lblMessage.Text;
                            progressBar1.Value = messageData.middleProgBarMessage.progBarValue;

                            btnRecall.Visible = false;
                            btnSettings.Visible = false;
                            btnFactory.Visible = false;

                            lblAnalysisMessage.Visible = true;
                            lblAnalysisMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter; 

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the progress bar in ProgressControl.cs");
            }
        }

        /// <summary>
        /// Update the progress bar value and the analysis message text depending on the udpate message received by the
        /// messasge queue
        /// </summary>
        /// <param name="msg"></param>
        internal void UpdateProgressControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT):
                        {
                            lblAnalysisMessage.Text = new string(messageData.middleProgBarMessage.message);

                            lblMessage.ForeColor = Color.FromArgb((int)messageData.middleProgBarMessage.fontColor);
                            lblMessage.BackColor = Color.FromArgb((int)messageData.middleProgBarMessage.backgroundColor);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS):
                        {
                            //Update the progress bar control
                            progressBar1.Value = messageData.middleProgBarMessage.progBarValue;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_PROGRESS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS):
                        {
                            //Update the progress bar control
                            progressBar1.Value = messageData.middleHomeWarming.progBarValue;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT):
                        {
                            lblAnalysisMessage.Text = new string(messageData.middleProgBarMessage.message);

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the progress bar in ProgressControl.cs");
            }
        }

        /// <summary>
        /// Send the Recall button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRecall_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_RECALL);
        }

        /// <summary>
        /// Send the Settings button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnSettings_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_SETTINGS);
        }

        /// <summary>
        /// Send the Manufacturing/Factory button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnFactory_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_MANUFACTURING);
        }               
    }
}
