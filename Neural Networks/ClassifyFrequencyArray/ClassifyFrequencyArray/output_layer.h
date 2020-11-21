#ifndef __OUTPUT_LAYER_HPP
#define __OUTPUT_LAYER_HPP

#include "layer.h"
#include "data.h"

class OutputLayer : public Layer {
public:
	OutputLayer(int prev, int current) : Layer(prev, current) {}
	void feedForward(Layer);
	void backProp(data *data);
	void updateWeights(double, Layer *);
};

#pragma once
#endif // !__OUTPUT_LAYER_HPP
