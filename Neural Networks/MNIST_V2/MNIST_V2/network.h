#pragma once

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "data.h"
#include "neuron.h"
#include "layer.h"
#include "common.h"

class Network : public CommonData
{
public:
	std::vector<Layer *> layers;
	double learningRate;
	double testPerformance;
	Network(std::vector<int> spec, int, int, double);
	~Network();
	std::vector<double> fprop(Data *data);
	double activate(std::vector<double>, std::vector<double>); // dot product
	double transfer(double);
	//double transferDerivative(double); // used for backprop
	double transferDerivative(std::vector<double>,std::vector<double>); // used for backprop
	std::vector<double> CalculateDerivative(Data * data, Layer *);
	std::vector<double> CalculateDerivative(Layer *, Layer *);
	void bprop(Data *data);
	void bprop(std::vector<double> deriv_errors);
	void updateWeights(Data *data);
	int predict(Data *data); // return the index of the maximum value in the output array.
	void train(int, int); // num iterations, class to learn
	double test();
	void validate();
};

#endif