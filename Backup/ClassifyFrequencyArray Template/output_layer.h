#ifndef __OUTPUT_LAYER_HPP
#define __OUTPUT_LAYER_HPP

#include "layer.h"
#include "data.h"

template<class T>
class OutputLayer : public Layer<T> {
public:
	OutputLayer(int prev, int current) : Layer<T>(prev, current) {}
	void feedForward(Layer<T>);
	void backProp(data<T> *data);
	void updateWeights(T, Layer<T> *);
};

#pragma once
#endif // !__OUTPUT_LAYER_HPP
