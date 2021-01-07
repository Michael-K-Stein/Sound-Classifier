#ifndef __NEURON_HPP
#define __NEURON_HPP
#pragma once

#include <cmath>
#include <vector>
#include <stdio.h>

template<class T>
class Neuron {
public:
	std::vector<T> weights;
	T bias;
	T delta;
	T output;
	T activation;
	Neuron(int, int);

	void initializeWeights(int, int);
	T activate(std::vector<T>);
	T transfer(T value);
	T transferDerivative(T value);

};

#endif // !__NEURON_HPP
