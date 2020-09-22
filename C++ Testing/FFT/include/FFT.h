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

        std::vector<Complex> * FourierTransfer();

        const double PI = 3.141592653589793238460;

        Complex * getOriginalWave();
        int getOriginalWaveSize();

        Complex * getFrequencyOutput();
        std::vector<double> * ActualValues;
        std::vector<Complex> * FrequencyOutput;

        std::vector<double> * getNormalizedFrequency();

        double peakHigh;
        double peakLow;

        std::vector<std::vector<double>*> * peaks;

        int getK() { return k; }


    protected:

    private:
        int fft(CArray& x);
        int inverse_fft(CArray& x);

        int ReloadArrays();

        int k = 3; // Amount of peaks to keep track of

        void ValidatePeak(double ind, double p);

};

#endif // FFT_H
