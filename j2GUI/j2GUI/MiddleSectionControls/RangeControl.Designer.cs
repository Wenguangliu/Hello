namespace j2GUI.j2MiddleSectionControls
{
    partial class RangeControl
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
            this.txtUpper = new Resco.Controls.CommonControls.TouchTextBox();
            this.lblUpper = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblLower = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnUpperDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnUpperUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnLowerDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnLowerUp = new Resco.Controls.OutlookControls.ImageButton();
            this.imageButton1 = new Resco.Controls.OutlookControls.ImageButton();
            this.imageButton0 = new Resco.Controls.OutlookControls.ImageButton();
            this.txtLower = new Resco.Controls.CommonControls.TouchTextBox();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.lblUpper)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblLower)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUpperDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUpperUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLowerDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLowerUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton0)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtUpper
            // 
            this.txtUpper.BackColor = System.Drawing.Color.White;
            this.txtUpper.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtUpper.Enabled = false;
            this.txtUpper.Location = new System.Drawing.Point(247, 117);
            this.txtUpper.Name = "txtUpper";
            this.txtUpper.ReadOnly = true;
            this.txtUpper.Size = new System.Drawing.Size(213, 75);
            this.txtUpper.TabIndex = 23;
            this.txtUpper.Text = "touchTextBox2";
            this.txtUpper.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblUpper
            // 
            this.lblUpper.AutoSize = false;
            this.lblUpper.Location = new System.Drawing.Point(247, 70);
            this.lblUpper.Name = "lblUpper";
            this.lblUpper.Size = new System.Drawing.Size(213, 45);
            this.lblUpper.Text = "Upper";
            this.lblUpper.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // lblLower
            // 
            this.lblLower.AutoSize = false;
            this.lblLower.Location = new System.Drawing.Point(20, 70);
            this.lblLower.Name = "lblLower";
            this.lblLower.Size = new System.Drawing.Size(213, 45);
            this.lblLower.Text = "Lower";
            this.lblLower.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // btnUpperDown
            // 
            this.btnUpperDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnUpperDown.BorderCornerSize = 0;
            this.btnUpperDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnUpperDown.Location = new System.Drawing.Point(247, 295);
            this.btnUpperDown.Name = "btnUpperDown";
            this.btnUpperDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnUpperDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnUpperDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnUpperDown.Size = new System.Drawing.Size(213, 75);
            this.btnUpperDown.TabIndex = 21;
            this.btnUpperDown.Tag = "1";
            this.btnUpperDown.Text = "Down";
            this.btnUpperDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseDown);
            this.btnUpperDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseUp);
            // 
            // btnUpperUp
            // 
            this.btnUpperUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnUpperUp.BorderCornerSize = 0;
            this.btnUpperUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnUpperUp.Location = new System.Drawing.Point(247, 205);
            this.btnUpperUp.Name = "btnUpperUp";
            this.btnUpperUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnUpperUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnUpperUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnUpperUp.Size = new System.Drawing.Size(213, 75);
            this.btnUpperUp.TabIndex = 19;
            this.btnUpperUp.Tag = "1";
            this.btnUpperUp.Text = "Up";
            this.btnUpperUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseDown);
            this.btnUpperUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseUp);
            // 
            // btnLowerDown
            // 
            this.btnLowerDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnLowerDown.BorderCornerSize = 0;
            this.btnLowerDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnLowerDown.Location = new System.Drawing.Point(20, 295);
            this.btnLowerDown.Name = "btnLowerDown";
            this.btnLowerDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnLowerDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnLowerDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnLowerDown.Size = new System.Drawing.Size(213, 75);
            this.btnLowerDown.TabIndex = 20;
            this.btnLowerDown.Tag = "0";
            this.btnLowerDown.Text = "Down";
            this.btnLowerDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseDown);
            this.btnLowerDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseUp);
            // 
            // btnLowerUp
            // 
            this.btnLowerUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnLowerUp.BorderCornerSize = 0;
            this.btnLowerUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnLowerUp.Location = new System.Drawing.Point(20, 205);
            this.btnLowerUp.Name = "btnLowerUp";
            this.btnLowerUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnLowerUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnLowerUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnLowerUp.Size = new System.Drawing.Size(213, 75);
            this.btnLowerUp.TabIndex = 18;
            this.btnLowerUp.Tag = "0";
            this.btnLowerUp.Text = "Up";
            this.btnLowerUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseDown);
            this.btnLowerUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseUp);
            // 
            // imageButton1
            // 
            this.imageButton1.BorderColor = System.Drawing.Color.Transparent;
            this.imageButton1.BorderCornerSize = 0;
            this.imageButton1.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.imageButton1.Location = new System.Drawing.Point(247, 425);
            this.imageButton1.Name = "imageButton1";
            this.imageButton1.PressedBackColor = System.Drawing.Color.Transparent;
            this.imageButton1.PressedBorderColor = System.Drawing.Color.Transparent;
            this.imageButton1.PressedForeColor = System.Drawing.Color.Transparent;
            this.imageButton1.Size = new System.Drawing.Size(213, 75);
            this.imageButton1.TabIndex = 27;
            this.imageButton1.Tag = "1";
            this.imageButton1.Text = "imageButton1";
            this.imageButton1.Click += new System.EventHandler(this.imageButton_Click);
            // 
            // imageButton0
            // 
            this.imageButton0.BorderColor = System.Drawing.Color.Transparent;
            this.imageButton0.BorderCornerSize = 0;
            this.imageButton0.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.imageButton0.Location = new System.Drawing.Point(20, 425);
            this.imageButton0.Name = "imageButton0";
            this.imageButton0.PressedBackColor = System.Drawing.Color.Transparent;
            this.imageButton0.PressedBorderColor = System.Drawing.Color.Transparent;
            this.imageButton0.PressedForeColor = System.Drawing.Color.Transparent;
            this.imageButton0.Size = new System.Drawing.Size(213, 75);
            this.imageButton0.TabIndex = 26;
            this.imageButton0.Tag = "0";
            this.imageButton0.Text = "imageButton0";
            this.imageButton0.Click += new System.EventHandler(this.imageButton_Click);
            // 
            // txtLower
            // 
            this.txtLower.BackColor = System.Drawing.Color.White;
            this.txtLower.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtLower.Enabled = false;
            this.txtLower.Location = new System.Drawing.Point(20, 117);
            this.txtLower.Name = "txtLower";
            this.txtLower.ReadOnly = true;
            this.txtLower.Size = new System.Drawing.Size(213, 75);
            this.txtLower.TabIndex = 28;
            this.txtLower.Text = "touchTextBox2";
            this.txtLower.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.txtUpper);
            this.touchPanel1.Controls.Add(this.txtLower);
            this.touchPanel1.Controls.Add(this.btnLowerUp);
            this.touchPanel1.Controls.Add(this.imageButton1);
            this.touchPanel1.Controls.Add(this.btnLowerDown);
            this.touchPanel1.Controls.Add(this.imageButton0);
            this.touchPanel1.Controls.Add(this.btnUpperUp);
            this.touchPanel1.Controls.Add(this.btnUpperDown);
            this.touchPanel1.Controls.Add(this.lblUpper);
            this.touchPanel1.Controls.Add(this.lblLower);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // RangeControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "RangeControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.lblUpper)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblLower)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUpperDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUpperUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLowerDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLowerUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageButton0)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TouchTextBox txtUpper;
        private Resco.Controls.CommonControls.TransparentLabel lblUpper;
        private Resco.Controls.CommonControls.TransparentLabel lblLower;
        private Resco.Controls.OutlookControls.ImageButton btnUpperDown;
        private Resco.Controls.OutlookControls.ImageButton btnUpperUp;
        private Resco.Controls.OutlookControls.ImageButton btnLowerDown;
        private Resco.Controls.OutlookControls.ImageButton btnLowerUp;
        private Resco.Controls.OutlookControls.ImageButton imageButton1;
        private Resco.Controls.OutlookControls.ImageButton imageButton0;
        private Resco.Controls.CommonControls.TouchTextBox txtLower;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
