#include "network.h"

Network::~Network() {}
Network::Network(std::vector<int> hidden_layers_specification, int input_size, int number_of_classes, double learning_rate) {
	for (int i = 0; i < hidden_layers_specification.size(); i++)
	{
		if (i == 0)
			layers.push_back(new Layer(input_size, hidden_layers_specification.at(i)));
		else
			layers.push_back(new Layer(layers.at(i - 1)->neurons.size(), hidden_layers_specification.at(i)));

	}
	layers.push_back(new Layer(layers.at(layers.size() - 1)->neurons.size(), number_of_classes));

	this->learning_rate = learning_rate;
}
Network::Network(double learning_rate) {
	this->learning_rate = learning_rate;
}
std::vector<double> Network::fprop(Data * d) {
	std::vector<double> inputs = *d->get_normalized_feature_vector();

	for (int i = 0; i < layers.size(); i++)
	{
		Layer * l = layers.at(i);
		std::vector<double> new_inputs;
		int neuron_index = 0;
		for (Neuron * n : l->neurons)
		{
			double activation = n->activate(inputs);
			n->output = this->transfer_activation(activation);
			new_inputs.push_back(n->output);
		}
		//l->layer_outputs = new_inputs;
		inputs = new_inputs;
	}
	return inputs; // output layer outputs
}
int Network::fprop_c(Data * d, double ** output) { // fprop with only c
	int inputs_size = d->get_feature_array_size();
	double * inputs = (double*)malloc(inputs_size * sizeof(double));

	memcpy(inputs, d->get_normalized_feature_array(), inputs_size * sizeof(double));

	for (int layer_index = 0; layer_index < layers_size; layer_index++) {
		Layer * l = (layers_array[layer_index]);
		double * new_inputs = (double *)malloc(l->neurons_size * sizeof(double));
		for (int neuron_index = 0; neuron_index < l->neurons_size; neuron_index++) {
			Neuron * n = (l->neurons_array[neuron_index]);
			double activation = n->activate(inputs_size, inputs);
			n->output = this->transfer_activation(activation);
			new_inputs[neuron_index] = n->output;
		}
		inputs_size = l->neurons_size;
		//memcpy(l->layer_outputs_array, new_inputs, inputs_size * sizeof(double));
		inputs = (double *)realloc(inputs, inputs_size * sizeof(double));
		memcpy(inputs, new_inputs, inputs_size * sizeof(double));
		free(new_inputs);
	}
	*output = (double *)malloc(inputs_size * sizeof(double));
	memcpy(*output, inputs, inputs_size * sizeof(double));
	free(inputs);

	return inputs_size;
}

void Network::bprop(std::vector<double> deriv_errors) {
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		Layer * l = layers.at(i);
		std::vector<double> errors;
		if (i != layers.size() - 1)
		{
			for (int j = 0; j < l->neurons.size(); j++)
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
			errors = deriv_errors;
		}

		for (int j = 0; j < l->neurons.size(); j++)
		{
			Neuron * n = l->neurons.at(j);
			n->delta = errors[j];
		}
	}
}
void Network::bprop_c(double * deriv_errors) {
	double * errors;
	for (int i = layers_size - 1; i >= 0; i--)
	{
		Layer * l = (layers_array[i]);

		if (i != layers_size - 1)
		{
			errors = (double *)malloc(l->neurons_size * sizeof(double));
			for (int j = 0; j < l->neurons_size; j++)
			{
				/// for each neuron in current layer.

				double error = 0.0;
				for (int k = 0; k < layers_array[i + 1]->neurons_size; k++)
				{
					/// for each neuron in next layer
					Neuron * n = (layers_array[i + 1]->neurons_array[k]);
					error += (n->weights_array[j] * n->delta);
				}
				errors[j] = error;
			}
		}
		else {
			// If is last layer (i == layers.size()-1)
			errors = deriv_errors;
		}

		for (int j = 0; j < l->neurons_size; j++)
		{
			Neuron * n = (l->neurons_array[j]);
			n->delta = errors[j];
		}

		if (deriv_errors != errors) {
			free(errors);
		}
	}
}
void Network::update_weights(Data * d) {
	std::vector<double> inputs = *d->get_normalized_feature_vector();
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
				n->weights.at(j) += this->learning_rate * n->delta * inputs.at(j);
			}
			n->weights.back() += this->learning_rate * n->delta;
		}
		inputs.clear();
	}
}
void Network::update_weights_c(Data * d) {
	int inputs_size = d->get_feature_array_size();
	double * inputs = (double *)malloc(inputs_size * sizeof(double));
	memcpy(inputs, d->get_normalized_feature_array(), inputs_size * sizeof(double));

	for (int i = 0; i < layers_size; i++)
	{
		if (i != 0)
		{
			inputs_size = layers_array[i - 1]->neurons_size;
			inputs = (double *)realloc(inputs, inputs_size * sizeof(double));

			for (int j = 0; j < layers_array[i - 1]->neurons_size; j++)
			{
				Neuron * n = (layers_array[i - 1]->neurons_array[j]);
				inputs[i] = n->output;
			}
		}
		for (int j = 0; j < layers_array[i]->neurons_size; j++)
		{
			Neuron * n = (layers_array[i]->neurons_array[j]);
			for (int k = 0; k < inputs_size; k++)
			{
				n->weights_array[k] += this->learning_rate * n->delta * inputs[k];
			}
			n->weights_array[inputs_size] += this->learning_rate * n->delta;
		}
	}

	free(inputs);
}

