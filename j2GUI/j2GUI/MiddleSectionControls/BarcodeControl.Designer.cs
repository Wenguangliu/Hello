namespace j2GUI.j2MiddleSectionControls
{
    partial class BarcodeControl
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
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            this.btnDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnOpen = new Resco.Controls.OutlookControls.ImageButton();
            this.btnUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnRead = new Resco.Controls.OutlookControls.ImageButton();
            this.txtCurrent = new Resco.Controls.CommonControls.TouchTextBox();
            this.btnCalibration = new Resco.Controls.OutlookControls.ImageButton();
            this.lblCurrent = new Resco.Controls.CommonControls.TransparentLabel();
            this.txtResult = new Resco.Controls.CommonControls.TouchTextBox();
            this.touchPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnOpen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnRead)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCalibration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblCurrent)).BeginInit();
            this.SuspendLayout();
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.btnDown);
            this.touchPanel1.Controls.Add(this.btnOpen);
            this.touchPanel1.Controls.Add(this.btnUp);
            this.touchPanel1.Controls.Add(this.btnRead);
            this.touchPanel1.Controls.Add(this.txtCurrent);
            this.touchPanel1.Controls.Add(this.btnCalibration);
            this.touchPanel1.Controls.Add(this.lblCurrent);
            this.touchPanel1.Controls.Add(this.txtResult);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // btnDown
            // 
            this.btnDown.Location = new System.Drawing.Point(247, 385);
            this.btnDown.Name = "btnDown";
            this.btnDown.Size = new System.Drawing.Size(213, 75);
            this.btnDown.TabIndex = 7;
            this.btnDown.Text = "Down";
            this.btnDown.Click += new System.EventHandler(this.DownButton_Click);
            // 
            // btnOpen
            // 
            this.btnOpen.Location = new System.Drawing.Point(20, 385);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.Size = new System.Drawing.Size(213, 75);
            this.btnOpen.TabIndex = 6;
            this.btnOpen.Tag = "2";
            this.btnOpen.Text = "Open";
            this.btnOpen.Click += new System.EventHandler(this.Button_Click);
            // 
            // btnUp
            // 
            this.btnUp.Location = new System.Drawing.Point(247, 295);
            this.btnUp.Name = "btnUp";
            this.btnUp.Size = new System.Drawing.Size(213, 75);
            this.btnUp.TabIndex = 5;
            this.btnUp.Text = "Up";
            this.btnUp.Click += new System.EventHandler(this.UpButton_Click);
            // 
            // btnRead
            // 
            this.btnRead.Location = new System.Drawing.Point(20, 295);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(213, 75);
            this.btnRead.TabIndex = 4;
            this.btnRead.Tag = "1";
            this.btnRead.Text = "Read";
            this.btnRead.Click += new System.EventHandler(this.Button_Click);
            // 
            // txtCurrent
            // 
            this.txtCurrent.BackColor = System.Drawing.SystemColors.Window;
            this.txtCurrent.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtCurrent.Enabled = false;
            this.txtCurrent.Location = new System.Drawing.Point(247, 207);
            this.txtCurrent.Name = "txtCurrent";
            this.txtCurrent.Size = new System.Drawing.Size(213, 75);
            this.txtCurrent.TabIndex = 3;
            this.txtCurrent.Text = "txtCurrent";
            this.txtCurrent.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btnCalibration
            // 
            this.btnCalibration.Location = new System.Drawing.Point(20, 205);
            this.btnCalibration.Name = "btnCalibration";
            this.btnCalibration.Size = new System.Drawing.Size(213, 75);
            this.btnCalibration.TabIndex = 2;
            this.btnCalibration.Tag = "0";
            this.btnCalibration.Text = "Calibrate";
            this.btnCalibration.Click += new System.EventHandler(this.Button_Click);
            // 
            // lblCurrent
            // 
            this.lblCurrent.AutoSize = false;
            this.lblCurrent.Location = new System.Drawing.Point(247, 156);
            this.lblCurrent.Name = "lblCurrent";
            this.lblCurrent.Size = new System.Drawing.Size(213, 45);
            this.lblCurrent.Text = "Current";
            this.lblCurrent.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // txtResult
            // 
            this.txtResult.BackColor = System.Drawing.SystemColors.Window;
            this.txtResult.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtResult.Enabled = false;
            this.txtResult.Location = new System.Drawing.Point(20, 70);
            this.txtResult.Name = "txtResult";
            this.txtResult.Size = new System.Drawing.Size(440, 75);
            this.txtResult.TabIndex = 0;
            this.txtResult.Text = "txtResult";
            this.txtResult.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // BarcodeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.touchPanel1);
            this.Name = "BarcodeControl";
            this.Size = new System.Drawing.Size(480, 680);
            this.touchPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnOpen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnRead)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCalibration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblCurrent)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
        private Resco.Controls.CommonControls.TouchTextBox txtResult;
        private Resco.Controls.CommonControls.TransparentLabel lblCurrent;
        private Resco.Controls.OutlookControls.ImageButton btnDown;
        private Resco.Controls.OutlookControls.ImageButton btnOpen;
        private Resco.Controls.OutlookControls.ImageButton btnUp;
        private Resco.Controls.OutlookControls.ImageButton btnRead;
        private Resco.Controls.CommonControls.TouchTextBox txtCurrent;
        private Resco.Controls.OutlookControls.ImageButton btnCalibration;
    }
}
