namespace j2GUI
{
    partial class WinMessageControls
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
            this.panelBottom = new System.Windows.Forms.Panel();
            this.lblTopTitle = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnInfo = new Resco.Controls.OutlookControls.ImageButton();
            this.lblMessage = new Resco.Controls.CommonControls.TransparentLabel();
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).BeginInit();
            this.SuspendLayout();
            // 
            // panelBottom
            // 
            this.panelBottom.Location = new System.Drawing.Point(0, 725);
            this.panelBottom.Name = "panelBottom";
            this.panelBottom.Size = new System.Drawing.Size(480, 75);
            this.panelBottom.Visible = false;
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
            // btnInfo
            // 
            this.btnInfo.BorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.BorderCornerSize = 0;
            this.btnInfo.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.Location = new System.Drawing.Point(175, 559);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnInfo.Size = new System.Drawing.Size(130, 130);
            this.btnInfo.TabIndex = 2;
            this.btnInfo.Text = "imageButton1";
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = false;
            this.lblMessage.Font = new System.Drawing.Font("Arial", 26F, System.Drawing.FontStyle.Regular);
            this.lblMessage.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.lblMessage.Location = new System.Drawing.Point(15, 155);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(450, 300);
            this.lblMessage.Text = "transparentLabel1";
            this.lblMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // WinMessageControls
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.lblMessage);
            this.Controls.Add(this.btnInfo);
            this.Controls.Add(this.lblTopTitle);
            this.Controls.Add(this.panelBottom);
            this.Name = "WinMessageControls";
            this.Size = new System.Drawing.Size(480, 800);
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelBottom;
        private Resco.Controls.CommonControls.TransparentLabel lblTopTitle;
        private Resco.Controls.OutlookControls.ImageButton btnInfo;
        private Resco.Controls.CommonControls.TransparentLabel lblMessage;

    }
}
