#pragma once
#include "GameItem.h"
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
class Inventory
{
public:
	Inventory();
	~Inventory();
	struct InventoryItemStruct {
		GameItem gameItem;
		int count;
	} Items[9];
	void Draw(sf::RenderWindow& rW);
	void SetSelect();
	int getSelect();
	void AddToInventory(GameItem gItem, int count);
private:
	sf::Texture tInventory;
	sf::Texture tInventorySelect;
	int selected;
};

