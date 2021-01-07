#include "layer.h"
template<class T>
Layer<T>::Layer(int previousLayerSize, int currentLayerSize)
{
	for (int i = 0; i < currentLayerSize; i++)
	{
		neurons.push_back(new Neuron<T>(previousLayerSize, currentLayerSize));
		this->layerOutput.push_back(0.0);
	}
	this->currentLayerSize = currentLayerSize;
	this->id = ++layerId;
}
template<class T>
Layer<T>::~Layer()
{

}
template<class T>
Layer<T>::Layer()
{

}