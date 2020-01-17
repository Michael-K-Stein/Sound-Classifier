#include "Barn.h"



Barn::Barn(int x, int y)
{
	sf::Texture tBarn;
	tBarn.loadFromFile("graphics/Items/Barn.png");

	GameItem::itemTexture = tBarn;
	GameItem::Location.x = x;
	GameItem::Location.y = y;
	GameItem::isInInventory = false;
}


Barn::~Barn()
{
}
