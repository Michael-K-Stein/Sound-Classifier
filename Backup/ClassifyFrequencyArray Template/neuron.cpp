#include "neuron.h"
#include <random>

template<class T>
T generateRandomNumder(T min, T max)
{
	T random = (T)rand() / RAND_MAX;
	return min + random * (max - min);
}

template<class T>
Neuron<T>::Neuron(int previousLayerSize, int currentLayerSize)
{
	initializeWeights(previousLayerSize, currentLayerSize);
}
template<class T>
void Neuron<T>::initializeWeights(int previousLayerSize, int currentLayerSize)
{
	std::default_random_engine generator;
	std::normal_distribution<T> distribution(0.0, 1.0);
	for (int i = 0; i < previousLayerSize; i++)
	{
		weights.push_back(generateRandomNumder(-0.5, 0.5));
	}
}
template<class T>
T Neuron<T>::activate(std::vector<T> input)
{
	this->activation = this->bias;
	for (int i = 0; i < input.size(); i++)
	{
		this->activation += weights.at(i) * input.at(i);
	}
	return this->activation;
}

template<class T>
T Neuron<T>::transfer(T activation)
{
	return (1.0 / (1 + exp(activation)));
}

template<class T>
T Neuron<T>::transferDerivative(T output)
{
	return output * (1 - output);
}
