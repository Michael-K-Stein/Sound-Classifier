using System;
using System.Windows.Forms;
using System.IO;
using ControlsLibrary;
using NAudio;
using NAudio.Wave;

namespace SoundClassifier.Form_Parts
{
    public partial class LoadSoundFileTab : UserControl
    { 
        public string filePath = "";
        public MediaFileData fileData;

        public LoadSoundFileTab()
        {
            InitializeComponent();
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
            fileData = new ControlsLibrary.MediaFileData(filePath);
            lSelectedFile.Text = filePath;
            lFDSize.Text = "Size: " +  fileData.getFileSizeFancy();
            lFDLength.Text = "Length: " + fileData.getSoundLengthFancy();
            NAudioFileData();
        }

        void NAudioFileData()
        {
            NAudio.Wave.WaveStream waveStream = new NAudio.Wave.WaveFileReader(filePath);

            waveViewer1.SamplesPerPixel = 400;
            waveViewer1.StartPosition = 40000;

            waveViewer1.WaveStream = waveStream;


        }

        void FourierIfy()
        {
            NAudio.Dsp.FastFourierTransform.FFT(false, 1,  );


            //using (NAudio.Wave.WaveFileReader reader = new NAudio.Wave.WaveFileReader(filePath))
            //{
            //    NAudio.Wave.IWaveProvider stream32 = new NAudio.Wave.Wave16ToFloatProvider(reader);
            //    NAudio.Wave.IWaveProvider streamEffect = new AutoTuneWaveProvider(stream32, autotuneSettings);
            //    NAudio.Wave.IWaveProvider stream16 = new NAudio.Wave.WaveFloatTo16Provider(streamEffect);
            //    using (NAudio.Wave.WaveFileWriter converted = new NAudio.Wave.WaveFileWriter(tempFile, stream16.WaveFormat))
            //    {
            //        // buffer length needs to be a power of 2 for FFT to work nicely
            //        // however, make the buffer too long and pitches aren't detected fast enough
            //        // successful buffer sizes: 8192, 4096, 2048, 1024
            //        // (some pitch detection algorithms need at least 2048)
            //        byte[] buffer = new byte[8192];
            //        int bytesRead;
            //        do
            //        {
            //            bytesRead = stream16.Read(buffer, 0, buffer.Length);
            //            converted.Write(buffer, 0, bytesRead);
            //        } while (bytesRead != 0 && converted.Length < reader.Length);
            //    }
            //}
        }

        void UnloadFileData()
        {
            lSelectedFile.Text = "Please select a file";
            lFDSize.Text = "Size: 0 KB";
            lFDLength.Text = "Length: 0 sec";
        }


    }
}
