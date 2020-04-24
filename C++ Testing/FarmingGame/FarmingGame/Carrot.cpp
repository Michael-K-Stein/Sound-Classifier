#include "Carrot.h"

Carrot::Carrot()
{
	sf::Texture tCarrot;
	tCarrot.loadFromFile("graphics/Items/Inventory/Carrot.png");

	GameItem::itemTexture = tCarrot;
}


Carrot::~Carrot()
{
}

int Carrot::getTicksToGrow() {
	return Carrot::ticksToGrow;
}