#include "FFT.h"

FFT::FFT() {
	//ctor
	FFT::ActualValues = new std::vector<double>();
	//ResetPeaks();
	//ResetNormalizedPeaks();
}


FFT::~FFT() {
	//dtor
}

int FFT::fft(CArray& x) {
	const size_t N = x.size();
	if (N <= 1) return 1;

	// divide
	CArray even = x[std::slice(0, N / 2, 2)];
	CArray  odd = x[std::slice(1, N / 2, 2)];

	// conquer
	fft(even);
	fft(odd);

	// combine
	for (size_t k = 0; k < N / 2; ++k)
	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}

	return 0;
}


int FFT::AppendToWave(double val) {
	ActualValues->push_back(val);
	return 0;
}

std::vector<Complex> * FFT::FourierTransfer_Part(uint32_t samplingRate, uint32_t index) {
	SamplingRate = samplingRate;

	Complex * originalWave = (Complex *)calloc(SamplingRate, sizeof(Complex));
	for (int i = 0; i < SamplingRate; i++) {
		originalWave[i] = ActualValues->at((index * SamplingRate) + i);
	}

	CArray data(originalWave, SamplingRate);
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
	free(originalWave);
	return liveFrequencyOutput;
}

uint32_t FFT::MaxFrequency() {
	return std::min(FrequencyRange, SamplingRate / 2);
}