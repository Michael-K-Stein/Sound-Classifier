#pragma once

#include <stdio.h>
#include <vector>
#include <cmath>
#include <random>
#include <time.h>

static int import_c_only = 1;

class Neuron
{
public:
	double output;
	double delta;
	std::vector<double> weights;
	double * weights_array;
	uint32_t weights_array_size;

	Neuron(int, int); // ctor
	Neuron(); // ctor for import

	void initialize_weights(int); // randomize weight values

	double activate(std::vector<double> inputs);
	double activate(int inputs_size, double * inputs); // c

	void c_only();


	uint32_t export_neuron(char * buffer);
	void import_neuron(uint32_t prev_layer_size, uint32_t layer_size, char * buffer);
	uint64_t raw_size();
};

