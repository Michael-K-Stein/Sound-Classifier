#include <SFML/Graphics.hpp>
using namespace sf;

const int bF = 16; // Pixels per length of block. Depends on the graphics
const int bW = 100; // Amount of blocks horizontally
const int bH = 50;  // Amount of blocks vertically
const int Width = bW * bF; // Width of the screen.
const int Height = bH * bF; // Height of the screen.

const int nBombs = 600; // Number of bombs

int grid[bW][bH];// Visible grid
int secGrid[bW][bH];// Secret game grid

void OpenNearby(int x, int y){ // A recursive function that opens all the empty spaces next to the current one.
    if (x - 1 >= 0) {
        if (y - 1 >= 0){
            if (secGrid[x - 1][y - 1] == 0){
                grid[x-1][y-1] = 0;
                secGrid[x-1][y-1] = 12;
                OpenNearby(x-1,y-1);
            } else if (secGrid[x - 1][y - 1] > 0 && secGrid[x - 1][y - 1] < 9){
                grid[x-1][y-1] = secGrid[x-1][y-1];
            }
        }
        if (secGrid[x - 1][y] == 0){
            grid[x-1][y] = 0;
            secGrid[x-1][y] = 12;
            OpenNearby(x-1, y);
        } else if (secGrid[x - 1][y] > 0 && secGrid[x - 1][y] < 9){
            grid[x-1][y] = secGrid[x-1][y];
        }
        if (y + 1 < bH){
            if (secGrid[x - 1][y + 1] == 0){
                grid[x-1][y+1] = 0;
                secGrid[x-1][y+1] = 12;
                OpenNearby(x-1,y+1);
            } else if (secGrid[x - 1][y + 1] > 0 && secGrid[x - 1][y + 1] < 9){
                grid[x - 1][y + 1] = secGrid[x - 1][y + 1];
            }
        }
    }
    if (y - 1 >= 0){
        if (secGrid[x][y - 1] == 0){
            grid[x][y-1] = 0;
            secGrid[x][y-1] = 12;
            OpenNearby(x,y-1);
        } else if (secGrid[x][y-1] > 0 && secGrid[x][y-1] < 9){
            grid[x][y-1] = secGrid[x][y-1];
        }
    }
    if (y + 1 < bH){
        if (secGrid[x][y + 1] == 0){
            grid[x][y+1] = 0;
            secGrid[x][y+1] = 12;
            OpenNearby(x,y+1);
        } else if (secGrid[x][y+1] > 0 && secGrid[x][y+1] < 9){
            grid[x][y+1] = secGrid[x][y+1];
        }
    }
    if (x + 1 < bW) {
        if (y - 1 >= 0){
            if (secGrid[x + 1][y - 1] == 0){
                grid[x+1][y-1] =0;
                secGrid[x+1][y-1] = 12;
                OpenNearby(x+1,y-1);
            } else if (secGrid[x+1][y-1] > 0 && secGrid[x+1][y-1] < 9){
                grid[x+1][y-1] = secGrid[x+1][y-1];
            }
        }
        if (secGrid[x + 1][y] == 0){
            grid[x+1][y] = 0;
            secGrid[x+1][y] = 12;
            OpenNearby(x+1, y);
        } else if (secGrid[x+1][y] > 0 && secGrid[x+1][y] < 9){
            grid[x+1][y] = secGrid[x+1][y];
        }
        if (y + 1 < bH){
            if (secGrid[x + 1][y + 1] == 0){
                grid[x+1][y+1] = 0;
                secGrid[x+1][y+1] = 12;
                OpenNearby(x+1,y+1);
            } else if (secGrid[x+1][y+1] > 0 && secGrid[x+1][y+1] < 9){
                grid[x+1][y+1] = secGrid[x+1][y+1];
            }
        }
    }
}

