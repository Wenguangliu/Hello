using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using CppCsWrapper;
using Resco.Controls.AdvancedList;

namespace j2GUI.j2MiddleSectionControls
{
    /// <summary>  
    /// LISTBOX + UP AND DOWN CONTROLS
    /// This middle sub section has a list box which can be programmed dynamically to be a multi select or single select or no select. Also the 
    /// list box can be programmatically set to one or two or three columns depending on the column width. The template used by the list box has 
    /// always three columns but depending on the message Id, the columns are shown or not shown.
    /// </summary>
    public partial class ListControl : UserControl
    {
        private Utilities Util = new Utilities();
        private MiddleSectionList ListDefines = new MiddleSectionList();
        private Messaging.SendQueue SendMessageToQueue;
        private Timer RepeatTimer = new Timer();        
        private bool IsUpPressed = true;
        private int UserTag;

        private int FirstColumnWidth;
        private int SecondColumnWidth;
        private int ThirdColumnWidth;

        public ListControl()
        {
            InitializeComponent();

            touchPanel1.BackgroundImage = Properties.Resources._480x680_Background;

            RepeatTimer.Tick += new System.EventHandler(RepeatTimer_Tick);
            RepeatTimer.Interval = GuiDefines.REPEATTIMERINTERVAL; //used for repeated scrolling    

            SendMessageToQueue = new j2GUI.Messaging.SendQueue();

            //initialize the controls
            btnUp.ImageDefault = Properties.Resources._213x75_scrollup_normal;
            btnUp.ImagePressed = Properties.Resources._213x75_scrollup_down;

            btnDown.ImageDefault = Properties.Resources._213x75_scrolldown_normal;
            btnDown.ImagePressed = Properties.Resources._213x75scrolldown_down;

            btnUp.Text = "";
            btnDown.Text = "";
            
            rowTemplateMultiColumns.Height = ListDefines.RowHeight;
            rowSelectedTemplateMultiColumns.Height = ListDefines.RowHeight;
            rowSelectedTemplateMultiColumns.BackColor = ListDefines.SelectedBackColor;           
        }
            
        /// <summary>
        /// Set the cell properties like height and border type for all the three cells in a row
        /// </summary>
        /// <param name="column">column number</param>
        private void SetColumn(int column)
        {
            RowTemplate.CellCollection collection = rowTemplateMultiColumns.CellTemplates;
            RowTemplate.CellCollection selectedCollection = rowSelectedTemplateMultiColumns.CellTemplates;

            switch (column)
            {
                case 1:
                    {
                        Cell listCell = collection[0];
                        Cell listSelectedCell = selectedCollection[0];
                        listCell.Bounds = new Rectangle(0, 0, FirstColumnWidth, List.DataRows[0].Height);
                        listSelectedCell.Bounds = new Rectangle(0, 0, FirstColumnWidth, List.DataRows[0].Height);
                        listCell.Border = BorderType.Flat;
                        break;
                    }
                case 2:
                    {
                        Cell listCell = collection[1];
                        Cell listSelectedCell = selectedCollection[1];
                        listCell.Bounds = new Rectangle(FirstColumnWidth, 0, SecondColumnWidth, List.DataRows[0].Height);
                        listSelectedCell.Bounds = new Rectangle(FirstColumnWidth, 0, SecondColumnWidth, List.DataRows[0].Height);
                        listCell.Border = BorderType.Flat;
                        break;
                    }
                case 3:
                    {
                        Cell listCell = collection[2];
                        Cell listSelectedCell = selectedCollection[2];
                        listCell.Bounds = new Rectangle(FirstColumnWidth + SecondColumnWidth, 0, ThirdColumnWidth, List.DataRows[0].Height);
                        listSelectedCell.Bounds = new Rectangle(FirstColumnWidth + SecondColumnWidth, 0, ThirdColumnWidth, List.DataRows[0].Height);
                        listCell.Border = BorderType.Flat;
                        break;
                    }
            }
        }

