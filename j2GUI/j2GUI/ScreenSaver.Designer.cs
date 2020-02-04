namespace j2GUI
{
    partial class ScreenSaver
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
            this.pictureBoxScreenSaver = new System.Windows.Forms.PictureBox();
            this.SuspendLayout();
            // 
            // pictureBoxScreenSaver
            // 
            this.pictureBoxScreenSaver.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxScreenSaver.Name = "pictureBoxScreenSaver";
            this.pictureBoxScreenSaver.Size = new System.Drawing.Size(480, 800);
            this.pictureBoxScreenSaver.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBoxScreenSaver.Click += new System.EventHandler(this.pictureBoxScreenSaver_Click);
            // 
            // ScreenSaver
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.Controls.Add(this.pictureBoxScreenSaver);
            this.Name = "ScreenSaver";
            this.Size = new System.Drawing.Size(480, 800);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxScreenSaver;
    }
}
