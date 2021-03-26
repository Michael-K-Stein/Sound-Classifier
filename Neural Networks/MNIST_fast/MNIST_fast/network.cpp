#include "network.h"


network::network(std::vector<int> hidden_layers_specification, int input_size, int number_of_classes, double learning_rate) {
	for (int i = 0; i < hidden_layers_specification.size(); i++)
	{
		if (i == 0)
			layers.push_back(new layer(input_size, hidden_layers_specification.at(i)));
		else
			layers.push_back(new layer(layers.at(i - 1)->neurons.size(), hidden_layers_specification.at(i)));

	}
	layers.push_back(new layer(layers.at(layers.size() - 1)->neurons.size(), number_of_classes));

	this->learning_rate = learning_rate;
}
network::~network() {

}
std::vector<double> network::fprop(data * d) {
	std::vector<double> inputs = *d->get_normalized_feature_vector();

	for (int i = 0; i < layers.size(); i++)
	{
		layer * l = layers.at(i);
		std::vector<double> new_inputs;
		int neuron_index = 0;
		for (neuron * n : l->neurons)
		{
			double activation = n->activate(inputs);
			n->output = this->transfer_activation(activation);
			new_inputs.push_back(n->output);
		}
		l->layer_outputs = new_inputs;
		inputs = new_inputs;
	}
	return inputs; // output layer outputs
}
int network::fprop_c(data * d, double ** output) { // fprop with only c
	int inputs_size = d->get_feature_array_size();
	double * inputs = (double*)malloc(inputs_size * sizeof(double));

	memcpy(inputs, d->get_normalized_feature_array(), inputs_size * sizeof(double));

	for (int layer_index = 0; layer_index < layers_size; layer_index++) {
		layer * l = (layers_array[layer_index]);
		double * new_inputs = (double *)malloc(l->neurons_size * sizeof(double));
		for (int neuron_index = 0; neuron_index < l->neurons_size; neuron_index++) {
			neuron * n = (l->neurons_array[neuron_index]);
			double activation = n->activate(inputs_size, inputs);
			n->output = this->transfer_activation(activation);
			new_inputs[neuron_index] = n->output;
		}
		inputs_size = l->neurons_size;
		memcpy(l->layer_outputs_array, new_inputs, inputs_size * sizeof(double));
		inputs = (double *)realloc(inputs, inputs_size * sizeof(double));
		memcpy(inputs, new_inputs, inputs_size * sizeof(double));
		free(new_inputs);
	}
	*output = (double *)malloc(inputs_size * sizeof(double));
	memcpy(*output, inputs, inputs_size * sizeof(double));
	free(inputs);

	return inputs_size;
}

void network::bprop(std::vector<double> deriv_errors) {
	for (int i = layers.size() - 1; i >= 0; i--)
	{
		layer * l = layers.at(i);
		std::vector<double> errors;
		if (i != layers.size() - 1)
		{
			for (int j = 0; j < l->neurons.size(); j++)
			{
				double error = 0.0;
				for (neuron *n : layers.at(i + 1)->neurons)
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
			neuron * n = l->neurons.at(j);
			n->delta = errors[j];
		}
	}
}
void network::bprop_c(double * deriv_errors) {
	double * errors;
	for (int i = layers_size - 1; i >= 0; i--)
	{
		layer * l = (layers_array[i]);
		
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
					neuron * n = (layers_array[i + 1]->neurons_array[k]);
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
			neuron * n = (l->neurons_array[j]);
			n->delta = errors[j];
		}

		if (deriv_errors != errors) {
			free(errors);
		}
	}
}
void network::update_weights(data * d) {
	std::vector<double> inputs = * d->get_normalized_feature_vector();
	for (int i = 0; i < layers.size(); i++)
	{
		if (i != 0)
		{
			for (neuron *n : layers.at(i - 1)->neurons)
			{
				inputs.push_back(n->output);
			}
		}
		for (neuron *n : layers.at(i)->neurons)
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
void network::update_weights_c(data * d) {
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
				neuron * n = (layers_array[i-1]->neurons_array[j]);
				inputs[i] = n->output;
			}
		}
		for (int j = 0; j < layers_array[i]->neurons_size; j++)
		{
			neuron * n = (layers_array[i]->neurons_array[j]);
			for (int k = 0; k < inputs_size; k++)
			{
				n->weights_array[k] += this->learning_rate * n->delta * inputs[k];
			}
			n->weights_array[inputs_size] += this->learning_rate * n->delta;
		}
	}

	free(inputs);
}

double network::train() {
	double sum_error = 0.0;
	std::vector<double> deriv_error;

	int output_vector_size = this->training_data->at(0)->get_class_vector()->size();
	for (int i = 0; i < output_vector_size; i++) { deriv_error.push_back(0.0); }

	for (data * d : *this->training_data)
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
double network::validate() {
	double num_correct = 0.0;
	double count = 0.0;
	for (data * d : *this->validation_data)
	{
		count++;
		int index = predict(d);
		if (d->get_class_vector()->at(index) == 1) { num_correct++; }
	}

	return num_correct / count;
}
double network::test() {
	double num_correct = 0.0;
	double count = 0.0;
	for (data * d : *this->test_data)
	{
		count++;
		int index = predict(d);
		if (d->get_class_vector()->at(index) == 1) { num_correct++; }
	}

	return num_correct / count;
}

double network::train_c() {
	double sum_error = 0.0;
	int output_vector_size = this->training_data->at(0)->get_class_array_size();
	double * deriv_error = (double *)calloc(output_vector_size, sizeof(double));

	for (data * d : *this->training_data)
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
	}

	free(deriv_error);

	return sum_error;
}
double network::validate_c() {
	double num_correct = 0.0;
	double count = 0.0;
	for (data * d : *this->validation_data)
	{
		count++;
		int index = predict_c(d);
		if (d->get_class_array()[index] == 1) { num_correct++; }
	}

	return num_correct / count;
}
double network::test_c() {
	double num_correct = 0.0;
	double count = 0.0;
	for (data * d : *this->test_data)
	{
		count++;
		int index = predict_c(d);
		if (d->get_class_array()[index] == 1) { num_correct++; }
	}

	return num_correct / count;
}

double network::transfer_activation(double activat) {
	return 1.0 / (1.0 + exp(-activat));
}

int network::predict(data * data) {
	std::vector<double> outputs = fprop(data);
	return std::distance(outputs.begin(), std::max_element(outputs.begin(), outputs.end()));
}
int network::predict_c(data * data) {
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

void network::c_only() {
	layers_size = layers.size();
	layers_array = (layer **)malloc(layers_size * sizeof(layer *));
	for (int i = 0; i < layers_size; i++) {
		layers_array[i] = (layers.at(i));
	}

	layers.clear();

	for (int i = 0; i < training_data->size(); i++) {
		training_data->at(i)->c_only();
	}
	for (int i = 0; i < validation_data->size(); i++) {
		validation_data->at(i)->c_only();
	}
	for (int i = 0; i < test_data->size(); i++) {
		test_data->at(i)->c_only();
	}

	for (int i = 0; i < layers_size; i++) {
		layer * l = layers_array[i];
		l->c_only();
	}

	printf("Network is now C only!\n");
}