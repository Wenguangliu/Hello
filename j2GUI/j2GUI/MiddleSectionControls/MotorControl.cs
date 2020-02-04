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
    public partial class MotorControl : UserControl
    {
        private MiddleSectionButton ButtonDefines;
        private MiddleSectionEnterValue MotorDefines;
        private Messaging.SendQueue SendMessageToQueue;

        public MotorControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;
            SendMessageToQueue = new j2GUI.Messaging.SendQueue();
            ButtonDefines = new MiddleSectionButton();
            MotorDefines = new MiddleSectionEnterValue();

            txtCurrentSpeed.Text = "";
            txtCurrentSpeed.Font = MotorDefines.TextBoxFontSmall;
            txtCurrentSpeed.ForeColor = MotorDefines.TextBoxFontColor;

            lblDirection.Font = MotorDefines.LabelFont;
            lblDirection.ForeColor = MotorDefines.LabelFontColor;

            lblSpeed.Font = MotorDefines.LabelFont;
            lblSpeed.ForeColor = MotorDefines.LabelFontColor;

            txtDirection.Text = "";
            txtDirection.Font = MotorDefines.TextBoxFont;
            txtDirection.ForeColor = MotorDefines.TextBoxFontColor;

            txtSetSpeed.Text = "";
            txtSetSpeed.Font = MotorDefines.TextBoxFont;
            txtSetSpeed.ForeColor = MotorDefines.TextBoxFontColor;

            btnDirectionUp.Text = "";
            btnDirectionUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnDirectionUp.ImagePressed = Properties.Resources._213x75_scrollup_down;
            btnDirectionUp.Font = ButtonDefines.Font;
            btnDirectionUp.ForeColor = ButtonDefines.NormalFontColor;
            btnDirectionUp.PressedForeColor = ButtonDefines.PressedFontColor;

            btnDirectionDown.Text = "";
            btnDirectionDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDirectionDown.ImagePressed = Properties.Resources._213x75scrolldown_down;
            btnDirectionDown.Font = ButtonDefines.Font;
            btnDirectionDown.ForeColor = ButtonDefines.NormalFontColor;
            btnDirectionDown.PressedForeColor = ButtonDefines.PressedFontColor;

            btnSetSpeedUp.Text = "";
            btnSetSpeedUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnSetSpeedUp.ImagePressed = Properties.Resources._213x75_scrollup_down;
            btnSetSpeedUp.Font = ButtonDefines.Font;
            btnSetSpeedUp.ForeColor = ButtonDefines.NormalFontColor;
            btnSetSpeedUp.PressedForeColor = ButtonDefines.PressedFontColor;

            btnSetSpeedDown.Text = "";
            btnSetSpeedDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnSetSpeedDown.ImagePressed = Properties.Resources._213x75scrolldown_down;
            btnSetSpeedDown.Font = ButtonDefines.Font;
            btnSetSpeedDown.ForeColor = ButtonDefines.NormalFontColor;
            btnSetSpeedDown.PressedForeColor = ButtonDefines.PressedFontColor;
        }

        /// <summary>
        /// Set the Motor control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetMotorControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MOTOR_DATA):
                        {
                            txtCurrentSpeed.Text = new string(messageData.middleMotor.motorSpeedReadValue);
                            txtDirection.Text = new string(messageData.middleMotor.motorDirection);
                            txtSetSpeed.Text = new string(messageData.middleMotor.motorSpeedSetValue);
                            break;
                        }
                }               
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the controls in MotorControl.cs");
            }
        }

        /// <summary>
        /// Update the Motor control with the received message data.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void UpdateMotorControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_MOTOR_DATA):
                        {
                            if (messageData.middleMotor.motorSpeedReadValue.Sum(total => total) > 0)
                            {
                                txtCurrentSpeed.Text = new string(messageData.middleMotor.motorSpeedReadValue);
                            }

                            if (messageData.middleMotor.motorDirection.Sum(total => total) > 0)
                            {
                                txtDirection.Text = new string(messageData.middleMotor.motorDirection);
                            }

                            if (messageData.middleMotor.motorSpeedSetValue.Sum(total => total) > 0)
                            {
                                txtSetSpeed.Text = new string(messageData.middleMotor.motorSpeedSetValue);
                            }
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the controls in MotorControl.cs");
            }
        }

        /// <summary>
        /// Send the up button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UpButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendUpMessage(sender);
        }

        /// <summary>
        /// Send the down button click event to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void DownButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendDownMessage(sender);
        }

    }
}
