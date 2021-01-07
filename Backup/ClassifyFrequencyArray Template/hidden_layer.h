#ifndef __HIDDEN_LAYER_HPP
#define __HIDDEN_LAYER_HPP

#include "layer.h"

template<class T>
class HiddenLayer : public Layer<T> {
public:
	HiddenLayer(int prev, int current) : Layer(prev, current) {}
	void feedForward(Layer<T> prev);
	void backProp(Layer<T> next);
	void updateWeights(T, Layer<T> *);
};

#pragma once
#endif // !__HIDDEN_LAYER_HPP