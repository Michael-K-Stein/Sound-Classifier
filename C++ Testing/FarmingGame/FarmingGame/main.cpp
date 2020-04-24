#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h> 

#include "Game.h"
#include "Inventory.h"

// All game items
#include "Farmland.h"
#include "Barn.h"
#include "Wheat.h"
#include "Carrot.h"
#include "Potato.h"

const int width = 1080;
const int height = 720;

int Game::InventoryX = 238;
int Game::InventoryY = 630;
int Game::InventoryItemSize = 64;
int Game::maxInventoryItems = 64;
sf::Font Game::font = sf::Font();

int main()
{
	srand(time(0)); // Random seed

	Game::font.loadFromFile("Fonts/arial.ttf");

	sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Farming Game"); // Open a new window of the desired dimensions with the title "Snake!".

	gameWindow.setTitle("Farming Game"); // new sf::RenderWindow(sf::VideoMode(width, height), "Farming Game");
	gameWindow.setSize(sf::Vector2u(width, height));

	Inventory playerInventory = Inventory();
	playerInventory.Items[0].gameItem = Wheat();
	playerInventory.Items[0].gameItem.SetTicksToGrow(Wheat::ticksToGrow);
	playerInventory.Items[0].gameItem.type = "Wheat";
	playerInventory.Items[0].count = 1;

	playerInventory.Items[1].gameItem = Carrot();
	playerInventory.Items[1].gameItem.SetTicksToGrow(Carrot::ticksToGrow);
	playerInventory.Items[1].gameItem.type = "Carrot";
	playerInventory.Items[1].count = 1;

	playerInventory.Items[2].gameItem = Potato();
	playerInventory.Items[2].gameItem.SetTicksToGrow(Potato::ticksToGrow);
	playerInventory.Items[2].gameItem.type = "Potato";
	playerInventory.Items[2].count = 1;

	sf::Texture tGrass;
	tGrass.loadFromFile("graphics/Grass.png");
	sf::Sprite sGrass(tGrass);
	sGrass.setScale(1.541,2.1);

	Barn myBarn = Barn(30,30);

	sf::Sprite sMouseItem;

	Farmland farmLands[3] = { Farmland(200, 300), Farmland(450, 150), Farmland(700, 380) };

	//Create game Clock object
	sf::Clock gClock;
	float gTimer = 0; // Game Timer
	float timeDelay = 1/ 10;

	while (gameWindow.isOpen()) {
		gTimer += gClock.getElapsedTime().asSeconds(); // Get time since last clock check and add it to the game timer.
		gClock.restart(); // Set clock time to 0


		if (gTimer > timeDelay) { // In order to prevent spam actions
			gTimer = 0;

			// Do tick
			for (int i = 0; i < 3; i++)
			{
				farmLands[i].Grow();
			}
		}

		sf::Event e; // SFML Window event
		while (gameWindow.pollEvent(e)) { // If an event needs to happen
			if (e.type == sf::Event::Closed) { // If user wants to exit
				gameWindow.close(); // Exit
			}
			else if (e.type == sf::Event::MouseButtonPressed)
			{
				for (int i = 0; i < 3; i++) {
					if (farmLands[i].CheckClicked(gameWindow)) {
						if (farmLands[i].isFull()) {
							playerInventory.AddToInventory(farmLands[i].itemType, farmLands[i].getCropAmount());
							farmLands[i].CropsRemoved();
						}
						else {
							if (playerInventory.Items[playerInventory.getSelect()].count > 0 && farmLands[i].CoolOffTime == 0) {
								farmLands[i].SetObject(playerInventory.Items[playerInventory.getSelect()].gameItem);
								playerInventory.Items[playerInventory.getSelect()].count--;
							}
						}
					}
				}
			}
		}

		playerInventory.SetSelect();

		gameWindow.clear(); // Remove all window graphics

		gameWindow.draw(sGrass);

		myBarn.Draw(gameWindow);

		for (int i = 0; i < 3; i++)
		{
			farmLands[i].Draw(gameWindow);
			if (farmLands[i].CoolOffTime != 0) { farmLands[i].CoolOffTime--; }
		}

		playerInventory.Draw(gameWindow);

		if (playerInventory.Items[playerInventory.getSelect()].count > 0) {
			sMouseItem.setTexture(playerInventory.Items[playerInventory.getSelect()].gameItem.itemTexture);
			sMouseItem.setPosition(sf::Mouse::getPosition(gameWindow).x - 32, sf::Mouse::getPosition(gameWindow).y - 32);
			gameWindow.draw(sMouseItem);
		}

		gameWindow.display(); // Show the new display
	}
	return EXIT_SUCCESS;
}
