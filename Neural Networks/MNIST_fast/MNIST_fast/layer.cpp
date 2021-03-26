#include "layer.h"

layer::layer(int prev_layer_size, int curr_layer_size)
{
	for (int i = 0; i < curr_layer_size; i++)
	{
		neurons.push_back(new neuron(prev_layer_size, curr_layer_size));
	}

	this->current_layer_size = curr_layer_size;
}

void layer::c_only() {
	neurons_size = neurons.size();

	neurons_array = (neuron**)malloc(neurons_size * sizeof(neuron*));

	for (int i = 0; i < neurons_size; i++) {
		neurons_array[i] = neurons.at(i);
		neurons_array[i]->c_only();
	}

	layer_outputs_array = (double *)malloc(neurons_size * sizeof(double));

	neurons.clear();
	layer_outputs.clear();
}