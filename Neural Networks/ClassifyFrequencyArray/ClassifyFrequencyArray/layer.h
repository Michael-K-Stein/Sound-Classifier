#ifndef __LAYER_HPP
#define __LAYER_HPP

#include "neuron.h"
#include <vector>

static int layerId = 0;

class Layer {
	public:
		int id;
		int currentLayerSize;
		std::vector<Neuron *> neurons;
		std::vector<double> layerOutput;
		Layer(int, int);
		Layer(int, int, bool);
		Layer();
		~Layer();
		std::vector<double> getLayerOutputs();
		int getSize();
};

#pragma once
#endif // !__LAYER_HPP