#include "layer.h"

Layer::Layer(int prev_layer_size, int curr_layer_size)
{
	for (int i = 0; i < curr_layer_size; i++)
	{
		neurons.push_back(new Neuron(prev_layer_size, curr_layer_size));
	}

	this->current_layer_size = curr_layer_size;
}

Layer::Layer(int prev_layer_size, int curr_layer_size, bool importing)
{
	if (importing) {
		if (import_c_only) {
			if (neurons_array != NULL) { free(neurons_array); }
			neurons_size = curr_layer_size;
			neurons_array = (Neuron**)malloc(neurons_size *sizeof(Neuron*));
		}

		for (int i = 0; i < curr_layer_size; i++)
		{
			if (import_c_only) {
				neurons_array[i] = new Neuron();
			}
			else {
				neurons.push_back(new Neuron());
			}
		}
	}
	else {
		for (int i = 0; i < curr_layer_size; i++)
		{
			neurons.push_back(new Neuron(prev_layer_size, curr_layer_size));
		}
	}
	this->current_layer_size = curr_layer_size;
	this->id = ++layerId;
}

void Layer::c_only() {
	neurons_size = neurons.size();

	neurons_array = (Neuron**)malloc(neurons_size * sizeof(Neuron*));

	for (int i = 0; i < neurons_size; i++) {
		neurons_array[i] = neurons.at(i);
		neurons_array[i]->c_only();
	}

	layer_outputs_array = (double *)malloc(neurons_size * sizeof(double));

	neurons.clear();
	layer_outputs.clear();
}