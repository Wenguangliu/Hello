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
using Resco.Controls.OutlookControls;

namespace j2GUI.j2MiddleSectionControls
{
    public partial class AlphaNumericKeyboardControl : UserControl
    {
        public event j2GUI.j2Startup.KeyboardEventHandler ShowKeyboard;

        private MiddleSectionButton ButtonDefines;
        private MiddleSectionKeyboard KeyboardDefines;
        private Messaging.SendQueue SendMessageToQueue;
        private Utilities Util;
        private Sound PlaySound;
        private int MessageId;

        public AlphaNumericKeyboardControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            KeyboardDefines = new MiddleSectionKeyboard();
            ButtonDefines = new MiddleSectionButton();
            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            Util = new Utilities();
            PlaySound = new Sound();

            SetupKeyboard();

            //now reset the properties for the Keyboard button
            //imageButtonKeyboard.Text = string.Empty;
           // imageButtonKeyboard.ImageDefault = Properties.Resources._442x75_normal;
            //imageButtonKeyboard.ImagePressed = Properties.Resources._442x75_down;

            txtInput.Font = KeyboardDefines.InputTextFont;
            txtInput.ReadOnly = false;//was true
        }

        private void SetupKeyboard()
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
                        //set the default properties for ALL the buttons
                        button.ImageDefault = Properties.Resources._120x75_normal;
                        button.ImagePressed = Properties.Resources._120x75_down;
                        button.Font = ButtonDefines.Font;
                        button.ForeColor = ButtonDefines.NormalFontColor;
                        button.PressedForeColor = ButtonDefines.PressedFontColor;
                        button.FocusedBorderColor = Color.Transparent;
                    }
                }
            }
        }

        /// <summary>
        /// Set up the on screen number keypad
        /// </summary>
        /// <param name="msg"></param>
        internal void SetNumberKeyPad(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                MessageId = msg.msgId;//save it for later use

                txtInput.Text = string.Empty;
                txtInput.Focus();
                ShowKeys(true);

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP):
                        {
                            imageButtonLeft.ImageDefault = Properties.Resources._120x75_normal;
                            imageButtonLeft.ImagePressed = Properties.Resources._120x75_down;

                            imageButtonLeft.Font = KeyboardDefines.NumericKeyFont;
                            imageButtonLeft.Text = "C";
                            imageButtonLeft.Tag = -1;

                            imageButtonRight.ImageDefault = Properties.Resources._120x75_normal;
                            imageButtonRight.ImagePressed = Properties.Resources._120x75_down;

                            imageButtonRight.Font = KeyboardDefines.NumericKeyFont;
                            imageButtonRight.Text = ".";
                            imageButtonRight.Tag = -2;

                            imageButtonKeyboard.Visible = false;

                            txtInput.PasswordChar = '\0';
                            txtInput.MaxLength = 20;
                           
                            UpdateText(Util.OneCharArrayToString(messageData.middleEnterIp.text), msg.msgId);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID):
                        {
                            imageButtonLeft.Font = KeyboardDefines.ArrowKeyFont;                           

                            imageButtonLeft.Text = string.Empty;
                            imageButtonLeft.ImageDefault = Properties.Resources._120x75_Left_Arrow_Norm;
                            imageButtonLeft.ImagePressed = Properties.Resources._120x75_Left_Arrow_Down;

                            imageButtonLeft.Tag = '<';

                            imageButtonRight.Font = KeyboardDefines.ArrowKeyFont;
                         
                            imageButtonRight.Text = string.Empty;
                            imageButtonRight.ImageDefault = Properties.Resources._120x75_Right_Arrow_Norm;
                            imageButtonRight.ImagePressed = Properties.Resources._120x75_Right_Arrow_Down;

                            imageButtonRight.Tag = '-';

                            imageButtonKeyboard.Visible = true;

                            //check if the ID is a password
                            if (messageData.middleEnterId.isPassword != GuiDefines.FALSE)
                            {
                                txtInput.PasswordChar = '*';
                            }
                            else
                            {
                                txtInput.PasswordChar = '\0';//reset back the Password char
                            }

                            txtInput.MaxLength = 14;
                            UpdateText(Util.OneCharArrayToString(messageData.middleEnterId.initialValue), msg.msgId);
                                                       
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the Numeric keypad in NumericKeyboardControl.cs");
            }
        }

        /// <summary>
        /// Change the visibility of all the keys on the middle section (not Resco Keyboard). Dont show the keys if Resco
        /// keyboard is being displayed.
        /// </summary>
        /// <param name="show">show if true</param>
        private void ShowKeys(bool show)
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
                        button.Visible = show;//show/not show all the numeric keys
                    }
                }
            }           
        }

        /// <summary>
        /// This is called by the key image buttons on the screen and not by the external key board
        /// Update the input text box to reflect the user input. Also limit the number of characters in the text box
        /// to 14 characters
        /// </summary>
        /// <param name="input"></param>
        private void UpdateText(string input, int msgId)
        {
            try
            {                
                if ((msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID) || ((msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_ID)))
                {
                    if (txtInput.Text.Length >= 14)//14 characters max in the text box for Enter ID
                    {
                        return;
                    }

                    //for ID text, the text box needs to get updated in the GUI itself. It does not send and 
                    //recieve messages from and to the gui process
                    txtInput.Text += input;
                }
                else if ((msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP) || (msgId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP))
                {
                    if (txtInput.Text.Length >= 20)//20 characters max in the text box for Enter IP
                    {
                        //if user has not selected 'C' button ie cancel, then dont allow more than 20 button clicks
                        if (input != string.Empty)
                        {
                            return;
                        }
                    }

                    //for IP address text update, the text box gets updated from the console application
                    txtInput.Text = input;
                }

                //place the cursor at the end
                txtInput.Select(txtInput.Text.Length, txtInput.Text.Length);
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the text in NumericKeyboardControl.cs");
            }
        }

        /// <summary>
        /// For EnterIp message, the input text box is not updated from the Gui process, but it is updated from
        /// the receiving process. But for enterId,the input text box is updated from the Gui itself. On a key press,
        /// the message is not sent out to another process. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btn_Click(object sender, EventArgs e)
        {
            ImageButton btn = (ImageButton)sender;
            if (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP)
            {
                //for Enter IP, the receiving process will update the Gui
                SendMessageToQueue.SendButtonMessage(sender);
            }
            else
            {
                //for Enter ID, we need to update the text box in the Gui itself
                if (btn.Tag != null)
                {
                   UpdateText(btn.Tag.ToString(), MessageId);
                }

                PlaySound.Click();
            } 
        }

        /// <summary>
        /// The left button in the last row was clicked. Depending on the message Id, the event is handled
        /// The left button is a backspace if the displayed screen is for EnterId but it is clearall if the 
        /// message was EnterIp
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnLeft_Click(object sender, EventArgs e)
        {
            ImageButton btn = (ImageButton)sender;

            if (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID)
            {
                //backspace
                if (txtInput.Text.Length >= 1)
                {
                    txtInput.Text = txtInput.Text.Remove(txtInput.Text.Length - 1, 1);

                    txtInput.Select(txtInput.Text.Length, txtInput.Text.Length);
                }

                PlaySound.Click();
            }
            else if (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP)
            {
                SendMessageToQueue.SendButtonMessage(sender);
            }           
        }

        /// <summary>
        /// The right button in the last row was clicked. Depending on the message Id, the event is handled
        /// The right button is a 'dash' if the displayed screen is for EnterId but it is 'period' if the 
        /// message was EnterIp
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnRight_Click(object sender, EventArgs e)
        {
            ImageButton btn = (ImageButton)sender;

            if (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID)
            {
                if (btn.Tag != null)
                {
                    UpdateText(btn.Tag.ToString(),MessageId);
                }

                PlaySound.Click();
            }
            else if (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP)
            {
                SendMessageToQueue.SendButtonMessage(sender);
            }
        }

        /// <summary>
        /// Update the input text box value
        /// </summary>
        /// <param name="msg">the received message</param>
        internal void UpdateNumberKeyPad(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_ID):
                        {
                            UpdateText(Util.OneCharArrayToString(messageData.middleEnterId.initialValue), msg.msgId);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP):
                        {
                            UpdateText(Util.OneCharArrayToString(messageData.middleEnterIp.text), msg.msgId);

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the progress bar in ProgressControl.cs");
            }
        }

        internal void SendIdText()
        {
            SendMessageToQueue.SendTextMessage(txtInput.Text);
        }

        /// <summary>
        /// This is called only by the external key board. Here we do the validation of key input entry
        /// For ID text, the text box needs to get updated in the GUI itself. It does not send and 
        /// recieve messages from and to the gui process 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void txtInput_KeyPress(object sender, KeyPressEventArgs e)
        {
            //PlaySound.Click(); //no need of click sound generation if external keyboard or barcode scanner is used 01/18/2012

            //currently in NGA, we are not verifying the key char entries for IP addresses
            if ((MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP) || (MessageId == (int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP))
            {
                return;
            }

            //no need of any verification for BackSpace. Always allow backspace
            if (e.KeyChar == 0x08)
            {
                return;
            }

            //when the user presses the Enter key that means he is done
            if(e.KeyChar == 0x0D)
            {
                SendMessageToQueue.SendActivate();
            }
            
            //for ID messages  we need to do validation if the input to the text box is from an external keyboard          
            bool valid = InputVerify(e.KeyChar);

            //if the key pressed is not a valid one, then exit from the method
            if (!valid)
            {
                e.Handled = true;//dont show on the text box
                return;
            }

            //the char is valid, but check if the max length is reached
            if (txtInput.Text.Length >= 14)//14 characters max in the text box for Enter ID
            {
                e.Handled = true;
                return;
            }
        }


        /// <summary>
        /// Check if the key pressed on the keyboard is valid entry
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        private bool InputVerify(char input)
        {
            // These are the characters that are allowed.  All others are ignored.
            //string allowed = " !\"#$%&'()*+,-./0123456789:;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¿ÄÅÆÉÑÖÜßàáâäåæçèéêëìíîïñòóôöùúûüÿ";
            string allowed = " !\"#$%&'()*+,-./0123456789:;?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¿ÄÅÆÉÑÖÜßàáâäå ãæçèéêëìíîïñòóôö õùúûüÿ";
            char[] allowedChars = allowed.ToCharArray();           

            foreach (char c in allowed.ToCharArray())
            {
                if (input == c)
                {
                    return true;
                }
            }

            return false;
        }
        
        private void imageButtonKeyboard_Click(object sender, EventArgs e)
        {
            PlaySound.Click();

            //make all the keys invisible before showing the alphanumeric keyboard
            ShowKeys(false);

            ShowKeyboard(true);

            txtInput.Focus();
        }     
    }
}
