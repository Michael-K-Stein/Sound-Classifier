#ifndef __NETWORK_HPP
#define __NETWORK_HPP

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "hidden_layer.h"
#include "input_layer.h"
#include "output_layer.h"
#include "common_data.h"
/*
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
	void bprop(data * data);
	void updateWeights(data * data);
	void train();
	void test();
	void validate();
	void threadedTrain(std::vector<data *>, int start, int end);
};
*/

class Network : public common_data
{
public:
	std::vector<Layer *> layers;
	double learningRate;
	double testPerformance;
	Network(std::vector<int> spec, int, int, double, int thread_id);
	~Network();
	std::vector<double> fprop(data *d);
	double activate(std::vector<double>, std::vector<double>); // dot product
	double transfer(double);
	double transferDerivative(double); // used for backprop
	void bprop(data *d);
	void updateWeights(data *d);
	int predict(data *d); // return the index of the maximum value in the output array.
	void train(int); // num iterations
	double test();
	void validate();
};

#pragma once
#endif