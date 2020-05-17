#pragma once
#include "Layer.h"
#include "LayerConnections.h"
#include <math.h>

class Network
{
public:
	Network();
	~Network();
	void AddLayer(Layer inpLayer);
	void Randomize();
	Layer* layers;
	LayerConnections* layerConnections;
	int getDepth();
	float ActivationFunction(float inp);

	Layer Calculate(Layer inps);
};

