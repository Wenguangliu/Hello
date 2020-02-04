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
    /// <summary>  
    /// GRAPHICS + MESSAGE CONTROL
    /// This middle sub section has the graphics and a message label to display the information. The graphics displayed various depending
    /// on the message Id received by the Gui message queue
    /// </summary>
    public partial class GraphicsInfoControl : UserControl
    {
        private MiddleSectionInformationMessage InformationMessageDefines = new MiddleSectionInformationMessage();
        private Messaging.SendQueue SendMessageToQueue = new j2GUI.Messaging.SendQueue();

        public GraphicsInfoControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            lblInformation.Font = InformationMessageDefines.Font;
            lblInformation.ForeColor = InformationMessageDefines.FontColor;
        }

        /// <summary>
        /// Sets the correct controls on the user control screen depending on the message Id received. 
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetGraphicsControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
             try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                lblInformation.Text = new string(messageData.middleGraphicInfo.message);

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO1):
                        {
                            pictureBoxGraphics.Image = Properties.Resources.J2vs2_step1;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO2):
                        {
                            pictureBoxGraphics.Image = Properties.Resources.J2vs2_step2;

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO3):
                        {
                            pictureBoxGraphics.Image = Properties.Resources.J2vs2_step3;

                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the graphics info control in GraphicsInfoControl.cs");
            }
        }
    }
}
