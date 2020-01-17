#include "GameItem.h"

GameItem::GameItem() { }

/*GameItem::GameItem(sf::Texture inpTexture)
{
	itemTexture = inpTexture;
}*/


GameItem::~GameItem()
{
}

void GameItem::Draw(sf::RenderWindow& rW, int ind) {
	sf::Sprite sObject(GameItem::itemTexture);
	if (isInInventory) {
		sObject.setPosition(Game::InventoryX + 3 + (Game::InventoryItemSize + 3) * ind, Game::InventoryY + 3);
		rW.draw(sObject);
	}
	else {
		sObject.setPosition(Location.x, Location.y);
		rW.draw(sObject);
	}
}
void GameItem::Draw(sf::RenderWindow& rW) {
	sf::Sprite sObject(GameItem::itemTexture);
	if (isInInventory) {
		sObject.setPosition(Game::InventoryX + 3 + (Game::InventoryItemSize + 3) * (-1), Game::InventoryY + 3);
		rW.draw(sObject);
	}
	else {
		sObject.setPosition(Location.x, Location.y);
		rW.draw(sObject);
	}
}

sf::Texture GameItem::getTexture() {
	return GameItem::itemTexture;
}

int GameItem::getTicksToGrow() {
	return GameItem::ticksToGrow;
}

void GameItem::SetTicksToGrow(int inpTicks) {
	GameItem::ticksToGrow = inpTicks;
}