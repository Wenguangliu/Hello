namespace j2GUI.j2Startup
{
    partial class MainScreen
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.touchPanel1 = new Resco.Controls.CommonControls.TouchPanel();
            this.keyboardPro1 = new Resco.Controls.KeyboardPro.KeyboardPro(this, this.components);
            this.touchPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // touchPanel1
            // 
            this.touchPanel1.Location = new System.Drawing.Point(0, 0);
            this.touchPanel1.Name = "touchPanel1";
            this.touchPanel1.Size = new System.Drawing.Size(480, 800);
            // 
            // keyboardPro1
            // 
            this.keyboardPro1.Bounds = new System.Drawing.Rectangle(0, 220, 480, 370);
            this.keyboardPro1.GestureMode = Resco.Controls.KeyboardPro.GestureModeType.NoGestures;
            this.keyboardPro1.PanelPlacement = Resco.Controls.KeyboardPro.PanelPlacementType.Pos;
            this.keyboardPro1.PreviewMode = Resco.Controls.KeyboardPro.PreviewModeType.NoPreview;
            this.keyboardPro1.Skin = "AnalyzerSkin";
            // 
            // MainScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(96F, 96F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.Color.Black;
            this.ClientSize = new System.Drawing.Size(480, 800);
            this.ControlBox = false;
            this.Controls.Add(this.touchPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MainScreen";
            this.Text = "Abaxis Analyzer";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.MainScreen_Closing);
            this.touchPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Resco.Controls.CommonControls.TouchPanel touchPanel1;
        private Resco.Controls.KeyboardPro.KeyboardPro keyboardPro1;

    }
}