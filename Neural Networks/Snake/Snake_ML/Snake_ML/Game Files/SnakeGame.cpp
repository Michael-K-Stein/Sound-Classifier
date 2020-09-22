#include "SnakeGame.h"

SnakeGame::SnakeGame(ML * inpml) {
	ml = inpml;
}

bool SnakeGame::isBlocked(int x, int y) {
	bool flag = false;
	for (int i = 0; i < sLength; i++) {
		flag = (flag || (s[i].x == x && s[i].y == y));
	}
	if (y < 0 || y == bH) { flag = true; }
	if (x < 0 || x == bW) { flag = true; }
	return flag;
}

bool SnakeGame::isBlockedInFront() {
	int x = s[0].x; // Front Block X
	int y = s[0].y; // Front Block Y
	if (sDir == 0) {
		y = (y + bH-1) % bH;
	} else if (sDir == 2) {
		y = (y + 1) % bH;
	}
	else if (sDir == 1) {
		x = (x + 1) % bW;
	}
	else if (sDir == 3) {
		x = (x + bW - 1) % bW;
	}
	return isBlocked(x, y);
}

bool SnakeGame::isBlockedOnRight() {
	int x = s[0].x; // Right Block X
	int y = s[0].y; // Right Block Y
	if (sDir == 3) {
		y = (y + bH - 1) % bH;
	}
	else if (sDir == 1) {
		y = (y + 1) % bH;
	}
	else if (sDir == 0) {
		x = (x + 1) % bW;
	}
	else if (sDir == 2) {
		x = (x + bW - 1) % bW;
	}
	return isBlocked(x, y);
}

bool SnakeGame::isBlockedOnLeft() { 
	int x = s[0].x; // Left Block X
	int y = s[0].y; // Left Block Y
	if (sDir == 1) {
		y = (y + bH - 1) % bH;
	}
	else if (sDir == 3) {
		y = (y + 1) % bH;
	}
	else if (sDir == 2) {
		x = (x + 1) % bW;
	}
	else if (sDir == 0) {
		x = (x + bW - 1) % bW;
	}
	return isBlocked(x, y);
}


int SnakeGame::StartGame(int ML) {
	srand(time(0)); // Random seed

	RenderWindow window(VideoMode(width, height), "Snake!"); // Open a new window of the desired dimensions with the title "Snake!".

	// Load graphics
	Texture tRed, tGreen, tGray;
	tRed.loadFromFile("graphics/red.png");
	tGreen.loadFromFile("graphics/green.png");
	tGray.loadFromFile("graphics/gray.png");

	// Create drawable graphic objects
	Sprite sGray(tGray);
	Sprite sGreen(tGreen);
	Sprite sRed(tRed);

	//Create game Clock object
	Clock gClock;
	float gTimer = 0; // Game Timer
	float timeDelay = 0.1;

	GenFruit(); // Add the initial fruit at a random location

	s[0].x = startingX;
	s[0].y = startingY;

	while (window.isOpen()) {
		gTimer += gClock.getElapsedTime().asSeconds(); // Get time since last clock check and add it to the game timer.
		gClock.restart(); // Set clock time to 0

		Event e; // SFML Window event
		while (window.pollEvent(e)) { // If an event needs to happen
			if (e.type == Event::Closed) { // If user wants to exit
				window.close(); // Exit
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Space)) { timeDelay = 0.1; }
		if (Keyboard::isKeyPressed(Keyboard::Enter)) { timeDelay = 0.000000001; }
		if (Keyboard::isKeyPressed(Keyboard::Num1)) { ML = 1; }
		if (Keyboard::isKeyPressed(Keyboard::Num2)) { ML = 2; }
		if (Keyboard::isKeyPressed(Keyboard::Num3)) { timeDelay = 0.5; }

		if (ML == 0) {
			// Change desired snake direction according to user input
			if (Keyboard::isKeyPressed(Keyboard::Up)) { sDir = 0; }
			else if (Keyboard::isKeyPressed(Keyboard::Right)) { sDir = 1; }
			else if (Keyboard::isKeyPressed(Keyboard::Down)) { sDir = 2; }
			else if (Keyboard::isKeyPressed(Keyboard::Left)) { sDir = 3; }
		}
		else if (ML == 1){
			std::vector<double> * inputData = new std::vector<double>();

			inputData->push_back(isBlockedInFront());
			inputData->push_back(isBlockedOnLeft());
			inputData->push_back(isBlockedOnRight());

			inputData->push_back(s[0].x);
			inputData->push_back(s[0].y);
			inputData->push_back(f.x);
			inputData->push_back(f.y);
			inputData->push_back(sDir);


			data * d = new data();
			d->setNormalizedFeatureVector(inputData);

			std::vector<double> * des = new std::vector<double>;

			int desOut = 0;

			int oldDir = sDir;

			// Change desired snake direction according to user input
			if (Keyboard::isKeyPressed(Keyboard::Up)) { sDir = 0; }
			else if (Keyboard::isKeyPressed(Keyboard::Right)) { sDir = 1; }
			else if (Keyboard::isKeyPressed(Keyboard::Down)) { sDir = 2; }
			else if (Keyboard::isKeyPressed(Keyboard::Left)) { sDir = 3; }

			if (sDir == (oldDir + 1) % 4) { desOut = 2; } // Turned Right
			if (sDir == (oldDir + 3) % 4) { desOut = 1; } // Turned Left
			if (sDir == oldDir) { desOut = 0; } // Continued Straight

			des->push_back(0);
			des->push_back(0);
			des->push_back(0);
		
			des->at(desOut) = 1;
			d->setDesiredOutput(des);

			ml->AddTrainingData(d);
		}

		if (gTimer > timeDelay) { // In order to prevent spam actions
			gTimer = 0;


			std::vector<double> * inputData = new std::vector<double>();

			inputData->push_back(isBlockedInFront());
			inputData->push_back(isBlockedOnLeft());
			inputData->push_back(isBlockedOnRight());

			inputData->push_back(s[0].x);
			inputData->push_back(s[0].y);
			inputData->push_back(f.x);
			inputData->push_back(f.y);
			inputData->push_back(sDir);


			data * d = new data();
			d->setNormalizedFeatureVector(inputData);

			int oldLength = sLength;

			if (ML == 2) { 
				int decision = ml->Decide(d);  // Let the ML algorithm make the decision.
				if (decision == 0) {
					// Continue straight
				} else if (decision == 1) {
					// Turn right
					sDir = (sDir + 1) % 4;
				} else if (decision == 2) {
					// Turn left
					sDir = (sDir + 3) % 4;
				}

				/*// Change desired snake direction according to user input
				if (decision == 0) { sDir = 0; }
				else if (decision == 1) { sDir = 1; }
				else if (decision == 2) { sDir = 2; }
				else if (decision == 3) { sDir = 3; }
				else if (decision == 4) { /* do nothing  }*/

			}
			GameTick(); // Do tick

			currentMoves++;

			if (ML == 2) {
				float currDist = calcDistToFruit();
				bool good = false;
				if (currDist < lastDistToFruit) { good = true; }
				lastDistToFruit = currDist;
				if (restarting > 0) { good = false; }
				if (sLength > oldLength) { good = true; }
				if (sLength > LengthHighScore) { good = true; }

				//if (currentMoves > MovesHighScore) { good = true; }
				if ((sLength > oldLength) || (sLength > LengthHighScore) || (restarting > 0) || (currDist < lastDistToFruit)) {
					ml->Update(d, good);
				}
			}
		}

		window.clear(); // Remove all window graphics
		
		if (restarting > 0) {
			// Draw Red graphic everywhere
			/*
			for (int x = 0; x < bW; x++) {
				for (int y = 0; y < bH; y++) {
					sRed.setPosition(x * bFact, y * bFact);
					window.draw(sRed);
				}
			}*/
			restarting--;
		}
		else {
			// Draw Gray graphic everywhere
			for (int x = 0; x < bW; x++) {
				for (int y = 0; y < bH; y++) {
					sGray.setPosition(x * bFact, y * bFact);
					window.draw(sGray);
				}
			}
		}

		for (int i = 0; i < sLength; i++) { // Loop through the snake parts
			sGreen.setPosition(s[i].x * bFact, s[i].y * bFact); // Set the position of the Green drawable graphics to the current snake part's location.
			window.draw(sGreen); // Draw the Green graphics onto the screen.
		}

		// Draw Fruit
		sRed.setPosition(f.x * bFact, f.y * bFact);
		window.draw(sRed);

		window.display(); // Show the new display
	}

	return EXIT_SUCCESS;
}

