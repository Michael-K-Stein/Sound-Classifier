#include "neuron.h"

double generateRandomNumber(double min, double max)
{	
	double random = (double)rand() / RAND_MAX;
	return min + random * (max - min);
}

neuron::neuron(int prev_layer_size, int curr_layer_size)
{
	initialize_weights(prev_layer_size);
}
void neuron::initialize_weights(int prev_layer_size)
{
	for (int i = 0; i < prev_layer_size + 1; i++)
	{
		weights.push_back(generateRandomNumber(-1.0, 1.0));
	}
}

double neuron::activate(std::vector<double> inputs) {
	double activation = weights.back(); // bias
	for (int i = 0; i < weights.size() - 1; i++) {
		activation += weights[i] * inputs[i];
	}
	return activation;
}
double neuron::activate(int input_size, double * inputs) {
	double activation = weights_array[input_size]; // bias
	for (int i = 0; i < input_size; i++) {
		activation += weights_array[i] * inputs[i];
	}
	return activation;
}

void neuron::c_only() {
	if (weights.size() > 0) {
		weights_array = (double*)malloc(weights.size() * sizeof(double));
		weights_array_size = weights.size();
		for (int i = 0; i < weights.size(); i++) {
			weights_array[i] = weights.at(i);
		}
		weights.clear();
	}
}

uint64_t neuron::raw_size() {
	return (weights_array_size * sizeof(weights_array[0]));
}

uint32_t neuron::export_neuron(char * buffer) {
	memcpy(buffer, weights_array, (weights_array_size * sizeof(weights_array[0])));

	return (weights_array_size * sizeof(weights_array[0]));
}