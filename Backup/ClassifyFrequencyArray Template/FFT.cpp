#include "FFT.h"

FFT::FFT()  {
    //ctor
    ActualValues = new std::vector<double>();
    //ResetPeaks();
    //ResetNormalizedPeaks();
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
