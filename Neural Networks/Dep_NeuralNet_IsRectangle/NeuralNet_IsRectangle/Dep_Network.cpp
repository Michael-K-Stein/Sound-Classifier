#include "pch.h"
#include "Network.h"


Network::Network()
{
}


Network::~Network()
{
}

int Network::getDepth() {
	return (sizeof(Network::layerConnections) / sizeof(Network::layerConnections[0]));
}

void Network::AddLayer(Layer inpLayer) {
	Network::layers[Network::getDepth() + 2] = inpLayer;
}

void Network::Randomize() {
	for (int i = 0; i < Network::getDepth() + 1; i++) {
		Network::layers[i].Randomize();
	}
	for (int i = 0; i < Network::getDepth(); i++) {
		Network::layerConnections[i].Randomize();
	}
}

float Sigmoid(float x) {
	float k = 2; // Slope multiplier
	return (float)(1.0 / (1.0 + pow(2.718, -2.0 * k * x)));
}

float Network::ActivationFunction(float inp) {
	return Sigmoid(inp);
}

Layer Network::Calculate(Layer inps) {
	int depth = Network::getDepth();

	for (int layerInd = 0; layerInd < depth; layerInd++) { // For each layer
		for (int targetInd = 0; targetInd < Network::layers[layerInd + 1].getSize(); targetInd++) {
			float targetTot = 0; // Before activation function
			for (int preInd = 0; preInd < Network::layers[layerInd].getSize(); preInd++) {
				targetTot += Network::layers[layerInd].nodes[preInd].weight * Network::layerConnections[layerInd].connections[preInd][targetInd];
			}
			Network::layers[layerInd + 1].nodes[targetInd].weight = Network::ActivationFunction(targetTot);
		}
	}

	return Network::layers[depth + 1];
}