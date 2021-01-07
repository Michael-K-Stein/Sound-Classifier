#ifndef INPUT_LAYER_HPP
#define INPUT_LAYER_HPP
#include "layer.h"
#include "data.h"
template<class T>
class InputLayer : public Layer<T> {

public:
	InputLayer(int prev, int curr) : Layer(prev, curr) {}
	void setLayerOutputs(data<T> *d);
};
#endif