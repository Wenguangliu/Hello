namespace j2GUI.j2MiddleSectionControls
{
    partial class ProgessControl
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
            this.btnSettings = new Resco.Controls.OutlookControls.ImageButton();
            this.btnRecall = new Resco.Controls.OutlookControls.ImageButton();
            this.progressBar1 = new Resco.Controls.ProgressBar.ProgressBar();
            this.lblMessage = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblAnalysisMessage = new Resco.Controls.CommonControls.TransparentLabel();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            this.btnFactory = new Resco.Controls.OutlookControls.ImageButton();
            ((System.ComponentModel.ISupportInitialize)(this.btnSettings)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnRecall)).BeginInit();
            this.touchPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.btnFactory)).BeginInit();
            this.SuspendLayout();
            // 
            // btnSettings
            // 
            this.btnSettings.BorderColor = System.Drawing.Color.Transparent;
            this.btnSettings.BorderCornerSize = 0;
            this.btnSettings.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnSettings.Location = new System.Drawing.Point(326, 503);
            this.btnSettings.Name = "btnSettings";
            this.btnSettings.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnSettings.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnSettings.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnSettings.Size = new System.Drawing.Size(130, 130);
            this.btnSettings.TabIndex = 7;
            this.btnSettings.Text = "Settings";
            this.btnSettings.Visible = false;
            this.btnSettings.Click += new System.EventHandler(this.btnSettings_Click);
            // 
            // btnRecall
            // 
            this.btnRecall.BorderColor = System.Drawing.Color.Transparent;
            this.btnRecall.BorderCornerSize = 0;
            this.btnRecall.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnRecall.Location = new System.Drawing.Point(23, 503);
            this.btnRecall.Name = "btnRecall";
            this.btnRecall.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnRecall.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnRecall.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnRecall.Size = new System.Drawing.Size(130, 130);
            this.btnRecall.TabIndex = 6;
            this.btnRecall.Text = "Recall";
            this.btnRecall.Visible = false;
            this.btnRecall.Click += new System.EventHandler(this.btnRecall_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.BarGradientColor = new Resco.Controls.ProgressBar.GradientColor(System.Drawing.Color.Transparent, System.Drawing.Color.Transparent, System.Drawing.Color.Transparent, System.Drawing.Color.Transparent, 50, 50, Resco.Controls.ProgressBar.FillDirection.Vertical);
            this.progressBar1.BorderStyle = Resco.Controls.ProgressBar.ProgressBarBorderStyle.None;
            this.progressBar1.Location = new System.Drawing.Point(15, 19);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.ShowText = true;
            this.progressBar1.Size = new System.Drawing.Size(450, 75);
            this.progressBar1.TabIndex = 8;
            this.progressBar1.TextOutputFormat = "";
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = false;
            this.lblMessage.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.lblMessage.Location = new System.Drawing.Point(191, 100);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(73, 34);
            this.lblMessage.Text = "Hidden";
            this.lblMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            this.lblMessage.Visible = false;
            // 
            // lblAnalysisMessage
            // 
            this.lblAnalysisMessage.AutoSize = false;
            this.lblAnalysisMessage.Location = new System.Drawing.Point(15, 160);
            this.lblAnalysisMessage.Name = "lblAnalysisMessage";
            this.lblAnalysisMessage.Size = new System.Drawing.Size(450, 360);
            this.lblAnalysisMessage.Text = "Information";
            this.lblAnalysisMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.MiddleCenter;
            this.lblAnalysisMessage.Visible = false;
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.btnFactory);
            this.touchPanel1.Controls.Add(this.progressBar1);
            this.touchPanel1.Controls.Add(this.lblMessage);
            this.touchPanel1.Controls.Add(this.lblAnalysisMessage);
            this.touchPanel1.Controls.Add(this.btnSettings);
            this.touchPanel1.Controls.Add(this.btnRecall);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // btnFactory
            // 
            this.btnFactory.BorderColor = System.Drawing.Color.Transparent;
            this.btnFactory.BorderCornerSize = 0;
            this.btnFactory.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnFactory.Location = new System.Drawing.Point(174, 503);
            this.btnFactory.Name = "btnFactory";
            this.btnFactory.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnFactory.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnFactory.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnFactory.Size = new System.Drawing.Size(130, 130);
            this.btnFactory.TabIndex = 11;
            this.btnFactory.Text = "Factory";
            this.btnFactory.Visible = false;
            this.btnFactory.Click += new System.EventHandler(this.btnFactory_Click);
            // 
            // ProgessControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "ProgessControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnSettings)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnRecall)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.btnFactory)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.OutlookControls.ImageButton btnSettings;
        private Resco.Controls.OutlookControls.ImageButton btnRecall;
        private Resco.Controls.ProgressBar.ProgressBar progressBar1;
        private Resco.Controls.CommonControls.TransparentLabel lblMessage;
        private Resco.Controls.CommonControls.TransparentLabel lblAnalysisMessage;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
        private Resco.Controls.OutlookControls.ImageButton btnFactory;
    }
}
