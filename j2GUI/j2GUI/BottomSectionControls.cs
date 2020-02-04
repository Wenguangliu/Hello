using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;
using Resco.Controls.CommonControls;
using Resco.Controls.OutlookControls;

namespace j2GUI
{
    /// <summary>
    /// This class caters for designing the bottom button controls. Depending on the message Id received, dynamically controls are placed. This control's controls 
    /// are first cleared to begin with and then dynamically created. 
    /// </summary>
    public partial class BottomSectionControls : UserControl
    {
        private BottomGuiDefines BottomDefines;
        private Utilities Util;
        private TransparentLabel lblBottomLabel;
        private string[] DaysOfWeek;
        private string[] Months;
        private char TwentyFourHourFormat;
        public System.Windows.Forms.Timer TimerDateTime;
        private int PreviousHour;
        private int PreviousMinute;
        private Messaging.SendQueue SendMessageToQueue;

        public BottomSectionControls()
        {
            InitializeComponent();

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            TimerDateTime = new System.Windows.Forms.Timer();
            TimerDateTime.Interval = 1000;// 1000 milliseconds
            TimerDateTime.Tick += new System.EventHandler(TimerDateTime_Tick);

            PreviousHour = -1;
            PreviousMinute = -1;
            BottomDefines = new BottomGuiDefines();
            Util = new Utilities();

            //set the bottom user control height and location
            this.Height = GuiForm.BottomFormHeight;
            this.BackColor = BottomDefines.LabelBackColor;
            this.Location = new Point(0, GuiForm.FormHeight - this.Height);
            //this.Location = new Point(0, GuiForm.TopFormHeight);//for front panel tester BottomButtonsUp Message ID
        }

        /// <summary>
        /// Add the buttons to the user control depending on the button count. Also set the colors of the button
        /// depending on the highlight flag
        /// </summary>
        /// <param name="count">number of buttons to be placed in the bottom section user control</param>
        /// <param name="msgdata">data related to the bottom section controls</param>
        private void LoadBottomButtons(int count, GuiBottomButtonsData_t msgdata)
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

                    button.Location = new Point(i * this.ClientRectangle.Width / count, 0);
                    if (count == 1)//only one button on the bottom section
                    {
                        button.Dock = DockStyle.Fill;
                    }
                    else
                    {
                        button.Size = new Size(this.ClientRectangle.Width / count, this.ClientRectangle.Height);
                    }

                    //set default properties for the buttons
                    button.Font = BottomDefines.Font;
                    button.ForeColor = BottomDefines.NormalFontColor;
                    button.PressedForeColor = BottomDefines.PressedFontColor;
                    button.BorderStyle = Resco.Controls.RescoBorderStyle.FixedSingle;
                    button.FocusedBorderColor = Color.Transparent;
                    button.BorderCornerSize = 0;
                    button.VistaButtonInflate = new Size(-1, -1);
                    button.Tag = i;

                    //set the text property of the button depending on what is received from the message queue data
                    button.Text = buttonText[i];

                    if (BottomDefines.FillDirection == "Vertical")
                    {
                        button.GradientColors.FillDirection = Resco.Controls.OutlookControls.FillDirection.Vertical;
                        button.GradientColorsPressed.FillDirection = Resco.Controls.OutlookControls.FillDirection.Vertical;
                    }
                    else
                    {
                        button.GradientColors.FillDirection = Resco.Controls.OutlookControls.FillDirection.Horizontal;
                        button.GradientColorsPressed.FillDirection = Resco.Controls.OutlookControls.FillDirection.Horizontal;
                    }

                    //get the button which should have highlighted button colors
                    if (msgdata.buttonHighlighted[i] != GuiDefines.NORMAL)
                    {
                        button.GradientColors.StartColor = BottomDefines.HighlightStartButtonColor;
                        button.GradientColors.EndColor = BottomDefines.HighlightEndButtonColor;
                        button.GradientColorsPressed.StartColor = BottomDefines.PressedHighlightStartButtonColor;
                        button.GradientColorsPressed.EndColor = BottomDefines.PressedHighlightEndButtonColor;

                        button.BorderColor = BottomDefines.HighlightButtonBorderColor;
                    }
                    else
                    {
                        button.GradientColors.StartColor = BottomDefines.NormalStartButtonColor;
                        button.GradientColors.EndColor = BottomDefines.NormalEndButtonColor;
                        button.GradientColorsPressed.StartColor = BottomDefines.PressedNormalStartButtonColor;
                        button.GradientColorsPressed.EndColor = BottomDefines.PressedNormalEndButtonColor;

                        button.BorderColor = BottomDefines.NormalButtonBorderColor;
                    }

