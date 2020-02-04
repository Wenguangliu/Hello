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
    /// Middle section user control can place different varieties of middle sub sections like MessageInfo, Buttons, ButtonInfo etc. To begin with, all the controls are created during GUI
    /// start process and then they are hidden. Depending on the various message ids, the middle sub sections are shown.
    /// </summary>
    public partial class MiddleSectionControls : UserControl
    {
        private Messaging.SendQueue SendMessageQueue = new j2GUI.Messaging.SendQueue();

        private const int CONTROLINFOCOUNT = 4;
        private const int NOCONTROLS = 0;
        private const int MULTICONTROLCOUNT = 6;

        j2MiddleSectionControls.MessageInfoControl MessageInfo;
        j2MiddleSectionControls.ButtonsInfoControl ButtonsInfo;
        j2MiddleSectionControls.SelectInfoControl SelectInfo;
        j2MiddleSectionControls.OptionInfoControl OptionInfo;
        j2MiddleSectionControls.HomeScreenControl HomeScreen;
        j2MiddleSectionControls.ProgessControl Progress;
        j2MiddleSectionControls.ListControl List;
        j2MiddleSectionControls.ViewControl View;
        j2MiddleSectionControls.DateControl Date;
        j2MiddleSectionControls.TimeControl Time;
        j2MiddleSectionControls.AlphaNumericKeyboardControl AlphaNumericKeyPad;
        j2MiddleSectionControls.TypeControl Type;
        j2MiddleSectionControls.GraphicsInfoControl GraphicsInfo;
        j2MiddleSectionControls.IconsMatrixControl IconMatrix;
        j2MiddleSectionControls.ValueControl Value;
        j2MiddleSectionControls.RangeControl Range;
        j2MiddleSectionControls.TemperatureControl Temperature; //added nwr 4/25/2013
        j2MiddleSectionControls.MotorControl Motor; //added nwr 5/6/2013
        j2MiddleSectionControls.BarcodeControl Barcode; //added nwr 5/7/2013
        j2MiddleSectionControls.CuvetteControl Cuvette; //added nwr 5/30/2013
        j2MiddleSectionControls.LoadOrderControl LoadOrder; //added by nj 12/16/2015
        j2MiddleSectionControls.ListColoredControl ListColored; //added by nj 12/16/2015
       
        public MiddleSectionControls()
        {
            InitializeComponent();

            //touchPanel1.BackgroundImage = Properties.Resources.J2background;

            this.Height = GuiForm.MiddleFormHeight;
            this.Location = new Point(0, GuiForm.TopFormHeight);

            //for front panel tester BottomButtonsUp Message ID
            //this.Height = GuiForm.MiddleFormHeight;
            //this.Location = new Point(0, GuiForm.TopFormHeight + GuiForm.BottomFormHeight);

            InitializeMiddleSectionControls();
        }

        /// <summary>
        /// Initialize the AlphaNumericKeyPad object
        /// </summary>
        /// <param name="keyboard"></param>
        public void SetAlphaNumericKeyPad(j2GUI.j2MiddleSectionControls.AlphaNumericKeyboardControl keyboard)
        {
            AlphaNumericKeyPad = keyboard;
        }

        private void InitializeMiddleSectionControls()
        {
            MessageInfo = new j2GUI.j2MiddleSectionControls.MessageInfoControl();
            ButtonsInfo = new j2GUI.j2MiddleSectionControls.ButtonsInfoControl();
            SelectInfo = new j2GUI.j2MiddleSectionControls.SelectInfoControl();
            OptionInfo = new j2GUI.j2MiddleSectionControls.OptionInfoControl();
            HomeScreen = new j2GUI.j2MiddleSectionControls.HomeScreenControl();
            Progress = new j2GUI.j2MiddleSectionControls.ProgessControl();
            List = new j2GUI.j2MiddleSectionControls.ListControl();
            View = new j2GUI.j2MiddleSectionControls.ViewControl();
            Date = new j2GUI.j2MiddleSectionControls.DateControl();
            Time = new j2GUI.j2MiddleSectionControls.TimeControl();
            LoadOrder = new j2GUI.j2MiddleSectionControls.LoadOrderControl();
            ListColored = new j2GUI.j2MiddleSectionControls.ListColoredControl();

            //this is taken out and moved to MainScreen.cs since the Keyboard pro control 
            //can only be added to a Form control and so events need to be passed between this and MainScreen
            //It is easy to add event handlers to AlphaNumericKeyboardControl class in MainScreen.cs
            //NumberKeyPad = new j2GUI.j2MiddleSectionControls.AlphaNumericKeyboardControl();

            Type = new j2GUI.j2MiddleSectionControls.TypeControl();
            GraphicsInfo = new j2GUI.j2MiddleSectionControls.GraphicsInfoControl();
            IconMatrix = new j2GUI.j2MiddleSectionControls.IconsMatrixControl();
            Value = new j2GUI.j2MiddleSectionControls.ValueControl();
            Range = new j2GUI.j2MiddleSectionControls.RangeControl();

            Temperature = new j2GUI.j2MiddleSectionControls.TemperatureControl(); //added nwr 4/25/2013
            Motor = new j2GUI.j2MiddleSectionControls.MotorControl(); //added nwr 5/6/2013
            Barcode = new j2GUI.j2MiddleSectionControls.BarcodeControl(); //added nwr 5/7/2013
            Cuvette = new j2GUI.j2MiddleSectionControls.CuvetteControl(); //added nwr 5/30/2013
        }

        /// <summary>
        /// To begin with, hide all the middle sub section controls. Then show only which needs to be displayed.
        /// Update the middle section with the correct set of controls
        /// </summary>
        /// <param name="msg">recieved data</param>
        internal void SetMiddleControl(GuiReceiveMsg_t msg)
        {
            try
            {
                //to beginwith, hide all the controls in the user control
                HideMiddleSectionControls();
                                
                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_NONE):
                        {
                            this.Hide();//hide the middle section control

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS_INFO):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BUTTONS):
                        {
                            ButtonsInfo.SetButtonsInfoControl(msg);
                            this.Controls.Add(ButtonsInfo);
                            ButtonsInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE_INFO):
                        {
                            MessageInfo.SetMessageInfoControl(msg);
                            this.Controls.Add(MessageInfo);
                            MessageInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MESSAGE):
                        {
                            MessageInfo.SetMessageInfoControl(msg);
                            this.Controls.Add(MessageInfo);
                            MessageInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_INFO):
                        {
                            SelectInfo.SetSelectInfoControl(msg);
                            this.Controls.Add(SelectInfo);
                            SelectInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MULTI_SELECT):
                        {
                            SelectInfo.SetSelectInfoControl(msg);
                            this.Controls.Add(SelectInfo);
                            SelectInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_MFG):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_READY_RD):    //added nwr 5/14/2013
                        {
                            HomeScreen.SetHomeScreenControl(msg);
                            this.Controls.Add(HomeScreen);
                            HomeScreen.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_MFG):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_HOME_WARMING_RD):  //added nwr 5/14/2013
                        {
                            Progress.SetProgressControl(msg);
                            this.Controls.Add(Progress);
                            Progress.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_PROG_BAR_MESSAGE):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ANALYSIS_IN_PROCESS):
                        {
                            Progress.SetProgressControl(msg);
                            this.Controls.Add(Progress);
                            Progress.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_OPTION):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_OPTION_INFO):
                        {
                            OptionInfo.SetOptionInfoControl(msg);
                            this.Controls.Add(OptionInfo);
                            OptionInfo.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_MULTI):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_RESULTS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_SETTINGS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_REF_RANGES):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST):
                        {
                            List.SetListControl(msg);
                            this.Controls.Add(List);
                            List.Show();

                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_ID):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_IP):
                        {
                            AlphaNumericKeyPad.SetNumberKeyPad(msg);
                            this.Controls.Add(AlphaNumericKeyPad);
                            AlphaNumericKeyPad.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_SFONT): //added nwr 5/14/2013
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_VIEW_RESULTS_MFONT): //added nwr 7/24/2013
                        {
                            View.SetViewControl(msg);
                            this.Controls.Add(View);
                            View.Show();
                            this.Show();

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_DATE):
                        {
                            Date.SetDateControl(msg);
                            this.Controls.Add(Date);
                            Date.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_TIME):
                        {
                            Time.SetTimeControl(msg);
                            this.Controls.Add(Time);
                            Time.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_SELECT_TYPE):
                        {
                            Type.SetTypeControl(msg);
                            this.Controls.Add(Type);
                            Type.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO1):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO2):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_GRAPHIC_INFO3):
                        {
                            GraphicsInfo.SetGraphicsControl(msg);
                            this.Controls.Add(GraphicsInfo);
                            GraphicsInfo.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_TEMPERATURE_INFO):
                        {
                            Temperature.SetTemperatureControl(msg);
                            this.Controls.Add(Temperature);
                            Temperature.Show();
                            this.Show();
                            break;

                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_MOTOR_DATA):
                        {
                            //added nwr 5/6/2013
                            Motor.SetMotorControl(msg);
                            this.Controls.Add(Motor);
                            Motor.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_BARCODE_DATA):
                        {
                            //added nwr 5/6/2013
                            Barcode.SetBarcodeControl(msg);
                            this.Controls.Add(Barcode);
                            Barcode.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_1):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ICON_MATRIX_2):
                        {
                            IconMatrix.SetIconsControl(msg);
                            this.Controls.Add(IconMatrix);
                            IconMatrix.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LOAD_ORDER):
                        {
                            //nj added on 12/16/2015 for Order project
                            LoadOrder.SetLoadOrderControl(msg);                            
                            this.Controls.Add(LoadOrder);
                            LoadOrder.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_COLORED):
                        {
                            //nj added on 12/16/2015 for Orders project                               
                            ListColored.SetListColoredControl(msg);
                            this.Controls.Add(ListColored);
                            ListColored.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_VALUE):
                        {
                            Value.SetValueControl(msg);
                            this.Controls.Add(Value);
                            Value.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_ENTER_RANGE):
                        {
                            Range.SetRangeControl(msg);
                            this.Controls.Add(Range);
                            Range.Show();
                            this.Show();
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_CUVETTE_DATA):
                        {
                            Cuvette.SetCuvetteControl(msg);
                            this.Controls.Add(Cuvette);
                            Cuvette.Show();
                            this.Show();
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
                SendMessageQueue.SendLogErrorMessage("Failed to Set the Middle controls in SetMiddleControl.cs");
            }
        }

        /// <summary>
        /// Hide all the middle sub section windows to begin with
        /// </summary>
        private void HideMiddleSectionControls()
        {
            MessageInfo.Hide();
            ButtonsInfo.Hide();
            SelectInfo.Hide();
            OptionInfo.Hide();
            HomeScreen.Hide();
            Progress.Hide();
            View.Hide();
            AlphaNumericKeyPad.Hide();
            List.Hide();
            Date.Hide();
            Time.Hide();
            Time.StopTimer();//stop this timer if the active window on the screen is not 'EnterTime' middle sub section
            Type.Hide();
            GraphicsInfo.Hide();
            IconMatrix.Hide();
            Value.Hide();
            Range.Hide();
            Temperature.Hide(); //added nwr 5/3/2013
            Motor.Hide(); //added nwr 5/6/2013
            Barcode.Hide(); //added nwr 5/7/2013
            Cuvette.Hide(); //added nwr 5/30/2013
            LoadOrder.Hide(); //nj added on 12/18/2015
            ListColored.Hide(); //nj added on 12/18/2015
        }

        internal void UpdateMiddleControl(GuiReceiveMsg_t msg)
        {
            try
            {
                //this is just an update message from the other process to the Gui, so dont show or hide the screen
                //or load or unload any controls
                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_LEFT):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_MIDDLE):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_DATE_RIGHT):
                        {
                            Date.UpdateDateControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_ID):
                        {
                            AlphaNumericKeyPad.UpdateNumberKeyPad(msg);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_IP):
                        {
                            AlphaNumericKeyPad.UpdateNumberKeyPad(msg);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ENTER_VALUE_NEW):
                        {
                            Value.UpdateValue(msg);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_TEXT):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_PROGRESS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_PROG_BAR_MESSAGE_TEXT):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_ANALYSIS_IN_PROCESS_PROGRESS):

                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_PROGRESS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_MFG_PROGRESS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_HOME_WARMING_RD_PROGRESS):
                        {
                            Progress.UpdateProgressControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_RESULTS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_REF_RANGES):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_SETTINGS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_MULTI):
                        {
                            List.UpdateListControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_TEMPERATURE_VALUES):  //added nwr 4/24/2013
                        {
                            Temperature.UpdateTemperatureControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_READ_MOTOR_DATA):
                        {
                            Motor.UpdateMotorControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_BARCODE_DATA):
                        {
                            Barcode.UpdateBarcodeControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_CUVETTE_DATA):
                        {
                            Cuvette.UpdateCuvetteControl(msg);                            
                            break;
                        }
                    default:
                        {
                            break;
                        }

                }
            }
            catch (System.Exception e)
            {
                SendMessageQueue.SendLogErrorMessage("Failed to update the middle controls in UpdateMiddleControl.cs");
            }
        }

        /// <summary>
        /// Get the Id text
        /// </summary>
        internal void GetIdText()
        {
            try
            {
                AlphaNumericKeyPad.SendIdText();
            }
            catch (System.Exception ex)
            {
                SendMessageQueue.SendLogErrorMessage("Failed to Get Id Text in MiddleSectionControls.cs");
            }
        }

        /// <summary>
        /// Get the Range to send it to the receiving process
        /// </summary>
        internal void GetRange()
        {
            try
            {
                Range.SendRange();
            }
            catch (System.Exception ex)
            {
                SendMessageQueue.SendLogErrorMessage("Failed to Get Range in MiddleSectionControls.cs");
            }
        }
    }
}