bool SnakeGame::isSamePoint(Snake iS, Fruit iF) {
	return ((iS.x == iF.x) && (iS.y == iF.y));
}

bool SnakeGame::isSamePoint(Snake iS, Snake iS2) {
	return ((iS.x == iS2.x) && (iS.y == iS2.y));
}

void SnakeGame::GenFruit(int x, int y) { f.x = x; f.y = y; } // Adds a fruit at point.
void SnakeGame::GenFruit() { // Adds a fruit at a random point.
	// Generate Fruit
	f.x = rand() % bW;
	f.y = rand() % bH;
}

void SnakeGame::RestartGame() { // Restarts the game
	gamesPlayed++;
	printf("Game no. %d has ended.\n", gamesPlayed);
	printf("My Length %d | Best Length %d\n", sLength, LengthHighScore);
	printf("My Move Count %d | Best move count %d\n", currentMoves, MovesHighScore);

	if (sLength > LengthHighScore) { LengthHighScore = sLength; }
	if (currentMoves > MovesHighScore) { MovesHighScore = currentMoves; }

	currentMoves = 0;
	movesSinceLastFruit = 0;
	sLength = 4;
	restarting = 1; // This is to make the fact that the player lost more prominent by making the timeout longer.
	s[0].x = startingX;
	s[0].y = startingY;
	sDir = 1;

	GenFruit();
}

float SnakeGame::calcDistToFruit() {
	float c = pow((s[0].x - f.x), 2) + pow((s[0].y - f.y), 2);
	c = pow(c, 0.5);
	return c;
}

void SnakeGame::GameTick() {
	for (int i = sLength; i > 0; i--) { // For length of the snake, loop from tail to head
		// Move Snake part to the location of the part ahead of it in the snake body array
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}

	// Move head of snake (the lead) according to the direction player inputted
	if (sDir == 0) { s[0].y -= 1; } // Move North
	else if (sDir == 1) { s[0].x += 1; } // Move East
	else if (sDir == 2) { s[0].y += 1; } // Move South
	else if (sDir == 3) { s[0].x -= 1; } // Move West

	// If Snake caught Fruit
	if (isSamePoint(s[0], f)) {
		sLength++; // Make the snake longer

		movesSinceLastFruit = 0;

		GenFruit(); // Generate a new fruit, at a random location.
	}
	else { movesSinceLastFruit++; }


	if (s[0].x > bW) { RestartGame(); }
	if (s[0].x < 0) { RestartGame(); }
	if (s[0].y > bH) { RestartGame(); }
	if (s[0].y < 0) { RestartGame(); }

	// If snake ate himself, restart game
	for (int i = 1; i < sLength; i++) {
		if (isSamePoint(s[0], s[i])) {
			RestartGame();
		}
	}

	// If hasn't eaten in "a lot" of moves
	if (movesSinceLastFruit > bW * bH) { RestartGame(); }
}