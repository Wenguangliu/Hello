using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using Resco.Controls.OutlookControls;
using System.Windows.Forms;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// RANGE CONTROL
    /// Range interval controls
    /// </summary>
    public partial class RangeControl : UserControl
    {
        private Utilities Util;
        private MiddleSectionButton ButtonDefines;
        private MiddleSectionTypeCheckBox CheckBoxDefines;
        private MiddleSectionEnterRange RangeDefines;
        private Messaging.SendQueue SendMessageToQueue;
        private Sound PlaySound;
        private Timer RepeatTimer = new Timer();//timer thread used for repeated scrolling
        private bool IsUpPressed = true;

        private float LowerValue;
        private float UpperValue;
        private float LowerDefault;
        private float UpperDefault;
        private CommonDefines.NumericFormat_t Format;
        private int RepeatCount;
        private int UserTag;

        public RangeControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            Util = new Utilities();
            ButtonDefines = new MiddleSectionButton();
            CheckBoxDefines = new MiddleSectionTypeCheckBox();
            RangeDefines = new MiddleSectionEnterRange();
            PlaySound = new Sound();

            RepeatCount = 0; //initialize

            btnLowerUp.Text = "";
            btnLowerDown.Text = "";

            btnUpperUp.Text = "";
            btnUpperDown.Text = "";

            btnLowerUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnLowerUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnLowerDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnLowerDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnUpperUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUpperUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnUpperDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnUpperDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

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

            lblLower.Font = RangeDefines.LabelFont;
            lblLower.ForeColor = RangeDefines.LabelFontColor;

            lblUpper.Font = RangeDefines.LabelFont;
            lblUpper.ForeColor = RangeDefines.LabelFontColor;

            txtLower.Font = RangeDefines.TextBoxFont;
            txtLower.ForeColor = RangeDefines.TextBoxFontColor;

            txtUpper.Font = RangeDefines.TextBoxFont;
            txtUpper.ForeColor = RangeDefines.TextBoxFontColor;
        }
       
        /// <summary>
        /// Set the middle sub section with correct set of controls
        /// </summary>
        /// <param name="msg">received message data</param>
        internal void SetRangeControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_RANGE):
                        {
                            LowerValue = messageData.middleEnterRange.lowerValue;
                            UpperValue = messageData.middleEnterRange.upperValue;
                            LowerDefault = messageData.middleEnterRange.lowerDefault;
                            UpperDefault = messageData.middleEnterRange.upperDefault;
                            Format = messageData.middleEnterRange.format;

                            lblLower.Text = new string(messageData.middleEnterRange.lowerLabel);
                            lblUpper.Text = new string(messageData.middleEnterRange.upperLabel);

                            txtLower.Text = FormatValue(LowerValue);
                            txtUpper.Text = FormatValue(UpperValue);

                            imageButton0.Visible = false;
                            imageButton1.Visible = false;

                            string[] buttonText = Util.TwoCharArrayToStringArray(messageData.middleEnterRange.buttonText);

                            if (buttonText[0] != string.Empty)
                            {
                                imageButton0.Text = buttonText[0];
                                imageButton0.Visible = true;
                            }

                            if (buttonText[1] != string.Empty)
                            {
                                imageButton1.Text = buttonText[1];
                                imageButton1.Visible = true;
                            }

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in RangeControl.cs");
            }
        }

        /// <summary>
        /// Format the value according to the Numeric format set before displaying onto the user
        /// </summary>
        /// <param name="value">the reference range to be formatted</param>
        /// <returns>formatted string</returns>
        private string FormatValue(float value)
        {
            string formattedValue = string.Empty;

            try
            {  
                switch (Format)
                {
                    case CommonDefines.NumericFormat_t.Xxxx:
                        {
                            //display three decimal places
                            //sprintf(buff, ".%03.0f", value * 1000.0);
                            formattedValue = string.Format("{0:F3}", value);

                            break;
                        }
                    case CommonDefines.NumericFormat_t.Xxx:
                        {                 
                            //display two decimal places
                            formattedValue = string.Format("{0:F2}", value);

                            break;
                        }
                    case CommonDefines.NumericFormat_t.Xx:
                        {
                            //display one decimal places
                            formattedValue = string.Format("{0:F1}", value);

                            break;
                        }
                    case CommonDefines.NumericFormat_t.X:
                        {
                            //display no decimal places
                            formattedValue = string.Format("{0:F0}", value);

                            break;
                        }
                    case CommonDefines.NumericFormat_t.X0:
                        {
                            //display the value rounded to the nearest 10
                            int x0 = ((int)Math.Round(value / 10.0)) * 10;
                            formattedValue = x0.ToString("D2");
                          
                            break;
                        }
                    case CommonDefines.NumericFormat_t.X00:
                        {
                            //display the value rounded to the nearest 100
                            int x00 = ((int)Math.Round(value / 100.0)) * 100;
                            formattedValue = x00.ToString("D3");
                           
                            break;
                        }
                    default:
                        {
                            formattedValue = string.Empty;
                            break;
                        }
                }                
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to format value in RangeControl.cs");
            }

            return (formattedValue);
        }

        /// <summary>
        /// Depending on the Numeric format, set the increment factor
        /// </summary>
        /// <returns>increment factor to be used to adjust the value</returns>
        private float IncreaseFactor()
        {
            float incFactor = 1.0f;

            switch (Format)
            {
                case CommonDefines.NumericFormat_t.Xxxx:
                    {
                        incFactor = 0.001f;
                        break;
                    }
                case CommonDefines.NumericFormat_t.Xxx:
                    {
                        incFactor = 0.01f;
                        break;
                    }
                case CommonDefines.NumericFormat_t.Xx:
                    {
                        incFactor = 0.1f;
                        break;
                    }
                case CommonDefines.NumericFormat_t.X:
                    {
                        incFactor = 1;
                        break;
                    }
                case CommonDefines.NumericFormat_t.X0:
                    {
                        incFactor = 10;
                        break;
                    }
                case CommonDefines.NumericFormat_t.X00:
                    {
                        incFactor = 100;
                        break;
                    }
                default:
                    {
                        incFactor = 1.0f;
                        break;
                    }

            }

            return (incFactor);
        }

        /// <summary>
        /// Adjust the lower and upper range values
        /// </summary>
        /// <param name="index">Upper or Lower</param>
        /// <param name="adjustment">Adjustment can be 1 or -1 or..</param>
        private void AdjustValue(int index, int adjustment)
        {
            try
            {
                switch (index)
                {
                    case 0:		// Lower
                        {
                            LowerValue += adjustment * IncreaseFactor();

                            if (LowerValue < 0.0)
                            {
                                LowerValue = 0.0f;
                            }

                            if (LowerValue > UpperValue)
                            {
                                UpperValue = LowerValue;
                            }
                            break;
                        }
                    case 1:		// Upper
                        {
                            UpperValue += adjustment * IncreaseFactor();

                            if (UpperValue < 0.0)
                            {
                                UpperValue = 0.0f;
                            }

                            if (UpperValue < LowerValue)
                            {
                                LowerValue = UpperValue;
                            }

                            break;
                        }
                    default:
                        {
                            break;
                        }
                }

                txtLower.Text = FormatValue(LowerValue);
                txtUpper.Text = FormatValue(UpperValue);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to Adjust value in RangeControl.cs");
            }
        }       
        

        /// <summary>
        /// Handler for Clear and Default button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton_Click(object sender, EventArgs e)
        {
            int buttonIdentity;

            ImageButton btn = (ImageButton)sender;

            if (btn.Tag != null)
            {
                buttonIdentity = Int32.Parse(btn.Tag.ToString());
            }
            else
            {
                buttonIdentity = -1;
            }

            switch (buttonIdentity)
            {
                case 0:
                    {// Clear
                        LowerValue = 0.0f;
                        UpperValue = 0.0f;
                        break;
                    }
                case 1:
                    {// Default
                        LowerValue = LowerDefault;
                        UpperValue = UpperDefault;
                        break;
                    }
                default:
                    {
                        break;
                    }
            }

            txtLower.Text = FormatValue(LowerValue);
            txtUpper.Text = FormatValue(UpperValue);

            PlaySound.Click();
        }
        /// <summary>
        /// Send the range to the controlling process
        /// </summary>
        internal void SendRange()
        {
            GuiSendRangeData_t rangeData = new GuiSendRangeData_t();
            rangeData.lower = LowerValue;
            rangeData.upper = UpperValue;

            SendMessageToQueue.SendRangeMessage(rangeData);
        }

        /// <summary>
        /// Disable the repeat timer. 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            try
            {
                RepeatTimer.Enabled = false;

                AdjustValue(UserTag, 1);              

                RepeatCount = 0;
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Failed in btnUp Mouse Up event in RangeControl.cs");

            }
        }

        /// <summary>
        /// Enable the timer and set the UpPressed flag
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        {
            try
            {
                RepeatTimer.Enabled = true;
                IsUpPressed = true;//Lower or Upper up button clicked
                PlaySound.Click();

                UserTag = SendMessageToQueue.GetTagData(sender);//User Tag will indicate if the lower up button was clicked or the upper
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Failed at btnUp Mouse Down event handler in RangeControl.cs");
            }
        }

        /// <summary>
        /// Enable the repeat timer
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            try
            {
                RepeatTimer.Enabled = true;
                IsUpPressed = false;//Up button is not pressed
                PlaySound.Click();

                UserTag = SendMessageToQueue.GetTagData(sender);
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Failed in btnDown MouseDown event in RangeControl.cs");
            }
        }

        /// <summary>
        /// Disable the timer and update the lower value
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            try
            {
                RepeatTimer.Enabled = false;

                AdjustValue(UserTag, -1);

                RepeatCount = 0;
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Failed at btnDown Mouse Up event in RangeControl.cs");
            }
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
                RepeatUpRange();
            }
            else
            {
                RepeatDownRange();
            }
        }

        /// <summary>
        /// Repeat decrementing range. Depending on the RepeatCount, decrement in steps of 1 10 or 100
        /// </summary>
        private void RepeatDownRange()
        {
            RepeatCount++;

            if (RepeatCount < 10)
            {
                AdjustValue(UserTag, -1);
            }
            else if (RepeatCount < 50)
            {
                AdjustValue(UserTag, -10);
            }
            else
            {
                AdjustValue(UserTag, -100);
            }
        }

        /// <summary>
        /// Repeat incrementing the range
        /// </summary>
        private void RepeatUpRange()
        {
            RepeatCount++;

            if (RepeatCount < 10)
            {
                AdjustValue(UserTag, 1);
            }
            else if (RepeatCount < 50)
            {
                AdjustValue(UserTag, 10);
            }
            else
            {
                AdjustValue(UserTag, 100);
            }
        }        
    }
}
