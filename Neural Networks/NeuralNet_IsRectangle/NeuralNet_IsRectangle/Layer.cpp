#include "pch.h"
#include "Layer.h"


Layer::Layer()
{
}

Layer::Layer(int Size) {
	Layer::nodes = new Node[Size];
	for (int i = 0; i < Size; i++) {
		Layer::nodes[i] = Node();
	}
}

Layer::~Layer()
{
}

int Layer::getSize() {
	return ( sizeof(Layer::nodes) / sizeof(Layer::nodes[0]) );
}

void Layer::Randomize() {
	for (int i = 0; i < Layer::getSize(); i++) {
		Layer::nodes[i].Randomize();
	}
}

std::string Layer::ToString() {
	std::string tot;
	for (int i = 0; i < Layer::getSize()-1; i++) {
		tot += Layer::nodes[i].weight + ',' + ' ';
	}
	tot += Layer::nodes[Layer::getSize() - 1].weight;

	return tot;
}