#include "network.h"
#include "layer.h"
#include "data_handler.h"
#include <numeric>
#include <thread>

Network::Network(){}
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
	this->eta = 0.05;
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

void Network::fprop_fast(data *data) /// Uses arrays instead of vectors
{
	//std::vector<double> inputs = *data->getNormalizedFeatureVector();
	int input_size = data->getNormalizedFeatureVector()->size();
	double * inputs = (double*)malloc(input_size * sizeof(double));
	memcpy(inputs, data->getNormalizedFeatureArray(), input_size * sizeof(double));
	for (Layer *layer : layers)
	{
		//std::vector<double> newInputs;
		double * newInputs = (double*)malloc(layer->neurons.size() * sizeof(double));
		int i = 0;
		for (Neuron *n : layer->neurons)
		{
			double activation = n->activate_fast(inputs, input_size);
			n->output = n->transfer(activation);
			//newInputs.push_back(n->output);
			newInputs[i] = n->output;
			i++;
		}
		free(inputs);
		inputs = (double*)malloc(i * sizeof(double));
		memcpy(inputs, newInputs, i * sizeof(double));
		free(newInputs);
		//inputs = newInputs;
	}
	free(inputs);
}

void Network::bprop(data *d)
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
void Network::bprop_fast(data *d)
{
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		Layer *layer = layers.at(i);
		//std::vector<double> errors;
		double * errors = (double*)malloc(layer->neurons.size() * sizeof(double));
		if (i == layers.size() - 1)
		{
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				Neuron *n = layer->neurons.at(j);
				double v = d->get_class_vector()->at(j);
				errors[j] = v - n->output;
				//errors.push_back(v - n->output);
			}
		}
		else {
			double error = 0.0;
			for (int j = 0; j < layer->neurons.size(); j++)
			{
				for (Neuron *n : layers.at(i + 1)->neurons)
				{
					error += (n->weights_arr[j] * n->delta);
				}
				errors[j] = (error);
			}
		}
		for (int j = 0; j < layer->neurons.size(); j++)
		{
			Neuron *n = layer->neurons.at(j);
			n->delta = errors[j] * n->transferDerivative(n->output);
		}
		free(errors);
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
				n->weights.at(j) += (eta * n->delta * inputs.at(j));
			}
			n->bias += n->delta * eta;
		}
	}
}

void Network::updateWeights_fast(data *data)
{
	for (int i = 0; i < layers.size(); i++)
	{
		int input_size = data->getNormalizedFeatureVector()->size();
		double * inputs;
		int totJ = 0;
		//std::vector<double> inputs;
		if (i != 0)
		{
			inputs = (double*)malloc(layers.at(i - 1)->neurons.size() *sizeof(double));
			int j = 0;
			for (Neuron *n : layers.at(i - 1)->neurons)
			{
				inputs[j++] = n->output;
				//inputs.push_back(n->output);
			}
			totJ = j;
		}
		else {
			inputs = (double*)malloc(input_size * sizeof(double));
			memcpy(inputs, data->getNormalizedFeatureArray(), input_size * sizeof(double));
			//inputs = *data->getNormalizedFeatureVector();
		}
		for (Neuron *n : layers.at(i)->neurons)
		{
			for (int j = 0; j < input_size; j++)
			{
				n->weights_arr[j] += (eta * n->delta * inputs[j]);
			}
			n->bias += n->delta * eta;
		}
		free(inputs);
	}
}

void Network::train()
{
	int pos = 0;
	int size = training_data->size();
	for (data *data : *this->training_data)
	{
		if (data->get_label() != target) continue;
		fprop(data);
		bprop(data);
		updateWeights(data);
		//data_handler::print_loading(false, pos, size);
		pos++;
	}
	//fprintf(stderr, "Training Done\n");
}
void Network::train_fast()
{
	int pos = 0;
	int size = training_data->size();
	for (data *data : *this->training_data)
	{
		//if (data->get_label() != target) continue;
		fprop_fast(data);
		bprop_fast(data);
		updateWeights_fast(data);
		//data_handler::print_loading(false, pos, size);
		pos++;
	}
	//fprintf(stderr, "Training Done\n");
}

