#pragma once
#include "GameItem.h"
class Carrot :
	public GameItem
{
public:
	Carrot();
	~Carrot();
	int getTicksToGrow();
	const static int ticksToGrow = 5000;
};

