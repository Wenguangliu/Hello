using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Collections;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// CHECK BOX + INFO CONTROL
    /// This middle sub section has check box controls and a info button control. The info button control becomes visible or not 
    /// depending on the message Id received from the message queue.
    /// </summary>
    public partial class SelectInfoControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionCheckBox CheckBoxDefines = new MiddleSectionCheckBox();
        private Messaging.SendQueue SendMessageToQueue;

        public SelectInfoControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            btnInfo.Text = "";//clear the button image text
            btnInfo.ImageDefault = Properties.Resources.Info_Icon;
        }

        /// <summary>
        /// Sets the middle sub section user control to the correct set of user controls depending on the message Id received 
        /// by the GUI message
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetSelectInfoControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;
                ClearCheckBoxes();

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_INFO):
                        {
                            string[] checkBoxText = Util.TwoCharArrayToStringArray(messagedata.middleMultiInfo.buttonText);
                            char[] buttonChecked = messagedata.middleMultiInfo.buttonChecked;

                            LoadCheckBoxes(msg, buttonChecked);

                            //make only those check boxes visible    
                            if (checkBoxText[0] != string.Empty)
                            {
                                touchCheckBox1.Visible = true;
                            }

                            if (checkBoxText[1] != string.Empty)
                            {
                                touchCheckBox2.Visible = true;
                            }

                            if (checkBoxText[2] != string.Empty)
                            {
                                touchCheckBox3.Visible = true;
                            }

                            if (checkBoxText[3] != string.Empty)
                            {
                                touchCheckBox4.Visible = true;
                            }                            

                            touchCheckBox1.Text = "   " + checkBoxText[0];
                            touchCheckBox2.Text = "   " + checkBoxText[1];
                            touchCheckBox3.Text = "   " + checkBoxText[2];
                            touchCheckBox4.Text = "   " + checkBoxText[3];                            
                            
                            btnInfo.Visible = true;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_SELECT):
                        {
                            string[] checkBoxText = Util.TwoCharArrayToStringArray(messagedata.middleMultiSelect.buttonText);
                            char[] buttonChecked = messagedata.middleMultiSelect.buttonChecked;

                            LoadCheckBoxes(msg, buttonChecked);                         

                            //make only those check boxes visible    
                            if (checkBoxText[0] != string.Empty)
                            {
                                touchCheckBox1.Visible = true;
                            }

                            if (checkBoxText[1] != string.Empty)
                            {
                                touchCheckBox2.Visible = true;
                            }

                            if (checkBoxText[2] != string.Empty)
                            {
                                touchCheckBox3.Visible = true;
                            }

                            if (checkBoxText[3] != string.Empty)
                            {
                                touchCheckBox4.Visible = true;
                            }

                            if (checkBoxText[4] != string.Empty)
                            {
                                touchCheckBox5.Visible = true;
                            }

                            if (checkBoxText[5] != string.Empty)
                            {
                                touchCheckBox6.Visible = true;
                            }

                            touchCheckBox1.Text = "   " + checkBoxText[0];
                            touchCheckBox2.Text = "   " + checkBoxText[1];
                            touchCheckBox3.Text = "   " + checkBoxText[2];
                            touchCheckBox4.Text = "   " + checkBoxText[3];
                            touchCheckBox5.Text = "   " + checkBoxText[4];
                            touchCheckBox6.Text = "   " + checkBoxText[5];

                            btnInfo.Visible = false;

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the Select controls in SetSelectInfoControl.cs");
            }
        }

        /// <summary>
        /// Make all the check boxes invisible to begin with and then decide whether to show or not
        /// depending on the message data received on the Gui message queue
        /// </summary>
        private void ClearCheckBoxes()
        {
            touchCheckBox1.Visible = false;
            touchCheckBox2.Visible = false;
            touchCheckBox3.Visible = false;
            touchCheckBox4.Visible = false;
            touchCheckBox5.Visible = false;
            touchCheckBox6.Visible = false;
        }

        /// <summary>
        /// Load all the check boxes with the correct images and properties
        /// </summary>
        /// <param name="message">received message structure</param>        
        /// <param name="buttonChecked">check box checked status array</param>
        private void LoadCheckBoxes(GuiReceiveMsg_t message, char[] buttonChecked)
        {
            //iterate thro' ALL the controls on the user control and then set properties for check boxes only
            IEnumerator iEnum = touchPanel1.Controls.GetEnumerator();
            iEnum.Reset();

            while (iEnum.MoveNext())
            {
                object obj = iEnum.Current;
                string typeName = obj.GetType().Name;

                //we are interested only in image buttons
                if (typeName == "TouchCheckBox")
                {
                    Resco.Controls.CommonControls.TouchCheckBox checkBox = (Resco.Controls.CommonControls.TouchCheckBox)obj;

                    //only the button image controls properties need to be set
                    if (checkBox.Name.Contains("touchCheckBox"))
                    {
                        //set the default properties for ALL the buttons. Even the buttons which are invisible to keep the coding neat
                        checkBox.Text = "";//remove the default text
                        checkBox.CheckedImage = Properties.Resources._75x75_check_down;
                        checkBox.UncheckedImage = Properties.Resources._75x75_radio_normal;
                        checkBox.Checked = false;

                        checkBox.Font = CheckBoxDefines.Font;
                        checkBox.ForeColor = CheckBoxDefines.NormalFontColor;

                        checkBox.Visible = false;//make all the buttons invisible to begin with
                        //checkBox.BoxColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                        //checkBox.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                        checkBox.CheckBoxBorder = false;
                    }
                }
            }

            LoadChecks(message.msgId, buttonChecked);
        }

        /// <summary>
        /// Check the check boxes only if the data in the received message queue has been set
        /// </summary>
        /// <param name="msgId">message Id for the received message</param>
        /// <param name="buttonChecked">check boxes checked status array</param>
        private void LoadChecks(int msgId, char[] buttonChecked)
        {
            if (buttonChecked[0] != GuiDefines.FALSE)
            {
                touchCheckBox1.Checked = true;
            }

            if (buttonChecked[1] != GuiDefines.FALSE)
            {
                touchCheckBox2.Checked = true;
            }

            if (buttonChecked[2] != GuiDefines.FALSE)
            {
                touchCheckBox3.Checked = true;
            }

            if (buttonChecked[3] != GuiDefines.FALSE)
            {
                touchCheckBox4.Checked = true;
            }

            if (msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_SELECT)
            {
                if (buttonChecked[4] != GuiDefines.FALSE)
                {
                    touchCheckBox5.Checked = true;
                }

                if (buttonChecked[5] != GuiDefines.FALSE)
                {
                    touchCheckBox6.Checked = true;
                }
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
        /// The user has changed the state of a check box. Notify the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>       
        private void touchCheckBox_MouseUp(object sender, MouseEventArgs e)
        {            
            SendMessageToQueue.SendToggleMessage(sender);
        }
       
    }
}
