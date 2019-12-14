#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int bW = 30; // Amount of blocks horizontally
int bH = 20; // Amount of blocks vertically
int bFact = 25; // Pixels per length of block
int width = bW * bFact;
int height = bH * bFact;

int sDir = 1; // The direction snake is moving
int sLength = 4; // The length of the snake

int restarting = 0;

struct Snake {
    int x;
    int y;
} s[100];

struct Fruit {
    int x;
    int y;
} f;

bool isSamePoint(Snake iS, Fruit iF) {
    return ((iS.x == iF.x) && (iS.y == iF.y));
}
bool isSamePoint(Snake iS, Snake iS2) {
    return ((iS.x == iS2.x) && (iS.y == iS2.y));
}

void GenFruit(int x, int y) { f.x = x; f.y = y; }
void GenFruit() {
    // Generate Fruit
    f.x = rand() % bW;
    f.y = rand() % bH;
}

void RestartGame() {
    sLength = 4;
    restarting = 250;
}

void GameTick() {
    for (int i = sLength; i > 0; i--) { // For length of the snake, loop from tail to head
        // Move Snake part to the location of the part ahead of it in the snake body array
        s[i].x = s[i-1].x;
        s[i].y = s[i-1].y;
    }

    // Move head of snake (the lead) according to the direction player inputted
    if (sDir == 0) { s[0].y -= 1; } // Move North
    else if (sDir == 1) { s[0].x += 1; } // Move East
    else if (sDir == 2) { s[0].y += 1; } // Move South
    else if (sDir == 3) { s[0].x -= 1; } // Move West

    // If Snake caught Fruit
    if (isSamePoint(s[0], f)) {
        sLength++; // Make the snake longer

        GenFruit();
    }

    // Fix snake out of bounds
    if (s[0].x > bW) { s[0].x = 0; }
    if (s[0].x < 0) { s[0].x = bW; }
    if (s[0].y > bH) { s[0].y = 0; }
    if (s[0].y < 0) { s[0].y = bH; }

    // If snake ate himself, restart game
    for (int i = 1; i < sLength; i++) {
        if (isSamePoint(s[0], s[i])) {
            RestartGame();
        }
    }
}

int main()
{
    srand(time(0)); // Random seed

    RenderWindow window(VideoMode(width, height), "Snake!");

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
    float gTimer = 0;
    float timeDelay = 0.1;

    GenFruit();

    while (window.isOpen()) {
        gTimer += gClock.getElapsedTime().asSeconds(); // Get game time
        gClock.restart();

        Event e;
        while (window.pollEvent(e)){ // If an event needs to happen
            if (e.type == Event::Closed){ // If user wants to exit
                window.close(); // Exit
            }
        }

        // Change desired snake direction according to user input
        if (Keyboard::isKeyPressed(Keyboard::Up)){ sDir = 0; }
        else if (Keyboard::isKeyPressed(Keyboard::Right)){ sDir = 1; }
        else if (Keyboard::isKeyPressed(Keyboard::Down)){ sDir = 2; }
        else if (Keyboard::isKeyPressed(Keyboard::Left)){ sDir = 3; }

        if (gTimer > timeDelay) { // In order to prevent spam actions
            gTimer = 0;
            GameTick(); // Do tick
        }

        window.clear();

        if (restarting > 0) {
            // Draw Red graphic everywhere
            for (int x = 0; x < bW; x++) {
                for (int y = 0; y < bH; y++) {
                    sRed.setPosition(x * bFact, y * bFact);
                    window.draw(sRed);
                }
            }
            restarting--;
        } else {
            // Draw Gray graphic everywhere
            for (int x = 0; x < bW; x++) {
                for (int y = 0; y < bH; y++) {
                    sGray.setPosition(x * bFact, y * bFact);
                    window.draw(sGray);
                }
            }
        }

        for (int i = 0; i < sLength; i++) { // Loop through the snake parts
            sGreen.setPosition(s[i].x * bFact, s[i].y * bFact);
            window.draw(sGreen);
        }

        // Draw Fruit
        sRed.setPosition(f.x * bFact, f.y * bFact);
        window.draw(sRed);

        window.display();
    }

    return EXIT_SUCCESS;
}
