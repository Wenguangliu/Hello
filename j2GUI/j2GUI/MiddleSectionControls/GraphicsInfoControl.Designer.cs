namespace j2GUI.j2MiddleSectionControls
{
    partial class GraphicsInfoControl
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
            this.lblInformation = new Resco.Controls.CommonControls.TransparentLabel();
            this.pictureBoxGraphics = new System.Windows.Forms.PictureBox();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblInformation
            // 
            this.lblInformation.AutoSize = false;
            this.lblInformation.Location = new System.Drawing.Point(31, 364);
            this.lblInformation.Name = "lblInformation";
            this.lblInformation.Size = new System.Drawing.Size(420, 250);
            this.lblInformation.Text = "Information";
            this.lblInformation.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // pictureBoxGraphics
            // 
            this.pictureBoxGraphics.Location = new System.Drawing.Point(15, 19);
            this.pictureBoxGraphics.Name = "pictureBoxGraphics";
            this.pictureBoxGraphics.Size = new System.Drawing.Size(450, 328);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.lblInformation);
            this.touchPanel1.Controls.Add(this.pictureBoxGraphics);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // GraphicsInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "GraphicsInfoControl";
            this.Size = new System.Drawing.Size(480, 680);
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TransparentLabel lblInformation;
        private System.Windows.Forms.PictureBox pictureBoxGraphics;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
