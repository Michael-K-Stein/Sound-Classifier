#include "neuron.h"

double generateRandomNumber(double min, double max)
{
	double random = (double)rand() / RAND_MAX;
	return min + random * (max - min);
}

Neuron::Neuron() {}

Neuron::Neuron(int prev_layer_size, int curr_layer_size)
{
	initialize_weights(prev_layer_size);
}
void Neuron::initialize_weights(int prev_layer_size)
{
	for (int i = 0; i < prev_layer_size + 1; i++)
	{
		weights.push_back(generateRandomNumber(-1.0, 1.0));
	}
}

double Neuron::activate(std::vector<double> inputs) {
	double activation = weights.back(); // bias
	for (int i = 0; i < weights.size() - 1; i++) {
		activation += weights[i] * inputs[i];
	}
	return activation;
}
double Neuron::activate(int input_size, double * inputs) {
	double activation = weights_array[input_size]; // bias
	for (int i = 0; i < input_size; i++) {
		activation += weights_array[i] * inputs[i];
	}
	return activation;
}

void Neuron::c_only() {
	if (weights.size() > 0) {
		weights_array = (double*)malloc(weights.size() * sizeof(double));
		weights_array_size = weights.size();
		for (int i = 0; i < weights.size(); i++) {
			weights_array[i] = weights.at(i);
		}
		weights.clear();
	}
}

uint64_t Neuron::raw_size() {
	return (weights_array_size * sizeof(weights_array[0]));
}

uint32_t Neuron::export_neuron(char * buffer) {
	memcpy(buffer, weights_array, (weights_array_size * sizeof(weights_array[0])));

	return (weights_array_size * sizeof(weights_array[0]));
}

void Neuron::import_neuron(uint32_t prev_layer_size, uint32_t layer_size, char * buffer) {
	if (weights_array != NULL) { free(weights_array); }
	weights_array_size = prev_layer_size + 1;
	weights_array = (double*)malloc(weights_array_size * sizeof(double));

	for (int i = 0; i < weights_array_size; i++) {
		double val = 0;
		memcpy(&val, buffer + (i * sizeof(val)), sizeof(val));
		if (import_c_only) {
			weights_array[i] = val;
		} else { weights.push_back(val); }
		
	}
}