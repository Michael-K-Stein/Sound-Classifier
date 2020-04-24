#pragma once
#include "GameItem.h"
class Wheat :
	public GameItem
{
public:
	Wheat();
	~Wheat();
	int getTicksToGrow();
	const static int ticksToGrow = 2000;
private:
};