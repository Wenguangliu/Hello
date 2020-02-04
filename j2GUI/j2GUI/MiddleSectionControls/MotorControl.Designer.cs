namespace j2GUI.j2MiddleSectionControls
{
    partial class MotorControl
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
            this.txtCurrentSpeed = new Resco.Controls.CommonControls.TouchTextBox();
            this.lblDirection = new Resco.Controls.CommonControls.TransparentLabel();
            this.lblSpeed = new Resco.Controls.CommonControls.TransparentLabel();
            this.txtDirection = new Resco.Controls.CommonControls.TouchTextBox();
            this.txtSetSpeed = new Resco.Controls.CommonControls.TouchTextBox();
            this.btnDirectionUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnSetSpeedUp = new Resco.Controls.OutlookControls.ImageButton();
            this.btnDirectionDown = new Resco.Controls.OutlookControls.ImageButton();
            this.btnSetSpeedDown = new Resco.Controls.OutlookControls.ImageButton();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.lblDirection)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDirectionUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnSetSpeedUp)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDirectionDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnSetSpeedDown)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txtCurrentSpeed
            // 
            this.txtCurrentSpeed.BackColor = System.Drawing.SystemColors.Window;
            this.txtCurrentSpeed.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtCurrentSpeed.Enabled = false;
            this.txtCurrentSpeed.Location = new System.Drawing.Point(20, 70);
            this.txtCurrentSpeed.Name = "txtCurrentSpeed";
            this.txtCurrentSpeed.Size = new System.Drawing.Size(440, 75);
            this.txtCurrentSpeed.TabIndex = 0;
            this.txtCurrentSpeed.Text = "txtCurrentSpeed";
            this.txtCurrentSpeed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // lblDirection
            // 
            this.lblDirection.AutoSize = false;
            this.lblDirection.Location = new System.Drawing.Point(20, 156);
            this.lblDirection.Name = "lblDirection";
            this.lblDirection.Size = new System.Drawing.Size(213, 45);
            this.lblDirection.Text = "Direction";
            this.lblDirection.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // lblSpeed
            // 
            this.lblSpeed.AutoSize = false;
            this.lblSpeed.Location = new System.Drawing.Point(247, 156);
            this.lblSpeed.Name = "lblSpeed";
            this.lblSpeed.Size = new System.Drawing.Size(213, 45);
            this.lblSpeed.Text = "Speed";
            this.lblSpeed.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // txtDirection
            // 
            this.txtDirection.BackColor = System.Drawing.SystemColors.Window;
            this.txtDirection.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtDirection.Enabled = false;
            this.txtDirection.Location = new System.Drawing.Point(20, 205);
            this.txtDirection.Name = "txtDirection";
            this.txtDirection.Size = new System.Drawing.Size(213, 75);
            this.txtDirection.TabIndex = 3;
            this.txtDirection.Text = "txtDirection";
            this.txtDirection.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // txtSetSpeed
            // 
            this.txtSetSpeed.BackColor = System.Drawing.SystemColors.Window;
            this.txtSetSpeed.DisabledForeColor = System.Drawing.SystemColors.ControlText;
            this.txtSetSpeed.Enabled = false;
            this.txtSetSpeed.Location = new System.Drawing.Point(247, 205);
            this.txtSetSpeed.Name = "txtSetSpeed";
            this.txtSetSpeed.Size = new System.Drawing.Size(213, 75);
            this.txtSetSpeed.TabIndex = 4;
            this.txtSetSpeed.Text = "txtSetSpeed";
            this.txtSetSpeed.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // btnDirectionUp
            // 
            this.btnDirectionUp.Location = new System.Drawing.Point(20, 300);
            this.btnDirectionUp.Name = "btnDirectionUp";
            this.btnDirectionUp.Size = new System.Drawing.Size(213, 75);
            this.btnDirectionUp.TabIndex = 5;
            this.btnDirectionUp.Tag = "0";
            this.btnDirectionUp.Text = "Up";
            this.btnDirectionUp.Click += new System.EventHandler(this.UpButton_Click);
            // 
            // btnSetSpeedUp
            // 
            this.btnSetSpeedUp.Location = new System.Drawing.Point(247, 300);
            this.btnSetSpeedUp.Name = "btnSetSpeedUp";
            this.btnSetSpeedUp.Size = new System.Drawing.Size(213, 75);
            this.btnSetSpeedUp.TabIndex = 6;
            this.btnSetSpeedUp.Tag = "1";
            this.btnSetSpeedUp.Text = "Up";
            this.btnSetSpeedUp.Click += new System.EventHandler(this.UpButton_Click);
            // 
            // btnDirectionDown
            // 
            this.btnDirectionDown.Location = new System.Drawing.Point(20, 390);
            this.btnDirectionDown.Name = "btnDirectionDown";
            this.btnDirectionDown.Size = new System.Drawing.Size(213, 75);
            this.btnDirectionDown.TabIndex = 7;
            this.btnDirectionDown.Tag = "0";
            this.btnDirectionDown.Text = "Down";
            this.btnDirectionDown.Click += new System.EventHandler(this.DownButton_Click);
            // 
            // btnSetSpeedDown
            // 
            this.btnSetSpeedDown.Location = new System.Drawing.Point(247, 390);
            this.btnSetSpeedDown.Name = "btnSetSpeedDown";
            this.btnSetSpeedDown.Size = new System.Drawing.Size(213, 75);
            this.btnSetSpeedDown.TabIndex = 8;
            this.btnSetSpeedDown.Tag = "1";
            this.btnSetSpeedDown.Text = "Down";
            this.btnSetSpeedDown.Click += new System.EventHandler(this.DownButton_Click);
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.txtCurrentSpeed);
            this.touchPanel1.Controls.Add(this.btnSetSpeedDown);
            this.touchPanel1.Controls.Add(this.lblDirection);
            this.touchPanel1.Controls.Add(this.btnSetSpeedUp);
            this.touchPanel1.Controls.Add(this.btnDirectionDown);
            this.touchPanel1.Controls.Add(this.txtSetSpeed);
            this.touchPanel1.Controls.Add(this.txtDirection);
            this.touchPanel1.Controls.Add(this.lblSpeed);
            this.touchPanel1.Controls.Add(this.btnDirectionUp);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // MotorControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this.touchPanel1);
            this.Name = "MotorControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.lblDirection)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDirectionUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnSetSpeedUp)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnDirectionDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnSetSpeedDown)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TouchTextBox txtCurrentSpeed;
        private Resco.Controls.CommonControls.TransparentLabel lblDirection;
        private Resco.Controls.CommonControls.TransparentLabel lblSpeed;
        private Resco.Controls.CommonControls.TouchTextBox txtDirection;
        private Resco.Controls.CommonControls.TouchTextBox txtSetSpeed;
        private Resco.Controls.OutlookControls.ImageButton btnDirectionUp;
        private Resco.Controls.OutlookControls.ImageButton btnSetSpeedUp;
        private Resco.Controls.OutlookControls.ImageButton btnDirectionDown;
        private Resco.Controls.OutlookControls.ImageButton btnSetSpeedDown;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
