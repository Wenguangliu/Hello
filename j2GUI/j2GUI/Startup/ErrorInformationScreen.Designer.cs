namespace j2GUI.j2Startup
{
    partial class ErrorInformationScreen
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
            this.txtErrorInfo = new System.Windows.Forms.TextBox();
            this.btnDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnUp = new Resco.Controls.OutlookControls.ImageButton();
            this.lblTopTitle = new Resco.Controls.CommonControls.TransparentLabel();
            this.imageButton1 = new Resco.Controls.OutlookControls.ImageButton();
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton1)).BeginInit();
            this.SuspendLayout();
            // 
            // txtErrorInfo
            // 
            this.txtErrorInfo.BackColor = System.Drawing.Color.White;
            this.txtErrorInfo.Location = new System.Drawing.Point(19, 64);
            this.txtErrorInfo.Multiline = true;
            this.txtErrorInfo.Name = "txtErrorInfo";
            this.txtErrorInfo.ReadOnly = true;
            this.txtErrorInfo.Size = new System.Drawing.Size(442, 525);
            this.txtErrorInfo.TabIndex = 11;
            // 
            // btnDown
            // 
            this.btnDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnDown.BorderCornerSize = 0;
            this.btnDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.Location = new System.Drawing.Point(248, 604);
            this.btnDown.Name = "btnDown";
            this.btnDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnDown.Size = new System.Drawing.Size(213, 75);
            this.btnDown.TabIndex = 13;
            this.btnDown.Text = "Down";
            // 
            // btnUp
            // 
            this.btnUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnUp.BorderCornerSize = 0;
            this.btnUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.Location = new System.Drawing.Point(19, 604);
            this.btnUp.Name = "btnUp";
            this.btnUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnUp.Size = new System.Drawing.Size(213, 75);
            this.btnUp.TabIndex = 12;
            this.btnUp.Text = "Up";
            // 
            // lblTopTitle
            // 
            this.lblTopTitle.AutoSize = false;
            this.lblTopTitle.Location = new System.Drawing.Point(0, 0);
            this.lblTopTitle.Name = "lblTopTitle";
            this.lblTopTitle.Size = new System.Drawing.Size(480, 45);
            this.lblTopTitle.Text = "transparentLabel1";
            this.lblTopTitle.TextAlignment = Resco.Controls.CommonControls.Alignment.MiddleCenter;
            // 
            // imageButton1
            // 
            this.imageButton1.BorderCornerSize = 0;
            this.imageButton1.BorderStyle = Resco.Controls.RescoBorderStyle.FixedSingle;
            this.imageButton1.ButtonStyle = Resco.Controls.OutlookControls.ImageButton.ButtonType.GradientImageButton;
            this.imageButton1.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.imageButton1.Location = new System.Drawing.Point(0, 725);
            this.imageButton1.Name = "imageButton1";
            this.imageButton1.Size = new System.Drawing.Size(480, 75);
            this.imageButton1.TabIndex = 16;
            this.imageButton1.Text = "Continue";
            this.imageButton1.VistaButtonInflate = new System.Drawing.Size(-1, -1);
            this.imageButton1.Click += new System.EventHandler(this.imageButton1_Click);
            // 
            // ErrorInformationScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.imageButton1);
            this.Controls.Add(this.lblTopTitle);
            this.Controls.Add(this.btnDown);
            this.Controls.Add(this.btnUp);
            this.Controls.Add(this.txtErrorInfo);
            this.Name = "ErrorInformationScreen";
            this.Size = new System.Drawing.Size(480, 800);
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TextBox txtErrorInfo;
        private Resco.Controls.OutlookControls.ImageButton btnDown;
        private Resco.Controls.OutlookControls.ImageButton btnUp;
        private Resco.Controls.CommonControls.TransparentLabel lblTopTitle;
        private Resco.Controls.OutlookControls.ImageButton imageButton1;
    }
}
