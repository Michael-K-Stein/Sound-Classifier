#ifndef FFT_H
#define FFT_H

#include <complex>
#include <iostream>
#include <valarray>
#include <float.h>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class FFT
{
    public:
        FFT();
        virtual ~FFT();

        int LoadWave(double[]);
        int AppendToWave(double);

        //std::vector<Complex> * FourierTransfer();
        std::vector<Complex> * FourierTransfer(uint32_t samplingRate);
        std::vector<Complex> * FourierTransfer_Part(uint32_t samplintRate, uint32_t index);

        const double PI = 3.141592653589793238460;

        Complex * getOriginalWave();
        int getOriginalWaveSize();

        Complex * getFrequencyOutput();
        std::vector<double> * ActualValues;
        std::vector<Complex> * FrequencyOutput;

        std::vector<std::vector<double> * > * splitValues;

        std::vector<double> * getNormalizedFrequency(); // Frequencies ranged from 0 to 100 (as in percent). Relative to Frequency output, obviously.

        double peakHigh;
        double peakLow;
        double range;

        std::vector<std::vector<double>> * peaks;
        std::vector<std::vector<double>> * NormalizedPeaks;

        int getK() { return k; }


    protected:

    private:
        int fft(CArray& x);
        int inverse_fft(CArray& x);

        int SplitValues(uint32_t samplingRate);

        int ReloadArrays();

        int k = 5; // Amount of peaks to keep track of

        void ValidatePeak(double ind, double p);
        void ValidateNormalizedPeak(double ind, double p);
        void ResetPeaks();
        void ResetNormalizedPeaks();

};

#endif // FFT_H
