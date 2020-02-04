namespace j2GUI.j2MiddleSectionControls
{
    partial class MessageInfoControl
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
            this.lblMessage = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnInfo = new Resco.Controls.OutlookControls.ImageButton();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = false;
            this.lblMessage.Font = new System.Drawing.Font("Arial", 30F, System.Drawing.FontStyle.Regular);
            this.lblMessage.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.lblMessage.Location = new System.Drawing.Point(15, 110);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(450, 300);
            this.lblMessage.Text = "transparentLabel1";
            this.lblMessage.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // btnInfo
            // 
            this.btnInfo.BorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.BorderCornerSize = 0;
            this.btnInfo.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.Location = new System.Drawing.Point(175, 514);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnInfo.Size = new System.Drawing.Size(130, 130);
            this.btnInfo.TabIndex = 3;
            this.btnInfo.Text = "imageButton1";
            this.btnInfo.Visible = false;
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.lblMessage);
            this.touchPanel1.Controls.Add(this.btnInfo);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // MessageInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "MessageInfoControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TransparentLabel lblMessage;
        private Resco.Controls.OutlookControls.ImageButton btnInfo;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
