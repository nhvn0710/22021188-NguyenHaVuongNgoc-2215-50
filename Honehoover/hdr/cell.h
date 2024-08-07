#pragma once
using namespace std;

class Cell {
public:
    Cell();

    bool isMine() const;
    bool isRevealed() const;
    bool isFlagged() const;
    int getAdjacentMines() const;

    void setMine(bool mine);
    void setAdjacentMines(int count);
    void reveal();
    void toggleFlag();

private:
    bool mine;
    bool revealed;
    bool flagged;
    int adjacentMines;
};