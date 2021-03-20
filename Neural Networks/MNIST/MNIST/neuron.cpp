#include "neuron.h"
#include <random>
#include <time.h>

double generateRandomNumder(double min, double max, int thread_id)
{
	srand(time(NULL) + thread_id*17);
	double random = (double)rand() / RAND_MAX;
	return min + random * (max - min);
}

Neuron::Neuron(int previousLayerSize, int currentLayerSize, int thread_id)
{
	initializeWeights(previousLayerSize, currentLayerSize, thread_id);
}

void Neuron::initializeWeights(int previousLayerSize, int currentLayerSize, int thread_id)
{
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < previousLayerSize; i++)
	{
		weights.push_back(generateRandomNumder(-0.5, 0.5, thread_id));
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

double Neuron::transfer(double activation)
{
	return (1.0 / (1 + exp(activation)));
}

double Neuron::transferDerivative(double output)
{
	return output * (1 - output);
}
