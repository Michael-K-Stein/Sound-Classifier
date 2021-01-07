#include "input_layer.h"

template<class T>
void InputLayer<T>::setLayerOutputs(data<T> *d)
{
	this->layerOutput = *d->getNormalizedFeatureVector();
}