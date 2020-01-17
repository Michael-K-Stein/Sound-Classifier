#include <SFML/Graphics.hpp>
#include <fstream>
using namespace sf;
using namespace std;

int Blocks[64];

Sprite blockSprites[9];

// The direction additive for the ball every tick
int dx = 5;
int dy = 3;

// The location of the ball
int xb = 200;
int yb = 350;

// The location of the paddle
int xp = 327;
int yp = 450;

int frmRate = 160;

int cLevel = 0; // The current level the player is on

float sclFact = 0.15; // Ball scale factor

// Load a new level
void LoadLevel(char* levelPath) {
    int totInd = 0;
    int tmpBlocks[64];
    string tot = "";
    ifstream myReadFile;
    myReadFile.open(levelPath);
    char outputChars[20];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> outputChars;
            for (int ind = 0; ind < 16; ind+=2) {
                tmpBlocks[totInd] = outputChars[ind] - 48;
                totInd++;
            }
        }
    }
    myReadFile.close();

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            Blocks[(i*8)+j] = tmpBlocks[(j*8)+i];
        }
    }
}

void SetNewLevel() {
    bool allZero = true;
    for (int i = 0; i < 64; i++) { // Check if one of the blocks still exists
        if (Blocks[i] != 0) { allZero = false; break; }
    }
    if (allZero) { // If all blocks are broken
        if (cLevel == 0) { LoadLevel("Levels/1.txt"); }
        else if (cLevel == 1) { LoadLevel("Levels/2.txt"); }
        else if (cLevel == 2) { LoadLevel("Levels/3.txt"); }
        else if (cLevel == 3) { LoadLevel("Levels/4.txt"); }
        else { // If player is above level 3, set a random board
            srand(time(0));
            for (int i = 0; i < 64; i++) { Blocks[i] = rand() % 10; }
        }
        cLevel++;
        xb = 852/2;
        yb = 400;
    }
}

bool isCollide(Sprite obj1, Sprite obj2) { // Are two objects colliding
    return obj1.getGlobalBounds().intersects(obj2.getGlobalBounds());
}

int main() {
    LoadLevel("Levels/0.txt");

    // Create the main window
    RenderWindow window(VideoMode(852, 480), "Arkanoid");
    window.setFramerateLimit(frmRate);

    // Load graphics
    Texture tBackground, tPaddle, tBall;
    tBackground.loadFromFile("graphics/background.jpg");
    Sprite sBackground(tBackground);
    tPaddle.loadFromFile("graphics/paddle.png");
    Sprite sPaddle(tPaddle);
    tBall.loadFromFile("graphics/ball.png");
    Sprite sBall(tBall);

    sBall.setScale(Vector2f(0.15f, 0.15f));

    // Load blocks
    Texture tAqua, tBlue, tGray, tGreen, tOrange, tPink, tPurple, tRed, tYellow;
    tAqua.loadFromFile("graphics/aqua.png");
    tBlue.loadFromFile("graphics/blue.png");
    tGray.loadFromFile("graphics/gray.png");
    tGreen.loadFromFile("graphics/green.png");
    tOrange.loadFromFile("graphics/orange.png");
    tPink.loadFromFile("graphics/pink.png");
    tPurple.loadFromFile("graphics/purple.png");
    tRed.loadFromFile("graphics/red.png");
    tYellow.loadFromFile("graphics/yellow.png");

    Sprite sAqua(tAqua);
    Sprite sBlue(tBlue);
    Sprite sGray(tGray);
    Sprite sGreen(tGreen);
    Sprite sOrange(tOrange);
    Sprite sPink(tPink);
    Sprite sPurple(tPurple);
    Sprite sRed(tRed);
    Sprite sYellow(tYellow);

    // Add graphics to an array of the graphics
    blockSprites[0] = sAqua;
    blockSprites[1] = sBlue;
    blockSprites[2] = sGray;
    blockSprites[3] = sGreen;
    blockSprites[4] = sOrange;
    blockSprites[5] = sPink;
    blockSprites[6] = sPurple;
    blockSprites[7] = sRed;
    blockSprites[8] = sYellow;

	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event)) {
            // Close window : exit
            if (event.type == Event::Closed)
                window.close();
        }

        // Fix ball position
        if (xb < 0 || xb > 852 - 19) { dx = -dx; }
        if (yb < 0) { dy = -dy; }
        if (yb > 480 - 19) { dy = -dy; sclFact -= 0.01; }
        if (sclFact == 0) { sclFact = 0.0000001; }

        xb += dx;
        yb += dy;

        // If paddle hit ball
        if (isCollide(sBall, sPaddle)){ dy = -dy; yb -= (7 + sclFact*10); sclFact += 0.01; }

        // Move paddle
        if (Keyboard::isKeyPressed(Keyboard::Left)){ xp -= 6; }
        else if (Keyboard::isKeyPressed(Keyboard::Right)){ xp += 6; }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sBackground);

        sBall.setPosition(xb, yb);
        window.draw(sBall);

        sPaddle.setPosition(xp, yp);
        window.draw(sPaddle);

        // Draw the blocks
        int blockInd = 0;
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (Blocks[blockInd] != 0) {
                    blockSprites[Blocks[blockInd] - 1].setPosition((i * 102) + 19, (j * 27) + 10);
                    window.draw(blockSprites[Blocks[blockInd] - 1]);
                    if (isCollide(sBall, blockSprites[Blocks[blockInd] - 1])){ Blocks[blockInd] -= 1; dy = -dy; yb += 10; } // If the ball hit the block, lower its level and move ball
                }
                blockInd++;
            }
        }

        // Update the window
        window.display();
        SetNewLevel();
        sBall.setScale(Vector2f(sclFact, sclFact));
    }

    return EXIT_SUCCESS;
}