int main() {
    // Create the main window
    RenderWindow window(VideoMode(Width, Height), "Minesweeper");

    // Load the block graphics
    Texture tEmpty, tEmptyOpen, tFlag, tBomb;

    tEmpty.loadFromFile("Graphics/blocks/empty.png");
    tEmptyOpen.loadFromFile("Graphics/blocks/emptyOpen.png");
    tFlag.loadFromFile("Graphics/blocks/flag.png");
    tBomb.loadFromFile("Graphics/blocks/bomb.png");

    Sprite sEmpty(tEmpty);
    Sprite sEmptyOpen(tEmptyOpen);
    Sprite sFlag(tFlag);
    Sprite sBomb(tBomb);

    // Load the number graphics
    Texture t1, t2, t3, t4, t5, t6, t7, t8;

    t1.loadFromFile("Graphics/nums/1.png");
    t2.loadFromFile("Graphics/nums/2.png");
    t3.loadFromFile("Graphics/nums/3.png");
    t4.loadFromFile("Graphics/nums/4.png");
    t5.loadFromFile("Graphics/nums/5.png");
    t6.loadFromFile("Graphics/nums/6.png");
    t7.loadFromFile("Graphics/nums/7.png");
    t8.loadFromFile("Graphics/nums/8.png");

    Sprite s1(t1);
    Sprite s2(t2);
    Sprite s3(t3);
    Sprite s4(t4);
    Sprite s5(t5);
    Sprite s6(t6);
    Sprite s7(t7);
    Sprite s8(t8);

    // Fill Visible Grid
    for (int i = 0; i < bW; i++){
        for (int ii = 0; ii < bH; ii++){
            grid[i][ii] = 11;
        }
    }

    // Create Random Game (secret) Grid
    for (int i = 0; i < bW; i++){
        for (int ii = 0; ii < bH; ii++){
            secGrid[i][ii] = 0;
        }
    }

    srand(time(0));

    // Place the mines (bombs)
    for (int i = 0; i < nBombs; i++){
        int xBmb = rand() % bW;
        int yBmb = rand() % bH;
        while (false){
            /* set the while condition to "secGrid[xBmb][yBmb] == 10"
            to make sure the bombs do not get placed on top of each other,
            this does take much much longer to load, however. */
            srand(time(0));
            int xBmb = rand() % bW;
            int yBmb = rand() % bH;
        }
        if (secGrid[xBmb][yBmb] != 10){ // Irrelevant if the while is set-up properly.
            secGrid[xBmb][yBmb] = 10; // Add the randomly placed bomb to the secret grid.

            // Add 1 to blocks around the bomb
            if (xBmb - 1 >= 0 && yBmb - 1 >= 0){
                if (secGrid[xBmb - 1][yBmb - 1] != 10){
                    secGrid[xBmb - 1][yBmb - 1] += 1;
                }
            }

            if (xBmb - 1 >= 0 && yBmb >= 0){
                if (secGrid[xBmb - 1][yBmb] != 10){
                    secGrid[xBmb - 1][yBmb] += 1;
                }
            }

            if (xBmb - 1 >= 0 && yBmb + 1 < bH){
                if (secGrid[xBmb - 1][yBmb + 1] != 10){
                    secGrid[xBmb - 1][yBmb + 1] += 1;
                }
            }

            if (xBmb >= 0 && yBmb  - 1 >= 0){
                if (secGrid[xBmb][yBmb - 1] != 10){
                    secGrid[xBmb][yBmb - 1] += 1;
                }
            }

            if (xBmb >= 0 && yBmb + 1 < bH){
                if (secGrid[xBmb][yBmb + 1] != 10){
                    secGrid[xBmb][yBmb + 1] += 1;
                }
            }

            if (xBmb + 1 < bW && yBmb - 1 >= 0){
                if (secGrid[xBmb + 1][yBmb - 1] != 10){
                    secGrid[xBmb + 1][yBmb - 1] += 1;
                }
            }

            if (xBmb + 1 < bW && yBmb >= 0){
                if (secGrid[xBmb + 1][yBmb] != 10){
                    secGrid[xBmb + 1][yBmb] += 1;
                }
            }

            if (xBmb + 1 < bW && yBmb + 1 < bH){
                if (secGrid[xBmb + 1][yBmb + 1] != 10){
                    secGrid[xBmb + 1][yBmb + 1] += 1;
                }
            }
        }
    }

	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == Event::Closed){
                window.close();
            } else if (event.type == Event::MouseButtonPressed){
                Vector2i mPos = Mouse::getPosition(window);

                // Get mouse click location with respect to the game grid
                int xB = mPos.x / bF;
                int yB = mPos.y / bF;

                if (event.key.code == Mouse::Left) { // If block is left clicked.
                    if (grid[xB][yB] != 9 && secGrid[xB][yB] != 12){ // If block is neither a bomb nor already opened.
                        grid[xB][yB] = secGrid[xB][yB]; // Show the player what the block really is.
                        if (secGrid[xB][yB] == 0){ // If is an empty block
                            grid[xB][yB] = 12; // Set it to empty visible (mode 12)
                            OpenNearby(xB, yB); // Open all nearby empty spaces.
                        }
                    }
                } else if (event.key.code == Mouse::Right){ // If block is right clicked
                    if (grid[xB][yB] == 11){ // If closed block is empty
                        grid[xB][yB] = 9; // Place a flag on the visible grid
                    } else if (grid[xB][yB] == 9) { // If closed block is flagged
                        grid[xB][yB] = 11; // Remove the flag
                    }
                }
            }
        }

        // Clear screen
        window.clear();

        // Draw the grid
        for (int x = 0; x < bW; x++){
            for (int y = 0; y < bH; y++){
                if (grid[x][y] == 11) {
                    sEmpty.setPosition(x * bF, y * bF);
                    window.draw(sEmpty);
                } else if (grid[x][y] == 1) {
                    s1.setPosition(x * bF, y * bF);
                    window.draw(s1);
                } else if (grid[x][y] == 2) {
                    s2.setPosition(x * bF, y * bF);
                    window.draw(s2);
                } else if (grid[x][y] == 3) {
                    s3.setPosition(x * bF, y * bF);
                    window.draw(s3);
                } else if (grid[x][y] == 4) {
                    s4.setPosition(x * bF, y * bF);
                    window.draw(s4);
                } else if (grid[x][y] == 5) {
                    s5.setPosition(x * bF, y * bF);
                    window.draw(s5);
                } else if (grid[x][y] == 6) {
                    s6.setPosition(x * bF, y * bF);
                    window.draw(s6);
                } else if (grid[x][y] == 7) {
                    s7.setPosition(x * bF, y * bF);
                    window.draw(s7);
                } else if (grid[x][y] == 8) {
                    s8.setPosition(x * bF, y * bF);
                    window.draw(s8);
                } else if (grid[x][y] == 9) {
                    sFlag.setPosition(x * bF, y * bF);
                    window.draw(sFlag);
                } else if (grid[x][y] == 10) {
                    sBomb.setPosition(x * bF, y * bF);
                    window.draw(sBomb);
                } else if (grid[x][y] == 0) {
                    sEmptyOpen.setPosition(x * bF, y * bF);
                    window.draw(sEmptyOpen);
                } else {
                    sEmptyOpen.setPosition(x * bF, y * bF);
                    window.draw(sEmptyOpen);
                }
            }
        }

        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}
