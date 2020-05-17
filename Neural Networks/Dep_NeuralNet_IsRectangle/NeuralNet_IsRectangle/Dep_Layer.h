#pragma once
#include "Node.h"
class Layer // The layer: and array of nodes.
{
public:
	Layer();
	Layer(int Size);
	~Layer();
	void Randomize();
	Node* nodes;
	int getSize();
	std::string ToString();
};

