#pragma once

#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <time.h>

class neuron
{
public:
	double output;
	double delta;
	std::vector<double> weights;
	double * weights_array;
	uint32_t weights_array_size;

	neuron(int, int); // ctor

	void initialize_weights(int); // randomize weight values

	double activate(std::vector<double> inputs);
	double activate(int inputs_size, double * inputs); // c

	void c_only();

	uint64_t raw_size();
	uint32_t export_neuron(char * buffer);
};

