#include "Farmland.h"

Farmland::Farmland(int x, int y)
{
	sf::Texture tFarmland;
	tFarmland.loadFromFile("graphics/Items/Farmland.png");

	GameItem::itemTexture = tFarmland;
	GameItem::Location.x = x;
	GameItem::Location.y = y;
	GameItem::isInInventory = false;

	cropAmount = 0;
}


Farmland::~Farmland()
{
}

bool Farmland::CheckClicked(sf::RenderWindow& gameWindow) {
	sf::Mouse mouse;
	return (
		mouse.getPosition(gameWindow).x > GameItem::Location.x &&
		mouse.getPosition(gameWindow).x < GameItem::Location.x + 200 &&
		mouse.getPosition(gameWindow).y > GameItem::Location.y &&
		mouse.getPosition(gameWindow).y < GameItem::Location.y + 200
		);
}

void Farmland::SetObject(GameItem itemType) {
	Farmland::objectTexture = itemType.itemTexture;
	Farmland::cropAmount = 1;
	Farmland::ticksToGrow = itemType.getTicksToGrow();
	Farmland::itemType = itemType;
}

void Farmland::Draw(sf::RenderWindow& rW) {
	sf::Sprite sObject(GameItem::itemTexture);
	sObject.setPosition(Location.x, Location.y);
	rW.draw(sObject);
	sf::Sprite sObject2(Farmland::objectTexture);
	sObject2.setRotation(45);
	if (Farmland::cropAmount > 0) {
		sObject2.setPosition(Location.x + 100, Location.y);
		rW.draw(sObject2);
		if (Farmland::cropAmount > 1) {
			sObject2.setPosition(Location.x + 50, Location.y + 50);
			rW.draw(sObject2);
			if (Farmland::cropAmount > 2) {
				sObject2.setPosition(Location.x + 100, Location.y + 100);
				rW.draw(sObject2);
				if (Farmland::cropAmount > 3) {
					sObject2.setPosition(Location.x + 150, Location.y + 50);
					rW.draw(sObject2);
				}
			}
		}
	}
}

bool Farmland::isFull() {
	return (Farmland::cropAmount > 0);
}

void Farmland::SetTicksToGrow(int inpTicks) {
	Farmland::ticksToGrow = inpTicks;
}

void Farmland::Grow() {
	if (Farmland::cropAmount > 0) {
		Farmland::ticksPast++;
		if (Farmland::ticksPast >= Farmland::ticksToGrow) {
			Farmland::cropAmount++;
			Farmland::ticksPast = 0;
		}
	}
}

int Farmland::getCropAmount() {
	return Farmland::cropAmount;
}

void Farmland::CropsRemoved() {
	Farmland::cropAmount = 0;
	Farmland::CoolOffTime = 10;
}