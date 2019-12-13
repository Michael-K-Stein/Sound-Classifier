using System;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace SoundClassifier.Form_Parts
{
    public partial class LoadSoundFileTab : UserControl
    { 

        public string filePath = "";

        public LoadSoundFileTab()
        {
            InitializeComponent();
        }

        public static double GetSoundLength(string fileName)
        {
            return 0;
        }

        private void bLoadFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog oFD = new OpenFileDialog();
            oFD.FileOk += oFDSoundFileOk;
            oFD.Filter = "All Media Files|*.wav;*.aac;*.wma;*.wmv;*.avi;*.mpg;*.mpeg;*.m1v;*.mp2;*.mp3;*.mpa;*.mpe;*.m3u;*.mp4;*.mov;*.3g2;*.3gp2;*.3gp;*.3gpp;*.m4a;*.cda;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi;*.mkv;*.WAV;*.AAC;*.WMA;*.WMV;*.AVI;*.MPG;*.MPEG;*.M1V;*.MP2;*.MP3;*.MPA;*.MPE;*.M3U;*.MP4;*.MOV;*.3G2;*.3GP2;*.3GP;*.3GPP;*.M4A;*.CDA;*.AIF;*.AIFC;*.AIFF;*.MID;*.MIDI;*.RMI;*.MKV"; 
            oFD.ShowDialog();
        }

        void oFDSoundFileOk(object sender, System.ComponentModel.CancelEventArgs e) {
            if (!e.Cancel)
            {
                if (File.Exists((sender as OpenFileDialog).FileName))
                {
                    filePath = (sender as OpenFileDialog).FileName;
                    LoadFileData();
                }else
                {
                    UnloadFileData();
                }
            }
        }

        void LoadFileData()
        {
            lSelectedFile.Text = filePath;
            lFDSize.Text = "Size: " + fileSizes(new FileInfo(filePath).Length);
            lFDLength.Text = "Length: " + GetSoundLength(filePath).ToString() + " sec";
        }

        void UnloadFileData()
        {
            lSelectedFile.Text = "Please select a file";
            lFDSize.Text = "Size: 0 KB";
            lFDLength.Text = "Length: 0 sec";
        }

        public static string fileSizes(Int64 size)
        {
            const int sizeMult = 1;
            if (size < 1024 * sizeMult) {
                return size + " B";
            } else if (size < 1048576 * sizeMult) {
                return Math.Round(size / 1024.0, 2) + " KB";
            } else if (size < 1073741824.0 * sizeMult) {
                return Math.Round(size / 1048576.0, 2) + " MB";
            } else if (size < 1099511600000 * sizeMult) {
                return Math.Round(size / 1073741824.0, 2) + " GB";
            } else if (size > 1099511600000 * sizeMult) {
                return "Too big :(";
            }
            return 0 + " B";
        }
    }
}
