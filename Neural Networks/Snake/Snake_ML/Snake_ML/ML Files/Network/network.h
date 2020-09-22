#ifndef __NETWORK_HPP
#define __NETWORK_HPP

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "hidden_layer.h"
#include "input_layer.h"
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
	Network(std::vector<int> hiddenLayerSpec, int, int);
	~Network();
	void fprop(data * data);
	void bprop(data * data, std::vector<double> * out);
	void updateWeights(data * data);
	void train();
	void test();
	void validate();
	void threadedTrain(std::vector<data *>, int start, int end);

	std::vector<double> Proccess(data * data);

};


#pragma once
#endif