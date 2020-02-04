namespace j2GUI.j2MiddleSectionControls
{
    partial class SelectInfoControl
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
            this.touchCheckBox1 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.touchCheckBox2 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.touchCheckBox3 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.touchCheckBox4 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.touchCheckBox5 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.touchCheckBox6 = new Resco.Controls.CommonControls.TouchCheckBox();
            this.btnInfo = new Resco.Controls.OutlookControls.ImageButton();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // touchCheckBox1
            // 
            this.touchCheckBox1.AutoTransparent = true;
            this.touchCheckBox1.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox1.Location = new System.Drawing.Point(19, 12);
            this.touchCheckBox1.Name = "touchCheckBox1";
            this.touchCheckBox1.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox1.TabIndex = 0;
            this.touchCheckBox1.Tag = "0";
            this.touchCheckBox1.Text = "touchCheckBox1";
            this.touchCheckBox1.Visible = false;
            this.touchCheckBox1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // touchCheckBox2
            // 
            this.touchCheckBox2.AutoTransparent = true;
            this.touchCheckBox2.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox2.Location = new System.Drawing.Point(19, 102);
            this.touchCheckBox2.Name = "touchCheckBox2";
            this.touchCheckBox2.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox2.TabIndex = 1;
            this.touchCheckBox2.TabStop = false;
            this.touchCheckBox2.Tag = "1";
            this.touchCheckBox2.Text = "touchCheckBox2";
            this.touchCheckBox2.Visible = false;
            this.touchCheckBox2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // touchCheckBox3
            // 
            this.touchCheckBox3.AutoTransparent = true;
            this.touchCheckBox3.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox3.Location = new System.Drawing.Point(19, 192);
            this.touchCheckBox3.Name = "touchCheckBox3";
            this.touchCheckBox3.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox3.TabIndex = 2;
            this.touchCheckBox3.TabStop = false;
            this.touchCheckBox3.Tag = "2";
            this.touchCheckBox3.Text = "touchCheckBox3";
            this.touchCheckBox3.Visible = false;
            this.touchCheckBox3.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // touchCheckBox4
            // 
            this.touchCheckBox4.AutoTransparent = true;
            this.touchCheckBox4.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox4.Location = new System.Drawing.Point(19, 282);
            this.touchCheckBox4.Name = "touchCheckBox4";
            this.touchCheckBox4.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox4.TabIndex = 3;
            this.touchCheckBox4.TabStop = false;
            this.touchCheckBox4.Tag = "3";
            this.touchCheckBox4.Text = "touchCheckBox4";
            this.touchCheckBox4.Visible = false;
            this.touchCheckBox4.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // touchCheckBox5
            // 
            this.touchCheckBox5.AutoTransparent = true;
            this.touchCheckBox5.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox5.Location = new System.Drawing.Point(19, 372);
            this.touchCheckBox5.Name = "touchCheckBox5";
            this.touchCheckBox5.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox5.TabIndex = 4;
            this.touchCheckBox5.TabStop = false;
            this.touchCheckBox5.Tag = "4";
            this.touchCheckBox5.Text = "touchCheckBox5";
            this.touchCheckBox5.Visible = false;
            this.touchCheckBox5.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // touchCheckBox6
            // 
            this.touchCheckBox6.AutoTransparent = true;
            this.touchCheckBox6.CheckBoxSize = new System.Drawing.Size(75, 75);
            this.touchCheckBox6.Location = new System.Drawing.Point(19, 462);
            this.touchCheckBox6.Name = "touchCheckBox6";
            this.touchCheckBox6.Size = new System.Drawing.Size(442, 90);
            this.touchCheckBox6.TabIndex = 5;
            this.touchCheckBox6.TabStop = false;
            this.touchCheckBox6.Tag = "5";
            this.touchCheckBox6.Text = "touchCheckBox6";
            this.touchCheckBox6.Visible = false;
            this.touchCheckBox6.MouseUp += new System.Windows.Forms.MouseEventHandler(this.touchCheckBox_MouseUp);
            // 
            // btnInfo
            // 
            this.btnInfo.BorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.BorderCornerSize = 0;
            this.btnInfo.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.Location = new System.Drawing.Point(183, 560);
            this.btnInfo.Name = "btnInfo";
            this.btnInfo.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnInfo.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnInfo.Size = new System.Drawing.Size(106, 106);
            this.btnInfo.TabIndex = 6;
            this.btnInfo.Text = "InfoImage";
            this.btnInfo.Visible = false;
            this.btnInfo.Click += new System.EventHandler(this.btnInfo_Click);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.touchCheckBox5);
            this.touchPanel1.Controls.Add(this.btnInfo);
            this.touchPanel1.Controls.Add(this.touchCheckBox1);
            this.touchPanel1.Controls.Add(this.touchCheckBox6);
            this.touchPanel1.Controls.Add(this.touchCheckBox2);
            this.touchPanel1.Controls.Add(this.touchCheckBox3);
            this.touchPanel1.Controls.Add(this.touchCheckBox4);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // SelectInfoControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "SelectInfoControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnInfo)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox1;
        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox2;
        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox3;
        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox4;
        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox5;
        private Resco.Controls.CommonControls.TouchCheckBox touchCheckBox6;
        private Resco.Controls.OutlookControls.ImageButton btnInfo;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
