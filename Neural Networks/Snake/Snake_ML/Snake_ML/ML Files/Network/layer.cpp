#include "layer.h"

Layer::Layer(int previousLayerSize, int currentLayerSize)
{
	for (int i = 0; i < currentLayerSize; i++)
	{
		neurons.push_back(new Neuron(previousLayerSize, currentLayerSize));
		this->layerOutput.push_back(0.0);
	}
	this->currentLayerSize = currentLayerSize;
	this->id = ++layerId;
}

Layer::~Layer()
{

}

Layer::Layer()
{

}