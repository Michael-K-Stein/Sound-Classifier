namespace SoundClassifier.Form_Parts
{
    partial class LoadSoundFileTab
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
            this.bLoadFile = new System.Windows.Forms.Button();
            this.lSelectedFile = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.lFDLength = new System.Windows.Forms.Label();
            this.lFDSize = new System.Windows.Forms.Label();
            this.waveViewer1 = new NAudio.Gui.WaveViewer();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // bLoadFile
            // 
            this.bLoadFile.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bLoadFile.Location = new System.Drawing.Point(3, 3);
            this.bLoadFile.Name = "bLoadFile";
            this.bLoadFile.Size = new System.Drawing.Size(120, 30);
            this.bLoadFile.TabIndex = 0;
            this.bLoadFile.Text = "Load File";
            this.bLoadFile.UseVisualStyleBackColor = true;
            this.bLoadFile.Click += new System.EventHandler(this.bLoadFile_Click);
            // 
            // lSelectedFile
            // 
            this.lSelectedFile.AutoSize = true;
            this.lSelectedFile.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lSelectedFile.Location = new System.Drawing.Point(129, 8);
            this.lSelectedFile.Name = "lSelectedFile";
            this.lSelectedFile.Size = new System.Drawing.Size(118, 19);
            this.lSelectedFile.TabIndex = 1;
            this.lSelectedFile.Text = "Please select a file";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.lFDLength);
            this.groupBox1.Controls.Add(this.lFDSize);
            this.groupBox1.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(3, 39);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(120, 62);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "FIle Data";
            // 
            // lFDLength
            // 
            this.lFDLength.AutoSize = true;
            this.lFDLength.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lFDLength.Location = new System.Drawing.Point(6, 35);
            this.lFDLength.Name = "lFDLength";
            this.lFDLength.Size = new System.Drawing.Size(89, 19);
            this.lFDLength.TabIndex = 1;
            this.lFDLength.Text = "Length: 0 sec";
            // 
            // lFDSize
            // 
            this.lFDSize.AutoSize = true;
            this.lFDSize.Font = new System.Drawing.Font("Times New Roman", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lFDSize.Location = new System.Drawing.Point(6, 16);
            this.lFDSize.Name = "lFDSize";
            this.lFDSize.Size = new System.Drawing.Size(75, 19);
            this.lFDSize.TabIndex = 0;
            this.lFDSize.Text = "Size: 0 KB";
            // 
            // waveViewer1
            // 
            this.waveViewer1.Location = new System.Drawing.Point(3, 107);
            this.waveViewer1.Name = "waveViewer1";
            this.waveViewer1.SamplesPerPixel = 128;
            this.waveViewer1.Size = new System.Drawing.Size(870, 425);
            this.waveViewer1.StartPosition = ((long)(0));
            this.waveViewer1.TabIndex = 3;
            this.waveViewer1.WaveStream = null;
            // 
            // LoadSoundFileTab
            // 
            this.Controls.Add(this.waveViewer1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.lSelectedFile);
            this.Controls.Add(this.bLoadFile);
            this.Name = "LoadSoundFileTab";
            this.Size = new System.Drawing.Size(876, 535);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bLoadFile;
        private System.Windows.Forms.Label lSelectedFile;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label lFDLength;
        private System.Windows.Forms.Label lFDSize;
        private NAudio.Gui.WaveViewer waveViewer1;
    }
}
