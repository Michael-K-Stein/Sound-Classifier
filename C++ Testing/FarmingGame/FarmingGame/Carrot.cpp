#include "Carrot.h"

Carrot::Carrot()
{
	sf::Texture tWheat;
	tWheat.loadFromFile("graphics/Items/Inventory/Carrot.png");

	GameItem::itemTexture = tWheat;
}


Carrot::~Carrot()
{
}

int Carrot::getTicksToGrow() {
	return Carrot::ticksToGrow;
}