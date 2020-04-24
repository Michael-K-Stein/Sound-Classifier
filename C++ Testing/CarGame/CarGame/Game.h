#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class Game
{
public:
	Game();
	Game(const Game&);
	~Game();
	int width = 1080;
	int height = 720;
	sf::RenderWindow gameWindow;
	static float Deg2Rad(float deg);
private:
	void initDisplay();
};