        /// <summary>
        /// Sets the list control on the middle sub section with the correct properties and column widths depending on the
        /// Gui message receive Ids
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void SetListControl(CppCsWrapper.GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;
                char hasUpDown = GuiDefines.TRUE;//initialize

                //initialize to beginwith
                List.SelectionMode = SelectionMode.NoSelect;
                List.MultiSelect = false;

                List.DataRows.Clear();

                switch (msg.msgId)
                {
                        //multi select list box
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_MULTI):
                        {                           
                            hasUpDown = messagedata.middleListMulti.hasUpDown;//indicates if up and down buttons need to be displayed
                            List.SelectionMode = SelectionMode.SelectDeselect;
                            List.MultiSelect = true;  //allows multiple rows to be selected

                            ShowMultiSelectList(msg);

                            //the first column width takes up the whole width of the list control for this message Id
                            FirstColumnWidth = -1;
                            SecondColumnWidth = 0;
                            ThirdColumnWidth = 0;

                            SetColumn(1);
                            SetColumn(2);
                            SetColumn(3);

                            break;
                        }

                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST):
                        {
                            hasUpDown = messagedata.middleList.hasUpDown;
                            ShowMultiColumnList(messagedata.middleList.items);

                            //set the selection mode for the list box
                            if (messagedata.middleList.isSelectable != GuiDefines.FALSE)
                            {
                                List.SelectionMode = SelectionMode.SelectDeselect;
                            }
                            else
                            {
                                List.SelectionMode = SelectionMode.NoSelect;
                            }

                            //the first column width takes up the whole width of the list control for this message Id
                            FirstColumnWidth = -1;
                            SecondColumnWidth = 0;
                            ThirdColumnWidth = 0;

                            SetColumn(1);
                            SetColumn(2);
                            SetColumn(3);
                                  
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_REF_RANGES):
                        {
                            hasUpDown = messagedata.middleList.hasUpDown;
                            ShowMultiColumnList(messagedata.middleList.items);

                            if (messagedata.middleList.isSelectable != GuiDefines.FALSE)
                            {
                                List.SelectionMode = SelectionMode.SelectDeselect;
                            }
                            else
                            {
                                List.SelectionMode = SelectionMode.NoSelect;
                            }

                            //list reference range has three columns of information
                            FirstColumnWidth = 150;
                            SecondColumnWidth = 180;
                            ThirdColumnWidth = 110;

                            SetColumn(1);
                            SetColumn(2);
                            SetColumn(3);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_SETTINGS):
                        {
                            hasUpDown = messagedata.middleList.hasUpDown;
                            ShowMultiColumnList(messagedata.middleList.items);

                            if (messagedata.middleList.isSelectable != GuiDefines.FALSE)
                            {
                                List.SelectionMode = SelectionMode.SelectDeselect;
                            }
                            else
                            {
                                List.SelectionMode = SelectionMode.NoSelect;
                            }

                            //show only two columns
                            FirstColumnWidth = 220;
                            SecondColumnWidth = 220;
                            ThirdColumnWidth = 0;

                            SetColumn(1);
                            SetColumn(2);
                            SetColumn(3);

                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_MIDDLE_LIST_RESULTS):
                        {
                            hasUpDown = messagedata.middleList.hasUpDown;
                            ShowMultiColumnList(messagedata.middleList.items);

                            if (messagedata.middleList.isSelectable != GuiDefines.FALSE)
                            {
                                List.SelectionMode = SelectionMode.SelectDeselect;
                            }
                            else
                            {
                                List.SelectionMode = SelectionMode.NoSelect;
                            }

                            //show all the three columns
                            FirstColumnWidth = 32;
                            SecondColumnWidth = 168;
                            ThirdColumnWidth = 240;

                            SetColumn(1);
                            SetColumn(2);
                            SetColumn(3);

                            break;
                        }                  
                }

