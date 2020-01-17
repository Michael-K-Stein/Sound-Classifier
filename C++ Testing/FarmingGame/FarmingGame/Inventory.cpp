#include "Inventory.h"



Inventory::Inventory()
{
	tInventory.loadFromFile("graphics/Inventory.png");
	tInventorySelect.loadFromFile("graphics/InventorySelect.png");
	selected = 0;
}


Inventory::~Inventory()
{
}

void Inventory::Draw(sf::RenderWindow& rW) {
	sf::Sprite sInventory(tInventory);
	sInventory.setPosition(Game::InventoryX, Game::InventoryY);
	rW.draw(sInventory);
	for (int ind = 0; ind < 9; ind++)
	{
		if (Items[ind].count > 0) {
			Items[ind].gameItem.Draw(rW, ind);
			sf::Text tCount;
			tCount.setString(std::to_string(Items[ind].count));
			tCount.setCharacterSize(24);
			tCount.setFillColor(sf::Color::White);
			tCount.setPosition(Game::InventoryX + 3 + (Game::InventoryItemSize + 3) * (ind), Game::InventoryY + 3 + 40);
			tCount.setFont(Game::font);
			rW.draw(tCount);
		}
		else {
			Items[ind].gameItem = GameItem();
		}
	}
	sf::Sprite sInventorySelect(tInventorySelect);
	sInventorySelect.setPosition(Game::InventoryX + (67 * selected), Game::InventoryY);
	rW.draw(sInventorySelect);
}

void Inventory::SetSelect() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		selected = 0;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		selected = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
		selected = 2;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
		selected = 3;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
		selected = 4;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
		selected = 5;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
		selected = 6;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)) {
		selected = 7;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)) {
		selected = 8;
	}
}

int Inventory::getSelect() {
	return selected;
}

void Inventory::AddToInventory(GameItem gItem, int count) {
	//bool notAlreadyPlaced = true;;
	for (int ind = 0; ind < 9; ind++) {
		if (Inventory::Items[ind].gameItem.type == gItem.type && Inventory::Items[ind].count < Game::maxInventoryItems) {
			if (Inventory::Items[ind].count + count > Game::maxInventoryItems) {
				count = Inventory::Items[ind].count + count - Game::maxInventoryItems;
				Inventory::Items[ind].count = Game::maxInventoryItems;
			}
			else {
				if (Inventory::Items[ind].count + count <= Game::maxInventoryItems) {
					Inventory::Items[ind].count += count;
					count = 0;
					break;
				}
				else {
					count = Inventory::Items[ind].count + count - Game::maxInventoryItems;
					Inventory::Items[ind].count = Game::maxInventoryItems;
				}
			}
		}
	}
	if (count > 0) {
		for (int ind = 0; ind < 9; ind++) {
			if (Inventory::Items[ind].count <= 0) {
				Inventory::Items[ind].count = 0;
				if (Inventory::Items[ind].count + count <= Game::maxInventoryItems) {
					Inventory::Items[ind].count += count;
					count = 0;
				}
				else {
					count = Inventory::Items[ind].count + count - Game::maxInventoryItems;
					Inventory::Items[ind].count = Game::maxInventoryItems;
				}
				Inventory::Items[ind].gameItem = gItem;
			}
		}
	}
}