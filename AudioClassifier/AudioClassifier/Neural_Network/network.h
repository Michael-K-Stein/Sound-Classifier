#pragma once

#include "Data/data.h"
#include "Data/data_handler.h"
#include "Data/common_data.h"
#include "neuron.h"
#include "layer.h"

#include <numeric>
#include <algorithm>
#include <thread>

class Network : public Common_data
{
public:
	std::vector<Layer *> layers;
	Layer ** layers_array;
	int layers_size;
	double learning_rate;
	double test_performance;

	Network(double); // ctor for import
	Network(std::vector<int> hidden_layers_specification, int, int, double); // ctor
	~Network(); // dtor

	void c_only();

	std::vector<double> fprop(Data * d); // forward propagation
	void bprop(std::vector<double> deriv_errors); // back propagation
	void update_weights(Data * data); // update weights after bprop

	int fprop_c(Data * d, double ** output); // forward propagation - c | returns class count
	void bprop_c(double * deriv_errors); // back propagation - c
	void update_weights_c(Data * data); // update weights after bprop - c

	double train(); // returns error
	double validate(); // return percentage correct
	double test(); // return percentage correct

	double train_c(); // returns error - c
	double train_c(int target); // Train only datasamples with this label | returns error - c
	double validate_c(); // return percentage correct - c
	double test_c(); // return percentage correct - c

	double transfer_activation(double activat);

	int predict(Data * data);
	int predict_c(Data * data);

	int real_predict();

	int export_network(char ** buffer); // returns the size of buffer
	void import_network(char * buffer);
};