#include "layer.h"

Layer::Layer(int previousLayerSize, int currentLayerSize)
{
	for (int i = 0; i < currentLayerSize; i++)
	{
		neurons.push_back(new Neuron(previousLayerSize, currentLayerSize));
	}
	this->currentLayerSize = currentLayerSize;
}