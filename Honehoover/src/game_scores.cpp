#include <algorithm>
#include <fstream>
#include <iostream>

#include "game.h"

void Game::loadHighScores() {
    ifstream inFile("highscores.txt");
    highScores.clear();

    int time;
    string difficulty;
    while (inFile >> time >> difficulty) {
        DifficultyLevel difficultyLevel = DifficultyLevel::EASY;
        if (difficulty == "Easy") difficultyLevel = DifficultyLevel::EASY;
        else if (difficulty == "Medium") difficultyLevel = DifficultyLevel::MEDIUM;
        else if (difficulty == "Hard") difficultyLevel = DifficultyLevel::HARD;

        highScores.push_back({ time, difficultyLevel });
    }

    sort(highScores.begin(), highScores.end(), compareHighScores);
}

void Game::updateHighScores() {
    if (gameWon) {
        HighScore newScore{};
        newScore.time = finalTime;
        newScore.difficulty = currentDifficulty;

        highScores.push_back(newScore);
        sort(highScores.begin(), highScores.end(), compareHighScores);

        // Trim list to top 5
        if (highScores.size() > 5) {
            highScores.resize(5);
        }

        saveHighScores();
    }
}

void Game::saveHighScores() const {
    ofstream outFile("highscores.txt");
    if (outFile) {
        for (const auto& score : highScores) {
            string difficultyName;
            switch (score.difficulty) {
            case DifficultyLevel::EASY:
                difficultyName = "Easy";
                break;
            case DifficultyLevel::MEDIUM:
                difficultyName = "Medium";
                break;
            case DifficultyLevel::HARD:
                difficultyName = "Hard";
                break;
            }
            outFile << to_string(score.time) << " " << difficultyName << "\n";
        }
    }
}


bool Game::compareHighScores(const HighScore& a, const HighScore& b) {
    int weightA = a.difficulty == DifficultyLevel::HARD ? 1 : a.difficulty == DifficultyLevel::MEDIUM ? 6 : 24;
    int weightB = b.difficulty == DifficultyLevel::HARD ? 1 : b.difficulty == DifficultyLevel::MEDIUM ? 6 : 24;

    return a.time * weightA < b.time * weightB;
}

void Game::displayHighScores() {
    SDL_Color color = { 255, 255, 0, 255 };
    int yPosition = 100;
    renderCenteredText("High Scores:", yPosition, color);
    yPosition += 30;

    for (const auto& score : highScores) {
        string difficultyName = (score.difficulty == DifficultyLevel::EASY ? "Easy" :
            score.difficulty == DifficultyLevel::MEDIUM ? "Medium" :
            score.difficulty == DifficultyLevel::HARD ? "Hard" : "Unknown");
        string text = difficultyName + " - " + to_string(score.time) + "s";
        renderCenteredText(text, yPosition, color);
        yPosition += 30;
    }
}