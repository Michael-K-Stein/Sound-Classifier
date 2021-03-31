# FFT - Fast Fourier Transform
 Fast Fourier Transform is a fast algorithm to calculate the Discrete Fourier Transform of a complex set. FFT improves DFT, as it reduces the time complexity from O(N²) to O(N log(N)).
## DFT - Discrete Fourier Transform
DFT transforms a finite sequence of equally-spaced samples of a function into a same-length sequence of equally spaced samples of the discrete-time Fourier Transform. DFT can be used to calculate the coefficients of the complex sinusoids which built the input.
	DFT can be regarded as transforming a functions’ time/space domain into a frequency domain.
	The Discrete Fourier Transform of {xn}:=x0, x1, ..., xN-1 to {Xk}:=X0, X1, ..., XN-1
  is defined by: 
	Xk=n=0N-1xne(-i2Nkn) 
and following Euler’s formula:
	Xk=n=0N-1xncos(2Nkn)-isin(2Nkn)
We will be using FFT to transform audio data into a fixed size array of the relative amplitudes on each frequency from 1 to 4000. The input we will be giving the FFT function is the time-amplitude coordinates of an audio wave, and it will return an array with the amplitudes of the frequencies. A simple example of a fourier transform:

We will be using the classic Cooley-Tukey FFT algorithm. The Cooley-Tukey algorithm recursively breaks down a Discrete Fourier Transform into Discrete Fourier Transforms of smaller sizes, a divide-and-conquer approach. The divide-and-conquer algorithmic approach is a design paradigm which recursively breaks down a problem into multiple subproblems of a similar type until these subproblems are elementary enough to be directly solved. The solutions to the subproblems are then combined to solve the original problem.
The method used here is to divide the Discrete Transform into two sets of size N/2 (N being the size of the input set at each recursive calling) each time. Each calling then combines, by summation, the results into the passed array.

