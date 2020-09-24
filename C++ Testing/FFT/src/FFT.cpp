#include "FFT.h"

FFT::FFT()
{
    //ctor
    ActualValues = new std::vector<double>();
    ResetPeaks();
    ResetNormalizedPeaks();
}


FFT::~FFT()
{
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

int FFT::fft(CArray& x)
{
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

int FFT::inverse_fft(CArray& x){
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

int FFT::LoadWave(double wave[]) {
    //originalWave = double[sizeof(wave) / sizeof(wave[0])];
    for (int i = 0; i < sizeof(wave) / sizeof(wave[0]); i++) {
        ActualValues->push_back(wave[i]);
    }

    return 0;
}

int FFT::AppendToWave(double val) {
    ActualValues->push_back(val);
    return 0;
}

std::vector<Complex> * FFT::FourierTransfer() {

    //if (ReloadArrays()) { std::cout << "Failed to reload arrays" << std::endl; }

    //std::cout << "size: " << ActualValues->size() << std::endl;
    //std::cout << "size2: " << ActualValues->size() / sizeof(ActualValues->at(0)) << std::endl;

    Complex originalWave[ActualValues->size()];
    for (int i = 0; i < ActualValues->size(); i++) {
        //std::cout << "Ind: " << i << " | Act: " << ActualValues->at(i) << std::endl;
        originalWave[i] = ActualValues->at(i);
    }

    CArray data(originalWave, sizeof(originalWave) / sizeof(originalWave[0]));
    FFT::fft(data);

    FrequencyOutput = new std::vector<Complex>();

    double pHigh = data[0].real();
    double pLow = data[0].real();

    ResetNormalizedPeaks();

    for (int i = 0; i < sizeof(originalWave) / sizeof(originalWave[0]); i++){
        pHigh = std::max(pHigh, data[i].real());
        pLow = std::min(pLow, data[i].real());
        FrequencyOutput->push_back(data[i]);
        ValidatePeak((double)i, data[i].real());
    }

    peakHigh = pHigh;
    peakLow = pLow;
    range = pHigh - pLow;
    return FrequencyOutput;
}

Complex * FFT::getOriginalWave(){
    Complex newWave[ActualValues->size()];
    for (int i = 0; i < ActualValues->size(); i++) {
        newWave[i] = ActualValues->at(i);
    }

    return newWave;
}

int FFT::getOriginalWaveSize() {
    return ActualValues->size();
}

Complex * FFT::getFrequencyOutput(){
    Complex newFO[FrequencyOutput->size()];
    for (int i = 0; i < FrequencyOutput->size(); i++) {
        newFO[i] = FrequencyOutput->at(i);
    }

    return newFO;
}

int FFT::ReloadArrays(){
    Complex newWave[ActualValues->size()];
    for (int i = 0; i < ActualValues->size(); i++) {
        newWave[i] = ActualValues->at(i);
    }

    //originalWave = newWave;

    return 0;
}


std::vector<double> * FFT::getNormalizedFrequency() {
    // Absolute Value EVERYTHING
    std::vector<double> * ABSFrequencyOutput = new std::vector<double>();
    double pHigh = -1;
    double pLow = -1;
    for (int i = 0; i < FrequencyOutput->size(); i++) {
        double NormalizedValue = std::abs(FrequencyOutput->at(i).real());
        ABSFrequencyOutput->push_back(NormalizedValue);
        pHigh = std::max(pHigh, NormalizedValue);
        pLow = std::min(pLow, NormalizedValue);
    }


    std::vector<double> * NormalizedFrequencies = new std::vector<double>();
    for (int i = 0; i < FrequencyOutput->size(); i++) {
        double NormalizedValue = ( ( ABSFrequencyOutput->at(i) - pLow ) / ( pHigh - pLow ) ) * 100;
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
