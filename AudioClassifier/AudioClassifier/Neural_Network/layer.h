#pragma once

#include "neuron.h"
#include <stdint.h>
#include <vector>

static int layerId = 0;

class Layer
{
public:
	int id;

	int current_layer_size;
	std::vector<Neuron *> neurons;
	std::vector<double> layer_outputs;

	Neuron ** neurons_array;
	int neurons_size;
	double * layer_outputs_array;

	Layer(int prev_layer_size, int curr_layer_size); // ctor
	Layer(int, int, bool); // ctor for import

	void c_only();
};

