#pragma once
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();

	sf::RenderWindow window;
	static int InventoryX;
	static int InventoryY;
	static int InventoryItemSize;
	static int maxInventoryItems;
	static sf::Font font;
};

