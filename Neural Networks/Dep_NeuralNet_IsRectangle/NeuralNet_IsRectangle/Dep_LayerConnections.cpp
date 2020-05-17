#include "pch.h"
#include "LayerConnections.h"


LayerConnections::LayerConnections()
{
}


LayerConnections::~LayerConnections()
{
}

void LayerConnections::Randomize() {
	for (int i = 0; i < LayerConnections::prevLayer.getSize(); i++) {
		for (int j = 0; j < LayerConnections::nextLayer.getSize(); j++) {
			connections[i][j] = (rand() % 3) - 1;
		}
	}
}