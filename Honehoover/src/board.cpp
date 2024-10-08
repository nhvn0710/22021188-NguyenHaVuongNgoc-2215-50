#include "board.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <SDL_ttf.h>


using namespace std;

Board::Board(int width, int height, int mineCount)
    : width(width), height(height), mineCount(mineCount), gameOver(false), firstReveal(false), lifeCount(lifeMax) {
    reset();
}

void Board::reset() {
    cells.clear();
    cells.resize(height, vector<Cell>(width));
    gameOver = false;
    firstReveal = false;
    restoreExtraLife();
    initializeBoard();
}

void Board::initializeBoard() {
    calculateAdjacentMines();
}

void Board::placeMines(int firstX, int firstY) {
    vector<int> positions(width * height);
    for (int i = 0; i < positions.size(); ++i) {
        positions[i] = i;
    }

    positions.erase(remove_if(positions.begin(), positions.end(), [this, firstX, firstY](int pos) {
        int x = pos % width, y = pos / width;
        return abs(x - firstX) <= 1 && abs(y - firstY) <= 1;
        }), positions.end());

    random_device rd;
    mt19937 g(rd());
    g.seed(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));
    shuffle(positions.begin(), positions.end(), g);

    for (int i = 0; i < mineCount; ++i) {
        int pos = positions[i];
        int x = pos % width;
        int y = pos / width;
        cells[y][x].setMine(true);
    }
}


void Board::calculateAdjacentMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!cells[y][x].isMine()) {
                cells[y][x].setAdjacentMines(countAdjacentMines(x, y));
            }
        }
    }
}

int Board::countAdjacentMines(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && cells[ny][nx].isMine()) {
                ++count;
            }
        }
    }
    return count;
}

bool Board::revealCell(int x, int y) {

    if (x < 0 || x >= width || y < 0 || y >= height || cells[y][x].isRevealed() || cells[y][x].isFlagged()) {
        return false;
    }

    if (!firstReveal) {
        firstReveal = true;
        placeMines(x, y); 
        calculateAdjacentMines();
    }

    cells[y][x].reveal();

    if (cells[y][x].isMine()) {
        if (extraLife && hasExtraLife()) {
            useExtraLife();
            cells[y][x].reveal();
            return true;
        }
        else {
            gameOver = true;
            return true;
        }
    }

    if (cells[y][x].getAdjacentMines() == 0) {
        revealAdjacentCells(x, y);
    }

    return true;
}

void Board::revealAdjacentCells(int x, int y) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                revealCell(nx, ny);
            }
        }
    }
}

bool Board::autoRevealAdjacentCells(int x, int y) {
    if (!cells[y][x].isRevealed() || cells[y][x].isMine()) {
        return false;
    }
    int flaggedMines = 0;
    int actualMines = 0;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (cells[ny][nx].isFlagged()) flaggedMines++;
                if (cells[ny][nx].isMine()) actualMines++;
            }
        }
    }

    if (flaggedMines != cells[y][x].getAdjacentMines()) {
        return false;
    }

    if (flaggedMines != actualMines) {
        gameOver = true;
        return true;
    }

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && !cells[ny][nx].isFlagged()) {
                revealCell(nx, ny);
            }
        }
    }
    return true;
}

void Board::toggleFlag(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height && !cells[y][x].isRevealed()) {
        cells[y][x].toggleFlag();
    }
}

bool Board::isGameOver() const {
    return gameOver;
}

bool Board::isGameWon() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (!cells[y][x].isMine() && !cells[y][x].isRevealed()) {
                return false;
            }
        }
    }
    return true;
}

void Board::render(SDL_Renderer* renderer, TTF_Font* font, int screenWidth, int screenHeight, const map<string, SDL_Texture*>& textures) const {
    int cellWidth = screenWidth / width;
    int cellHeight = screenHeight / height;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Rect cellRect = { x * cellWidth, y * cellHeight, cellWidth, cellHeight };

            if (cells[y][x].isRevealed()) {
                if (cells[y][x].isMine()) {
                    SDL_RenderCopy(renderer, textures.at("cross"), nullptr, &cellRect);
                }
                else {
                    SDL_RenderCopy(renderer, textures.at("clean"), nullptr, &cellRect);
                }
            }
            else if (cells[y][x].isFlagged()) {
                SDL_RenderCopy(renderer, textures.at("frame"), nullptr, &cellRect);
            }
            else {
                SDL_RenderCopy(renderer, textures.at("grass"), nullptr, &cellRect);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cellRect);

            if (cells[y][x].isRevealed() && !cells[y][x].isMine() && cells[y][x].getAdjacentMines() > 0) {
                SDL_Color textColor;
                switch (cells[y][x].getAdjacentMines()) {
                case 1: textColor = { 0, 0, 255, 255 }; break;  // Blue
                case 2: textColor = { 0, 128, 0, 255 }; break;  // Green
                case 3: textColor = { 255, 0, 0, 255 }; break;  // Red
                case 4: textColor = { 0, 0, 128, 255 }; break;  // Dark Blue
                case 5: textColor = { 128, 0, 0, 255 }; break;  // Dark Red
                case 6: textColor = { 0, 128, 128, 255 }; break;  // Teal
                case 7: textColor = { 0, 0, 0, 255 }; break;  // Black
                case 8: textColor = { 128, 128, 128, 255 }; break;  // Gray
                default: textColor = { 0, 0, 0, 255 }; break;
                }

                string text = to_string(cells[y][x].getAdjacentMines());
                SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), textColor);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        int textWidth = surface->w;
                        int textHeight = surface->h;
                        SDL_Rect destRect = {
                            cellRect.x + (cellRect.w - textWidth) / 2,
                            cellRect.y + (cellRect.h - textHeight) / 2,
                            textWidth,
                            textHeight
                        };
                        SDL_RenderCopy(renderer, texture, NULL, &destRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }
        }
    }
}

void Board::revealAllMines() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (cells[y][x].isMine()) {
                cells[y][x].reveal();
            }
        }
    }
}