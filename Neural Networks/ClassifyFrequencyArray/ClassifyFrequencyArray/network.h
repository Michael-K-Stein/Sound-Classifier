#ifndef __NETWORK_HPP
#define __NETWORK_HPP

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "hidden_layer.h"
#include "input_layer.h"
#include "output_layer.h"
#include "common_data.h"

class Network : public common_data {
private:
	InputLayer * inputLayer;
	//OutputLayer * outputLayer;
	std::vector<Layer *> layers;
	//std::vector<HiddenLayer *> hiddenLayers;
	double eta; // Learning rate
	int iteration = 0;


public:
	int target;
	double testPerformance;
	double validationPerformance;
	//Network(std::vector<int> hiddenLayerSpec, int, int);
	Network(std::vector<int>, int, int);
	~Network();
	Network();
	void fprop(data * data);
	void fprop_fast(data * data);
	void bprop(data * data);
	void bprop_fast(data * data);
	void updateWeights(data * data);
	void updateWeights_fast(data * data);
	void train();
	void train_fast();
	void test();
	void test_fast();
	void validate();
	void validate_fast();
	void threadedTrain(std::vector<data *>, int start, int end);

	int real_predict();

	void Export(char ** buffer);
	void Import(char * buffer);
};


#pragma once
#endif