using System;
using WMPLib;
using System.IO;

namespace ControlsLibrary
{
    public class MediaFileData
    {
        private string filePath = "";

        public MediaFileData(string inpFilePath)
        {
            filePath = inpFilePath;
        }
        
        public int getSoundLength()
        {
            WindowsMediaPlayer wmp = new WindowsMediaPlayer();
            IWMPMedia mediaInformation = wmp.newMedia(filePath);
            return (int)mediaInformation.duration;
        }

        public string getSoundLengthFancy()
        {
            return getSoundLengthFancy(getSoundLength());
        }

        static string ZFill(string inp, int length)
        {
            while (inp.Length < length)
            {
                inp = "0" + inp;
            }
            return inp;
        }
        static string RoundAndZFill(double inp)
        {
            return ZFill(Math.Round(inp).ToString(),2);
        }

        public static string getSoundLengthFancy(int inpLength) // input in seconds
        {
            int seconds = inpLength % 60;
            int minutes = ((inpLength - seconds) % 3600) / 60;
            int hours = (inpLength - (minutes*60) - seconds) / 3600;
            //if (inpLength < 90)
            //{
            //    return inpLength + " sec";
            //}
            //else if (inpLength < 3600)
            //{
            //    double seconds = inpLength % 60;
            //    double minutes = (inpLength - seconds) / 60;
            //    return minutes + ":" + seconds;
            //} else
            //{
            //    double seconds = inpLength % 60;
            //    double minutes = (inpLength - seconds) % 3600;
            //    double hours = (inpLength - minutes - seconds) / 3600;
            //    return hours + ":" + minutes + ":" + seconds;
            //}
            return RoundAndZFill(hours) + ":" + RoundAndZFill(minutes) + ":" + RoundAndZFill(seconds);
        }

        public Int64 getFileSize()
        {
            return new FileInfo(filePath).Length;
        }

        public string getFileSizeFancy()
        {
            return getFileSizeFancy(getFileSize());
        }

        public static string getFileSizeFancy(Int64 size) // input in bytes
        {
            const int sizeMult = 1;
            if (size < 1024 * sizeMult)
            {
                return size + " B";
            }
            else if (size < 1048576 * sizeMult)
            {
                return Math.Round(size / 1024.0, 2) + " KB";
            }
            else if (size < 1073741824.0 * sizeMult)
            {
                return Math.Round(size / 1048576.0, 2) + " MB";
            }
            else if (size < 1099511600000 * sizeMult)
            {
                return Math.Round(size / 1073741824.0, 2) + " GB";
            }
            else if (size > 1099511600000 * sizeMult)
            {
                return "Too big :(";
            }
            return 0 + " B";
        }
    }
}
