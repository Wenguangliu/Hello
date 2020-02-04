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
using Resco.Controls.CommonControls;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// ENTER VALUE CONTROL
    /// This middle sub section has controls to enter the new values. The text boxes get the data from the other process.
    /// </summary>
    public partial class ValueControl : UserControl
    {
        private Utilities Util;
        private MiddleSectionButton ButtonDefines;
        private MiddleSectionTypeCheckBox CheckBoxDefines;
        private MiddleSectionEnterValue ValueDefines;
        private Timer RepeatTimer = new Timer();       
        private Messaging.SendQueue SendMessageToQueue;
        private bool IsUpPressed = true;
        private int UserTag;

        /// <summary>
        /// Set all the default properties for the controls
        /// </summary>
        public ValueControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    
            
            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            Util = new Utilities();
            ButtonDefines = new MiddleSectionButton();
            CheckBoxDefines = new MiddleSectionTypeCheckBox();
            ValueDefines = new MiddleSectionEnterValue();

            btnUp.Text = "";
            btnDown.Text = "";           

            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            imageButton0.ImageDefault = Properties.Resources._213x75_normal;
            imageButton0.ImagePressed = Properties.Resources._213x75_down;
            imageButton0.Font = ButtonDefines.Font;
            imageButton0.ForeColor = ButtonDefines.NormalFontColor;
            imageButton0.PressedForeColor = ButtonDefines.PressedFontColor;

            imageButton1.ImageDefault = Properties.Resources._213x75_normal;
            imageButton1.ImagePressed = Properties.Resources._213x75_down;
            imageButton1.Font = ButtonDefines.Font;
            imageButton1.ForeColor = ButtonDefines.NormalFontColor;
            imageButton1.PressedForeColor = ButtonDefines.PressedFontColor;

            lblCurrent.Font = ValueDefines.LabelFont;
            lblCurrent.ForeColor = ValueDefines.LabelFontColor;

            lblNew.Font = ValueDefines.LabelFont;
            lblNew.ForeColor = ValueDefines.LabelFontColor;

            txtCurrent.Font = ValueDefines.TextBoxFont;
            txtCurrent.ForeColor = ValueDefines.TextBoxFontColor;
            txtCurrent.DisabledForeColor = ValueDefines.TextBoxFontColor;

            txtNew.Font = ValueDefines.TextBoxFont;
            txtNew.ForeColor = ValueDefines.TextBoxFontColor;
        }
               
        /// <summary>
        /// Set the Enter Value middle sub section with the received message data.
        /// </summary>
        /// <param name="msg">recived message</param>
        internal void SetValueControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_VALUE):
                        {
                            lblCurrent.Text = new string(messageData.middleEnterValue.currentLabel);
                            lblNew.Text = new string(messageData.middleEnterValue.newLabel);
                            txtCurrent.Text = new string(messageData.middleEnterValue.currentValue);
                            txtNew.Text = new string(messageData.middleEnterValue.newValue);

                            imageButton0.Text = new string(messageData.middleEnterValue.button0Text);
                            imageButton1.Text = new string(messageData.middleEnterValue.button1Text);

                            string[] checkButtonText = Util.TwoCharArrayToStringArray(messageData.middleEnterValue.checkButtonText);

                            //set the check boxes with correct properties and images
                            LoadCheckBoxes();

                            //to begin with, do not show the check boxes
                            touchCheckBox1.Visible = false;
                            touchCheckBox2.Visible = false;                            

                            if (checkButtonText[0] != string.Empty)
                            {
                                //set the text for the check box
                                touchCheckBox1.Text = checkButtonText[0];

                                //check the check boxes depending on the message received data
                                if (messageData.middleEnterValue.buttonChecked[0] != GuiDefines.FALSE)
                                {
                                    touchCheckBox1.Checked = true;
                                }
                                else
                                {
                                    touchCheckBox1.Checked = false;
                                }

                                touchCheckBox1.Visible = true;
                            }

                            if (checkButtonText[1] != string.Empty)
                            {
                                //set the text for the check box
                                touchCheckBox2.Text = checkButtonText[1];

                                //check the check boxes depending on the message received data
                                if (messageData.middleEnterValue.buttonChecked[1] != GuiDefines.FALSE)
                                {
                                    touchCheckBox2.Checked = true;
                                }
                                else
                                {
                                    touchCheckBox2.Checked = false;
                                }

                                touchCheckBox2.Visible = true;
                            }
                                  
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in ValueControl.cs");
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
                        
                        checkBox.CheckBoxBorder = false;
                    }
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton_MouseUp(object sender, MouseEventArgs e)
        {
            SendMessageToQueue.SendButtonMessage(sender);
        }      

        /// <summary>
        /// Update the value and the check status for the check boxes depending on the message Id and the data received
        /// </summary>
        /// <param name="msg"></param>
        internal void UpdateValue(GuiReceiveMsg_t msg)
        {
            GuiReceiveMsgData_t messageData = msg.data;

            txtNew.Text = new string(messageData.middleEnterValue.newValue);

            //check the check boxes depending on the message received data
            //the code below was commented out on 09/26/2011 since it is a bug in the original code and it gets
            //covered up because of the implementation in Photon Builder
            ////if (messageData.middleEnterValue.buttonChecked[0] != GuiDefines.FALSE)
            ////{
            ////    touchCheckBox1.Checked = true;
            ////}
            ////else
            ////{
            ////    touchCheckBox1.Checked = false;
            ////}
                       
            ////if (messageData.middleEnterValue.buttonChecked[1] != GuiDefines.FALSE)
            ////{
            ////    touchCheckBox2.Checked = true;
            ////}
            ////else
            ////{
            ////    touchCheckBox2.Checked = false;
            ////}
        }       

        /// <summary>
        /// Timer event handler gets called when the user has pressed a up or a down button continuously
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void RepeatTimer_Tick(object sender, EventArgs e)
        {
            if (IsUpPressed)
            {                
                SendMessageToQueue.SendUpMessage(UserTag);
            }
            else
            {                
                SendMessageToQueue.SendDownMessage(UserTag);
            }
        }

        /// <summary>
        /// Called when the Up button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;            

            //this will be called if the click on the button is quicker than the timer tick to trigger ie when the user wants 
            //to just increment by one and not continuously
            SendMessageToQueue.SendUpMessage(sender);
        }

        /// <summary>
        /// Called when the Up button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        { 
            lock (GuiDefines.RepeatVolumeSettingLock)
            {
                if (GuiDefines.RepeatVolumeSetting)
                {
                    RepeatTimer.Enabled = true;
                }
                else
                {
                    RepeatTimer.Enabled = false;
                }
            }          

            IsUpPressed = true;//an up button is clicked

            UserTag = SendMessageToQueue.GetTagData(sender);//get the tag of the button clicked. It can be either Day or Month or Year button
        }

        /// <summary>
        /// Called when the Down button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            lock (GuiDefines.RepeatVolumeSettingLock)
            {
                if (GuiDefines.RepeatVolumeSetting)
                {
                    RepeatTimer.Enabled = true;
                }
                else
                {
                    RepeatTimer.Enabled = false;
                }
            }

            IsUpPressed = false;

            UserTag = SendMessageToQueue.GetTagData(sender);
        }

        /// <summary>
        /// Called when the Down button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            //this will be called if the click on the button is quicker than the timer tick to trigger ie when the user wants 
            //to just increment by one and not continuously
            SendMessageToQueue.SendDownMessage(sender);
        }

        /// <summary>
        /// Here touchCheckBox1 and touchCheckBox2 are acting like radio buttons ie check boxes
        /// in the same group. Here touchCheckBox1 is always checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void touchCheckBox1_MouseUp(object sender, MouseEventArgs e)
        {
            touchCheckBox2.Checked = false;
            touchCheckBox1.Checked = true;

            SendMessageToQueue.SendToggleMessage(sender);
        }

        /// <summary>
        /// Here touchCheckBox1 and touchCheckBox2 are acting like radio buttons ie check boxes
        /// in the same group. Here touchCheckBox2 is always checked
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void touchCheckBox2_MouseUp(object sender, MouseEventArgs e)
        {
            touchCheckBox1.Checked = false;
            touchCheckBox2.Checked = true;

            SendMessageToQueue.SendToggleMessage(sender);
        }        
    }
}