The code we will use to perform this Cooley-Tukey Fast Fourier Transform is:
```C++
int fft(CArray& x) {
    const size_t N = x.size();
    if (N <= 1) return 1;
 
    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];
 
    // conquer
    fft(even);
    fft(odd);
 
    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
 
    return 0;
}


And in order to alleviate the tension on the rest of the code, we built a wrapper class for the Fast Fourier Transform which will take care of all the processing.

Code - FFT.h + FFT.cpp:
FFT.h

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
 


FFT.cpp

 
#include "FFT.h"
 
FFT::FFT()  {
    //ctor
    ActualValues = new std::vector<double>();
}
 
 
FFT::~FFT() {
    //dtor
}
 
void FFT::ResetPeaks() {
    peaks = new std::vector<std::vector<double>>();
    std::vector<double> demoPeak = std::vector<double>(); demoPeak.push_back(-1.0); demoPeak.push_back(-DBL_MAX);
    for (int i = 0; i < k; i++) { peaks->push_back(demoPeak); }
}
 
void FFT::ResetNormalizedPeaks() {
    NormalizedPeaks = new std::vector<std::vector<double>>();
    std::vector<double> demoPeak2 = std::vector<double>(); demoPeak2.push_back(-1.0); demoPeak2.push_back(-DBL_MAX);
    for (int i = 0; i < k; i++) { NormalizedPeaks->push_back(demoPeak2); }
}
 
int FFT::fft(CArray& x) {
    const size_t N = x.size();
    if (N <= 1) return 1;
 
    // divide
    CArray even = x[std::slice(0, N/2, 2)];
    CArray  odd = x[std::slice(1, N/2, 2)];
 
    // conquer
    fft(even);
    fft(odd);
 
    // combine
    for (size_t k = 0; k < N/2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k    ] = even[k] + t;
        x[k+N/2] = even[k] - t;
    }
 
    return 0;
}
 
int FFT::inverse_fft(CArray& x) {
    // conjugate the complex numbers
    x = x.apply(std::conj);
 
    // forward fft
    fft( x );
 
    // conjugate the complex numbers again
    x = x.apply(std::conj);
 
    // scale the numbers
    x /= x.size();
 
    return 0;
}
 
int FFT::SplitValues() {
    splitValues = new std::vector<std::vector<double> *>();
 
    int amtSplits = std::ceil(ActualValues->size() / SamplingRate);
 
    for (int i = 0; i < amtSplits; i++) {
        std::vector<double> * partSamps = new std::vector<double>();
 
        for (int x = 0; x < SamplingRate; x++) {
            partSamps->push_back(ActualValues->at((i * SamplingRate) + x));
        }
 
        splitValues->push_back(partSamps);
    }
 
    return 0;
}
 
int FFT::LoadWave(double wave[]) {
    for (int i = 0; i < sizeof(wave) / sizeof(wave[0]); i++) {
        ActualValues->push_back(wave[i]);
    }
 
    return (ActualValues->size() == sizeof(wave) / sizeof(wave[0]));
}
 
int FFT::AppendToWave(double val) {
    ActualValues->push_back(val);
    return 0;
}
 
std::vector<Complex> * FFT::FourierTransfer(uint32_t samplingRate) {
    SamplingRate = samplingRate;
    SplitValues();
 
    FrequencyOutput = new std::vector<Complex>();
 
    for (int i = 0; i < samplingRate; i++) { FrequencyOutput->push_back(0); }
 
    int amtSplits = std::ceil(ActualValues->size() / SamplingRate);
 
    for (int splitInd = 0; splitInd < amtSplits; splitInd++) {
        std::vector<double> * liveValues = splitValues->at(splitInd);
 
        Complex originalWave[8000];
        for (int i = 0; i < liveValues->size(); i++) {
            originalWave[i] = liveValues->at(i);
        }
 
        CArray data(originalWave, sizeof(originalWave) / sizeof(originalWave[0]));
        FFT::fft(data);
 
        std::vector<Complex> * liveFrequencyOutput = new std::vector<Complex>();
 
        double pHigh = data[0].real();
        double pLow = data[0].real();
 
        int pHighInd = 0;
        int pLowInd = 0;
 
        ResetNormalizedPeaks();
 
        for (int i = 0; i < MaxFrequency(); i++){
            pHigh = std::max(pHigh, data[i].real());
            if (pHigh == data[i].real()) { pHighInd = i; }
            pLow = std::min(pLow, data[i].real());
            if (pLow == data[i].real()) { pLowInd = i; }
            liveFrequencyOutput->push_back(data[i]);
            ValidatePeak((double)(i), data[i].real());
            FrequencyOutput->at(i) = FrequencyOutput->at(i) + data[i].real();
        }
 
        peakHigh = pHigh;
        peakLow = pLow;
        range = pHigh - pLow;
        std::cout << "Sample #" << splitInd << ": {" << pHighInd << ", " << peakHigh << "}, {" << pLowInd << ", " << peakLow << "}" << std::endl;
    }
 
    for (int i = 0; i < SamplingRate; i++) { FrequencyOutput->at(i) = (double)FrequencyOutput->at(i).real() / (double)amtSplits; }
 
    return FrequencyOutput;
}
 
std::vector<Complex> * FFT::FourierTransfer_Part(uint32_t samplingRate, uint32_t index) {
    SamplingRate = samplingRate;
 
    Complex originalWave[8000];
    for (int i = 0; i < SamplingRate; i++) {
        originalWave[i] = ActualValues->at((index * SamplingRate) + i);
    }
 
    CArray data(originalWave, sizeof(originalWave) / sizeof(originalWave[0]));
    FFT::fft(data);
 
    std::vector<Complex> * liveFrequencyOutput = new std::vector<Complex>();
 
    double pHigh = std::abs(data[0].real());
    double pLow = std::abs(data[0].real());
 
    int pHighInd = 0;
    int pLowInd = 0;
 
    for (int i = 0; i < MaxFrequency(); i++) {
        data[i] = std::abs(data[i].real());
 
        pHigh = std::max(pHigh, data[i].real());
        if (pHigh == data[i].real()) { pHighInd = i; }
        pLow = std::min(pLow, data[i].real());
        if (pLow == data[i].real()) { pLowInd = i; }
    }
    for (int i = 0; i < MaxFrequency(); i++) {
        liveFrequencyOutput->push_back(((255.0 * (data[i] - pLow)) / (pHigh - pLow)));    // Normalize the frequency amplitudes (convert arbitrary values to 0-255)
    }
 
    std::cout << "Sample #" << index << ": {" << pHighInd << ", " << pHigh << "}, {" << pLowInd << ", " << pLow << "}" << std::endl;
 
    return liveFrequencyOutput;
}
 
std::vector<std::vector<double> * > * FFT::OrderFrequencyOutputs(std::vector<Complex> * freqOut) {
    std::vector<std::vector<double> * > * ordFreqOut = new std::vector<std::vector<double> * >();
 
    std::vector<double> * tmpV = new std::vector<double>(); tmpV->push_back(0); tmpV->push_back(-1);
    for ( int i = 0; i < k; i++) { ordFreqOut->push_back(tmpV); }
 
    //uint32_t usedIndexs[freqOut->size()];
 
    for (uint32_t ind = 0; ind < MaxFrequency(); ind++) {
        for (int i = 0; i < k; i++) {
            if ( freqOut->at(ind).real() > ordFreqOut->at(i)->at(1) ) {
                // Move back the rest
                for (int ii = k-1; ii > i; ii--){
                    ordFreqOut->at(ii) = ordFreqOut->at(ii-1);
                }
 
                std::vector<double> * tV = new std::vector<double>(); tV->push_back(ind); tV->push_back(freqOut->at(ind).real());
                ordFreqOut->at(i) = tV;
                break;
            }
        }
    }
 
    return ordFreqOut;
}
 
std::vector<double> * FFT::getNormalizedFrequency() {
    // Absolute Value EVERYTHING
    std::vector<double> * ABSFrequencyOutput = new std::vector<double>();
    double pHigh = -1;
    double pLow = -1;
    for (int i = 0; i < MaxFrequency(); i++) {
        double NormalizedValue = std::abs(FrequencyOutput->at(i).real());
        ABSFrequencyOutput->push_back(NormalizedValue);
        pHigh = std::max(pHigh, NormalizedValue);
        pLow = std::min(pLow, NormalizedValue);
    }
 
 
    std::vector<double> * NormalizedFrequencies = new std::vector<double>();
    for (int i = 0; i < MaxFrequency(); i++) {
        double NormalizedValue = ( ( 255.0 * ( ABSFrequencyOutput->at(i) - pLow ) ) / ( pHigh - pLow ) );
        NormalizedFrequencies->push_back(NormalizedValue);
        ValidateNormalizedPeak((double)i, NormalizedValue);
    }
 
    return NormalizedFrequencies;
}
 
void FFT::ValidatePeak(double ind, double p) {
    bool repl = false;
    for (int i = 0; i < k; i++) {
        if (p > peaks->at(i).at(1) && !repl) {
 
            for (int ii = k-2; ii >= i; ii--) {
                peaks->at(ii + 1).at(0) = peaks->at(ii).at(0);
                peaks->at(ii + 1).at(1) = peaks->at(ii).at(1);
            }
 
            peaks->at(i).at(0) = ind;
            peaks->at(i).at(1) = p;
            repl = true;
            break;
        }
    }
}
 
void FFT::ValidateNormalizedPeak(double ind, double p) {
    bool repl = false;
    for (int i = 0; i < k; i++) {
 
        std::vector<double> t1 = NormalizedPeaks->at(i);
        double t2 = NormalizedPeaks->at(i).at(0);
 
        if (p > NormalizedPeaks->at(i).at(1) && !repl) {
 
            for (int ii = k-2; ii >= i; ii--) {
                NormalizedPeaks->at(ii + 1).at(0) = NormalizedPeaks->at(ii).at(0);
                NormalizedPeaks->at(ii + 1).at(1) = NormalizedPeaks->at(ii).at(1);
            }
 
            NormalizedPeaks->at(i).at(0) = ind;
            NormalizedPeaks->at(i).at(1) = p;
            repl = true;
            break;
        }
    }
}
 
uint32_t FFT::MaxFrequency() {
    return std::min( FrequencyRange , SamplingRate/2 );
}
```

Note: this class uses a static sampling rate of 8000Hz, and as such we will preprocess all our files to this sampling rate.
