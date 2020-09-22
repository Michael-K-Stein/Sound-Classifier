#include "network.h"
#include "layer.h"
#include <numeric>
#include <thread>


Network::Network(std::vector<int> spec, int inputSize, int numClasses)
{
	inputLayer = new InputLayer(0, inputSize);
	for (int i = 0; i < spec.size(); i++)
	{
		if (i == 0)
			layers.push_back(new Layer(inputSize, spec.at(i)));
		else
			layers.push_back(new Layer(layers.at(i - 1)->neurons.size(), spec.at(i)));

	}
	layers.push_back(new Layer(layers.at(layers.size() - 1)->neurons.size(), numClasses));
	this->eta = 0.1;
}

Network::~Network()
{

}

void Network::fprop(data *data)
{
	std::vector<double> inputs = *data->getNormalizedFeatureVector();
	for (Layer *layer : layers)
	{
		std::vector<double> newInputs;
		for (Neuron *n : layer->neurons)
		{
			double activation = n->activate(inputs);
			n->output = n->transfer(activation);
			newInputs.push_back(n->output);
		}
		inputs = newInputs;
	}
}

void Network::bprop(data *d, std::vector<double> * out)
{
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		Layer *layer = layers.at(i);
		std::vector<double> errors;
		if (i == layers.size() - 1)
		{
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				Neuron *n = layer->neurons.at(j);
				errors.push_back((double)(out->at(j)) - n->output);
			}
		}
		else {
			double error = 0.0;
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				for (Neuron *n : layers.at(i + 1)->neurons)
				{
					error += (n->weights.at(j) * n->delta);
				}
				errors.push_back(error);
			}
		}
		for (int j = 0; j < layer->neurons.size(); j++)
		{
			Neuron *n = layer->neurons.at(j);
			n->delta = errors.at(j) * n->transferDerivative(n->output);
		}
	}
}

void Network::updateWeights(data *data)
{
	for (int i = 0; i < layers.size(); i++)
	{
		std::vector<double> inputs;
		if (i != 0)
		{
			for (Neuron *n : layers.at(i - 1)->neurons)
			{
				inputs.push_back(n->output);
			}
		}
		else {
			inputs = *data->getNormalizedFeatureVector();
		}
		for (Neuron *n : layers.at(i)->neurons)
		{
			for (int j = 0; j < inputs.size(); j++)
			{
				n->weights[j] += (eta * n->delta * inputs.at(j));
			}
			n->bias += n->delta * eta;
		}
	}
}


std::vector<double> Network::Proccess(data * data) {
	fprop(data);
	std::vector<double> outputs;
	//printf("Output Layer: ");
	for (Neuron *n : layers.back()->neurons) {
		//printf("%.4f ", n->output);
		outputs.push_back(n->output);
	}
	//printf("\n");
	return outputs;
}