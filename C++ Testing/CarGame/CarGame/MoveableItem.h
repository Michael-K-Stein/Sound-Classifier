#pragma once
#include "DrawableItem.h"
class MoveableItem :
	public DrawableItem
{
public:
	std::string Name;
	MoveableItem();
	MoveableItem(std::string TexturePath);
	~MoveableItem();
	void MoveForward(float dist);
};