double Network::train() {
	double sum_error = 0.0;
	std::vector<double> deriv_error;

	int output_vector_size = this->training_data->at(0)->get_class_vector()->size();
	for (int i = 0; i < output_vector_size; i++) { deriv_error.push_back(0.0); }

	for (Data * d : *this->training_data)
	{
		std::vector<double> outputs = fprop(d);
		std::vector<int> expected = *(d->get_class_vector());
		double tmp_sum_error = 0.0;
		for (int j = 0; j < outputs.size(); j++)
		{
			tmp_sum_error += pow((double)expected.at(j) - outputs.at(j), 2);
			deriv_error[j] += (double)expected.at(j) - outputs.at(j);
		}
		sum_error += tmp_sum_error;

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error.at(j) /= this->training_data->size();
			deriv_error.at(j) *= 2;
		}

		bprop(deriv_error);
		update_weights(d);

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error.at(j) = 0;
		}
	}

	return sum_error;
}
double Network::validate() {
	double num_correct = 0.0;
	double count = 0.0;
	for (Data * d : *this->validation_data)
	{
		count++;
		int index = predict(d);
		if (d->get_class_vector()->at(index) == 1) { num_correct++; }
	}

	return num_correct / count;
}
double Network::test() {
	double num_correct = 0.0;
	double count = 0.0;
	for (Data * d : *this->test_data)
	{
		count++;
		int index = predict(d);
		if (d->get_class_vector()->at(index) == 1) { num_correct++; }
	}

	return num_correct / count;
}

double Network::train_c() {
	double sum_error = 0.0;
	int output_vector_size = this->training_data->at(0)->get_class_array_size();
	double * deriv_error = (double *)calloc(output_vector_size, sizeof(double));

	for (Data * d : *this->training_data)
	{
		double * outputs;
		fprop_c(d, &outputs);

		int * expected = d->get_class_array();
		double tmp_sum_error = 0.0;
		for (int j = 0; j < output_vector_size; j++)
		{
			double exp = (double)expected[j];
			double out = outputs[j];
			tmp_sum_error += pow(exp - out, 2);
			deriv_error[j] += exp - out;
		}
		sum_error += tmp_sum_error;

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error[j] /= this->training_data->size();
			deriv_error[j] *= 2;
		}

		bprop_c(deriv_error);
		update_weights_c(d);

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error[j] = 0;
		}

		free(outputs);
	}

	free(deriv_error);

	return sum_error;
}

