namespace j2GUI.j2MiddleSectionControls
{
    partial class ViewControl
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
            this.btnDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnUp = new Resco.Controls.OutlookControls.ImageButton();
            this.txtViewResults = new System.Windows.Forms.TextBox();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnDown
            // 
            this.btnDown.BorderColor = System.Drawing.Color.Transparent;
            this.btnDown.BorderCornerSize = 0;
            this.btnDown.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.Location = new System.Drawing.Point(248, 559);
            this.btnDown.Name = "btnDown";
            this.btnDown.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnDown.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnDown.Size = new System.Drawing.Size(213, 75);
            this.btnDown.TabIndex = 9;
            this.btnDown.Text = "Down";
            this.btnDown.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseDown);
            this.btnDown.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnDown_MouseUp);
            // 
            // btnUp
            // 
            this.btnUp.BorderColor = System.Drawing.Color.Transparent;
            this.btnUp.BorderCornerSize = 0;
            this.btnUp.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.Location = new System.Drawing.Point(19, 559);
            this.btnUp.Name = "btnUp";
            this.btnUp.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnUp.PressedForeColor = System.Drawing.Color.Transparent;
            this.btnUp.Size = new System.Drawing.Size(213, 75);
            this.btnUp.TabIndex = 8;
            this.btnUp.Text = "Up";
            this.btnUp.MouseDown += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseDown);
            this.btnUp.MouseUp += new System.Windows.Forms.MouseEventHandler(this.btnUp_MouseUp);
            // 
            // txtViewResults
            // 
            this.txtViewResults.BackColor = System.Drawing.Color.White;
            this.txtViewResults.Enabled = false;
            this.txtViewResults.Location = new System.Drawing.Point(19, 19);
            this.txtViewResults.Multiline = true;
            this.txtViewResults.Name = "txtViewResults";
            this.txtViewResults.ReadOnly = true;
            this.txtViewResults.Size = new System.Drawing.Size(442, 525);
            this.txtViewResults.TabIndex = 10;
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.txtViewResults);
            this.touchPanel1.Controls.Add(this.btnUp);
            this.touchPanel1.Controls.Add(this.btnDown);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // ViewControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "ViewControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.btnDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnUp)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.OutlookControls.ImageButton btnDown;
        private Resco.Controls.OutlookControls.ImageButton btnUp;
        private System.Windows.Forms.TextBox txtViewResults;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
