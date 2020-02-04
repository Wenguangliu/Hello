using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;
using System.Collections;
using Resco.Controls.OutlookControls;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>  
    /// BUTTONS + INFO CONTROL
    /// This middle sub section has button controls and a info button control. The info button control becomes visible or not 
    /// depending on the message Id received from the message queue. Also the number of buttons appearing on the screen depends on the 
    /// message Id. For Buttons in the middle sub section user controls, we are using images for buttons instead of just a gradient button control from Resco since
    /// the Resco gradient controls dont have the property that can set the border thickness
    /// </summary>
    public partial class ButtonsInfoControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private Messaging.SendQueue SendMessageToQueue;
       // LogError log = new LogError();

        public ButtonsInfoControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnInfo.Text = "";//clear the button image text
            btnInfo.ImageDefault = Properties.Resources.Info_Icon;
        }

        /// <summary>
        /// Sets the correct controls on the user control screen depending on the message Id received. We have used images for the buttons
        /// instead of the Resco out of the box controls
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetButtonsInfoControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;
                ClearButtons();

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS_INFO):
                        {
                            string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleButtonsInfo.buttonText);
                            char[] buttonHighlighted = messageData.middleButtonsInfo.buttonHighlighted;

                            LoadButtons(messageData, buttonText, buttonHighlighted);

                            imageButton0.Text = buttonText[0];
                            imageButton1.Text = buttonText[1];
                            imageButton2.Text = buttonText[2];
                            imageButton3.Text = buttonText[3];
                            
                            //make only those buttons visible 
                            if (buttonText[0] != string.Empty)
                            {
                                imageButton0.Visible = true;
                            }

                            if (buttonText[1] != string.Empty)
                            {
                                imageButton1.Visible = true;
                            }

                            if (buttonText[2] != string.Empty)
                            {
                                imageButton2.Visible = true;
                            }

                            if (buttonText[3] != string.Empty)
                            {
                                imageButton3.Visible = true;
                            }                            

                            btnInfo.Visible = true;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS):
                        {
                            string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleButtons.buttonText);
                            char[] buttonHighlighted = messageData.middleButtons.buttonHighlighted;

                            LoadButtons(messageData, buttonText, buttonHighlighted);

                            imageButton0.Text = buttonText[0];
                            imageButton1.Text = buttonText[1];
                            imageButton2.Text = buttonText[2];
                            imageButton3.Text = buttonText[3];
                            imageButton4.Text = buttonText[4];
                            imageButton5.Text = buttonText[5];

                            //make only those buttons visible                            
                            if (buttonText[0] != string.Empty)
                            {
                                imageButton0.Visible = true;
                            }

                            if (buttonText[1] != string.Empty)
                            {
                                imageButton1.Visible = true;
                            }

                            if (buttonText[2] != string.Empty)
                            {
                                imageButton2.Visible = true;
                            }

                            if (buttonText[3] != string.Empty)
                            {
                                imageButton3.Visible = true;
                            }

                            if (buttonText[4] != string.Empty)
                            {
                                imageButton4.Visible = true;
                            }

                            if (buttonText[5] != string.Empty)
                            {
                                imageButton5.Visible = true;
                            }                            

                            btnInfo.Visible = false;

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in ButtonsInfoControl.cs");
            }
        }

        /// <summary>
        /// Make the buttons invisible to begin with
        /// </summary>
        private void ClearButtons()
        {
            imageButton0.Visible = false;
            imageButton1.Visible = false;
            imageButton2.Visible = false;
            imageButton3.Visible = false;
            imageButton4.Visible = false;
            imageButton5.Visible = false;
        }

        /// <summary>
        /// Load the correct set of buttons. Sets the default properties of the button by looping through the control set
        /// inside the user control. Also inserts the highlight image to the button which needs to be highlighted
        /// </summary>
        /// <param name="messagedata">message data for the recieved message</param>
        /// <param name="buttonText">text that goes on the buttons loaded</param>
        /// <param name="buttonHighlighted">tells if the button needs to be highlighted or not</param>
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
               // IEnumerator iEnum = Controls.GetEnumerator();
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
                SendMessageToQueue.SendLogErrorMessage("Unable to load the buttons in SetButtonsInfoControl.cs");
            }
        }

        /// <summary>
        /// Send the message to the receiving process indicating that the user has clicked on the Info button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnInfo_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendInfoMessage();
        }       

        /// <summary>
        /// Send the message to the receiving process indicating that the user has clicked on one of the buttons
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        } 
    }
}