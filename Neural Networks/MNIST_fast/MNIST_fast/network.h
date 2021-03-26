#pragma once

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "common_data.h"
#include "data_handler.h"

#include <numeric>
#include <algorithm>
#include <thread>

class network : public common_data
{
public:
	std::vector<layer *> layers;
	layer ** layers_array;
	int layers_size;
	double learning_rate;
	double test_performance;

	network(std::vector<int> hidden_layers_specification, int, int, double); // ctor
	~network(); // dtor

	void c_only();

	std::vector<double> fprop(data * d); // forward propagation
	void bprop(std::vector<double> deriv_errors); // back propagation
	void update_weights(data * data); // update weights after bprop

	int fprop_c(data * d, double ** output); // forward propagation - c | returns class count
	void bprop_c(double * deriv_errors); // back propagation - c
	void update_weights_c(data * data); // update weights after bprop - c

	double train(); // returns error
	double validate(); // return percentage correct
	double test(); // return percentage correct

	double train_c(); // returns error - c
	double validate_c(); // return percentage correct - c
	double test_c(); // return percentage correct - c

	double transfer_activation(double activat);

	int predict(data * data);
	int predict_c(data * data);

	int export_network(char ** buffer); // returns the size of buffer
};

