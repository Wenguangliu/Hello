namespace j2AMSGUI
{
    partial class AMSGUIMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AMSGUIMain));
            this.lblTopTitle = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblMessage = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnOpen = new Resco.Controls.OutlookControls.ImageButton();
            this.btnClose = new Resco.Controls.OutlookControls.ImageButton();
            this.btnHeaterTest = new Resco.Controls.OutlookControls.ImageButton();
            this.btnDrawerTest = new Resco.Controls.OutlookControls.ImageButton();
            this.btnCancel = new Resco.Controls.OutlookControls.ImageButton();
            this.lblErrorCode = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnCalibration = new Resco.Controls.OutlookControls.ImageButton();
            this.btnExit = new Resco.Controls.OutlookControls.ImageButton();
            this.transparentLabel1 = new Resco.Controls.CommonControls.TransparentLabel();
            this.transparentLabel2 = new Resco.Controls.CommonControls.TransparentLabel();
            ((System.ComponentModel.ISupportInitialize)(this.btnOpen)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnClose)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHeaterTest)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDrawerTest)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCancel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCalibration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnExit)).BeginInit();
            this.SuspendLayout();
            // 
            // lblTopTitle
            // 
            this.lblTopTitle.AutoSize = false;
            this.lblTopTitle.Location = new System.Drawing.Point(0, 0);
            this.lblTopTitle.Name = "lblTopTitle";
            this.lblTopTitle.Size = new System.Drawing.Size(480, 45);
            this.lblTopTitle.Text = "transparentLabel1";
            //TODO this.lblTopTitle.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.lblTopTitle.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = false;
            this.lblMessage.BackColor = System.Drawing.Color.White;
            this.lblMessage.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.lblMessage.ForeColor = System.Drawing.Color.Black;
            this.lblMessage.Location = new System.Drawing.Point(15, 107);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(450, 45);
            this.lblMessage.Text = "Rotor Chamber is empty";
            //TODO this.lblMessage.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.lblMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter; //TODO System.Drawing.ContentAlignment.TopCenter;
            // 
            // btnOpen
            // 
            this.btnOpen.BorderColor = System.Drawing.Color.Transparent;
            this.btnOpen.BorderCornerSize = 0;
            this.btnOpen.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnOpen.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnOpen.ImageDefault")));
            this.btnOpen.Location = new System.Drawing.Point(252, 200);
            this.btnOpen.Name = "btnOpen";
            this.btnOpen.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnOpen.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnOpen.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnOpen.Size = new System.Drawing.Size(213, 75);
            this.btnOpen.TabIndex = 5;
            this.btnOpen.Tag = "0";
            this.btnOpen.Text = "Open";
            this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
            // 
            // btnClose
            // 
            this.btnClose.BorderColor = System.Drawing.Color.Transparent;
            this.btnClose.BorderCornerSize = 0;
            this.btnClose.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnClose.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnClose.ImageDefault")));
            this.btnClose.Location = new System.Drawing.Point(252, 323);
            this.btnClose.Name = "btnClose";
            this.btnClose.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnClose.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnClose.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnClose.Size = new System.Drawing.Size(213, 75);
            this.btnClose.TabIndex = 6;
            this.btnClose.Tag = "1";
            this.btnClose.Text = "Close";
            this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
            // 
            // btnHeaterTest
            // 
            this.btnHeaterTest.BorderColor = System.Drawing.Color.Transparent;
            this.btnHeaterTest.BorderCornerSize = 0;
            this.btnHeaterTest.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnHeaterTest.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnHeaterTest.ImageDefault")));
            this.btnHeaterTest.Location = new System.Drawing.Point(15, 323);
            this.btnHeaterTest.Name = "btnHeaterTest";
            this.btnHeaterTest.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnHeaterTest.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnHeaterTest.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnHeaterTest.Size = new System.Drawing.Size(213, 75);
            this.btnHeaterTest.TabIndex = 7;
            this.btnHeaterTest.Text = "Heater Test";
            this.btnHeaterTest.Click += new System.EventHandler(this.btnHeaterTest_Click);
            // 
            // btnDrawerTest
            // 
            this.btnDrawerTest.BorderColor = System.Drawing.Color.Transparent;
            this.btnDrawerTest.BorderCornerSize = 0;
            this.btnDrawerTest.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnDrawerTest.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnDrawerTest.ImageDefault")));
            this.btnDrawerTest.Location = new System.Drawing.Point(15, 446);
            this.btnDrawerTest.Name = "btnDrawerTest";
            this.btnDrawerTest.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnDrawerTest.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnDrawerTest.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnDrawerTest.Size = new System.Drawing.Size(213, 75);
            this.btnDrawerTest.TabIndex = 8;
            this.btnDrawerTest.Text = "Drawer Test";
            this.btnDrawerTest.Click += new System.EventHandler(this.btnDrawerTest_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.BorderColor = System.Drawing.Color.Transparent;
            this.btnCancel.BorderCornerSize = 0;
            this.btnCancel.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnCancel.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnCancel.ImageDefault")));
            this.btnCancel.Location = new System.Drawing.Point(252, 446);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnCancel.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnCancel.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnCancel.Size = new System.Drawing.Size(213, 75);
            this.btnCancel.TabIndex = 9;
            this.btnCancel.Tag = "2";
            this.btnCancel.Text = "Cancel";
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // lblErrorCode
            // 
            this.lblErrorCode.AutoSize = false;
            this.lblErrorCode.BackColor = System.Drawing.Color.White;
            this.lblErrorCode.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.lblErrorCode.ForeColor = System.Drawing.Color.Black;
            this.lblErrorCode.Location = new System.Drawing.Point(15, 621);
            this.lblErrorCode.Name = "lblErrorCode";
            this.lblErrorCode.Size = new System.Drawing.Size(450, 45);
            this.lblErrorCode.Text = "Error Code";
            //TODO this.lblErrorCode.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.lblErrorCode.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // btnCalibration
            // 
            this.btnCalibration.BorderColor = System.Drawing.Color.Transparent;
            this.btnCalibration.BorderCornerSize = 0;
            this.btnCalibration.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnCalibration.ImageDefault = ((System.Drawing.Image)(resources.GetObject("btnCalibration.ImageDefault")));
            this.btnCalibration.Location = new System.Drawing.Point(15, 200);
            this.btnCalibration.Name = "btnCalibration";
            this.btnCalibration.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnCalibration.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnCalibration.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnCalibration.Size = new System.Drawing.Size(213, 75);
            this.btnCalibration.TabIndex = 12;
            this.btnCalibration.Text = "Calibration";
            this.btnCalibration.Click += new System.EventHandler(this.btnCalibration_Click);
            // 
            // btnExit
            // 
            this.btnExit.Location = new System.Drawing.Point(0, 725);
            this.btnExit.Name = "btnExit";
            this.btnExit.Size = new System.Drawing.Size(480, 75);
            this.btnExit.TabIndex = 0;
            this.btnExit.Text = "Exit";
            this.btnExit.Click += new System.EventHandler(this.btnExit_Click);
            // 
            // transparentLabel1
            // 
            this.transparentLabel1.AutoSize = false;
            this.transparentLabel1.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.transparentLabel1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.transparentLabel1.Location = new System.Drawing.Point(15, 569);
            this.transparentLabel1.Name = "transparentLabel1";
            this.transparentLabel1.Size = new System.Drawing.Size(450, 45);
            this.transparentLabel1.Text = "Error Code";
            //this.transparentLabel1.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.transparentLabel1.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // transparentLabel2
            // 
            this.transparentLabel2.AutoSize = false;
            this.transparentLabel2.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.transparentLabel2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.transparentLabel2.Location = new System.Drawing.Point(15, 51);
            this.transparentLabel2.Name = "transparentLabel2";
            this.transparentLabel2.Size = new System.Drawing.Size(450, 45);
            this.transparentLabel2.Text = "AMS Process Status";
            //TODO this.transparentLabel2.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.transparentLabel2.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // AMSGUIMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.transparentLabel2);
            this.Controls.Add(this.transparentLabel1);
            this.Controls.Add(this.btnExit);
            this.Controls.Add(this.btnCalibration);
            this.Controls.Add(this.lblErrorCode);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnDrawerTest);
            this.Controls.Add(this.btnHeaterTest);
            this.Controls.Add(this.btnClose);
            this.Controls.Add(this.btnOpen);
            this.Controls.Add(this.lblMessage);
            this.Controls.Add(this.lblTopTitle);
            this.Name = "AMSGUIMain";
            this.Size = new System.Drawing.Size(480, 800);
            ((System.ComponentModel.ISupportInitialize)(this.btnOpen)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnClose)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnHeaterTest)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDrawerTest)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCancel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnCalibration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnExit)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TransparentLabel lblTopTitle;
        private Resco.Controls.CommonControls.TransparentLabel lblMessage;
        private Resco.Controls.OutlookControls.ImageButton btnOpen;
        private Resco.Controls.OutlookControls.ImageButton btnClose;
        private Resco.Controls.OutlookControls.ImageButton btnHeaterTest;
        private Resco.Controls.OutlookControls.ImageButton btnDrawerTest;
        private Resco.Controls.OutlookControls.ImageButton btnCancel;
        private Resco.Controls.CommonControls.TransparentLabel lblErrorCode;
        private Resco.Controls.OutlookControls.ImageButton btnCalibration;
        private Resco.Controls.OutlookControls.ImageButton btnExit;
        private Resco.Controls.CommonControls.TransparentLabel transparentLabel1;
        private Resco.Controls.CommonControls.TransparentLabel transparentLabel2;

    }
}
