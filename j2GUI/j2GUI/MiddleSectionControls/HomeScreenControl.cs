using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>  
    /// BUTTONS + RECALL AND SETTINGS IMAGE CONTROLS
    /// This middle sub section has button controls, recall and settings image controls. The number of buttons displayed depends on the message
    /// Id received.
    /// </summary>
    public partial class HomeScreenControl : UserControl
    {
        Utilities Util = new Utilities();
        MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private Messaging.SendQueue SendMessageToQueue;

        public HomeScreenControl()
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
        }

        /// <summary>
        /// Sets the correct controls on the user control screen depending on the message Id received. We have used images for the buttons
        /// instead of the Resco out of the box controls
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetHomeScreenControl(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleHomeReady.buttonText);
                char[] buttonHighlighted = messageData.middleHomeReady.buttonHighlighted;

                LoadButtons(messageData, buttonText, buttonHighlighted);

                //make only those buttons visible whose text is not empty
                if (buttonText[0] != string.Empty)
                {
                    imageButton1.Visible = true;
                }

                if (buttonText[1] != string.Empty)
                {
                    imageButton2.Visible = true;
                }

                if (buttonText[2] != string.Empty)
                {
                    imageButton3.Visible = true;
                }

                imageButton1.Text = buttonText[0];
                imageButton2.Text = buttonText[1];
                imageButton3.Text = buttonText[2];

                btnRecall.Visible = true;
                btnSettings.Visible = true;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY):
                        {
                            btnFactory.Visible = false;
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_MFG):
                        {
                            btnFactory.Visible = true;
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_RD):
                        {
                            //added nwr 5/14/2013
                            btnFactory.ImageDefault = Properties.Resources.FactorySettingsRD_Icon;
                            btnFactory.Visible = true;
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in SetHomeScreenControl.cs");
            }
        }

        /// <summary>
        /// Load the correct set of buttons. Set the default properties of the button by looping through the control set
        /// inside the user control. Also insert the highlight image to the button which needs to be highlighted
        /// </summary>
        /// <param name="messagedata">message data for the received message</param>
        /// <param name="buttonText">button text that goes on the button</param>
        /// <param name="buttonHighlighted">indicates whether the displayed button needs to be highlighted or not</param>
        private void LoadButtons(GuiReceiveMsgData_t messagedata, string[] buttonText, char[] buttonHighlighted)
        {
            try
            {
                int highlight = 0;

                //get the button name which needs to be highlighted
                for (int i = 0; i < (buttonText.GetUpperBound(0) + 1); i++)
                {
                    if (buttonHighlighted[i] != GuiDefines.NORMAL)
                    {
                        highlight = i;
                        break;
                    }
                }
                string btnHighlightName = "imageButton" + (highlight).ToString();//we do ++ since the button names are not zero-based

                //iterate thro' ALL the controls on the user control and then set properties for buttons only
                IEnumerator iEnum = touchPanel1.Controls.GetEnumerator();
                iEnum.Reset();
                while (iEnum.MoveNext())
                {
                    object obj = iEnum.Current;
                    string typeName = obj.GetType().Name;

                    //we are interested only in image buttons
                    if (typeName == "ImageButton")
                    {
                        Resco.Controls.OutlookControls.ImageButton button = (Resco.Controls.OutlookControls.ImageButton)obj;

                        //only the button image controls properties need to be set
                        if (button.Name.Contains("imageButton"))
                        {
                            //set the default properties for ALL the buttons. Even the buttons which are invisible to keep the coding neat
                            button.Text = "";//remove the default text
                            button.ImageDefault = Properties.Resources._442x75_normal;
                            button.ImagePressed = Properties.Resources._442x75_down;
                            button.Font = ButtonDefines.Font;
                            button.ForeColor = ButtonDefines.NormalFontColor;
                            button.PressedForeColor = ButtonDefines.PressedFontColor;
                            button.Visible = false;//make all the buttons invisible to begin with

                            //highlighted button has a different image than normal buttons
                            if (button.Name == btnHighlightName)
                            {
                                button.ImageDefault = Properties.Resources._442x75_highlighted;
                            }
                        }
                    }
                }

            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to load the buttons in LoadButtons.cs");
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
        /// Send the Factory button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnFactory_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_MANUFACTURING);
        }

        /// <summary>
        /// Send the button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }
    }
}
