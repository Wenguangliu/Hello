using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    public partial class TemperatureControl : UserControl
    {

        private Messaging.SendQueue SendMessageToQueue;

        public TemperatureControl()
        {
            MiddleSectionTemperature TemperatureDefines = new MiddleSectionTemperature();
          
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            //Clear all textboxes
            txtRotorTemperatureValue.Text = "";
            txtRotorTemperatureValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtRotorTemperatureValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtAmbientTemperatureValue.Text = "";
            txtAmbientTemperatureValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtAmbientTemperatureValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtAirTemperatureOffsetValue.Text = "";
            txtAirTemperatureOffsetValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtAirTemperatureOffsetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtAmbientDacOffsetValue.Text = "";
            txtAmbientDacOffsetValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtAmbientDacOffsetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtTopHeaterTemperatureValue.Text = "";
            txtTopHeaterTemperatureValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtTopHeaterTemperatureValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtTopHeaterCurrentValue.Text = "";
            txtTopHeaterCurrentValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtTopHeaterCurrentValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtBottomHeaterTemperatureValue.Text = "";
            txtBottomHeaterTemperatureValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtBottomHeaterTemperatureValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtBottomHeaterCurrentValue.Text = "";
            txtBottomHeaterCurrentValue.Font = TemperatureDefines.TextBoxFontSmall;
            txtBottomHeaterCurrentValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtTopHeaterTemperatureSetValue.Text = "";
            txtTopHeaterTemperatureSetValue.Font = TemperatureDefines.TextBoxFontLarge;
            txtTopHeaterTemperatureSetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtTopDacSetValue.Text = "";
            txtTopDacSetValue.Font = TemperatureDefines.TextBoxFontLarge;
            txtTopDacSetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtBottomHeaterTemperatureSetValue.Text = "";
            txtBottomHeaterTemperatureSetValue.Font = TemperatureDefines.TextBoxFontLarge;
            txtBottomHeaterTemperatureSetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            txtBottomDacSetValue.Text = "";
            txtBottomDacSetValue.Font = TemperatureDefines.TextBoxFontLarge;
            txtBottomDacSetValue.ForeColor = TemperatureDefines.TextBoxFontColor;

            lblAmbient.Font = TemperatureDefines.LabelFont;
            lblAmbient.ForeColor = TemperatureDefines.LabelFontColor;

            lblBottom.Font = TemperatureDefines.LabelFont;
            lblBottom.ForeColor = TemperatureDefines.LabelFontColor;

            lblHeaterTemperature.Font = TemperatureDefines.LabelFont;
            lblHeaterTemperature.ForeColor = TemperatureDefines.LabelFontColor;

            lblOffset.Font = TemperatureDefines.LabelFont;
            lblOffset.ForeColor = TemperatureDefines.LabelFontColor;

            lblPlateCurrent.Font = TemperatureDefines.LabelFont;
            lblPlateCurrent.ForeColor = TemperatureDefines.LabelFontColor;

            lblRotor.Font = TemperatureDefines.LabelFont;
            lblRotor.ForeColor = TemperatureDefines.LabelFontColor;

            lblSetBottom.Font = TemperatureDefines.LabelFont;
            lblSetBottom.ForeColor = TemperatureDefines.LabelFontColor;

            lblSetTop.Font = TemperatureDefines.LabelFont;
            lblSetTop.ForeColor = TemperatureDefines.LabelFontColor;

            lblTemperature.Font = TemperatureDefines.LabelFont;
            lblTemperature.ForeColor = TemperatureDefines.LabelFontColor;

            lblTop.Font = TemperatureDefines.LabelFont;
            lblTop.ForeColor = TemperatureDefines.LabelFontColor;

            btnSetTopUp.Text = "";
            btnSetTopUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnSetTopUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnSetTopDown.Text = "";
            btnSetTopDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnSetTopDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnSetBottomUp.Text = "";
            btnSetBottomUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnSetBottomUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnSetBottomDown.Text = "";
            btnSetBottomDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnSetBottomDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

        }

        /// <summary>
        /// Sets the middle sub section to display the view control
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetTemperatureControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_TEMPERATURE_INFO):
                        {
                            txtRotorTemperatureValue.Text = new string(msg.data.middleTemperature.rotorTemperatureValue);
                            txtAmbientTemperatureValue.Text = new string(msg.data.middleTemperature.ambientTemperatureValue);

                            txtAirTemperatureOffsetValue.Text = new string(msg.data.middleTemperature.airTemperatureOffsetValue);
                            txtAmbientDacOffsetValue.Text = new string(msg.data.middleTemperature.ambientDacOffsetValue);

                            txtTopHeaterTemperatureValue.Text = new string(msg.data.middleTemperature.topHeaterTemperatureValue);
                            txtTopHeaterCurrentValue.Text = new string(msg.data.middleTemperature.topHeaterCurrentValue);

                            txtBottomHeaterTemperatureValue.Text = new string(msg.data.middleTemperature.bottomHeaterTemperatureValue);
                            txtBottomHeaterCurrentValue.Text = new string(msg.data.middleTemperature.bottomHeaterCurrentValue);

                            txtTopHeaterTemperatureSetValue.Text = new string(msg.data.middleTemperature.topHeaterTemperatureSetValue);
                            txtTopDacSetValue.Text = new string(msg.data.middleTemperature.topDacSetValue);

                            txtBottomHeaterTemperatureSetValue.Text = new string(msg.data.middleTemperature.bottomHeaterTemperatureSetValue);
                            txtBottomDacSetValue.Text = new string(msg.data.middleTemperature.bottomDacSetValue);

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set textboxes in TemperatureControl.cs");
            }
        }

        /// <summary>
        /// Update the Temperature control depending on the fields received.
        /// messasge queue
        /// </summary>
        /// <param name="msg"></param>
        internal void UpdateTemperatureControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES):
                        {
                            if (msg.data.middleTemperature.rotorTemperatureValue.Sum(total => total) > 0)
                            {
                                txtRotorTemperatureValue.Text = new string(msg.data.middleTemperature.rotorTemperatureValue);
                            }

                            if (msg.data.middleTemperature.ambientTemperatureValue.Sum(total => total) > 0)
                            {
                                txtAmbientTemperatureValue.Text = new string(msg.data.middleTemperature.ambientTemperatureValue);
                            }

                            if (msg.data.middleTemperature.airTemperatureOffsetValue.Sum(total => total) > 0)
                            {
                                txtAirTemperatureOffsetValue.Text = new string(msg.data.middleTemperature.airTemperatureOffsetValue);
                            }

                            if (msg.data.middleTemperature.ambientDacOffsetValue.Sum(total => total) > 0)
                            {
                                txtAmbientDacOffsetValue.Text = new string(msg.data.middleTemperature.ambientDacOffsetValue);
                            }

                            if (msg.data.middleTemperature.topHeaterTemperatureValue.Sum(total => total) > 0)
                            {
                                txtTopHeaterTemperatureValue.Text = new string(msg.data.middleTemperature.topHeaterTemperatureValue);
                            }

                            if (msg.data.middleTemperature.topHeaterCurrentValue.Sum(total => total) > 0)
                            {
                                txtTopHeaterCurrentValue.Text = new string(msg.data.middleTemperature.topHeaterCurrentValue);
                            }

                            if (msg.data.middleTemperature.bottomHeaterTemperatureValue.Sum(total => total) > 0)
                            {
                                txtBottomHeaterTemperatureValue.Text = new string(msg.data.middleTemperature.bottomHeaterTemperatureValue);
                            }

                            if (msg.data.middleTemperature.bottomHeaterCurrentValue.Sum(total => total) > 0)
                            {
                                txtBottomHeaterCurrentValue.Text = new string(msg.data.middleTemperature.bottomHeaterCurrentValue);
                            }

                            if (msg.data.middleTemperature.topHeaterTemperatureSetValue.Sum(total => total) > 0)
                            {
                                txtTopHeaterTemperatureSetValue.Text = new string(msg.data.middleTemperature.topHeaterTemperatureSetValue);
                            }

                            if (msg.data.middleTemperature.topDacSetValue.Sum(total => total) > 0)
                            {
                                txtTopDacSetValue.Text = new string(msg.data.middleTemperature.topDacSetValue);
                            }

                            if (msg.data.middleTemperature.bottomHeaterTemperatureSetValue.Sum(total => total) > 0)
                            {
                                txtBottomHeaterTemperatureSetValue.Text = new string(msg.data.middleTemperature.bottomHeaterTemperatureSetValue);
                            }

                            if (msg.data.middleTemperature.bottomDacSetValue.Sum(total => total) > 0)
                            {
                                txtBottomDacSetValue.Text = new string(msg.data.middleTemperature.bottomDacSetValue);
                            }

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the progress bar in ProgressControl.cs");
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