                if (hasUpDown != GuiDefines.FALSE)
                {
                    btnUp.Visible = true;
                    btnDown.Visible = true;
                }
                else
                {
                    btnUp.Visible = false;
                    btnDown.Visible = false;
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the List Control in ListControl.cs");
            }
        }

        /// <summary>
        /// populate the list with the data received from the message queue
        /// </summary>
        /// <param name="items">items array as received in the message data</param>
        private void ShowMultiColumnList(char[, ,] items)
        {
            char[,] listItemsChars = new char[items.GetUpperBound(1) + 1, items.GetUpperBound(2) + 1];

            string[] listItems = new string[items.GetUpperBound(1) + 1];                  

            for (int first = 0; first < items.GetUpperBound(0) + 1; first++)
            {
                for (int second = 0; second < items.GetUpperBound(1) + 1; second++)
                {
                    for (int third = 0; third < items.GetUpperBound(2) + 1; third++)
                    {
                        listItemsChars[second, third] = items[first, second, third];
                    }
                }

                listItems = Util.TwoCharArrayToStringArray(listItemsChars);                

                //once you hit an empty string, stop adding to the list
                if (listItems[0] == string.Empty)
                {
                    break;
                }

                CreateList(listItems);
            }
        }
       
        /// <summary>
        /// Add each row item to the data row in the list box control
        /// </summary>
        /// <param name="items">items that has to be added to the data rows in the list box</param>
        private void CreateList(string[] items)
        {
            Row row = new Row(0, 1, items);

            List.DataRows.Add(row);
        }
        /// <summary>
        /// populate the multi select list with the data received from the message queue
        /// </summary>
        /// <param name="msg">recieved message</param>
        private void ShowMultiSelectList(GuiReceiveMsg_t msg)
        {
            GuiReceiveMsgData_t messagedata = msg.data;

            string[] itemsText = Util.TwoCharArrayToStringArray(messagedata.middleListMulti.items);
            
            string[] s = new string[1];
            for (int ind = 0; ind < (itemsText.GetUpperBound(0) + 1); ind++)
            {
                Array.Copy(itemsText, ind, s, 0, 1);

                //once you get a empty string in the list, dont add any more list items
                if (s[0] == string.Empty)
                {
                    break;
                }

                Row row = new Row(0, 1, s);

                if (messagedata.middleListMulti.isSelected[ind] != GuiDefines.FALSE)
                {
                    row.Selected = true;
                }
                else
                {
                    row.Selected = false;
                }

                List.DataRows.Add(row);
            }
        }

        /// <summary>
        /// When the row gets selected, send out a message from the Gui process with the row index
        /// selected
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void List_RowSelect(object sender, RowEventArgs e)
        {
            AdvancedList list = (AdvancedList)sender;

            if (list.MultiSelect)
            {
                SendMessageToQueue.SendMultiListSelectedMessage(sender);
            }
            else
            {
                SendMessageToQueue.SendListSelectedMessage(sender);
            }
        }

        /// <summary>
        /// Update the list control with new set of item data after clearing out the previous ones
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void UpdateListControl(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messageData = msg.data;

                switch (msg.msgId)
                {
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_RESULTS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_REF_RANGES):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_SETTINGS):
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST):
                        {
                            UpdateOneSelectListControl(msg);
                            break;
                        }
                    case ((int)GuiMessages.GuiMsgId_t.MSG_GUI_UPDATE_LIST_MULTI):
                        {
                            UpdateMultiSelectListControl(msg);
                            break;
                        }
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the List control in UpdateListControl.cs");
            }
        }

        /// <summary>
        /// Update the list control with new set of item data after clearing out the previous ones
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void UpdateOneSelectListControl(GuiReceiveMsg_t msg)
        {
            try
            {
                GuiReceiveMsgData_t messagedata = msg.data;
                char hasUpDown = GuiDefines.TRUE;//initialize
                List.DataRows.Clear();

                hasUpDown = messagedata.middleList.hasUpDown;
                ShowMultiColumnList(messagedata.middleList.items);

                //set the selection mode for the list box
                if (messagedata.middleList.isSelectable != GuiDefines.FALSE)
                {
                    List.SelectionMode = SelectionMode.SelectDeselect;
                }
                else
                {
                    List.SelectionMode = SelectionMode.NoSelect;
                }
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to set the Multi Column List Control in ListControl.cs");
            }
        }

        /// <summary>
        /// Update the multi select list control with new set of item data after clearing out the previous ones
        /// </summary>
        /// <param name="msg">recieved message</param>
        internal void UpdateMultiSelectListControl(GuiReceiveMsg_t msg)
        {
            try
            {
                char hasUpDown = GuiDefines.TRUE;//initialize

                List.DataRows.Clear();

                hasUpDown = msg.data.middleListMulti.hasUpDown;//indicates if up and down buttons need to be displayed
                List.SelectionMode = SelectionMode.SelectDeselect;
                List.MultiSelect = true;  //allows multiple rows to be selected

                ShowMultiSelectList(msg);
            }
            catch (System.Exception e)
            {
                SendMessageToQueue.SendLogErrorMessage("Unable to update the Multi Select List Control in ListControl.cs");
            }
        }

        /// <summary>
        /// Called when the Up button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;

            //this will be called if the click on the button is quicker than the timer tick to trigger ie when the user wants 
            //to just increment by one and not continuously
            SendMessageToQueue.SendUpMessage(sender);

        }

        /// <summary>
        /// Called when the Up button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUp_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = true;//an up button is clicked

            UserTag = SendMessageToQueue.GetTagData(sender);//get the tag of the button clicked. It can be either Day or Month or Year button
        }
        /// <summary>
        /// Called when the Down button is pressed down
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseDown(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = true;
            IsUpPressed = false;

            UserTag = SendMessageToQueue.GetTagData(sender);
        }

        /// <summary>
        /// Called when the Down button is pressed down and released
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDown_MouseUp(object sender, MouseEventArgs e)
        {
            RepeatTimer.Enabled = false;
            SendMessageToQueue.SendDownMessage(sender);
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
                SendMessageToQueue.SendUpMessage(UserTag);
            }
            else
            {
                SendMessageToQueue.SendDownMessage(UserTag);
            }
        }
    }
}
