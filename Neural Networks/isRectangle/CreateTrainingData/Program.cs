using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Drawing;

namespace CreateTrainingData
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Starting up training data creator!");

            if (!File.Exists("../../../Training Data/Shapes.byte"))
            {
                UInt32 imgAmt = 0;
                UInt32 imgW = 32;
                UInt32 imgH = 32;

                UInt32[] fileHeader = { imgAmt, imgW, imgH };
                byte[] fileHeaderBytes = new byte[fileHeader.Length * sizeof(UInt32)];

                fileHeaderBytes = fileHeader.Select(i => (byte)i).ToArray();

                File.WriteAllBytes("../../../Training Data/Shapes.byte", fileHeaderBytes);

                UInt32[] labelHeader = { imgAmt };
                fileHeaderBytes = new byte[fileHeader.Length * sizeof(UInt32)];

                fileHeaderBytes = labelHeader.Select(i => (byte)i).ToArray();

                File.WriteAllBytes("../../../Training Data/Labels.byte", fileHeaderBytes);
            }

            Console.WriteLine();
            Console.WriteLine("How many shapes should be added?");
            int amtShapesToAdd = Convert.ToInt32(Console.ReadLine());
            int startInd = Convert.ToInt32(File.ReadAllText("../../../Training Data/Shape Count.txt"));

            Brush bB = new SolidBrush(Color.White);
            int shapesAdded = 0;

            while (shapesAdded < amtShapesToAdd)
            {
                Bitmap bmp = new Bitmap(32, 32);

                Graphics g = Graphics.FromImage(bmp);
                Random r = new Random(startInd + amtShapesToAdd);
                int rn = r.Next() % 3;

                UInt32 type = UInt32.MaxValue;

                if (rn == 0) {
                    g.FillRectangle(bB, 0, 0, 32, 32);
                    type = 0;
                } else if (rn == 1) {
                    g.FillEllipse(bB, 0, 0, 32, 32);
                    type = 1;
                } else if (rn == 2) {
                    g.FillPolygon(bB, new Point[] { new Point(16,0), new Point(0, 32), new Point(32,32) });
                    type = 2;
                }

                byte[] imageBytes = new byte[1024];
                for (int x = 0; x < 32; x++)
                {
                    for (int y = 0; y < 32; y++)
                    {
                        if (bmp.GetPixel(x, y) == Color.FromArgb(255,255,255,255)) {
                            imageBytes[(y * 32) + x] = (byte)255;
                        }
                        else
                        {
                            imageBytes[(y * 32) + x] = (byte)0;
                        }
                        
                    }
                }

                byte[] prevBytes = File.ReadAllBytes("../../../Training Data/Shapes.byte");
                byte[] allBytes = new byte[prevBytes.Length + 1024];
                prevBytes.CopyTo(allBytes, 0);
                imageBytes.CopyTo(allBytes, prevBytes.Length);
                File.WriteAllBytes("../../../Training Data/Shapes.byte", allBytes);

                prevBytes = File.ReadAllBytes("../../../Training Data/Labels.byte");
                allBytes = new byte[prevBytes.Length + 32];
                prevBytes.CopyTo(allBytes, 0);
                UInt32[] typeArr = { type };
                (typeArr.Select(i => (byte)i).ToArray()).CopyTo(allBytes, prevBytes.Length);
                File.WriteAllBytes("../../../Training Data/Labels.byte", allBytes);

                shapesAdded++;
            }

            File.WriteAllText("../../../Training Data/Shape Count.txt", (startInd + shapesAdded).ToString());

            UInt32[] amtIMG = { Convert.ToUInt32(startInd + shapesAdded) };
            byte[] allShapeBytes = File.ReadAllBytes("../../../Training Data/Shapes.byte");
            byte[] allLabelBytes = File.ReadAllBytes("../../../Training Data/Labels.byte");

            allShapeBytes[0] = amtIMG.Select(i => (byte)i).ToArray()[0];
            allLabelBytes[0] = amtIMG.Select(i => (byte)i).ToArray()[0];

            //(amtIMG.Select(i => (byte)i).ToArray()).CopyTo(allShapeBytes, 0);
            File.WriteAllBytes("../../../Training Data/Shapes.byte", allShapeBytes);
            File.WriteAllBytes("../../../Training Data/Labels.byte", allLabelBytes);

        }
    }
}
