#pragma once

#include "neuron.h"
#include <stdint.h>
#include <vector>

class layer
{
public:
	int current_layer_size;
	std::vector<neuron *> neurons;
	std::vector<double> layer_outputs;
	
	neuron ** neurons_array;
	int neurons_size;
	double * layer_outputs_array;

	layer(int prev_layer_size, int curr_layer_size); // ctor

	void c_only();
};

