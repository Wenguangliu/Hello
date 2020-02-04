namespace j2GUI.j2MiddleSectionControls
{
    partial class TimeControl
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
            this.txtCurrentTime = new Resco.Controls.CommonControls.TouchTextBox();
            this.lblMinute = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblHour = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnMinuteDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnMinuteUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnHourDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnHourUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnZeroSecond = new Resco.Controls.OutlookControls.ImageButton();
            this.btnHourFormat = new Resco.Controls.OutlookControls.ImageButton();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.btnMinuteDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnMinuteUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnZeroSecond)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourFormat)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtCurrentTime
            // 
            this.txtCurrentTime.BackColor = System.Drawing.SystemColors.Window;
            this.txtCurrentTime.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtCurrentTime.Enabled = false;
            this.txtCurrentTime.Location = new System.Drawing.Point(20, 70);
            this.txtCurrentTime.Name = "txtCurrentTime";
            this.txtCurrentTime.ReadOnly = true;
            this.txtCurrentTime.Size = new System.Drawing.Size(440, 75);
            this.txtCurrentTime.TabIndex = 0;
            this.txtCurrentTime.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblMinute
            // 
            this.lblMinute.AutoSize = false;
            this.lblMinute.Location = new System.Drawing.Point(247, 156);
            this.lblMinute.Name = "lblMinute";
            this.lblMinute.Size = new System.Drawing.Size(213, 45);
            this.lblMinute.Text = "Minute";
            this.lblMinute.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblHour
            // 
            this.lblHour.AutoSize = false;
            this.lblHour.Location = new System.Drawing.Point(20, 156);
            this.lblHour.Name = "lblHour";
            this.lblHour.Size = new System.Drawing.Size(213, 45);
            this.lblHour.Text = "Hour";
            this.lblHour.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // btnMinuteDown
            // 
            this.btnMinuteDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteDown.BorderCornerSize = 0;
            this.btnMinuteDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteDown.Location = new System.Drawing.Point(247, 295);
            this.btnMinuteDown.Name = "btnMinuteDown";
            this.btnMinuteDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnMinuteDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnMinuteDown.Size = new System.Drawing.Size(213, 75);
            this.btnMinuteDown.TabIndex = 23;
            this.btnMinuteDown.Text = "Down";
            this.btnMinuteDown.Click += new System.EventHandler(this.btnMinuteDown_Click);
            // 
            // btnMinuteUp
            // 
            this.btnMinuteUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteUp.BorderCornerSize = 0;
            this.btnMinuteUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteUp.Location = new System.Drawing.Point(247, 205);
            this.btnMinuteUp.Name = "btnMinuteUp";
            this.btnMinuteUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnMinuteUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnMinuteUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnMinuteUp.Size = new System.Drawing.Size(213, 75);
            this.btnMinuteUp.TabIndex = 22;
            this.btnMinuteUp.Text = "Up";
            this.btnMinuteUp.Click += new System.EventHandler(this.btnMinuteUp_Click);
            // 
            // btnHourDown
            // 
            this.btnHourDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnHourDown.BorderCornerSize = 0;
            this.btnHourDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourDown.Location = new System.Drawing.Point(20, 295);
            this.btnHourDown.Name = "btnHourDown";
            this.btnHourDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnHourDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnHourDown.Size = new System.Drawing.Size(213, 75);
            this.btnHourDown.TabIndex = 21;
            this.btnHourDown.Text = "Down";
            this.btnHourDown.Click += new System.EventHandler(this.btnHourDown_Click);
            // 
            // btnHourUp
            // 
            this.btnHourUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnHourUp.BorderCornerSize = 0;
            this.btnHourUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourUp.Location = new System.Drawing.Point(20, 205);
            this.btnHourUp.Name = "btnHourUp";
            this.btnHourUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnHourUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnHourUp.Size = new System.Drawing.Size(213, 75);
            this.btnHourUp.TabIndex = 20;
            this.btnHourUp.Text = "Up";
            this.btnHourUp.Click += new System.EventHandler(this.btnHourUp_Click);
            // 
            // btnZeroSecond
            // 
            this.btnZeroSecond.BorderColor = System.Drawing.Color.Transparent;
            this.btnZeroSecond.BorderCornerSize = 0;
            this.btnZeroSecond.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnZeroSecond.Location = new System.Drawing.Point(247, 425);
            this.btnZeroSecond.Name = "btnZeroSecond";
            this.btnZeroSecond.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnZeroSecond.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnZeroSecond.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnZeroSecond.Size = new System.Drawing.Size(213, 75);
            this.btnZeroSecond.TabIndex = 25;
            this.btnZeroSecond.Text = "Zero Sec";
            this.btnZeroSecond.Click += new System.EventHandler(this.btnZeroSecond_Click);
            // 
            // btnHourFormat
            // 
            this.btnHourFormat.BorderColor = System.Drawing.Color.Transparent;
            this.btnHourFormat.BorderCornerSize = 0;
            this.btnHourFormat.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourFormat.Location = new System.Drawing.Point(20, 425);
            this.btnHourFormat.Name = "btnHourFormat";
            this.btnHourFormat.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnHourFormat.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnHourFormat.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnHourFormat.Size = new System.Drawing.Size(213, 75);
            this.btnHourFormat.TabIndex = 24;
            this.btnHourFormat.Tag = "0";
            this.btnHourFormat.Text = "12/24";
            this.btnHourFormat.Click += new System.EventHandler(this.btnHourFormat_Click);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.btnMinuteUp);
            this.touchPanel1.Controls.Add(this.btnZeroSecond);
            this.touchPanel1.Controls.Add(this.txtCurrentTime);
            this.touchPanel1.Controls.Add(this.btnHourFormat);
            this.touchPanel1.Controls.Add(this.btnHourUp);
            this.touchPanel1.Controls.Add(this.lblMinute);
            this.touchPanel1.Controls.Add(this.btnHourDown);
            this.touchPanel1.Controls.Add(this.lblHour);
            this.touchPanel1.Controls.Add(this.btnMinuteDown);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // TimeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "TimeControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnMinuteDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnMinuteUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnZeroSecond)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHourFormat)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TransparentLabel lblMinute;
        private Resco.Controls.CommonControls.TransparentLabel lblHour;
        private Resco.Controls.OutlookControls.ImageButton btnMinuteDown;
        private Resco.Controls.OutlookControls.ImageButton btnMinuteUp;
        private Resco.Controls.OutlookControls.ImageButton btnHourDown;
        private Resco.Controls.OutlookControls.ImageButton btnHourUp;
        private Resco.Controls.OutlookControls.ImageButton btnZeroSecond;
        private Resco.Controls.OutlookControls.ImageButton btnHourFormat;
        private Resco.Controls.CommonControls.TouchTextBox txtCurrentTime;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
