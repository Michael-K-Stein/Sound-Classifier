#ifndef __NEURON_HPP
#define __NEURON_HPP
#pragma once

#include <cmath>
#include <vector>
#include <stdio.h>

class Neuron {
public:
	std::vector<double> weights;
	double bias;
	double delta;
	double output;
	double activation;
	Neuron(int, int, int thread_id);

	void initializeWeights(int, int, int thread_id);
	double activate(std::vector<double>);
	double transfer(double value);
	double transferDerivative(double value);

};

#endif // !__NEURON_HPP
