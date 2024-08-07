#pragma once
#include <map>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "cell.h"

class Board {
public:
    Board(int width, int height, int mineCount);
    void reset();
    bool revealCell(int x, int y);
    void toggleFlag(int x, int y);
    bool isGameOver() const;
    bool isGameWon() const;
    void render(SDL_Renderer* renderer, TTF_Font* font, int screenWidth, int screenHeight, const map<string, SDL_Texture*>& textures) const;
    bool autoRevealAdjacentCells(int x, int y);
	void revealAllMines();
    int getWidth() const;
    int getHeight() const;
    int getMineCount() const { return mineCount; }
    int getFlagCount() const;
    bool hasExtraLife() const { return extraLife; }
    void useExtraLife() { extraLife = false; }

private:
    bool firstReveal;
    void initializeBoard();
    void placeMines(int firstX, int firstY);
    void calculateAdjacentMines();
    int countAdjacentMines(int x, int y) const;
    void revealAdjacentCells(int x, int y);
    
    bool extraLife;
    int width;
    int height;
    int mineCount;
    vector<vector<Cell>> cells;
    bool gameOver;
};