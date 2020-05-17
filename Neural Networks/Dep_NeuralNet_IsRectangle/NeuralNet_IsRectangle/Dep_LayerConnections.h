#pragma once
#include "Layer.h"
class LayerConnections
{
	// Stores a matrix(2D array) with the connection weights between two layers
public:
	LayerConnections();
	~LayerConnections();
	void Randomize();
	float** connections; // connections[I][J]: the weight of the connection between node I in layer A, to node J in layer B.
private:
	Layer prevLayer;
	Layer nextLayer;
};

