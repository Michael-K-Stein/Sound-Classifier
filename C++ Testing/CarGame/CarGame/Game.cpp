#include "Game.h"
#include <SFML/Graphics.hpp>



Game::Game()
{
	initDisplay();
}
Game::Game(const Game&)
{
	initDisplay();
}

Game::~Game()
{
}

void Game::initDisplay() {
	Game::gameWindow.create(sf::VideoMode(width, height), "Car Game");
}

float Game::Deg2Rad(float deg) {
	return deg * (3.14 / 180);
}