#ifndef INPUT_LAYER_HPP
#define INPUT_LAYER_HPP
#include "layer.h"
#include "data.h"
class InputLayer : public Layer {

public:
	InputLayer(int prev, int curr) : Layer(prev, curr) {}
	InputLayer(int prev, int curr, bool importing) : Layer(prev, curr, importing) {}
	void setLayerOutputs(data *d);
};
#endif