void Network::test()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data *data : *this->test_data)
	{
		if (data->get_label() != target) continue;
		count++;
		fprop(data);
		std::vector<double> outputs;
		for (Neuron *n : layers.back()->neurons) {
			outputs.push_back(n->output);
		};
		int maxIndex = 0;
		double maxValue = outputs.at(0);
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

void Network::test_fast()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data *data : *this->test_data)
	{
		//if (data->get_label() != target) continue;
		count++;
		fprop_fast(data);
		double * outputs = (double*)malloc(layers.back()->neurons.size() * sizeof(double));
		int j = 0;
		for (Neuron *n : layers.back()->neurons) {
			outputs[j++] = n->output;
			//outputs.push_back(n->output);
		};
		int maxIndex = 0;
		double maxValue = outputs[0];
		for (int i = 1; i < layers.back()->neurons.size(); i++)
		{
			if (outputs[i] < maxValue)
			{
				maxValue = outputs[i];
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
		free(outputs);
	}

	testPerformance = (numCorrect / count) * 100;
	fprintf(stderr, "Test Performance: %.4f\n", numCorrect / count);
}

void Network::validate()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data *data : *this->validation_data)
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
	fprintf(stderr, "Validation Performance: %.4f\n", numCorrect / count);
}

void Network::validate_fast()
{
	double numCorrect = 0.0;
	double count = 0.0;
	for (data *data : *this->validation_data)
	{
		//if (data->get_label() != target) continue;
		count++;
		fprop_fast(data);
		double * outputs = (double*)malloc(layers.back()->neurons.size() * sizeof(double));
		//printf("Output Layer: ");
		int j = 0;
		for (Neuron *n : layers.back()->neurons) {
			//printf("%.4f ", n->output);
			outputs[j++] = n->output;
			//outputs.push_back(n->output);
		}
		//printf("\n");
		int minIndex = 0;
		double minValue = outputs[0];
		for (int i = 1; i < layers.back()->neurons.size(); i++)
		{
			if (outputs[i] < minValue)
			{
				minValue = outputs[i];
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
		free(outputs);
	}

	if (abs(validationPerformance - ((numCorrect * 100.0) / count)) < 1) {
		this->eta = this->eta / 2;
	}

	validationPerformance = (numCorrect * 100.0) / count;
	fprintf(stderr, "Validation Performance: %.4f\n", (numCorrect * 100.0) / count);
}

int Network::real_predict()
{
	// A map to predictions and the amout of times they were made
	std::map<int, double> predictions;

	double numCorrect = 0.0;
	double count = 0.0;
	int size = test_data->size();
	for (data *data : *this->test_data)
	{
		data_handler::print_loading(false, count, size);
		count++;
		fprop_fast(data);
		std::vector<double> outputs;
		for (Neuron *n : layers.back()->neurons) {
			outputs.push_back(n->output);
		};

		for (int i = 0; i < outputs.size(); i++) {
			if (count == 0) {
				predictions[i] = outputs.at(i);
			}
			else {
				predictions[i] += outputs.at(i);
			}
		}
	}

	printf("\r                \r");

	double total = 0;
	for (auto tpl : predictions) {
		total += tpl.second;
	}

	int top_guessed = predictions.begin()->first;
	double top_guessed_count = total;

	printf("Prediction table: \n");
	for (auto tpl : predictions) {
		printf("\t%u) %.2f%%\n", tpl.first, (float)(total- tpl.second) * 100 / total);
		if (tpl.second < top_guessed_count) {
			top_guessed = tpl.first;
			top_guessed_count = tpl.second;
		}
	}

	return top_guessed;
}

void Network::Export(char ** buffer) {
	printf("Now exporting...\n");
	unsigned long long int totalSize = 0;// = layers.at(i - 1)->neurons.size() * sizeof(double);

	totalSize += sizeof(totalSize);

	uint32_t layerSize = 0;
	uint32_t prevlayerSize = layers.at(0)->neurons.at(0)->weights.size();

	totalSize += sizeof(layers.size());

	for (int i = 0; i < layers.size(); i++)
	{
		layerSize = layers.at(i)->currentLayerSize;

		totalSize += 2 * sizeof(uint32_t); // prevlayerSize, layerSize

		for (Neuron * n : layers.at(i)->neurons) {
			totalSize += n->raw_size();
		}

		prevlayerSize = layerSize;
	}

	printf("Export size: %llu\n", totalSize);

	*buffer = (char *)calloc(totalSize, sizeof(char));

	uint32_t currentSizeIndex = 0;

	memcpy(*buffer, &totalSize, sizeof(totalSize));
	currentSizeIndex += sizeof(totalSize);

	layerSize = 0;
	prevlayerSize = layers.at(0)->neurons.at(0)->weights.size();

	uint32_t layer_sizes = layers.size();
	memcpy(*buffer + currentSizeIndex, &layer_sizes, sizeof(layer_sizes));
	currentSizeIndex += 1 * sizeof(layer_sizes); // layer_sizes

	memcpy(*buffer + currentSizeIndex, &prevlayerSize, sizeof(prevlayerSize));
	currentSizeIndex += 1 * sizeof(uint32_t); // prevlayerSize

	for (int i = 0; i < layers.size(); i++)
	{
		layerSize = layers.at(i)->currentLayerSize;
		
		memcpy(*buffer + currentSizeIndex, &layerSize, sizeof(layerSize));
		currentSizeIndex += 1 * sizeof(uint32_t); // layerSize

		for (Neuron * n : layers.at(i)->neurons) {
			currentSizeIndex += n->Export((*buffer) + currentSizeIndex);
		}

		prevlayerSize = layerSize;

		memcpy(*buffer + currentSizeIndex, &prevlayerSize, sizeof(prevlayerSize));
		currentSizeIndex += 1 * sizeof(uint32_t); // prevlayerSize
	}

	if (currentSizeIndex != totalSize) {
		printf("Total size does not match!\n");
	}
	else {
		FILE * f = fopen("Network_Output.net", "wb");
		fwrite(*buffer, sizeof(char), totalSize, f);
		fclose(f);
		printf("Successfully exported network!\n");
	}	
}

void Network::Import(char * buffer) {
	uint32_t currentSizeIndex = 0;
	unsigned long long totalSize = 0;
	memcpy(&totalSize, buffer, sizeof(totalSize));
	currentSizeIndex += sizeof(totalSize);

	uint32_t layer_sizes = 0;
	memcpy(&layer_sizes, buffer + currentSizeIndex, sizeof(layer_sizes));
	currentSizeIndex += sizeof(layer_sizes);

	uint32_t layerSize = 0;
	uint32_t prevlayerSize = 0;

	memcpy(&prevlayerSize, buffer + currentSizeIndex, sizeof(prevlayerSize));
	currentSizeIndex += 1 * sizeof(uint32_t); // prevlayerSize

	inputLayer = new InputLayer(0, prevlayerSize, true);

	for (int i = 0; i < layer_sizes; i++)
	{
		memcpy(&layerSize, buffer + currentSizeIndex, sizeof(layerSize));
		currentSizeIndex += 1 * sizeof(uint32_t); // layerSize

		Layer * layer = new Layer(prevlayerSize, layerSize, true);

		for (Neuron * n : layer->neurons) {
			n->Import(prevlayerSize, layerSize, buffer + currentSizeIndex);
			currentSizeIndex += n->raw_size();
		}

		layers.push_back(layer);

		memcpy(&prevlayerSize, buffer + currentSizeIndex, sizeof(prevlayerSize));
		currentSizeIndex += 1 * sizeof(uint32_t); // prevlayerSize
	}

	if (currentSizeIndex != totalSize) {
		printf("Import sizes do not match!\n");
	}
	else {
		printf("Successfully imported network!\n");
	}
}