using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;

namespace j2GUI
{
    /// <summary>
    /// The Top User Control has the gradient panel filled into it. This panel is used as the default label
    /// control. All other controls are added dynamically depending on the message Id.
    /// </summary>
    public partial class TopSectionControls : UserControl
    {
        TopGuiDefines TopDefines;
        Messaging.SendQueue SendMessageQueue = new j2GUI.Messaging.SendQueue();

        public TopSectionControls()
        {            
            InitializeComponent();

            TopDefines = new TopGuiDefines();

            //set the top user control height and location
            this.Height = GuiForm.TopFormHeight;
            this.BackColor = TopDefines.BackColor;
            lblTopTitle.Font = TopDefines.Font;
            lblTopTitle.ForeColor = TopDefines.FontColor;
            lblTopTitle.BackColor = TopDefines.BackColor;
        }      

        /// <summary>
        /// Set the text for the transparent label
        /// </summary>
        /// <param name="title">char array that needs to be shown in the top section title window</param>
        private void LoadTopLabel(char[] title)
        {
            lblTopTitle.Text = new string(title);
        }
             
        /// <summary>
        /// Update the top section control of the mainscreen
        /// </summary>
        /// <param name="msg">the recieve msg structure which has the msg Id and the data</param>
        internal void SetTopControl(GuiReceiveMsg_t msg)
        {
            try
            {
                switch (msg.msgId)
                {
                        //no top section
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_NONE):
                        {
                            this.Hide();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_TOP_LABEL):
                        {
                            LoadTopLabel(msg.data.topLabel.label);                           
                            this.Show();//the top section may get hidden if the previous screen was a windows message. So as default show the label for this message Id again      
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
                SendMessageQueue.SendLogErrorMessage("Unable to set top control in TopSectionControls.cs");
            }
        }        
    }
}
