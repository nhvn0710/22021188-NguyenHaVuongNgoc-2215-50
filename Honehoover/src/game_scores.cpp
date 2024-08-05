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
        else if (difficulty == "VeryHard") difficultyLevel = DifficultyLevel::VERYHARD;

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

        // Trim list to top 15
        if (highScores.size() > 15) {
            highScores.resize(15);
        }

        saveHighScores();
    }
}

void Game::saveHighScores() {
    ofstream outFile("highscores.txt");
    if (outFile) {
        for (const auto& score : highScores) {
            outFile << to_string(score.time) << " " << difficultyToString(score.difficulty) << "\n";
        }
    }
}


bool Game::compareHighScores(const HighScore& a, const HighScore& b) {
    int weightA = a.difficulty == DifficultyLevel::VERYHARD ? 1 : a.difficulty == DifficultyLevel::HARD ? 3 : a.difficulty == DifficultyLevel::MEDIUM ? 10 : 24;
    int weightB = b.difficulty == DifficultyLevel::VERYHARD ? 1 : b.difficulty == DifficultyLevel::HARD ? 3 : b.difficulty == DifficultyLevel::MEDIUM ? 10 : 24;
    if (a.difficulty == DifficultyLevel::CUSTOM) {
        weightA = 100;
    }
    if (b.difficulty == DifficultyLevel::CUSTOM) {
        weightB = 100;
    }
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
            score.difficulty == DifficultyLevel::HARD ? "Hard" :
            score.difficulty == DifficultyLevel::VERYHARD ? "Hard" : "Unknown");
        string text = difficultyName + " - " + to_string(score.time) + "s";
        renderCenteredText(text, yPosition, color);
        yPosition += 30;
    }
}