#include "Game Files/SnakeGame.h"

int main() {
	ML * ml = new ML();

	ml->Train();

	SnakeGame * game = new SnakeGame(ml);

	game->StartGame(0); // Human Mode | Learning Mode | ML Mode

	return EXIT_SUCCESS;
}