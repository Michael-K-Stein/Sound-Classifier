#include "network.h"
#include "layer.h"
#include "DataHandler.h"
#include <numeric>
#include <algorithm>
#include <thread>


#include <chrono>
#include <utility>
typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

Network::Network(std::vector<int> spec, int inputSize, int numClasses, double learningRate)
{
	for (int i = 0; i < spec.size(); i++)
	{
		if (i == 0)
			layers.push_back(new Layer(inputSize, spec.at(i)));
		else
			layers.push_back(new Layer(layers.at(i - 1)->neurons.size(), spec.at(i)));

	}
	layers.push_back(new Layer(layers.at(layers.size() - 1)->neurons.size(), numClasses));
	this->learningRate = learningRate;
}

Network::~Network() {}

double Network::activate(std::vector<double> weights, std::vector<double> input)
{
	double activation = weights.back(); // bias term
	for (int i = 0; i < weights.size() - 1; i++)
	{
		activation += weights[i] * input[i];
	}
	return activation;
}

double Network::transfer(double activation)
{
	return 1.0 / (1.0 + exp(-activation));
}

double Network::transferDerivative(std::vector<double> errors, std::vector<double> inputs)
{
	double derivative = 0;

	for (int i = 0; i < errors.size(); i++) {
		derivative += errors[i] * inputs[i];
	}

	derivative *= 2.0 / errors.size();

	return derivative;

	///return output * (1 - output); // The bug is here!!!!!!
}

std::vector<double> Network::CalculateDerivative(Data * data, Layer * myLayer) {

	std::vector<double> errors1;
	std::vector<double> errors2;

	double gamma = 0.00000001;

	for (Neuron *n : myLayer->neurons) {
		double error = n->weights.at(n->weights.size() - 1); // bias
		for (int w = 0; w < n->weights.size() - 1; w++) {
			error += n->weights[w] * (data->getNormalizedFeatureVector()->at(w));
		}
		errors1.push_back(error);
	}

	for (Neuron *n : myLayer->neurons) {
		double error = n->weights.at(n->weights.size() - 1); // bias
		for (int w = 0; w < n->weights.size() - 1; w++) {
			error += n->weights[w] * (data->getNormalizedFeatureVector()->at(w) + gamma);
		}
		errors2.push_back(error);
	}

	std::vector<double> errors_difference;

	for (int i = 0; i < errors1.size(); i++) {
		errors_difference.push_back((errors2[i] - errors1[i]) / gamma);
	}

	return errors_difference;
}
std::vector<double> Network::CalculateDerivative(Layer * preLayer, Layer * myLayer) {

	std::vector<double> errors1;
	std::vector<double> errors2;

	double gamma = 0.00000001;

	for (Neuron *n : myLayer->neurons) {
		double error = n->weights.at(n->weights.size() - 1); // bias
		for (int w = 0; w < n->weights.size() - 1; w++) {
			error += n->weights[w] * preLayer->neurons[w]->output;
		}
		errors1.push_back(error);
	}

	for (Neuron *n : myLayer->neurons) {
		double error = n->weights.at(n->weights.size() - 1); // bias
		for (int w = 0; w < n->weights.size() - 1; w++) {
			error += n->weights[w] * (preLayer->neurons[w]->output + gamma);
		}
		errors2.push_back(error);
	}

	std::vector<double> errors_difference;

	for (int i = 0; i < errors1.size(); i++) {
		errors_difference.push_back((errors2[i] - errors1[i]) / gamma);
	}

	return errors_difference;
}

std::vector<double> Network::fprop(Data *data)
{
	std::vector<double> inputs = *data->getNormalizedFeatureVector();
	for (int i = 0; i < layers.size(); i++)
	{
		Layer *layer = layers.at(i);
		std::vector<double> newInputs;
		for (Neuron *n : layer->neurons)
		{
			double activation = this->activate(n->weights, inputs);
			n->output = this->transfer(activation);
			newInputs.push_back(n->output);
		}
		layer->layerOutputs = newInputs;
		inputs = newInputs;
	}
	return inputs; // output layer outputs
}

void Network::bprop(Data *data)
{
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		Layer *layer = layers.at(i);
		std::vector<double> errors;
		if (i != layers.size() - 1)
		{
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				double error = 0.0;
				for (Neuron *n : layers.at(i + 1)->neurons)
				{
					error += (n->weights.at(j) * n->delta);
				}
				errors.push_back(error);
			}
		}
		else {
			// If is last layer (i == layers.size()-1)
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				Neuron *n = layer->neurons.at(j);
				errors.push_back((double)data->getClassVector().at(j) - n->output); // expected - actual
			}
		}

		//double gradient = this->transferDerivative(errors, layer->layerOutputs);


		std::vector<double> gradient;
		if (i != 0) {
			gradient = this->CalculateDerivative(layers.at(i - 1), layers.at(i));
		}
		else {
			gradient = this->CalculateDerivative(data, layers.at(i));
		}

		for (int j = 0; j < layer->neurons.size(); j++)
		{
			Neuron *n = layer->neurons.at(j);
			n->delta = n->output - gradient[j] * (1);  //errors.at(j) * ; //gradient / derivative part of back prop.
		}
	}
}