double Network::train_c(int target) {
	double sum_error = 0.0;
	int output_vector_size = this->training_data->at(0)->get_class_array_size();
	double * deriv_error = (double *)calloc(output_vector_size, sizeof(double));

	for (Data * d : *this->training_data)
	{
		if (d->get_label() != target) { continue; } // Skip anything which does not match the target label


		double * outputs;
		fprop_c(d, &outputs);

		int * expected = d->get_class_array();
		double tmp_sum_error = 0.0;
		for (int j = 0; j < output_vector_size; j++)
		{
			double exp = (double)expected[j];
			double out = outputs[j];
			tmp_sum_error += pow(exp - out, 2);
			deriv_error[j] += exp - out;
		}
		sum_error += tmp_sum_error;

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error[j] /= this->training_data->size();
			deriv_error[j] *= 2;
		}

		bprop_c(deriv_error);
		update_weights_c(d);

		for (int j = 0; j < output_vector_size; j++)
		{
			deriv_error[j] = 0;
		}

		free(outputs);
	}

	free(deriv_error);

	return sum_error;
}

double Network::validate_c() {
	double num_correct = 0.0;
	double count = 0.0;
	for (Data * d : *this->validation_data)
	{
		count++;
		int index = predict_c(d);
		if (d->get_class_array()[index] == 1) { num_correct++; }
	}

	return num_correct / count;
}
double Network::test_c() {
	double num_correct = 0.0;
	double count = 0.0;
	for (Data * d : *this->test_data)
	{
		count++;
		int index = predict_c(d);
		if (d->get_class_array()[index] == 1) { num_correct++; }
	}

	test_performance = (num_correct / count);

	return num_correct / count;
}

double Network::transfer_activation(double activat) {
	return 1.0 / (1.0 + exp(-activat));
}

int Network::predict(Data * data) {
	std::vector<double> outputs = fprop(data);
	return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}
int Network::predict_c(Data * data) {
	double * outputs;
	int class_count = fprop_c(data, &outputs);

	int max_ind = 0;
	double max = 0;
	for (int i = class_count - 1; i >= 0; i--) {
		if (outputs[i] > max) { max_ind = i; max = outputs[i]; }
	}

	free(outputs);

	return max_ind;
}

int Network::real_predict()
{
	// A map to predictions and the amout of times they were made
	std::map<int, double> predictions;

	double numCorrect = 0.0;
	double count = 0.0;
	int size = prediction_data->size();
	for (Data *data : *this->prediction_data)
	{
		Data_Handler::print_loading(false, count, size);
		count++;
		double * outputs;
		int outputs_size = fprop_c(data, &outputs);

		for (int i = 0; i < outputs_size; i++) {
			if (count == 0) {
				predictions[i] = outputs[i];
			}
			else {
				predictions[i] += outputs[i];
			}
		}
	}

	printf("\r                \r");

	double total = 0;
	for (auto tpl : predictions) {
		total += tpl.second;
	}

	int top_guessed = predictions.begin()->first;
	double top_guessed_count = predictions.begin()->second;

	printf("Prediction table: \n");
	for (auto tpl : predictions) {
		printf("\t%u) %.2f%%\n", tpl.first, (float)(tpl.second * 100.0 / total));
		if (tpl.second > top_guessed_count) {
			top_guessed = tpl.first;
			top_guessed_count = tpl.second;
		}
	}

	return top_guessed;
}

void Network::c_only() {
	layers_size = layers.size();
	layers_array = (Layer **)malloc(layers_size * sizeof(Layer *));
	for (int i = 0; i < layers_size; i++) {
		layers_array[i] = (layers.at(i));
	}

	layers.clear();

	for (int i = 0; i < layers_size; i++) {
		Layer * l = layers_array[i];
		l->c_only();
	}

	printf("Network is now C only!\n");
}

