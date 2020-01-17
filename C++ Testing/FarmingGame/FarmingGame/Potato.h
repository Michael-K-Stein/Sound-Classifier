#pragma once
#include "GameItem.h"
class Potato :
	public GameItem
{
public:
	Potato();
	~Potato();
	int getTicksToGrow();
	const static int ticksToGrow = 8000;
};