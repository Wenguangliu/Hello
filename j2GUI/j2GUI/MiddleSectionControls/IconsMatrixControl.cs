using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using Resco.Controls.OutlookControls;
using CppCsWrapper;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>
    /// ICON MATRIX CONTROL
    /// This middle sub section has the icons set up in a matrix. This displays both the Icon Matrix 1 and Icon Matrix2.
    /// Depending on the message Id received, the middle sub section is dynamically set.
    /// </summary>
    public partial class IconsMatrixControl : UserControl
    {
        private Messaging.SendQueue SendMessageToQueue;

        public IconsMatrixControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            //image buttons should show empty strings for text property
            imageButton1.Text = string.Empty;
            imageButton2.Text = string.Empty;
            imageButton3.Text = string.Empty;
            imageButton4.Text = string.Empty;
            imageButton5.Text = string.Empty;
            imageButton6.Text = string.Empty;
            imageButton7.Text = string.Empty;
            imageButton8.Text = string.Empty;
            imageButton9.Text = string.Empty;
            imageButton10.Text = string.Empty;
            imageButton11.Text = string.Empty;
            imageButton12.Text = string.Empty;
        }

        /// <summary>
        /// Set up the middle sub section with correct set of icons after making all the icons invisible to beginwith.
        /// The tag property of each ImageButton is set here.
        /// </summary>
        /// <param name="msg">received message</param>
        internal void SetIconsControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                byte productCode = 1;//default

                HideImageControls();

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_1):
                        {
                            imageButton1.Visible = true;
                            imageButton1.ImageDefault = Properties.Resources.WhoAmI_Icon;
                            imageButton2.Visible = true;
                            imageButton2.ImageDefault = Properties.Resources.DateTime_Icon;
                            imageButton3.Visible = true;
                            imageButton3.ImageDefault = Properties.Resources.AlarmSettings_Icon;
                            imageButton4.Visible = true;
                            imageButton4.ImageDefault = Properties.Resources.PrinterSettings_Icon;
                            imageButton5.Visible = true;
                            imageButton5.ImageDefault = Properties.Resources.ScreenSettings_Icon;
                            imageButton6.Visible = true;
                            imageButton6.ImageDefault = Properties.Resources.Language_Icon;

                            //navigation icon
                            imageButton12.Visible = true;
                            imageButton12.ImageDefault = Properties.Resources.MoreSettings_Icon;

                            //set the tag property for the first icon matrix screen Icon Matrix 1
                            imageButton1.Tag = "Who";
                            imageButton2.Tag = "DateTime";
                            imageButton3.Tag = "Sound";
                            imageButton4.Tag = "Printer";
                            imageButton5.Tag = "Screen";
                            imageButton6.Tag = "Language"; 

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_2):
                        {

                            imageButton1.Visible = true;
                            imageButton1.ImageDefault = Properties.Resources.Units_Icon;
                            imageButton2.Visible = true;
                            imageButton2.ImageDefault = Properties.Resources.RefRanges_Icon;
                            imageButton3.Visible = true;
                            imageButton3.ImageDefault = Properties.Resources.ArchiveRefRanges_Icon;
                            imageButton4.Visible = true;
                            imageButton4.ImageDefault = Properties.Resources.DataFormat_Icon;
                            imageButton5.Visible = true;
                            imageButton5.ImageDefault = Properties.Resources.SoftwareUpdate_Icon;
                            imageButton6.Visible = true;
                            imageButton6.ImageDefault = Properties.Resources.FactoryDefaults_Icon;
                            imageButton7.Visible = true;                           
                            imageButton7.ImageDefault = Properties.Resources.Advanced_Icon;

                            Native.GetAbaxisProductCode(ref productCode);

                            if ( productCode == CommonDefines.PICCOLO_PRODUCT_CODE)
                            {
                                imageButton8.Visible = true;
                                imageButton8.ImageDefault = Properties.Resources.Security_Icon;
                            }

                            //navigation icon
                            imageButton10.Visible = true;
                            imageButton10.ImageDefault = Properties.Resources.LessSettings_Icon;

                            //set the tag property for the second icon matrix screen Icon Matrix 2
                            imageButton1.Tag = "Units";
                            imageButton2.Tag = "RefRanges";
                            imageButton3.Tag = "ArchiveRefRanges";
                            imageButton4.Tag = "Comm";
                            imageButton5.Tag = "Update";
                            imageButton6.Tag = "Factory";
                            imageButton7.Tag = "Advanced";
                            imageButton8.Tag = "Security";

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the icons matrix in IconsMatrixControl.cs");
            }
        }

        /// <summary>
        /// Hide all the icon image buttons
        /// </summary>
        private void HideImageControls()
        {
            imageButton1.Visible = false;
            imageButton2.Visible = false;
            imageButton3.Visible = false;
            imageButton4.Visible = false;
            imageButton5.Visible = false;
            imageButton6.Visible = false;
            imageButton7.Visible = false;
            imageButton8.Visible = false;
            imageButton9.Visible = false;
            imageButton10.Visible = false;
            imageButton11.Visible = false;
            imageButton12.Visible = false;
        }

        /// <summary>
        /// Send the icon click message to the receiving process
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage(sender);
        }

        /// <summary>
        /// Send the previous settings button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void previousSettings_Click(object sender, EventArgs e)
        {
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_PREVIOUS_SETTINGS);
        }

        /// <summary>
        /// Send the next settings button click message
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void nextSettings_Click(object sender, EventArgs e)
        {
            //next settings icon click
            SendMessageToQueue.SendIconMessage((int)GuiMessages.GuiMsgId_t.MSG_GUI_OUT_NEXT_SETTINGS);
        }
    }
}
