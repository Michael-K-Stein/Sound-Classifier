#include "input_layer.h"

void InputLayer::setLayerOutputs(data *d)
{
	this->layerOutput = *d->getNormalizedFeatureVector();
}