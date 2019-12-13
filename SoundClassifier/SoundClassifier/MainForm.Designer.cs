namespace SoundClassifier
{
    partial class MainForm
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
            this.tbCMain = new System.Windows.Forms.TabControl();
            this.tbLoadFIle = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.loadSoundFileTab1 = new SoundClassifier.Form_Parts.LoadSoundFileTab();
            this.tbCMain.SuspendLayout();
            this.tbLoadFIle.SuspendLayout();
            this.SuspendLayout();
            // 
            // tbCMain
            // 
            this.tbCMain.Controls.Add(this.tbLoadFIle);
            this.tbCMain.Controls.Add(this.tabPage2);
            this.tbCMain.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbCMain.Location = new System.Drawing.Point(0, 0);
            this.tbCMain.Name = "tbCMain";
            this.tbCMain.SelectedIndex = 0;
            this.tbCMain.Size = new System.Drawing.Size(884, 561);
            this.tbCMain.TabIndex = 0;
            // 
            // tbLoadFIle
            // 
            this.tbLoadFIle.Controls.Add(this.loadSoundFileTab1);
            this.tbLoadFIle.Location = new System.Drawing.Point(4, 22);
            this.tbLoadFIle.Name = "tbLoadFIle";
            this.tbLoadFIle.Padding = new System.Windows.Forms.Padding(3);
            this.tbLoadFIle.Size = new System.Drawing.Size(876, 535);
            this.tbLoadFIle.TabIndex = 0;
            this.tbLoadFIle.Text = "Load File";
            this.tbLoadFIle.UseVisualStyleBackColor = true;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(876, 535);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // loadSoundFileTab1
            // 
            this.loadSoundFileTab1.Location = new System.Drawing.Point(0, 0);
            this.loadSoundFileTab1.Name = "loadSoundFileTab1";
            this.loadSoundFileTab1.Size = new System.Drawing.Size(876, 535);
            this.loadSoundFileTab1.TabIndex = 0;
            //this.loadSoundFileTab1.Load += new System.EventHandler(this.loadSoundFileTab1_Load);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(884, 561);
            this.Controls.Add(this.tbCMain);
            this.Name = "MainForm";
            this.Text = "Sound Classifier";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.tbCMain.ResumeLayout(false);
            this.tbLoadFIle.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tbCMain;
        private System.Windows.Forms.TabPage tbLoadFIle;
        private System.Windows.Forms.TabPage tabPage2;
        private Form_Parts.LoadSoundFileTab loadSoundFileTab1;
    }
}

