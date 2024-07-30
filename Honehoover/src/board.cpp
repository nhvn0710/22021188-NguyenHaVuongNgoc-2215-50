#include "Board.h"
#include <random>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <SDL_ttf.h>
#include <string>

using namespace std;

Board::Board(int width, int height, int mineCount)
    : width(width), height(height), mineCount(mineCount), gameOver(false) {
    reset();
}

void Board::reset() {
    cells.clear();
    cells.resize(height, vector<Cell>(width));
    gameOver = false;
    initializeBoard();
}

void Board::initializeBoard() {
    placeMines();
    calculateAdjacentMines();
}

void Board::placeMines() {
    vector<int> positions(width * height);
    iota(positions.begin(), positions.end(), 0);
    random_device rd;
    mt19937 g(rd());
    g.seed(chrono::steady_clock::now().time_since_epoch().count());
    vector<int> innerPositions;
    copy_if(positions.begin(), positions.end(), back_inserter(innerPositions), [this](int i) {
        int x = i % width, y = i / width;
        // Exclude the edges
        return x > 0 && x < width - 1 && y > 0 && y < height - 1;
        });

    shuffle(innerPositions.begin(), innerPositions.end(), g);
    for (int i = 0; i < mineCount; ++i) {
        int pos = innerPositions[i];
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

    cells[y][x].reveal();

    if (cells[y][x].isMine()) {
        gameOver = true;
        return true;
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

void Board::render(SDL_Renderer* renderer, TTF_Font* font, int screenWidth, int screenHeight) const {
    int cellWidth = screenWidth / width;
    int cellHeight = screenHeight / height;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            SDL_Rect cellRect = { x * cellWidth, y * cellHeight, cellWidth, cellHeight };

            if (cells[y][x].isRevealed()) {
                if (cells[y][x].isMine()) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                }
            }
            else if (cells[y][x].isFlagged()) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
            }

            SDL_RenderFillRect(renderer, &cellRect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &cellRect);

            if (cells[y][x].isRevealed() && !cells[y][x].isMine() && cells[y][x].getAdjacentMines() > 0) {
                // Render the number of adjacent mines
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

int Board::getFlagCount() const {
    int count = 0;
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            if (cell.isFlagged()) {
                count++;
            }
        }
    }
    return count;
}

int Board::getWidth() const {
    return width;
}

int Board::getHeight() const {
    return height;
}