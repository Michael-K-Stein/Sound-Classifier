#include "neuron.h"
#include <random>
#include <time.h>

double generateRandomNumder(double min, double max)
{
	double random = (double)rand() / RAND_MAX;
	return min + random * (max - min);
}
Neuron::Neuron(){}
Neuron::Neuron(int previousLayerSize, int currentLayerSize)
{
	initializeWeights(previousLayerSize, currentLayerSize);
}

void Neuron::initializeWeights(int previousLayerSize, int currentLayerSize)
{
	weights_arr = (double*)malloc(previousLayerSize * sizeof(double));

	srand(time(NULL));
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < previousLayerSize; i++)
	{
		double val = generateRandomNumder(-0.5, 0.5);
		weights.push_back(val);
		weights_arr[i] = val;
	}
}

double Neuron::activate(std::vector<double> input)
{
	this->activation = this->bias;
	for (int i = 0; i < input.size(); i++)
	{
		this->activation += weights.at(i) * input.at(i);
	}
	return this->activation;
}
double Neuron::activate_fast(double* input, int size)
{
	this->activation = this->bias;
	for (int i = 0; i < size; i++)
	{
		this->activation += weights_arr[i] * input[i];
	}
	return this->activation;
}

double Neuron::transfer(double activation)
{
	return (1.0 / (1 + exp(activation)));
}

double Neuron::transferDerivative(double output)
{
	return output * (1 - output);
}

uint64_t Neuron::raw_size() {
	return (weights.size() * sizeof(weights.at(0))) + sizeof(bias);
}

uint32_t Neuron::Export(char * buffer) {
	memcpy(buffer, &bias, sizeof(bias));
	memcpy(buffer + sizeof(bias), weights_arr, (weights.size() * sizeof(weights.at(0))));

	return sizeof(bias) + (weights.size() * sizeof(weights.at(0)));
}

void Neuron::Import(uint32_t prevLayerSize, uint32_t layerSize, char * buffer) {
	memcpy(&bias, buffer + 0, sizeof(bias));

	weights_arr = (double*)malloc(prevLayerSize * sizeof(double));

	for (int i = 0; i < prevLayerSize; i++) {
		double val = 0;
		memcpy(&val, buffer + sizeof(bias) + (i * sizeof(val)), sizeof(val));
		weights.push_back(val);
		weights_arr[i] = val;
	}
}