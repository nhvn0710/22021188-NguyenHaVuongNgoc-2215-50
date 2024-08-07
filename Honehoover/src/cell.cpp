#include "cell.h"
using namespace std;

Cell::Cell() : mine(false), revealed(false), flagged(false), adjacentMines(0) {}

bool Cell::isMine() const {
    return mine;
}

bool Cell::isRevealed() const {
    return revealed;
}

bool Cell::isFlagged() const {
    return flagged;
}

int Cell::getAdjacentMines() const {
    return adjacentMines;
}

void Cell::setMine(bool mine) {
    this->mine = mine;
}

void Cell::setAdjacentMines(int count) {
    adjacentMines = count;
}

void Cell::reveal() {
    revealed = true;
}

void Cell::toggleFlag() {
    if (!revealed) {
        flagged = !flagged;
    }
}