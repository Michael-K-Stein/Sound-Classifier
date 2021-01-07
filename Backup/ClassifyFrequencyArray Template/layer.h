#ifndef __LAYER_HPP
#define __LAYER_HPP

#include "neuron.h"
#include <vector>

static int layerId = 0;

template<class T>
class Layer {
	public:
		int id;
		int currentLayerSize;
		std::vector<Neuron<T> *> neurons;
		std::vector<T> layerOutput;
		Layer(int, int);
		Layer();
		~Layer();
		std::vector<T> getLayerOutputs();
		int getSize();
};

#pragma once
#endif // !__LAYER_HPP