                    //dynamically buttons are added to the bottom section user control
                    this.Controls.Add(button);
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to load the bottom buttons in BottomSectionControls.cs");
            }
        }
        /// <summary>
        /// Set the correct date and time for the bottom section label
        /// </summary>
        /// <param name="message"></param>
        private void LoadBottomLabel(string message)
        {
            try
            {
                TransparentLabel lblBottomLabel = new TransparentLabel();

                lblBottomLabel.AutoSize = false;
                lblBottomLabel.Dock = System.Windows.Forms.DockStyle.Fill;
                lblBottomLabel.Location = new System.Drawing.Point(0, 0);
                lblBottomLabel.Name = "lblBottomLabel";
                lblBottomLabel.Size = new System.Drawing.Size(480, 75);
                lblBottomLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;

                lblBottomLabel.Text = message;
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to load the bottom label in BottomSectionControls.cs");
            }
        }

        /// <summary>
        /// Clear all the controls on the bottom panel
        /// </summary>
        internal void ClearControls()
        {
            StopTimer();
            this.Controls.Clear();
        }

        /// <summary>
        /// Populate the bottom control with the correct set of controls depending on the message Ids
        /// </summary>
        /// <param name="msg"></param>
        internal void SetBottomControl(GuiReceiveMsg_t msg)
        {
            try
            {
                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_NONE):
                        {
                            //to beginwith, clear all the controls in the panel and then start adding the new ones
                            ClearControls();

                            this.Hide();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_ONE_BUTTON):
                        {
                            //to beginwith, clear all the controls in the panel and then start adding the new ones
                            ClearControls();

                            LoadBottomButtons(GuiDefines.ONEBUTTON, msg.data.bottomButtons);
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_TWO_BUTTONS):
                        {
                            //to beginwith, clear all the controls in the panel and then start adding the new ones
                            ClearControls();

                            LoadBottomButtons(GuiDefines.TWOBUTTONS, msg.data.bottomButtons);
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_THREE_BUTTONS):
                        {
                            //to beginwith, clear all the controls in the panel and then start adding the new ones
                            ClearControls();

                            LoadBottomButtons(GuiDefines.THREEBUTTONS, msg.data.bottomButtons);
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_BOTTOM_DATE_TIME)://205
                        {
                            //to beginwith, clear all the controls in the panel and then start adding the new ones
                            ClearControls();

                            AddDateTimeLabel();

                            //show the bottom label with correct date and time
                            LoadBottomLabel();

                            PreviousHour = -1;
                            PreviousMinute = -1;

                            //start the timer to update the time since the main screen will be
                            //showing the date time label in the bottom section
                            StartTimer();

                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_SET_DATE_TIME_STRINGS)://503
                        {
                            //assign the message data from the recieved message
                            DaysOfWeek = Util.TwoCharArrayToStringArray(msg.data.bottomDateTime.daysOfWeek);
                            Months = Util.TwoCharArrayToStringArray(msg.data.bottomDateTime.months);
                            TwentyFourHourFormat = msg.data.bottomDateTime.twentyFourHourFormat;

                            break;
                        }
                    default:
                        {
                            this.Hide();
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the bottom control in BottomSectionControls.cs");
            }
        }

        private void AddDateTimeLabel()
        {
            //add the label which will display the date and time
            lblBottomLabel = new TransparentLabel();

            lblBottomLabel.AutoSize = false;
            //lblBottomLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            lblBottomLabel.Location = new System.Drawing.Point(0, 24);
            lblBottomLabel.Name = "lblBottomLabel";
            lblBottomLabel.Size = new System.Drawing.Size(480, 75);
            lblBottomLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            lblBottomLabel.Font = BottomDefines.LabelFont;
            lblBottomLabel.ForeColor = BottomDefines.NormalFontColor;
            lblBottomLabel.BackColor = BottomDefines.LabelBackColor;

            //dynamically buttons are added to the bottom section user control
            this.Controls.Add(lblBottomLabel);
        }

        /// <summary>
        /// Set the current date and time in correct format
        /// </summary>
        private void LoadBottomLabel()
        {
            DateTime currentDateTime = DateTime.Now;
            int hour = currentDateTime.Hour;
            int minute = currentDateTime.Minute;
            string meridian;

            // Update the display if we've changed to the next minute.  We compare the
            // hour, too, in case someone has just changed the system time by exactly one
            // hour.
            if ((PreviousHour != hour) || (PreviousMinute != minute))
            {
                // Adjust the hour and set the time suffix, depending on the time format.
                if (TwentyFourHourFormat == GuiDefines.FALSE)
                {
                    if (currentDateTime.Hour >= 12)
                    {
                        meridian = " PM";
                        if (currentDateTime.Hour > 12)
                        {
                            hour -= 12;
                        }
                    }
                    else
                    {
                        meridian = " AM";
                        if (hour == 0)
                        {
                            hour = 12;
                        }
                    }
                }
                else
                {
                    meridian = "";
                }

                //build the date and time string to be displayed
                StringBuilder dateTime = new StringBuilder();
                dateTime.Append(DaysOfWeek[(int)currentDateTime.DayOfWeek]);
                dateTime.Append(" ");
                dateTime.Append(currentDateTime.Day);
                dateTime.Append(" ");
                dateTime.Append(Months[(currentDateTime.Month - 1)]);
                dateTime.Append(" ");
                dateTime.Append(currentDateTime.Year.ToString());
                dateTime.Append(" ");
                dateTime.Append(hour.ToString().PadLeft(2, '0'));
                dateTime.Append(":");
                dateTime.Append(minute.ToString().PadLeft(2, '0'));
                dateTime.Append(meridian);

                lblBottomLabel.Text = dateTime.ToString();

                PreviousHour = currentDateTime.Hour;
                PreviousMinute = currentDateTime.Minute;
            }
        }

        /// <summary>
        /// start the time ONLY if the message Id was MSG_GUI_BOTTOM_DATE_TIME
        /// </summary>
        private void StartTimer()
        {
            TimerDateTime.Enabled = true;
        }

        /// <summary>
        /// Stop the timer if the bottom section is other than date time control
        /// </summary>
        private void StopTimer()
        {
            TimerDateTime.Enabled = false;
        }

        /// <summary>
        /// Update the date time label
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void TimerDateTime_Tick(object sender, EventArgs e)
        {
            UpdateBottomLabel();
        }

        /// <summary>
        /// Show the updated date and time
        /// </summary>
        private void UpdateBottomLabel()
        {
            try
            {
                LoadBottomLabel();
            }
            catch (System.Exception ex)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update bottom label in BottomSectionControls.cs");
            }
        }

        private void BottomButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendBottomButtonMessage(sender);
        }
    }
}
