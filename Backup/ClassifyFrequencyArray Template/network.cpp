#include "network.h"
#include "layer.h"
#include "data_handler.h"
#include <numeric>
#include <thread>

template<class T>
Network<T>::Network(std::vector<int> spec, int inputSize, int numClasses)
{
	
	inputLayer = new InputLayer<T>(0, inputSize);
	for (int i = 0; i < spec.size(); i++)
	{
		if (i == 0)
			layers.push_back(new Layer<T>(inputSize, spec.at(i)));
		else
			layers.push_back(new Layer<T>(layers.at(i - 1)->neurons.size(), spec.at(i)));

	}
	layers.push_back(new Layer<T>(layers.at(layers.size() - 1)->neurons.size(), numClasses));
	this->eta = 0.1;
}

template<class T>
Network<T>::~Network()
{

}

template<class T>
void Network<T>::fprop(data<T> *data)
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
template<class T>
void Network<T>::bprop(data<T> *d)
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
				double v = d->get_class_vector()->at(j);
				errors.push_back(v - n->output);
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

template<class T>
void Network<T>::updateWeights(data<T> *data)
{
	for (int i = 0; i < layers.size(); i++)
	{
		std::vector<T> inputs;
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

template<class T>
void Network<T>::train()
{
	for (data<T> *data : *this->training_data)
	{
		if (data->get_label() != target) continue;
		fprop(data);
		bprop(data);
		updateWeights(data);
	}
	//fprintf(stderr, "Training Done\n");
}

template<class T>
void Network<T>::test()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data<T> *data : *this->test_data)
	{
		if (data->get_label() != target) continue;
		count++;
		fprop(data);
		std::vector<T> outputs;
		for (Neuron *n : layers.back()->neurons) {
			outputs.push_back(n->output);
		};
		int maxIndex = 0;
		T maxValue = outputs.at(0);
		for (int i = 1; i < outputs.size(); i++)
		{
			if (outputs.at(i) < maxValue)
			{
				maxValue = outputs.at(i);
				maxIndex = i;
			}
		}
		for (int i = 0; i < data->get_class_vector()->size(); i++)
		{
			if (data->get_class_vector()->at(i) == 1)
			{
				if (i == maxIndex) numCorrect++;
				break;
			}
		}
	}

	testPerformance = (numCorrect / count) * 100;
	fprintf(stderr, "Test Performance: %.4f\n", numCorrect / count);
}

template<class T>
void Network<T>::validate()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data<T> *data : *this->validation_data)
	{
		if (data->get_label() != target) continue;
		count++;
		fprop(data);
		std::vector<double> outputs;
		//printf("Output Layer: ");
		for (Neuron *n : layers.back()->neurons) {
			//printf("%.4f ", n->output);
			outputs.push_back(n->output);
		}
		//printf("\n");
		int minIndex = 0;
		double minValue = outputs.at(0);
		for (int i = 1; i < outputs.size(); i++)
		{
			if (outputs.at(i) < minValue)
			{
				minValue = outputs.at(i);
				minIndex = i;
			}
		}

		for (int i = 0; i < data->get_class_vector()->size(); i++)
		{
			if (data->get_class_vector()->at(i) == 1)
			{
				if (i == minIndex) numCorrect++;
				break;
			}
		}
	}
	//fprintf(stderr, "Validation Performance: %.4f\n", numCorrect / count);
}

