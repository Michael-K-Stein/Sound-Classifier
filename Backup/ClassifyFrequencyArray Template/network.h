#ifndef __NETWORK_HPP
#define __NETWORK_HPP

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "hidden_layer.h"
#include "input_layer.h"
#include "output_layer.h"
#include "common_data.h"

template<class T>
class Network : public common_data<T> {
private:
	InputLayer<T> * inputLayer;
	//OutputLayer * outputLayer;
	std::vector<Layer<T> *> layers;
	//std::vector<HiddenLayer *> hiddenLayers;
	double eta; // Learning rate
	int iteration = 0;


public:
	int target;
	double testPerformance;
	//Network(std::vector<int> hiddenLayerSpec, int, int);
	Network(std::vector<int>, int, int);
	~Network();
	void fprop(data<T> * data);
	void bprop(data<T> * data);
	void updateWeights(data<T> * data);
	void train();
	void test();
	void validate();
	void threadedTrain(std::vector<data<T> *>, int start, int end);
};


#pragma once
#endif