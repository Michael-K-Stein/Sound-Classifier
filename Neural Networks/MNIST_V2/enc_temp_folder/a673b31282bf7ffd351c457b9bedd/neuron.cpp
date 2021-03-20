#include "neuron.h"
#include <random>
#include <time.h>

int n = 0;

double generateRandomNumber(double min, double max)
{
	srand(time(NULL) + (n++));
	double random = (double)rand() / RAND_MAX;
	return min + random * (max - min);
}

Neuron::Neuron(int previousLayerSize, int currentLayerSize)
{
	initializeWeights(previousLayerSize);
}
void Neuron::initializeWeights(int previousLayerSize)
{
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < previousLayerSize + 1; i++)
	{
		weights.push_back(generateRandomNumber(-1.0, 1.0));
	}
}