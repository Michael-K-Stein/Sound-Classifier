#ifndef FFT_H
#define FFT_H

#include <complex>
#include <iostream>
#include <valarray>
#include <float.h>
#include <vector>
#include <algorithm>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class FFT
{
    public:
        FFT();
        virtual ~FFT();

        int LoadWave(double[]);
        int AppendToWave(double);

        std::vector<Complex> * FourierTransfer(uint32_t samplingRate); /// Returns the frequency outputs for an entire
        std::vector<Complex> * FourierTransfer_Part(uint32_t samplintRate, uint32_t index);

        const double PI = 3.141592653589793238460;

        Complex * getOriginalWave();
        int getOriginalWaveSize();

        Complex * getFrequencyOutput();
        std::vector<double> * ActualValues;
        std::vector<Complex> * FrequencyOutput;

        std::vector<std::vector<double> * > * splitValues;

        std::vector<double> * getNormalizedFrequency(); // Frequencies ranged from 0 to 255. Relative to Frequency output, obviously.
        std::vector<std::vector<double> * > * OrderFrequencyOutputs(std::vector<Complex> * freqOut);

        double peakHigh;
        double peakLow;
        double range;

        std::vector<std::vector<double>> * peaks;
        std::vector<std::vector<double>> * NormalizedPeaks;

        int getK() { return k; }

        uint32_t MaxFrequency(); // The highest frequency to check.

    protected:

    private:
        int fft(CArray& x);
        int inverse_fft(CArray& x);

        int SplitValues();

        int ReloadArrays();

        int k = 25; // Amount of peaks to keep track of

        uint32_t FrequencyRange = 4096; // The highest frequency to check. (To speed up processing)
        uint32_t SamplingRate = 8000;

        void ValidatePeak(double ind, double p);
        void ValidateNormalizedPeak(double ind, double p);
        void ResetPeaks();
        void ResetNormalizedPeaks();



};

#endif // FFT_H
