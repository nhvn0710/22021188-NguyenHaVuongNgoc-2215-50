#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

#include "cell.h"

using namespace std;

class Board {
public:
    Board(int width, int height, int mineCount);
    void reset();
    bool revealCell(int x, int y);
    void toggleFlag(int x, int y);
    bool isGameOver() const;
    bool isGameWon() const;
    void render(SDL_Renderer* renderer, TTF_Font* font, int screenWidth, int screenHeight) const;
    void revealAllMines();
    int getWidth() const;
    int getHeight() const;
    int getMineCount() const { return mineCount; }
    int getFlagCount() const;

private:
    void initializeBoard();
    void placeMines();
    void calculateAdjacentMines();
    int countAdjacentMines(int x, int y) const;
    void revealAdjacentCells(int x, int y);

    int width;
    int height;
    int mineCount;
    vector<vector<Cell>> cells;
    bool gameOver;
};