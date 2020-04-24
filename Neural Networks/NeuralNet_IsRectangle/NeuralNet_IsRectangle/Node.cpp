#include "pch.h"
#include "Node.h"


Node::Node()
{
}


Node::~Node()
{
}

void Node::Randomize() {
	Node::weight = rand() % 3 - 1;
}