using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;

namespace j2GUI.j2Startup
{
    /// <summary>
    /// Error Information during start up is handled by this user control
    /// </summary>
    public partial class ErrorInformationScreen : UserControl
    {
        private WinErrorGuiDefines ErrorWinDefines = new WinErrorGuiDefines();
        private MiddleMultiText MultiTextDefines = new MiddleMultiText();

        public ErrorInformationScreen()
        {
            InitializeComponent();

            //top title color
            lblTopTitle.BackColor = ErrorWinDefines.TitleBackColor;
            lblTopTitle.Font = ErrorWinDefines.TitleFont;
            lblTopTitle.ForeColor = ErrorWinDefines.TitleFontColor;

            //multi text properties
            txtErrorInfo.Font = MultiTextDefines.Font;
            txtErrorInfo.ForeColor = MultiTextDefines.FontColor;

            //up down button properties
            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnUp.Text = "";
            btnDown.Text = "";
            
            //bottom button properties
            imageButton1.Font = ErrorWinDefines.ButtonFont;
            imageButton1.ForeColor = ErrorWinDefines.NormalButtonFontColor;
            imageButton1.PressedForeColor = ErrorWinDefines.PressedNormalButtonFontColor;
            imageButton1.GradientColors.StartColor = ErrorWinDefines.HighlightStartButtonColor;
            imageButton1.GradientColors.EndColor = ErrorWinDefines.HighlightEndButtonColor;
            imageButton1.GradientColorsPressed.StartColor = ErrorWinDefines.PressedHighlightStartButtonColor;
            imageButton1.GradientColorsPressed.EndColor = ErrorWinDefines.PressedHighlightEndButtonColor;
            imageButton1.BorderColor = ErrorWinDefines.HighlightButtonBorderColor;
            imageButton1.PressedBorderColor = ErrorWinDefines.HighlightButtonBorderColor;
        }       

        /// <summary>
        /// Display the error information text
        /// </summary>
        /// <param name="msg"></param>
        internal void SetErrorInformation(GuiReceiveMsg_t msg)
        {
            GuiReceiveMsgData_t msgData = msg.data;
            
            lblTopTitle.Text = new string(msgData.winError.topLabel);
            txtErrorInfo.Text = new string(msgData.winError.message);
            imageButton1.Text = new string(msgData.winError.buttonText);          
        }

        /// <summary>
        /// Their was a problem with startup and so analyzer will show black screen when clicked on this button
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void imageButton1_Click(object sender, EventArgs e)
        {
            //error on startup and so now only a black window is presented
            this.Hide();
        }
    }
}
