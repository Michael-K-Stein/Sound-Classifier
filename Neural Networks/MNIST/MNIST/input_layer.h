#ifndef INPUT_LAYER_HPP
#define INPUT_LAYER_HPP
#include "layer.h"
#include "data.h"
class InputLayer : public Layer {

public:
	InputLayer(int prev, int curr, int thread_id) : Layer(prev, curr, thread_id) {}
	void setLayerOutputs(data *d);
};
#endif