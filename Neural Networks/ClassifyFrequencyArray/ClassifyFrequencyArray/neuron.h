#ifndef __NEURON_HPP
#define __NEURON_HPP
#pragma once

#include <cmath>
#include <vector>
#include <stdio.h>

class Neuron {
public:
	std::vector<double> weights;
	double * weights_arr;
	double bias;
	double delta;
	double output;
	double activation;
	Neuron(int, int);
	Neuron();

	void initializeWeights(int, int);
	double activate(std::vector<double>);
	double activate_fast(double*,int);
	double transfer(double value);
	double transferDerivative(double value);

	uint32_t Export(char * buffer);
	void Import(uint32_t prevLayerSize, uint32_t layerSize, char * buffer);
	uint64_t raw_size();
};

#endif // !__NEURON_HPP
