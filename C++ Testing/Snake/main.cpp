#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int bW = 30; // Amount of blocks horizontally
int bH = 20; // Amount of blocks vertically
int bFact = 25; // Pixels per length of block. Depends on the graphics
int width = bW * bFact;  // Width of the screen.
int height = bH * bFact; // Height of the screen.

int sDir = 1; // The direction snake is moving
int sLength = 4; // The length of the snake

int restarting = 0; // Is zero when game-play is normal, changes to higher number to indicate the game is restarting.

struct Snake { // The structure of a snake body part as a vector of the x and y coordinates,
    int x;
    int y;
} s[100]; // Initialize a full 'Snake' as an array of 100 snake body parts. Notice: There is a limitation of the maximum size of the snake. (The world record is 252, I think this is fine...)

struct Fruit { // The structure of a fruit. A vector of it's x and y coordinates.
    int x;
    int y;
} f;

bool isSamePoint(Snake iS, Fruit iF) { // Is an object of type Snake on the same 'block' as an object of type Fruit.
    return ((iS.x == iF.x) && (iS.y == iF.y));
}
bool isSamePoint(Snake iS, Snake iS2) { // Same as previous function, except for 2 Snake objects.
    return ((iS.x == iS2.x) && (iS.y == iS2.y));
}

void GenFruit(int x, int y) { f.x = x; f.y = y; } // Adds a fruit at point.
void GenFruit() { // Adds a fruit at a random point.
    // Generate Fruit
    f.x = rand() % bW;
    f.y = rand() % bH;
}

void RestartGame() { // Restarts the game
    sLength = 4;
    restarting = 250; // This is to make the fact that the player lost more prominent by making the timeout longer.
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

        GenFruit(); // Generate a new fruit, at a random location.
    }

    /* Fix snake out of bounds. (In the original game, when the snake went out of bounds, it would die.
       However, I am not very good at this game so when going out of bounds,
       the snake will come out the other side, like in Pac-Man */
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

    while (window.isOpen()) {
        gTimer += gClock.getElapsedTime().asSeconds(); // Get time since last clock check and add it to the game timer.
        gClock.restart(); // Set clock time to 0

        Event e; // SFML Window event
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

        window.clear(); // Remove all window graphics

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
