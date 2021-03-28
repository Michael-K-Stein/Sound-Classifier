#pragma once
#ifndef FFT_H
#define FFT_H

#include <complex>
#include <iostream>
#include <valarray>
#include <float.h>
#include <vector>
#include <math.h>
#include <algorithm>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


class FFT
{
public:
	FFT();
	virtual ~FFT();
	int AppendToWave(double);

	std::vector<Complex> * FourierTransfer_Part(uint32_t samplintRate, uint32_t index);

	std::vector<double> * ActualValues;

	const double PI = 3.141592653589793238460;

	uint32_t MaxFrequency(); // The highest frequency to check.

private:
	int fft(CArray& x);

	uint32_t FrequencyRange = 4000; // The highest frequency to check. (To speed up processing)
	uint32_t SamplingRate = 8000;
};

#endif // FFT_H