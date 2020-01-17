#include "Potato.h"



Potato::Potato()
{
	sf::Texture tPotato;
	tPotato.loadFromFile("graphics/Items/Inventory/Potato.png");

	GameItem::itemTexture = tPotato;
}


Potato::~Potato()
{
}

int Potato::getTicksToGrow() {
	return Potato::ticksToGrow;
}