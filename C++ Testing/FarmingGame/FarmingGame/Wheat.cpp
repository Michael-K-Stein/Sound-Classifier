#include "Wheat.h"

Wheat::Wheat()
{
	sf::Texture tWheat;
	tWheat.loadFromFile("graphics/Items/Inventory/Wheat.png");
	
	GameItem::itemTexture = tWheat;
}


Wheat::~Wheat()
{
}

int Wheat::getTicksToGrow() {
	return Wheat::ticksToGrow;
}