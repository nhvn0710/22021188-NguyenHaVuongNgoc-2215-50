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
        newScore.weight = (100-sliderValue)*60+1;

        isNewHighScore = (find_if(highScores.begin(), highScores.end(),
            [this](const HighScore& score) {
                return (score.time == finalTime && score.difficulty == currentDifficulty && score.weight==sliderValue);
            }) == highScores.end());

        highScores.push_back(newScore);
        sort(highScores.begin(), highScores.end(), compareHighScores);

        if (highScores.size() > HIGHSCORE_COUNT) {
            highScores.resize(HIGHSCORE_COUNT);
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
    int weightA = a.difficulty == DifficultyLevel::VERYHARD ? 1 : a.difficulty == DifficultyLevel::HARD ? 4 : a.difficulty == DifficultyLevel::MEDIUM ? 50 : 600;
    int weightB = b.difficulty == DifficultyLevel::VERYHARD ? 1 : b.difficulty == DifficultyLevel::HARD ? 4 : b.difficulty == DifficultyLevel::MEDIUM ? 50 : 600;
    if (a.difficulty == DifficultyLevel::CUSTOM) {
        weightA = a.weight;
    }
    if (b.difficulty == DifficultyLevel::CUSTOM) {
        weightB = b.weight;
    }
    return ((a.difficulty == DifficultyLevel::EASY) ? a.time + 10 : a.time) * weightA < ((b.difficulty == DifficultyLevel::EASY) ? b.time + 10 : b.time) * weightB;
}

void Game::displayHighScores() {
    SDL_Color color = { 255, 255, 0, 255 };
    int yPosition = 100;
    yPosition += 30;

    for (const auto& score : highScores) {
        string difficultyName = (score.difficulty == DifficultyLevel::EASY ? "Easy" :
            score.difficulty == DifficultyLevel::MEDIUM ? "Medium" :
            score.difficulty == DifficultyLevel::HARD ? "Hard" :
            score.difficulty == DifficultyLevel::VERYHARD ? "Very Hard" : "Unknown");
        string text = difficultyName + " - " + to_string(score.time) + "s";
        renderCenteredText(text, yPosition, color);
        yPosition += 30;
    }
}