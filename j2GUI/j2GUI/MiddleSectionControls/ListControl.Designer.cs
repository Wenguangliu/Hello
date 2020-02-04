namespace j2GUI.j2MiddleSectionControls
{
    partial class ListControl
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.List = new Resco.Controls.AdvancedList.AdvancedList();
            this.rowTemplateMultiColumns = new Resco.Controls.AdvancedList.RowTemplate();
            this.textCell1 = new Resco.Controls.AdvancedList.TextCell();
            this.textCell2 = new Resco.Controls.AdvancedList.TextCell();
            this.textCell3 = new Resco.Controls.AdvancedList.TextCell();
            this.rowSelectedTemplateMultiColumns = new Resco.Controls.AdvancedList.RowTemplate();
            this.textCell4 = new Resco.Controls.AdvancedList.TextCell();
            this.textCell5 = new Resco.Controls.AdvancedList.TextCell();
            this.textCell6 = new Resco.Controls.AdvancedList.TextCell();
            this.btnDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnUp = new Resco.Controls.OutlookControls.ImageButton();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // List
            // 
            this.List.BackColor = System.Drawing.Color.White;
            this.List.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.List.DataRows.Clear();
            this.List.Location = new System.Drawing.Point(19, 19);
            this.List.Name = "List";
            this.List.SelectedTemplateIndex = 1;
            this.List.ShowScrollbar = false;
            this.List.Size = new System.Drawing.Size(442, 450);
            this.List.TabIndex = 0;
            this.List.Templates.Add(this.rowTemplateMultiColumns);
            this.List.Templates.Add(this.rowSelectedTemplateMultiColumns);
            this.List.RowSelect += new Resco.Controls.AdvancedList.RowEventHandler(this.List_RowSelect);
            // 
            // rowTemplateMultiColumns
            // 
            this.rowTemplateMultiColumns.CellTemplates.Add(this.textCell1);
            this.rowTemplateMultiColumns.CellTemplates.Add(this.textCell2);
            this.rowTemplateMultiColumns.CellTemplates.Add(this.textCell3);
            this.rowTemplateMultiColumns.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(5)))), ((int)(((byte)(55)))), ((int)(((byte)(109)))));
            this.rowTemplateMultiColumns.Name = "rowTemplateMultiColumns";
            // 
            // textCell1
            // 
            this.textCell1.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell1.CellSource.ColumnIndex = 0;
            this.textCell1.DesignName = "textCell1";
            this.textCell1.Location = new System.Drawing.Point(0, 0);
            this.textCell1.Size = new System.Drawing.Size(-1, 16);
            this.textCell1.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // textCell2
            // 
            this.textCell2.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell2.CellSource.ColumnIndex = 1;
            this.textCell2.DesignName = "textCell2";
            this.textCell2.Location = new System.Drawing.Point(0, 16);
            this.textCell2.Size = new System.Drawing.Size(-1, 16);
            this.textCell2.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // textCell3
            // 
            this.textCell3.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell3.CellSource.ColumnIndex = 2;
            this.textCell3.DesignName = "textCell3";
            this.textCell3.Location = new System.Drawing.Point(0, 32);
            this.textCell3.Size = new System.Drawing.Size(-1, 16);
            this.textCell3.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // rowSelectedTemplateMultiColumns
            // 
            this.rowSelectedTemplateMultiColumns.BackColor = System.Drawing.SystemColors.Highlight;
            this.rowSelectedTemplateMultiColumns.CellTemplates.Add(this.textCell4);
            this.rowSelectedTemplateMultiColumns.CellTemplates.Add(this.textCell5);
            this.rowSelectedTemplateMultiColumns.CellTemplates.Add(this.textCell6);
            this.rowSelectedTemplateMultiColumns.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(5)))), ((int)(((byte)(55)))), ((int)(((byte)(109)))));
            this.rowSelectedTemplateMultiColumns.Name = "rowSelectedTemplateMultiColumns";
            // 
            // textCell4
            // 
            this.textCell4.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell4.CellSource.ColumnIndex = 0;
            this.textCell4.DesignName = "textCell4";
            this.textCell4.Location = new System.Drawing.Point(0, 0);
            this.textCell4.Size = new System.Drawing.Size(-1, 16);
            this.textCell4.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // textCell5
            // 
            this.textCell5.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell5.CellSource.ColumnIndex = 1;
            this.textCell5.DesignName = "textCell5";
            this.textCell5.Location = new System.Drawing.Point(0, 16);
            this.textCell5.Size = new System.Drawing.Size(-1, 16);
            this.textCell5.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // textCell6
            // 
            this.textCell6.Alignment = Resco.Controls.AdvancedList.Alignment.MiddleLeft;
            this.textCell6.CellSource.ColumnIndex = 2;
            this.textCell6.DesignName = "textCell6";
            this.textCell6.Location = new System.Drawing.Point(0, 32);
            this.textCell6.Size = new System.Drawing.Size(-1, 16);
            this.textCell6.TextFont = new System.Drawing.Font("Arial", 22F, System.Drawing.FontStyle.Regular);
            // 
            // btnDown
            // 
            this.btnDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnDown.BorderCornerSize = 0;
            this.btnDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.Location = new System.Drawing.Point(248, 559);
            this.btnDown.Name = "btnDown";
            this.btnDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnDown.Size = new System.Drawing.Size(213, 75);
            this.btnDown.TabIndex = 9;
            this.btnDown.Tag = "0";
            this.btnDown.Text = "Down";
            this.btnDown.Visible = false;
            this.btnDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseDown);
            this.btnDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseUp);
            // 
            // btnUp
            // 
            this.btnUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnUp.BorderCornerSize = 0;
            this.btnUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.Location = new System.Drawing.Point(19, 559);
            this.btnUp.Name = "btnUp";
            this.btnUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnUp.Size = new System.Drawing.Size(213, 75);
            this.btnUp.TabIndex = 8;
            this.btnUp.Tag = "0";
            this.btnUp.Text = "Up";
            this.btnUp.Visible = false;
            this.btnUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseDown);
            this.btnUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseUp);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.List);
            this.touchPanel1.Controls.Add(this.btnUp);
            this.touchPanel1.Controls.Add(this.btnDown);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // ListControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "ListControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.AdvancedList.AdvancedList List;
        private Resco.Controls.AdvancedList.RowTemplate rowTemplateMultiColumns;
        private Resco.Controls.AdvancedList.TextCell textCell1;
        private Resco.Controls.AdvancedList.TextCell textCell2;
        private Resco.Controls.AdvancedList.TextCell textCell3;
        private Resco.Controls.AdvancedList.RowTemplate rowSelectedTemplateMultiColumns;
        private Resco.Controls.AdvancedList.TextCell textCell4;
        private Resco.Controls.AdvancedList.TextCell textCell5;
        private Resco.Controls.AdvancedList.TextCell textCell6;
        private Resco.Controls.OutlookControls.ImageButton btnDown;
        private Resco.Controls.OutlookControls.ImageButton btnUp;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