int Network::export_network(char ** buffer) {
	printf("Now exporting...\n");
	unsigned long long int total_size = 0;

	total_size += sizeof(total_size);

	total_size += sizeof(double); // network_accuracy

	uint32_t layer_size = 0;
	uint32_t prev_layer_size = layers_array[0]->neurons_array[0]->weights_array_size-1;

	total_size += sizeof(layers_size);

	total_size += sizeof(prev_layer_size);

	for (int i = 0; i < layers_size; i++)
	{
		layer_size = layers_array[i]->current_layer_size;

		total_size += 1 * sizeof(layer_size); // layer_size

		for (int j = 0; j < layers_array[i]->neurons_size; j++) {
			Neuron * n = layers_array[i]->neurons_array[j];
			total_size += n->raw_size();
		}

		prev_layer_size = layer_size;

		total_size += 1 * sizeof(prev_layer_size); // prev_layer_size
	}

	printf("Export size: %llu\n", total_size);

	*buffer = (char *)calloc(total_size, sizeof(char));



	uint32_t current_size_index = 0;

	memcpy(*buffer, &total_size, sizeof(total_size));
	current_size_index += sizeof(total_size);

	double network_accuracy = this->test_performance;
	memcpy(*buffer + current_size_index, &network_accuracy, sizeof(network_accuracy));
	current_size_index += sizeof(network_accuracy);

	layer_size = 0;
	prev_layer_size = layers_array[0]->neurons_array[0]->weights_array_size - 1;

	uint32_t layer_sizes = layers_size;
	memcpy(*buffer + current_size_index, &layer_sizes, sizeof(layer_sizes));
	current_size_index += 1 * sizeof(layer_sizes); // layer_sizes

	memcpy(*buffer + current_size_index, &prev_layer_size, sizeof(prev_layer_size));
	current_size_index += 1 * sizeof(uint32_t); // prevlayerSize

	for (int i = 0; i < layers_size; i++)
	{
		layer_size = layers_array[i]->current_layer_size;

		memcpy(*buffer + current_size_index, &layer_size, sizeof(layer_size));
		current_size_index += 1 * sizeof(uint32_t); // layerSize

		for (int j = 0; j < layers_array[i]->neurons_size; j++) {
			Neuron * n = layers_array[i]->neurons_array[j];
			current_size_index += n->export_neuron((*buffer) + current_size_index);
		}

		prev_layer_size = layer_size;

		memcpy(*buffer + current_size_index, &prev_layer_size, sizeof(prev_layer_size));
		current_size_index += 1 * sizeof(uint32_t); // prevlayerSize
	}

	if (current_size_index != total_size) {
		printf("Total size does not match!\n");
	}
	else {
		/*FILE * f;
		errno_t err = fopen_s(&f, "Network_Output.net", "wb");
		fwrite(*buffer, sizeof(char), total_size, f);
		fclose(f);*/
		printf("Successfully exported network to buffer!\n");
		return total_size;
	}
}

void Network::import_network(char * buffer) {
	uint32_t current_size_index = 0;
	unsigned long long total_size = 0;
	memcpy(&total_size, buffer, sizeof(total_size));
	current_size_index += sizeof(total_size);

	double network_accuracy = 0;
	memcpy(&network_accuracy, buffer + current_size_index, sizeof(network_accuracy));
	current_size_index += sizeof(network_accuracy);

	uint32_t layer_sizes = 0;
	memcpy(&layer_sizes, buffer + current_size_index, sizeof(layer_sizes));
	current_size_index += sizeof(layer_sizes);

	uint32_t layer_size = 0;
	uint32_t prev_layer_size = 0;

	memcpy(&prev_layer_size, buffer + current_size_index, sizeof(prev_layer_size));
	current_size_index += 1 * sizeof(uint32_t); // prev_layer_size

	//inputLayer = new InputLayer(0, prev_layer_size, true);

	if (layers_array != NULL) { free(layers_array); }
	layers_size = layer_sizes; // netw->layers_size := layer_size
	layers_array = (Layer**)malloc(layers_size * sizeof(Layer*));

	for (int i = 0; i < layer_sizes; i++)
	{
		memcpy(&layer_size, buffer + current_size_index, sizeof(layer_size));
		current_size_index += 1 * sizeof(uint32_t); // layerSize

		Layer * layer = new Layer(prev_layer_size, layer_size, true);

		for (int j = 0; j < layer->neurons_size; j++) {
			Neuron * n = layer->neurons_array[j];
			n->import_neuron(prev_layer_size, layer_size, buffer + current_size_index);
			current_size_index += n->raw_size();
		}

		layers_array[i] = layer;

		memcpy(&prev_layer_size, buffer + current_size_index, sizeof(prev_layer_size));
		current_size_index += 1 * sizeof(uint32_t); // prev_layer_size
	}

	if (current_size_index != total_size) {
		printf("Import sizes do not match!\n");
	}
	else {
		printf("Successfully imported network with accuracy: %.4f!\n", network_accuracy);
	}
}