void Network::bprop(std::vector<double> deriv_errors)
{
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		Layer *layer = layers.at(i);
		std::vector<double> errors;
		if (i != layers.size() - 1)
		{
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				double error = 0.0;
				for (Neuron *n : layers.at(i + 1)->neurons)
				{
					error += (n->weights.at(j) * n->delta);
				}
				errors.push_back(error);
			}
		}
		else {
			// If is last layer (i == layers.size()-1)
			/*for (int j = 0; j < layer->neurons.size(); j++)
			{
				double partial_deriv = deriv_errors[j];

				Neuron *n = layer->neurons.at(j);
				errors.push_back((double)data->getClassVector().at(j) - n->output); // expected - actual
			}*/
			errors = deriv_errors;
		}

		//double gradient = this->transferDerivative(errors, layer->layerOutputs);


		/*std::vector<double> gradient;
		if (i != 0) {
			gradient = this->CalculateDerivative(layers.at(i - 1), layers.at(i));
		}
		else {
			gradient = this->CalculateDerivative(data, layers.at(i));
		}*/

		for (int j = 0; j < layer->neurons.size(); j++)
		{
			Neuron *n = layer->neurons.at(j);
			n->delta = errors[j];  //errors.at(j) * ; //gradient / derivative part of back prop.
		}
	}
}

void Network::updateWeights(Data *data)
{
	std::vector<double> inputs = *data->getNormalizedFeatureVector();
	for (int i = 0; i < layers.size(); i++)
	{
		if (i != 0)
		{
			for (Neuron *n : layers.at(i - 1)->neurons)
			{
				inputs.push_back(n->output);
			}
		}
		for (Neuron *n : layers.at(i)->neurons)
		{
			for (int j = 0; j < inputs.size(); j++)
			{
				n->weights.at(j) += this->learningRate * n->delta * inputs.at(j);
			}
			n->weights.back() += this->learningRate * n->delta;
		}
		inputs.clear();
	}
}

int Network::predict(Data * data)
{
	std::vector<double> outputs = fprop(data);
	return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}

void Network::train(int numEpochs, int iter)
{
	for (int i = 0; i < numEpochs; i++)
	{
		double sumError = 0.0;
		std::vector<double> deriv_error;

		int output_vector_size = this->trainingData->at(0)->getClassVector().size();
		for (int i = 0; i < output_vector_size; i++) { deriv_error.push_back(0.0); }

		for (Data *data : *this->trainingData)
		{
			//if (data->getLabel() != iter) { continue; }
			std::vector<double> outputs = fprop(data);
			std::vector<int> expected = data->getClassVector();
			double tempErrorSum = 0.0;
			for (int j = 0; j < outputs.size(); j++)
			{
				tempErrorSum += pow((double)expected.at(j) - outputs.at(j), 2);
				deriv_error[j] += (double)expected.at(j) - outputs.at(j);
			}
			sumError += tempErrorSum;

			for (int j = 0; j < output_vector_size; j++)
			{
				deriv_error.at(j) /= this->trainingData->size();
				deriv_error.at(j) *= 2;
			}

			bprop(deriv_error);
			updateWeights(data);

			for (int j = 0; j < output_vector_size; j++)
			{
				deriv_error.at(j) = 0;
			}
		}
		printf("Iteration: %d \t Error=%.4f\n", (iter * numEpochs) + i, sumError);
	}
}

double Network::test()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (Data *data : *this->testData)
	{
		count++;
		int index = predict(data);
		if (data->getClassVector().at(index) == 1) numCorrect++;
	}

	testPerformance = (numCorrect / count);
	return testPerformance;
}

void Network::validate()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (Data *data : *this->validationData)
	{
		count++;
		int index = predict(data);
		if (data->getClassVector().at(index) == 1) numCorrect++;
	}
	printf("Validation Performance: %.4f%%\n", (numCorrect*100) / count);
}

int main()
{
	DataHandler *dataHandler = new DataHandler();
//#ifdef MNIST
	dataHandler->readInputData("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-images.idx3-ubyte");
	dataHandler->readLabelData("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\\Training Data\\train-labels.idx1-ubyte");
	dataHandler->countClasses();
//#else
	//dataHandler->readCsv("C:\\Users\\stein\\Desktop\\Research Project 2020\\Neural Networks\\MNIST\Training Data\\iris.data", ",");
//#endif
	dataHandler->splitData();
	std::vector<int> hiddenLayers = { 10,10 };
	Network *netw = new Network(
		hiddenLayers,
		dataHandler->getTrainingData()->at(0)->getNormalizedFeatureVector()->size(),
		dataHandler->getClassCounts(),
		0.25);
	netw->setTrainingData(dataHandler->getTrainingData());
	netw->setTestData(dataHandler->getTestData());
	netw->setValidationData(dataHandler->getValidationData());

	printf("Initial Validation: \n");
	netw->validate();
	printf("Now training...\n");

	for (int i = 0; i < 100; i++) {

		TimeVar t1 = timeNow();
		netw->train(2, i);
		double trainTime = duration(timeNow() - t1);

		TimeVar t2 = timeNow();
		netw->validate();
		double validateTime = duration(timeNow() - t2);
		

		
		

		printf("Training time: %.2f miliseconds\n", trainTime);
		printf("Validating time: %.2f miliseconds\n", validateTime);

		if (i % 10 == 0) {
			printf("Test Performance: %.3f\n", netw->test());
		}
	}
	printf("Test Performance: %.3f\n", netw->test());


	/*
	auto lambda = [&](Network * net, int iter) {
		for (int i = 0; i < 5; i++) {
			net->train(1, iter);
			net->validate();
		}
		printf("Test Performance: %.3f @ iter: %d\n", net->test(), iter);
	};
	
	for (int j = 0; j < 3; j++) {
		std::vector<std::thread> threads;

		for (int i = 0; i < 10; i++)
		{
			threads.emplace_back(std::thread(lambda, netw, i));
		}

		for (auto &th : threads)
		{
			th.join();
		}

		printf("Test Performance: %.3f\n", netw->test());
	}*/

	return 0;
}