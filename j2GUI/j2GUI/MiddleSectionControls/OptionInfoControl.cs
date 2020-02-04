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

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// RADIO BUTTON + INFO CONTROL
    /// This middle sub section has radio button controls and a info button control. The info button control becomes visible or not 
    /// depending on the message Id received from the message queue.
    /// </summary>
    public partial class OptionInfoControl : UserControl
    {
        private MiddleSectionRadioButton RadioButtonDefines = new MiddleSectionRadioButton();
        private Utilities Util = new Utilities();
        private Messaging.SendQueue SendMessageToQueue;

        public OptionInfoControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnInfo.Text = "";//clear the button image text
            btnInfo.ImageDefault = Properties.Resources.Info_Icon;
        }

        /// <summary>
        /// Set the controls depending on the message Id received in the receive data
        /// </summary>
        /// <param name="msg"></param>
        internal void SetOptionInfoControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;
                ClearRadioButtons();

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_OPTION_INFO):
                        {

                            string[] radioButtonText = Util.TwoCharArrayToStringArray(messagedata.middleOptionInfo.buttonText);
                            int radioButtonSelected = messagedata.middleOptionInfo.selectedButton;

                            LoadRadioButton(msg, radioButtonSelected);

                            //make only those radio buttons visible 

                            if (radioButtonText[0] != string.Empty)
                            {
                                touchRadioButton1.Visible = true;
                            }

                            if (radioButtonText[1] != string.Empty)
                            {
                                touchRadioButton2.Visible = true;
                            }

                            if (radioButtonText[2] != string.Empty)
                            {
                                touchRadioButton3.Visible = true;
                            }

                            if (radioButtonText[3] != string.Empty)
                            {
                                touchRadioButton4.Visible = true;

                            }

                            if (radioButtonText[4] != string.Empty)
                            {
                                touchRadioButton5.Visible = true;
                            }

                            touchRadioButton1.Text = "   " + radioButtonText[0];
                            touchRadioButton2.Text = "   " + radioButtonText[1];
                            touchRadioButton3.Text = "   " + radioButtonText[2];
                            touchRadioButton4.Text = "   " + radioButtonText[3];
                            touchRadioButton5.Text = "   " + radioButtonText[4];

                            btnInfo.Visible = true;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_OPTION):
                        {
                            string[] radioButtonText = Util.TwoCharArrayToStringArray(messagedata.middleSelectOption.buttonText);
                            int radioButtonSelected = messagedata.middleSelectOption.selectedButton;

                            LoadRadioButton(msg, radioButtonSelected);

                            //make only those radio buttons visible 

                            if (radioButtonText[0] != string.Empty)
                            {
                                touchRadioButton1.Visible = true;
                            }

                            if (radioButtonText[1] != string.Empty)
                            {
                                touchRadioButton2.Visible = true;
                            }

                            if (radioButtonText[2] != string.Empty)
                            {
                                touchRadioButton3.Visible = true;
                            }

                            if (radioButtonText[3] != string.Empty)
                            {
                                touchRadioButton4.Visible = true;
                            }

                            if (radioButtonText[4] != string.Empty)
                            {
                                touchRadioButton5.Visible = true;
                            }

                            if (radioButtonText[5] != string.Empty)
                            {
                                touchRadioButton6.Visible = true;
                            }

                            touchRadioButton1.Text = "   " + radioButtonText[0];
                            touchRadioButton2.Text = "   " + radioButtonText[1];
                            touchRadioButton3.Text = "   " + radioButtonText[2];
                            touchRadioButton4.Text = "   " + radioButtonText[3];
                            touchRadioButton5.Text = "   " + radioButtonText[4];
                            touchRadioButton6.Text = "   " + radioButtonText[5];

                            btnInfo.Visible = false;

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the option controls in SetOptionInfoControl.cs");
            }
        }

        /// <summary>
        /// Make all the radio buttons invisible to beginwith. Then later on depending
        /// on the message data received, it can be made visible or not
        /// </summary>
        private void ClearRadioButtons()
        {
            touchRadioButton1.Visible = false;
            touchRadioButton2.Visible = false;
            touchRadioButton3.Visible = false;
            touchRadioButton4.Visible = false;
            touchRadioButton5.Visible = false;
            touchRadioButton6.Visible = false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <param name="radioButtonSelected"></param>
        private void LoadRadioButton(GuiReceiveMsg_t message, int radioButtonSelected)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = message.data;
                //int buttonCount = 0;//reset

                //iterate thro' ALL the controls on the user control and then set properties for radio buttons only
                IEnumerator iEnum = touchPanel1.Controls.GetEnumerator();
                iEnum.Reset();

                while (iEnum.MoveNext())
                {
                    object obj = iEnum.Current;
                    string typeName = obj.GetType().Name;

                    //we are interested only in image buttons
                    if (typeName == "TouchRadioButton")
                    {
                        Resco.Controls.CommonControls.TouchRadioButton radioButton = (Resco.Controls.CommonControls.TouchRadioButton)obj;

                        //only the button image controls properties need to be set
                        if (radioButton.Name.Contains("touchRadioButton"))
                        {
                            //set the default properties for ALL the buttons. Even the buttons which are invisible to keep the coding neat
                            radioButton.Text = "";//remove the default text
                            radioButton.CheckedImage = Properties.Resources._75x75_radio_down;
                            radioButton.UncheckedImage = Properties.Resources._75x75_radio_normal;
                            radioButton.Checked = false;

                            radioButton.Font = RadioButtonDefines.Font;
                            radioButton.ForeColor = RadioButtonDefines.NormalFontColor;

                            radioButton.Visible = false;//make all the buttons invisible to begin with
                            radioButton.CheckBoxBorder = false;                         

                            LoadSelected(radioButtonSelected);
                        }
                    }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to load the buttons in LoadCheckBoxes.cs");
            }
        }

        /// <summary>
        /// Select the radio button depending on the data received from the gui message queue
        /// </summary>
        /// <param name="radioButtonSelected">the radio button that is selected</param>
        private void LoadSelected(int radioButtonSelected)
        {
            if (radioButtonSelected == 0)
            {
                touchRadioButton1.Checked = true;
            }

            if (radioButtonSelected == 1)
            {
                touchRadioButton2.Checked = true;
            }

            if (radioButtonSelected == 2)
            {
                touchRadioButton3.Checked = true;
            }

            if (radioButtonSelected == 3)
            {
                touchRadioButton4.Checked = true;
            }

            if (radioButtonSelected == 4)
            {
                touchRadioButton5.Checked = true;
            }

            if (radioButtonSelected == 5)
            {
                touchRadioButton6.Checked = true;
            }
        }

        /// <summary>
        /// User has clicked on the Info button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnInfo_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendInfoMessage();    
        }     

        /// <summary>
        /// The user has selected a radio button. Notify the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void touchRadioButton_MouseUp(object sender, MouseEventArgs e)
        {
            SendMessageToQueue.SendToggleMessage(sender);
        }
    }
}
