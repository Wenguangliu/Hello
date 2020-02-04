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
    /// SELECT TYPE CONTROLS
    /// This middle sub section has controls like buttons and check boxes. The number of button depends on the message Id received
    /// by the Gui from the other processes like Recall and Analysis
    /// </summary>
    public partial class TypeControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionButton ButtonDefines = new MiddleSectionButton();
        private MiddleSectionTypeCheckBox CheckBoxDefines = new MiddleSectionTypeCheckBox();
        private Messaging.SendQueue SendMessageToQueue;

        public TypeControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
        }

        /// <summary>
        /// Depending on the message Id sent to the Gui message queue, the screen contents are dynamically set
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetTypeControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;                

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_TYPE):
                        {
                            string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleSelectType.buttonText);

                            LoadButtons();

                            LoadCheckBoxes();  

                            //make only those buttons visible whose text is not empty
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
                           
                            //set the text of all the controls
                            imageButton0.Text = buttonText[0];
                            imageButton1.Text = buttonText[1];
                            imageButton2.Text = buttonText[2];
                            imageButton3.Text = buttonText[3];
                            touchCheckBox1.Text = buttonText[4];
                            touchCheckBox2.Text = buttonText[5];

                            //set the checked state of the check boxes
                            if (messageData.middleSelectType.buttonChecked[0] != GuiDefines.FALSE)
                            {
                                touchCheckBox1.Checked = true;
                            }
                            else
                            {
                                touchCheckBox1.Checked = false;
                            }

                            if (messageData.middleSelectType.buttonChecked[1] != GuiDefines.FALSE)
                            {
                                touchCheckBox2.Checked = true;
                            }
                            else
                            {
                                touchCheckBox2.Checked = false;
                            }
                           
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in TypeControl.cs");
            }
        }

        /// <summary>
        /// Load the correct images for the check box and set their individual properties
        /// </summary>
        private void LoadCheckBoxes()
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

                        //checkBox.BoxColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                        //checkBox.BackColor = Color.FromArgb(GuiForm.FormRBackColor, GuiForm.FormGBackColor, GuiForm.FormBBackColor);
                        checkBox.CheckBoxBorder = false;
                    }
                }
            }
        }
                
        /// <summary>
        /// Load the correct set of buttons. Set the default properties of the button by looping through the control set
        /// inside the user control. Also insert the highlight image to the button which needs to be highlighted
        /// </summary>
        private void LoadButtons()
        {
            try
            {               
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
                        }
                    }
                }

            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the Load Buttons in TypeControl.cs");
            }
        }

        /// <summary>
        /// Button click event triggered. Notify the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
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
