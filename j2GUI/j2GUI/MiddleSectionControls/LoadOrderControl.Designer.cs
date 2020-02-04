namespace j2GUI.j2MiddleSectionControls
{
    partial class LoadOrderControl
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
            this.lblRotorName = new Resco.Controls.CommonControls.TransparentLabel();
            this.btnLoadOrder = new Resco.Controls.OutlookControls.ImageButton();
            this.lblOrderLabel = new Resco.Controls.CommonControls.TransparentLabel();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            ((System.ComponentModel.ISupportInitialize)(this.lblRotorName)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLoadOrder)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblOrderLabel)).BeginInit();
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblRotorName
            // 
            this.lblRotorName.AutoSize = false;
            this.lblRotorName.Location = new System.Drawing.Point(15, 160);
            this.lblRotorName.Name = "lblRotorName";
            this.lblRotorName.Size = new System.Drawing.Size(450, 150);
            this.lblRotorName.Text = "Information";
            this.lblRotorName.TextAlignment = Resco.Controls.CommonControls.Alignment.MiddleCenter;
            this.lblRotorName.Visible = false;
            // 
            // btnLoadOrder
            // 
            this.btnLoadOrder.BorderColor = System.Drawing.Color.Transparent;
            this.btnLoadOrder.FocusedBorderColor = System.Drawing.Color.Transparent;
            this.btnLoadOrder.Location = new System.Drawing.Point(19, 585);
            this.btnLoadOrder.Name = "btnLoadOrder";
            this.btnLoadOrder.PressedBackColor = System.Drawing.Color.Transparent;
            this.btnLoadOrder.PressedBorderColor = System.Drawing.Color.Transparent;
            this.btnLoadOrder.Size = new System.Drawing.Size(442, 75);
            this.btnLoadOrder.TabIndex = 3;
            this.btnLoadOrder.Tag = "0";
            this.btnLoadOrder.Text = "button0";
            this.btnLoadOrder.Visible = false;
            this.btnLoadOrder.VistaButtonInflate = new System.Drawing.Size(0, 0);
            this.btnLoadOrder.Click += new System.EventHandler(this.btnLoadOrder_Click);
            // 
            // lblOrderLabel
            // 
            this.lblOrderLabel.AutoSize = false;
            this.lblOrderLabel.Font = new System.Drawing.Font("Arial", 30F, System.Drawing.FontStyle.Regular);
            this.lblOrderLabel.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(42)))), ((int)(((byte)(89)))), ((int)(((byte)(169)))));
            this.lblOrderLabel.Location = new System.Drawing.Point(19, 19);
            this.lblOrderLabel.Name = "lblOrderLabel";
            this.lblOrderLabel.Size = new System.Drawing.Size(442, 75);
            this.lblOrderLabel.Text = "transparentLabel1";
            this.lblOrderLabel.TextAlignment = Resco.Controls.CommonControls.Alignment.TopCenter;
            // 
            // touchPanel1
            // 
            this.touchPanel1.Controls.Add(this.lblOrderLabel);
            this.touchPanel1.Controls.Add(this.btnLoadOrder);
            this.touchPanel1.Controls.Add(this.lblRotorName);
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 680);
            // 
            // LoadOrderControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(215)))), ((int)(((byte)(226)))), ((int)(((byte)(245)))));
            this.Controls.Add(this.touchPanel1);
            this.Name = "LoadOrderControl";
            this.Size = new System.Drawing.Size(480, 680);
            ((System.ComponentModel.ISupportInitialize)(this.lblRotorName)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.btnLoadOrder)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.lblOrderLabel)).EndInit();
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TransparentLabel lblRotorName;
        private Resco.Controls.OutlookControls.ImageButton btnLoadOrder;
        private Resco.Controls.CommonControls.TransparentLabel lblOrderLabel;
        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
    }
}
