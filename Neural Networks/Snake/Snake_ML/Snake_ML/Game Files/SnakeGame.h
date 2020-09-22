#ifndef __SnakeGame

#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include "../ML Files/ML.h"
#include "../ML Files/Network/data.h"
using namespace sf;

class SnakeGame {

public:

	int gamesPlayed = 0;
	int LengthHighScore = 4;
	int MovesHighScore = 0;

	int currentMoves = 0;

	int bW = 20; // Amount of blocks horizontally
	int bH = 20; // Amount of blocks vertically
	int bFact = 25; // Pixels per length of block. Depends on the graphics
	int width = bW * bFact;  // Width of the screen.
	int height = bH * bFact; // Height of the screen.

	int sDir = 1; // The direction snake is moving
	int sLength = 4; // The length of the snake

	int restarting = 0; // Is zero when game-play is normal, changes to higher number to indicate the game is restarting.

	ML * ml;

	struct Snake { // The structure of a snake body part as a vector of the x and y coordinates,
		int x;
		int y;
	} s[100]; // Initialize a full 'Snake' as an array of 100 snake body parts. Notice: There is a limitation of the maximum size of the snake. (The world record is 252, I think this is fine...)

	struct Fruit { // The structure of a fruit. A vector of it's x and y coordinates.
		int x;
		int y;
	} f;

	SnakeGame(ML * inpml);

	int StartGame(int ML); // Human Mode | Learning Mode | ML Mode

private:
	bool isSamePoint(Snake iS, Fruit iF); // Is an object of type Snake on the same 'block' as an object of type Fruit.
	bool isSamePoint(Snake iS, Snake iS2); // Same as previous function, except for 2 Snake objects.

	void GenFruit(int x, int y);// Adds a fruit at point.
	void GenFruit(); // Adds a fruit at a random point.

	void RestartGame();

	void GameTick();

	int movesSinceLastFruit = 0;

	float lastDistToFruit = 1000;
	float calcDistToFruit();

	int startingX = 0;
	int startingY = 0;

	// For ML
	bool isBlocked(int x, int y);
	bool isBlockedInFront();
	bool isBlockedOnLeft();
	bool isBlockedOnRight();
};

#endif // !__SnakeGame