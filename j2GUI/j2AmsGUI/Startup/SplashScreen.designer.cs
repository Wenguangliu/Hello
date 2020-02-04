namespace j2AMSGUI.j2Startup
{
    partial class SplashScreen
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
            this.pictureBoxSplash = new System.Windows.Forms.PictureBox();
            this.timerStartup = new System.Windows.Forms.Timer();
            this.lblTopTitle = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblMessage = new Resco.Controls.CommonControls.TransparentLabel();
            this.progressBar1 = new Resco.Controls.ProgressBar.ProgressBar();
            this.SuspendLayout();
            // 
            // pictureBoxSplash
            // 
            this.pictureBoxSplash.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.pictureBoxSplash.Location = new System.Drawing.Point(0, 47);
            this.pictureBoxSplash.Name = "pictureBoxSplash";
            this.pictureBoxSplash.Size = new System.Drawing.Size(480, 626);
            this.pictureBoxSplash.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            // 
            // timerStartup
            // 
            this.timerStartup.Tick += new System.EventHandler(this.timerStartup_Tick);
            // 
            // lblTopTitle
            // 
            this.lblTopTitle.AutoSize = false;
            this.lblTopTitle.Location = new System.Drawing.Point(0, 0);
            this.lblTopTitle.Name = "lblTopTitle";
            this.lblTopTitle.Size = new System.Drawing.Size(480, 45);
            this.lblTopTitle.Text = "transparentLabel1";
            this.lblTopTitle.TextAlignment  = Resco.Controls.CommonControls.Alignment.TopCenter; //TODO = System.Drawing.ContentAlignment.TopCenter;
            // 
            // lblMessage
            // 
            this.lblMessage.AutoSize = false;
            this.lblMessage.Font = new System.Drawing.Font("Arial", 20F, System.Drawing.FontStyle.Regular);
            this.lblMessage.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.lblMessage.Location = new System.Drawing.Point(0, 675);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(480, 40);
            this.lblMessage.Text = "transparentLabel1";
            //TODO this.lblMessage.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.lblMessage.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(0, 722);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.ShowText = true;
            this.progressBar1.Size = new System.Drawing.Size(480, 75);
            this.progressBar1.TabIndex = 9;
            this.progressBar1.TextOutputFormat = "";
            // 
            // SplashScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.lblMessage);
            this.Controls.Add(this.lblTopTitle);
            this.Controls.Add(this.pictureBoxSplash);
            this.Name = "SplashScreen";
            this.Size = new System.Drawing.Size(480, 800);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxSplash;
        private System.Windows.Forms.Timer timerStartup;
        private Resco.Controls.CommonControls.TransparentLabel lblTopTitle;
        private Resco.Controls.CommonControls.TransparentLabel lblMessage;
        private Resco.Controls.ProgressBar.ProgressBar progressBar1;
    }
}
