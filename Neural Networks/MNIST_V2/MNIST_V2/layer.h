#pragma once

#ifndef __LAYER_HPP
#define __LAYER_HPP
#include "neuron.h"
#include <stdint.h>
#include <vector>

class Layer {

public:
	int currentLayerSize;
	std::vector<Neuron *> neurons;
	std::vector<double> layerOutputs;
	Layer(int, int);
};
#endif