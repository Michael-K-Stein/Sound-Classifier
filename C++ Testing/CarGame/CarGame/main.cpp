#include <SFML/Graphics.hpp>
#include <time.h>

#include <string>
#include <iostream>
#include <array>

#include "Game.h"
#include "DrawableItem.h"
#include "MoveableItem.h"

void Draw(Game& g, DrawableItem* items) {
	for (int i = 0; i < 1; i++)
	{
		items[i].Draw(g);
	}
}

int main() {
	srand(time(0)); // Random seed

	Game g = Game();
	g.gameWindow.setFramerateLimit(60);
	//g.gameWindow.create(sf::VideoMode(g.width, g.height), "Car Game");

	sf::Texture tGrass;
	tGrass.loadFromFile("Graphics/Grass.png");
	sf::Sprite sGrass(tGrass);
	sGrass.setScale(1.541, 2.1);

	sf::Texture tGrass2;
	tGrass2.loadFromFile("Graphics/track.png");
	sf::Sprite sTrack(tGrass2);
	sTrack.setScale(0.2, 0.2);

	DrawableItem items[1];
	MoveableItem car = MoveableItem("Graphics/car2.png");
	car.SetPosition(500, 350);
	car.mySprite.setOrigin(53, 20);
	//ar.mySprite.setScale(0.5, 0.5);
	items[0] = car;
	car.Name = "My Car";

	while (g.gameWindow.isOpen()) {
		sf::Event e; // SFML Window event
		while (g.gameWindow.pollEvent(e)) { // If an event needs to happen
			if (e.type == sf::Event::Closed) { // If user wants to exit
				g.gameWindow.close(); // Exit
			}
			else if (e.type == sf::Event::MouseButtonPressed)
			{
				
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { car.MoveForward(2); }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { car.MoveForward(-1); }
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { car.mySprite.rotate(2); }
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { car.mySprite.rotate(-2); }

		g.gameWindow.clear();

		g.gameWindow.draw(sGrass);
		g.gameWindow.draw(sTrack);
		//Draw(g, items);
		g.gameWindow.draw(car.mySprite);

		g.gameWindow.display();
	}
}